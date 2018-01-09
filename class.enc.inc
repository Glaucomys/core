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
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}()\ax
	AMIDEAD
	/doevents
	CHECKTIE
	
	/if (${coreAuto} && !${Me.Invis}) {
		/call check_engaged
		/if (!${Bool[${Macro.Return}]} && ${switchPcPet} && !${switchCharmPet}) /call check_pet
	}

	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /return
	
	CHECKREZ
	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/call send_pet
		/call stick_to_target
		/call check_deBuffs ${Target2Kill}
		/call click_DPS_${Me.Class.ShortName}
		/call click_item
		/call disc_${Me.Class.ShortName}
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke	
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
		/call set_combat RANGE
		/call send_pet
		/call stick_to_target
		/call check_deBuffs
		/call click_DPS_${Me.Class.ShortName}
		/call click_item
		/call disc_${Me.Class.ShortName}
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke			
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
		/call set_combat NUKE
		/call send_pet
		/call check_deBuffs ${Target2Kill}
		/call click_DPS_${Me.Class.ShortName}
		/call click_item
		/call disc_${Me.Class.ShortName}
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke	
	} else /if (${Select[${combat},HEAL]}) {
		/call set_combat HEAL
		/call send_pet
	} else /if (${Select[${combat},DUAL]}) {
		/call get_target_ENC
		/if (${isValidTarget} && ${Target.ID} && ${Target.LineOfSight} && ${Target.Distance} <= ${MobAgro} && (${Target.PctHPs} <= ${smartengageAt} || ${Spawn[PC ID ${Me.TargetOfTarget.ID} Radius 60].ID})) /goto :loop
		| /if (${Defined[timer_mez_${Target2Kill}]}) /deletevar timer_mez_${Target2Kill}
		/if (${Bool[${mobList}]}) /call list_delete mobList ${Target2Kill}
		/return
		:loop
		AMIDEAD
		CHECKTIE
		/if (${Spawn[${Target2Kill}].Type.Equal[Corpse]}  || !${Target2Kill} || !${Target.ID} || ${Target.Type.Equal[corpse]} || !${Spawn[${Target2Kill}].ID}) /call get_target_ENC
		/call check_for_adds
		/if (${addList.Count[|]} && ${switchdeBuffMezz}) /call mez_adds
		/call check_for_adds
		/if (${addList.Count[|]}) /return
		/if (${mobList.Find[|]}) /call list_update mobList ${Target2Kill}
		/call check_for_adds
		/if (${addList.Count[|]}) /return
		/if (!${addList.Count[|]}) /call check_deBuffs ${Target2Kill}
		/if (${mobList.Find[|]}) /call list_update mobList ${Target2Kill}
		/call check_for_adds
		/if (${addList.Count[|]}) /return
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
		/call click_item
		/if (${switchBuffSelf} && !${timer_check_runes}) /call check_rune_${Me.Class.ShortName}
	}
	/if (${coreAuto}) {	
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/call use_mod_rod
		/if (${switchCharmPet} && ${switchPcPet} && !${Me.Pet.ID}) /call find_mob_to_charm
		/if (${switchBuffItem}) /call check_crystals
		/if (${switchHealCure}) /call cast_cure
	}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} && !${is_target_dead[${Target2Kill}]}) /goto :killtarget
	/if (!${coreAuto} && ${Target.ID} && ${Bool[${combat}]} && !${is_target_dead[${Target.ID}]}) /goto :killtarget
/return



|***
 *	DES: Keeps chanter runed. cause the're fragile and shit.
 *	USE: /call check_rune or /rb
 *	INI: 
 *  NOTE: 
 ***|
