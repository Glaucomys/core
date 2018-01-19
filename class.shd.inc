|***
 *	class.shd.inc
 *	Core Shadow Knight Include
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
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}()\ax
	/doevents
	AMIDEAD
	CHECKTIE
	
	/if (${coreAuto}) {
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
		/call use_skill_melee ${Target2Kill}
		/if (${switchAgro}) /call get_agro_${Me.Class.ShortName}
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/call click_DPS_${Me.Class.ShortName}
		/call stick_to_target
		/if (${DoDoT}) /call cast_DOT
		/call use_skill_melee ${Target2Kill}
		/if (${DoNuke}) /call cast_nuke
		/if (${switchAgro}) /call get_agro_${Me.Class.ShortName}
		/call stick_to_target
		/call use_skill_melee ${Target2Kill}
		/call click_item TRUE
		/if (${switchHarmonious}) /call check_harmonious
		/if (${switchSkin} && !${timer_check_Skin}) /call check_skin	
		/if (${switchStance} && (!${timer_check_Stance} || !${Bool[${Me.Buff[${Stance}]}]})) /call check_stance
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
		/call set_combat RANGE
		/call check_deBuffs ${Target2Kill}
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke} && ${switchAgro}) /call cast_nuke
		/call use_skill_melee ${Target2Kill}
		/call click_item
		/call click_DPS_${Me.Class.ShortName}
		/if (${switchHarmonious}) /call check_harmonious
		/if (${switchSkin} && !${timer_check_Skin}) /call check_skin	
		/if (${switchStance} && (!${timer_check_Stance} || !${Bool[${Me.Buff[${Stance}]}]})) /call check_stance
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
	}
	/if (${coreAuto}) {	
		/call use_mod_rod
		/if (${switchHealCure}) /call cast_cure
	}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} && !${is_target_dead[${Target2Kill}]}) /goto :killtarget
	/if (!${coreAuto} && ${Target.ID} && ${Bool[${combat}]} && !${is_target_dead[${Target.ID}]}) /goto :killtarget
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub disc_SHD()
	DEBUGCLASS ${cbug}disc_SHD()\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return
	CHECKTIE

	| defensive	
	/if (${coreAuto} && ${switchAgro} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]} >= ${defCount}) {
		/declare _count int local 0
		/for _count 1 to 3
			/if (${Bool[${Me.ActiveDisc}]}) /break
			/if (${Me.CombatAbilityReady[${DefensiveDisc${_count}}]}) {
				/call core_cast "${DefensiveDisc${_count}}" disc 0 FALSE
				/break
			}			
		/next _count
	}
	

	/if (${switchBanestrike}) /call cast_AA_banestrike ${Target2Kill}
	
	| check challenge
	/call cast_challenge ${Target2Kill}
	
	| get me sum...
	/if (${switchAgro}) /call get_agro_${Me.Class.ShortName}

	| tap disc
	/if (${switchHealSelf} && ${Me.PctHPs} < 50*.${tmpHealOverride} && ${Me.CombatAbilityReady[${DiscRancor}]}) /call core_cast "${DiscRancor}" disc ${Target2Kill} FALSE

	| heal clickies
	/call click_item_heal
	
	/if (${Me.CombatAbilityReady[${Withstand}]} && ${Me.CurrentEndurance} > ${Spell[${Withstand}].EnduranceCost}) /call core_cast "${Withstand}" disc 0 FALSE
	/if (${Me.AltAbilityReady[Thought Leech]} &&  ${Me.PctMana} <= 60*.${tmpHealOverride}) /call core_cast "Thought Leech" alt ${Target2Kill} FALSE
	/if (${Me.CombatAbilityReady[${BladeStrike}]} && ${Me.CurrentEndurance} > ${Spell[${BladeStrike}].EnduranceCost}) /call core_cast "${BladeStrike}" disc ${Target2Kill} FALSE
	/if (${switchHealSelf} &&  ${Me.AltAbilityReady[Vicious Bite of Chaos]} && ${Target.Distance} <= ${Spell[Vicious Bite of Chaos].MyRange}) /call core_cast "Vicious Bite of Chaos" alt ${Target2Kill} FALSE
	/if (${Me.AltAbilityReady[Soul Flay]}) /call core_cast "Soul Flay" alt 0 FALSE
	
	/if (${switchHealSelf} && ${Me.PctHPs} < 30*.${tmpHealOverride} && ${Me.AltAbilityReady[Leech Touch]}) /call core_cast "Leech Touch" alt ${Target2Kill} FALSE
	/if (${switchHealSelf} && ${Me.PctHPs} < 50*.${tmpHealOverride} && ${Me.CombatAbilityReady[${DiscLeech}]} && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${combatRadius}*2 targetable]} > 2 ) /call core_cast "${DiscLeech}" disc 0 FALSE
	/if (!${Bool[${Me.ActiveDisc}]} && ${Me.PctHPs} < 50*.${tmpHealOverride} && ${Me.CombatAbilityReady[Deflection Discipline]}) /call core_cast "Deflection Discipline" disc 0 FALSE
	/call check_deBuffs ${Target2Kill}

