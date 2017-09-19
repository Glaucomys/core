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
	/call prep_combat
	/if (!${Macro.Return}) /return

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
		/return
		:loop
		/doevents
		AMIDEAD

		CHECKTIE
		/if (${Spawn[${Target2Kill}].Type.Equal[Corpse]}  || !${Target2Kill} || !${Target.ID} || ${Target.Type.Equal[corpse]} || !${Spawn[${Target2Kill}].ID}) /call get_assist_target
		/call check_for_adds
		/if (${addList.Count[|]} && ${switchdeBuffMezz}) /call mez_adds_BRD
		/call check_for_adds
		/if (${addList.Count[|]}) /return
		/if (${mobList.Find[|]}) /call list_update
		/call check_for_adds
		/if (${addList.Count[|]}) /return
		/call is_target_valid ${Target2Kill}
		/if (${isValidTarget}) {
			/if (!${addList.Count[|]}) /call set_combat MELEE
			/if (!${addList.Count[|]}) /call stick_to_target
			/call check_for_adds
			/if (${addList.Count[|]}) /return
			/if (!${addList.Count[|]} && ${DoNuke}) /call bard_nuke
			/call check_for_adds
			/if (${addList.Count[|]}) /return
			/if (!${addList.Count[|]}) /call pulse
		}

	}
	/if (${coreAuto}) {	
		/if (${switchHealCure}) /call cast_cure
	}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} || !${coreAuto} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Macro.Return}) /goto :killtarget
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
	/call is_target_dead
	/if (${Macro.Return}) /return
	/call stick_to_target
	/if ((${Me.AltAbilityReady[Selo's Kick]} || ${Me.AltAbilityReady[Banestrike]} || ${Me.AltAbilityReady[Cacophony]} || ${Me.AltAbilityReady[Bladed Song]} || ${Me.AltAbilityReady[Boastful Bellow]}) && ${Me.Casting.ID}) /call pulse_stop
	/if (${Me.ItemReady[Darkened Breath of Harmony]} && !${Me.Song[Doben's Spry Sonata Rk. II].ID}) /call core_cast "Darkened Breath of Harmony" item 0
		/if (${switchBanestrike}) /call cast_AA_banestrike ${Target2Kill}
	/if (${Me.AltAbilityReady[Selo's Kick]}) /call core_cast "Selo's Kick" alt ${Target2Kill}
	/if (${switchPcSwarm}) /call cast_servant_swarm ${Target2Kill}
	/if (${Me.AltAbilityReady[Cacophony]}) /call core_cast "Cacophony" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[Bladed Song]}) /call core_cast "Bladed Song" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[Boastful Bellow]}) /call core_cast "Boastful Bellow" alt ${Target2Kill}
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
	/call is_target_dead
	/if (${Macro.Return}) /return
	
	/if (${Spawn[${_tmpID}].LineOfSight} && ${Target.PctHPs} > 10 && ${Spawn[${_tmpID}].Distance} < ${MobAgro}) {
		/pet swarm
		/if (${Me.AltAbilityReady[${ServantNukeAA1}]}) {
			/call core_cast "${ServantNukeAA1}" alt ${_tmpID}
			/return
		} else /if (${Me.AltAbilityReady[${ServantNukeAA2}]}) {
			/call core_cast "${ServantNukeAA2}" alt ${_tmpID}
			/return
		} else /if (${Me.SpellReady[${songServantNuke}]}) {
			/call core_sing ${Me.Gem[${songServantNuke}]}
			/return
		}
	}
/return



|***
 *	DES: memorize songs
 *	USE: /call mem_song_line []
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
			/call scribe "${_currentSong}" "${_index}" FALSE
		/next _index	
	/next _count	
	VOUT Line ${sep} ${cinfo}${lineCurrent}\ax scribe completed.
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
			AMIDEAD
			CHECKEXIT
			/doevents
			/call is_target_dead
			/if (${Macro.Return}) /return
			/call stick_to_target
			| /if (${Me.Casting.ID}) /stopcast
			/call pulse_valid ${_index}
			/if (${Macro.Return}) /call core_sing ${_index}
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
	/declare _pulseSong			bool local TRUE
	/if (${Me.CurrentMana} < ${Spell[${Me.Gem[${_gem}].Name}].Mana}) /varset _pulseSong FALSE
	/if (${Me.Buff[${Me.Gem[${_gem}].Name}].Duration.Seconds} > 6) /varset _pulseSong FALSE
	/if (${Me.Song[${Me.Gem[${_gem}].Name}].Duration.Seconds} > 6) /varset _pulseSong FALSE
	/if (!${Me.SpellReady[${Me.Gem[${_gem}].Name}]}) /varset _pulseSong FALSE
	/if (!${Spell[${Me.Gem[${_gem}].Name}].Stacks}) /varset _pulseSong FALSE
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
	/while (${Me.Casting.ID}) {
		/delay 1
		/stopcast
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
	/call fix_gem_id "${_spellgem}"
	/declare _gem			int local ${Macro.Return}
	/if (${Me.CurrentMana} < ${Spell[${Me.Gem[${_gem}].Name}].Mana}) /return	
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
		/call is_target_dead
		/if (${Macro.Return}) /return
		/if (${Spawn[${Target2Kill}].LineOfSight} && ${Me.SpellReady[${Nuke${_count}}]} && ${Spawn[${Target2Kill}].Distance} <= ${Spell[${Nuke${_count}}].MyRange} && ${Me.CurrentMana} >= ${Spell[${Nuke${_count}}].Mana}) {
			/call stick_to_target
			/call core_sing ${Me.Gem[${Nuke${_count}}]}
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
	  NOUT ${owarning} IMMUNE TO\ax ${csp}${spellMez}\ax ${sep} [${ctar}${Target.CleanName}\ax]
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
 *  AA - Rallying Call
 *  Song - Rallying Call Azia V
 ***|
sub cast_AA_Rallying()
 	DEBUGCAST ${cbug}cast_AA_Rallying(${Me.AltAbilityReady[Rallying Call]})\ax
	AMIDEAD
	/doevents
 	/if (${SafeZone} && !${switchOverrideSafezone} || !${Me.AltAbilityReady[Rallying Call]}) /return
 	/declare _endurancePoint		int local 30
 	/declare _manaPoint		int local 30

 	/if (((${Me.PctEndurance} < ${_endurancePoint}) || (${Me.PctMana} < ${_manaPoint})) && ${Select[${Me.CombatState},RESTING]}) {
		/call core_cast "Rallying Call" alt ${Me.ID}
	}
/return



 |***
 *	DES: Bard burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_BRD()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	
 	/if (${Me.AltAbilityReady[Bladed Song]}) /call core_cast "Bladed Song" alt 0
 	/if (${Me.CombatAbilityReady[Thousand Blades]}) /call core_cast "Thousand Blades" disc 0	
	/if (${Me.AltAbilityReady[Fierce Eye]}) /call core_cast "Fierce Eye" alt 0
	/call cast_AA_spire
	/if (${Me.AltAbilityReady[Funeral Dirge]}) /call core_cast "Funeral Dirge" alt 0
	/if (${Me.AltAbilityReady[Quick Time]} && !${Me.Song[Ruaabri's Fury].ID}) /call core_cast "Quick Time" alt 0

	/call use_glyph
	/call synergy_click_Circle
	/if (${doclickItemBurn}) /call click_item_burn	
/return



|***
 *	DES: Class controls
 *	USE: /brd []
 *	INI: 
 *  NOTE:
 ***|
#bind setBRD /brd
sub Bind_setBRD(string _classType, string _classVerbage)
	SDEBUG ${cbug}Bind_setBRD(type:${_classType}, verbage:${_classVerbage})\ax
	/if (!${Select[${Me.Class.ShortName},BRD]}) {
		/call echo_bitch EQBC
		/return
	}
	/declare _count	int local 0
	
	| enable/disable sets bard song line
	/if (${Bool[${_classType.Equal[line]}]}) {

		/if (${Bool[${lineType.Find[${_classVerbage.Lower}]}]}) {
			SDEBUG ${lineType.Find[${_classVerbage.Lower}]}
			/call writeINI lineCurrent Song ${_classVerbage.Left[1].Upper}${_classVerbage.Right[-1].Lower}
			/delay 2
			/call mem_song_line ${lineCurrent}
		}
		/declare _listout 	string 	local
		/declare _sep 			bool 		local
		/for _count 1 to ${lineType.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${lineCurrent.Equal[${lineType.Arg[${_count},|]}]},${cgood},${cinfo}]}${lineType.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /brd line [${_listout}]	


	| spire control
	} else /if (${Bool[${_classType.Equal[spiregroup]}]} || ${Bool[${_classType.Equal[spireraid]}]}) {
		/declare _spiretype string local ${_classType.Left[5].Lower}${_classType.Mid[6,1].Upper}${_classType.Right[-6].Lower}
		/if (${Range.Between[0,3:${Int[${_classVerbage}]}]}) {
			/call writeINI ${_spiretype} Buffs ${_classVerbage}
		} 
		/declare _listout 	string 	local
		/declare _sep 			bool 		local
		/for _count 1 to ${typeSpire.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${${_spiretype}} == ${typeSpire.Arg[${_count},|]},${cgood},${cinfo}]}${typeSpire.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /${Me.Class.ShortName.Lower} ${_classType.Lower} [${_listout}]	


	} else {
		/call echos listtype ${Me.Class.ShortName.Lower} ${Me.Class.ShortName.Lower} 0 0
	} 
/return