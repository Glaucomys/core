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
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}()\ax
	/doevents
	AMIDEAD

	/if (${Bool[${toonControl}]} && ${Bool[${toonControlXTslot}]} && ${toonControlProtect}) /call protect_always_xt
	CHECKTIE

  /if (!${Bool[${timer_set_xtarget}]} && ${switchHealXtarget}) {
    /call set_xtarget
    /call create_timer timer_set_xtarget ${tXHeal}
  }	
	
	/if (${coreAuto}) {
		/doevents
		/call check_engaged
		/if (!${Macro.Return} && ${switchPcPet}) /call check_pet
	}	


	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /return

	CHECKREZ
	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
		/call set_combat NUKE
		/call send_pet
		/call check_deBuffs ${Target2Kill}
		/call disc_${Me.Class.ShortName}
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke
		/call disc_${Me.Class.ShortName}			
		/call click_DPS_${Me.Class.ShortName}
		/call click_item
	} else /if (${combat.Equal[HEAL]}) {
		/call set_combat HEAL
		/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
		/if (${switchHealXTarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/if (${switchHealPet}) /call check_pet_heal_${Me.Class.ShortName}
		/call check_deBuffs ${Target2Kill}
	} else /if (${Select[${combat},DUAL]}) {
		CHECKREZ
		/call set_combat HEAL
		/call send_pet
		/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
		/call is_target_valid ${Target2Kill}
		/if (${isValidTarget}) {
			CHECKREZ
			/call set_combat NUKE
			/call check_deBuffs ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/if (${DoDoT}) /call cast_DOT
			/if (${DoNuke}) /call cast_nuke
			/call disc_${Me.Class.ShortName}			
			/call click_DPS_${Me.Class.ShortName}
			/call click_item
		}
	}
	/if (${coreAuto}) {	
		/call use_mod_rod
		/if (${switchHealCure}) /call cast_cure
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
	}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} || !${coreAuto} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Macro.Return}) /goto :killtarget
	}
/return



|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: Disc
 *	INI: 
 *  NOTE: 
 ***|
sub disc_DRU()
	DEBUGCLASS	${cbug}disc.${Me.Class.ShortName}()\ax
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	/if (${Me.AltAbilityReady[Storm Strike]}) /call core_cast "Storm Strike" alt ${Target2Kill} FALSE
	/if (${Me.AltAbilityReady[Season's Wrath]}) /call core_cast "Season's Wrath" alt ${Target2Kill} FALSE
/return



|***
 *	DES: emergency healing
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_DRU()
	DEBUGCLASS ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	CHECKEXIT
	/if (${Me.PctHPs} <= 50*.${tmpHealOverride} && ${Me.AltAbilityReady[Convergence of Spirits]}) /call core_cast "Convergence of Spirits" alt ${Me.ID} FALSE
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0 FALSE
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0 FALSE
	/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${MobAgro} playerstate 4 targetable]} >= 4 && ${Me.ItemReady[${CircleofLife}]} && !${Me.Song[${FindItem[${CircleofLife}].Spell}].ID} && ${Spell[${FindItem[${CircleofLife}].Spell}].Stacks}) /call core_cast "${CircleofLife}" item 0 FALSE
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_DRU()
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${Bool[${Epic}]} && ${Me.ItemReady[${Epic}]} && ${Spell[${FindItem[${Epic}].Spell}].Stacks} && !${Me.Buff[${FindItem[${Epic}].Spell}].ID} && !${Me.Song[${FindItem[${Epic}].Spell}].ID})  /call core_cast "${Epic}" item ${Target2Kill} FALSE
	/call click_horn_of_unity 	
/return



|***
 *	DES: casts druid Blessing of Ro AA deBuff
 *	USE: automatic
 *	INI: 
 *  NOTE: 
 ***|
