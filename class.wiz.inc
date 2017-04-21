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
	DEBUG Loop start: ${cbug}core_WIZ()\ax
	/doevents
	ISDEAD
	CHECKTIE
	
	/if (${AutoBot}) {
		/if (${Me.PctMana} <= ${GatherPercent}*.${tmpHealOverride}) /call check_harvest
	}
	
	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /goto :exit

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
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
sub disc_WIZ()
	DEBUG ${cbug}disc_WIZ()\ax 
	ISDEAD
	/doevents
	CHECKTIE
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/call is_target_dead
	/if (${Macro.Return}) /goto :exit
	
	| debuff the mob
	/if (${switchdeBuffEradicate}) /call cast_eradicate_magic ${Target2Kill}
	/if (${switchdeBuffSnare}) /call cast_AA_snare ${Target2Kill}
	/if (!${Target.Buff[Lower Element].ID}) /call core_cast "Lower Element" alt ${Target2Kill}

	| ditch agro
	/if (${Me.PctAggro} > 30) /call core_cast "Arcane Whisper" alt ${Target2Kill}
	/if (${Me.PctAggro} > 80) /call core_cast "Concussive Intuition" alt ${Target2Kill}

	| filler DPS. used when inbetween/down spellgem casts
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	
	/if (${Me.AltAbilityReady[Force of Flame]}) {
		/call core_cast "Force of Flame" alt ${Target2Kill}
		/goto :exit
	}
	/if (${Me.AltAbilityReady[Force of Will]}) {
		/call core_cast "Force of Will" alt ${Target2Kill}
		/goto :exit
	}
	/if (${Me.AltAbilityReady[Force of Ice]}) {
		/call core_cast "Force of Ice" alt ${Target2Kill}
		/goto :exit
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
	DEBUG ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
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
	DEBUG ${cbug}check_harvest()\ax
	ISDEAD
	/doevents
	CHECKTIE
	/call check_exit
	/if (${Macro.Return}) /goto :exit	

	/if (${Me.Buff[Improved Twincast].ID} || ${Me.Buff[${TwincastSpell}].ID}) /goto :exit

	/if (${Me.SpellReady[${GatherSpell}]}) {
		/call core_cast "${GatherSpell}" 0 ${Me.ID}
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
