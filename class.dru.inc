|***
 *	class.dru.inc
 *	Core Druid Include
 *
 *	
 ***|



|***
 *	DES: Main Druid Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_DRU()
	CHECKINPUTS
	AMIDEAD
	CHECKTIE
	/if (${Bool[${toonControl}]} && ${Bool[${toonControlXTslot}]} && ${toonControlProtect}) /call protect_always_xt
  /if (!${Bool[${timer_set_xtarget}]} && ${switchHealXtarget}) {
    /call set_xtarget
    /call create_timer timer_set_xtarget ${tXHeal}
  }	
	
	/if (${coreAuto}) {
		CHECKINPUTS
		/call check_engaged
		/if (!${Macro.Return} && ${switchPcPet}) /call check_pet
	}	

	/while (${prep_combat[]}) {
		/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
		/if (${switchHealCure}) /call cast_cure

		/if (${switchHealXtarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/if (${switchHealPet}) /call check_pet_heal_${Me.Class.ShortName}
		/if (${exit_class_loop[]}) /return

		/if (${isValidTarget}) {
			CHECKREZ
			/call set_combat ${combat.Upper}
			/if (${setBurnControl}) /call synergy_${Me.Class.ShortName}
			/if (${switchPcPet} && !${Me.Pet.ID}) /call cast_temp_pet_${Me.Class.ShortName}
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
			/if (${switchModRod} && ${validate_class[FALSE, ${CastList}]}) /call use_mod_rod
			/if (${validate_class[FALSE, ${MeleeList}]}) /call cast_stamina_recovery
		}
	}
/return



|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: Disc
 *	INI: 
 *  NOTE: 
 ***|
sub disc_DRU()
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return

	/if (${validate_cast[FALSE, alt, "Storm Strike", ${Target2Kill}]}) /call core_cast2 "Storm Strike" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "Season's Wrath", ${Target2Kill}]}) /call core_cast2 "Season's Wrath" alt ${Target2Kill} FALSE
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_DRU()
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	/call click_horn_of_unity 	
	
	| epic 2.0
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]} && ${Spawn[${Target2Kill}].Distance} <= ${CombatDistance}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE
	
	
/return



|***
 *	DES: casts druid Grasp of Ro AA deBuff
 *	USE: automatic
 *	INI: 
 *  NOTE: 
 ***|
sub cast_AA_ro(int _tmpID)
	CHECKEXIT
	CHECKINPUTS
	/if (${timer_ro_${_tmpID}} || ${Target.Buff[Grasp of Ro].ID}) /return
	/if (${is_target_dead[${_tmpID}]}) /return
	
	/if (${Bool[${debuffRo}]} && ${Me.AltAbilityReady[${debuffRo}]}) {
		/if (${validate_cast[FALSE, alt, "${debuffRo}", ${_tmpID}]}) /call core_cast2 "${debuffRo}" alt ${_tmpID} FALSE
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_RESISTED,CAST_RESIST,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_ro_${_tmpID} ${Spell[${debuffRo}].Duration.TotalSeconds}s
		}		
	}
/return



|***
 *	DES: Check Group and heal the fucks if the need it.
 *	USE: /healgroup
 *	INI: 
 *  NOTE: 
 ***|
