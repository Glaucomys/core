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
		/call check_engaged
		/if (!${Bool[${Macro.Return}]}) {
			/if (${switchPcPet}) /call check_pet
		}
	}	

	:killtarget
	CHECKREZ
	/call prep_combat
	/if (${Macro.Return} && ${Bool[${combat}]}) {
		DEBUG combat:${cinfo}${combat.Upper}\ax		

		/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
		/if (${switchHealXtarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/if (${switchHealPet}) /call check_pet_heal_${Me.Class.ShortName}
		/if (${switchHealCure}) /call cast_cure

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${switchPcSwarm} || ${switchPcPet}) /call send_pet
			/if (${switchCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/call check_deBuffs ${Target2Kill}
			/if (${switchCombatDoT}) /call cast_DOT
			/if (${switchCombatNuke}) /call cast_nuke
			/if (${switchAlliance}) /call trigger_alliance ${Target2Kill}
			/call click_BP
			/call click_item
			/if (${switchPcSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
		}

		/if (${coreAuto}) {
			/call check_cani	
			/if (${validate_class[FALSE, ${CastList}]}) /call use_mod_rod
			/if (${validate_class[FALSE, ${MeleeList}]}) /call cast_stamina_recovery
			/if (!${is_target_dead[${Target2Kill}]}) /goto :killtarget
		} else /if (!${coreAuto}) {
			/if (${Target.ID} && ${Bool[${combat}]} && !${is_target_dead[${Target.ID}]}) /goto :killtarget
		}
	}

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
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub check_cani()
	DEBUGCLASS ${cbug}check_cani()\ax
	CHECKEXIT
	/doevents
	AMIDEAD
	
	| canni spell first...
	/if (${Me.PctMana} <= ${CanniPctManna} && ${Me.PctHPs} >= ${CanniPctHPs}) {
		/declare _cancast bool local FALSE
	
		/if (${spellCannibalization.Equal[Cannibalize]} && ${Me.CurrentHPs} > 50) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Cannibalize II]} && ${Me.CurrentHPs} > 67) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Cannibalize III]} && ${Me.CurrentHPs} > 74) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Cannibalize IV]} && ${Me.CurrentHPs} > 148) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancient Chaotic Pain]} && ${Me.CurrentHPs} > 668) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Pained Memory]} && ${Me.CurrentHPs} > 688) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancient Ancestral Calling]} && ${Me.CurrentHPs} > 868) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Bargain]} && ${Me.CurrentHPs} > 1042) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Bargain Rk. II]} && ${Me.CurrentHPs} > 1085) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Bargain Rk. III]} && ${Me.CurrentHPs} > 1128) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Hearkening]} && ${Me.CurrentHPs} > 1161) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Hearkening Rk. II]} && ${Me.CurrentHPs} > 1324) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Hearkening Rk. III]} && ${Me.CurrentHPs} > 1141) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Obligation]} && ${Me.CurrentHPs} > 1417) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Obligation Rk. II]} && ${Me.CurrentHPs} > 1615) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Obligation Rk. III]} && ${Me.CurrentHPs} > 1721) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Covenant]} && ${Me.CurrentHPs} > 1559) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Covenant Rk. II]} && ${Me.CurrentHPs} > 1777) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Covenant Rk. III]} && ${Me.CurrentHPs} > 1893) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Arrangement]} && ${Me.CurrentHPs} > 2366) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Arrangement Rk. II]} && ${Me.CurrentHPs} > 2484) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Arrangement Rk. III]} && ${Me.CurrentHPs} > 2608) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Pact]} && ${Me.CurrentHPs} > 4564) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Pact Rk. II]} && ${Me.CurrentHPs} > 4792) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Ancestral Pact Rk. III]} && ${Me.CurrentHPs} > 5032) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Tribal Pact]} && ${Me.CurrentHPs} > 6693) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Tribal Pact Rk. II]} && ${Me.CurrentHPs} > 7028) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Tribal Pact Rk. III]} && ${Me.CurrentHPs} > 7379) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Tribal Bargain]} && ${Me.CurrentHPs} > 8117) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Tribal Bargain Rk. II]} && ${Me.CurrentHPs} > 8523) /varset _cancast TRUE
		/if (${spellCannibalization.Equal[Tribal Bargain Rk. III]} && ${Me.CurrentHPs} > 8949) /varset _cancast TRUE

		/if (${_cancast}) {
			/if (${validate_cast[FALSE, spell, "${spellCannibalization}", ${Me.ID}]}) /call core_cast2 "${spellCannibalization}" 0 0 FALSE
			/return
		}

		| use the aa cani
		/if (${validate_cast[FALSE, alt, "Cannibalization", ${Me.ID}]}) /call core_cast2 "Cannibalization" alt 0 FALSE
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
    /if (${validate_cast[FALSE, alt, "Call of the Ancients", ${Me.ID}]}) /call core_cast2 "Call of the Ancients" alt ${Me.ID} FALSE
  } 

 	| agressive healing sort
 	/if (${switchHealAggressive} && (${Me.SpellReady[${healReckless1}]} || ${Me.SpellReady[${healReckless2}]} || ${Me.SpellReady[${healReckless3}]})) {
		/goto :recklessheals
 	} else /if (!${switchHealAggressive} && (${Me.SpellReady[${healIntervention1}]} || ${Me.SpellReady[${healMain}]})) {
 		/goto :otherheals
 	}	
	
	| reckless heals
	:recklessheals
	/if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.${tmpHealOverride}) {
  	/if (${Me.SpellReady[${healReckless1}]}) {
     /if (${validate_cast[FALSE, spell, "${healReckless1}", ${WorstHurt}]}) /call core_cast2 "${healReckless1}" 0 ${WorstHurt} checkHP_${Me.Class.ShortName}
    } else /if (${Me.SpellReady[${healReckless2}]}) {
      /if (${validate_cast[FALSE, spell, "${healReckless2}", ${WorstHurt}]}) /call core_cast2 "${healReckless2}" 0 ${WorstHurt} checkHP_${Me.Class.ShortName}
    } else /if (${Me.SpellReady[${healReckless3}]}) {
      /if (${validate_cast[FALSE, spell, "${healReckless3}", ${WorstHurt}]}) /call core_cast2 "${healReckless3}" 0 ${WorstHurt} checkHP_${Me.Class.ShortName}
    } 
  }

	| Intervention Heal
	:otherheals
	/if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.${tmpHealOverride}) {
		/if (${Me.SpellReady[${healIntervention1}]}) {
			/if (${validate_cast[FALSE, spell, "${healIntervention1}", ${WorstHurt}]}) /call core_cast2 "${healIntervention1}" 0 ${WorstHurt} checkHP_${Me.Class.ShortName}
		} else /if (${Me.SpellReady[${healMain}]}) {
			/if (${validate_cast[FALSE, spell, "${healMain}", ${WorstHurt}]}) /call core_cast2 "${healMain}" 0 ${WorstHurt} checkHP_${Me.Class.ShortName}
		} 
	} 

	
  | check for surge
  /if (${switchSurge}) {
		/if (${SpawnCount[npc radius ${MobAgro} playerstate 4 targetable]}) /call cast_surge_heal
	}
	
	| Group regenerations. Use all the time. Don't care if hurt or not.
 	/if (${useGroupHoT}) { 
 		/if (${SpawnCount[npc radius ${MobAgro} playerstate 4 targetable]}) {
	 	 	/if (${Me.SpellReady[${healWholeGroupHoT}]} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${Bool[${Me.Song[${healWholeGroupHoT}]}]} && !${Bool[${Me.Song[Elixir of the Seas]}]} && !${Bool[${Me.Song[Elixir of Wulthan]}]}) {
		    /if (${validate_cast[FALSE, spell, "${healWholeGroupHoT}", ${WorstHurt}]}) /call core_cast2 "${healWholeGroupHoT}" 0 ${Me.ID} FALSE
		  }
		}
	}
 
	/call check_no_cast_buffs
  
	| group HoT
	/if (${useGroupHeal}) {
		/if (${Math.Calc[${MajorHurt}+${MinorHurt}]} >= 3 && ${Me.SpellReady[${healWholeGroup}]}) {
			/if (${validate_cast[FALSE, spell, "${healWholeGroup}", ${WorstHurt}]}) /call core_cast2 "${healWholeGroup}" 0 ${Me.ID} checkHP_${Me.Class.ShortName}
		} 
	}
  
	| check for personal debuffs
	/if (${switchHealCure}) {
		/if (${Debuff.Count}) {
			| AA Radient Cure SELF ONLY 100' range 1 min
			/if (${Me.AltAbilityReady[Radiant Cure]} && (${Debuff.Diseased} || ${Debuff.Poisoned} || ${Debuff.Cursed})) {
				/if (${validate_cast[FALSE, alt, "Radiant Cure", ${Me.ID}]}) /call core_cast2 "Radiant Cure" alt ${Me.ID} FALSE
			}
			| AA Purified Spirits SELF ONLY 0' range 2 min
			/if (${Me.AltAbilityReady[Purified Spirits]} && ${Debuff.Count}) {
				/if (${validate_cast[FALSE, alt, "Purified Spirits", ${Me.ID}]}) /call core_cast2 "Purified Spirits" alt ${Me.ID} FALSE
			}
		}  
	}
  
  | check for surge
  /if (${switchSurge}) {
		/if (${SpawnCount[npc radius ${MobAgro} playerstate 4 targetable]}) /call cast_surge_heal
	}
	
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
			DEBUG NAME: ${Me.XTarget[${_count}].DisplayName} -- ID: ${Me.XTarget[${_count}].ID}
			DEBUG CHECK:${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} <= ${${Spawn[id ${Me.XTarget[${_count}].ID}].Class.ShortName}Point}*.${tmpHealOverride}
			/if (${Me.SpellReady[${healMain}]}) {
				/if (${validate_cast[FALSE, spell, "${healMain}", ${Me.XTarget[${_count}].ID}]}) /call core_cast2 "${healMain}" 0 ${Me.XTarget[${_count}].ID} checkHP_${Me.Class.ShortName}
			} else /if (${Me.SpellReady[${healIntervention1}]}) {
				/if (${validate_cast[FALSE, spell, "${healIntervention1}", ${Me.XTarget[${_count}].ID}]}) /call core_cast2 "${healIntervention1}" 0 ${Me.XTarget[${_count}].ID} checkHP_${Me.Class.ShortName}
			} else /if (${Me.SpellReady[${healReckless}]}) {
				/if (${validate_cast[FALSE, spell, "${healReckless1}", ${Me.XTarget[${_count}].ID}]}) /call core_cast2 "${healReckless1}" 0 ${Me.XTarget[${_count}].ID} checkHP_${Me.Class.ShortName}
			} else /if (${Me.SpellReady[${healReckless2}]}) {
				/if (${validate_cast[FALSE, spell, "${healReckless2}", ${Me.XTarget[${_count}].ID}]}) /call core_cast2 "${healReckless2}" 0 ${Me.XTarget[${_count}].ID} checkHP_${Me.Class.ShortName}
			} else /if (${Me.SpellReady[${healReckless3}]}) {
				/if (${validate_cast[FALSE, spell, "${healReckless3}", ${Me.XTarget[${_count}].ID}]}) /call core_cast2 "${healReckless3}" 0 ${Me.XTarget[${_count}].ID} checkHP_${Me.Class.ShortName}
			}
		}
		
	/next _count
	UPDATEHUD FALSE  
