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
	
	:killtarget
	/if (${useMG} && !${flagCallNeedRez} && !${timer_check_MG}) /call cast_AA_marrs_gift
	CHECKREZ
	/call prep_combat
	/if (${Macro.Return} && ${Bool[${combat}]}) {
		DEBUG combat:${cinfo}${combat.Upper}\ax		

		/if (${switchHealGroup}) /call check_splash_group_${Me.Class.ShortName}
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
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
			/if (${switchHarmonious}) /call check_harmonious
			/if (${switchSteel} && !${timer_check_Steel}) /call check_steel	
			/if (${switchStance} && (!${timer_check_Stance} || !${Bool[${Me.Buff[${Stance}]}]})) /call check_stance
		}

		/if (${coreAuto}) {	
			/if (${validate_class[FALSE, ${CastList}]}) /call use_mod_rod
			/if (${validate_class[FALSE, ${MeleeList}]}) /call cast_stamina_recovery
			/if (!${Spawn[${Target2Kill}].ID} && !${is_target_dead[${Target2Kill}]}) /goto :killtarget
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
	DEBUGCLASS	${cbug}disc_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return
	CHECKTIE

	| defensive	
	/if (${coreAuto} && ${switchAgro} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]} >= ${defCount}) {
		/declare _count int local 0
		/for _count 1 to 3
			/if (${Bool[${Me.ActiveDisc}]}) /break
			/if (${validate_cast[FALSE, disc, "${DefensiveDisc${_count}}", ${Me.ID}]}) {
				/call core_cast2 "${DefensiveDisc${_count}}" disc 0 FALSE
				/break
			}			
		/next _count
	}
	
	| check challenge
	/call cast_challenge ${Target2Kill}
	
	/if (${switchAgro}) /call get_agro_${Me.Class.ShortName}
	
	/if (${validate_cast[FALSE, disc, "${DiscFastRune}", ${Me.ID}]}) /call core_cast2 "${DiscFastRune}" disc 0 FALSE
	
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}

	| AA inquisitors armor stacking control
	/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 30 targetable]} > 2) {
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
	/if (!${Me.Sitting} && ${doYaulp}) /call cast_yaulp
	
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

	| Panic Heal 1
  /if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.50*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, spell, "${healPanic1}", ${Me.ID}]}) /call core_cast2 "${healPanic1}" 0 FALSE
	}
	
	| Hand of Piety AA Group heal	
	/if (${Me.PctHPs} < ${${Me.Class.ShortName}Point}*.50*.${tmpHealOverride} && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${combatRadius} targetable]} > 1 ) {
		/if (${validate_cast[FALSE, alt, "Hand of Piety", ${Me.ID}]}) /call core_cast2 "Hand of Piety" alt 0 FALSE
	}

	| dpsheal
	/if (${Me.PctHPs} < ${${Me.Class.ShortName}Point}*.40*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, disc, "${DiscReflexive}", ${Target2Kill}]}) /call core_cast2 "${DiscReflexive}" disc ${Target2Kill} FALSE
	}

	| Panic Heal 2
  /if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.20*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, spell, "${healPanic2}", ${Me.ID}]}) /call core_cast2 "${healPanic2}" 0 FALSE
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

	| AE Stuns
	/varset mobAECount ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 60 playerstate 4 targetable]}
	
	/if  (${mobAECount} > 1) {
		/if (!${Bool[${Raid.Members}]} || ${Bool[${Raid.Members}]} && ${Raid.MainAssist.Name.Equal[${Me.DisplayName}]}) {
			/if (${validate_cast[FALSE, alt, "${AAAELodestar}", ${Target2Kill}]}) /call core_cast2 "${AAAELodestar}" alt ${Target2Kill} FALSE
		}
	} else /if (${mobAECount} > 4) {
		/if (!${Bool[${Raid.Members}]} || ${Bool[${Raid.Members}]} && ${Raid.MainAssist.Name.Equal[${Me.DisplayName}]}) {
			/if (${validate_cast[FALSE, alt, "${AAAEBeacon}", ${Target2Kill}]}) /call core_cast2 "${AAAEBeacon}" alt ${Target2Kill} FALSE
		}
	}

	| chain stun
	/if (${validate_cast[FALSE, alt, "${chainStun}", ${Target2Kill}]}) /call core_cast2 "${chainStun}" alt ${Target2Kill} FALSE

	| AA pet hate (the little sword thingie)
	/if (${Me.PctAggro} < 80 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
		/if (${validate_cast[FALSE, alt, "Projection of Piety", ${Target2Kill}]}) /call core_cast2 "Projection of Piety" alt ${Target2Kill} cast_break
	}

	| snap hate
	/if ((${Me.PctAggro} < 60 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) && !${Raid.Members}) {
		/if (${validate_cast[FALSE, alt, "Ageless Enmity", ${Target2Kill}]}) /call core_cast2 "Ageless Enmity" alt ${Target2Kill} cast_break
	}

	| named fake hate
	/if (${Me.PctAggro} < 80 && ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]} && ${NameList.Find[${Target.DisplayName}]}) {
		/disc ${DiscUnflinching}
	}

	| Valiant Defense. agro+heal
	/if (${Me.PctAggro} < 95 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
		/if (${validate_cast[FALSE, spell, "${ValiantDefense}", ${Target2Kill}]}) /call core_cast2 "${ValiantDefense}" 0 ${Target2Kill} cast_break
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
	AMIDEAD
	CHECKEXIT
	/doevents
	/call click_horn_of_unity 	
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE

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
  /if (${Spawn[${WorstHurt}].PctHPs} <= 45*.${tmpHealOverride} && ${Spawn[${WorstHurt}].Distance} <= ${Spell[${splashPanic}].MyRange}) {
		/if (${validate_cast[FALSE, spell, "${splashPanic}", ${Me.ID}]}) /call core_cast2 "${splashPanic}" 0 FALSE
		/if (${castReturn.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
	}
	
	| group heals
	/if (${useGroupHeal}) {
		AMIDEAD
		CHECKEXIT
		| Group Splash
	  /if (${groupHurt} >= 1) {
	    /if (${validate_cast[FALSE, spell, "${splashGroup}", ${Me.ID}]}) /call core_cast2 "${splashGroup}" 0 FALSE
	    /if (${Macro.Return.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
	  } 
			
		| AA Splash
	  /if (${groupHurt} >= 3) {
	  	/if (${validate_cast[FALSE, alt, "Hand of Piety", ${Me.ID}]}) /call core_cast2 "Hand of Piety" alt 0 FALSE
	  }
	  
		AMIDEAD
	 	/doevents
		| Touch Heal 
	  /if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.${tmpHealOverride}) {
	  	/if (${validate_cast[FALSE, spell, "${healTouch}", ${WorstHurt}]}) /call core_cast2 "${healTouch}" 0 ${WorstHurt} FALSE
	  }
	}
	 
	UPDATEHUD
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

	| to Yaulp or not		
	/if (${Bool[${_classType.Equal[yaulp]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI useYaulpPct Buffs ${_verbage}
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} yaulp [${cinfo}${useYaulpPct}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		} else {
		 	/call writeINI doYaulp Buffs ${If[${doYaulp},FALSE,TRUE]}  
		 	/call echos switch "Yaulp" doYaulp 0 0
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

	| AA Marr's Gift	Control	
	} else /if (${Bool[${_classType.Equal[mg]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${_verbage}]}) {
			/call writeINI useMGPct Buffs ${_verbage} 
			/call echos number "Marr's Gift" ${_verbage}		
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${_verbage}]}) {
			VOUT /${_bind} mg [${cinfo}##\ax] allowable range ${sep} ${cinfo}1 - 99\ax
		}	else /if (!${Bool[${_verbage}]}) {
			/call writeINI useMG Buffs ${If[${useMG},FALSE,TRUE]}  
			/call echos switch "Marr's Gift(${cnum}${useMGPct}\ax)" useMG
			/return
		}	
		

	| use group Heals
	} else /if (${Bool[${_classType.Equal[groupheal]}]}) {
		/call writeINI useGroupHeal Heal ${If[${useGroupHeal},FALSE,TRUE]} 	
	 	/call echos switch "Use Group Heal" useGroupHeal
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
	
	| on/off switch for Steel
	} else /if (${Bool[${_classType.Equal[steel]}]}) {
		/call writeINI switchSteel Buffs ${If[${switchSteel},FALSE,TRUE]}  
		/call echos switch "Buff ${sep} ${Steel}" switchSteel
		/return
	
	| on/off switch for Harmonious
	} else /if (${Bool[${_classType.Equal[harmonious]}]}) {
		/call writeINI switchHarmonious Buffs ${If[${switchHarmonious},FALSE,TRUE]}  
		/call echos switch "Buff ${sep} ${Harmonious}" switchHarmonious
		/return

	| on/off switch for Stance
	} else /if (${Bool[${_classType.Equal[stance]}]}) {
		/call writeINI switchStance Buffs ${If[${switchStance},FALSE,TRUE]}  
		/call echos switch "Buff ${sep} ${Stance}" switchStance
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
		
	| defensive mob agro count
	} else /if (${Bool[${_classType.Equal[defcount]}]}) {
		/if (${Range.Between[1,50:${Int[${_verbage}]}]}) {
			/call writeINI defCount Discs ${_verbage}
		} else /if (!${Bool[${_verbage}]} || !${Range.Between[1,50:${Int[${_verbage}]}]}) {
			VOUT /${_bind} defcount [${cinfo}${countMobAgro}\ax] allowable range ${sep} ${cinfo}1 - 50 min\ax
			/return
		}	

	}
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
		/if (${${_bind}Type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[defcount]}) /varset _listout ${_listout}(${cnum}${defCount}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[mg]}) /varset _listout ${_listout}(${cnum}${useMGPct}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[xttime]}) /varset _listout ${_listout}(${cnum}${tXHeal}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[xtradius]}) /varset _listout ${_listout}(${cnum}${XTHealRadius}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[yaulp]}) /varset _listout ${_listout}(${cnum}${useYaulpPct}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]

/return



