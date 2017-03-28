|***
	class.nec.inc
	Core Necromancer Include

***|



sub core_NEC()
	DEBUG Loop start: ${cbug}core_NEC()\ax
	/doevents
	ISDEAD
	/if (!${Bool[${timer_check_Tie}]} && ${Bool[${SetTie}]}) /call set_tie
	/call check_AA_now

	/if (${AutoBot} && !${Me.Invis}) {
		/call check_engaged
		/if (!${Macro.Return} && ${usePet}) /call check_pet
	}




|***
	DES: Auto combat routines
***|
	:autokilltarget
	/if (${AutoBot}) /call check1offBuffs
	/if (!${Me.Invis} && ${AutoBot} && (${Bool[${Target2Kill}]} && !${combat.Equal[HEAL]})) /call is_target_valid ${Target2Kill}
	/if (${AutoBot} && !${Me.Invis}) {
		/call check1offBuffs
		/if (${useSmartAssist1} && ${Target2Kill} != ${Me.XTarget[1].ID}) /call clear_combat
		/if (${useSmartAssist2} && ${Target2Kill} != ${Me.XTarget[2].ID}) /call clear_combat
		/if (${useSmartAssistG} && ${Target2Kill} != ${Me.XTarget[1].ID}) /call clear_combat
		/if ((!${isValidTarget} || !${Bool[${Spawn[${Target2Kill}]}]}) && !${combat.Equal[HEAL]}) {
			/call clear_combat
			/goto :exit
		}	
		/if (${combat.Equal[MELEE]}) {
		}
		/if (${combat.Equal[RANGE]}) {
		}
		/if (${combat.Equal[HEAL]}) {
		}
		/if (${combat.Equal[NUKE]}) {
			/if (${isValidTarget}) /call setNukeAttack
			/if (!${Bool[${timer_check_Tie}]} && ${Bool[${SetTie}]}) /call set_tie
			/call get_assist_target
			/call send_pet
			/if (${Bool[${Target2Kill}]} && (${setToastMode} || (${AutoToast} && ${NameList.Find[${Target.CleanName}]}) || (${Bool[${burnMobCount}]} && ${SpawnCount[npc radius ${MobAgro} los targetable playerstate 4]} >= ${burnMobCount}))) /call burn.${Me.Class.ShortName} 
|			/if (${Bool[${Target2Kill}]}) /call cast_Call_for_Blood
			/if (${switchdeBuffEradicate} && !${timer_eradicate_${Target2Kill}}) /call cast_eradicate_magic ${Target2Kill}
			/if (${Bool[${Target2Kill}]} && ${Bool[${DoDoT}]}) /call castNUKE
			/if (${Bool[${Target2Kill}]} && ${Bool[${DoDoT}]}) /call castDOT
			/if (${Bool[${Target2Kill}]}) /call disc.${Me.Class.ShortName}
			/if (${Bool[${Target2Kill}]} && ${Spawn[${Target2Kill}].PctHPs} >= 20) /call doclickItem
			/if (${Bool[${Target2Kill}]}) /call clickDPS.${Me.Class.ShortName}
			/if (${Bool[${Target2Kill}]}) /call useModRod
		}
		/if (${combat.Equal[DUAL]}) {
		}
		/call isTargetDead
		/if (!${Macro.Return}) /goto :autokilltarget
	}


	
	|***
	 *	DES: Manual Attack stuff
	 *	USE: /squelch /bct ${Me} MODE (MELEE,RANGE,NUKE,HEAL,DUAL)
	 **|
	/if (!${AutoBot} && ${combatMode.NotEqual[FALSE]}) {
		/if (${Target.ID} != ${Target2Kill}) {
			/varset Target2Kill ${Target.ID}
		}
		/if (!${Target.ID}) /goto :exit
		/if (${combatMode.Equal[MELEE]}) {
		} 
		/if (${combatMode.Equal[RANGE]}) {
		}	
		/if (${combatMode.Equal[HEAL]}) {
		}
		/if (${combatMode.Equal[NUKE]}) {
			/if (${Me.Invis}) /makemevisible
			:manualnuke
			/if (!${Target.LineOfSight}) {
				VOUT \arNO\ax line of sight for ranged attack.
				/if (${Me.Casting.ID}) /stopcast
				/varset combatMode FALSE
				/goto :exit 	
			}
			/if (${usePet} && ${Pet.ID}) /pet attack
			/call setNukeAttack
			/doevents		
			/call get_assist_target
			/if (!${Bool[${Target.Malod}]}) /call castMaloTash
			/call doBPClickie
			/if (!${Bool[${timer_Call_for_Blood}]}) /call cast_call_for_blood
			/call castNecroDot		
	
|			/call dotCycle
					
			/if (${Spawn[${Target2Kill}].PctHPs} >= 20) /call doclickItem
			/call disc.${Me.Class.ShortName}
			/if ((${setToastMode} || ${AutoToast}) && ${NameList.Find[${Target.CleanName}]}) /call burn.${Me.Class.ShortName} 
			/call clickDPS.${Me.Class.ShortName}
			/call isTargetDead
			/if (!${Macro.Return}) /goto :manualnuke
			/goto :exit
		}
		/if (${combatMode.Equal[DUAL]}) {
		}
		/call check_AA_now
	}
	:exit	
	
	
	
