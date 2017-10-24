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
		/if (${isValidTarget}) /call stick_to_target
		/if (${isValidTarget} && ${Me.CombatAbilityReady[${DiscSlapintheFace}]} && ${Me.PctAggro} <= 0 && !${Me.Combat} && ${Me.CurrentEndurance} > ${Spell[${DiscSlapintheFace}].EnduranceCost}) /call core_cast "${DiscSlapintheFace}" disc ${Target2Kill}
		/call set_combat MELEE
		/call stick_to_target
		/call use_skill_melee
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/call stick_to_target
		/call use_skill_melee
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}		
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
	}
	/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} || !${coreAuto} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Bool[${Macro.Return}]}) /goto :killtarget
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
	/if (${Me.CombatAbilityReady[${DiscOverwhelmingFrenzy}]}) /call core_cast "${DiscOverwhelmingFrenzy}" disc ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscEnduranceTap}]} && ${Me.CurrentEndurance} > ${Spell[${DiscEnduranceTap}].EnduranceCost} && ${Me.PctHPs} > 90) /call core_cast "${DiscEnduranceTap}" disc 0
	/if (${Bool[${Me.Buff[Frenzied Resolve]}]} && ${Spawn[${Target2Kill}].Distance} > ${CombatDistance}) /call core_cast "Drawn to Blood" alt 0
	/if (${Me.AltAbilityReady[Battle Leap]} && !${Me.Underwater} && !${Me.Song[Battle Leap Warcry].ID}) /call core_cast "Battle Leap" alt 0	
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
		
	/if (${Me.CombatAbilityReady[${DiscCombatFrenzy}]} && ${Me.CurrentEndurance} > ${Spell[${DiscCombatFrenzy}].EnduranceCost}) /call core_cast "${DiscCombatFrenzy}" disc ${Target2Kill}
		
	/if (${Me.AltAbilityReady[Communion of Blood]} && ${Me.PctHPs} > 75 && ${Me.PctEndurance} < 50) /call core_cast "Communion of Blood" alt 0
	/if (${Me.CombatAbilityReady[${DiscJolt}]} && ${Me.PctAggro} > 80 && ${Me.CurrentEndurance} > ${Spell[${DiscJolt}].EnduranceCost}) /call core_cast "${DiscJolt}" disc ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscDicho}]} && ${Me.CurrentEndurance} > ${Spell[${DiscDicho}].EnduranceCost}*2) /disc 49307

	| /if (${Me.CombatAbilityReady[${DiscOpenWound}]} && ${Me.PctHPs} > 90 && !${Me.Song[${OpenWoundEffect}].ID} && ${Me.CurrentEndurance} > ${Spell[${DiscOpenWound}].EnduranceCost})  /call core_cast "${DiscOpenWound}" disc 0
	
	/if (${Me.AltAbilityReady[Bloodfury]} && ${Me.PctHPs} > 90 && !${Me.Song[${OpenWoundEffect}].ID}) /call core_cast "Bloodfury" alt 0
	
	/if (${Me.CombatAbilityReady[${DiscAugmentedFrenzy}]} && !${Me.Buff[${AugmentedFrenzyEffect}].ID} && ${Me.PctHPs} < 90 && ${Me.CurrentEndurance} > ${Spell[${DiscAugmentedFrenzy}].EnduranceCost}) /call core_cast "${DiscAugmentedFrenzy}" disc 0
	/if (${Me.CombatAbilityReady[${DiscCryHavoc}]} && !${Me.Song[${DiscCryHavoc}].ID} && ${Me.CurrentEndurance} > ${Spell[${DiscCryHavoc}].EnduranceCost}) /call core_cast "${DiscCryHavoc}" disc 0
	/if (${Me.CombatAbilityReady[${DiscFesteringRage}]} && !${Me.Song[Bubbling Rage].ID} && ${Me.CurrentEndurance} > ${Spell[${DiscFesteringRage}].EnduranceCost}) /call core_cast "${DiscFesteringRage}" disc 0

	| clear this for burns
	/if (${Me.CombatAbilityReady[${DiscDisconcert}]} && ${Me.CurrentEndurance} > ${Spell[${DiscDisconcert}].EnduranceCost}) /call core_cast "${DiscDisconcert}" disc 0

	| axes
	/if (${Me.CombatAbilityReady[${DiscVolley}]} && ${Me.CurrentEndurance} > ${Spell[${DiscVolley}].EnduranceCost}) /call core_cast "${DiscVolley}" disc ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscAxeofRallos}]} && ${Me.CurrentEndurance} > ${Spell[${DiscAxeofRallos}].EnduranceCost}) /call core_cast "${DiscAxeofRallos}" disc ${Target2Kill}
	/if (${Me.CombatAbilityReady[${ThrowingAxe}]} && ${Me.CurrentEndurance} > ${Spell[${ThrowingAxe}].EnduranceCost}) /call core_cast "${ThrowingAxe}" disc ${Target2Kill}

	/if (${Me.AltAbilityReady[Distraction Attack]} && ${Me.PctAggro} > 96 ) /call core_cast "Distraction Attack" alt 0
	/if (!${Me.Song[${SharedBloodLustRecourse}].ID} && ${Me.CombatAbilityReady[${SharedBloodLust}]} && ${Me.CurrentEndurance} > ${Spell[${SharedBloodLust}].EnduranceCost}) /call core_cast "${SharedBloodLust}" disc 0

	:exit
