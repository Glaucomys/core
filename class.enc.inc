|***
 *	class.enc.inc
 *	Core Enchanter Include
 *
 * 
 ***|



|***
 *	DES: Main Enchanter Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_ENC()
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}(${cinfo}${version${Me.Class.ShortName}}\ax${cbug})\ax
	/doevents
	ISDEAD
	CHECKTIE
	
	/if (${coreAuto} && !${Me.Invis}) {
		/call check_engaged
		/if (!${Bool[${Macro.Return}]} && ${switchPcPet}) /call check_pet
	}

	:killtarget
	/call prep_combat
	/if (!${Bool[${Macro.Return}]}) /goto :exit
	
	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
		/call set_combat NUKE
		/call send_pet
		/if (${switchdeBuffTash} && !${timer_tash_${Target2Kill}}) /call cast_tash ${Target2Kill}
		/if (${switchdeBuffSlow} && !${timer_slow_${Target2Kill}}) /call cast_slow ${Target2Kill}
		/if (${switchdeBuffCripple} && ${timer_cripple_${Target2Kill}} && (${Spell[${spellCripple}].ID} != ${Spell[${spellSlow}].ID})) /call cast_cripple ${Target2Kill}
		/call click_DPS_${Me.Class.ShortName}
		/if (${doclickItem}) /call click_item
		/call disc_${Me.Class.ShortName}
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke	
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
		/doevents
		ISDEAD
		CHECKTIE
		/call get_target_ENC
		/if (${isValidTarget} && ${Target.ID} && ${Target.LineOfSight} && ${Target.Distance} <= ${MobAgro} && (${Target.PctHPs} <= ${smartengageAt} || ${Spawn[PC ID ${Me.TargetOfTarget.ID} Radius 60].ID})) /goto :loop
		/if (${Defined[timer_mez_${Target2Kill}]}) /deletevar timer_mez_${Target2Kill}
		/call list_delete mobList ${Target2Kill}
		/goto :exit
		:loop
		/doevents
		ISDEAD
		CHECKTIE
		/if (${Spawn[${Target2Kill}].Type.Equal[Corpse]}  || !${Target2Kill} || !${Target.ID} || ${Target.Type.Equal[corpse]} || !${Spawn[${Target2Kill}].ID}) /call get_target_ENC
		/call check_for_adds
		/if (${addList.Count[|]} && ${switchdeBuffMezz}) /call mez_adds
		/call check_for_adds
		/if (${addList.Count[|]}) /goto :exit
		/if (${mobList.Find[|]}) /call list_update
		/call check_for_adds
		/if (${addList.Count[|]}) /goto :exit
		/if (!${addList.Count[|]}) /call debuff_mobs
		/if (${mobList.Find[|]}) /call list_update
		/call check_for_adds
		/if (${addList.Count[|]}) /goto :exit
		/call is_target_valid ${Target2Kill}
		/if (${isValidTarget}) {
			/if (!${addList.Count[|]}) /call send_pet
			/if (!${addList.Count[|]} && ${DoDoT}) /call cast_DOT
			/call check_for_adds
			/if (!${addList.Count[|]} && ${DoNuke}) /call cast_nuke
			/call check_for_adds
			/if (!${addList.Count[|]}) /call disc_ENC
		}
		/call click_BP
		/if (${doclickItem}) /call click_item
		/if (${switchBuffSelf} && !${timer_check_runes}) /call check_rune_${Me.Class.ShortName}
	}
	/call use_mod_rod
	/if (${switchBuffItem}) /call check_crystals
	/if (${coreAuto}) {	
		/if (${switchHealCure}) /call cast_cure
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
	}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} || !${coreAuto} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Bool[${Macro.Return}]}) /goto :killtarget
	}
	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub debuff_mobs
	/doevents
	ISDEAD
	CHECKTIE
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/declare _count 				int local
	/declare _tmpID 				string local
	
	
	/if (${mobList.Find[|]} > 1) /varset mobList ${mobList.Right[-${Math.Calc[${mobList.Find[|]}-1]}]}
	
	/for _count 1 to ${mobList.Count[|]}
		/varset _tmpID ${mobList.Arg[${_count},|.Left[-1]}
	
		UPDATEHUD DEBUFF
		/squelch /target ID ${_tmpID}
		
		/if (${switchdeBuffTash} && !${timer_tash_${_tmpID}}) {
			/call cast_tash ${_tmpID}
			/call check_for_adds
			/if (${addList.Count[|]}) /goto :exit
		}
		/if (${switchdeBuffSlow} && !${timer_slow_${_tmpID}}) {
			DEBUGCLASS SLOWING ID ${_tmpID}
			/call cast_slow ${_tmpID}
			/call check_for_adds
			/if (${addList.Count[|]}) /goto :exit
		}
		/if (${switchdeBuffCripple} && ${timer_cripple_${_tmpID}}) {
			/if (${Spell[${spellCripple}].ID} != ${Spell[${spellSlow}].ID}) /call cast_cripple ${_tmpID}
			/call check_for_adds
			/if (${addList.Count[|]}) /goto :exit
		}
		/if (${switchdeBuffEradicate} && !${timer_eradicate_${Target2Kill}}) {
			/call cast_eradicate_magic ${_tmpID}
			/call check_for_adds
			/if (${addList.Count[|]}) /goto :exit
		}
	/next _count

	:exit
	UPDATEHUD
/return




|***
 *	DES: Keeps chanter runed. cause the're fragile and shit.
 *	USE: /call check_rune or /rb
 *	INI: 
 *  NOTE: 
 ***|
sub check_rune_ENC()
	DEBUGCLASS ${cbug}check_rune_ENC()\aw
	ISDEAD
	CHECKEXIT
	| /if (${timer_check_runes}) /goto :exit
	UPDATEHUD "BUFF: Rune"
	/if (${Me.AltAbilityReady[${runeVeil}]} && !${Bool[${Me.Buff[Veil of Mindshadow]}]}) /call core_cast "${runeVeil}" alt 0
	/if (${Me.AltAbilityReady[${runeEldritch}]} && !${Bool[${Me.Buff[Eldritch Rune]}]}) /call core_cast "${runeEldritch}" alt 0
	/if (${Me.AltAbilityReady[${runeSpray}]}) /call core_cast "${runeSpray}" alt 0
	/if (${Me.AltAbilityReady[${runeReactive}]}) /call core_cast "${runeReactive}" alt 0
	/if (${Me.SpellReady[${runeAuspice}]}) {
		/if (${Bool[${runeAuspice}]} && !${Bool[${Me.Buff[${runeAuspice}]}]} && ${Me.CurrentMana} >= ${Spell[${runeAuspice}].Mana}) /call core_cast "${runeAuspice}" 0 ${Me.ID}
		/if (${Me.SpellReady[${runeAuspice}]}) /call check_rune_auspice
	}
	/if (${Me.SpellReady[${runeWard}]} && !${Me.Buff[${runeWard}].ID}) /call core_cast "${runeWard}" 0
	/if (${Me.AltAbilityReady[${runeMind}]} && ${Me.PctMana} > 60) /call core_cast "${runeMind}" alt 0
	/if (!${Me.Buff[${runeEntAuspice}].ID} && ${FindItemCount[Peridot]} > 1) /call core_cast "${runeEntAuspice}" ${buffGem} ${Me.ID}

	/if (${Bool[${runePoly}]} && !${Bool[${Me.Buff[${runePoly}]}]} && ${Me.CurrentMana} >= ${Spell[${runePoly}].Mana}) /call core_cast "${runePoly}" ${buffGem} ${Me.ID}
	/if (${Bool[${runeBrimstone}]} && !${Bool[${Me.Buff[${runeBrimstone}]}]} && ${Me.CurrentMana} >= ${Spell[${runeBrimstone}].Mana}) /call core_cast "${runeBrimstone}" ${buffGem} ${Me.ID}

	/if (${Bool[${runeShieldofFate}]} && !${Bool[${Me.Buff[${runeShieldofFate}]}]} && ${Me.CurrentMana} >= ${Spell[${runeShieldofFate}].Mana}) /call core_cast "${runeShieldofFate}" ${buffGem} ${Me.ID}

	UPDATEHUD
	/call create_timer timer_check_runes ${tCheck_Runes}
	:exit
/return



|***
 *	DES: EQBC Buff Rune Ward spell
 *	USE: have the spell memed and the toon identified in the INI.
 *	INI: 
 *  NOTE: 
 ***|
sub check_rune_auspice()
	DEBUGCLASS ${cbug}check_rune_auspice()\ax
	ISDEAD
	/doevents
	CHECKTIE
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/call check1offBuffs
	
	/declare _runeAuspiceID int local ${Spell[${runeAuspice}].ID}
	/declare _AuspiceToon string local
	/declare _count int local
	/declare _toon string local ${runeAuspiceToon}

	/for _count 1 to ${_toon.Count[|]}
|		/if (!${Spawn[${_AuspiceToon}]}) /next _count
		ISDEAD
		/doevents
		/if (${Bool[${Spawn[id ${NetBots[${_toon.Arg[${_count},|]}].ID}]}]} && ${Spawn[id ${NetBots[${_AuspiceToon}].ID}].Distance} < ${Spell[${runeWard}].MyRange} && ${Me.SpellReady[${runeWard}]}) {
			/varset _AuspiceToon ${_toon.Arg[${_count},|]}
			DEBUGCLASS  ${_count}: ${_toon.Count[|]}, ${_toon.Arg[${_count},|]}, ${Bool[${Spawn[id ${NetBots[${_toon.Arg[${_count},|]}].ID}]}]}, found: ${Bool[${NetBots[${_AuspiceToon}].ShortBuff.Find[${_runeAuspiceID}]}]}
			/goto :buff
		}
	/next _count	
	
	:buff
	/if (!${Bool[${Spawn[${_AuspiceToon}]}]} || !${Me.SpellReady[${runeAuspice}]} || ${Me.CurrentMana} > ${Spell[${runeAuspice}].Mana} || ${Bool[${NetBots[${_AuspiceToon}].ShortBuff.Find[${_runeAuspiceID}]}]} || ${Spawn[${_AuspiceToon}].Type.Equal[corpse]}) /goto :exit
	UPDATEHUD "BUFF: BC Ward"
  DEBUGCLASS BC Buffing: ${csp}${runeAuspice}\ax on [${cpc}${_AuspiceToon}\ax].
	:loopretry
	/call core_cast "${runeAuspice}" 0 ${NetBots[${_AuspiceToon}].ID} check_for_adds
	/if (${Macro.Return.Equal[CAST_FIZZLE]}) /goto :loopretry
	:exit
	UPDATEHUD
/return



|***
 *	DES: basic DPS discs
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub disc_ENC()
	DEBUGCLASS ${cbug}disc_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	CHECKEXIT
	CHECKTIE
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	
	/if (${Me.AltAbilityReady[Crippling Aurora]} ) /call core_cast "Crippling Aurora" alt 0
	/if (${Me.AltAbilityReady[Phantasmal Opponent]} && ${switchPcSwarm} && (${Target.PctHPs} <= ${PcSwarmengageat} || ${switchOverrideEngageat})) /call core_cast "Phantasmal Opponent" alt 0
	/if (${Me.AltAbilityReady[Chromatic Haze]}) /call core_cast "Chromatic Haze" alt 0
	
	:exit
/return



|***
 *	DES: Keep the chanter stocked with self crystals
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub check_crystals()
	DEBUGCLASS ${cbug}check_crystals()\ax
	/doevents
	ISDEAD
	CHECKEXIT
	CHECKTIE

	/if (${Bool[${mannaCrystal}]} && !${FindItem[${mannaCrystal}].ID} && !${Cursor.Name.Equal[${mannaCrystal}]} && ${Spell[${mannaCrystal}].Mana} < ${Me.CurrentMana} && ${Me.AltAbilityReady[${mannaCrystal}]}) {
		/call core_cast "${mannaCrystal}" alt 0 check_for_adds
		/delay 20s !${Me.Casting.ID}
		/delay 2s ${Cursor.Name.Equal[${mannaCrystal}]}
	}
	CLEARCURSOR TRUE

	/if (${Bool[${healCrystal}]} && !${FindItem[${healCrystal}].ID} && !${Cursor.Name.Equal[${healCrystal}]} && ${Spell[${healCrystal}].Mana} < ${Me.CurrentMana} && ${Me.AltAbilityReady[${healCrystal}]}) {
		/call core_cast "${healCrystal}" alt 0 check_for_adds
		/delay 20s !${Me.Casting.ID}
		/delay 2s ${Cursor.Name.Equal[${healCrystal}]}
	}
	CLEARCURSOR TRUE

	/if (${Me.PctMana} < 70*.${tmpHealOverride} && ${Bool[${mannaCrystal}]} && ${FindItem[${mannaCrystal}].ID} && !${FindItem[${mannaCrystal}].Timer}) /call core_cast "${mannaCrystal} IV" item
	/if (${Me.PctMana} < 60*.${tmpHealOverride} && ${Me.AltAbilityReady[Fundament: Second Spire of Enchantment]}) /call core_cast "Fundament: Second Spire of Enchantment" alt
	/if (${Me.PctMana} < 50*.${tmpHealOverride} && ${Me.AltAbilityReady[Gather Mana]}) /call core_cast "Gather Mana" alt	
	
	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_ENC()
	DEBUGCLASS ${cbug}clickDPS_ENC()\ax
	/doevents
	ISDEAD
	CHECKEXIT
	CHECKTIE
	/call click_horn_of_unity
	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub petHeal_ENC()
/return




|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_ENC()
	DEBUGCLASS ${cbug}patch_heal_ENC_${Me.Class.ShortName}()\ax
	ISDEAD
	CHECKTIE
	/declare _count int local
	/declare _tmpID int local
	/if (${SpawnCount[npc radius 15]}) {
		/for _count 1 to ${SpawnCount[npc radius 15]}
		ISDEAD
		CHECKTIE
		/varset _tmpID ${NearestSpawn[${_count},npc radius 15].ID}
		/if (!${NearestSpawn[${_count},npc radius 15].ID}) /next _count
		/squelch /target id ${_tmpID}
		/delay 5 ${Target.ID}==${_tmpID}
		/delay 1
		/if (${Me.TargetOfTarget.ID} == ${Me.ID} && ${Target.ID} && ${Target.ID} != ${Me.ID} && (!${NoMezzList.Find[${Target.DisplayName}]} || ${timer_mez_${Target.ID}} > 0)) {
			/call is_attacking ${NearestSpawn[${_count},npc radius 15].ID}
			/if (${Macro.Return}) {
				| /if (${Me.SpellReady[${spellStun}]}) /call core_cast "${stunSpell}" 0 0
				/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.AltAbilityReady[Color Shock]}) /call core_cast "Color Shock" alt
				/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.AltAbilityReady[Doppelganger]}) /call core_cast "Doppelganger" alt
				/if (${Me.PctHPs} <= 50*.${tmpHealOverride} && ${Me.AltAbilityReady[Beguiler's Banishment]} && ${SpawnCount[npc radius 20]} > 2) /call core_cast "Beguiler's Banishment" alt
				/if (${Me.PctHPs} <= 40*.${tmpHealOverride} && ${Me.AltAbilityReady[Beguiler's Directed Banishment]}) /call core_cast "Beguiler's Directed Banishment" alt
				/if (${Me.PctHPs} <= 30*.${tmpHealOverride} && ${Me.AltAbilityReady[Nightmare Stasis]} && ${Target.Level} < 76) /call core_cast "Nightmare Stasis" alt
				/if (${Me.PctHPs} <= 30*.${tmpHealOverride} && ${Me.AltAbilityReady[Dimensional Instability]}) /call core_cast "Dimensional Instability" alt
				/if (${Defined[timer_mez_${Target.ID}]} && ${timer_mez_${Target.ID}} != -1) {
					/varset timer_mez_${Target.ID} 20
				} else /if (!${Defined[timer_mez_${Target.ID}]}) {
					/declare timer_mez_${Target.ID} 20
				}
			}
		}
		/next _count
	}

	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub mez_adds()
	DEBUGCLASS ${cbug}mez_adds()\ax
	ISDEAD
	/doevents
	/declare _tmpID string local

	:loopmezzadds
	/if (${addList.Count[|]} && ${Me.CurrentMana} >= ${Spell[${spellMez}].Mana}) {
		| /if (${addList.Count[|]} > 1) /varset addList ${addList.Right[-${Math.Calc[${addList.Find[|]}-1]}]}
		| /varset _tmpID ${addList.Arg[1,|].Left[-1]}
		/varset _tmpID ${addList.Arg[1,|]}
		/if (${_tmpID} == ${Target2Kill}) {
			/call list_delete addList ${_tmpID}
			/goto :loopmezzadds
		}
		/if (!${Spawn[${_tmpID}].Type.Equal[NPC]}) {
			/call clear_timer ${_tmpID}
		} else {
			/squelch /target id ${_tmpID}
			/delay 5 ${Target.ID} == ${_tmpID}
			/delay 2.25s ${Me.SpellReady[${spellMez}]}
			/if (${SpawnCount[npc los radius 30 zradius 15 targetable]} >= 3 ) {
				VOUT ${sepO} ${cinfo}${SpawnCount[npc radius 30 zradius 15]}\ax ${sepC} Targets in point blank range.

				/if (${switchdeBuffTash} && ${Me.AltAbilityReady[Bite of Tashani]}) /call core_cast "Bite of Tashani" alt 0 check_for_adds
				
				/if (${Me.Gem[${spellpbaeMez}]} && ${Me.SpellReady[${spellpbaeMez}]}) {
					/if (${Me.Casting.Name.NotEqual[${spellpbaeMez}]}) /call interrupt
					/call core_cast "${spellpbaeMez}" 0 0 check_for_adds
				}
				/if (${Me.Gem[${spellStun}]} && ${Me.SpellReady[${spellStun}]}) {
					/if (${Me.Casting.Name.NotEqual[${spellStun}]}) /call interrupt
					/call core_cast "${spellStun}" 0 0 check_for_adds
				}
			}
			/if (${Spawn[${_tmpID}].NearestSpawn[3,npc radius 30].ID} && ${Spawn[${_tmpID}].Distance3D} <= 200 && ${Spawn[${_tmpID}].Distance3D} >= ${MobAgro}*.5) {
				/if (${Me.AltAbilityReady[Beam of Slumber]}) {
					/if (${Me.Casting.ID}) /call interrupt
					/face ID ${_tmpID} ${If[${SetFaceFast},fast,]}
					/call core_cast "Beam of Slumber" alt 0				
				} else /if (${Me.AltAbilityReady[Scintillating Beam]}) {
					/if (${Me.Casting.ID}) /call interrupt
					/face ID ${_tmpID} ${If[${SetFaceFast},fast,]}
					/call core_cast "Scintillating Beam" alt 0
				}
			}

			/call cast_mez ${_tmpID}
		}
		/call list_delete addList ${_tmpID}
		| VOUT ${sepO} ${cinfo}${addList.Count[|]}\ax ${sepC} ${If[${addList.Count[|]} == 1,Target,Targets]} left to mez.
		/if (!${addList.Count[|]} && ${switchBuffSelf} && !${timer_check_runes}) /call check_rune_${Me.Class.ShortName}
		/goto :loopmezzadds
	}
	:exit
/return



|***
 *	DES: mez the target
 *	USE: debufs
 *	INI: 
 *  NOTE: 
 ***|
sub cast_mez(int _tmpID)
	/doevents
	ISDEAD
	/declare _resistCount int local 0
	:loopmez
	ISDEAD
	/if (!${Target.LineOfSight} || ${ExcludeList.Find[${Target.DisplayName}]} || ${Spawn[${_tmpID}].Type.Equal[corpse]} || !${Spawn[${_tmpID}].ID}) /goto :exit

	| check for mez immune list mobs
	/if (${NoMezzList.Find[${Spawn[ID ${_tmpID}].DisplayName}]}) {
	  /call create_timer timer_mez_${_tmpID} ${Spell[${spellMez}].Duration.TotalSeconds}m
	  NOUT ${owarning} IMMUNE TO\ax ${csp}${spellMez}\ax ${sep} [${ctar}${Target.DisplayName}\ax]
	  /call list_delete addList ${_tmpID}	
		/goto :exit
	}
	
	/if (${Target.Mezzed.Duration} && ${Spell[${Target.Mezzed}].Name.Find[${spellMez}]}) {
		/call create_timer timer_mez_${_tmpID} ${Target.Mezzed.Duration.TotalSeconds}s
		/goto :exit
	} else /if (${Target.PctHPs} < 90) {
		/call create_timer timer_mez_${_tmpID} 5m
		VOUT [${ctar}${Target.DisplayName}\ax] is being attacked!
		/goto :exit
	}
	/if (${Me.CurrentMana}>${Spell[${spellMez}].Mana}) {
		/delay 2.25s ${Me.SpellReady[${spellMez}]}
		/call core_cast "${spellMez}" 0 ${_tmpID} check_for_adds
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_mez_${_tmpID} ${Spell[${spellMez}].Duration.Seconds}s
			/if (${Select[${castReturn},CAST_IMMUNE]}) {
			  /call create_timer timer_mez_${_tmpID} ${Spell[${spellMez}].Duration.TotalSeconds}m
			  NOUT ${owarning} IMMUNE TO\ax ${csp}${spellMez}\ax ${sep} [${ctar}${Target.DisplayName}\ax]
			  ECHOCHANNEL MEZ IMMUNE to ${spellMez} ... ${Target.DisplayName}
			  /call list_delete addList ${_tmpID}
			}
			/goto :exit
		} else /if (${Select[${castReturn},CAST_RESISTED,CAST_RESIST]}) {
			/varcalc _resistCount ${_resistCount}+1
			/if (${_resistCount}==2 && !${timer_tash_${_tmpID}}) /call cast_tash ${_tmpID}
		}
	| VOUT ${sepO} ${cinfo}${addList.Count[|]}\ax ${sepC} ${If[${addList.Count[|]} == 1,Target,Targets]} left to mez.

		/goto :loopmez
	}
	:exit
/return



|***
 *	DES: tashes the target
 *	USE: debufs
 *	INI: 
 *  NOTE: 
 ***|
sub cast_tash(int _tmpID)
	/doevents
	ISDEAD
	CHECKTIE
	/if (!${Target.LineOfSight} || ${ExcludeList.Find[${Target.DisplayName}]} || ${Spawn[${_tmpID}].Type.Equal[corpse]} || !${Spawn[${_tmpID}].ID}) /goto :exit

	/if (${Target.Tashed.Duration} && !${Target.Buff[Bite of Tashani].ID}) {
		DEBUGCLASS ${Target.DisplayName} already has ${Target.Tashed}
		/goto :exit
	}
	
	/if (${Target.Distance} <= ${Spell[${spellTash}].MyRange} && ${Target.LineOfSight}) {
		/delay 2.25s ${Me.SpellReady[${spellTash}]}
		/call core_cast "${spellTash}" 0 ${_tmpID} check_for_adds
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_RESISTED,CAST_RESIST,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_tash_${_tmpID} ${Spell[${spellTash}].Duration.TotalSeconds}s
			/goto :exit
		}
	}
	:exit
/return



|***
 *	DES: slows the target
 *	USE: debufs
 *	INI: 
 *  NOTE: 
 ***|
sub cast_slow(int _tmpID)
	DEBUGCLASS ${cbug}cast_slow(int ${_tmpID})\ax
	ISDEAD
	/doevents
	/declare _tmpSlow string local ${Target.Slowed}

	/if (${Target.Crippled.Duration}) /call create_timer timer_cripple_${_tmpID} ${Target.Crippled.Duration.TotalSeconds}s

	/if (${ExcludeList.Find[${Target.DisplayName}]} || ${Target.Type.Equal[corpse]} || ${Spawn[${_tmpID}].Type.Equal[corpse]} || !${Spawn[${_tmpID}].ID}) /goto :exit
	DEBUGCLASS passed bail checks
	
	
	/if (${Target.Slowed.Duration}) {
		DEBUGCLASS already slowed ffs.. ${Target.Slowed}
		/if (${_tmpSlow.Find[helix]}) {
			/call create_timer timer_slow_${_tmpID} ${Target.Slowed.Duration.TotalSeconds}s
			/goto :exit
		}
		
		| /if (${_tmpSlow.Find[tendon]}) /call create_timer timer_slow_${_tmpID} ${Target.Slowed.Duration.TotalSeconds}s
		
	}
		

	/declare _resistCount int local 0
	:loopslow
	DEBUGCLASS :loopslow
	ISDEAD
	/if (${Spawn[${_tmpID}].Type.NotEqual[PC]} && ${Spawn[${_tmpID}].Distance} <= ${Spell[${spellSlow}].MyRange}) {
		/delay 2.25s ${Me.SpellReady[${spellSlow}]}
		/call core_cast "${spellSlow}" 0 ${_tmpID} check_for_adds

		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_slow_${_tmpID} ${Spell[${spellSlow}].Duration.TotalSeconds}s
		} else /if (${Select[${castReturn},CAST_RESISTED,CAST_RESIST]}) {
			/varcalc _resistCount ${_resistCount}+1
			/if (${_resistCount} >= 3) {
				/call create_timer timer_slow_${_tmpID} 10000s
			} else {
				/goto :loopslow
			}
		}
	}
	:exit
