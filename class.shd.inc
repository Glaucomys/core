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

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/call send_pet
		/call stick_to_target
		/call use_skill_melee
		/if (${switchAgro}) /call get_agro_${Me.Class.ShortName}
		/call disc_${Me.Class.ShortName}
		/if (${switchdeBuffEradicate}) /call cast_eradicate_magic ${Target2Kill}
		/if (${switchdeBuffCripple}) /call cast_cripple_${Me.Class.ShortName} ${Target.ID}
		/call click_BP
		/call click_DPS_${Me.Class.ShortName}
		/call stick_to_target
		/if (${DoDoT}) /call cast_DOT
		/call use_skill_melee
		/if (${DoNuke}) /call cast_nuke
		/if (${switchAgro}) /call get_agro_${Me.Class.ShortName}
		/call stick_to_target
		/call use_skill_melee
		/if (${doclickItem}) /call click_item TRUE
		/if (${switchdeBuffSnare}) /call cast_aa_snare ${Target2Kill}
		/if (${switchHarmonious}) /call check_harmonious
		/if (${switchSkin} && !${timer_check_Skin}) /call check_skin	
		/if (${switchStance} && (!${timer_check_Stance} || !${Bool[${Me.Buff[${Stance}]}]})) /call check_stance
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
	}
	/if (${coreAuto}) {	
		/call use_mod_rod
		/if (${switchHealCure}) /call cast_cure
	}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} || !${coreAuto} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Macro.Return}) /goto :killtarget
	}
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
	/call is_target_dead
	/if (${Macro.Return}) /return
	CHECKTIE
	/if (${switchBanestrike}) /call cast_AA_banestrike ${Target2Kill}
	
	| check challenge
	/call cast_challenge ${Target2Kill}
	
	| get me sum...
	/if (${switchAgro}) /call get_agro_${Me.Class.ShortName}

	| tap disc
	/if (${switchHealSelf} && ${Me.PctHPs} < 50*.${tmpHealOverride} && ${Me.CombatAbilityReady[${DiscRancor}]}) /call core_cast "${DiscRancor}" disc ${Target2Kill}

	| heal clickies
	/if (${switchHealSelf} && ${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0
	/if (${switchHealSelf} && ${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0


	
	/if (${Me.CombatAbilityReady[${Withstand}]} && ${Me.CurrentEndurance} > ${Spell[${Withstand}].EnduranceCost}) /call core_cast "${Withstand}" disc 0
	/if (${Me.AltAbilityReady[Thought Leech]} &&  ${Me.PctMana} <= 60*.${tmpHealOverride}) /call core_cast "Thought Leech" alt ${Target2Kill}
	/if (${Me.CombatAbilityReady[${BladeStrike}]} && ${Me.CurrentEndurance} > ${Spell[${BladeStrike}].EnduranceCost}) /call core_cast "${BladeStrike}" disc ${Target2Kill}
	/if (${switchHealSelf} &&  ${Me.AltAbilityReady[Vicious Bite of Chaos]} && ${Target.Distance} <= ${Spell[Vicious Bite of Chaos].MyRange}) /call core_cast "Vicious Bite of Chaos" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[Soul Flay]}) /call core_cast "Soul Flay" alt 0
	
	/if (${switchHealSelf} && ${Me.PctHPs} < 30*.${tmpHealOverride} && ${Me.AltAbilityReady[Leech Touch]}) /call core_cast "Leech Touch" alt ${Target2Kill}
	/if (${switchHealSelf} && ${Me.PctHPs} < 50*.${tmpHealOverride} && ${Me.CombatAbilityReady[${DiscLeech}]} && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${combatRadius}*2 targetable]} > 2 ) /call core_cast "${DiscLeech}" disc 0
	/if (!${Bool[${Me.ActiveDisc}]} && ${Me.PctHPs} < 50*.${tmpHealOverride} && ${Me.CombatAbilityReady[Deflection Discipline]}) /call core_cast "Deflection Discipline" disc 0

	/if (${switchdeBuffSnare}) /call cast_AA_snare ${Target2Kill}
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
	/call is_target_dead
	/if (${Macro.Return}) /return
	/call use_skill_melee
	/if (${switchPcPet}) /call send_pet

	| single hate
	/if (${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]} && ${Me.AltAbilityReady[Stream of Hatred]}) /call core_cast "Stream of Hatred" alt ${Target2Kill}
	/if ((${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) && ${Me.SpellReady[${Hate1}]} && ${Target.Distance} <= ${Spell[${Hate1}].MyRange} && !${Target.Buff[${Hate1}].ID} && !${Target.Buff[${Hate2}].ID}) /call core_cast "${Hate1}" 0 ${Target2Kill}
	/if ((${Me.PctAggro} < 80 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) && ${Me.SpellReady[${Hate2}]} && ${Target.Distance} <= ${Spell[${Hate2}].MyRange} && !${Target.Buff[${Hate1}].ID} && !${Target.Buff[${Hate2}].ID}) /call core_cast "${Hate2}" 0 ${Target2Kill}


	| AE Hate
	/varset mobAECount ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius 50 playerstate 4 targetable]}
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
	/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${combatRadius}*2 playerstate 4 targetable]} >= 2 && !${SpawnCount[pc enchanter radius ${combatRadius}*2]} && ${Me.SpellReady[${AELifeTap}]}) /call core_cast "${AELifeTap}" 0 ${Target2Kill}
	
	| fake hate
	/if ((${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) && ${Me.AltAbilityReady[Ageless Enmity]}) /call core_cast "Ageless Enmity" alt ${Target2Kill}
	| named fake hate
	/if ((${Me.PctAggro} < 90 || ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) && (${NameList.Find[${Target.DisplayName}]} && !${switchOverrideName}) && ${Me.AltAbilityReady[Mindless Hatred]} && ${Target.Level} <= 110) /call core_cast "Mindless Hatred" alt ${Target2Kill}
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
	/call use_skill_melee
	/call click_horn_of_unity 	
/return



|***
 *	DES: cripples the target
 *	USE: debufs /debuffcripple
 *	INI: 
 *  NOTE: 
 ***|
sub cast_cripple_SHD(int _tmpID)
	DEBUGCLASS ${cbug}cast_cripple_SHD()\ax
	/if (${timer_cripple_${_tmpID}} || !${_tmpID} || !${Target.ID}) /return
	AMIDEAD
	CHECKEXIT
	/doevents
	/declare _resistCount int local 0
	/call is_target_dead
	/if (${Macro.Return}) /return
	
	/if (${Target.Crippled.Duration}) {
		/call create_timer timer_cripple_${_tmpID} ${Target.Crippled.Duration.TotalSeconds}s
		/return
	}

	:loopcripple
	/if (${Target.Distance} <= ${Spell[${spellCripple}].MyRange} && ${Me.CurrentMana} > ${Spell[${spellCripple}].Mana} && ${Target.LineOfSight}) {
		/call core_cast "${spellCripple}" 0 ${_tmpID}
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_cripple_${_tmpID} ${Spell[${spellCripple}].Duration.TotalSeconds}s
		} else /if (${Select[${castReturn},CAST_RESIST]}) {
			/varcalc _resistCount ${_resistCount}
			/if (${_resistCount} >= 3) {
				OUT ${ctar}${Target.CleanName}\ax resisted ${csp}${spellCripple}\ax ${cinfo}${_resistCount}\ax times, moving on!
				/call create_timer timer_cripple_${_tmpID} ${Spell[${spellCripple}].Duration.TotalSeconds}m
			} else {
				/delay 5
				/goto :loopcripple
			}
		}
	}
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
	/call is_target_dead
	/if (${Macro.Return}) /return		
	/if (!${Bool[${Me.Buff[${Skin}]}]} && ${Me.SpellReady[${Skin}]}) {
		/call core_cast "${Skin}" 0
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
	/call is_target_dead
	/if (${Macro.Return}) /return		

	| defensive	
	/if (${Me.CombatAbilityReady[${DefensiveDisc}]} && ${Me.CurrentEndurance} > ${Spell[${DefensiveDisc}].EnduranceCost}) {
		/call core_cast "${DefensiveDisc}" disc 0
	} else /if (!${Me.CombatAbilityReady[${DefensiveDisc}]} && ${Me.CurrentEndurance} > ${Spell[${DefensiveDisc2}].EnduranceCost} && !${Bool[${Me.ActiveDisc}]}) {
		/call core_cast "${DefensiveDisc2}" disc 0
	} else /if (!${Me.CombatAbilityReady[${DefensiveDisc2}]} && ${Me.CurrentEndurance} > ${Spell[${DefensiveDisc3}].EnduranceCost} && !${Bool[${Me.ActiveDisc}]}) {
		/call core_cast "${DefensiveDisc3}" disc 0
	}
	
	/if (${Me.AltAbilityReady[T`Vyl's Resolve]} && ${Target.Distance} <= ${Spell[T`Vyl's Resolve].MyRange}) /call core_cast "T`Vyl's Resolve" alt ${Target2Kill}
	/if (${Me.CombatAbilityReady[${CrimsonBlade}]} && ${Me.CurrentEndurance} > ${Spell[${CrimesonBlade}].EnduranceCost}) /call core_cast "${CrimsonBlade}" disc 0
	/call cast_AA_spire
	/if (${Me.AltAbilityReady[Veil of Darkness]}) /call core_cast "Veil of Darkness" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[Harm Touch]}) /call core_cast "Harm Touch" alt ${Target2Kill}
	/if (${switchAgro} && ${Me.AltAbilityReady[Scourge Skin]}) /call core_cast "Scourge Skin" alt

	| AA Visage of Decay/Death stacking issues
	/if (${Me.AltAbilityReady[Visage of Death]} && !${Me.Buff[Visage of Decay].ID}) {
		/call core_cast "Visage of Death" alt 0
	} else /if (${Me.AltAbilityReady[Visage of Decay]} && !${Me.AltAbilityReady[Visage of Death]} && !${Me.Buff[Visage of Death].ID}) {
		/call core_cast "Visage of Decay" alt 0
	}

	| swarm pets
	/if (${switchPcSwarm} && ${Me.AltAbilityReady[Chattering Bones]}) /call core_cast "Chattering Bones" alt ${Target2Kill}
	
	| epic 2.0
	/if (${Me.ItemReady[${Epic}]} && ${Spell[${FindItem[${Epic}].Spell}].Stacks} && !${Me.Buff[${FindItem[${Epic}].Spell}].ID} && !${Me.Song[${FindItem[${Epic}].Spell}].ID}) /call core_cast "${Epic}" item 0

	/call use_glyph	
	/call synergy_click_Circle
	/if (${doclickItemBurn}) /call click_item_burn
/return



|***
 *	DES: Class controls
 *	USE: /shd [] [##]
 *	INI: 
 *  NOTE:
 ***|
#bind setSHD /shd
sub Bind_setSHD(string _classType, string _classVerbage)
	DEBUGCLASS ${cbug}Bind_setSHD(type:${_classType}, verbage:${_classVerbage})\ax
	/if (!${Select[${Me.Class.ShortName},SHD]}) {
		/call echo_bitch EQBC
		/return
	}
	/declare _count int local 0


	| on/off switch for Vizat's Skin
	/if (${Bool[${_classType.Find[skin]}]}) {
		/call writeINI switchSkin Buffs ${If[${switchSkin},FALSE,TRUE]}  
		/call echos switch "Buff ${sep} ${Skin}" switchSkin 0 0
	
	| on/off switch for Harmonious
	} else /if (${Bool[${_classType.Find[harmonious]}]}) {
		/call writeINI switchHarmonious Buffs ${If[${switchHarmonious},FALSE,TRUE]}  
		/call echos switch "Buff ${sep} ${Harmonious}" switchHarmonious 0 0

	| on/off switch for Stance
	} else /if (${Bool[${_classType.Find[stance]}]}) {
		/call writeINI switchStance Buffs ${If[${switchStance},FALSE,TRUE]}  
		/call echos switch "Buff ${sep} ${Stance}" switchStance 0 0

	| spire control
	} else /if (${Bool[${_classType.Equal[spiregroup]}]} || ${Bool[${_classType.Equal[spireraid]}]}) {
		/declare _spiretype string local ${_classType.Left[5].Lower}${_classType.Mid[6,1].Upper}${_classType.Right[-6].Lower}
		/if (${Range.Between[0,3:${Int[${_classVerbage}]}]}) {
			/call writeINI ${_spiretype} Buffs ${_classVerbage}
		} 
		/declare _listout 	string 	local
		/declare _sep 			bool 		local
		/for _count 1 to ${typeSpire.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${${_spiretype}} == ${typeSpire.Arg[${_count},|]},${cgood},${cinfo}]}${typeSpire.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /${Me.Class.ShortName.Lower} ${_classType.Lower} [${_listout}]	


	} else {
		/call echos listtype ${Me.Class.ShortName.Lower} ${Me.Class.ShortName.Lower} 0 ##
	} 
/return

