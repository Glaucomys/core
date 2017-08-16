|***
 *	class.shm.inc
 *	Core Shaman Include
 *
 *  /echo ${Target.CanSplashLand}
 ***|



|***
 *	DES: Main Shaman Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_SHM()
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}(${cinfo}${version${Me.Class.ShortName}}\ax${cbug})\ax
	/doevents
	ISDEAD
	/if (${Bool[${toonControl}]} && ${Bool[${toonControlXTslot}]} && ${toonControlProtect}) /call protect_always_xt
	CHECKTIE

  /if (!${timer_set_xtarget} && ${switchHealXTarget}) {
    /call set_xtarget
    /call create_timer timer_set_xtarget ${tXHeal}
  }	

	/if (${coreAuto} && !${Me.Invis}) {
		/call check_cani
	}	
	
	
	:killtarget
	/call prep_combat
	/if (!${Bool[${Macro.Return}]}) /goto :exit

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},HEAL]}) {
		/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
		/if (${switchHealXTarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/if (${switchdeBuffMalo}) /call cast_malo_tash ${Target.ID}
		/if (${switchdeBuffCripple}) /call cast_cripple_${Me.Class.ShortName} ${Target.ID}
		/if (${switchdeBuffSlow}) /call cast_AA_slow ${Target2Kill} "Turgur's Swarm"
		/call disc_${Me.Class.ShortName}
		/call is_target_valid ${Target2Kill}
		/if (${isValidTarget}) {
			/if (${DoDoT}) /call cast_DOT
			/if (${DoNuke}) /call cast_nuke
			/if (${doclickItem}) /call doclickItem
			/call send_pet
			/call clickDPS_${Me.Class.ShortName}
			/call disc_${Me.Class.ShortName}
		}
		/if (${switchPcPet} && !${Me.Pet.ID}) /call check_pet
	} else /if (${Select[${combat},DUAL]}) {
	}
	/if (${coreAuto}) {	
		/if (${switchHealCure}) /call cast_cure
	}
	/call check_cani
	/call use_mod_rod
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
sub disc_SHM()
	DEBUGCLASS	${cbug}disc_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	CHECKEXIT
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}

	:exit
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub clickDPS_SHM()
	DEBUGCLASS ${cbug}clickDPS.${Me.Class.ShortName}()\ax 
	ISDEAD
	/doevents
	CHECKEXIT
	/call click_horn_of_unity 	

	/if (${Me.ItemReady[${Epic}]} && !${Me.Song[Prophet's Gift of the Ruchu].ID} && ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${MobAgro} playerstate 4 targetable]} >= 3) {
		/call core_cast "${Epic}" item
		/goto :exit
	}
	
	:exit
/return



|***
 *	DES: AA Slow
 *	USE: auto
 *	INI: 
 *  NOTE: 

sub cast_AA_slow_SHM(int _tmpID)
	DEBUGCLASS ${cbug}cast_AA_slow_SHM(${_tmpID})\ax
	ISDEAD
	/doevents
	/call check_exit TRUE
	/if (${Bool[${Macro.Return}]} || ${timer_slow_${_tmpID}}) /goto :exit

	| check for overwrite slows
	/declare _tmpSlow string local ${Target.Slowed}
	/if (${Target.Slowed.Duration})
		/if (${_tmpSlow.Find[Undermining]}) /call create_timer timer_slow_${_tmpID} ${Target.Slowed.Duration.TotalSeconds}s
		/goto :exit
	}

	/declare _resistCount int local 0
	:loopslow
	ISDEAD
	/if (${Spawn[${_tmpID}].Type.NotEqual[PC]} && ${Spawn[${_tmpID}].Distance} <= ${Spell[Turgur's Swarm].MyRange}) {
		/delay 2.25s ${Me.AltAbilityReady[Turgur's Swarm]}
		/call core_cast "Turgur's Swarm" alt ${Target2Kill}
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_slow_${_tmpID} ${Spell[Turgur's Swarm].Duration.TotalSeconds}s
		} else /if (${Select[${castReturn},CAST_RESISTED,CAST_RESIST,CAST_RESIST]}) {
			/varcalc _resistCount ${_resistCount}+1
			/if (${_resistCount} >= 3) {
				/call create_timer timer_slow_${_tmpID} ${Spell[Turgur's Swarm].Duration.TotalSeconds}m
			} else {
				/goto :loopslow
			}
		}
	}
	:exit

/return
 ***|


|***
 *	DES: We want Mana
 *	USE: /cani #
 *	INI: 
 *  NOTE: 
 ***|
sub check_cani()
	DEBUGCLASS ${cbug}check_cani()\ax
	ISDEAD
	/doevents
	CHECKEXIT
	
	/if (${Me.PctMana} < ${CaniPercent} && ${Me.SpellReady[${spellCannibalization}]}) {
		/call core_cast "${spellCannibalization}" 0 0
	}
	/if (${Me.PctMana} < ${CaniPercent} && ${Me.AltAbilityReady[Cannibalization]}) {
		/call core_cast "Cannibalization" alt 0
	}
	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub check_heal_group_SHM()
	DEBUGCLASS ${cbug}checkHealGroup.SHM()\ax
	CHECKEXIT

	/if (${Bool[${Me.Song[Touch of the Divine]}]}) /goto :exit
	/declare _count	int	local

	:loopheal
	ISDEAD
	/doevents
	CHECKTIE
	/varset WorstHurt 				${Me.ID}
	/varset WorstHP 					${If[${HealOverride},${Float[${HealOverride}].Centi},99.99]}
	/varset WorstClass 				SHM
	/varset MinorHurt 				0
	/varset MajorHurt					0
	

	| Does the control toon need healing?
	/if (!${Bool[${Group.Member[${toonControl}]}]} && ${Bool[${toonControl}]} && ${toonControlProtect} && !${Spawn[${toonControl}].Dead} && ${Bool[${Spawn[${toonControl}]}]} && ${Spawn[${toonControl}].ID} && ${Spawn[${toonControl}].PctHPs} <= ${WorstHP} && (${Spawn[${toonControl}].Distance} <= ${Spell[${healRemedy1}].MyRange} && ${Spawn[${toonControl}].PctHPs} <= ${${Spawn[${toonControl}].Class.ShortName}Point}*.${tmpHealOverride} || ${Spawn[${toonControl}].Distance} <= ${Spell[${healRenewal1}].MyRange} && ${Spawn[${toonControl}].PctHPs} <= ${${Spawn[${toonControl}].Class.ShortName}Point}*.${tmpHealOverride}*.7 )) {
		/varset WorstHurt ${Spawn[${toonControl}].ID}
		/varset WorstHP ${Spawn[${toonControl}].PctHPs}
		/varset WorstClass ${Spawn[${toonControl}].Class.ShortName}
	} 
	
	/for _count 0 to ${Group.Members}
		ISDEAD
		/if (!${Group.Member[${_count}].Dead} && (${Spawn[pc id ${Group.Member[${_count}].ID}].ID} || ${Spawn[mercenary id ${Group.Member[${_count}].ID}].ID}) && ${Group.Member[${_count}].PctHPs} <= ${WorstHp} && (${Group.Member[${_count}].Distance} <= ${Spell[${healMain}].MyRange} && ${Group.Member[${_count}].PctHPs} <= ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride} || ${Group.Member[${_count}].Distance} <= ${Spell[${healMain}].MyRange} && ${Group.Member[${_count}].PctHPs} <= ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride})) {
			/varset WorstHurt ${Group.Member[${_count}].ID}
			/varset WorstHp ${Group.Member[${_count}].PctHPs}
			/varset WorstClass ${Group.Member[${_count}].Class.ShortName}
		} 
    /if (!${Spawn[pc id ${Group.Member[${_count}].ID}].Dead} && (${Group.Member[${_count}].PctHPs}<85 || ${Group.Member[${_count}].PctHPs} < ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}) && ${Group.Member[${_count}].PctHPs} > ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}*.7 && ${Group.Member[${_count}].Distance} <= ${Spell[${healWholeGroup}].AERange}) {
      /varcalc MinorHurt ${MinorHurt}+1
    } else /if (!${Spawn[pc id ${Group.Member[${_count}].ID}].Dead} && ${Group.Member[${_count}].PctHPs}<${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}*.7 && ${Group.Member[${_count}].Type.NotEqual[Corpse]} && ${Group.Member[${_count}].Distance} <= ${Spell[${healWholeGroup}].AERange}) {
      /varcalc MajorHurt ${MajorHurt}+1
    }
    /doevents
  /next _count

	/if (${SpawnCount[npc radius ${MobAgro} playerstate 4 targetable]} >= 1) /call cast_surge_heal
	
	| Group regenerations. Use all the time. Don't care if hurt or not.
 	/if (${useGroupHoT} && ${SpawnCount[npc radius ${MobAgro} playerstate 4 targetable]} >= 1) {
 	 	/if (${Me.SpellReady[${healWholeGroupHoT}]} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${Bool[${Me.Song[${healWholeGroupHoT}]}]} && !${Bool[${Me.Song[Elixir of the Seas]}]}) {
	    /call core_cast "${healWholeGroupHoT}" 0 ${Me.ID}
	  }
	}
  

  /if ((!${MajorHurt} && !${MinorHurt}) || ${Bool[${Me.Buff["Divine Aura"]}]} || ${Bool[${Me.Buff["Divine Barrier"]}]} || ${Bool[${Me.Buff["Sacred Barrier"]}]} ) /goto :exit
	/call check_no_cast_buffs
 
  /if (${Math.Calc[${MajorHurt}+${MinorHurt}]} >= 3 && ${Me.SpellReady[${healWholeGroup}]}) {
    /call core_cast "${healWholeGroup}" 0 ${Me.ID} CheckHP.${Me.Class.ShortName}
    /if (${Macro.Return.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
  } 
  
  /if ( ${MajorHurt} >= 3 && ${Me.AltAbilityReady[Call of the Ancients]}) {
    /call core_cast "Call of the Ancients" alt ${Me.ID}
  } 
  
 |*** Intervention Heals ***|
  /if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.${tmpHealOverride} && ${Me.SpellReady[${healIntervention}]}) {
    /if (${Me.SpellReady[${healIntervention}]}) {
     /call core_cast "${healIntervention}" 0 ${WorstHurt} CheckHP.${Me.Class.ShortName}
      /goto :loopheal
    } 
  }
    
  
  | Single Heals
  /if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.${tmpHealOverride} && (${Me.SpellReady[${healMain}]} || ${Me.SpellReady[${healReckless}]})) {
      /if (${Me.SpellReady[${healMain}]}) {
        /call core_cast "${healMain}" 0 ${WorstHurt} CheckHP.${Me.Class.ShortName}
        /goto :loopheal
      } else /if (${Me.SpellReady[${healReckless}]}) {
        /call core_cast "${healReckless}" 0 ${WorstHurt} CheckHP.${Me.Class.ShortName}
        /goto :loopheal
      } 
  }  
  
	| check for personal debuffs
	/if (${switchHealCure} && ${Debuff.Count}) {
		| AA Radient Cure SELF ONLY 100' range 1 min
		/if (${Me.AltAbilityReady[Radiant Cure]} && (${Debuff.Diseased} || ${Debuff.Poisoned} || ${Debuff.Cursed})) /call core_cast "Radiant Cure" alt ${Me.ID} 
		| AA Purified Spirits SELF ONLY 0' range 2 min
		/if (${Me.AltAbilityReady[Purified Spirits]} && ${Debuff.Counters}) /call core_cast "Purified Spirits" alt ${Me.ID} 
	}  
  
 	:exit
 	
  /if ( ${Me.PctHPs} < ${SHMPoint}*.${tmpHealOverride} && !${Me.Song[${healGroupHot}].ID} && ${Me.SpellReady[${healGroupHot}]} ) {
    /call core_cast "${healGroupHot}" 0 ${Me.ID}
  }
  
  /if (${Me.CombatState.Equal[COMBAT]} && ${SpawnCount[npc radius ${MobAgro} playerstate 4 targetable]} >= 1) /call cast_surge_heal

/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub check_heal_XTarget_SHM()
	DEBUGCLASS ${cbug}check_heal_XTarget_SHM()\ax
	ISDEAD
	/if (${Bool[${Me.Song[Touch of the Divine]}]}) /goto :exit
  /declare _count int local

  /for _count 1 to 13
  	ISDEAD
    /if (${Spawn[${Me.XTarget[${_count}].ID}].ID} && (${Spawn[${Me.XTarget[${_count}].ID}].Type.Equal[PC]} || ${ProtectNPCList.Find[${Spawn[${Me.XTarget[${_count}].ID}].CleanName}]}) && ${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} <= ${${Spawn[id ${Me.XTarget[${_count}].ID}].Class.ShortName}Point}*.${tmpHealOverride}) {
			/if (${Spawn[${Me.XTarget[${_count}].ID}].Distance} <= ${Spell[${healMain}].MyRange} && ${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} <= ${${Spawn[id ${Me.XTarget[${_count}].ID}].Class.ShortName}Point}*.${tmpHealOverride} && (${Me.SpellReady[${healMain}]} || ${Me.SpellReady[${healIntervention}]} && || ${Me.SpellReady[${healReckless}]})) {
        /if (${Me.SpellReady[${healMain}]}) {
          /call core_cast "${healMain}" 0 ${Me.XTarget[${_count}].ID} checkHP.${Me.Class.ShortName}
        } else /if (${Me.SpellReady[${healIntervention}]}) {
          /call core_cast "${healIntervention}" 0 ${Me.XTarget[${_count}].ID} checkHP.${Me.Class.ShortName}
        } else /if (${Me.SpellReady[${healReckless}]}) {
          /call core_cast "${healReckless}" 0 ${Me.XTarget[${_count}].ID} checkHP.${Me.Class.ShortName}
        }
      }
    }
		:exitloop
  /next _count

	:exit
/return



|***
 *	DES: surge heal
 *	USE: part of heal routine
 *	INI: 
 *  NOTE: this will try and get a twinheal proc first.
 ***|
sub cast_surge_heal()
	DEBUGCLASS ${cbug}cast_surge_heal()\ax
	ISDEAD
	/doevents
	/if (${Bool[${Me.Song[Touch of the Divine]}]}) /goto :exit
	/declare _tmpTarget int local
	
	
	| nuke to engage twin heal proc
	/if (${Target.ID} == ${Me.ID}) /squelch /target clear
	/if (!${Me.SpellReady[${TC1}]} && !${Me.SpellReady[${TC2}]}) /goto :exit
	
	| /call get_assist_target
	| /call is_target_valid ${Target2Kill}
	| /if (!${Macro.Return}) /goto :exit
	
	
	| target the main assist to trigger.
	/if (${useSmartAssistG}) {
		/varset _tmpTarget ${Spawn[pc ${Group.MainAssist}].ID}
		/goto :nuke
	} else /if (${useSmartAssist1}) {
		/varset _tmpTarget ${Spawn[pc ${Raid.MainAssist}].ID}
		/goto :nuke
	} else /if (${useSmartAssist2}) {
	| /call get_assist_target
	| /call is_target_valid ${Target2Kill}
	| /if (!${Macro.Return}) /goto :exit

	} else /if (${Bool[${MA1}]} && ${Bool[${Spawn[pc ${MA1}].ID}]} && !${Bool[${Spawn[pc ${MA1}].Dead}]} && !${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG}) {
		/varset _tmpTarget ${Spawn[pc ${MA1}].ID}
		/goto :nuke
	} else /if (${Bool[${MA2}]} && ${Bool[${Spawn[pc ${MA2}].ID}]} && !${Bool[${Spawn[pc ${MA2}].Dead}]} && !${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG}) {
		/varset _tmpTarget ${Spawn[pc ${MA2}].ID}
		/goto :nuke
	}
	
	:nuke
	/if (!${Target.LineOfSight} || ${Target.Distance} > ${Spell[${TC1}].MyRange}) /goto :exit
  /if (${Me.SpellReady[${TC1}]} && !${Me.Song[Healing Twincast].ID}) {
		/call core_cast "${TC1}" 0 ${_tmpTarget} 
	} else /if (${Me.SpellReady[${TC2}]} && !${Me.Song[Healing Twincast].ID}) {
		/call core_cast "${TC2}" 0 ${_tmpTarget} 
	}

	| spalsh surge heal	
	/if (${Me.SpellReady[${healSurge}]} && ${Me.Song[Healing Twincast].ID}) {
		/delay 2.25s ${Me.SpellReady[${healSurge}]}
		/call core_cast "${healSurge}" 0 ${Me.ID}
	}
	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub checkHP.SHM()
	/if (${HealList.Find[${Me.Casting}]}) {
		/if (${Target.PctHPs} >= 90) {
 | VOUT Interrupting ${Me.Casting} because ${Target.CleanName} is safe now.
		/stopcast
		/delay 1s !${Me.Casting.ID}
		/goto :exit
		}
	}
	/if (!${HealList.Find[${Me.Casting}]}) /call check_heal_group_${Me.Class.ShortName}
	:exit
/return



|***
 *	DES: MGB Shaman Ancestral Aid if both AAs are ready
 *	USE: /bc spice, /spice
 *	INI: 
 *  NOTE: Will not cast Aid if the buff is present.
 ***|
sub doAid()
	DEBUGCLASS ${cbug}doAid()\ax ${If[${Me.AltAbilityReady[Mass Group Buff]},\agMGB\ax,\arMGB\ax]} ${If[${Me.AltAbilityReady[Ancestral Aid]},\agSpice\ax,\arSpice\ax]}
	ISDEAD
	/doevents
	/if (${Bool[${Me.Song[Touch of the Divine]}]}) /goto :exit
	/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[Ancestral Aid]} && !${Bool[${Me.Song[Ancestral Aid]}]}) {
		/call core_cast "Mass Group Buff" alt 0
		/call core_cast "Ancestral Aid" alt 0
		/if (${Bool[${Raid.Members}]}) /rs ${AidTXT}
	} else /if (${Bool[${Me.Song[Ancestral Aid]}]}) {
		NOUT ${cbad}Quit\ax trying to cast ${csp}Ancestral Aid\ax while its running.	
	}		
	:exit
/return



|***
 *	DES: cripples the target
 *	USE: debufs
 *	INI: 
 *  NOTE: 
 ***|
sub cast_cripple_SHM(int _tmpID)
	/doevents
	ISDEAD
	/declare _resistCount int local
	/call check_exit
	/if (${Bool[${Macro.Return}]} || ${timer_cripple_${_tmpID}} || !${_tmpID}) /goto :exit
	
	/if (${Target.Crippled.Duration}) {
		/call create_timer timer_cripple_${_tmpID} ${Target.Crippled.Duration.TotalSeconds}s
		/goto :exit
	}

	:loopcripple
	/if (${Target.Distance} <= ${Spell[${spellCripple}].MyRange} && ${Me.CurrentMana} > ${Spell[${spellCripple}].Mana} && ${Target.LineOfSight}) {
		/delay 2.25s ${Me.SpellReady[${spellCripple}]}
		/call core_cast "${spellCripple}" 0 ${_tmpID}
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_cripple_${_tmpID} ${Spell[${spellCripple}].Duration.TotalSeconds}s
		} else /if (${Select[${castReturn},CAST_RESISTED,CAST_RESIST,CAST_RESIST,CAST_RESIST]}) {
			/varcalc _resistCount ${_resistCount}
			/if (${_resistCount} >= 3) {
				VOUT ${ctar}${Target.CleanName}\ax resisted ${csp}${spellCripple}\ax ${cinfo}${_resistCount}\ax times, moving on!
				/call create_timer timer_cripple_${_tmpID} ${Spell[${spellCripple}].Duration.TotalSeconds}m
			} else {
				/delay 5
				/goto :loopcripple
			}
		}
	}
	:exit
/return