|***
 *	class.mnk.inc
 *	Core Monk Include
 *
 *
 ***|
 


|***
 *	DES: Main Monk Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_MNK()
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
sub disc_MNK()
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return
	/call check_deBuffs ${Target2Kill}
	/call get_agro_${Me.Class.ShortName}


	/if (!${switchAgro} && ${Me.PctAggro} >= ${setFDAgroPct}) {
		/doability "Feign Death" 
		/delay 1
	}

	/if (${validate_cast[FALSE, alt, "Five Point Palm", ${Target2Kill}]}) /call core_cast2 "Five Point Palm" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "Two-Finger Wasp Touch", ${Target2Kill}]}) /call core_cast2 "Two-Finger Wasp Touch" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "Whorl of Fists", ${Me.ID}]}) /call core_cast2 "Whorl of Fists" disc 0 FALSE
	/if (${validate_cast[FALSE, alt, "Zan Fi's Whistle", ${Target2Kill}]}) /call core_cast2 "Zan Fi's Whistle" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "Drunken Monkey Style", ${Me.ID}]}) /call core_cast2 "Drunken Monkey Style" disc 0 FALSE
	/if (${validate_cast[FALSE, disc, "Doomwalker's Synergy", ${Me.ID}]}) /call core_cast2 "Doomwalker's Synergy" disc 0 FALSE
	/if (${validate_cast[FALSE, disc, "Zalikor's Fang", ${Me.ID}]}) /call core_cast2 "Zalikor's Fang" disc 0 FALSE
	/if (${validate_cast[FALSE, disc, "Curse of the Thirteen Fingers", ${Me.ID}]}) /call core_cast2 "Curse of the Thirteen Fingers" disc 0 FALSE
	/if (${validate_cast[FALSE, alt, "Five Point Palm", ${Target2Kill}]}) /call core_cast2 "Five Point Palm" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "Curse of the Thirteen Fingers", ${Me.ID}]}) /call core_cast2 "Curse of the Thirteen Fingers" disc 0 FALSE

/return



|***
 *	DES: emergency healing
 *	USE: /call patch_heal_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_MNK()
	CHECKEXIT
	AMIDEAD
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	/if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.${tmpHealOverride} || ${switchMNKTrainMelee}) /doability Mend
	/if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /call core_cast "Defer Death" disc 0 FALSE
	/call click_item_heal
/return



|***
 *	DES: DPS Clickies
 *	USE: /call click_DPS_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_MNK()
	CHECKEXIT
	CHECKINPUTS
	/call click_horn_of_unity 	
/return



|***
 *	DES: Agro Routines
 *	USE: /call get_agro_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: manual/Named/raid. all sorts...
 ***|
sub get_agro_MNK()
	CHECKEXIT
/return



 |***
 *	DES: Monk burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_MNK()
	CHECKEXIT
	CHECKINPUTS
	
	/call cast_AA_spire
	
	| AA Zan Fi's Whistle - DPS increase
	| aa Focused Destructive Force
	| AA Silent Strikes
	| aa five point palm
	
	/call use_glyph
	/call synergy_click_Circle
	/call click_item_burn
/return


|***
 *	DES: swap melee skills to train them
 *	USE: /mnk trainmelee | /call melee_train_MNK
 *	INI: 
 *  NOTE:
 ***|
sub melee_train_MNK()
	
	| mend training
	/if (${switchMNKTrainMelee} && ${Me.Skill[Mend]} <= ${Me.SkillCap[Mend]} && ${Me.AbilityReady[Mend]}) /doability Mend

	| kick skills
	/if (${Me.Skill[Kick]} <= ${Me.Skill[Flying Kick]} && ${Me.Skill[Kick]} <= ${Me.Skill[Round Kick]}) {
		/varset kick TRUE
		/varset flyingkick FALSE
		/varset roundkick FALSE
	} else /if (${Me.Skill[Flying Kick]} <= ${Me.Skill[Kick]} && ${Me.Skill[Flying Kick]} <= ${Me.Skill[Round Kick]}) {
		/varset kick FALSE
		/varset flyingkick TRUE
		/varset roundkick FALSE
	} else /if (${Me.Skill[Round Kick]} <= ${Me.Skill[Kick]} && ${Me.Skill[Round Kick]} <= ${Me.Skill[Flying Kick]}) {
		/varset kick FALSE
		/varset flyingkick FALSE
		/varset roundkick TRUE
	}

	| punch skills
	/if (${Me.Skill[Dragon Punch]} <= ${Me.Skill[Eagle Strike]} && ${Me.Skill[Dragon Punch]} <= ${Me.Skill[Tiger Claw]}) {
		/varset dragonpunch TRUE
		/varset eaglestrike FALSE
		/varset tigerclaw FALSE
	} else /if (${Me.Skill[Eagle Strike]} <= ${Me.Skill[Dragon Punch]} && ${Me.Skill[Eagle Strike]} <= ${Me.Skill[Tiger Claw]}) {
		/varset dragonpunch FALSE
		/varset eaglestrike TRUE
		/varset tigerclaw FALSE
	} else /if (${Me.Skill[Tiger Claw]} <= ${Me.Skill[Dragon Punch]} && ${Me.Skill[Tiger Claw]} <= ${Me.Skill[Eagle Strike]}) {
		/varset dragonpunch FALSE
		/varset eaglestrike FALSE
		/varset tigerclaw TRUE
	}
/return



|***
 *	DES: Class controls
 *	USE: /mnk []
 *	INI: 
 *  NOTE:
 ***|
#bind setMNK /mnk
sub Bind_setMNK(string _classType, string _verbage)
	/if (!${validate_class[TRUE, |MNK]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _type string	local |fdpct|healpoint|spiregroup|spireraid|trainmelee

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
	
	| set FD agro PCT
	/if (${Bool[${_classType.Find[fdpct]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI setFDAgroPct Melee ${_verbage}
		} else /if (!${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} fdpct(${cnum}${setFDAgroPct}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	
	
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
			VOUT /${_bind} healpoint(${cnum}${${Me.Class.ShortName}Point}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}

	| enable/disable training melee punches and kicks
	} else /if (${Bool[${_classType.Find[trainmelee]}]}) {
		/call writeINI switchMNKTrainMelee Common ${If[${switchMNKTrainMelee},FALSE,TRUE]}
		VOUT /${_bind} trainmelee ${sep} ${If[${switchMNKTrainMelee},${oenabled},${odisabled}]}
		/return

	}
	
	/if (${switchMQclearonCLS}) /mqclear
	/for _count 1 to ${_type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_type.Arg[${_count},|]}\ax
		/if (${_type.Arg[${_count},|].Equal[fdpct]}) /varset _listout ${_listout}(${cnum}${setFDAgroPct}\ax)
		/if (${_type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${_type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${_type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}${${Me.Class.ShortName}Point}\ax)
		/if (${_type.Arg[${_count},|].Equal[ae]}) /varset _listout ${_listout}(${If[${AEMelee},${oon},${ooff}]})
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]

/return

