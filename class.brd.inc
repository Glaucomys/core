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
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}(${cinfo}${version${Me.Class.ShortName}}\ax${cbug})\ax
	/doevents
	ISDEAD
	CHECKTIE
		
	:killtarget
	/call prep_combat
	/if (!${Bool[${Macro.Return}]}) /goto :exit

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/call send_pet
		/call stick_to_target
		/if (${Bool[${Target2Kill}]}) /call disc_${Me.Class.ShortName}
		/call click_DPS_${Me.Class.ShortName}
		/if (${DoNuke}) /call bard_nuke
		/call pulse
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
		/call get_assist_target
		/if (${isValidTarget} && ${Target.ID} && ${Target.LineOfSight} && ${Target.Distance} <= ${MobAgro} && (${Target.PctHPs} <= ${smartengageAt} || ${Spawn[PC ID ${Me.TargetOfTarget.ID} Radius 60].ID})) /goto :loop
		/if (${Defined[timer_mez_${Target2Kill}]}) /deletevar timer_mez_${Target2Kill}
		/call list_delete mobList ${Target2Kill}
		/goto :exit
		:loop
		/doevents
		ISDEAD
		CHECKTIE
		/if (${Spawn[${Target2Kill}].Type.Equal[Corpse]}  || !${Target2Kill} || !${Target.ID} || ${Target.Type.Equal[corpse]} || !${Spawn[${Target2Kill}].ID}) /call get_assist_target
		/call check_for_adds
		/if (${addList.Count[|]} && ${switchdeBuffMezz}) /call mez_adds_BRD
		/call check_for_adds
		/if (${addList.Count[|]}) /goto :exit
		/if (${mobList.Find[|]}) /call list_update
		/call check_for_adds
		/if (${addList.Count[|]}) /goto :exit
		/call is_target_valid ${Target2Kill}
		/if (${isValidTarget}) {
			/if (!${addList.Count[|]}) /call set_combat MELEE
			/if (!${addList.Count[|]}) /call stick_to_target
			/call check_for_adds
			/if (${addList.Count[|]}) /goto :exit
			/if (!${addList.Count[|]} && ${DoNuke}) /call bard_nuke
			/call check_for_adds
			/if (${addList.Count[|]}) /goto :exit
			/if (!${addList.Count[|]}) /call pulse
		}

	}
	/if (${coreAuto}) {	
		/if (${switchHealCure}) /call cast_cure
	}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} || !${coreAuto} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Bool[${Macro.Return}]}) /goto :killtarget
	}

	:exit
/return



|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: Disc
 *	INI: 
 *  NOTE: 
 ***|
