|***
 *	class.shd.inc
 *	Core Shadow Knight Include
 *
 * 
 * Frosty Gem of Enhanced Protection
 ***|



|***
 *	DES: Main Shadow Knight Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_SHD()
	DEBUG Loop start: ${cbug}core_SHD()\ax
	/doevents
	ISDEAD
	CHECKTIE
	
	/if (${AutoBot}) {
		/call check_engaged
		/if (!${Macro.Return} && ${usePet}) /call check_pet
	}	
	
	/call check_harmonious
	
	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /goto :exit

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/call send_pet
		/call stick_to_target
		/call use_skill_melee
		/if (${setAgroMode}) /call get_agro_${Me.Class.ShortName}
		/call disc_${Me.Class.ShortName}
		/if (${switchdeBuffEradicate}) /call cast_eradicate_magic ${Target2Kill}
		/if (${switchdeBuffCripple}) /call cast_cripple_${Me.Class.ShortName} ${Target.ID}
		/call click_BP
		/call click_DPS_${Me.Class.ShortName}
		/call stick_to_target
		/if (${DoDoT}) /call cast_DOT
		/call use_skill_melee
		/if (${DoNuke}) /call cast_nuke
		/if (${setAgroMode}) /call get_agro_${Me.Class.ShortName}
		/call stick_to_target
		/call use_skill_melee
		/if (${doclickItem}) /call click_item TRUE
		/if (${switchdeBuffSnare}) /call cast_aa_snare ${Target2Kill}
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
	}
	/call use_mod_rod
	/call check_harmonious
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
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub disc_SHD()
	DEBUG ${cbug}disc_SHD()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/call is_target_dead
	/if (${Macro.Return} || !${Target.ID}) /goto :exit
	CHECKTIE
	/if (${setAgroMode}) /call get_agro_${Me.Class.ShortName}
	/if (${switchBanestrike}) /call cast_AA_banestrike ${Target2Kill}
	/if (${Me.CombatAbilityReady[${Withstand}]} && ${Me.CurrentEndurance} > ${Spell[${Withstand}].EnduranceCost}) /call core_cast "${Withstand}" disc 0
	/if (${Me.AltAbilityReady[Thought Leech]} &&  ${Me.PctMana} <= 60*.${tmpHealOverride}) /call core_cast "Thought Leech" alt ${Target2Kill}
	/if (${Me.CombatAbilityReady[${BladeStrike}]} && ${Me.CurrentEndurance} > ${Spell[${BladeStrike}].EnduranceCost}) /call core_cast "${BladeStrike}" disc ${Target2Kill}
	/if (${Me.AltAbilityReady[Vicious Bite of Chaos]} && ${Target.Distance} <= ${Spell[Vicious Bite of Chaos].MyRange}) /call core_cast "Vicious Bite of Chaos" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[Soul Flay]}) /call core_cast "Soul Flay" alt 0
	
	/if (${Me.PctHPs} < 30*.${tmpHealOverride} && ${Me.AltAbilityReady[Leech Touch]}) /call core_cast "Leech Touch" alt ${Target2Kill}
	/if (${Me.PctHPs} < 50*.${tmpHealOverride} && ${Me.CombatAbilityReady[${DiscLeech}]} && ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${combatRadius}*2 targetable]} > 2 ) /call core_cast "${DiscLeech}" disc 0
	/if (!${Bool[${Me.ActiveDisc}]} && ${Me.PctHPs} < 50*.${tmpHealOverride} && ${Me.CombatAbilityReady[Deflection Discipline]}) /call core_cast "Deflection Discipline" disc 0

	/if (${switchdeBuffSnare}) /call cast_AA_snare ${Target2Kill}
	:exit
/return