sub cast_AA_ro(int _tmpID)
	DEBUGCLASS ${cbug}cast_AA_ro()\ax
	CHECKEXIT
	/doevents
	/if (${timer_ro_${_tmpID}} || ${Target.Buff[Blessing of Ro].ID} || ${Target.Buff[Fixation of Ro].ID}) /return
	/if (${is_target_dead[${_tmpID}]}) /return
	
	| /if ((${Target.BuffDuration[]} <= 2 && ${Spawn[${Target.Buff[].Caster}].ID} == ${Me.ID}) || !${Target.Buff[].ID}) /return
	/if (${Bool[${debuffRo}]} && ${Me.AltAbilityReady[${debuffRo}]}) {
		/call core_cast "${debuffRo}" alt ${_tmpID} FALSE
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
	DEBUGCLASS ${cbug}check_heal_group_DRU()\ax
	/if (${Me.State.Equal[FEIGN]}) /return
	/declare _count int	local 0

	:loopheal
	CHECKEXIT
	/doevents
	CHECKTIE
	/varset WorstHurt 				${Me.ID}
	/varset WorstHP 					${If[${HealOverride},${Float[${HealOverride}].Centi},99.99]}
	/varset WorstClass 				DRU
	/varset MinorHurt 				0
	/varset MajorHurt					0
	
	| Does the control toon need healing?
	/if (!${Bool[${Group.Member[${toonControl}]}]} && ${Bool[${toonControl}]} && ${toonControlProtect} && (${Bool[${RampageTank}]} && ${RampageTank.NotEqual[${toonControl}]})) {
		/if (!${Spawn[${toonControl}].Dead} && ${Bool[${Spawn[${toonControl}]}]} && ${Spawn[${toonControl}].ID} && ${Spawn[${toonControl}].PctHPs} <= ${WorstHp} && (${Spawn[${toonControl}].Distance} <= ${Spell[${healRemedy1}].MyRange} && ${Spawn[${toonControl}].PctHPs} <= ${${Spawn[${toonControl}].Class.ShortName}Point}*.${tmpHealOverride} || ${Spawn[${toonControl}].Distance} <= ${Spell[${healRenewal1}].MyRange} && ${Spawn[${toonControl}].PctHPs} <= ${${Spawn[${toonControl}].Class.ShortName}Point}*.${tmpHealOverride}*.7) && (${Bool[${RampageTank}]} && ${RampageTank.NotEqual[${toonControl}]})) {
			/varset WorstHurt ${Spawn[${toonControl}].ID}
			/varset WorstHp ${Spawn[${toonControl}].PctHPs}
			/varset WorstClass ${Spawn[${toonControl}].Class.ShortName}
		} 
	}

	/for _count 0 to ${Group.Members}
		/if (${Bool[${RampageTank}]} && ${RampageTank.Equal[${Group.Member[${_count}].DisplayName}]}) /continue
		/if (!${Group.Member[${_count}].Dead} && (${Spawn[pc id ${Group.Member[${_count}].ID}].ID} || ${Spawn[mercenary id ${Group.Member[${_count}].ID}].ID}) && ${Group.Member[${_count}].PctHPs} <= ${WorstHp} && (${Group.Member[${_count}].Distance} <= ${Spell[${healRejuvilation}].MyRange} && ${Group.Member[${_count}].PctHPs} <= ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride} || ${Group.Member[${_count}].Distance} <= ${Spell[${healSanavida}].MyRange} && ${Group.Member[${_count}].PctHPs} <= ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride})) {
			/varset WorstHurt ${Group.Member[${_count}].ID}
			/varset WorstHp ${Group.Member[${_count}].PctHPs}
			/varset WorstClass ${Group.Member[${_count}].Class.ShortName}
		} 
		
    /if (!${Spawn[pc id ${Group.Member[${_count}].ID}].Dead} && (${Group.Member[${_count}].PctHPs} < 85*.${tmpHealOverride} || ${Group.Member[${_count}].PctHPs} < ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}) && ${Group.Member[${_count}].PctHPs} > ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}*.7 && ${Group.Member[${_count}].Distance} <= ${Spell[${healWholeGroup}].AERange}) {
      /varcalc MinorHurt ${MinorHurt}+1
    } else /if (!${Spawn[pc id ${Group.Member[${_count}].ID}].Dead} && ${Group.Member[${_count}].PctHPs} < ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}*.7 && ${Group.Member[${_count}].Type.NotEqual[Corpse]} && ${Group.Member[${_count}].Distance} <= ${Spell[${healWholeGroup}].AERange}) {
      /varcalc MajorHurt ${MajorHurt}+1
    }
	 	/doevents
  /next _count
	CHECKEXIT


  /if (${Bool[${Me.Buff["Divine Aura"]}]} || ${Bool[${Me.Buff["Divine Barrier"]}]} || ${Bool[${Me.Buff["Sacred Barrier"]}]} ) /return
  UPDATEHUD "HEAL"
  CHECKREZ

 	/call check_no_cast_buffs

	| Panic Heals 
	/if (${Spawn[${WorstHurt}].PctHPs} <= 45*.${tmpHealOverride} && ${Spawn[${WorstHurt}].Distance} <= ${Spell[${healPanic}].MyRange} && ${Me.SpellReady[${healPanic}]}) {
		/call core_cast "${healPanic}" 0 ${WorstHurt} FALSE
		/if (${castReturn.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
	}
	/if (!${Me.SpellReady[${healRejuvilation}]} && ${Spawn[${WorstHurt}].PctHPs} < ${${WorstClass}Point}*.6*.${tmpHealOverride} && ${Me.AltAbilityReady[peaceful Convergence of Spirits]} && ${Spawn[${WorstHurt}].Distance} <= ${Spell[${healPanic}].MyRange}) {
		/call core_cast "Convergence of Spirits" alt ${WorstHurt} FALSE
	}
	/if (!${Me.SpellReady[${healRejuvilation}]} && ${Spawn[${WorstHurt}].PctHPs} < ${${WorstClass}Point}*.5*.${tmpHealOverride} && ${Me.AltAbilityReady[Convergence of Spirits]} && ${Spawn[${WorstHurt}].Distance} <= ${Spell[${healPanic}].MyRange}) {
		/call core_cast "peaceful Convergence of Spirits" alt ${WorstHurt} FALSE
	}
	/if (!${Me.SpellReady[${healRejuvilation}]} && ${Spawn[${WorstHurt}].PctHPs} < 35*.${tmpHealOverride} && ${Me.AltAbilityReady[Swarm of Fireflies]} && ${Spawn[${WorstHurt}].Distance} <= ${Spell[Swarm of Fireflies].MyRange}) {
		/call core_cast "Swarm of Fireflies" alt ${WorstHurt} FALSE
	}
  
	| Group Heal
	/doevents
	/if (${useGroupHeal}) {
	  /if (${MajorHurt}+${MinorHurt} > 1 && ${Me.SpellReady[${healWholeGroup}]}) {
	    /call core_cast "${healWholeGroup}" 0 ${Me.ID} FALSE
	    /if (${Select[${castReturn},CAST_SUCCESS]} && !${Select[${castReturn},CAST_OUTOFMANA]})  /goto :loopheal
	  } 	
	 	/if (${MajorHurt}+${MinorHurt} > 2 && ${Me.AltAbilityReady[peaceful Spirit of the Wood]}) {
			/call core_cast "peaceful Spirit of the Wood" alt ${Me.ID} FALSE
		}
	 	/if (${MajorHurt}+${MinorHurt} > 3 && ${Me.AltAbilityReady[Blessing of Tunare]}) {
			/call core_cast "Blessing of Tunare" alt ${Me.ID} FALSE
		}

	}
	
	CHECKEXIT
	/doevents

	| Rejuvilation Heal
	/if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.${tmpHealOverride} && ${Me.SpellReady[${healRejuvilation}]}) {
		/if (${Me.SpellReady[${healRejuvilation}]}) {
			/call core_cast "${healRejuvilation}" 0 ${WorstHurt} FALSE
		} 
		/goto :loopheal
	}
	CHECKEXIT
	/doevents
 	
	| Adrenaline / Sanavida Heal 
	/if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.${tmpHealOverride} && (${Me.SpellReady[${healAdrenaline}]} || ${Me.SpellReady[${healSanavida}]})) {
		/if (${Me.SpellReady[${healAdrenaline}]}) {
			/call core_cast "${healAdrenaline}" 0 ${WorstHurt}  FALSE
		} else /if (${Me.SpellReady[${healSanavida}]}) {
			/call core_cast "${healSanavida}" 0 ${WorstHurt} FALSE
		} 
		/goto :loopheal
	}  
   	UPDATEHUD
