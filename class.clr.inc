|***
 *	class.clr.inc
 *	Core Cleric Include
 *
 * 
 ***| 

#event ditchRampage "#*#hits YOU for #*# points of damage. (Rampage)#*#"
sub Event_ditchRampage()
	/if (${Me.AltAbilityReady[Divine Peace]}) /call core_cast "Divine Peace" alt 0
/return


#event ToTD "#*#Your body is encased in perfect etheral crystal#*#"
sub Event_ToTD()
	/if (${Select[${Me.Class.ShortName},CLR]}) {
		ECHOCHANNEL ToTD fired 5 second dealy for heals.
		/delay 5s
		/squelch /removebuff Touch of the Divine
	}
/return


|***
 *	DES: Main Cleric Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_CLR()
	DEBUG Loop start: ${cbug}core_CLR()\ax
	/doevents
	ISDEAD
	| /if (${AutoBot} && !${timer_auto_toon_control} && ${autoControl} && ${Raid.Members}) /call auto_control_swap
	/if (${Bool[${toonControl}]} && ${Bool[${toonControlXTslot}]} && ${toonControlProtect}) /call protect_always_xt
	CHECKTIE

  /if (!${Bool[${timer_set_xtarget}]} && ${switchHealXTarget}) {
    /call set_xtarget
    /call create_timer timer_set_xtarget ${tXHeal}
  }	
	
	CHECKREZ
	/if (${AutoBot} && !${Me.Invis}) {
		/if (${Bool[${sumWeaponItem}]} && (${InvSlot[mainhand].Item.Name.NotEqual[${sumWeaponItem}]} || !${Bool[${Me.Primary}]})) /call cast_sum_weapon
		/if (${Me.PctMana} < 60*.${tmpHealOverride}  && ${Me.AltAbilityReady[Veturika's Perseverance]}) /call core_cast "Veturika's Perseverance" alt 0
		/if (${Me.AltAbilityReady[Quiet Miracle]}) /call cast_quiet_miracle
	}	

	:killtarget
	CHECKREZ
	/if (${AutoBot} && !${Me.Invis}) {
		/call check1offBuffs
		/if (${Select[${combat},MELEE,RANGE,NUKE]}) {
			/call is_target_valid ${Target2Kill}
			/if (!${isValidTarget} || !${Spawn[${Target2Kill}].ID}) {
				/call clear_combat
				/goto :exit
			} else /if (${isValidTarget} && ${Spawn[${Target2Kill}].ID}) {
				/call is_toast_time
				/if (${Macro.Return}) /call synergy_${Me.Class.ShortName}
			}
		} else /if (${Select[${combat},HEAL,DUAL]}) {
			/call is_toast_time
			/if (${Macro.Return}) /call synergy_${Me.Class.ShortName}
		}
	} else /if (!${AutoBot}) {
		/if (${Target.ID} != ${Target2Kill}) /varset Target2Kill ${Target.ID}
		/if (!${Target.ID} || !${Spawn[${Target2Kill}].ID}) /goto :exit
	}
	
	CLEARCURSOR

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},HEAL]}) {
		CHECKREZ
		/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
		/if (${switchHealCure}) /call cast_cure
		/if (${switchHealXTarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/if (${usePet} && !${Me.Pet.ID}) /call cast_temp_pet_${Me.Class.ShortName}
		/if (${switchHealPet}) /call check_pet_heal_${Me.Class.ShortName}
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}
		/call disc_${Me.Class.ShortName}
		/if (${Me.SpellReady[${healWard}]} && !${timer_heal_ward}) /call check_heal_ward
	} else /if (${Select[${combat},DUAL]}) {
	}
	/call use_mod_rod
	/call cast_quiet_miracle
	/if (${AutoBot} && !${Spawn[${Target2Kill}].ID} || !${AutoBot} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Macro.Return}) /goto :killtarget
	}
	:exit
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_CLR()
	DEBUG ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/call click_horn_of_unity 	
	:exit
/return



|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: Disc
 *	INI: 
 *  NOTE: 
 ***|
sub disc_CLR()
	DEBUG	${cbug}disc_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/if (${doYaulp}) /call cast_AA_yaulp
	/call is_target_valid ${Target2Kill}
	/if (${isValidTarget}) {
		/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	}
	:exit
/return



|***
 *	DES: This will summon the clerics awesom headsmasher hammer if you do not have a good primary weapon.
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub cast_sum_weapon()
	DEBUG ${cbug}cast_sum_weapon()\ax
	ISDEAD
	/doevents
	/if (${Me.Primary}) /goto :exit

	:loopneedhammer
	ISDEAD
	/if (!${Bool[${FindItem[${sumWeaponItem}]}]}) {
		/call core_cast "${sumWeaponSpell}" ${buffGem} ${Me.ID}
	}
	/delay 1
	/if (!${Bool[${FindItem[${sumWeaponItem}]}]}) /goto :loopneedhammer
	/if (${Bool[${Cursor.ID}]}) {
		/call clear_cursor TRUE
		/call clear_cursor TRUE
	} else {
		/exchange "${sumWeaponItem}" mainhand 
	}
	:exit
