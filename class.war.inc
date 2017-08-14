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
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}(${cinfo}${version${Me.Class.ShortName}}\ax${cbug})\ax
	/doevents
	ISDEAD
	CHECKTIE

	:killtarget
	/call prep_combat
	/if (!${Bool[${Macro.Return}]}) /goto :exit

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/call stick_to_target
		/call use_skill_melee
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/call click_DPS_${Me.Class.ShortName}
		/call stick_to_target
		/if (${doclickItem}) /call click_item
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
	}
	/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
	/if (${AutoBot} && !${Spawn[${Target2Kill}].ID} || !${AutoBot} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Bool[${Macro.Return}]}) /goto :killtarget
	}
	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub disc_WAR()
	DEBUGCLASS ${cbug}disc_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit TRUE
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/if (${switchAgroMode}) /call get_agro_${Me.Class.ShortName}
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	/if (${switchdeBuffSnare}) /call cast_aa_snare ${Target2Kill}

	/if (${Me.AltAbilityReady[${AABraceforImpact}]}) /call core_cast "${AABraceforImpact}" alt 0
	/if (${Me.AltAbilityReady[${AAWarStomp}]}) /call core_cast "${AAWarStomp}" alt 0
	/if (${Me.AltAbilityReady[${AAGutPunch}]} && ${Target.Distance} <= ${Spell[Gut Punch].MyRange}) /call core_cast "${AAGutPunch}" alt ${Target2Kill}

	/if (${Me.AltAbilityReady[${AAImpCommand}]}) /call core_cast "${AAImpCommand}" alt 0
	/if (${Me.AltAbilityReady[${AAKneeStrike}]} && ${Target.Distance} <= ${Spell[${AAKneeStrike}].MyRange}) /call core_cast "${AAKneeStrike}" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[${AAMageHunter}]} && !${Me.Buff[${AAMageHunter}].ID}) /call core_cast "${AAMageHunter}" alt 0
	
	/if (${Me.AltAbilityReady[${AAHeroicBlade}]} && ${Target.Distance} <= ${Spell[${AAHeroicBlade}].MyRange} && !${switchAgroMode}) /call core_cast "${AAHeroicBlade}" alt 0
	/if (${Me.AltAbilityReady[${AABladeGuardian}]} && ${Me.Song[${AABladeGuardian}].Stacks}) /call core_cast "${AABladeGuardian}" alt 0

	/if (${Me.CombatAbilityReady[${DiscField}]} && ${Me.CurrentEndurance} > ${Spell[${DiscField}].EnduranceCost} && !${Me.Song[${DiscField}].ID} ) /call core_cast "${DiscField}" disc 0

	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub get_agro_WAR()
	DEBUGCLASS ${cbug}get_agro_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	CHECKEXIT
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/if (${Me.AltAbilityReady[${AARageoftheForsaken}]} && ${Target.Distance} <= ${Spell[${AARageoftheForsaken}].MyRange} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast "${AARageoftheForsaken}" alt ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscInsult}]} && ${Me.CurrentEndurance} > ${Spell[${DiscInsult}].EnduranceCost} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast "${DiscInsult}" disc ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscWade}]} && ${Me.CurrentEndurance} > ${Spell[${DiscWade}].EnduranceCost} && ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius 30 targetable]} >= 3) /call core_cast "${DiscWade}" disc 0
	/if (${Me.CombatAbilityReady[${DiscCyclone}]} && ${Me.CurrentEndurance} > ${Spell[${DiscCyclone}].EnduranceCost} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast "${DiscCyclone}" disc ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscChallenge}]} && ${Me.CurrentEndurance} > ${Spell[${DiscChallenge}].EnduranceCost} && ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius 30 targetable]} >= 3) /call core_cast "${DiscChallenge}" disc 0
	/if (${Me.CombatAbilityReady[${DiscShout}]} && ${Me.CurrentEndurance} > ${Spell[${DiscShout}].EnduranceCost} ) /call core_cast "${DiscShout}" disc 0
	/if (${Me.AltAbilityReady[${AABlastofAnger}]} && ${Target.Distance} <= ${Spell[${AABlastofAnger}].MyRange} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast "${AABlastofAnger}" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[${AAAETaunt}]} && ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius 50 targetable]} >= 2) /call core_cast "${AAAETaunt}" alt 0
	:exit
/return



|***
 *	DES: Emergency Self heals
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_WAR()
	DEBUGCLASS ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	ISDEAD
	CHECKEXIT
	CHECKTIE
	/if (${Me.CombatAbilityReady[${DiscStoutDefense}]} && ${Me.CurrentEndurance} > ${Spell[${DiscStoutDefense}].EnduranceCost} && !${Raid.Members} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscStoutDefense}" disc 0
	
	/if (${Me.PctHPs} <= ${PanicHPPCT}*.${tmpHealOverride} && ${Me.CombatAbilityReady[${DiscNoTime}]} && ${Me.CurrentEndurance} >= ${Spell[${DiscNoTime}].EnduranceCost}) /call core_cast "${DiscNoTime}" disc 0
	
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0
	/if (${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${MobAgro} playerstate 4 targetable]} >= 4 && ${Me.ItemReady[${CircleofLife}]} && !${Me.Song[${FindItem[${CircleofLife}].Spell}].ID} && ${Spell[${FindItem[${CircleofLife}].Spell}].Stacks}) /call core_cast "${CircleofLife}" item 0
	:exit
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_WAR()
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	ISDEAD
	/doevents
	/call click_horn_of_unity 	
	:exit
/return
