|***
	class.rog.inc
	Core Rogue Include

***|
 


|***
 *	DES: Main Rogue Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_ROG()
	DEBUGCLASS Loop start: ${cbug}core_ROG()\ax
	/doevents
	AMIDEAD
	CHECKTIE

	/if (${coreAuto}) {
		/if (!${timer_click_poison}) /call click_poison
		/if (${switchSos}) /call automaticSoS
	}
	
	:killtarget
	CHECKREZ
	/call prep_combat
	/if (${Macro.Return} && ${Bool[${combat}]}) {
		DEBUG combat:${cinfo}${combat.Upper}\ax		

		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${switchHealCure}) /call cast_cure

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${switchPcSwarm} || ${switchPcPet}) /call send_pet
			/if (${switchCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/if (${switchAlliance}) /call trigger_alliance ${Target2Kill}
			/call check_deBuffs ${Target2Kill}
			/call click_BP
			/call click_item
			/if (${switchPcSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
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
 *	DES: execute the target!
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub sneak_attack()
	DEBUG ${cbug}sneak_attack\ax
	/if (${Bool[${DiscSneakAttack}]} && !${Me.CombatAbilityReady[${DiscSneakAttack}]}) /return
	CHECKEXIT
	/if (${is_target_dead[${Target2Kill}]}) /return
	
	/if (${Target.ID} && ${Me.Invis} && ${Me.Sneaking}) {
		/if (${validate_cast[FALSE, disc, "${DiscSneakAttack}", ${Target2Kill}]}) /call core_cast2 "${DiscSneakAttack}" disc ${Target2Kill} FALSE
		/doability backstab
	}
/return



|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: Disc
 *	INI: 
 *  NOTE: 
 ***|
sub disc_ROG()
	DEBUGCLASS	${cbug}disc_${Me.Class.ShortName}()\ax
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${validate_cast[FALSE, disc, "${DiscAssault}", ${Target2Kill}]}) /call core_cast2 "${DiscAssault}" disc ${Target2Kill} FALSE
	
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	/if (${validate_cast[FALSE, alt, "Shadow's Flanking", ${Me.ID}]}) /call core_cast2 "Shadow's Flanking" alt 0 FALSE
	/if (${validate_cast[,FALSE disc, "${DiscVision}", ${Me.ID}]}) /call core_cast2 "${DiscVision}" disc 0 FALSE
	/if (${validate_cast[FALSE, alt, "${TwistedShank}", ${Target2Kill}]}) /call core_cast2 "${TwistedShank}" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "${AbsorbingAgent}", ${Target2Kill}]}) /call core_cast2 "${AbsorbingAgent}" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscShadowHunter}", ${Target2Kill}]}) /call core_cast2 "${DiscShadowHunter}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscDisassociative}", ${Target2Kill}]}) /call core_cast2 "${DiscDisassociative}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscBleed}", ${Target2Kill}]}) /call core_cast2 "${DiscBleed}" disc ${Target2Kill} FALSE

	|** hold for now..

	| /if (${Target.ID} && ${Me.CombatAbilityReady[${DiscPlay}]} && ${Bool[${DiscPlay}]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscPlay}" disc 0 FALSE

	/if (${Me.CombatAbilityReady[${DiscPlay}]}) {
		/if (${NameList.Find[${Target.DisplayName}]} && ${Me.CombatAbilityReady[${DiscFrenzy}]}) /goto :skipDiscPlay
		/if (${validate_buff[disc, discs, DiscPlay]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscPlay}" disc 0 FALSE
		:skipDiscPlay
	}

	/if (!${NameList.Find[${Target.DisplayName}]} && !${Me.CombatAbilityReady[${DiscFrenzy}]}) {
		/if (${validate_buff[disc, discs, DiscPlay]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscPlay}" disc 0 FALSE
	}	
	**|
	
	
	/if (${Me.CombatAbilityReady[${DiscPlay}]}) {
		/if (!${NameList.Find[${Spawn[${Target2Kill}].DisplayName}]} && !${Me.CombatAbilityReady[${DiscFrenzy}]}) {
			/if (${validate_cast[FALSE, disc, "${DiscPlay}", ${Me.ID}]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast2 "${DiscPlay}" disc 0 FALSE
		}
	}

	| /if (!${NameList.Find[${Target.DisplayName}]} && !${Me.CombatAbilityReady[${DiscFrenzy}]}) {
	| 	/if (${validate_buff[disc, discs, DiscPlay]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscPlay}" disc 0 FALSE
	| }	
	
	
	
	
	
	/if (${Target.ID} && ${Me.CombatAbilityReady[${DiscPhantom}]} && ${Bool[${DiscPhantom}]}) /call core_cast2 "${DiscPhantom}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscHiddenBlade}", ${Me.ID}]}) /call core_cast2 "${DiscHiddenBlade}" disc 0 FALSE
	/if (${validate_cast[FALSE, disc, "${DiscHack}", ${Target2Kill}]}) /call core_cast2 "${DiscHack}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscBladePoison}", ${Me.ID}]}) /call core_cast2 "${DiscBladePoison}" disc 0 FALSE
	/if (${validate_cast[FALSE, disc, "${DiscAggro}", ${Target2Kill}]}) /call core_cast2 "${DiscAggro}" disc ${Target2Kill} FALSE
	/if (${validate_buff[FALSE, disc, "${DiscBlinding}", ${Target2Kill}]} && ${Target.Level} <= 110) /call core_cast2 "${DiscBlinding}" disc ${Target2Kill} FALSE
	