/return



|***
 *	DES: cripples the target
 *	USE: debufs
 *	INI: 
 *  NOTE: 
 ***|
sub cast_cripple(int _tmpID)
	/doevents
	ISDEAD
	/declare _resistCount int local
	/call check_exit
	/if (${Bool[${Macro.Return}]} || !${Spawn[${_tmpID}].ID}) /goto :exit

	/if (${Target.Crippled.Duration}) {
		/call create_timer timer_cripple_${_tmpID} ${Target.Crippled.Duration.TotalSeconds}s
		/goto :exit
	}

	/declare _tmpSlow string local ${Target.Slowed}
	/if (${Target.Slowed.Duration}) {
		/if (${_tmpSlow.Find[Undermining]}) {
			/call create_timer timer_slow_${_tmpID} ${Target.Slowed.Duration.TotalSeconds}s
			/goto :exit
		}
	}	
	
	:loopcripple
	/if (${Target.Distance} <= ${Spell[${spellCripple}].MyRange} && ${Me.CurrentMana} > ${Spell[${spellCripple}].Mana} && ${Target.LineOfSight}) {
		/delay 2.25s ${Me.SpellReady[${spellCripple}]}
		/call core_cast "${spellCripple}" 0 ${_tmpID} check_for_adds
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_cripple_${_tmpID} ${Spell[${spellCripple}].Duration.TotalSeconds}s
		} else /if (${Select[${castReturn},CAST_IMMUNE]}) {
			/call create_timer timer_cripple_${_tmpID} ${Spell[${spellCripple}].Duration.TotalSeconds}m
		} else /if (${Select[${castReturn},CAST_RESISTED,CAST_RESIST]}) {
			/varcalc _resistCount ${_resistCount}+1
			/if (${_resistCount} >= 3) {
				VOUT ${ctar}${Target.DisplayName}\ax resisted ${csp}${spellCripple}\ax ${cinfo}${_resistCount}\ax times, moving on!
				/call create_timer timer_cripple_${_tmpID} 10000s
			} else {
				/delay 5
				/goto :loopcripple
			}
		}
	}
	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 

