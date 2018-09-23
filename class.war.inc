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
	AMIDEAD

	CHECKINPUTS
	/call check_exit TRUE
	/if (${Bool[${Macro.Return}]}) /return
	/if (${is_target_dead[${Target2Kill}]}) /return
	
	/if (${switchAgro}) /call get_agro_${Me.Class.ShortName}

	/if (${validate_cast[FALSE, alt, "${AABraceforImpact}", ${Me.ID}]}) /call core_cast2 "${AABraceforImpact}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAWarStomp}", ${Me.ID}]]}) /call core_cast2 "${AAWarStomp}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAGutPunch}", ${Target2Kill}]}) /call core_cast2 "${AAGutPunch}" alt ${Target2Kill} FALSE

	/if (${validate_cast[FALSE, alt, "${AAImpCommand}", ${Me.ID}]}) /call core_cast2 "${AAImpCommand}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAKneeStrike}", ${Target2Kill}]}) /call core_cast2 "${AAKneeStrike}" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "${AAMageHunter}", ${Me.ID}]]}) /call core_cast2 "${AAMageHunter}" alt 0 FALSE
	
	/if (${validate_cast[FALSE, alt, "${AAHeroicBlade}", ${Me.ID}]]} && !${switchAgro}) /call core_cast2 "${AAHeroicBlade}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AABladeGuardian}", ${Me.ID}]]}) /call core_cast2 "${AABladeGuardian}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAVehementRage}", ${Me.ID}]]}) /call core_cast2 "${AAVehementRage}" alt 0 FALSE

	/if (${validate_cast[FALSE, disc, "${DiscField}", ${Me.ID}]]} && !${Me.Song[${DiscField}].ID}) /call core_cast2 "${DiscField}" disc 0 FALSE

/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub get_agro_WAR()
	AMIDEAD

	CHECKINPUTS
	CHECKEXIT
	/if (${is_target_dead[${Target2Kill}]}) /return
	
	
	/if (${validate_cast[FALSE, alt, "${AARageoftheForsaken}", ${Target2Kill}]} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast2 "${AARageoftheForsaken}" alt ${Target2Kill} FALSE

	/if (${validate_cast[FALSE, disc, "${DiscInsult}", ${Target2Kill}]} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast2 "${DiscInsult}" disc ${Target2Kill} FALSE


	/if (${AEMelee}) {
		/if (${validate_cast[FALSE, disc, "$${DiscWade}", ${Me.ID}]} && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 30 targetable]} >= 3) /call core_cast2 "${DiscWade}" disc 0 FALSE
		/if (${validate_cast[FALSE, alt, "${AAAETaunt}", ${Me.ID}]} && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 50 targetable]} >= 2) /call core_cast2 "${AAAETaunt}" alt 0 FALSE

		/if (${validate_cast[FALSE, disc, "$${DiscChallenge}", ${Me.ID}]} && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 30 targetable]} >= 3) /call core_cast2 "${DiscChallenge}" disc 0 FALSE

	}


	/if (${validate_cast[FALSE, disc, "$${DiscCyclone}", ${Target2Kill}]} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast2 "${DiscCyclone}" disc ${Target2Kill} FALSE


	/if (${validate_cast[FALSE, disc, "${DiscShout}", ${Me.ID}]) /call core_cast2 "${DiscShout}" disc 0 FALSE

	/if (${validate_cast[FALSE, alt, "${AABlastofAnger}", ${Target2Kill}]} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast2 "${AABlastofAnger}" alt ${Target2Kill} FALSE



/return



|***
 *	DES: Emergency Self heals
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_WAR()
	AMIDEAD
	CHECKEXIT
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return

	/if (!${Raid.Members}) {
		/if (${validate_cast[FALSE, activedisc, "${DiscStoutDefense}", ${Me.ID}]]}) /call core_cast2 "${DiscStoutDefense}" disc 0 FALSE
	}
	/if (${Me.PctHPs} <= ${PanicHPPCT}*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, disc, "${DiscNoTime}", ${Me.ID}]]}) /call core_cast2 "${DiscNoTime}" disc 0 FALSE
	}
	/call click_item_heal
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_WAR()
	AMIDEAD
	CHECKINPUTS
	/call click_horn_of_unity 	
/return



 |***
 *	DES: Warrior burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_WAR()
	AMIDEAD

	CHECKINPUTS
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /return	
	
	
	/if (${validate_cast[FALSE, disc, "${DiscAttention}", ${Me.ID}]}) /call core_cast2 "${DiscAttention}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscPhantom}", ${Me.ID}]} && ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) /call core_cast2 "${DiscPhantom}" disc ${Target2Kill} FALSE

	/if (${Me.AltAbilityReady[${AAResplendentGlory}]} && !${Me.Song[${AAHoldtheLine}].ID}) {
		/if (${validate_cast[FALSE, alt, "${AAResplendentGlory}", ${Me.ID}]]}) /call core_cast2 "${AAResplendentGlory}" alt 0 FALSE
	} else /if (${Me.AltAbilityReady[${AAHoldtheLine}]} && !${Me.Song[${AAResplendentGlory}].ID}) {
		/if (${validate_cast[FALSE, alt, "${AAHoldtheLine}", ${Me.ID}]]}) /call core_cast2 "${AAHoldtheLine}" alt 0 FALSE
	}
	/if (${validate_cast[FALSE, alt, "${AAWarlordsTenacity}", ${Me.ID}]}) /call core_cast2 "${AAWarlordsTenacity}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAWarlordsBravery}"m ${Me.ID}]}) /call core_cast2 "${AAWarlordsBravery}" alt 0 FALSE

	/call cast_AA_spire
	/if (${validate_cast[FALSE, disc, "${DiscFlash}", ${Me.ID}]}) /call core_cast2 "${DiscFlash}" disc 0 FALSE
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
	/if (!${validate_class[TRUE, |WAR]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _type string	local |ae|fade|healpoint|spiregroup|spireraid|taunt

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

	| on/off taunt
	} else /if (${Bool[${_classType.Equal[taunt]}]}) {
		/call writeINI taunt meleedisc ${If[${taunt},FALSE,TRUE]}  

	| panic heal pct
	} else /if (${Bool[${_classType.Find[panicheal]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI PanicHPPCT Spells ${_verbage}
		} else /if (!${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} panicheal [${cinfo}${${PanicHPPCT}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}
		
	| AE or Focused Melee
	} else /if (${Bool[${_classType.Equal[ae]}]}) {
		/call writeINI AEMelee Discs ${If[${AEMelee},FALSE,TRUE]}

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
		/if (${_type.Arg[${_count},|].Equal[taunt]}) /varset _listout ${_listout}(${If[${Bool[${taunt}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}(${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]})
		/if (${_type.Arg[${_count},|].Equal[ae]}) /varset _listout ${_listout}(${If[${AEMelee},${oon},${ooff}]})


		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return