6656|***
 *	class.bst.inc
 *	Core Beastlord Include
 *
 *
 ***|
 


|***
 *	DES: Main Beastlord Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_BST()
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}()\ax
	/doevents
	AMIDEAD
	CHECKTIE

	/if (${coreAuto} && !${Me.Invis}) {
		/call cast_AA_focused_paragon
		/call check_engaged
		/if (!${Macro.Return} && ${switchPcPet}) /call check_pet
	}

	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /return

	CHECKREZ
	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/call send_pet
		/call stick_to_target
		/call check_deBuffs ${Target2Kill}
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke
		/if (${${switchPcSwarm}} && ${Bool[${ServantNuke}]}) /call cast_nuke_servant
		/call use_skill_melee ${Target2Kill}
		/call click_item
		/call click_DPS_${Me.Class.ShortName}
		/call cast_AA_focused_paragon
		/if (${switchHealPet}) /call pet_heal_${Me.Class.ShortName}
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
		/call set_combat RANGE
		/call send_pet
		/call click_BP
		/call check_deBuffs ${Target2Kill}
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke
		/if (${${switchPcSwarm}} && ${Bool[${ServantNuke}]}) /call cast_nuke_servant
		/call click_item
		/call click_DPS_${Me.Class.ShortName}
		/call cast_AA_focused_paragon
		/if (${switchHealPet}) /call pet_heal_${Me.Class.ShortName}
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
		/call set_combat NUKE
		/call send_pet
		/call click_BP
		/call check_deBuffs ${Target2Kill}
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke
		/if (${${switchPcSwarm}} && ${Bool[${ServantNuke}]}) /call cast_nuke_servant
		/call click_item
		/call click_DPS_${Me.Class.ShortName}
		/call cast_AA_focused_paragon
		/if (${switchHealPet}) /call pet_heal_${Me.Class.ShortName}
	} else /if (${Select[${combat},HEAL]}) {
	}	else /if (${Select[${combat},DUAL]}) {
	}
	/if (${switchHealCure}) /call cast_cure
	/if (${coreAuto}) {	
		/call use_mod_rod
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${switchHealPet}) /call pet_heal_${Me.Class.ShortName}
	}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} && !${is_target_dead[${Target2Kill}]}) /goto :killtarget
	/if (!${coreAuto} && ${Target.ID} && ${Bool[${combat}]} && !${is_target_dead[${Target.ID}]}) /goto :killtarget
/return



|***
 *	DES: 
 *	USE: /call disc_BST
 *	INI: 
 *  NOTE: 
 ***|
sub disc_BST()
	DEBUGCLASS ${cbug}disc_BST()\ax
	/if (${Me.PctEndurance} < 3) /return
	AMIDEAD
	CHECKEXIT
	/doevents

	/if (${is_target_dead[${Target2Kill}]}) /return
			
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}

	/if (!${Me.Song[${GrowlSpell}].ID} && ${Me.Pet.ID} && !${Me.PetBuff[Savage Rage]} && !${Me.Song[${DiscSavage}].ID}) /call core_cast "${GrowlSpell}" 0 0 FALSE
	/call core_cast "Chameleon Strike" alt ${Target2Kill} FALSE
	| /call core_cast "Feral Swipe" alt ${Target2Kill} FALSE
	| /if (${Me.AltAbilityReady[Bite of the Asp]}) /call core_cast "Bite of the Asp" alt ${Target2Kill} FALSE
	/if (${Me.CombatAbilityReady[${DiscForay}]} && ${Me.PctEndurance} > 5 ) /call core_cast "${DiscForay}" disc ${Target2Kill} FALSE
	/call core_cast "${DiscFocusClaws}" disc ${Target2Kill} FALSE
	/call core_cast "Roar of Thunder" alt ${Target2Kill} FALSE
	/if (!${Bool[${Target.Buff[${DiscEnduring}]}]}) /call core_cast "${DiscEnduring}" alt ${Target2Kill} FALSE
	/if (${Me.PctEndurance} > 5 ) /call core_cast "${DiscRending}" disc ${Target2Kill} FALSE
/return



|***
 *	DES: Focused Paragon. will maintain the bst then group if able
 *	USE: /call cast_AA_focused_paragon
 *	INI: 
 *  NOTE: 
 ***|
