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
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}()\ax
	/doevents
	AMIDEAD

	CHECKTIE
	
	CHECKREZ
	/if (${coreAuto} && !${Me.Invis}) {
		/call check_engaged
		/if (!${Macro.Return}) {
			/if (${switchPcPet}) /call check_pet
		}
	}


	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /return

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
		/call set_combat NUKE
		/if (${coreAuto} && ${switchPcPet} && ${Pet.ID}) /call send_pet
		/if (${switchdeBuffSlow} && !${Defined[timer_slow_${Target2Kill}]} && ${Target.Body.Name.Equal[Undead]}) /call cast_AA_slow ${Target2Kill} "Death's Malaise"
		/call disc_${Me.Class.ShortName}
		/if (${switchPcSwarm} && ${Target.PctHPs} > 15 && ${SpawnCount[corpse radius ${Spell[Wake the Dead].MyRange}]} > 3) /call core_cast "Wake the Dead" alt ${Target2Kill}
		/if (${switchdeBuffEradicate}) /call cast_eradicate_magic ${Target2Kill}
		/if (${switchdeBuffMalo}) /call cast_malo_${Me.Class.ShortName} ${Target.ID}
		/call click_BP
		/call click_DPS_${Me.Class.ShortName}
		/if (!${timer_check_blood}) /call cast_blood ${Target2Kill}
		/if (${DoNuke}) /call cast_nuke
		/if (${DoDoT}) /call cast_DOT

		/if (${doclickItem}) /call click_item TRUE
		/if (${switchdeBuffSnare}) /call cast_aa_snare ${Target2Kill}

		
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
	}
	CHECKREZ
	/if (${coreAuto}) {	
		/call use_mod_rod
		/if (${switchHealCure}) /call cast_cure
		/if (${switchHealPet} && ${Me.Pet.ID}) /call pet_heal_${Me.Class.ShortName}
	}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} || !${coreAuto} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Macro.Return}) /goto :killtarget
	}

/return





|***
 *	DES: Cast Necro DoT's
 *	USE: /call cast_DOT_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 

sub cast_DOT_NEC()
	DEBUGCAST ${cbug}cast_DOT_NEC(count ${DoDoT})\ax
	/call get_assist_target
	/declare _count int local 0
	/for _count 1 to ${DoDoT}
		AMIDEAD
		CHECKEXIT
		/call is_target_dead
		/if (${Macro.Return}) /return

		/if ((${Target.BuffDuration[${DoT${_count}chk}]} <= 2 && ${Spawn[${Target.Buff[${DoT${_count}chk}].Caster}].ID} == ${Me.ID}) || !${Target.Buff[${DoT${_count}chk}].ID}) {
			/call core_cast "${DoT${_count}}" 0  ${Target2Kill} cast_break
		}
	/next _count
/return
 ***|




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
	DEBUGCLASS ${cbug}disc_${Me.Class.ShortName}()\ax 
	CHECKEXIT
	/doevents
	CHECKTIE
	/call is_target_dead
	/if (!${Macro.Return}) /return
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}

	| heal clickies
	/if (${switchHealSelf} && ${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0
	/if (${switchHealSelf} && ${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0

	| aa wake the dead
	/if (${switchPcSwarm} && ${Target.PctHPs} > 15 && ${SpawnCount[corpse radius ${Spell[Wake the Dead].MyRange}]} > 3) /call core_cast "Wake the Dead" alt ${Target2Kill}
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_NEC()
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	CHECKEXIT
	CHECKTIE
	/doevents
	/call click_horn_of_unity 	
/return



|***
 *	DES: casts call of blood line of spells
 *	USE: /call cast_blood [ID]
 *	INI: 
 *  NOTE: 
 ***|
sub cast_blood(int _tmpID)
	CHECKEXIT
	/doevents
	/call create_timer timer_check_blood ${tCheck_Blood}
	/if (${Bool[${Me.Song[Chaotic Power]}]}) /return
	/call is_target_dead
	/if (${Macro.Return}) /return
	/call core_cast "${spellBlood}" 0 ${_tmpID}	
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
 *	DES: malo the target
 *	USE: debufs /debuff malo
 *	INI: 
 *  NOTE: 
 ***|
sub cast_malo_NEC(int _tmpID)
	DEBUGCLASS ${cbug}cast_malo_NEC()\ax
	/doevents
	CHECKEXIT
	/if (${timer_malo_${_tmpID}} || !${_tmpID}) /return
	/call is_target_dead
	/if (${Macro.Return} || !${Target.ID}) /return
	
	/if (${Target.Maloed.Duration}) {
		/call create_timer timer_malo_${_tmpID} ${Target.Maloed.Duration.TotalSeconds}s
		/return
	}
	/declare _resistCount int local 0

	:loopmalo
	/if (${Target.Distance} <= ${Spell[${spellMalo}].MyRange} && ${Me.CurrentMana} > ${Spell[${spellMalo}].Mana} && ${Target.LineOfSight}) {
		/call core_cast "${spellMalo}" alt ${_tmpID}
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_malo_${_tmpID} ${Spell[${spellMalo}].Duration.TotalSeconds}s
		} else /if (${Select[${castReturn},CAST_RESIST]}) {
			/varcalc _resistCount ${_resistCount}
			/if (${_resistCount} >= 3) {
				OUT ${ctar}${Target.CleanName}\ax resisted ${csp}${spellMalo}\ax ${cinfo}${_resistCount}\ax times, moving on!
				/call create_timer timer_malo_${_tmpID} ${Spell[${spellMalo}].Duration.TotalSeconds}m
			} else {
				/delay 5
				/goto :loopmalo
			}
		}
	}
/return



 |***
 *	DES: Necromancer burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_NEC()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	CHECKEXIT
	/doevents
	
	/call cast_AA_spire
	
	| aa swarm of decay
	/if (${switchPcSwarm}) /call core_cast "Swarm of Decay" alt ${Target2Kill}
	| aa Rise of bones
	/if (${switchPcSwarm}) /call core_cast "Rise of Bones" alt ${Target2Kill}

	/call use_glyph
	/call synergy_click_Circle
	/if (${doclickItemBurn}) /call click_item_burn
/return



|***
 *	DES: Class controls
 *	USE: /nec []
 *	INI: 
 *  NOTE:
 ***|
#bind setNEC /nec
sub Bind_setNEC(string _classType, string _classVerbage)
	DEBUG ${cbug}Bind_setNEC(type:${_classType}, verbage:${_classVerbage})\ax
	/if (!${Select[${Me.Class.ShortName},NEC]}) {
		/call echo_bitch EQBC
		/return
	}
	/declare _count	int local 0

	
	/if (${Bool[${_classType.Equal[TEST]}]}) {
		| /call Event_SetVar NA switchNukeSummer Spells ${If[${switchNukeSummer},FALSE,TRUE]}

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
		VOUT /${Me.Class.ShortName.Lower} ${_classType.Lower} [${_listout}]	


	} else {
		/call echos listtype ${Me.Class.ShortName.Lower} ${Me.Class.ShortName.Lower} 0 0
	} 
/return