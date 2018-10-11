|***
 *	class.pal.inc
 * 	Core Paladin Include
 *
 *	
 ***|
 


|***
 *	DES: Main Paladin Routine
 *	USE: primarly for making unicorn jokes at. also useful for throwing at raid mobs as speed bumps.
 *	INI: 
 *  NOTE: 
 ***|
sub core_PAL()

		/if (${coreAuto}) {
		  /if (${switchHealXtarget} && !${timer_set_xtarget}) {
		    /call set_xtarget
		    /call create_timer timer_set_xtarget ${tXHeal}
		  }	
			/if (${useShieldFlash}) /call cast_AA_shield_flash
			| aa LoH
			/if (${switchHealSelf} && ${useLoH}) {
			  /if (${Me.PctHPs} <= ${useLoHPct}*.${tmpHealOverride}) {
					/if (${validate_cast[FALSE, alt, "Lay on Hands", ${Me.ID}]}) /call core_cast2 "Lay on Hands" alt ${Me.ID} FALSE
				}
			}	
	
			/if (${useMG}) {
				/if (!${flagCallNeedRez} && !${timer_check_MG}) /call cast_AA_marrs_gift
			}
		}
		
	/while (${prep_combat[]}) {
		/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
		/if (${exit_class_loop[]}) /return

		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${switchHealXtarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/if (${switchHealPet}) /call check_pet_heal_${Me.Class.ShortName}
		/if (${switchHealCure}) /call cast_cure

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${setBurnControl}) /call synergy_${Me.Class.ShortName}
			/if (${switchPcSwarm} || ${switchPcPet}) /call send_pet
			/if (${switchAgro}) /call get_agro_${Me.Class.ShortName}
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
			/if (${switchHarmonious}) /call check_harmonious
			/if (${switchSteel} && !${timer_check_Steel}) /call check_steel	
			/if (${switchStance} && (!${timer_check_Stance} || !${Bool[${Me.Buff[${Stance}]}]})) /call check_stance
		}

		/if (${coreAuto}) {	
			/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
			/if (${switchModRod} && ${validate_class[FALSE, ${CastList}]}) /call use_mod_rod
			/if (${validate_class[FALSE, ${MeleeList}]}) /call cast_stamina_recovery
			/if (${doYaulp}) /call cast_yaulp
		}
	}

