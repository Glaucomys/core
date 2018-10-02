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

	/if (${coreAuto}) /call check_fd

	/while (${prep_combat[]}) {
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${switchHealCure}) /call cast_cure
		/if (${exit_class_loop[]}) /return

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${setBurnControl}) /call synergy_${Me.Class.ShortName}
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
			/call check_fd
			/if (${validate_class[FALSE, ${MeleeList}]}) /call cast_stamina_recovery
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

	/if (${Me.PctAggro} >= ${setAgroPct}) {
		/if (${validate_cast[FALSE, alt, "${AASilent}", ${Me.ID}]}) /call core_cast2 "${AASilent}" alt 0 FALSE
	}
	/if (${validate_cast[FALSE, disc, "${DiscDoubleAttack}", ${Me.ID}]}) /call core_cast2 "${DiscDoubleAttack}" disc 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAZanFi}", ${Me.ID}]}) /call core_cast2 "${AAZanFi}" alt 0 FALSE
	/if (${validate_cast[FALSE, disc, "${DiscMonkey}", ${Target2Kill}]}) /call core_cast2 "${DiscMonkey}" disc ${Target2Kill} FALSE	
	/if (${Me.AbilityReady["Intimidation"]}) /doability Intimidation  
	/if (${validate_cast[FALSE, alt, "${AAWasp}", ${Target2Kill}]}) /call core_cast2 "${AAWasp}" alt ${Target2Kill} FALSE   
	/if (${validate_cast[FALSE, disc, "${DiscFists}", ${Target2Kill}]}) /call core_cast2 "${DiscFists}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscSynergy}", ${Target2Kill}]}) /call core_cast2 "${DiscSynergy}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscFang}", ${Target2Kill}]}) /call core_cast2 "${DiscFang}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscCurse}", ${Target2Kill}]}) /call core_cast2 "${DiscCurse}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscThrowing}", ${Target2Kill}]}) /call core_cast2 "${DiscThrowing}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscShuriken}", ${Target2Kill}]}) /call core_cast2 "${DiscShuriken}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscStorm}", ${Me.ID}]}) /call core_cast2 "${DiscStorm}" disc 0 FALSE

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
	
	| mend training
	/if (${switchMNKTrainMelee}) {
		/if (${Me.Skill[Mend]} < ${Me.SkillCap[Mend]} && ${Me.AbilityReady[Mend]}) {
			/doability Mend
			/return
			}
	}

	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return

	/if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.${tmpHealOverride} && ${Me.AbilityReady[Mend]}) {
		/doability Mend
		/return
	}

	/if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) {
		/if (${validate_cast[FALSE, disc, "${DiscDelayDeath}", ${Me.ID}]}) /call core_cast2 "${DiscDelayDeath}" disc 0 FALSE
	}
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
	
	/if (${validate_cast[FALSE, disc, "${DiscDicho}", ${Me.ID}]}) /call core_cast2 "${DiscDicho}" disc 0 FALSE
	/if (${validate_cast[FALSE, disc, "${MNKAlliance}", ${Target2Kill}]}) /call core_cast2 "${MNKAlliance}" disc ${Target2Kill} FALSE 
	/call cast_AA_spire
	/if (${validate_cast[FALSE, disc, "${DiscBalance}", ${Me.ID}]}) /call core_cast2 "${DiscBalance}" disc 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAThunder}", ${Me.ID}]}) /call core_cast2 "${AAThunder}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAFocus}", ${Target2Kill}]}) /call core_cast2 "${AAFocus}" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscMonkey}", ${Me.ID}]}) /call core_cast2 "${DiscMonkey}" disc 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AASilent}", ${Me.ID}]}) /call core_cast2 "${AASilent}" alt 0 FALSE
	
