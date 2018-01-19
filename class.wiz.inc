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

	CHECKREZ
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
		/call click_item
		/call click_DPS_${Me.Class.ShortName}
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
		/call set_combat NUKE
		CHECKTIE
		/call cast_temp_pet ${Target2Kill} "${tempPet}"
		/call send_pet
		/if (${switchForceTwincast} && !${timer_force_twincast}) /call force_twincast_${Me.Class.ShortName}
		/call cast_nuke_wiz
		/call disc_${Me.Class.ShortName}
		/call click_item
		/call click_DPS_${Me.Class.ShortName}
		/call click_BP
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
	}
	/if (${coreAuto}) {	
		/call use_mod_rod
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
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
	/if (${switchdeBuffCripple} && !${Target.Buff[Lower Element].ID} && !${Target.Crippled.Duration}) /call core_cast "Lower Element" alt ${Target2Kill} FALSE

	| ditch agro
	/if (!${switchAgro} && ${Me.PctAggro} > 30) /call core_cast "Arcane Whisper" alt ${Target2Kill} FALSE
	/if (!${switchAgro} && ${Me.PctAggro} > 80) /call core_cast "Concussive Intuition" alt ${Target2Kill} FALSE

	| filler DPS. used when inbetween/down spellgem casts
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	
	| aa Force of Flame
	/if (${lineCurrent.Equal[Fire]} && ${Me.AltAbilityReady[Force of Flame]}) {
		/if (${Me.CurrentMana} > ${Spell[Force of Flame].Mana}) {
			/call core_cast "Force of Flame" alt ${Target2Kill} FALSE
			/return
		}
	| aa Force of Ice
	} else /if (${lineCurrent.Equal[Ice]} && ${Me.AltAbilityReady[Force of Ice]}) {
		/if (${Me.CurrentMana} > ${Spell[Force of Ice].Mana}) {
			/call core_cast "Force of Ice" alt ${Target2Kill} FALSE
			/return
		}
	| aa Force of Will
	| } else /if (${lineCurrent.Equal[Magic]} && ${Me.AltAbilityReady[Force of Will]}) {
	| 	/if (${Me.CurrentMana} > ${Spell[Force of Will].Mana}) {
	| 		/call core_cast "Force of Will" alt ${Target2Kill} FALSE
	| 		/return
	| 	}
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

	/if (${switchGather}) {
		/if (!${Me.SpellReady[${GatherSpell}]} && ${Me.AltAbilityReady[Forceful Rejuvenation]}) {
			/call core_cast "Forceful Rejuvenation" alt 0 FALSE
			/delay 5			
		}
		/if (${Me.PctMana} < ${GatherPercent}*.${tmpHealOverride} && ${Me.SpellReady[${GatherSpell}]}) {
			/call core_cast "${GatherSpell}" 0 0 FALSE
			/return
		}
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
 *	DES: emergency healing
 *	USE: /call patch_heal_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_WIZ()
	DEBUGCLASS ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	/call click_item_heal
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
	/if (${validate_buff[spell, rune, runeMagiWard]} && ${FindItemCount[Peridot]} > 1) /call core_cast "${runeMagiWard}" ${buffGem} 0 FALSE TRUE
	/if (${validate_buff[spell, rune, runeShieldofFate]}) /call core_cast "${runeShieldofFate}" ${buffGem} 0 FALSE TRUE
	/if (${validate_buff[spell, rune, runeCrystalwing]}) /call core_cast "${runeCrystalwing}" ${buffGem} 0 FALSE TRUE
	/if (${validate_buff[spell, rune, runeGuard]}) /call core_cast "${runeGuard}" ${buffGem} 0 FALSE TRUE
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
	/if (${Me.ItemReady[${${Me.Class.ShortName}Epic2}]} && ${Spell[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].Stacks} && !${Me.Buff[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID} && !${Me.Song[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID}) /call core_cast "${${Me.Class.ShortName}Epic2}" item 0 FALSE

	/call use_glyph	
	/call synergy_click_Circle
	/call click_item_burn
	
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
	} else /if (!${Me.SpellReady[${TwincastSpell${lineCurrent}}]} && !${Me.Buff[Improved Twincast].ID} && !${Me.Buff[${TwincastSpell}].ID} && !${Me.Buff[Twincast].ID}) {
		/call core_cast "${NukeTwincast${lineCurrent}}" 0 ${Target2Kill} FALSE
	}
/return



|***
 *	DES: memorize spell line
 *	USE: /call mem_song_line [] | /wiz line [fire|ice|magic|mix]
 *	INI: [Spell] -> line[fire|ice|magic|mix]
 *  NOTE:
 ***|
sub mem_spell_line(string _line)
 	DEBUGCLASS ${cbug}mem_spell_line(group: ${_line})\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	/declare _index 				int 		local 0
	/declare _list		 			string 	local ${line${lineCurrent}}
	/declare _currentSong 	string 	local NULL
	
	| mem twincast spell
	/if (!${Me.Gem[1].Name.Equal[${NukeTwincast${lineCurrent}}]} && ${Bool[${NukeTwincast${lineCurrent}}]}) {
		/call mem_spell TRUE "${NukeTwincast${lineCurrent}}" 1 FALSE
	}	
	
	| mem remainder line of spells
	/for _index 1 to ${_list.Count[|]}
		/if (${Me.Gem[${_index}].Name.Equal[${Spell[${_list.Arg[${_index},|]}].RankName}]}) /continue
		/call mem_spell TRUE "${Spell[${_list.Arg[${_index},|]}].RankName}" ${Int[${Math.Calc[${_index}+1]}]} FALSE
	/next _index	
	VOUT Line ${sep} ${cinfo}${lineCurrent}\ax memorization completed.
/return



|***
 *	DES: Cast nukes
 *	USE: /call cast_nuke
 *	INI: 
 *  NOTE: "The only good bug is a dead bug" -- Would you like to know more?
 ***|
sub cast_nuke_WIZ()
	DEBUGCAST ${cbug}cast_nuke_wiz()\ax
	/if (${timer_Nuke_Delay}) /return	
	/call get_assist_target
	/declare _count 				int 		local 0
	/declare _list		 			string 	local ${line${lineCurrent}}
	
	/for _count 1 to ${_list.Count[|]}
		/if (!${Range.Between[1,${_list.Count[|]}:${Int[${_count}]}]}) /continue
		DEBUG _count:${_count}, spell:${Spell[${_list.Arg[${_count},|]}].RankName}, manna:${Me.CurrentMana} < ${Spell[${_list.Arg[${_count},|]}].Mana}
		AMIDEAD
		CHECKEXIT
		
		/if (${is_target_dead[${Target2Kill}]}) /return
		/if (${Me.CurrentMana} < ${Spell[${_list.Arg[${_count},|]}].Mana} || ${Me.CurrentMana} < 2) /continue 
		/if (!${cast_condition[line${lineCurrent}Condition${_count}]}) /continue 
		
		/call core_cast "${Spell[${_list.Arg[${_count},|]}].RankName}" 0 ${Target2Kill} cast_break
		/if (${Select[${castReturn},CAST_SUCCESS]}) /call create_timer timer_Nuke_Delay ${tNuke_Delay}

		| toons ini switch. maintain the loop of nuked till all are cycled.
		/if (${forceNukeLoop}) {
			/delay 10s !${Me.Casting.ID}
			/delay 2.5s
			/delay ${timer_Nuke_Delay}
		}

	/next _count
/return








|***
 *	DES: Make sure you are using the AA casting boost you want..
 *	USE: /call sort_mancy_lineAA
 *	INI: 
 *  NOTE:
 ***|
sub sort_mancy_lineAA()

|**
	/if (${lineCurrent.Equal[Fire]} && !${Me.Buff[${lineAAFire}].ID}) {
		/if (${Me.Buff[${lineAAIce}].ID}) /squelch /removebuff ${lineAAIce}
		/if (${Me.Buff[${lineAAMagic}].ID}) /squelch /removebuff ${lineAAMagic}
		/if (${Me.Buff[${lineAAMix}].ID}) /squelch /removebuff ${lineAAMix}
	} else /if (${lineCurrent.Equal[Ice]} && !${Me.Buff[${lineAAIce}].ID}) {
		/if (${Me.Buff[${lineAAFire}].ID}) /squelch /removebuff ${lineAAFire}
		/if (${Me.Buff[${lineAAMagic}].ID}) /squelch /removebuff ${lineAAMagic}
		/if (${Me.Buff[${lineAAMix}].ID}) /squelch /removebuff ${lineAAMix}
	} else /if (${lineCurrent.Equal[Magic]} && !${Me.Buff[${lineAAMagic}].ID}) {
		/if (${Me.Buff[${lineAAFire}].ID}) /squelch /removebuff ${lineAAFire}
		/if (${Me.Buff[${lineAAIce}].ID}) /squelch /removebuff ${lineAAIce}
		/if (${Me.Buff[${lineAAMix}].ID}) /squelch /removebuff ${lineAAMix}
	} else /if (${lineCurrent.Equal[Mix]}&& !${Me.Buff[${lineAAMix}].ID}) {
		/if (${Me.Buff[${lineAAFire}].ID}) /squelch /removebuff ${lineAAFire}
		/if (${Me.Buff[${lineAAIce}].ID}) /squelch /removebuff ${lineAAIce}
		/if (${Me.Buff[${lineAAMagic}].ID}) /squelch /removebuff ${lineAAMagic}
	}
**|	
	/if (${lineCurrent.Equal[Fire]} && ${Me.Buff[${lineAAFire}].ID}) /return TRUE
	/if (${lineCurrent.Equal[Ice]} && ${Me.Buff[${lineAAIce}].ID}) /return TRUE
	/if (${lineCurrent.Equal[Magic]} && ${Me.Buff[${lineAAMagic}].ID}) /return TRUE
	/if (${lineCurrent.Equal[Mix]}&& ${Me.Buff[${lineAAMix}].ID}) /return TRUE
	/call core_cast "${lineAA${lineCurrent}}" alt 0 FALSE	
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
		} else /if (${Bool[${_classVerbage}]} && !${Range.Between[1,99:${Int[${_classVerbage}]}]}) {
			VOUT /${_bind} gather(${cnum}${GatherPercent}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_classVerbage}]}) {
			/call writeINI switchGather Spells ${If[${switchGather},FALSE,TRUE]}  
			VOUT /${_bind} gather(${cnum}${GatherPercent}\ax) ${sep} ${If[${switchGather},${oenabled},${odisabled}]}
			/return
		}			

	| spell line. not quite done with this. 
	} else /if (${Bool[${_classType.Equal[line]}]}) {
		| mem a line of spells.
		/if (${Bool[${lineType.Find[${_verbage.Lower}]}]}) {
			DEBUG ${lineType.Find[${_verbage.Lower}]}
			/call writeINI lineCurrent Spells ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
			/call sort_mancy_lineAA
			/delay 2
			
			/call mem_spell_line ${lineCurrent}
			/return
		}
		/for _count 1 to ${lineType.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${lineCurrent.Equal[${lineType.Arg[${_count},|]}]},${cgood},${cinfo}]}${lineType.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} line [${_listout}]	
		/return


	| enable/disable force twincasting
	} else /if (${Bool[${_classType.Equal[forcetwincast]}]}) {
		/call writeINI switchForceTwincast Spells ${If[${switchForceTwincast},FALSE,TRUE]}
		VOUT /${_bind} forcetwincast ${sep} ${If[${switchForceTwincast},${oenabled},${odisabled}]}
		/return

	| enable / disable manaburn AA
	} else /if (${Bool[${_classType.Equal[manaburn]}]}) {
		/call writeINI switchManaBurn Spells ${If[${switchManaBurn},FALSE,TRUE]}
		VOUT /${_bind} manaburn ${sep} ${If[${switchManaBurn},${oenabled},${odisabled}]}
		/return

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

	| healpoint
	} else /if (${Bool[${_classType.Find[healpoint]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI ${Me.Class.ShortName}Point Heal ${_verbage}
		} else /if (!${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} healpoint(${cnum}${${Me.Class.ShortName}Point}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}

	}
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
		/if (${${_bind}Type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}${${Me.Class.ShortName}Point}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[gather]}) /varset _listout ${_listout}(${cnum}${GatherPercent}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return