/return



|***
 *	DES: Check XTargets and heal the bastards if the need it.
 *	USE: /healxtarget
 *	INI: 
 *  NOTE: 
 ***|
sub check_healXTarget_DRU()
	DEBUGCLASS ${cbug}check_HealXTarget_${Me.Class.ShortName}()\ax
	AMIDEAD

	/if (${Me.State.Equal[FEIGN]}) /return
  /declare _count int local
  /for _count 1 to 13
  	/doevents
  	UPDATEHUD "HEAL XTarget"

    /if (${Spawn[${Me.XTarget[${_count}].ID}].ID} && (${Spawn[${Me.XTarget[${_count}].ID}].Type.Equal[PC]} || ${ProtectNPCList.Find[${Spawn[${Me.XTarget[${_count}].ID}].CleanName}]}) && ${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} <= ${${Spawn[id ${Me.XTarget[${_count}].ID}].Class.ShortName}Point}*.${tmpHealOverride}) {
      /if (${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} <= 45 && ${Spawn[${Me.XTarget[${_count}].ID}].Distance} <= ${Spell[${healPanic}].MyRange} && ${Me.SpellReady[${healPanic}]}) {
        /call core_cast "${healPanic}" 0 ${Me.XTarget[${_count}].ID} FALSE
      } else /if (${Spawn[${Me.XTarget[${_count}].ID}].Distance} <= ${Spell[${healRenewal1}].MyRange} && ${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} <= ${${Spawn[id ${Me.XTarget[${_count}].ID}].Class.ShortName}Point}*.${tmpHealOverride} && (${Me.SpellReady[${healRenewal1}]} || ${Me.SpellReady[${healRenewal2}]} && || ${Me.SpellReady[${healRenewal3}]})) {
        /if (${Me.SpellReady[${healRenewal1}]}) {
          /call core_cast "${healRenewal1}" 0 ${Me.XTarget[${_count}].ID} FALSE
        } else /if (${Me.SpellReady[${healRenewal2}]}) {
          /call core_cast "${healRenewal2}" 0 ${Me.XTarget[${_count}].ID} FALSE
        } 
      } else /if (${Spawn[${Me.XTarget[${_count}].ID}].Distance} <= ${Spell[${healIntervention1}].MyRange} && ${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} <= ${${Spawn[id ${Me.XTarget[${_count}].ID}].Class.ShortName}Point}*.${tmpHealOverride} && (${Me.SpellReady[${healIntervention1}]} || ${Me.SpellReady[${healIntervention2}]} || ${Me.SpellReady[${healIntervention3}]})) {
        /if (${Me.SpellReady[${InterventionSpell1}]}) {
          /call core_cast "${healIntervention1}" 0 ${Me.XTarget[${_count}].ID} FALSE
        } else /if (${Me.SpellReady[${healIntervention2}]}) {
          /call core_cast "${healIntervention2}" 0 ${Me.XTarget[${_count}].ID} FALSE
        } 
      } 
    }
  /next _count
	UPDATEHUD
