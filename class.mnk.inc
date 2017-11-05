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

	/if (${Bool[${RangeItemSummon}]}) /call check_ammo

	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /return
	
	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/if (${isValidTarget}) /call stick_to_target
		/call set_combat MELEE
		/call stick_to_target
		/call use_skill_melee ${Target2Kill}
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/call stick_to_target
		/call use_skill_melee ${Target2Kill}
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}		
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
	}
	/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} || !${coreAuto} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Macro.Return}) /goto :killtarget
	}
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
	/call is_target_dead
	/if (${Macro.Return}) /return
	/call check_deBuffs ${Target2Kill}
	/call get_agro_${Me.Class.ShortName}
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}

	/if (${Me.AltAbilityReady[Five Point Palm]}) /call core_cast "Five Point Palm" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[Eye Gouge]} && !${switchdeBuffSnare}) /call core_cast "Eye Gouge" alt ${Target2Kill} 
	/if (${Me.AltAbilityReady[Stunning Kick]}) /call core_cast "Stunning Kick" alt ${Target2Kill} 
	/if (${Me.AltAbilityReady[Two-Finger Wasp Touch]}) /call core_cast "Two-Finger Wasp Touch" alt ${Target2Kill}
	
	/if (${Me.CombatAbilityReady[Drunken Monkey Style]} && ${Me.CurrentEndurance} > ${Spell[Drunken Monkey Style].EnduranceCost}) /call core_cast "Drunken Monkey Style" disc 0
	/if (${Me.CombatAbilityReady[Whorl of Fists]} && ${Me.CurrentEndurance} > ${Spell[Whorl of Fists].EnduranceCost}) /call core_cast "Whorl of Fists" disc 0

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
	/if (${Me.PctHPs} > 81*.${tmpHealOverride}) /return
	
	/if (${Me.PctHPs} <= 50*.${tmpHealOverride}) /doability Mend
	/if (${Me.CombatAbilityReady[Defer Death]} && ${Me.PctHPs} <= 50*.${tmpHealOverride}) /call core_cast "Defer Death" disc 0

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
	/if (${doclickItemBurn}) /call click_item_burn
/return



|***
 *	DES: Class controls
 *	USE: /mnk []
 *	INI: 
 *  NOTE:
 ***|
#bind setMNK /mnk
sub Bind_setMNK(string _classType, string _classVerbage)
	DEBUG ${cbug}Bind_setMNK(type:${_classType}, verbage:${_classVerbage})\ax
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
	} else /if (${Bool[${${_bind}Type.Find[${_classType}]}]} && ${_classVerbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_classType.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_classType.Lower}]}
		/return
	}	
	/if (${Bool[${_classType.Equal[TEST]}]}) {
		| /call Event_SetVar NA switchNukeSummer Spells ${If[${switchNukeSummer},FALSE,TRUE]}

	| spire control
	} else /if (${Bool[${_classType.Equal[spiregroup]}]} || ${Bool[${_classType.Equal[spireraid]}]}) {
		/declare _spiretype string local ${_classType.Left[5].Lower}${_classType.Mid[6,1].Upper}${_classType.Right[-6].Lower}
		/if (${Range.Between[0,3:${Int[${_classVerbage}]}]}) {
			/call writeINI ${_spiretype} Buffs ${_classVerbage}
		} 
		/declare _listout 	string 	local
		/declare _sep 			bool 		local FALSE
		/for _count 1 to ${typeSpire.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${${_spiretype}} == ${typeSpire.Arg[${_count},|]},${cgood},${cinfo}]}${typeSpire.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} ${_classType.Lower} [${_listout}]	


	} else {
		/for _count 1 to ${${_bind}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
	} 
/return