sub check_rune_ENC()
	DEBUGCLASS ${cbug}check_rune_ENC()\aw
	/call create_timer timer_check_runes ${tCheck_Runes}
	AMIDEAD
	CHECKEXIT
	UPDATEHUD "BUFF: Rune"
	/if (${validate_buff[alt, rune, runeVeil]}) /call core_cast "${runeVeil}" 0 0 FALSE TRUE
	/if (${validate_buff[alt, rune, runeEldritch]}) /call core_cast "${runeEldritch}" 0 0 FALSE TRUE
	/if (${validate_buff[alt, rune, runeSpray]}) /call core_cast "${runeSpray}" 0 0 FALSE TRUE
	/if (${validate_buff[alt, rune, runeReactive]}) /call core_cast "${runeReactive}" 0 0 FALSE TRUE
	/if (${validate_buff[alt, rune, runeMind]}) /call core_cast "${runeMind}" 0 0 FALSE TRUE
	/if (${validate_buff[spell, rune, runeAuspice]}) /call core_cast "${runeAuspice}" 0 ${Me.ID} FALSE TRUE
	/if (${validate_buff[spell, rune, runeEntAuspice]} && ${FindItemCount[Peridot]} > 1) /call core_cast "${runeEntAuspice}" ${buffGem} ${Me.ID} FALSE TRUE
	/if (${validate_buff[spell, rune, runeWard]}) /call core_cast "${runeWard}" 0 0 FALSE TRUE
	/if (${validate_buff[spell, rune, runePoly]}) /call core_cast "${runePoly}" ${buffGem} ${Me.ID} FALSE TRUE
	/if (${validate_buff[spell, rune, runeBrimstone]}) /call core_cast "${runeBrimstone}" ${buffGem} ${Me.ID} FALSE TRUE
	/if (${validate_buff[spell, rune, runeShieldofFate]}) /call core_cast "${runeShieldofFate}" ${buffGem} ${Me.ID} FALSE TRUE
	/if (${validate_buff[spell, rune, runeLegion]}) /call core_cast "${runeLegion}" ${buffGem} ${Me.ID} FALSE TRUE
	/if (${Me.SpellReady[${runeAuspice}]}) /call check_rune_auspice
	UPDATEHUD FALSE
/return



|***
 *	DES: EQBC Buff Rune Ward spell
 *	USE: have the spell memed and the toon identified in the INI.
 *	INI: 
 *  NOTE: 
 ***|
sub check_rune_auspice()
	DEBUGCLASS ${cbug}check_rune_auspice()\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	CHECKTIE
	/call check1offBuffs
	
	/declare _runeAuspiceID int local ${Spell[${runeAuspice}].ID}
	/declare _AuspiceToon string local
	/declare _count int local 0
	/declare _toon string local ${runeAuspiceToon}

	/for _count 1 to ${_toon.Count[|]}
		/if (!${Spawn[${_AuspiceToon}]}) /continue
		CHECKEXIT
		/doevents
		/if (${Bool[${Spawn[id ${NetBots[${_toon.Arg[${_count},|]}].ID}]}]} && ${Spawn[id ${NetBots[${_AuspiceToon}].ID}].Distance} < ${Spell[${runeWard}].MyRange} && ${Me.SpellReady[${runeWard}]}) {
			/varset _AuspiceToon ${_toon.Arg[${_count},|]}
			DEBUGCLASS  ${_count}: ${_toon.Count[|]}, ${_toon.Arg[${_count},|]}, ${Bool[${Spawn[id ${NetBots[${_toon.Arg[${_count},|]}].ID}]}]}, found: ${Bool[${NetBots[${_AuspiceToon}].ShortBuff.Find[${_runeAuspiceID}]}]}
			/break
		}
	/next _count	
	
	/if (!${Bool[${Spawn[${_AuspiceToon}]}]} || !${Me.SpellReady[${runeAuspice}]}  || ${Bool[${NetBots[${_AuspiceToon}].ShortBuff.Find[${_runeAuspiceID}]}]} || ${Spawn[${_AuspiceToon}].Type.Equal[corpse]}) /return
	UPDATEHUD "BUFF: BC Ward"
  DEBUGCLASS BC Buffing: ${csp}${runeAuspice}\ax on [${cpc}${_AuspiceToon}\ax].

	/call core_cast "${runeAuspice}" 0 ${NetBots[${_AuspiceToon}].ID} check_for_adds
	UPDATEHUD FALSE
/return



|***
 *	DES: basic DPS discs
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub disc_ENC()
	DEBUGCLASS ${cbug}disc_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	CHECKTIE
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	
	/if (${Me.AltAbilityReady[Crippling Aurora]} ) /call core_cast "Crippling Aurora" alt 0 FALSE
	/if (${Me.AltAbilityReady[Phantasmal Opponent]} && ${switchPcSwarm} && (${Target.PctHPs} <= ${PcSwarmengageat} || ${switchOverrideEngageat})) /call core_cast "Phantasmal Opponent" alt 0 FALSE
	/if (${Me.AltAbilityReady[Chromatic Haze]}) /call core_cast "Chromatic Haze" alt 0 FALSE
/return