/return



|***
 *	DES: take what you want. there is no try...
 *	USE: /call get_agro_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub get_agro_SHD()
	DEBUGCLASS ${cbug}get_agro_${Me.Class.ShortName}()\ax
	/if (!${switchAgro}) /return
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return
	/call use_skill_melee ${Target2Kill}
	/if (${switchPcPet}) /call send_pet

	| single hate
	| /if (${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) /call core_cast "Stream of Hatred" alt ${Target2Kill} FALSE
	/if ((${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) && ${Me.SpellReady[${Hate1}]} && ${Target.Distance} <= ${Spell[${Hate1}].MyRange} && !${Target.Buff[${Hate1}].ID} && !${Target.Buff[${Hate2}].ID}) /call core_cast "${Hate1}" 0 ${Target2Kill} FALSE
	/if ((${Me.PctAggro} < 80 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) && ${Me.SpellReady[${Hate2}]} && ${Target.Distance} <= ${Spell[${Hate2}].MyRange} && !${Target.Buff[${Hate1}].ID} && !${Target.Buff[${Hate2}].ID}) /call core_cast "${Hate2}" 0 ${Target2Kill} FALSE


	| AE Hate
	/varset mobAECount ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 50 playerstate 4 targetable]}
	/if  (${mobAECount} > 1) {
		/if (${Me.SpellReady[${AEHate1}]}) /call core_cast "${AEHate1}" 0 0 FALSE
	} else /if (${mobAECount} > 2) {
		/if (${Me.SpellReady[${AEHate2}]}) /call core_cast "${AEHate2}" 0 0 FALSE
	} else /if (${mobAECount} > 4) {
		/if (${Me.AltAbilityReady[Explosion of Hatred]}) /call core_cast  "Explosion of Hatred" alt 0 FALSE
	} else /if (${mobAECount} > 6) {
		/if (${Me.AltAbilityReady[Explosion of Spite]}) /call core_cast "Explosion of Spite" alt 0 FALSE
	}

	| AE lifetap	
	/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${combatRadius}*2 playerstate 4 targetable]} >= 2 && !${SpawnCount[pc enchanter radius ${combatRadius}*2]} && ${Me.SpellReady[${AELifeTap}]}) /call core_cast "${AELifeTap}" 0 ${Target2Kill} FALSE
	
	| fake hate
	/if ((${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) && ${Me.AltAbilityReady[Ageless Enmity]}) /call core_cast "Ageless Enmity" alt ${Target2Kill} FALSE
	| named fake hate
	/if ((${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) && (${NameList.Find[${Target.DisplayName}]} && !${switchOverrideName}) && ${Me.AltAbilityReady[Mindless Hatred]} && ${Target.Level} <= 110) /call core_cast "Mindless Hatred" alt ${Target2Kill} FALSE
	/if ((${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) && (${NameList.Find[${Target.DisplayName}]} && !${switchOverrideName}) && ${Me.CombatAbilityReady[${DiscFakeHate}]} && ${Me.CurrentEndurance} > ${Spell[${DiscFakeHate}].EnduranceCost}) /disc ${DiscFakeHate}
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_SHD()
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	AMIDEAD
	CHECKEXIT
	/doevents
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
	DEBUGCLASS ${cbug}check_skin()\ax
	/if (!${switchSkin}) /return
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return	
	/if (!${Bool[${Me.Buff[${Skin}]}]} && ${Me.SpellReady[${Skin}]}) {
		/call core_cast "${Skin}" 0 0 FALSE
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
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return		
	
	| defensive	
	/declare _count int local 0
	/for _count 1 to 3
		/if (${Bool[${Me.ActiveDisc}]}) /break
		/if (${Me.CombatAbilityReady[${DefensiveDisc${_count}}]}) {
			/call core_cast "${DefensiveDisc${_count}}" disc 0 FALSE
			/break
		}			
	/next _count	

	/if (${Me.AltAbilityReady[T`Vyl's Resolve]} && ${Target.Distance} <= ${Spell[T`Vyl's Resolve].MyRange}) /call core_cast "T`Vyl's Resolve" alt ${Target2Kill} FALSE
	/if (${Me.CombatAbilityReady[${CrimsonBlade}]} && ${Me.CurrentEndurance} > ${Spell[${CrimesonBlade}].EnduranceCost}) /call core_cast "${CrimsonBlade}" disc 0 FALSE
	/call cast_AA_spire
	/if (${Me.AltAbilityReady[Veil of Darkness]}) /call core_cast "Veil of Darkness" alt ${Target2Kill} FALSE
	/if (${Me.AltAbilityReady[Harm Touch]}) /call core_cast "Harm Touch" alt ${Target2Kill} FALSE
	/if (${switchAgro} && ${Me.AltAbilityReady[Scourge Skin]}) /call core_cast "Scourge Skin" alt 0 FALSE

	| AA Visage of Decay/Death stacking issues
	/if (${Me.AltAbilityReady[Visage of Death]} && !${Me.Buff[Visage of Decay].ID}) {
		/call core_cast "Visage of Death" alt 0 FALSE
	} else /if (${Me.AltAbilityReady[Visage of Decay]} && !${Me.AltAbilityReady[Visage of Death]} && !${Me.Buff[Visage of Death].ID}) {
		/call core_cast "Visage of Decay" alt 0 FALSE
	}

	| swarm pets
	/if (${switchPcSwarm} && ${Me.AltAbilityReady[Chattering Bones]}) /call core_cast "Chattering Bones" alt ${Target2Kill} FALSE
	
	| epic 2.0
	/if (${Me.ItemReady[${${Me.Class.ShortName}Epic2}]} && ${Spell[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].Stacks} && !${Me.Buff[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID} && !${Me.Song[${FindItem[${${Me.Class.ShortName}Epic2}].Spell}].ID}) /call core_cast "${${Me.Class.ShortName}Epic2}" item 0 FALSE

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
	DEBUGCLASS ${cbug}Bind_setSHD(type:${_classType}, verbage:${_verbage})\ax
	/if (!${validate_class[TRUE, |SHD]}) /return

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

	| on/off switch for Vizat's Skin
	/if (${Bool[${_classType.Find[skin]}]}) {
		/call writeINI switchSkin Buffs ${If[${switchSkin},FALSE,TRUE]}  
		/call echos switch "Buff ${sep} ${Skin}" switchSkin 0 0
		/reuturn
	
	| on/off switch for Harmonious
	} else /if (${Bool[${_classType.Find[harmonious]}]}) {
		/call writeINI switchHarmonious Buffs ${If[${switchHarmonious},FALSE,TRUE]}  
		/call echos switch "Buff ${sep} ${Harmonious}" switchHarmonious 0 0
		/return

	| on/off switch for Stance
	} else /if (${Bool[${_classType.Find[stance]}]}) {
		/call writeINI switchStance Buffs ${If[${switchStance},FALSE,TRUE]}  
		/call echos switch "Buff ${sep} ${Stance}" switchStance 0 0
		/return

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
			VOUT /${_bind} healpoint [${cinfo}${${Me.Class.ShortName}Point}\ax] allowable range ${sep} ${cinfo}1 - 99\ax
			/return
		}

	| defensive mob agro count
	} else /if (${Bool[${_classType.Equal[defcount]}]}) {
		/if (${Range.Between[1,50:${Int[${_verbage}]}]}) {
			/call writeINI defCount Discs ${_verbage}
		} else /if (!${Bool[${_verbage}]} || !${Range.Between[1,50:${Int[${_verbage}]}]}) {
			VOUT /${_bind} defcount [${cinfo}${countMobAgro}\ax] allowable range ${sep} ${cinfo}1 - 50 min\ax
			/return
		}	


	}
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
		/if (${${_bind}Type.Arg[${_count},|].Equal[spiregroup]}) /varset _listout ${_listout}(${cnum}${spireGroup}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[spireraid]}) /varset _listout ${_listout}(${cnum}${spireRaid}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[healpoint]}) /varset _listout ${_listout}(${cnum}${${Me.Class.ShortName}Point}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[defcount]}) /varset _listout ${_listout}(${cnum}${defCount}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return

