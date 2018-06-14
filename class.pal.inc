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
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}()\ax
	/doevents
	AMIDEAD
	CHECKTIE
	
  /if (${switchHealXtarget}) {
  	/if (!${timer_set_xtarget}) {
	    /call set_xtarget
	    /call create_timer timer_set_xtarget ${tXHeal}
	  }	
	}
	
	:killtarget
	/if (${useMG}) {
		/if (!${flagCallNeedRez} && !${timer_check_MG}) /call cast_AA_marrs_gift
	}
	CHECKREZ
	/call prep_combat
	/if (${Macro.Return} && ${Bool[${combat}]}) {
		DEBUG combat:${cinfo}${combat.Upper}\ax		

		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${switchHealGroup}) /call check_splash_group_${Me.Class.ShortName}
		/if (${switchHealXtarget}) /call check_heal_XTarget_${Me.Class.ShortName}
		/if (${switchHealPet}) /call check_pet_heal_${Me.Class.ShortName}
		/if (${switchHealCure}) /call cast_cure

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
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
			/if (${validate_class[FALSE, ${CastList}]}) /call use_mod_rod
			/if (${validate_class[FALSE, ${MeleeList}]}) /call cast_stamina_recovery
			/if (!${is_target_dead[${Target2Kill}]}) /goto :killtarget
		} else /if (!${coreAuto}) {
			/if (${Target.ID} && ${Bool[${combat}]} && !${is_target_dead[${Target.ID}]}) /goto :killtarget
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
	DEBUGCLASS ${cbug}disc_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return
	CHECKTIE

	/if (${validate_cast[FALSE, activedisc, "${DiscFastRune}", ${Me.ID}]}) /call core_cast2 "${DiscFastRune}" disc 0 FALSE
	/if (${switchPull}) /call set_combat ${combat.Upper}

	| defensive	
	/if (${coreAuto} && ${switchAgro} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]} >= ${defCount}) {
		/declare _count int local 0
		/for _count 1 to 3
			/if (${Bool[${Me.ActiveDisc}]}) /break
			/if (${validate_cast[FALSE, activedisc, "${DefensiveDisc${_count}}", ${Me.ID}]}) /call core_cast2 "${DefensiveDisc${_count}}" disc 0 FALSE
		/next _count
	}
		
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}

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
	/if (${doYaulp}) /call cast_yaulp
	
/return



|***
 *	DES: emergency healing
 *	USE: /call patch_heal_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_PAL()
	DEBUGCLASS ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return

	| Hand of Piety AA Group heal	
	/if (${Me.PctHPs} < ${${Me.Class.ShortName}Point}*.50*.${tmpHealOverride} && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${combatRadius} targetable]} > 1 ) {
		/if (${validate_cast[FALSE, alt, "${AAHandofPiety}", ${Me.ID}]}) /call core_cast2 "${AAHandofPiety}" alt 0 FALSE
	}

	| Panic Heal 1
	/if (${Me.Gem[${healPanic1}]}) {
	  /if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.45*.${tmpHealOverride}) {
			/if (${validate_cast[FALSE, spell, "${healPanic1}", ${Me.ID}]}) /call core_cast2 "${healPanic1}" 0 FALSE
		}
	}

	| dpsheal
	/if (${Me.PctHPs} < ${${Me.Class.ShortName}Point}*.40*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, disc, "${DiscReflexive}", ${Target2Kill}]}) /call core_cast2 "${DiscReflexive}" disc ${Target2Kill} FALSE
	}

	| Panic Heal 2
	/if (${Me.Gem[${healPanic2}]}) {
	  /if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.20*.${tmpHealOverride}) {
			/if (${validate_cast[FALSE, spell, "${healPanic2}", ${Me.ID}]}) /call core_cast2 "${healPanic2}" 0 FALSE
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
	DEBUGCLASS ${cbug}get_agro_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${switchPull}) /call set_combat ${combat.Upper}

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
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	/if (${is_target_dead[${Target2Kill}]}) /return
	AMIDEAD
	CHECKEXIT
	/doevents
	/call click_horn_of_unity 	
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]} && ${Spawn[${Target2Kill}].Distance} <= ${CombatDistance}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE

