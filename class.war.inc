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
	CHECKREZ
	/call prep_combat
	/if (${Macro.Return} && ${Bool[${combat}]}) {
		DEBUG combat:${cinfo}${combat.Upper}\ax		

		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${switchHealCure}) /call cast_cure

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
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
			/if (${validate_class[FALSE, ${CastList}]}) /call use_mod_rod
			/if (${validate_class[FALSE, ${MeleeList}]}) /call cast_stamina_recovery
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
sub disc_WAR()
	DEBUGCLASS ${cbug}disc_${Me.Class.ShortName}()\ax
	AMIDEAD

	/doevents
	/call check_exit TRUE
	/if (${Bool[${Macro.Return}]}) /return
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /return
	/if (${switchAgro}) /call get_agro_${Me.Class.ShortName}
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}

	/if (${Me.AltAbilityReady[${AABraceforImpact}]}) /call core_cast "${AABraceforImpact}" alt 0 FALSE
	/if (${Me.AltAbilityReady[${AAWarStomp}]}) /call core_cast "${AAWarStomp}" alt 0 FALSE
	/if (${Me.AltAbilityReady[${AAGutPunch}]} && ${Target.Distance} <= ${Spell[Gut Punch].MyRange}) /call core_cast "${AAGutPunch}" alt ${Target2Kill} FALSE

	/if (${Me.AltAbilityReady[${AAImpCommand}]}) /call core_cast "${AAImpCommand}" alt 0 FALSE
	/if (${Me.AltAbilityReady[${AAKneeStrike}]} && ${Target.Distance} <= ${Spell[${AAKneeStrike}].MyRange}) /call core_cast "${AAKneeStrike}" alt ${Target2Kill} FALSE
	/if (${Me.AltAbilityReady[${AAMageHunter}]} && !${Me.Buff[${AAMageHunter}].ID}) /call core_cast "${AAMageHunter}" alt 0 FALSE
	
	/if (${Me.AltAbilityReady[${AAHeroicBlade}]} && ${Target.Distance} <= ${Spell[${AAHeroicBlade}].MyRange} && !${switchAgro}) /call core_cast "${AAHeroicBlade}" alt 0 FALSE
	/if (${Me.AltAbilityReady[${AABladeGuardian}]} && ${Me.Song[${AABladeGuardian}].Stacks}) /call core_cast "${AABladeGuardian}" alt 0 FALSE
	/if (${Me.AltAbilityReady[${AAVehementRage}]}) /call core_cast "${AAVehementRage}" alt 0 FALSE

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
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return

	/if (!${Raid.Members} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscStoutDefense}" disc 0 FALSE
	/if (${Me.PctHPs} <= ${PanicHPPCT}*.${tmpHealOverride}) /call core_cast "${DiscNoTime}" disc 0 FALSE
	/call click_item_heal
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
sub Bind_setWAR(string _classType, string _verbage)
	DEBUG ${cbug}Bind_setWAR(type:${_classType}, verbage:${_verbage})\ax
	/if (!${validate_class[TRUE, |WAR]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _type string	local |healpoint|spiregroup|spireraid

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

	| spire control
	/if (${Bool[${_classType.Equal[spiregroup]}]} || ${Bool[${_classType.Equal[spireraid]}]}) {
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

	| panic heal pct
	} else /if (${Bool[${_classType.Find[panicheal]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI PanicHPPCT Spells ${_verbage}
		} else /if (!${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} panicheal [${cinfo}${${PanicHPPCT}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}

	| defensive mob agro count
	} else /if (${Bool[${_classType.Equal[defcount]}]}) {
		/if (${Range.Between[1,50:${Int[${_verbage}]}]}) {
			/call writeINI defCount Discs ${_verbage}
		} else /if (!${Bool[${_verbage}]} || !${Range.Between[1,50:${Int[${_verbage}]}]}) {
			VOUT /${_bind} defcount [${cinfo}${countMobAgro}\ax] allowable range ${sep} ${cinfo}1 - 50 min\ax
			/return
		}	

	}
	/if (${switchMQclearonCLS}) /mqclear
	/for _count 1 to ${_type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_type.Arg[${_count},|]}\ax
		/if (${_type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${_type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${_type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}${${Me.Class.ShortName}Point}\ax)
		/if (${_type.Arg[${_count},|].Equal[panicheal]}) /varset _listout ${_listout}(${cnum}${PanicHPPCT}\ax)
		/if (${_type.Arg[${_count},|].Equal[defcount]}) /varset _listout ${_listout}(${cnum}${defCount}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return