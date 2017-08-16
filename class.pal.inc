|***
 *	class.pal.inc
 * 	Core Paladin Include
 *
 *	
 ***|
 


|***
 *	DES: Main Paladin Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_PAL()
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}(${cinfo}${version${Me.Class.ShortName}}\ax${cbug})\ax
	/doevents
	ISDEAD
	CHECKTIE

	/call check_harmonious
	/if (${useMG} && !${flagCallNeedRez} && !${timer_check_MG}) /call cast_AA_marrs_gift
	
	:killtarget
	/call prep_combat
	/if (!${Bool[${Macro.Return}]} ) /goto :exit

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/call stick_to_target
		/call use_skill_melee
		/call disc_${Me.Class.ShortName}
		/call stick_to_target
		/call click_BP
		/call stick_to_target
		/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
		/call stick_to_target
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke
		/call use_skill_melee
		/call stick_to_target
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},HEAL]}) {
		/call set_combat HEAL
		/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
	} else /if (${Select[${combat},DUAL]}) {
	} 
	/call use_mod_rod
	/if (${switchBuffSelf}) /call check_harmonious
	/if (${useMG} && !${flagCallNeedRez} && !${timer_check_MG}) /call cast_AA_marrs_gift
	/if (${coreAuto}) {	
		/if (${switchHealCure}) /call cast_cure
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
	}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} || !${coreAuto} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Macro.Return}) /goto :killtarget
	}

	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub disc_PAL()
	DEBUGCLASS	${cbug}disc_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit TRUE
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/if (${doYaulp}) /call cast_AA_yaulp
	/if (${switchAgroMode}) /call get_agro_${Me.Class.ShortName}
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscFastRune}]} && ${Bool[${DiscFastRune}]}) /call core_cast "${DiscFastRune}" disc 0


	| AA inquisitors armor stacking control
	/if (${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius 30 targetable]} > 2) {
		/if (${Me.AltAbilityReady[${AAInquisitor}]} && !${Me.Song[[${AAGroupInquisitor}].ID}) { 
			/call core_cast "${AAInquisitor}" alt 0
		} else /if (${Me.AltAbilityReady[${AAGroupInquisitor}]} && !${Me.Song[${AAInquisitor}].ID}) {
			/call core_cast "${AAGroupInquisitor}" alt 0
		}
	}


| Vanquish the Fallen. aa undead nuke
| helix of the undying AA undead debuff


	| Hand of Piety AA Group heal	
	/if (${Me.AltAbilityReady[Hand of Piety]} && ${Me.PctHPs} < 50*.${tmpHealOverride} && ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${combatRadius} targetable]} > 1 ) /call core_cast "Hand of Piety" alt 0

| AA  | 2 mobs / named

| aa Thunder of Karna | spell damage


	:exit
/return



|***
 *	DES: fight for mobs attention
 *	USE: /call get_agro_PAL
 *	INI: 
 *  NOTE: 
 ***|
