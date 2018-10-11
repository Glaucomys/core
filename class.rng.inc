|***
 *	class.rng.inc
 *	Core Ranger Include
 *
 *
 ***|
 



|***
 *	DES: Main Ranger Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_RNG()

	/while (${prep_combat[]}) {
		/if (${exit_class_loop[]}) /return

		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${switchHealCure}) /call cast_cure

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${setBurnControl}) /call synergy_${Me.Class.ShortName}
			/if (${switchPcSwarm} || ${switchPcPet}) /call send_pet
			/if (${switchCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/call check_deBuffs ${Target2Kill}
			/if (${switchCombatDoT}) /call cast_DOT
			/if (${switchCombatNuke}) /call cast_nuke
			/if (${switchAlliance}) /call trigger_alliance ${Target2Kill}
			/if (${switchNukeSummer} && ${switchCombatNuke}) /call cast_nuke_summer ${Target2Kill}
			/call click_BP
			/call click_item
			/if (${switchPcSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
		}
	
		/if (${coreAuto}) {	
			/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
			/if (${switchModRod} && ${validate_class[FALSE, ${CastList}]}) /call use_mod_rod
			/if (${validate_class[FALSE, ${MeleeList}]}) /call cast_stamina_recovery
		}
	}
/return




|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: /call disc_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub disc_RNG()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	
	| agro control
	/if (${switchAgro}) {
		/if (${validate_cast[FALSE, disc, "${DiscEnrage}", ${Target2Kill}]}) /call core_cast2 "${DiscEnrage}" disc ${Target2Kill} FALSE
	} else /if (!${switchAgro}) {
		/if (${validate_cast[FALSE, disc, "${DiscJolt}", ${Target2Kill}]}) /call core_cast2 "${DiscJolt}" disc ${Target2Kill} FALSE
	}	
		
	| defensive	
	/if (${coreAuto} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]} > 1 && !${Bool[${Me.ActiveDisc}]}) {
		/if (${validate_cast[FALSE, disc, "${AADefensive1}", ${Me.ID}]} && ${Me.PctHPs} < 60*.${tmpHealOverride}) /call core_cast2 "${AADefensive1}" disc 0 FALSE
		/if (${validate_cast[FALSE, disc, "${AADefensive2}", ${Me.ID}]} && ${Me.PctHPs} < 40*.${tmpHealOverride}) /call core_cast2 "${AADefensive2}" disc 0 FALSE
		/if (${validate_cast[FALSE, disc, "${AADefensive3}", ${Me.ID}]} && ${Me.PctHPs} < 20*.${tmpHealOverride}) /call core_cast2 "${AADefensive3}" disc 0 FALSE

		| deflection
		/if (${validate_cast[FALSE, disc, "Weapon Shield Discipline", ${Me.ID}]} && ${Me.PctHPs} < 10*.${tmpHealOverride}) /call core_cast2 "Weapon Shield Discipline" disc 0 FALSE
	}

	| elemental arrows rotation
	/if (${Me.AltAbilityReady[Elemental Arrow]}) {
		/if (!${Bool[${Target.Buff[Elemental Arrow]}]} && !${Bool[${Target.Buff[Glacial Arrow]}]} && !${Bool[${Target.Buff[Volatile Arrow]}]}) {
			/if (${validate_cast[FALSE, alt, "Elemental Arrow", ${Target2Kill}]}) /call core_cast2 "Elemental Arrow" alt ${Target2Kill} FALSE
		}
	} else /if (${Me.AltAbilityReady[Glacial Arrow]}) {
		/if (!${Bool[${Target.Buff[Elemental Arrow]}]} && !${Bool[${Target.Buff[Glacial Arrow]}]}) {
			/if (${validate_cast[FALSE, alt, "Glacial Arrow", ${Target2Kill}]}) /call core_cast2 "Glacial Arrow" alt ${Target2Kill} FALSE
		}
	}	else /if (${Me.AltAbilityReady[Volatile Arrow]}) {
		/if (!${Bool[${Target.Buff[Elemental Arrow]}]} && !${Bool[${Target.Buff[Volatile Arrow]}]}) {
			/if (${validate_cast[FALSE, alt, "Volatile Arrow", ${Target2Kill}]}) /call core_cast2 "Volatile Arrow" alt ${Target2Kill} FALSE
		}
	}		

	| reverse DS DOT
	/if (${switchCombatDoT}) {
		/if (${validate_cast[FALSE, spell, "${DoTSwarmDS}", ${Target2Kill}]} && !${Bool[${Spawn[${Target.Buff[${DoTSwarmDSdeBuff}].Caster}].ID}]}) /call core_cast2 "${DoTSwarmDS}" 0 ${Target2Kill} FALSE
	}
	/if (${validate_cast[FALSE, disc, "${BladeStorm}", ${Target2Kill}]}) /call core_cast2 "${BladeStorm}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscSqual}", ${Target2Kill}]}) /call core_cast2 "${DiscSqual}" disc ${Target2Kill} FALSE
	