/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub disc_PAL()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	CHECKTIE

	/if (${validate_cast[FALSE, activedisc, "${DiscFastRune}", ${Me.ID}]}) /call core_cast2 "${DiscFastRune}" disc 0 FALSE
	/if (${switchPull}) /call set_combat ${combat.Upper}

	| defensive	
	/if (${coreAuto} && ${switchAgro} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]} >= ${defCount} && !${Bool[${Me.ActiveDisc}]}) {
		/declare _count int local 0
		/for _count 1 to 3
			| /if (${Bool[${Me.ActiveDisc}]}) /break
			/if (${validate_cast[FALSE, activedisc, "${DefensiveDisc${_count}}", ${Me.ID}]}) /call core_cast2 "${DefensiveDisc${_count}}" disc 0 FALSE
		/next _count
	}
		
	/if (${useShieldFlash}) /call cast_AA_shield_flash
	
	| aa LoH
	/if (${switchHealSelf}) {
		/if (${useLoH}) {
		  /if (${Me.PctHPs} <= ${useLoHPct}.${tmpHealOverride}) {
				/if (${validate_cast[FALSE, alt, "Lay on Hands", ${Me.ID}]}) /call core_cast2 "Lay on Hands" alt ${Me.ID} FALSE
			}
		}	
	}

	| AA inquisitors armor stacking control
	/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 30 targetable]} >= ${defCount}) {
		/if (!${Me.Song[[${AAGroupInquisitor}].ID}) { 
			/if (${validate_cast[FALSE, alt, "${AAInquisitor}", ${Me.ID}]}) /call core_cast2 "${AAInquisitor}" alt 0 FALSE

		| dont use group armor in raids
		} else /if (!${Me.Song[${AAInquisitor}].ID} && !${Raid.Members}) {
			/if (${validate_cast[FALSE, alt, "${AAGroupInquisitor}", ${Me.ID}]})  /call core_cast2 "${AAGroupInquisitor}" alt 0 FALSE
		}
	}

	| undead cripple
	/if (${Target.Body.Name.Equal[Undead]}) {
		/if (${validate_cast[FALSE, alt, "Vanquish the Fallen", ${Target2Kill}]}) /call core_cast2 "Vanquish the Fallen" alt ${Target2Kill} FALSE
	}
	
	| Yaulp.. all the time!!
	| /if (${doYaulp}) /call cast_yaulp
	
/return



|***
 *	DES: emergency healing
 *	USE: /call patch_heal_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_PAL()
	AMIDEAD
	CHECKEXIT
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	/if (${useShieldFlash}) /call cast_AA_shield_flash

	| Hand of Piety AA Group heal	
	/if (${Me.PctHPs} < ${${Me.Class.ShortName}Point}*.50*.${tmpHealOverride} && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${combatRadius} targetable]} >= ${defCount} ) {
		/if (${validate_cast[FALSE, alt, "${AAHandofPiety}", ${Me.ID}]}) /call core_cast2 "${AAHandofPiety}" alt 0 FALSE
	}
	
	| AA Gift of Life
	/if (${Me.PctHPs} < ${${Me.Class.ShortName}Point}*.30*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, alt, "Gift of Life", ${Me.ID}]}) /call core_cast2 "Gift of Life" alt 0 FALSE
	}	

	| Panic Heal 1
	/if (${Me.Gem[${healPanic1}]}) {
	  /if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.45*.${tmpHealOverride}) {
			/if (${validate_cast[FALSE, spell, "${healPanic1}", ${Me.ID}]}) /call core_cast2 "${healPanic1}" 0 0 FALSE
		}
	}

	| dpsheal
	/if (${Me.PctHPs} < ${${Me.Class.ShortName}Point}*.40*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, disc, "${DiscReflexive}", ${Target2Kill}]}) /call core_cast2 "${DiscReflexive}" disc ${Target2Kill} FALSE
	}

	| Panic Heal 2
	/if (${Me.Gem[${healPanic2}]}) {
	  /if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.20*.${tmpHealOverride}) {
			/if (${validate_cast[FALSE, spell, "${healPanic2}", ${Me.ID}]}) /call core_cast2 "${healPanic2}" 0 0 FALSE
		}
	}
	
	| aa LoH
	/if (${useLoH}) {
	  /if (${Me.PctHPs} <= ${useLoHPct}.${tmpHealOverride}) {
			/if (${validate_cast[FALSE, alt, "Lay on Hands", ${Me.ID}]}) /call core_cast2 "Lay on Hands" alt ${Me.ID} FALSE
		}
	}
	
	/call click_item_heal
/return



|***
 *	DES: I'm more important then you are!
 *	USE: /call get_agro_PAL
 *	INI: 
 *  NOTE: 
 ***|
