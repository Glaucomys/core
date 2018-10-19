|***
 *	class.shd.inc
 *	Core Shadowknight Include
 *
 * 
 * 
 ***|



|***
 *	DES: Main Shadow Knight Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_SHD()
	DEBUG core_SHD()

	/if (${coreAuto}) {
		/if (!${check_engaged[]} && ${switchPcPet}) /call check_pet
	}	
	
	/if (${useShieldFlash}) /call cast_AA_shield_flash

	/while (${prep_combat[]}) {
		CHECKINPUTS
		/if (${exit_class_loop[]}) /return FALSE

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
			/call click_BP
			/call click_item
			/if (${switchPcSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
			/if (${switchHarmonious}) /call check_harmonious
			/if (${switchSkin} && !${timer_check_Skin}) /call check_skin	
			/if (${switchStance} && (!${timer_check_Stance} || !${Bool[${Me.Buff[${Stance}]}]})) /call check_stance
		}
		/if (${coreAuto}) {	
			/if (${switchAAFade} && ${Me.PctHPs} <= ${useAAFadePct}) /call cast_AA_fade
			/if (${switchModRod} && ${validate_class[FALSE, ${CastList}]}) /call use_mod_rod
			/if (${validate_class[FALSE, ${MeleeList}]}) /call cast_stamina_recovery
		}
	}
/return TRUE



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub disc_SHD()
	DEBUG disc_SHD()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	CHECKTIE

	| defensive	
	/if (${coreAuto} && ${switchAgro} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]} >= ${defCount}) {
		/declare _count int local 0
		/for _count 1 to 3
			/if (${Bool[${Me.ActiveDisc}]}) /break
			/if (${validate_cast[FALSE, activedisc, "${DefensiveDisc${_count}}", ${Me.ID}]}) /call core_cast2 "${DefensiveDisc${_count}}" disc 0 FALSE
		/next _count
	}
	/if (${useShieldFlash}) /call cast_AA_shield_flash

	| AA Voice of Thule AA
	/if (${switchAgro} && !${timer_check_vot} && ${switchVoT}) {
		/if (!${Me.Buff[Voice of Thule].ID}) {	
			/if (${validate_cast[FALSE, alt, "Voice of Thule", ${Me.ID}]}) /call core_cast2 "Voice of Thule" alt ${Me.ID} FALSE
			/call create_timer timer_check_vot ${tcheck_vot}
		}
	} else /if ((!${switchAgro} && ${Me.Buff[Voice of Thule].ID}) || !${switchVoT}) {
			/invoke ${Me.Buff[Voice of Thule].Remove}
	}

	| get me sum...
	/if (${switchAgro}) /call get_agro_${Me.Class.ShortName}

	| tap 'dat ass..
	/if (${switchHealSelf}) {
		/if (${validate_cast[FALSE, disc, "${DiscRancor}", ${Target2Kill}]} && ${Me.PctHPs} < ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /call core_cast2 "${DiscRancor}" disc ${Target2Kill} FALSE
		/if (${validate_cast[FALSE, alt, "Leech Touch", ${Target2Kill}]} && ${Me.PctHPs} < ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /call core_cast2 "Leech Touch" alt ${Target2Kill} FALSE
		/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${combatRadius}*2 targetable]} >= ${defCount}) {
			/if (${validate_cast[FALSE, disc, "${DiscLeech}", ${Me.ID}]} && ${Me.PctHPs} < ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /call core_cast2 "${DiscLeech}" disc 0 FALSE
		}
		/if (${validate_cast[FALSE, alt, "Vicious Bite of Chaos" ${Target2Kill}]} && ${Me.PctHPs} < ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /call core_cast2 "Vicious Bite of Chaos" alt ${Target2Kill} FALSE
	}

	| heal clickies
	/call click_item_heal


	| swarm pets
	/if (${switchPcSwarm}) {
		/if (${validate_cast[FALSE, alt, "Chattering Bones", ${Target2Kill}]}) /call core_cast2 "Chattering Bones" alt ${Target2Kill} FALSE
	}
	
	/if (${validate_cast[FALSE, disc, "${Withstand}", ${Me.ID}]}) /call core_cast2 "${Withstand}" disc 0 FALSE
	/if (${validate_cast[FALSE, alt, "Thought Leech", ${Target2Kill}]} &&  ${Me.PctMana} <= 60*.${tmpHealOverride}) /call core_cast2 "Thought Leech" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${BladeStrike}", ${Target2Kill}]}) /call core_cast2 "${BladeStrike}" disc ${Target2Kill} FALSE

/return



|***
 *	DES: take what you want. there is no try...
 *	USE: /call get_agro_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub get_agro_SHD()
	DEBUG get_agro_SHD()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${switchPcPet}) /call send_pet
	
	/call cast_challenge ${Target2Kill}
	/if (${useShieldFlash}) /call cast_AA_shield_flash
	
	| AA pet hate
	/if (${Me.PctAggro} < 80 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
		/if (${validate_cast[FALSE, alt, "Projection of Doom", ${Target2Kill}]}) /call core_cast2 "Projection of Doom" alt ${Target2Kill} FALSE
	}	
	

	| single hate
	/if ((!${Target.Buff[${Hate1}].ID} && !${Target.Buff[${Hate2}].ID}) || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
		/if (${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
			/if (${validate_cast[FALSE, spell, "${Hate1}", ${Target2Kill}]}) /call core_cast2 "${Hate1}" 0 ${Target2Kill} FALSE
		}
		/if (${Me.PctAggro} < 80 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
			/if (${validate_cast[FALSE, spell, "${Hate2}", ${Target2Kill}]}) /call core_cast2 "${Hate2}" 0 ${Target2Kill} FALSE
		}
	}

	| AE Hate
	/if (${AEMelee}) {
		/varset mobAECount ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 50]}
		/if (${mobAECount} > 1) {
			/if (${Me.AltAbilityReady[Explosion of Hatred]}) {
				/if (${validate_cast[FALSE, alt, "Explosion of Hatred", ${Me.ID}]}) /call core_cast2 "Explosion of Hatred" alt 0 FALSE
			} else /if (${Me.SpellReady[${AEHate1}]}) {
				/if (${validate_cast[FALSE, spell, "${AEHate1}", ${Me.ID}]}) /call core_cast2 "${AEHate1}" 0 0 FALSE
			}
		} 
		/if (${mobAECount} > 3) {
			/if (${Me.AltAbilityReady[Explosion of Spite]}) {
				/if (${validate_cast[FALSE, alt, "Explosion of Spite", ${Me.ID}]}) /call core_cast2 "Explosion of Spite" alt 0 FALSE
			} else /if (${Me.SpellReady[${AEHate2}]}) {
				/if (${validate_cast[FALSE, spell, "${AEHate2}", ${Me.ID}]}) /call core_cast2 "${AEHate2}" 0 0 FALSE
			}
		}	
	}

	| AE lifetap	
	/if (${AEMelee}) {
		/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${combatRadius}*2 playerstate 4 targetable]} >= 2 && !${SpawnCount[pc enchanter radius ${combatRadius}*2]}) {
			/if (${validate_cast[FALSE, spell, "${AELifeTap}", ${Target2Kill}]}) /call core_cast2 "${AELifeTap}" 0 ${Target2Kill} FALSE
		}
	}
	
	| fake hate
	/if (${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) { 
		/if (${validate_cast[FALSE, alt, "Ageless Enmity", ${Target2Kill}]}) /call core_cast2 "Ageless Enmity" alt ${Target2Kill} FALSE
	}
	| named fake hate
	/if (${NameList.Find[${Target.DisplayName}]} && !${switchOverrideName}) {
		/if (${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
			/if (${validate_cast[FALSE, alt, "Mindless Hatred", ${Target2Kill}]} && ${Target.Level} <= 110) /call core_cast2 "Mindless Hatred" alt ${Target2Kill} FALSE
		}
		/if (${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) { 
			/if (${validate_cast[FALSE, disc, "${DiscFakeHate}", ${Target2Kill}]}) /disc ${DiscFakeHate}
		}
	}
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_SHD()
	DEBUG click_DPS_SHD()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/call use_skill_melee ${Target2Kill}
	/call click_horn_of_unity 	
/return



|***
 *	DES: keeps Shadowknigt Vizat's Skin
 *	USE: /call check_skin
 *	INI: [buffs] - Steel
 *  NOTE: 
 ***|
sub check_skin()
	DEBUG check_skin()
	AMIDEAD
	SIMPLEEXIT
	/if (${useShieldFlash}) /call cast_AA_shield_flash
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return	
	/if (!${Bool[${Me.Buff[${Skin}]}]} && ${Me.SpellReady[${Skin}]}) {
		/if (${validate_cast[FALSE, spell, "${Skin}", ${Me.ID}]}) /call core_cast2 "${Skin}" 0 0 FALSE
		/call create_timer timer_check_Skin ${tcheck_Skin}
	}
/return



 |***
 *	DES: Shadowknight burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_SHD()
	DEBUG synergy_SHD()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return		
	
	| defensive	
	/declare _count int local 0
	/for _count 1 to 3
		/if (${Bool[${Me.ActiveDisc}]}) /break
		/if (${validate_cast[FALSE, disc, "${DefensiveDisc${_count}}", ${Me.ID}]}) {
			/call core_cast2 "${DefensiveDisc${_count}}" disc 0 FALSE
			/break
		}
	/next _count
	
	| deflection
	/if (!${Bool[${Me.ActiveDisc}]} && ${Me.PctHPs} < 50*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, disc, "Deflection Discipline", ${Me.ID}]}) {
			/call core_cast2 "Deflection Discipline" disc 0 FALSE
			/return
		}
	}	
	
	/if (${useShieldFlash}) /call cast_AA_shield_flash


	/if (${validate_cast[FALSE, alt, "T`Vyl's Resolve", ${Me.ID}]}) /call core_cast2 "T`Vyl's Resolve" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${CrimsonBlade}", ${Me.ID}]}) /call core_cast2 "${CrimsonBlade}" disc 0 FALSE
	/call cast_AA_spire
	/if (${validate_cast[FALSE, alt, "Veil of Darkness", ${Me.ID}]}) /call core_cast2 "Veil of Darkness" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "Harm Touch", ${Me.ID}]}) /call core_cast2 "Harm Touch" alt ${Target2Kill} FALSE
	/if (${switchAgro}) {
		/if (${validate_cast[FALSE, alt, "Scourge Skin", ${Me.ID}]}) /call core_cast2 "Scourge Skin" alt 0 FALSE
	}

	| AA Visage of Decay/Death stacking issues
	/if (!${Me.Buff[Visage of Decay].ID}) {
		/if (${validate_cast[FALSE, alt, "Visage of Death", ${Me.ID}]}) /call core_cast2 "Visage of Death" alt 0 FALSE
	} else /if (!${Me.AltAbilityReady[Visage of Death]} && !${Me.Buff[Visage of Death].ID}) {
		/if (${validate_cast[FALSE, alt, "Visage of Decay", ${Me.ID}]})	/call core_cast2 "Visage of Decay" alt 0 FALSE
	}

	
	| epic 2.0
	/if (${validate_cast[FALSE, epicitem, "2", ${Me.ID}]} && ${Spawn[${Target2Kill}].Distance} <= ${CombatDistance}) /call core_cast2 "${${Me.Class.ShortName}Epic2}" item 0 FALSE

	/call use_glyph	
	/call synergy_click_Circle
	/call click_item_burn