/return



|***
 *	DES: MGB ranger Auspice if both AAs are ready
 *	USE: 
 *	INI: 
 *  NOTE: Will not cast spice if the buff is present or if your name is Norbert.
 ***|
sub cast_AA_auspice()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[${RNGauspice}]} && !${Bool[${Me.Song[${RNGauspice}]}]}) {
		/if (${validate_cast[FALSE, alt, "Mass Group Buff", ${Me.ID}]}) /call core_cast2 "Mass Group Buff" alt 0 FALSE
		/if (${validate_cast[FALSE, alt, "${RNGauspice}", ${Me.ID}]}) /call core_cast2 "${RNGauspice}" alt 0 FALSE
		/if (${Bool[${ClassChannel}]}) /${ClassChannel} ${AuspiceTXT}
	 	/if (${Bool[${Raid.Members}]}) /rs ${AuspiceTXT}
	} else /if (${Bool[${Me.Song[${RNGauspice}]}]}) {
		VOUT ${cbad}QUIT\ax trying to cast ${csp}Auspice of the Hunter\ax while its running.	
	}		
/return



|***
 *	DES: emergency healing
 *	USE: /call patch_heal_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_RNG()
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	AMIDEAD
	CHECKEXIT
	| /if (${validate_cast[FALSE, spell, "${FastHeal}", ${Me.ID}]} && ${Me.PctHPs} <= ${FastHealPCT}*.${tmpHealOverride}) /call core_cast2 "${FastHeal}" 0 ${Me.ID} cast_break
	
	| heal 1
	/if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, spell, "${heal1}", ${Me.ID}]}) /call core_cast2 "${heal1}" 0 ${Me.ID}
	}
	
	| Reflexive Healing Disc
	/if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.${tmpHealOverride} || ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${Spell[${DiscHeal}].MyRange} playerstate 4 targetable]} >= 3) {
		/if (${validate_cast[FALSE, disc, "${DiscHeal}", ${Me.ID}]} && !${Bool[${Me.Song[Bladespur Reflection]}]}) /call core_cast2 "${DiscHeal}" disc ${Target.ID} FALSE
	}
	
	| heal 2
	/if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, spell, "${heal2}", ${Me.ID}]}) /call core_cast2 "${heal2}" 0 ${Me.ID}
	}
	
	/call click_item_heal

/return



|***
 *	DES: DPS Clickies
 *	USE: /call click_DPS_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_RNG()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/call use_skill_melee ${Target2Kill}
	/call click_horn_of_unity 	
	
	/if (!${Bool[${Me.ActiveDisc}]} && !${Bool[${Me.Song[Empowered Blades]}]} && !${Bool[${Me.Buff[${useGlyph}]}]}) {
		| epic 2.0
		/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]} && ${Spawn[${Target2Kill}].Distance} <= ${CombatDistance}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE
	}
	
/return



|***
 *	DES: Control for Spam Agro Nuke
 *	USE: /call cast_nuke_summer
 *	INI: 
 *  NOTE: When burn is engaged, the timer for this is ignored.
 ***|