sub disc_BRD()
	DEBUGCLASS	${cbug}disc_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/call stick_to_target
	/if ((${Me.AltAbilityReady[Selo's Kick]} || ${Me.AltAbilityReady[Banestrike]} || ${Me.AltAbilityReady[Cacophony]} || ${Me.AltAbilityReady[Bladed Song]} || ${Me.AltAbilityReady[Boastful Bellow]}) && ${Me.Casting.ID}) /call pulse_stop
	/call item_harmony_blade
	/if (${Me.AltAbilityReady[Banestrike]} && ${switchBanestrike}) /call MQ2Cast "Banestrike" -targetID|${Target2Kill}
	/if (${Me.AltAbilityReady[Selo's Kick]}) /call MQ2Cast "Selo's Kick" alt -targetID|${Target2Kill}
	/if (${usePetSwarm}) /call cast_servant_swarm ${Target2Kill}
	/if (${Me.AltAbilityReady[Cacophony]}) /call MQ2Cast "Cacophony" alt -targetID|${Target2Kill}
	/if (${Me.AltAbilityReady[Bladed Song]}) /call MQ2Cast "Bladed Song" alt -targetID|${Target2Kill}
	/if (${Me.AltAbilityReady[Boastful Bellow]}) /call MQ2Cast "Boastful Bellow" alt -targetID|${Target2Kill}

	:exit
/return



|***
 *	DES: DPS Clickies
 *	USE: /call click_DPS_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_BRD()
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/call stick_to_target
	/call click_horn_of_unity 	
	:exit
/return



|***
 *	DES: rotates bard swarm pets as DPS fillers
 *	USE: /call cast_servant_swarm
 *	INI: 
 *  NOTE: send in ALL THE PETS
 ***|
sub cast_servant_swarm(int _tmpID)
	DEBUGCLASS ${cbug}cast_servant_swarm(target: ${_tmpID})\ax
	ISDEAD
	/doevents
	/call check_exit TRUE
	/if (${Bool[${Macro.Return}]} || !${usePetSwarm}) /goto :exit
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /goto :exit
	
	/if (${Spawn[${_tmpID}].LineOfSight} && ${Target.PctHPs} > 10 && ${Spawn[${_tmpID}].Distance} < ${MobAgro}) {
		/pet swarm
		/if (${Me.AltAbilityReady[${ServantNukeAA1}]}) {
			/call core_cast "${ServantNukeAA1}" alt ${_tmpID}
			/goto :exit
		} else /if (${Me.AltAbilityReady[${ServantNukeAA2}]}) {
			/call core_cast "${ServantNukeAA2}" alt ${_tmpID}
			/goto :exit
		} else /if (${Me.SpellReady[${songServantNuke}]}) {
			/call core_sing ${Me.Gem[${songServantNuke}]}
			/goto :exit
		}
	}
	
	:exit
/return



|***
 *	DES: memorize songs
 *	USE: /call mem_song_set [TANK|CASTER|MELEE|MIX]
 *	INI: [Song] -> song[TANK|CASTER|MELEE|MIX]List
 *  NOTE: send in ALL THE PETS
 ***|
sub mem_song_set(string _group)
 	DEBUGCLASS ${cbug}mem_song_set(group: ${_group})\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit

	/call Event_SetVar NA songList Song ${_group.Upper}
	
	/declare _count 				int local
	/declare _index 				int local	
	/declare _list[1] 			string ${song${songList.Upper}List}
	/declare _currentSong 	string NULL
	
	/for _count 1 to ${_list.Size}
		/for _index 1 to ${_list[${_count}].Count[|]}
			/varset _currentSong ${Spell[${_list[${_count}].Arg[${_index},|]}].RankName}
			/if (${Me.Gem[${_index}].Name.Equal[${_currentSong}]} ) /goto :skipmemorize
			/call scribe "${_currentSong}" "${_index}" FALSE
			:skipmemorize
		/next _index	
	/next _count	
	VOUT Song Line ${cinfo}${songList.Upper}\ax scribe completed.
	
	:exit	
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
	/declare _list[1] 			string ${song${songList.Upper}List}

	UPDATEHUD "PULSE: Song"
	
	/for _count 1 to ${_list.Size}
		/for _index 1 to ${_list[${_count}].Count[|]}
			ISDEAD
			/doevents
			/call check_exit
			/if (${Bool[${Macro.Return}]}) /goto :exit
			/call is_target_dead
			/if (${Bool[${Macro.Return}]}) /goto :exit
			/call stick_to_target
			| /if (${Me.Casting.ID}) /stopcast
			/call pulse_valid ${_index}
			/if (${Bool[${Macro.Return}]}) /call core_sing ${_index}
		/next _index	
	/next _count	

	:exit
	UPDATEHUD
/return



|***
 *	DES: check pulse cast. we check the target to see if it is worth casting on.
 *	USE: /call pulse_valid [gem##] 
 *	INI: 
 *  NOTE:
 ***|
sub pulse_valid(_gem)
	DEBUGCLASS ${cbug}pulse_valid(gem: ${_gem})\ax
	/declare _pulseSong			bool local TRUE
	/if (${Me.CurrentMana} < ${Spell[${Me.Gem[${_gem}].Name}].Mana}) /varset _pulseSong FALSE
	/if (${Me.Buff[${Me.Gem[${_gem}].Name}].Duration.Seconds} > 6) /varset _pulseSong FALSE
	/if (${Me.Song[${Me.Gem[${_gem}].Name}].Duration.Seconds} > 6) /varset _pulseSong FALSE
	/if (!${Me.SpellReady[${Me.Gem[${_gem}].Name}]}) /varset _pulseSong FALSE
	/if (!${Spell[${Me.Gem[${_gem}].Name}].Stacks}) /varset _pulseSong FALSE
	:exit
	DEBUGCLASS VALID CAST: ${If[${_pulseSong},${cgood}${Me.Gem[${_gem}].Name}\ax,${cbad}${Me.Gem[${_gem}].Name}\ax]}
/return ${Bool[${_pulseSong}]}



|***
 *	DES: quit singing. your voice sounds like ass.
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pulse_stop()
	DEBUGCLASS ${cbug}pulse_stop()\ax 
	
	/stopcast		
	/delay 2
	:stoppulse
	/if (${Me.Casting.ID}) {
		/delay 1
		/stopcast
		/goto :stoppulse
	}
	:exit
/return


|***
 *	DES: Sing Songs
 *	USE: /call core_sing [gem##]
 *	INI: 
 *  NOTE:
 ***|
sub core_sing(_spellgem)
	DEBUGCLASS ${cbug}core_sing(gem: ${_spellgem})\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/call fix_gem_id "${_spellgem}"
	/declare _gem			int local ${Macro.Return}
	/if (${Me.CurrentMana} < ${Spell[${Me.Gem[${_gem}].Name}].Mana}) /goto :exit	
	/call stick_to_target
	/if (${Me.Casting.ID}) {
		| /call Interrupt
		/stopcast
	}
	VOUT Pulse ${sep} ${csp}${Me.Gem[${_gem}].Name}\ax
	/melody ${_gem}
	| /delay 25
	/delay ${Spell[${Me.Gem[${_gem}].Name}].CastTime.Seconds}s !${Me.Casting.ID}
	
	
	/delay 2
	/call pulse_stop

	:exit
/return



|***
 *	DES: Cast nukes
 *	USE: /call bard_nuke
 *	INI: 
 *  NOTE: 
 ***|
sub bard_nuke()
	DEBUGCLASS ${cbug}bard_nuke()\ax
	ISDEAD
	/doevents 
	/call get_assist_target
	/declare _count int local
	/for _count 1 to ${DoNuke}
		ISDEAD
		/doevents
		/call check_exit TRUE
		/if (${Bool[${Macro.Return}]}) /goto :exit	
		/call is_target_dead
		/if (${Bool[${Macro.Return}]}) /goto :exit	
		/if (${Spawn[${Target2Kill}].LineOfSight} && ${Me.SpellReady[${Nuke${_count}}]} && ${Spawn[${Target2Kill}].Distance} <= ${Spell[${Nuke${_count}}].MyRange} && ${Me.CurrentMana} >= ${Spell[${Nuke${_count}}].Mana}) {
			/call stick_to_target
			/call core_sing ${Me.Gem[${Nuke${_count}}]}
		}
	/next _count
	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub mez_adds_BRD()
	DEBUGCLASS ${cbug}mez_adds()\ax
	ISDEAD
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
					/if (${Me.Casting.Name.NotEqual[${spellpbaeMez}]}) /stopcast
					/call core_sing ${Me.Gem[${spellpbaeMez}]}
				}
			}
			/call cast_mez_BRD ${_tmpID}
		}
		/call list_delete addList ${_tmpID}
		VOUT ${sepO} ${cinfo}${addList.Count[|]}\ax ${sepC} ${If[${addList.Count[|]} == 1,Target,Targets]} left to mez.
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
sub cast_mez_BRD(int _tmpID)
	DEBUGCLASS ${cbug}cast_mez_BRD(${_tmpID})\ax
	/doevents
	ISDEAD
	:loopmez
	ISDEAD
	/if (!${Target.LineOfSight} || ${ExcludeList.Find[${Target.DisplayName}]} || ${Spawn[${_tmpID}].Type.Equal[corpse]} || !${Spawn[${_tmpID}].ID}) /goto :exit

	| check for mez immune list mobs
	/if (${NoMezzList.Find[${Spawn[ID ${_tmpID}].CleanName}]}) {
	  /call create_timer timer_mez_${_tmpID} ${Spell[${spellMez}].Duration.TotalSeconds}m
	  NOUT ${owarning} IMMUNE TO\ax ${csp}${spellMez}\ax ${sep} [${ctar}${Target.CleanName}\ax]
	  /call list_delete addList ${_tmpID}	
		/goto :exit
	}
	
	/if (${Target.Mezzed.Duration} && ${Spell[${Target.Mezzed}].Name.Find[${spellMez}]}) {
		/call create_timer timer_mez_${_tmpID} ${Target.Mezzed.Duration.TotalSeconds}s
		/goto :exit
	} else /if (${Target.PctHPs} < 90) {
		/call create_timer timer_mez_${_tmpID} 10m
		VOUT [${ctar}${Target.CleanName}\ax] is being attacked!
		/goto :exit
	}
	/if (${Me.CurrentMana}>${Spell[${spellMez}].Mana}) {
		/delay 6s ${Me.SpellReady[${spellMez}]}
		/target ID ${_tmpID}
		/delay 5 ${Target.ID} == ${_tmpID}
		/call core_sing ${Me.Gem[${spellMez}]}
		/if (${Target.Mezzed.Duration.TotalSeconds}) /call create_timer timer_mez_${_tmpID} ${Spell[${spellMez}].Duration.Seconds}s
		/goto :loopmez
	}
	:exit
/return



|***
 *	DES: uses harmony blade from HH Old Seb.
 *	USE: /call item_harmony_blade
 *	INI: 
 *  NOTE: 
 ***|
sub item_harmony_blade()
	DEBUGCLASS ${cbug}item_harmony_blade()\ax
	/if (${Me.ItemReady[Darkened Breath of Harmony]} || !${Me.Song[Doben's Spry Sonata Rk. II].ID}) {
		/call core_cast "Darkened Breath of Harmony" item 0
	}
/return



|***
 *	DES: Bard AA version of Breather
 *	USE: /call cast_AA_Rallying
 *	INI: 
 *  NOTE: 
 *  AA - Rallying Call
 *  Song - Rallying Call Azia V
 ***|
sub cast_AA_Rallying()
 	DEBUGCAST ${cbug}cast_AA_Rallying(${Me.AltAbilityReady[Rallying Call]})\ax
	ISDEAD
	/doevents

 	/if (${SafeZone} && !${switchOverrideSafezone} || !${Me.AltAbilityReady[Rallying Call]}) /goto :exit
	
 	/if (!${Defined[_endurancePoint]}) /declare _endurancePoint		int local 30
 	/if (!${Defined[_manaPoint]}) /declare _manaPoint		int local 30

 	/if (((${Me.PctEndurance} < ${_endurancePoint}) || (${Me.PctMana} < ${_manaPoint})) && ${Select[${Me.CombatState},RESTING]}) {
		/call core_cast "Rallying Call" alt ${Me.ID}
	}

	:exit
/return



|***
 *	DES: Class controls
 *	USE: /brd []
 *	INI: 
 *  NOTE:
 ***|
#bind setBRD /brd
sub Bind_setBRD(string _classType, string _classVerbage)
	DEBUG ${cbug}Bind_setBRD(type:${_classType}, verbage:${_classVerbage})\ax
	/if (!${Select[${Me.Class.ShortName},BRD]}) {
		/call echo_bitch EQBC
		/goto :exit
	}
	
	| enable/disable summer line of spells
	/if (${Bool[${_classType.Find[TEST]}]}) {
		| /call Event_SetVar NA switchNukeSummer Spells ${If[${switchNukeSummer},FALSE,TRUE]}


	} else {
		/call echos listtype ${Me.Class.ShortName.Lower} ${Me.Class.ShortName.Lower}
	} 
	:exit
/return