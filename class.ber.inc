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
	DEBUG Loop start: ${cbug}core_BER()\ax
	/doevents
	ISDEAD
	CHECKTIE
	
	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /goto :exit
	
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
	/if (${AutoBot} && !${Spawn[${Target2Kill}].ID} || !${AutoBot} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Macro.Return}) /goto :killtarget
	}
	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub disc_BER()
	DEBUG ${cbug}disc_BER()\ax
	ISDEAD
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
	DEBUG ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	/call check_exit
	/if (${Macro.Return} || ${Me.PctHPs} > 81*.${tmpHealOverride}) /goto :exit

	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.AltAbilityReady[Communion of Blood]}) /call core_cast "Uncanny Resilience" alt 0
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.AltAbilityReady[Blood Sustenance]}) /call core_cast "Blood Sustenance" alt 0
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Bool[${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0
	/if (${SpawnCount[npc los radius 60 playerstate 4 targetable]} >= 5 && ${Me.ItemReady[Signet of Gannar]} && !${Bool[${Me.Song[Circle of Life]}]}) /call core_cast "Signet of Gannar" item 0
	:exit
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_BER()
	DEBUG ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/call click_horn_of_unity 	
	:exit
/return
