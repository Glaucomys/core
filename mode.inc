|***
 *	modes.inc
 *	Core Mode Specific Functions
 *
 *
 *
 ***|




|***
 *	DES: set mode type
 *	USE: /mode [fish|forage|drag|petfarm|scorch|hunt]
 *	INI: NA
 *  NOTE: starts/ends non-standard modes
 ***|
#bind mode /mode
sub Bind_mode(string _modeType)
	DEBUG ${cbug}Bind_mode(${_modeType})\ax
	/if (${Bool[${modeTypes.Find[${_modeType}]}]} && ${_modeType.NotEqual[stop]}) {
		/varset setModeType ${_modeType.Left[1].Upper}${_modeType.Right[-1].Lower}
		/if (${Defined[setMode${setModeType}]}) {
			/varset setMode${setModeType} ${If[${setMode${setModeType}},FALSE,TRUE]}
			/call echos switch "Mode ${cinfo}${setModeType}\ax" setMode${setModeType}
			/if (${setModeType.Equal[hunt]}) /call echos hunter
		} else {
			/goto :namefail
		}
	} else /if (${Bool[${modeTypes.Find[${_modeType}]}]} && ${_modeType.Equal[stop]} && ${Bool[${setModeType}]}) {
		/call mode_clear_all
		/call set_stop TRUE
		/call clear_combat
	} else /if (!${Bool[${modeTypes.Find[${_modeType}]}]} && ${_modeType.NotEqual[stop]}) {
		:namefail
		VOUT ${onotice} ${sep} Invalid Non-Standard Mode Type
		VOUT /mode [${cinfo}${modeTypes}\ax]
	}
/return



