|***
 *	class.war.inc
 *	Core Warrior Include
 *
 *	
 ***|
 


|***
 *	DES: Main Warrior Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_WAR()
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}()\ax
	/doevents
	AMIDEAD
	CHECKTIE

	:killtarget
	/call prep_combat
	/if (!${Bool[${Macro.Return}]}) /return

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/call stick_to_target
		/call use_skill_melee ${Target2Kill}
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/call click_DPS_${Me.Class.ShortName}
		/call stick_to_target
		/call click_item
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
		/call set_combat RANGE
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/call click_DPS_${Me.Class.ShortName}
		/call click_item
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
	}
	/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} && !${is_target_dead[${Target2Kill}]}) /goto :killtarget
	/if (!${coreAuto} && ${Target.ID} && ${Bool[${combat}]} && !${is_target_dead[${Target.ID}]}) /goto :killtarget
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub disc_WAR()
	DEBUGCLASS ${cbug}disc_${Me.Class.ShortName}()\ax
	AMIDEAD

	/doevents
	/call check_exit TRUE
	/if (${Bool[${Macro.Return}]}) /return
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /return
	/if (${switchAgroMode}) /call get_agro_${Me.Class.ShortName}
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	/call check_deBuffs ${Target2Kill}
	/if (${switchdeBuffSnare}) /call cast_aa_snare ${Target2Kill}

	/if (${Me.AltAbilityReady[${AABraceforImpact}]}) /call core_cast "${AABraceforImpact}" alt 0 FALSE
	/if (${Me.AltAbilityReady[${AAWarStomp}]}) /call core_cast "${AAWarStomp}" alt 0 FALSE
	/if (${Me.AltAbilityReady[${AAGutPunch}]} && ${Target.Distance} <= ${Spell[Gut Punch].MyRange}) /call core_cast "${AAGutPunch}" alt ${Target2Kill} FALSE

	/if (${Me.AltAbilityReady[${AAImpCommand}]}) /call core_cast "${AAImpCommand}" alt 0 FALSE
	/if (${Me.AltAbilityReady[${AAKneeStrike}]} && ${Target.Distance} <= ${Spell[${AAKneeStrike}].MyRange}) /call core_cast "${AAKneeStrike}" alt ${Target2Kill} FALSE
	/if (${Me.AltAbilityReady[${AAMageHunter}]} && !${Me.Buff[${AAMageHunter}].ID}) /call core_cast "${AAMageHunter}" alt 0 FALSE
	
	/if (${Me.AltAbilityReady[${AAHeroicBlade}]} && ${Target.Distance} <= ${Spell[${AAHeroicBlade}].MyRange} && !${switchAgroMode}) /call core_cast "${AAHeroicBlade}" alt 0 FALSE
	/if (${Me.AltAbilityReady[${AABladeGuardian}]} && ${Me.Song[${AABladeGuardian}].Stacks}) /call core_cast "${AABladeGuardian}" alt 0 FALSE

	/if (${Me.CombatAbilityReady[${DiscField}]} && ${Me.CurrentEndurance} > ${Spell[${DiscField}].EnduranceCost} && !${Me.Song[${DiscField}].ID} ) /call core_cast "${DiscField}" disc 0 FALSE

/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub get_agro_WAR()
	DEBUGCLASS ${cbug}get_agro_${Me.Class.ShortName}()\ax
	AMIDEAD

	/doevents
	CHECKEXIT
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /return
	/if (${Me.AltAbilityReady[${AARageoftheForsaken}]} && ${Target.Distance} <= ${Spell[${AARageoftheForsaken}].MyRange} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast "${AARageoftheForsaken}" alt ${Target2Kill} FALSE
	/if (${Me.CombatAbilityReady[${DiscInsult}]} && ${Me.CurrentEndurance} > ${Spell[${DiscInsult}].EnduranceCost} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast "${DiscInsult}" disc ${Target2Kill} FALSE
	/if (${Me.CombatAbilityReady[${DiscWade}]} && ${Me.CurrentEndurance} > ${Spell[${DiscWade}].EnduranceCost} && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 30 targetable]} >= 3) /call core_cast "${DiscWade}" disc 0 FALSE
	/if (${Me.CombatAbilityReady[${DiscCyclone}]} && ${Me.CurrentEndurance} > ${Spell[${DiscCyclone}].EnduranceCost} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast "${DiscCyclone}" disc ${Target2Kill} FALSE
	/if (${Me.CombatAbilityReady[${DiscChallenge}]} && ${Me.CurrentEndurance} > ${Spell[${DiscChallenge}].EnduranceCost} && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 30 targetable]} >= 3) /call core_cast "${DiscChallenge}" disc 0 FALSE
	/if (${Me.CombatAbilityReady[${DiscShout}]} && ${Me.CurrentEndurance} > ${Spell[${DiscShout}].EnduranceCost} ) /call core_cast "${DiscShout}" disc 0 FALSE
	/if (${Me.AltAbilityReady[${AABlastofAnger}]} && ${Target.Distance} <= ${Spell[${AABlastofAnger}].MyRange} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast "${AABlastofAnger}" alt ${Target2Kill} FALSE
	/if (${Me.AltAbilityReady[${AAAETaunt}]} && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 50 targetable]} >= 2) /call core_cast "${AAAETaunt}" alt 0 FALSE