/return



|***
 *	DES: Checks and Heals Pets. Group ONLY
 *	USE: /healpet [toggle]
 *	INI: 
 *  NOTE: 
 ***|
sub check_pet_heal_DRU()
	DEBUGCLASS ${cbug}checkPetHeal.${Me.Class.ShortName}()\ax
	AMIDEAD

	/if (${Me.State.Equal[FEIGN]} || ${Me.Moving} || ${Target.ID} == ${Me.Pet.ID}) /return
	/declare _count int local
	/varset pWorstHurt ${Me.CleanName}
	/varset pWorstHP 100
	/varset pWorstHurtID ${Me.ID}
	/varset pWorstClass NULL
	/varset pMajorHurt 0

	/for _count 0 to ${Group.Members}
		/doevents
		/if (${Group.Member[${_count}].Pet.ID}) {
			/if (${Group.Member[${_count}].Pet.Distance} <= ${Spell[${healRenewal1}].MyRange}) {
				/if ((${Group.Member[${_count}].Pet.PctHPs} <= ${pWorstHP}) && (${Group.Member[${_count}].Pet.PctHPs} <= ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride})) {
					/varset pWorstClass ${Group.Member[${_count}].Pet.Class.ShortName}
					/varset pWorstHurt ${Group.Member[${_count}].Pet.Name}
					/varset pWorstHurtID ${Group.Member[${_count}].Pet.ID}
					/varset pWorstHP ${Group.Member[${_count}].Pet.PctHPs}
				}
			}
		}
	/next _count

	/if (${pWorstHurtID} == ${Me.ID}) /return
	/if (${Spawn[${pWorstHurtID}].PctHPs} < ${${pWorstClass}Point}*.${tmpHealOverride} && ${Spawn[${pWorstHurtID}].Distance} <= ${Spell[${healRenewal1}].MyRange} && ${Me.SpellReady[${healRenewal1}]}) {
		/call core_cast "${healRenewal1}" 0 ${pWorstHurtID} FALSE
	} else /if (${Spawn[${pWorstHurtID}].PctHPs} < ${${pWorstClass}Point}*.${tmpHealOverride} && ${Spawn[${pWorstHurtID}].Distance} <= ${Spell[${healRenewal2}].MyRange} && ${Me.SpellReady[${healRenewal2}]}) {
		/call core_cast "${healRenewal2}" 0 ${pWorstHurtID} FALSE
	}

