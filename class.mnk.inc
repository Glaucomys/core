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
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}()\ax
	/doevents
	AMIDEAD
	CHECKTIE

	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /return
	
	CHECKREZ
	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/call stick_to_target
		/call use_skill_melee ${Target2Kill}
		/call check_deBuffs ${Target2Kill}
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/call stick_to_target
		/call use_skill_melee ${Target2Kill}
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/call click_item
		/call click_DPS_${Me.Class.ShortName}		
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
		/call set_combat RANGE
		/call use_skill_melee ${Target2Kill}
		/call check_deBuffs ${Target2Kill}
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
	/if (${coreAuto}) {
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
	}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} && !${is_target_dead[${Target2Kill}]}) /goto :killtarget
	/if (!${coreAuto} && ${Target.ID} && ${Bool[${combat}]} && !${is_target_dead[${Target.ID}]}) /goto :killtarget
/return
	
	
	
	|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub disc_MNK()
	DEBUGCLASS ${cbug}disc_MNK()\ax
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return
	/call check_deBuffs ${Target2Kill}
	/call get_agro_${Me.Class.ShortName}
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}


	/if (!${switchAgro} && ${Me.PctAggro} >= ${setFDAgroPct}) {
		/doability "Feign Death" 
		/delay 1
	}


	/call core_cast "Five Point Palm" alt ${Target2Kill} FALSE
	/call core_cast "Two-Finger Wasp Touch" alt ${Target2Kill} FALSE
	
	/call core_cast "Drunken Monkey Style" disc 0 FALSE
	/call core_cast "Whorl of Fists" disc 0 FALSE

	/call core_cast "Zan Fi's Whistle" alt ${Target2Kill} FALSE
		
	/call core_cast "Drunken Monkey Style" disc 0 FALSE
	/call core_cast "Torrent of Fists Rk. II" disc 0 FALSE
	/call core_cast "Doomwalker's Synergy" disc 0 FALSE
	/call core_cast "Zalikor's Fang" disc 0 FALSE
	/call core_cast "Curse of the Thirteen Fingers" disc 0 FALSE

	/call core_cast "Five Point Palm" alt ${Target2Kill} FALSE
	/call core_cast "Zan Fi's Whistle" alt ${Target2Kill} FALSE
	/call core_cast "Two-Finger Wasp Touch" alt ${Target2Kill} FALSE
		
	/call core_cast "Drunken Monkey Style" disc 0 FALSE
	/call core_cast "Torrent of Fists Rk. II" disc 0 FALSE
	/call core_cast "Doomwalker's Synergy" disc 0 FALSE
	/call core_cast "Zalikor's Fang" disc 0 FALSE
	/call core_cast "Curse of the Thirteen Fingers" disc 0 FALSE

/return





|***
 *	DES: emergency healing
 *	USE: /call patch_heal_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_MNK()
	DEBUGCLASS ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	CHECKEXIT
	AMIDEAD
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	/if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /doability Mend
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
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	CHECKEXIT
	/doevents
	/call click_horn_of_unity 	
/return



|***
 *	DES: Agro Routines
 *	USE: /call get_agro_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: manual/Named/raid. all sorts...
 ***|
sub get_agro_MNK()
	DEBUGCLASS ${cbug}get_agro_${Me.Class.ShortName}()\ax
	CHECKEXIT
/return



 |***
 *	DES: Monk burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_MNK()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	CHECKEXIT
	/doevents
	
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
	DEBUG ${cbug}melee_train_MNK()\ax

	| kick skills
	/if (${Me.Skill[Kick]} <= ${Me.Skill[Flying Kick]} && ${Me.Skill[Kick]} <= ${Me.Skill[Round Kick]}) {
		SDEBUG setting: Kick
		/varset kick TRUE
		/varset flyingkick FALSE
		/varset roundkick FALSE
	} else /if (${Me.Skill[Flying Kick]} <= ${Me.Skill[Kick]} && ${Me.Skill[Flying Kick]} <= ${Me.Skill[Round Kick]}) {
		SDEBUG setting: Flying Kick
		/varset kick FALSE
		/varset flyingkick TRUE
		/varset roundkick FALSE
	} else /if (${Me.Skill[Round Kick]} <= ${Me.Skill[Kick]} && ${Me.Skill[Round Kick]} <= ${Me.Skill[Flying Kick]}) {
		SDEBUG setting: Round Kick
		/varset kick FALSE
		/varset flyingkick FALSE
		/varset roundkick TRUE
	}

	| punch skills
	/if (${Me.Skill[Dragon Punch]} <= ${Me.Skill[Eagle Strike]} && ${Me.Skill[Dragon Punch]} <= ${Me.Skill[Tiger Claw]}) {
		SDEBUG setting: Dragon Punch
		/varset dragonpunch TRUE
		/varset eaglestrike FALSE
		/varset tigerclaw FALSE
	} else /if (${Me.Skill[Eagle Strike]} <= ${Me.Skill[Dragon Punch]} && ${Me.Skill[Eagle Strike]} <= ${Me.Skill[Tiger Claw]}) {
		SDEBUG setting: Eagle Strike
		/varset dragonpunch FALSE
		/varset eaglestrike TRUE
		/varset tigerclaw FALSE
	} else /if (${Me.Skill[Tiger Claw]} <= ${Me.Skill[Dragon Punch]} && ${Me.Skill[Tiger Claw]} <= ${Me.Skill[Eagle Strike]}) {
		SDEBUG setting: Tiger Claw
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
	DEBUG ${cbug}Bind_setMNK(type:${_classType}, verbage:${_verbage})\ax
	/if (!${validate_class[TRUE, |MNK]}) /return

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
	
	| set FD agro PCT
	} else /if (${Bool[${_classType.Find[fdpct]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI setFDAgroPct Melee ${_verbage}
		} else /if (!${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} fdpct(${cnum}${setFDAgroPct}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
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
			VOUT /${_bind} healpoint(${cnum}${${Me.Class.ShortName}Point}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}

	| enable/disable training melee punches and kicks
	/if (${Bool[${_classType.Find[trainmelee]}]}) {
		/call writeINI switchMNKTrainMelee Common ${If[${switchMNKTrainMelee},FALSE,TRUE]}
		VOUT /${_bind} trainmelee ${sep} ${If[${switchMNKTrainMelee},${oenabled},${odisabled}]}
		/return

	} else {
		/for _count 1 to ${${_bind}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
			
			/if (${${_bind}Type.Arg[${_count},|].Equal[fdpct]}) /varset _listout ${_listout}(${cnum}${setFDAgroPct}\ax)
			/if (${${_bind}Type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
			/if (${${_bind}Type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
			/if (${${_bind}Type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}${${Me.Class.ShortName}Point}\ax)
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
	} 
/return

