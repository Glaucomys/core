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
	
	| /if (!${Me.Combat} && !${SpawnCount[npc xtarget]} && !${Me.Invis}) /call do_SoS TRUE
	/if (${Bool[${RangeItemSummon}]}) /call check_ammo
	/if (!${timer_click_poison}) /call click_poison
	

	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /return

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/call send_pet
		/call stick_to_target
		/call use_skill_melee
		/call stick_to_target
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/call use_skill_melee
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
		/call set_combat RANGE
		/call send_pet
		CHECKTIE
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
	}
	/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
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
sub disc_ROG()
	DEBUGCLASS	${cbug}disc_${Me.Class.ShortName}()\ax
	CHECKEXIT
	/doevents
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /return
	/if (${Target.ID} && ${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	/if (${Target.ID} && ${switchdeBuffSnare}) /call cast_aa_snare ${Target2Kill}
	/call use_skill_melee
	/if (${Target.ID} && ${Me.CombatAbilityReady[${DiscVision}]} && ${Bool[${DiscVision}]} && !${Bool[${Me.Song[${DiscVision}]}]}) /call core_cast "${DiscVision}" disc 0
	/if (${Target.ID} && ${Me.AltAbilityReady[Twisted Shank]}) /call core_cast "Twisted Shank" alt ${Target2Kill}
	/if (${Target.ID} && ${Me.AltAbilityReady[Absorbing Agent]}) /call core_cast "Absorbing Agent" alt ${Target2Kill}
	| /if (${Target.ID} && ${Me.AltAbilityReady[Dirty Fighting]}) /call core_cast "Dirty Fighting" alt ${Target2Kill}
	/if (${Target.ID} && ${Me.CombatAbilityReady[${DiscAssault}]} && ${Bool[${DiscAssault}]}) /call core_cast "${DiscAssault}" disc ${Target2Kill}
	/if (${Target.ID} && ${Me.CombatAbilityReady[${DiscBleed}]} && ${Bool[${DiscBleed}]}) /call core_cast "${DiscBleed}" disc ${Target2Kill}
	/if (${Target.ID} && ${Me.CombatAbilityReady[${DiscPlay}]} && ${Bool[${DiscPlay}]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscPlay}" disc 0
	/if (${Target.ID} && ${Me.CombatAbilityReady[${DiscPhantom}]} && ${Bool[${DiscPhantom}]}) /call core_cast "${DiscPhantom}" disc ${Target2Kill}
	/if (${Target.ID} && ${Me.CombatAbilityReady[${DiscHiddenBlade}]} && ${Bool[${DiscHiddenBlade}]}) /call core_cast "${DiscHiddenBlade}" disc 0
	/call use_skill_melee
	/if (${Target.ID} && ${Me.CombatAbilityReady[${DiscHack}]} && ${Bool[${DiscHack}]}) /call core_cast "${DiscHack}" disc ${Target2Kill}
	/if (${Target.ID} && ${Me.CombatAbilityReady[${DiscBladePoison}]} && ${Bool[${DiscBladePoison}]}) /call core_cast "${DiscBladePoison}" disc 0
	/if (${Target.ID} && ${Bool[${BladeStorm}]}) /call core_cast "${BladeStorm}" disc ${Target2Kill}
	/if (${Target.ID} && ${Me.CombatAbilityReady[${DiscAggro}]} && ${Bool[${DiscAggro}]}) /call core_cast "${DiscAggro}" disc ${Target2Kill}
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
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /return
	/call use_skill_melee
	/call click_horn_of_unity 	
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
	/if (${Me.PctHPs} <= 55*.${tmpHealOverride} && ${Me.AltAbilityReady[Tumble]}) /call core_cast "Tumble" alt 0
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0
/return



|***
 *	DES: SoS switch
 *	USE: auto
 *	INI: 
 *  NOTE: Work in progress
 ***|
sub do_SoS(bool _hide)
	DEBUGCLASS ${cbug}do_SoS()\ax
	AMIDEAD

	CHECKTIE
	CHECKREZ
	
	/if (${_hide}) {
		/varset flagSoS TRUE
		/if (!${Me.Casting.ID} && ${Me.State.NotEqual[BIND]} && !${Window[TradeWnd].Open} && !${Window[MerchantWnd].Open} && !${Window[BigBankWnd].Open}) {
			/if (${Me.AbilityReady[Sneak]} && !${Me.Sneaking}) /doability Sneak 
			/if (${Me.AbilityReady[Hide]}) /doability Hide 
		}
	} else /if (!${_hide}) {
		/varset flagSoS FALSE
		/makemevisible
	}

/return


|**
	${flagSoS}
	

${Me.AbilityReady[Sneak]}
${Me.Sneaking}
/doability Sneak


${Me.AbilityReady[Hide]}
/doability Hide

**|



|***
 *	DES: 
 *	USE: /call click_poison OR /poison
 *	INI: 
 *  NOTE: 
 ***|
sub click_poison()
	DEBUGCLASS ${cbug}click_poison(${FindItemCount[${clickPoisonItem}]} <= ${clickPoisonCount})\ax
	CHECKEXIT
	CHECKTIE
	
	| keep a stock on hand
	/if (${FindItemCount[${clickPoisonItem}]} < ${clickPoisonCount} && ${Me.ItemReady[${clickPoisonDispenser}]}) {
		DEBUGCLASS click ${clickPoisonDispenser}
		/call core_cast "${clickPoisonDispenser}" item ${Me.ID}
		/call sort_cursor TRUE
	}
	
	| give us the buff
	/if (!${Bool[${Me.Buff[${clickPoisonBuff}]}]} && ${Me.ItemReady[${clickPoisonItem}]}) /call MQ2Cast "${clickPoisonItem}" item -targetid|${Me.ID}

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
		
	/if (${Me.CombatAbilityReady[Dichotomic Weapons]}) /call core_cast "Dichotomic Weapons" disc 0
	/call use_skill_melee
	/if (${Me.AltAbilityReady[Focused Rake's Rampage]}) /call core_cast "Focused Rake's Rampage" alt 0
	/if (${Me.CombatAbilityReady[${DiscPinPoint}]}) /call core_cast "${DiscPinPoint}" disc ${Target2Kill}
	/if (${Me.AltAbilityReady[Envenomed Blades]}) /call core_cast "Envenomed Blades" alt 0
	/if (${Bool[${Target2Kill}]} && ${Me.CombatAbilityReady[${DiscAggro}]} && ${Bool[${DiscAggro}]}) /call core_cast "${DiscAggro}" disc ${Target2Kill}
	/if (${Me.AltAbilityReady[Rogue's Fury]}) /call core_cast "Rogue's Fury" alt 0
	/if (${Me.CombatAbilityReady[${DiscMark}]} && ${Me.PctEndurance} > 15) /call core_cast "${DiscMark}" disc ${Target2Kill}
	/call cast_AA_spire
	| /call use_glyph
	/if (${Me.AltAbilityReady[Intensity of the Resolute]} && ${Raid.Members}) /call core_cast "Intensity of the Resolute" alt 0
  /if (${Me.CombatAbilityReady[${DiscFrenzy}]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscFrenzy}" disc 0
  /if (${Me.CombatAbilityReady[${DiscTwistedChance}]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscTwistedChance}" disc 0
  /if (${Me.CombatAbilityReady[${DiscEradicator}]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscEradicator}" disc 0
  /if (${Me.CombatAbilityReady[${DiscRazor}]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscRazor}" disc 0

	/call synergy_click_Circle
	/if (${doclickItemBurn}) /call click_item_burn
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
	/if (!${Select[${Me.Class.ShortName},ROG]}) {
		/call echo_bitch EQBC
		/return
	}
	/declare _count int local 0

	| set poison
	/if (${Bool[${_classType.Equal[poison]}]}) {
		/declare _num int local ${_classVerbage}
		
		| set max num of poison to keep
		/if (${Range.Between[1,100:${Int[${_num}]}]}) {
			/call writeINI clickPoisonCount Poison ${_num}
			VOUT Rogue Poison Keep ${sep} ${cinfo}${_num}\ax
		} else /if (!${Bool[${_num}]} || !${Range.Between[1,100:${Int[${_num}]}]}) {
			NOUT /rog poison [${cinfo}${clickPoisonCount}\ax] of poison to keep ${cinfo}1 - 100\ax
		}

	| spire control
	} else /if (${Bool[${_classType.Equal[spiregroup]}]} || ${Bool[${_classType.Equal[spireraid]}]}) {
		/declare _spiretype string local ${_classType.Left[5].Lower}${_classType.Mid[6,1].Upper}${_classType.Right[-6].Lower}
		/if (${Range.Between[0,3:${Int[${_classVerbage}]}]}) {
			/call writeINI ${_spiretype} Buffs ${_classVerbage}
		} 
		/declare _listout 	string 	local
		/declare _sep 			bool 		local
		/for _count 1 to ${typeSpire.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${${_spiretype}} == ${typeSpire.Arg[${_count},|]},${cgood},${cinfo}]}${typeSpire.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /${Me.Class.ShortName.Lower} ${_classType.Lower} [${_listout}]	


	} else {
		/call echos listtype ${Me.Class.ShortName.Lower} ${Me.Class.ShortName.Lower} 0 ##
	} 
	:exit
/return
