sub cast_AA_focused_paragon()
	DEBUGCLASS ${cbug}cast_AA_focused_paragon()\ax
	/if (!${coreAuto}) /return
	AMIDEAD
	CHECKEXIT
	/doevents
	/declare _count 							int 	local 0
	/declare _tmpID 						int 	local 0
	/declare _tmpTarget2Kill		int		local 0
	
	/if (${Target2Kill}) /varset _tmpTarget2Kill ${Target2Kill}
	
	/if (${Me.AltAbilityReady[Consumption of Spirit]} && ${Me.PctHPs} > ${minConsumptionHP}*.${tmpHealOverride} && ${Me.PctMana} < ${selfParagon}*.${tmpHealOverride}) /call core_cast "Consumption of Spirit" alt ${Me.ID} FALSE

	| self only focus
	/if (${Me.AltAbilityReady[Focused Paragon of Spirits]} && ${Me.PctMana} < ${selfParagon} && !${Me.Song[Paragon].ID}) {
		/call core_cast "Focused Paragon of Spirits" alt ${Me.ID} FALSE
		/if (${_tmpTarget2Kill}) {
			/target ID ${_tmpTarget2Kill}
			/delay 1s ${Target.ID} == ${_tmpTarget2Kill}
		}
	}

	| check the group
	/if (${Group} && ${Me.AltAbilityReady[Focused Paragon of Spirits]}) {
		 /for _count 1 to ${Group}
		 		/if (${Group.Member[${_count}].ID} && ${Group.Member[${_count}].Distance} <= ${Spell[Focused Paragon of Spirits].MyRange}  && (${Group.Member[${_count}].CurrentMana} < ${groupParagon}*.${tmpHealOverride}) && ${CanCast.Find[${Group.Member[${_count}].Class}]}) {
				/varset _tmpID ${Group.Member[${i}].ID}	 		
		 	}
		/next _count
		/if (${_tmpID}) /call core_cast "Focused Paragon of Spirits" alt ${_tmpID} FALSE
	}	