|***
 *	DES: Keep the chanter stocked with self crystals
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub check_crystals()
	DEBUGCLASS ${cbug}check_crystals()\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	CHECKTIE

	/if (${Bool[${mannaCrystal}]} && !${FindItem[${mannaCrystal}].ID} && !${Cursor.Name.Equal[${mannaCrystal}]} && ${Me.AltAbilityReady[${mannaCrystal}]}) {
		/call core_cast "${mannaCrystal}" alt 0 check_for_adds
		/delay 20s !${Me.Casting.ID}
		/delay 2s ${Cursor.Name.Equal[${mannaCrystal}]}
	}
	/call sort_cursor TRUE

	/if (${Bool[${healCrystal}]} && !${FindItem[${healCrystal}].ID} && !${Cursor.Name.Equal[${healCrystal}]} && ${Me.AltAbilityReady[${healCrystal}]}) {
		/call core_cast "${healCrystal}" alt 0 check_for_adds
		/delay 20s !${Me.Casting.ID}
		/delay 2s ${Cursor.Name.Equal[${healCrystal}]}
	}
	/call sort_cursor TRUE

	/if (${Me.PctMana} < 70*.${tmpHealOverride} && ${Bool[${mannaCrystal}]} && ${FindItem[${mannaCrystal}].ID} && !${FindItem[${mannaCrystal}].Timer}) /call core_cast "${mannaCrystal} IV" item 0 FALSE
	/if (${Me.PctMana} < 60*.${tmpHealOverride} && ${Me.AltAbilityReady[Fundament: Second Spire of Enchantment]}) /call core_cast "Fundament: Second Spire of Enchantment" alt 0 FALSE
	/if (${switchGather} && ${Me.PctMana} < ${GatherPercent}*.${tmpHealOverride} && ${Me.AltAbilityReady[Gather Mana]}) /call core_cast "Gather Mana" alt	0 FALSE
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_ENC()
	DEBUGCLASS ${cbug}clickDPS_ENC()\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	CHECKTIE
	/call click_horn_of_unity 	
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_ENC()
	DEBUGCLASS ${cbug}patch_heal_ENC_${Me.Class.ShortName}()\ax
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	/declare _count int local
	/declare _tmpID int local
	/if (${SpawnCount[npc radius 15]}) {
		/for _count 1 to ${SpawnCount[npc radius 15]}
			AMIDEAD
			CHECKEXIT
			/doevents
			CHECKTIE
		/varset _tmpID ${NearestSpawn[${_count},npc radius 15].ID}
		/if (!${NearestSpawn[${_count},npc radius 15].ID}) /next _count
		/squelch /target id ${_tmpID}
		/delay 5 ${Target.ID}==${_tmpID}
		/delay 1
		/if (${Me.TargetOfTarget.ID} == ${Me.ID} && ${Target.ID} && ${Target.ID} != ${Me.ID} && (!${NoMezzList.Find[${Target.DisplayName}]} || ${timer_mez_${Target.ID}} > 0)) {
			/call is_attacking ${NearestSpawn[${_count},npc radius 15].ID}
			/if (${Bool[${Macro.Return}]}) {
				| /if (${Me.SpellReady[${spellStun}]}) /call core_cast "${stunSpell}" 0 0
				| /if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.AltAbilityReady[Color Shock]}) /call core_cast "Color Shock" alt
				/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.AltAbilityReady[Doppelganger]}) /call core_cast "Doppelganger" alt ${Me.ID} FALSE
				| /if (${Me.PctHPs} <= 50*.${tmpHealOverride} && ${Me.AltAbilityReady[Beguiler's Banishment]} && ${SpawnCount[npc radius 20]} > 2) /call core_cast "Beguiler's Banishment" alt
				| /if (${Me.PctHPs} <= 40*.${tmpHealOverride} && ${Me.AltAbilityReady[Beguiler's Directed Banishment]}) /call core_cast "Beguiler's Directed Banishment" alt ${Target2Kill} FALSE
				/if (${Me.PctHPs} <= 30*.${tmpHealOverride} && ${Me.AltAbilityReady[Nightmare Stasis]} && ${Target.Level} < 76) /call core_cast "Nightmare Stasis" alt alt ${Target2Kill} FALSE
				/if (${Me.PctHPs} <= 30*.${tmpHealOverride} && ${Me.AltAbilityReady[Dimensional Shield]}) /call core_cast "Dimensional Shield" alt ${Me.ID} FALSE
				/if (${Defined[timer_mez_${Target.ID}]} && ${timer_mez_${Target.ID}} != -1) {
					/varset timer_mez_${Target.ID} 20
				} else /if (!${Defined[timer_mez_${Target.ID}]}) {
					/declare timer_mez_${Target.ID} 20
				}
			}
		}
		/next _count
	}
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub mez_adds()
	DEBUGCLASS ${cbug}mez_adds()\ax
	AMIDEAD
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
			/if (${SpawnCount[npc los radius 30 zradius 15 targetable]} > 1 ) {
				VOUT ${sepO} ${cinfo}${SpawnCount[npc radius 30 zradius 15]}\ax ${sepC} ${If[${SpawnCount[npc los radius 30 zradius 15 targetable]} == 1,Target,Targets]} in point blank range.

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
				} 
			}
			/if (!${timer_mez_${_tmpID}}) /call cast_mez ${_tmpID}
		}
		/call list_delete addList ${_tmpID}
		| VOUT ${sepO} ${cinfo}${addList.Count[|]}\ax ${sepC} ${If[${addList.Count[|]} == 1,Target,Targets]} left to mez.
		/if (!${addList.Count[|]} && ${switchBuffSelf} && !${timer_check_runes}) /call check_rune_${Me.Class.ShortName}
		/goto :loopmezzadds
	}
