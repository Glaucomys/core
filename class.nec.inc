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
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}(${cinfo}${version${Me.Class.ShortName}}\ax${cbug})\ax
	
			/call sac_toon()

|**	
	DEBUGCLASS Loop start: ${cbug}core_NEC()\ax
	/doevents
	ISDEAD
	CHECKTIE

	/if (${AutoBot} && !${Me.Invis}) {
		/call check_engaged
		/if (!${Macro.Return} && ${usePet}) /call check_pet
	}

	:killtarget
	/call prep_combat
	/if (!${Bool[${Macro.Return}]}) /goto :exit

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
	}
	/call use_mod_rod
	/if (${AutoBot}) {	
		/if (${switchHealCure}) /call cast_cure
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
	}
	/if (${AutoBot} && !${Spawn[${Target2Kill}].ID} || !${AutoBot} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Bool[${Macro.Return}]}) /goto :killtarget
	}
	**|
	:exit	
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
	DEBUGCLASS \atdisc.${Me.Class.ShortName}()\ax 
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
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub clickDPS_NEC()
	DEBUGCLASS \atclickDPS.NEC()\ax 
	/doevents
	ISDEAD
	/if ((${AutoBot} && ${Bool[${NoCastList.Find[${Target.DisplayName}]}]}) || ${Me.Moving} || ${Bool[${Target.Mezzed}]}) /goto :exit
	/if (${Bool[${HornofUnity}]} && !${Bool[${Me.Song[Blessing of Unity]}]} && !${FindItem[Miniature Horn of Unity].Timer}) {
		/call click_horn_of_unity 	
		/goto :exit
	}
	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub castNecroDot()
	DEBUGCLASS \atcastNecroDot()\ax 
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



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
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



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub cast_Call_for_Blood()
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



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub sac_toon()
	/declare sactarget string local Jraoz
	/if (${Spawn[pc ${sactarget}].ID}) {
		 /delay 1
		 /target id ${spawn[${sactarget}].ID}
		/delay 1


		/autoinventory
		/call MQ2Cast "Sacrifice" gem12 5s -targetID|${Spawn[${sactarget}].ID} 
		/delay 15s ${Cursor.ID}
		/autoinventory
		/delay 3s
	}
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub dotCycle()
	DEBUGCLASS \atdotCycle()\ax 
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


