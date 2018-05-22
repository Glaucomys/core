|***
 *	modes.inc
 *	Core Mode Specific Functions
 *
 *
 *
 ***|




|***
 *	DES: set mode type
 *	USE: /mode []
 *	INI: NA
 *  NOTE: starts/ends non-standard modes
 ***|
#bind mode /mode
sub Bind_mode(string _modeType, string _verbage)
	DEBUGMODE ${ctar}Bind_mode()\ax

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local mode

	| get help
	/if (${_modeType.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Arg[${_count},|]}]}
		/next _count
		/return			
	} else /if (${Bool[${${_bind}Type.Find[${_modeType}]}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_modeType.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_modeType.Lower}]}
		/return
	}	

	
	| set mode agro range
	/if (${Bool[${_modeType.Find[agro]}]}) {
		/if (${Range.Between[1,999999:${Int[${_verbage}]}]}) {
			/call writeINI setModeAgro Common ${_verbage}  
		} else {
			VOUT /mode agro [${cinfo}${setModeAgro}\ax] allowable range is ${cinfo}1 - 99999\ax
			/return
		}	
		
	| set harvest counts	
	} else /if (${Bool[${_modeType.Find[harvestcount]}]}) {
		/if (${Range.Between[1,999999:${Int[${_verbage}]}]}) {
			/call writeINI modeHarvestCount Common ${_verbage}  
		} else {
			VOUT /${_bind} harvestcount [${cinfo}${modeHarvestCount}\ax] allowable range is ${cinfo}1 - 999999\ax
			/return
		}	

	| all switches except stop
	} else /if (${Bool[${modeType.Find[${_modeType}]}]} && ${_modeType.NotEqual[stop]}) {
		/varset setModeType ${_modeType.Left[1].Upper}${_modeType.Right[-1].Lower}
		/if (${Defined[switchMode${setModeType}]}) {
			/varset switchMode${setModeType} ${If[${switchMode${setModeType}},FALSE,TRUE]}
			/if ((${_modeType.Equal[hunt]} || ${_modeType.Equal[scorch]}) && ${Bool[${setModeType}]}) {
				/call set_stop TRUE
				/call clear_combat
			}
		} 
		
	| clear all modes
	} else /if (${Bool[${modeType.Find[${_modeType}]}]} && ${_modeType.Equal[stop]} && ${Bool[${setModeType}]}) {
		/call switch_clear_mode
		/call set_stop TRUE
		/call clear_combat
	}
		
	| output
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[ ${switchMode${${_bind}Type.Arg[${_count},|].Left[1].Upper}${${_bind}Type.Arg[${_count},|].Right[-1].Lower}} ,${cgood},${cinfo}]}${${_bind}Type.Arg[${_count},|]}\ax
		/if (${${_bind}Type.Arg[${_count},|].Equal[agro]}) /varset _listout ${_listout}(${cnum}${setModeAgro}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[harvestcount]}) /varset _listout ${_listout}(${cnum}${modeHarvestCount}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]				

	/varset switchOverrideLos ${Ini[${INIClass},"Override","switchOverrideLos"]}
	/varset switchOverrideEngageat ${Ini[${INIClass},"Override","switchOverrideEngageat"]}
	/varset switchAutoCursor ${Ini[${INIClass},"Common","switchAutoCursor"]}

/return



|***
 *	DES: stop all non standard modes
 *	USE: /call switch_clear_mode
 *	INI: NA
 *  NOTE: 
 ***|
sub switch_clear_mode()
	DEBUGMODE ${cbug}switch_clear_mode()\ax
	/declare _count int local
	/if (${setMode}) /varset setMode FALSE

	/for _count 1 to ${modeType.Count[|]}
		/if (${switchMode${modeType.Arg[${_count},|].Left[1].Upper}${modeType.Arg[${_count},|].Right[-1].Lower}}) {
			/varset switchMode${modeType.Arg[${_count},|].Left[1].Upper}${modeType.Arg[${_count},|].Right[-1].Lower} FALSE  
		}
	/next _count	
	
	/varset switchOverrideLos ${Ini[${INIClass},"Override","switchOverrideLos"]}
	/varset switchOverrideEngageat ${Ini[${INIClass},"Override","switchOverrideEngageat"]}
	/varset switchAutoCursor ${Ini[${INIClass},"Common","switchAutoCursor"]}
	/varset switchModeTrainspellSPC FALSE
/return



|***
 *	DES: which modes do we want to do?
 *	USE: /call do_mode
 *	INI: NA
 *  NOTE: 
 ***|