/return



|***
 *	DES: mez the target
 *	USE: debufs
 *	INI: 
 *  NOTE: 
 ***|
sub cast_mez(int _tmpID)
	DEBUG ${cbug}cast_mez(ID: ${_tmpID})\ax
	DEBUG ${Target.ID} - ${timer_mez_${_tmpID}}
	/if (${timer_mez_${_tmpID}}) /return
	/declare _resistCount int local 0

	:loopmez
	AMIDEAD
	/doevents
	/if (!${Target.LineOfSight} || ${ExcludeList.Find[${Spawn[${_tmpID}].DisplayName}]} || ${Spawn[${_tmpID}].Type.Equal[corpse]} || !${Spawn[${_tmpID}].ID}) /return

	| check for mez immune list mobs
	/if (${NoMezzList.Find[${Spawn[ID ${_tmpID}].DisplayName}]}) {
	  /call create_timer timer_mez_${_tmpID} ${Spell[${spellMez}].Duration.TotalSeconds}m
	  VOUT ${owarning} IMMUNE TO\ax ${csp}${spellMez}\ax ${sep} [${ctar}${Target.DisplayName}\ax]
	  /call list_delete addList ${_tmpID}	
		/return
	}
	
	/if (${Target.Mezzed.Duration} && ${Spell[${Target.Mezzed}].Name.Find[${spellMez}]}) {
		DEBUG target mezed: ${Spell[${Target.Mezzed}].Name.Find[${spellMez}]}
		/call create_timer timer_mez_${_tmpID} ${Target.Mezzed.Duration.TotalSeconds}s
		/return
	} else /if (${Target.PctHPs} < 90) {
		/call create_timer timer_mez_${_tmpID} 5m
		VOUT [${ctar}${Target.DisplayName}\ax] is being attacked!
		/return
	}
	/if (${Me.CurrentMana} > ${Spell[${spellMez}].Mana}) {
		/delay 2.25s ${Me.SpellReady[${spellMez}]}
		/call core_cast "${spellMez}" 0 ${_tmpID} check_for_adds
		DEBUG ${spellMez} -> ${castReturn}
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_mez_${_tmpID} ${Spell[${spellMez}].Duration.Seconds}s
			/if (${Select[${castReturn},CAST_IMMUNE]}) {
			  /call create_timer timer_mez_${_tmpID} ${Spell[${spellMez}].Duration.TotalSeconds}m
			  VOUT ${owarning} IMMUNE TO\ax ${csp}${spellMez}\ax ${sep} [${ctar}${Target.DisplayName}\ax]
			  ECHOCHANNEL MEZ IMMUNE to ${spellMez} ... ${Target.DisplayName}
			  /call list_delete addList ${_tmpID}
			}
			/return
		} else /if (${Select[${castReturn},CAST_RESISTED,CAST_RESIST]}) {
			/varcalc _resistCount ${_resistCount}+1
			/if (${_resistCount}==2 && !${timer_tash_${_tmpID}}) /call cast_tash ${_tmpID}
		}
	| VOUT ${sepO} ${cinfo}${addList.Count[|]}\ax ${sepC} ${If[${addList.Count[|]} == 1,Target,Targets]} left to mez.

		/goto :loopmez
	}
