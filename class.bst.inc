|***
 *	class.bst.inc
 *	Core Beastlord Include
 *
 *
 ***|



|***
 *	DES: Main Beastlord Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_BST()
	DEBUG Loop start: ${cbug}core_BST()\ax
	/doevents
	ISDEAD
	CHECKTIE

	/if (!${Me.Invis}) /call cast_focused_paragon

	/if (${AutoBot} && !${Me.Invis}) {
		/call check_engaged
		/if (!${Macro.Return} && ${usePet}) /call check_pet
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
			| /call is_toast_time
			| /if (${Macro.Return}) /call synergy_${Me.Class.ShortName}
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
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke
		/if (${switchdeBuffSlow}) /call cast_AA_slow ${Target2Kill} "Sha's Reprisal"
		/if (${usePet} && ${Bool[${ServantNuke}]}) /call cast_nuke_servant
		/call use_skill_melee
		/if (${doclickItem}) /call click_Item
		/call click_DPS_${Me.Class.ShortName}
		/call cast_focused_paragon
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
		/call set_combat NUKE
		/call send_pet
		/call click_BP
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke
		/if (${usePet} && ${Bool[${ServantNuke}]}) /call cast_nuke_servant
		/if (${switchdeBuffSlow}) /call cast_AA_slow ${Target2Kill} "Sha's Reprisal"
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}
		/call cast_focused_paragon
	} else /if (${Select[${combat},HEAL]}) {
	}	else /if (${Select[${combat},DUAL]}) {
	}
	/if (${AutoBot}) {	
		/if (${switchHealCure}) /call cast_cure
	}
	/call use_mod_rod
	/if (${AutoBot} && ${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
	/if (${AutoBot} && !${Spawn[${Target2Kill}].ID} || !${AutoBot} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Macro.Return}) /goto :killtarget
	}
	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub disc_BST()
	DEBUG ${cbug}disc.BST()\ax
	ISDEAD
	/doevents

	/if (${Me.PctEndurance} < 3) /goto :exit
	/call check_exit
	/if (${Macro.Return}) /goto :exit
			
|	/if (${Me.CurrentMana} > ${Spell[${FeroSpell}].Mana} && !${Me.Buff[${FeroSpell}].ID}) /call core_cast "${FeroSpell}" ${BuffGem} ${Me.ID}
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}

	/if (!${Me.Song[${GrowlSpell}].ID} && ${Me.Pet.ID} && !${Me.PetBuff[Savage Rage]} && !${Me.Song[${DiscSavage}].ID}) /call core_cast "${GrowlSpell}"
	/if (${Me.AltAbilityReady[Chameleon Strike]} && ${Target.Distance} < ${Spell[Chameleon Strike].MyRange}) /call core_cast "Chameleon Strike" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[Feral Swipe]} && ${Target.Distance} < ${Spell[Feral Swipe].MyRange}) /call core_cast "Feral Swipe" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[Bite of the Asp]} && ${Target.Distance} < ${Spell[Bite of the Asp].MyRange}) /call core_cast "Bite of the Asp" alt ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscForay}]} && ${Target.Distance} < ${Spell[${DiscForay}].MyRange} && ${Me.PctEndurance} > 5 ) /call core_cast "${DiscForay}" disc 0
	/if (${Me.CombatAbilityReady[${DiscFocusClaws}]}) /call core_cast "${DiscFocusClaws}" disc 0
	/if (${Me.CombatAbilityReady[${DiscBestial}]} &&  !${Me.Song[${DiscBestial}].ID}) /call core_cast "${DiscBestial}" disc 0
	/if (${Me.AltAbilityReady[Roar of thunder]}) /call core_cast "Roar of Thunder" alt 0
	/if (${Me.AltAbilityReady[${DiscEnduring}]} && ${Target.Distance} < ${Spell[${DiscEnduring}].MyRange} && !${Bool[${Targe.Buff[${DiscEnduring}]}]}) /call core_cast "${DiscEnduring}" alt ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscRending}]} && ${Me.PctEndurance} > 5 ) /call core_cast "${DiscRending}" disc 0

	/call cast_focused_paragon
	:exit
/return



|***
 *	DES: Focused Paragon. will maintain the bst then group if able
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub cast_focused_paragon()
	DEBUG ${cbug}cast_focused_paragon()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/declare _i 							int 	local
	/declare _tmpID 					int 	local 0
	/if (${Me.AltAbilityReady[Focused Paragon of Spirits]} && ${Me.PctMana} < ${selfParagon} && !${Me.Song[Paragon].ID}) /call core_cast "Focused Paragon of Spirits" alt ${Me.ID}
	/if (${Me.AltAbilityReady[Consumption of Spirit]} && ${Me.PctHPs} > ${minConsumptionHP}*.${tmpHealOverride} && ${Me.PctMana} < ${selfParagon}*.${tmpHealOverride}) /call core_cast "Consumption of Spirit" alt ${Me.ID}
	/if (${Group} && ${Me.AltAbilityReady[Focused Paragon of Spirits]}) {
		 /for _i 1 to ${Group}
		 		/if (${Group.Member[${_i}].ID} && ${Group.Member[${_i}].Distance} <= ${Spell[Focused Paragon of Spirits].MyRange}  && (${Group.Member[${_i}].CurrentMana} < ${groupParagon}*.${tmpHealOverride}) && ${CanCast.Find[${Group.Member[${_i}].Class}]}) {
				/varset _tmpID ${Group.Member[${i}].ID}	 		
		 	}
		/next _i
		/if (${_tmpID}) /call core_cast "Focused Paragon of Spirits" alt ${_tmpID}
	}	
	:exit
