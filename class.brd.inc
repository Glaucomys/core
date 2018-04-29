|***
 *	class.brd.inc
 *	Core Bard Include
 *
 *  
 ***|



|***
 *	DES: Main Bard Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_BRD()
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}()\ax
	/doevents
	AMIDEAD
	CHECKTIE
		
	:killtarget
	CHECKREZ
	/call prep_combat
	/if (${Macro.Return} && ${Bool[${combat}]}) {
		DEBUG combat:${cinfo}${combat.Upper}\ax		
	
		/if (${switchdeBuffMezz}) {
			/call get_assist_target
			/if (${isValidTarget} && ${Target.ID} && ${Target.LineOfSight} && ${Target.Distance} <= ${MobAgro} && (${Target.PctHPs} <= ${smartengageAt} || ${Spawn[PC ID ${Me.TargetOfTarget.ID} Radius 60].ID})) /goto :loop
			/if (${Defined[timer_mez_${Target2Kill}]}) /deletevar timer_mez_${Target2Kill}
			/call list_delete mobList ${Target2Kill}
			/return
			:loop
			/if (${Spawn[${Target2Kill}].Type.Equal[Corpse]}  || !${Target2Kill} || !${Target.ID} || ${Target.Type.Equal[corpse]} || !${Spawn[${Target2Kill}].ID}) /call get_assist_target
			/call check_for_adds
			/if (${addList.Count[|]}) /call mez_adds_BRD
			/call check_for_adds
			/if (${addList.Count[|]}) /return
			/if (${mobList.Find[|]}) /call list_update
			/call check_for_adds
			/if (${addList.Count[|]}) /return
		}

		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${switchHealCure}) /call cast_cure
		
		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${switchPcSwarm} || ${switchPcPet}) /call send_pet
			/if (${switchCombatMelee}) /call use_skill_melee ${Target2Kill}
			/if (${switchdeBuffMezz}) {
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}			
			/call disc_${Me.Class.ShortName}
			/if (${switchCombatDoT}) /call bard_dot
			/if (${switchCombatNuke}) /call bard_nuke
			/call pulse
			/if (${switchAlliance}) /call trigger_alliance ${Target2Kill}
			/if (${switchdeBuffMezz}) {
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}		
			/call click_BP
			/call click_item
			/if (${switchPcSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
	
			/if (${coreAuto}) {	
				/if (${validate_class[FALSE, ${CastList}]}) /call use_mod_rod
				/if (${validate_class[FALSE, ${MeleeList}]}) /call cast_stamina_recovery
				/if (!${Spawn[${Target2Kill}].ID} && !${is_target_dead[${Target2Kill}]}) /goto :killtarget
			} else /if (!${coreAuto}) {
				/if (${Target.ID} && ${Bool[${combat}]} && !${is_target_dead[${Target.ID}]}) /goto :killtarget
			}
		} 
	}
/return




|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: Disc
 *	INI: 
 *  NOTE: 
 ***|
