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
		/call use_skill_melee ${Target2Kill}
		/if (${switchForceTwincast} && !${timer_force_twincast}) /call force_twincast_${Me.Class.ShortName}
		/if (${DoNuke}) /call cast_nuke
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/call stick_to_target
		/call use_skill_melee ${Target2Kill}
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
		/call set_combat NUKE
		CHECKTIE
		/call cast_temp_pet ${Target2Kill} "${tempPet}"
		/call send_pet
		/if (${switchForceTwincast} && !${timer_force_twincast}) /call force_twincast_${Me.Class.ShortName}
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
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} && !${is_target_dead[${Target2Kill}]}) /goto :killtarget
	/if (!${coreAuto} && ${Target.ID} && ${Bool[${combat}]} && !${is_target_dead[${Target.ID}]}) /goto :killtarget
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
	/if (${is_target_dead[${Target2Kill}]}) /return
	
	| debuff the mob
	/call check_deBuffs ${Target2Kill}
	/if (${switchdeBuffCripple} && !${Target.Buff[Lower Element].ID}) /call core_cast "Lower Element" alt ${Target2Kill} FALSE

	| ditch agro
	/if (!${switchAgro} && ${Me.PctAggro} > 30) /call core_cast "Arcane Whisper" alt ${Target2Kill} FALSE
	/if (!${switchAgro} && ${Me.PctAggro} > 80) /call core_cast "Concussive Intuition" alt ${Target2Kill} FALSE

	| filler DPS. used when inbetween/down spellgem casts
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	
	/if (${Me.AltAbilityReady[Force of Flame]}) {
		/call core_cast "Force of Flame" alt ${Target2Kill} FALSE
	} else /if (${Me.AltAbilityReady[Force of Will]}) {
		/call core_cast "Force of Will" alt ${Target2Kill} FALSE
	} else /if (${Me.AltAbilityReady[Force of Ice]}) {
		/call core_cast "Force of Ice" alt ${Target2Kill} FALSE
	}
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

	/if (${switchGather} && ${Me.PctMana} < ${GatherPercent}*.${tmpHealOverride} && ${Me.SpellReady[${GatherSpell}]}) {
		/call core_cast "${GatherSpell}" 0 0 FALSE
		/return
	}
	/if (${switchGather} && ${Me.PctMana} < ${GatherPercent}*.${tmpHealOverride} && ${Me.AltAbilityReady[Harvest of Druzzil]}) {
		/call core_cast "Harvest of Druzzil" alt 0 FALSE
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
	 	/call core_cast "Exodus" alt 0 FALSE
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
	/call create_timer timer_check_runes ${tCheck_Runes}
	/if (${validate_buff[spell, rune, runeMagiWard]} && ${FindItemCount[Peridot]} > 1) /call core_cast "${runeMagiWard}" ${buffGem} 0 FALSE
	/if (${validate_buff[spell, rune, runeShieldofFate]}) /call core_cast "${runeShieldofFate}" ${buffGem} 0 FALSE
	/if (${validate_buff[spell, rune, runeCrystalwing]}) /call core_cast "${runeCrystalwing}" ${buffGem} 0 FALSE
	/if (${validate_buff[spell, rune, runeGuard]}) /call core_cast "${runeGuard}" ${buffGem} 0 FALSE
	UPDATEHUD FALSE
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
	/if (${Me.AltAbilityReady[Silent Casting]}) /call core_cast "Silent Casting" alt 0 FALSE

	| next 45 spells +51% crit rate
	/if (${Me.AltAbilityReady[Frenzied Devastation]}) /call core_cast "Frenzied Devastation" alt 0 FALSE
	| increase chance to land
	/if (${Me.AltAbilityReady[Focus of Arcanum]}) /call core_cast "Focus of Arcanum" alt 0 FALSE


	| alternate twincast
	/if (${Me.AltAbilityReady[Improved Twincast]} && !${Me.Buff[${TwincastSpell}].ID}) {
		/call core_cast "Improved Twincast" alt 0 FALSE
	} else /if (${Me.SpellReady[${TwincastSpell}]} && !${Me.Buff[Improved Twincast].ID}) {
		/call core_cast "${TwincastSpell}" 0 0 FALSE
	}
	
	/call cast_AA_spire

	| boost spell type damage
	/if (${Me.AltAbilityReady[Fury of the Gods]}) /call core_cast "Fury of the Gods" alt 0 FALSE

	/if (${Me.AltAbilityReady[Arcane Destruction]}) /call core_cast "Arcane Destruction" alt 0 FALSE
	
	| increase damage
	/if (${Me.AltAbilityReady[Arcane Fury]}) /call core_cast "Arcane Fury" alt 0 FALSE
	
	| aww baby
	/if (${switchManaBurn} && ${Me.AltAbilityReady[Mana Burn]} && (${Target.ID} != ${Me.ID})) /call core_cast "Mana Burn" alt ${Target2Kill} FALSE

	| swarm pet
	/if (${switchPcSwarm} && ${Me.AltAbilityReady[Call of Xuzl]}) /call core_cast "Call of Xuzl" alt ${Target2Kill} FALSE

	| ditch agrgro on named
	/if (!${switchAgro} && ${Me.PctAggro} > 80 && ${NameList.Find[${Target.DisplayName}]} && ${Me.AltAbilityReady[Mind Crash]}) /call core_cast "Mind Crash" alt ${Target2Kill} FALSE
	
	| epic 2.0
	/if (${Me.ItemReady[${Epic}]} && ${Spell[${FindItem[${Epic}].Spell}].Stacks} && !${Me.Buff[${FindItem[${Epic}].Spell}].ID} && !${Me.Song[${FindItem[${Epic}].Spell}].ID}) /call core_cast "${Epic}" item 0 FALSE

	/call use_glyph	
	/call synergy_click_Circle
	/if (${doclickItemBurn}) /call click_item_burn
	
/return



|***
 *	DES: Control for getting Twincast either spell or Proc
 *	USE: /call force_twincast_wiz
 *	INI: 
 *  NOTE: Twincast lets get it.
 ***|
sub force_twincast_WIZ()
	DEBUGCLASS ${cbug}force_twincast_WIZ()\ax 
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (!${timer_force_twincast}) /call create_timer timer_force_twincast ${tForce_Twincast}
	/if (!${cast_condition[NukeTwincastCondition]}) /return
	/if (${Me.SpellReady[${TwincastSpell}]} && !${Me.Buff[Improved Twincast].ID} && !${Me.Buff[Twincast].ID}) {
		/call core_cast "${TwincastSpell}" 0 0 FALSE
		/return
	} else /if (!${Me.SpellReady[${TwincastSpell}]} && !${Me.Buff[Improved Twincast].ID} && !${Me.Buff[${TwincastSpell}].ID} && !${Me.Buff[Twincast].ID}) {
		/call core_cast "${NukeTwincast}" 0 ${Target2Kill} FALSE
	}
/return



|***
 *	DES: Class controls
 *	USE: /wiz []
 *	INI: 
 *  NOTE:
 ***|
#bind setWIZ /wiz
sub Bind_setWIZ(string _classType, string _verbage)
	DEBUG ${cbug}Bind_setWIZ(type:${_classType}, verbage:${_verbage})\ax
	/if (!${validate_class[TRUE, |WIZ]}) /return

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
	| gather enable/disable /pct
	/if (${Bool[${_classType.Find[gather]}]}) {
		/if (${Bool[${_classVerbage}]} && ${Range.Between[1,99:${Int[${_classVerbage}]}]}) {
			/call writeINI GatherPercent Spells ${_classVerbage} 
			/call echos number "Gather Mana %" ${_classVerbage} 0 0
		} else /if (${Bool[${_classVerbage}]} && !${Range.Between[1,99:${Int[${_classVerbage}]}]}) {
			VOUT /${Me.Class.ShortName} gather [${cinfo}${GatherPercent}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
		}	else /if (!${Bool[${_classVerbage}]}) {
			/call writeINI switchGather Spells ${If[${switchGather},FALSE,TRUE]}  
			/call echos switch "Gather Mana ${sep} ${GatherPercent}%" switchGather 0 0
		}			
	
	| 
	} else /if (${Bool[${_classType.Equal[line]}]}) {
		| mem a line of spells.-
		/if (${Bool[${lineType.Find[${_verbage.Lower}]}]}) {
			SDEBUG ${lineType.Find[${_verbage.Lower}]}
			/call writeINI lineCurrent Spell ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
			/delay 2
			/call mem_spell_line ${lineCurrent}
		}
		/declare _listout 	string 	local
		/declare _sep 			bool 		local
		/for _count 1 to ${lineType.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${lineCurrent.Equal[${lineType.Arg[${_count},|]}]},${cgood},${cinfo}]}${lineType.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} line [${_listout}]	

	| enable/disable force twincasting
	} else /if (${Bool[${_classType.Equal[forcetwincast]}]}) {
		/call writeINI switchForceTwincast Spells ${If[${switchForceTwincast},FALSE,TRUE]}
		/call echos switch "Force Twincast" switchForceTwincast 0 0

	| enable / disable manaburn AA
	} else /if (${Bool[${_classType.Equal[manaburn]}]}) {
		/call writeINI switchManaBurn Spells ${If[${switchManaBurn},FALSE,TRUE]}
		/call echos switch "Mana Burn" switchManaBurn 0 0
		
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


	} else {
		/for _count 1 to ${${_bind}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
	} 
/return







| hold for wiz burn adjustmetns... someday

	

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