|***
 *	class.wiz.inc
 *	Core Wizard Include
 *
 *	
 ***|
 


|***
 *	DES: Main Wizard Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_WIZ()
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}(${cinfo}${version${Me.Class.ShortName}}\ax${cbug})\ax
	/doevents
	ISDEAD
	CHECKTIE
	
	/if (${coreAuto}) {
		/if (${Me.PctMana} <= ${GatherPercent}*.${tmpHealOverride}) /call check_harvest
		/if (${switchBuffSelf} && !${timer_check_runes}) /call check_rune_${Me.Class.ShortName}
	}
	
	:killtarget
	/call prep_combat
	/if (!${Bool[${Macro.Return}]}) /goto :exit

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/call stick_to_target
		/if (${DoNuke}) /call cast_nuke
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/call stick_to_target
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
		/call set_combat NUKE
		CHECKTIE
		/call cast_temp_pet ${Target2Kill} "${tempPet}"
		/call send_pet
		/if (${DoNuke}) /call cast_nuke
		/call disc_${Me.Class.ShortName}
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}
		/call click_BP
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
	}
	/call use_mod_rod
	/if (${coreAuto}) {	
		/if (${switchHealCure}) /call cast_cure
		| /if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
	}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} || !${coreAuto} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Bool[${Macro.Return}]}) /goto :killtarget
	}
	:exit
/return



|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: Disc
 *	INI: 
 *  NOTE: 
 ***|
sub disc_WIZ()
	DEBUGCLASS ${cbug}disc_WIZ()\ax 
	ISDEAD
	/doevents
	CHECKTIE
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit	
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /goto :exit	
	
	| debuff the mob
	/if (${switchdeBuffEradicate}) /call cast_eradicate_magic ${Target2Kill}
	/if (${switchdeBuffSnare}) /call cast_AA_snare ${Target2Kill}
	/if (${switchdeBuffCripple} && !${Target.Buff[Lower Element].ID}) /call core_cast "Lower Element" alt ${Target2Kill}

	| ditch agro
	/if (${Me.PctAggro} > 30) /call core_cast "Arcane Whisper" alt ${Target2Kill}
	/if (${Me.PctAggro} > 80) /call core_cast "Concussive Intuition" alt ${Target2Kill}

	| filler DPS. used when inbetween/down spellgem casts
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	
	/if (${Me.AltAbilityReady[Force of Flame]}) {
		/call core_cast "Force of Flame" alt ${Target2Kill}
	} else /if (${Me.AltAbilityReady[Force of Will]}) {
		/call core_cast "Force of Will" alt ${Target2Kill}
	} else /if (${Me.AltAbilityReady[Force of Ice]}) {
		/call core_cast "Force of Ice" alt ${Target2Kill}
	}

	:exit
/return



|***
 *	DES: summon/cast Magician DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_WIZ()
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	/doevents
	ISDEAD
	/call click_horn_of_unity 	
	:exit
/return



|***
 *	DES: We want Mana
 *	USE: /gather #
 *	INI: 
 *  NOTE: 
 ***|
sub check_harvest()
	DEBUGCLASS ${cbug}check_harvest()\ax
	ISDEAD
	/doevents
	CHECKTIE
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit	

	/if (${Me.Buff[Improved Twincast].ID} || ${Me.Buff[${TwincastSpell}].ID}) /goto :exit

	/if (${Me.PctMana} <= ${GatherPercent}*.${tmpHealOverride} && ${Me.SpellReady[${GatherSpell}]}) {
		/call core_cast "${GatherSpell}" 0 0
		/goto :exit
	}
	/if (${Me.PctMana} <= ${GatherPercent}*.${tmpHealOverride} && ${Me.AltAbilityReady[Harvest of Druzzil]}) {
		/call core_cast "Harvest of Druzzil" alt 0
		/goto :exit
	}
	:exit
/return



|***
 *	DES: get the group out.
 *	USE: /g gtfo
 *	INI: 
 *  NOTE: 
 ***|
#event gtfo			"#1# tells the group, 'gtfo'"
sub Event_gtfo(string line, string _sender)
	/if (${Select[${Me.Class.ShortName},WIZ]}) {
		CHECKSAFELIST
	  /delay 10s ${Me.AltAbilityReady[Exodus]}
	  /if (${Me.Invis}) /makemevisible
	  ECHOCHANNEL Fuck this place in particular..
	 	/call core_cast "Exodus" alt 0
	}
	:exit
/return



|***
 *	DES: Keeps chanter runed. cause the're fragile and shit.
 *	USE: /call check_rune or /rb
 *	INI: 
 *  NOTE: 
 ***|
sub check_rune_WIZ()
	DEBUGCLASS ${cbug}check_rune_WIZ()\aw
	ISDEAD
	CHECKEXIT
	UPDATEHUD "BUFF: Runes"
	/if (!${Me.Buff[${runeMagiWard}].ID} && ${FindItemCount[Peridot]} && ${Spell[${runeMagiWard}].Stacks}) /call core_cast "${runeMagiWard}" ${buffGem} 0
	/if (!${Bool[${Me.Buff[${runeShieldofFate}].ID}]} && ${Spell[${runeShieldofFate}].Stacks}) /call core_cast "${runeShieldofFate}" ${buffGem} 0
	/if (!${Me.Buff[${runeCrystalwing}].ID} && ${Spell[${runeCrystalwing}].Stacks}) /call core_cast "${runeCrystalwing}" ${buffGem} 0
	/if (!${Me.Buff[${runeGuard}].ID} && ${Spell[${runeGuard}].Stacks}) /call core_cast "${runeWard}" ${buffGem} 0
	UPDATEHUD
	/call create_timer timer_check_runes ${tCheck_Runes}
	:exit
/return



|***
 *	DES: Class controls
 *	USE: /wiz []
 *	INI: 
 *  NOTE:
 ***|
#bind setWIZ /wiz
sub Bind_setWIZ(string _classType, string _classVerbage)
	DEBUG ${cbug}Bind_setWIZ(type:${_classType}, verbage:${_classVerbage})\ax
	/if (!${Select[${Me.Class.ShortName},WIZ]}) {
		/call echo_bitch EQBC
		/goto :exit
	}
	
	| 
	/if (${Bool[${_classType.Find[TEST]}]}) {
		| /call Event_SetVar NA switchNukeSummer Spells ${If[${switchNukeSummer},FALSE,TRUE]}


	} else {
		/call echos listtype ${Me.Class.ShortName.Lower} ${Me.Class.ShortName.Lower}
	} 
	:exit
/return