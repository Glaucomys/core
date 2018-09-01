|***
 *	class.clr.inc
 *	Core Cleric Include
 *
 * 
 ***| 
 


|***
 *	DES: Main Cleric Routine
 *	USE: returning the dead to life 'cause they were stupid
 *	INI: 
 *  NOTE: 
 ***|
sub core_CLR()
	AMIDEAD
	CHECKINPUTS
	CHECKTIE
	/if (${Bool[${toonControl}]} && ${Bool[${toonControlXTslot}]} && ${toonControlProtect}) /call protect_always_xt

  /if (${switchHealXtarget}) {
  	/if (!${timer_set_xtarget}) {
	    /call set_xtarget
	    /call create_timer timer_set_xtarget ${tXHeal}
	  }	
	}
	
	CHECKREZ
	/if (${coreAuto} && !${Me.Invis}) {
		/if (${Bool[${sumWeaponItem}]} && (${InvSlot[mainhand].Item.Name.NotEqual[${sumWeaponItem}]} || !${Bool[${Me.Primary}]})) /call cast_sum_weapon
		/if (!${check_exit[]} && !${SafeZone} && !${Me.Buff[Revival Sickness].ID} && ${switchVP}) {
		 	/if (${Me.PctMana} < ${useVPPct}*.${tmpHealOverride} && !${flagCallNeedRez} && !${switchAmIDead}) {
		 		/if (${validate_cast[FALSE, alt, "Veturika's Perseverance", ${Me.ID}]}) /call core_cast2 "Veturika's Perseverance" alt 0 FALSE
		 	}
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
		/if (${doYaulp}) /call cast_yaulp

		/if (${isValidTarget}) {
			CHECKREZ
			/call set_combat ${combat.Upper}
			/if (${switchPcPet} && !${Me.Pet.ID}) /call cast_temp_pet_${Me.Class.ShortName}
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
		/if (${Me.SpellReady[${healWard}]} && !${timer_heal_ward}) /call check_heal_ward

		/if (${coreAuto}) {	
			/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
			/if (${switchModRod} && ${validate_class[FALSE, ${CastList}]}) /call use_mod_rod
			/if (${validate_class[FALSE, ${MeleeList}]}) /call cast_stamina_recovery
			/if (!${is_target_dead[${Target2Kill}]}) /goto :killtarget
		} else /if (!${coreAuto}) {
			/if (${Target.ID} && ${Bool[${combat}]} && !${is_target_dead[${Target.ID}]}) /goto :killtarget
		}
	}

/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_CLR()
	AMIDEAD
	CHECKEXIT
	/call click_horn_of_unity 	
/return



|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: Disc
 *	INI: 
 *  NOTE: 
 ***|
sub disc_CLR()
	AMIDEAD
	CHECKEXIT
	/if (${doYaulp}) /call cast_yaulp

	/if (${is_target_valid[${Target2Kill}]}) {
		/if (${Spawn[${Target2Kill}].Body.Name.Equal[Undead]}) {
			/if (${validate_cast[FALSE, alt, "Turn Undead", ${Target2Kill}]} && !${Raid.Members}) /call core_cast2 "Turn Undead" alt ${Target2Kill} FALSE
		}
	}
/return



|***
 *	DES: This will summon the clerics awesom nut smasher hammer if you do not have a good primary weapon.
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub cast_sum_weapon()
	/if (${Me.Primary}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (!${Bool[${FindItem[${sumWeaponItem}]}]}) {
		/if (${validate_cast[FALSE, spell, "${sumWeaponSpell}", ${Me.ID}]}) /call core_cast2 "${sumWeaponSpell}" ${buffGem} ${Me.ID} FALSE
	}
	/if (${Bool[${Cursor.ID}]}) {
		/call sort_cursor TRUE
	} else {
		/exchange "${sumWeaponItem}" mainhand 
	}
/return



|***
 *	DES: cleric main heal sub
 *	USE: /call check_heal_group_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: bread and butter baby...
 ***|
sub check_heal_group_CLR()
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
	
	| Regen Heals 
 	/if (${useGroupHoT} && !${hurtCount}) {
	 	/if (${SpawnCount[npc radius ${MobAgro} playerstate 4 targetable]} >= 1 && !${SafeZone}) {
	 	 	/if (${Me.AltAbilityReady[Celestial Regeneration]} && !${Raid.Members} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${Bool[${Me.Song[Focused Celestial Regeneration]}]} && !${Bool[${Me.Song[${healWholeGroupHoT}]}]} && !${Bool[${Me.Song[Shear of Renewal]}]} && !${Bool[${Me.Song[Ghost of Renewal]}]}) {
		 		/if (${validate_cast[FALSE, alt, "Celestial Regeneration", ${Me.ID}]}) /call core_cast2 "Celestial Regeneration" alt 0 FALSE
		  } else /if (${Me.SpellReady[${healWholeGroupHoT}]} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${Bool[${Me.Song[Focused Celestial Regeneration]}]} && !${Bool[${Me.Song[${healWholeGroupHoT}]}]} && !${Bool[${Me.Song[Shear of Renewal]}]} && !${Bool[${Me.Song[Ghost of Renewal]}]}) {
		    /if (${validate_cast[FALSE, spell, "${healWholeGroupHoT}", ${Me.ID}]}) /call core_cast2 "${healWholeGroupHoT}" 0 0 FALSE
		  }
		}
	}
	
	| if no one is hurt. back to the start
	/if (!${hurtCount}) /return	

	| Complete Heal
	| requires group roll Main Tank set to function
	/if (${switchCompleteHeal}) { 
		/if (${Spawn[${Group.MainTank.ID}].ID}) { 
			/if (${hurtID} == ${Group.MainTank.ID} && ${Spawn[${Group.MainTank.ID}].PctHPs} <= ${${Group.MainTank.Class.ShortName}Point}*.${CompleteHealpct}) {
				/if (${validate_cast[FALSE, spell, "Complete Heal", ${Group.MainTank.ID}]}) /call core_cast2 "Complete Heal" 0 ${Group.MainTank.ID} check_HP_${Me.Class.ShortName}
			}
		}
	}

	| Focused Celestial Regeneration
	/if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride} && ${Spawn[${hurtID}].Distance} <= ${Spell[Focused Celestial Regeneration].MyRange} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${SafeZone}) { 
		/if (${Me.AltAbilityReady[Focused Celestial Regeneration]}) {
			/if (${validate_cast[FALSE, alt, "Focused Celestial Regeneration", ${hurtID}]}) /call core_cast2 "Focused Celestial Regeneration" alt ${hurtID} FALSE
		}
	}
	
  | AA Spire
	/if (!${Bool[${Raid.Members}]} && ${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride} && ${Spawn[${hurtID}].Distance} <= ${Spell[${CLRspire3}].MyRange} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${SafeZone} && ${Me.Combat}) { 
    /if (${validate_cast[FALSE, alt, "${CLRspire3}", ${hurtID}]}) /call core_cast2 "${CLRspire3}" alt ${hurtID} FALSE
  }	
  
	| Dichotomic Heal
	/if (${Me.Song[Gift of Mana (${GoMLevel})].ID}) {
		/if (${Me.SpellReady[${GoMNuke}]} && ${Spawn[${hurtID}].PctHPs} < ${GoMpoint}*.${tmpHealOverride}) { 
			/call cast_nuke_GoM ${hurtID}
	  }
	}
  
	/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade

	| Panic AA 
	/if (${Me.AltAbilityReady[Burst of Life]}) {
	  /if (${Spawn[${hurtID}].PctHPs} <= 45*.${tmpHealOverride}) {
			/if (${validate_cast[FALSE, alt, "Burst of Life", ${hurtID}]}) /call core_cast2 "Burst of Life" alt ${hurtID} FALSE
	  }
	}
  
	| Panic heal
	/if (${Me.SpellReady[${healPanic}]}) {
		/if (${Spawn[${hurtID}].PctHPs} < 20*.${HealOverride}) {
			/if (${Me.Casting.ID}) /stopcast
			/if (${validate_cast[FALSE, spell, "${healPanic}", ${hurtID}]}) /call core_cast2 "${healPanic}" 0 ${hurtID} FALSE
		}
	}  

	| Arbitration
  /if (${Spawn[${hurtID}].PctHPs} < ${${hurtCLS}Point}*.${tmpHealOverride}*.5 || ${hurtCount} > 2) {
    /call cast_divine_arbitration
  }
  
  | Forced Group Heals ONLY
	/if (${switchForceGroupHeal}) {
		/if ( ${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
			/if (${Me.SpellReady[${healWholeGroup}]}) {
				/if (${validate_cast[FALSE, spell, "${healWholeGroup}", ${Me.ID}]}) /call core_cast2 "${healWholeGroup}" 0 0 FALSE
			} else /if (${Me.SpellReady[${healWholeGroup2}]}) {
				/if (${validate_cast[FALSE, spell, "${healWholeGroup2}", ${Me.ID}]}) /call core_cast2 "${healWholeGroup2}" 0 0 check_HP_${Me.Class.ShortName}
			} else /if (${Me.SpellReady[${healWholeGroup3}]}) {
				/if (${validate_cast[FALSE, spell, "${healWholeGroup3}", ${Me.ID}]}) /call core_cast2 "${healWholeGroup3}" 0 0 check_HP_${Me.Class.ShortName}
			}
		}
		| if forced group healing. restart heal cycle at this point.
		/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
		/return
	}
    
  | Group Heal
  /if (${useGroupHeal}) {
	  /if (${hurtCount} >= 2 && ${Me.SpellReady[${healWholeGroup}]}) {
	    /if (${validate_cast[FALSE, spell, "${healWholeGroup}", ${Me.ID}]}) /call core_cast2 "${healWholeGroup}" 0 0 FALSE
	    /return
	  } 
	}
  
  | ward of surety short buff 
	/if (!${Bool[${timer_heal_ward}]} && ${Me.SpellReady[${healWard}]}) /call check_heal_ward

	| check for personal debuffs
	/if (${switchHealCure}) {
		/if (${Debuff.Count}) {
			| AA Purified Spirits SELF ONLY 0' range 2 min
			/if (${validate_cast[FALSE, alt, "Purified Spirits", ${Me.ID}]}) /call core_cast2 "Purified Spirits" alt ${Me.ID} FALSE
			| AA Radient Cure SELF ONLY 100' range 1 min
			/if (${validate_cast[FALSE, alt, "Radiant Cure", ${Me.ID}]}) /call core_cast2 "Radiant Cure" alt 0 FALSE 
		}
	}
	
	| AA heal stuffs
  /if (${hurtCount} && !${SafeZone}) {
	  /if (${hurtCount} == 2) {
		  /if (${validate_cast[FALSE, alt, "Beacon of Life", ${Me.ID}]}) /call core_cast2 "Beacon of Life" alt 0 FALSE
		} else /if (${hurtCount} == 3) {
		 	/if (${validate_cast[FALSE, alt, "Channeling the Divine", ${Me.ID}]}) /call core_cast2 "Channeling the Divine" alt 0 FALSE
		} else /if (${hurtCount} == 4) {
 			/if (${validate_cast[FALSE, alt, "Healing Frenzy", ${Me.ID}]}) /call core_cast2 "Healing Frenzy" alt 0 FALSE
 			/if (${validate_cast[FALSE, alt, "Fury of Life", ${Me.ID}]}) /call core_cast2 "Fury of Life" alt 0 FALSE
 		} else /if (${hurtCount} >= 5) {
			/if (${validate_cast[FALSE, alt, "Exquisite Benediction", ${Me.ID}]}) /call core_cast2 "Exquisite Benediction" alt 0 FALSE
			/if (${validate_cast[FALSE, alt, "Silent Casting", ${Me.ID}]}) /call core_cast2 "Silent Casting" alt 0 FALSE
		} else /if (${hurtCount} == 6) {
			/if (${validate_cast[FALSE, alt, "Divine Retribution", ${Me.ID}]}) /call core_cast2 "Divine Retribution" alt 0 FALSE
			/call use_glyph
		}
  } 
	AMIDEAD
	/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
 	CHECKINPUTS
 	
 	| agressive healing sort
 	/if (${switchHealAggressive}) {
 		/if (${Me.SpellReady[${healIntervention1}]} || ${Me.SpellReady[${healIntervention2}]} || ${Me.SpellReady[${healIntervention3}]} || ${Me.SpellReady[${healIntervention4}]}) /goto :interventionheals
 	} 
 	
 	
 	| Remedy Heals 
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${healRemedy${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healRemedy${_count}}", ${hurtID}]}) /call core_cast2 "${healRemedy${_count}}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
				/break
			}
		/next _count 
	}	
	
	| Intervention Heals
	:interventionheals
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 4
			/if (${Me.SpellReady[${healIntervention${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healIntervention${_count}}", ${hurtID}]}) /call core_cast2 "${healIntervention${_count}}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
				/break
			}
		/next _count
	}
	
	| Renewal Heals
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${healRenewal${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healRenewal${_count}}", ${hurtID}]}) /call core_cast2 "${healRenewal${_count}}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
				/break
			}
		/next _count 
	}			

 	
|**
	| Remedy Heals 
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
    /if (${Me.SpellReady[${healRemedy1}]}) {
      /if (${validate_cast[FALSE, spell, "${healRemedy1}", ${hurtID}]}) /call core_cast2 "${healRemedy1}" 0 ${hurtID} check_HP_${Me.Class.ShortName} 
    } else /if (${Me.SpellReady[${healRemedy2}]}) {
      /if (${validate_cast[FALSE, spell, "${healRemedy2}", ${hurtID}]}) /call core_cast2 "${healRemedy2}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
    } else /if (${Me.SpellReady[${healRemedy3}]}) {
      /if (${validate_cast[FALSE, spell, "${healRemedy3}", ${hurtID}]}) /call core_cast2 "${healRemedy3}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
    } 
  }     	
		
	| Intervention Heals
	:interventionheals
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
  	/if (${Me.SpellReady[${healIntervention1}]}) {
     /if (${validate_cast[FALSE, spell, "${healIntervention1}", ${hurtID}]}) /call core_cast2 "${healIntervention1}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
    } else /if (${Me.SpellReady[${healIntervention2}]}) {
      /if (${validate_cast[FALSE, spell, "${healIntervention2}", ${hurtID}]}) /call core_cast2 "${healIntervention2}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
    } else /if (${Me.SpellReady[${healIntervention3}]}) {
      /if (${validate_cast[FALSE, spell, "${healIntervention3}", ${hurtID}]}) /call core_cast2 "${healIntervention3}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
    } else /if (${Me.SpellReady[${healIntervention4}]}) {
      /if (${validate_cast[FALSE, spell, "${healIntervention4}", ${hurtID}]}) /call core_cast2 "${healIntervention4}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
    }
  }

	CHECKREZ

	| Renewal Heals
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
    /if (${Me.SpellReady[${healRenewal1}]}) {
       /if (${validate_cast[FALSE, spell, "${healRenewal1}", ${hurtID}]}) /call core_cast2 "${healRenewal1}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
     } else /if (${Me.SpellReady[${healRenewal2}]}) {
       /if (${validate_cast[FALSE, spell, "${healRenewal2}", ${hurtID}]}) /call core_cast2 "${healRenewal2}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
     } else /if (${Me.SpellReady[${healRenewal3}]}) {
       /if (${validate_cast[FALSE, spell, "${healRenewal3}", ${hurtID}]}) /call core_cast2 "${healRenewal3}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
     }
  } 