sub get_agro_PAL()
	AMIDEAD
	CHECKEXIT
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${switchPull}) /call set_combat ${combat.Upper}
	/if (${useShieldFlash}) /call cast_AA_shield_flash
	
	| aa LoH
	/if (${switchHealSelf}) {
		/if (${useLoH}) {
		  /if (${Me.PctHPs} <= ${useLoHPct}.${tmpHealOverride}) {
				/if (${validate_cast[FALSE, alt, "Lay on Hands", ${Me.ID}]}) /call core_cast2 "Lay on Hands" alt ${Me.ID} FALSE
			}
		}	
	}	

	| challenge
	/call cast_challenge ${Target2Kill}

	| chain stun
	/if (${validate_cast[FALSE, alt, "${chainStun}", ${Target2Kill}]}) /call core_cast2 "${chainStun}" alt ${Target2Kill} FALSE

	| AE Stuns
	/if (${AEMelee}) {
		/varset mobAECount ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 60 playerstate 4 targetable]}
		
		/if (${mobAECount} > 1) {
			/if (!${Bool[${Raid.Members}]} || ${Bool[${Raid.Members}]} && ${Raid.MainAssist.Name.Equal[${Me.DisplayName}]}) {
				/if (${validate_cast[FALSE, alt, "${AAAELodestar}", ${Target2Kill}]}) /call core_cast2 "${AAAELodestar}" alt ${Target2Kill} FALSE
			}
		} else /if (${mobAECount} > 4) {
			/if (!${Bool[${Raid.Members}]} || ${Bool[${Raid.Members}]} && ${Raid.MainAssist.Name.Equal[${Me.DisplayName}]}) {
				/if (${validate_cast[FALSE, alt, "${AAAEBeacon}", ${Me.ID}]}) /call core_cast2 "${AAAEBeacon}" alt 0 FALSE
			}
		}
	}

	| AA pet hate (the little sword thingie)
	/if (${Me.PctAggro} < 80 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
		/if (${validate_cast[FALSE, alt, "Projection of Piety", ${Target2Kill}]}) /call core_cast2 "Projection of Piety" alt ${Target2Kill} FALSE
	}

	| snap hate
	/if (!${Raid.Members}) {
		/if (${Me.PctAggro} < 60 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
			/if (${validate_cast[FALSE, alt, "Ageless Enmity", ${Target2Kill}]}) /call core_cast2 "Ageless Enmity" alt ${Target2Kill} FALSE
		}
	}

	| named fake hate
	/if ((${Me.PctAggro} < 60 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) && ${NameList.Find[${Target.DisplayName}]}) {
		/if (${validate_cast[FALSE, disc, "${DiscUnflinching}", ${Target2Kill}]}) /call core_cast2 "${DiscUnflinching}" disc ${Target2Kill} FALSE
	}

	| Valiant Defense. agro+heal
	/if (${Me.PctAggro} < 95) {
		/if (${validate_cast[FALSE, spell, "${ValiantDefense}", ${Target2Kill}]}) /call core_cast2 "${ValiantDefense}" 0 ${Target2Kill} FALSE
	}
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_PAL()
	/if (${is_target_dead[${Target2Kill}]}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/call click_horn_of_unity 	
	/if (${useShieldFlash}) /call cast_AA_shield_flash
	
	| aa LoH
	/if (${switchHealSelf}) {
		/if (${useLoH}) {
		  /if (${Me.PctHPs} <= ${useLoHPct}.${tmpHealOverride}) {
				/if (${validate_cast[FALSE, alt, "Lay on Hands", ${Me.ID}]}) /call core_cast2 "Lay on Hands" alt ${Me.ID} FALSE
			}
		}	
	}	
	
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]} && ${Spawn[${Target2Kill}].Distance} <= ${CombatDistance}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE

/return



|***
 *	DES: Palading group heal. centered more to splashing
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub check_heal_group_PAL()
	AMIDEAD
	CHECKEXIT
 	CHECKINPUTS	
	CHECKTIE
	/declare _count int local 0
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
	
			| toon even in zone
			/if (!${Spawn[${Group.Member[${_count}]}].ID}) /continue

			| rampage tank
			/if (${Bool[${RampageTank}]} && ${RampageTank.Equal[${Group.Member[${_count}].DisplayName}]}) /continue

			| no heallist bitches
			/if (${heallist.Find[${Group.Member[${_count}].DisplayName}]}) /continue

			/if (${Group.Member[${_count}].Distance} <= ${MobAgro}) {
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
  
	UPDATEHUD "HEAL"
	/if (${useShieldFlash}) /call cast_AA_shield_flash

	| aa LoH
	/if (${switchHealSelf}) {
		/if (${useLoH}) {
		  /if (${Me.PctHPs} <= ${useLoHPct}.${tmpHealOverride}) {
				/if (${validate_cast[FALSE, alt, "Lay on Hands", ${Me.ID}]}) /call core_cast2 "Lay on Hands" alt ${Me.ID} FALSE
			}
		}	
	}
  
	| Panic Splash
	/if (${Bool[${Group.Members}]}) {
	  /if (${Spawn[${hurtID}].PctHPs} <= 45*.${tmpHealOverride}) {
			/if (${validate_cast[FALSE, spell, "${splashPanic}", ${Me.ID}]}) /call core_cast2 "${splashPanic}" 0 0 FALSE
		}
	}
		
	| group heals
	/if (${useGroupHeal}) {
		| Group Splash
	  /if (${hurtCount} >= 1) {
	    /if (${validate_cast[FALSE, spell, "${splashGroup}", ${Me.ID}]}) /call core_cast2 "${splashGroup}" 0 0 FALSE
	  } 
			
		| AA Splash
	  /if (${hurtCount} >= 3) {
	  	/if (${validate_cast[FALSE, alt, "${AAHandofPiety}", ${Me.ID}]}) /call core_cast2 "${AAHandofPiety}" alt 0 FALSE
	  }
	}

	AMIDEAD
 	CHECKINPUTS
	| Touch Heal 
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
  	/if (${validate_cast[FALSE, spell, "${healTouch}", ${hurtID}]}) /call core_cast2 "${healTouch}" 0 ${hurtID} FALSE
  }
	 
	UPDATEHUD
