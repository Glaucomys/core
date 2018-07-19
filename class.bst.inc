|***
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

	/if (${coreAuto}) {
		/call check_mana_${Me.Class.ShortName}
		/if (!${check_engaged[]} && ${switchPcPet}) /call check_pet
	}

	:killtarget
	CHECKREZ
	/call prep_combat
	/if (${Macro.Return} && ${Bool[${combat}]}) {
		DEBUG combat:${cinfo}${combat.Upper}\ax		

		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${switchHealPet}) /call pet_heal_${Me.Class.ShortName}
		/if (${switchHealCure}) /call cast_cure

		/if (${isValidTarget}) {
			/call set_combat ${combat.Upper}
			/if (${switchPcSwarm} || ${switchPcPet}) /call send_pet
			/if (${switchPcSwarm}) /call cast_nuke_servant
			/if (${switchCombatMelee}) /call use_skill_melee ${Target2Kill}
			/call disc_${Me.Class.ShortName}
			/if (${switchAlliance}) /call trigger_alliance ${Target2Kill}
			/call check_deBuffs ${Target2Kill}
			/if (${switchCombatDoT}) /call cast_DOT
			/if (${switchCombatNuke}) /call cast_nuke
			/call click_BP
			/call click_item
			/if (${switchPcSwarm}) /call click_item_swarm ${Target2Kill}
			/call click_DPS_${Me.Class.ShortName}
			/call check_mana_${Me.Class.ShortName}
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
			
	| do we want the mobs attention?
	/if (${switchAgro}) {
		/if (${validate_cast[FALSE, alt, "Roaring Strike", ${Target2Kill}]}) /call core_cast2 "Roaring Strike" alt ${Target2Kill} FALSE
	} else /if (!${switchAgro}) {
		/if (${validate_cast[FALSE, alt, "Roar of Thunder", ${Target2Kill}]}) /call core_cast2 "Roar of Thunder" alt ${Target2Kill} FALSE
		/if (${validate_cast[FALSE, alt, "Chameleon Strike", ${Target2Kill}]}) /call core_cast2 "Chameleon Strike" alt ${Target2Kill} FALSE
	}

	/if (${Me.Pet.ID}) {
		/if (!${Me.Song[${GrowlSpell}].ID} && !${Me.PetBuff[Savage Rage]} && !${Me.Song[${DiscSavage}].ID}) {
			/if (${validate_cast[FALSE, spell, "${GrowlSpell}", ${Me.ID}]}) /call core_cast2 "${GrowlSpell}" 0 0 FALSE
		}
	}

	/if (${Me.PctEndurance} > 5) {
		/if (${validate_cast[FALSE, disc, "${DiscForay}", ${Target2Kill}]}) /call core_cast2 "${DiscForay}" disc ${Target2Kill} FALSE
		/if (${validate_cast[FALSE, disc, "${DiscRending}", ${Target2Kill}]}) /call core_cast2 "${DiscRending}" disc ${Target2Kill} FALSE
	}

	/if (${validate_cast[FALSE, disc, "${DiscFocusClaws}", ${Target2Kill}]}) /call core_cast2 "${DiscFocusClaws}" disc ${Target2Kill} FALSE
	/if (${validate_cast[FALSE, alt, "${DiscEnduring}", ${Target2Kill}]} && !${Bool[${Target.Buff[${DiscEnduring}]}]}) /call core_cast2 "${DiscEnduring}" alt ${Target2Kill} FALSE
	
	/if (${validate_cast[FALSE, alt, "Bestial Savagery", ${Target2Kill}]} && !${Me.Song[Bestial Savagery].ID}) /call core_cast2 "Bestial Savagery" alt 0 FALSE
	
/return



|***
 *	DES: Focused Paragon. will maintain the bst
 *	USE: /call check_mana_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub check_mana_BST()
	AMIDEAD
	SIMPLEEXIT
	
	| consumption of Spirits AA
	/if (${switchConsumeSpirit}) {
		/if (${Me.PctMana} < ${ConsumeSpiritPct}*.${tmpHealOverride}) {
		 	/if (${validate_cast[FALSE, alt, "Consumption of Spirit", ${Me.ID}]}) /call core_cast2 "Consumption of Spirit" alt ${Me.ID} FALSE
		}
	}
	
	/if (${Me.Song[Paragon of Spirit].ID}) /return
	/if (${Me.Song[Focused Paragon of Spirit].ID}) /return
		
	| self paragon
	/if (${switchSelfParagon}) {
		/if (${Me.PctMana} < ${selfParagonPct}*.${tmpHealOverride}) {
			/if (!${Me.Song[Focused Paragon].ID}) {
				/if (${validate_cast[FALSE, alt, "Focused Paragon of Spirits", ${Me.ID}]}) /call core_cast2 "Focused Paragon of Spirits" alt ${Me.ID} FALSE
			}
		}
	}
	
/return


 
|***
 *	DES: emergency healing
 *	USE: /call patch_heal_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_BST()
	DEBUGCLASS ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	/if (${Me.PctHPs} > ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /return
	AMIDEAD
	CHECKEXIT
	
 	/if (${Me.PctHPs} < ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) {
 		/if (${validate_cast[FALSE, alt, "Warder's Gift", ${Me.ID}]}) /call core_cast2 "Warder's Gift" alt 0 FALSE
	}

	/if (${Me.PctHPs} <= ${${Me.Class.ShortName}Point}*.${tmpHealOverride} && ${Me.SpellReady[${FastHeal}]}) {
		/if (${validate_cast[FALSE, spell, "${FastHeal}", ${Me.ID}]}) /call core_cast2 "${FastHeal}" 0 ${Me.ID} FALSE
	}
	/call click_item_heal
/return



|***
 *	DES: If you have to ask
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub pet_heal_BST()
	DEBUGCLASS ${cbug}pet_heal_BST()\aw
	/if (!${Me.Pet.ID}) /return
	AMIDEAD
	SIMPLEEXIT

	/if (${Pet.PctHPs} < 20) {
		/if (${validate_cast[FALSE, alt, "Mend Companion, "${Me.Pet.ID}]}) /call core_cast2 "Mend Companion" alt ${Me.Pet.ID} FALSE
	}
	/if (${Pet.PctHPs} < 60) {
		/if (${validate_cast[FALSE, spell, "${PetHealSpell}", ${Me.Pet.ID}]}) /call core_cast2 "${PetHealSpell}" 0 ${Me.Pet.ID} FALSE
	}
	/if (${Pet.PctHPs} < 80) {
		/if (${validate_cast[FALSE, spell, "${PetRenewalSpell}", ${Me.Pet.ID}]}) /call core_cast2 "${PetRenewalSpell}" 0 ${Me.Pet.ID} FALSE
	}
/return



|***
 *	DES: MGB beast Paragon if both AAs are ready
 *	USE: /paragon
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
		/if (${validate_cast[FALSE, alt, "Mass Group Buff", ${Me.ID}]}) /call core_cast2 "Mass Group Buff" alt 0 FALSE
		/if (${validate_cast[FALSE, alt, "Paragon of Spirit", ${Me.ID}]}) /call core_cast2 "Paragon of Spirit" alt 0 FALSE
		| /if (${Raid.Members}) /rs ${ParagonTXT}
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
	
	/if (${validate_cast[FALSE, alt, "Bestial Alignment", ${Me.ID}]}) /call core_cast2 "Bestial Alignment" alt 0 FALSE
	/if (${validate_cast[FALSE, alt, "Frenzied Swipes", ${Me.ID}]}) /call core_cast2 "Frenzied Swipes" alt 0 FALSE

	/if (${switchPcSwarm}) {
		/if (${validate_cast[FALSE, alt, "Attack of the Warders", ${Target2Kill}]}) /call core_cast2 "Attack of the Warders" alt ${Target2Kill} FALSE
	}

	/if (${Me.Pet.ID}) {
		/if (${validate_cast[FALSE, alt, "Companions Furry", ${Me.ID}]}) /call core_cast2 "Companions Furry" alt 0 FALSE
	}

	/call use_glyph


	/if (${Me.CombatAbilityReady[${DiscRuaabri}]} && !${Me.Song[Frenzy of Spirit].ID} && !${Me.Buff[Dichotomic Fury].ID}) {
		/if (${validate_cast[FALSE, disc, "${DiscRuaabri}", ${Me.ID}]}) /call core_cast2 "${DiscRuaabri}" disc 0 FALSE
	} else {
		/if (${Me.Song[${DiscRuaabri}].ID}) /return
		/if (${validate_cast[FALSE, alt, "Frenzy of Spirit", ${Me.ID}]}) /call core_cast2 "Frenzy of Spirit" alt 0 FALSE
		/if (${validate_cast[FALSE, alt, "BloodLust", ${Me.ID}]}) /call core_cast2 "BloodLust" alt 0 FALSE
		/call cast_AA_spire
		/if (${validate_cast[FALSE, disc, "${DiscSavage}", ${Me.ID}]} && !${Me.Song[Bestial Alignment].ID}) /call core_cast2 "${DiscSavage}" disc 0 FALSE
		/if (${validate_cast[FALSE, alt, "Ferociousness", ${Me.ID}]} && !${Me.Song[${DiscSavage}].ID}) /call core_cast2 "Ferociousness" alt 0 FALSE
		/if (${validate_cast[FALSE, alt, "Group Bestial Alignment", ${Me.ID}]} && !${Me.Buff[Ferociousness].ID}) /call core_cast2 "Group Bestial Alignment" alt 0 FALSE
	}
	
	| epic 2.0
	| /if (${Me.ItemReady[${${Me.Class.ShortName}Epic2}]} && ${Spell[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].Stacks} && !${Me.Buff[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID} && !${Me.Song[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID}) /call core_cast "${${Me.Class.ShortName}Epic2}" item ${Me.Pet.ID} FALSE

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
sub Bind_setBST(string _classType, string _verbage)
	DEBUG ${cbug}Bind_setBST(type:${_classType}, verbage:${_verbage})\ax
	/if (!${validate_class[TRUE, |BST]}) /return

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local ${Me.Class.ShortName.Lower}
  /declare _type string	local |consume|healpoint|paragon|spiregroup|spireraid

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
		
	| focused paragon	AA
	} else /if (${Bool[${_classType.Equal[paragon]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI selfParagonPct Spells ${_verbage}
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} paragon [${cinfo}${selfParagonPct}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		} else {
		 	/call writeINI switchSelfParagon Spells ${If[${switchSelfParagon},FALSE,TRUE]}  
		}
		
	| consume spirit AA
	} else /if (${Bool[${_classType.Equal[consume]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI ConsumeSpiritPct Spells ${_verbage}
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} consume [${cinfo}${ConsumeSpiritPct}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		} else {
		 	/call writeINI switchConsumeSpirit Spells ${If[${switchConsumeSpirit},FALSE,TRUE]}  
		}
	}		

	
	/if (${switchMQclearonCLS}) /mqclear
	/for _count 1 to ${_type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_type.Arg[${_count},|]}\ax
		/if (${_type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${_type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${_type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}${${Me.Class.ShortName}Point}\ax)
		/if (${_type.Arg[${_count},|].Equal[fastheal]}) /varset _listout ${_listout}(${cnum}${FastHealPCT}\ax)
		/if (${_type.Arg[${_count},|].Equal[paragon]}) /varset _listout ${_listout}(${If[${switchSelfParagon},${cnum}${selfParagonPct}\ax,${cbad}${selfParagonPct}\ax]})
		/if (${_type.Arg[${_count},|].Equal[consume]}) /varset _listout ${_listout}(${If[${switchConsumeSpirit},${cnum}${ConsumeSpiritPct}\ax,${cbad}${ConsumeSpiritPct}\ax]})
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]

/return

		
		