/return





sub petHeal.NEC()
	| Yah... whatever... placeholder
/return




sub disc.NEC()
	DEBUG \atdisc.${Me.Class.ShortName}()\ax 
	/doevents
	ISDEAD
	/if ((${AutoBot} && ${Bool[${NoCastList.Find[${Target.DisplayName}]}]}) || ${Me.Moving} || ${Bool[${Target.Mezzed}]}) /goto :exit
	/call get_assist_target
	/if (${useBanestrike} && ${Me.AltAbilityReady[Banestrike]} && ${Spawn[${Target2Kill}].Distance} <= ${Spell[Banestrike].MyRange}) /call MQ2Cast "Banestrike" alt breakCast -targetid|${Spawn[${Target2Kill}].ID}
|	/if (${Me.AltAbilityReady[Force of Elements]} && ${Spawn[${Target2Kill}].Distance} <= 200) /call MQ2Cast "Force of Elements" alt breakCast -targetid|${Spawn[${Target2Kill}].ID}
	/if (${Bool[${useBuffKill}]} && !${timerBuffKill}) /call castBuffKill
	:exit
/return



|***
	DES: cast DPS Clickies
	USE: this shits auto, learn to love it.
***|
sub clickDPS.NEC()
	DEBUG \atclickDPS.NEC()\ax 
	/doevents
	ISDEAD
	/if ((${AutoBot} && ${Bool[${NoCastList.Find[${Target.DisplayName}]}]}) || ${Me.Moving} || ${Bool[${Target.Mezzed}]}) /goto :exit
	/if (${Bool[${HornofUnity}]} && !${Bool[${Me.Song[Blessing of Unity]}]} && !${FindItem[Miniature Horn of Unity].Timer}) {
		/call click_horn_of_unity 	
		/goto :exit
	}
	:exit
/return




sub castNecroDot()
	DEBUG \atcastNecroDot()\ax 
	/doevents
	ISDEAD
	/if (!${Bool[${dotR${set}-${gem}}]}) /goto :exitcastNecroDot
	/if ((${AutoBot} && ${Bool[${NoCastList.Find[${Target.DisplayName}]}]}) || ${Me.Moving} || ${Bool[${Target.Mezzed}]}) /goto :exitcastNecroDot2

	/if (${Spawn[${Target2Kill}].LineOfSight} && ${Spawn[${Target2Kill}].Distance} <= ${Spell[${dotR${set}-${gem}}].MyRange} && ${Me.CurrentMana} >= ${Spell[${dotR${set}-${gem}}].Mana}) {
		/if ((${Target.BuffDuration[${dotR${set}-${gem}}]} <= 6 && ${Spawn[${Target.Buff[${dotR${set}-${gem}}].Caster}].ID} == ${Me.ID}) || !${Target.Buff[${dotR${set}-${gem}}].ID}) {
			/call isTargetDead
			/if (${Macro.Return}) /goto :exitcastNecroDot
			VOUT Casting \a-r${dotR${set}-${gem}}\ax on [\a-w${Spawn[${Target2Kill}].CleanName}\ax] \ag${set}\ax\aw:\ax\ag${gem}
			/call MQ2Cast "${dotR${set}-${gem}}" gem${gem} 5s breakCast -targetid|${Target2Kill}
		}
	}

	:exitcastNecroDot
	
	
	:exitcastNecroDot2