| 	Check if we're using a disc, if not use Heel, if another, cancel it and use Heel

	/if (!${Bool[${Me.ActiveDisc.Name}]}) {
		/if (${validate_cast[FALSE, disc, "${DiscHeel}", ${Target2Kill}]}) {
		/if (!${flyingkick} || !${eaglestrike}) {
			/varset kick FALSE
			/varset flyingkick TRUE
			/varset roundkick FALSE
			/varset dragonpunch FALSE
			/varset eaglestrike TRUE
			/varset tigerclaw FALSE
		}
		/call core_cast2 "${DiscHeel}" disc ${Target2Kill} FALSE
		}
	} else /if (${Bool[${Me.ActiveDisc.Name}]} && !${Bool[${Me.ActiveDisc.Name.Find[Heel]}]}) {
		/if (${validate_cast[FALSE, disc, "${DiscHeel}", ${Target2Kill}]}) {
			/stopdisc
			/if (!${flyingkick} || !${eaglestrike}) {
				/varset kick FALSE
				/varset flyingkick TRUE
				/varset roundkick FALSE
				/varset dragonpunch FALSE
				/varset eaglestrike TRUE
				/varset tigerclaw FALSE
			}
			/call core_cast2 "${DiscHeel}" disc ${Target2Kill} FALSE
		}
	}
	/if (${validate_cast[FALSE, disc, "${DiscAshenhand}", ${Me.ID}]}) /call core_cast2 "${DiscAshenhand}" disc 0 FALSE
	/if (${validate_cast[FALSE, disc, "${DiscCrane}", ${Target2Kill}]}) /call core_cast2 "${DiscCrane}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "${AATwinStrike}", ${Target2Kill}]}) /call core_cast2 "${AATwinStrike}" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "${AAChant}", ${Me.ID}]}) /call core_cast2 "${AAChant}" alt 0 FALSE    

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
	
| This code catches the call in the skillup sub and returns out using most efficient skills if not training melee
	/if (!${switchMNKTrainMelee}) {
		/varset kick FALSE
		/varset flyingkick TRUE
		/varset roundkick FALSE
		/varset dragonpunch FALSE
		/varset eaglestrike TRUE
		/varset tigerclaw FALSE
		/return
	}

	| kick skills
	/if (${Me.Skill[Flying Kick]} <= ${Me.Skill[Kick]} && ${Me.Skill[Flying Kick]} <= ${Me.Skill[Round Kick]}) {
		/varset kick FALSE
		/varset flyingkick TRUE
		/varset roundkick FALSE
	} else /if (${Me.Skill[Kick]} <= ${Me.Skill[Flying Kick]} && ${Me.Skill[Kick]} <= ${Me.Skill[Round Kick]}) {
		/varset kick TRUE
		/varset flyingkick FALSE
		/varset roundkick FALSE
	} else /if (${Me.Skill[Round Kick]} <= ${Me.Skill[Kick]} && ${Me.Skill[Round Kick]} <= ${Me.Skill[Flying Kick]}) {
		/varset kick FALSE
		/varset flyingkick FALSE
		/varset roundkick TRUE
	}

	| punch skills
	/if (${Me.Skill[Eagle Strike]} <= ${Me.Skill[Dragon Punch]} && ${Me.Skill[Eagle Strike]} <= ${Me.Skill[Tiger Claw]}) {
		/varset dragonpunch FALSE
		/varset eaglestrike TRUE
		/varset tigerclaw FALSE
	} else /if (${Me.Skill[Tiger Claw]} <= ${Me.Skill[Dragon Punch]} && ${Me.Skill[Tiger Claw]} <= ${Me.Skill[Eagle Strike]}) {
		/varset dragonpunch FALSE
		/varset eaglestrike FALSE
		/varset tigerclaw TRUE
	} else /if (${Me.Skill[Dragon Punch]} <= ${Me.Skill[Eagle Strike]} && ${Me.Skill[Dragon Punch]} <= ${Me.Skill[Tiger Claw]}) {
		/varset dragonpunch TRUE
		/varset eaglestrike FALSE
		/varset tigerclaw FALSE
	}
/return



#event MNKfdcheck "You have fallen to the ground"
sub Event_MNKfdcheck(string line, string _verbage)
	/if (!${validate_class[FALSE, |MNK]}) /return
	/if (${Me.State.Equal[FEIGN]} && !${switchAmIDead}) {
		/if (${Me.AltAbilityReady["${MNKAAFade}"]}) {
			/if (${validate_cast[FALSE, alt, "${MNKAAFade}", ${Me.ID}]}) /call core_cast2 "${MNKAAFade}" alt 0 FALSE
			/return
		}
		/stand
		/if (${Me.AbilityReady["Mend"]}) /doability Mend
		/if (${validate_cast[FALSE, disc, "${DiscDelayDeath}", ${Me.ID}]}) /call core_cast2 "${DiscDelayDeath}" disc 0 FALSE
	}
/return



|***
 *  DES: To FD when hitpoints get low, cancel combat if driving, or stand up if healthy
 *  USE: 
 *  INI: 
 *  NOTE: 
 ***|   