sub get_agro_PAL()
	DEBUGCLASS ${cbug}get_agro_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]} ) /goto :exit

	/if (${Me.SpellReady[${ChallengeSpell}]} && (${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]} || ${Me.PctAggro} < 80)) /call core_cast "${ChallengeSpell}" 0 ${Target2Kill} cast_break
	
	| /if (${Me.AltAbilityReady[${AAForceofDisruption}]} && ${Bool[${AAForceofDisruption}]}) /call core_cast "${AAForceofDisruption}" alt ${Target2Kill} cast_break
	/if (${Me.AltAbilityReady[${AADisruptivePersecution}]} && ${Bool[${AADisruptivePersecution}]}) /call core_cast "${AADisruptivePersecution}" alt ${Target2Kill} cast_break
	| /if (${Me.AltAbilityReady[${AADivineStun}]} && ${Bool[${AADivineStun}]}) /call core_cast "${AADivineStun}" alt ${Target2Kill} cast_break
	/if (${Me.SpellReady[${ValiantDefense}]}) /call core_cast "${ValiantDefense}" 0 ${Target2Kill} cast_break

	| AE Stuns
	/varset mobAECount ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius 50 playerstate 4 targetable]}
	/if  (${mobAECount} > 1) {
		/if (${Me.AltAbilityReady[${AAAELodestar}]}) /call core_cast "${AAAELodestar}" alt ${Target2Kill} cast_break
	} else /if (${mobAECount} > 3) {
		/if (${Me.AltAbilityReady[${AAAEBeacon}]}) /call core_cast "${AAAEBeacon}" alt ${Target2Kill} cast_break
	}
	
	| AA pet hate (the little sword thingie)
	/if (${Me.AltAbilityReady[Projection of Piety]} && (${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]} || ${Me.PctAggro} < 80)) /call core_cast "Projection of Piety" alt ${Target2Kill} cast_break
		
	| fake hate
	/if (${Me.AltAbilityReady[Ageless Enmity]} && (${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]} || ${Me.PctAggro} < 60)) /call core_cast "Ageless Enmity" alt ${Target2Kill} cast_break
	| named fake hate
	/if (${Me.CombatAbilityReady[${DiscUnflinching}]} && (${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]} || ${Me.PctAggro} < 80) && ${NameList.Find[${Target.DisplayName}]}  && ${Me.CurrentEndurance} > ${Spell[${DiscUnflinching}].EnduranceCost}) /disc ${DiscUnflinching}

	:exit
/return