sub check_heal_group_DRU()
	AMIDEAD
	CHECKEXIT
	/declare _count int local 0
	CHECKTIE
	/varset hurtID 						${Me.ID}
	/varset hurtPctHP 				${If[${HealOverride},${Float[${HealOverride}].Centi},99.99]}
	/varset hurtCLS 					${Me.Class.ShortName}
	/varset hurtCount					0


	| Does the control toon need healing?
	/if (${toonControlProtect} && ${Bool[${toonControl}]} && ${Bool[${Spawn[${toonControl}]}]}) {
		/if (!${Bool[${Group.Member[${toonControl}]}]} && ${RampageTank.NotEqual[${toonControl}]}) {
			/if (!${Spawn[${toonControl}].Dead} && ${Spawn[${toonControl}].ID} && ${Spawn[${toonControl}].PctHPs} <= ${hurtPctHP} && ${Spawn[${toonControl}].Distance} <= ${MobAgro} && ${Spawn[${toonControl}].PctHPs} <= ${${Spawn[${toonControl}].Class.ShortName}Point}*.${tmpHealOverride}) {
				/varset hurtID ${Spawn[${toonControl}].ID}
				/varset hurtPctHP ${Spawn[${toonControl}].PctHPs}
				/varset hurtCLS ${Spawn[${toonControl}].Class.ShortName}
			} 
		}
	}

	| Do group members need healing? ${Group.Member[${_count}].State.Equal[HOVER]}
	/if (${Bool[${Group.Members}]}) {
		/for _count 0 to ${Group.Members}
			/if (${Bool[${RampageTank}]} && ${RampageTank.Equal[${Group.Member[${_count}].DisplayName}]}) /continue
			| no heallist bitches
			/if (${heallist.Find[${Group.Member[${_count}].DisplayName}]}) /continue
			/if (!${Group.Member[${_count}].Dead} && (${Spawn[pc id ${Group.Member[${_count}].ID}].ID} || ${Spawn[mercenary id ${Group.Member[${_count}].ID}].ID}) && ${Group.Member[${_count}].PctHPs} <= ${hurtPctHP} && ${Group.Member[${_count}].Distance} <= ${MobAgro} && ${Group.Member[${_count}].PctHPs} <= ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}) {
				/varset hurtID ${Group.Member[${_count}].ID}
				/varset hurtPctHP ${Group.Member[${_count}].PctHPs}
				/varset hurtCLS ${Group.Member[${_count}].Class.ShortName}
			} 

	 		| set count for # of toons hurt
			/if (!${Group.Member[${_count}].Dead} && ${Group.Member[${_count}].PctHPs} < ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}) {
				/varcalc hurtCount ${hurtCount}+1
			}   
	    
	  /next _count
	}
	
  /if (${Bool[${Me.Buff["Divine Aura"]}]} || ${Bool[${Me.Buff["Divine Barrier"]}]} || ${Bool[${Me.Buff["Sacred Barrier"]}]} ) /return
	UPDATEHUD "HEAL"
	CHECKREZ
	/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
	
	| if no one is hurt. back to the start
	/if (!${hurtCount}) /return	
  
	CHECKEXIT


 	/call check_no_cast_buffs

	| Panic heal
	/if (${Me.SpellReady[${healPanic}]}) {
		/if (${Spawn[${hurtID}].PctHPs} < 20*.${HealOverride}) {
			/if (${Me.Casting.ID}) /stopcast
			/if (${validate_cast[FALSE, spell, "${healPanic}", ${hurtID}]}) /call core_cast2 "${healPanic}" 0 ${hurtID} FALSE
		}
	}  	
	
	
	| single AA heals
	/if (${Spawn[${hurtID}].PctHPs} < ${${hurtCLS}Point}*.6*.${tmpHealOverride} && ${Me.AltAbilityReady[peaceful Convergence of Spirits]}) {
		/if (${validate_cast[FALSE, alt, "Convergence of Spirits", ${hurtID}]}) /call core_cast2 "Convergence of Spirits" alt ${hurtID} FALSE
	}
	/if (${Spawn[${hurtID}].PctHPs} < ${${hurtCLS}Point}*.5*.${tmpHealOverride} && ${Me.AltAbilityReady[Convergence of Spirits]}) {
		/if (${validate_cast[FALSE, alt, "Peaceful Convergence of Spirits", ${hurtID}]}) /call core_cast2 "Peaceful Convergence of Spirits" alt ${hurtID} FALSE
	}
  
	| Group Heals
	CHECKINPUTS
	/if (${useGroupHeal}) {

	  /if (${hurtCount} && !${SafeZone}) {
	  	
	  	/if (${hurtCount} > 3 && ${Me.SpellReady[${healWholeGroup}]}) {
	  		/if (${validate_cast[FALSE, spell, "${healWholeGroup}", ${Me.ID}]}) /call core_cast2 "${healWholeGroup}" 0 ${Me.ID} FALSE
	  	} 	
	  	
		  /if (${hurtCount} == 1) {
		  } else /if (${hurtCount} == 2) {
		  } else /if (${hurtCount} == 3) {
		  	/if (${validate_cast[FALSE, alt, "Peaceful Spirit of the Wood", ${Me.ID}]}) /call core_cast2 "Peaceful Spirit of the Wood" alt ${Me.ID} FALSE

		  } else /if (${hurtCount} == 4) {
		  } else /if (${hurtCount} == 5) {
			 	/if (${validate_cast[FALSE, alt, "Swarm of Fireflies", ${Me.ID}]}) /call core_cast2 "Swarm of Fireflies" alt 0 FALSE
			} else /if (${hurtCount} == 6) {
			 	/if (${validate_cast[FALSE, alt, "Blessing of Tunare", ${Me.ID}]}) /call core_cast2 "Blessing of Tunare" alt 0 FALSE
			}			
	  }	
	
	}
	
	
	| single target heals
  	/if (${Me.SpellReady[${healRejuvilation}]}) {
     /if (${validate_cast[FALSE, spell, "${healRejuvilation}", ${hurtID}]}) /call core_cast2 "${healRejuvilation}" 0 ${hurtID} FALSE
    } else /if (${Me.SpellReady[${healSanavida}]}) {
      /if (${validate_cast[FALSE, spell, "${healSanavida}", ${hurtID}]}) /call core_cast2 "${healSanavida}" 0 ${hurtID} FALSE
    } else /if (${Me.SpellReady[${healAdrenaline}]}) {
      /if (${validate_cast[FALSE, spell, "${healAdrenaline}", ${hurtID}]}) /call core_cast2 "${healAdrenaline}" 0 ${hurtID} FALSE
    } 
	
	UPDATEHUD FALSE