/return



|***
 *	DES: MGB druid SoWood if both AAs are ready
 *	USE: /bc wood, /wood
 *	INI: 
 *  NOTE: Will not cast wood if the buff is present.
 ***|
sub cast_AA_spirit_wood()
	DEBUGCLASS ${cbug}cast_AA_wood()\ax ${If[${Me.AltAbilityReady[Mass Group Buff]},${cgood}MGB\ax,${cbad}MGB\ax]} ${If[${Me.AltAbilityReady[Spirit of the Wood]},${cgood}Wood\ax,${cbad}Wood\ax]}
	AMIDEAD
	/doevents
	/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[Spirit of the Wood]} && !${Bool[${Me.Song[Spirit of the Wood]}]}) {
		/call core_cast "Mass Group Buff" alt 0 FALSE
		/call core_cast "Spirit of the Wood" alt 0 FALSE
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
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	CHECKEXIT
	/doevents
	
	| swarm pets
	/if (${Me.AltAbilityReady[Natures Guardian]}) /call core_cast "Natures Guardian" alt ${Target2Kill} FALSE
	| /if (${Me.AltAbilityReady[Spirits of Nature]}) /call core_cast "Spirits of Nature" alt ${Target2Kill} FALSE
	
	| defense
	/if (${Me.AltAbilityReady[Protection of Direwood]}) /call core_cast "Protection of Direwood" alt 0 FALSE
	/if (${Me.AltAbilityReady[Silent Casting]}) /call core_cast "Silent Casting" alt 0 FALSE

	| damage boost
	/if (${Me.AltAbilityReady[Distructive Vortex]}) /call core_cast "Distructive Vortex" alt 0 FALSE
	/if (${Me.AltAbilityReady[Natures Fury]}) /call core_cast "Natures Fury" alt 0 FALSE
	/if (${Me.AltAbilityReady[Group Spirit of the Black Wolf]}) /call core_cast "Group Spirit of the Black Wolf" alt 0 FALSE
	/if (${Me.AltAbilityReady[Seasons Wrath]}) /call core_cast "Seasons Wrath" alt ${Target2Kill} FALSE
	
	| heal boost
	/if (${Me.AltAbilityReady[Nature's Blessing]}) /call core_cast "Nature's Blessing" alt 0 FALSE

	/call cast_AA_spire
	
	| AA AE heal
	/if (${useGroupHeal} && ${Me.AltAbilityReady[Nature's Boon]}) /call core_cast "Nature's Boon" alt 0 FALSE

	| epic 2.0
	/if (${Me.ItemReady[${${Me.Class.ShortName}Epic2}]} && ${Spell[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].Stacks} && !${Me.Buff[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID} && !${Me.Song[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID}) /call core_cast "${${Me.Class.ShortName}Epic2}" item 0 FALSE



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
sub Bind_setDRU(string _classType, string _classVerbage)
	DEBUG ${cbug}Bind_setDRU(type:${_classType}, verbage:${_classVerbage})\ax
	/if (!${validate_class[TRUE, |DRU]}) /return

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
	/if (${Bool[${_classType.Equal[groupheal]}]}) {
		/call writeINI useGroupHeal Heal ${If[${useGroupHeal},FALSE,TRUE]} 	
	 	/call echos switch "Use Group Heal" useGroupHeal 0 0

	| set Class Heal Point	
	} else /if (${Bool[${_classType.Find[healpoint]}]}) {
		/if (${Bool[${_classVerbage}]}) /declare _num int local ${_classVerbage}
		/if (${Target.ID} && ${_num} && ${Spawn[${Target.ID}].Type.NotEqual[Pet]}) {
			/call writeINI ${Target.Class.ShortName}Point Heal ${_num}
			VOUT ${ctar}${Target.Class}\ax Heal Point ${sep} ${cinfo}${_classVerbage}\ax
		} else /if (${Target.ID} && ${_num} && ${Spawn[${Target.ID}].Type.Equal[Pet]}) {
			/call writeINI PetPoint Heal ${_num}
			VOUT ${ctar}Pet\ax Heal Point ${sep} ${cinfo}${_classVerbage}\ax
		} else {
			VOUT /dru healpoint [${cinfo}##\ax] [${cinfo}TARGET\ax]
		}

	| set XTar heal radius
	} else /if (${Bool[${_classType.Equal[xtradius]}]}) {
		/if (${Range.Between[1,300:${Int[${_classVerbage}]}]}) {
			/call writeINI XTHealRadius Heal ${_classVerbage}
			/call echos switch "XTarget Heal Radius ${sep}" XTHealRadius 0 0
		} else /if (!${Bool[${_classVerbage}]} || !${Range.Between[1,300:${Int[${_classVerbage}]}]}) {
			VOUT /${Me.Class.ShortName.Lower} xtradius [${cinfo}${XTHealRadius}\ax] allowable range ${sep} ${cinfo}1 - 300\ax
		}	

	| set XT refresh list Time
	} else /if (${Bool[${_classType.Equal[xttime]}]}) {
		/if (${Range.Between[1,5:${Int[${_classVerbage}]}]}) {
			/call writeINI tXHeal Timer ${_classVerbage}
			/call echos number "XTarget refresh time" ${tXHeal} 0 0
		} else /if (!${Bool[${_classVerbage}]} || !${Range.Between[1,5:${Int[${_classVerbage}]}]}) {
			VOUT /${Me.Class.ShortName.Lower} xttime [${cinfo}${tXHeal}m\ax] allowable range ${sep} ${cinfo}1 - 5 min\ax
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
		VOUT /${Me.Class.ShortName.Lower} ${_classType.Lower} [${_listout}]	

	} else {
		/for _count 1 to ${${_bind}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
			/if (${${_bind}Type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}[${cgood}${spireGroup}\ax]
			/if (${${_bind}Type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}[${cgood}${spireRaid}\ax]
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
	} 
/return