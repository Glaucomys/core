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
	DEBUG core_WIZ()

	
	/if (${coreAuto}) {
		/if (${Me.PctMana} <= ${GatherPercent}*.${tmpHealOverride}) /call check_harvest
		/if (${switchBuffSelf} && !${timer_check_runes}) /call check_rune_${Me.Class.ShortName}
	}
	
	/while (${prep_combat[]}) {
		CHECKINPUTS
		/if (${exit_class_loop[]}) /return FALSE

		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${switchHealCure}) /call cast_cure
	
		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${setBurnControl}) /call synergy_${Me.Class.ShortName}
			/if (${switchPcSwarm} || ${switchPcPet}) /call send_pet
			/if (${switchPcSwarm} && !${Me.Pet.ID}) /call cast_temp_pet ${Target2Kill} "${tempPet}"
			/if (${switchAlliance}) /call trigger_alliance ${Target2Kill}
			/call check_deBuffs ${Target2Kill}
			/if (${switchCombatNuke}) {
				/if (${switchForceTwincast}) /call force_twincast_${Me.Class.ShortName}
				/call cast_nuke_line
			}
			/if (${switchCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/call click_BP
			/call click_item
			/if (${switchPcSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
		}
		
		/if (${coreAuto}) {	
			/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
			/if (${switchModRod} && ${validate_class[FALSE, ${CastList}]}) /call use_mod_rod
		}	
	}	
/return TRUE



|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: Disc
 *	INI: 
 *  NOTE: 
 ***|
sub disc_WIZ()
	DEBUG disc_WIZ()
	CHECKEXIT
	CHECKINPUTS
	CHECKTIE
	/if (${is_target_dead[${Target2Kill}]}) /return
	
	| debuff the mob
	/if (${switchdeBuffCripple}) {
		/if (${validate_cast[FALSE, alt, "Lower Element", ${Target2Kill}]} && !${Target.Buff[Lower Element].ID} && !${Target.Crippled.Duration}) /call core_cast2 "Lower Element" alt ${Target2Kill} FALSE
	}

	| agro control
	/if (${switchAgro}) {
	} else /if (!${switchAgro}) {
		/if (${validate_cast[FALSE, alt, "Arcane Whisper", ${Target2Kill}]} && ${Me.PctAggro} > 30) /call core_cast2 "Arcane Whisper" alt ${Target2Kill} FALSE
		/if (${validate_cast[FALSE, alt, "Mind Crash", ${Target2Kill}]} && ${Me.PctAggro} > 80 && ${NameList.Find[${Spawn[${Target2Kill}].DisplayName}]}) /call core_cast2 "Mind Crash" alt ${Target2Kill} FALSE
		/if (${validate_cast[FALSE, alt, "Concussive Intuition", ${Target2Kill}]} && ${Me.PctAggro} > 80) /call core_cast2 "Concussive Intuition" alt ${Target2Kill} FALSE
		/if (${validate_cast[FALSE, spell, "${spellConcussion}", ${Target2Kill}]} && ${Me.PctAggro} > ${spellConcussionPct}) /call core_cast2 "${spellConcussion}" 0 ${Target2Kill} FALSE
	}
	
	| filler DPS. used when inbetween/down spellgem casts
	| aa Force of Flame
	/if (${lineCurrent.Equal[Fire]} && !${NoFireList.Find[${Spawn[${Target2Kill}].DisplayName}]}) {
		/if (${validate_cast[FALSE, alt, "Force of Flame", ${Target2Kill}]}) /call core_cast2 "Force of Flame" alt ${Target2Kill} FALSE
		/return
	| aa Force of Ice
	} else /if (${lineCurrent.Equal[Ice]} && !${NoColdList.Find[${Spawn[${Target2Kill}].DisplayName}]}) {
		/if (${validate_cast[FALSE, alt, "Force of Ice", ${Target2Kill}]}) /call core_cast2 "Force of Ice" alt ${Target2Kill} FALSE
		/return
	| aa Force of Will
	} else /if (${lineCurrent.Equal[Magic]} && !${NoMagicList.Find[${Spawn[${Target2Kill}].DisplayName}]}) {
		/if (${validate_cast[FALSE, alt, "Force of Will", ${Target2Kill}]}) /call core_cast2 "Force of Will" alt ${Target2Kill} FALSE
		/return
	}
/return



|***
 *	DES: summon/cast Magician DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_WIZ()
	DEBUG click_DPS_WIZ()
	CHECKINPUTS
	AMIDEAD
	/call click_horn_of_unity 	
/return



|***
 *	DES: We want Mana
 *	USE: /wiz gather #
 *	INI: 
 *  NOTE: 
 ***|
sub check_harvest()
	DEBUG check_harvest()
	CHECKEXIT
	CHECKINPUTS
	CHECKTIE

	/if (${Me.Buff[Improved Twincast].ID} || ${Me.Buff[${TwincastSpell}].ID}) /return

	/if (${switchGather}) {
		/if (!${Me.SpellReady[${GatherSpell}]}) {
			/if (${validate_cast[FALSE, alt, "Forceful Rejuvenation", ${Me.ID}]}) /call core_cast2 "Forceful Rejuvenation" alt 0 FALSE
			/delay 5			
		}
		/if (${Me.PctMana} < ${GatherPercent}*.${tmpHealOverride}) {
			/if (${Me.SpellReady[${GatherSpell}]}) {
				/if (${validate_cast[FALSE, spell, "${GatherSpell}", ${Me.ID}]}) /call core_cast2 "${GatherSpell}" 0 0 FALSE
			}
			/if (${Me.AltAbilityReady[Harvest of Druzzil]}) {
				/if (${validate_cast[FALSE, alt, "Harvest of Druzzil", ${Me.ID}]}) /call core_cast2 "Harvest of Druzzil" alt 0 FALSE
			}
		}
	}
/return



|***
 *	DES: get the group out.
 *	USE: /g gtfo
 *	INI: 
 *  NOTE: 

#event gtfo "#1# tells the group, 'gtfo'"
sub Event_gtfo(string line, string _sender)
	/if (${Select[${Me.Class.ShortName},WIZ]}) {
		CHECKSAFELIST
	  /delay 10s ${Me.AltAbilityReady[Exodus]}
	  /if (${Me.Invis}) /makemevisible
	  ECHOCHANNEL Fuck this place in particular..
	 	/if (${validate_cast[FALSE, alt, "Exodus", ${Me.ID}]}) /call core_cast2 "Exodus" alt 0 FALSE
	}
/return
 ***|

|***
 *	DES: emergency healing
 *	USE: /call patch_heal_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_WIZ()
	DEBUG patch_heal_WIZ()
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
	DEBUG check_rune_WIZ()
	AMIDEAD
	CHECKEXIT
	UPDATEHUD "BUFF: Runes"
	/call create_timer timer_check_runes ${tCheck_Runes}
	/if (${validate_cast[FALSE, spell, "${runeMagiWard}", ${Me.ID}]} && ${FindItemCount[Peridot]} > 1) /call core_cast2 "${runeMagiWard}" ${buffGem} 0 FALSE TRUE
	/if (${validate_cast[FALSE, spell, "${runeShieldofFate}", ${Me.ID}]}) /call core_cast2 "${runeShieldofFate}" ${buffGem} 0 FALSE TRUE
	/if (${validate_cast[FALSE, spell, "${runeCrystalwing}", ${Me.ID}]}) /call core_cast2 "${runeCrystalwing}" ${buffGem} 0 FALSE TRUE
	/if (${validate_cast[FALSE, spell, "${runeGuard}", ${Me.ID}]}) /call core_cast2 "${runeGuard}" ${buffGem} 0 FALSE TRUE
	UPDATEHUD FALSE
/return



|***
 *	DES: Wizard burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_WIZ()
	DEBUG synergy_WIZ()
	CHECKEXIT
	CHECKINPUTS
	
	| shhhhh..
	/if (${validate_cast[FALSE, alt, "Silent Casting", ${Me.ID}]}) /call core_cast2 "Silent Casting" alt 0 FALSE

	| next 45 spells +51% crit rate
	/if (${validate_cast[FALSE, alt, "Frenzied Devastation", ${Me.ID}]}) /call core_cast2 "Frenzied Devastation" alt 0 FALSE
	| increase chance to land
	/if (${validate_cast[FALSE, alt, "Focus of Arcanum", ${Me.ID}]}) /call core_cast2 "Focus of Arcanum" alt 0 FALSE


	| alternate twincast
	/if (!${Me.Buff[${TwincastSpell}].ID}) {
		/if (${validate_cast[FALSE, alt, "Improved Twincast", ${Me.ID}]}) /call core_cast2 "Improved Twincast" alt 0 FALSE
	} else /if (!${Me.Buff[Improved Twincast].ID} && !${Me.AltAbilityReady[Improved Twincast]} && ${Me.SpellReady[${TwincastSpell}]}) {
		/if (${validate_cast[FALSE, spell, "${TwincastSpell}", ${Me.ID}]}) /call core_cast2 "${TwincastSpell}" 0 0 FALSE
	}
	
	/call cast_AA_spire

	| boost spell type damage
	/if (${validate_cast[FALSE, alt, "Fury of the Gods", ${Me.ID}]}) /call core_cast2 "Fury of the Gods" alt 0 FALSE

	/if (${validate_cast[FALSE, alt, "Arcane Destruction", ${Me.ID}]}) /call core_cast2 "Arcane Destruction" alt 0 FALSE
	
	| increase damage
	/if (${validate_cast[FALSE, alt, "Arcane Fury", ${Me.ID}]}) /call core_cast2 "Arcane Fury" alt 0 FALSE
	
	| aww baby
	/if (${switchManaBurn}) {
		/if (${Target.ID} != ${Me.ID}) {
			/target ID ${Target2Kill}
		}
		/if (!${Target.Buff[Mana Burn].ID}) {
			/if (${validate_cast[FALSE, alt, "Mana Burn", ${Target2Kill}]}) /call core_cast2 "Mana Burn" alt ${Target2Kill} FALSE
		}
	}

	| swarm pet
	/if (${switchPcSwarm}) {
		/if (${validate_cast[FALSE, alt, "Call of Xuzl", ${Target2Kill}]}) /call core_cast2 "Call of Xuzl" alt ${Target2Kill} FALSE
	}

	| epic 2.0
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE

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
	DEBUG force_twincast_WIZ()
	/if (!${switchForceTwincast} || ${timer_force_twincast}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (!${timer_force_twincast}) /call create_timer timer_force_twincast ${tForce_Twincast}
	/if (!${cast_condition[NukeTwincastCondition]}) /return
	/if (!${Me.Buff[Improved Twincast].ID} && !${Me.Buff[Twincast].ID} && ${Me.SpellReady[${TwincastSpell}]}) {
		/if (${validate_cast[FALSE, spell, "${TwincastSpell}", ${Me.ID}]}) /call core_cast2 "${TwincastSpell}" 0 0 FALSE
		/return
	} else /if (!${Me.Buff[Improved Twincast].ID} && !${Me.Buff[${TwincastSpell}].ID} && !${Me.Buff[Twincast].ID}) {
		/if (${validate_cast[FALSE, spell, "${NukeTwincast${lineCurrent}}", ${Target2Kill}]}) /call core_cast2 "${NukeTwincast${lineCurrent}}" 0 ${Target2Kill} FALSE
	}
/return



|***
 *	DES: Make sure you are using the AA casting boost you want..
 *	USE: /call sort_mancy_lineAA
 *	INI: 
 *  NOTE:
 ***|
sub sort_mancy_lineAA()
	DEBUG sort_mancy_lineAA()
	/if (${lineCurrent.Equal[Fire]} && ${Me.Buff[${lineAAFire}].ID}) /return TRUE
	/if (${lineCurrent.Equal[Ice]} && ${Me.Buff[${lineAAIce}].ID}) /return TRUE
	/if (${lineCurrent.Equal[Magic]} && ${Me.Buff[${lineAAMagic}].ID}) /return TRUE
	/if (${lineCurrent.Equal[Mix]}&& ${Me.Buff[${lineAAMix}].ID}) /return TRUE
	/if (${validate_cast[FALSE, alt, "${lineAA${lineCurrent}}", ${Me.ID}]}) /call core_cast2 "${lineAA${lineCurrent}}" alt 0 FALSE	
/return



|***
 *	DES: Class controls
 *	USE: /wiz []
 *	INI: 
 *  NOTE:
 ***|
#bind setWIZ /wiz
sub Bind_setWIZ(string _classType, string _verbage)
	/if (!${validate_class[TRUE, |WIZ]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _type string local |fade|forcetwincast|gather|healpoint|line|manaburn|rod|spiregroup|spireraid
	
	| get help
	/if (${_classType.Equal[info]}) {
		/for _count 1 to ${_type.Count[|]}
			VOUT /${_bind} ${cinfo}${_type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${_type.Arg[${_count},|]}]}
		/next _count
		/return			
	} else /if (${Bool[${_type.Find[${_classType}]}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_classType.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_classType.Lower}]}
		/return
	}

	| gather enable/disable /pct
	/if (${Bool[${_classType.Find[gather]}]}) {
		/if (${Bool[${_classVerbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI GatherPercent Spells ${_verbage} 
		} else /if (${Bool[${_classVerbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
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
			/call writeINI lineCurrent Spells ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
			/if (${_verbage.Equal[off]}) {
				VOUT Casting line ${sep} ${odisabled}
				/return
			}
			/call sort_mancy_lineAA
			/delay 2
			
			/call mem_nuke_line ${lineCurrent}
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

	| use Modulation Rods
	} else /if (${Bool[${_classType.Equal[rod]}]}) {
		/call writeINI switchModRod Buffs ${If[${switchModRod},FALSE,TRUE]} 	

	| set AA fade use %	
	} else /if (${Bool[${_classType.Equal[fade]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI useAAFadePct Buffs ${_verbage}
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} fade(${cnum}${useAAFadePct}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		} else {
		 	/call writeINI switchAAFade Buffs ${If[${switchAAFade},FALSE,TRUE]}  
		}

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
	/if (${switchMQclearonCLS}) /mqclear
	/for _count 1 to ${_type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_type.Arg[${_count},|]}\ax
		/if (${_type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${_type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${_type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}${${Me.Class.ShortName}Point}\ax)
		/if (${_type.Arg[${_count},|].Equal[gather]}) /varset _listout ${_listout}(${cnum}${GatherPercent}\ax)
		/if (${_type.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}(${If[${Bool[${switchModRod}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[manaburn]}) /varset _listout ${_listout}(${If[${Bool[${switchManaBurn}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[forcetwincast]}) /varset _listout ${_listout}(${If[${Bool[${switchForceTwincast}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}(${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]})
		/if (${_type.Arg[${_count},|].Equal[line]}) /varset _listout ${_listout}(${cnum}${lineCurrent}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return