/return



|***
 *	DES: Agro Routines
 * 	USE: manual/Named/raid. all sorts...
 *	INI: 
 *  NOTE: 
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
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Bool[${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0
	:exit
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
	
	/if (${Me.AltAbilityReady[desperation]} && ${Me.PctHPs} < 90) /call core_cast "Desperation" alt 0
	/if (${Me.CombatAbilityReady[${DiscScream}]} && ${Me.CurrentEndurance} > ${Spell[${DiscScream}].EnduranceCost}) /disc ${DiscScream}
	/if (${Me.CombatAbilityReady[${CryOfChaos}]} && !${Me.Song[${CryOfChaos}].ID}) {
		/delay ${Math.Rand[8]}s ${Me.Song[${CryOfChaos}].ID}
		/if (!${Me.Song[${CryOfChaos}].ID}) {
			/if (${Me.AltAbilityReady[Cry of Battle]} && ${Raid.Members}) /call core_cast "Cry of Battle" alt 0
			/if (${Me.CurrentEndurance} > ${Spell[${CryOfChaos}].EnduranceCost}) /disc ${CryOfChaos}
		}
	} 
	/if (${Me.AltAbilityReady[Savage Spirit]} && ${Window[CombatAbilityWnd].Child[CAW_CombatEffectLabel].Text.Equal["No Effect"]}) {
		/if (${Me.CombatAbilityReady[${DiscOpenWound}]} && ${Me.PctHPs} > 90 && !${Me.Song[${OpenWoundEffect}].ID} && ${Me.CurrentEndurance} > ${Spell[${DiscOpenWound}].EnduranceCost})  /call core_cast "${DiscOpenWound}" disc 0
		/if (${Me.CurrentEndurance} > ${Spell[${DiscFrenzyResolve}].EnduranceCost}) /call core_cast "${DiscFrenzyResolve}" disc 0
		/delay 3
		/call core_cast "Savage Spirit" alt 0
		/if (${Me.AltAbilityReady[Silent Strikes]}) /call core_cast "Silent Strikes" alt 0
		/if (${Me.AltAbilityReady[${UntamedgRage}]}) /call core_cast "${UntamedgRage}" alt 0
		/if (${Me.AltAbilityReady[${RecklessAbandon}]}) /call core_cast "${RecklessAbandon}" alt 0
		/call cast_AA_spire
		/if (${Me.AltAbilityReady[Blood Pact]}) /call core_cast "Blood Pact" alt 0
	} else /if (${Window[CombatAbilityWnd].Child[CAW_CombatEffectLabel].Text.Equal["No Effect"]} && ${Me.CombatAbilityReady[${DiscBerserking}]} && ${Me.CurrentEndurance} > ${Spell[${DiscBerserking}].EnduranceCost}) {
		/if (${Me.AltAbilityReady[Blinding Fury]} && !${Me.Song[Strike of Savagery].ID}) /call core_cast "Blinding Fury" alt 0
		/if (${Me.Buff[Blinded by Fury].ID}) /call core_cast "${DiscBerserking}" disc 0
		/if (${Me.AltAbilityReady[Juggernaut Surge]}) /call core_cast "Juggernaut Surge" alt 0
		/if (!${Bool[${Me.ActiveDisc[${DiscMele}]}]} || !${Bool[${Me.Song[Empowered Blades]}]}) /call use_glyph
		/if (${Me.AltAbilityReady[Intensity of the Resolute]} && !${Me.Song[Reckless Abandon].ID} && ${Raid.Members}) /call core_cast "Intensity of the Resolute" alt 0
	} else /if (${Me.CombatAbilityReady[${DiscCleavingAnger}]} && ${Window[CombatAbilityWnd].Child[CAW_CombatEffectLabel].Text.Equal["No Effect"]} && ${Me.CurrentEndurance} > ${Spell[${DiscCleavingAnger}].EnduranceCost}) {
		/disc ${DiscCleavingAnger}
		/if (${Me.AltAbilityReady[Vehement Rage]} && !${Me.Song[Reckless Abandon].ID}) /call core_cast "Vehement Rage" alt 0
	} else /if (${Window[CombatAbilityWnd].Child[CAW_CombatEffectLabel].Text.Equal["No Effect"]} && ${Me.CombatAbilityReady[${DiscVengeful}]} && ${Me.CurrentEndurance} > ${Spell[${DiscVengeful}].EnduranceCost}) {
		/disc ${DiscVengeful}
		/if (${Me.AltAbilityReady[Blood Pact]}) /call core_cast "Blood Pact" alt 0
	}	
	
	
	/call synergy_click_Circle
	/if (${doclickItemBurn}) /call click_item_burn	
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
	/if (!${validate_class[|BER]}) /return

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
	| enable/disable summer line of spells
	/if (${Bool[${_classType.Find[TEST]}]}) {
		| /call Event_SetVar NA switchNukeSummer Spells ${If[${switchNukeSummer},FALSE,TRUE]}

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
