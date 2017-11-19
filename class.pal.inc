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

	/if (${useMG} && !${flagCallNeedRez} && !${timer_check_MG} && (!${SafeZone} && !${switchOverrideSafezone})) /call cast_AA_marrs_gift
	
	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /return

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/call stick_to_target
		/call use_skill_melee ${Target2Kill}
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/if (${switchHealGroup}) /call check_splash_group_${Me.Class.ShortName}
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke} && ${switchAgro}) /call cast_nuke
		/call use_skill_melee ${Target2Kill}
		/call stick_to_target
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}
		/if (${switchHarmonious}) /call check_harmonious
		/if (${switchSteel} && !${timer_check_Steel}) /call check_steel	
		/if (${switchStance} && (!${timer_check_stance} || !${Bool[${Me.Buff[${Stance}]}]})) /call check_stance
		/if (${useMG} && !${flagCallNeedRez} && !${timer_check_MG}) /call cast_AA_marrs_gift
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},HEAL]}) {
		/call set_combat HEAL
		/if (${switchHealGroup}) /call check_splash_group_${Me.Class.ShortName}
	} else /if (${Select[${combat},DUAL]}) {
	} 
	/if (${coreAuto}) {	
		/call use_mod_rod
		/if (${switchHealCure}) /call cast_cure
	}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} && !${is_target_dead[${Target2Kill}]}) /goto :killtarget
	/if (!${coreAuto} && ${Target.ID} && ${Bool[${combat}]} && !${is_target_dead[${Target.ID}]}) /goto :killtarget
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
	
	/if (${is_target_dead[${Target2Kill}]}) /return
	
	| check challenge
	/call cast_challenge ${Target2Kill}
	
	/if (${switchAgro}) /call get_agro_${Me.Class.ShortName}
	
	/if (${validate_buff[disc, discs, DiscFastRune]}) /call core_cast "${DiscFastRune}" disc 0 FALSE
	
	
	| Hand of Piety AA Group heal	
	/if (${switchHealSelf} && ${Me.AltAbilityReady[Hand of Piety]} && ${Me.PctHPs} < 50*.${tmpHealOverride} && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${combatRadius} targetable]} > 2 ) /call core_cast "Hand of Piety" alt 0 FALSE
	| dpsheal
	/if (${switchHealSelf} && ${Me.CombatAbilityReady[${DiscReflexive}]} && ${Me.PctHPs} < 40*.${tmpHealOverride}) /call core_cast "${DiscReflexive}" disc ${Target2Kill} FALSE

	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}

	| AA inquisitors armor stacking control
	/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 30 targetable]} > 2) {
		/if (${Me.AltAbilityReady[${AAInquisitor}]} && !${Me.Song[[${AAGroupInquisitor}].ID}) { 
			/call core_cast "${AAInquisitor}" alt 0 FALSE
		} else /if (${Me.AltAbilityReady[${AAGroupInquisitor}]} && !${Me.Song[${AAInquisitor}].ID} && !${Raid.Members}) {
			/call core_cast "${AAGroupInquisitor}" alt 0 FALSE
		}
	}

	| undead cripple
	/if (${Target.Body.Name.Equal[Undead]}) {
		/call core_cast "Vanquish the Fallen" alt ${Target2Kill} FALSE
	}
	
	| Yaulp.. all the time!!
	/if (!${Me.Sitting} && ${doYaulp}) /call cast_yaulp

	/if (${switchHealSelf} && ${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0 FALSE
	/if (${switchHealSelf} && ${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0 FALSE
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
		/if (!${Bool[${Raid.Members}]} && ${validate_cast[alt, ${AAAELodestar}, ${Target2Kill}]}) /call core_cast "${AAAELodestar}" alt ${Target2Kill} cast_break
	} else /if (${mobAECount} > 3) {
		/if (!${Bool[${Raid.Members}]} && ${validate_cast[alt, ${AAAELodestar}, ${AAAEBeacon}]}) /call core_cast "${AAAEBeacon}" alt ${Target2Kill} cast_break
	}

	| chain stun
	| /if (${Bool[${chainStun}]} && ${Me.AltAbilityReady[${chainStun}]} && ${Me.CurrentMana} > ${Spell[${chainStun}].Mana}) /call core_cast "${chainStun}" alt ${Target2Kill} cast_break
	
	/if (${validate_cast[alt, ${chainStun}, ${Target2Kill}]}) /call core_cast "${chainStun}" alt ${Target2Kill} cast_break

	| AA pet hate (the little sword thingie)
	/if (${Me.AltAbilityReady[Projection of Piety]} && (${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]} || ${Me.PctAggro} < 80)) /call core_cast "Projection of Piety" alt ${Target2Kill} cast_break

	| fake hate
	/if (${Me.AltAbilityReady[Ageless Enmity]} && (${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]} || ${Me.PctAggro} < 60)) /call core_cast "Ageless Enmity" alt ${Target2Kill} cast_break

	| named fake hate
	/if (${Me.CombatAbilityReady[${DiscUnflinching}]} && (${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]} || ${Me.PctAggro} < 80) && ${NameList.Find[${Target.DisplayName}]}  && ${Me.CurrentEndurance} > ${Spell[${DiscUnflinching}].EnduranceCost}) /disc ${DiscUnflinching}

	| Valiant Defense. agro+heal
	/if (${Me.SpellReady[${ValiantDefense}]} && (${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]} || ${Me.PctAggro} < 70)) /call core_cast "${ValiantDefense}" 0 ${Target2Kill} cast_break

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
  /if (${Spawn[${WorstHurt}].PctHPs} <= 45*.${tmpHealOverride} && ${Spawn[${WorstHurt}].Distance} <= ${Spell[${splashPanic}].MyRange} && ${Me.SpellReady[${splashPanic}]}) {
	| /if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.8*.${tmpHealOverride} && ${Spawn[${WorstHurt}].Distance} <= ${Spell[${splashPanic}].MyRange} && ${Me.SpellReady[${splashPanic}]}) {
		/call core_cast "${splashPanic}" 0 FALSE
		/if (${castReturn.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
	}
	AMIDEAD
	CHECKEXIT
	| Group Splash
  /if (${useGroupHeal} && ${groupHurt} >= 2 && ${Me.SpellReady[${splashGroup}]}) {
    /call core_cast "${splashGroup}" 0 FALSE
    /if (${Macro.Return.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
  } 
		
	| AA Splash
  /if (${useGroupHeal} && ${groupHurt} >= 3) /if (${Me.AltAbilityReady[Hand of Piety]}) /call core_cast "Hand of Piety" alt 0 FALSE
	 
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
	/if (!${Me.AltAbilityReady[Marr's Gift]} || ${timer_zone_pause} || ${SafeZone} && !${switchOverrideSafezone}  || ${Me.Sitting}) /return
	/if (${Me.PctMana} < ${useMGPct}*.${tmpHealOverride} && ${Me.AltAbilityReady[Marr's Gift]}) /call core_cast "Marr's Gift" alt 0 FALSE
	/if (${switchHealSelf} && ${Me.PctHPs} < ${useMGPct}*.${tmpHealOverride} && ${Me.AltAbilityReady[Marr's Gift]}) /call core_cast "Marr's Gift" alt 0 FALSE
/return	



|***
 *	DES: keeps Paladin Reinvigorating Steel applied
 *	USE: /call check_steel
 *	INI: [buffs] - Steel
 *  NOTE: 
 ***|
sub check_steel()
	DEBUGCAST ${cbug}check_steel()\ax
	AMIDEAD
	CHECKEXIT

	/if (${SafeZone} && !${switchOverrideSafezone} || !${switchSteel}) /return
	/if (${validate_buff[spell, buffs, Steel]}) {
		/call core_cast "${Steel}" 0 0 FALSE
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
	/if (${Me.CombatAbilityReady[${DiscArmor}]} && ${Me.CurrentEndurance} > ${Spell[${DiscArmor}].EnduranceCost}) {
		/call core_cast "${DiscArmor}" disc 0 FALSE
	} else /if (!${Me.CombatAbilityReady[${DiscGuard}]} && ${Me.CurrentEndurance} > ${Spell[${DiscGuard}].EnduranceCost} && !${Bool[${Me.ActiveDisc}]}) {
		/call core_cast "${DiscGuard}" disc 0 FALSE
	} else /if (!${Me.CombatAbilityReady[${DiscHoly}]} && ${Me.CurrentEndurance} > ${Spell[${DiscHoly}].EnduranceCost} && !${Bool[${Me.ActiveDisc}]}) {
		/call core_cast "${DiscHoly}" disc 0 FALSE
	}
	
	/call cast_AA_spire	

	/if (${Me.AltAbilityReady[${AAJudgement}]}) /call core_cast "${AAJudgement}" alt 0 FALSE
	/if (${Me.AltAbilityReady[${AAValorousRage}]}) /call core_cast "${AAValorousRage}" alt 0 FALSE
	/if (${Me.AltAbilityReady[${AAShield}]}) /call core_cast "${AAShield}" alt 0 FALSE
	
	| epic 2.0
	/if (${Me.ItemReady[${Epic}]} && ${Spell[${FindItem[${Epic}].Spell}].Stacks} && !${Me.Buff[${FindItem[${Epic}].Spell}].ID} && !${Me.Song[${FindItem[${Epic}].Spell}].ID}) /call core_cast "${Epic}" item 0 FALSE
	
	
| aa Thunder of Karna | spell damage	
	
	
	/call use_glyph
	/call synergy_click_Circle
	/if (${doclickItemBurn}) /call click_item_burn
/return



|***
 *	DES: Class controls
 *	USE: /pal [] [##]
 *	INI: 
 *  NOTE:
 ***|
#bind setPAL /pal
sub Bind_setPAL(string _classType, string _classVerbage)
	DEBUGCLASS ${cbug}Bind_setPAL(type:${_classType}, verbage:${_classVerbage})\ax
	/if (!${validate_class[TRUE, |PAL]}) /return

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
	| to Yaulp or not		
	 } else /if (${Bool[${_classType.Equal[yaulp]}]}) {
		/if (${Bool[${_classVerbage}]} && ${Range.Between[1,99:${Int[${_classVerbage}]}]}) {
			/declare _num int local ${_classVerbage}
			/call writeINI useYaulpPct Buffs ${_num}
			VOUT /${_bind} Yaulp Ptc ${sep} ${cinfo}${_num}\ax
		} else /if (${Bool[${_classVerbage}]} && !${Range.Between[1,99:${Int[${_classVerbage}]}]}) {
			VOUT /${_bind} yaulp [${cinfo}${useYaulpPct}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
		} else {
		 	/call writeINI doYaulp Buffs ${If[${doYaulp},FALSE,TRUE]}  
		 	/call echos switch "Yaulp" doYaulp 0 0
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
		VOUT /${_bind} ${_classType.Lower} [${_listout}]	

	| AA Marr's Gift	Control	
	} else /if (${Bool[${_classType.Equal[mg]}]}) {
		/if (${Bool[${_classVerbage}]} && ${Range.Between[1,99:${_classVerbage}]}) {
			/call writeINI useMGPct Buffs ${_classVerbage} 
			/call echos number "Marr's Gift" ${_classVerbage}		
		} else /if (${Bool[${_classVerbage}]} && !${Range.Between[1,99:${_classVerbage}]}) {
			VOUT /${_bind} mg [${cinfo}##\ax] allowable range ${sep} ${cinfo}1 - 99\ax
		}	else /if (!${Bool[${_classVerbage}]}) {
			/call writeINI useMG Buffs ${If[${useMG},FALSE,TRUE]}  
			/call echos switch "Marr's Gift ${sep} ${useMGPct}%" useMG
		}	
		

	| use group Heals
	} else /if (${Bool[${_classType.Equal[groupheal]}]}) {
		/call writeINI useGroupHeal Heal ${If[${useGroupHeal},FALSE,TRUE]} 	
	 	/call echos switch "Use Group Heal" useGroupHeal

	| set Class Heal Point	
	} else /if (${Bool[${_classType.Equal[healpoint]}]}) {
		/if (${Bool[${_classVerbage}]}) /declare _num int local ${_classVerbage}
		/if (${Target.ID} && ${_num} && ${Spawn[${Target.ID}].Type.NotEqual[Pet]}) {
			/call writeINI ${Target.Class.ShortName}Point Heal ${_num}
			VOUT ${ctar}${Target.Class}\ax Heal Point ${sep} ${cinfo}${_classVerbage}\ax
		} else /if (${Target.ID} && ${_num} && ${Spawn[${Target.ID}].Type.Equal[Pet]}) {
			/call writeINI PetPoint Heal ${_num}
			VOUT ${ctar}Pet\ax Heal Point ${sep} ${cinfo}${_classVerbage}\ax
		} else {
			VOUT /${_bind} healpoint [${cinfo}##\ax] [${cinfo}TARGET\ax]
		}

	| on/off switch for Steel
	} else /if (${Bool[${_classType.Equal[steel]}]}) {
		/call writeINI switchSteel Buffs ${If[${switchSteel},FALSE,TRUE]}  
		/call echos switch "Buff ${sep} ${Steel}" switchSteel
	
	| on/off switch for Harmonious
	} else /if (${Bool[${_classType.Equal[harmonious]}]}) {
		/call writeINI switchHarmonious Buffs ${If[${switchHarmonious},FALSE,TRUE]}  
		/call echos switch "Buff ${sep} ${Harmonious}" switchHarmonious

	| on/off switch for Stance
	} else /if (${Bool[${_classType.Equal[stance]}]}) {
		/call writeINI switchStance Buffs ${If[${switchStance},FALSE,TRUE]}  
		/call echos switch "Buff ${sep} ${Stance}" switchStance
		
	| set XTar heal radius
	} else /if (${Bool[${_classType.Equal[xtradius]}]}) {
		/if (${Range.Between[1,300:${Int[${_classVerbage}]}]}) {
			/call writeINI XTHealRadius Heal ${_classVerbage}
			/call echos switch "XTarget Heal Radius ${sep}" XTHealRadius 0 0
		} else /if (!${Bool[${_classVerbage}]} || !${Range.Between[1,300:${Int[${_classVerbage}]}]}) {
			VOUT /${_bind} xtradius [${cinfo}${XTHealRadius}\ax] allowable range ${sep} ${cinfo}1 - 300\ax
		}	

	| set XT refresh list Time
	} else /if (${Bool[${_classType.Equal[xttime]}]}) {
		/if (${Range.Between[1,5:${Int[${_classVerbage}]}]}) {
			/call writeINI tXHeal Timer ${_classVerbage}
			/call echos number "XTarget refresh time" ${tXHeal} 0 0
		} else /if (!${Bool[${_classVerbage}]} || !${Range.Between[1,5:${Int[${_classVerbage}]}]}) {
			VOUT /${_bind} xttime [${cinfo}${tXHeal}m\ax] allowable range ${sep} ${cinfo}1 - 5 min\ax
		}	

	| Stun Control
	} else /if (${Bool[${_classType.Equal[stun]}]}) {
		
		/if (${Bool[${_classVerbage}]}) {
			/if (${Bool[${_classVerbage.Equal[stop]}]}) {
					/call writeINI chainStun Discs FALSE
			} else /if (${Bool[${_classVerbage.Find[divine]}]}) {
					/call writeINI chainStun Discs "Divine Stun"
			} else /if (${Bool[${_classVerbage.Find[force]}]}) {
					/call writeINI chainStun Discs "Force of Disruption"
			} else /if (${Bool[${_classVerbage.Find[disruptive]}]}) {
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
		

	} else {
		/for _count 1 to ${${_bind}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
	} 
/return
