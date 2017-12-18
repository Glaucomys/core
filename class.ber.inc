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
	/call prep_combat
	/if (!${Macro.Return}) /return
	
	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call stick_to_target
		/if (${Me.CombatAbilityReady[${DiscSlapintheFace}]} && ${Me.PctAggro} <= 0 && !${Me.Combat} && ${Me.CurrentEndurance} > ${Spell[${DiscSlapintheFace}].EnduranceCost}) /call core_cast "${DiscSlapintheFace}" disc ${Target2Kill}
		/call set_combat MELEE
		/call stick_to_target
		/call use_skill_melee ${Target2Kill}
		/call disc_${Me.Class.ShortName}
		/call click_DPS_${Me.Class.ShortName}		
		/call click_BP
		/call stick_to_target
		/call use_skill_melee ${Target2Kill}
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/call click_item
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
		/if (${Me.CombatAbilityReady[${DiscSlapintheFace}]} && ${Me.PctAggro} <= 0 && !${Me.Combat} && ${Me.CurrentEndurance} > ${Spell[${DiscSlapintheFace}].EnduranceCost}) /call core_cast "${DiscSlapintheFace}" disc ${Target2Kill}
		/call set_combat RANGE
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/call use_skill_melee ${Target2Kill}
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/call click_item
		/call click_DPS_${Me.Class.ShortName}		
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
	}
	/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
	| /if (${coreAuto} && !${Spawn[${Target2Kill}].ID} && !${is_target_dead[${Target2Kill}]}) /goto :killtarget
	/if (${coreAuto} && !${is_target_dead[${Target2Kill}]}) /goto :killtarget
	/if (!${coreAuto} && ${Target.ID} && ${Bool[${combat}]} && !${is_target_dead[${Target.ID}]}) /goto :killtarget
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
	
	/if (${AEMelee}) /call core_cast "Arcslice" disc 0 FALSE

	| /if (${Me.CombatAbilityReady[${DiscOpenWound}]} && !${Me.Song[${OpenWoundEffect}].ID} && ${Me.CurrentEndurance} > ${Spell[${DiscOpenWound}].EnduranceCost})  /call core_cast "${DiscOpenWound}" disc 0
	
	/if (${Me.PctHPs} > 90 && !${Me.Song[${OpenWoundEffect}].ID}) /call core_cast "Bloodfury" alt 0 FALSE
	
	/if (!${Me.Buff[${AugmentedFrenzyEffect}].ID}) /call core_cast "${DiscAugmentedFrenzy}" disc 0 FALSE
	/if (!${Me.Song[${DiscCryHavoc}].ID}) /call core_cast "${DiscCryHavoc}" disc 0 FALSE
	/if (!${Me.Song[${DiscFesteringRage}].ID}) /call core_cast "${DiscFesteringRage}" disc 0 FALSE

	| clear this for burns
	/if (!${is_burn_time[]}) /call core_cast "${DiscDisconcert}" disc 0 FALSE
	
	
	| swarm pet
	/if (${switchPcSwarm}) /call core_cast "${DiscPet}" disc ${Target2Kill} FALSE

	| axes
	/call core_cast "${DiscVolley}" disc ${Target2Kill} FALSE
	/call core_cast "${DiscAxeofRallos}" disc ${Target2Kill} FALSE
	/call core_cast "${ThrowingAxe}" disc ${Target2Kill} FALSE

	/if (!${Me.Song[${SharedBloodLustRecourse}].ID}) /call core_cast "${SharedBloodLust}" disc 0 FALSE
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
	/if (${Me.PctHPs} > 81*.${tmpHealOverride}) /return
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.AltAbilityReady[Communion of Blood]}) /call core_cast "Uncanny Resilience" alt 0
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.AltAbilityReady[Blood Sustenance]}) /call core_cast "Blood Sustenance" alt 0
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Bool[${Me.ItemReady[Prayers of Life]}]}) /call core_cast "Prayers of Life" item 0
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

	| epic item
	/call core_cast "${Epic}" item 0 FALSE
	
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
	
	/if (${Me.AltAbilityReady[desperation]} && ${Me.PctHPs} < 90) /call core_cast "Desperation" alt 0 FALSE
	/if (${Me.CombatAbilityReady[${DiscScream}]} && ${Me.CurrentEndurance} > ${Spell[${DiscScream}].EnduranceCost}) /disc ${DiscScream}
	/if (${Me.CombatAbilityReady[${CryOfChaos}]} && !${Me.Song[${CryOfChaos}].ID}) {
		/delay ${Math.Rand[8]}s ${Me.Song[${CryOfChaos}].ID}
		/if (!${Me.Song[${CryOfChaos}].ID}) {
			/if (${Raid.Members}) /call core_cast "Cry of Battle" alt 0 FALSE
			/if (${Me.CombatAbilityReady[${CryOfChaos}]}  &&${Me.CurrentEndurance} > ${Spell[${CryOfChaos}].EnduranceCost}) /disc ${CryOfChaos}
		}
	} 
	
	| AA rampage choice
	/if (${AEMelee} && ${SpawnCount[npc group radius 50 playerstate 4]} > 1) /call core_cast "Furious Rampage" alt 0 FALSE
	/if (!${AEMelee}) /call core_cast "Focused Furious Rampage" alt 0 FALSE
	
	
	| Brutal Discipline
	/if (${Window[CombatAbilityWnd].Child[CAW_CombatEffectLabel].Text.Equal["No Effect"]} && ${Me.CombatAbilityReady[${DiscBerserking}]}) {
		/if (${Me.AltAbilityReady[Blinding Fury]} && !${Me.Song[Strike of Savagery].ID}) /call core_cast "Blinding Fury" alt 0 FALSE
		/if (${Me.Buff[Blinded by Fury].ID}) /call core_cast "${DiscBerserking}" disc 0 FALSE
		/if (${Me.AltAbilityReady[Juggernaut Surge]}) /call core_cast "Juggernaut Surge" alt 0 FALSE
		/if (!${Bool[${Me.ActiveDisc[${DiscMele}]}]} || !${Bool[${Me.Song[Empowered Blades]}]}) /call use_glyph
		/if (${Me.AltAbilityReady[Intensity of the Resolute]} && !${Me.Song[Reckless Abandon].ID} && ${Raid.Members}) /call core_cast "Intensity of the Resolute" alt 0 FALSE

	| savage spirit .. does not stack with shaman epic buff
	} else /if (${Me.AltAbilityReady[Savage Spirit]} && ${Window[CombatAbilityWnd].Child[CAW_CombatEffectLabel].Text.Equal["No Effect"]} && !${Bool[${Me.Song[Prophet's Gift of the Ruchu]}]}) {
		| /if (${Me.PctHPs} > 90 && !${Me.Song[${OpenWoundEffect}].ID})  /call core_cast "${DiscOpenWound}" disc 0 FALSE
		/call core_cast "${DiscFrenzyResolve}" disc 0 FALSE
		/delay 3
		/call core_cast "Savage Spirit" alt 0 FALSE
		/if (!${switchAgro}) /call core_cast "Silent Strikes" alt 0 FALSE
		/call core_cast "${UntamedgRage}" alt 0 FALSE
		/call core_cast "${RecklessAbandon}" alt 0 FALSE
		/call cast_AA_spire
		/call core_cast "Blood Pact" alt 0 FALSE
		
	| Cleaving Acrimony Discipline
	} else /if (${Me.CombatAbilityReady[${DiscCleavingAnger}]} && ${Window[CombatAbilityWnd].Child[CAW_CombatEffectLabel].Text.Equal["No Effect"]}) {
		/disc ${DiscCleavingAnger}
		/if (!${Me.Song[Reckless Abandon].ID}) /call core_cast "Vehement Rage" alt 0 FALSE

	| Avenging Flurry Discipline	
	} else /if (${Window[CombatAbilityWnd].Child[CAW_CombatEffectLabel].Text.Equal["No Effect"]} && ${Me.CombatAbilityReady[${DiscVengeful}]}) {
		/disc ${DiscVengeful}
		/call core_cast "Blood Pact" alt 0 FALSE
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
sub Bind_setBER(string _classType, string _classVerbage)
	DEBUG ${cbug}Bind_setBER(type:${_classType}, verbage:${_classVerbage})\ax
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
	} else /if (${Bool[${${_bind}Type.Find[${_classType}]}]} && ${_classVerbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_classType.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_classType.Lower}]}
		/return
	}	

	| AE or Focused Melee
	/if (${Bool[${_classType.Equal[ae]}]}) {
		/call writeINI AEMelee Discs ${If[${AEMelee},FALSE,TRUE]}
		VOUT /${_bind} ${cinfo}AE Melee\ax ${sep} ${If[${AEMelee},${oenabled},${odisabled}]}


	| spire control
	} else /if (${Bool[${_classType.Equal[spiregroup]}]} || ${Bool[${_classType.Equal[spireraid]}]}) {
		/declare _spiretype string local ${_classType.Left[5].Lower}${_classType.Mid[6,1].Upper}${_classType.Right[-6].Lower}
		/if (${Range.Between[0,3:${Int[${_classVerbage}]}]}) {
			/call writeINI ${_spiretype} Buffs ${_classVerbage}
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