/return



|***
 *	DES: tashes the target
 *	USE: /call cast_tash [ID] | /debuff tash
 *	INI: 
 *  NOTE: 
 ***|
sub cast_tash(int _tmpID)
	DEBUG ${cbug}cast_tash(ID: ${_tmpID})\ax
	/if (${timer_tash_${_tmpID}}) /return
	AMIDEAD
	CHECKEXIT
	/doevents
	CHECKTIE
	/if (!${Target.LineOfSight} || ${ExcludeList.Find[${Target.DisplayName}]} || ${Spawn[${_tmpID}].Type.Equal[corpse]} || !${Spawn[${_tmpID}].ID}) /return

	| if it's tashed, mark it and move on. better things to do
	/if (${Target.Tashed.Duration} && !${Target.Buff[Bite of Tashani].ID}) {
		DEBUGCLASS ${Target.DisplayName} already has ${Target.Tashed}
		/call create_timer timer_tash_${_tmpID} ${Spell[${spellTash}].Duration.TotalSeconds}s		
		/return
	}
	
	/if (${Target.Distance} <= ${Spell[${spellTash}].MyRange} && ${Target.LineOfSight} && ${Me.Gem[${spellTash}]} && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 49]} == 1) {
		/call core_cast "${spellTash}" 0 ${_tmpID} check_for_adds
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_RESISTED,CAST_RESIST,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_tash_${_tmpID} ${Spell[${spellTash}].Duration.TotalSeconds}s
		}
	} else /if (${Me.AltAbilityReady[Bite of Tashani]} && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 49]} > 1) {
		/call core_cast "Bite of Tashani" alt 0 FALSE
	}

/return



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
	DEBUGCLASS ${cbad}Unknown Animation\ax #${_ani}, by mob ID: ${_tmpID} (${Spawn[${_tmpID}].Name})
/return ${Bool[${_isAttacking}]}	



 |***
 *	DES: we need a target. specific for the enchanter
 *	USE: uses /ma1-2 or assists
 *	INI: 
 *  NOTE: 
 *	----Need to work this out of the system. use get_assist_target instead..
 ***|
sub get_target_ENC()
	DEBUGCLASS ${cbug}get_target_ENC()\ax
	/if (${Me.Moving}) /return
	/call get_assist_target
	/if (!${ExcludeList.Find[${Target.DisplayName}]}) {
		/if (${Spawn[${Target2Kill}].Type.Equal[npc]} && ${Spawn[${Target2Kill}].LineOfSight}) {
			/varset isValidTarget TRUE
			/if (${Defined[timer_mez_${Target2Kill}]}) /deletevar timer_mez_${Target2Kill}
			/call create_timer timer_mez_${Target2Kill} timer outer 600s
			/call list_add mobList ${Target2Kill}
		}
		/return
	}
	/varset isValidTarget FALSE
/return



