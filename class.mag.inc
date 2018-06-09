3|***
 *	class.mag.inc
 *	Core Magician Include
 *
 * 
 ***| 



|***
 *	DES: Main Magician Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_MAG()
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}()\ax
	/doevents
	AMIDEAD

	CHECKTIE

	/if (${coreAuto}) {
		/if (${Bool[${useAAFade}]} && ${Bool[${AAFade}]} && ${Me.PctHPs} <= ${useAAFade}) /call cast_AA_fade
		/if (${switchGather}) {
			/if (${Me.PctMana} <= ${GatherPercent}*.${tmpHealOverride}) /call check_gather
		}
		/call check_engaged
		/if (!${Bool[${Macro.Return}]}) {
			/if (${switchPcPet}) /call check_pet
			CHECKEXIT
			/if (!${timer_caldron_use}) /call use_cauldron
		}
	}

	:killtarget
	CHECKREZ
	/call prep_combat
	/if (${Macro.Return} && ${Bool[${combat}]}) {
		DEBUG combat:${cinfo}${combat.Upper}\ax		

		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${switchHealPet}) /call pet_heal_${Me.Class.ShortName}
		/if (${switchHealCure}) /call cast_cure

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${switchPcSwarm} || ${switchPcPet}) /call send_pet
			/if (${switchPcSwarm}) /call cast_nuke_servant
			/if (${switchCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/if (${switchAlliance}) /call trigger_alliance ${Target2Kill}
			/call check_deBuffs ${Target2Kill}
			/if (${switchCombatNuke}) {
				/if (${switchForceTwincast}) /call force_twincast_${Me.Class.ShortName}
				/if (${switchPcPet}) /call cast_nuke_salvo
				/call cast_nuke_line
				| /if (${useRain}) /call cast_nuke_rain
			}
			/call click_BP
			/call click_item
			/if (${switchPcSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
		}

		/if (${coreAuto}) {	
			/if (${validate_class[FALSE, ${CastList}]}) /call use_mod_rod
			/if (${validate_class[FALSE, ${MeleeList}]}) /call cast_stamina_recovery
			/if (!${is_target_dead[${Target2Kill}]}) /goto :killtarget
		} else /if (!${coreAuto}) {
			/if (${Target.ID} && ${Bool[${combat}]} && !${is_target_dead[${Target.ID}]}) /goto :killtarget
		}
	}
/return



|***
 *	DES: Discs
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub disc_MAG()
	DEBUGCLASS ${cbug}disc_${Me.Class.ShortName}()\ax 
	CHECKEXIT
	CHECKTIE
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	/if (${validate_cast[FALSE, alt,"Force of Elements", ${Target2Kill}]}) /call core_cast2 "Force of Elements" alt ${Target2Kill} FALSE
/return



|***
 *	DES: summon cauldron item
 *	USE: auto on timer
 *	INI: 
 *  NOTE: 
 ***|
sub use_cauldron()
	DEBUGCLASS ${cbug}use_cauldron()\ax 
	CHECKEXIT
	CHECKTIE
	/call create_timer timer_caldron_use ${tCauldron_use}

	/if (${Bool[${Cauldron}]}) {
		/if (${validate_cast[FALSE, item, "${Cauldron}", ${Me.ID}]}) /call core_cast2 "${Cauldron}" item 0 FALSE
		/call sort_cursor TRUE
	}
/return



|***
 *	DES: click dps thingies on bad guys
 *	USE: 
 *	INI: 
 *  NOTE: this is filler DPS; compensate for global cooldown
 ***|
sub click_DPS_MAG()
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	AMIDEAD
	CHECKEXIT
	/doevents
	/call click_horn_of_unity 	
	
	| click summon rods. DPS fillers....
	/if (${validate_cast[FALSE, item, "${ParadoxRodName}", ${Target2Kill}]}) /call core_cast2 "${ParadoxRodName}" item ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, item, "${OrbName}", ${Target2Kill}]}) /call core_cast2 "${OrbName}" item ${Target2Kill} FALSE

	/if (${switchPcSwarm}) {
		/if (${validate_cast[FALSE, item, "${ElementRodName}", ${Target2Kill}]}) /call core_cast2 "${ElementRodName}" item ${Target2Kill} FALSE
		/return
	}
/return



|***
 *	DES: Pet nuke
 *	USE: auto when 3+ pets in range
 *	INI: 
 *  NOTE: 
 ***|