/return


 
|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_BST()
	DEBUG ${cbug}patch_heal_BST()\ax
	/doevents	
 	/if (${Me.PctHPs} < 75*.${tmpHealOverride}  && ${Me.AltAbilityReady[Warder's Gift]}) /call core_cast "Warder's Gift" alt 0
	/if (${Me.PctHPs} <= ${FastHealPCT}*.${tmpHealOverride} && ${Me.SpellReady[${FastHeal}]}) {
		/call core_cast "FastHeal" 0 ${Me.ID}
		/if (${Me.AltAbilityReady[Focused Paragon of Spirits]}) /call core_cast "Focused Paragon of Spirits" alt ${Me.ID}
	}
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} || ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${Spell[${DiscHeal}].MyRange} playerstate 4 targetable]} >= 3) /if (${Select[${combat},MELEE]} && !${Bool[${Me.Song[Bladespur Reflection]}]}) /call core_cast "${DiscHeal}" disc ${Target.ID}
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0
	/if (${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${MobAgro} playerstate 4 targetable]} >= 4 && ${Me.ItemReady[${CircleofLife}]} && !${Me.Song[${FindItem[${CircleofLife}].Spell}].ID} && ${Spell[${FindItem[${CircleofLife}].Spell}].Stacks}) /call core_cast "${CircleofLife}" item 0
	:exit
/return



|***
 *	DES: If you have to ask
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub pet_heal_BST()
	DEBUG ${cbug}pet_heal_BST()\aw
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/if (${usePet} && ${Pet.PctHPs} < 20 && ${Me.AltAbilityReady[Mend Companion]}) {
		VOUT Healing \a-w${Pet.CleanName}\ax with \a-rMend Companion\ax.
		/call MQ2Cast "Mend Companion" alt
	 	/delay 5s !${Me.SpellInCooldown}
	}
	/if (${usePet} && ${Pet.PctHPs} < 60 && ${Me.SpellReady[${PetHealSpell}]}) {
		VOUT Healing \a-w${Pet.CleanName}\ax with \a-r${PetHealSpell}\ax.
		/call MQ2Cast "${PetHealSpell}" ${PetHealType} 30s -targetid|${Pet.ID}
	 	/delay 5s !${Me.SpellInCooldown}
	}
	/if (${usePet} && ${Pet.PctHPs} < 80 && ${Me.SpellReady[${PetRenewalSpell}]}) {
		VOUT Healing \a-w${Pet.CleanName}\ax with \a-r${PetRenewalSpell}\ax.
		/call MQ2Cast "${PetRenewalSpell}" ${PetRenewalType} 30s
	 	/delay 5s !${Me.SpellInCooldown}
	}
	:exit
/return



|***
 *	DES: MGB beast Paragon if both AAs are ready
 *	USE: /bc paragon, /paragon
 *	INI: 
 *  NOTE: 
 ***|
sub cast_paragon()
	DEBUG ${cbug}cast_paragon()\ax ${If[${Me.AltAbilityReady[Mass Group Buff]},\agMGB\ax,\arMGB\ax]} ${If[${Me.AltAbilityReady[Paragon of Spirit]},${cgood}Paragon\ax,${cbad}Paragon\ax]}
	ISDEAD
	/doevents
	/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[Paragon of Spirit]} && !${Bool[${Me.Song[Paragon of Spirit]}]}) {
		/call core_cast "Mass Group Buff" alt 0
		/call core_cast "Paragon of Spirit" alt 0
		| /if (${Bool[${Raid.Members}]}) /rs ${ParagonTXT}
	} else /if (${Bool[${Me.Song[Paragon of Spirit]}]}) {
		NOUT ${owarning} trying to cast ${csp}Paragon of Spirit\ax while its running.	
	}		
	:exit
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_BST()
	DEBUG ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/call click_horn_of_unity 	
	/if (${Me.PctMana} < 60*.${tmpHealOverride} && ${Me.ItemReady[${CircleofMana}]} && !${Me.Song[${FindItem[${CircleofMana}].Spell}].ID} && ${Spell[${FindItem[${CircleofMana}].Spell}].Stacks}) /call core_cast "${CircleofMana}" item 0
	:exit
/return