sub do_mode()
	DEBUGMODE ${ctar}do_mode()\ax
	AMIDEAD

	CLEARCURSOR TRUE
	/doevents
	/if (${switchModeDrag}) /call mode_drag
	/if (${switchModeForage} && ${Me.AbilityReady[forage]}) /call mode_forage
	/if (${switchModeFish} && ${Me.AbilityReady[fishing]}) /call mode_fish
	/if (${switchModePetfarm} && !${timer_Petfarm_delay}) /call mode_petfarm
	/if (${switchModeHunt} && !${timer_wait_for_mob_to_kill}) /call mode_hunt
	/if (${switchModeScorch} && !${timer_ScorchedEarth_delay}) /call mode_scorched_earth
	/if (${switchModeHarvest} && !${timer_Harvest_delay}) /call mode_harvest
	/if (${switchModeTradeskill}) /call mode_tradeskill
	/if (${switchModeLush} && !${Me.Drunk}) /call mode_get_waisted
	/if (${switchModeDrink} && !${Me.Drunk}) /call mode_get_drunk
	/if (${switchModeTrainspell}) /call mode_trainspell
	/if (${switchModeOpen} && !${timer_mode_open}) /call mode_open

/return



|***
 *	DES: central get target and kill for hunting modes
 *	USE: /call mode_get_mob
 *	INI: 
 *  NOTE: 
 ***|
