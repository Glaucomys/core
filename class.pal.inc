|***
	class.pal.inc
	Core Paladin Include

***|



|***
 *	DES: Main Paladin Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_PAL()
	DEBUG Loop start: ${cbug}core_PAL()\ax
	/doevents
	ISDEAD
	CHECKTIE

	:killtarget
	CHECKREZ
	/if (${AutoBot} && !${Me.Invis}) {
		/call check1offBuffs
		/if (${Select[${combat},MELEE,RANGE,NUKE]}) {
			/call is_target_valid ${Target2Kill}
			/if (!${isValidTarget} || !${Spawn[${Target2Kill}].ID}) {
				/call clear_combat
				/goto :exit
			} else /if (${isValidTarget} && ${Spawn[${Target2Kill}].ID}) {
				/call is_toast_time
				/if (${Macro.Return}) /call synergy_${Me.Class.ShortName}
			}
		} else /if (${Select[${combat},HEAL,DUAL]}) {
			/call is_toast_time
			/if (${Macro.Return}) /call synergy_${Me.Class.ShortName}
		}
	} else /if (!${AutoBot}) {
		/if (${Target.ID} != ${Target2Kill}) /varset Target2Kill ${Target.ID}
		/if (!${Target.ID} || !${Spawn[${Target2Kill}].ID}) /goto :exit
	}
	
	/if (${Bool[${autoInventoryItem.Find[${Cursor.Name}]}]}) /call clear_cursor

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
	/if (${AutoBot}) {	
		/if (${switchHealCure}) /call cast_cure
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
	}
	/if (${AutoBot} && !${Spawn[${Target2Kill}].ID} || !${AutoBot} && ${Target.ID} && ${Bool[${combat}]}) {
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
	DEBUG	${cbug}disc_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit TRUE
	/if (${Macro.Return}) /goto :exit
	/call is_target_dead
	/if (${Macro.Return}) /goto :exit
	/if (${doYaulp}) /call cast_AA_yaulp
	/call get_agro_${Me.Class.ShortName}
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscFastRune}]} && ${Bool[${DiscFastRune}]}) /call core_cast "${DiscFastRune}" disc 0
	| /if (${Me.AltAbilityReady[${AAForceofDisruption}]} && ${Bool[${AAForceofDisruption}]}) /call core_cast "${AAForceofDisruption}" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[${AADisruptivePersecution}]} && ${Bool[${AADisruptivePersecution}]}) /call core_cast "${AADisruptivePersecution}" alt ${Target2Kill}

	/if (${Me.AltAbilityReady[${AAAEBeacon}]} ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius 30 targetable]} >= 2) /call core_cast "${AAAEBeacon}" alt 0
	/if (${Me.AltAbilityReady[${AAAELodestar}]} ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius 30 targetable]} >= 4) /call core_cast "${AAAELodestar}" alt 0


	/if (${Me.AltAbilityReady[${AAInquisitor}]} ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius 30 targetable]} >= 2 && !${Me.Song[armor of the Inquisitor].ID}) /call core_cast "${AAInquisitor}" alt 0




|	/if (${Me.AltAbilityReady[Twisted Shank]}) /call core_cast "Twisted Shank" alt ${Target2Kill}





| AA  | 2 mobs / named

| aa Thunder of Karna | spell damage
|




	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub get_agro_PAL()
	DEBUG ${cbug}get_agro_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/if (${setAgroMode}) {
		/if (${Me.SpellReady[${ChallengeSpell}]} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast "${ChallengeSpell}" 0 ${Target2Kill} cast_break
	/if (${Me.AltAbilityReady[${AADivineStun}]} && ${Bool[${AADivineStun}]} && ${Me.TargetOfTarget.Name.NotEqual[${Me.CleanName}]}) /call core_cast "${AADivineStun}" alt ${Target2Kill}
	
	} else /if (!${setAgroMode}) {
		| aa Mar's Salvation  | anti agro

	}
		

	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_PAL()
	DEBUG ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/call click_horn_of_unity 	
	/if (${Me.PctMana} < 60*.${tmpHealOverride} && ${Me.ItemReady[${CircleofMana}]} && !${Me.Song[${FindItem[${CircleofMana}].Spell}].ID} && ${Spell[${FindItem[${CircleofMana}].Spell}].Stacks}) /call core_cast "${CircleofMana}" item 0
	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_PAL()
	DEBUG ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	/call check_exit
	/if (${Macro.Return} || ${Me.PctHPs} > 81*.${tmpHealOverride}) /goto :exit
	
	| AA Armor of the Inquisitor | 3 mobs
	| AA Gift of Life | TargetofTarget
	| AA Hand of Piety | 3 targets
	| AA Lay on Hands
	
	| AA Marr's Gift
	
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0
	/if (${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${MobAgro} playerstate 4 targetable]} >= 4 && ${Me.ItemReady[${CircleofLife}]} && !${Me.Song[${FindItem[${CircleofLife}].Spell}].ID} && ${Spell[${FindItem[${CircleofLife}].Spell}].Stacks}) /call core_cast "${CircleofLife}" item 0
	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub check_heal_group_PAL()
	DEBUG ${cbug}check_heal_group_PAL()\ax
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
	
	| Do group members need healing? ${Group.Member[${_count}].State.Equal[HOVER]}
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


  
|****** Panic Heal ******|
  /if (${Spawn[${WorstHurt}].PctHPs} <= 45*.${tmpHealOverride} && ${Spawn[${WorstHurt}].Distance} <= ${Spell[${healPanic}].MyRange} && ${Me.SpellReady[${healPanic}]}) {
    /call core_cast "${healPanic}" 0 ${WorstHurt}
    /if (${castReturn.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
  }

		
|****** AA heal stuffs ******|
  /if (${MajorHurt}+${MinorHurt} >= 3) /if (${Me.AltAbilityReady[Hand of Piety]}) /call core_cast "Hand of Piety" alt ${Me.ID}
	ISDEAD
	CHECKEXIT
 	/doevents
		
|****** Group Heal ******|
  /if (${MajorHurt}+${MinorHurt} >= 3 && ${Me.SpellReady[${healWholeGroup}]} && ${useGroupHeal}) {
    /call core_cast "${healWholeGroup}" 0 ${Me.ID}
    /if (${Macro.Return.NotEqual[CAST_SUCCESS]} && ${Macro.Return.NotEqual[CAST_OUTOFMANA]}) /goto :loopheal
  } 
	ISDEAD
	CHECKEXIT
 	/doevents
	
|****** Touch Heals ******|
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