|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_PAL()
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/call click_horn_of_unity 	
	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_PAL()
	DEBUGCLASS ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	/call check_exit
	/if (${Bool[${Macro.Return}]}  || ${Me.PctHPs} > 81*.${tmpHealOverride}) /goto :exit
	
	| AA Armor of the Inquisitor | 3 mobs
	| AA Gift of Life | TargetofTarget
	| AA Hand of Piety | 3 targets
	| AA Lay on Hands
	
	
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0

	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub check_heal_group_PAL()
	DEBUGCLASS ${cbug}check_heal_group_PAL()\ax
	CHECKEXIT
	/declare _count						int	local
	
	:loopheal
	ISDEAD
	/doevents
	CHECKTIE
	/varset WorstHurt 				${Me.ID}
	/varset WorstHP 					${If[${HealOverride},${Float[${HealOverride}].Centi},99.99]}
	/varset WorstClass 				PAL
	/varset MinorHurt 				0
	/varset MajorHurt					0
	
	/for _count 0 to ${Group.Members}
		ISDEAD
		CHECKEXIT
	 	/doevents
		/if (!${Group.Member[${_count}].Dead} && (${Spawn[pc id ${Group.Member[${_count}].ID}].ID} || ${Spawn[mercenary id ${Group.Member[${_count}].ID}].ID}) && ${Group.Member[${_count}].PctHPs} <= ${WorstHP} && (${Group.Member[${_count}].Distance} <= ${Spell[${healRemedy1}].MyRange} && ${Group.Member[${_count}].PctHPs} <= ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride} || ${Group.Member[${_count}].Distance} <= ${Spell[${healRenewal1}].MyRange} && ${Group.Member[${_count}].PctHPs} <= ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}*.7)) {
			/varset WorstHurt ${Group.Member[${_count}].ID}
			/varset WorstHP ${Group.Member[${_count}].PctHPs}
			/varset WorstClass ${Group.Member[${_count}].Class.ShortName}
		} 
    /if (!${Group.Member[${_count}].Dead} && !${Group.Member[${_count}].State.Equal[HOVER]} && ${Group.Member[${_count}].Type.NotEqual[Corpse]} && && (${Group.Member[${_count}].PctHPs} < 85*.${tmpHealOverride} || ${Group.Member[${_count}].PctHPs}*.${tmpHealOverride} < ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}) && ${Group.Member[${_count}].PctHPs}*.${tmpHealOverride} > ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}*.7 && ${Group.Member[${_count}].Distance} <= ${Spell[${healWholeGroup}].AERange}) {
      /varcalc MinorHurt ${MinorHurt}+1
      UPDATEHUD "HEAL"
    } else /if (!${Group.Member[${_count}].Dead} && !${Group.Member[${_count}].State.Equal[HOVER]} && ${Group.Member[${_count}].Type.NotEqual[Corpse]} && ${Group.Member[${_count}].PctHPs} < ${${Group.Member[${_count}].Class.ShortName}Point}*.${tmpHealOverride}*.7 && ${Group.Member[${_count}].Distance} <= ${Spell[${healWholeGroup}].AERange}) {
      /varcalc MajorHurt ${MajorHurt}+1
      UPDATEHUD "HEAL"
    }
  /next _count
	
  /if ( ${Bool[${Me.Buff["Divine Aura"]}]} || ${Bool[${Me.Buff["Divine Barrier"]}]} || ${Bool[${Me.Buff["Sacred Barrier"]}]} ) /goto :exit
	UPDATEHUD "HEAL"


  
	| Panic Heal
  /if (${Spawn[${WorstHurt}].PctHPs} <= 45*.${tmpHealOverride} && ${Spawn[${WorstHurt}].Distance} <= ${Spell[${healPanic}].MyRange} && ${Me.SpellReady[${healPanic}]}) {
    /call core_cast "${healPanic}" 0 ${WorstHurt}
    /if (${castReturn.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
  }

		
	| AAheal stuffs
  /if (${MajorHurt}+${MinorHurt} >= 3) /if (${Me.AltAbilityReady[Hand of Piety]}) /call core_cast "Hand of Piety" alt 0
	ISDEAD
	CHECKEXIT
 	/doevents
		
	| Group Heal
  /if (${MajorHurt}+${MinorHurt} >= 3 && ${Me.SpellReady[${healWholeGroup}]} && ${useGroupHeal}) {
    /call core_cast "${healWholeGroup}" 0 ${Me.ID}
    /if (${Macro.Return.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
  } 
	ISDEAD
	CHECKEXIT
 	/doevents
	
	| Touch Heals 
	/doevents
  /if (${Spawn[${WorstHurt}].PctHPs} <= ${${WorstClass}Point}*.${tmpHealOverride} && ${Me.SpellReady[${healTouch}]}) {
		/call core_cast "${healTouch}" 0 ${WorstHurt}
	} 
	ISDEAD
	CHECKEXIT
 	/doevents
 
	:exit
	UPDATEHUD
/return



|***
 *	DES: Mar's Gift 30ish k HP/15k manna 
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub cast_AA_marrs_gift()
	DEBUGCLASS ${cbug}cast_AA_marrs_gift()\aw
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}  || !${Me.AltAbilityReady[Marr's Gift]} || ${Bool[${timer_zone_pause}]}) /goto :exit
	/if ((${Me.PctMana} || ${Me.PctHPs}) < ${useMGPct}*.${tmpHealOverride}) /call core_cast "Marr's Gift" alt ${Me.ID} 
	:exit
	/call create_timer timer_check_MG ${tcheck_MG}
/return	



|***
 *	DES: Use Marr's Gift?
 *	USE: /usemg
 *	INI: 
 *  NOTE:
 ***| 	
#bind useMG /usemg
sub Bind_useMG()
	/call Event_SetVar NA useMG SelfBuff ${If[${useMG},FALSE,TRUE]}  
	/call echos switch "Use Mar's Gift" useMG
/return

	
	
|***
 *	DES: Marr's Gift % usage
 *	USE: /mgpct [##]
 *	INI: 
 *  NOTE:
 ***| 	
#bind useMGPct /mgpct
sub Bind_useMGPct(int _useMGPct)
	/if (${Range.Between[1,99:${_useMGPct}]}) {
		/call Event_SetVar NA useMGPct SelfBuff ${_useMGPct} 
		/call echos number "Marr's Gift At" ${_useMGPct}
	} else {
		NOUT /mgpct [${cinfo}##\ax] range ${cinfo}1 - 99\ax
	}		
/return	
