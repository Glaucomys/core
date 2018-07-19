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
	CHECKREZ
	/call prep_combat
	/if (${Macro.Return} && ${Bool[${combat}]}) {
		DEBUG combat:${cinfo}${combat.Upper}\ax		

		/if (${switchdeBuffMezz}) {
			/call get_target_ENC
			/if (${isValidTarget} && ${Target.ID} && ${Target.LineOfSight} && ${Target.Distance} <= ${MobAgro} && (${Target.PctHPs} <= ${smartengageAt} || ${Spawn[PC ID ${Me.TargetOfTarget.ID} Radius 60].ID})) /goto :loop
			/if (${Bool[${mobList}]}) /call list_delete mobList ${Target2Kill}
			/return
			:loop
			/if (${Spawn[${Target2Kill}].Type.Equal[Corpse]} || !${Target2Kill} || !${Target.ID} || ${Target.Type.Equal[corpse]} || !${Spawn[${Target2Kill}].ID}) /call get_target_ENC
			/call check_for_adds
			/if (${addList.Count[|]}) /call mez_adds
			/call check_for_adds
			/if (${addList.Count[|]}) /return
			/if (${mobList.Find[|]}) /call list_update mobList ${Target2Kill}
			/call check_for_adds
			/if (${addList.Count[|]}) /return
			/if (!${addList.Count[|]}) /call check_deBuffs ${Target2Kill}
			/if (${mobList.Find[|]}) /call list_update mobList ${Target2Kill}
		}

		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${switchHealCure}) /call cast_cure
		
		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/call check_deBuffs ${Target2Kill}
			/if (${switchPcSwarm} || ${switchPcPet}) /call send_pet
			/if (${switchCombatMelee}) /call use_skill_melee ${Target2Kill}
			/if (${switchdeBuffMezz}) {
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}
			/call disc_${Me.Class.ShortName}
			/if (${switchCombatDoT}) /call cast_DOT
			/if (${switchdeBuffMezz}) {
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}
			/if (${switchCombatNuke}) /call cast_nuke
			/if (${switchdeBuffMezz}) {
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}
			/if (${switchAlliance}) /call trigger_alliance ${Target2Kill}
			/call click_BP
			/call click_item
			/if (${switchPcSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
		}

		/if (${coreAuto}) {	
			/if (${validate_class[FALSE, ${CastList}]}) /call use_mod_rod
			/if (${validate_class[FALSE, ${MeleeList}]}) /call cast_stamina_recovery
			| /if (${switchCharmPet} && ${switchPcPet} && !${Me.Pet.ID}) /call find_mob_to_charm
			/if (${switchBuffItem}) /call check_crystals
			/if (!${is_target_dead[${Target2Kill}]}) /goto :killtarget
		} else /if (!${coreAuto}) {
			/if (${Target.ID} && ${Bool[${combat}]} && !${is_target_dead[${Target.ID}]}) /goto :killtarget
		}
	}
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

	/if (${validate_cast[FALSE, alt, "${runeVeil}", ${Me.ID}]}) /call core_cast2 "${runeVeil}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${runeEldritch}", ${Me.ID}]}) /call core_cast2 "${runeEldritch}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${runeSpray}", ${Me.ID}]}) /call core_cast2 "${runeSpray}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${runeReactive}", ${Me.ID}]}) /call core_cast2 "${runeReactive}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${runeMind}", ${Me.ID}]}) /call core_cast2 "${runeMind}" alt 0 FALSE
	/if (${validate_cast[FALSE, spell, "${runeAuspice}", ${Me.ID}]} && ${FindItemCount[Peridot]}) /call core_cast2 "${runeAuspice}" 0 ${Me.ID} FALSE
	
	| /if (${validate_buff[spell, rune, runeEntAuspice]} && ${FindItemCount[Peridot]}) /call core_cast "${runeEntAuspice}" ${buffGem} ${Me.ID} FALSE TRUE
	| /if (${validate_cast[FALSE, spell, "${runeEntAuspice}", ${Me.ID}]} && ${FindItemCount[Peridot]}) /call core_cast2 "${runeEntAuspice}" ${buffGem} ${Me.ID} FALSE
	 
	/if (${validate_cast[FALSE, spell, "${runeWard}", ${Me.ID}]}) /call core_cast2 "${runeWard}" 0 0 FALSE
	/if (${validate_cast[FALSE, spell, "${runePoly}", ${Me.ID}]}) /call core_cast2 "${runePoly}" ${buffGem} ${Me.ID} FALSE
	/if (${validate_cast[FALSE, spell, "${runeBrimstone}", ${Me.ID}]}) /call core_cast2 "${runeBrimstone}" ${buffGem} ${Me.ID} FALSE
	/if (${validate_cast[FALSE, spell, "${runeShieldofFate}", ${Me.ID}]}) /call core_cast2 "${runeShieldofFate}" ${buffGem} ${Me.ID} FALSE
	/if (${validate_cast[FALSE, spell, "${runeLegion}", ${Me.ID}]} && ${FindItemCount[Peridot]}) /call core_cast2 "${runeLegion}" ${buffGem} ${Me.ID} FALSE

	/call check_rune_auspice
	UPDATEHUD FALSE