/return



|***
 *	DES: Class controls
 *	USE: /shd [] [##]
 *	INI: 
 *  NOTE:
 ***|
#bind setSHD /shd
sub Bind_setSHD(string _classType, string _verbage)
	/if (!${validate_class[TRUE, |SHD]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _type string	local |ae|defcount|fade|harmonious|healpoint|rod|shieldflash|skin|spiregroup|spireraid|stance|taunt|vot

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
	
	| Shield Flash oh shit
	/if (${Bool[${_classType.Equal[shieldflash]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${_verbage}]}) {
			/call writeINI useShieldFlashPct Discs ${_verbage} 
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${_verbage}]}) {
			VOUT /${_bind} shieldflash [${cinfo}##\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	else /if (!${Bool[${_verbage}]}) {
			/call writeINI useShieldFlash Discs ${If[${useShieldFlash},FALSE,TRUE]}  
		}			

	| use Modulation Rods
	} else /if (${Bool[${_classType.Equal[rod]}]}) {
		/call writeINI switchModRod Buffs ${If[${switchModRod},FALSE,TRUE]} 	

	| on/off switch for Vizat's Skin
	} else /if (${Bool[${_classType.Find[skin]}]}) {
		/call writeINI switchSkin Buffs ${If[${switchSkin},FALSE,TRUE]}  
	
	| on/off switch for Harmonious
	} else /if (${Bool[${_classType.Find[harmonious]}]}) {
		/call writeINI switchHarmonious Buffs ${If[${switchHarmonious},FALSE,TRUE]}  

	| on/off switch for Stance
	} else /if (${Bool[${_classType.Find[stance]}]}) {
		/call writeINI switchStance Buffs ${If[${switchStance},FALSE,TRUE]}  

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

	| on/off taunt
	} else /if (${Bool[${_classType.Equal[taunt]}]}) {
		/call writeINI taunt meleedisc ${If[${taunt},FALSE,TRUE]}  


	| AE or Focused Melee
	} else /if (${Bool[${_classType.Equal[ae]}]}) {
		/call writeINI AEMelee Discs ${If[${AEMelee},FALSE,TRUE]}


	| AA Voice of Thule
	} else /if (${Bool[${_classType.Equal[vot]}]}) {
		/call writeINI switchVoT Buffs ${If[${switchVoT},FALSE,TRUE]}


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
		/if (${_type.Arg[${_count},|].Equal[defcount]}) /varset _listout ${_listout}(${cnum}${defCount}\ax)
		/if (${_type.Arg[${_count},|].Equal[harmonious]}) /varset _listout ${_listout}(${If[${Bool[${switchHarmonious}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[skin]}) /varset _listout ${_listout}(${If[${Bool[${switchSkin}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[stance]}) /varset _listout ${_listout}(${If[${Bool[${switchStance}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[ae]}) /varset _listout ${_listout}(${If[${Bool[${AEMelee}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[shieldflash]}) /varset _listout ${_listout}(${If[${useShieldFlash},${cnum}${useShieldFlashPct}\ax,${cbad}${useShieldFlashPct}\ax]})
		/if (${_type.Arg[${_count},|].Equal[taunt]}) /varset _listout ${_listout}(${If[${Bool[${taunt}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[vot]}) /varset _listout ${_listout}(${If[${Bool[${switchVoT}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[rod]}) /varset _listout ${_listout}(${If[${Bool[${switchModRod}]},${oon},${ooff}]})
		/if (${_type.Arg[${_count},|].Equal[fade]}) /varset _listout ${_listout}(${If[${switchAAFade},${cnum}${useAAFadePct}\ax,${cbad}${useAAFadePct}\ax]})

		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return
switchVoT