/return



|***
 *	DES: Quiet Miracle 30ish k HP/Manna
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub cast_quiet_miracle()
	DEBUG ${cbug}cast_quiet_miracle()\aw
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return} || !${Me.AltAbilityReady[Quiet Miracle]}) /goto :exit
	
	/declare _count 					int local
	/varset WorstManaID				${Me.ID}
	/varset WorstManaPct			${If[${HealOverride},${useQMPct}*.${tmpHealOverride},${useQMPct}]}

	
	| QM cleric first always
	/if (!${useQM} && ${Me.PctMana} < ${useQMPct}*.${tmpHealOverride}) /call core_cast "Quiet Miracle" alt ${Me.ID} 

	
	| Do group members need Manna
	/for _count 0 to ${Group.Members}
		DEBUG ${_count}: ${Group.Member[${_count}].ID}-${Group.Member[${_count}].DisplayName}
		/if (${Group.Member[${_count}].Dead} || !${CastList.Find[${Group.Member[${_count}].Class.ShortName}]}) /next _count

		/if (${Spawn[pc id ${Group.Member[${_count}].ID}].ID} && !${Spawn[mercenary id ${Group.Member[${_count}].ID}].ID} && ${Group.Member[${_count}].Distance} <= ${Spell[Quiet Miracle].MyRange} && ${Group.Member[${_count}].PctMana} < ${WorstManaPct}) {
			/varset WorstManaID ${Group.Member[${_count}].ID}
			/varset WorstManaPct ${Group.Member[${_count}].PctMana}
		} 
  /next _count


	/if (${WorstManaPct} < ${useQMPct}*.${tmpHealOverride}) {
		/call core_cast "Quiet Miracle" alt ${WorstManaID}
		ECHOCHANNEL Quiet Miracle ... ${Target.DisplayName}
	}
	:exit
/return	



|***
 *	DES: cleric main heal sub
 *	USE: /call check_heal_group_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: bread and butter baby...
 ***|