sub cast_nuke_summer(int _tmpID)
	/if (${timer_cast_nuke_summer}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/call create_timer timer_cast_nuke_summer ${tNuke_Summer}
	/if (${validate_cast[FALSE, spell, "${NukeSummer}", ${_tmpID}]} && ${cast_condition[NukeSummerCondition]}) /call core_cast2 "${NukeSummer}" 0 ${_tmpID} cast_break
/return



 |***
 *	DES: Ranger burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_RNG()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	
	| MELEE
	/if (${combat.Equal[MELEE]}) {
		/if (!${Bool[${Me.ActiveDisc}]} && !${Bool[${Me.Buff[${useGlyph}]}]}) {
			/if (${validate_cast[FALSE, alt, "Empowered Blades", ${Me.ID}]}) /call core_cast2 "Empowered Blades" alt 0 FALSE
		} else /if (!${Bool[${Me.ActiveDisc}]} && !${Bool[${Me.Song[Empowered Blades]}]} && !${Bool[${Me.Buff[${useGlyph}]}]}) {
			/if (${validate_cast[FALSE, disc, "${DiscMele}", ${Me.ID}]}) /call core_cast2 "${DiscMele}" disc 0 FALSE
		} else /if (!${Bool[${Me.ActiveDisc}]} && !${Me.CombatAbilityReady[${DiscMele}]} && !${Me.AltAbilityReady[Empowered Blades]} && !${Bool[${Me.Song[Empowered Blades]}]}) {
			/call use_glyph
		} else /if (!${Bool[${Me.ActiveDisc}]} && !${Me.CombatAbilityReady[${DiscMele}]} && !${Me.AltAbilityReady[Empowered Blades]} && !${Bool[${Me.Song[Empowered Blades]}]} && !${Bool[${Me.Buff[${useGlyph}]}]}) {
			/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE
		}

	| RANGE
	} else /if (${combat.Equal[RANGE]}) {
		/if (!${Bool[${Me.ActiveDisc}]} && !${Bool[${Me.Buff[${useGlyph}]}]}) {
			/if (${validate_cast[FALSE, disc, "${DiscRange}", ${Me.ID}]}) /call core_cast2 "${DiscRange}" disc 0 FALSE
		} else /if (!${Bool[${Me.ActiveDisc}]} && !${Me.CombatAbilityReady[${DiscRange}]}) {
			/call use_glyph
		}
	}
	
	/call cast_AA_spire
	
	| proc
	/if (${validate_cast[FALSE, alt, "Imbued Ferocity", ${Me.ID}]} && !${Bool[${Me.Buff[${RNGspire1}]}]}) {
		/call core_cast2 "Imbued Ferocity" alt 0 FALSE
	}

	| cheeta
	/if (${validate_cast[FALSE, alt, "${RNGcheetah}", ${Me.ID}]}) /call core_cast2 "${RNGcheetah}" alt 0 FALSE
	
	| att buffs
	/if (${Me.AltAbilityReady[Outrider's Accuracy]} && !${Bool[${Me.Buff[Outrider's Attack]}]}) {
		/if (${validate_cast[FALSE, alt, "Outrider's Accuracy", ${Me.ID}]}) /call core_cast2 "Outrider's Accuracy" alt 0 FALSE
	} else /if (${Me.AltAbilityReady[Outrider's Attack]} && !${Bool[${Me.Song[Outrider's Accuracy]}]} && !${Me.AltAbilityReady[Outrider's Accuracy]} ) {
		/if (${validate_cast[FALSE, alt, "Outrider's Attack", ${Me.ID}]}) /call core_cast2 "Outrider's Attack" alt 0 FALSE
	}
	
	| guardian buff
	/if (!${Bool[${Me.Song[${RNGgroupguardian}]}]}) {  
		/if (${validate_cast[FALSE, alt, "Guardian of the Forest", ${Me.ID}]}) /call core_cast2 "Guardian of the Forest" alt 0 FALSE
	} else /if (!${Bool[${Me.Song[Guardian of the Forest]}]} && !${Me.AltAbilityReady[Guardian of the Forest]}) {
		/if (${validate_cast[FALSE, alt, "${RNGgroupguardian}", ${Me.ID}]}) /call core_cast2 "${RNGgroupguardian}" alt 0 FALSE
	}
		
	| swarm
	/if (${validate_cast[FALSE, alt, "Pack Hunt", ${Target2Kill}]} && ${switchPcSwarm}) /call core_cast2 "Pack Hunt" alt ${Target2Kill} FALSE   
	
	| ae regen 
	/if (${validate_cast[FALSE, disc, "${DiscHeal}", ${Target2Kill}]} !${Bool[${DiscHeal}]} && !${Bool[${Me.Song[Bladespur Reflection]}]}) /call core_cast2 "${DiscHeal}" disc ${Target2Kill} FALSE
			
	/call synergy_click_Circle
	/call click_item_burn
 /return
 	


|***
 *	DES: Class controls
 *	USE: /RNG []
 *	INI: 
 *  NOTE:
 ***|
#bind setRNG /rng
sub Bind_setRNG(string _classType, string _verbage)
	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _type string	local |fade|healpoint|rod|spice|spiregroup|spireraid|summer|taunt
		
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
	
	| enable/disable summer line of spells
	/if (${Bool[${_classType.Find[summer]}]}) {
		/call writeINI switchNukeSummer Spells ${If[${switchNukeSummer},FALSE,TRUE]}

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

	| use Modulation Rods
	} else /if (${Bool[${_classType.Equal[rod]}]}) {
		/call writeINI switchModRod Buffs ${If[${switchModRod},FALSE,TRUE]} 	

	| on/off taunt
	} else /if (${Bool[${_classType.Equal[taunt]}]}) {
		/call writeINI taunt meleedisc ${If[${taunt},FALSE,TRUE]}  
		
	| mgb spice
	} else /if (${Bool[${_classType.Find[spice]}]}) {
		/stopcast
		/delay 2s !${Me.Casting.ID}
		/call cast_AA_auspice
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
		/if (${_type.Arg[${_count},|].Equal[summer]}) /varset _listout ${_listout}(${If[${switchNukeSummer},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[taunt]}) /varset _listout ${_listout}(${If[${Bool[${taunt}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}(${If[${Bool[${switchModRod}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}(${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]})

		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return