/return



|***
 *	DES: Check XTargets and heal the bastards if the need it.
 *	USE: /call check_heal_XTarget_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub check_heal_XTarget_PAL()
	/if (!${validate_expansion[FALSE, |16]}) /return
	CHECKREZ
	AMIDEAD
	SIMPLEEXIT
	CHECKINPUTS
	
	/declare _count int local 0
	/varset hurtID 					${Me.ID}
	/varset hurtPctHP 			${If[${HealOverride},${Float[${HealOverride}].Centi},99.99]}
	/varset hurtCLS 				${Me.Class.ShortName}
	/varset hurtCount				0

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

  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/if (${Me.SpellReady[${healTouch}]}) {
		  /if (${validate_cast[FALSE, spell, "${healTouch}", ${hurtID}]}) /call core_cast2 "${healTouch}" 0 ${hurtID} check_HP_${Me.Class.ShortName} 
		}
	}

	UPDATEHUD FALSE
/return



|***
 *	DES: Checks and Heals Pets. Group ONLY
 *	USE: /healpet
 *	INI: 
 *  NOTE: 
 ***|
sub check_pet_heal_PAL()
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
   /if (${useShieldFlash}) /call cast_AA_shield_flash

	| aa LoH
	/if (${switchHealSelf}) {
		/if (${useLoH}) {
		  /if (${Me.PctHPs} <= ${useLoHPct}.${tmpHealOverride}) {
				/if (${validate_cast[FALSE, alt, "Lay on Hands", ${Me.ID}]}) /call core_cast2 "Lay on Hands" alt ${Me.ID} FALSE
			}
		}	
	}

	| touch heal
  /if (${Spawn[${hurtID}].PctHPs} < ${PetPoint} ) {
    /if (${validate_cast[FALSE, spell, "${healTouch}", ${hurtID}]}) /call core_cast2 "${healTouch}" 0 ${hurtID} FALSE
  }
/return



|***
 *	DES: Fast check to see if anyone is hurt worse then person currently being healed.
 *	USE: added as a subcheck for core_cast2
 *	INI: 
 *  NOTE: 
 ***|
sub check_HP_PAL()
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
	/if (${useShieldFlash}) /call cast_AA_shield_flash

	| aa LoH
	/if (${switchHealSelf}) {
		/if (${useLoH}) {
		  /if (${Me.PctHPs} <= ${useLoHPct}.${tmpHealOverride}) {
				/if (${validate_cast[FALSE, alt, "Lay on Hands", ${Me.ID}]}) /call core_cast2 "Lay on Hands" alt ${Me.ID} FALSE
			}
		}	
	}

	| touch heal
	/if (${Spawn[${hurtID}].PctHPs} < 20*.${HealOverride}) {
		/if (${Me.Casting.ID}) /stopcast
		/if (${validate_cast[FALSE, spell, "${healTouch}", ${hurtID}]}) /call core_cast2 "${healTouch}" 0 ${hurtID}
	} 
/return



|***
 *	DES: Mar's Gift 30ish k HP/15k manna 
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub cast_AA_marrs_gift()
	AMIDEAD
	SIMPLEEXIT
	/call create_timer timer_check_MG ${tcheck_MG}
	/if (!${validate_cast[FALSE, alt, "Marr's Gift", ${Me.ID}]}) /return
	/if (${timer_zone_pause} || ${SafeZone} && !${switchOverrideSafezone}  || ${Me.Sitting}) /return
	
	/if (${Me.PctMana} < ${useMGPct}*.${tmpHealOverride}) {
		/call core_cast2 "Marr's Gift" alt 0 FALSE
	} else /if (${switchHealSelf} && ${Me.PctHPs} < ${useMGPct}*.${tmpHealOverride}) {
		/call core_cast2 "Marr's Gift" alt 0 FALSE
	}
/return	



|***
 *	DES: keeps Paladin Reinvigorating Steel applied
 *	USE: /call check_steel
 *	INI: [buffs] - Steel
 *  NOTE: 
 ***|
sub check_steel()
	/if (${SafeZone} && !${switchOverrideSafezone}) /return
	AMIDEAD
	SIMPLEEXIT

	/if (${validate_cast[FALSE, spell, "${Steel}", ${Me.ID}]}) {
		/call core_cast2 "${Steel}" 0 0 FALSE
		/call create_timer timer_check_Steel ${tcheck_Steel}
	}