sub cast_nuke_salvo()
	DEBUGCLASS ${cbug}cast_nuke_salvo()\ax
	CHECKEXIT
	CHECKTIE
	/if (${SpawnCount[pet radius ${MobAgro}]} < 3) /return
	| /if (${validate_cast[FALSE, spell, "${NukeSalvo}", ${Target2Kill}]} && ${switchPcPet} && ${Spawn[${Target2Kill}].NearestSpawn[3,${Me.Pet.ID} radius 40]}) {
	/if (${validate_cast[FALSE, spell, "${NukeSalvo}", ${Target2Kill}]}) /call core_cast2 "${NukeSalvo}" 0 ${Target2Kill} FALSE
/return



|***
 *	DES: Pet heal routines
 *	USE: /call petHeal_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub pet_heal_MAG()
	DEBUGCLASS ${cbug}pet_heal_${Me.Class.ShortName}()${citem}
	CHECKEXIT
	CHECKTIE
	/if (${validate_cast[FALSE, alt, "${PetAAHeal}", ${Me.Pet.ID}]} && ${Pet.PctHPs} < 40) /call core_cast2 "${PetAAHeal}" alt ${Me.Pet.ID} FALSE
	/if (${validate_cast[FALSE, spell, "${PetHealSpell}", ${Me.Pet.ID}]} && ${Pet.PctHPs} < 60) /call core_cast2 "${PetHealSpell}" ${PetHealType} ${Me.Pet.ID} FALSE
	/if (${validate_cast[FALSE, spell, "${PetRenewalSpell}", ${Me.Pet.ID}]} && ${Pet.PctHPs} < 80) /call core_cast2 "${PetRenewalSpell}" ${PetRenewalType} ${Me.Pet.ID} FALSE
/return



|***
 *	DES: emergency healing
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_MAG()
	DEBUGCLASS ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	CHECKEXIT
	CHECKTIE
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	/call click_item_heal
/return



 |***
 *	DES: Magician burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_MAG()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	CHECKEXIT
	CHECKTIE
	/if (${validate_cast[FALSE, alt, "Elemental Ward", ${Me.ID}]}) /call core_cast2 "Elemental Ward" alt 0 FALSE
	
	/if (${Target.PctHPs} > 20) {
		/if (${validate_cast[FALSE, alt, "Focus of Arcanum", ${Me.ID}]}) /call core_cast2 "Focus of Arcanum" alt 0 FALSE
	}
	
	| direct damage bonus
	/if (${Target.PctHPs} > 10) {
		/if (${Bool[${Me.Buff[Elemental Union]}]}) {
			/if (${validate_cast[FALSE, alt, "Heart of Flames", ${Me.ID}]}) /call core_cast2 "Heart of Flames" alt 0 FALSE
		} else /if (!${Bool[${Me.Buff[Heart of Flames]}]} && !${Me.AltAbilityReady[Heart of Flames]}) {
			/if (${validate_cast[FALSE, alt, "Elemental Union", ${Me.ID}]}) /call core_cast2 "Elemental Union" alt 0 FALSE
		} else /if (!${Me.AltAbilityReady[Heart of Flames]} && !${Bool[${Me.Buff[Heart of Flames]}]} && !${Me.AltAbilityReady[Elemental Union]} && !${Bool[${Me.Buff[Elemental Union]}]}) {
			/call use_glyph
		}
	}

	/call cast_AA_spire
	/if (${validate_cast[FALSE, alt, "${SilentCast}", ${Me.ID}]}) /call core_cast2 "${SilentCast}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Heart of Stone", ${Me.ID}]}) /call core_cast2 "Heart of Stone" alt 0 FALSE

	| pet AAs
	/if (${switchPcPet}) {
		/if (${validate_cast[FALSE, alt, "Servant of Ro", ${Target2Kill}]}) /call core_cast2 "Servant of Ro" alt ${Target2Kill} FALSE
		/if (${validate_cast[FALSE, alt, "Companion's Fury", ${Me.Pet.ID}]}) /call core_cast2 "Companion's Fury" alt ${Me.Pet.ID} FALSE
	}
	/if (${switchPcSwarm}) {
		/if (${validate_cast[FALSE, alt, "Host of the Elements", ${Target2Kill}]}) /call core_cast2 "Host of the Elements" alt ${Target2Kill} FALSE
	}

	| epic 2.0
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item ${Me.Pet.ID} FALSE

	/call synergy_click_Circle
	/call click_item_burn
/return



|***
 *  DES: Control for getting Twincast either spell or Proc
 *  USE: /call force_twincast_MAG
 *  INI: 
 *  NOTE: Twincast lets get it.
 ***|
sub force_twincast_MAG()
	DEBUG ${cbug}force_twincast_MAG()\ax 
