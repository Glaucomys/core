|***
 *	class.mnk.inc
 *	Core Monk Include
 *
 *
 ***|



|***
 *	DES: Main Monk Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_MNK()
	DEBUG Loop start: ${cbug}core_MNK()\ax
	/doevents
	ISDEAD
	CHECKTIE

	/if (${Bool[${RangeItemSummon}]}) /call check_ammo

	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /goto :exit
	
	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/if (${isValidTarget}) /call stick_to_target
		/call set_combat MELEE
		/call stick_to_target
		/call use_skill_melee
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/call stick_to_target
		/call use_skill_melee
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}		
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
	}
	/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
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
sub disc_MNK()
	DEBUG ${cbug}disc_MNK()\ax
	ISDEAD
	/doevents
	/call check_exit TRUE
	/if (${Macro.Return}) /goto :exit
	/call is_target_dead
	/if (${Macro.Return}) /goto :exit
	/if (${switchdeBuffSnare}) /call cast_AA_snare ${Target2Kill}
	/call get_agro_${Me.Class.ShortName}
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}

	/if (${Me.AltAbilityReady[Five Point Palm]}) /call core_cast "Five Point Palm" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[Eye Gouge]} && !${switchdeBuffSnare}) /call core_cast "Eye Gouge" alt ${Target2Kill} 
	/if (${Me.AltAbilityReady[Stunning Kick]}) /call core_cast "Stunning Kick" alt ${Target2Kill} 
	/if (${Me.AltAbilityReady[Two-Finger Wasp Touch]}) /call core_cast "Two-Finger Wasp Touch" alt ${Target2Kill}
	
	/if (${Me.CombatAbilityReady[Drunken Monkey Style]} && ${Me.CurrentEndurance} > ${Spell[Drunken Monkey Style].EnduranceCost}) /call core_cast "Drunken Monkey Style" disc 0
	/if (${Me.CombatAbilityReady[Whorl of Fists]} && ${Me.CurrentEndurance} > ${Spell[Whorl of Fists].EnduranceCost}) /call core_cast "Whorl of Fists" disc 0

	:exit
/return



|***
 *	DES: emergency healing
 *	USE: /call patch_heal_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_MNK()
	DEBUG ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	/call check_exit
	/if (${Macro.Return} || ${Me.PctHPs} > 81*.${tmpHealOverride}) /goto :exit
	
	/if (${Me.PctHPs} <= 50*.${tmpHealOverride}) /doability Mend
	/if (${Me.CombatAbilityReady[Defer Death]} && ${Me.PctHPs} <= 50*.${tmpHealOverride}) /call core_cast "Defer Death" disc 0

	:exit
/return



|***
 *	DES: DPS Clickies
 *	USE: /call click_DPS_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_MNK()
	DEBUG ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/call use_skill_melee
	/call click_horn_of_unity 	
	:exit
/return



|***
 *	DES: Agro Routines
 *	USE: /call get_agro_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: manual/Named/raid. all sorts...
 ***|
sub get_agro_MNK()
	DEBUG ${cbug}get_agro_${Me.Class.ShortName}()\ax
	ISDEAD

	:exit
/return