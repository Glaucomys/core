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
	CHECKINPUTS
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

		/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
		/if (${switchHealXtarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/if (${switchHealPet}) /call check_pet_heal_${Me.Class.ShortName}
		/if (${switchHealCure}) /call cast_cure

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${switchCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/call check_deBuffs ${Target2Kill}
			/if (${switchCombatDoT}) /call cast_DOT
			/if (${switchCombatNuke}) /call cast_nuke
			/if (${switchAlliance}) /call trigger_alliance ${Target2Kill}
			/if (${switchPcSwarm} || ${switchPcPet}) /call send_pet
			/call click_BP
			/call click_item
			/if (${switchPcSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
		}

		/if (${coreAuto}) {
			/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
			/call check_cani	
			/if (${switchModRod} && ${validate_class[FALSE, ${CastList}]}) /call use_mod_rod
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
	CHECKEXIT
	CHECKINPUTS
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_SHM()
	CHECKEXIT
	CHECKINPUTS
	/call click_horn_of_unity 	
/return
	


|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub check_heal_group_SHM()
	AMIDEAD
	CHECKEXIT
	CHECKTIE

	/declare _count 					int local 0
	/varset hurtID 						${Me.ID}
	/varset hurtPctHP 				${If[${HealOverride},${Float[${HealOverride}].Centi},99.99]}
	/varset hurtCLS 					${Me.Class.ShortName}
	/varset hurtCount					0

	| Does the control toon need healing?
	/if (${toonControlProtect} && ${Bool[${toonControl}]} && ${Bool[${Spawn[${toonControl}]}]}) {
		/if (!${Bool[${Group.Member[${toonControl}]}]} && ${RampageTank.NotEqual[${toonControl}]}) {
			/if (!${Spawn[${toonControl}].Dead} && ${Spawn[pc ${toonControl}].ID} && ${Spawn[pc ${toonControl}].PctHPs} <= ${hurtPctHP} && ${Spawn[${toonControl}].Distance} <= ${MobAgro} && ${Spawn[${toonControl}].PctHPs} <= ${${Spawn[${toonControl}].Class.ShortName}Point}*.${tmpHealOverride}) {
				/varset hurtID ${Spawn[pc ${toonControl}].ID}
				/varset hurtPctHP ${Spawn[pc ${toonControl}].PctHPs}
				/varset hurtCLS ${Spawn[pc ${toonControl}].Class.ShortName}
			} 
		}
	}

	| Do group members need healing?
	/if (${Bool[${Group.Members}]}) {
		/for _count 0 to ${Group.Members}

			| no healing the rampage tank
			/if (${Bool[${RampageTank}]} && ${RampageTank.Equal[${Group.Member[${_count}].DisplayName}]}) /continue

			| no heallist bitches
			/if (${heallist.Find[${Group.Member[${_count}].DisplayName}]}) /continue

			/if (${Spawn[${Group.Member[${_count}]}].ID} && ${Group.Member[${_count}].Distance} <= ${MobAgro}) {
				/if (!${Group.Member[${_count}].Dead} && ${Group.Member[${_count}].PctHPs} <= ${hurtPctHP} && ${Group.Member[${_count}].PctHPs} <= ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}) {
					/varset hurtID ${Group.Member[${_count}].ID}
					/varset hurtPctHP ${Group.Member[${_count}].PctHPs}
					/varset hurtCLS ${Group.Member[${_count}].Class.ShortName}
				} 

		 		| set count for # of toons hurt
				/if (!${Group.Member[${_count}].Dead} && ${Group.Member[${_count}].PctHPs} < ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}) {
					/varcalc hurtCount ${hurtCount}+1
				}   
			}
	    
	  /next _count
	}
	
	CHECKEXIT
	UPDATEHUD "HEAL"
	CHECKREZ
	/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
	
	
	| Group regeneration
 	/if (${useGroupHoT} && !${hurtCount}) { 
 		/if (${SpawnCount[npc radius ${MobAgro} playerstate 4 targetable]}) {
	 	 	/if (${Me.SpellReady[${healWholeGroupHoT}]} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${Bool[${Me.Song[${healWholeGroupHoT}]}]} && !${Bool[${Me.Song[Elixir of the Seas]}]} && !${Bool[${Me.Song[Elixir of Wulthan]}]}) {
		    /if (${validate_cast[FALSE, spell, "${healWholeGroupHoT}", ${hurtID}]}) /call core_cast2 "${healWholeGroupHoT}" 0 ${Me.ID} FALSE
		  }
		}
	}	
	
	| no one is hurt. we're out of here
	/if (!${hurtCount}) /return

	CHECKREZ
	
	/call check_no_cast_buffs

  | stationary ward heal
  /if (${hurtCount} >= 3 && ${Me.AltAbilityReady[Call of the Ancients]}) {
    /if (${validate_cast[FALSE, alt, "Call of the Ancients", ${Me.ID}]}) /call core_cast2 "Call of the Ancients" alt ${Me.ID} FALSE
  } 

	| aggressive switch sort 	
  /if (${switchHealAggressive}) {
 		/if (${Me.SpellReady[${healReckless1}]} || ${Me.SpellReady[${healReckless2}]} || ${Me.SpellReady[${healReckless3}]}) /goto :recklessheals
 	} 	
 	
	| intervention/main heals
	/if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/if (${Me.SpellReady[${healIntervention1}]}) {
			/if (${validate_cast[FALSE, spell, "${healIntervention1}", ${hurtID}]}) /call core_cast2 "${healIntervention1}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
		} else /if (${Me.SpellReady[${healMain}]}) {
			/if (${validate_cast[FALSE, spell, "${healMain}", ${hurtID}]}) /call core_cast2 "${healMain}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
		} 
	} 
  
 	| Reckless Heals
	:recklessheals
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${healReckless${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healReckless${_count}}", ${hurtID}]}) /call core_cast2 "${healReckless${_count}}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
				/break
			}
		/next _count 
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

	UPDATEHUD FALSE
/return



|***
 *	DES: Heals xtarget people
 *	USE: 
 *	INI: the protect ones.
 *  NOTE: 
 ***|
sub check_heal_XTarget_SHM()
	/if (!${validate_expansion[FALSE, |16]}) /return
	CHECKREZ
	AMIDEAD
	SIMPLEEXIT
	CHECKINPUTS
	
	/declare _count int local 0
	/varset hurtID 				${Me.ID}
	/varset hurtPctHP 					${If[${HealOverride},${Float[${HealOverride}].Centi},99.99]}
	/varset hurtCLS 				${Me.Class.ShortName}
	/varset hurtCount					0

	| does anyone need healing?
	/for _count 1 to 13
		
		| no xtarget position
  	/if (!${Me.XTarget[${_count}].ID}) /continue
  	
  	| distance bequond agro?
  	/if (${Me.XTarget[${_count}].Distance} > ${MobAgro}) /continue

		| no heal rampage tanks
		/if (${Bool[${RampageTank}]} && ${RampageTank.Equal[${Me.XTarget[${_count}].DisplayName}]}) /continue
	
		| no heallist bitches
		/if (${heallist.Find[${Me.XTarget[${_count}].DisplayName}]}) /continue
  
  	| dont heal NPCs on the list unless they are part of the protected list
  	/if (!${Me.XTarget[${_count}].Type.Equal[PC]} && !${ProtectNPCList.Find[${Me.XTarget[${_count}].DisplayName}]}) /continue
  	
		| if not hurt enough, skip'em
		/if (!${Me.XTarget[${_count}].ID} || ${Me.XTarget[${_count}].PctHPs} > ${${Me.XTarget[${_count}].Class.ShortName}Point}*.${tmpHealOverride}) /continue
		
		| out of whomever is left, which is hurt the worse?	
		/if (!${Me.XTarget[${_count}].Dead} && ${Me.XTarget[${_count}].PctHPs} <= ${hurtPctHP}) {
			/varset hurtID ${Me.XTarget[${_count}].ID}
			/varset hurtPctHP ${Me.XTarget[${_count}].PctHPs}
			/varset hurtCLS ${Me.XTarget[${_count}].Class.ShortName}
			/varset hurtCount 1
		} 
  /next _count
  	
	UPDATEHUD "HEAL XTarget"
	/if (!${hurtCount}) /return	

	/call check_no_cast_buffs

	| aggressive switch
  /if (${switchHealAggressive}) {
 		/if (${Me.SpellReady[${healReckless1}]} || ${Me.SpellReady[${healReckless2}]} || ${Me.SpellReady[${healReckless3}]}) /goto :recklessheals
 	} 	
 	
	| intervention/main heals
	/if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/if (${Me.SpellReady[${healIntervention1}]}) {
			/if (${validate_cast[FALSE, spell, "${healIntervention1}", ${hurtID}]}) /call core_cast2 "${healIntervention1}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
		} else /if (${Me.SpellReady[${healMain}]}) {
			/if (${validate_cast[FALSE, spell, "${healMain}", ${hurtID}]}) /call core_cast2 "${healMain}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
		} 
	} 

	| reckless heals
	:recklessheals
	/if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
  	/if (${Me.SpellReady[${healReckless1}]}) {
    	/if (${validate_cast[FALSE, spell, "${healReckless1}", ${hurtID}]}) /call core_cast2 "${healReckless1}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
    } else /if (${Me.SpellReady[${healReckless2}]}) {
      /if (${validate_cast[FALSE, spell, "${healReckless2}", ${hurtID}]}) /call core_cast2 "${healReckless2}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
    } else /if (${Me.SpellReady[${healReckless3}]}) {
      /if (${validate_cast[FALSE, spell, "${healReckless3}", ${hurtID}]}) /call core_cast2 "${healReckless3}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
    } 
  }
		
	UPDATEHUD FALSE  
