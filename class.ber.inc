|***
 *	class.ber.inc
 *	Core Berzerker Include  
 *
 *	
 ***|



|***
 *	DES: Main Berzerker Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_BER()
	CHECKINPUTS
	AMIDEAD

	CHECKTIE
	
	:killtarget
	CHECKREZ
	/call prep_combat
	/if (${Macro.Return} && ${Bool[${combat}]}) {

		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${switchHealCure}) /call cast_cure

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${validate_cast[FALSE, disc, "${DiscSlapintheFace}", ${Target2Kill}]} && ${Me.PctAggro} <= 0 && !${Me.Combat}) /call core_cast2 "${DiscSlapintheFace}" disc ${Target2Kill}
			/if (${switchPcSwarm} || ${switchPcPet}) /call send_pet
			/if (${switchCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/if (${switchAlliance}) /call trigger_alliance ${Target2Kill}
			/call check_deBuffs ${Target2Kill}
			/call click_BP
			/call click_item
			/if (${switchPcSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
		}

		/if (${coreAuto}) {	
			/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
			/call cast_stamina_recovery
			/call check_ammo
			/if (!${is_target_dead[${Target2Kill}]}) /goto :killtarget
		} else /if (!${coreAuto}) {
			/if (${Target.ID} && ${Bool[${combat}]} && !${is_target_dead[${Target.ID}]}) /goto :killtarget
		}
	} 
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub disc_BER()
	AMIDEAD

	CHECKINPUTS
	/if (${validate_cast[FALSE, disc, "${DiscOverwhelmingFrenzy}", ${Target2Kill}]}) /call core_cast2 "${DiscOverwhelmingFrenzy}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscEnduranceTap}", ${Me.ID}]}) /call core_cast2 "${DiscEnduranceTap}" disc 0 FALSE
	/if (${Bool[${Me.Buff[Frenzied Resolve]}]} && ${Spawn[${Target2Kill}].Distance} > ${CombatDistance}) {
		/if (${validate_cast[FALSE, alt, "Drawn to Blood", ${Me.ID}]}) /call core_cast2 "Drawn to Blood" alt 0 FALSE
	}
	/if (${validate_cast[FALSE, alt, "Battle Leap", ${Me.ID}]} && !${Me.Underwater} && !${Me.Song[Battle Leap Warcry].ID}) /call core_cast2 "Battle Leap" alt 0 FALSE
	
	/call is_burn_time
	/if (!${Macro.Return}) {
		/if (${validate_cast[FALSE, disc, "${DiscDisconcert}", ${Me.ID}]}) /call core_cast2 "${DiscDisconcert}" disc 0 FALSE			
		/if (${validate_cast[FALSE, alt, "${RecklessAbandon}", ${Me.ID}]}) /call core_cast2 "${RecklessAbandon}" alt 0 FALSE
	}
		
	/if (${validate_cast[FALSE, alt, "Binding Axe", ${Me.ID}]}) /call core_cast2 "Binding Axe" alt 0 FALSE
	
	/if (${AEMelee}) {
		/if (${validate_cast[FALSE, alt, "Rampage", ${Me.ID}]} && ${Bool[${Me.Song[Strike of Savagery]}]}) /call core_cast2 "Rampage" alt 0 FALSE
	}
		
	/if (!${Me.Song[Reckless Abandon].ID}) {
		/if (${validate_cast[FALSE, alt, "Vehement Rage", ${Me.ID}]}) /call core_cast2 "Vehement Rage" alt 0 FALSE
	}

	| to agro or not ?
	/if (!${switchAgro}) {
		/if (${validate_cast[FALSE, alt, "Distraction Attack", ${Me.ID}]} && ${Me.PctAggro} > 96) /call core_cast2 "Distraction Attack" alt 0 FALSE
		/if (${validate_cast[FALSE, disc, "${DiscJolt}", ${Target2Kill}]} && ${Me.PctAggro} > 80) /call core_cast2 "${DiscJolt}" disc ${Target2Kill}
	}

	/if (${Me.PctHPs} < 90) {
		/if (${validate_cast[FALSE, disc, "${DiscCombatFrenzy}", ${Target2Kill}]}) /call core_cast2 "${DiscCombatFrenzy}" disc ${Target2Kill} FALSE
	}
		
	/if (${Me.PctHPs} > 75 && ${Me.PctEndurance} < 50) {
		/if (${validate_cast[FALSE, alt, "Communion of Blood", ${Me.ID}]}) /call core_cast2 "Communion of Blood" alt 0 FALSE
	}
	
	| this hits up to 8 targets and can cause fear.
	/if (${AEMelee}) {
		/if (${validate_cast[FALSE, alt, "War Cry of the Braxi", ${Me.ID}]}) /call core_cast2 "War Cry of the Braxi" alt 0 FALSE
	}

	| dumb ass zerker has a dicho and a dicho ##. cant use the name to cast with..
	/if (${Me.CombatAbilityReady[${DiscDicho}]} && ${Me.CurrentEndurance} > ${Spell[${DiscDicho}].EnduranceCost}*2) /disc 49307
	
	/if (${AEMelee}) {
		/if (${validate_cast[FALSE, disc, "Vicious Cyclone", ${Me.ID}]}) /call core_cast2 "Vicious Cyclone" disc 0 FALSE
	}

	/if (${Me.PctHPs} > 90 && !${Me.Song[${OpenWoundEffect}].ID}) {
		/if (${validate_cast[FALSE, alt, "Bloodfury", ${Me.ID}]}) /call core_cast2 "Bloodfury" alt 0 FALSE
	}
	
	/if (${validate_cast[FALSE, disc, "${DiscAugmentedFrenzy}", ${Me.ID}]} && !${Me.Buff[${AugmentedFrenzyEffect}].ID}) /call core_cast2 "${DiscAugmentedFrenzy}" disc 0 FALSE
	/if (${validate_cast[FALSE, disc, "${DiscCryHavoc}", ${Me.ID}]} && !${Me.Song[${DiscCryHavoc}].ID}) /call core_cast2 "${DiscCryHavoc}" disc 0 FALSE
	/if (${validate_cast[FALSE, disc, "${DiscFesteringRage}", ${Me.ID}]} && !${Me.Song[${DiscFesteringRage}].ID}) /call core_cast2 "${DiscFesteringRage}" disc 0 FALSE
	
	| swarm pet
	/if (${switchPcSwarm}) {
		/if (${validate_cast[FALSE, disc, "${DiscPet}", ${Target2Kill}]}) /call core_cast2 "${DiscPet}" disc ${Target2Kill} FALSE
	}

	| axes
	/if (${validate_cast[FALSE, disc, "${DiscVolley}", ${Target2Kill}]}) /call core_cast2 "${DiscVolley}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscAxeofRallos}", ${Target2Kill}]}) /call core_cast2 "${DiscAxeofRallos}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${ThrowingAxe}", ${Target2Kill}]}) /call core_cast2 "${ThrowingAxe}" disc ${Target2Kill} FALSE

	/if (!${Me.Song[${SharedBloodLustRecourse}].ID} && ${Bool[${Me.TargetOfTarget}]}) {
		/if (${validate_cast[FALSE, disc, "${SharedBloodLust}", ${Me.ID}]}) /call core_cast2 "${SharedBloodLust}" disc 0 FALSE
	}