sub mode_get_mob()
	DEBUGMODE ${cbug}mode_get_mob(${Target2Kill})\ax
	:movetomob
	/doevents
	AMIDEAD

	CLEARCURSOR TRUE
	/doevents
	
	/if (!${Spawn[${Target2Kill}].ID} || ${Spawn[${Target2Kill}].Dead}) {
		DEBUG DEAD: ${Spawn[${Target2Kill}].Dead}
		/call set_stop TRUE
		/call clear_combat
		/return
	}

	/call check_watch_spawn

	| get us to the mob
	/if (${Spawn[${Target2Kill}].Distance} > ${CombatDistance}) {
		/squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
		
		| use nave for movement
		/if (${SetTieMode.Equal[nav]} && ${Bool[${Plugin[MQ2Nav]}]} && !${Navigation.Active}) {
			| /echo nav to ${Target2Kill}
			/if (${Navigation.Active}) /squelch /nav stop
			/squelch /nav id ${Target2Kill}

		| adjust nav	
		| } else /if (${SetTieMode.Equal[nav]} && ${Bool[${Plugin[MQ2Nav]}]} && ${Navigation.Active}) {
		| 	/squelch /nav id ${Target2Kill}

		| use stick for movement
		} else /if (${SetTieMode.Equal[stick]} && ${Bool[${Plugin[MQ2MoveUtils]}]} && !${Stick.Active}) {
			/squelch /stick ID ${Target2Kill} ${CombatDistance} UW
		}
		
	| at the mob, do something about it		
	} else /if (${Spawn[${Target2Kill}].Distance} <= ${CombatDistance} || ${Spawn[${Target2Kill}].LineOfSight} && ${Spawn[${Target2Kill}].Distance} <= ${MobAgro}) {
		/squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
		/target ID ${Target2Kill}
	}
					
	/if (${Spawn[${Target2Kill}].Distance} < ${MobAgro} && ${Spawn[${Target2Kill}].Distance} > 31) {
		DEBUG distance ${Spawn[${Target2Kill}].Distance} range
		/target ID ${Target2Kill}
		/if (${Me.Pet.ID} && !${Me.Pet.Target.ID}) /pet attack
		/if (${Target.ID} && ${Spawn[${Target2Kill}].LineOfSight}) /range
	} else /if (${Spawn[${Target2Kill}].Distance} < ${CombatDistance}) {
		DEBUG distance ${Spawn[${Target2Kill}].Distance} melee
		/call set_combat MELEE
		/if (${Me.Pet.ID} && !${Me.Pet.Target.ID}) /pet attack
		/if (${Target.ID} && ${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
		
		/if (${Bool[${ModeNuke}]}) {
			/if (${Me.Book[${ModeNuke}]}) /call core_cast "${ModeNuke}" ${buffGem} ${Target2Kill}
			/if (${Me.ItemReady[${ModeNuke}]}) /call core_cast "${ModeNuke}" item ${Target2Kill}
			/if (${Me.CombatAbility[${ModeNuke}]}) /call core_cast "${ModeNuke}" disc ${Target2Kill}
			/if (${Me.AltAbilityReady[${ModeNuke}]})  /call core_cast "${ModeNuke}" alt ${Target2Kill}
		}
	} 

	/if (${Spawn[${Target2Kill}].ID} && ${Spawn[${Target2Kill}].Distance} > ${CombatDistance}) /goto :movetomob	

/return



|***
 *	DES: Train non spell skills. Best to use during a duel with a pet.
 *	USE:
 *	INI: 
 *  NOTE: This is not working yet...
 ***|
sub mode_skill_train()
	DEBUGMODE ${cbug}mode_skill_train()\ax
	AMIDAD
	UPDATEHUD "Skill Train"
	:looptrainingskills
	/doevents
	/delay 10
	/if (${Me.Skill[Taunt]} < ${Skill[Taunt].SkillCap} && ${Me.AbilityReady[Taunt]}) /doability Taunt
	/if (${Me.Skill[Disarm]} < ${Skill[Disarm].SkillCap} && ${Me.AbilityReady[Disarm]}) /doability Disarm
	/if (${Me.Skill[Bash]} < ${Skill[Bash].SkillCap} && ${Me.AbilityReady[Bash]}) /doability Bash
	/if (${Me.Skill[Kick]} < ${Skill[Kick].SkillCap} && ${Me.AbilityReady[Kick]}) /doability Kick
	/if (${Me.Skill[Sneak]} < ${Skill[Sneak].SkillCap} && ${Me.AbilityReady[Sneak]}) /doability sneak
	/if (${Me.Skill[Begging]} < ${Skill[Begging].SkillCap} && ${Me.AbilityReady[Begging]}) /doability begging
	/if (${Me.Skill[Bind Wound]} < ${Skill[Bind Wound].SkillCap} && ${Me.AbilityReady[Bind Wound]} && ${Me.PctHPs} < 99*.${tmpHealOverride}) {
		/target ID ${Me.ID}
	  /delay 3s ${Target.ID} == ${Me.ID}
		/doability "Bind Wound"
	}
	/goto :looptrainingskills
	UPDATEHUD
/return



|***
 *	DES: to help your rusty dagger collection.. why else would you fish?
 *	USE: /call do_fish_mode | /mode fish
 *	INI: 
 *  NOTE: 
 ***|
sub mode_fish()
	DEBUGMODE ${cbug}mode_fish()\ax
	AMIDEAD
	UPDATEHUD "Fish"
	/if (!${Me.Standing}) /stand
	/if (${Me.ItemReady[Fisherman's Companion]} && !${Me.ItemReady[Brell's Fishin' Pole]}) {
		DEBUGMODE Pole broke. Learn to play with your things better..
		/if (${Me.Invis}) /docommand /makemevisable
		/call core_cast "Fisherman's Companion" item ${Me.ID}
		CLEARCURSOR TRUE
	}
	/if (${Me.AbilityReady[fishing]}) /doability fishing
	/delay 16
	CLEARCURSOR TRUE
	/doevents
	UPDATEHUD
/return



|***
 *	DES: to play in the dirt
 *	USE: /call mode_forage | /mode forage
 *	INI: 
 *  NOTE: 
 ***|
sub mode_forage()
	DEBUGMODE ${cbug}mode_forage()\ax
	/if (${Select[${Me.CombatState},COMBAT]}) /return
	AMIDEAD
	UPDATEHUD "Forage"
	/if (!${switchAutoCursor}) /varset switchAutoCursor TRUE
	/if (!${Me.Standing}) /stand
	/doability forage
	/call sort_cursor TRUE
	UPDATEHUD
/return



|***
 *	DES: Kill people then drag them around like trophys!!! Win Win...
 *	USE: /call mode_drag | /mode drag
 *	INI: na
 *  NOTE: 
 ***|
sub mode_drag()
	DEBUGMODE ${cbug}mode_drag()\ax
	/declare _count 		int local
	/declare _corpseID 	int local
	UPDATEHUD "Drag"
	/doevents
	AMIDEAD

	/delay 1
	/if (${SpawnCount[pccorpse radius 110]}) {
		/for _count 1 to ${SpawnCount[pccorpse radius 110]}
			AMIDEAD

			/varset _corpseID ${NearestSpawn[${_count},pccorpse radius 110].ID}
			/if (${Spawn[${_corpseID}].Distance} > 30) {
				/target id ${_corpseID}
				/delay 5 ${Target.ID} == ${_corpseID}
				/corpse
			}
		/next _count
	}
	UPDATEHUD
/return



|***
 *	DES: turns the toon into a hunter of things.
 *	USE: /call mode_hunt | /mode hunt
 *	INI: core.zone.ini [zoneshortname] -> huntmob:|MOB1|MOB2
 *  NOTE: works better in open zones. don't say i didn't wan you.
 ***|
sub mode_hunt()
	DEBUGMODE ${cbug}mode_hunt()\ax
	/declare _count int local 0
	/doevents
	AMIDEAD

	UPDATEHUD "Hunt"
	CLEARCURSOR TRUE
	
	/for _count 1 to ${HuntMobList.Count[|]}
		/if (${Spawn[npc ${HuntMobList.Arg[${_count},|]} radius ${setModeAgro} targetable].ID}) {
			/varset Target2Kill ${NearestSpawn[npc ${HuntMobList.Arg[${_count},|]}].ID}
			/call echos hunt 0 0 ${Target2Kill} 0
			/call writeINI watchSpawn Common "${Spawn[${Target2Kill}].DisplayName}"
			/goto :haveatarget
		}
	/next _count	

	:haveatarget
	/if (!${Target2Kill}) {
		/call create_timer timer_wait_for_mob_to_kill ${tHunt_delay}
		/call check_buff_routines TRUE
	}

	/if (!${Bool[${FindItemCount[${RangeItemSummon}]}]} && ${Bool[${RangeItemSummon}]}) /call check_ammo
	/if (${switchUseADVL}) {
		/if ((${coreAuto} || ${switchOverrideLoot}) && !${SafeZone} && !${switchOverrideSafezone}) /call do_loot
	}
	/if (${Spawn[${Target2Kill}].ID}) {
		/call mode_get_mob
	} else /if (${Spawn[${Target2Kill}].Dead} || !${Spawn[${Target2Kill}].ID} || !${timer_scorched_earth_fail}) {
		/if (!${timer_scorched_earth_fail}) /call create_timer timer_scorched_earth_ignore_${Target2Kill} ${tScorchedEarth_ignore} 
		DEBUG DEAD: ${Spawn[${Target2Kill}].Dead}
		/call set_stop TRUE
		/call clear_combat
	}	
		
	UPDATEHUD
/return



|***
 *	DES: Pet Farm
 *	USE: /call mode_petfarm | /mode petfarm
 *	INI: 
 *  NOTE: Use with caution. For static pet classes only.
 ***|
sub mode_petfarm()
	DEBUGMODE ${cbug}mode_petfarm()\ax
	AMIDEAD

	/declare _tmpID					int local 0
	/declare _spawnCount 		int local 0
	/declare _resetCount		int local 0
	/declare _nameloop			int local 1

	/varset switchOverrideEngageat TRUE
	/varset switchOverrideLos TRUE
	
	UPDATEHUD "Pet Farm"
	:loopfindmob
	/doevents
	/if (!${switchModePetfarm}) /return
	/if (!${Select[${Me.Class.ShortName},MAG,ENC,NEC,BST,SHD,SHM,RNG,DRU]} || (${SafeZone} && !${switchOverrideSafezone})) {
		/call echos failboat 0 0 0 0
		/call switch_clear_mode
		/return
	}
	/call check_watch_spawn
	/if ((${coreAuto} || ${switchOverrideLoot}) && !${SafeZone} && !${switchOverrideSafezone}) /call do_loot
	
	/if (${Select[${Me.Class.ShortName},MAG]} && ${switchHealPet}) /call pet_heal_MAG

	/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${MobAgro} zradius ${zradius} targetable]}) {
		/varset _spawnCount ${SpawnCount[npccorpse ${If[${switchOverrideLos},,los]} radius ${MobAgro} zradius ${zradius}]}	
		/varset Target2Kill ${Spawn[${NearestSpawn[1, ${If[${switchOverrideLos},,los]} npc radius ${MobAgro} zradius ${zradius} targetable]}].ID}

		| skip excluded mobs
		:loopnamedmob
		/if (${ExcludeList.Find[${Spawn[id ${Target2Kill}].DisplayName}]})	{
			DEBUGMODE name:${Spawn[id ${Target2Kill}].DisplayName} ID:${_tmpID} EXCLUDELIST .. loop:${_nameloop}
			/varcalc _nameloop ${_nameloop}+1
			/if (${_spawnCount} > 1) {
				/varset Target2Kill ${Spawn[${NearestSpawn[${_nameloop}, ${If[${switchOverrideLos},,los]} npc radius ${MobAgro} zradius ${zradius} targetable]}].ID}

				/goto :loopnamedmob
			} else {
				/varset Target2Kill 0
				/varset _nameloop 1
			}
			DEBUGMODE _tmpID:${_tmpID}
		}
				
		/if (!${Target2Kill}) /goto :nomob
		/varset _resetCount 0
	} else {
		:nomob
		DEBUGMODE :nomob RESET the numbers
		/doevents
		/varset _resetCount 0
		/varset _spawnCount 0
		/varset Target2Kill 0
		/varset _nameloop 1
		/if (${timer_Petfarm_delay}) /goto :loopfindmob

		/if (!${timer_Petfarm_delay}) /call create_timer timer_Petfarm_delay ${tPetfarm_delay}
		/if (${Me.Pet.Target.ID}) /pet back off
		/if (${Me.AltAbilityReady[Summon Companion]} && ${Me.Pet.Distance} > 80) /call core_cast "Summon Companion" alt 0

		/return
	}
	
	:gettarget
	DEBUGMODE :gettarget ID ${Target2Kill}
	/doevents
	/target ID ${Target2Kill}
	/if (!${ExcludeList.Find[${Spawn[id ${Target2Kill}].DisplayName}]}) {
		/if (!${Pet.Target.ID}) {
			/delay 5
			/call echos petfarm 0 ${Target2Kill} 0 0
			/pet attack
		}
	}
	/varcalc _resetCount ${_resetCount}+1
	
	/if (${_resetCount} > 30) {
		/varset _resetCount 0
		/if (${Me.AltAbilityReady[Summon Companion]}) /call core_cast "Summon Companion" alt 0
		/goto :loopfindmob
	}		
		
	:loopwaittilldead
	DEBUG :loopwaittilldead
	/delay 1s
	/doevents
	/if (${Spawn[${Target2Kill}].Type.Equal[CORPSE]} || !${Spawn[${_tmpID}].ID}) /goto :loopfindmob
	/if (${Spawn[${Target2Kill}].Distance} > ${MobAgro}) {
		/goto :loopfindmob
	}
	/if (${Me.Pet.Target.ID} != ${Target2Kill}) /goto :gettarget
	/if (${Spawn[${Target2Kill}].Type.NotEqual[CORPSE]}) /goto :loopwaittilldead

	UPDATEHUD
/return



|***
 *	DES: Scorched Earth Mode
 *	USE: /call mode_scorched_earth | /mode scorch
 *	INI: 
 *  NOTE: This will kill everything in a zone. or at least try /shrug
 ***|
sub mode_scorched_earth()
	DEBUGMODE ${cbug}mode_scorched_earth()\ax
	AMIDEAD
	/declare _spawnCount 		int local 0
	/declare _resetCount		int local 0
	/declare _nameloop			int local 1

	/varset switchOverrideEngageat TRUE
	/varset switchOverrideLos TRUE
	/call clear_combat
	
	:loopfindmob
	UPDATEHUD "Kill 'Em All"
	/doevents
	/if (!${switchModeScorch}) /return
	/if (${SafeZone} && !${switchOverrideSafezone}) {
		/call echos failboat 0 0 0 0
		/call mode_clear_all
		/return
	}
	/call check_watch_spawn
	/if ((${coreAuto} || ${switchOverrideLoot}) && !${SafeZone} && !${switchOverrideSafezone}) /call do_loot
	/call check_buff_routines TRUE
	/if (!${Bool[${FindItemCount[${RangeItemSummon}]}]} && ${Bool[${RangeItemSummon}]}) /call check_ammo
	/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${setModeAgro} targetable]}) {
		/varset _spawnCount ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${setModeAgro} targetable]}	
		/varset Target2Kill ${Spawn[${NearestSpawn[1, npc ${If[${switchOverrideLos},,los]} radius ${setModeAgro} targetable]}].ID}
		/call create_timer timer_scorched_earth_fail ${tScorchedEarth_fail}

		DEBUGMODE SpawnCount: ${_spawnCount} ${sep} Target2Kill:${Target2Kill}

		:loopignoreddmob
		
		/if (${ExcludeList.Find[${Spawn[id ${Target2Kill}].DisplayName}]})	{
			DEBUGMODE name:${Spawn[id ${Target2Kill}].DisplayName} ID:${Target2Kill} EXCLUDELIST .. loop:${_nameloop}
			/varcalc _nameloop ${_nameloop}+1
			/if (${_spawnCount} > 1) {
				/varset Target2Kill ${Spawn[${NearestSpawn[${_nameloop}, npc ${If[${switchOverrideLos},,los]} radius ${setModeAgro} targetable]}].ID}
				/goto :loopignoreddmob
			} else {
				/varset Target2Kill 0
				/varset _nameloop 1
			}
		}
		/if (!${Target2Kill}) /goto :nomob
		/varset _resetCount 0
	} else {
		:nomob
		/doevents
		/varset _resetCount 0
		/varset _spawnCount 0
		/varset Target2Kill 0
		/varset _nameloop 1
		/if (!${timer_ScorchedEarth_delay}) /call create_timer timer_ScorchedEarth_delay ${tScorchedEarth_delay}
		/if (${Me.Pet.Target.ID}) /pet back off
		/if (${Me.Pet.ID} && ${Me.AltAbilityReady[Summon Companion]} && ${Me.Pet.Distance} > 80) /call core_cast "Summon Companion" alt 0
		/return
	}
	/call echos hunt 0 0 ${Target2Kill} 0

	:gettarget
	/doevents
	
	/if (!${ExcludeList.Find[${Spawn[id ${Target2Kill}].DisplayName}]} && !${timer_scorched_earth_ignore_${Target2Kill}}) {
		/if (${Spawn[${Target2Kill}].ID}) {
			/call mode_get_mob
		} else /if (${Spawn[${Target2Kill}].Dead} || !${Spawn[${Target2Kill}].ID} || !${timer_scorched_earth_fail}) {
			/if (!${timer_scorched_earth_fail}) /call create_timer timer_scorched_earth_ignore_${Target2Kill} ${tScorchedEarth_ignore} 
			DEBUGMODE DEAD: ${Spawn[${Target2Kill}].Dead}
			/call set_stop TRUE
			/call clear_combat
		}
	}
	
	/varcalc _resetCount ${_resetCount}+1

	/if (${_resetCount} > 30) {
		/varset _resetCount 0
		/if (${Me.Pet.ID} && ${Me.AltAbilityReady[Summon Companion]} && ${Me.Pet.Distance} > 80) /call core_cast "Summon Companion" alt 0
		/goto :loopfindmob
	}		
		
	:loopwaittilldead
	/delay 1s
	/doevents
	/if (${Spawn[${Target2Kill}].Type.Equal[CORPSE]} || !${Spawn[${Target2Kill}].ID}) /goto :loopfindmob
	/if (${Spawn[${Target2Kill}].Distance} > ${MobAgro}) {
		/goto :loopfindmob
	}
	/if (${Me.Pet.Target.ID} != ${Target2Kill}) /goto :gettarget
	/if (${Spawn[${Target2Kill}].Type.NotEqual[CORPSE]}) /goto :loopwaittilldead

	UPDATEHUD