/return



|***
 *	DES: Checks and Heals Pets. Group ONLY
 *	USE: /heal pet
 *	INI: 
 *  NOTE: 
 ***|
sub check_pet_heal_SHM()
	AMIDEAD
	SIMPLEEXIT
	CHECKINPUTS
	CHECKREZ

	/declare _count int local
	/varset hurtID 				${Me.ID}
	/varset hurtPctHP 		100
  
  /for _count 0 to ${Group}
    /if (${Group.Member[${_count}].Pet.ID}) {
      /if (${Group.Member[${_count}].Pet.Distance} <= ${MobAGro}) {
        /if (${Group.Member[${_count}].Pet.PctHPs} < ${hurtPctHP} && ${Group.Member[${_count}].Pet.PctHPs} < ${PetPoint}) {
          /varset hurtID ${Group.Member[${_count}].Pet.ID}
          /varset hurtPctHP ${Group.Member[${_count}].Pet.PctHPs}
        }
      }
    }
  /next _count

	| any pets hurt?
  /if (${hurtID} == ${Me.ID}) /return
 	UPDATEHUD "HEAL Pet"
	
	| aggressive switch sort 	
  /if (${switchHealAggressive}) {
 		/if (${Me.SpellReady[${healReckless1}]} || ${Me.SpellReady[${healReckless2}]} || ${Me.SpellReady[${healReckless3}]}) /goto :recklessheals
 	} 	
 	
	| intervention/main heals
	/if (${Spawn[${hurtID}].PctHPs} <= ${PetPoint}*.${tmpHealOverride}) {
		/if (${Me.SpellReady[${healIntervention1}]}) {
			/if (${validate_cast[FALSE, spell, "${healIntervention1}", ${hurtID}]}) /call core_cast2 "${healIntervention1}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
		} else /if (${Me.SpellReady[${healMain}]}) {
			/if (${validate_cast[FALSE, spell, "${healMain}", ${hurtID}]}) /call core_cast2 "${healMain}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
		} 
	} 
  
 	| Reckless Heals
	:recklessheals
  /if (${Spawn[${hurtID}].PctHPs} <= ${PetPoint}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${healReckless${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healReckless${_count}}", ${hurtID}]}) /call core_cast2 "${healReckless${_count}}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
				/break
			}
		/next _count 
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
	/if (!${Bool[${TC1}]} && !${Bool[${TC1}]}) /return
	
	CHECKEXIT
	AMIDEAD
	CHECKINPUTS
	
	| nuke to engage twin heal proc
	/if (${Target.ID} == ${Me.ID}) /squelch /target clear
	/if (!${Me.SpellReady[${TC1}]} && !${Me.SpellReady[${TC2}]}) /return

	/declare _tmpID int local
	
	/if (${useSmartAssistG}) {
		/varset _tmpID ${Spawn[pc ${Group.MainAssist}].ID}
	} else /if (${useSmartAssist1}) {
		/varset _tmpID ${Spawn[pc ${Raid.MainAssist}].ID}
	} else /if (${useSmartAssist2}) {
	} else /if (${useSmartAssist3}) {
	} else /if (${Bool[${MA1}]} && ${Bool[${Spawn[pc ${MA1}].ID}]} && !${Bool[${Spawn[pc ${MA1}].Dead}]}) {
		/varset _tmpID ${Spawn[pc ${MA1}].ID}
	} else /if (${Bool[${MA2}]} && ${Bool[${Spawn[pc ${MA2}].ID}]} && !${Bool[${Spawn[pc ${MA2}].Dead}]}) {
		/varset _tmpID ${Spawn[pc ${MA2}].ID}
	}

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
sub check_HP_SHM()
	AMIDEAD
	/if (!${Me.Casting.ID}) /return
	/declare _count int local 0

	/varset hurtID 					${Me.ID}
	/varset hurtPctHP 			${If[${HealOverride},${Float[${HealOverride}].Centi},99.99]}
	/varset hurtCLS 				${Me.Class.ShortName}

	/if (${Bool[${Group.Members}]}) {
		/for _count 0 to ${Group.Members}
	  	/if (${Bool[${RampageTank}]} && ${RampageTank.Equal[${Group.Member[${_count}].DisplayName}]}) /continue
	  	/if (${heallist.Find[${Group.Member[${_count}].DisplayName}]}) /continue
			SIMPLEEXIT
			/if (${Group.Member[${_count}].Type.NotEqual[Corpse]} && ${Group.Member[${_count}].ID}) {
				/if (${Group.Member[${_count}].PctHPs} <= ${hurtPctHP} || !${hurtID}) {
				/varset hurtID ${Group.Member[${_count}].ID} 
				/varset hurtPctHP ${Group.Member[${_count}].PctHPs} 
				/varset hurtCLS ${Group.Member[${_count}].Class.ShortName} 
			}
		}
		/next _count
	}

	/if (${hurtID}  == ${Me.ID}) /return
	
	| aggressive switch sort 	
  /if (${switchHealAggressive}) {
 		/if (${Me.SpellReady[${healReckless1}]} || ${Me.SpellReady[${healReckless2}]} || ${Me.SpellReady[${healReckless3}]}) /goto :recklessheals
 	} 	
 	
	| intervention/main heals
	/if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/if (${Me.SpellReady[${healIntervention1}]}) {
			/if (${validate_cast[FALSE, spell, "${healIntervention1}", ${hurtID}]}) /call core_cast2 "${healIntervention1}" 0 ${hurtID} FALSE
		} else /if (${Me.SpellReady[${healMain}]}) {
			/if (${validate_cast[FALSE, spell, "${healMain}", ${hurtID}]}) /call core_cast2 "${healMain}" 0 ${hurtID} FALSE
		} 
	} 
  
 	| Reckless Heals
	:recklessheals
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${healReckless${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healReckless${_count}}", ${hurtID}]}) /call core_cast2 "${healReckless${_count}}" 0 ${hurtID} FALSE
				/break
			}
		/next _count 
	}
