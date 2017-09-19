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
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}()\ax
	/doevents
	AMIDEAD

	CHECKTIE
	
	/if (${coreAuto}) {
		/if (${Me.PctMana} <= ${GatherPercent}*.${tmpHealOverride}) /call check_harvest
		/if (${switchBuffSelf} && !${timer_check_runes}) /call check_rune_${Me.Class.ShortName}
	}
	
	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /return

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
	/if (${coreAuto}) {	
		/call use_mod_rod
		/if (${switchHealCure}) /call cast_cure
	}
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
sub disc_WIZ()
	DEBUGCLASS ${cbug}disc_WIZ()\ax 
	CHECKEXIT
	/doevents
	CHECKTIE
	/call is_target_dead
	/if (${Macro.Return}) /goto :exit	
	
	| debuff the mob
	/if (${switchdeBuffEradicate}) /call cast_eradicate_magic ${Target2Kill}
	/if (${switchdeBuffSnare}) /call cast_AA_snare ${Target2Kill}
	/if (${switchdeBuffCripple} && !${Target.Buff[Lower Element].ID}) /call core_cast "Lower Element" alt ${Target2Kill}

	| ditch agro
	/if (!${switchAgro} && ${Me.PctAggro} > 30) /call core_cast "Arcane Whisper" alt ${Target2Kill}
	/if (!${switchAgro} && ${Me.PctAggro} > 80) /call core_cast "Concussive Intuition" alt ${Target2Kill}

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
	AMIDEAD

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
	CHECKEXIT
	/doevents
	CHECKTIE

	/if (${Me.Buff[Improved Twincast].ID} || ${Me.Buff[${TwincastSpell}].ID}) /return

	/if (${Me.PctMana} <= ${GatherPercent}*.${tmpHealOverride} && ${Me.SpellReady[${GatherSpell}]}) {
		/call core_cast "${GatherSpell}" 0 0
		/return
	}
	/if (${Me.PctMana} <= ${GatherPercent}*.${tmpHealOverride} && ${Me.AltAbilityReady[Harvest of Druzzil]}) {
		/call core_cast "Harvest of Druzzil" alt 0
		/return
	}
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
/return



|***
 *	DES: Keeps chanter runed. cause the're fragile and shit.
 *	USE: /call check_rune or /rb
 *	INI: 
 *  NOTE: 
 ***|
sub check_rune_WIZ()
	DEBUGCLASS ${cbug}check_rune_WIZ()\aw
	AMIDEAD

	CHECKEXIT
	UPDATEHUD "BUFF: Runes"
	/if (!${Me.Buff[${runeMagiWard}].ID} && ${FindItemCount[Peridot]} && ${Spell[${runeMagiWard}].Stacks}) /call core_cast "${runeMagiWard}" ${buffGem} 0
	/if (!${Bool[${Me.Buff[${runeShieldofFate}].ID}]} && ${Spell[${runeShieldofFate}].Stacks}) /call core_cast "${runeShieldofFate}" ${buffGem} 0
	/if (!${Me.Buff[${runeCrystalwing}].ID} && ${Spell[${runeCrystalwing}].Stacks}) /call core_cast "${runeCrystalwing}" ${buffGem} 0
	/if (!${Me.Buff[${runeGuard}].ID} && ${Spell[${runeGuard}].Stacks}) /call core_cast "${runeWard}" ${buffGem} 0
	UPDATEHUD
	/call create_timer timer_check_runes ${tCheck_Runes}