/return



|***
 *	DES: EQBC Buff RuneAuspice spell
 *	USE: have the spell memed and the toon identified in the INI.
 *	INI: 
 *  NOTE: 
 ***|
sub check_rune_auspice()
	DEBUGCLASS ${cbug}check_rune_auspice(toons: ${runeAuspiceToon})\ax
	/if (!${FindItemCount[Peridot]}) /return
	AMIDEAD
	CHECKEXIT
	/doevents
	CHECKTIE
	/call check1offBuffs
	
	/declare _AuspiceToon string local FALSE
	/declare _count int local 0

	/for _count 1 to ${runeAuspiceToon.Count[|]}
		/if (!${Me.SpellReady[${runeAuspice}]}) /break
		/if (!${Spawn[${runeAuspiceToon.Arg[${_count},|]}].ID}) /continue
		/if (${NetBots[${runeAuspiceToon.Arg[${_count},|]}].Buff.Find[${Spell[${runeAuspice}].ID}]}) /continue
		CHECKEXIT
		/doevents
		/if (${Bool[${Spawn[pc ${runeAuspiceToon.Arg[${_count},|]}]}]} && ${Spawn[pc ${runeAuspiceToon.Arg[${_count},|]}].Distance} < ${Spell[${runeAuspice}].MyRange}) {
			/varset _AuspiceToon ${runeAuspiceToon.Arg[${_count},|]}
			/break
		}
	/next _count	

	| more reasons to bail..
	/if (!${Bool[${_AuspiceToon}]}) /return
	/if (!${Bool[${Spawn[${_AuspiceToon}]}]}) /return
	/if (${Spawn[${_AuspiceToon}].Type.Equal[corpse]}) /return
	
	UPDATEHUD "BUFF: BC Auspice"
	DEBUGCLASS BC Buffing ${sep} ${csp}${runeAuspice}\ax on [${cpc}${_AuspiceToon}\ax].

	/if (${validate_cast[FALSE, spell, "${runeAuspice}", ${NetBots[${_AuspiceToon}].ID}]}) /call core_cast2 "${runeAuspice}" 0 ${NetBots[${_AuspiceToon}].ID} check_for_adds
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
	
	/if (${validate_cast[FALSE, alt, "Crippling Aurora", ${Me.ID}]}) /call core_cast2 "Crippling Aurora" alt 0 FALSE
	/if (${switchPcSwarm}) {
		/if (${validate_cast[FALSE, alt, "Phantasmal Opponent", ${Me.ID}]} && (${Target.PctHPs} <= ${PcSwarmengageat} || ${switchOverrideEngageat})) /call core_cast2 "Phantasmal Opponent" alt 0 FALSE
	}
	/if (${validate_cast[FALSE, alt, "Chromatic Haze", ${Me.ID}]}) /call core_cast2 "Chromatic Haze" alt 0 FALSE
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


	/if (${Bool[${mannaCrystal}]} && !${FindItem[${mannaCrystal}].ID} && !${Cursor.Name.Equal[${mannaCrystal}]}) {
		/if (${validate_cast[FALSE, alt, "Azure Mind Crystal", ${Me.ID}]}) /call core_cast2 "Azure Mind Crystal" alt 0 check_for_adds
		/delay 20s !${Me.Casting.ID}
		/delay 2s ${Cursor.Name.Equal[${mannaCrystal}]}
		/call sort_cursor TRUE
	}

	/if (${Bool[${healCrystal}]} && !${FindItem[${healCrystal}].ID} && !${Cursor.Name.Equal[${healCrystal}]}) {
		/if (${validate_cast[FALSE, alt, "${healCrystal}", ${Me.ID}]}) /call core_cast2 "${healCrystal}" alt 0 check_for_adds
		/delay 20s !${Me.Casting.ID}
		/delay 2s ${Cursor.Name.Equal[${healCrystal}]}
		/call sort_cursor TRUE
	}

	/if (${Bool[${mannaCrystal}]}) {
		/if (${Me.PctMana} < 70*.${tmpHealOverride} && ${FindItem[${mannaCrystal}].ID} && !${FindItem[${mannaCrystal}].Timer}) {
			/if (${validate_cast[FALSE, item, "${mannaCrystal}", ${Me.ID}]}) /call core_cast2 "${mannaCrystal}" item 0 FALSE
		}
	}

	/call cast_AA_spire
	
	/if (${switchGather}) { 
		/if (${validate_cast[FALSE, alt, "Gather Mana", ${Me.ID}]} && ${Me.PctMana} < ${GatherPercent}*.${tmpHealOverride}) /call core_cast2 "Gather Mana" alt 0 FALSE
	}

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
				/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${validate_cast[FALSE, alt, "Doppelganger", ${Me.ID}]}) /call core_cast2 "Doppelganger" alt ${Me.ID} FALSE
				| /if (${Me.PctHPs} <= 50*.${tmpHealOverride} && ${Me.AltAbilityReady[Beguiler's Banishment]} && ${SpawnCount[npc radius 20]} > 2) /call core_cast "Beguiler's Banishment" alt
				| /if (${Me.PctHPs} <= 40*.${tmpHealOverride} && ${Me.AltAbilityReady[Beguiler's Directed Banishment]}) /call core_cast "Beguiler's Directed Banishment" alt ${Target2Kill} FALSE
				/if (${Me.PctHPs} <= 30*.${tmpHealOverride} && ${validate_cast[FALSE, alt, "Nightmare Stasis", ${Target2Kill}]} && ${Target.Level} < 76) /call core_cast2 "Nightmare Stasis" alt ${Target2Kill} FALSE
				/if (${Me.PctHPs} <= 30*.${tmpHealOverride} && ${validate_cast[FALSE, alt, "Dimensional Shield", ${Me.ID}]}) /call core_cast2 "Dimensional Shield" alt ${Me.ID} FALSE
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

				/if (${switchdeBuffTash}) {
					/if (${validate_cast[FALSE, alt, "Bite of Tashani", ${Me.ID}]}) /call core_cast2 "Bite of Tashani" alt 0 check_for_adds
				}
				
				/if (${Me.Gem[${spellpbaeMez}]} && ${Me.SpellReady[${spellpbaeMez}]}) {
					/if (${Me.Casting.Name.NotEqual[${spellpbaeMez}]}) /call interrupt
					/if (${validate_cast[FALSE, spell, "${spellpbaeMez}", ${Me.ID}]}) /call core_cast2 "${spellpbaeMez}" 0 0 check_for_adds
				}
				/if (${Me.Gem[${spellStun}]} && ${Me.SpellReady[${spellStun}]}) {
					/if (${Me.Casting.Name.NotEqual[${spellStun}]}) /call interrupt
					/if (${validate_cast[FALSE, spell, "${spellStun}", ${Me.ID}]}) /call core_cast2 "${spellStun}" 0 0 check_for_adds
				}
			}
			/if (${Spawn[${_tmpID}].NearestSpawn[3,npc radius 30].ID} && ${Spawn[${_tmpID}].Distance3D} <= 200 && ${Spawn[${_tmpID}].Distance3D} >= ${MobAgro}*.5) {
				/if (${Me.AltAbilityReady[Beam of Slumber]}) {
					/if (${Me.Casting.ID}) /call interrupt
					/face ID ${_tmpID} ${If[${SetFaceFast},fast,]}
					/if (${validate_cast[FALSE, alt, "Beam of Slumber", ${Me.ID}]}) /call core_cast2 "Beam of Slumber" alt 0				
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
	/if (${ExcludeList.Find[${Spawn[${_tmpID}].DisplayName}]}) /return
	/if (${Spawn[${_tmpID}].Type.Equal[corpse]}) /return
	/if (!${Spawn[${_tmpID}].ID}) /return
	/if (!${Target.LineOfSight}) /return

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
	| /if (${Me.CurrentMana} > ${Spell[${spellMez}].Mana}) {
		/delay 2.25s ${Me.SpellReady[${spellMez}]}
		/if (${validate_cast[FALSE, spell, "${spellMez}", ${_tmpID}]}) /call core_cast2 "${spellMez}" 0 ${_tmpID} check_for_adds
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
	| }
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

	/if (${ExcludeList.Find[${Spawn[${_tmpID}].DisplayName}]}) /return
	/if (${Spawn[${_tmpID}].Type.Equal[corpse]}) /return
	/if (!${Spawn[${_tmpID}].ID}) /return
	/if (!${Target.LineOfSight}) /return


	| if it's tashed, mark it and move on. better things to do
	/if (${Target.Tashed.Duration} && !${Target.Buff[Bite of Tashani].ID}) {
		DEBUGCLASS ${Target.DisplayName} already has ${Target.Tashed}
		/call create_timer timer_tash_${_tmpID} ${Spell[${spellTash}].Duration.TotalSeconds}s		
		/return
	}
	
	/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 49]} == 1) {
		/if (${validate_cast[FALSE, spell, "${spellTash}", ${_tmpID}]}) /call core_cast2 "${spellTash}" 0 ${_tmpID} check_for_adds
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_RESISTED,CAST_RESIST,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_tash_${_tmpID} ${Spell[${spellTash}].Duration.TotalSeconds}s
		}
	} else /if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 49]} > 1) {
		/if (${validate_cast[FALSE, alt, "Bite of Tashani", ${Me.ID}]}) /call core_cast2 "Bite of Tashani" alt 0 FALSE
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
	
	/if (${validate_cast[FALSE, alt, "${ENCIoG}", ${Me.ID}]}) /call core_cast2 "${ENCIoG}" alt 0
	/call cast_AA_spire
	/if (${validate_cast[FALSE, alt, "Dimensional Shield", ${Me.ID}]}) /call core_cast2 "Dimensional Shield" alt 0
	/if (${validate_cast[FALSE, alt, "Calculated Insanity", ${Me.ID}]}) /call core_cast2 "Calculated Insanity" alt 0

	| epic 2.0
	| /if (${Me.ItemReady[${${Me.Class.ShortName}Epic2}]} && ${Spell[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].Stacks} && !${Me.Buff[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID} && !${Me.Song[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID}) /call core_cast "${${Me.Class.ShortName}Epic2}" item 0 FALSE
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item ${Me.Pet.ID} FALSE

	
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
	/declare _maxlvl int local ${Int[${Math.Calc[${Me.Level}-1]}]}
	
	/delay 1
	
	/if (${SpawnCount[npc los Range 1 ${_maxlvl} radius ${MobAgro} targetable]}) {
		/varset _spawns ${SpawnCount[npc los Range 1 ${_maxlvl} radius ${MobAgro} targetable]} 
		/for _count 1 to ${_spawns}
			/varset _tmpID ${NearestSpawn[${_count}, npc los Range 1 ${_maxlvl} radius ${MobAgro} targetable].ID}
			/if (${CharmMobList.Find[${Spawn[id ${_tmpID}].DisplayName}]} && !${Spawn[${_tmpID}].Master.Type.Equal[Pet]} && !${Spawn[${_tmpID}].Master.Type.Equal[Corpse]}) {	
				ECHOCHANNEL Oh yeah found a mob to charm up in this bitch! Grabbing ${Spawn[id ${_tmpID}].DisplayName}

				/squelch /target ID ${_tmpID}
				/if (!${Target.Tashed.Duration}) /call cast_tash ${_tmpID}
				/if (${validate_cast[FALSE, spell, "${spellpbaeMez}", ${Me.ID}]}) /call core_cast2 "${spellpbaeMez}" 0 0 check_for_adds

				/if (${Target.Level} <= 46) {
					/if (${validate_cast[FALSE, alt, "Dire Charm", ${_tmpID}]}) /call core_cast2 "Dire Charm" alt ${_tmpID} FALSE					
				} else {
					/if (${validate_cast[FALSE, spell, "${spellCharm}", ${_tmpID}]}) /call core_cast2 "${spellCharm}" 0 ${_tmpID} FALSE
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
  /declare _type string	local |charm|gather|healpoint|spiregroup|spireraid


	| get help
	/if (${_classType.Equal[info]}) {
		/for _count 1 to ${_type.Count[|]}
			VOUT /${_bind} ${cinfo}${_type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${_type.Arg[${_count},|]}]}
		/next _count
		/return			
	} else /if (${Bool[${_type.Find[${_classType}]}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_classType.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_classType.Lower}]}
		/return
	}
	
	| charm pet
	/if (${Bool[${_classType.Find[charm]}]}) {
		/call writeINI switchCharmPet Pet ${If[${switchCharmPet},FALSE,TRUE]}  
		VOUT /${_bind} charm ${sep} ${If[${switchCharmPet},${oenabled},${odisabled}]}
		/return

	| gather enable/disable /pct
	} else /if (${Bool[${_classType.Find[gather]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI GatherPercent Spells ${_verbage} 
			/call echos number "Gather Mana %" ${_verbage} 0 0
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} gather [${cinfo}${GatherPercent}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call writeINI switchGather Spells ${If[${switchGather},FALSE,TRUE]}  
			/call echos switch "Gather Mana(${cnum}${GatherPercent}\ax)" switchGather 0 0
			/return
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
		/return

	| healpoint
	} else /if (${Bool[${_classType.Find[healpoint]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI ${Me.Class.ShortName}Point Heal ${_verbage}
		} else /if (!${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} healpoint [${cinfo}${${Me.Class.ShortName}Point}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}

	}
	/if (${switchMQclearonCLS}) /mqclear
	/for _count 1 to ${_type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_type.Arg[${_count},|]}\ax
		/if (${_type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${_type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${_type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}${${Me.Class.ShortName}Point}\ax)
		/if (${_type.Arg[${_count},|].Equal[gather]}) /varset _listout ${_listout}(${cnum}${GatherPercent}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]

/return