**| 

	| netbots single heal over time
	/if (${Bool[${healHoT}]}) {
		/if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
			/if (${NetBots[${Spawn[ID ${hurtID}].DisplayName}].Stacks[${healHoT}]} && !${Bool[${NetBots[${Spawn[ID ${hurtID}].DisplayName}].ShortBuff.Find[${Spell[${healHoT}].ID}]}]}) {
				/if (${validate_cast[FALSE, spell, "${healHoT}", ${hurtID}]}) /call core_cast2 "${healHoT}" 0 ${hurtID} FALSE
			}
		}
	}

  | check for group debuffs
	/if (${switchHealCure}) {
		/if (${Bool[${Group.Members}]} && ${hurtCount}) /call cast_cure
	}
	
	UPDATEHUD FALSE
/return



|***
 *	DES: Check XTargets and heal the bastards if the need it.
 *	USE: /call check_heal_XTarget_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub check_heal_XTarget_CLR()
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

	| AA heal
	/if (${Spawn[${hurtID}].PctHPs} < 45*.${tmpHealOverride} && ${Me.AltAbilityReady[Burst of Life]}) {
		/if (${validate_cast[FALSE, alt, "Burst of Life", ${hurtID}]}) /call core_cast2 "Burst of Life" alt ${hurtID} FALSE
	}

	| panic heal
	/if (${Me.SpellReady[${healPanic}]}) {
		/if (${Spawn[id ${hurtID}].PctHPs} < 20*.${HealOverride}) {
			/if (${Me.Casting.ID}) /stopcast
			/if (${validate_cast[FALSE, spell, "${healPanic}", ${hurtID}]}) /call core_cast2 "${healPanic}" 0 ${hurtID} FALSE
		}
	}
	
	/if (${switchHealAggressive}) {
		/if (${Me.SpellReady[${healIntervention1}]} || ${Me.SpellReady[${healIntervention2}]} || ${Me.SpellReady[${healIntervention3}]} || ${Me.SpellReady[${healIntervention4}]}) /goto :interventionheals
	} 
	
	| Remedy Heals 
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${healRemedy${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healRemedy${_count}}", ${hurtID}]}) /call core_cast2 "${healRemedy${_count}}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
				/break
			}
		/next _count 
	}	
	
	| Intervention Heals
	:interventionheals
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 4
			/if (${Me.SpellReady[${healIntervention${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healIntervention${_count}}", ${hurtID}]}) /call core_cast2 "${healIntervention${_count}}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
				/break
			}
		/next _count
	}	
	
	| Renewal Heals
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${healRenewal${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healRenewal${_count}}", ${hurtID}]}) /call core_cast2 "${healRenewal${_count}}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
				/break
			}
		/next _count 
	}	



	|**
	| Remedy Heals 
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/if (${Me.SpellReady[${healRemedy1}]}) {
		  /if (${validate_cast[FALSE, spell, "${healRemedy1}", ${hurtID}]}) /call core_cast2 "${healRemedy1}" 0 ${hurtID} check_HP_${Me.Class.ShortName} 
		} else /if (${Me.SpellReady[${healRemedy2}]}) {
		  /if (${validate_cast[FALSE, spell, "${healRemedy2}", ${hurtID}]}) /call core_cast2 "${healRemedy2}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
		} else /if (${Me.SpellReady[${healRemedy3}]}) {
		  /if (${validate_cast[FALSE, spell, "${healRemedy3}", ${hurtID}]}) /call core_cast2 "${healRemedy3}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
		}
	} 
	


		
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/if (${Me.SpellReady[${healIntervention1}]}) {
		 /if (${validate_cast[FALSE, spell, "${healIntervention1}", ${hurtID}]}) /call core_cast2 "${healIntervention1}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
		} else /if (${Me.SpellReady[${healIntervention2}]}) {
		  /if (${validate_cast[FALSE, spell, "${healIntervention2}", ${hurtID}]}) /call core_cast2 "${healIntervention2}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
		} else /if (${Me.SpellReady[${healIntervention3}]}) {
		  /if (${validate_cast[FALSE, spell, "${healIntervention3}", ${hurtID}]}) /call core_cast2 "${healIntervention3}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
		} else /if (${Me.SpellReady[${healIntervention4}]}) {
		  /if (${validate_cast[FALSE, spell, "${healIntervention4}", ${hurtID}]}) /call core_cast2 "${healIntervention4}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
		}
	}
	


	| Renewal Heals
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/if (${Me.SpellReady[${healRenewal1}]}) {
			/if (${validate_cast[FALSE, spell, "${healRenewal1}", ${hurtID}]}) /call core_cast2 "${healRenewal1}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
		} else /if (${Me.SpellReady[${healRenewal2}]}) {
			/if (${validate_cast[FALSE, spell, "${healRenewal2}", ${hurtID}]}) /call core_cast2 "${healRenewal2}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
		} else /if (${Me.SpellReady[${healRenewal3}]}) {
			/if (${validate_cast[FALSE, spell, "${healRenewal3}", ${hurtID}]}) /call core_cast2 "${healRenewal3}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
		}
	}	
	**|
	
	
	
	UPDATEHUD FALSE