/return



|***
 *	DES: Emergency Self heals
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_WAR()
	DEBUGCLASS ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	CHECKTIE
	/if (${Me.CombatAbilityReady[${DiscStoutDefense}]} && ${Me.CurrentEndurance} > ${Spell[${DiscStoutDefense}].EnduranceCost} && !${Raid.Members} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscStoutDefense}" disc 0 FALSE
	/if (${Me.PctHPs} <= ${PanicHPPCT}*.${tmpHealOverride} && ${Me.CombatAbilityReady[${DiscNoTime}]} && ${Me.CurrentEndurance} >= ${Spell[${DiscNoTime}].EnduranceCost}) /call core_cast "${DiscNoTime}" disc 0 FALSE
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0 FALSE
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0 FALSE
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_WAR()
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	AMIDEAD
	/doevents
	/call click_horn_of_unity 	
/return



 |***
 *	DES: Warrior burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_WAR()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	AMIDEAD

	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /return	
	
	
	/if (${Me.CombatAbilityReady[${DiscAttention}]} && ${Me.CurrentEndurance} > ${Spell[${DiscAttention}].EnduranceCost}) /call core_cast "${DiscAttention}" disc ${Target2Kill} FALSE
	/if (${Me.CombatAbilityReady[${DiscPhantom}]} && ${Me.CurrentEndurance} > ${Spell[${DiscPhantom}].EnduranceCost} && ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) /call core_cast "${DiscPhantom}" disc ${Target2Kill} FALSE

	/if (${Me.AltAbilityReady[${AAResplendentGlory}]} && !${Me.Song[${AAHoldtheLine}].ID}) {
		/call core_cast "${AAResplendentGlory}" alt 0 FALSE
	} else /if (${Me.AltAbilityReady[${AAHoldtheLine}]} && !${Me.Song[${AAResplendentGlory}].ID}) {
		/call core_cast "${AAHoldtheLine}" alt 0 FALSE
	}
	/if (${Me.AltAbilityReady[${AAWarlordsTenacity}]}) /call core_cast "${AAWarlordsTenacity}" alt 0 FALSE
	/if (${Me.AltAbilityReady[${AAWarlordsBravery}]}) /call core_cast "${AAWarlordsBravery}" alt 0 FALSE

	/call cast_AA_spire
	/if (${Me.CombatAbilityReady[${DiscFlash}]} && ${Me.CurrentEndurance} > ${Spell[${DiscFlash}].EnduranceCost}) /call core_cast "${DiscFlash}" disc 0 FALSE
	/call use_glyph
	/call synergy_click_Circle
	/call click_item_burn
/return



|***
 *	DES: Class controls
 *	USE: /war []
 *	INI: 
 *  NOTE:
 ***|
#bind setWAR /war
sub Bind_setWAR(string _classType, string _classVerbage)
	DEBUG ${cbug}Bind_setWAR(type:${_classType}, verbage:${_classVerbage})\ax
	/if (!${validate_class[TRUE, |WAR]}) /return

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
	

	/if (${Bool[${_classType.Find[TEST]}]}) {

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
			/if (${${_bind}Type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}[${cgood}${spireGroup}\ax]
			/if (${${_bind}Type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}[${cgood}${spireRaid}\ax]
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
	} 
/return