sub mob_charm(int _tmpID)
	/declare _oldtmpID int local ${Target.ID}
	/squelch /target id ${_tmpID}
	/if (${Me.SpellReady[${spellpbaeMez}]}) {
		/if (${Target.Distance3D} <= 30) /call core_cast "${spellpbaeMez}" 0
		VOUT Pet broke PB mezzing him!
	}
	/squelch /target id ${_tmpID}
	/squelch /if ( ${tashTimer${_tmpID}}  <5500 ) /call Tash

	/call core_cast "${spellCharm}" 0 ${_tmpID}
	/if (${Me.Pet.ID}) {
		/if (${Spawn[${_oldtmpID}].ID}) {
			/squelch /target id ${_oldtmpID}
		} else {
			/squelch /target clear
		}
	}
/return
 ***|



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
sub mob_charm2()
	/declare _count int local
	/declare _mobs int local
	/declare _tmpID int local
	/delay 1
	/if (${SpawnCount[npc los Range 70 110 radius 200 targetable]}) {
	/varset _mobs ${SpawnCount[npc los Range 70 110 radius 200 targetable]} 
	/for _count 1 to ${_mobs}
		/varset _tmpID ${NearestSpawn[${_count}, npc los Range 70 110 radius 200 targetable].ID}
		/if (${CharmList.Find["${Spawn[id ${_tmpID}].DisplayName}"]} && !${Spawn[${_tmpID}].Master.Type.Equal[Pet]} && !${Spawn[${_tmpID}].Master.Type.Equal[Corpse]}) {
			VOUT Oh yeah found a mob to charm up in this bitch! Grabbing ${Spawn[id ${_tmpID}].DisplayName} --Nyte
			/call Charm ${_tmpID}
			/if (${Defined[mezTimer${_tmpID}]}) /deletevar mezTimer${_tmpID} 
			/if (${Me.Pet.ID}) /call create_timer mezTimer${_tmpID} -1
			/return
		}
	/next _count
	}
	:exit