/return



|***
 *	DES: Agro Routines
 * 	USE: manual/Named/raid. all sorts...
 *	INI: 
 *  NOTE: Zerker agro  control is built in to the routines. not here.
 ***|
sub get_agro_BER()
/return



|***
 *	DES: Patch Healing.  It's a zerker. use what we got...
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_BER()
	CHECKEXIT
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	
	/if (${validate_cast[FALSE, alt, "Uncanny Resilience", ${Me.ID}]} && ${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /call core_cast2 "Uncanny Resilience" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Blood Sustenance", ${Me.ID}]} && ${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /call core_cast2 "Blood Sustenance" alt 0 FALSE
	/call click_item_heal
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_BER()
	CHECKEXIT
	CHECKINPUTS
	/call click_horn_of_unity 	

	| epic 2.0
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]} && ${Spawn[${Target2Kill}].Distance} <= ${CombatDistance}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE
/return



 |***
 *	DES: Berzerker burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_BER()
	CHECKEXIT
	CHECKINPUTS
	
	/if (${Me.ActiveDisc.Name.Equal[${DiscDisconcert}]}) {
		:loopmakestop
		/delay 5
		/if (${Me.ActiveDisc.Name.Equal[${DiscDisconcert}]}) /stopdisc
		/delay 5
		/if (${Me.ActiveDisc.Name.Equal[${DiscDisconcert}]}) /goto :loopmakestop
	}
	
	/if (${validate_cast[FALSE, alt, "Desperation", ${Me.ID}]} && ${Me.PctHPs} < 90) /call core_cast2 "Desperation" alt 0 FALSE
	/if (${Me.CombatAbilityReady[${DiscScream}]} && ${Me.CurrentEndurance} > ${Spell[${DiscScream}].EnduranceCost}) /disc ${DiscScream}
	/if (${Me.CombatAbilityReady[${CryOfChaos}]} && !${Me.Song[${CryOfChaos}].ID}) {
		/delay ${Math.Rand[8]}s ${Me.Song[${CryOfChaos}].ID}
		/if (!${Me.Song[${CryOfChaos}].ID}) {
			/if (${Raid.Members}) {
				/if (${validate_cast[FALSE, alt, "Cry of Battle", ${Me.ID}]}) /call core_cast2 "Cry of Battle" alt 0 FALSE
			}
			/if (${Me.CombatAbilityReady[${CryOfChaos}]} && ${Me.CurrentEndurance} > ${Spell[${CryOfChaos}].EnduranceCost}) /disc ${CryOfChaos}
		}
	} 
	
	| AA rampage choice
	/if (${AEMelee} && ${SpawnCount[npc group radius 50 playerstate 4]} > 1) {
		/if (${validate_cast[FALSE, alt, "Furious Rampage", ${Me.ID}]}) /call core_cast2 "Furious Rampage" alt 0 FALSE
	} else /if (!${AEMelee}) {
		/if (${validate_cast[FALSE, alt, "Focused Furious Rampage", ${Me.ID}]}) /call core_cast2 "Focused Furious Rampage" alt 0 FALSE
	}

	/if (!${switchAgro}) {
		/if (${validate_cast[FALSE, alt, "Silent Strikes", ${Me.ID}]}) /call core_cast2 "Silent Strikes" alt 0 FALSE
	}
	
	/if (${validate_cast[FALSE, alt, "${UntamedgRage}", ${Me.ID}]}) /call core_cast2 "${UntamedgRage}" alt 0 FALSE

	/if (${validate_cast[FALSE, alt, "Blinding Fury", ${Me.ID}]}) /call core_cast2 "Blinding Fury" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Juggernaut Surge", ${Me.ID}]}) /call core_cast2 "Juggernaut Surge" alt 0 FALSE
	/call use_glyph
	
	/if (!${Me.Song[Reckless Abandon].ID} && ${Raid.Members}) {
		/if (${validate_cast[FALSE, alt, "Intensity of the Resolute", ${Me.ID}]}) /call core_cast2 "Intensity of the Resolute" alt 0 FALSE
	}
	
	/call cast_AA_spire
	/if (${validate_cast[FALSE, alt, "Blood Pact", ${Me.ID}]}) /call core_cast2 "Blood Pact" alt 0 FALSE

	/if (${Bool[${Spawn[group PC Shaman].ID}]}) {
	 	/if (!${Me.Buff[Savage Spirit].ID} && !${Bool[${Me.ActiveDisc}]}) {
	 		/if (${validate_cast[FALSE, disc, "${DiscBerserking}", ${Me.ID}]}) /call core_cast2 "${DiscBerserking}" disc 0 FALSE		
	 	}
		/if (!${Bool[${Me.ActiveDisc}]} && ${Me.AltAbilityReady[Savage Spirit]}) {
		 /alt act 465
		 /delay 5
		 /return
		}
	} else {
		/if (!${Bool[${Me.ActiveDisc}]} && ${Me.AltAbilityReady[Savage Spirit]}) {
		 	/alt act 465
		 	/delay 5
		 	/return
		}
		/if (!${Me.Buff[Savage Spirit].ID} && !${Bool[${Me.ActiveDisc}]}) {
			/if (${validate_cast[FALSE, disc, "${DiscBerserking}", ${Me.ID}]}) /call core_cast2 "${DiscBerserking}" disc 0 FALSE		
		}
	}
	
	/if (!${Me.Buff[Savage Spirit].ID} && !${Bool[${Me.ActiveDisc}]}) {

		/if (${Me.PctHPs} < 89) {
			/if (${validate_cast[FALSE, disc, "${DiscFrenzyResolve}", ${Me.ID}]}) /call core_cast2 "${DiscFrenzyResolve}" disc 0 FALSE
		}

		/if (!${Me.CombatAbilityReady[${DiscFrenzyResolve}]}) /disc ${DiscVengeful}
		/if (!${Me.CombatAbilityReady[${DiscFrenzyResolve}]} && !${Me.CombatAbilityReady[${DiscVengeful}]}) /disc ${DiscCleavingAnger}
	}

	/call synergy_click_Circle
	/call click_item_burn	
/return



|***
 *	DES: Class controls
 *	USE: /ber []
 *	INI: 
 *  NOTE:
 ***|
#bind setBER /ber
sub Bind_setBER(string _classType, string _verbage)
	/if (!${validate_class[TRUE, |BER]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _type string	local |ae|fade|healpoint|spiregroup|spireraid

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

	| AE or Focused Melee
	/if (${Bool[${_classType.Equal[ae]}]}) {
		/call writeINI AEMelee Discs ${If[${AEMelee},FALSE,TRUE]}
		VOUT /${_bind} ${cinfo}AE Melee\ax ${sep} ${If[${AEMelee},${oenabled},${odisabled}]}
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

	| healpoint
	} else /if (${Bool[${_classType.Find[healpoint]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI ${Me.Class.ShortName}Point Heal ${_verbage}
		} else /if (!${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} healpoint [${cinfo}${${Me.Class.ShortName}Point}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}

	}
	/for _count 1 to ${_type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_type.Arg[${_count},|]}\ax
		/if (${_type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${_type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${_type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}${${Me.Class.ShortName}Point}\ax)
		/if (${_type.Arg[${_count},|].Equal[ae]}) /varset _listout ${_listout}(${If[${AEMelee},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}(${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]})

		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return