/return



 |***
 *	DES: Wizard burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_WIZ()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	CHECKEXIT
	/doevents
	
	| shhhhh..
	/if (${Me.AltAbilityReady[Silent Casting]}) /call core_cast "Silent Casting" alt 0

	| next 45 spells +51% crit rate
	/if (${Me.AltAbilityReady[Frenzied Devastation]}) /call core_cast "Frenzied Devastation" alt 0
	| increase chance to land
	/if (${Me.AltAbilityReady[Focus of Arcanum]}) /call core_cast "Focus of Arcanum" alt 0


	| alternate twincast
	/if (${Me.AltAbilityReady[Improved Twincast]} && !${Me.Buff[${TwincastSpell}].ID}) {
		/call core_cast "Improved Twincast" alt 0
	} else /if (${Me.SpellReady[${TwincastSpell}]} && !${Me.Buff[Improved Twincast].ID}) {
		/call core_cast "${TwincastSpell}" 0 0
	}
	
	/call cast_AA_spire

	| boost spell type damage
	/if (${Me.AltAbilityReady[Fury of Kerafyrm]} && !${Bool[${Me.Buff[Fury of Ro]}]}) /call core_cast "Fury of Kerafyrm" alt 0
	/if (${Me.AltAbilityReady[Fury of Ro]} && !${Bool[${Me.Buff[Fury of Kerafyrm]}]}) /call core_cast "Fury of Ro" alt 0

	/if (${Me.AltAbilityReady[Arcane Destruction]}) /call core_cast "Arcane Destruction" alt 0
	
	| increase damage
	/if (${Me.AltAbilityReady[Arcane Fury]}) /call core_cast "Arcane Fury" alt 0
	
	| aww baby
	/if (${Me.AltAbilityReady[Mana Burn]} && (${Target.ID} != ${Me.ID})) /call core_cast "Mana Burn" alt ${Target2Kill}

	| swarm pet
	/if (${switchPcSwarm} && ${Me.AltAbilityReady[Call of Xuzl]}) /call core_cast "Call of Xuzl" alt ${Target2Kill}    

	| ditch agrgro on named
	/if (!${switchAgro} && ${Me.PctAggro} > 80 && ${NameList.Find[${Target.DisplayName}]} && ${Me.AltAbilityReady[Mind Crash]}) /call core_cast "Mind Crash" alt ${Target2Kill}
	
	| epic 2.0
	/if (${Me.ItemReady[${Epic}]} && ${Spell[${FindItem[${Epic}].Spell}].Stacks} && !${Me.Buff[${FindItem[${Epic}].Spell}].ID} && !${Me.Song[${FindItem[${Epic}].Spell}].ID}) /call core_cast "${Epic}" item 0

	/call use_glyph	
	/call synergy_click_Circle
	/if (${doclickItemBurn}) /call click_item_burn
	
/return
	

frostbound alliance - XXXX
- dichotomic fire
- ethereal fuse
- cloudburst stormstrike
- ethereal skyblaze
- claw of the flameweaver
ethereal rimblast
- twincast
chaos scintillation
ethereal hoarfrost XXXXX
shield of consequence
- quintescent harvest/gambit



1211 - Focus of Arcanum
759 - Fury of Kerafyrm
840 - Arcane Fury
1351 - second spire
515 - Improved Twincast

500 - Silent Casting

759 - Fury of Kerafyrm  -- still on cooldown
1152 - Fury of Ro
7019- Glyph of Destruction II


1265 - Arcane Distruction
308 - Frenzied Devastation
1152 - Fury of Ro

1152 - Fury of Ro
7019 - Glyph of Destruction II


1154 - Force of Will
1266 - Force of Flame
1267 - Force of Ice
577 - concussive intuition
15073 - banestrike



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
		/return
	}
	/declare _count int local 0

	
	| 
	/if (${Bool[${_classType.Equal[line]}]}) {


		| mem a line of spells.-
		/if (${Bool[${lineType.Find[${_classVerbage.Lower}]}]}) {
			SDEBUG ${lineType.Find[${_classVerbage.Lower}]}
			/call writeINI lineCurrent Spell ${_classVerbage.Left[1].Upper}${_classVerbage.Right[-1].Lower}
			/delay 2
			/call mem_spell_line ${lineCurrent}
		}
		/declare _listout 	string 	local
		/declare _sep 			bool 		local
		/for _count 1 to ${lineType.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${lineCurrent.Equal[${lineType.Arg[${_count},|]}]},${cgood},${cinfo}]}${lineType.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /wiz line [${_listout}]	
		
		
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
		/call echos listtype ${Me.Class.ShortName.Lower} ${Me.Class.ShortName.Lower} 0 0
	} 
	:exit
/return