/return



|***
 *	DES: Fast check to see if anyone is hurt worse then person currently being healed.
 *	USE: added as a subcheck for core_cast
 *	INI: 
 *  NOTE: 
 ***|
sub check_HP_CLR()
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

	| panic heal
	/if (${Me.SpellReady[${healPanic}]}) {
		/if (${Spawn[${hurtID}].PctHPs} < 20*.${HealOverride}) {
			/if (${Me.Casting.ID}) /stopcast
			/if (${validate_cast[FALSE, spell, "${healPanic}", ${hurtID}]}) /call core_cast2 "${healPanic}" 0 ${hurtID} FALSE
		}
	}
		
 	/if (${switchHealAggressive}) {
 		/if (${Me.SpellReady[${healIntervention1}]} || ${Me.SpellReady[${healIntervention2}]} || ${Me.SpellReady[${healIntervention3}]} || ${Me.SpellReady[${healIntervention4}]}) /goto :interventionheals
 	} 
 	
 	| Remedy Heals 
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${healRemedy${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healRemedy${_count}}", ${hurtID}]}) /call core_cast2 "${healRemedy${_count}}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
				/break
			}
		/next _count 
	}	
	
	| Intervention Heals
	:interventionheals
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/for _count 1 to 4
			/if (${Me.SpellReady[${healIntervention${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healIntervention${_count}}", ${hurtID}]}) /call core_cast2 "${healIntervention${_count}}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
				/break
			}
		/next _count
	}		
  	
|**
	| Remedy Heals 
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/if (${Me.Casting.ID}) /stopcast
    /if (${Me.SpellReady[${healRemedy1}]}) {
      /if (${validate_cast[FALSE, spell, "${healRemedy1}", ${hurtID}]}) /call core_cast2 "${healRemedy1}" 0 ${hurtID} FALSE 
    } else /if (${Me.SpellReady[${healRemedy2}]}) {
      /if (${validate_cast[FALSE, spell, "${healRemedy2}", ${hurtID}]}) /call core_cast2 "${healRemedy2}" 0 ${hurtID} FALSE
    } else /if (${Me.SpellReady[${healRemedy3}]}) {
      /if (${validate_cast[FALSE, spell, "${healRemedy3}", ${hurtID}]}) /call core_cast2 "${healRemedy3}" 0 ${hurtID} FALSE
    } 
  }     	
		
	| Intervention Heals
	:interventionheals
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/if (${Me.Casting.ID}) /stopcast
  	/if (${Me.SpellReady[${healIntervention1}]}) {
     /if (${validate_cast[FALSE, spell, "${healIntervention1}", ${hurtID}]}) /call core_cast2 "${healIntervention1}" 0 ${hurtID} FALSE
    } else /if (${Me.SpellReady[${healIntervention2}]}) {
      /if (${validate_cast[FALSE, spell, "${healIntervention2}", ${hurtID}]}) /call core_cast2 "${healIntervention2}" 0 ${hurtID} FALSE
    } else /if (${Me.SpellReady[${healIntervention3}]}) {
      /if (${validate_cast[FALSE, spell, "${healIntervention3}", ${hurtID}]}) /call core_cast2 "${healIntervention3}" 0 ${hurtID} FALSE
    } else /if (${Me.SpellReady[${healIntervention4}]}) {
      /if (${validate_cast[FALSE, spell, "${healIntervention4}", ${hurtID}]}) /call core_cast2 "${healIntervention4}" 0 ${hurtID} FALSE
    }
  } 