/return



|***
 *	DES: Checks and Heals Pets. Group ONLY
 *	USE: /heal pet
 *	INI: 
 *  NOTE: 
 ***|
sub check_pet_heal_SHM()
	DEBUGCLASS ${cbug}check_pet_heal_CLR()\ax
	/declare _count int local
	/varset pWorstHurt ${Me.CleanName}
	/varset pWorstHP 100
	/varset pWorstHurtID ${Me.ID}
  
  /for _count 0 to ${Group}
  	AMIDEAD
		SIMPLEEXIT
		/doevents
		CHECKREZ
    /if (${Group.Member[${_count}].Pet.ID}) {
      /if (${Group.Member[${_count}].Pet.Distance} <= ${Spell[${healRemedy1}].MyRange}) {
        /if (${Group.Member[${_count}].Pet.PctHPs} < ${pWorstHP} && ${Group.Member[${_count}].Pet.PctHPs} < ${PetPoint}) {
          /varset pWorstHurt ${Group.Member[${_count}].Pet.Name}
          /varset pWorstHurtID ${Group.Member[${_count}].Pet.ID}
          /varset pWorstHP ${Group.Member[${_count}].Pet.PctHPs}
        }
      }
    }
  /next _count

  /if (${pWorstHurtID} == ${Me.ID}) /return
 	UPDATEHUD "HEAL Pet"
  DEBUGCLASS PetHeal: ${Group.Member[${_count}].Pet.ID}  ${pWorstHurtID}

	/if (${Me.SpellReady[${healMain}]}) {
		/if (${validate_cast[FALSE, spell, "${healMain}", ${pWorstHurtID}]}) /call core_cast2 "${healMain}" 0 ${pWorstHurtID} checkHP_${Me.Class.ShortName}
	} else /if (${Me.SpellReady[${healIntervention1}]}) {
		/if (${validate_cast[FALSE, spell, "${healIntervention1}", ${pWorstHurtID}]}) /call core_cast2 "${healIntervention1}" 0 ${pWorstHurtID} checkHP_${Me.Class.ShortName}
	} else /if (${Me.SpellReady[${healReckless}]}) {
		/if (${validate_cast[FALSE, spell, "${healReckless1}", ${pWorstHurtID}]}) /call core_cast2 "${healReckless1}" 0 ${pWorstHurtID} checkHP_${Me.Class.ShortName}
	} else /if (${Me.SpellReady[${healReckless2}]}) {
		/if (${validate_cast[FALSE, spell, "${healReckless2}", ${pWorstHurtID}]}) /call core_cast2 "${healReckless2}" 0 ${pWorstHurtID} checkHP_${Me.Class.ShortName}
	} else /if (${Me.SpellReady[${healReckless3}]}) {
		/if (${validate_cast[FALSE, spell, "${healReckless3}", ${pWorstHurtID}]}) /call core_cast2 "${healReckless3}" 0 ${pWorstHurtID} checkHP_${Me.Class.ShortName}
	}  

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
		/if (${validate_cast[FALSE, spell, "${TC1}", ${_tmpID}]}) /call core_cast2 "${TC1}" 0 ${_tmpID} FALSE
	} else /if (${Me.SpellReady[${TC2}]} && !${Me.Song[Healing Twincast].ID}) {
  	/delay 1s ${Target.ID} == ${_tmpID}
		/if (${validate_cast[FALSE, spell, "${TC2}", ${_tmpID}]}) /call core_cast2 "${TC2}" 0 ${_tmpID} FALSE
	}

	| spalsh surge heal	
	/if (${Me.SpellReady[${healSurge}]} && ${Me.Song[Healing Twincast].ID}) {
		/delay 2.25s ${Me.SpellReady[${healSurge}]}
		/if (${validate_cast[FALSE, spell, "${healSurge}", ${_tmpID}]}) /call core_cast2 "${healSurge}" 0 ${_tmpID} FALSE
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
		/if (${validate_cast[FALSE, alt, "Mass Group Buff", ${_tmpID}]}) /call core_cast2 "Mass Group Buff" alt 0
		/if (${validate_cast[FALSE, alt, "Ancestral Aid", ${_tmpID}]}) /call core_cast2 "Ancestral Aid" alt 0
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
	/if (${validate_cast[FALSE, spell, "${spellTalisman}", ${Me.ID}]}) /call core_cast2 "${spellTalisman}" 0 ${Me.ID} FALSE
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
  /declare _type string	local |aggressive|cannihps|cannimanna|groupheal|grouphot|healpoint|rampage|spiregroup|spireraid|surge|talisman|xtradius|xttime

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

	| enable/disable talisman line of spells
	/if (${Bool[${_classType.Find[talisman]}]}) {
		/call writeINI switchTalisman Spells ${If[${switchTalisman},FALSE,TRUE]}

	| set rampage tank toon
	} else /if (${Bool[${_classType.Equal[rampage]}]}) {
		/if (${Bool[${_verbage}]}) {
			/call writeINI RampageTank Heal ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		} else /if (!${Bool[${_verbage}]}) {
			/call writeINI RampageTank Heal FALSE
		}

	| use group Heals
	} else /if (${Bool[${_classType.Equal[groupheal]}]}) {
		/call writeINI useGroupHeal Heal ${If[${useGroupHeal},FALSE,TRUE]} 	

	| use surge Heals
	} else /if (${Bool[${_classType.Equal[surge]}]}) {
		/call writeINI switchSurge Heal ${If[${switchSurge},FALSE,TRUE]} 	

	
	| use group Heal over Time	
	} else /if (${Bool[${_classType.Equal[grouphot]}]}) {
		/call writeINI useGroupHoT Heal ${If[${useGroupHoT},FALSE,TRUE]} 	
	 	
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

	| canni HP pct
	} else /if (${Bool[${_classType.Equal[cannihp]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI CanniPctHPs Spells ${_verbage}
		} else {
			VOUT /${_bind} cannihp [${cinfo}${CanniPctHPs}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	

	| enable/disable lead with aggerssive heals
	} else /if (${Bool[${_classType.Equal[aggressive]}]}) {
		/call writeINI switchHealAggressive Heal ${If[${switchHealAggressive},FALSE,TRUE]}

	| canni Manna pct
	} else /if (${Bool[${_classType.Equal[cannimanna]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI CanniPctManna Spells ${_verbage}
		} else {
			VOUT /${_bind} cannimanna [${cinfo}${CanniPctHPs}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	

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
	/if (${switchMQclearonCLS}) /mqclear
	/for _count 1 to ${_type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_type.Arg[${_count},|]}\ax
		/if (${_type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${_type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${_type.Arg[${_count},|].Equal[xttime]}) /varset _listout ${_listout}(${cnum}${tXHeal}\ax)
		/if (${_type.Arg[${_count},|].Equal[xtradius]}) /varset _listout ${_listout}(${cnum}${XTHealRadius}\ax)
		/if (${_type.Arg[${_count},|].Equal[cannihps]}) /varset _listout ${_listout}(${cnum}${CanniPctHPs}\ax)
		/if (${_type.Arg[${_count},|].Equal[cannimanna]}) /varset _listout ${_listout}(${cnum}${CanniPctManna}\ax)
		/if (${_type.Arg[${_count},|].Equal[groupheal]}) /varset _listout ${_listout}(${If[${useGroupHeal},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[grouphot]}) /varset _listout ${_listout}(${If[${useGroupHoT},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[talisman]}) /varset _listout ${_listout}(${If[${switchTalisman},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[surge]}) /varset _listout ${_listout}(${If[${switchSurge},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[aggressive]}) /varset _listout ${_listout}(${If[${switchHealAggressive},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[rampage]}) /varset _listout ${_listout}(${If[${Bool[${RampageTank}]},${cnum}${RampageTank}\ax,${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}->\ax)

		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]

/return