/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub check_splash_group_PAL()
	DEBUGCLASS ${cbug}check_splash_group_PAL()\ax
	/declare _count int	local 0
	
	:loopheal
	/doevents
	CHECKTIE
	/varset WorstHurt 				${Me.ID}
	/varset WorstHP 					${If[${HealOverride},${Float[${HealOverride}].Centi},99.99]}
	/varset WorstClass 				${Me.Class.ShortName}
	/varset groupHurt					0
	
	| lets see whos hurt
	/if (${Bool[${Group.Members}]}) {
		/for _count 0 to ${Group.Members}
			/if (${Bool[${RampageTank}]} && ${RampageTank.Equal[${Group.Member[${_count}].DisplayName}]}) /continue
			AMIDEAD
			CHECKEXIT
		 	/doevents

			/if ((!${Group.Member[${_count}].Dead} && (${Spawn[pc id ${Group.Member[${_count}].ID}].ID} || ${Spawn[mercenary id ${Group.Member[${_count}].ID}].ID})) && ${Group.Member[${_count}].PctHPs} <= ${WorstHP} && ${Group.Member[${_count}].PctHPs} <= ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride} && (${Group.Member[${_count}].Distance} < ${Spell[${splashPanic}].MyRange} || ${Group.Member[${_count}].Distance} < ${Spell[${splashGroup}].MyRange})) {
				/varset WorstHurt ${Group.Member[${_count}].ID}
				/varset WorstHP ${Group.Member[${_count}].PctHPs}
				/varset WorstClass ${Group.Member[${_count}].Class.ShortName}
			} 
			
			| set count for # of toons hurt
			/if (!${Group.Member[${_count}].Dead} && ${Group.Member[${_count}].PctHPs} < ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}) {
				/varcalc groupHurt ${groupHurt}+1
			}
	  /next _count
	}
  
	| bail if ??
	/if (${Bool[${Me.Buff["Divine Aura"]}]} || ${Bool[${Me.Buff["Divine Barrier"]}]} || ${Bool[${Me.Buff["Sacred Barrier"]}]}) /return
	UPDATEHUD "SPLASH"
  
	| Panic Splash
	/if (${Bool[${Group.Members}]}) {
	  /if (${Spawn[${WorstHurt}].PctHPs} <= 45*.${tmpHealOverride} && ${Spawn[${WorstHurt}].Distance} <= ${Spell[${splashPanic}].MyRange}) {
			/if (${validate_cast[FALSE, spell, "${splashPanic}", ${Me.ID}]}) /call core_cast2 "${splashPanic}" 0 0 FALSE
			/if (${castReturn.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
		}
	}
		
	| group heals
	/if (${useGroupHeal}) {
		AMIDEAD
		CHECKEXIT
		| Group Splash
	  /if (${groupHurt} >= 1) {
	    /if (${validate_cast[FALSE, spell, "${splashGroup}", ${Me.ID}]}) /call core_cast2 "${splashGroup}" 0 0 FALSE
	    /if (${Macro.Return.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
	  } 
			
		| AA Splash
	  /if (${groupHurt} >= 3) {
	  	/if (${validate_cast[FALSE, alt, "${AAHandofPiety}", ${Me.ID}]}) /call core_cast2 "${AAHandofPiety}" alt 0 FALSE
	  }
	  
	}

	AMIDEAD
 	/doevents
	| Touch Heal 
  /if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.${tmpHealOverride}) {
  	/if (${validate_cast[FALSE, spell, "${healTouch}", ${WorstHurt}]}) /call core_cast2 "${healTouch}" 0 ${WorstHurt} FALSE
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
	DEBUGCLASS ${cbug}check_heal_XTarget_${Me.Class.ShortName}()\ax
	/if (!${validate_expansion[FALSE, |16]}) /return
  /declare _count int local 0
	CHECKREZ

  /for _count 1 to 13
  	/if (${Bool[${RampageTank}]} && ${RampageTank.Equal[${Spawn[${Me.XTarget[${_count}].ID}].DisplayName}]}) /continue
  	/if (!${Spawn[${Me.XTarget[${_count}].ID}].Type.Equal[PC]} && !${ProtectNPCList.Find[${Spawn[${Me.XTarget[${_count}].ID}].DisplayName}]}) /continue
  	/if (${heallist.Find[${Spawn[${Me.XTarget[${_count}].ID}].DisplayName}]}) /continue
  	UPDATEHUD "HEAL XTarget"
		/call check_no_cast_buffs
		AMIDEAD
		SIMPLEEXIT
		/doevents

		/if (!${Spawn[${Me.XTarget[${_count}].ID}].ID} || ${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} > ${${Spawn[id ${Me.XTarget[${_count}].ID}].Class.ShortName}Point}*.${tmpHealOverride}) /continue

		| Touch Heal
		/if (${Me.SpellReady[${healTouch}]}) {
			/if (${Spawn[${Me.XTarget[${_count}].ID}].Distance} <= ${Spell[${healTouch}].MyRange} && ${Spawn[${Me.XTarget[${_count}].ID}].PctHPs} <= ${${Spawn[id ${Me.XTarget[${_count}].ID}].Class.ShortName}Point}*.${tmpHealOverride}) {
				/if (${validate_cast[FALSE, spell, "${healTouch}", ${Me.XTarget[${_count}].ID}]}) /call core_cast2 "${healTouch}" 0 ${Me.XTarget[${_count}].ID} check_HP_${Me.Class.ShortName}
			}
		}				
		
		
	/next _count

	UPDATEHUD FALSE
/return



|***
 *	DES: Checks and Heals Pets. Group ONLY
 *	USE: /healpet
 *	INI: 
 *  NOTE: 
 ***|
sub check_pet_heal_PAL()
	DEBUGCLASS ${cbug}check_pet_heal_${Me.Class.ShortName()\ax
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
  DEBUGCLASS PetHeal: ${Group.Member[${_count}].Pet.ID}  ${pWorstHurtID}

	| touch heal
  /if (${Spawn[${pWorstHurtID}].PctHPs} < ${PetPoint} && ${Spawn[${pWorstHurtID}].Distance} <= ${Spell[${healTouch}].MyRange} && ${Me.SpellReady[${healTouch}]}) {
    /if (${validate_cast[FALSE, spell, "${healTouch}", ${pWorstHurtID}]}) /call core_cast2 "${healTouch}" 0 ${pWorstHurtID} FALSE
  } 
/return



|***
 *	DES: Fast check to see if anyone is hurt worse then person currently being healed.
 *	USE: added as a subcheck for core_cast
 *	INI: 
 *  NOTE: 
 ***|
sub check_HP_PAL()
	DEBUGCLASS ${cbug}check_HP_${Me.Class.ShortName}()\ax
	AMIDEAD
	/if (!${Me.Casting.ID}) /return
	/declare _count int local 0

	/varset WorstHurt 				${Me.ID}
	/varset WorstHP 					${If[${HealOverride},${Float[${HealOverride}].Centi},99.99]}
	/varset WorstClass 				${Me.Class.ShortName}
	/varset MajorHurt					0

	/if (${Bool[${Group.Members}]}) {
		/for _count 0 to ${Group.Members}
	  	/if (${Bool[${RampageTank}]} && ${RampageTank.Equal[${Group.Member[${_count}].DisplayName}]}) /continue
	  	/if (${heallist.Find[${Group.Member[${_count}].DisplayName}]}) /continue
			AMIDEAD
			SIMPLEEXIT
			/if (${Group.Member[${_count}].Type.NotEqual[Corpse]} && ${Group.Member[${_count}].ID}) {
				/if (${Group.Member[${_count}].Distance}<=${Spell[${healRemedy2}].MyRange} && ${Group.Member[${_count}].PctHPs} <= ${WorstHP} || !${WorstHurt}) {
				/varset WorstHurt ${Group.Member[${_count}].ID} 
				/varset WorstHP ${Group.Member[${_count}].PctHPs} 
				/varset WorstClass ${Group.Member[${_count}].Class.ShortName} 
			}
			/if (${Group.Member[${_count}].PctHPs} < ${${Group.Member[${_count}].Class.ShortName}Point}*.${HealOverride}*.5) /varcalc MajorHurt ${MajorHurt}+1
		}
		/next _count
	}

	/if (!${WorstHurt}) /return

	| touch heal
	/if (${Spawn[${WorstHurt}].PctHPs} < 20*.${HealOverride} && ${Spawn[${WorstHurt}].Distance}<=${Spell[${healPanic}].MyRange} && ${Cast.Timing}/1000<${Spell[${healTouch}].MyCastTime} && ${Me.SpellReady[${healTouch}]}) {
		/if (${Me.Casting.ID}) /stopcast
		/if (${validate_cast[FALSE, spell, "${healTouch}", ${WorstHurt}]}) /call core_cast2 "${healTouch}" 0 ${WorstHurt}
	} 
/return




|***
 *	DES: Mar's Gift 30ish k HP/15k manna 
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub cast_AA_marrs_gift()
	DEBUGCLASS ${cbug}cast_AA_marrs_gift()\aw
	AMIDEAD
	CHECKEXIT
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
	DEBUGCAST ${cbug}check_steel()\ax
	/if (${SafeZone} && !${switchOverrideSafezone}) /return
	AMIDEAD
	CHECKEXIT

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
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
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
		} else /if (${validate_cast[FALSE, alt, "Shield Flash", ${Me.ID}]}) {
			/call core_cast2 "Shield Flash" alt 0 FALSE
			/return
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
	DEBUGCLASS ${cbug}Bind_setPAL(type:${_classType}, verbage:${_verbage})\ax
	/if (!${validate_class[FALSE, |PAL]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _type string	local |ae|defcount|groupheal|harmonious|healpoint|mg|rampage|spiregroup|spireraid|stance|steel|stun|xtradius|xttime|yaulp

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

	| use group Heals
	} else /if (${Bool[${_classType.Equal[groupheal]}]}) {
		/call writeINI useGroupHeal Heal ${If[${useGroupHeal},FALSE,TRUE]} 	

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

		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return
