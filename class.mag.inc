|***
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

	/if (${coreAuto} && !${Me.Invis}) {
		/if (${Bool[${useAAFade}]} && ${Bool[${AAFade}]} && ${Me.PctHPs} <= ${useAAFade}) /call cast_AA_fade
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
			/if (${switchCombatNuke}) {
				/if (${switchForceTwincast}) /call force_twincast_${Me.Class.ShortName}
				/call cast_nuke
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
			/if (!${Spawn[${Target2Kill}].ID} && !${is_target_dead[${Target2Kill}]}) /goto :killtarget
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
	/if (${Me.AltAbilityReady[Force of Elements]} && ${Spawn[${Target2Kill}].Distance} <= ${Spell[Force of Elements].MyRange}) /call core_cast "Force of Elements" alt ${Target2Kill} FALSE
	/if (${switchForceTwincast} && ${Me.SpellReady[${TwincastSpell}]} && !${Me.Buff[Improved Twincast].ID} && !${Me.Buff[Twincast].ID}) /call core_cast "${TwincastSpell}" 0 ${Target2Kill} FALSE
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

	/if (${Bool[${Cauldron}]} && ${Me.ItemReady[${Cauldron}]}) {
		/call core_cast "${Cauldron}" item 0 FALSE
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
	/if (${Me.ItemReady[${ParadoxRodName}]}) {
		/call MQ2Cast "${ParadoxRodName}" item -targetid|${Target2Kill}
		| /call core_cast "${ParadoxRodName}" item ${Target2Kill} FALSE
		/return
	}
	/if (${Me.ItemReady[${OrbName}]}) {
		/call MQ2Cast "${OrbName}" item -targetid|${Target2Kill}
		| /call core_cast "${OrbName}" item ${Target2Kill} FALSE
		/return
	}
	/if (${switchPcPet} && ${Me.ItemReady[${ElementRodName}]} && ${Target.PctHPs} > 20) {
		/call MQ2Cast "${ElementRodName}" item -targetid|${Target2Kill}
		| /call core_cast "${ElementRodName}" item ${Target2Kill} FALSE
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
	/if (${switchPcPet} && ${Spawn[${Target2Kill}].LineOfSight} && ${Me.SpellReady[${NukeSalvo}]} && ${Me.CurrentMana} >= ${Spell[${NukeSalvo}].Mana} && ${Spawn[${Target.ID}].NearestSpawn[3,pet radius 30]}) {
		/call core_cast "${NukeSalvo}" 0 ${Target2Kill} FALSE
	}
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
	/if (${Pet.PctHPs} < 40 && ${Me.AltAbilityReady[${PetAAHeal}]}) /call core_cast "${PetAAHeal}" alt ${Me.Pet.ID} FALSE
	/if (${Pet.PctHPs} < 60 && ${Me.SpellReady[${PetHealSpell}]}) /call core_cast "${PetHealSpell}" ${PetHealType} ${Me.Pet.ID} FALSE
	/if (${Pet.PctHPs} < 80 && ${Me.SpellReady[${PetRenewalSpell}]}) /call core_cast "${PetRenewalSpell}" ${PetRenewalType} ${Me.Pet.ID} FALSE
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
	/if (${Me.AltAbilityReady[Elemental Ward]}) /call core_cast "Elemental Ward" alt 0 FALSE
	
	| twincast
	/if (${Me.AltAbilityReady[Improved Twincast]} && !${Me.Buff[Twincast].ID}) {
		/call core_cast "Improved Twincast" alt 0 FALSE
	} else /if (${Me.SpellReady[Twincast]} && !${Me.Buff[Improved Twincast].ID}) {
		/call core_cast "Twincast" 0 0 FALSE
	}
	
	/if (${Me.AltAbilityReady[Focus of Arcanum]} && !${Bool[${Me.Buff[Focus of Arcanum]}]} && ${Target.PctHPs} > 20) /call core_cast "Focus of Arcanum" alt 0 FALSE
	
	| direct damage bonus
	/if (${Target.PctHPs} > 10) {
		/if (${Me.AltAbilityReady[Heart of Flames]} && !${Bool[${Me.Buff[Elemental Union]}]}) {
			/call core_cast "Heart of Flames" alt 0 FALSE
		} else /if (${Me.AltAbilityReady[Elemental Union]} && !${Bool[${Me.Buff[Heart of Flames]}]} && !${Me.AltAbilityReady[Heart of Flames]}) {
			/call core_cast "Elemental Union" alt 0 FALSE
		} else /if (!${Me.AltAbilityReady[Heart of Flames]} && !${Bool[${Me.Buff[Heart of Flames]}]} && !${Me.AltAbilityReady[Elemental Union]} && !${Bool[${Me.Buff[Elemental Union]}]}) {
			/call use_glyph
		}
	}

	/call cast_AA_spire
	/if (${Me.AltAbilityReady[${SilentCast}]}) /call core_cast "${SilentCast}" alt 0 FALSE
	/if (${Me.AltAbilityReady[Heart of Stone]}) /call core_cast "Heart of Stone" alt 0 FALSE

	| pet AAs
	/if (${switchPcPet}) {
		/if (${Me.AltAbilityReady[Servant of Ro]} && ${Spawn[${Target2Kill}].Distance} <= ${Spell[Servant of Ro].MyRange}) /call core_cast "Servant of Ro" alt ${Target2Kill} FALSE
		/if (${Me.AltAbilityReady[Companion's Fury]}) /call core_cast "Companion's Fury" alt ${Me.Pet.ID} FALSE
	}
	/if (${switchPcSwarm}) /if (${Me.AltAbilityReady[Host of the Elements]} && ${Spawn[${Target2Kill}].Distance} <= ${Spell[Host of the Elements].MyRange}) /call core_cast "Host of the Elements" alt ${Target2Kill} FALSE

	| epic 2.0
	/if (${Me.ItemReady[${${Me.Class.ShortName}Epic2}]} && ${Spell[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].Stacks} && !${Me.Buff[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID}) /call core_cast "${${Me.Class.ShortName}Epic2}" item ${Me.Pet.ID} FALSE



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
	/if (!${switchForceTwincast} || ${timer_force_twincast}) /return
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return 
	
	| SDEBUG pass checks
	
	/if (${Me.SpellReady[${NukeTwincast}]} && !${Me.Buff[Improved Twincast].ID} && !${Me.Buff[Twincast].ID}) {
		| /echo twincast nuke
		/call core_cast "${NukeTwincast}" 0 ${Target2Kill} FALSE
		/call create_timer timer_force_twincast ${tForce_Twincast}
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

	| get help
	/if (${_classType.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Arg[${_count},|]}]}
		/next _count
		/return			
	} else /if (${Bool[${${_bind}Type.Find[${_classType}]}]} && ${_verbage.Equal[info]}) {
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
			VOUT /${_bind} gather(${cnum}${GatherPercent}\ax) ${sep} ${If[${switchGather},${oenabled},${odisabled}]}
			/return
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
		VOUT /${_bind} forcetwincast ${sep} ${If[${switchForceTwincast},${oenabled},${odisabled}]}
		/return

	| get rods.. cause things and stuff
	} else /if (${Bool[${_classType.Equal[rods]}]}) {

		| self mod rod
		/if (${Bool[${SelfRodName}]}) {
			/if (!${FindItem[${SelfRodName}].ID}) {
				/if (!${Cursor.Name.Equal[${SelfRodName}]} && ${Me.CurrentMana} >= ${Spell[${SelfRodSpell}].Mana}) {
					/call core_cast "${SelfRodSpell}" ${buffGem} ${Me.ID} FALSE
					/call sort_cursor TRUE
				}
			}
		}
			
		
		| group mod rod
		/if (${Bool[${ModRodName}]} && !${FindItem[${ModRodName}].ID} && !${Cursor.Name.Equal[${ModRodName}]}) {
			| /target ID ${Me.ID}
			/call core_cast "${ModRodSpell}" alt 0 FALSE
			/call sort_cursor TRUE
		}
		| paradox rod	
		/if (${Bool[${ParadoxRodName}]} && !${Bool[${FindItem[${ParadoxRodName}]}]} && !${Bool[${Cursor.Name.Equal[${ParadoxRodName}]}]} && ${Me.CurrentMana} >= ${Spell[${ParadoxSpell}].Mana}) {
			/call core_cast "${ParadoxSpell}" ${buffGem} ${Me.ID} FALSE
			/call sort_cursor TRUE
		}
		| elemental orb
		/if (${Bool[${OrbName}]} && !${Bool[${FindItem[${OrbName}]}]} && !${Bool[${Cursor.Name.Equal[${OrbName}]}]} && ${Me.CurrentMana} >= ${Spell[${OrbSpell}].Mana}) {
			/call core_cast "${OrbSpell}" ${buffGem} 0 FALSE
			/call sort_cursor TRUE
		}
		| elemental rod
		/if (${Bool[${ElementRodName}]} && !${Bool[${FindItem[${ElementRodName}]}]} && !${Bool[${Cursor.Name.Equal[${ElementRodName}]}]} && ${Me.CurrentMana} >= ${Spell[${ElementRodSpell}].Mana}) {
			/call core_cast "${ElementRodSpell}" ${buffGem} 0 FALSE
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

	| get mana
	} else /if (${Bool[${_classType.Equal[mana]}]}) {

		/if (${switchGather} && ${Me.PctMana} < ${GatherPercent}*.${tmpHealOverride} && ${Bool[${SelfRodName}]} && ${Me.ItemReady[${SelfRodName}]}) /call core_cast "${SelfRodName}" item 0 FALSE
		/if (${switchGather} && ${Me.PctMana} < ${GatherPercent}*.${tmpHealOverride} && ${Bool[${ModRodName}]} && ${Me.ItemReady[${ModRodName}]}) /call core_cast "${ModRodName}" item 0 FALSE
		/if (${switchGather} && ${Me.PctMana} < ${GatherPercent}*.${tmpHealOverride} && ${Me.PctMana} <= 19 && ${Me.AltAbilityReady[Mana Reserve]}) /call core_cast "Mana Reserve" alt 0 FALSE
		
		:checkgather
	  /if (${switchGather} && ${Me.PctMana} < ${GatherPercent}*.${tmpHealOverride} && ${Bool[${GatherSpell}]} && ${Me.SpellReady[${GatherSpell}]}) /call core_cast "${GatherSpell}" 0 0 FALSE
	  
		/if (${Me.AltAbilityReady[Forceful Rejuvenation]} && ${Me.PctMana} < ${autoRest}*.${tmpHealOverride} && !${Me.SpellReady[${GatherSpell}]}) {
			/call core_cast "Forceful Rejuvenation" alt 0 FALSE
			/goto :checkgather
		}
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
		/if (${${_bind}Type.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}(${cnum}${useAAFade}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return