|**
	/if (!${switchForceTwincast} || ${timer_force_twincast}) /return
	/if (${is_target_dead[${Target2Kill}]}) /return 
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (!${timer_force_twincast}) /call create_timer timer_force_twincast ${tForce_Twincast}

	/if (${Me.Buff[Improved Twincast].ID} || ${Me.Buff[${TwincastSpell}].ID} || ${Me.Buff[Twincast].ID}) /return
	
	| twincast spell
	/if (${Me.SpellReady[${TwincastSpell}]}) {
		/if (${validate_cast[FALSE, spell, "${TwincastSpell}", ${Me.ID}]}) /call core_cast2 "${TwincastSpell}" 0 0 FALSE

	| twincast AA
	} else /if (${Me.AltAbility[Improved Twincast]}) {
		/if (${validate_cast[FALSE, alt, "Improved Twincast", ${Me.ID}]}) /call core_cast2 "Improved Twincast" alt 0 FALSE
		
	| nuke twincast
	} else {
		/if (!${cast_condition[NukeTwincastCondition]}) /return
		/if (${validate_cast[FALSE, spell, "${NukeTwincast${lineCurrent}}", ${Target2Kill}]}) /call core_cast2 "${NukeTwincast${lineCurrent}}" 0 ${Target2Kill} FALSE
	}
	
	**|
	
	
	
	/if (!${switchForceTwincast} || ${timer_force_twincast}) /return
	/if (${is_target_dead[${Target2Kill}]}) /return
	/return	AMIDEAD
	CHECKEXIT
	/doevents
	/if (!${timer_force_twincast}) /call create_timer timer_force_twincast ${tForce_Twincast}
	/if (!${Me.Buff[Improved Twincast].ID} && !${Me.Buff[Twincast].ID} && ${Me.SpellReady[${TwincastSpell}]}) {
		/if (${validate_cast[FALSE, spell, "${TwincastSpell}", ${Me.ID}]}) /call core_cast2 "${TwincastSpell}" 0 0 FALSE
		/return
	} else /if (!${Me.Buff[Improved Twincast].ID} && !${Me.Buff[${TwincastSpell}].ID} && !${Me.Buff[Twincast].ID}) {
		/if (!${cast_condition[NukeTwincastCondition]}) /return
		/if (${validate_cast[FALSE, spell, "${NukeTwincast${lineCurrent}}", ${Target2Kill}]}) /call core_cast2 "${NukeTwincast${lineCurrent}}" 0 ${Target2Kill} FALSE
	}	
	
	
/return



|***
 *	DES: lemme heare everyone say ... hey .. we want some ..... umm manna..
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub check_gather()
	DEBUGCLASS ${cbug}check_gather()\ax
	CHECKEXIT
	/doevents
	CHECKTIE

	/if (${Me.Buff[Improved Twincast].ID} || ${Me.Buff[${TwincastSpell}].ID} || ${Me.Buff[Twincast].ID}) /return

	| pet consumption for manna		
	/if (${Me.Pet.ID}) {
		/if (${Me.PctMana} <= 19 && ${validate_cast[FALSE, alt, "Elemental Conversion", ${Me.ID}]}) /call core_cast2 "Elemental Conversion" alt 0 FALSE
	}
	
	| rods
	/if (${Bool[${SelfRodName}]}) {
		/if (${validate_cast[FALSE, item, "${SelfRodName}", ${Me.ID}]}) /call core_cast2 "${SelfRodName}" item 0 FALSE
	}
	/if (${Bool[${ModRodName}]}) {
		/if (${validate_cast[FALSE, item, "${ModRodName}", ${Me.ID}]}) /call core_cast2 "${ModRodName}" item 0 FALSE
	}
	
	| stop here to save time is nothing else is available
	/if (!${Me.SpellReady[${GatherSpell}]} && !${Me.AltAbilityReady[Forceful Rejuvenation]}) /return
		
	| force rejuv if we can proc gather spell again
	/if (!${Me.SpellReady[${GatherSpell}]}) {
		/if (${validate_cast[FALSE, alt, "Forceful Rejuvenation", ${Me.ID}]}) /call core_cast2 "Forceful Rejuvenation" alt 0 FALSE
		/delay 1s
	}

	/if (${Me.SpellReady[${GatherSpell}]}) {
		/if (${validate_cast[FALSE, spell, "${GatherSpell}", ${Me.ID}]}) /call core_cast2 "${GatherSpell}" 0 0 FALSE
	}

/return


		
|***
 *	DES: Class controls
 *	USE: /mag []
 *	INI: 
 *  NOTE:
 ***|