**|

/return



|***
 *	DES: Checks and Heals Pets. Group ONLY
 *	USE: /healpet
 *	INI: 
 *  NOTE: 
 ***|
sub check_pet_heal_CLR()
	AMIDEAD
	SIMPLEEXIT
	CHECKINPUTS
	CHECKREZ

	/declare _count int local
	/varset hurtID 				${Me.ID}
	/varset hurtPctHP 		100
  
  /for _count 0 to ${Group}
    /if (${Group.Member[${_count}].Pet.ID}) {
      /if (${Group.Member[${_count}].Pet.Distance} <= ${MobAgro}) {
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
	
 	/if (${switchHealAggressive}) {
 		/if (${Me.SpellReady[${healIntervention1}]} || ${Me.SpellReady[${healIntervention2}]} || ${Me.SpellReady[${healIntervention3}]} || ${Me.SpellReady[${healIntervention4}]}) /goto :interventionheals
 	} 
 	
 	| Remedy Heals 
  /if (${Spawn[${hurtID}].PctHPs} <= ${PetPoint}) {
		/for _count 1 to 3
			/if (${Me.SpellReady[${healRenewal${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healRemedy${_count}}", ${hurtID}]}) /call core_cast2 "${healRemedy${_count}}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
				/break
			}
		/next _count 
	}	
	
	| Intervention Heals
	:interventionheals
  /if (${Spawn[${hurtID}].PctHPs} <= ${PetPoint}) {
		/for _count 1 to 4
			/if (${Me.SpellReady[${healIntervention${_count}}]}) {
				/if (${validate_cast[FALSE, spell, "${healIntervention${_count}}", ${hurtID}]}) /call core_cast2 "${healIntervention${_count}}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
				/break
			}
		/next _count
	}		

	UPDATEHUD FALSE
/return



|***
 *	DES: Use cleric divine arbitration AA or Epic shield click.
 *	USE: auto.
 *	INI: NA
 *  NOTE: Setting [Heal] -> setDivinetoGroupHeal to TRUE in the clerics INI will force a group heal after the health arbitration.
 ***|
sub cast_divine_arbitration()
	AMIDEAD
	SIMPLEEXIT
	/if (${Bool[${Me.Buff[Divine Aura]}]} || ${Bool[${Me.Buff[Divine Barrier]}]} || ${Bool[${Me.Buff[Sacred Barrier]}]}) /return
  /if (${Me.ItemReady[${CLREpic2}]}) {    
    /if (${validate_cast[FALSE, item, "${CLREpic2}", ${Me.ID}]}) /call core_cast2 "${CLREpic2}" item 0 FALSE
  } else /if (${Me.ItemReady[${CLREpic15}]}) {
    /if (${validate_cast[FALSE, item, "${CLREpic15}", ${Me.ID}]}) /call core_cast2 "${CLREpic15}" item 0 FALSE
  } else /if (${Me.AltAbilityReady[Divine Arbitration]}) {
    /if (${validate_cast[FALSE, alt, "Divine Arbitration", ${Me.ID}]}) /call core_cast2 "Divine Arbitration" alt 0 FALSE
  }
  /if (${validate_cast[FALSE, alt, "Celestial Rapidity", ${Me.ID}]}) /call core_cast2 "Celestial Rapidity" alt 0 FALSE
	
	/if (${setDivinetoGroupHeal} && ${useGroupHeal} && ${Me.SpellReady[${healWholeGroup}]}) {
		/if (${validate_cast[FALSE, spell, "${healWholeGroup}", ${Me.ID}]}) /call core_cast2 "${healWholeGroup}" 0  FALSE
	}
/return



|***
 *	DES: MGB cleric celestial regeneration if both AAs are ready
 *	USE: /bc cr, /cr
 *	INI: 
 *  NOTE: 
 *	Will not cast cr if the buff is present or if your name is Norbert.
 ***|
Sub cast_AA_celest_regen()
	AMIDEAD
	SIMPLEEXIT
	CHECKINPUTS
	/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[Celestial Regeneration]} && !${Bool[${Me.Song[Celestial Regeneration]}]}) {
		/if (${validate_cast[FALSE, alt, "Mass Group Buff", ${Me.ID}]}) /call core_cast2 "Mass Group Buff" alt ${Me.ID} FALSE
		/if (${validate_cast[FALSE, alt, "Celestial Regeneration", ${Me.ID}]}) /call core_cast2 "Celestial Regeneration" alt ${Me.ID} FALSE
		/if (${Bool[${Raid.Members}]}) /rs ${CRTXT}
	} else /if (${Bool[${Me.Song[Celestial Regeneration]}]}) {
		VOUT ${cbad}QUIT\ax trying to cast Celestial Regeneration while its running.	
	}		
	/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
/return



|***
 *	DES: Will the cleric floatie shield AE regen. Twin cast it if the AA is ready.
 *	USE: spell must be memed.
 *	INI: 
 *  NOTE: 
 *  NOTE.. THIS IS BROKEN.. It's not my fault.
 ***|
sub cast_issuance_heal()
	AMIDEAD
	SIMPLEEXIT
	/if (${Me.SpellReady[${healIssuance}]}) /call castFreeTargetSpell "${healIssuance}" ${Me.ID}
	/delay 3
	/if (${Me.AltAbilityReady[Forceful Rejuvenation]}) {
		/call MQ2Cast "Forceful Rejuvenation" alt
		/delay 3
		/if (${Me.SpellReady[${healIssuance}]}) /call castFreeTargetSpell "${healIssuance}" ${Me.ID}
	} 
/return



|***
 *	DES: Will cast Temp pet
 *	USE: must have a MA set and /companion
 *	INI: 
 *  NOTE: 
 ***|
sub cast_temp_pet_CLR()
	AMIDEAD
	CHECKEXIT
	CHECKTIE
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${burnForce} || (${burnAuto} && ${NameList.Find[${Target.DisplayName}]})) {
		/if (${validate_cast[FALSE, alt, "Celestial Hammer", ${Target2Kill}]}) /call core_cast2 "Celestial Hammer" alt ${Target2Kill} FALSE
	}
	/if (${validate_cast[FALSE, spell, "${tempPet}", ${Target2Kill}]}) /call core_cast2 "${tempPet}" 0 ${Target2Kill} FALSE

/return



|***
 *	DES: EQBC Buff Heal Ward of Surety spell
 *	USE: have the spell memed and the toon identified.
 *	INI: [Heal] -> HealWardToon
 *  NOTE: 
 ***|
sub check_heal_ward()
	/if (!${Me.SpellReady[${healWard}]}) /return
	/if (${coreAuto}) /call create_timer timer_heal_ward ${tHealWard}
	
	/declare _healwardID int local ${Spell[${healWard}].ID}
	/declare _wardToon string local
	/declare _count int local
	/declare _toon string local ${healWardToon}

	/for _count 1 to ${_toon.Count[|]}
		AMIDEAD
		SIMPLEEXIT
		CHECKINPUTS
		CHECKTIE
		/varset _wardToon ${_toon.Arg[${_count},|]}
		/if (${Bool[${RampageTank}]} && ${RampageTank.Equal[${_wardToon}]}) /continue

		/if (${Bool[${Spawn[${NetBots[${_wardToon}].ID}]}]} && ${Spawn[id ${NetBots[${_wardToon}].ID}].Distance} < ${Spell[${healWard}].MyRange} && ${Me.SpellReady[${healWard}]}) {
			/if (${Bool[${NetBots[${_wardToon}].ShortBuff.Find[${_healwardID}]}]} || ${Spawn[${_wardToon}].Type.Equal[corpse]}) /continue
			/if (${validate_cast[FALSE, spell, "${healWard}", ${Spawn[${_wardToon}].ID}]}) /call core_cast2 "${healWard}" 0 ${Spawn[${_wardToon}].ID} FALSE
		}
	/next _count	
/return



|***
 *	DES: Swap the Control Toon during raids
 *	USE: /autocontrol
 *	INI: 
 *  NOTE: THIS DOES NOT WORK.. /cry
 ***|
sub auto_control_swap()
	/if (!${validate_expansion[FALSE, |16]}) /return
	/return
	/if (!${autoControl}) /return
	/call create_timer timer_auto_toon_control ${tAuto_Control_Toon}

	AMIDEAD
	CHECKINPUTS


	/if (${Bool[${useSmartAssistR}]}) {
		/target ID ${Me.XTarget[1].ID}
		/delay 1
|		/delay 1s ${Target.ID} == ${Me.XTarget[1].ID}
	| } else /if (${Bool[${useSmartAssist2}]}) {
	| 	/target ID ${Me.XTarget[2].ID}
	| 	/delay 1s ${Target.ID} == ${Me.XTarget[2].ID}
	}
	
	/target ${Me.TargetOfTarget}
	
	/call writeINI toonControl Common ${Target.CleanName}
	/if (${Bool[${toonControlXTslot}]} && ${toonControlProtect}) /call protect_always_xt
/return


 
 |***
 *	DES: Cleric burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: you will find little of value here.
 ***| 
sub synergy_CLR()
	SIMPLEEXIT
	| please note. All of the clerics, what would be considerd heal burns, are built into the cleric routine.
	/call use_glyph
	/call synergy_click_Circle
	/call click_item_burn
/return





 |***
 *	DES: locks working target to the established MA
 *	USE: /clr lock | /call lock_ma
 *	INI: 
 *	NOTE: 
sub lock_ma()
/return
	/if (${SafeZone} && !${switchOverrideSafezone} || !${autoCore}) /return
	AMIDEAD
	CHECKEXIT
	/declare _tmpID int local 0
	
	
	/if (${Bool[${Group.Members}]} && !${Raid.Members}) {
		/if (${Spawn[pc ${Group.MainAssist}].ID}) /varset _tmpID ${Spawn[pc ${Group.MainAssist}].ID}		
	
	
	${Me.GroupAssistTarget}
	
		/if (${Spawn[pc ${MA1}].ID}) {
			/varset _targetID 
		} else /if (!${Spawn[pc ${MA1}].ID} && ${Spawn[pc ${MA2}].ID}) {
			
		}
				
		
	} else /if (${Raid.Members}) {

				${Me.RaidAssistTarget[1]}
				${Me.RaidAssistTarget[2]}
				${Me.RaidAssistTarget[3]}
		
	}
	
	/if (${_tmpID}) /target ID ${_tmpID}
		
/return




${Me.TargetOfTarget}: Target of target (moved to character type) 

bool spawn.Assist: Current Raid or Group assist target? 
int spawn.Mark: Current Raid or Group marked npc mark number (raid first) 

${Me.RaidAssistTarget[]} Current raid assist target (1-3) 
${Me.GroupAssistTarget} Current group assist target 
${Me.RaidMarkNPC[]} Current raid marked NPC (1-3) 
${Me.GroupMarkNPC[]} Current group marked NPC (1-3)

 ***| 




|***
 *	DES: Class controls
 *	USE: /clr []
 *	INI: 
 *  NOTE:
 ***|
#bind setCLR /clr
sub Bind_setCLR(string _classType, string _verbage)
	/if (!${validate_class[TRUE, |CLR]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _type string	local |aggressive|ch|fade|forcegroupheal|groupheal|grouphot|healpoint|rampage|regen|rod|spiregroup|spireraid|vp|xtclass|xtradius|xttime|yaulp

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

	| enable/disable lead with aggerssive heals
	/if (${Bool[${_classType.Equal[aggressive]}]}) {
		/call writeINI switchHealAggressive Heal ${If[${switchHealAggressive},FALSE,TRUE]}

	| lock group heals
	} else /if (${Bool[${_classType.Equal[forcegroupheal]}]}) {
	 	/call writeINI switchForceGroupHeal Heal ${If[${switchForceGroupHeal},FALSE,TRUE]}  

	| use Modulation Rods
	} else /if (${Bool[${_classType.Equal[rod]}]}) {
		/call writeINI switchModRod Buffs ${If[${switchModRod},FALSE,TRUE]} 	

	| mgb celestial regen
	} else /if (${Bool[${_classType.Equal[regen]}]}) {
		/delay 2s !${Me.Casting.ID}
		/call cast_AA_celest_regen
		/return
		
	| set rampage tank toon
	} else /if (${Bool[${_classType.Equal[rampage]}]}) {
		/echo rampage
		/if (${Bool[${_verbage}]}) {
			/call writeINI RampageTank Heal ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		} else /if (!${Bool[${_verbage}]}) {
			/call writeINI RampageTank Heal FALSE
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
		/if (${Bool[${_verbage}]} && ${Range.Between[1,5:${Int[${_verbage}]}]}) {
			/call writeINI tXHeal Timer ${_verbage}
		} else {
			VOUT /${_bind} xttime [${cinfo}${tXHeal}m\ax] allowable range ${sep} ${cinfo}1 - 5 min\ax
			/return
		}	
		
	| to Yaulp or not		
	} else /if (${Bool[${_classType.Equal[yaulp]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI useYaulpPct Buffs ${_verbage}
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} yaulp [${cinfo}${useYaulpPct}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		} else {
		 	/call writeINI doYaulp Buffs ${If[${doYaulp},FALSE,TRUE]}  
		}

	| Veturika's Perseverance	Control	
	} else /if (${Bool[${_classType.Equal[vp]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI useVPPct Buffs ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} vp [${cinfo}${useVPPct}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call writeINI switchVP Buffs ${If[${switchVP},FALSE,TRUE]}  
		}	
		
	| use group Heals
	} else /if (${Bool[${_classType.Equal[groupheal]}]}) {
		/call writeINI useGroupHeal Heal ${If[${useGroupHeal},FALSE,TRUE]} 	

	| use group Heal over Time	
	} else /if (${Bool[${_classType.Equal[grouphot]}]}) {
		/call writeINI useGroupHoT Heal ${If[${useGroupHoT},FALSE,TRUE]} 	

	| set rampage tank toon
	} else /if (${Bool[${_classType.Equal[rampage]}]}) {
		/if (${Bool[${_verbage}]}) {
			/call writeINI RampageTank Heal ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		} else /if (!${Bool[${_verbage}]}) {
			/call writeINI RampageTank Heal FALSE
		}
				
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
		}
		/mqclear
		/for _count 1 to ${AllClassList.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${AllClassList.Arg[${_count},|].Upper}\ax(${If[${XHealClass.Find[${AllClassList.Arg[${_count},|]}]},${oon},${ooff}]})
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
		/return	

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
		
	| Complete Heal
	} else /if (${Bool[${_classType.Equal[ch]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI CompleteHealpct Heal ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} ch(${cnum}${CompleteHealpct}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call writeINI switchCompleteHeal Heal ${If[${switchCompleteHeal},FALSE,TRUE]}  
		}

	}
	/if (${switchMQclearonCLS}) /mqclear
	/for _count 1 to ${_type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_type.Arg[${_count},|]}\ax
		/if (${_type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${_type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${_type.Arg[${_count},|].Equal[xtclass]}) /varset _listout ${_listout}(${cnum}->\ax)
		/if (${_type.Arg[${_count},|].Equal[xttime]}) /varset _listout ${_listout}(${cnum}${tXHeal}\ax)
		/if (${_type.Arg[${_count},|].Equal[xtradius]}) /varset _listout ${_listout}(${cnum}${XTHealRadius}\ax)
		/if (${_type.Arg[${_count},|].Equal[yaulp]}) /varset _listout ${_listout}(${If[${doYaulp},${cnum}${useYaulpPct}\ax,${cbad}${useYaulpPct}\ax]})
		/if (${_type.Arg[${_count},|].Equal[ch]}) /varset _listout ${_listout}(${If[${switchCompleteHeal},${cnum}${CompleteHealpct}\ax,${cbad}${CompleteHealpct}\ax]})
		/if (${_type.Arg[${_count},|].Equal[vp]}) /varset _listout ${_listout}(${If[${switchVP},${cnum}${useVPPct}\ax,${cbad}${useVPPct}\ax]})
		/if (${_type.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}(${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]})
		/if (${_type.Arg[${_count},|].Equal[groupheal]}) /varset _listout ${_listout}(${If[${useGroupHeal},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[forcegroupheal]}) /varset _listout ${_listout}(${If[${switchForceGroupHeal},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[grouphot]}) /varset _listout ${_listout}(${If[${useGroupHoT},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[aggressive]}) /varset _listout ${_listout}(${If[${switchHealAggressive},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}->\ax)
		/if (${_type.Arg[${_count},|].Equal[rampage]}) /varset _listout ${_listout}(${If[${Bool[${RampageTank}]},${cnum}${RampageTank}\ax,${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}(${If[${Bool[${switchModRod}]},${oon},${ooff}]})
	/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return