|***
 *	DES: Enchanter burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_ENC()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	CHECKEXIT
	/doevents
	
	/if (${Me.AltAbilityReady[${ENCIoG}]}) /call core_cast "${ENCIoG}" alt 0
	/call cast_AA_spire
	/if (${Me.AltAbilityReady[Dimensional Shield]} && ${Spell[Dimensional Shield].Stacks}) /call core_cast "Dimensional Shield" alt 0
	/if (${Me.AltAbilityReady[Calculated Insanity]} && ${Spell[Calculated Insanity].Stacks}) /call core_cast "Calculated Insanity" alt 0

	| epic 2.0
	/if (${Me.ItemReady[${${Me.Class.ShortName}Epic2}]} && ${Spell[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].Stacks} && !${Me.Buff[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID} && !${Me.Song[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID}) /call core_cast "${${Me.Class.ShortName}Epic2}" item 0 FALSE

	
	/call use_glyph
	/call synergy_click_Circle
	/call click_item_burn
/return




|***
 *	DES: Charm Mobs. cause fucking charmed mobs and shits 
 *	USE: /call find_mob_to_charm()
 *	INI: fgeeds from core.zone.ini->[zone]->CharmMobList
 *	NOTE: 
 ***|   
sub find_mob_to_charm()

	/declare _count int local 0
	/declare _spawns int local 0
	/declare _tmpID int local 0
	/delcare _maxlvl int local ${Int[${Math.Calc[${Me.Level}-1]}]}
	
	/delay 1
	
	/if (${SpawnCount[npc los Range 1 ${_maxlvl} radius ${MobAgro} targetable]}) {
		/varset _spawns ${SpawnCount[npc los Range 1 ${_maxlvl} radius ${MobAgro} targetable]} 
		/for _count 1 to ${_spawns}
			/varset _tmpID ${NearestSpawn[${_count}, npc los Range 1 ${_maxlvl} radius ${MobAgro} targetable].ID}
			/if (${CharmMobList.Find[${Spawn[id ${_tmpID}].DisplayName}]} && !${Spawn[${_tmpID}].Master.Type.Equal[Pet]} && !${Spawn[${_tmpID}].Master.Type.Equal[Corpse]}) {	
				ECHOCHANNEL Oh yeah found a mob to charm up in this bitch! Grabbing ${Spawn[id ${_tmpID}].DisplayName}

				/squelch /target ID ${_tmpID}
				/if (!${Target.Tashed.Duration}) /call cast_tash ${_tmpID}
				/call core_cast "${spellpbaeMez}" 0 0 check_for_adds

				/if (${Target.Level} <= 46  && ${Me.AltAbilityReady[Dire Charm]}) {
					/call core_cast "Dire Charm" alt ${_tmpID} FALSE					
				} else {
					/call core_cast "${spellCharm}" 0 ${_tmpID} FALSE
				}
				
				/if (${Defined[timer_mez_${_tmpID}]}) /deletevar timer_mez_${_tmpID}
				/break
			}
		/next _count
	}
/return



|***
 *	DES: Class controls
 *	USE: /enc []
 *	INI: 
 *  NOTE:
 ***|
#bind setENC /enc
sub Bind_setENC(string _classType, string _verbage)
	DEBUG ${cbug}Bind_setENC(type:${_classType}, verbage:${_verbage})\ax
	/if (!${validate_class[TRUE, |ENC]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}

	| get help
	/if (${_classType.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Arg[${_count},|]}]}
		/next _count
		/return			
	} else /if (${Bool[${${_bind}Type.Find[${_classType}]}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_classType.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_classType.Lower}]}
		/return
	}
	
	| charm pet
	/if (${Bool[${_classType.Find[charm]}]}) {
		/call writeINI switchCharmPet Pet ${If[${switchCharmPet},FALSE,TRUE]}  
		VOUT /${_bind} charm ${sep} ${If[${switchCharmPet},${oenabled},${odisabled}]}

	| gather enable/disable /pct
	} else /if (${Bool[${_classType.Find[gather]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI GatherPercent Spells ${_verbage} 
			/call echos number "Gather Mana %" ${_verbage} 0 0
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} gather [${cinfo}${GatherPercent}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
		}	else /if (!${Bool[${_verbage}]}) {
			/call writeINI switchGather Spells ${If[${switchGather},FALSE,TRUE]}  
			/call echos switch "Gather Mana ${sep} ${GatherPercent}%" switchGather 0 0
		}			

	| spire control
	} else /if (${Bool[${_classType.Equal[spiregroup]}]} || ${Bool[${_classType.Equal[spireraid]}]}) {
		/declare _spiretype string local ${_classType.Left[5].Lower}${_classType.Mid[6,1].Upper}${_classType.Right[-6].Lower}
		/if (${Range.Between[0,3:${Int[${_verbage}]}]}) {
			/call writeINI ${_spiretype} Buffs ${_verbage}
		} 
		/for _count 1 to ${typeSpire.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${${_spiretype}} == ${typeSpire.Arg[${_count},|]},${cgood},${cinfo}]}${typeSpire.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} ${_classType.Lower} [${_listout}]	

	| healpoint
	} else /if (${Bool[${_classType.Find[healpoint]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI ${Me.Class.ShortName}Point Heal ${_verbage}
			VOUT /${_bind} healpoint ${sep} ${cinfo}${_verbage}\ax
		} else /if (!${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} healpoint [${cinfo}${${Me.Class.ShortName}Point}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
		}

	} else {
		/for _count 1 to ${${_bind}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
			/if (${${_bind}Type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
			/if (${${_bind}Type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
			/if (${${_bind}Type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}${${Me.Class.ShortName}Point}\ax)
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
	} 
/return