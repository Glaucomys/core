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
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}(${cinfo}${version${Me.Class.ShortName}}\ax${cbug})\ax
	/doevents
	ISDEAD
	CHECKTIE

	/if (${AutoBot} && !${Me.Invis}) {
		/call cast_AA_focused_paragon
		/call check_engaged
		/if (!${Bool[${Macro.Return}]} && ${switchPcPet}) /call check_pet
	}

	:killtarget
	/call prep_combat
	/if (!${Bool[${Macro.Return}]}) /goto :exit

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/call send_pet
		/call stick_to_target
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke
		/if (${switchdeBuffSlow}) /call cast_AA_slow ${Target2Kill} "Sha's Reprisal"
		/if (${${switchPcSwarm}} && ${Bool[${ServantNuke}]}) /call cast_nuke_servant
		/call use_skill_melee
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}
		/call cast_AA_focused_paragon
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
		/call set_combat NUKE
		/call send_pet
		/call click_BP
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke
		/if (${${switchPcSwarm}} && ${Bool[${ServantNuke}]}) /call cast_nuke_servant
		/if (${switchdeBuffSlow}) /call cast_AA_slow ${Target2Kill} "Sha's Reprisal"
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}
		/call cast_AA_focused_paragon
	} else /if (${Select[${combat},HEAL]}) {
	}	else /if (${Select[${combat},DUAL]}) {
	}
	/if (${AutoBot}) {	
		/if (${switchHealCure}) /call cast_cure
	}
	/call use_mod_rod
	/if (${AutoBot} && ${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
	/if (${AutoBot} && !${Spawn[${Target2Kill}].ID} || !${AutoBot} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Bool[${Macro.Return}]}) /goto :killtarget
	}
	:exit
/return



|***
 *	DES: 
 *	USE: /call disc_BST
 *	INI: 
 *  NOTE: 
 ***|
sub disc_BST()
	DEBUGCLASS ${cbug}disc_BST()\ax
	ISDEAD
	/doevents

	/if (${Me.PctEndurance} < 3) /goto :exit
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /goto :exit
			
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}

	/if (!${Me.Song[${GrowlSpell}].ID} && ${Me.Pet.ID} && !${Me.PetBuff[Savage Rage]} && !${Me.Song[${DiscSavage}].ID}) /call core_cast "${GrowlSpell}"
	/if (${Me.AltAbilityReady[Chameleon Strike]} && ${Target.Distance} < ${Spell[Chameleon Strike].MyRange}) /call core_cast "Chameleon Strike" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[Feral Swipe]} && ${Target.Distance} < ${Spell[Feral Swipe].MyRange}) /call core_cast "Feral Swipe" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[Bite of the Asp]} && ${Target.Distance} < ${Spell[Bite of the Asp].MyRange}) /call core_cast "Bite of the Asp" alt ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscForay}]} && ${Target.Distance} < ${Spell[${DiscForay}].MyRange} && ${Me.PctEndurance} > 5 ) /call core_cast "${DiscForay}" disc ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscFocusClaws}]}) /call core_cast "${DiscFocusClaws}" disc ${Target2Kill}
	/if (${Me.AltAbilityReady[Roar of thunder]}) /call core_cast "Roar of Thunder" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[${DiscEnduring}]} && ${Target.Distance} < ${Spell[${DiscEnduring}].MyRange} && !${Bool[${Targe.Buff[${DiscEnduring}]}]}) /call core_cast "${DiscEnduring}" alt ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscRending}]} && ${Me.PctEndurance} > 5 ) /call core_cast "${DiscRending}" disc ${Target2Kill}
	:exit
/return



|***
 *	DES: Focused Paragon. will maintain the bst then group if able
 *	USE: /call cast_AA_focused_paragon
 *	INI: 
 *  NOTE: 
 ***|
sub cast_AA_focused_paragon()
	DEBUGCLASS ${cbug}cast_AA_focused_paragon()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]} || !${AutoBot}) /goto :exit
	/declare _cout 							int 	local
	/declare _tmpID 						int 	local 0
	/declare _tmpTarget2Kill		int		local 
	
	/if (${Target2Kill}) /varset _tmpTarget2Kill ${Target2Kill}
	
	/if (${Me.AltAbilityReady[Consumption of Spirit]} && ${Me.PctHPs} > ${minConsumptionHP}*.${tmpHealOverride} && ${Me.PctMana} < ${selfParagon}*.${tmpHealOverride}) /call core_cast "Consumption of Spirit" alt ${Me.ID}

	| self only focus
	/if (${Me.AltAbilityReady[Focused Paragon of Spirits]} && ${Me.PctMana} < ${selfParagon} && !${Me.Song[Paragon].ID}) {
		/call core_cast "Focused Paragon of Spirits" alt ${Me.ID}
		/goto :exit
	}

	| check the group
	/if (${Group} && ${Me.AltAbilityReady[Focused Paragon of Spirits]}) {
		 /for _cout 1 to ${Group}
		 		/if (${Group.Member[${_cout}].ID} && ${Group.Member[${_cout}].Distance} <= ${Spell[Focused Paragon of Spirits].MyRange}  && (${Group.Member[${_cout}].CurrentMana} < ${groupParagon}*.${tmpHealOverride}) && ${CanCast.Find[${Group.Member[${_cout}].Class}]}) {
				/varset _tmpID ${Group.Member[${i}].ID}	 		
		 	}
		/next _cout
		/if (${_tmpID}) /call core_cast "Focused Paragon of Spirits" alt ${_tmpID}
	}	

	:exit
	/if (${_tmpTarget2Kill}) {
		/target ID ${_tmpTarget2Kill}
		/delay 1s ${Target.ID} == ${_tmpTarget2Kill}
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
	/doevents	
 	/if (${Me.PctHPs} < 75*.${tmpHealOverride}  && ${Me.AltAbilityReady[Warder's Gift]}) /call core_cast "Warder's Gift" alt 0
	/if (${Me.PctHPs} <= ${FastHealPCT}*.${tmpHealOverride} && ${Me.SpellReady[${FastHeal}]}) {
		/call core_cast "FastHeal" 0 ${Me.ID}
		/if (${Me.AltAbilityReady[Focused Paragon of Spirits]}) /call core_cast "Focused Paragon of Spirits" alt ${Me.ID}
	}
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0
	:exit
/return



|***
 *	DES: If you have to ask
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub pet_heal_BST()
	DEBUGCLASS ${cbug}pet_heal_BST()\aw
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/if (${switchPcPet} && ${Pet.PctHPs} < 20 && ${Me.AltAbilityReady[Mend Companion]}) {
		/call core_cast "Mend Companion" alt ${Me.Pet.ID}
		| VOUT Healing \a-w${Pet.CleanName}\ax with \a-rMend Companion\ax.
		| /call MQ2Cast "Mend Companion" alt
	 	| /delay 5s !${Me.SpellInCooldown}
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
	:exit
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
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[Paragon of Spirit]} && !${Bool[${Me.Song[Paragon of Spirit]}]}) {
		/call core_cast "Mass Group Buff" alt 0
		/call core_cast "Paragon of Spirit" alt 0
		/if (${Bool[${Raid.Members}]}) /rs ${ParagonTXT}
	} else /if (${Bool[${Me.Song[Paragon of Spirit]}]}) {
		NOUT ${owarning} trying to cast ${csp}Paragon of Spirit\ax while its running.	
	}		
	:exit
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_BST()
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/call click_horn_of_unity 	
	:exit
/return