/return




|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_ROG()
	DEBUGCLASS ${cbug}click_DPS_items_${Me.Class.ShortName}()\ax 
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return
	/call click_horn_of_unity 	
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE
/return



|***
 *	DES: emergency healing
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_ROG()
	DEBUGCLASS ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	/if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.${tmpHealOverride} && ${Me.AltAbilityReady[Tumble]} && ${validate_cast[FALSE, alt, "Tumble", ${Me.ID}]}) /call core_cast2 "Tumble" alt 0 FALSE
	/call click_item_heal
/return


|***
 *	DES: SoS switch
 *	USE: auto
 *	INI: 
 *  NOTE: Work in progress
 ***|
sub automaticSoS()
	DEBUGCLASS ${cbug}automaticSoS()\ax
	/if (!${switchSos}) /return
	AMIDEAD
	CHECKTIE

	/if (${Me.Combat} || ${Me.Stunned} || ${Me.State.Equal[FEIGN]}) /return
	/if (${Me.Casting.ID} || ${Me.State.Equal[BIND]} || ${Window[TradeWnd].Open} || ${Window[MerchantWnd].Open} || ${Window[BigBankWnd].Open}) /return
	
	/declare _sos bool local FALSE

	/if (${Me.Invis} && ${Me.Sneaking} && !${Me.AbilityReady[hide]} && !${Me.AbilityReady[sneak]}) {
		/return
	}

	/if (!${Me.Invis} && ${Me.AbilityReady[hide]} && ${Me.Sneaking} && !${Me.AbilityReady[sneak]} ) {
		/if (${Me.AbilityReady[hide]}) /doability hide
		/return	
	} else /if (!${Me.Invis} && ${Me.Sneaking} && !${Me.AbilityReady[sneak]}) {
		/if (${Me.AbilityReady[sneak]}) /doability sneak
		/return
	}
	
	/if (!${_sos} && ${Me.AbilityReady[hide]} && ${Me.AbilityReady[sneak]}) {
		/doability hide
		/delay 1
		/doability sneak
	}
/return



|***
 *	DES: keep poisons running
 *	USE: /call click_poison OR /poison
 *	INI: 
 *  NOTE: 
 ***|
sub click_poison()
	DEBUGCLASS ${cbug}click_poison(${FindItemCount[${clickPoisonItem}]} <= ${clickPoisonCount})\ax
	CHECKEXIT
	CHECKTIE
	/declare _count int local 0
	/declare _found bool local FALSE
	
	| keep a stock on hand
	/if (${FindItemCount[${clickPoisonItem}]} < ${clickPoisonCount} && ${Me.ItemReady[${clickPoisonDispenser}]}) {
		DEBUGCLASS click ${clickPoisonDispenser}
		/if (${validate_cast[FALSE, item, "${clickPoisonDispenser}", ${Me.ID}]}) /call core_cast2 "${clickPoisonDispenser}" item ${Me.ID} FALSE
		/call sort_cursor TRUE
	}
	
	| check the buffs
	/for _count 1 to ${clickPoisonBuff.Count[|]}
		AMIDEAD
		/doevents
		/if (${Bool[${Me.Buff[${clickPoisonBuff.Arg[${_count},|]}]}]}) /varset _found TRUE
	/next _count		
	/if (!${_found} && ${validate_cast[FALSE, item, "${clickPoisonItem}", ${Me.ID}]}) /call core_cast2 "${clickPoisonItem}" item ${Me.ID} FALSE
	/call create_timer timer_click_poison ${tClick_Poison}