|***
 *	DES: will force some modes to only work in a specified range
 *	USE: /modeagro [##]
 *	INI: 
 *  NOTE:
 ***| 	
#bind setModeAgro /setmodeagro
sub Bind_setModeAgro(int _ModeAgro)
	/if (${Range.Between[1,9999:${_ModeAgro}]}) {
		/call Event_SetVar NA setModeAgro Common ${_ModeAgro} 
		/call echos number "Mode Agro Range" ${_ModeAgro}
	} else /if (${_ModeAgro} == 0) {
		/call Event_SetVar NA setModeAgro Common FALSE
		/call echos number "Mode Agro Range" ZONE
	} else {
		NOUT /setmodeagro [${cinfo}##\ax] allowable range is ${cinfo}1 - 9999\ax
		NOUT /setmodeagro [${cinfo}NULL|0\ax] for the whole zone.
	}
/return



|***
 *	DES: stop all non standard modes
 *	USE: /call mode_clear_all
 *	INI: NA
 *  NOTE: 
 ***|
sub mode_clear_all()
	/if (${setMode}) /varset setMode FALSE
	/if (${setModeDrag}) {
		/varset setModeDrag FALSE 
		/call echos switch "Mode ${cinfo}Drag\ax" setModeDrag
	}
	/if (${setModeForage}) {
		/varset setModeForage FALSE 
		/call echos switch "Mode ${cinfo}Forage\ax" setModeForage
	}
	/if (${setModeFish}) {
		/varset setModeFish FALSE 
		/call echos switch "Mode ${cinfo}Fish\ax" setModeFish
	}
	/if (${setModeSkill}) {
		/varset setModeSkill FALSE 
		/call echos switch "Mode ${cinfo}Skills\ax" setModeSkill
	}
	/if (${setModePetfarm}) {
		/varset setModePetfarm FALSE 
		/call echos switch "Mode ${cinfo}Pet Farm\ax" setModePetfarm
		/if (${ignoreEngageAt}) /varset ignoreEngageAt FALSE
		/if (${setIgnoreLOS} && !${Select[${Me.Class.ShortName},ROG]}) /varset setIgnoreLOS FALSE
	}
	/if (${setModeScorch}) {
		/varset setModeScorch FALSE 
		/call echos switch "Mode ${cinfo}Scorched Earth\ax" setModeScorch
		/if (${ignoreEngageAt}) /varset ignoreEngageAt FALSE
		/if (${setIgnoreLOS} && !${Select[${Me.Class.ShortName},ROG]}) /varset setIgnoreLOS FALSE
	}
	/if (${setModeHunt}) {
		/varset setModeHunt FALSE
		/call echos switch "Mode ${cinfo}Hunt\ax" setModeHunt
	}
/return



|***
 *	DES: which modes do we want to do?
 *	USE: /call do_mode
 *	INI: NA
 *  NOTE: 
 ***|
sub do_mode()
	DEBUG ${cbug}do_mode()\ax
	ISDEAD
	CLEARCURSOR TRUE
	/doevents
	/if (${setModeDrag}) /call mode_drag
	/if (${setModeForage} && ${Me.AbilityReady[forage]}) /call mode_forage
	/if (${setModeFish} && ${Me.AbilityReady[fish]}) /call mode_fish
	/if (${setModePetfarm} && !${timer_Petfarm_delay}) /call mode_petfarm
	/if (${setModeHunt} && !${timer_wait_for_mob_to_kill}) /call mode_hunt
	/if (${setModeScorch} && !${timer_ScorchedEarth_delay}) /call mode_scorched_earth
	:exit
/return



|***
 *	DES: Train non spell skills. Best to use during a duel with a pet.
 *	USE:
 *	INI: 
 *  NOTE: This is not working yet...
 ***|
sub mode_skill_train()
	DEBUG ${cbug}mode_skill_train()\ax
	UPDATEHUD "TRAINING"
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
	:exit
	UPDATEHUD
/return



|***
 *	DES: to help your rusty dagger collection.. why else would you fish?
 *	USE: /call do_fish_mode | /fish
 *	INI: 
 *  NOTE: 
 ***|
sub mode_fish()
	DEBUG ${cbug}mode_fish()\ax
	UPDATEHUD "Fish"
	/if (!${Me.Standing}) /stand
	/if (!${Me.ItemReady[Brell's Fishin' Pole]}) {
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
	DEBUG ${cbug}mode_forage()\ax
	UPDATEHUD "Forage"
	CLEARCURSOR TRUE
	/if (!${Me.Standing}) /stand
	/doability forage
	CLEARCURSOR TRUE
	/doevents
	UPDATEHUD
/return



|***
 *	DES: Kill people then drag them around like trophys!!! Win Win...
 *	USE: /call set_drag_mode or /drag
 *	INI: na
 *  NOTE: 
 ***|
sub mode_drag()
	DEBUG ${cbug}mode_drag()\ax
	/declare _count 		int local
	/declare _corpseID 	int local
	UPDATEHUD "Drag Corpse"
	/doevents
	ISDEAD
	/delay 1
	/if (${SpawnCount[pccorpse radius 110]}) {
		/for _count 1 to ${SpawnCount[pccorpse radius 110]}
			ISDEAD
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
 *	USE: /hunt
 *	INI: core.zone.ini [zoneshortname] -> huntmob:|MOB1|MOB2
 *  NOTE: works better in open zones. don't say i didn't wan you.
 ***|
sub mode_hunt()
	DEBUG ${cbug}mode_hunt()\ax
	/declare _count 									int 	local 0
	/doevents
	ISDEAD
	UPDATEHUD "Hunt"
	CLEARCURSOR TRUE
	
	/for _count 1 to ${HuntMobList.Count[|]}
		/if (${Spawn[npc ${HuntMobList.Arg[${_count},|]}].ID} ${If[${setModeAgro},radius ${setModeAgro},]}) {
			/varset Target2Kill ${NearestSpawn[npc ${HuntMobList.Arg[${_count},|]}].ID}
			| /call echos hunt 0 0 ${Target2Kill}
			/call Event_SetVar NA watchSpawn Common "${Spawn[${Target2Kill}].DisplayName}"
			/goto :haveatarget
		}
	/next _count	

	/if (!${Target2Kill}) {
		/call create_timer timer_wait_for_mob_to_kill ${tHunt_delay}
		/call check_engaged
		/if (!${Bool[${Macro.Return}]}) {
			/if (${BuffItem} && !${timer_check_buffs_item}) /call check_buffs_item
			/if (${BuffSelf} && ${Bool[${SelfBuffCount}]} && !${timer_check_buffs_self}) /call check_buffs_self
			/if (${BuffPet} && ${Me.Pet.ID} && !${timer_check_pet_buffs}) /call check_pet_buffs
			/if ((${Bool[${Aura1Spell}]} || ${Bool[${Aura2Spell}]}) && !${timer_check_aura}) /call check_aura
		}
	}
	:haveatarget
	
	/if (${Spawn[${Target2Kill}].ID} && ${Spawn[${Target2Kill}].Distance} > ${CombatDistance}) {
		:movetomob
		/if (!${Spawn[${Target2Kill}].ID} || ${Spawn[${Target2Kill}].Dead}) /goto :hunttargetdead
		/doevents
		ISDEAD	
		/call check_watch_spawn
		/if (!${Stick.Active} && ${Spawn[${Target2Kill}].Distance} > ${CombatDistance}) {
			/squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
			/squelch /stick ID ${Target2Kill} ${CombatDistance} UW
		}
		CLEARCURSOR TRUE
		/doevents
		/delay 1		
		
		/if (${Spawn[${Target2Kill}].ID}) {

			/if (${Spawn[${Target2Kill}].Distance} < ${MobAgro} && ${Spawn[${Target2Kill}].Distance} > 31) {
				DEBUG distance ${Spawn[${Target2Kill}].Distance} range
				/target ID ${Target2Kill}
				| /delay 1s ${Target.ID} == ${Target2Kill}
				/if (${Me.Pet.ID}) /pet attack
				/if (${Target.ID} && ${Spawn[${Target2Kill}].LineOfSight}) /range
			} else /if (${Spawn[${Target2Kill}].Distance} < ${CombatDistance}) {
				DEBUG distance ${Spawn[${Target2Kill}].Distance} melee
				| /call set_combat MELEE
				/target ID ${Target2Kill}
				/if (${Select[${Me.Class.ShortName},WAR,MON,SHD,BST,BER,RNG,BRD]}) {
					/varset combat MELEE
					/call core_${Me.Class.ShortName}
				}
				/if (${Me.Pet.ID}) /pet attack
				/if (${Select[${Me.Class.ShortName},WIZ]}) /call core_cast "${HuntNuke}" 0 ${Target2Kill}
				/if (${Select[${Me.Class.ShortName},CLR]}) /call core_cast "${tempPet}" ${buffGem} ${Target2Kill}
			} 
			/if (${Spawn[${Target2Kill}].ID}) /goto :movetomob
		} else /if (${Spawn[${Target2Kill}].Dead} || !${Spawn[${Target2Kill}].ID}) {
			:hunttargetdead
			DEBUG DEAD: ${Spawn[${Target2Kill}].Dead}
			/call set_stop TRUE
			/call clear_combat
		}
	}
		
	:exit
	UPDATEHUD
/return



|***
 *	DES: Pet Farm Mode
 *	USE: /call do_pet_farm_mode | /petfarm
 *	INI: 
 *  NOTE: Use with caution. For static pet classes only.
 ***|
sub mode_petfarm()
	DEBUG ${cbug}mode_petfarm()\ax

	/declare _tmpID					int local 0
	/declare _spawnCount 		int local 0
	/declare _resetCount		int local 0
	/declare _nameloop			int local 1

	/varset ignoreEngageAt TRUE
	/varset setIgnoreLOS TRUE
	
	UPDATEHUD "Pet Farm"
	:loopfindmob
	/doevents
	/if (!${setModePetfarm}) /goto :exit
	/if (!${Select[${Me.Class.ShortName},MAG,ENC,NEC,BST,SHD,SHM,RNG,DRU]} || ${SafeZone}) {
		/call echos failboat
		/call mode_clear_all
		/goto :exit
	}
	/call check_watch_spawn

	/if (${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${MobAgro} zradius ${zradius} targetable]}) {
		/varset _spawnCount ${SpawnCount[npccorpse ${If[${setIgnoreLOS},,los]} radius ${MobAgro} zradius ${zradius}]}	
		/varset Target2Kill ${Spawn[${NearestSpawn[1, ${If[${setIgnoreLOS},,los]} npc radius ${MobAgro} zradius ${zradius} targetable]}].ID}

		| skip excluded mobs
		:loopnamedmob
		/if (${ExcludeList.Find[${Spawn[id ${Target2Kill}].DisplayName}]})	{
			DEBUG name:${Spawn[id ${Target2Kill}].DisplayName} ID:${_tmpID} EXCLUDELIST .. loop:${_nameloop}
			/varcalc _nameloop ${_nameloop}+1
			/if (${_spawnCount} > 1) {
				/varset Target2Kill ${Spawn[${NearestSpawn[${_nameloop}, ${If[${setIgnoreLOS},,los]} npc radius ${MobAgro} zradius ${zradius} targetable]}].ID}

				/goto :loopnamedmob
			} else {
				/varset Target2Kill 0
				/varset _nameloop 1
			}
			DEBUG _tmpID:${_tmpID}
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
		/if (${timer_Petfarm_delay}) /goto :loopfindmob

		/if (!${timer_Petfarm_delay}) /call create_timer timer_Petfarm_delay ${tPetfarm_delay}
		/if (${Me.Pet.Target.ID}) /pet back off
		/if (${Me.AltAbilityReady[Summon Companion]} && ${Me.Pet.Distance} > 80) /call core_cast "Summon Companion" alt 0

		/goto :exit
	}
	
	:gettarget
	/doevents
	/target ID ${Target2Kill}
	/if (!${ExcludeList.Find[${Spawn[id ${Target2Kill}].DisplayName}]}) {
		/if (!${Pet.Target.ID}) {
			/delay 5
			/call echos petfarm 0 ${Target2Kill}
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
	/delay 1s
	/doevents
	/if (${Spawn[${Target2Kill}].Type.Equal[CORPSE]} || !${Spawn[${_tmpID}].ID}) /goto :loopfindmob
	/if (${Spawn[${Target2Kill}].Distance} > ${MobAgro}) {
		/goto :loopfindmob
	}
	/if (${Me.Pet.Target.ID} != ${Target2Kill}) /goto :gettarget
	/if (${Spawn[${Target2Kill}].Type.NotEqual[CORPSE]}) /goto :loopwaittilldead

	:exit
	UPDATEHUD
/return



|***
 *	DES: Scorched Earth Mode
 *	USE: /call mode_scorched_earth 
 *	INI: 
 *  NOTE: This will kill everything in a zone
 ***|
sub mode_scorched_earth()
	DEBUG ${cbug}mode_scorched_earth()\ax
	/declare _spawnCount 		int local 0
	/declare _resetCount		int local 0
	/declare _nameloop			int local 1

	/varset ignoreEngageAt TRUE
	/varset setIgnoreLOS TRUE
	/call clear_combat
	
	:loopfindmob
	UPDATEHUD "Scorched Earth"
	/doevents
	/if (!${setModeScorch}) /goto :exit
	/if (${SafeZone}) {
		/call echos failboat
		/call mode_clear_all
		/goto :exit
	}
	/call check_watch_spawn

	/if (${SpawnCount[npc ${If[${setIgnoreLOS},,los]} ${If[${setModeAgro},radius ${setModeAgro},]} targetable]}) {
		/varset _spawnCount ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} ${If[${setModeAgro},radius ${setModeAgro},]} targetable]}	
		/varset Target2Kill ${Spawn[${NearestSpawn[1, npc ${If[${setIgnoreLOS},,los]} ${If[${setModeAgro},radius ${setModeAgro},]} targetable]}].ID}
		/call create_timer timer_scorched_earth_fail ${tScorchedEarth_fail}

		DEBUG SpawnCount: ${_spawnCount} ${sep} Target2Kill:${Target2Kill}

		:loopignoreddmob
		
		/if (${ExcludeList.Find[${Spawn[id ${Target2Kill}].DisplayName}]})	{
			DEBUG name:${Spawn[id ${Target2Kill}].DisplayName} ID:${Target2Kill} EXCLUDELIST .. loop:${_nameloop}
			/varcalc _nameloop ${_nameloop}+1
			/if (${_spawnCount} > 1) {
				/varset Target2Kill ${Spawn[${NearestSpawn[${_nameloop}, npc ${If[${setIgnoreLOS},,los]} ${If[${setModeAgro},radius ${setModeAgro},]} targetable]}].ID}
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
		/goto :exit
	}
	
	VOUT Kill ${sep} [${ctar}${Spawn[id ${Target2Kill}].DisplayName}\ax] ${sep} ${If[${Spawn[${Target}].LineOfSight},${cgood}${Target.Distance}\ax,${cbad}${Spawn[id ${Target2Kill}].Distance}\ax]}

	:gettarget
	/doevents
	
	/if (!${ExcludeList.Find[${Spawn[id ${Target2Kill}].DisplayName}]} && !${timer_scorched_earth_ignore_${Target2Kill}}) {
		/if (${Spawn[${Target2Kill}].ID}) {
			:movetomob
			/if (!${Spawn[${Target2Kill}].ID} || ${Spawn[${Target2Kill}].Dead}) /goto :hunttargetdead
			/doevents
			ISDEAD
			CLEARCURSOR TRUE
			/doevents
				
			/call check_watch_spawn
			/if (!${Stick.Active} && ${Spawn[${Target2Kill}].Distance} > ${CombatDistance}) {
				/squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
				/squelch /stick ID ${Target2Kill} ${CombatDistance} UW
			} else /if (${Spawn[${Target2Kill}].Distance} <= ${CombatDistance}) {
				/squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
				/target ID ${Target2Kill}
				/goto :killmob
			}
					
			/if (${Spawn[${Target2Kill}].Distance} < ${MobAgro} && ${Spawn[${Target2Kill}].Distance} > 31) {
				DEBUG distance ${Spawn[${Target2Kill}].Distance} range
				/target ID ${Target2Kill}
				/if (${Me.Pet.ID} && !${Me.Pet.Target.ID}) /pet attack
				/if (${Target.ID} && ${Spawn[${Target2Kill}].LineOfSight}) /range
			} else /if (${Spawn[${Target2Kill}].Distance} < ${CombatDistance}) {
				:killmob
				DEBUG distance ${Spawn[${Target2Kill}].Distance} melee
				/call set_combat MELEE
				/if (${Me.Pet.ID} && !${Me.Pet.Target.ID} && ${Me.Pet.Distance}) /pet attack
				/if (${Select[${Me.Class.ShortName},WIZ]}) /call core_cast "${HuntNuke}" 0 ${Target2Kill}
				/if (${Select[${Me.Class.ShortName},CLR]}) /call core_cast "${tempPet}" ${buffGem} ${Target2Kill}
			} 
			/if (${Spawn[${Target2Kill}].ID} && ${Spawn[${Target2Kill}].Distance} > ${CombatDistance}) /goto :movetomob
		} else /if (${Spawn[${Target2Kill}].Dead} || !${Spawn[${Target2Kill}].ID} || !${timer_scorched_earth_fail}) {
			/if (!${timer_scorched_earth_fail}) /call create_timer timer_scorched_earth_ignore_${Target2Kill} ${tScorchedEarth_ignore} 
				:hunttargetdead
				DEBUG DEAD: ${Spawn[${Target2Kill}].Dead}
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

	:exit
	UPDATEHUD
/return
