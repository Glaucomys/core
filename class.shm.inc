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
	
	CHECKREZ
	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /return

	CHECKREZ
	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/call send_pet
		/call stick_to_target
		/call check_deBuffs ${Target2Kill}
		/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
		/if (${switchHealXtarget}) /call check_heal_XTarget_${Me.Class.ShortName}
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
		/if (${switchHealXtarget}) /call check_heal_XTarget_${Me.Class.ShortName}
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
		/if (${switchHealXtarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/call click_DPS_${Me.Class.ShortName}
		/call click_item
		/call disc_${Me.Class.ShortName}
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke	
	} else /if (${Select[${combat},HEAL]}) {
		/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
		/if (${switchHealXtarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/call disc_${Me.Class.ShortName}
		/if (${is_target_valid[${Target2Kill}]}) {
			/call check_deBuffs ${Target2Kill}
			/if (${DoDoT}) /call cast_DOT
			/if (${DoNuke}) /call cast_nuke
			/call click_item
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
		/if (${switchTalisman} && !${timer_cast_talisman}) /call cast_talisman
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
		/return
	}
	/if (${Me.PctMana} < ${CaniPercent} && ${Me.AltAbilityReady[Cannibalization]}) {
		/call core_cast "Cannibalization" alt 0 FALSE
		/return
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
		/if (!${Spawn[${toonControl}].Dead} && ${Bool[${Spawn[${toonControl}]}]} && ${Spawn[${toonControl}].ID} && ${Spawn[${toonControl}].PctHPs} <= ${WorstHP} && ${Spawn[${toonControl}].PctHPs} <= ${${Spawn[${toonControl}].Class.ShortName}Point}*.${tmpHealOverride} && ${Spawn[${toonControl}].Distance} <= ${Spell[${healMain}].MyRange}) {
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

			/if (!${Group.Member[${_count}].Dead} && (${Spawn[pc id ${Group.Member[${_count}].ID}].ID} || ${Spawn[mercenary id ${Group.Member[${_count}].ID}].ID}) && ${Group.Member[${_count}].PctHPs} <= ${WorstHp} && (${Group.Member[${_count}].Distance} <= ${Spell[${healMain}].MyRange} && ${Group.Member[${_count}].PctHPs} <= ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride})) {
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
	
  | stationary ward heal
  /if (${MajorHurt} >= 3 && ${Me.AltAbilityReady[Call of the Ancients]}) {
    /call core_cast "Call of the Ancients" alt ${Me.ID} FALSE
  } 

	| Intervention Heal
	/if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.${tmpHealOverride} && ${Me.SpellReady[${healIntervention1}]}) {
		/if (${Me.SpellReady[${healIntervention1}]}) {
			/call core_cast "${healIntervention1}" 0 ${WorstHurt} checkHP_${Me.Class.ShortName}
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
	
		
  | check for surge
	/if (${SpawnCount[npc radius ${MobAgro} playerstate 4 targetable]} >= 1) /call cast_surge_heal
	
	| Group regenerations. Use all the time. Don't care if hurt or not.
 	/if (${useGroupHoT} && ${SpawnCount[npc radius ${MobAgro} playerstate 4 targetable]} >= 1) {
 	 	/if (${Me.SpellReady[${healWholeGroupHoT}]} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${Bool[${Me.Song[${healWholeGroupHoT}]}]} && !${Bool[${Me.Song[Elixir of the Seas]}]} && !${Bool[${Me.Song[Elixir of Wulthan]}]}) {
	    /call core_cast "${healWholeGroupHoT}" 0 ${Me.ID} FALSE
	  }
	}
  
	/call check_no_cast_buffs
  
	| group HoT
	/if (${useGroupHeal} && ${Math.Calc[${MajorHurt}+${MinorHurt}]} >= 3 && ${Me.SpellReady[${healWholeGroup}]}) {
		/call core_cast "${healWholeGroup}" 0 ${Me.ID} checkHP_${Me.Class.ShortName}
		/if (${Macro.Return.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
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

	DEBUGCLASS ${cbug}check_heal_XTarget_${Me.Class.ShortName}()\ax
	/if (!${validate_expansion[FALSE, |16]}) /return
  /declare _count int local 0

  /for _count 1 to 13
		/if (${Bool[${RampageTank}]} && ${RampageTank.Equal[${Spawn[${Me.XTarget[${_count}].ID}].DisplayName}]}) /continue
  	| /if (${Me.XTarget[${_count}].DisplayName.Equal[NULL]}) /continue
  	|  /echo ${Me.XTarget[${_count}].DisplayName}
  	/if (!${Spawn[${Me.XTarget[${_count}].ID}].Type.Equal[PC]} && !${ProtectNPCList.Find[${Spawn[${Me.XTarget[${_count}].ID}].DisplayName}]}) /continue
  	
  	UPDATEHUD "HEAL XTARGET"
		/call check_no_cast_buffs
		AMIDEAD
		SIMPLEEXIT
		/doevents

		/if (!${Spawn[${Me.XTarget[${_count}].ID}].ID} || ${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} > ${${Spawn[pc ${Me.XTarget[${_count}].ID}].Class.ShortName}Point}*.${tmpHealOverride}) /continue

		/if (${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} <= ${${Spawn[id ${Me.XTarget[${_count}].ID}].Class.ShortName}Point}*.${tmpHealOverride}) {
			SDEBUG NAME: ${Me.XTarget[${_count}].DisplayName} -- ID: ${Me.XTarget[${_count}].ID}
			SDEBUG CHECK:${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} <= ${${Spawn[id ${Me.XTarget[${_count}].ID}].Class.ShortName}Point}*.${tmpHealOverride}
			/if (${Me.SpellReady[${healMain}]}) {
				/call core_cast "${healMain}" 0 ${Me.XTarget[${_count}].ID} check_HP_${Me.Class.ShortName}
			} else /if (${Me.SpellReady[${healIntervention1}]}) {
				/call core_cast "${healIntervention1}" 0 ${Me.XTarget[${_count}].ID} check_HP_${Me.Class.ShortName}
			} else /if (${Me.SpellReady[${healReckless}]}) {
				/call core_cast "${healReckless}" 0 ${Me.XTarget[${_count}].ID} check_HP_${Me.Class.ShortName}
			}
		}
		
	/next _count
	UPDATEHUD FALSE  
/return



|***
 *	DES: surge heal
 *	USE: part of heal routine
 *	INI: 
 *  NOTE: this will try and get a twinheal proc first.
 ***|
sub cast_surge_heal()
	DEBUGCLASS ${cbug}cast_surge_heal(TC1:${Me.SpellReady[${TC1}]}, TC2:${Me.SpellReady[${TC2}]})\ax
	/if (!${Bool[${TC1}]} && !${Bool[${TC1}]}) /return
	/if (${Bool[${Me.Song[Touch of the Divine]}]}) /return
	
	CHECKEXIT
	AMIDEAD
	/doevents
	
	| nuke to engage twin heal proc
	/if (${Target.ID} == ${Me.ID}) /squelch /target clear
	/if (!${Me.SpellReady[${TC1}]} && !${Me.SpellReady[${TC2}]}) /return

	/declare _tmpID int local
	
	/if (${useSmartAssistG}) {
		/varset _tmpID ${Spawn[pc ${Group.MainAssist}].ID}
	} else /if (${useSmartAssistR}) {
		/varset _tmpID ${Spawn[pc ${Raid.MainAssist}].ID}
	} else /if (${Bool[${MA1}]} && ${Bool[${Spawn[pc ${MA1}].ID}]} && !${Bool[${Spawn[pc ${MA1}].Dead}]}) {
		DEBUG MA1: ${Spawn[pc ${MA1}].ID}
		/varset _tmpID ${Spawn[pc ${MA1}].ID}
	} else /if (${Bool[${MA2}]} && ${Bool[${Spawn[pc ${MA2}].ID}]} && !${Bool[${Spawn[pc ${MA2}].Dead}]}) {
		/varset _tmpID ${Spawn[pc ${MA2}].ID}
	}
	DEBUG _tmpID[${_tmpID}] is: ${Spawn[${_tmpID}]} -- ${Spawn[${_tmpID}].DisplayName}

 	/target ID ${Spawn[${_tmpID}].ID}
 	/delay 1s ${Target.ID} == ${Spawn[${_tmpID}].ID}

	
	/if (!${Spawn[${_tmpID}].LineOfSight} || ${Spawn[${_tmpID}].Distance} > ${Spell[${TC1}].MyRange}) /return
	
  /if (${Me.SpellReady[${TC1}]} && !${Me.Song[Healing Twincast].ID}) {
  	/delay 1s ${Target.ID} == ${_tmpID}
		/call core_cast "${TC1}" 0 ${_tmpID} FALSE
	} else /if (${Me.SpellReady[${TC2}]} && !${Me.Song[Healing Twincast].ID}) {
  	/delay 1s ${Target.ID} == ${_tmpID}
		/call core_cast "${TC2}" 0 ${_tmpID} FALSE
	}

	| spalsh surge heal	
	/if (${Me.SpellReady[${healSurge}]} && ${Me.Song[Healing Twincast].ID}) {
		/delay 2.25s ${Me.SpellReady[${healSurge}]}
		/call core_cast "${healSurge}" 0 ${_tmpID} FALSE
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

	| epic 2.0
	/if (${Me.ItemReady[${${Me.Class.ShortName}Epic2}]} && ${Spell[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].Stacks} && !${Me.Buff[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID} && !${Me.Song[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID}) /call core_cast "${${Me.Class.ShortName}Epic2}" item 0 FALSE

	/call cast_AA_spire

	/call use_glyph
	/call synergy_click_Circle
	/call click_item_burn
/return



|***
 *	DES: Control for Talisman Buff
 *	USE: /call cast_talisman
 *	INI: 
 *  NOTE: keeps talisman buff line up
 ***|
sub cast_talisman()
	DEBUGCLASS ${cbug}cast_talisman()\ax 
	/if (!${switchTalisman}) /return	
	/if (${Me.Buff[${spellTalisman}].ID}) /return
	AMIDEAD
	CHECKEXIT
	/doevents
	/call create_timer timer_cast_Talisman ${tTalisman}
	/call core_cast "${spellTalisman}" 0 ${Me.ID} FALSE
/return



|***
 *	DES: Class controls
 *	USE: /shm []
 *	INI: 
 *  NOTE:
 ***|
#bind setSHM /shm
sub Bind_setSHM(string _classType, string _verbage)
	DEBUG ${cbug}Bind_setSHM(type:${_classType}, verbage:${_verbage})\ax
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
	} else /if (${Bool[${${_bind}Type.Find[${_classType}]}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_classType.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_classType.Lower}]}
		/return
	}	

	| enable/disable talisman line of spells
	/if (${Bool[${_classType.Find[talisman]}]}) {
		/call writeINI switchTalisman Spells ${If[${switchTalisman},FALSE,TRUE]}
		VOUT /${_bind} talisman ${sep} ${If[${switchTalisman},${oenabled},${odisabled}]}
		/return

	| use group Heals
	} else /if (${Bool[${_classType.Equal[groupheal]}]}) {
		/call Event_SetVar NA useGroupHeal Heal ${If[${useGroupHeal},FALSE,TRUE]} 	
	 	/call echos switch "Use Group Heal" useGroupHeal 0 0
	 	/return
	
	| use group Heal over Time	
	} else /if (${Bool[${_classType.Equal[grouphot]}]}) {
		/call Event_SetVar NA useGroupHoT Heal ${If[${useGroupHoT},FALSE,TRUE]} 	
	 	/call echos switch "Use Group Heal over Time" useGroupHoT 0 0
	 	/return
	 	
	| set Class Heal Point	
	} else /if (${Bool[${_classType.Equal[healpoint]}]}) {
		/if (${Target.ID} && ${Range.Between[1,99:${Int[${_verbage}]}]} && ${Spawn[${Target.ID}].Type.NotEqual[Pet]}) {
			/call writeINI ${Target.Class.ShortName}Point Heal ${_verbage}
		} else /if (${Target.ID} && ${Range.Between[1,99:${Int[${_verbage}]}]} && ${Spawn[${Target.ID}].Type.Equal[Pet]}) {
			/call writeINI PetPoint Heal ${_verbage}
		} else {
			VOUT /${_bind} healpoint [${cinfo}##\ax] [${cinfo}TARGET\ax]
		}
		/for _count 1 to ${AllClassList.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${AllClassList.Arg[${_count},|].Upper}\ax(${cnum}${${AllClassList.Arg[${_count},|].Upper}Point}\ax)
			/varset _sep TRUE
		/next _count
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}PET\ax(${cnum}${PetPoint}\ax)
		VOUT /${_bind} [${_listout}]
		/return

	| set XTar heal radius
	} else /if (${Bool[${_classType.Equal[xtradius]}]}) {
		/if (!${validate_expansion[TRUE, |16]}) /return
		/if (${Range.Between[1,300:${Int[${_verbage}]}]}) {
			/call writeINI XTHealRadius Heal ${_verbage}
		} else {
			VOUT /${_bind} xtradius [${cinfo}${XTHealRadius}\ax] allowable range ${sep} ${cinfo}1 - 300\ax
			/return
		}	
		
	| set XT refresh list Time
	} else /if (${Bool[${_classType.Equal[xttime]}]}) {
		/if (!${validate_expansion[TRUE, |16]}) /return
		/if (${Range.Between:${Int[${_verbage}]}]}) {
			/call writeINI tXHeal Timer ${_verbage}
		} else {
			VOUT /${_bind} xttime [${cinfo}${tXHeal}m\ax] allowable range ${sep} ${cinfo}1 - 5 min\ax
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

	}
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
		/if (${${_bind}Type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[xttime]}) /varset _listout ${_listout}(${cnum}${tXHeal}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[xtradius]}) /varset _listout ${_listout}(${cnum}${XTHealRadius}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]

/return