#bind setMAG /mag
sub Bind_setMAG(string _classType, string _verbage)
	DEBUG ${cbug}Bind_setMAG(type:${_classType}, verbage:${_verbage})\ax
	/if (!${validate_class[TRUE, |MAG]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _type string	local |fade|forcetwincast|gather|healpoint|line|rods|spiregroup|spireraid

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
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI GatherPercent Spells ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} gather(${cnum}${GatherPercent}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
		}	else {
			/call writeINI switchGather Spells ${If[${switchGather},FALSE,TRUE]}  
		}			
		
	| set AA fade use %	
	} else /if (${Bool[${_classType.Equal[fade]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI useAAFade Buffs ${_verbage}
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} fade(${cnum}${useAAFade}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}		
	
	| enable/disable force twincasting
	} else /if (${Bool[${_classType.Equal[forcetwincast]}]}) {
		/call writeINI switchForceTwincast Spells ${If[${switchForceTwincast},FALSE,TRUE]}


	| get rods.. cause things and stuff
	} else /if (${Bool[${_classType.Equal[rods]}]}) {

		| self mod rod
		/if (${Bool[${SelfRodName}]}) {
			/if (!${FindItem[${SelfRodName}].ID}) {
				/if (${validate_cast[FALSE, spell, "${SelfRodSpell}", ${Me.ID}]}) /call core_cast2 "${SelfRodSpell}" ${buffGem} ${Me.ID} FALSE
				/call sort_cursor TRUE
			}
		}
			
		
		| group mod rod
		/if (${Bool[${ModRodName}]} && !${FindItem[${ModRodName}].ID} && !${Cursor.Name.Equal[${ModRodName}]}) {
			/if (${validate_cast[FALSE, spell, "${ModRodSpell}", ${Me.ID}]}) /call core_cast2 "${ModRodSpell}" alt 0 FALSE
			/call sort_cursor TRUE
		}
		| paradox rod	
		/if (${Bool[${ParadoxRodName}]} && !${Bool[${FindItem[${ParadoxRodName}]}]} && !${Bool[${Cursor.Name.Equal[${ParadoxRodName}]}]} && ${Me.CurrentMana} >= ${Spell[${ParadoxSpell}].Mana}) {
			/if (${validate_cast[FALSE, spell, "${ParadoxSpell}", ${Me.ID}]}) /call core_cast2 "${ParadoxSpell}" ${buffGem} ${Me.ID} FALSE
			/call sort_cursor TRUE
		}
		| elemental orb
		/if (${Bool[${OrbName}]} && !${Bool[${FindItem[${OrbName}]}]} && !${Bool[${Cursor.Name.Equal[${OrbName}]}]} && ${Me.CurrentMana} >= ${Spell[${OrbSpell}].Mana}) {
			/if (${validate_cast[FALSE, spell, "${OrbSpell}", ${Me.ID}]}) /call core_cast2 "${OrbSpell}" ${buffGem} 0 FALSE
			/call sort_cursor TRUE
		}
		| elemental rod
		/if (${Bool[${ElementRodName}]} && !${Bool[${FindItem[${ElementRodName}]}]} && !${Bool[${Cursor.Name.Equal[${ElementRodName}]}]} && ${Me.CurrentMana} >= ${Spell[${ElementRodSpell}].Mana}) {
			/if (${validate_cast[FALSE, spell, "${ElementRodSpell}", ${Me.ID}]}) /call core_cast2 "${ElementRodSpell}" ${buffGem} 0 FALSE
			/call sort_cursor TRUE
		}
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
		VOUT /${Me.Class.ShortName.Lower} ${_classType.Lower} [${_listout}]	
		/return


	| healpoint
	} else /if (${Bool[${_classType.Find[healpoint]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI ${Me.Class.ShortName}Point Heal ${_verbage}
		} else /if (!${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} healpoint(${cnum}${${Me.Class.ShortName}Point}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}
		
	| spell line. not quite done with this. 
	} else /if (${Bool[${_classType.Equal[line]}]}) {
		| mem a line of spells.
		/if (${Bool[${lineType.Find[${_verbage.Lower}]}]}) {
			DEBUG ${lineType.Find[${_verbage.Lower}]}
			/call writeINI lineCurrent Spells ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
			/if (${_verbage.Equal[off]}) {
				VOUT Casting line ${sep} ${odisabled}
				/return
			}
			
			/call mem_nuke_line ${lineCurrent}
			/return
		}
		/for _count 1 to ${lineType.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${lineCurrent.Equal[${lineType.Arg[${_count},|]}]},${cgood},${cinfo}]}${lineType.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} line [${_listout}]	
		/return		
		
		
		

	}
	/if (${switchMQclearonCLS}) /mqclear
	/for _count 1 to ${_type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_type.Arg[${_count},|]}\ax
		/if (${_type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${_type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${_type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}${${Me.Class.ShortName}Point}\ax)
		/if (${_type.Arg[${_count},|].Equal[gather]}) /varset _listout ${_listout}(${cnum}${If[${switchGather},${cnum}${GatherPercent}\ax,${ooff}]}\ax)
		/if (${_type.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}(${cnum}${useAAFade}\ax)
		/if (${_type.Arg[${_count},|].Equal[forcetwincast]}) /varset _listout ${_listout}(${If[${switchForceTwincast},${oon},${ooff}]})


		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return