|***
 *	DES: take what you want. there is no try...
 *	USE: /call get_agro_${Me.Class.ShortName
 *	INI: 
 *  NOTE: 
 ***|
sub get_agro_SHD()
	DEBUG ${cbug}get_agro_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/call is_target_dead
	/if (${Macro.Return}) /goto :exit
	/call use_skill_melee
	/if (${usePet}) /call send_pet

	| single hate
	/if (${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]} && ${Me.AltAbilityReady[Stream of Hatred]}) /call core_cast "Stream of Hatred" alt ${Target2Kill}
	/if (${Me.SpellReady[${ChallengeSpell}]} && ${Target.Distance} <= ${Spell[${ChallengeSpell}].MyRange} && !${Target.Buff[${ChallengeSpell}].ID}) /call core_cast "${ChallengeSpell}"  0 ${Target2Kill}
	/if ((${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) && ${Me.SpellReady[${Hate1}]} && ${Target.Distance} <= ${Spell[${Hate1}].MyRange} && !${Target.Buff[${Hate1}].ID} && !${Target.Buff[${Hate2}].ID}) /call core_cast "${Hate1}" 0 ${Target2Kill}
	/if ((${Me.PctAggro} < 80 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) && ${Me.SpellReady[${Hate2}]} && ${Target.Distance} <= ${Spell[${Hate2}].MyRange} && !${Target.Buff[${Hate1}].ID} && !${Target.Buff[${Hate2}].ID}) /call core_cast "${Hate2}" 0 ${Target2Kill}


	| AE Hate
	/varset mobAECount ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius 50 playerstate 4 targetable]}
	/if  (${mobAECount} > 1) {
		/if (${Me.SpellReady[${AEHate1}]}) /call core_cast "${AEHate1}" 0 0
	} else /if (${mobAECount} > 2) {
		/if (${Me.SpellReady[${AEHate2}]}) /call core_cast "${AEHate2}" 0 0
	} else /if (${mobAECount} > 4) {
		/if (${Me.AltAbilityReady[Explosion of Hatred]}) /call core_cast  "Explosion of Hatred" alt 0
	} else /if (${mobAECount} > 6) {
		/if (${Me.AltAbilityReady[Explosion of Spite]}) /call core_cast "Explosion of Spite" alt 0
	}

	| AE lifetap	
	/if (${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${combatRadius}*2 playerstate 4 targetable]} >= 2 && !${SpawnCount[pc enchanter radius ${combatRadius}*2]} && ${Me.SpellReady[${AeLifeTap}]}) /call core_cast "${AeLifeTap}" 0 ${Target2Kill}
	
	| fake hate
	/if ((${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) && ${Me.AltAbilityReady[Ageless Enmity]}) /call core_cast "Ageless Enmity" alt ${Target2Kill}
	| named fake hate
	/if ((${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) && ${NameList.Find[${Target.DisplayName}]} && ${Me.AltAbilityReady[Mindless Hatred]} && ${Target.Level} <= 110) /call core_cast "Mindless Hatred" alt ${Target2Kill}
	/if ((${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) && ${NameList.Find[${Target.DisplayName}]} && ${Me.CombatAbilityReady[${DiscFakeHate}]} && ${Me.CurrentEndurance} > ${Spell[${DiscFakeHate}].EnduranceCost}) /disc ${DiscFakeHate}

	:exit
/return



|***
 *	DES: DPS Clickies
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_SHD()
	DEBUG ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	ISDEAD
	/doevents
	/call use_skill_melee
	/if (${usePet}) /call send_pet
	/call click_horn_of_unity 	
	:exit
/return



|***
 *	DES: Patch healing. cause clerics are slow fuckers
 *	USE: this shits auto, learn to love it.
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_SHD()
	DEBUG ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	ISDEAD
	/call check_exit
	/if (${Macro.Return} || ${Me.PctHPs} > 81*.${tmpHealOverride}) /goto :exit

	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0
	:exit
/return



|***
 *	DES: cripples the target
 *	USE: debufs /debuffcripple
 *	INI: 
 *  NOTE: 
 ***|
sub cast_cripple_SHD(int _tmpID)
	DEBUG ${cbug}cast_cripple_SHD()\ax
	| /declare _slowString string local
	/doevents
	ISDEAD
	/declare _resistCount int local
	/call check_exit
	/if (${Macro.Return} || ${timer_cripple_${_tmpID}} || !${_tmpID}) /goto :exit
	/call is_target_dead
	/if (${Macro.Return} || !${Target.ID}) /goto :exit
	
	/if (${Target.Crippled.Duration}) {
		/call create_timer timer_cripple_${_tmpID} ${Target.Crippled.Duration.TotalSeconds}s
		/goto :exit
	}

	:loopcripple
	/if (${Target.Distance} <= ${Spell[${spellCripple}].MyRange} && ${Me.CurrentMana} > ${Spell[${spellCripple}].Mana} && ${Target.LineOfSight}) {
		/delay 2.25s ${Me.SpellReady[${spellCripple}]}
		/call core_cast "${spellCripple}" 0 ${_tmpID}
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_cripple_${_tmpID} ${Spell[${spellCripple}].Duration.TotalSeconds}s
		} else /if (${Select[${castReturn},CAST_RESISTED,CAST_RESIST,CAST_RESIST,CAST_RESIST]}) {
			/varcalc _resistCount ${_resistCount}
			/if (${_resistCount} >= 3) {
				VOUT ${ctar}${Target.CleanName}\ax resisted ${csp}${spellCripple}\ax ${cinfo}${_resistCount}\ax times, moving on!
				/call create_timer timer_cripple_${_tmpID} ${Spell[${spellCripple}].Duration.TotalSeconds}m
			} else {
				/delay 5
				/goto :loopcripple
			}
		}
	}
	:exit
/return