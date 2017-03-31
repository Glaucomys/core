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
	DEBUG Loop start: ${cbug}core_ROG()\ax
	/doevents
	ISDEAD
	CHECKTIE
	
	| /if (!${Me.Combat} && !${SpawnCount[npc xtarget]} && !${Me.Invis}) /call do_SoS TRUE
	

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
		/call set_combat MELEE
		/call send_pet
		/call stick_to_target
		| /call use_skill_melee
		| /call stick_to_target
		/call disc_${Me.Class.ShortName}
		| /call click_BP
		| /call use_skill_melee
		| /if (${doclickItem}) /call click_item
		| /call click_DPS_${Me.Class.ShortName}
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
	/if (${Bool[${RangeItemSummon}]}) /call check_ammo
	/if (${AutoBot} && !${Spawn[${Target2Kill}].ID} || !${AutoBot} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Macro.Return}) /goto :killtarget
	}
	:exit
/return



|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: Disc
 *	INI: 
 *  NOTE: 
 ***|
sub disc_ROG()
	DEBUG	${cbug}disc_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/call is_target_dead
	/if (${Macro.Return}) /goto :exit
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	/if (${switchdeBuffSnare}) /call cast_aa_snare ${Target2Kill}
	/call use_skill_melee
	/if (${Me.CombatAbilityReady[${DiscVision}]} && ${Bool[${DiscVision}]} && !${Bool[${Me.Song[${DiscVision}]}]}) /call core_cast "${DiscVision}" disc 0
	/if (${Me.AltAbilityReady[Twisted Shank]}) /call core_cast "Twisted Shank" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[Absorbing Agent]}) /call core_cast "Absorbing Agent" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[Dirty Fighting]}) /call core_cast "Dirty Fighting" alt ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscAssault}]} && ${Bool[${DiscAssault}]}) /call core_cast "${DiscAssault}" disc ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscBleed}]} && ${Bool[${DiscBleed}]}) /call core_cast "${DiscBleed}" disc ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscPlay}]} && ${Bool[${DiscPlay}]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscPlay}" disc 0
	/if (${Me.CombatAbilityReady[${DiscPhantom}]} && ${Bool[${DiscPhantom}]}) /call core_cast "${DiscPhantom}" disc ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscHiddenBlade}]} && ${Bool[${DiscHiddenBlade}]}) /call core_cast "${DiscHiddenBlade}" disc 0
	/call use_skill_melee
	/if (${Me.CombatAbilityReady[${DiscHack}]} && ${Bool[${DiscHack}]}) /call core_cast "${DiscHack}" disc ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscBladePoison}]} && ${Bool[${DiscBladePoison}]}) /call core_cast "${DiscBladePoison}" disc 0
	/if (${Bool[${BladeStorm}]}) /call core_cast "${BladeStorm}" disc ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscAggro}]} && ${Bool[${DiscAggro}]}) /call core_cast "${DiscAggro}" disc ${Target2Kill}
	:exit
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_ROG()
	DEBUG ${cbug}click_DPS_items_${Me.Class.ShortName}()\ax 
	ISDEAD
	/doevents
	/call is_target_dead
	/if (${Macro.Return}) /goto :exit
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/call use_skill_melee
	/call click_horn_of_unity 	
	:exit
/return



|***
 *	DES: emergency healing
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_ROG()
	DEBUG ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	/call check_exit
	/if (${Macro.Return} || ${Me.PctHPs} > 81*.${tmpHealOverride}) /goto :exit
	/if (${Me.PctHPs} <= 55*.${tmpHealOverride} && ${Me.AltAbilityReady[Tumble]}) /call core_cast "Tumble" alt 0
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0
	:exit
/return



|***
 *	DES: SoS switch
 *	USE: auto
 *	INI: 
 *  NOTE: Work in progress
 ***|
sub do_SoS(bool _hide)
	DEBUG ${cbug}do_SoS()\ax
	ISDEAD
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

	:exit
/return


|**
	${flagSoS}
	

${Me.AbilityReady[Sneak]}
${Me.Sneaking}
/doability Sneak


${Me.AbilityReady[Hide]}
/doability Hide

**|