/return
 ***|



|***
 *	DES: this will check each mob in area of the chanter. take note of its current animation and decide if it is attacking the enchanter.
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub is_attacking(int _tmpID)
	/declare _ani int local
	/declare _isAttacking FALSE
	/varset _ani ${Spawn[${_tmpID}].Animation}
	DEBUGCLASS Animation #${_ani}, by mob ID: ${_tmpID} (${Spawn[${_tmpID}].Name}).
	/if (${_ani} == 5) /varset _isAttacking TRUE
	/if (${_ani} == 7) /varset _isAttacking TRUE
	/if (${_ani} == 8) /varset _isAttacking TRUE
	/if (${_ani} == 12) /varset _isAttacking TRUE
	/if (${_ani} == 13) /varset _isAttacking TRUE
	/if (${_ani} == 16) /varset _isAttacking FALSE
	/if (${_ani} == 17) /varset _isAttacking FALSE
	| /if (${animation} == 18) /return 2
	/if (${_ani} == 26) /varset _isAttacking FALSE
	/if (${_ani} == 32) /varset _isAttacking FALSE
	/if (${_ani} == 33) /varset _isAttacking FALSE
	/if (${_ani} == 34) /varset _isAttacking FALSE
	/if (${_ani} == 37) /varset _isAttacking FALSE
	/if (${_ani} == 38) /varset _isAttacking FALSE
	/if (${_ani} == 42) /varset _isAttacking TRUE
	/if (${_ani} == 43) /varset _isAttacking FALSE
	/if (${_ani} == 44) /varset _isAttacking TRUE
	/if (${_ani} == 71) /varset _isAttacking FALSE
	/if (${_ani} == 72) /varset _isAttacking FALSE
	/if (${_ani} == 73) /varset _isAttacking FALSE
	/if (${_ani} == 80) /varset _isAttacking TRUE
	/if (${_ani} == 106) /varset _isAttacking TRUE
	/if (${_ani} == 110) /varset _isAttacking TRUE
	/if (${_ani} == 111) /varset _isAttacking FALSE
	/if (${_ani} == 129) /varset _isAttacking TRUE
	/if (${_ani} == 144) /varset _isAttacking TRUE
	/if (${_ani} == 146) /varset _isAttacking FALSE
	DEBUGCLASS ${cbad}Unknown Animation\ax #${_ani}, by mob ID: ${_tmpID} (${Spawn[${_tmpID}].Name}).
	:exit
/return ${_isAttacking}	



 |***
 *	DES: we need a target. specific for the enchanter
 *	USE: uses /ma1-2 or assists
 *	INI: 
 *  NOTE: 
 *	----Need to work this out of the system. use get_assist_target instead..
 ***|
sub get_target_ENC()
	DEBUGCLASS ${cbug}get_target_ENC()\ax
	/if (${Me.Moving}) /goto :exit
	/call get_assist_target
	/if (${ExcludeList.Find[${Target.DisplayName}]}) /goto :notgood
	/if (${Spawn[${Target2Kill}].Type.Equal[npc]} && ${Spawn[${Target2Kill}].LineOfSight}) {
		/varset isValidTarget TRUE
		/if (${Defined[timer_mez_${Target2Kill}]}) /deletevar timer_mez_${Target2Kill}
		/call create_timer timer_mez_${Target2Kill} timer outer 600s
		/call list_add mobList ${Target2Kill}
	} else {
		:notgood
		/varset isValidTarget FALSE
	}
	:exit
/return