sub check_heal_group_CLR()
	DEBUG ${cbug}check_heal_group_CLR()\ax
	CHECKEXIT
	/declare _count						int	local
	
	:loopheal
	ISDEAD
	/doevents
	CHECKTIE
	/varset WorstHurt 				${Me.ID}
	/varset WorstHP 					${If[${HealOverride},${Float[${HealOverride}].Centi},99.99]}
	/varset WorstClass 				${Me.Class.ShortName}
	/varset MinorHurt 				0
	/varset MajorHurt					0
	

	| HOLD FOR REFERENCE FOR NOW...
	| Does the control toon need healing?
	/if (!${Bool[${Group.Member[${toonControl}]}]} && ${Bool[${toonControl}]} && ${toonControlProtect} && !${Spawn[${toonControl}].Dead} && ${Bool[${Spawn[${toonControl}]}]} && ${Spawn[${toonControl}].ID} && ${Spawn[${toonControl}].PctHPs} <= ${WorstHP} && (${Spawn[${toonControl}].Distance} <= ${Spell[${healRemedy1}].MyRange} && ${Spawn[${toonControl}].PctHPs} <= ${${Spawn[${toonControl}].Class.ShortName}Point}*.${tmpHealOverride} || ${Spawn[${toonControl}].Distance} <= ${Spell[${healRenewal1}].MyRange} && ${Spawn[${toonControl}].PctHPs} <= ${${Spawn[${toonControl}].Class.ShortName}Point}*.${tmpHealOverride}*.7 )) {
		/varset WorstHurt ${Spawn[${toonControl}].ID}
		/varset WorstHP ${Spawn[${toonControl}].PctHPs}
		/varset WorstClass ${Spawn[${toonControl}].Class.ShortName}
	} 

	| Do group members need healing? ${Group.Member[${_count}].State.Equal[HOVER]}
	/for _count 0 to ${Group.Members}
		ISDEAD
		CHECKEXIT
	 	/doevents
		/if (!${Group.Member[${_count}].Dead} && (${Spawn[pc id ${Group.Member[${_count}].ID}].ID} || ${Spawn[mercenary id ${Group.Member[${_count}].ID}].ID}) && ${Group.Member[${_count}].PctHPs} <= ${WorstHP} && (${Group.Member[${_count}].Distance} <= ${Spell[${healRemedy1}].MyRange} && ${Group.Member[${_count}].PctHPs} <= ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride} || ${Group.Member[${_count}].Distance} <= ${Spell[${healRenewal1}].MyRange} && ${Group.Member[${_count}].PctHPs} <= ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}*.7)) {
			/varset WorstHurt ${Group.Member[${_count}].ID}
			/varset WorstHP ${Group.Member[${_count}].PctHPs}
			/varset WorstClass ${Group.Member[${_count}].Class.ShortName}
		} 
    /if (!${Group.Member[${_count}].Dead} && !${Group.Member[${_count}].State.Equal[HOVER]} && ${Group.Member[${_count}].Type.NotEqual[Corpse]} && && (${Group.Member[${_count}].PctHPs} < 85*.${tmpHealOverride} || ${Group.Member[${_count}].PctHPs}*.${tmpHealOverride} < ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}) && ${Group.Member[${_count}].PctHPs}*.${tmpHealOverride} > ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}*.7 && ${Group.Member[${_count}].Distance} <= ${Spell[${healWholeGroup}].AERange}) {
      /varcalc MinorHurt ${MinorHurt}+1
      UPDATEHUD "HEAL"
    } else /if (!${Group.Member[${_count}].Dead} && !${Group.Member[${_count}].State.Equal[HOVER]} && ${Group.Member[${_count}].Type.NotEqual[Corpse]} && ${Group.Member[${_count}].PctHPs} < ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}*.7 && ${Group.Member[${_count}].Distance} <= ${Spell[${healWholeGroup}].AERange}) {
      /varcalc MajorHurt ${MajorHurt}+1
      UPDATEHUD "HEAL"
    }
  /next _count
	
  /if ( ${Bool[${Me.Buff["Divine Aura"]}]} || ${Bool[${Me.Buff["Divine Barrier"]}]} || ${Bool[${Me.Buff["Sacred Barrier"]}]} ) /goto :exit
	UPDATEHUD "HEAL"
	CHECKREZ

	| Regen Heals 
	| Use all the time in combat. Don't care if hurt or not.
 	/if (${useGroupHoT} && ${SpawnCount[npc radius ${MobAgro} playerstate 4 targetable]} >= 1) {
 	 	/if (${Me.AltAbilityReady[Celestial Regeneration]} && !${Raid.Members} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${Bool[${Me.Song[Focused Celestial Regeneration]}]} && !${Bool[${Me.Song[${healWholeGroupHoT}]}]} && !${Bool[${Me.Song[Shear of Renewal]}]}) {
	 		/call core_cast "Celestial Regeneration" alt ${Me.ID} 
	 		/goto :exitGroupRegen	
	  } else /if (${Me.SpellReady[${healWholeGroupHoT}]} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${Bool[${Me.Song[Focused Celestial Regeneration]}]} && !${Bool[${Me.Song[${healWholeGroupHoT}]}]} && !${Bool[${Me.Song[Shear of Renewal]}]}) {
	    /call core_cast "${healWholeGroupHoT}" 0 ${Me.ID} 
	    /goto :exitGroupRegen
	  }
	  :exitGroupRegen
	}

	| Focused Celestial Regeneration
	/if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.${tmpHealOverride} && ${Spawn[${WorstHurt}].Distance} <= ${Spell[Focused Celestial Regeneration].MyRange} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${SafeZone}) { 
		/if (${Me.AltAbilityReady[Focused Celestial Regeneration]}) {
			/call core_cast "Focused Celestial Regeneration" alt ${WorstHurt}
		}
	}
	ISDEAD
	CHECKEXIT
 	/doevents
	
  | AA Spire
	/if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.${tmpHealOverride} && ${Spawn[${WorstHurt}].Distance} <= ${Spell[${Spire}].MyRange} && !${Bool[${Me.Song[Celestial Regeneration]}]} && !${SafeZone}) { 
    /if (${Me.AltAbilityReady[${Spire}]}) {
    	/call core_cast "${Spire}" alt ${WorstHurt}
    }
  }	
  
	| Dichotomic Heal
	/if (${Bool[${Me.Song[Gift of Mana (${GoMLevel})]}]} && (${Me.SpellReady[${GoMNuke}]} || ${Me.SpellReady[${GoMNuke2}]}) && ${Spawn[${WorstHurt}].PctHPs} <= 95*.${tmpHealOverride} && ${Spawn[${WorstHurt}].Distance} <= ${Spell[${GoMNuke}].MyRange}) { 
    /if (${Me.SpellReady[${GoMNuke}]}) {
    	/call core_cast "${GoMNuke}" 0 ${WorstHurt}
    	/goto :exitgom
    } else /if (${Me.SpellReady[${GoMNuke2}]}) {
    	/call core_cast "${GoMNuke2}" 0 ${WorstHurt}
    	/goto :exitgom
    }
    :exitgom
    /if (${castReturn.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
  }
	ISDEAD
	CHECKEXIT
 	/doevents

	| Panic Heal 
  /if (${Spawn[${WorstHurt}].PctHPs} <= 45*.${tmpHealOverride} && ${Spawn[${WorstHurt}].Distance} <= ${Spell[${healPanic}].MyRange} && ${Me.SpellReady[${healPanic}]}) {
    /call core_cast "${healPanic}" 0 ${WorstHurt}
    /if (${castReturn.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
  }

	| Arbitration
  /if (${Spawn[${WorstHurt}].PctHPs} < ${${WorstClass}Point}*.${tmpHealOverride}*.5 || ${MajorHurt} > 2 ) {
    /call cast_divine_arbitration
    /goto :loopheal
  }
  
	| check for debuffs
	/if (${switchHealCure}) /call cast_cure
  
  | Group Heal
  /if (${MajorHurt}+${MinorHurt} >= 2 && ${Me.SpellReady[${healWholeGroup}]} && ${useGroupHeal}) {
    /call core_cast "${healWholeGroup}" 0 ${Me.ID} check_HP_${Me.Class.ShortName}
    /if (${Macro.Return.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
  } 
	ISDEAD
	CHECKEXIT
 	/doevents
  
  | ward of surety short buff 
	/if (!${Bool[${timer_heal_ward}]} && ${Me.SpellReady[${healWard}]}) /call check_heal_ward

	ISDEAD
	CHECKEXIT
 	/doevents
		
	| AA heal stuffs
  /if (${Math.Calc[${MajorHurt}+${MinorHurt}]} && !${SafeZone}) {
	  /if (${Math.Calc[${MajorHurt}+${MinorHurt}]} >= 2) {
		  /if (${Me.AltAbilityReady[Beacon of Life]}) /call core_cast "Beacon of Life" alt ${Me.ID}
		}
	  /if (${Math.Calc[${MajorHurt}+${MinorHurt}]} >= 3) {
		 	/if (${Me.AltAbilityReady[Channeling the Divine]}) /call core_cast "Channeling the Divine" alt ${Me.ID}
		}
 		/if (${Math.Calc[${MajorHurt}+${MinorHurt}]} >= 4) {
 			/if (${Me.AltAbilityReady[Healing Frenzy]}) /call core_cast "Healing Frenzy" alt ${Me.ID}
 		}
		/if (${Math.Calc[${MajorHurt}+${MinorHurt}]} >= 5) {
			/if (${Me.AltAbilityReady[Exquisite Benediction]}) /call core_cast "Exquisite Benediction" alt ${Me.ID}
			/if (${Me.AltAbilityReady[Silent Casting]}) /call core_cast "Silent Casting" alt ${Me.ID}
		}
		/if (${Math.Calc[${MajorHurt}+${MinorHurt}]} == 6) {
			/if (${Me.AltAbilityReady[Divine Retribution]}) /call core_cast "Divine Retribution" alt ${Me.ID}
			/call use_glyph
		}
  } 
	ISDEAD
	CHECKEXIT
 	/doevents
		
	| Intervention Heals
	/doevents
  /if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.${tmpHealOverride} && (${Me.SpellReady[${healIntervention1}]} || ${Me.SpellReady[${healIntervention2}]} || ${Me.SpellReady[${healIntervention3}]})) {
  	/if (${Spawn[${WorstHurt}].PctHPs} < ${${WorstClass}Point}*.${tmpHealOverride} && ${Me.AltAbilityReady[Burst of Life]}) {
			/call core_cast "Burst of Life" alt ${WorstHurt} check_HP_${Me.Class.ShortName}
      /goto :loopheal
		} else /if (${Me.SpellReady[${healIntervention1}]}) {
     /call core_cast "${healIntervention1}" 0 ${WorstHurt} check_HP_${Me.Class.ShortName}
      /goto :loopheal
    } else /if (${Me.SpellReady[${healIntervention2}]}) {
      /call core_cast "${healIntervention2}" 0 ${WorstHurt} check_HP_${Me.Class.ShortName}
      /goto :loopheal
    } else /if (${Me.SpellReady[${healIntervention3}]}) {
      /call core_cast "${healIntervention3}" 0 ${WorstHurt} check_HP_${Me.Class.ShortName}
      /goto :loopheal
    }
  }
	ISDEAD
	CHECKEXIT
 	/doevents

	| Remedy Heals 
  /if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.${tmpHealOverride} && (${Me.SpellReady[${healRemedy1}]} || ${Me.SpellReady[${healRemedy2}]})) {
      /if (${Me.SpellReady[${healRemedy1}]}) {
        /call core_cast "${healRemedy1}" 0 ${WorstHurt} check_HP_${Me.Class.ShortName} 
        /goto :loopheal
      } else /if (${Me.SpellReady[${healRemedy2}]}) {
        /call core_cast "${healRemedy2}" 0 ${WorstHurt} check_HP_${Me.Class.ShortName}
        /goto :loopheal
      } 
  }    
	ISDEAD
	CHECKEXIT
 	/doevents

	| Renewal Heals
	/doevents
  /if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.${tmpHealOverride} && (${Me.SpellReady[${healRenewal1}]} || ${Me.SpellReady[${healRenewal2}]} || ${Me.SpellReady[${healRenewal3}]})) {
    /if (${Me.SpellReady[${healRenewal1}]}) {
       /call core_cast "${healRenewal1}" 0 ${WorstHurt} check_HP_${Me.Class.ShortName}
       /goto :loopheal
     } else /if (${Me.SpellReady[${healRenewal2}]}) {
       /call core_cast "${healRenewal2}" 0 ${WorstHurt} check_HP_${Me.Class.ShortName}
       /goto :loopheal
     } else /if (${Me.SpellReady[${healRenewal3}]}) {
       /call core_cast "${healRenewal3}" 0 ${WorstHurt} check_HP_${Me.Class.ShortName}
       /goto :loopheal
     }
  }  
  
	:exit
	UPDATEHUD
/return



|***
 *	DES: Check XTargets and heal the bastards if the need it.
 *	USE: /call check_heal_XTarget_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub check_heal_XTarget_CLR()
	DEBUG ${cbug}check_heal_XTarget_${Me.Class.ShortName}()\ax
	ISDEAD
	/call check_exit
	/if (${Macro.Return}) /goto :exit
  /declare _count 					int local

  /for _count 1 to 13
  	UPDATEHUD "HEAL XTARGET"
		/call check_no_cast_buffs
		/doevents
		/call check_exit
		/if (${Macro.Return}) /goto :exit
		ISDEAD

    /if (${Spawn[${Me.XTarget[${_count}].ID}].ID} && (${Spawn[${Me.XTarget[${_count}].ID}].Type.Equal[PC]} || ${ProtectNPCList.Find[${Spawn[${Me.XTarget[${_count}].ID}].CleanName}]}) && ${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} <= ${${Spawn[id ${Me.XTarget[${_count}].ID}].Class.ShortName}Point}*.${tmpHealOverride}) {
      /if (${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} < 45*.${tmpHealOverride} && ${Me.AltAbilityReady[Burst of Life]}) {
        /call core_cast "Burst of Life" 0 ${Me.XTarget[${_count}].ID} 
      }
      /if (${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} <= 45*.${tmpHealOverride} && ${Spawn[${Me.XTarget[${_count}].ID}].Distance} <= ${Spell[${healPanic}].MyRange} && ${Me.SpellReady[${healPanic}]}) {
        /call core_cast "${healPanic}" 0 ${Me.XTarget[${_count}].ID} check_HP_${Me.Class.ShortName}
      } else /if (${Spawn[${Me.XTarget[${_count}].ID}].Distance} <= ${Spell[${healRenewal1}].MyRange} && ${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} <= ${${Spawn[id ${Me.XTarget[${_count}].ID}].Class.ShortName}Point}*.${tmpHealOverride} && (${Me.SpellReady[${healRenewal1}]} || ${Me.SpellReady[${healRenewal2}]} && || ${Me.SpellReady[${healRenewal3}]})) {
        /if (${Me.SpellReady[${healRenewal1}]}) {
          /call core_cast "${healRenewal1}" 0 ${Me.XTarget[${_count}].ID} check_HP_${Me.Class.ShortName}
        } else /if (${Me.SpellReady[${healRenewal2}]}) {
          /call core_cast "${healRenewal2}" 0 ${Me.XTarget[${_count}].ID} check_HP_${Me.Class.ShortName}
        } else /if (${Me.SpellReady[${healRenewal3}]}) {
          /call core_cast "${healRenewal3}" 0 ${Me.XTarget[${_count}].ID} check_HP_${Me.Class.ShortName}
        }
      } else /if (${Spawn[${Me.XTarget[${_count}].ID}].Distance} <= ${Spell[${healIntervention1}].MyRange} && ${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} <= ${${Spawn[id ${Me.XTarget[${_count}].ID}].Class.ShortName}Point}*.${tmpHealOverride} && (${Me.SpellReady[${healIntervention1}]} || ${Me.SpellReady[${healIntervention2}]} || ${Me.SpellReady[${healIntervention3}]})) {
        /if (${Me.SpellReady[${InterventionSpell1}]}) {
          /call core_cast "${healIntervention1}" 0 ${Me.XTarget[${_count}].ID} check_HP_${Me.Class.ShortName}
        } else /if (${Me.SpellReady[${healIntervention2}]}) {
          /call core_cast "${healIntervention2}" 0 ${Me.XTarget[${_count}].ID} check_HP_${Me.Class.ShortName}
        } else /if (${Me.SpellReady[${InterventionSpell3}]}) {
          /call core_cast "${healIntervention3}" 0 ${Me.XTarget[${_count}].ID} check_HP_${Me.Class.ShortName}
        }
      } else /if (${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} <= ${${Spawn[id ${Me.XTarget[${_count}].ID}].Class.ShortName}Point}*.${tmpHealOverride} && ${Spawn[${Me.XTarget[${_count}].ID}].Distance} <= ${Spell[${HealSpell}].MyRange} && ${Me.SpellReady[${HealSpell}]} && !${Me.SpellReady[${healIntervention1}]} && !${Me.SpellReady[${healIntervention2}]} && !${Me.SpellReady[${healIntervention3}]} && && !${Me.SpellReady[${healRenewal1}]} && !${Me.SpellReady[${healRenewal2}]} && !${Me.SpellReady[${healRenewal3}]}) {
        /call core_cast "${HealSpell}" 0 ${Me.XTarget[${_count}].ID} check_HP_${Me.Class.ShortName}
      }
    }
  /next _count
	:exit
	UPDATEHUD
/return



|***
 *	DES: Fast check to see if anyone is hurt worse then person currently being healed.
 *	USE: added as a subcheck for core_cast
 *	INI: 
 *  NOTE: 
 ***|
sub check_HP_CLR()
	DEBUG ${cbug}check_HP_CLR()\ax
	ISDEAD
	/if (!${Me.Casting.ID}) /goto :exit
	/declare _count 					int local

	/varset WorstHurt 				${Me.ID}
	/varset WorstHP 					${If[${HealOverride},${Float[${HealOverride}].Centi},99.99]}
	/varset WorstClass 				CLR
	/varset MajorHurt					0
	

	/for _count 0 to ${Group.Members}
		/call check_exit
		ISDEAD
		/call check_exit
		/if (${Macro.Return}) /goto :exit
		/if (${Group.Member[${_count}].Type.NotEqual[Corpse]} && ${Group.Member[${_count}].ID}) {
			/if (${Group.Member[${_count}].Distance}<=${Spell[${healRemedy2}].MyRange} && ${Group.Member[${_count}].PctHPs} <= ${WorstHP} || !${WorstHurt}) {
			/varset WorstHurt ${Group.Member[${_count}].ID} 
			/varset WorstHP ${Group.Member[${_count}].PctHPs} 
			/varset WorstClass ${Group.Member[${_count}].Class.ShortName} 
		}
		/if (${Group.Member[${_count}].PctHPs} < ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}*.5) /varcalc MajorHurt ${MajorHurt}+1
	}
	/next _count

	/if (!${WorstHurt}) /goto :exit

	/if (${Spawn[${WorstHurt}].PctHPs} < 45*.${tmpHealOverride} && ${Spawn[${WorstHurt}].Distance}<=${Spell[${healPanic}].MyRange} && ${Cast.Timing}/1000<${Spell[${healPanic}].MyCastTime} && ${Me.SpellReady[${healPanic}]}) {
      /if (${Me.Casting.ID}) /call Interrupt
      /call core_cast "${healPanic}" 0 ${WorstHurt}
   } else /if (${Spawn[${WorstHurt}].PctHPs} < ${${WorstClass}Point}*.${tmpHealOverride}*.5 && ${Spawn[${WorstHurt}].Distance} <= ${Spell[${healRemedy2}].MyRange} && ${Target.ID} != ${WorstHurt} && ${Target.PctHPs} >= (${${Target.Class.ShortName}Point}*.${tmpHealOverride})*.5 && ${Cast.Timing}/1000 < ${Spell[${PatchSpell}].MyCastTime}) {
     /if (${Me.Casting.ID}) /call Interrupt
     /call core_cast "${healRemedy2}" 0 ${WorstHurt}
   } else /if ((${Target.ID} && ${Bool[${Group.Member[${Target.CleanName}]}]}) != ${WorstHurt} && ${${Target.Class.ShortName}Point}*.${tmpHealOverride} < ${Target.PctHPs}*.${tmpHealOverride}*.6 && ${Spawn[${Target.ID}].Distance} < ${Spell[Divine Arbitration].MyRange}) {
     /call cast_divine_arbitration 
   } else /if (${Spawn[${WorstHurt}].PctHPs} < ${${WorstClass}Point}*.${tmpHealOverride}*.4 && ${Spawn[${WorstHurt}].Distance} < ${Spell[Divine Arbitration].MyRange}) {
     /call cast_divine_arbitration
   }
   
   :exit
/return



|***
 *	DES: Checks and Heals Pets. Group ONLY
 *	USE: /healpet
 *	INI: 
 *  NOTE: 
 ***|
sub check_pet_heal_CLR()
	DEBUG ${cbug}check_pet_heal_CLR()\ax
	/if (${Target.ID} == ${Me.Pet.ID}) /goto :exit
	/declare _count int local
	/varset pWorstHurt ${Me.CleanName}
	/varset pWorstHP 100
	/varset pWorstHurtID ${Me.ID}
	/varset pWorstClass NULL
  
  /for _count 0 to ${Group}
		/call check_exit
		/if (${Macro.Return}) /goto :exit
		ISDEAD
		/doevents
		CHECKEXIT
		CHECKREZ
    /if (${Group.Member[${_count}].Pet.ID}) {
      /if (${Group.Member[${_count}].Pet.Distance} <= ${Spell[${healRemedy1}].MyRange}) {
        /if ((${Group.Member[${_count}].Pet.PctHPs} <= ${pWorstHP}) && (${Group.Member[${_count}].Pet.PctHPs} <= ${${Group.Member[${_count}].Class.ShortName}Point})) {
          /varset pWorstClass ${Group.Member[${_count}].Pet.Class.ShortName}
          /varset pWorstHurt ${Group.Member[${_count}].Pet.Name}
          /varset pWorstHurtID ${Group.Member[${_count}].Pet.ID}
          /varset pWorstHP ${Group.Member[${_count}].Pet.PctHPs}
        }
      }
    }
  /next _count

  /if (${pWorstHurtID} == ${Me.ID}) /goto :exit
  /if (${Spawn[${pWorstHurtID}].PctHPs} < ${${pWorstClass}Point} && ${Spawn[${pWorstHurtID}].Distance} <= ${Spell[${healRemedy1}].MyRange} && ${Me.SpellReady[${healRemedy1}]}) {
    /call core_cast "${healRemedy1}" 0 ${pWorstHurtID} check_HP_${Me.Class.ShortName}
  } else /if (${Spawn[${pWorstHurtID}].PctHPs} < ${${pWorstClass}Point} && ${Spawn[${pWorstHurtID}].Distance} <= ${Spell[${healIntervention3}].MyRange} && ${Me.SpellReady[${healIntervention3}]}) {
    /call core_cast "${healIntervention3}" 0 ${pWorstHurtID} check_HP_${Me.Class.ShortName}
  }
  
  :exit
/return



|***
 *	DES: Use cleric divine arbitration AA or Epic shield click.
 *	USE: auto.
 *	INI: NA
 *  NOTE: Setting [Heal] -> setDivinetoGroupHeal to TRUE in the clerics INI will force a group heal after the health arbitration.
 ***|
sub cast_divine_arbitration()
	DEBUG ${cbug}cast_divine_arbitration()\ax
	ISDEAD
	/if ( ${Bool[${Me.Buff["Divine Aura"]}]} || ${Bool[${Me.Buff["Divine Barrier"]}]} || ${Bool[${Me.Buff["Sacred Barrier"]}]} ) /goto :exit
  /if (${Me.ItemReady[Aegis of Superior Divinity]}) {    
    /call core_cast "Aegis of Superior Divinity" item 0
  } else /if (${Me.ItemReady[Harmony of the Soul]}) {
    /call core_cast "Harmony of the Soul" item 0
  } else /if (${Me.AltAbilityReady[Divine Arbitration]}) {
    /call core_cast "Divine Arbitration" alt 0
  }

  /if (${Me.AltAbilityReady[Celestial Rapidity]}) /call core_cast "Celestial Rapidity" alt 0
	
	:loopheal
	ISDEAD
	/if (${setDivinetoGroupHeal} && ${Me.SpellReady[${healWholeGroup}]} && ${useGroupHeal}) {
		/call core_cast "${healWholeGroup}" 0 ${Me.ID} check_HP_${Me.Class.ShortName}
		/if (${Macro.Return.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
	}
   
  :exit
/return



|***
 *	DES: MGB cleric celestial regeneration if both AAs are ready
 *	USE: /bc cr, /cr
 *	INI: 
 *  NOTE: 
 *	Will not cast cr if the buff is present or if your name is Norbert.
 ***|
Sub cast_celest_regen()
	DEBUG ${cbug}cast_celest_regen()\ax ${If[${Me.AltAbilityReady[Mass Group Buff]},${cgood}MGB\ax,${cbad}MGB\ax]} ${If[${Me.AltAbilityReady[Celestial Regeneration]},${cgood}CR\ax,${cbad}CR\ax]}
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[Celestial Regeneration]} && !${Bool[${Me.Song[Celestial Regeneration]}]}) {
		/call core_cast "Mass Group Buff" alt ${Me.ID}
		/call core_cast "Celestial Regeneration" alt ${Me.ID}
		/if (${Bool[${Raid.Members}]}) /rs ${CRTXT}
	} else /if (${Bool[${Me.Song[Celestial Regeneration]}]}) {
		VOUT ${cbad}QUIT\ax trying to cast Celestial Regeneration while its running.	
	}		
	:exit
/return



|***
 *	DES: Will the cleric floatie shield AE regen. Twin cast it if the AA is ready.
 *	USE: spell must be memed.
 *	INI: 
 *  NOTE: 
 *  NOTE.. THIS IS BROKEN.. It's not my fault.
 ***|
sub cast_issuance_heal()
	DEBUG ${cbug}cast_issuance_heal()\ax
	ISDEAD
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/if (${Me.SpellReady[${healIssuance}]}) /call castFreeTargetSpell "${healIssuance}" ${Me.ID}
	/delay 3
	/if (${Me.AltAbilityReady[Forceful Rejuvenation]}) {
		/call MQ2Cast "Forceful Rejuvenation" alt
		/delay 3
		/if (${Me.SpellReady[${healIssuance}]}) /call castFreeTargetSpell "${healIssuance}" ${Me.ID}
	} 
	:exit
/return



|***
 *	DES: Will cast Temp pet
 *	USE: must have a MA set and /companion
 *	INI: 
 *  NOTE: 
 ***|
sub cast_temp_pet_CLR()
	DEBUG ${cbug}cast_temp_pet()\ax
	ISDEAD
	/doevents
	CHECKTIE
	/call get_assist_target
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/call is_target_dead
	/if (${Macro.Return} || ${Pet.ID}) /goto :exit
	/if (${Spawn[${Target2Kill}].Distance} < ${MobAgro} && ${Spawn[${Target2ll}].PctHPs} <= ${sendPetPerc}) {
		/if (${setToastMode} || (${AutoToast} && ${NameList.Find[${Target.CleanName}]}) && ${Me.AltAbilityReady[Celestial Hammer]}) /call core_cast "Celestial Hammer" alt ${Target2Kill}    
		VOUT Sending ${cpc}${tempPet}\ax ${sep} [${ctar}${Spawn[${Target2Kill}].CleanName}\ax].
		/call MQ2Cast "${tempPet}" -targetid|${Target2Kill}
	}
	:exit
/return



|***
 *	DES: EQBC Buff Heal Ward of Surety spell
 *	USE: have the spell memed and the toon identified.
 *	INI: [Heal] -> HealWardToon
 *  NOTE: 
 ***|
sub check_heal_ward()
	DEBUG ${cbug}check_heal_ward()\ax
	/if (!${Me.SpellReady[${healWard}]}) /goto :exit
	
	/declare _healwardID int local ${Spell[${healWard}].ID}
	/declare _wardToon string local
	/declare _count int local
	/declare _toon string local ${healWardToon}

	/for _count 1 to ${_toon.Count[|]}
		ISDEAD
		/doevents
		CHECKTIE
		/call check_exit
		/if (${Macro.Return}) /goto :exit
		/varset _wardToon ${_toon.Arg[${_count},|]}
		/if (${Bool[${Spawn[${NetBots[${_wardToon}].ID}]}]} && ${Spawn[id ${NetBots[${_wardToon}].ID}].Distance} < ${Spell[${healWard}].MyRange} && ${Me.SpellReady[${healWard}]}) {
			DEBUG  ${_count}: ${_toon.Count[|]}, ID:${Spawn[${_wardToon}].ID}, ${_toon.Arg[${_count},|]}, ${Bool[${Spawn[${_wardToon}].ID}]}, found: ${Bool[${NetBots[${_wardToon}].ShortBuff.Find[${_healwardID}]}]}
			/if (${Bool[${NetBots[${_wardToon}].ShortBuff.Find[${_healwardID}]}]} || ${Spawn[${_wardToon}].Type.Equal[corpse]}) /goto :nextcount
			/call core_cast "${healWard}" 0 ${Spawn[${_wardToon}].ID}
		}
		:nextcount
	/next _count	
	:exit
	/if (${AutoBot}) /call create_timer timer_heal_ward ${tHealWard}
/return



|***
 *	DES: Swap the Control Toon during raids
 *	USE: /autocontrol
 *	INI: 
 *  NOTE: THIS DOES NOT WORK.. /cry
 ***|
sub auto_control_swap()
	DEBUG ${cbug}auto_control_swap()\ax
	/if (!${autoControl}) /goto :exit
	ISDEAD
	/doevents


	/if (${Bool[${useSmartAssist1}]}) {
		/target ID ${Me.XTarget[1].ID}
		/delay 1
|		/delay 1s ${Target.ID} == ${Me.XTarget[1].ID}
	} else /if (${Bool[${useSmartAssist2}]}) {
		/target ID ${Me.XTarget[2].ID}
		/delay 1s ${Target.ID} == ${Me.XTarget[2].ID}
	}
	
	/target ${Me.TargetOfTarget}
	
	/call Event_SetVar NA toonControl Common ${Target.CleanName}
	/if (${Bool[${toonControlXTslot}]} && ${toonControlProtect}) /call protect_always_xt

	/call create_timer timer_auto_toon_control ${tAuto_Control_Toon}
	:exit
/return