/return



 |***
 *	DES: Paladin burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_PAL()
	/if (${is_target_dead[${Target2Kill}]}) /return
	AMIDEAD
	CHECKEXIT

	| defensive	
	/declare _count int local 0
	/for _count 1 to 3
		/if (${Bool[${Me.ActiveDisc}]}) /break
		/if (${validate_cast[FALSE, disc, "${DefensiveDisc${_count}}", ${Me.ID}]}) {
			/call core_cast2 "${DefensiveDisc${_count}}" disc 0 FALSE
			/break
		}
	/next _count

	| deflection
	/if (!${Bool[${Me.ActiveDisc}]} && ${Me.PctHPs} < 50*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, disc, "Deflection Discipline", ${Me.ID}]}) {
			/call core_cast2 "Deflection Discipline" disc 0 FALSE
			/return
		}
	}
	
	/if (${useShieldFlash}) /call cast_AA_shield_flash

	| aa LoH
	/if (${switchHealSelf}) {
		/if (${useLoH}) {
		  /if (${Me.PctHPs} <= ${useLoHPct}.${tmpHealOverride}) {
				/if (${validate_cast[FALSE, alt, "Lay on Hands", ${Me.ID}]}) /call core_cast2 "Lay on Hands" alt ${Me.ID} FALSE
			}
		}	
	}

	/call cast_AA_spire	

	/if (${validate_cast[FALSE, alt, "${AAJudgement}", ${Me.ID}]}) /call core_cast2 "${AAJudgement}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAValorousRage}", ${Me.ID} ]]}) /call core_cast2 "${AAValorousRage}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAShield}", ${Me.ID}]}) /call core_cast2 "${AAShield}" alt 0 FALSE
	
	/call use_glyph
	/call synergy_click_Circle
	/call click_item_burn
/return