/return



|***
 *	DES: Harvest Mode
 *	USE: /call mode_Harvest | /mode harvest
 *	INI: core.zone.ini [zone]->HarvestList=
 *  NOTE: Will run around and pick up shit off the ground
 ***|
sub mode_harvest()
	DEBUGMODE ${cbug}mode_harvest()\ax
	AMIDEAD
	/declare _count int local
	UPDATEHUD "Harvest"
	/if (!${switchAutoCursor}) /varset switchAutoCursor TRUE

	CLEARCURSOR TRUE
	/for _count 1 to ${HarvestList.Count[|]}
		/if (${FindItemCount[${HarvestList.Arg[${_count},|]}]} >= (${modeHarvestCount})) {
			/varset switchModeHarvest FALSE
			/autoinventory
			/return
		}
		/if (${GroundItemCount[${HarvestList.Arg[${_count},|]}]}) {
			/call get_ground_harvest "${HarvestList.Arg[${_count},|]}"
		} else {
			/if (!${timer_Harvest_delay}) /call create_timer timer_Harvest_delay ${tHarvest_delay}
		}
	/next _count	

	UPDATEHUD
/return



|***
 *	DES: moves to the ground item and picks it up
 *	USE: /call get_ground_harvest "ITEM NAME"
 *	INI: 
 *  NOTE: 
 ***|