/return



|***
 *	DES: MGB Shaman Ancestral Aid if both AAs are ready
 *	USE: /bc spice, /spice
 *	INI: 
 *  NOTE: Will not cast Aid if the buff is present.
 ***|
sub doAid()
	AMIDEAD

	CHECKINPUTS
	CHECKEXIT
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
	CHECKEXIT
	CHECKINPUTS

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
	/if (!${switchTalisman}) /return	
	/if (${Me.Buff[${spellTalisman}].ID}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/call create_timer timer_cast_Talisman ${tTalisman}
	/if (${validate_cast[FALSE, spell, "${spellTalisman}", ${Me.ID}]}) /call core_cast2 "${spellTalisman}" 0 ${Me.ID} FALSE
/return



|***
 *	DES: We want Mana
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub check_cani()
	CHECKEXIT
	CHECKINPUTS
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
			/if (${validate_cast[FALSE, spell, "${spellCannibalization}", ${Me.ID}]}) /call core_cast2 "${spellCannibalization}" 0 0 check_HP_${Me.Class.ShortName}
			/return
		}

		| use the aa cani
		/if (${validate_cast[FALSE, alt, "Cannibalization", ${Me.ID}]}) /call core_cast2 "Cannibalization" alt 0 check_HP_${Me.Class.ShortName}
	}
	
/return



|***
 *	DES: Class controls
 *	USE: /shm []
 *	INI: 
 *  NOTE:
 ***|
#bind setSHM /shm
sub Bind_setSHM(string _classType, string _verbage)
	/if (!${validate_class[TRUE, |SHM]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _type string	local |aggressive|cannihps|cannimanna|fade|groupheal|grouphot|healpoint|rampage|rod|spiregroup|spireraid|surge|talisman|xtradius|xttime

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

	| set AA fade use %	
	} else /if (${Bool[${_classType.Equal[fade]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI useAAFadePct Buffs ${_verbage}
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} fade(${cnum}${useAAFadePct}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		} else {
		 	/call writeINI switchAAFade Buffs ${If[${switchAAFade},FALSE,TRUE]}  
		}

	| use Modulation Rods
	} else /if (${Bool[${_classType.Equal[rod]}]}) {
		/call writeINI switchModRod Buffs ${If[${switchModRod},FALSE,TRUE]} 	

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
		/if (${_type.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}(${If[${Bool[${switchModRod}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}(${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]})

		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]

/return