/return


sub mem_spell_set()

/varset gem ${Math.Calc[${gem}+1]}
	/if (${gem} > 11 ) {
		/varset gem 1
		/varset set ${Math.Calc[${set}+1]}
		/if (${set} > ${SpellSets}) /varset set 1
		/if (${set} > 1) {
			VOUT Meming spell set \awcore${set}\ax
			/memspellset core${set}
			/delay 1s
		}

	}



/return









sub cast_Call_for_Blood()
	DEBUG \atcastCallforBlood()\ax 
	ISDEAD
	/doevents
	/if ((${AutoBot} && ${Bool[${NoCastList.Find[${Target.DisplayName}]}]}) || ${Me.Moving} || ${Bool[${Target.Mezzed}]} || ${Bool[${Me.Song[Chaotic Power]}]}) /goto :exit
	/if (${Spawn[${Target2Kill}].Distance} < ${Spell[${CallforBlood}].MyRange} && ${Me.CurrentMana} > ${Spell[${CallforBlood}].Mana} && !${Bool[${Me.Song[Chaotic Power]}]}) {
		VOUT Casting \a-r${CallforBlood}\ax on [\a-w${Spawn[${Target2Kill}].CleanName}\ax].
		/call MQ2Cast "${CallforBlood}" -targetid|${Target2Kill}		

	}
	:exit
	/call createTimer timer_Call_for_Blood ${tCall_for_Blood}
/return










|*** Sacrifice stuff

/declare sactarget string local


|/if (!${Spawn[tilane].Dead}) {
|	/varset sactarget ${Target.CleanName}
|	/delay 1
|	/target tilane
|	/delay 1
|} else 

/if (!${Spawn[Bloodbliss].Dead}) {
	 /varset sactarget ${Target.CleanName}
	 /delay 1
	 /target Bloodbliss
	/delay 1
}



/autoinventory
/call MQ2Cast "Sacrifice" gem12 5s -targetID|${Spawn[${sactarget}].ID} 
/delay 2s
/autoinventory
***|





sub dotCycle()
	DEBUG \atdotCycle()\ax 
	/doevents
	/if (${AutoBot} && (${Bool[${NoCastList.Find[${Target.DisplayName}]}]} || ${Me.Moving} || ${Bool[${Target.Mezzed}]})) /goto :exitdotCycle
	ISDEAD
	/declare x int local
	/declare y int local
	/for x 1 to ${SpellSets}
		/delay 1
		/call isTargetDead
		/if (${Macro.Return}) /goto :exitdotCycle
		/for y 1 to 12
			/if (!${Bool[${dotR${x}-${y}}]}) /goto :skipdot
			/delay 5s !${Me.SpellInCooldown}
			/if (${Spawn[${Target2Kill}].LineOfSight} && ${Spawn[${Target2Kill}].Distance} <= ${Spell[${dotR${x}-${y}}].MyRange} && ${Me.CurrentMana} >= ${Spell[${dotR${x}-${y}}].Mana}) {
				/if ((${Target.BuffDuration[${dotR${x}-${y}}]} <= 1 && ${Spawn[${Target.Buff[${dotR${x}-${y}}].Caster}].ID} == ${Me.ID}) || !${Target.Buff[${dotR${x}-${y}}].ID}) {
					/call castCallforBlood
					VOUT Casting \a-r${dotR${x}-${y}}\ax on [\a-w${Spawn[${Target2Kill}].CleanName}\ax]. (Set:${x} Spell:${y})
					/call MQ2Cast "${dotR${x}-${y}}" gem${y} 5s breakCast -targetid|${Target2Kill}
					/doevents
				}
			}
		:skipdot
		/next y
	/next x
	:exitdotCycle
/return