sub get_ground_harvest(string _tmpItem)
	DEBUGMODE ${cbug}get_ground_harvest(Harvest: ${_tmpItem})\ax
	/squelch /itemtarget ${_tmpItem}
	/delay 2s ${Bool[${Ground.ID}]}
	/if (${Bool[${Ground.ID}]}) {
		/if (${Ground.Distance3D} < ${setModeAgro}) {
			
		/if (${Bool[${Plugin[MQ2Nav]}]} && ${SetTieMode.Equal[nav]}) {	
			/if (${Navigation.MeshLoaded}) {	
				SDEBUG ${_tmpItem}: ${Ground.Y} ${Ground.X} ${Ground.Z}
				/delay 1 
				
				/if (${Navigation.PathExists[item]}) {
					/echo here nav
	        /nav item
	    	} else /if (!${Navigation.PathExists[item]}) {
	    		/echo no path
	    	}
				
				| /nav loc ${Ground.Y} ${Ground.X} ${Ground.Z}
				| /nav item
				
				| /while (${Navigation.Active} || ${Ground.Distance3D} > 19) {
				| 	/delay 1s
				| }
				
				/if (${Ground.Distance3D} < 19) /call Bind_grab ${_tmpItem}
				
				
				/squelch /nav stop
				
			} else /if (!${Navigation.MeshLoaded}) {
				OUT NO Nav Mesh for this zone. Returning to Stick.
				/call Bind_setTieControl mode stick
			}
				

		} else {
			:loopMove 
			/face nolook loc ${Ground.Y}, ${Ground.X}
			/delay 1 
			/keypress forward hold 
			/if (${Ground.Distance3D} > 19) /goto :loopMove
			/keypress forward
			/click left item
			/delay 5s (${Cursor.ID})
			/delay ${Math.Rand[10]}
			| VOUT Keep ${sep} ${cinfo}${Cursor.Name}\ax ${sep} [${ctar}${FindItemCount[${Cursor.Name}]}\ax] 
			/autoinventory
			CLEARCURSOR TRUE
			/delay 5s (!${Cursor.ID})
			/delay ${Math.Rand[10]}
		}

	}
/return



|***
 *	DES: Tradeskill combind
 *	USE: /call mode_tradeskill 
 *	INI: NA
 *  NOTE: 
 ***|
#event mode_CombineError "#*#There was no place to put that#*#"
#event mode_CombineError "#*#You cannot combine these items in this container type!#*#"
#event mode_CombineError "#*#did not accept these items#*#"
#event mode_CombineError "#*#Sorry, but you don't have everything you need for this recipe in your general inventory.#*#"
sub Event_mode_CombineError(string _line)
	/varset switchModeTradeskill FALSE
	/call echos mode "Tradeskill" switchModeTradeskil 0 0
	/call echos failboat 0 0 0 0
	/varset switchAutoCursor ${Ini[${INIClass},"Common","switchAutoCursor"]}
/return

sub mode_tradeskill()
	DEBUGMODE ${cbug}mode_tradeskill()\ax
	
	/if (!${Window[TradeskillWnd].Open}) {
		VOUT Tradeskill window must be open.
		VOUT Make All option must be deselected.
		/varset switchModeTradeskill FALSE
		/return
	}
	/if (!${switchAutoCursor}) /varset switchAutoCursor TRUE
	UPDATEHUD "Tradeskill"

	/nomodkey /notify TradeskillWnd CombineButton leftmouseup
	/delay 1.5s ${Cursor.ID}
	/doevents
	
	UPDATEHUD
/return



|***
 *	DES: Alcohol Tolerance
 *	USE: /call mode_get_waisted 
 *	INI: NA
 *  NOTE: 
 ***|
sub mode_get_waisted()
	DEBUGMODE ${cbug}mode_get_waisted()\ax
	/declare _ale 				string 	local Summoned: Ale
	/declare _aleCount		int		 	local 20	
	/declare _pole				string	local Brell's Fishin' Pole
	/declare _box					string	local Fisherman's Companion

	| no fishermans companion. end this mess
	/if (!${FindItem[=${_box}].ID}) {
		VOUT ${onotice} ${sep} No ${_box}
		VOUT This Mode uses Fisherman's Companion.
		VOUT Go get one.
		/varset switchModeLush FALSE
		/return
	}

	UPDATEHUD "Drink"
	/if (!${switchAutoCursor}) /varset switchAutoCursor TRUE
	| check max stats too ?
	
	| need booze! keep a stock on hand
	/if (${FindItemCount[${_ale}]} < ${_aleCount} && ${Me.ItemReady[${_pole}]} && !${Me.Drunk}) {
		DEBUGMODE click ${clickPoisonDispenser}
		/call core_cast "${_pole}" item ${Me.ID} FALSE
		CLEARCURSOR TRUE
	}
	
	| lets get lit
	/if (${FindItemCount[${_ale}]} && ${Me.Drunk} < ${Skill[Alcohol Tolerance].SkillCap}) {
		VOUT Drinking ${sep} ${cinfo}${_ale}\ax
		/useitem ${_ale}
	}
	UPDATEHUD FALSE
/return



|***
 *	DES: 
 *	USE: /call mode_get_drunk 
 *	INI: NA
 *  NOTE: 
 ***|
sub mode_get_drunk()
	DEBUGMODE ${cbug}mode_get_drunk()\ax
	/if (!${FindItem[${modeDrinkofChoice}].ID} || !${Bool[${modeDrinkofChoice}]}) {
		/varset switchModeDrink FALSE
		/return
	}
	UPDATEHUD "Drink"

	| start drinking.
	/while (${Me.Skill[Alcohol Tolerance]} < ${Skill[Alcohol Tolerance].SkillCap}) {
		/if (!${Me.ItemReady[${modeDrinkofChoice}]}) /break
		/if (${Me.Drunk} < 200) /useitem ${modeDrinkofChoice}
		/delay 2
		/delay 2s ${Me.Drunk} < 200
	}

	| maxed skill turn this off
	/if (${Me.Skill[Alcohol Tolerance]} == ${Skill[Alcohol Tolerance].SkillCap}) {	
		VOUT Alcohol Tolerance Maxed ${sep} (${cnum}${Me.Skill[Alcohol Tolerance]}\ax)
		/varset switchModeDrink FALSE
		/return
	}
	
	UPDATEHUD FALSE
/return



|***
 *	DES: train spells
 *	USE: /call mode_trainspell | /mode trainspell
 *	INI: 
 *  NOTE: 
 ***|
sub mode_trainspell(string _type)
	DEBUG ${cbug}mode_trainspell()\ax
	AMIDEAD
	UPDATEHUD "Train Spell"
	
	/if (${_type.Equal[specialize]} && !${switchModeTrainspellSPC}) {
		/echo specialize
		/varset switchModeTrainspellSPC TRUE
	
	}

	/declare _skill string local |alteration|abjuration|conjuration|divination|evocation
	/declare _count int local 0
	/declare _gem[5] bool local FALSE

	| figure out what we want to cast
	/for _count 1 to ${_skill.Count[|]}
		DEBUG _count:${_count}
		DEBUG skill: ${_skill.Arg[${_count},|]}(${Me.Skill[${_skill.Arg[${_count},|]}]}:${Me.SkillCap[${_skill.Arg[${_count},|]}]})
		DEBUG spell: ${${_skill.Arg[${_count},|]}}, skill: ${Spell[${${_skill.Arg[${_count},|]}}].Skill}
		
		| validate skill caps and spell. if cap is maxed, move on		
		/if (${Me.Skill[${If[${switchModeTrainspellSPC},specialize ,]}${_skill.Arg[${_count},|]}]} == ${Me.SkillCap[${If[${switchModeTrainspellSPC},specialize ,]}${_skill.Arg[${_count},|]}]}) /continue

		| if the skill spell is set to FALSE, move on
		/if (!${Bool[${_skill.Arg[${_count},|]}]}) /continue
		
		| if the spell is FALSE, move on
		/if (!${Bool[${Spell[${${_skill.Arg[${_count},|]}}].Skill}]}) /continue

		| if I already have the spell memed, move on
		/if (!${Me.Gem[${_count}].Name.Equal[${${_skill.Arg[${_count},|]}}]}) /call mem_spell TRUE "${${_skill.Arg[${_count},|]}}" ${_count} FALSE
		
		/varset _gem[${_count}] TRUE
	/next _count

	| start getting things done
	/for _count 1 to ${_skill.Count[|]}
		DEBUG gem${_count}:${_gem[${_count}]}
		/if (!${_gem[${_count}]}) /continue
		/call core_cast "${${_skill.Arg[${_count},|]}}" 0 0 FALSE
		/call sort_cursor TRUE
	/next _count
/return



|***
 *	DES: open something
 *	USE: /call mode_open | /mode open
 *	INI: 
 *  NOTE: 
 ***|
sub mode_open(string _type)
	DEBUG ${cbug}mode_trainspell()\ax
	AMIDEAD
	UPDATEHUD "Open"

	/call create_timer timer_mode_open ${tMode_Open}
	/declare _tmpID int local

	/if (${SpawnCount[${modeOpenItem} npc radius 20]}) {
		/varset _tmpID ${Spawn[${modeOpenItem} npc radius 20].ID}
		/target ID ${_tmpID}
		/if (${Target.Dead}) /return
		/delay 2s ${Target.ID} == ${_tmpID}
		/open
		/delay 1s
		/return
	} 

	UPDATEHUD FALSE
/return