sub check_fd()
	AMIDEAD
	/if (${Me.Combat} && ${Me.PctHPs} <= ${setFDPct}) {
		/if (!${Me.State.Equal[FEIGN]} && !${switchAmIDead}) {
			/if (${Me.AbilityReady[Feign Death]}) {
				/attack off
				/doability "Feign Death"
				/delay 5
			}
			/doevents MNKfdcheck
			/if (!${coreAuto}) {
				/call set_stop
				/call clear_combat
				/return
			}
		}
	}

	/if (${Me.State.Equal[FEIGN]}) {
		/if (${Me.PctHPs} > ${setFDPct} && ${coreAuto} && ${Me.PctAggro} <= ${setAgroPct}) /stand
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
  /declare _type string	local |ae|agpct|fdpct|healpoint|spiregroup|spireraid|trainmelee
	
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

	| set FD HP PCT
	/if (${Bool[${_classType.Find[fdpct]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI setFDAgroPct Melee ${_verbage}
		} else /if (!${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} fdpct(${cnum}${setFDAgroPct}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}	
		
	| set FD agro PCT
	} else /if (${Bool[${_classType.Find[agpct]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI setAgroPct Melee ${_verbage}
		} else /if (!${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} agpct(${cnum}${setAgroPct}\ax) allowable range ${sep} ${cinfo}1 - 99\ax
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

	| AE or Focused Melee
	} else /if (${Bool[${_classType.Equal[ae]}]}) {
		/call writeINI AEMelee Discs ${If[${AEMelee},FALSE,TRUE]}

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
		/if (${${_bind}Type.Arg[${_count},|].Equal[agpct]}) /varset _listout ${_listout}(${cnum}${setAgroPct}\ax)
		/if (${_type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${_type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${_type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}${${Me.Class.ShortName}Point}\ax)
		/if (${_type.Arg[${_count},|].Equal[ae]}) /varset _listout ${_listout}(${If[|***
 *	DES: Monk burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_MNK()
	CHECKEXIT
	CHECKINPUTS
	
	/if (${validate_cast[FALSE, disc, "${DiscDicho}", ${Me.ID}]}) /call core_cast2 "${DiscDicho}" disc 0 FALSE
	/if (${validate_cast[FALSE, disc, "${MNKAlliance}", ${Target2Kill}]}) /call core_cast2 "${MNKAlliance}" disc ${Target2Kill} FALSE 
	/call cast_AA_spire
	/if (${validate_cast[FALSE, disc, "${DiscBalance}", ${Me.ID}]}) /call core_cast2 "${DiscBalance}" disc 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAThunder}", ${Me.ID}]}) /call core_cast2 "${AAThunder}" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AAFocus}", ${Target2Kill}]}) /call core_cast2 "${AAFocus}" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, disc, "${DiscMonkey}", ${Me.ID}]}) /call core_cast2 "${DiscMonkey}" disc 0 FALSE
	/if (${validate_cast[FALSE, alt, "${AASilent}", ${Me.ID}]}) /call core_cast2 "${AASilent}" alt 0 FALSE
	
| 	Check if we're using a disc, if not use Heel, if another, cancel it and use Heel

	/if (!${Bool[${Me.ActiveDisc.Name}]}) {
		/if (${validate_cast[FALSE, disc, "${DiscHeel}", ${Target2Kill}]}) {
		/if (!${flyingkick} || !${eaglestrike}) {
			/varset kick FALSE
			/varset flyingkick TRUE
			/varset roundkick FALSE
			/varset dragonpunch FALSE
			/varset eaglestrike TRUE
			/varset tigerclaw FALSE
		}
		/call core_cast2 "${DiscHeel}" disc ${Target2Kill} FALSE
		}
	} else /if (${Bool[${Me.ActiveDisc.Name}]} && !${Bool[${Me.ActiveDisc.Name.Find[Heel]}]}) {
		/if (${validate_cast[FALSE, disc, "${DiscHeel}", ${Target2Kill}]}) {
			/stopdisc
			/if (!${flyingkick} || !${eaglestrike}) {
				/varset kick FALSE
				/varset flyingkick TRUE
				/varset roundkick FALSE
				/varset dragonpunch FALSE
				/varset eaglestrike TRUE
				/varset tigerclaw FALSE
			}
			/call core_cast2 "${DiscHeel}" disc ${Target2Kill} FALSE
		}
	}
	/if (${validate_cast[FALSE, disc, "${DiscAshenhand}", ${Me.ID}]}) /call core_cast2 "${DiscAshenhand}" disc 0 FALSE
	/if (${validate_cast[FALSE, disc, "${DiscCrane}", ${Target2Kill}]}) /call core_cast2 "${DiscCrane}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "${AATwinStrike}", ${Target2Kill}]}) /call core_cast2 "${AATwinStrike}" alt ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "${AAChant}", ${Me.ID}]}) /call core_cast2 "${AAChant}" alt 0 FALSE    

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
	
| This code catches the call in the skillup sub and returns out using most efficient skills if not training melee
	/if (!${switchMNKTrainMelee}) {
		/varset kick FALSE
		/varset flyingkick TRUE
		/varset roundkick FALSE
		/varset dragonpunch FALSE
		/varset eaglestrike TRUE
		/varset tigerclaw FALSE
		/return
	}

	| kick skills
	/if (${Me.Skill[Flying Kick]} <= ${Me.Skill[Kick]} && ${Me.Skill[Flying Kick]} <= ${Me.Skill[Round Kick]}) {
|		SDEBUG setting: Flying Kick
		/varset kick FALSE
		/varset flyingkick TRUE
		/varset roundkick FALSE
	} else /if (${Me.Skill[Kick]} <= ${Me.Skill[Flying Kick]} && ${Me.Skill[Kick]} <= ${Me.Skill[Round Kick]}) {
|		SDEBUG setting: Kick
		/varset kick TRUE
		/varset flyingkick FALSE
		/varset roundkick FALSE
	} else /if (${Me.Skill[Round Kick]} <= ${Me.Skill[Kick]} && ${Me.Skill[Round Kick]} <= ${Me.Skill[Flying Kick]}) {
|		SDEBUG setting: Round Kick
		/varset kick FALSE
		/varset flyingkick FALSE
		/varset roundkick TRUE
	}

	| punch skills
	/if (${Me.Skill[Eagle Strike]} <= ${Me.Skill[Dragon Punch]} && ${Me.Skill[Eagle Strike]} <= ${Me.Skill[Tiger Claw]}) {
|		SDEBUG setting: Eagle Strike
		/varset dragonpunch FALSE
		/varset eaglestrike TRUE
		/varset tigerclaw FALSE
	} else /if (${Me.Skill[Tiger Claw]} <= ${Me.Skill[Dragon Punch]} && ${Me.Skill[Tiger Claw]} <= ${Me.Skill[Eagle Strike]}) {
|		SDEBUG setting: Tiger Claw
		/varset dragonpunch FALSE
		/varset eaglestrike FALSE
		/varset tigerclaw TRUE
	} else /if (${Me.Skill[Dragon Punch]} <= ${Me.Skill[Eagle Strike]} && ${Me.Skill[Dragon Punch]} <= ${Me.Skill[Tiger Claw]}) {
|		SDEBUG setting: Dragon Punch
		/varset dragonpunch TRUE
		/varset eaglestrike FALSE
		/varset tigerclaw FALSE
	}
/return



#event MNKfdcheck "You have fallen to the ground"
sub Event_MNKfdcheck(string line, string _verbage)
	/if (!${validate_class[FALSE, |MNK]}) /return
	/if (${Me.State.Equal[FEIGN]} && !${switchAmIDead}) {
		/if (${Me.AltAbilityReady["${MNKAAFade}"]}) {
			/if (${validate_cast[FALSE, alt, "${MNKAAFade}", ${Me.ID}]}) /call core_cast2 "${MNKAAFade}" alt 0 FALSE
			/return
		}
		/stand
		/if (${Me.AbilityReady["Mend"]}) /doability Mend
		/if (${validate_cast[FALSE, disc, "${DiscDelayDeath}", ${Me.ID}]}) /call core_cast2 "${DiscDelayDeath}" disc 0 FALSE
	}
/return



|***
 *  DES: To FD when hitpoints get low, cancel combat if driving, or stand up if healthy
 *  USE: 
 *  INI: 
 *  NOTE: 
 ***|   
sub check_fd()
	AMIDEAD
	/if (${Me.Combat} && ${Me.PctHPs} <= ${setFDPct}) {
		/if (!${Me.State.Equal[FEIGN]} && !${switchAmIDead}) {
			/if (${Me.AbilityReady[Feign Death]}) {
				/attack off
				/doability "Feign Death"
				/delay 5
			}
			/doevents MNKfdcheck
			/if (!${coreAuto}) {
				/call set_stop
				/call clear_combat
				/return
			}
		}
	}

	/if (${Me.State.Equal[FEIGN]}) {
		/if (${Me.PctHPs} > ${setFDPct} && ${coreAuto} && ${Me.PctAggro} <= ${setAgroPct}) /stand
	}
/return 
,${oon},${ooff}]})
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]

/return