/return


 
|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_BST()
	DEBUGCLASS ${cbug}patch_heal_BST()\ax
	AMIDEAD
	CHECKEXIT
	/doevents	
 	/if (${Me.PctHPs} < 75*.${tmpHealOverride}  && ${Me.AltAbilityReady[Warder's Gift]}) /call core_cast "Warder's Gift" alt 0 FALSE
	/if (${Me.PctHPs} <= ${FastHealPCT}*.${tmpHealOverride} && ${Me.SpellReady[${FastHeal}]}) {
		/call core_cast "FastHeal" 0 ${Me.ID} FALSE
		/call core_cast "Focused Paragon of Spirits" alt ${Me.ID} FALSE
	}
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0 FALSE
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0 FALSE
/return



|***
 *	DES: If you have to ask
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub pet_heal_BST()
	DEBUGCLASS ${cbug}pet_heal_BST()\aw
	AMIDEAD
	CHECKEXIT
	/doevents

	/if (${switchPcPet} && ${Pet.PctHPs} < 20 && ${Me.AltAbilityReady[Mend Companion]}) {
		/call core_cast "Mend Companion" alt ${Me.Pet.ID} FALSE
	}
	/if (${switchPcPet} && ${Pet.PctHPs} < 60 && ${Me.SpellReady[${PetHealSpell}]}) {
		VOUT Healing \a-w${Pet.CleanName}\ax with \a-r${PetHealSpell}\ax.
		/call MQ2Cast "${PetHealSpell}" ${PetHealType} 30s -targetid|${Pet.ID}
	 	/delay 5s !${Me.SpellInCooldown}
	}
	/if (${switchPcPet} && ${Pet.PctHPs} < 80 && ${Me.SpellReady[${PetRenewalSpell}]}) {
		VOUT Healing \a-w${Pet.CleanName}\ax with \a-r${PetRenewalSpell}\ax.
		/call MQ2Cast "${PetRenewalSpell}" ${PetRenewalType} 30s
	 	/delay 5s !${Me.SpellInCooldown}
	}
/return



|***
 *	DES: MGB beast Paragon if both AAs are ready
 *	USE: /bc paragon, /paragon
 *	INI: 
 *  NOTE: 
 ***|
 #bind cast_AA_paragon /paragon
sub Bind_cast_AA_paragon()
	DEBUGCLASS ${cbug}Bind_cast_AA_paragon()\ax
	/if (${Select[${Me.Class.ShortName},BST]}) /call cast_AA_paragon
/return

sub cast_AA_paragon()
	DEBUGCLASS ${cbug}cast_AA_paragon()\ax ${If[${Me.AltAbilityReady[Mass Group Buff]},${cgood}MGB\ax,${cbad}MGB\ax]} ${If[${Me.AltAbilityReady[Paragon of Spirit]},${cgood}Paragon\ax,${cbad}Paragon\ax]}
	AMIDEAD
	CHECKEXIT
	/doevents

	/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[Paragon of Spirit]} && !${Bool[${Me.Song[Paragon of Spirit]}]}) {
		/call core_cast "Mass Group Buff" alt 0 FALSE
		/call core_cast "Paragon of Spirit" alt 0 FALSE
		/if (${Bool[${Raid.Members}]}) /rs ${ParagonTXT}
	} else /if (${Bool[${Me.Song[Paragon of Spirit]}]}) {
		VOUT ${owarning} trying to cast ${csp}Paragon of Spirit\ax while its running.	
	}		
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_BST()
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return
	/call click_horn_of_unity 	
/return



 |***
 *	DES: Beast burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_BST()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return

	| /if (${Me.AltAbilityReady[Bloodlust]}) /call core_cast "Bloodlust" alt 0
	/if (${Me.AltAbilityReady[Bestial Alignment]}) /call core_cast "Bestial Alignment" alt 0 FALSE
	/if (${Me.AltAbilityReady[Frenzied Swipes]}) /call core_cast "Frenzied Swipes" alt 0 FALSE
	/if (${Me.Pet.ID} && ${Me.AltAbilityReady[Companions Furry]}) /call core_cast "Companions Furry" alt 0 FALSE
	/call use_glyph
	/if (${usePet}) /call core_cast "Attack of the Warders" alt ${Target2Kill} FALSE
	/if (${Me.CombatAbilityReady[${DiscRuaabri}]} && !${Me.Song[Frenzy of Spirit].ID} && !${Me.Buff[Dichotomic Fury].ID}) {
		/call core_cast "${DiscRuaabri}" disc 0 FALSE
	} else {
		/if (${Me.Song[${DiscRuaabri}].ID}) /return
		/if (${Me.AltAbilityReady[Frenzy of Spirit]}) /call core_cast "Frenzy of Spirit" alt 0 FALSE
		/if (${Me.AltAbilityReady[BloodLust]}) /call core_cast "BloodLust" alt 0 FALSE
		/call cast_AA_spire
		/if (!${Me.Song[Bestial Alignment].ID} && ${Me.CombatAbilityReady[${DiscSavage}]}) /call core_cast "${DiscSavage}" disc 0 FALSE
		/if (!${Me.Song[${DiscSavage}].ID} && ${Me.AltAbilityReady[Ferociousness]}) /call core_cast "Ferociousness" alt 0 FALSE
		/if (${Me.AltAbilityReady[Group Bestial Alignment]} && !${Me.Buff[Ferociousness].ID}) /call core_cast "Group Bestial Alignment" alt 0 FALSE
	}
	
	| epic 2.0
	/if (${Me.ItemReady[${${Me.Class.ShortName}Epic2}]} && ${Spell[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].Stacks} && !${Me.Buff[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID} && !${Me.Song[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID}) /call core_cast "${${Me.Class.ShortName}Epic2}" item ${Me.Pet.ID} FALSE

	/call use_glyph
	/call synergy_click_Circle
	/call click_item_burn	
/return



|***
 *	DES: Class controls
 *	USE: /bst []
 *	INI: 
 *  NOTE:
 ***|
#bind setBST /bst
sub Bind_setBST(string _classType, string _classVerbage)
	DEBUG ${cbug}Bind_setBST(type:${_classType}, verbage:${_classVerbage})\ax
	/if (!${validate_class[TRUE, |BST]}) /return

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
			/if (${${_bind}Type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}[${cgood}${spireGroup}\ax]
			/if (${${_bind}Type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}[${cgood}${spireRaid}\ax]
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
	} 
/return