sub disc_BRD()
	DEBUGCLASS	${cbug}disc_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${switchdeBuffSlow} && !${timer_slow_${Target2Kill}}) /call pulse_slow ${Target2Kill}
	/if (${Me.Casting.ID}) /call pulse_stop
	/if (!${Me.Song[Doben's Spry Sonata Rk. II].ID}) /call core_cast "Darkened Breath of Harmony" item 0 FALSE
	/if (${switchBanestrike}) /call cast_AA_banestrike ${Target2Kill} FALSE
	/call check_deBuffs ${Target2Kill}
	/if (${switchPcSwarm}) /call cast_servant_swarm ${Target2Kill} FALSE
	/call core_cast "Cacophony" alt ${Target2Kill} FALSE
	/call core_cast "Bladed Song" alt ${Target2Kill} FALSE
	/call core_cast "Boastful Bellow" alt ${Target2Kill} FALSE	
	
	| trigger Frenzied kicks AA for bards if target is debuffed from intimidate
	/if (${Bool[${intimidation}]} && ${switchIntimidKick} && ${Target.Buff[Harbinger's Intimidation].ID} && ${Spawn[${Target2Kill}].Distance} <= 15) /call core_cast "Frenzied Kicks" alt 0 FALSE
	
/return



|***
 *	DES: emergency healing
 *	USE: /call patch_heal_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_BRD()
	DEBUGCLASS ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	/call click_item_heal
/return



|***
 *	DES: DPS Clickies
 *	USE: /call click_DPS_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_BRD()
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	AMIDEAD
	CHECKEXIT
	/doevents
	/call stick_to_target
	/call click_horn_of_unity 	
/return



|***
 *	DES: rotates bard swarm pets as DPS fillers
 *	USE: /call cast_servant_swarm
 *	INI: 
 *  NOTE: send in ALL THE PETS
 ***|
sub cast_servant_swarm(int _tmpID)
	DEBUGCLASS ${cbug}cast_servant_swarm(target: ${_tmpID})\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${Spawn[${_tmpID}].LineOfSight} && ${Target.PctHPs} > 10 && ${Spawn[${_tmpID}].Distance} < ${MobAgro}) {
		/if (${is_target_dead[${Target2Kill}]}) /return

		/call get_target_2kill
		/if (${Target.ID}) /pet swarm

		/if (${Me.AltAbilityReady[${ServantNukeAA1}]}) {
			/if (${Me.Casting.ID}) /call pulse_stop
			/call core_cast "${ServantNukeAA1}" alt ${_tmpID}
			/return
		} else /if (${Me.AltAbilityReady[${ServantNukeAA2}]}) {
			/if (${Me.Casting.ID}) /call pulse_stop
			/call core_cast "${ServantNukeAA2}" alt ${_tmpID}
			/return
		} else /if (${Me.SpellReady[${songServantNuke}]}) {
			/if (${Me.Casting.ID}) /call pulse_stop
			/call core_sing ${Me.Gem[${songServantNuke}]}
			/return
		}
	}
/return



|***
 *	DES: memorize songs
 *	USE: /call mem_song_line [] | /brd line [TANK|CASTER|MELEE|MIX|OTHER]
 *	INI: [Song] -> line[TANK|CASTER|MELEE|MIX]
 *  NOTE:
 ***|
sub mem_song_line(string _line)
 	DEBUGCLASS ${cbug}mem_song_line(group: ${_line})\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	/declare _count 				int local
	/declare _index 				int local	
	/declare _list[1] 			string ${line${lineCurrent}}
	/declare _currentSong 	string NULL
	
	/for _count 1 to ${_list.Size}
		/for _index 1 to ${_list[${_count}].Count[|]}
			/varset _currentSong ${Spell[${_list[${_count}].Arg[${_index},|]}].RankName}
			/if (${Me.Gem[${_index}].Name.Equal[${_currentSong}]}) /continue
			/call mem_spell TRUE "${_currentSong}" "${_index}" FALSE
		/next _index	
	/next _count	
	VOUT Line ${sep} ${cinfo}${lineCurrent}\ax memorization completed.
/return



|***
 *	DES: Sing "Pulse" a single song
 *	USE: /call pulse [gem##] 
 *	INI: 
 *  NOTE:
 ***|
sub pulse()
 	DEBUGCLASS ${cbug}pulse()\ax

	/declare _pulseSong			bool local FALSE
	/declare _count 				int local
	/declare _index 				int local	
	/declare _list[1] 			string ${line${lineCurrent.Left[1].Upper}${lineCurrent.Right[-1].Lower}}

	UPDATEHUD "PULSE: Song"
	
	/for _count 1 to ${_list.Size}
		AMIDEAD
		CHECKEXIT
		/doevents
		/call is_target_dead
		/if (${Macro.Return}) {
			/call pulse_stop
			/return
		}
		/for _index 1 to ${_list[${_count}].Count[|]}
			AMIDEAD
			| CHECKEXIT
			/doevents
			/call is_target_dead
			/if (${Macro.Return}) {
				/call pulse_stop
				/return
			}
			/call pulse_valid ${_index}
			/if (${Macro.Return}) {
				/call pulse_stop
				/call core_sing ${_index}
			}
		/next _index	
	/next _count	

	UPDATEHUD FALSE
/return



|***
 *	DES: check pulse cast. we check the target to see if it is worth casting on.
 *	USE: /call pulse_valid [gem##] 
 *	INI: 
 *  NOTE:
 ***|
sub pulse_valid(_gem)
	DEBUGCLASS ${cbug}pulse_valid(gem: ${_gem})\ax
	/declare _pulseSong bool local TRUE
	/if (${Me.CurrentMana} < ${Spell[${Me.Gem[${_gem}].Name}].Mana}) /varset _pulseSong FALSE
	/if (${Me.Buff[${Me.Gem[${_gem}].Name}].Duration.Seconds} > 6) /varset _pulseSong FALSE
	/if (${Me.Song[${Me.Gem[${_gem}].Name}].Duration.Seconds} > 6) /varset _pulseSong FALSE
	/if (!${Me.SpellReady[${Me.Gem[${_gem}].Name}]}) /varset _pulseSong FALSE
	/if (!${Spell[${Me.Gem[${_gem}].Name}].Stacks}) /varset _pulseSong FALSE
	DEBUGCLASS VALID PULSE: ${If[${_pulseSong},${cgood}${Me.Gem[${_gem}].Name}\ax,${cbad}${Me.Gem[${_gem}].Name}\ax]}
/return ${Bool[${_pulseSong}]}



|***
 *	DES: quit singing. your voice sounds like ass.
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pulse_stop()
	DEBUGCLASS ${cbug}pulse_stop()\ax 
	/while (${Me.Casting.ID}) {
		/stopcast
		/delay 1
	}
/return


|***
 *	DES: Sing Songs
 *	USE: /call core_sing [gem##]
 *	INI: 
 *  NOTE:
 ***|
sub core_sing(_spellgem)
	DEBUGCLASS ${cbug}core_sing(gem: ${_spellgem})\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return
	/call fix_gem_id "${_spellgem}"
	/declare _gem int local ${Macro.Return}
	/if (${Me.CurrentMana} < ${Spell[${Me.Gem[${_gem}]}].Mana}) /return	
	/call stick_to_target
	/if (${Me.Casting.ID}) /call pulse_stop
	VOUT Pulse ${sep} ${csp}${Me.Gem[${_gem}]}\ax
	/melody ${_gem}
	| /delay 25
	/delay ${Spell[${Me.Gem[${_gem}].Name}].CastTime.Seconds}s !${Me.Casting.ID}
	/delay 2
	/call pulse_stop
/return



|***
 *	DES: Cast nukes
 *	USE: /call bard_nuke
 *	INI: 
 *  NOTE: 
 ***|
sub bard_nuke()
	DEBUGCLASS ${cbug}bard_nuke()\ax
	/call get_assist_target
	/declare _count int local
	/for _count 1 to ${DoNuke}
		AMIDEAD
		CHECKEXIT
		/doevents
		/if (${is_target_dead[${Target2Kill}]}) /return
		/if (${Spawn[${Target2Kill}].LineOfSight} && ${Me.SpellReady[${Nuke${_count}}]} && ${Spawn[${Target2Kill}].Distance} <= ${Spell[${Nuke${_count}}].MyRange} && ${Me.CurrentMana} >= ${Spell[${Nuke${_count}}].Mana}) {
			/call stick_to_target
			/call core_sing ${Me.Gem[${Nuke${_count}}]}
		}
	/next _count
/return




|***
 *	DES: Cast nukes
 *	USE: /call bard_nuke
 *	INI: 
 *  NOTE: 
 ***|
sub bard_dot()
	DEBUGCLASS ${cbug}bard_dot()\ax
	/call get_assist_target
	/declare _count int local
	/for _count 1 to ${DoDoT}
		AMIDEAD
		CHECKEXIT
		/doevents
		/if (${is_target_dead[${Target2Kill}]}) /return
		/if (${Target.BuffDuration[${DoT${_count}}]} >= 2) /return
		/if (${Spawn[${Target2Kill}].LineOfSight} && ${Me.SpellReady[${DoT${_count}}]} && ${Spawn[${Target2Kill}].Distance} <= ${Spell[${DoT${_count}}].MyRange} && ${Me.CurrentMana} >= ${DoT[${DoT${_count}}].Mana}) {
			/call stick_to_target
			/call core_sing ${Me.Gem[${DoT${_count}}]}
		}
	/next _count
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub mez_adds_BRD()
	DEBUGCLASS ${cbug}mez_adds()\ax
	AMIDEAD

	/doevents
	/declare _tmpID string local

	:loopmezzadds
	/if (${addList.Count[|]} && ${Me.CurrentMana} >= ${Spell[${spellMez}].Mana}) {
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
			/delay 4s ${Me.SpellReady[${spellpbaeMez}]}
			/if (${SpawnCount[npc los radius 30 zradius 15 targetable]} > 3 ) {
				VOUT ${sepO} ${cinfo}${SpawnCount[npc radius 30 zradius 15]}\ax ${sepC} Targets in point blank range.
				/if (${Me.Gem[${spellpbaeMez}]} && ${Me.SpellReady[${spellpbaeMez}]}) {
					/if (${Me.Casting.Name.NotEqual[${spellpbaeMez}]}) /call pulse_stop
					/call core_sing ${Me.Gem[${spellpbaeMez}]}
				}
			}
			/call cast_mez_BRD ${_tmpID}
		}
		/call list_delete addList ${_tmpID}
		VOUT ${sepO} ${cinfo}${addList.Count[|]}\ax ${sepC} ${If[${addList.Count[|]} == 1,Target,Targets]} left to mez.
		/goto :loopmezzadds
	}
/return



|***
 *	DES: mez the target
 *	USE: debufs
 *	INI: 
 *  NOTE: 
 ***|
sub cast_mez_BRD(int _tmpID)
	DEBUGCLASS ${cbug}cast_mez_BRD(${_tmpID})\ax

	:loopmez
	AMIDEAD

	/if (!${Target.LineOfSight} || ${ExcludeList.Find[${Target.DisplayName}]} || ${Spawn[${_tmpID}].Type.Equal[corpse]} || !${Spawn[${_tmpID}].ID}) /return

	| check for mez immune list mobs
	/if (${NoMezzList.Find[${Spawn[ID ${_tmpID}].CleanName}]}) {
	  /call create_timer timer_mez_${_tmpID} ${Spell[${spellMez}].Duration.TotalSeconds}m
	  VOUT ${owarning} IMMUNE TO\ax ${csp}${spellMez}\ax ${sep} [${ctar}${Target.CleanName}\ax]
	  /call list_delete addList ${_tmpID}	
		/return
	}
	
	/if (${Target.Mezzed.Duration} && ${Spell[${Target.Mezzed}].Name.Find[${spellMez}]}) {
		/call create_timer timer_mez_${_tmpID} ${Target.Mezzed.Duration.TotalSeconds}s
		/return
	} else /if (${Target.PctHPs} < 90) {
		/call create_timer timer_mez_${_tmpID} 10m
		VOUT [${ctar}${Target.CleanName}\ax] is being attacked!
		/return
	}
	/if (${Me.CurrentMana}>${Spell[${spellMez}].Mana}) {
		/delay 6s ${Me.SpellReady[${spellMez}]}
		/target ID ${_tmpID}
		/delay 5 ${Target.ID} == ${_tmpID}
		/call core_sing ${Me.Gem[${spellMez}]}
		/if (${Target.Mezzed.Duration.TotalSeconds}) /call create_timer timer_mez_${_tmpID} ${Spell[${spellMez}].Duration.Seconds}s
		/goto :loopmez
	}
/return



|***
 *	DES: Bard AA version of Breather
 *	USE: /call cast_AA_Rallying
 *	INI: 
 *  NOTE: 
 *  AA - Rallying Solo
 *  Song - Rallying Solo Azia V
 ***|
sub cast_AA_Rallying()
 	DEBUGCLASS ${cbug}cast_AA_Rallying(${Me.AltAbilityReady[Rallying Solo]})\ax
 	/if (${SafeZone} && !${switchOverrideSafezone} || !${Me.AltAbilityReady[Rallying Solo]}) /return
	AMIDEAD
	/doevents
 	/declare _endurancePoint		int local 30
 	/declare _manaPoint		int local 30

 	/if (((${Me.PctEndurance} < ${_endurancePoint}) || (${Me.PctMana} < ${_manaPoint})) && ${Select[${Me.CombatState},RESTING]}) {
		/call core_cast "Rallying Solo" alt ${Me.ID}
	}
/return



 |***
 *	DES: Bard burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_BRD()
	DEBUGCLASS ${cbug}synergy_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${Me.Casting.ID}) /call pulse_stop
	
	| epic 2.0
	/if (${Me.ItemReady[${${Me.Class.ShortName}Epic2}]} && ${Spell[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].Stacks} && !${Me.Buff[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID} && !${Me.Song[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID}) /call core_cast "${${Me.Class.ShortName}Epic2}" item 0 FALSE
	
 	/if (${Me.AltAbilityReady[Bladed Song]}) /call core_cast "Bladed Song" alt 0 FALSE
 	/if (${Me.CombatAbilityReady[Thousand Blades]}) /call core_cast "Thousand Blades" disc 0 FALSE	
	/if (${Me.AltAbilityReady[Fierce Eye]}) /call core_cast "Fierce Eye" alt 0 FALSE
	/call cast_AA_spire
	/if (${Me.AltAbilityReady[Funeral Dirge]}) /call core_cast "Funeral Dirge" alt 0 FALSE
	/if (${Me.AltAbilityReady[Quick Time]} && !${Me.Song[Ruaabri's Fury].ID}) /call core_cast "Quick Time" alt 0 FALSE

	/call use_glyph
	/call synergy_click_Circle
	/call click_item_burn	
/return



|***
 *	DES: AA Slow
 *	USE: /call pulse_slow [TARGET ID]
 *	INI: 
 *  NOTE: 
 ***|
sub pulse_slow(int _tmpID)
	DEBUGCLASS ${cbug}pulse_slow(ID ${_tmpID})\ax
	/if (${timer_slow_${_tmpID}}) /return
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return
		
	| dont waste time if its slowed already
	/if (${Target.Slowed.Duration}) {
		DEBUG Target Slowed: ${Target.Slowed.Duration.TotalSeconds}s
		/call create_timer timer_slow_${_tmpID} ${Target.Slowed.Duration.TotalSeconds}s
		/return
	}
	
	/if (${Spawn[${_tmpID}].Type.NotEqual[PC]} && ${Spawn[${_tmpID}].Distance} <= ${Spell[${spellSlow}].MyRange} && !${timer_slow_${_tmpID}}) {
		/call core_sing ${Me.Gem[${spellSlow}]}
	}
/return



| Under Construction...
sub validate_pulse(string _type, string _spell)
	DEBUGCLASS ${cbug}validate_pulse(type: ${_type}, ${_spell})\ax
	/if (${_type.Equal[alt]} && ${Me.AltAbilityReady[${_spell}]}) {
		/if (${Me.CurrentMana} < ${Spell[${_spell}].Mana}) /return FALSE
		/call pulse_stop
		
		
	}	else /if (${_type.Equal[spell]} && ${Me.SpellReady[${_spell}]}) {
		/if (${Me.CurrentMana} < ${Spell[${_spell}].Mana}) /return FALSE
		/call pulse_stop
	} else /if (${_type.Equal[disc]} && ${Me.CombatAbilityReady[${_spell}]}) {
		/call pulse_stop
	
	}
/return TRUE



|***
 *	DES: Class controls
 *	USE: /brd []
 *	INI: 
 *  NOTE:
 ***|
#bind setBRD /brd
sub Bind_setBRD(string _classType, string _verbage)
	DEBUGCLASS ${cbug}Bind_setBRD(type:${_classType}, verbage:${_verbage})\ax
	/if (!${validate_class[TRUE, |BRD]}) /return

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
	
	| enable/disable rest song
	/if (${Bool[${_classType.Equal[restsong]}]}) {
		/call writeINI switchRestSong Song ${If[${switchRestSong},FALSE,TRUE]}
		VOUT /${_bind} restsong ${sep} ${If[${switchRestSong},${oenabled},${odisabled}]}

	| enable/disable sets bard song line
	} else /if (${Bool[${_classType.Equal[line]}]}) {
		/if (${Bool[${lineType.Find[${_verbage.Lower}]}]}) {
			DEBUGCLASS ${lineType.Find[${_verbage.Lower}]}
			/call writeINI lineCurrent Song ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
			/delay 2
			/call mem_song_line ${lineCurrent}
		}
		/for _count 1 to ${lineType.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${lineCurrent.Equal[${lineType.Arg[${_count},|]}]},${cgood},${cinfo}]}${lineType.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} line [${_listout}]	
		/return

	| intimidation/frenzied kick control
	} else /if (${Bool[${_classType.Equal[intimidkick]}]}) {
		/call writeINI switchIntimidKick meleedisc ${If[${switchIntimidKick},FALSE,TRUE]}
		VOUT /${_bind} intimidkick ${sep} ${If[${switchIntimidKick},${oenabled},${odisabled}]}

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
			VOUT /${_bind} healpoint(${cnum}${${Me.Class.ShortName}Point}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}

	}
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
		/if (${${_bind}Type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}${${Me.Class.ShortName}Point}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return