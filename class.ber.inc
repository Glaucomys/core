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
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}()\ax
	/doevents
	AMIDEAD

	CHECKTIE
	
	:killtarget
	CHECKREZ
	/call prep_combat
	/if (${Macro.Return} && ${Bool[${combat}]}) {
		DEBUG combat:${cinfo}${combat.Upper}\ax		

		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${switchHealCure}) /call cast_cure

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${validate_cast[disc, "${DiscSlapintheFace}", ${Target2Kill}]} && ${Me.PctAggro} <= 0 && !${Me.Combat}) /call core_cast2 "${DiscSlapintheFace}" disc ${Target2Kill}
			/if (${switchPcSwarm} || ${switchPcPet}) /call send_pet
			/if (${switchCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/if (${switchAlliance}) /call trigger_alliance ${Target2Kill}
			/call click_BP
			/call click_item
			/if (${switchPcSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
		}

		/if (${coreAuto}) {	
			/call cast_stamina_recovery
			/if (!${Spawn[${Target2Kill}].ID} && !${is_target_dead[${Target2Kill}]}) /goto :killtarget
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
	DEBUGCLASS ${cbug}disc_BER()\ax
	AMIDEAD

	/doevents
	/call core_cast "${DiscOverwhelmingFrenzy}" disc ${Target2Kill} FALSE
	/call core_cast "${DiscEnduranceTap}" disc 0 FALSE
	/if (${Bool[${Me.Buff[Frenzied Resolve]}]} && ${Spawn[${Target2Kill}].Distance} > ${CombatDistance}) /call core_cast "Drawn to Blood" alt 0 FALSE
	/if (!${Me.Underwater} && !${Me.Song[Battle Leap Warcry].ID}) /call core_cast "Battle Leap" alt 0 FALSE
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	
	/call is_burn_time
	/if (!${Macro.Return}) {
			/if (${Me.CombatAbilityReady[${DiscDisconcert}]}) /call core_cast "${DiscDisconcert}" disc 0 FALSE			
			/call core_cast "${RecklessAbandon}" alt 0 FALSE
	}
		
	/call core_cast "Binding Axe" alt 0 FALSE
	
	/if (${Bool[${Me.Song[Strike of Savagery]}]}) /call core_cast "Rampage" alt 0 FALSE
		
	/if (!${Me.Song[Reckless Abandon].ID}) /call core_cast "Vehement Rage" alt 0 FALSE

	| to agro or not ?
	/if (!${switchAgro} && ${Me.PctAggro} > 96) /call core_cast "Distraction Attack" alt 0 FALSE
	/if (!${switchAgro} && ${Me.PctAggro} > 80) /call core_cast "${DiscJolt}" disc ${Target2Kill}

	/if (${Me.PctHPs} < 90) /call core_cast "${DiscCombatFrenzy}" disc ${Target2Kill} FALSE
		
	/if (${Me.PctHPs} > 75 && ${Me.PctEndurance} < 50) /call core_cast "Communion of Blood" alt 0 FALSE
	
	| this hits up to 8 targets and can cause fear.
	/if (${AEMelee}) /call core_cast "War Cry of the Braxi" alt 0 FALSE

	| dumb ass zerker has a dicho and a dicho ##. cant use the name to cast with..
	/if (${Me.CombatAbilityReady[${DiscDicho}]} && ${Me.CurrentEndurance} > ${Spell[${DiscDicho}].EnduranceCost}*2) /disc 49307
	| /if (${Me.CurrentEndurance} > ${Spell[${DiscDicho}].EnduranceCost}*2)  /call core_cast "${DiscDicho} 6" disc 0 FALSE
	
	/if (${AEMelee}) /call core_cast "Vicious Cyclone" disc 0 FALSE

	| /if (${Me.CombatAbilityReady[${DiscOpenWound}]} && !${Me.Song[${OpenWoundEffect}].ID} && ${Me.CurrentEndurance} > ${Spell[${DiscOpenWound}].EnduranceCost})  /call core_cast "${DiscOpenWound}" disc 0
	
	/if (${Me.PctHPs} > 90 && !${Me.Song[${OpenWoundEffect}].ID}) /call core_cast "Bloodfury" alt 0 FALSE
	
	/if (!${Me.Buff[${AugmentedFrenzyEffect}].ID}) /call core_cast "${DiscAugmentedFrenzy}" disc 0 FALSE
	/if (!${Me.Song[${DiscCryHavoc}].ID}) /call core_cast "${DiscCryHavoc}" disc 0 FALSE
	/if (!${Me.Song[${DiscFesteringRage}].ID}) /call core_cast "${DiscFesteringRage}" disc 0 FALSE
	
	| swarm pet
	/if (${switchPcSwarm}) /call core_cast "${DiscPet}" disc ${Target2Kill} FALSE

	| axes
	/call core_cast "${DiscVolley}" disc ${Target2Kill} FALSE
	/call core_cast "${DiscAxeofRallos}" disc ${Target2Kill} FALSE
	/call core_cast "${ThrowingAxe}" disc ${Target2Kill} FALSE

	/if (!${Me.Song[${SharedBloodLustRecourse}].ID} && ${Bool[${Me.TargetOfTarget}]}) /call core_cast "${SharedBloodLust}" disc 0 FALSE
	/call check_deBuffs ${Target2Kill}
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
	DEBUGCLASS ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	CHECKEXIT
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	
	/if (${validate_cast[alt, "Uncanny Resilience", ${Me.ID}]} && ${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /call core_cast2 "Uncanny Resilience" alt 0 FALSE
	/if (${validate_cast[alt, "Blood Sustenance", ${Me.ID}]} && ${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /call core_cast2 "Blood Sustenance" alt 0 FALSE
	/call click_item_heal
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_BER()
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	CHECKEXIT
	/doevents
	/call click_horn_of_unity 	

	| epic 2.0
	/if (${Me.ItemReady[${${Me.Class.ShortName}Epic2}]} && ${Spell[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].Stacks} && !${Me.Buff[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID} && !${Me.Song[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID}) /call core_cast "${${Me.Class.ShortName}Epic2}" item 0 FALSE
/return



 |***
 *	DES: Berzerker burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 

sub synergy_BER()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	CHECKEXIT
	/doevents
	
	/if (${Me.ActiveDisc.Name.Equal[${DiscDisconcert}]}) {
		:loopmakestop
		/delay 5
		/if (${Me.ActiveDisc.Name.Equal[${DiscDisconcert}]}) /stopdisc
		/delay 5
		/if (${Me.ActiveDisc.Name.Equal[${DiscDisconcert}]}) /goto :loopmakestop
	}
	
	/if (${validate_cast[alt, "Desperation", ${Me.ID}]} && ${Me.PctHPs} < 90) /call core_cast2 "Desperation" alt 0 FALSE
	/if (${Me.CombatAbilityReady[${DiscScream}]} && ${Me.CurrentEndurance} > ${Spell[${DiscScream}].EnduranceCost}) /disc ${DiscScream}
	/if (${Me.CombatAbilityReady[${CryOfChaos}]} && !${Me.Song[${CryOfChaos}].ID}) {
		/delay ${Math.Rand[8]}s ${Me.Song[${CryOfChaos}].ID}
		/if (!${Me.Song[${CryOfChaos}].ID}) {
			/if (${Raid.Members}) /call core_cast "Cry of Battle" alt 0 FALSE
			/if (${Me.CombatAbilityReady[${CryOfChaos}]} && ${Me.CurrentEndurance} > ${Spell[${CryOfChaos}].EnduranceCost}) /disc ${CryOfChaos}
		}
	} 
	
	| AA rampage choice
	/if (${AEMelee} && ${SpawnCount[npc group radius 50 playerstate 4]} > 1) {
		/call core_cast "Furious Rampage" alt 0 FALSE
	} else /if (!${AEMelee}) {
		/call core_cast "Focused Furious Rampage" alt 0 FALSE
	}

	/if (!${switchAgro}) /call core_cast "Silent Strikes" alt 0 FALSE
	/call core_cast "${UntamedgRage}" alt 0 FALSE

	/call core_cast "Blinding Fury" alt 0 FALSE
	/call core_cast "Juggernaut Surge" alt 0 FALSE
	/call use_glyph
	/if (!${Me.Song[Reckless Abandon].ID} && ${Raid.Members}) /call core_cast "Intensity of the Resolute" alt 0 FALSE
	/call cast_AA_spire
	/call core_cast "Blood Pact" alt 0 FALSE

	/if (${Bool[${Spawn[group PC Shaman].ID}]}) {
	 	/if (!${Me.Buff[Savage Spirit].ID} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscBerserking}" disc 0 FALSE		
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
		/if (!${Me.Buff[Savage Spirit].ID} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscBerserking}" disc 0 FALSE		
	}
	
	/if (!${Me.Buff[Savage Spirit].ID} && !${Bool[${Me.ActiveDisc}]}) {
		/if (${Me.PctHPs} < 89) /call core_cast "${DiscFrenzyResolve}" disc 0 FALSE
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
	DEBUG ${cbug}Bind_setBER(type:${_classType}, verbage:${_verbage})\ax
	/if (!${validate_class[TRUE, |BER]}) /return

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

	| healpoint
	} else /if (${Bool[${_classType.Find[healpoint]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI ${Me.Class.ShortName}Point Heal ${_verbage}
		} else /if (!${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} healpoint [${cinfo}${${Me.Class.ShortName}Point}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}

	}
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
		/if (${${_bind}Type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}${${Me.Class.ShortName}Point}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[ae]}) /varset _listout ${_listout}(${If[${AEMelee},${oon},${ooff}]})
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return