/return



|***
 *	DES: Fast check to see if anyone is hurt worse then person currently being healed.
 *	USE: added as a subcheck for core_cast
 *	INI: 
 *  NOTE: 
 ***|
sub check_HP_DRU()
	AMIDEAD
	/if (!${Me.Casting.ID}) /return
	/declare _count int local 0

	/varset hurtID 				${Me.ID}
	/varset hurtPctHP 					${If[${HealOverride},${Float[${HealOverride}].Centi},99.99]}
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
		
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/if (${Me.Casting.ID}) /stopcast
  	/if (${Me.SpellReady[${healRejuvilation}]}) {
     /if (${validate_cast[FALSE, spell, "${healRejuvilation}", ${hurtID}]}) /call core_cast2 "${healRejuvilation}" 0 ${hurtID} FALSE
    } else /if (${Me.SpellReady[${healSanavida}]}) {
      /if (${validate_cast[FALSE, spell, "${healSanavida}", ${hurtID}]}) /call core_cast2 "${healSanavida}" 0 ${hurtID} FALSE
    } else /if (${Me.SpellReady[${healAdrenaline}]}) {
      /if (${validate_cast[FALSE, spell, "${healAdrenaline}", ${hurtID}]}) /call core_cast2 "${healAdrenaline}" 0 ${hurtID} FALSE
    } 
  } 

/return


|***
 *	DES: Check XTargets and heal the bastards if the need it.
 *	USE: /healxtarget
 *	INI: 
 *  NOTE: 
 ***|
sub check_heal_XTarget_DRU()
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


	| panic heal
	/if (${Me.SpellReady[${healPanic}]}) {
		/if (${Spawn[id ${hurtID}].PctHPs} < 20*.${HealOverride}) {
			/if (${Me.Casting.ID}) /stopcast
			/if (${validate_cast[FALSE, spell, "${healPanic}", ${hurtID}]}) /call core_cast2 "${healPanic}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
		}
	}
	
  /if (${Spawn[${hurtID}].PctHPs} <= ${${hurtCLS}Point}*.${tmpHealOverride}) {
		/if (${Me.Casting.ID}) /stopcast
  	/if (${Me.SpellReady[${healRejuvilation}]}) {
     /if (${validate_cast[FALSE, spell, "${healRejuvilation}", ${hurtID}]}) /call core_cast2 "${healRejuvilation}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
    } else /if (${Me.SpellReady[${healSanavida}]}) {
      /if (${validate_cast[FALSE, spell, "${healSanavida}", ${hurtID}]}) /call core_cast2 "${healSanavida}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
    } else /if (${Me.SpellReady[${healAdrenaline}]}) {
      /if (${validate_cast[FALSE, spell, "${healAdrenaline}", ${hurtID}]}) /call core_cast2 "${healAdrenaline}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
    } 
  } 	

	UPDATEHUD FALSE
/return



|***
 *	DES: Checks and Heals Pets. Group ONLY
 *	USE: /healpet [toggle]
 *	INI: 
 *  NOTE: 
 ***|
sub check_pet_heal_DRU()
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
  
  /if (${Spawn[${hurtID}].PctHPs} < ${PetPoint}) {
  	/if (${Me.SpellReady[${healRejuvilation}]}) {
    	/if (${validate_cast[FALSE, spell, "${healRejuvilation}", ${hurtID}]}) /call core_cast2 "${healRejuvilation}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
  	} else /if (${Me.SpellReady[${healSanavida}]}) {
    	/if (${validate_cast[FALSE, spell, "${healSanavida}", ${hurtID}]}) /call core_cast2 "${healSanavida}" 0 ${hurtID} check_HP_${Me.Class.ShortName}
    }
  } 
  