|***
 *	DES: Class controls
 *	USE: /pal [] [##]
 *	INI: 
 *  NOTE:
 ***|
#bind setPAL /pal
sub Bind_setPAL(string _classType, string _verbage)
	/if (!${validate_class[FALSE, |PAL]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _type string	local |ae|defcount|fade|groupheal|harmonious|healpoint|loh|mg|rampage|rod|shieldflash|spiregroup|spireraid|stance|steel|stun|taunt|xtclass|xtradius|xttime|yaulp

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
	
	| to Yaulp or not		
	/if (${Bool[${_classType.Equal[yaulp]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI useYaulpPct Buffs ${_verbage}
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} yaulp [${cinfo}${useYaulpPct}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		} else {
		 	/call writeINI doYaulp Buffs ${If[${doYaulp},FALSE,TRUE]}  
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

	| AA Marr's Gift	Control	
	} else /if (${Bool[${_classType.Equal[mg]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${_verbage}]}) {
			/call writeINI useMGPct Buffs ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${_verbage}]}) {
			VOUT /${_bind} mg [${cinfo}##\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call writeINI useMG Buffs ${If[${useMG},FALSE,TRUE]}  
		}	

	| AA Lay on Hands	
	} else /if (${Bool[${_classType.Equal[loh]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${_verbage}]}) {
			/call writeINI useLoHPct Heal ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${_verbage}]}) {
			VOUT /${_bind} loh [${cinfo}##\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call writeINI useLoH Heal ${If[${useLoH},FALSE,TRUE]}  
		}	
		
	| Shield Flash oh shit
	} else /if (${Bool[${_classType.Equal[shieldflash]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${_verbage}]}) {
			/call writeINI useShieldFlashPct Discs ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${_verbage}]}) {
			VOUT /${_bind} shieldflash [${cinfo}##\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call writeINI useShieldFlash Discs ${If[${useShieldFlash},FALSE,TRUE]}  
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

	| use group Heals
	} else /if (${Bool[${_classType.Equal[groupheal]}]}) {
		/call writeINI useGroupHeal Heal ${If[${useGroupHeal},FALSE,TRUE]} 	
		
	| use Modulation Rods
	} else /if (${Bool[${_classType.Equal[rod]}]}) {
		/call writeINI switchModRod Buffs ${If[${switchModRod},FALSE,TRUE]} 	

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
		VOUT /${_bind} healpoint [${_listout}]
		/return
			
	| on/off taunt
	} else /if (${Bool[${_classType.Equal[taunt]}]}) {
		/call writeINI taunt meleedisc ${If[${taunt},FALSE,TRUE]}  

	| on/off switch for Steel
	} else /if (${Bool[${_classType.Equal[steel]}]}) {
		/call writeINI switchSteel Buffs ${If[${switchSteel},FALSE,TRUE]}  
		VOUT /${_bind} steel (${cnum}${Steel}\ax) ${sep} ${If[${switchSteel},${oenabled},${odisabled}]}
		/return
	
	| on/off switch for Harmonious
	} else /if (${Bool[${_classType.Equal[harmonious]}]}) {
		/call writeINI switchHarmonious Buffs ${If[${switchHarmonious},FALSE,TRUE]}  
		VOUT /${_bind} harmonious (${cnum}${Harmonious}\ax) ${sep} ${If[${switchHarmonious},${oenabled},${odisabled}]}
		/return

	| on/off switch for Stance
	} else /if (${Bool[${_classType.Equal[stance]}]}) {
		/call writeINI switchStance Buffs ${If[${switchStance},FALSE,TRUE]}  
		VOUT /${_bind} stance (${cnum}${Stance}\ax) ${sep} ${If[${switchStance},${oenabled},${odisabled}]}
		/return

	| set the XTarget class to heal
	} else /if (${Bool[${_classType.Equal[xtclass]}]}) {
		/if (${Bool[${_verbage}]} && ${AllClassList.Find[${_verbage}]}) {
			| if its there, delete
			/if (${XHealClass.Find[${_verbage}]}) {
				/declare _tmpList string local
				/for _count 1 to ${XHealClass.Count[|]}
					/if (${${XHealClass.Arg[${_count},|]}.Equal[FALSE]}) /continue
					/if (${AllClassList.Find[${XHealClass.Arg[${_count},|]}]}) /continue
					/if (${Bool[${XHealClass.Arg[${_count},|]}]}) /varset _tmpList ${_tmpList}|${XHealClass.Arg[${_count},|]}
					/echo ${_tmpList}
				/next _count
				/call writeINI XHealClass Heal ${If[${Bool[${_tmpList}]},${_tmpList},|FALSE]}
			| if its not there, add	
			}	else /if (!${XHealClass.Find[${_verbage}]} && ${AllClassList.Find[${_verbage}]}) {
				/call writeINI XHealClass Heal ${If[${Bool[${XHealClass}]},${XHealClass}|${_verbage.Upper},|${_verbage.Upper}]}
			}			
			
		  /if (${switchHealXtarget}) {
		    /call set_xtarget
		    /call create_timer timer_set_xtarget ${tXHeal}
			}			
			
		}
		/mqclear
		/for _count 1 to ${AllClassList.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${AllClassList.Arg[${_count},|].Upper}\ax(${If[${XHealClass.Find[${AllClassList.Arg[${_count},|]}]},${oon},${ooff}]})
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
		/return	
		
	| set XTar heal radius
	} else /if (${Bool[${_classType.Equal[xtradius]}]}) {
		/if (${Range.Between[1,300:${Int[${_verbage}]}]}) {
			/call writeINI XTHealRadius Heal ${_verbage}
		} else /if (!${Bool[${_verbage}]} || !${Range.Between[1,300:${Int[${_verbage}]}]}) {
			VOUT /${_bind} xtradius [${cinfo}${XTHealRadius}\ax] allowable range ${sep} ${cinfo}1 - 300\ax
			/return
		}	

	| set XT refresh list Time
	} else /if (${Bool[${_classType.Equal[xttime]}]}) {
		/if (${Range.Between[1,5:${Int[${_verbage}]}]}) {
			/call writeINI tXHeal Timer ${_verbage}
			/call echos number "XTarget refresh time" ${_verbage} 0 0
		} else /if (!${Bool[${_verbage}]} || !${Range.Between[1,5:${Int[${_verbage}]}]}) {
			VOUT /${_bind} xttime [${cinfo}${tXHeal}m\ax] allowable range ${sep} ${cinfo}1 - 5 min\ax
		}	

	| AE or Focused Melee
	} else /if (${Bool[${_classType.Equal[ae]}]}) {
		/call writeINI AEMelee Discs ${If[${AEMelee},FALSE,TRUE]}

	| set rampage tank toon
	} else /if (${Bool[${_classType.Equal[rampage]}]}) {
		/if (${Bool[${_verbage}]}) {
			/call writeINI RampageTank Heal ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		} else /if (!${Bool[${_verbage}]}) {
			/call writeINI RampageTank Heal FALSE
		}

	| Stun Control
	} else /if (${Bool[${_classType.Equal[stun]}]}) {
		/if (${Bool[${_verbage}]}) {
			/if (${Bool[${_verbage.Equal[stop]}]}) {
					/call writeINI chainStun Discs FALSE
			} else /if (${Bool[${_verbage.Find[divine]}]}) {
					/call writeINI chainStun Discs "Divine Stun"
			} else /if (${Bool[${_verbage.Find[force]}]}) {
					/call writeINI chainStun Discs "Force of Disruption"
			} else /if (${Bool[${_verbage.Find[disruptive]}]}) {
					/call writeINI chainStun Discs "Disruptive Persecution"
			}	
		} else {
			/declare _stunType |stop|divine|force|disruptive
			/for _count 1 to ${_stunType.Count[|]}
				/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_stunType.Arg[${_count},|]}\ax
				/varset _sep TRUE
			/next _count
			VOUT /${_bind} stun [${_listout}]
		}
		VOUT Chain Stun: ${cinfo}${chainStun}\ax
		/return
		
	| defensive mob agro count
	} else /if (${Bool[${_classType.Equal[defcount]}]}) {
		/if (${Range.Between[1,50:${Int[${_verbage}]}]}) {
			/call writeINI defCount Discs ${_verbage}
		} else /if (!${Bool[${_verbage}]} || !${Range.Between[1,50:${Int[${_verbage}]}]}) {
			VOUT /${_bind} defcount [${cinfo}${countMobAgro}\ax] allowable range ${sep} ${cinfo}1 - 50 min\ax
			/return
		}	

	}

	/if (${switchMQclearonCLS}) /mqclear
	/for _count 1 to ${_type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_type.Arg[${_count},|]}\ax
		/if (${_type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${_type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${_type.Arg[${_count},|].Equal[defcount]}) /varset _listout ${_listout}(${cnum}${defCount}\ax)
		/if (${_type.Arg[${_count},|].Equal[mg]}) /varset _listout ${_listout}(${If[${useMG},${cnum}${useMGPct}\ax,${cbad}${useMGPct}\ax]})
		/if (${_type.Arg[${_count},|].Equal[xtclass]}) /varset _listout ${_listout}(${cnum}->\ax)
		/if (${_type.Arg[${_count},|].Equal[xttime]}) /varset _listout ${_listout}(${cnum}${tXHeal}\ax)
		/if (${_type.Arg[${_count},|].Equal[xtradius]}) /varset _listout ${_listout}(${cnum}${XTHealRadius}\ax)
		/if (${_type.Arg[${_count},|].Equal[yaulp]}) /varset _listout ${_listout}(${If[${doYaulp},${cnum}${useYaulpPct}\ax,${cbad}${useYaulpPct}\ax]})
		/if (${_type.Arg[${_count},|].Equal[groupheal]}) /varset _listout ${_listout}(${If[${useGroupHeal},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[stance]}) /varset _listout ${_listout}(${If[${Bool[${switchStance}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[steel]}) /varset _listout ${_listout}(${If[${Bool[${switchSteel}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[harmonious]}) /varset _listout ${_listout}(${If[${Bool[${switchHarmonious}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[stun]}) /varset _listout ${_listout}(${If[${Bool[${chainStun}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}->\ax)
		/if (${_type.Arg[${_count},|].Equal[ae]}) /varset _listout ${_listout}(${If[${AEMelee},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[rampage]}) /varset _listout ${_listout}(${If[${Bool[${RampageTank}]},${cnum}${RampageTank}\ax,${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[shieldflash]}) /varset _listout ${_listout}(${If[${useShieldFlash},${cnum}${useShieldFlashPct}\ax,${cbad}${useShieldFlashPct}\ax]})
		/if (${_type.Arg[${_count},|].Equal[loh]}) /varset _listout ${_listout}(${If[${useLoH},${cnum}${useLoHPct}\ax,${cbad}${useLoHPct}\ax]})
		/if (${_type.Arg[${_count},|].Equal[taunt]}) /varset _listout ${_listout}(${If[${Bool[${taunt}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}(${If[${Bool[${switchModRod}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}(${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]})
		
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return



