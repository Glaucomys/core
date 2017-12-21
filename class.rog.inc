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
	
	/if (${Bool[${RangeItemSummon}]}) /call check_ammo
	/if (!${timer_click_poison}) /call click_poison
	/if (${coreAuto} && ${switchSos}) /call automaticSoS


	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /return

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/if (${Me.Invis}) /call sneak_attack
		/call stick_to_target
		/call use_skill_melee ${Target2Kill}
		/call send_pet
		/call stick_to_target
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/call use_skill_melee ${Target2Kill}
		/call click_item
		/call click_DPS_${Me.Class.ShortName}
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
		/call set_combat RANGE
		/call send_pet
		CHECKTIE
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/call click_item
		/call click_DPS_${Me.Class.ShortName}
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
	}
	/if (${coreAuto}) /call cast_disc_breather
	/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} && !${is_target_dead[${Target2Kill}]}) /goto :killtarget
	/if (!${coreAuto} && ${Target.ID} && ${Bool[${combat}]} && !${is_target_dead[${Target.ID}]}) /goto :killtarget
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
	
	/if (${Target.ID} && ${Me.Invis}) {
		/call core_cast "${DiscSneakAttack}" disc ${Target2Kill} FALSE
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
	/if (${Target.ID} && ${Me.CombatAbilityReady[${DiscAssault}]} && ${Bool[${DiscAssault}]}) /call core_cast "${DiscAssault}" disc ${Target2Kill} FALSE
	/if (${Target.ID} && ${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	/if (${Target.ID} && ${switchdeBuffSnare}) /call cast_aa_snare ${Target2Kill}
	/call core_cast "Shadow's Flanking" alt 0 FALSE
	/call use_skill_melee ${Target2Kill}
	
	| /if (${Target.ID} && ${Me.CombatAbilityReady[${DiscVision}]} && ${Bool[${DiscVision}]} && !${Bool[${Me.Song[${DiscVision}]}]}) /call core_cast "${DiscVision}" disc 0 FALSE
	/if (${validate_buff[disc, discs, DiscVision]}) /call core_cast "${DiscVision}" disc 0 FALSE

	/if (${Target.ID} && ${Me.AltAbilityReady[${TwistedShank}]}) /call core_cast "${TwistedShank}" alt ${Target2Kill} FALSE
	/if (${Target.ID} && ${Me.AltAbilityReady[${AbsorbingAgent}]}) /call core_cast "${AbsorbingAgent}" alt ${Target2Kill} FALSE
	/if (${Target.ID} && ${Me.CombatAbilityReady[${DiscShadowHunter}]}) /call core_cast "${DiscShadowHunter}" disc ${Target2Kill} FALSE
	/if (${Target.ID} && ${Me.CombatAbilityReady[${DiscDisassociative}]}) /call core_cast "${DiscDisassociative}" disc ${Target2Kill} FALSE
	
	/if (${Target.ID} && ${Me.CombatAbilityReady[${DiscBleed}]} && ${Bool[${DiscBleed}]}) /call core_cast "${DiscBleed}" disc ${Target2Kill} FALSE
	| /if (${Target.ID} && ${Me.CombatAbilityReady[${DiscPlay}]} && ${Bool[${DiscPlay}]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscPlay}" disc 0 FALSE

	/if (${Me.CombatAbilityReady[${DiscPlay}]}) {
		/if (${NameList.Find[${Target.DisplayName}]} && ${Me.CombatAbilityReady[${DiscFrenzy}]}) /goto :skipDiscPlay
		/if (${validate_buff[disc, discs, DiscPlay]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscPlay}" disc 0 FALSE
		:skipDiscPlay
	}

	/if (!${NameList.Find[${Target.DisplayName}]} && !${Me.CombatAbilityReady[${DiscFrenzy}]}) {
		/if (${validate_buff[disc, discs, DiscPlay]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscPlay}" disc 0 FALSE
	}	
	
	/if (${Target.ID} && ${Me.CombatAbilityReady[${DiscPhantom}]} && ${Bool[${DiscPhantom}]}) /call core_cast "${DiscPhantom}" disc ${Target2Kill} FALSE
	| /if (${Target.ID} && ${Me.CombatAbilityReady[${DiscHiddenBlade}]} && ${Bool[${DiscHiddenBlade}]}) /call core_cast "${DiscHiddenBlade}" disc 0 FALSE
	/if (${validate_buff[disc, discs, DiscHiddenBlade]}) /call core_cast "${DiscHiddenBlade}" disc 0 FALSE

	/call use_skill_melee ${Target2Kill}
	/if (${Target.ID} && ${Me.CombatAbilityReady[${DiscHack}]} && ${Bool[${DiscHack}]}) /call core_cast "${DiscHack}" disc ${Target2Kill} FALSE
	| /if (${Target.ID} && ${Me.CombatAbilityReady[${DiscBladePoison}]} && ${Bool[${DiscBladePoison}]}) /call core_cast "${DiscBladePoison}" disc 0 FALSE
	/if (${validate_buff[disc, discs, DiscBladePoison]}) /call core_cast "${DiscBladePoison}" disc 0 FALSE

	| /if (${Target.ID} && ${Bool[${BladeStorm}]}) /call core_cast "${BladeStorm}" disc ${Target2Kill} FALSE
	/if (${Target.ID} && ${Me.CombatAbilityReady[${DiscAggro}]} && ${Bool[${DiscAggro}]}) /call core_cast "${DiscAggro}" disc ${Target2Kill} FALSE
	/call check_deBuffs ${Target2Kill}
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
	/call use_skill_melee ${Target2Kill}
	/call click_horn_of_unity 	

	| epic item
	/if (${Me.ItemReady[${Epic}]}) /call core_cast "${Epic}" item 0 FALSE	
	
/return



|***
 *	DES: emergency healing
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_ROG()
	DEBUGCLASS ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	CHECKEXIT
	/if (${Me.PctHPs} > 81*.${tmpHealOverride}) /goto :exit
	/if (${Me.PctHPs} <= 55*.${tmpHealOverride} && ${Me.AltAbilityReady[Tumble]}) /call core_cast "Tumble" alt 0 FALSE
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0 FALSE
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0 FALSE
/return



|***
 *	DES: SoS switch
 *	USE: auto
 *	INI: 
 *  NOTE: Work in progress
 ***|
sub automaticSoS()
	DEBUGCLASS ${cbug}automaticSoS()\ax
	AMIDEAD
	CHECKTIE

	/if (!${switchSos}) /return
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
		/call core_cast "${clickPoisonDispenser}" item ${Me.ID} FALSE
		/call sort_cursor TRUE
	}
	
	| check the buffs
	/for _count 1 to ${clickPoisonBuff.Count[|]}
		AMIDEAD
		/doevents
		/if (${Bool[${Me.Buff[${clickPoisonBuff.Arg[${_count},|]}]}]}) /varset _found TRUE
	/next _count		

	/if (!${_found} && ${Me.ItemReady[${clickPoisonItem}]}) /call core_cast "${clickPoisonItem}" item ${Me.ID} FALSE

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

	/if (${Target.ID} && ${Me.AltAbilityReady[Dirty Fighting]}) /call core_cast "Dirty Fighting" alt ${Target2Kill} FALSE
	/call core_cast "Dichotomic Weapons" disc 0 FALSE
	/call use_skill_melee ${Target2Kill}
	
	/if (!${AEMelee}) {
		/if (${Me.AltAbilityReady[Focused Rake's Rampage]}) /call core_cast "Focused Rake's Rampage" alt 0 FALSE
	} else /if (${AEMelee}) {
		/if (${Me.AltAbilityReady[Rake's Rampage]}) /call core_cast "Rake's Rampage" alt 0 FALSE
	}

	/call core_cast "${DiscPinPoint}" disc ${Target2Kill} FALSE
	/call core_cast "Envenomed Blades" alt 0 FALSE
	/if (${Bool[${Target2Kill}]} && ${Me.CombatAbilityReady[${DiscAggro}]} && ${Bool[${DiscAggro}]}) /call core_cast "${DiscAggro}" disc ${Target2Kill} FALSE
	/call core_cast "Rogue's Fury" alt 0 FALSE
	/if ( ${Me.PctEndurance} > 15) /call core_cast "${DiscMark}" disc ${Target2Kill} FALSE
	/call cast_AA_spire
	/if (${Raid.Members}) /call core_cast "Intensity of the Resolute" alt 0 FALSE
  /if (!${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscFrenzy}" disc 0 FALSE
  /delay 1
  /if (!${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscTwistedChance}" disc 0 FALSE
  /if (!${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscEradicator}" disc 0 FALSE
  /if (!${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscRazor}" disc 0 FALSE

  /if (!${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscAspBleeder}" disc 0 FALSE

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
sub Bind_setROG(string _classType, string _classVerbage)
	DEBUGCLASS ${cbug}Bind_setROG(type:${_classType}, verbage:${_classVerbage})\ax
	/if (!${validate_class[TRUE, |ROG]}) /return

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
	
	| set poison
	/if (${Bool[${_classType.Equal[poison]}]}) {
		/declare _num int local ${_classVerbage}
		
		| set max num of poison to keep
		/if (${Range.Between[1,100:${Int[${_num}]}]}) {
			/call writeINI clickPoisonCount Poison ${_num}
			VOUT Rogue Poison Keep ${sep} ${cinfo}${_num}\ax
		} else /if (!${Bool[${_num}]} || !${Range.Between[1,100:${Int[${_num}]}]}) {
			VOUT /${_bind} poison [${cinfo}${clickPoisonCount}\ax] of poison to keep ${cinfo}1 - 100\ax
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

	| auto SOS... or not...
	} else /if (${Bool[${_classType.Equal[autosos]}]}) {
		/call writeINI switchSos Common ${If[${switchSos},FALSE,TRUE]}
		VOUT /${_bind} ${cinfo}autosos\ax ${sep} ${If[${switchSos},${oenabled},${odisabled}]}

	| AE or Focused Melee
	} else /if (${Bool[${_classType.Equal[ae]}]}) {
		/call writeINI AEMelee Discs ${If[${AEMelee},FALSE,TRUE]}
		VOUT /${_bind} ${cinfo}AE Melee\ax ${sep} ${If[${AEMelee},${oenabled},${odisabled}]}

	} else {
		/for _count 1 to ${${_bind}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
	} 
/return