/return



 |***
 *	DES: Rogue burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_ROG()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	CHECKEXIT
	/doevents

	/if (${Me.ActiveDisc.Name.Equal[${DiscPlay}]} && ${Me.CombatAbilityReady[${DiscFrenzy}]}) {
		:loopmakestop
		/delay 5
		/if (${Me.ActiveDisc.Name.Equal[${DiscPlay}]} && ${Me.CombatAbilityReady[${DiscFrenzy}]}) /stopdisc
		/delay 5
		/if (${Me.ActiveDisc.Name.Equal[${DiscPlay}]} && ${Me.CombatAbilityReady[${DiscFrenzy}]}) /goto :loopmakestop
	}

	| envenomed blades / dirty fighting stack issue
	/if (${Target.ID} && ${Me.AltAbilityReady[Dirty Fighting]} && !${Me.Buff[Envenomed Blades].ID}) {
		/if (${validate_cast[FALSE, alt, "Dirty Fighting", ${Target2Kill}]}) /call core_cast2 "Dirty Fighting" alt ${Target2Kill} FALSE
	} else /if (${Me.AltAbilityReady[Envenomed Blades]} && !${Me.Buff[Dirty Fighting].ID}) {
		/if (${validate_cast[FALSE, alt, "Envenomed Blades", ${Me.ID}]}) /call core_cast2 "Envenomed Blades" alt 0 FALSE
	}

	/if (${validate_cast[FALSE, disc, "Dichotomic Weapons", ${Me.ID}]}) /call core_cast2 "Dichotomic Weapons" disc 0 FALSE
	
	/if (!${AEMelee}) {
		/if (${validate_cast[FALSE, alt, "Focused Rake's Rampage", ${Me.ID}]}) /call core_cast2 "Focused Rake's Rampage" alt 0 FALSE
	} else /if (${AEMelee}) {
		/if (${validate_cast[FALSE, alt, "Rake's Rampage", ${Me.ID}]}) /call core_cast2 "Rake's Rampage" alt 0 FALSE
	}

	/if (${validate_cast[FALSE, disc, "${DiscPinPoint}", ${Target2Kill}]}) /call core_cast2 "${DiscPinPoint}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscAggro}", ${Target2Kill}]}) /call core_cast2 "${DiscAggro}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "Rogue's Fury", ${Me.ID}]}) /call core_cast2 "Rogue's Fury" alt 0 FALSE
	/if (${validate_cast[FALSE, disc, "${DiscMark}", ${Target2Kill}]} && ${Me.PctEndurance} > 15) /call core_cast2 "${DiscMark}" disc ${Target2Kill} FALSE
	/call cast_AA_spire
	/if (${validate_cast[FALSE, alt, "Intensity of the Resolute", ${Me.ID}]} && ${Raid.Members}) /call core_cast2 "Intensity of the Resolute" alt 0 FALSE
  /if (${validate_cast[FALSE, disc, "${DiscFrenzy}", ${Me.ID}]} && !${Me.ActiveDisc.ID}) {
  	/call core_cast2 "${DiscFrenzy}" disc 0 FALSE
  	/delay 2
  }
  /if (${validate_cast[FALSE, disc, "${DiscTwistedChance}", ${Me.ID}]} && !${Me.ActiveDisc.ID}) /call core_cast2 "${DiscTwistedChance}" disc 0 FALSE
  /if (${validate_cast[FALSE, disc, "${DiscEradicator}", ${Me.ID}]} && !${Me.ActiveDisc.ID}) /call core_cast2 "${DiscEradicator}" disc 0 FALSE
  /if (${validate_cast[FALSE, disc, "${DiscRazor}", ${Me.ID}]} && !${Me.ActiveDisc.ID}) /call core_cast2 "${DiscRazor}" disc 0 FALSE

  /if (${validate_cast[FALSE, disc, "${DiscAspBleeder}", ${Me.ID}]}) && !${Me.ActiveDisc.ID}) /call core_cast2 "${DiscAspBleeder}" disc 0 FALSE

	/call synergy_click_Circle
	/call click_item_burn
/return



|***
 *	DES: Class controls
 *	USE: /rog []
 *	INI: 
 *  NOTE:
 ***|
#bind setROG /rog
sub Bind_setROG(string _classType, string _verbage)
	DEBUGCLASS ${cbug}Bind_setROG(type:${_classType}, verbage:${_verbage})\ax
	/if (!${validate_class[FALSE, |ROG]}) /return

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
	
	| set poison
	/if (${Bool[${_classType.Equal[poison]}]}) {
		
		| set max num of poison to keep
		/if (${Range.Between[1,100:${Int[${_verbage}]}]}) {
			/call writeINI clickPoisonCount Poison ${_num}
			VOUT Rogue Poison Keep ${sep} ${cinfo}${_num}\ax
			/return
		} else /if (!${Bool[${_verbage}]} || !${Range.Between[1,100:${Int[${_verbage}]}]}) {
			VOUT /${_bind} poison [${cinfo}${clickPoisonCount}\ax] of poison to keep ${cinfo}1 - 100\ax
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
		/return

	| auto SOS... or not...
	} else /if (${Bool[${_classType.Equal[autosos]}]}) {
		/call writeINI switchSos Common ${If[${switchSos},FALSE,TRUE]}
		VOUT /${_bind} ${cinfo}autosos\ax ${sep} ${If[${switchSos},${oenabled},${odisabled}]}
		/return

	| AE or Focused Melee
	} else /if (${Bool[${_classType.Equal[ae]}]}) {
		/call writeINI AEMelee Discs ${If[${AEMelee},FALSE,TRUE]}
		VOUT /${_bind} ${cinfo}AE Melee\ax ${sep} ${If[${AEMelee},${oenabled},${odisabled}]}
		/return

	| healpoint
	} else /if (${Bool[${_classType.Find[healpoint]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI ${Me.Class.ShortName}Point Heal ${_verbage}
		} else /if (!${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} healpoint [${cinfo}${${Me.Class.ShortName}Point}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}

	}
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
		/if (${${_bind}Type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}${${Me.Class.ShortName}Point}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[poison]}) /varset _listout ${_listout}(${cnum}${clickPoisonCount}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return
