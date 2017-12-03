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
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}()\ax
	/doevents
	AMIDEAD

	/if (${Bool[${toonControl}]} && ${Bool[${toonControlXTslot}]} && ${toonControlProtect}) /call protect_always_xt
	CHECKTIE

  /if (!${Bool[${timer_set_xtarget}]} && ${switchHealXtarget}) {
    /call set_xtarget
    /call create_timer timer_set_xtarget ${tXHeal}
  }	

	/if (${coreAuto} && !${Me.Invis}) {
		/call check_cani
	}	
	
	
	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /return

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/call send_pet
		/call stick_to_target
		/call check_deBuffs ${Target2Kill}
		/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
		/if (${switchHealXTarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/call click_DPS_${Me.Class.ShortName}
		/call click_item
		/call disc_${Me.Class.ShortName}
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke	
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
		/call set_combat RANGE
		/call send_pet
		/call stick_to_target
		/call check_deBuffs ${Target2Kill}
		/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
		/if (${switchHealXTarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/call click_DPS_${Me.Class.ShortName}
		/call click_item
		/call disc_${Me.Class.ShortName}
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke			
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
		/call set_combat NUKE
		/call send_pet
		/call check_deBuffs ${Target2Kill}
		/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
		/if (${switchHealXTarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/call click_DPS_${Me.Class.ShortName}
		/call click_item
		/call disc_${Me.Class.ShortName}
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke	
	} else /if (${Select[${combat},HEAL]}) {
		/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
		| /if (${switchHealXTarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/call disc_${Me.Class.ShortName}
		/if (${is_target_valid[${Target2Kill}]}) {
			/call check_deBuffs ${Target2Kill}
			/if (${DoDoT}) /call cast_DOT
			/if (${DoNuke}) /call cast_nuke
			/call doclickItem
			/call send_pet
			/call click_DPS_${Me.Class.ShortName}
			/call disc_${Me.Class.ShortName}
		}
		/if (${switchPcPet} && !${Me.Pet.ID}) /call check_pet
	} else /if (${Select[${combat},DUAL]}) {
	}
	/if (${coreAuto}) {	
		/if (${switchHealCure}) /call cast_cure
		/call check_cani
		/call use_mod_rod
	}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} && !${is_target_dead[${Target2Kill}]}) /goto :killtarget
	/if (!${coreAuto} && ${Target.ID} && ${Bool[${combat}]} && !${is_target_dead[${Target.ID}]}) /goto :killtarget
/return



|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: Disc
 *	INI: 
 *  NOTE: 
 ***|
sub disc_SHM()
	DEBUGCLASS	${cbug}disc_${Me.Class.ShortName}()\ax
	CHECKEXIT
	/doevents
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_SHM()
	DEBUGCLASS ${cbug}clickDPS.${Me.Class.ShortName}()\ax 
	CHECKEXIT
	/doevents
	/call click_horn_of_unity 	
	/if (${Me.ItemReady[${Epic}]} && !${Me.Song[Prophet's Gift of the Ruchu].ID} && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${MobAgro} playerstate 4 targetable]} >= 3)  /call core_cast "${Epic}" item 0 FALSE
/return
	


|***
 *	DES: We want Mana
 *	USE: /cani #
 *	INI: 
 *  NOTE: 
 ***|
sub check_cani()
	DEBUGCLASS ${cbug}check_cani()\ax
	CHECKEXIT
	/doevents
	
	/if (${Me.PctMana} < ${CaniPercent} && ${Me.SpellReady[${spellCannibalization}]}) {
		/call core_cast "${spellCannibalization}" 0 0 FALSE
	}
	/if (${Me.PctMana} < ${CaniPercent} && ${Me.AltAbilityReady[Cannibalization]}) {
		/call core_cast "Cannibalization" alt 0 FALSE
	}
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub check_heal_group_SHM()
	DEBUG ${cbug}check_heal_group_SHM()\ax

	/if (${Bool[${Me.Song[Touch of the Divine]}]}) /return
	/declare _count	int	local 0

	:loopheal
	DEBUG :loopheal
	CHECKEXIT
	/doevents
	CHECKTIE
	/varset WorstHurt 				${Me.ID}
	/varset WorstHP 					${If[${HealOverride},${Float[${HealOverride}].Centi},99.99]}
	/varset WorstClass 				${Me.Class.ShortName}
	/varset MinorHurt 				0
	/varset MajorHurt					0
	

	| Does the control toon need healing?
	/if (${Bool[${toonControl}]} && ${toonControlProtect} && (${Bool[${RampageTank}]} && ${RampageTank.NotEqual[${toonControl}]})) {
		/if (!${Spawn[${toonControl}].Dead} && ${Bool[${Spawn[${toonControl}]}]} && ${Spawn[${toonControl}].ID} && ${Spawn[${toonControl}].PctHPs} <= ${WorstHP} && (${Spawn[${toonControl}].Distance} <= ${Spell[${healRemedy1}].MyRange} && ${Spawn[${toonControl}].PctHPs} <= ${${Spawn[${toonControl}].Class.ShortName}Point}*.${tmpHealOverride} || ${Spawn[${toonControl}].Distance} <= ${Spell[${healRenewal1}].MyRange} && ${Spawn[${toonControl}].PctHPs} <= ${${Spawn[${toonControl}].Class.ShortName}Point}*.${tmpHealOverride}*.7 )) {
			/varset WorstHurt ${Spawn[${toonControl}].ID}
			/varset WorstHP ${Spawn[${toonControl}].PctHPs}
			/varset WorstClass ${Spawn[${toonControl}].Class.ShortName}
		} 
	}

	/if (${Bool[${Group.Members}]}) {
		/for _count 0 to ${Group.Members}
			DEBUG _count ${_count} ${Group.Member[${_count}].DisplayName}
			/if (${Bool[${RampageTank}]} && ${RampageTank.Equal[${Group.Member[${_count}].DisplayName}]}) /continue
			AMIDEAD

			/if (!${Group.Member[${_count}].Dead} && (${Spawn[pc id ${Group.Member[${_count}].ID}].ID} || ${Spawn[mercenary id ${Group.Member[${_count}].ID}].ID}) && ${Group.Member[${_count}].PctHPs} <= ${WorstHp} && (${Group.Member[${_count}].Distance} <= ${Spell[${healMain}].MyRange} && ${Group.Member[${_count}].PctHPs} <= ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride} || ${Group.Member[${_count}].Distance} <= ${Spell[${healMain}].MyRange} && ${Group.Member[${_count}].PctHPs} <= ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride})) {
				/varset WorstHurt ${Group.Member[${_count}].ID}
				/varset WorstHp ${Group.Member[${_count}].PctHPs}
				/varset WorstClass ${Group.Member[${_count}].Class.ShortName}
				DEBUG WorstHurt: ${Group.Member[${_count}].DisplayName}[${Group.Member[${_count}].ID}] ${Group.Member[${_count}].PctHPs}% - hp check loop
			} 
	    /if (!${Spawn[pc id ${Group.Member[${_count}].ID}].Dead} && (${Group.Member[${_count}].PctHPs}<85 || ${Group.Member[${_count}].PctHPs} < ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}) && ${Group.Member[${_count}].PctHPs} > ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}*.7 && ${Group.Member[${_count}].Distance} <= ${Spell[${healWholeGroup}].AERange}) {
	      /varcalc MinorHurt ${MinorHurt}+1
	    } else /if (!${Spawn[pc id ${Group.Member[${_count}].ID}].Dead} && ${Group.Member[${_count}].PctHPs}<${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}*.7 && ${Group.Member[${_count}].Type.NotEqual[Corpse]} && ${Group.Member[${_count}].Distance} <= ${Spell[${healWholeGroup}].AERange}) {
	      /varcalc MajorHurt ${MajorHurt}+1
	    }
	    /doevents
	  /next _count
	}
	
  | check for surge
	/if (${SpawnCount[npc radius ${MobAgro} playerstate 4 targetable]} >= 1) /call cast_surge_heal
	
	| Group regenerations. Use all the time. Don't care if hurt or not.
 	/if (${useGroupHoT} && ${SpawnCount[npc radius ${MobAgro} playerstate 4 targetable]} >= 1) {
 	 	/if (${Me.SpellReady[${healWholeGroupHoT}]} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${Bool[${Me.Song[${healWholeGroupHoT}]}]} && !${Bool[${Me.Song[Elixir of the Seas]}]}) {
	    /call core_cast "${healWholeGroupHoT}" 0 ${Me.ID} FALSE
	  }
	}
  
  | /if ((!${MajorHurt} && !${MinorHurt}) || ${Bool[${Me.Buff["Divine Aura"]}]} || ${Bool[${Me.Buff["Divine Barrier"]}]} || ${Bool[${Me.Buff["Sacred Barrier"]}]} ) /return


	/call check_no_cast_buffs
  
	| group HoT
	/if (${useGroupHeal} && ${Math.Calc[${MajorHurt}+${MinorHurt}]} >= 3 && ${Me.SpellReady[${healWholeGroup}]}) {
		/call core_cast "${healWholeGroup}" 0 ${Me.ID} checkHP_${Me.Class.ShortName}
		/if (${Macro.Return.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
	} 
  
  | stationary ward heal
  /if (${MajorHurt} >= 3 && ${Me.AltAbilityReady[Call of the Ancients]}) {
    /call core_cast "Call of the Ancients" alt ${Me.ID} FALSE
  } 

	| Intervention Heal
	/if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.${tmpHealOverride}*.4 && ${Me.SpellReady[${healIntervention}]}) {
		/if (${Me.SpellReady[${healIntervention}]}) {
			/call core_cast "${healIntervention}" 0 ${WorstHurt} checkHP_${Me.Class.ShortName}
			/goto :loopheal
		} 
	}

  | Single Heals
  /if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.${tmpHealOverride} && (${Me.SpellReady[${healMain}]} || ${Me.SpellReady[${healReckless}]})) {
      /if (${Me.SpellReady[${healMain}]}) {
        /call core_cast "${healMain}" 0 ${WorstHurt} checkHP_${Me.Class.ShortName}
        /goto :loopheal
      } else /if (${Me.SpellReady[${healReckless}]}) {
        /call core_cast "${healReckless}" 0 ${WorstHurt} checkHP_${Me.Class.ShortName}
        /goto :loopheal
      } 
  }  
  
	| check for personal debuffs
	/if (${switchHealCure} && ${Debuff.Count}) {
		| AA Radient Cure SELF ONLY 100' range 1 min
		/if (${Me.AltAbilityReady[Radiant Cure]} && (${Debuff.Diseased} || ${Debuff.Poisoned} || ${Debuff.Cursed})) /call core_cast "Radiant Cure" alt ${Me.ID} FALSE
		| AA Purified Spirits SELF ONLY 0' range 2 min
		/if (${Me.AltAbilityReady[Purified Spirits]} && ${Debuff.Counters}) /call core_cast "Purified Spirits" alt ${Me.ID} FALSE
	}  
  
  | check for surge
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
	AMIDEAD

	/if (${Bool[${Me.Song[Touch of the Divine]}]}) /return
  /declare _count int local 0

  /for _count 1 to 13
  	AMIDEAD

    /if (${Spawn[${Me.XTarget[${_count}].ID}].ID} && (${Spawn[${Me.XTarget[${_count}].ID}].Type.Equal[PC]} || ${ProtectNPCList.Find[${Spawn[${Me.XTarget[${_count}].ID}].CleanName}]}) && ${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} <= ${${Spawn[id ${Me.XTarget[${_count}].ID}].Class.ShortName}Point}*.${tmpHealOverride}) {
			/if (${Spawn[${Me.XTarget[${_count}].ID}].Distance} <= ${Spell[${healMain}].MyRange} && ${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} <= ${${Spawn[id ${Me.XTarget[${_count}].ID}].Class.ShortName}Point}*.${tmpHealOverride} && (${Me.SpellReady[${healMain}]} || ${Me.SpellReady[${healIntervention}]} && || ${Me.SpellReady[${healReckless}]})) {
        /if (${Me.SpellReady[${healMain}]}) {
          /call core_cast "${healMain}" 0 ${Me.XTarget[${_count}].ID} checkHP_${Me.Class.ShortName}
        } else /if (${Me.SpellReady[${healIntervention}]}) {
          /call core_cast "${healIntervention}" 0 ${Me.XTarget[${_count}].ID} checkHP_${Me.Class.ShortName}
        } else /if (${Me.SpellReady[${healReckless}]}) {
          /call core_cast "${healReckless}" 0 ${Me.XTarget[${_count}].ID} checkHP_${Me.Class.ShortName}
        }
      }
    }
  /next _count
/return



|***
 *	DES: surge heal
 *	USE: part of heal routine
 *	INI: 
 *  NOTE: this will try and get a twinheal proc first.
 ***|
sub cast_surge_heal()
	DEBUGCLASS ${cbug}cast_surge_heal(nuke1:${TC1}, nuke2:${TC2})\ax
	/if ((!${Bool[${TC1}]} && !${Bool[${TC1}]})) /return
	/if (${Bool[${Me.Song[Touch of the Divine]}]} || (!${Bool[${TC1}]} && !${Bool[${TC1}]})) /return

	AMIDEAD
	/doevents
	/declare _tmpTarget int local
	
	
	| nuke to engage twin heal proc
	/if (${Target.ID} == ${Me.ID}) /squelch /target clear
	/if (!${Me.SpellReady[${TC1}]} && !${Me.SpellReady[${TC2}]}) /return
	
	| /call get_assist_target
	| /call is_target_valid ${Target2Kill}
	| /if (!${Bool[${Macro.Return}]}) /return
	
	
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
	| /if (!${Bool[${Macro.Return}]}) /return

	} else /if (${Bool[${MA1}]} && ${Bool[${Spawn[pc ${MA1}].ID}]} && !${Bool[${Spawn[pc ${MA1}].Dead}]} && !${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG}) {
		/varset _tmpTarget ${Spawn[pc ${MA1}].ID}
		/goto :nuke
	} else /if (${Bool[${MA2}]} && ${Bool[${Spawn[pc ${MA2}].ID}]} && !${Bool[${Spawn[pc ${MA2}].Dead}]} && !${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG}) {
		/varset _tmpTarget ${Spawn[pc ${MA2}].ID}
		/goto :nuke
	}
	
	:nuke
	/if (!${Target.LineOfSight} || ${Target.Distance} > ${Spell[${TC1}].MyRange}) /return
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
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub checkHP_SHM()
	/if (${HealList.Find[${Me.Casting}]}) {
		/if (${Target.PctHPs} >= 90) {
 | VOUT Interrupting ${Me.Casting} because ${Target.CleanName} is safe now.
		/stopcast
		/delay 1s !${Me.Casting.ID}
		/return
		}
	}
	/if (!${HealList.Find[${Me.Casting}]}) /call check_heal_group_${Me.Class.ShortName}
/return



|***
 *	DES: MGB Shaman Ancestral Aid if both AAs are ready
 *	USE: /bc spice, /spice
 *	INI: 
 *  NOTE: Will not cast Aid if the buff is present.
 ***|
sub doAid()
	DEBUGCLASS ${cbug}doAid()\ax ${If[${Me.AltAbilityReady[Mass Group Buff]},\agMGB\ax,\arMGB\ax]} ${If[${Me.AltAbilityReady[Ancestral Aid]},\agSpice\ax,\arSpice\ax]}
	AMIDEAD

	/doevents
	/if (${Bool[${Me.Song[Touch of the Divine]}]}) /return
	/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[Ancestral Aid]} && !${Bool[${Me.Song[Ancestral Aid]}]}) {
		/call core_cast "Mass Group Buff" alt 0
		/call core_cast "Ancestral Aid" alt 0
		/if (${Bool[${Raid.Members}]}) /rs ${AidTXT}
	} else /if (${Bool[${Me.Song[Ancestral Aid]}]}) {
		VOUT ${cbad}Quit\ax trying to cast ${csp}Ancestral Aid\ax while its running.	
	}		
/return




 |***
 *	DES: Shaman burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_SHM()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	CHECKEXIT
	/doevents
	/call cast_AA_spire

	/call use_glyph
	/call synergy_click_Circle
	/call click_item_burn
/return



|***
 *	DES: Class controls
 *	USE: /shm []
 *	INI: 
 *  NOTE:
 ***|
#bind setSHM /shm
sub Bind_setSHM(string _classType, string _classVerbage)
	DEBUG ${cbug}Bind_setSHM(type:${_classType}, verbage:${_classVerbage})\ax
	/if (!${validate_class[TRUE, |SHM]}) /return

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
	} else /if (${Bool[${${_bind}Type.Find[${_classType}]}]} && ${_classVerbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_classType.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_classType.Lower}]}
		/return
	}	
	| use group Heals
	} else /if (${Bool[${_classType.Equal[groupheal]}]}) {
		/call Event_SetVar NA useGroupHeal Heal ${If[${useGroupHeal},FALSE,TRUE]} 	
	 	/call echos switch "Use Group Heal" useGroupHeal 0 0
	
	| use group Heal over Time	
	} else /if (${Bool[${_classType.Equal[grouphot]}]}) {
		/call Event_SetVar NA useGroupHoT Heal ${If[${useGroupHoT},FALSE,TRUE]} 	
	 	/call echos switch "Use Group Heal over Time" useGroupHoT 0 0
	 	
	| set Class Heal Point	
	} else /if (${Bool[${_classType.Equal[healpoint]}]}) {
		/if (${Bool[${_classVerbage}]}) /declare _num int local ${_classVerbage}
		/if (${Target.ID} && ${_num} && ${Spawn[${Target.ID}].Type.NotEqual[Pet]}) {
			/call writeINI ${Target.Class.ShortName}Point Heal ${_num}
			VOUT ${ctar}${Target.Class}\ax Heal Point ${sep} ${cinfo}${_classVerbage}\ax
		} else /if (${Target.ID} && ${_num} && ${Spawn[${Target.ID}].Type.Equal[Pet]}) {
			/call writeINI PetPoint Heal ${_num}
			VOUT ${ctar}Pet\ax Heal Point ${sep} ${cinfo}${_classVerbage}\ax
		} else {
			VOUT /${_bind} healpoint [${cinfo}##\ax] [${cinfo}TARGET\ax]
		}

	| set XTar heal radius
	} else /if (${Bool[${_classType.Equal[xtradius]}]}) {
		/if (${Range.Between[1,300:${Int[${_classVerbage}]}]}) {
			/call writeINI XTHealRadius Heal ${_classVerbage}
			/call echos switch "XTarget Heal Radius ${sep}" XTHealRadius 0 0
		} else /if (!${Bool[${_classVerbage}]} || !${Range.Between[1,300:${Int[${_classVerbage}]}]}) {
			VOUT /${_bind} xtradius [${cinfo}${XTHealRadius}\ax] allowable range ${sep} ${cinfo}1 - 300\ax
		}	

	| set XT refresh list Time
	} else /if (${Bool[${_classType.Equal[xttime]}]}) {
		/if (${Range.Between[1,5:${Int[${_classVerbage}]}]}) {
			/call writeINI tXHeal Timer ${_classVerbage}
			/call echos number "XTarget refresh time" ${tXHeal} 0 0
		} else /if (!${Bool[${_classVerbage}]} || !${Range.Between[1,5:${Int[${_classVerbage}]}]}) {
			VOUT /${_bind} xttime [${cinfo}${tXHeal}m\ax] allowable range ${sep} ${cinfo}1 - 5 min\ax
		}	

	| spire control
	} else /if (${Bool[${_classType.Equal[spiregroup]}]} || ${Bool[${_classType.Equal[spireraid]}]}) {
		/declare _spiretype string local ${_classType.Left[5].Lower}${_classType.Mid[6,1].Upper}${_classType.Right[-6].Lower}
		/if (${Range.Between[0,3:${Int[${_classVerbage}]}]}) {
			/call writeINI ${_spiretype} Buffs ${_classVerbage}
		} 
		/for _count 1 to ${typeSpire.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${${_spiretype}} == ${typeSpire.Arg[${_count},|]},${cgood},${cinfo}]}${typeSpire.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} ${_classType.Lower} [${_listout}]	
	 	

	} else {
		/for _count 1 to ${${_bind}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
	} 
/return