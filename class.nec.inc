|***
 *	class.nec.inc
 *	Core Necro Include
 *
 *
 ***|



|***
 *	DES: Main Necro Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_NEC()
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}()\ax
	/doevents
	AMIDEAD
	CHECKTIE
	CHECKREZ
	/if (${coreAuto} && !${Me.Invis}) {
		/call check_engaged
		/if (!${Macro.Return}) {
			/if (${switchPcPet}) /call check_pet
		}
	}


	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /return

	CHECKREZ
	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
		/call set_combat NUKE
		/if (${coreAuto} && ${switchPcPet} && ${Pet.ID}) /call send_pet
		/call check_deBuffs ${Target2Kill}
		/call disc_${Me.Class.ShortName}
		/call check_deBuffs ${Target2Kill}
		/call click_BP
		/call click_DPS_${Me.Class.ShortName}
		/if (!${timer_check_blood}) /call cast_blood ${Target2Kill}
		/if (${DoNuke}) /call cast_nuke
		/if (${DoDoT}) /call cast_DOT
		/call click_item TRUE
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
	}
	CHECKREZ
	/if (${coreAuto}) {	
		/call use_mod_rod
		/if (${switchHealCure}) /call cast_cure
		/if (${switchHealPet} && ${Me.Pet.ID}) /call pet_heal_${Me.Class.ShortName}
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
sub pet_heal_NEC()
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub disc_NEC()
	DEBUGCLASS ${cbug}disc_${Me.Class.ShortName}()\ax 
	CHECKEXIT
	/doevents
	CHECKTIE
	/call is_target_dead
	/if (!${Macro.Return}) /return
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}

	| heal clickies
	/if (${switchHealSelf} && ${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0 FALSE
	/if (${switchHealSelf} && ${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0 FALSE

	| aa wake the dead
	/if (${switchPcSwarm} && ${Target.PctHPs} > 15 && ${SpawnCount[corpse radius ${Spell[Wake the Dead].MyRange}]} > 3) /call core_cast "Wake the Dead" alt ${Target2Kill} FALSE
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_NEC()
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	CHECKEXIT
	CHECKTIE
	/doevents
	/call click_horn_of_unity 	
/return



|***
 *	DES: casts call of blood line of spells
 *	USE: /call cast_blood [ID]
 *	INI: 
 *  NOTE: 
 ***|
sub cast_blood(int _tmpID)
	CHECKEXIT
	/doevents
	/call create_timer timer_check_blood ${tCheck_Blood}
	/if (${Bool[${Me.Song[Chaotic Power]}]}) /return
	/call is_target_dead
	/if (${Macro.Return}) /return
	/call core_cast "${spellBlood}" 0 ${_tmpID}	FALSE
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
sub sac_toon()
	/declare sactarget string local NAMEHERE
	/if (${Spawn[pc ${sactarget}].ID}) {
		 /delay 1
		 /target id ${spawn[${sactarget}].ID}
		/delay 1
		/autoinventory
		/call MQ2Cast "Sacrifice" gem1 5s -targetID|${Spawn[${sactarget}].ID} 
		/delay 15s ${Cursor.ID}
		/autoinventory
		/delay 3s
	}
/return
 ***|



 |***
 *	DES: Necromancer burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_NEC()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	CHECKEXIT
	/doevents
	
	/call cast_AA_spire
	
	| aa swarm of decay
	/if (${switchPcSwarm}) /call core_cast "Swarm of Decay" alt ${Target2Kill} FALSE
	| aa Rise of bones
	/if (${switchPcSwarm}) /call core_cast "Rise of Bones" alt ${Target2Kill} FALSE

	/call use_glyph
	/call synergy_click_Circle
	/call click_item_burn
/return



|***
 *	DES: Class controls
 *	USE: /nec []
 *	INI: 
 *  NOTE:
 ***|
#bind setNEC /nec
sub Bind_setNEC(string _classType, string _classVerbage)
	DEBUG ${cbug}Bind_setNEC(type:${_classType}, verbage:${_classVerbage})\ax
	/if (!${validate_class[TRUE, |NEC]}) /return

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
	
	/if (${Bool[${_classType.Equal[TEST]}]}) {

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