/return



|***
 *	DES: MGB druid SoWood if both AAs are ready
 *	USE: /bc wood, /wood
 *	INI: 
 *  NOTE: Will not cast wood if the buff is present.
 ***|
sub cast_AA_spirit_wood()
	AMIDEAD
	CHECKINPUTS
	/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[Spirit of the Wood]} && !${Bool[${Me.Song[Spirit of the Wood]}]}) {
		/if (${validate_cast[FALSE, alt, "Mass Group Buff", ${Me.ID}]}) /call core_cast2 "Mass Group Buff" alt 0 FALSE
		/if (${validate_cast[FALSE, alt, "Spirit of the Wood", ${Me.ID}]}) /call core_cast2 "Spirit of the Wood" alt 0 FALSE
	} else /if (${Bool[${Me.Song[Spirit of the Wood]}]}) {
		VOUT ${owarning} ${sep} trying to cast Spirit of the Wood while its running.	
	}		
/return



 |***
 *	DES: Druid burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_DRU()
	CHECKEXIT
	CHECKINPUTS
	
	| swarm pets
	/if (${validate_cast[FALSE, alt, "Natures Guardian", ${Target2Kill}]}) /call core_cast2 "Natures Guardian" alt ${Target2Kill} FALSE
	
	| defense
	/if (${validate_cast[FALSE, alt, "Protection of Direwood", ${Me.ID}]}) /call core_cast2 "Protection of Direwood" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Silent Casting", ${Me.ID}]}) /call core_cast2 "Silent Casting" alt 0 FALSE

	| damage boost
	/if (${validate_cast[FALSE, alt, "Distructive Vortex", ${Me.ID}]}) /call core_cast2 "Distructive Vortex" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Natures Fury", ${Me.ID}]}) /call core_cast2 "Natures Fury" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Group Spirit of the Black Wolf", ${Me.ID}]}) /call core_cast2 "Group Spirit of the Black Wolf" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Seasons Wrath", ${Target2Kill}]}) /call core_cast2 "Seasons Wrath" alt ${Target2Kill} FALSE
	
	| heal boost
	/if (${validate_cast[FALSE, alt, "Nature's Blessing", ${Me.ID}]}) /call core_cast2 "Nature's Blessing" alt 0 FALSE

	/call cast_AA_spire
	
	| AA AE heal
	/if (${validate_cast[FALSE, alt, "Nature's Boon", ${Me.ID}]}) /call core_cast2 "Nature's Boon" alt 0 FALSE

	/call synergy_click_Circle
	/call click_item_burn
/return



|***
 *	DES: Class controls
 *	USE: /dru [groupheal]
 *	INI: 
 *  NOTE:
 ***|
#bind setDRU /dru
sub Bind_setDRU(string _classType, string _verbage)
	/if (!${validate_class[TRUE, |DRU]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _type string	local |fade|groupheal|healpoint|rampage|rod|spiregroup|spireraid|xtradius|xttime
	
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

	| use Modulation Rods
	/if (${Bool[${_classType.Equal[rod]}]}) {
		/call writeINI switchModRod Buffs ${If[${switchModRod},FALSE,TRUE]} 	

	| use group Heals
	} else /if (${Bool[${_classType.Equal[groupheal]}]}) {
		/call writeINI useGroupHeal Heal ${If[${useGroupHeal},FALSE,TRUE]} 	
	 	/call echos switch "Use Group Heal" useGroupHeal 0 0
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

	| set rampage tank toon
	} else /if (${Bool[${_classType.Equal[rampage]}]}) {
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

	}
	/if (${switchMQclearonCLS}) /mqclear
	/for _count 1 to ${_type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_type.Arg[${_count},|]}\ax
		/if (${_type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${_type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${_type.Arg[${_count},|].Equal[xttime]}) /varset _listout ${_listout}(${cnum}${tXHeal}\ax)
		/if (${_type.Arg[${_count},|].Equal[xtradius]}) /varset _listout ${_listout}(${cnum}${XTHealRadius}\ax)
		/if (${_type.Arg[${_count},|].Equal[rampage]}) /varset _listout ${_listout}(${If[${Bool[${RampageTank}]},${cnum}${RampageTank}\ax,${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[groupheal]}) /varset _listout ${_listout}(${If[${useGroupHeal},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}(${If[${Bool[${switchModRod}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}(${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]})

		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return