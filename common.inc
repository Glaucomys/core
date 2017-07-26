|***
 *	common.inc
 *	Core Common Routines/Functions
 *
 *
 ***|



|***
 *	DES: Checks EQBCS connection and channel
 *	USE: /call check_one_mind
 *	INI: 
 *  NOTE: 
 ***| 
sub check_one_mind()
	DEBUG ${cbug}check_one_mind()\ax
	/if (!${EQBC.Connected} && ${Bool[${EQBCServer}]}) {
		/bccmd connect ${EQBCServer} ${EQBCPort}
		/delay 15s !${EQBC.Connected}
	}
	/call create_timer timer_check_EQCB ${tCheck_EQBC}
/return



|***
 *	DES: checks to make sure all needed plugins are loaded.
 *	USE: /call check_plugin
 *	INI: 
 *  NOTE: 
 ***|
sub check_plugin()
	DEBUG ${cbug}check_plugin()\ax
	/declare _count int local
	/declare _plug string local ${Ini[${INICore},plugin,plist]}

	/for _count 1 to ${_plug.Count[|]}
		ISDEAD
		/if (!${Bool[${Plugin[${_plug.Arg[${_count},|]}]}]})  {
			NOUT Plugin \a-w${_plug.Arg[${_count},|]}\ax is missing. Loading it.
			/squelch /plugin ${_plug.Arg[${_count},|]}
		}
	/next _count	
	:exit
/return



|***
 *	DES: echos misc stuff to EQBC
 *	USE: /call echos [type] "desc text" [VARIABLE] [Target ID] [FORCED]
 *	INI: 
 *  NOTE: 
 ***|
sub echos(string _type, string _verbage, string _tmpVariable, int _tmpID, bool _forced)
	DEBUG ${cbug}echos(type ${_type}, verbage: ${_verbage})\ax

	/if (${_type.Equal[switch]}) {
		| on/off switch echos
		NOUT ${_verbage} ${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}
	} else /if (${_type.Equal[number]}) {
		| variable %s
		NOUT ${_verbage} ${sep} ${cinfo}${_tmpVariable}\ax
	} else /if (${_type.Equal[startup]}) {
		| core startup
		NOUT Initializing Core: ${cinfo}${versionCore}\ax ${sep} ${Me.Class} ${cinfo}${version${Me.Class.ShortName}}\ax
	} else /if (${_type.Equal[zone]}) {
		| zone initialization
		NOUT ${onotice} ${sep} Setting: ${cinfo}${Zone}\ax: \a-w${Zone.ShortName}\ax [\a-y${Zone.ID}\ax]
	} else /if (${_type.Equal[help]}) {
		| help command
		| NOUT Use ${cinfo}/info\ax to list '${cinfo}/\ax' commands.
	} else /if (${_type.Equal[ending]}) {
		| ending macro
		NOUT ${owarning} ${sep} ${cbad}ENDING\ax Core ${cinfo}${versionCore}\ax
	} else /if (${_type.Equal[needtarget]}) {
		| need target
		NOUT ${onotice} ${sep} You need a target.
	} else /if (${_type.Equal[needcursor]}) {
		| need cursor item
		NOUT ${onotice} ${sep} need something on cursor.
	} else /if (${_type.Equal[hunt]}) {
		| hunt
		NOUT Hunt ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax] ${sep} ${If[${Spawn[${_tmpID}].LineOfSight},${cgood}${Int[${Spawn[${_tmpID}].Distance}]}\ax,${cbad}${Int[${Spawn[${_tmpID}].Distance}]}\ax]}
	} else /if (${_type.Equal[hunter]}) {
		| hunter info
		NOUT Hunter Mode: ${sep} ${oenabled}
		NOUT Hunter List: ${cinfo}${HuntMobList}\ax
		NOUT Hunter Agro Range: ${If[${setHunterAgro},${cinfo}${setHunterAgro}\ax,${cinfo}Zone\ax]}
	} else /if (${_type.Equal[listexists]}) {
		| exists in zone.ini lists already
		NOUT ${onotice} ${sep} ${cinfo}${Target.DisplayName}\ax is already in list ${sep} [${ctar}${_verbage}\ax]
	} else /if (${_type.Equal[petfarm]}) {
		| petfarm
		VOUT Pet: ${cinfo}${Me.Pet.DisplayName}\ax ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax] ${sep} ${If[${Spawn[${Target2Kill}].LineOfSight},${cgood}${Spawn[${Target2Kill}].Distance}\ax,${cbad}${Spawn[${Target2Kill}].Distance}\ax]}
		| ${sep} ${sepO}${ctar}${_tmpVariable}\ax${sepC} ${If[${${_tmpVariable}} == 1,target,targets]}
	} else /if (${_type.Equal[pull]} && ${Target2Kill}) {
		| pull
		NOUT Pull ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax] ${sep} ${If[${Spawn[${Target}].LineOfSight},${cgood}${Int[${Target.Distance}]}\ax,${cbad}${Int[${Target.Distance}]}\ax]}
	} else /if (${_type.Equal[failboat]}) {
		NOUT My daughter says you're a nooblet.
	}
/return


 
|***
 *	DES: Check to see if toon is engaged in any way.
 *	USE: /call check_engaged
 *	INI: 
 *  NOTE: 
 ***|
sub check_engaged()
	DEBUG ${cbug}check_engaged()\ax
	ISDEAD
	/doevents
	/declare _isEngaged		bool local
	/varset _isEngaged FALSE
	/if (${Me.Invis}) /varset _isEngaged TRUE
	/if (${Bool[${Me.Casting}]} || ${Me.Casting.ID}) /varset _isEngaged TRUE
	/if (${Me.Moving} || ${Stick.Active}) /varset _isEngaged TRUE
	/if (${Me.Combat} || ${Me.AutoFire}) /varset _isEngaged TRUE
	/if (${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${MobAgro} playerstate 4]} && !${combatBuff}) /varset _isEngaged TRUE
	/if (((${setPullMode} && ${setAgroMode}) || ${GroupMainAssist} || ${GroupMainTank}) && ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${CombatDistance}*2 playerstate 4]}) /varset _isEngaged TRUE
	:exit
/return ${_isEngaged}



|***
 *	DES: Wait for and accept resurection or call to corpse
 *	USE: go die and see if this works
 *	INI: 
 *  NOTE: 
 *  NOTE: 96 rez: - "HiImaCleric wants to cast Blessing of Resurrection (96 percent) upon you. Do you wish this?"
 *	NOTE: call - "you will still be able to get a resurrection later"
 *	NOTE: rez increments: 0, 10, 20, 35, 50, 60, 75, 90, 96
 *	NOTE: lots of debug here. this is a problem child function.
 ***|
sub wait_for_rez()
	SDEBUG ${cbug}wait_for_rez((${Me.State.Equal[HOVER]} || ${Me.Hovering} || ${Me.Dead}) && ${rezAuto} || ${markerDead})\ax
	SDEBUG call Flag:${flagCallNeedRez}\ax
	SDEBUG events flushed\ax
	/doevents flush
	SDEBUG into rez, clear event marker\ax
	/if (!${Me.State.Equal[HOVER]} && !${Me.Hovering} && !${Me.Dead} && !${markerDead} && !${flagCallNeedRez}) /goto :exit
	SDEBUG update death count\ax
	/ini "${INICore}" "deathcount" "${Me.DisplayName}" "${Int[${Math.Calc[${Ini[${INICore},deathcount,${Me.DisplayName}]}+1]}]}"
	SDEBUG update MQ LOG\ax
	| MQLOG Death ${Ini[${INICore},deathcount,${Me.DisplayName}]}
	| /if (!${flagCallNeedRez} && ${Bool[${Ini[${INICore},bitch]}]}) ECHOCHANNEL ${Ini[${INICore},bitch,${Int[${Math.Calc[${Math.Rand[${Ini[${INICore},bitch,count]}]}+1]}]}]}

	SDEBUG stop movement\ax
	/ini "${INIFile}" "Movement" "SetTie" "FALSE"
	/varset SetTie FALSE
	/if (${Stick.Active}) /squelch /stick off
	/if (${tempsnapstuck}) /varset tempsnapstuck FALSE
	/if (${AdvPath.Following}) /squelch /afollow off
	| SDEBUG stop combat\ax
	/if (${Me.AutoFire}) /squelch /autofire
	/if (${Me.Combat}) /squelch /attack off
	| SDEBUG stop casting\ax
	/if (${Me.Casting.ID}) /stopcast
	SDEBUG clear targets\ax
	/varset Target2Kill 0
	/varset isValidTarget FALSE
	/if (${Target.ID}) /squelch /target clear
	
	SDEBUG update HUD tmpMode variable\ax
	/varset tmpMode "DEAD: ${Time}"
	/delay 3s
	:loopwaitforrezbox
	SDEBUG start wait for rez\ax
		/delay 10s ${Window[ConfirmationDialogBox].Open}
		/delay 1s
		/if (!${Window[ConfirmationDialogBox].Open}) /goto :loopwaitforrezbox
		SDEBUG we have a rez box\ax
		/if (${Window[ConfirmationDialogBox].Open}) {
			/if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(0 percent)"]}]}) {
				/nomodkey /notify ConfirmationDialogBox No_Button leftmouseup	
				/goto :loopwaitforrezbox
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(10 percent)"]}]}) {
				/nomodkey /notify ConfirmationDialogBox No_Button leftmouseup	
				/goto :loopwaitforrezbox
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(20 percent)"]}]}) {
				/nomodkey /notify ConfirmationDialogBox No_Button leftmouseup	
				/goto :loopwaitforrezbox
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(35 percent)"]}]}) {
				/nomodkey /notify ConfirmationDialogBox No_Button leftmouseup	
				/goto :loopwaitforrezbox
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(50 percent)"]}]}) {
				/nomodkey /notify ConfirmationDialogBox No_Button leftmouseup	
				/goto :loopwaitforrezbox
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(60 percent)"]}]}) {
				/nomodkey /notify ConfirmationDialogBox No_Button leftmouseup	
				/goto :loopwaitforrezbox
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(75 percent)"]}]}) {
				/nomodkey /notify ConfirmationDialogBox No_Button leftmouseup	
				/goto :loopwaitforrezbox
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(90 percent)"]}]}) {
				/nomodkey /notify ConfirmationDialogBox No_Button leftmouseup	
				/goto :loopwaitforrezbox
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(96 percent)"]}]}) {
				SDEBUG found 96 rez\ax
				/varset flagCallNeedRez FALSE
				/goto :acceptresbox
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(100 percent)"]}]}) {
				SDEBUG found 100 rez\ax
				/varset flagCallNeedRez FALSE
				/goto :acceptresbox
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["you will still be able to get a resurrection later"]}]}) {
				SDEBUG found call to corpse. setting flagCallNeedRez\ax
				/varset flagCallNeedRez TRUE
				/goto :acceptresbox
			} 
			SDEBUG :loopwaitforrez\ax
			/goto :loopwaitforrezbox
		}

	:acceptresbox	
	SDEBUG yes we want a rez\ax	
	/nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
	/delay 5
	SDEBUG sorting rez selection window\ax	
	/if (${Window[RespawnWnd].Open}) {
		/if (${Zone.ShortName.Equal[Powar]}) {
			/nomodkey /notify RespawnWnd RW_OptionsList listselect 1
		} else {
			SDEBUG option 2 in respawn window\ax
			/nomodkey /notify RespawnWnd RW_OptionsList listselect 2 
		}
	}
	/delay 1s
	SDEBUG click RW_SelectButton
	/nomodkey /notify RespawnWnd RW_SelectButton leftmouseup
	:cleardeadmarkers
		SDEBUG delays for rez status to clear\ax
		| /call set_stop
		/delay 2s !${Me.State.Equal[HOVER]}
		/delay 2s !${Me.Hovering}
		/delay 2s !${Me.Dead}
		SDEBUG clear HUD tmpMode variable\ax
		/varset tmpMode FALSE
		SDEBUG Exit rez, clear event marker\ax
		/varset markerDead FALSE
		/if (${markerDead} || ${tmpMode}) {
			| /call create_timer clear_dead_event_marker 1s
			/delay 5
			/goto :cleardeadmarkers
		}

  :exit
	SDEBUG ${cgood}EXIT\ax ${cbug}wait_for_rez((${Me.State.Equal[HOVER]} || ${Me.Hovering} || ${Me.Dead}) && ${rezAuto} || ${markerDead})\ax
	SDEBUG call flagCallNeedRez: ${flagCallNeedRez}\ax
/return 



|***
 *	DES: give weapons to pet
 *	USE: /call give_to_pet
 *	INI: [Pet] - 
 *  NOTE: 
 ***|
sub give_to_pet()
	DEBUG ${cbug}give_to_pet()\ax Giving ${Cursor.ID} to ${Me.Pet.ID}
	ISDEAD
	/doevents
	/if (${Target.ID} != ${Me.Pet.ID}) {
		/target id ${Me.Pet.ID}
		/delay 1s ${Target.ID} == ${Me.Pet.ID}
	}
	:loopgiveitem
	ISDEAD
	/delay 5 ${Cursor.ID}
	/if (${Cursor.ID} && ${Me.Pet.ID}) {
		/if (${Cursor.Name.Find[summoned:]}) {
			/click left target
			/delay 5s !${Bool[${Cursor.ID}]}
		} else {
			CLEARCURSOR TRUE
		}
		/goto :loopgiveitem
	}
	:exit
/return



|***
 *	DES: Summon pet. I mean, it won't summon cheeze
 *	USE: /call cast_summon_pet
 *	INI: Pet - PetTypeSpell
 *  NOTE: 
 ***|
sub cast_summon_pet()
	DEBUG ${cbug}cast_summon_pet()\ax
	ISDEAD
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/doevents
	/if (${AutoBot} && ${usePet} && ${Me.AltAbilityReady[Suspended Minion]}) {
		/call core_cast "Suspended Minion" alt 0
		/delay 5s ${Me.Pet.ID}
	}
	/if (${usePet} && !${Me.Pet.ID})  {
		VOUT Making a new pet: ${csp}${PetTypeSpell}\ax
		/call core_cast "${PetTypeSpell}" ${buffGem} 0 
		/call create_timer timer_check_pet_buffs 20
		/delay 20s !${Me.Casting.ID}
		/pet hold on
		/pet ghold on
		/if (${usePet} && ${Bool[${PetTank}]} && ${AutoBot}) { 
			/pet taunt on
		} else /if (${usePet} && !${Bool[${PetTank}]} && ${AutoBot}) { 
			/pet taunt off
		}
		/call send_pet
	}
	/delay 2s
	:exit
/return



|***
 *	DES: Shrinks pet.. wtf did you expect..
 *	USE: /call cast_shrink_pet
 *	INI: Pet - PetShrink
 *  NOTE: 
 ***|
sub cast_shrink_pet()
	DEBUG ${cbug}cast_shrink_pet()\ax
	ISDEAD
	/doevents
	CHECKEXIT
	/call check_engaged
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/if (${Me.Pet.ID} && ${Bool[${shrinkPet}]} && ${Me.Pet.Height} > 1.40) {
		/if (${Me.Book[${shrinkPet}]}) /call core_cast "${shrinkPet}" ${buffGem} ${Me.Pet.ID}
		/if (${Me.ItemReady[${shrinkPet}]}) /call core_cast "${shrinkPet}" item ${Me.Pet.ID}
		/if (${Me.AltAbilityReady[${shrinkPet}]}) /call core_cast "${shrinkPet}" alt ${Me.Pet.ID}
	}
	:exit
/return



|***
 *	DES: fixes pet for.. umm.. pet stuff.. yah that..
 *	USE: /call check_pet
 *	INI: 
 *  NOTE: chanter pets sword ID=10855
 ***|
sub check_pet()
	DEBUG ${cbug}check_pet()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/if (!${Me.Pet.ID}) /call cast_summon_pet
	/if (${Me.Pet.ID} && ${useSummonWeps} && (!${Bool[${Me.Pet.Equipment[primary]}]} || !${Bool[${Me.Pet.Equipment[offhand]}]} || ${Me.Pet.Equipment[primary]} == 10855)) {
		/if (!${Bool[${timer_ask_for_weapons}]} && !${Select[${Me.Class.ShortName},MAG]} && !${Bool[${FindItem[${summonedWepClosedBag}]}]}) {
			/call tell_resident_mage weapons
			/goto :exit
		} else /if (${Select[${Me.Class.ShortName},MAG]} && ${Me.Pet.ID} && ${useSummonWeps} && !${Bool[${FindItem[${summonedWepClosedBag}]}]}) {
			/call cast_summon_weapons
			/goto :exit
		}

	}	
	CHECKEXIT
	CHECKTIE
	/call check_engaged
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/if (${Me.Pet.ID} && ${useSummonWeps} && (!${Bool[${Me.Pet.Equipment[primary]}]} || ${Me.Pet.Equipment[primary]} == 10855) && (${Bool[${FindItem[${summonedWepClosedBag}]}]} || ${Bool[${FindItem[Pouch of Quellious]}]})) {
		CLEARCURSOR TRUE
		/if (${Bool[${FindItem[${summonedWepClosedBag}]}]} || ${Bool[${FindItem[Pouch of Quellious]}]}) /call cast_summon_weapons
	}
		
	:exit
	/if (${BuffPet} && ${Me.Pet.ID} && !${timer_check_pet_buffs}) /call check_pet_buffs
/return



|***
 *	DES: Summon and equip pet weapons
 *	USE: 
 *	INI: INI/Pet/useSummonWeps [TRUE|FALSE]
 *  NOTE: MAG,ENG,BST,NEC
 ***|
sub cast_summon_weapons()
	DEBUG ${cbug}cast_summon_weapons()\ax
	ISDEAD
	/doevents
	/declare _count int local
	
	/if (!${Bool[${Me.Pet.ID}]}) /goto :exit
	/if (${Select[${Me.Class.ShortName},MAG]} && !${FindItem[${summonedWepClosedBag}].ID} && !${FindItem[${summonedWepName}].ID}) {
		DEBUG mage needs bag
		/call core_cast "${summonedWepSpell}" ${buffGem} ${Me.ID}
		/delay 20s !${Me.Casting.ID}
		CLEARCURSOR TRUE
	}
	/delay 5
	
	/if (${FindItem[${summonedWepClosedBag}].ID}) {
		DEBUG have a bag. best not in hands
		CLEARCURSOR TRUE
		| /delay 2s !${Cursor.ID}
		/squelch /exchange ${FindItem[${summonedWepClosedBag}].ID} pack${PetBagSlotNumber}
		CLEARCURSOR TRUE
		| /delay 2s !${Cursor.ID}
		/nomodkey /itemnotify ${FindItem[${summonedWepClosedBag}].InvSlot} rightmouseup
		DEBUG open the dumb bag
		/delay 3s !${Me.Casting.ID}
		/delay 2s
		CLEARCURSOR TRUE
	}

	/if (${Bool[${FindItem[Pouch of Quellious].InvSlot}]}) {
		DEBUG give the demannding pet his shit
		/nomodkey /itemnotify ${FindItem[Pouch of Quellious].InvSlot} rightmouseup
		/delay 1s
		/for _count 1 to 2
			/if (${FindItem[${summonedWepName}].ID}) {
				/nomodkey /itemnotify ${FindItem[${summonedWepName}].InvSlot} leftmouseup
				/delay 3s ${Bool[${Cursor.ID}]}
				/call give_to_pet
				/delay 3s !${Bool[${Cursor.ID}]}
				/delay 1s
			}
		/next _count
		/if (${Window[GiveWnd].Open}) {
			/nomodkey /notify GiveWnd GVW_Give_Button leftmouseup
			/keypress ESC
			/keypress ESC
		}
		/if (${FindItem[Pouch of Quellious].InvSlot}) {
			:loopdestroy
			/delay 1s
			/nomodkey /itemnotify ${FindItem[=Pouch of Quellious].InvSlot} leftmouseup
			/delay 1s ${Cursor.Name.Equal["Pouch of Quellious"]}
			/delay 1s
			/if (${Cursor.Name.Equal["Pouch of Quellious"]}) /destroy
			/if (${FindItem[Pouch of Quellious].InvSlot}) /goto :loopdestroy
		}
	}
	:loopclosewindow
	/if (${Window[InventoryWindow].Open}) {
		/nomodkey /keypress inventory
		/delay 1s !${Window[InventoryWindow].Open}
		/if (${Window[InventoryWindow].Open}) /goto :loopclosewindow
	}
	:exit
/return



|***
 *	DES: keep exp at a specified level then max aa
 *	USE: 
 *	INI: INI/common/MaintExpLvl
 *  NOTE: 
 ***|
sub auto_adjust_EXP()
	DEBUG ${cbug}auto_adjust_EXP()\ax
	ISDEAD
	CHECKEXIT
	/doevents
	
	| set EXP cause we deaded too much
	/if (${Me.Level} < ${MaxLevel} || (${Me.Level} == ${MaxLevel} && ${Me.PctExp} <= ${MaintExpLvl})) {
		VOUT ${ocaution} ${sep} ${sepO}${cinfo}${Me.PctExp}%\ax${sepC} below Maintain EXP ${sepO}${cinfo}${MaintExpLvl}%\ax${sepC}
		/alternateadv off
	| set to AA cause the cleric rules	
	} else /if (${Me.Level} == ${MaxLevel} && ${Me.PctExp} >= ${MaintExpLvl} && (${Me.AAPoints} < ${Float[${Math.Calc[${Me.Level}*2]}].Int})) {
		VOUT Maintain EXP ${sepO}${cinfo}${MaintExpLvl}%\ax${sepC}. AA: ${sepO}${cinfo}${Me.AAPoints} < ${Float[${Math.Calc[${Me.Level}*2]}].Int}\ax${sepC}. 
		VOUT Adjusting EXP to full AA.
		/alternateadv on 100
	| set back to exp cause nothing else to buy
	} else /if (${Me.Level} == ${MaxLevel} && ${Me.PctExp} >= ${MaintExpLvl} && (${Me.AAPoints} == ${Float[${Math.Calc[${Me.Level}*2]}].Int})) {
		VOUT MAX EXP/Level/AA. Setting EXP to normal.
		/alternateadv off
	}
	:exit
	/call create_timer timer_check_EXP ${tCheck_EXP}
/return




|***
 *	DES: Check for running auras, If they are duped, remove the second one
 *	USE: /call check_aura
 *	INI: 
 *  NOTE: 
 ***|
sub check_aura()
	DEBUG ${cbug}check_aura()\ax 
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	| /if (${SpawnCount[radius ${combatRange} playerstate 4 targetable]}) /goto :exit
	UPDATEHUD "CHECK: Aura"
	
	| Aura: berzerker, monk
	/if (${Bool[${Aura1Buff}]} && ${Select[${Me.Class.ShortName},BER,MNK]} && !${Bool[${Me.Aura[1].Equal[${Aura1Buff}]}]}) {
		VOUT Disc: ${csp}${Aura1Spell}\ax
		/disc ${Aura1Spell}
		| /call core_cast "${Aura1Spell}" ${buffGem} ${Me.ID}
		/delay 8s
		/goto :exit
	}
	
	| Aura: Bard
	/if (${Bool[${Aura1Buff}]} && ${Select[${Me.Class.ShortName},BRD]} && !${Bool[${Me.Aura[1].Equal[${Aura1Buff}]}]}) {
		/call fix_gem_id "${buffGem}"
		/declare _gem			int local ${Macro.Return}
		/call scribe "${Aura1Buff}" "${_gem}" 10
		VOUT Pulse: ${csp}${Me.Gem[${_gem}].Name}\ax
		/melody ${_gem}
		/delay ${Spell[${Me.Gem[${_gem}].Name}].CastTime.Seconds}s !${Me.Casting.ID}
		/delay 2 

		:stoppulse
		/if ((${Bool[${Me.Aura[1].Equal[${Aura1Buff}]}]} || ${Me.Song[${Me.Gem[${_gem}].ID}]}) && (${Me.Casting.ID} == ${Me.Gem[${_gem}].ID})) {
			/delay 2
			/melody ${_gem}	
			/goto :stoppulse
		}	
		/goto :exit
	}

	| Aura: all others
	/if (${Bool[${Aura1Buff}]}) {
		/if (!${Bool[${Me.Aura[1].Equal[${Aura1Buff}]}]} && !${Bool[${Me.Aura[2].Equal[${Aura1Buff}]}]}) /call core_cast "${Aura1Spell}" ${buffGem} ${Me.ID}
	}
	/if (${Bool[${Aura2Buff}]}) {
		/if (!${Bool[${Me.Aura[1].Equal[${Aura2Buff}]}]} && !${Bool[${Me.Aura[2].Equal[${Aura2Buff}]}]}) /call core_cast "${Aura2Spell}" ${buffGem} ${Me.ID}
	}	
	
	:exit
	/call create_timer timer_check_aura ${tCheck_Aura}
	UPDATEHUD
/return



sub intialize_item_buffs()
	DEBUG ${cbug}intialize_item_buffs()\ax
	ISDEAD
	/doevents
	VOUT Initializing Item Click Buffs
	/declare ClickItem											int			outer 1
	/declare InvItem              					int			local 0
	/declare ContainerItem									int			local 0
	/declare UseItem[40]                  	string	outer 
	/declare SelfItem[${UseItem.Size}]   		string 	outer 
	/declare SpellItem[${UseItem.Size}]   	string	outer 
	/declare tmpClickName										string	outer ${Ini[${INIFile},ClickItems]}
	/declare tmpFoundSpell									bool		local FALSE

	/for InvItem 1 to 32
		ISDEAD
		/if (${Bool[${InvSlot[${InvItem}].Item.Container}]}) {
			/for ContainerItem 1 to ${InvSlot[${InvItem}].Item.Container}
				ISDEAD
				/if (${Me.Inventory[${InvItem}].Item[${ContainerItem}].ID} && ${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell.ID} && ${Me.Inventory[${InvItem}].Item[${ContainerItem}].EffectType.Equal[Click Inventory]} && ${Me.Level}>=${Me.Inventory[${InvItem}].Item[${ContainerItem}].RequiredLevel}) {
					DEBUG Finding ${Me.Inventory[${InvItem}].Item[${ContainerItem}].Name}: -- [${If[${tmpClickName.Find[${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell}]},TRUE,FALSE]}]
					/if (${Bool[${tmpClickName.Find[${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell}]}]} && ${Bool[${Ini[${INIFile},ClickItems,${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell},TRUE]}]}) { 
					  /varset UseItem[${ClickItem}] TRUE
					  /varset SelfItem[${ClickItem}] ${Me.Inventory[${InvItem}].Item[${ContainerItem}].Name}
					  /varset SpellItem[${ClickItem}] ${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell}		  
					  /varcalc ClickItem ${ClickItem}+1
					  DEBUG Setting "${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell}" clickitem:${ClickItem}
					} else /if (!${tmpClickName.Find[${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell}]}) {
					 	/ini "${INIFile}" "ClickItems" "${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell}" "FALSE"
						DEBUG Creating "ClickItems" "${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell}"
					}
				}
			/next ContainerItem
		} else /if (!${Bool[${InvSlot[${InvItem}].Item.Container}]} && ${InvSlot[${InvItem}].Item.ID} && ${InvSlot[${InvItem}].Item.Spell.ID} && ${InvSlot[${InvItem}].Item.EffectType.Equal[Click Inventory]} && ${Me.Level}>=${InvSlot[${InvItem}].Item.RequiredLevel}) {
			DEBUG Finding ${Me.Inventory[${InvItem}].Name}: ${tmpClickName.Find[${Me.Inventory[${InvItem}].Spell.Name}]} -- [${If[${tmpClickName.Find[${Me.Inventory[${InvItem}].Spell}]},TRUE,FALSE]}]
			ISDEAD
			/if (${Bool[${tmpClickName.Find[${Me.Inventory[${InvItem}].Spell}]}]} && ${Bool[${Ini[${INIFile},ClickItems,${Me.Inventory[${InvItem}].Spell},TRUE]}]}) { 
				/varset UseItem[${ClickItem}] TRUE		
				/varset SelfItem[${ClickItem}] ${InvSlot[${InvItem}].Item}
				/varset SpellItem[${ClickItem}] ${InvSlot[${InvItem}].Item.Spell}
				/varcalc ClickItem ${ClickItem}+1
			  DEBUG Setting "${Me.Inventory[${InvItem}].Spell}" clickitem:${ClickItem}
			} else /if (!${tmpClickName.Find[${Me.Inventory[${InvItem}].Spell}]}) {
				/ini "${INIFile}" "ClickItems" "${InvSlot[${InvItem}].Item.Spell}" "FALSE"
				DEBUG Creating "ClickItems" "${Me.Inventory[${InvItem}].Spell}"
			}
		}
	/next InvItem
	:exit
/return











|***
 *	DES: Checks current buffs. if one is missing. fix it.
 *	USE: /ib and auto
 *	INI: 
 *  NOTE: 
 ***|
sub check_buffs_item()
	DEBUG ${cbug}check_buffs_items()\ax
	ISDEAD
	/doevents
	CHECKEXIT
	UPDATEHUD "BUFF: Items"
	/declare _count int	local
	
	/if (${Me.State.NotEqual[Bind]} && !${Me.Moving}) {
		/for _count 1 to ${ClickItem}
			/doevents
			ISDEAD
			/if (${UseItem[${_count}]}&& ${Me.FreeBuffSlots} >= 1 && ${FindItem[${SelfItem[${_count}]}].ID} && ${Cast.Ready[${SelfItem[${_count}]}]} && !${Me.Buff[${SpellItem[${_count}]}].ID} && ${Spell[${SpellItem[${_count}]}].Stacks}) { 
				/call core_cast "${SelfItem[${_count}]}" item 0
				/delay 5s !${Me.ItemReady[${SelfItem[${_count}]}]}
				/delay 10s !${Me.Casting.ID}
	  	}
  	/next _count
  }

	| CoTF progression
  /if ((${Me.ItemReady["Gunthak Swabby's Eyepatch"]} && !${Bool[${Me.Buff[Gunthak]}]} && ${Spell[Gunthak Grit].Stacks}) && !${Bool[${Me.Buff[Gunthak Fortitude]}]} && ${Spell[Gunthak Fortitude].Stacks}) /call core_cast "Gunthak Swabby's Eyepatch" item
 	| PoWar Anniversary Mission
  /if (${Me.ItemReady["Bloodmetal Inlaid Gnollish Totem"]} && ${Spell[Direwind Gnoll Reaver Fortitude].Stacks} && !${Bool[${Me.Buff[Gnoll Reaver]}]}) /call core_cast "Bloodmetal Inlaid Gnollish Totem" item 0

 	CHECKEXIT
  /if (${Me.ItemReady[${IllusionItem}]} && (!${Bool[${Me.Buff[${IllusionBuff}]}]}) && (${Spell[${IllusionBuff}].Stacks})) {
		VOUT Item: ${citem}${IllusionItem}\ax
 		/call MQ2Cast "${IllusionItem}" item 30s
| 		/nomodkey /useitem ${IllusionItem}
 	}  	

	/if (!${Bool[${timer_Check_Familiar}]}) /call sort_familiar
 	
	/if (${Bool[${SelfBuffRemove}]}) /call remove_unwanted_buffs	
	/if (${AutoBot} && ${setAutoShrink}) /call check_shrink
	/if (${AutoBot}) /call create_timer timer_check_buffs_item ${tItem_Buff}
	:exit
	UPDATEHUD
/return



|***
 *	DES: Checks current buffs. NOT SONGS. IF one is found to match the INI listing per character, it is removed.
 *	USE: /call remove_unwanted_buffs
 *	INI: toon -> [SelfBuff] -> SelfBuffRemove
 *  NOTE: 
***|
sub remove_unwanted_buffs()
	DEBUG ${cbug}remove_unwanted_buffs()${cbug}
	ISDEAD
	/doevents
	/declare _count int local
	/declare _remove string local ${SelfBuffRemove}
	/for _count 1 to ${_remove.Count[|]}
		/doevents
		ISDEAD
		/if (${Bool[${Me.Buff[${_remove.Arg[${_count},|]}]}]})  {
			VOUT Buff ${cbad}${_remove.Arg[${_count},|]}\ax found. Removing it.
			/squelch /removebuff ${_remove.Arg[${_count},|]}
		}
	/next _count		
	:exit
/return



|***
 *	DES: Chekcs self buffs. If somethings missing, mem and cast it.
 *	USE: /sb and/or auto
 *	INI: 
 *  NOTE: 
 ***|
sub check_buffs_self()
	DEBUG ${cbug}check_buffs_self()\ax
	UPDATEHUD "BUFF: Self"
	/if (${AutoBot} && !${BuffCombat} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]}) /goto :exit

	/declare _count int local
	/for _count 1 to ${SelfBuffCount}
		UPDATEHUD "BUFF: Self"
		/doevents
		ISDEAD
		CHECKTIE
		| /call check1offBuffs
		CHECKEXIT
		/if (!${Bool[${Me.Buff[${SelfBuff${_count}chk}].Duration}]} && ${Spell[${SelfBuff${_count}}].Stacks}) {
			/if (!${Me.SpellReady[${SelfBuff${_count}}]} && ${Cursor.ID}) CLEARCURSOR TRUE
 		  :loopretrycast
	 		  /if (${Me.Book[${SelfBuff${_count}}]} && ${Me.CurrentMana} > ${Spell[${SelfBuff${_count}}].Mana}) /call core_cast "${SelfBuff${_count}}" ${buffGem} ${Me.ID}
				/if (${Me.AltAbility[${SelfBuff${_count}}].ID}) /call core_cast "${SelfBuff${_count}}" alt 0
				/if (${Me.ItemReady[${SelfBuff${_count}}]}) /call core_cast "${SelfBuff${_count}}" item ${Me.ID}
				/if (${Select[${castReturn},CAST_FIZZLE]}) {
					/delay 1s
					/goto :loopretrycast
				}
			}
	/next _count
	/if (!${Bool[${timer_Check_Familiar}]}) /call sort_familiar
	/if (${AutoBot} && ${setAutoShrink}) /call check_shrink
	/if (${Bool[${SelfBuffRemove}]}) /call remove_unwanted_buffs	
	/if ((${Bool[${Aura1Spell}]} || ${Bool[${Aura2Spell}]}) && !${timer_check_aura}) /call check_aura
	/if (${AutoBot}) /call create_timer timer_check_buffs_self ${tSelf_Buff}
	/if (${Target.ID}) /squelch /target clear
	:exit
	UPDATEHUD	
/return



|***
 *	DES: Checks pets buffs. If somethings missing, mem and cast it.
 *	USE: /pb and/or auto
 *	INI: 
 *  NOTE: this will do AA or Spell
 ***|
sub check_pet_buffs()
	DEBUG ${cbug}check_pet_buffs()\ax
	/if (!${Me.Pet.ID}) /goto :exit
	/if (${AutoBot} && (!${BuffCombat} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]})) /goto :exit
	UPDATEHUD "BUFF: Pet"

	/declare _count int local
	/declare _spellType string local
	
	/for _count 1 to ${PetBuffCount}
		ISDEAD
		/doevents
		CHECKEXIT
		CHECKTIE
		/call check_engaged
		/if (${Bool[${Macro.Return}]}) /goto :exit	
		/varset _spellType FALSE

		/if (!${Me.Pet.Buff[${PetBuff${_count}chk}]} && ${Me.CurrentMana} > ${Spell[${PetBuff${_count}}].Mana}) /varset _spellType ${buffGem}
		/if (!${Me.Pet.Buff[${PetBuff${_count}chk}]} && ${Me.AltAbilityReady[${PetBuff${_count}}]}) /varset _spellType alt
			
		/if (${Bool[${_spellType}]} && !${Me.Moving} ) {
			CLEARCURSOR TRUE
 		  :loopretrycast
			/call core_cast "${PetBuff${_count}}" ${_spellType} ${Me.Pet.ID}
			/if (${Select[${castReturn},CAST_FIZZLE]}) {
				/delay 1s
				/goto :loopretrycast
			}
		}
		:nextbuff
	/next _count
	
	| chekck for pet illusion
	/if (${Bool[${PetIllusion}]} && !${Bool[${Me.Pet.Buff[${PetIllusionBuff}]}]} && ${Me.Pet.Distance} < 100 && ${Spell[${PetIllusionBuff}].Stacks}) /call MQ2Cast "${PetIllusion}" item -targetid|${Me.Pet.ID}
	| shrink the little bastards..
	/if (${Bool[${Me.Pet.ID}]}) /call cast_shrink_pet
	| pet epic click
	/if (${Me.ItemReady[${EpicClick}]} && !${Me.Pet.Buff[Elemental Conjunction]} && !${Me.Pet.Buff[${EpicPetBuff}]} && !${Me.Pet.Buff[Blessing of Unity]} && ${Spell[${EpicPetBuff}].Stacks}) /call core_cast "${EpicClick}" item 0
	
	/if (${Target.ID}) /squelch /target clear
	:exit
	UPDATEHUD	
	/if (${AutoBot} || ${setPetFarmMode} || ${setHunterMode}) /call create_timer timer_check_pet_buffs ${tPet_Buff}
	| SDEBUG pet-timer: ${timer_check_pet_buffs}
/return



|***
 *	DES: checks your toon for buffs that prevent you from casting.
 *	USE: /call check_no_cast_buffs
 *	INI: 
 *  NOTE: this is by toon.
 ***|
sub check_no_cast_buffs()
	DEBUG ${cbug}check_no_cast_buffs()\ax
	ISDEAD
	/doevents
	/declare _count int local
	/varset canCast TRUE
	/for _count 1 to ${noCastWith.Count[|]}
		ISDEAD
		/doevents
		/if (${Bool[${Me.Buff[${noCastWith.Arg[${_count},|]}]}]} || ${Bool[${Target.Buff[${noCastWith.Arg[${_count},|]}]}]} || ${Bool[${Me.Song[${noCastWith.Arg[${_count},|]}]}]}) /varset canCast FALSE
	/next _count	
	:exit
/return ${canCast}



|***
 *	DES: checks and clicks minature horn of unity
 *	USE: /horn or /bc horn
 *	INI: Toon -> [Common] -> HornofUnity
 *  NOTE: 
 ***|
sub click_horn_of_unity()
	DEBUG ${cbug}click_horn_of_unity()\ax
	/if (!${Bool[${HornofUnity}]} || ${SafeZone} || ${Bool[${timer_zone_pause}]}) /goto :exit
	/if (${Me.ItemReady[Miniature Horn of Unity]} && !${Bool[${Me.Song[Blessing of Unity]}]} && (${Me.PctHPs} < 95*.${tmpHealOverride} || ${Me.PctMana} < 95*.${tmpHealOverride})) {
		/call core_cast "Miniature Horn of Unity" item 0 
	}
	:exit
/return



|***
 *	DES: EQBC Buff Routines
 *	USE: automatic for anyone sharing your EQBC
 *	INI: 
 *  NOTE: 
  ***|
sub check_buffs_BC()
	DEBUG ${cbug}check_buffs_BC()\ax
	/if (!${BuffBC}) /goto :exit
	ISDEAD
	/doevents
	CHECKEXIT
	UPDATEHUD "BUFF: BC"
	/if (${AutoBot} && (!${BuffCombat} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]})) /goto :exit

	/declare _x int local 
	/declare _b int local 
	/for _x 1 to ${NetBots.Counts}
		ISDEAD
		/doevents
		/if (!${NetBots[${NetBots.Client[${_x}]}].InZone} || ${NetBots[${NetBots.Client[${_x}]}].Dead}) /goto :skipbcspawn
		DEBUG [${_x}]:${NetBots.Client[${_x}]}
		/for _b 1 to ${BuffCount} 	
			/if (${Spawn[pc ${NetBots.Client[${_x}]}].Distance} > ${Spell[${Buff${_b}}].MyRange}) /goto :skipbcspell
			| /call check1offBuffs
			/doevents
			ISDEAD
			/if (!${Bool[${Buff${_b}}]} || !${Bool[${Buff${_b}chk}]} || !${Bool[${BCBuff${_b}Class}]} || (${Raid.Members} && ${Buff${_b}noBCinRaid}) || (!${Me.SpellReady[${Buff${_b}}]} && ${Me.Gem[${Buff${_b}}]}) || !${NetBots[${NetBots.Client[${_x}]}].Stacks[${Spell[${Buff${_b}chk}].ID}]}) /goto :skipBCspell
			DEBUG spell=${_b}/${BuffCount}: ${Buff${_b}chk} Stacks:${NetBots[${NetBots.Client[${_x}]}].Stacks[${Spell[${Buff${_b}chk}].ID}]}
			/if (${Me.CurrentMana} > ${Spell[${Buff${_b}}].Mana} && ${NetBots[${NetBots.Client[${_x}]}].Stacks[${Spell[${Buff${_b}chk}].ID}]} && ${BCBuff${_b}Class.Find[${Spawn[${NetBots.Client[${_x}]}].Class.ShortName}]}) { 
  		  VOUT BC Buff: ${csp}${Buff${_b}}\ax ${sep} [${cpc}${NetBots.Client[${_x}]}\ax]
				:retrycastcheckbcbuffs
				/call MQ2Cast "${Buff${_b}}" ${buffGem} 10s -targetid|${NetBots[${NetBots.Client[${_x}]}].ID} 	
				/if (${Macro.Return.Equal[CAST_FIZZLE]}) /goto :retrycastcheckbcbuffs
				/goto :exit
			}
		:skipbcspell
		/next _b
	:skipbcspawn
	/next _x
	:exit  
	/if (${AutoBot}) /call create_timer timer_check_buffs_BC ${tBC_Buff}
	UPDATEHUD
/return



|***
 *	DES: make sure you have ammo
 *	USE: /call check_ammo
 *	INI: [common] -> RangeItem & RangeItemSummon
 *  NOTE:
 ***|
sub check_ammo()
	DEBUG ${cbug}check_ammo()\ax
	ISDEAD
	CHECKEXIT
	/doevents
	/if (${Cursor.Name.Equal[${RangeItemSummon}]}) {
		CLEARCURSOR TRUE
		/goto :exit
	}	
	/if (!${Bool[${RangeItem}]} || !${Bool[${RangeItemSummon}]} || ${Me.Invis}) /goto :exit
	/if (!${Bool[${FindItemCount[${RangeItemSummon}]}]}) {
		/if (${Me.Book[${RangeItem}]}) /call core_cast "${RangeItem}" ${buffGem} ${Me.ID}
		/if (${Me.ItemReady[${RangeItem}]}) /call core_cast "${RangeItem}" item ${Me.ID}
		/if (${Me.AltAbilityReady[${RangeItem}]}) /call core_cast "${RangeItem}" alt ${Me.ID}
		/if (${Me.CombatAbilityReady[${RangeItem}]} || ${Select[${Me.Class.ShortName},BER]}) /call core_cast "${RangeItem}" disc ${Me.ID}
		/delay 10s !${Me.Casting.ID}
		CLEARCURSOR TRUE
	}
	:exit
	/call create_timer timer_Check_Ammo ${tCheck_Ammo}
/return



|***
 *	DES: Baby got back.
 *	USE: /call check_Shrink
 *	INI: [common] - shrinkItem
 *  NOTE: no, i don't know why the mage will not shrink.
 ***|
sub check_shrink()
	DEBUG ${cbug}check_shrink()\ax
	ISDEAD
	/doevents
	CHECKEXIT
	/if (${Me.Height} > 2.04 && ${Me.ItemReady[${shrinkItem}]}) {
		/if (${Me.Book[${shrinkItem}]}) /call core_cast "${shrinkItem}" ${buffGem} ${Me.ID}
		/if (${Me.ItemReady[${shrinkItem}]}) /call core_cast "${shrinkItem}" item ${Me.ID}
	}
	:exit
/return



|***
 *	DES: create misc timer
 *	USE: /call create_timer [TIMER VARIABLE NAME] [DURATION]
 *	INI: [Timer]
 *  NOTE: this will define and populate if the variable does not exist. if it exists, it will adjust the time acordingly
 ***|
sub create_timer(string _my_timer, string _my_duration)
	DEBUG ${cbug}create_timer(\ax${cinfo}${_my_timer}, ${_my_duration}\ax${cbug})\ax  
	ISDEAD
	/if (${verbose} && !${Bool[${_my_timer}]}) NOUT ${ocaution} ${sep} BAD TIMER CREATION. ${cinfo}[\ax${cbad}no name\ax${cinfo}]\ax
	/if (${verbose} && !${Bool[${_my_duration}]}) NOUT ${ocaution} ${sep} BAD TIMER CREATION. ${cinfo}[\ax${cbad}no duration\ax${cinfo}]\ax
	/if (!${Defined[${_my_timer}]}) /declare ${_my_timer} timer outer
	/varset ${_my_timer} ${_my_duration}
/return



|***
 *	DES: rest/not rest routines.
 *	USE: /call do_rest
 *	INI: na
 *  NOTE: 
 ***|
sub do_rest()
	DEBUG ${cbug}do_Rest(\ax${cinfo}manna:${Me.PctMana}% endurance:${Me.PctEndurance}%\ax${cbug})\ax
	ISDEAD

	/if (!${AutoBot}) /goto :exit
	/declare _needrest bool local FALSE
	
	/if (${HomeSet} && !${SpawnCount[npc radius ${CombatDistance}*2 playerstate 4 targetable]} && ((${Me.Y} != ${MakeCamp.AnchorY}) && (${Me.X} != ${MakeCamp.AnchorX}))) {
		/if (${setPullMode}) /makecamp upause
		/makecamp return
		/delay 3s ((${Me.Y} == ${MakeCamp.AnchorY}) && (${Me.X} == ${MakeCamp.AnchorX}))
	}
	
	:loopneedmorerest
	CHECKREZ
	/if (${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${MobAgro} playerstate 4]} || !${AutoBot}) {
		/if (${Me.State.NotEqual[STAND]}) /stand
		/goto :exit
	}

	/doevents
	ISDEAD
	CLEARCURSOR
	CHECKTIE
	/doevents
	| check self click buffs
	/if (!${Me.Invis}) {
		/call check_engaged
		/if (!${Bool[${Macro.Return}]} && ${BuffItem} && !${timer_check_buffs_item}) /call check_buffs_item
		/if (${BuffTell} && ${Bool[${BuffCount}]} && !${timer_check_buffs_tell}) /call check_buffs_tell
	}

	/if (${AutoBot} && (${Bool[${Group.Members}]} && ${Group.MasterLooter.ID}==${Me.ID}) && ${Me.UseAdvancedLooting} && !${AdvLoot.LootInProgress} && !${Stick.Active}) /call do_loot
	
	/varset _needrest FALSE

	| /if (${Bool[${autoRest}]} && !${Me.Casting.ID} && !${Me.Moving} && !${Select[${Me.CombatState},COMBAT,ACTIVE]} && !${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${MobAgro} playerstate 4]}) {
	/if (${Bool[${autoRest}]} && !${Me.Casting.ID} && !${Me.Moving} && !${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${MobAgro} playerstate 4]}) {
			
		| Hybrid: RNG,BRD,SHD,BST,PAL
		/if (${MeleeList.Find[${Me.Class.ShortName}]} && ${CastList.Find[${Me.Class.ShortName}]}) {
			| DEBUG REST RNG,BRD,SHD,BST,PAL
			/if (${Me.PctEndurance} < ${autoRest}*.${tmpHealOverride}) /varset _needrest TRUE
			/if (${Me.PctMana} < ${autoRest}*.${tmpHealOverride}) /varset _needrest TRUE
			/call use_mod_rod
			/call cast_disc_breather
			/if (${Select[${Me.Class.ShortName},PAL]}) {
				/if (${useMG} && !${timer_check_MG}) /call cast_AA_marrs_gift
			} else /if (${Select[${Me.Class.ShortName},SHD]}) {
				/if (${usePet}) /call check_pet
			} else /if (${Select[${Me.Class.ShortName},RNG]}) {
			} else /if (${Select[${Me.Class.ShortName},BST]}) {
			} else /if (${Select[${Me.Class.ShortName},BRD]}) {
				/if (${usePet}) /call check_pet
			}
		}
		
		| Melee: WAR,MNK,ROG,BER
		/if (${MeleeList.Find[${Me.Class.ShortName}]} && !${CastList.Find[${Me.Class.ShortName}]}) {
			| DEBUG REST WAR,MNK,ROG,BER
			/if (${Me.PctEndurance} < ${autoRest}*.${tmpHealOverride}) /varset _needrest TRUE
			/call cast_disc_breather
			/if (${Select[${Me.Class.ShortName},WAR]}) {
			} else /if (${Select[${Me.Class.ShortName},MNK]}) {
			} else /if (${Select[${Me.Class.ShortName},ROG]}) {
			} else /if (${Select[${Me.Class.ShortName},BER]}) {
			}
		}
		
		| Caster: CLR,DRU,SHM,ENC,WIZ,NEC,MAG
		/if (!${MeleeList.Find[${Me.Class.ShortName}]} && ${CastList.Find[${Me.Class.ShortName}]}) {
			| DEBUG REST CLR,DRU,SHM,ENC,WIZ,NEC,MAG
			/if (${Me.PctMana} < ${autoRest}*.${tmpHealOverride}) /varset _needrest TRUE
			/call use_mod_rod

			/if (${Select[${Me.Class.ShortName},CLR]}) {
				/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
				/if (${Me.PctMana} < 60*.${tmpHealOverride} && ${Me.AltAbilityReady[Veturika's Perseverance]} && !${flagCallNeedRez}) /call core_cast "Veturika's Perseverance" alt 0
				/if (${Me.AltAbilityReady[Quiet Miracle]} && !${flagCallNeedRez} && !${timer_check_QM}) /call cast_AA_quiet_miracle
			} else /if (${Select[${Me.Class.ShortName},DRU]}) {
				/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
			} else /if (${Select[${Me.Class.ShortName},SHM]}) {
				/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
				/if (${Me.PctMana} < 60*.${tmpHealOverride} && !${flagCallNeedRez}) /call check_cani
			} else /if (${Select[${Me.Class.ShortName},ENC]}) {
			} else /if (${Select[${Me.Class.ShortName},WIZ]}) {
			} else /if (${Select[${Me.Class.ShortName},NEC]}) {
				/if (${usePet}) /call check_pet
			} else /if (${Select[${Me.Class.ShortName},MAG]}) {
				/if (${usePet}) /call check_pet
			}
		} 			
	}
		
	/if (${_needrest}) {
		DEBUG RESTING ${Me.PctMana} / ${Me.PctEndurance}
		UPDATEHUD "REST"
		/if (${Me.State.NotEqual[SIT]} && !${Bool[${Me.Mount.ID}]}) /sit

			| /if (${setPullMode} && !${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${MobAgro} playerstate 4]}) {
			| 	/delay ${Math.Calc[${Math.Rand[5]}+2]}s ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${MobAgro} playerstate 4]}
			| } else /if (!${setPullMode}) {
				/delay 1
			| }
		CHECKTIE
		ISDEAD
		/goto :loopneedmorerest
	} 
	
	:exit
	UPDATEHUD
/return



|***
 *	DES: set Home camp spot
 *	USE: /call set_home
 *	INI: 
 *  NOTE: 
 ***|
sub set_home()
	DEBUG ${cbug}set_home(\ax${HomeSet}${cbug})\ax
	/if (${HomeSet}) {
		/call Event_SetVar NA SetTie Movement FALSE
		NOUT Radius: ${cinfo}${HomeRadius}\ax ${sep} Leash: ${cinfo}${HomeLeash}\ax
		NOUT ${cgood}Camp\ax established. Y: ${cinfo}${Me.Y}\ax ${sep} X: ${cinfo}${Me.X}\ax 
		/squelch /makecamp on radius ${HomeRadius} leash ${HomeLeash} mindelay ${HomeDelayMin} maxdelay ${HomeDelayMax}		
	} else /if (!${HomeSet}) {
		NOUT ${ocaution} ${sep} No Camp established.
		/squelch /makecamp off
	}
	/squelch /moveto unpause
	/squelch /stick off
	:exit
/return



|***
 *	DES: stops follow and re-establishes camp if needed. This stops everything.. seriously.. everything.
 *	USE: /stop or /alto or /squelch /bct ${Me} stop or /call set_stop [TRUE]
 *	INI: 
 *  NOTE: just because you stop something, does not mean you will not start it right back up again.
 ****|
sub set_stop(bool _noecho)
	DEBUG ${cbug}set_stop()\ax
	UPDATEHUD "STOPPING"
	/if (${Defined[combat]}) /varset combat ${If[${AutoBot},${Ini[${INIFile},Common,combat]},FALSE]}
	/if (!${Bool[${_noecho}]}) VOUT I was told to ${cbad}STOP\ax !! 
	/if (${Me.Casting.ID}) /stopcast
	| :loopkillstick
		ISDEAD
		| stop sticking to stuff
		/if (${Stick.Active}) /squelch /stick off
		/if (${Bool[${tempsnapstuck}]}) /varset tempsnapstuck FALSE
		/if (${Bool[${SetTie}]}) /call Event_SetVar NA SetTie Movement FALSE
		
		| stop advanced path
		/if (${AdvPath.Following}) /squelch /afollow off

		| stop moving
		/if (${Me.Moving}) {
			/keypress forward
			/keypress back
		}
 	| /if (${Stick.Active} || ${AdvPath.Following} || ${Bool[${tempsnapstuck}]} || ${Bool[${SetTie}]}) /goto :loopkillstick 	
 	 	
	| /call clear_combat
	/if (${Bool[${usePet}]} && ${Me.Pet.ID} && ${Bool[${Me.Pet.Target}]}) /pet back off
	/doevents flush
	/if (${Bool[${tempHomeSet}]}) {
		/call Event_SetVar NA HomeSet Movement TRUE 
		/call set_home
	}

	:exit
	UPDATEHUD
/return



|***
 *	DES: Clicks usefull 1 off items
 *	USE: /call click_item [TRUE|FALSE] *if you want epic clicked (self only)
 *	INI: [Items] -> doclickItem
 *  NOTE: 
 ***|
sub click_item(bool _clickEpic)
	DEBUG ${cbug}click_item()\ax
	/call check_exit
	/if (${Bool[${Macro.Return}]} || ${Spawn[${Target.ID}].PctHPs} < 15*.${tmpHealOverride}) /goto :exit
	/if (${_clickEpic} && ${Me.ItemReady[${Epic}]} && ${Spell[${FindItem[${Epic}].Spell}].Stacks} && !${Me.Buff[${FindItem[${Epic}].Spell}].ID} && !${Me.Song[${FindItem[${Epic}].Spell}].ID} && ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${MobAgro} playerstate 4 targetable]} >= 2) /call core_cast "${Epic}" item 0

	/declare _count int local
	/for _count 1 to ${doclickItem}
		/doevents
		ISDEAD
		CHECKTIE
		/call is_target_dead
		/if (${Bool[${Macro.Return}]}) /goto :exit
		| /if (${Me.ItemReady[${clickItem${_count}}]}) /call MQ2Cast "${clickItem${_count}}" item -targetid|${Target2Kill}
		/if (${Me.ItemReady[${clickItem${_count}}]}) /call core_cast "${clickItem${_count}}" item ${Target2Kill}

	/next _count
	:exit
/return



|***
 *	DES: how to handle cursor items
 *	USE: /call sort_cursor || CLEARCURSOR
 *	INI: core.loot.ini
 *  NOTE: 
 ***|
sub sort_cursor(bool _ignoreTimer)
	DEBUG ${cbug}sort_cursor()\ax
	/if (${Bool[${_ignoreTimer}]} || ${Bool[${autoInventoryItem.Find[${Cursor.Name}]}]}) /goto :ignoretimer
	/if (${AutoBot} && ${Bool[${timer_clear_cursor}]}) /goto :exit
	/if (!${AutoBot} && !${Bool[${autoInventoryItem.Find[${Cursor.Name}]}]}) /goto :exit
	/if (!${Bool[${Cursor.ID}]}) /goto :exit
	
	:ignoretimer
	/declare _cursorChoice string local 
	:loopiscursorempty
		DEBUG :loopiscursorempty
		/if (!${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}].Length}) /ini "${INILoot}" "${Cursor.Name.Left[1]}" "${Cursor.Name}" "Keep"
		/if (${Cursor.ID}) /varset _cursorChoice ${Ini[${INILoot},"${Cursor.Name.Left[1]}","${Cursor.Name}"]}
		/if (${Bool[${autoInventoryItem.Find[${Cursor.Name}]}]} || ${Select[${_cursorChoice},Keep,Sell,Announce]}) {
			/if (${Select[${_cursorChoice},Announce]}) ECHOCHANNEL Cursor: ${Cursor.Name}
			/if (${Cursor.ID}) VOUT Cursor: Keep ${sep} ${cinfo}${Cursor.Name}\ax
			/if (${Cursor.ID}) /nomodkey /autoinventory
		} else /if (${Select[${_cursorChoice},Destroy]}) {
			/if (${Cursor.ID}) VOUT Cursor: ${odestroy} ${sep} ${cinfo}${Cursor.Name}\ax
			/if (${Cursor.ID} && ${_cursorChoice.Equal[Destroy]}) /nomodkey /destroy
		} else /if (${Select[${_cursorChoice},Leave]}) {
			/if (${Cursor.ID}) VOUT Cursor: Leave ${sep} ${cinfo}${Cursor.Name}\ax
			/if (${Cursor.ID} && ${_cursorChoice.Equal[Leave]}) /nomodkey /drop
		} else /if (${Select[${_cursorChoice},Ignore]}) {
			/goto :exit
		}
	/delay 1
	/if (${Cursor.ID}) /goto :loopiscursorempty
	/if (${AutoBot}) /call create_timer timer_clear_cursor ${tClear_Cursor}
	:exit
/return



|***
 *	DES: XTARGET controls
 *	USE: its in the toons INI
 *	INI: umm several different places.
 *  NOTE: for healers its /healxtarget
 ***|
sub set_xtarget()
	DEBUG ${cbug}setXTarget()\ax 
	VOUT Sorting XTargets every ${cinfo}${tXHeal}\ax
	/declare _x 				int 		local 
	/declare _b 				int 		local 0
	/declare _count 		int			local
	
	
	| WTF is this for???
	|** 
	/for _count 1 to ${AllList.Count[|]}
		/declare ${AllList.Arg[${_count},|]} int local ${SpawnCount[pc ${AllList.Arg[${_count},|]} raid radius 300]}
	/next _count	
	**|


	/for _x 1 to 13
		/if (${Bool[${toonControl}]} && ${Bool[${toonControlXTslot}]} == ${_x} && ${toonControlProtect}) /next _x
		/xtarget set ${_x} Autohater
	/next _x


	| I left the DEBUG in for this cause I'll forget what I did :(
	| set protection for NPCs
	/if (${Bool[${ProtectNPCList}]}) {
		DEBUG Setting Protected NPCs ${cinfo}${ProtectNPCList}\ax
		/for _x 1 to ${ProtectNPCList.Count[|]}
			DEBUG x:${_x} b:${_b} count:${ProtectNPCList.Count[|]} spawn:${ProtectNPCList.Arg[${_x},|]}
			/if (${_b} > 13 || !${Bool[${Spawn[${ProtectNPCList.Arg[${_x},|]}]}]}) {
				DEBUG ${ProtectNPCList.Arg[${_x},|]}: not found in zone
			} else /if (${Bool[${Me.XTarget[${ProtectNPCList.Arg[${_x},|]}]}]}) {
				/varcalc _b ${_b}+1
				DEBUG ${ProtectNPCList.Arg[${_x},|]}: already in XTarget
			} else /if (${Bool[${Spawn[${ProtectNPCList.Arg[${_x},|]}]}]}) {
				DEBUG ${ProtectNPCList.Arg[${_x},|]}: adding to XTarget
				/varcalc _b ${_b}+1
				/xtarget set ${_b} ${Spawn[${ProtectNPCList.Arg[${_x},|]}].Name}
			}
		/next _x
	}	


	| protect PCs when outside group
	/if (!${Bool[${Raid.Members}]} && ${Bool[${ProtectOutsideGroup}]}) {
		DEBUG Setting Protected Out of Group Players ${cinfo}${ProtectOutsideGroup}\ax
		/for _x 1 to ${ProtectOutsideGroup.Count[|]}
			DEBUG x:${_x} b:${_b} count:${ProtectOutsideGroup.Count[|]} spawn:${ProtectOutsideGroup.Arg[${_x},|]}
			/if (${Spawn[${ProtectOutsideGroup.Arg[${_x},|]}].Name.Equal[${toonControl}]}) /next _x
			/if (${_b} > 13 || !${Bool[${Spawn[${ProtectOutsideGroup.Arg[${_x},|]}]}]} || ${Group.Member[${ProtectOutsideGroup.Arg[${_x},|]}].ID}) /next _x
			/if (${Bool[${Me.XTarget[${ProtectOutsideGroup.Arg[${_x},|]}]}]}) {
				/varcalc _b ${_b}+1
			} else /if (${Bool[${Spawn[${ProtectOutsideGroup.Arg[${_x},|]}]}]}) {
				/varcalc _b ${_b}+1
				/xtarget set ${_b} ${Spawn[${ProtectOutsideGroup.Arg[${_x},|]}].Name}
			}
		/next _x
	}				


	| set raid member classes to protect
	/if (${Bool[${Raid.Members}]}) {
		/if (${Bool[${ProtectInsideRaid}]}) {
			DEBUG Setting Raid Protected Players ${cinfo}${ProtectInsideRaid}\ax
			/for _x 1 to ${ProtectInsideRaid.Count[|]}
				/if (${Spawn[${ProtectOutsideGroup.Arg[${_x},|]}].Name.Equal[${toonControl}]}) /next _x
				/if (${_b} > 13 || !${Bool[${Spawn[${ProtectInsideRaid.Arg[${_x},|]}]}]}) {
				} else /if (${Bool[${Me.XTarget[${ProtectInsideRaid.Arg[${_x},|]}]}]}) {
					/varcalc _b ${_b}+1
				} else /if (${Bool[${Spawn[${ProtectInsideRaid.Arg[${_x},|]}]}]}) {
					/varcalc _b ${_b}+1
					/xtarget set ${_b} ${Spawn[${ProtectInsideRaid.Arg[${_x},|]}].Name}
				}
			/next _x
		}


		DEBUG Setting Raid Protected Classes ${cinfo}${XHealClass}\ax

		/if (${WAR} && ${XHealClass.Find[WAR]}) {
			/for _x 1 to ${WAR}
				/if (${_b} > 13) /goto :exit
				/if (${Group.Member[${NearestSpawn[${_x}, pc WAR raid]}].ID} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc WAR raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc WAR raid].Name}]}) /next _x 
				/xtarget set ${_b} ${NearestSpawn[${_x}, pc WAR raid]}
				/varcalc _b ${_b}+1
			/next _x
		}
		/if (${SHD} && ${XHealClass.Find[SHD]}) {
			/for _x 1 to ${SHD}
				/if (${_b} > 13) /goto :exit
				/if (${Group.Member[${NearestSpawn[${_x}, pc SHD raid].ID}]} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc SHD raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc SHD raid].Name}]}) /next _x
				/xtarget set ${_b} ${NearestSpawn[${_x}, pc SHD raid]}
				/varcalc _b ${_b}+1
			/next _x
		}
		/if (${PAL} && ${XHealClass.Find[PAL]}) {
			/for _x 1 to ${PAL}
				/if (${_b} > 13) /goto :exit
				/if (${Group.Member[${NearestSpawn[${_x}, pc PAL raid].ID}]} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc PAL raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc PAL raid].Name}]}) /next _x
				/xtarget set ${_b} ${NearestSpawn[${_x}, pc PAL raid]}
				/varcalc _b ${_b}+1
			/next _x
		}
		/if (${RNG} && ${XHealClass.Find[RNG]}) {
			/for x 1 to ${RNG}
				/if (${_b} > 13) /goto :exit
				/if (${Group.Member[${NearestSpawn[${_x}, pc RNG raid].ID}]} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc RNG raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc RNG raid].Name}]}) /next _x
				/xtarget set ${_b} ${NearestSpawn[${_x}, pc RNG raid]}
				/varcalc _b ${_b}+1
			/next x
		}
		/if (${BER} && ${XHealClass.Find[BER]}) {
			/for _x 1 to ${BER}
				/if (${_b} >13) /goto :exit
				/if (${Group.Member[${NearestSpawn[${_x}, pc BER raid].ID}]} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc BER raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc BER raid].Name}]}) /next _x
				/xtarget set ${_b} ${NearestSpawn[${_x}, pc BER raid]}
				/varcalc _b ${_b}+1
			/next _x
		}
		/if (${ROG} && ${XHealClass.Find[ROG]}) {
			/for _x 1 to ${ROG}
				/if (${_b} > 13) /goto :exit
				/if (${Group.Member[${NearestSpawn[${_x}, pc ROG raid].ID}]} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc ROG raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc ROG raid].Name}]}) /next _x
				/xtarget set ${_b} ${NearestSpawn[${_x}, pc ROG raid]}
				/varcalc _b ${_b}+1
			/next _x
		}	
		/if (${BRD} && ${XHealClass.Find[BRD]}) {
			/for _x 1 to ${BRD}
				/if (${_b} > 13) /goto :exit
				/if (${Group.Member[${NearestSpawn[${_x}, pc BAR raid].ID}]} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc BAR raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc BAR raid].Name}]}) /next _x
				/xtarget set ${_b} ${NearestSpawn[${_x}, pc BAR raid]}
				/varcalc _b ${_b}+1
			/next _x
		}
		/if (${MNK} && ${XHealClass.Find[MNK]}) {
			/for _x 1 to ${MNK}
				/if (${_b} > 13) /goto :exit
				/if (${Group.Member[${NearestSpawn[${_x}, pc MNK raid].ID}]} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc MNK raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc MNK raid].Name}]}) /next _x
				/xtarget set ${_b} ${NearestSpawn[${_x}, pc MNK raid]}
				/varcalc _b ${_b}+1
			/next _x
		}		
		/if (${BST} && ${XHealClass.Find[BST]}) {
			/for _x 1 to ${BST}
				/if (${_b} > 13) /goto :exit
				/if (${Group.Member[${NearestSpawn[${_x}, pc BST raid].ID}]} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc BST raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc BST raid].Name}]}) /next _x
				/xtarget set ${_b} ${NearestSpawn[${_x}, pc BST raid]}
				/varcalc _b ${_b}+1
			/next _x
		}			
		/if (${NEC} && ${XHealClass.Find[NEC]}) {
			/for _x 1 to ${NEC}
				/if (${_b} > 13) /goto :exit
				/if (${Group.Member[${NearestSpawn[${_x}, pc NEC raid].ID}]} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc NEC raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc NEC raid].Name}]}) /next _x
				/xtarget set ${_b} ${NearestSpawn[${_x}, pc NEC raid]}
				/varcalc _b ${_b}+1
			/next _x
		}
		/if (${ENC} && ${XHealClass.Find[ENC]}) {
			/for x 1 to ${ENC}
				/if (${_b} > 13) /goto :exit
				/if (${Group.Member[${NearestSpawn[${_x}, pc ENC raid].ID}]} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc ENC raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc ENC raid].Name}]}) /next _x
				/xtarget set ${_b} ${NearestSpawn[${_x}, pc ENC raid]}
				/varcalc _b ${_b}+1
			/next _x
		}
		/if (${MAG} && ${XHealClass.Find[MAG]}) {
			/for _x 1 to ${MAG}
				/if (${_b} > 13) /goto :exit
				/if (${Group.Member[${NearestSpawn[${_x}, pc MAG raid].ID}]} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc MAG raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc MAG raid].Name}]}) /next _x
				/xtarget set ${_b} ${NearestSpawn[${_x}, pc MAG raid]}
				/varcalc _b ${_b}+1
			/next _x
		}
		/if (${WIZ} && ${XHealClass.Find[WIZ]}) {
			/for _x 1 to ${WIZ}
				/if (${_b} > 13) /goto :exit
				/if (${Group.Member[${NearestSpawn[${_x}, pc WIZ raid].ID}]} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc WIZ raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc WIZ raid].Name}]}) /next _x
				/xtarget set ${_b} ${NearestSpawn[${_x}, pc WIZ raid]}
				/varcalc _b ${_b}+1
			/next _x
		}
		/if (${CLR} && ${XHealClass.Find[CLR]}) {
			/for _x 1 to ${CLR}
				/if (${_b} > 13) /goto :exit
				/if (${Group.Member[${NearestSpawn[${_x}, pc CLR raid].ID}]} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc CLR raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc CLR raid].Name}]}) /next _x
				/xtarget set ${_b} ${NearestSpawn[${_x}, pc CLR raid]}
				/varcalc _b ${_b}+1
			/next _x
		}
		/if (${DRU} && ${XHealClass.Find[DRU]}) {
			/for _x 1 to ${DRU}
				/if (${_b} > 13) /goto :exit
				/if (${Group.Member[${NearestSpawn[${_x}, pc DRU raid].ID}]} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc DRU raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc DRU raid].Name}]}) /next _x
				/xtarget set ${_b} ${NearestSpawn[${_x}, pc DRU raid]}
				/varcalc _b ${_b}+1
			/next _x
		}
		/if (${SHM} && ${XHealClass.Find[SHM]}) {
			/for _x 1 to ${SHM}
				/if (${_b} > 13) /goto :exit
				/if (${Group.Member[${NearestSpawn[${_x}, pc SHM raid].ID}]} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc SHM raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc SHM raid].Name}]}) /next _x
				/xtarget set ${_b} ${NearestSpawn[${_x}, pc SHM raid]}
				/varcalc _b ${_b}+1
			/next _x
		}
	}
	:exit
/return



|***
 *	DES: Sticks you to the target
 *	USE: /call stick_to_target [target ID]
 *	INI: na
 *  NOTE: for the love of whatever you find important, don't touch this ever again...
 ***|
sub stick_to_target(int _tmpID)
	DEBUG ${cbug}stick_to_target()\ax Setting Stick to: [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax] @ ${CombatDistance}% agro distance
	/if (${Select[${combat},RANGE,NUKE,HEAL]}) /goto :exit
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/doevents
	ISDEAD	
	/if (${AutoBot} && ${SetStickBehind}) {
		DEBUG ${cbug}stick_to_target(behind ${SetStickBehind})\ax AutoBot
		/call get_assist_target
		/if (${Target.ID} && ${Select[${combat},MELEE,RANGE]}) /squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
			
		/if (!${Stick.Active} && ${Target.ID} && ${isValidTarget} && ${SetStickBehind} && !${Stick.Behind} && !${timer_snap_rechedckID_${Target2Kill}}) {
			/squelch /stick id ${Target2Kill} ${CombatDistance} snaproll
		} 

		:checkstuck
		/doevents
		ISDEAD	
		/call is_target_dead
		/if (${Bool[${Macro.Return}]}) /goto :exit
		/if (${MoveUtils.Stuck}) {
			/delay 1
			/squelch /stick off
			/call create_timer timer_snap_rechedckID_${Target2Kill} ${tCheck_snap_retry}
			/goto :exit
		}	
			
		/if (${Spawn[${Target2Kill}].Distance} < ${CombatDistance} && !${SetStickBehind} && !${timer_snap_rechedckID_${Target2Kill}}) {
			/squelch /stick off
			/goto :exit
		}
		/if (!${Stick.Behind} && ${SetStickBehind} && !${timer_snap_rechedckID_${Target2Kill}}) /goto :checkstuck
	} else {
		DEBUG ${cbug}stick_to_target()\ax
		/if (${setHunterMode}) /varset isValidTarget TRUE
		:movetomob
		/doevents
		ISDEAD	
		/call is_target_dead
		/if (${Bool[${Macro.Return}]}) /goto :exit
		/if (!${Stick.Active} && ${Target.ID} && ${isValidTarget} && ${Spawn[${Target2Kill}].Distance} > ${CombatDistance}) {
			/if (${AutoBot} && ${Select[${combat},MELEE,RANGE]}) /squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
			/if (${Select[${combat},RANGE]}) /goto :exit
			DEBUG stick_to_target to mob. farther then:${CombatDistance}
			/squelch /stick id ${Target2Kill} ${CombatDistance} 
		}
		/doevents
		/delay 1
		/if (${Spawn[${Target2Kill}].Distance} > ${CombatDistance}) /goto :movetomob
		/if (${Spawn[${Target2Kill}].Distance} < ${CombatDistance}) /squelch /stick off
		/goto :exit
	}
	
	:exit
	/squelch /stick off
/return



|***
 *	DES: Acquire target, get bitches. This will establish ${Target2Kill} for everyone.
 *	USE: /call get_assist_target
 *	INI: 
 *  NOTE: 
 ***|
sub get_assist_target()
	DEBUG ${cbug}get_assist_target()\ax
	ISDEAD
	/if (!${AutoBot}) /goto :exit
	| if no MA present. move along..	
	/if (!${Bool[${Spawn[${MA1}].ID}]} && !${Bool[${Spawn[${MA2}].ID}]} && !${Bool[${useSmartAssist1}]} && !${Bool[${useSmartAssist2}]}&& !${Bool[${useSmartAssistG}]}) {
		DEBUG ${cbad}NO MA\ax
		/delay 1
		/goto :exit
	} 	
	
	| cannnot assist yourself
	/if (${Spawn[pc ${MA1}].ID} == ${Me.ID}) /MA1 FALSE
	/if (${Spawn[pc ${MA2}].ID} == ${Me.ID}) /MA2 FALSE
	/if (${Spawn[pc ${Group.MainAssist}].ID} == ${Me.ID}) /call Event_SetVar NA useSmartAssistG Common FALSE

	| tar acquisition when using smart assist. set XTarget positions.
	/if (${Bool[${useSmartAssist1}]} || ${Bool[${useSmartAssist2}]} || ${Bool[${useSmartAssistG}]}) {
		DEBUG Using SmartMA
	
		/if (${Me.XTarget[1].TargetType.NotEqual[Raid Assist 1 Target]} && ${Raid.Members} && ${Bool[${useSmartAssist1}]}) {
			/xtarget set 1 raidassist1target
			/xtarget set 2 Autohater
		} else /if (${Me.XTarget[2].TargetType.NotEqual[Raid Assist 2 Target]} && ${Raid.Members} && ${Bool[${useSmartAssist2}]}) {
			/xtarget set 1 raidassist1target
			/xtarget set 2 Autohater
		} else /if (${Me.XTarget[1].TargetType.NotEqual[Group Assist Target]} && ${Group.Members} && ${Bool[${useSmartAssistG}]}) {
			/xtarget set 1 groupassisttarget
		}
		
		| smart assist raid 1		
		/if (${Bool[${Spawn[${Me.XTarget[1].ID}].Type.Equal[NPC]}]} && ${Bool[${useSmartAssist1}]}) {
			/if (${Me.XTarget[1].PctHPs} <= ${smartengageAt} || ${ignoreEngageAt}) {
				/varset Target2Kill ${Me.XTarget[1].ID}
				/delay 1
				/if (${Spawn[id ${Target2Kill}].Distance} <= ${MobAgro} && ${Target.ID} != ${Target2Kill}) /target ID ${Target2Kill}
				/goto :exit
			}	
		}
		| smart assist raid 2
		/if (${Bool[${Spawn[${Me.XTarget[2].ID}].Type.Equal[NPC]}]} && ${Bool[${useSmartAssist2}]}) {
			/if (${Me.XTarget[2].PctHPs} <= ${smartengageAt} || ${ignoreEngageAt}) {
				/varset Target2Kill ${Me.XTarget[2].ID}
				/delay 1
				/if (${Spawn[id ${Target2Kill}].Distance} <= ${MobAgro} && ${Target.ID} != ${Target2Kill}) /target ID ${Target2Kill}
				/goto :exit
			}	
		}
		| smart assist group
		/if (${Bool[${Spawn[${Me.XTarget[1].ID}].Type.Equal[NPC]}]} && ${Bool[${useSmartAssistG}]}) {
			/if (${Me.XTarget[1].PctHPs} <= ${smartengageAt} || ${ignoreEngageAt} || (${Select[${Me.Class.ShortName},ENC]} && ${Select[${combat},DUAL]}) ) {
				/varset Target2Kill ${Me.XTarget[1].ID}
				/delay 1
				/if (${Spawn[id ${Target2Kill}].Distance} <= ${MobAgro} && ${Target.ID} != ${Target2Kill}) /target ID ${Target2Kill}
				/goto :exit
			}	
		}
		/goto :exit
	} 

	| target acquisition when not using smart assist.
	
	| MA1
	/if (${SpawnCount[npc radius ${MobAgro}]} && ${Bool[${MA1}]} && ${Bool[${Spawn[${MA1}].ID}]} && !${Bool[${Spawn[${MA1}].Dead}]} && !${Bool[${useSmartAssist1}]} && !${Bool[${useSmartAssist2}]} && !${Bool[${useSmartAssistG}]}) {
		:getassistma1
		/assist ${MA1}
		/delay 1s ${Me.AssistComplete}
		/if (!${Target2Kill} && ${Target.ID} && ${Spawn[${Target.ID}].Type.Equal[npc]}) {
			/varset Target2Kill ${Target.ID}
			DEBUG get_assist_target() target = ${Target.ID}
		} else /if (${Target2Kill} != ${Target.ID} && ${Target2Kill} != 0 && ${Target.ID} && ${Spawn[${Target}].Type.Equal[npc]}) {
			/delay 1
			/varset Target2Kill 0
			DEBUG get_assist_target() ${Target2Kill} != ${Target.ID} CLEAR TARGET
			/goto :exit
		}	
		/doevents
		ISDEAD
		/if (${Target2Kill} == ${Target.ID} && ${Spawn[${Target}].Type.Equal[npc]}) {
			/if (${Target.PctHPs} > ${engageAt} && !${ignoreEngageAt}) {
			DEBUG get_assist_target() Waiting to ENGAGE
				/delay 3
				/goto :getassistma1
			} 
		} else {
			DEBUG ${cbad}NO MA1 TARGET\ax
			/if (${Target.ID}) /squelch /target clear
			/goto :exit
		}
	}
	
	| MA2
	/if (${SpawnCount[npc radius ${MobAgro}]} && ${Bool[${MA2}]} && ${Bool[${Spawn[${MA2}].ID}]} && !${Bool[${Spawn[${MA2}].Dead}]} && !${Bool[${useSmartAssist1}]} && !${Bool[${useSmartAssist2}]} && !${Bool[${useSmartAssistG}]} && (!${Bool[${Spawn[${MA1}].ID}]} || ${Bool[${Spawn[${MA1}].Dead}]})) {
		:getassistma2
		/assist ${MA2}
		/delay 1s ${Me.AssistComplete}
		/if (!${Target2Kill} && ${Target.ID} && ${Spawn[${Target.ID}].Type.Equal[npc]}) {
			/varset Target2Kill ${Target.ID}
		} else /if (${Target2Kill} != ${Target.ID} && ${Target2Kill} != 0 && ${Target.ID} && ${Spawn[${Target}].Type.Equal[npc]}) {
			/delay 1
			/varset Target2Kill 0
			/goto :exit
		}	
		/doevents
		ISDEAD
		/if (${Target2Kill} == ${Target.ID} && ${Spawn[${Target}].Type.Equal[npc]}) {
			/if (${Target.PctHPs} > ${engageAt} && !${ignoreEngageAt}) {
				/delay 3
				/goto :getassistma1
			} 
		} else {
			/if (${Target.ID}) /squelch /target clear
			/goto :exit
		}
	}
	
	:exit
/return



|***
 *	DES: Make sure that what ever we are targeting is something we want to kill.
 *	USE: /call is_target_valid [target ID]
 *	INI: 
 *  NOTE: does not apply to manual mode. you are on your own there.
 ***|
sub is_target_valid(int _tmpID)
	DEBUG ${cbug}is_target_valid(\ax${_tmpID}${cbug})\ax checking: [${cinfo}${Spawn[${_tmpID}].DisplayName}\ax]
	ISDEAD
	/doevents
	/if (!${Target.ID} || (${SafeZone} && ${AutoBot})) {
		/varset isValidTarget FALSE
		/goto :exit
	}
	/varset isValidTarget TRUE
	/if (!${AutoBot}) /goto :exit
	/if (${Spawn[${_tmpID}].PctHPs} > ${engageAt} && !${setPullMode} && !${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG} && !${ignoreEngageAt}) {
		/delay 1
		/varset isValidTarget FALSE
		/goto :exit
	} else /if (${Spawn[${_tmpID}].PctHPs} > ${smartengageAt} && !${setPullMode} && (${useSmartAssist1} || ${useSmartAssist2} || ${useSmartAssistG}) && !${ignoreEngageAt} ) {
		/delay 1
		/varset isValidTarget FALSE
		/goto :exit
	}
	/if (${ExcludeList.Find[${Spawn[${_tmpID}].DisplayName}]}) /varset isValidTarget FALSE
	/if (${Target.ID} && ${Bool[${Target.Mezzed.Duration}]} && !${setPullMode}) /varset isValidTarget FALSE
	/if ((!${Spawn[${_tmpID}].LineOfSight} && !${setIgnoreLOS}) || ${Spawn[${_tmpID}].Distance} > ${MobAgro} ) /varset isValidTarget FALSE
	/if (${Spawn[${_tmpID}].Type.Equal[PC]}) /varset isValidTarget FALSE
	/if (${Spawn[${Target.ID}].Type.Equal[PC]}) /varset isValidTarget FALSE
	/if (${_tmpID} == ${Me.ID} || ${_tmpID} == ${Me.Pet.ID}) /varset isValidTarget FALSE


	| /if (${stoponDS} && ${Target.ID} && ${Bool[${Target.DSed}]} && !${Select[${combat},RANGE,HEAL]}) /varset isValidTarget FALSE
	| /if (${isValidTarget}) /varset Target2Kill ${Target.ID}
	| /if (${Spawn[${Target.ID}].Master.ID} || ${Spawn[${Spawn[${Target.ID}].Master}].Type.Equal[NPC]} || ${Spawn[${Target.ID}].Owner.ID}) /varset isValidTarget FALSE 
	| /if (${Spawn[${Target.ID}].Master.ID} || ${Spawn[${Target.ID}].Owner.ID}) /varset isValidTarget FALSE 
	
	| check smart assists
	/if (${Target.ID}) {
		/if (${useSmartAssist1} && ${Spawn[${Target2Kill}].ID} != ${Me.XTarget[1].ID}) {
			/call clear_combat
		} else /if (${useSmartAssist2} && ${Spawn[${Target2Kill}].ID} != ${Me.XTarget[2].ID}) {
			/call clear_combat
		} else /if (${useSmartAssistG} && ${Spawn[${Target2Kill}].ID} != ${Me.XTarget[1].ID}) {
			/call clear_combat
		}
	}
		
	:exit
	DEBUG ${Spawn[${_tmpID}].DisplayName} [ID:${cinfo}${_tmpID}\ax] is a valid target: ${If[${isValidTarget},${cgood}TRUE\ax,${cbad}FALSE\ax]} ${If[${Spawn[${_tmpID}].LineOfSight},${cgood}LoS\ax,${cbad}LoS\ax]} ${If[${Spawn[${_tmpID}].Distance} <= ${MobAgro},${cgood}${Spawn[${_tmpID}].Distance}\ax,${cbad}${Spawn[${_tmpID}].Distance}\ax]}
/return ${isValidTarget}



|***
 *	DES: Seriously, do you need to ask?
 *	USE: mob dead = win!
 *	INI: 
 *  NOTE: ass.U.ME the mob is alive until, it is not.
 ***|
sub is_target_dead()
	| DEBUG ${cbug}is_target_dead(\ax${Spawn[${Target2Kill}].DisplayName} [${Target2Kill}]${cbug})\ax
	ISDEAD
	/declare _isDead bool local FALSE

	/if (!${Target.ID} && !${setHunterMode}) /varset _isDead TRUE
	/if (${Target2Kill} == 0) /varset _isDead TRUE
	/if (${Bool[${_isDead}]}) /goto :isdead
	
	/if (${Target.Dead} || ${Target.Type.Equal[Corpse]}) /varset _isDead TRUE
	/if (${Bool[${_isDead}]}) /goto :isdead

	/if (${Bool[${Spawn[${Target2Kill}].Type.Equal[CORPSE]}]} || ${Spawn[${Target.ID}].Type.Equal[CORPSE]}) /varset _isDead TRUE
	/if (${Bool[${Spawn[${Target2Kill}]].Dead}]} || ${Spawn[${Target.ID}].Dead}) /varset _isDead TRUE
	/if (!${Spawn[${Target2Kill}].ID} || !${Bool[${Spawn[ID ${Target2Kill}]}]}) /varset _isDead TRUE
	/if (${Spawn[${Target.ID}].Type.Equal[PCCORPSE]}) /varset _isDead TRUE

	:isdead
	/if (${_isDead}) /call clear_combat
	
	DEBUG ${cbug}is_target_dead(\ax${ctar}${Spawn[${Target2Kill}].DisplayName}\ax [${cinfo}${Target2Kill}\ax]${cbug})\ax Dead: ${If[${_isDead},${cgood}TRUE\ax,${cbad}FALSE\ax]}
/return ${Bool[${_isDead}]}



|***
 *	DES: Clear all combat variables
 *	USE: /call clear_combat
 *	INI: na
 *  NOTE: 
 ***|
sub clear_combat()
	DEBUG ${cbug}clear_combat()\ax
	ISDEAD
	/if (${Me.AutoFire}) /squelch /autofire
	/if (${Me.Combat}) /squelch /attack off
	/if (${Bool[${Me.Casting}]} && (!${Spell[${Me.Casting.ID}].Category.Equal[Transport]} || !${Spell[${Me.Casting.ID}].Category.Equal[Heals]})) /stopcast
	/if (${Bool[${Target2Kill}]}) {
		/call clear_timer ${Target2Kill}
		/varset Target2Kill 0
	}
	/if (${Bool[${isValidTarget}]}) /varset isValidTarget FALSE
	/if (${Bool[${Plugin[mq2twist]}]}) /squelch /twist clear
	/if (${Stick.Active}) /squelch /stick off
	/if (${Target.ID}) /squelch /target clear
	UPDATEHUD
	:exit
/return



|***
 *	DES: Clears all defined timers for a specific ID
 *	USE: /call clear_timer [TARGETID]
 *	INI: na
 *  NOTE: timers that need clearing when the target is deaded
 ***|
sub clear_timer(int _tmpID)
	DEBUG	${cbug}clear_timers()\ax
	/if (!${_tmpID}) /goto :exit

	| mezed/resist timer for enchanters/necros/bards
	/if (${Defined[timer_mez_${_tmpID}]}) /deletevar timer_mez_${_tmpID}
	| slowed/resist timer
	/if (${Defined[timer_slow_${_tmpID}]}) /deletevar timer_slow_${_tmpID}
	| tash/resist timer
	/if (${Defined[timer_tash_${_tmpID}]}) /deletevar timer_tash_${_tmpID}
	| cripple/resist timer
	/if (${Defined[timer_cripple_${_tmpID}]}) /deletevar timer_cripple_${_tmpID}
	| eradicate magic for rangers, druids, and INT casters
	/if (${Defined[timer_eradicate_${_tmpID}]}) /deletevar timer_eradicate_${_tmpID}
	| dont want to keep snaring the same mob
	/if (${Defined[timer_snare_${_tmpID}]}) /deletevar timer_snare_${_tmpID}
	| druid Ro debuff
	/if (${Defined[timer_ro_${_tmpID}]}) /deletevar timer_ro_${_tmpID}
	| if you have to ask
	/if (${Defined[timer_banestrike_${_tmpID}]}) /deletevar timer_banestrike_${_tmpID}
	| stick to mobs ass check
	/if (${Defined[timer_snap_recheckID_${_tmpID}]}) /deletevar timer_snap_rechedckID_${_tmpID}
	
	:exit
/return



|***
 *	DES: Cast DoT's
 *	USE: /call cast_DOT
 *	INI: 
 *  NOTE: 
 ***|
sub cast_DOT()
	DEBUG ${cbug}cast_DOT(count ${DoDoT})\ax
	/declare _count int local
	/for _count 1 to ${DoDoT}
		ISDEAD
		/doevents
		/call check_exit TRUE
		/if (${Bool[${Macro.Return}]}) /goto :exit	
		/call is_target_dead
		/if (${Bool[${Macro.Return}]}) /goto :exit		

		/if ((${Target.BuffDuration[${DoT${_count}}]} <= 2 && ${Spawn[${Target.Buff[${DoT${_count}}].Caster}].ID} == ${Me.ID}) || !${Target.Buff[${DoT${_count}}].ID}) {
			/call core_cast "${DoT${_count}}" 0  ${Target2Kill} cast_break
		}
	/next _count
	:exit
/return



|***
 *	DES: Cast nukes
 *	USE: /call cast_nuke
 *	INI: 
 *  NOTE: "I say we take off and nuke the entire site from orbit." --Ripley
 ***|
sub cast_nuke()
	DEBUG ${cbug}cast_nuke()\ax
	/call get_assist_target
	/declare _count int local
	/for _count 1 to ${DoNuke}
		ISDEAD
		/doevents
		/call check_exit TRUE
		/if (${Bool[${Macro.Return}]}) /goto :exit	
		/call is_target_dead
		/if (${Bool[${Macro.Return}]}) /goto :exit		
		| /if (${Spawn[${Target2Kill}].LineOfSight} && ${Me.SpellReady[${Nuke${_count}}]} && ${Spawn[${Target2Kill}].Distance} <= ${Spell[${Nuke${_count}}].MyRange} && ${Me.CurrentMana} >= ${Spell[${Nuke${_count}}].Mana}) {
			/if (${Bool[${Target2Kill}]}) /call core_cast "${Nuke${_count}}" 0 ${Target2Kill} cast_break
		| }
	/next _count
	:exit
/return



|***
 *	DES: casts AE Rain spells. Magician mostly...
 *	USE: /call cast_nuke_rain | /userain [TRUE|FALSE]
 *	INI: 
 *  NOTE: I'd suggest not using this. though it is kind of fun to watch
 ***|
sub cast_nuke_rain()
	DEBUG ${cbug}cast_nuke_rain()${cinfo}
	/call get_assist_target
	/declare _count int local
	/for _count 1 to ${doRainNuke}
		ISDEAD
		/doevents
		/call check_exit TRUE
		/if (${Bool[${Macro.Return}]}) /goto :exit	
		/call is_target_dead
		/if (${Bool[${Macro.Return}]}) /goto :exit	
		/if (${Spawn[${Target2Kill}].LineOfSight} && ${Me.SpellReady[${RainNuke${_count}}]} && ${Spawn[${Target2Kill}].Distance} <= ${Spell[${RainNuke${_count}}].MyRange} && ${Me.CurrentMana} >= ${Spell[${RainNuke${_count}}].Mana}) {
			/call core_cast "${RainNuke${_count}}" 0 ${Target2Kill} cast_break
		}
	/next _count
	:exit
/return



|***
 *	DES: casts Malo or Tash depending on the class
 *	USE:  /call cast_malo_tash [target ID]
 *	INI: 
 *  NOTE: 
 ***|
sub cast_malo_tash(int _tmpID)
	DEBUG ${cbug}cast_malo_tash(\ax${cinfo}${_tmpID}\ax${cbug})\ax
	ISDEAD
	/doevents
	/if (!${switchdeBuffMalo} && !${switchdeBuffTash} || !${Target.ID}) /goto :exit
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /goto :exit
	UPDATEHUD "Malo / Tash"
	/if (${switchdeBuffMalo} && ${Bool[${MaloSpell}]} && !${timer_malo_${_tmpID}} && !${Target.Maloed.ID} && ${Spawn[${_tmpID}].LineOfSight} && ${Spawn[${_tmpID}].Distance} <= ${Spell[${MaloSpell}].MyRange}) {
		/if (${Me.AltAbilityReady[${MaloSpell}]}) {
			/if (${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${MobAgro} playerstate 4]} > 2 && ${Select[${Me.Class.ShortName},MAG]}) {
				/call core_cast "Wind of Malosinete" alt ${_tmpID}
			} else {
				/call core_cast "${MaloSpell}" alt ${_tmpID}
			}
		} else /if (${Me.SpellReady[${MaloSpell}]}) {
			/call core_cast "${MaloSpell}" 0 ${_tmpID}
		}
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_RESISTED,CAST_RESIST,CAST_NOHOLD,CAST_TAKEHOLD]} || ${Target.Maloed.ID}) /call create_timer timer_malo_${_tmpID} ${Spell[${MaloSpell}].Duration.TotalSeconds}s

	}
	/if (${switchdeBuffTash} && ${Bool[${TashSpell}]} && !${timer_tash_${_tmpID}} && ${Target.Tashed.ID} && ${Spawn[${_tmpID}].LineOfSight} && ${Spawn[${_tmpID}].Distance} <= ${Spell[${TashSpell}].MyRange}) {
		/if (${Me.AltAbilityReady[${TashSpell}]}) {
			/call core_cast "${TashSpell}" alt ${_tmpID}
		} else /if (${Me.SpellReady[${TashSpell}]})) {
			/call core_cast "${TashSpell}" 0 ${_tmpID}
		}
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_RESISTED,CAST_RESIST,CAST_NOHOLD,CAST_TAKEHOLD]} || ${Target.Tashed.ID}) /call create_timer timer_tash_${_tmpID} ${Spell[${TashSpell}].Duration.TotalSeconds}s
	}

	UPDATEHUD
	:exit
/return



|***
 *	DES: use Banestrike AA
 *	USE: /call cast_AA_banestrike [target ID]
 *	INI: 
 *  NOTE: slap all the mobs
 ***|
sub cast_AA_banestrike(int _tmpID)
	DEBUG ${cbug}cast_AA_banestrike(ID:${_tmpID})\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]} || ${timer_banestrike_${_tmpID}}) /goto :exit
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/call is_target_valid ${_tmpID}
	/if (!${Bool[${Macro.Return}]}) /goto :exit
	| /if (${Me.AltAbilityReady[Banestrike]} && ${Spawn[${_tmpID}].Distance} <= ${Spell[Banestrike].MyRange} && ${Target.ID}) {
		/if (${Bool[${_tmpID}]}) /call core_cast "Banestrike" alt ${_tmpID}
		/if (${Select[${castReturn},CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) /call create_timer timer_banestrike_${_tmpID} 30m
	| }
	:exit
/return



|***
 *	DES: Returns the toons target to the intended kill ID.
 *	USE: /call get_target_2kill
 *	INI: 
 *  NOTE: sometimes people need focus.
***|
sub get_target_2kill()
	DEBUG ${cbug}get_target_2kill()\ax
	ISDEAD
	/doevents
	:gettarget
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/call is_target_dead
	/if (!${Bool[${Macro.Return}]} && ${Target.ID} != ${Target2Kill} || ${Spawn[id ${Target2Kill}].Distance} > ${MobAgro}) {
		/target ID ${Target2Kill}
		| /delay 2 ${Target.ID} == ${Target2Kill}
		/if (${Target.ID} != ${Target2Kill}) /goto :gettarget
	}	
	/if (${AutoBot} && ${Target.Distance} > ${MobAgro}) /call clear_combat
	:exit
/return



|***
 *	DES: Uses Entropy of Nature/EradicateMagic/Whatever on target. only once every 60 seconds.
 *	USE: /call cast_eradicate_magic [target ID] & /debufferadicate [toggle]
 *	INI: 
 *  NOTE: 
 ***|
sub cast_eradicate_magic(int _tmpID)
	DEBUG ${cbug}cast_eradicate_magic()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]} || ${timer_eradicate_${_tmpID}}) /goto :exit

	/if (!${AASnareList.Find[${Me.Class.ShortName}]}) {
		VOUT ${onotice} ${sep} Class does not have the AA Eradicate.
		/call Event_SetVar NA switchdeBuffEradicate Common FALSE
		/call echos switch "deBuff Eradicate" switchdeBuffEradicate	
		/goto :exit
	}
	/declare debuffEradicate string local
	/if (${Select[${Me.Class.ShortName},ENC,MAG,NEC,WIZ]}) /varset debuffEradicate Eradicate Magic
	/if (${Select[${Me.Class.ShortName},RNG,DRU]}) /varset debuffEradicate Entropy of Nature
	
	/if (${Me.AltAbilityReady[${debuffEradicate}]} && ${Spawn[${_tmpID}].Distance} <= ${Spell[${debuffEradicate}].MyRange}) {
		/call core_cast "${debuffEradicate}" alt ${_tmpID}
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_RESISTED,CAST_RESIST,CAST_NOHOLD,CAST_TAKEHOLD]}) /call create_timer timer_eradicate_${_tmpID} 1m
	}
	:exit
/return



|***
 *	DES: Uses class specific AA snares
 *	USE: /call cast_AA_snare [target ID] & /debuffsnare [toggle]
 *	INI: na
 *  NOTE: did I mention this is for classes with AA snares..
 ***|
sub cast_AA_snare(int _tmpID)
	DEBUG ${cbug}cast_AA_snare()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]} || ${timer_snare_${_tmpID}} || !${Target.ID}) /goto :exit

	/if (!${Select[${Me.Class.ShortName},ENC,MAG,NEC,WIZ,RNG,DRU]}) {
		VOUT ${onotice} ${sep} Not an AA snaring class. Please come back later.
		/call Event_SetVar NA switchdeBuffSnare Common FALSE  
		/call echos switch "deBuff Snare" switchdeBuffSnare	
		/goto :exit
	}
	
	/declare _debuffSnare string local
	/if (${Select[${Me.Class.ShortName},RNG,DRU]}) /varset _debuffSnare Entrap
	/if (${Select[${Me.Class.ShortName},WAR]}) /varset _debuffSnare Knee Strike
	/if (${Select[${Me.Class.ShortName},ROG]}) /varset _debuffSnare Ligament Slice
	/if (${Select[${Me.Class.ShortName},NEC,SHD]}) /varset _debuffSnare Encroaching Darkness
	/if (${Select[${Me.Class.ShortName},WIZ]}) /varset _debuffSnare Atol's Shackles	
	/if (${Select[${Me.Class.ShortName},MNK]}) /varset _debuffSnare Crippling Strike	
	
	/if (${Me.AltAbilityReady[${_debuffSnare}]} && ${Spawn[${_tmpID}].Distance} <= ${combatRadius} || !${Target.Snared.ID}) {
		/call core_cast "${_debuffSnare}" alt ${_tmpID}
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_RESISTED,CAST_RESIST,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_snare_${_tmpID} ${Spell[${_debuffSnare}].Duration.TotalSeconds}s
		} else /if (${Select[${castReturn},CAST_IMMUNE]}) {
			/call create_timer timer_snare_${_tmpID} ${Spell[${_debuffSnare}].Duration.TotalSeconds}m
		}
	} else /if (${Target.Snared.ID} && ${Spell[${Target.Snared.ID}].Duration.TotalSeconds} > 30) {
		/call create_timer timer_snare_${_tmpID} ${Spell[${Target.Snared.ID}].Duration.TotalSeconds}s
	}
	:exit
/return



|***
 *	DES: Users class specific Breastplates. Will alternate 2 if you have both. 
 *	USE: /call click_BP
 *	INI: 
 *  NOTE: 
 ***|
sub click_BP()
	DEBUG ${cbug}click_BP(${cinfo}${Breastplate1}, ${Breastplate2}\ax${cbug})\ax
	/doevents
	ISDEAD
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/if (!${Me.ItemReady[${Breastplate1}]} && !${Me.ItemReady[${Breastplate2}]}) /goto :exit
	/if (${Bool[${Me.Song[${Breastplate1effect}]}]} || ${Bool[${Me.Buff[${Breastplate2effect}]}]}) /goto :exit
	/if (!${Bool[${Me.Song[${Breastplate1effect}]}]} && ${Me.ItemReady[${Breastplate2}]} && ${Spell[${Breastplate2effect}].Stacks}) {
		/call core_cast "${Breastplate2}" item 0
	} else /if (!${Bool[${Me.Buff[${Breastplate2effect}]}]} && ${Me.ItemReady[${Breastplate1}]} && ${Spell[${Breastplate1effect}].Stacks}) {
		/call core_cast "${Breastplate1}" item 0
	}
	:exit
/return



|***
 *	DES: Uses a mod rod if you have one in your inventory
 *	USE: /call use_mod_rod
 *	INI: 
 *  NOTE: listed all i could think of. dont want to keep up with this in an INI.
 ***|
sub use_mod_rod()
	DEBUG ${cbug}use_mod_rod()\ax 
	/if (${Select[${Me.Class.ShortName},ROG,BER,WAR,MNK,BRD]}) /goto :exit
	/doevents
	ISDEAD
	/call check_exit
	/if (${Bool[${Bool[${Macro.Return}]}]}) /goto :exit
		
	| use all the rods
	/if (${Me.PctHPs} > 50*.${tmpHealOverride}) {
		/if (${Me.ItemReady[Summoned: Giant Modulation Shard]} && ${Me.PctMana} < 80*.${tmpHealOverride}) /call core_cast "Summoned: Giant Modulation Shard" item 0
		/if (${Me.ItemReady[Summoned: Large Modulation Shard]} && ${Me.PctMana} < 80*.${tmpHealOverride}) /call core_cast "Summoned: Large Modulation Shard" item 0
		/if (${Me.ItemReady[Summoned: Medium Modulation Shard]} && ${Me.PctMana} < 80*.${tmpHealOverride}) /call core_cast "Summoned: Medium Modulation Shard" item 0
		/if (${Me.ItemReady[Summoned: Small Modulation Shard]} && ${Me.PctMana} < 80*.${tmpHealOverride}) /call core_cast "Summoned: Small Modulation Shard" item 0
		/if (${Me.ItemReady[Wand of Pelagic Transvergence]} && ${Me.PctMana} < 70*.${tmpHealOverride}) /call core_cast "Wand of Pelagic Transvergence" item 0
		/if (${Me.ItemReady[Wand of Phantasmal Transvergence]} && ${Me.PctMana} < 70*.${tmpHealOverride}) /call core_cast "Wand of Phantasmal Transvergence" item 0
		/if (${Me.ItemReady[Rod of Arcane Transvergence]} && ${Me.PctMana} < 70*.${tmpHealOverride}) /call core_cast "Rod of Arcane Transvergence" item 0
		/if (${Me.ItemReady[Rod of Spectral Transvergence]} && ${Me.PctMana} < 70*.${tmpHealOverride}) /call core_cast "Rod of Spectral Transvergence" item 0
		/if (${Me.ItemReady[Rod of Ethereal Transvergence]} && ${Me.PctMana} < 70*.${tmpHealOverride}) /call core_cast "Rod of Ethereal Transvergence" item 0
		/if (${Me.ItemReady[Rod of Mystical Transvergence]} && ${Me.PctMana} < 70*.${tmpHealOverride}) /call core_cast "Rod of Mystical Transvergence" item 0
	}
	
	| ask for a rod cause i'm fu'kn lazy...
	/if (!${Bool[${timer_ask_for_rod}]} && !${Select[${Me.Class.ShortName},MAG]} && !${Bool[${FindItem[Modulation Shard]}]}) {
		/call tell_resident_mage shard
	}	
	:exit
/return



|***
 *	DES: Send Pet to attack
 *	USE: /call send_pet
 *	INI: na
 *  NOTE: 
 ***| 
sub send_pet(int _tmpID)
	DEBUG ${cbug}send_pet()${cinfo}
	/if (!${usePetSwarm} && !${usePet}) /goto :exit
	ISDEAD
	/doevents
	CHECKEXIT
	/if (${Spawn[${Target2Kill}].Distance} > ${MobAgro} || ${Spawn[${Target.ID}].Type.Equal[corpse]} || ${Target.Dead}) /goto :exit
	
	/if (!${Target2Kill}) {
		/call get_assist_target
		/call is_target_valid ${Target2Kill}
		/if (!${Bool[${Macro.Return}]}) /goto :exit
	}
	
	/target ID ${Target2Kill}
		
	/if (${usePet} && ${Me.Pet.ID} &&  (${Target.PctHPs} <= ${sendPetPerc} || ${ignoreEngageAt}) && !${Bool[${Me.Pet.Target}]}) {
		/call echos petfarm "filler" ${Target2Kill}
		| VOUT Pet: ${cinfo}${Me.Pet.DisplayName}\ax ${sep} [${If[${Bool[${Target.ID}]},${ctar}${Target.DisplayName}\ax,${cbad}BAD TARGET\ax]}] ${sep} ${If[${Spawn[${Target}].LineOfSight},${cgood}${Target.Distance}\ax,${cbad}${Target.Distance}\ax]}
		/pet attack
	}
	
	| send in the swarm
	/if (${usePetSwarm} && (${Target.PctHPs} <= ${sendPetSwarmPerc} || ${ignoreEngageAt})) /pet swarm

	:exit
/return



|***
 *	DES: Will use a glyph and buy it if able.
 *	USE: /call use_glyph
 *	INI: 
 *  NOTE: 
 ***|
sub use_glyph()
	DEBUG ${cbug}use_glyph()\ax
	ISDEAD
	/if (!${Bool[${useGlyph}]} || !${Raid.Members}) /goto :exit
	/if (!${Me.AltAbility[${useGlyph}].ID} && ${Me.AAPoints} >= ${AltAbility[${useGlyph}].Cost}) /alt buy ${AltAbility[${useGlyph}].ID}
	/if (${Me.AltAbilityReady[${useGlyph}]}) /call core_cast "${useGlyph}" alt 0
	/if (!${Me.AltAbility[${useGlyph}].ID} && ${Me.AAPoints} >= ${AltAbility[${useGlyph}].Cost}) /alt buy ${AltAbility[${useGlyph}].ID}
	:exit
/return



|***
 *	DES: spell/disc/aa/item cast wrapper
 *	USE: /call core_cast "SPELLNAME" [alt|disc|gem#|item|0|NULL] [MobID|0|NULL] [subname]
 *	USE: /call core_cast "SPELLNAME" NULL NULL if you want to let the sub decide what you have to cast/use.
 *	INI: 
 *  NOTE: MQ2Cast ["spell name"|"item name"|"AA name"|"AA#"] [[item|slotname]|alt|gem#] [give up time][s|m] [custom subroutine name] [-targetid|###] [-maxtries|#] [-recast|#] [-setin|setname] [-bandolier|setname]
 *  NOTE2: How about we quit making changes to this....
 ***|
sub core_cast(string _spell, string _type, int _tmpID, string _sub)
	DEBUG ${cbug}core_cast(\ax${cinfo}SPELL:${_spell}, TYPE:${_type}, ID:${_tmpID}, SUB:${_sub}\ax${cbug})\ax
	/doevents
	ISDEAD
	/declare _bail string local FALSE
	
	| preemptive gtfo for this particular spell
	| /if (${Bool[${timer_${_tmpID}_${Spell[${_spell}].ID}}]}) /goto :cooldown
	
	| make sure we have our target 
	/if (${Bool[${_tmpID}]} && ${Target.ID} != ${_tmpID}) {
		DEBUG fixing target to ID:${_tmpID} ${sep} ${Spawn[ID ${_tmpID}].DisplayName}
		/tar id ${_tmpID}
		/delay 1s ${Target.ID} == ${_tmpID}
	}
	
	| used to let /call core_cast "whatever" NULL NULL
	| figure out what the toon has and use that.
	/if (${_type.Equal[NULL]} && !${Bool[${_tmpID}]}) {
		DEBUG fixing cast type and target id NULL/NULL
		| /varset _tmpID 0
		/if (${Bool[${Me.Book[${_spell}]}]}) {
			/varset _type ${buffGem}
		} else /if (${Me.ItemReady[${_spell}]}) {
			/varset _type item
		} else /if (${Me.AltAbilityReady[${_spell}]}) {
			/varset _type alt
		} else /if (${Me.CombatAbilityReady[${_spell}]}) {
			/varset _type disc
		}
	}
	
	| READY CHECKS
	| dead?
	/if (${Target.Type.Equal[Corpse]} || ${Spawn[${_tmpID}].Type.Equal[corpse]}) /varset _bail target_dead
	| no target
	/if (!${Select[${_type},item]} && ${Bool[${_tmpID}]} && !${Target.ID}) /varset _bail no_target
	| moving?
	/if (!${Select[${_type},alt,item,disc]} && (${Stick.Active} || ${Me.Moving} || ${AdvPath.Following})) /varset _bail im_moving
	| spell casting?
	/if (!${Select[${_type},alt,item,disc]} && ${Me.Casting.ID}) /varset _bail im_casting_spell
	| line of site valid?
	/if (${_tmpID} != 0 && !${setIgnoreLOS} && !${Target.LineOfSight} && !${Spell[${_spell}].SpellType.Equal[Beneficial]}) /varset _bail bad_LoS
	| target in range?
	/if (!${Select[${_type},item]} && ${_tmpID} != 0 && (${Target.Distance} > ${Spell[${_spell}].MyRange} || ${Spawn[${_tmpID}].Distance} > ${Spell[${_spell}].Range})) /varset _bail out_of_range
	| enough mana?
	/if (!${Select[${_type},item,disc]} && ${Me.CurrentMana} < ${Spell[${_spell}].Mana}) /varset _bail mana_too_low
	| spell ready?
	/if (!${Select[${_type},alt,item,disc]} && !${Me.SpellReady[${_spell}]}) /varset _bail spell_not_ready
	| item ready?
	/if (${Select[${_type},item]} && !${Me.ItemReady[${_spell}]}) /varset _bail item_not_ready
	| AA ready?
	/if (${Select[${_type},alt]} && (!${Me.AltAbilityReady[${_spell}]} || !${Me.AltAbility[${_spell}].ID})) /varset _bail aa_not_ready
	| disc ready?
	/if (${Select[${_type},disc]} && !${Me.CombatAbilityReady[${_spell}]}) /varset _bail disc_not_ready
	| check enough end for disc?
	/if (${Select[${_type},disc]} && ${Me.PctEndurance} > ${Spell[${_spell}].EnduranceCost}) /varset _bail disc_endurance
	| spell stacks ?
	/if (!${Select[${_type},alt,item,disc]} && !${Spell[${_spell}].Stacks}) /varset _bail does_not_stack
	
	

	| /if (${Bool[${_bail}]} && !${Spell[${_spell}].SpellType.Equal[Beneficial]} && (${Target.ID} || ${_tmpID})) {
	/if (${Bool[${_bail}]} && !${Spell[${_spell}].SpellType.Equal[Beneficial]}) {
		DEBUG ${owarning} ${sep} _bail = ${_bail} for ${_spell}
		| /varset castReturn CAST_NOTREADY
		/goto :cooldown
	}

	| skip needing a target
	/if (${_tmpID} == 0 || ${Spell[${_spell}].SpellType.Equal[Self]}) /goto :notarget
	
	/if (!${Spawn[${_tmpID}].ID} && !${timer_spellcast_nomob_ID}) {
		VOUT You tried to cast ${csp}${_spell}\ax on something that doesn't exist.
		/varset castReturn CAST_NOTREADY
		/call create_timer timer_spellcast_nomob_ID 60s
	}

	| check target to immune lists
	/if (${Target.ID} && ${NoDiseaseList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Disease]}) {
		VOUT ${owarning} ${sep} [${ctar}${Target.DisplayName}\ax] ${cbad}Immune\ax to ${csp}${_spell}\ax ${sep} DISEASE
		/varset _bail immune_disease
	} else 	/if (${Target.ID} && ${NoMagicList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Magic]}) {
		VOUT ${owarning} ${sep} [${ctar}${Target.DisplayName}\ax] ${cbad}Immune\ax to ${csp}${_spell}\ax ${sep} MAGIC
		/varset _bail immune_magic
	}	else /if (${Target.ID} && ${NoPoisonList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Poison]}) {
		VOUT ${owarning} ${sep} [${ctar}${Target.DisplayName}\ax] ${cbad}Immune\ax to ${csp}${_spell}\ax ${sep} POISON
		/varset _bail immune_poison
	}	else /if (${Target.ID} && ${NoFireList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Fire]}) {
		VOUT ${owarning} ${sep} [${ctar}${Target.DisplayName}\ax] ${cbad}Immune\ax to ${csp}${_spell}\ax ${sep} FIRE
		/varset _bail immune_fire
	} else /if (${Target.ID} && ${NoColdList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Cold]}) {
		VOUT ${owarning} ${sep} [${ctar}${Target.DisplayName}\ax] ${cbad}Immune\ax to ${csp}${_spell}\ax ${sep} COLD
		/varset _bail immune_cold
	} else /if (${Target.ID} && ${NoCorruptionList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Corruption]}) {
		VOUT ${owarning} ${sep} [${ctar}${Target.DisplayName}\ax] ${cbad}Immune\ax to ${csp}${_spell}\ax ${sep} CORRUPTION
		/varset _bail immune_corruption
	}

	| bailing on this spell. its either out of range, immune or something..
	/if (${Bool[${_bail}]} && !${Spell[${_spell}].SpellType.Equal[Beneficial]} && (${Target.ID} || ${_tmpID})) {
		DEBUG ${owarning} ${sep} _bail = ${_bail} immune too ${_spell}
		| /varset castReturn CAST_NOTREADY
		/goto :cooldown
	}

| 	gem${Me.Gem[${_spell}]}
| ${If[${Bool[${_type}]},${_type},gem${Me.Gem[${_spell}]}]}
	:notarget
	ISDEAD
	DEBUG ${cbug}(\ax\awSPELL:${_spell}, TYPE:${_type}, ID:${_tmpID}, SUB:${_sub}\ax${cbug})\ax

	| Passed all checks. lets cast some shits..
	DEBUG passed all checks

	| Spell
	/if (!${Select[${_type},alt,item,disc]}) {
		/if (${_tmpID} == 0) {
			VOUT Spell: ${csp}${_spell}\ax
			/call MQ2Cast "${_spell}" ${If[${Bool[${_type}]},${_type},gem${Me.Gem[${_spell}]}]} -targetid|${_tmpID}
		} else /if (${_tmpID} != 0 && ${Target.ID}) {
			/if (${Me.Song[Gift of Mana (${GoMLevel})].ID}) /call cast_nuke_GoM ${_tmpID}
			DEBUG MQ2Cast "${_spell}" ${If[${Bool[${_type}]},${_type},gem${Me.Gem[${_spell}]}]} -targetid|${_tmpID}
			VOUT Spell: ${csp}${_spell}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
			/call MQ2Cast "${_spell}" ${If[${Bool[${_type}]},${_type},gem${Me.Gem[${_spell}]}]} ${If[${Bool[${_sub}]},${_sub},]} -targetid|${_tmpID}
		}
		/goto :cooldown
	}

	| AA
	/if (${Select[${_type},alt]}) {
		/if (${_tmpID} == 0) {
			VOUT AA: ${csp}${_spell}\ax
			/call MQ2Cast "${_spell}" alt 
		} else /if (${_tmpID} != 0 && ${Target.ID}) {
			VOUT AA: ${csp}${_spell}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
			/call MQ2Cast "${_spell}" alt -targetid|${_tmpID}
		}
		/goto :cooldown
	}
	
	| item
	/if (${Select[${_type},item]}) {
		/if (${_tmpID} == 0) {
			VOUT Item: ${citem}${_spell}\ax
			/call MQ2Cast "${_spell}" item
		} else /if (${_tmpID} != 0 && ${Target.ID}) {
			VOUT Item: ${citem}${_spell}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
			/call MQ2Cast "${_spell}" item -targetid|${_tmpID}
		}
		/goto :cooldown
	}
	
	| disc
	/if (${Select[${_type},disc]}) {
		/if (${_tmpID} == 0) VOUT Disc: ${csp}${_spell}\ax
		/if (${_tmpID} != 0 && ${Target.ID}) VOUT Disc: ${csp}${_spell}\ax ${sep} [${ctar}${Spawn[ID ${_tmpID}].DisplayName}\ax]
		/disc ${_spell}
		/delay 5s !${Me.Casting.ID}
		/goto :cooldown
	}

	DEBUG ${csp}${_spell}\ax ${sep}${sep}${sep}${sep} ${sepO}${castReturn}${sepC}

	:cooldown
	ISDEAD
	| cooldown
	/if (!${Select[${_type},alt,item,disc]}) {
		/delay 2 !${Me.SpellReady[${_spell}]}
	} else /if (${Select[${_type},item]}) {
		/delay 2 !${Me.ItemReady[${_spell}]}
	} else /if (${Select[${_type},alt]}) {
		/delay 2 !${Me.AltAbilityReady[${_spell}]}
	} else /if (${Select[${_type},disc]}) {
		/delay 2 !${Me.CombatAbilityReady[${_spell}]}
	}

	:exit
/return ${Bool[${Macro.Return}]}



|***
 *	DES: will halt spell casting and force Gift of Manna nukes if setup
 *	USE: sub for use with MQ2Cast call
 *	INI: 
 *  NOTE: this is checked on most spell casts. prevents doing stupid shit.
 ***|
sub cast_break()
	DEBUG ${cbug}cast_break()\ax
	ISDEAD
	/doevents 
	/if (${Spell[${Me.Casting.ID}].Category.Equal[Transport]}) /goto :exit
	/if (${Spell[${Me.Casting.ID}].Category.Equal[Heal]}) /goto :exit
	
	/if (${Target.Mezzed.ID} || !${Spawn[${Target2Kill}].ID} || !${Bool[${Target2Kill}]}) {
		/stopcast
	} else /if (${Bool[${Me.Song[Gift of Mana (${GoMLevel})]}]} && (${Bool[${GoMNuke}]} || ${Bool[${GoMNuke2}]})) {
		/stopcast
		/call cast_nuke_GoM ${Target2Kill}
	}
	:exit
/return



|***
 *	DES: Cures/removes debuffs, we hope...
 *	USE: netbots are automatic.
 *	INI: 
 *  NOTE: 
 ***|
sub cast_cure()
	DEBUG ${cbug}cast_cure()\ax
	ISDEAD
	/doevents
	UPDATEHUD "CURE"

	| if i'm fine skip to netbots check
	/if (!${Debuff.Counters}) {
		/goto :imfinecheckgroup
	} else /if (${Debuff.Counters}) {
		| cure self only with items
		
		/if (${Select[${Me.Class.ShortName},MNK]}) /call core_cast "Purify Body" alt 0
		
		/if (${Debuff.Cursed} && ${Me.ItemReady[${cureCurseItem}]}) {
			/call core_cast "${cureCurseItem}" item ${Me.ID}
		} else /if (${Debuff.Corrupted} && ${Me.ItemReady[${cureCorruptItem}]}) {
			/call core_cast "${cureCorruptItem}" item ${Me.ID}
		} else /if (${Debuff.Poisoned} && ${Me.ItemReady[${curePoisonItem}]}) {
			/call core_cast "${curePoisonItem}" item ${Me.ID}
		} else /if (${Debuff.Diseased} && ${Me.ItemReady[${cureDiseaseItem}]}) {
			/call core_cast "${cureDiseaseItem}" item ${Me.ID}
		}
	}

	:imfinecheckgroup
	/if (!${Group.Members}) /goto :exit
	/if (!${Bool[${Plugin[MQ2netbots]}]} || !${Bool[${Plugin[MQ2debuffs]}]}) {
		/if (!${Bool[${Plugin[MQ2netbots]}]}) NOUT ${owarning} ${sep} You need the plugn ${cinfo}MQ2netbots\ax for group curing.
		/if (!${Bool[${Plugin[MQ2debuffs]}]}) NOUT ${owarning} ${sep} You need the plugn ${cinfo}MQ2debuffs\ax for group curing.
		/goto :exit
	}

	/declare BcClients ${NetBots.Client}
	/declare _count int local
	/for _count 1 to ${Math.Calc[${NetBots.Client.Count[ ]}+1]}
		ISDEAD
		/doevents 

		| are they debuffed in the zone and alive?
		/if (${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Count} <= 0 || !${Spawn[p${NetBots.Client[${_count}]}].ID} || ${Spawn[pc ${NetBots.Client[${_count}]}].Dead}) /next _count

		| AA Radient Cure 
		| 100' 1 min
		/if (${Select[${Me.Class.ShortName},PAL,SHM,CLR,DRU]} && ${Me.AltAbilityReady[Radiant Cure]}) {
			/if (${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Count}) {
				/if (${Spawn[${NetBots.Client[${_count}]}].Distance} <= ${Spell[radiant cure].MyRange}) /call core_cast "Radiant Cure" alt ${Spawn[pc ${NetBots.Client[${_count}]}].ID} 
			}
			/goto :exit
		}

		| AA Purify Soul
		| 200' 5 min
		/if (${Select[${Me.Class.ShortName},PAL,CLR,DRU]} && ${Me.AltAbilityReady[Purify Soul]}) {
			/if (${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Count}) {
				/if (${Spawn[pc ${NetBots.Client[${_count}]}].Distance} <= ${Spell[Purify Soul].MyRange}) /call core_cast "Purify Soul" alt ${Spawn[pc ${NetBots.Client[${_count}]}].ID} 
			}
			/goto :exit
		}
		
		| AA Group Purify Soul . were in deep shit if we've gotten this far.
		| 0/100' 15 min
		/if (${Select[${Me.Class.ShortName},PAL,CLR,DRU]} && ${Me.AltAbilityReady[Group Purify Soul]}) {
			/if (${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Count}) {
				/if (${Spawn[pc ${NetBots.Client[${_count}]}].Distance} <= 100) /call core_cast "Group Purify Soul" alt ${Me.ID} 
			}
			/goto :exit
		}

		| cure with spell
		/if (${Bool[${cureCurse}]} && ${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Cursed} && ${Spawn[${NetBots.Client[${_count}]}].Distance} <= ${Spell[${cureCurse}].MyRange}) {
			/call core_cast "${cureCurse}" ${buffGem} ${Spawn[pc ${NetBots.Client[${_count}]}].ID} 
		} else /if (${Bool[${cureCorrupt}]} && ${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Corrupted} && ${Spawn[pc ${NetBots.Client[${_count}]}].Distance} <= ${Spell[${cureCorrupt}].MyRange}) {
			/call core_cast "${cureCorrupt}" ${buffGem} ${Spawn[pc ${NetBots.Client[${_count}]}].ID}
		} else /if (${Bool[${curePoison}]} && ${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Poisoned} && ${Spawn[pc ${NetBots.Client[${_count}]}].Distance} <= ${Spell[${curePoison}].MyRange}) {
			/call core_cast "${curePoison}" ${buffGem} ${Spawn[pc ${NetBots.Client[${_count}]}].ID} 
		} else /if (${Bool[${cureDisease}]} && ${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Diseased} && ${Spawn[pc ${NetBots.Client[${_count}]}].Distance} <= ${Spell[${cureDisease}].MyRange}) {
			/call core_cast "${cureDisease}" ${buffGem} ${Spawn[pc ${NetBots.Client[${_count}]}].ID} 
		}
		
	/next _count
	:exit
	UPDATEHUD

	/call create_timer timer_check_cast_cure ${tCast_Cure}
/return



|***
 *	DES:targets and casts a free targeting area spell.
 *	USE: /call cast_free_target ["spell"] [target ID]
 *	INI: 
 *  NOTE: i could never get this to work right...
 ***|
sub cast_free_target(string _tmpSpell, int _tmpID)
	DEBUG ${cbug}cast_free_target()\ax

	/call core_cast "${_tmpSpell}" 0 ${_tmpID}
	/squelch /face ID ${_tmpID} ${If[${SetFaceFast},fast,]}
	/delay 5
	/click left center
	/delay 4s !${Me.Casting.ID}
/return



|***
 *	DES: use base melee discs
 *	USE: /call use_skill_melee
 *	INI: toon -> [meleedisc] set the skill to TRUE to use.
 *  NOTE: 
 ***|
sub use_skill_melee()
 	DEBUG ${cbug}use_skill_melee()\ax
	ISDEAD
	/doevents 
	/call is_target_dead
	/if (${Bool[${Macro.Return}]} || !${Target.ID}) /goto :exit
	/call check_exit TRUE
	/if (${Bool[${Macro.Return}]}) /goto :exit
	
	/if (${Target.ID} && ${Select[${combat},MELEE,RANGE]}) /squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}

	| taunt /setagro
	/if (${Bool[${taunt}]} && ${Bool[${setAgroMode}]} && ${Me.AbilityReady[Taunt]} && ${Spawn[${Target2Kill}].Distance} <= 18 && ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
		VOUT Melee: ${csp}Taunting\ax ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax]
		/if (${Target.ID}) /doability Taunt
	}
	| disarm
	/if (${Bool[${disarm}]} && !${Skill[Disarm].Auto}  && ${Me.AbilityReady[Disarm]} && ${Spawn[${Target2Kill}].Distance} <= 15) {
		VOUT Melee: ${csp}Disarm\ax ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax]
		/if (${Target.ID}) /doability Disarm
	}
	
	/if (${Me.Level} > 61) /goto :exit
	
	| kick
 	/if (!${Select[${Me.Class.ShortName},MNK]} && ${Bool[${kick}]} && !${Skill[Kick].Auto} && ${Me.AbilityReady[Kick]} && ${Spawn[${Target2Kill}].Distance} <= 15) {
		VOUT Melee: ${csp}Kick\ax ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax]
		/if (${Target.ID}) /doability Kick
	}
	| bash
	/if (${Bool[${bash}]} && !${Skill[Bash].Auto} && ${Me.AbilityReady[Bash]} && ${Spawn[${Target2Kill}].Distance} <= 15) {
		VOUT Melee: ${csp}Bash\ax ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax]
		/if (${Target.ID}) /doability Bash
	}
	
	| Zerker only skill.
	/if (${Bool[${frenzy}]} && !${Skill[Frenzy].Auto} && ${Me.AbilityReady[Frenzy]} && ${Spawn[${Target2Kill}].Distance} <= 13) {
		VOUT Melee: ${csp}Frenzy\ax ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax]
		/if (${Target.ID}) /doability Frenzy
	}

	| rogue only skill.
	/if (${Bool[${backstab}]} && !${Skill[Backstab].Auto} && ${Me.AbilityReady[Backstab]} && ${Spawn[${Target2Kill}].Distance} <= 13) {
		VOUT Melee: ${csp}Backstab\ax ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax]
		/if (${Target.ID}) /doability Backstab
	}
	
	| monk only skill 
	/if (${Select[${Me.Class.ShortName},MNK]} && ${Spawn[${Target2Kill}].Distance} <= 15) {
		| fist
		/if (${Bool[${dragonpunch}]} && !${Skill[Dragon Punch].Auto} && ${Me.AbilityReady[Dragon Punch]}) {
			VOUT Melee: ${csp}Backstab\ax ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax]
			/if (${Target.ID}) /doability "Dragon Punch"
		} else /if (${Bool[${eaglestrike}]} && !${Skill[Eagle Strike].Auto} && ${Me.AbilityReady[Eagle Strike]}) {
			VOUT Melee: ${csp}Backstab\ax ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax]
			/if (${Target.ID}) /doability "Eagle Strike"
		} else /if (${Bool[${tigerclaw}]} && !${Skill[Tiger Claw].Auto} && ${Me.AbilityReady[Tiger Claw]}) {
			VOUT Melee: ${csp}Backstab\ax ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax]
			/if (${Target.ID}) /doability "Tiger Claw"
		}
		| kick
	 	/if (${Bool[${kick}]} && !${Skill[Kick].Auto} && ${Me.AbilityReady[Kick]}) {
			VOUT Melee: ${csp}Kick\ax ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax]
			/if (${Target.ID}) /doability Kick
		} else /if (${Bool[${flyingkick}]} && !${Skill[Flying Kick].Auto} && ${Me.AbilityReady[Flying Kick]}) {
			VOUT Melee: ${csp}Kick\ax ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax]
			/if (${Target.ID}) /doability "Flying Kick"
		} else /if (${Bool[${roundkick}]} && !${Skill[Rounf Kick].Auto} && ${Me.AbilityReady[Round Kick]}) {
			VOUT Melee: ${csp}Kick\ax ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax]
			/if (${Target.ID}) /doability "Round Kick"
		}		
	}

	:exit
/return
 


 |***
 *	DES: single swarm pet
 *	USE: /call cast_nuke_servant
 *	INI: na
 *  NOTE: 
 ***|
sub cast_nuke_servant()
	DEBUG ${cbug}cast_nuke_servant()\ax
	ISDEAD
	/doevents
	CHECKEXIT
	/if (${usePetSwarm} && ${Spawn[${Target2Kill}].LineOfSight} && (${Me.SpellReady[${ServantNuke}]} || ${Me.AltAbilityReady[${ServantNuke}]}) && ${Me.CurrentMana} >= ${Spell[${ServantNuke}].Mana} && ${Spawn[${Target2Kill}].Distance} < ${MobAgro}) {
		/if (${Me.SpellReady[${ServantNuke}]}) /call core_cast "${ServantNuke}" 0 ${Target2Kill} cast_break
		/if (${Me.AltAbilityReady[${ServantNuke}]}) /call core_cast "${ServantNuke}" alt ${Target2Kill}
	}
	:exit
/return



|***
 *	DES: preps toon for combat modes
 *	USE: /call set_combat [MELEE|RANGE|NUKE|HEAL]
 *	INI: 
 *  NOTE: 
 ***|
sub set_combat(str _mode)
	DEBUG ${cbug}set_combat(${_mode})\ax
	ISDEAD
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/if (!${AutoBot} && ${Me.Invis}) /makemevisible
	UPDATEHUD "${mode}"
	/if (${Me.State.NotEqual[STAND]}) /stand
	/if (${AutoBot} && ${Me.State.Equal[FEIGN]}) /squelch /stand
	
		
	/if (${Select[${_mode},MELEE]}) {
		/squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
		/if (${AutoBot} && ${Bool[${Target.DSed}]} && ${stoponDS}) {
			/call set_stop FALSE
			/call clear_combat
		} else /if (!${Me.Combat}) {
			/if (${Target.Distance} > 31) /range
			/squelch /attack on
			/if (${Target.Distance} <= 31) {
				/call stick_to_target ${Target2Kill}
			}
		}		
	} else /if (${Select[${_mode},RANGE]}) {
		/squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
		/if ((${Spawn[${Target2Kill}].LineOfSight} && !${setIgnoreLOS}) && !${Me.AutoFire}) /squelch /autofire on
	} else /if (${Select[${_mode},NUKE]}) {
	} else /if (${Select[${_mode},HEAL]}) {
	}
	
	:exit
/return



|***
 *	DES: Gift of Manna Nuke
 *	USE: /call cast_nuke_GoM
 *	INI: Toon -> [Spells] -> gomnuke, gomnuke2 gomlevel
 *  NOTE: 
 ***|
sub cast_nuke_GoM(int _tmpID)
	DEBUG ${cbug}cast_nuke_GoM(ID: ${_tmpID})\ax
	ISDEAD
	/if (!${Select[${Me.Class.ShortName},CLR]}) {
		/call check_exit
		/if (${Bool[${Macro.Return}]}) /goto :exit
		/call is_target_dead
		/if (${Bool[${Macro.Return}]}) /goto :exit
	}

	:loopcasting
	/if (${Me.Casting.ID} && (${Me.Gem[${GoMNuke}]} || ${Me.Gem[${GoMNuke2}]})) {
		/delay 1
		/goto :loopcasting
	}
	
	/if (${Me.SpellReady[${GoMNuke}]}) {
		/target ID ${_tmpID}
		VOUT Dichotomic: ${csp}${GoMNuke}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
		/call MQ2Cast "${GoMNuke}" gem${Me.Gem[${GoMNuke}]} -targetid|${_tmpID}
	} else /if (${Me.SpellReady[${GoMNuke2}]}) {
		/target ID ${_tmpID}
		VOUT Dichotomic Backup: ${csp}${GoMNuke2}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
		/call MQ2Cast "${GoMNuke2}" gem${Me.Gem[${GoMNuke2}]} -targetid|${_tmpID}
	}
		
	:exit
/return



|***
 *	DES: Buff Raid Members in your /agro range
 *	USE: /call check_buffs_raid
 *	INI: uses INI [Buffs] 
 *  NOTE: 
 ***|
sub check_buffs_raid()
	DEBUG ${cbug}check_buffs_raid()\ax
	ISDEAD
	UPDATEHUD "BUFF: Raid"
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit

	| # people in raid
	/declare _i int local
	| # of buffs to cast
	/declare _x int local
	| targets buff count
	/declare _y int local
	
	/if (${Raid.Members} && ${Me.PctMana} > 10 && !${Me.Casting.ID}) {
		/for _i 1 to ${SpawnCount[raid radius ${MobAgro}]}
			DEBUG Checking Raid Buffs on [${cinfo}${Raid.Member[${_i}].DisplayName}\ax]
			/for _x 1 to ${BuffCount}
				ISDEAD
				/doevents
				CHECKTIE
				CLEARCURSOR TRUE
				/if (!${Bool[${timer_Raid_Buffs${Spell[${Buff${_x}}].ID}${Raid.Member[${_i}].ID}}]} && ${RaidBuff${x}Class.Find[${Spawn[${Raid.Member[${_i}].ID}].Class.ShortName}]} && ${Spawn[${Raid.Member[${_i}].ID}].Type.Equal[pc]} && ${Spawn[${Raid.Member[${_i}].ID}].Distance} < ${MobAgro} && ${Me.PctMana} >= 20) {
					/if (${Stick.Active}) /squelch /stick off
					/target ID ${Raid.Member[${_i}].ID}
					/delay 1s ${Target.ID} == ${Raid.Member[${_i}].ID}
					/delay 1s ${Target.BuffCount}
					/delay 1s
					DEBUG X=${_x} class:${RaidBuff${_x}Class.Find[${Spawn[${Raid.Member[${_i}].ID}].Class.ShortName}]} .. spell:${Buff${_x}chk}
					/for _y 1 to ${Target.BuffCount}	
						ISDEAD
						/if (!${Spell[${Target.Buff[${_y}]}].WillStack[${Spell[${Buff${_x}}]}]} && ${Target.Buff[${_y}].ID}) {
							/call create_timer timer_Raid_Buffs${Spell[${Buff${_x}chk}].ID}${Raid.Member[${_i}].ID} ${Spell[${Spell[${Buff${_x}chk}].ID}].Duration.TotalSeconds}s
							/goto :next
						}
					/next _y
					/if (!${Target.Buff[${Buff${_x}chk}].ID}) {
						CLEARCURSOR TRUE
						VOUT Raid Buffing: ${csp}${Buff${_x}}\ax on [${cpc}${Raid.Member[${_i}].DisplayName}\ax]
						/call MQ2Cast "${Buff${x}}" ${buffGem} -targetid|${Raid.Member[${_i}].ID} 
					}
					/call create_timer timer_Raid_Buffs${Spell[${Buff${_x}chk}].ID}${Raid.Member[${_i}].ID} ${Spell[${Spell[${Buff${_x}chk}].ID}].Duration.TotalSeconds}s
				}
				:next
			/next _x	
		/next _i
	}
	:exit
	/call create_timer timer_check_buffs_raid ${tRaid_Buff}
	UPDATEHUD
/return



|***
 *	DES: preps the queue with target toon
 *	USE: /call cast_buffs_queue [toon name] [buff word]
 *	INI: 
 *  NOTE: need to remome the pet crap someday...
 ***|
sub cast_buffs_queue(string _sender, string _verbage)
	DEBUG ${cbug}cast_buffs_queue()\ax

  /declare _a        int local
  /declare _i        int local

  /for _i 1 to ${BuffCount}
    /for _a 1 to ${Buff${_i}Alias.Count[|]}
    	ISDEAD
   		/doevents
      /if (${_verbage.Find["${Buff${_i}Alias.Arg[${_a},|]}"]}) {
        /if (${_verbage.Find["pet"]} || ${_verbage.Find[${Spawn[pc ${_sender}].Pet.DisplayName}]} || ${Spawn[pet ${_sender}].ID}) {
          VOUT Buffing ${cpc}${_sender}'s\ax Pet.
          /call queue_buff_add ${Spawn[pc ${_sender}].Pet.ID} ${_i}
        } else {
          DEBUG ${_sender} requested ${cinfo}${Buff${_i}}\ax
          /call queue_buff_add ${Spawn[pc ${_sender}].ID} ${_i}
        }
      }
    /next _a
  /next _i
  :exit
/return



 |***
 *	DES: will loosly tie to a specific toon. will moove close to the toon when it moves. set to timer [Timers] -> CheckTieTime
 *	USE: /tie [on/off toggle] requires /tietoon [NAME] and /tiedistance ##
 *	INI: 
 *  NOTE: 
 ***|
sub set_tie()
	DEBUG ${cbug}set_tie()\ax
	/if (${Bool[${timer_check_Tie}]}) /goto :exit
	ISDEAD
	/doevents
	| reasons to not tie to a toon
	/if (${SetTie} && ${Spawn[pc ${SetTieToon}].ID} && ${Spawn[pc ${SetTieToon}].Dead}) /goto :exit
	/if (${Spawn[pc ${SetTieToon}].Distance} <= ${SetTieDistance} || !${Spawn[pc ${SetTieToon}].ID}) {
		/if (${Stick.Active}) /stick off
		/if (${Me.Moving}) {
			/keypress forward
			/keypress back
		}
		/goto :exit
	}
	
	| turn off camp
	/if (${Bool[${MakeCamp}]}) {
		/makecamp off
		/call Event_SetVar NA HomeSet Movement FALSE 
	}
	
	| all present and or accounted for SIR!
	/squelch /face ID ${Spawn[pc ${SetTieToon}].ID} ${If[${SetFaceFast},fast,]}
	/squelch /stick ID ${Spawn[pc ${SetTieToon}].ID} ${SetTieDistance} UW

	:okweremoving
	ISDEAD
	/doevents
	/if (${Spawn[pc ${SetTieToon}].Distance} <= ${SetTieDistance}) {
		/stick off
		/goto :exit
	}
	| /delay 2 ${Stick.Stopped}
	
	/if (!${Stick.Stopped} && ${Spawn[pc ${SetTieToon}].ID}) {
		/goto :okweremoving
	} else {
		/if (${Stick.Active}) /stick off
		/if (${Me.Moving}) {
			/keypress forward
			/keypress back
		}
	}

	:exit
	/if (${AutoBot}) /call create_timer timer_check_Tie ${tCheck_Tie}
/return



 |***
 *	DES: Will sort out your familiar.
 *	USE: auto
 *	INI: 
 *  NOTE: /if (${Me.Pet.Body.ID}==31) /pet get lost
 ***|
sub sort_familiar()
	DEBUG ${cbug}sort_Familiar()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]} || !${Bool[${sumFamiliar}]}) /goto :exit
	/if (!${Bool[${Me.Buff[${sumFamiliarBuff}].Duration}]}) {
		/call core_cast "${sumFamiliar}" NULL NULL
		/goto :exit
	}
	/if (${FamiliarLeave} && ${Me.Pet.Name.Find[familiar]} && ${Me.Pet.Body.ID} == 31) /pet get lost
	:exit
	/call create_timer timer_Check_Familiar ${tCheck_Familiar}
/return



|***
 *	DES: Adjusts the XTarget for visual purposes to heal toonControl. *NO EFFECT ON HEALING
 *	USE: /call protect_always_xt & /control [toon name]
 *	INI: auto if toonControlXTslot is not FALSE in INI
 *  NOTE: 
 ***|
sub protect_always_xt()
	DEBUG ${cbug}protect_always_xt()\ax
	/doevents
	/if (${Bool[${toonControl}]} && ${Bool[${toonControlXTslot}]} && ${toonControlProtect} && ${Bool[${Spawn[pc ${toonControl}].ID}]} && !${Bool[${Spawn[${toonControl}].Dead}]}) {
		/if (${Group.Member[${toonControl}].ID}) {
			/xtarget set ${toonControlXTslot} Autohater
		} else /if (!${Group.Member[${toonControl}].ID}) {
			/xtarget set ${toonControlXTslot} ${toonControl}
		}
	} else /if ((!${Bool[${toonControl}]} || !${Bool[${Spawn[pc ${toonControl}].ID}]}) && ${Bool[${toonControlXTslot}]}) {
		/xtarget set ${toonControlXTslot} Autohater
	}
	:exit
/return



 |***
 *	DES: check and claim Free Station Cash
 *	USE: manual
 *	Credit: v1.0 - Initial release on Macroquest VIP forums - Dewey2461 on 7-7-2014 [butchered by me]
 *	INI: 
 *  NOTE: 
 ***|
sub check_free_ingame_cash()
	DEBUG ${cbug}check_free_ingame_cash()\ax
	/doevents
	/declare _claim_match 			string local Date this reward expires:
	/declare _current_funds 		string local 0
	/declare _claim_desc 				string local NULL
	| /if (${Time.Date.Equal[${Ini[${INIFile},Common,InGameCashChecked,"01/01/2000"]}]}) /goto :exit
	/delay 1
	/if (!${Window[MarketplaceWnd]}) {
		/market
		/delay 10s ${Window[MarketplaceWnd]}
		/delay 10s ${Window[MKPW_ClaimWindow].Child[MKPW_ClaimDescription].Text.Length}
	}

	/varset _current_funds ${Window[MarketplaceWnd].Child[MKPW_AvailableFundsUpper].Text}
	/varset _claim_desc ${Window[MKPW_ClaimWindow].Child[MKPW_ClaimDescription].Text}

	/if (${_claim_desc.Mid[1,25].NotEqual[${_claim_match}]}) {
		VOUT ${onotice} ${sep} No free in game cash [${cinfo}${_current_funds}\ax]
	} else {
		/notify MKPW_ClaimWindow MKPW_ClaimClickHereBtn leftmouseup
		/delay 1s
		/delay 10s ${Window[MarketplaceWnd].Child[MKPW_AvailableFundsUpper].Text.NotEqual[${_current_funds}]}

		/varset _current_funds ${Window[MarketplaceWnd].Child[MKPW_AvailableFundsUpper].Text}
		VOUT You have collected your free in game cash [${cinfo}${_current_funds}\ax]   
	}
	/market
	/ini ${INIFile} "Common" "InGameCashChecked" "${Time.Date}"

	:exit   
/return



 |***
 *	DES: will click items and autoinventory the result on a timer.
 *	USE: /call odds_and_ends
 *	INI: [Odds]
 *  NOTE: good for poisons, foods, whatevers.
***|
sub odds_and_ends()
	DEBUG ${cbug}odds_and_ends(${doclickOdds})\ax
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/declare _count int local
	/for _count 1 to ${doclickOdds}
		CLEARCURSOR TRUE
		/doevents
		ISDEAD
		/if (${Me.ItemReady[${clickOddItem${_count}}]}) /call core_cast "${clickOddItem${_count}}" item 0
	/next _count
	:exit
	/call create_timer timer_check_odds_and_ends ${tOdds_and_Ends}
	CLEARCURSOR TRUE
/return



 |***
 *	DES: If the followng criteria are met, the calling function will be skipped.
 *	USE: /call check_exit [TRUE|NULL] 
 *	INI: 
 *  NOTE: All reasons to not attack a target..
 ***|
sub check_exit(bool _mobs)
	| DEBUG ${cbug}check_exit(\ax${cinfo}${If[${Bool[${_mobs}]},TRUE,FALSE]}\ax${cbug})\ax
	/doevents
	ISDEAD
	/declare _exit 			bool 		local FALSE
	
	| movement
	/if ((${Me.Moving} || ${Stick.Active}) && !${Select[${Me.Class.ShortName},BRD]}) /varset _exit TRUE
	/if (${Bool[${_exit}]}) /goto :exit
	
	| invis 
	/if (${Me.Invis}) /varset _exit TRUE
	/if (${Bool[${_exit}]}) /goto :exit

	| clear ToTD if it procs.
	/if (${AutoBot} && ${Select[${Me.Class.ShortName},CLR,SHM,BER]}) {
		/if (${Me.Song[Touch of the Divine].ID}) {
			/varset _exit FALSE
			/delay 3s
			/removebuff Touch of the Divine
			ECHOCHANNEL REMOVING .. Touch of the Divine
		}
	}


	| SELF BUFFS/SONGS
	/if (${Me.Song[Divine Aura].ID}) /varset _exit TRUE
	/if (${Me.Song[Bestowed Divine Aura Beza].ID} || ${Me.Song[Bestowed Divine Aura Azia].ID}) /varset _exit TRUE
	/if (${Me.Buff[Death Overcomes].ID}) /varset _exit TRUE
	/if (${Me.Buff[Fermata of Preservation].ID}) /varset _exit TRUE
	/if (${Me.Buff[Kazumi's Note of Preservation].ID}) /varset _exit TRUE
	/if (${Bool[${_exit}]}) /goto :exit

	| BAD BUFFS
	/if (${Me.Buff[Protection of Sul].ID}) /varset _exit TRUE
	/if (${Me.Buff[Bewilder].ID}) /varset _exit TRUE
	/if (${Me.Buff[Chaotic Bewildering].ID}) /varset _exit TRUE
	/if (${Bool[${_exit}]}) /goto :exit
	
	| SELF STATES
	/if (${Me.State.Equal[FEIGN]}) /varset _exit TRUE
	/if (${Me.Ducking}) /varset _exit TRUE
	/if (${Me.Stunned}) /varset _exit TRUE 
	/if (${Me.Charmed.ID}) /varset _exit TRUE
	/if (${Bool[${_exit}]}) /goto :exit
	
	| check for ${noCastWith} buffs on self and target
	/declare _count 		int 		local
	/for _count 1 to ${noCastWith.Count[|]}
		/if (${Bool[${Me.Buff[${noCastWith.Arg[${_count},|]}]}]} || ${Bool[${Target.Buff[${noCastWith.Arg[${_count},|]}]}]} || ${Bool[${Me.Song[${noCastWith.Arg[${_count},|]}]}]}) /varset _exit TRUE
		/if (${Bool[${_exit}]}) /goto :exit
	/next _count	


	| forced target checking. passed bool _mobs
	/if (${Bool[${_mobs}]}) {
		| TARGET BUFFS 
		/if (${AutoBot} && ${Bool[${NoCastList.Find[${Target.DisplayName}]}]}) /varset _exit TRUE 
		/if (${AutoBot} && ${Bool[${Target.Mezzed}]} && !${setPullMode}) /varset _exit TRUE 	
		
		| TARGET DEAD
		| /if (${Bool[${Spawn[${Target.ID}].Type.Equal[CORPSE]}]}) /varset _exit TRUE
		| /if (${Bool[${Spawn[${Target.ID}].Dead}]}) /varset _exit TRUE
	}

	:exit
	DEBUG ${cbug}check_exit: ${cinfo}${_exit}\ax
/return ${Bool[${_exit}]}



 |***
 *	DES: Updates Notice sction of HUD.
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub update_HUD_notices(string _mode)
	DEBUG ${cbug}update_HUD_notices()\ax
	/declare _count 				int local
	/declare _index 				int local
	/declare _sep 					bool local
	/declare _IV[3] 				string local
	/declare _IVU[1]				string local
	
	/varset hudNotice
	/varset hudWarning
	/varset hudTarNotice

	/if (!${timer_check_status}) /call broadcast_status
	
	
	/if (${Bool[${_mode}]}) {
		/varset tmpMode "${_mode}"
	} else /if (!${Bool[${_mode}]}) {
		/varset tmpMode FALSE
	}

	| ** Notices for bottom of HUD	
	| DEBUG HUD: Bottom Notices
	/if (${Select[${Me.Class.ShortName},BRD]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} ${songList}
		/varset _sep TRUE
	}	
	/if (${AdvPath.Playing}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} AdvPath
		/varset _sep TRUE
	}
	/if (${Bool[${SafeZone}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} SafeZone
		/varset _sep TRUE
	}	
	/if (${Stick.Active}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} Stick
		/varset _sep TRUE
	}
	/if (${Me.AutoFire}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} AutoFire
		/varset _sep TRUE
	}
	/if (${Me.Combat}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} Melee
		/varset _sep TRUE
	}
	/if (${Bool[${useRain}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} AE
		/varset _sep TRUE
	}	
	/if (${Bool[${useQM}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} QM
		/varset _sep TRUE
	}	
	/if (${Bool[${useMG}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} MG
		/varset _sep TRUE
	}			
		
	
	| Mode Notice
	| DEBUG HUD: Mode Notice
	/if (${Bool[${setModeHunt}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} MODE: Hunt - ${If[${setModeAgro},${setModeAgro},Zone]}
		/varset _sep TRUE
	}		
	/if (${Bool[${setModeScorch}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} MODE: Scorched Earth - ${If[${setModeAgro},${setModeAgro},Zone]}
		/varset _sep TRUE
	}		
	/if (${Bool[${setModeDrag}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} MODE: Corpse Drag
		/varset _sep TRUE
	}
	|***
	/if (${Bool[${setModeForage}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} MODE: Forage
		/varset _sep TRUE
	}
	/if (${Bool[${setModeFish}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} MODE: Fish
		/varset _sep TRUE
	}
	/if (${Bool[${setModePetFarm}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} MODE: Pet Farm
		/varset _sep TRUE
	}
	/if (${Bool[${setModeSkill}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} MODE: Skill Trainer
		/varset _sep TRUE
	}		
	***|
		
	
	| WARNINGS FOR TOP OF HUD.. USE THIS SPARINGLY
	| DEBUG HUD: WARNING
	/if (${Bool[${debug}]}) {
		/varset hudWarning ${hudWarning} * D E B U G * 
		/goto :exitWARNING
	}	
	/if (${Bool[${flagCallNeedRez}]}) {
		/varset hudWarning ${hudWarning} * STILL NEED A REZ * 
		/goto :exitWARNING
	}
	
	| check for invis
	| still missing some
	/varset _sep FALSE
	| /varset _haveIV FALSE
	| /varset _haveIVU FALSE
	
	/varset _IV[1] |Shared Perfected Camouflage|Shared Camouflage|Camouflage|Group Perfected Invisibility|Invisibility|Group Perfected Spirit Walk Invisibility|Perfected Invisibility
	/varset _IV[2] |Phase March|Nethervoid March|Perfected Silent Presence|Cloak of Shadows|Cloak of Shadows I|Cloak of Shadows II|Shauri's Sonorous Clouding|Gather Umbra|Invisibility Cloak
	/varset _IV[3] |Perfected Natural Invisibility

	/for _count 1 to ${_IV.Size}
		/for _index 1 to ${_IV[${_count}].Count[|]}
			/if (${Me.Buff[${_IV[${_count}].Arg[${_index},|]}].Name.Equal["${_IV[${_count}].Arg[${_index},|]}"]}) {
				| /varset _haveIV TRUE
				/varset hudWarning ${hudWarning} IV
				/varset _sep TRUE
			}
		/next _index	
	/next _count

	| check for invis vs undead
	/varset _IVU[1] |Group Perfected Invisibility to Undead|Perfected Invisibility to Undead|Invisibility to Undead|Invisibility versus Undead

	/for _count 1 to ${_IVU.Size}
		/for _index 1 to ${_IVU[${_count}].Count[|]}
			/if (${Me.Buff[${_IVU[${_count}].Arg[${_index},|]}].Name.Equal["${_IVU[${_count}].Arg[${_index},|]}"]}) {
				/varset hudWarning ${hudWarning} IVU 
				/varset _sep TRUE
			}
			/next _index	
	/next _count


	/if (${Bool[${setIgnoreLOS}]}) {
		/varset hudWarning ${hudWarning} LoS 
		/varset _sep TRUE
	}	

	:exitWARNING

	| ** target debuffs notices
	/varset _sep FALSE

	/if (${Bool[${Spawn[${Target.ID}].Master}]}) {
		/varset hudTarNotice ${hudTarNotice} ${If[${_sep},-,]} Pet
		/varset _sep TRUE
	}	

	/if (${Bool[${Target.Maloed}]}) {
		/varset hudTarNotice ${hudTarNotice} ${If[${_sep},-,]} Malo
		/varset _sep TRUE
	}	

	/if (${Bool[${Target.Tashed}]}) {
		/varset hudTarNotice ${hudTarNotice} ${If[${_sep},-,]} Tash
		/varset _sep TRUE
	}	
	/if (${Bool[${Target.Snared}]}) {
		/varset hudTarNotice ${hudTarNotice} ${If[${_sep},-,]} Snare
		/varset _sep TRUE
	}	
	/if (${Bool[${Target.Buff[Mental Contortion]}]}) {
		/varset hudTarNotice ${hudTarNotice} ${If[${_sep},-,]} MC
		/varset _sep TRUE
	}	
	/if (${Bool[${Target.Slowed}]}) {
		/varset hudTarNotice ${hudTarNotice} ${If[${_sep},-,]} Slow
		/varset _sep TRUE
	}	
	/if (${Bool[${Target.DSed}]}) {
		/varset hudTarNotice ${hudTarNotice} ${If[${_sep},-,]} DS
		/varset _sep TRUE
	}	
	:exit	
/return




|***
 *	DES: AA Slow
 *	USE: /call cast_AA_slow [MOBID] [AA SLOW NAME]
 *	INI: 
 *  NOTE: 
 ***|
sub cast_AA_slow(int _tmpID, string _tmpSpell)
	DEBUG ${cbug}cast_AA_slow(${_tmpID}, ${_tmpSpell})\ax
	ISDEAD
	/doevents
	/call check_exit TRUE
	/if (${Bool[${Macro.Return}]} || ${timer_slow_${_tmpID}}) /goto :exit

	| check for overwrite slows
	/declare _tmpSlow string local ${Target.Slowed}
	/if (${Target.Slowed.Duration}) {
		/if (${_tmpSlow.Find[Undermining]}) /call create_timer timer_slow_${_tmpID} ${Target.Slowed.Duration.TotalSeconds}s
		/goto :exit
	}

	/declare _resistCount int local 0
	:loopslow
	ISDEAD
	/if (${Spawn[${_tmpID}].Type.NotEqual[PC]} && ${Spawn[${_tmpID}].Distance} <= ${Spell[${_tmpSpell}].MyRange}) {
		/delay 2.25s ${Me.AltAbilityReady[${_tmpSpell}]}
		/call core_cast "${_tmpSpell}" alt ${_tmpID}
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_slow_${_tmpID} ${Spell[${_tmpSpell}].Duration.TotalSeconds}s
		} else /if (${Select[${castReturn},CAST_RESISTED,CAST_RESIST,CAST_RESIST]}) {
			/varcalc _resistCount ${_resistCount}+1
			/if (${_resistCount} >= 3) {
				/call create_timer timer_slow_${_tmpID} ${Spell[${_tmpSpell}].Duration.TotalSeconds}m
			} else {
				/goto :loopslow
			}
		}
	}
	:exit

/return



|***
 *	DES: will cast Temp pet
 *	USE: /call cast_temp_pet [MOBID] [Temp Spell Name]
 *	INI: 
 *  NOTE: 
 ***|
sub cast_temp_pet(int _tmpID, string _tmpSpell)
	DEBUG ${cbug}cast_temp_pet(${_tmpID}, ${_tmpSpell})\ax
	ISDEAD
	/doevents
	CHECKTIE
	/call check_exit
	/if (${Bool[${Macro.Return}]} || !${usePet} || ${Me.Pet.ID}) /goto :exit
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/if (${Spawn[${_tmpID}].Distance} < ${MobAgro} && ${Spawn[${_tmpID}].PctHPs} <= ${sendPetPerc}) {
		VOUT Sending ${cpc}${_tmpSpell}\ax after [${If[${Bool[${Target.ID}]},${ctar}${Target.DisplayName}\ax,${cbad}BAD TARGET\ax]}] ${Target.Distance} ${If[${Spawn[${Target}].LineOfSight},${cgood}LoS\ax,${cbad}LoS\ax]}
		/call MQ2Cast "${_tmpSpell}" -targetid|${_tmpID}
	}
	:exit
/return



|***
 *	DES: Cleric/Paladin AA Yaulp
 *	USE: /call cast_AA_yaulp
 *	INI: [selfbuff] - doYaulp
 *  NOTE: if doYaulp is TRUE, will use at 98% manna or less.
 ***|
sub cast_AA_yaulp()
	DEBUG ${cbug}cast_AA_yaulp()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/if (!${Spell[Yaulp (Lesser)].Stacks} || ${Bool[${Me.Buff[Lesser Yaulp]}]} || ${Bool[${Me.Buff[Yaulp]}]}) /goto :exit
	/if (${Me.PctMana} < 98*.${tmpHealOverride}) /call core_cast "Yaulp (Lesser)" alt 0
	:exit
/return



|***
 *	DES: builds your group
 *	USE: /crew [main|main2|...] {you can rename these} 
 *	INI: core.ini [group] - main|idiotgroup|rejects|....
 *  NOTE: you can rename the groups. i suggest using main as your primary. it is referenced elsewhere.
 *	/grouproles set [NAME] [#]
 *	1=MainTank .. 					${GroupTank}
 *	2=MainAssist ..		 			${GroupAssist}
 *	3=Puller .. 						${GroupPuller}
 *	4=MarkNpc .. 						${GroupMark}
 *	5=MasterLooter .. 			${GroupLooter}
 *	/makeleader [NAME] .. 	${GroupLeader}
 ***|
sub set_crew(string _crewNumber)
 	DEBUG ${cbug}set_crew(${Ini[${INICore},Group,_crewNumber]})\ax

	/declare _count 				int local
	/declare _clist 				string local ${Ini[${INICore},Group,${_crewNumber}]}
	
	| drop raids and groups for a fresh start	
	/if (${Raid.Members}) {
		/bcaa //raiddisband
		/delay 2s ${Raid.Members} == 0
		/delay 1s
	}
	/if (${Group.GroupSize}) {
		/bcaa //disband
		/delay 2s !${Bool[${Group.GroupSize}]}
		/delay 1
	}

	| invite the members in the list...
	/for _count 1 to ${_clist.Count[|]}
		ISDEAD
		/if (!${Bool[${Group.Member[${_clist.Arg[${_count},|]}]}]}) {
			NOUT Inviting ${cpc}${_clist.Arg[${_count},|]}\ax
			/invite ${_clist.Arg[${_count},|]}
		}	
	/next _count	
	/delay 1s
	/delay 12s ${Group.GroupSize} == ${_clist.Count[|]}
	/if (${Group.GroupSize} != ${_clist.Count[|]}) {
		VOUT ${owarning} ${sep} Group failed to form properly. Try again.
		/goto :exit
	}
	
	| set Group Main Tank
	/if (${Bool[${GroupTank}]} && !${Group.Member[${GroupTank}].Offline}) {
		/delay 5
		/if (${Group.Member[${GroupTank}].Index}) /grouproles set ${GroupTank} 1
	}
	
	| set Group Main Assist
	/if (${Bool[${GroupAssist}]} && !${Group.Member[${GroupAssist}].Offline}) {
		/delay 5
		/if (${Group.Member[${GroupAssist}].Index}) /grouproles set ${GroupAssist} 2
	}
	
	| set Group Puller
	/if (${Bool[${GroupPuller}]} && !${Group.Member[${GroupPuller}].Offline}) {
		/delay 5
		/if (${Group.Member[${GroupPuller}].Index}) /grouproles set ${GroupPuller} 3
	}
	
	| set Group MarkNPC
	/if (${Bool[${GroupMark}]} && !${Group.Member[${GroupMark}].Offline}) {
		/delay 5
		/if (${Group.Member[${GroupMark}].Index}) /grouproles set ${GroupMark} 4
	}
	
	| set Group Master Looter
	/if (${Bool[${GroupLooter}]} && !${Group.Member[${GroupLooter}].Offline}) {
		/delay 5
		/if (${Group.Member[${GroupLooter}].Index}) /grouproles set ${GroupLooter} 5
	}

	| set Group leader ... last to make sure all roles are set
	/if (${Bool[${GroupLeader}]} && !${Group.Member[${GroupLeader}].Offline}) {
		/delay 5
		/if (${Group.Member[${Group.Leader}].Index} != ${Group.Member[${GroupLeader}].Index}) /makeleader ${GroupLeader}
	}
	:exit
/return



|***
 *	DES: uses melee disc breather
 *	USE: /call cast_disc_breather
 *	INI: 
 *  NOTE: 
 ***|
sub cast_disc_breather()
 	DEBUG ${cbug}cast_disc_breather(${Me.CombatAbilityReady[${DiscRest}]})\ax
	ISDEAD
	/doevents

 	/if (${SafeZone} || !${Me.CombatAbilityReady[${DiscRest}]}) /goto :exit
	
 	/declare _endurancePoint		int local 0
	/if (${Me.CombatAbility[Breather Rk. III]}) /varset _endurancePoint 29
	/if (${Me.CombatAbility[Breather Rk. II]}) /varset _endurancePoint 25
	/if (${Me.CombatAbility[Breather]}) /varset _endurancePoint 21

 	/if (${Me.PctEndurance} < ${_endurancePoint} && ${Select[${Me.CombatState},RESTING]} && !${Bool[${Me.ActiveDisc}]}) {
		/call core_cast "${DiscRest}" disc 0
	}

	:exit
/return



|***
 *	DES: corrects the Gem## to simply a #
 *	USE: /call fix_gem_id [gem##]
 *	INI: 
 *  NOTE:
 ***|
sub fix_gem_id(string _spellGem)
	DEBUG ${cbug}fix_gem_id(fix: ${_spellGem})\ax
	/declare _gem int local 0
	/if (${_spellGem.Length} == 1) /varset _gem ${_spellGem.Right[1]}
	/if (${_spellGem.Length} == 2) /varset _gem ${_spellGem.Right[2]} 
	/if (${_spellGem.Length} == 4) /varset _gem ${_spellGem.Right[1]}
	/if (${_spellGem.Length} == 5) /varset _gem ${_spellGem.Right[2]}
	DEBUG fix_gem_id(return: ${_gem})
/return ${_gem}



|***
 *	DES: Memorize a spell/song
 *	USE: /call scribe [spell name] [gem to mem it in] [TRUE|FALSE wait for recycle to cast]
 *	INI: 
 *  NOTE:
 ***|
sub scribe(string _spellname, string _spellgem, bool _wait)
 	DEBUG ${cbug}scribe(${_spellname}, ${_spellgem}, ${_wait})\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/call fix_gem_id "${_spellgem}"
	/declare _gem			int local ${Macro.Return}
	/if (!${Me.Book[${_spellname}]}) {
		/if (!${Me.Book[${_spellname}]} && !${timer_scribe_timer}) {
			/call create_timer timer_scribe_timer 60s
			VOUT You don't have ${csp}${_spellname}\ax in your book. Check INI.
		}
		/goto :exit
	}

	/if (${Me.Gem[${_gem}].Name.Equal[${Spell[${_spellname}].RankName}]}) /goto :exit
	VOUT Scribe: ${csp}${Spell[${_spellname}].RankName}\ax
	DEBUG ${_gem} "${Spell[${_spellname}].RankName}"
	/memspell ${_gem} "${Spell[${_spellname}].RankName}"
	/if (${_wait}) {
		:waitforgem
		/delay 10s ${Me.SpellReady[${_spellname}]}
		/delay 4
		/if (!${Me.SpellReady[${_spellname}]}) /goto :waitforgem
	} else /if (!${_wait}) {
		/delay 3s
	}
	
	:exit
/return



|***
 *	DES: sends a tell to the mages in the ResidentMage for shit you need.
 *	USE: /call tell_resident_mage [shard|weapons]
 *	INI: [common] -> ResidentMage
 *  NOTE: This will send the same tell to all mages in the list. welcome to ignore
 ***|
sub tell_resident_mage(string _request)
 	DEBUG ${cbug}tell_resident_mage("${_request}")\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]} || ${Select[${Me.Class.ShortName},MAG]}) /goto :exit
	/declare _count int local 0
	/call create_timer timer_ask_for_rod ${tAsk_for_rod}
	
	| cycle through the mages
	/for _count 1 to ${ResidentMage.Count[|]}

		/if (${Spawn[pc ${ResidentMage.Arg[${_count},|]}].ID} && ${Spawn[pc ${ResidentMage.Arg[${_count},|]}].Distance} < 150) {

			| ask for weapons
			/if (${_request.Equal[weapons]} && !${Bool[${timer_ask_for_weapons${Spawn[pc ${ResidentMage.Arg[${_count},|]}].ID}}]} && !${Bool[${FindItem[${summonedWepClosedBag}]}]}) {
				DEBUG Resident Mage: ${ResidentMage.Arg[${_count},|]} timer: timer_ask_for_weapons${Spawn[pc ${ResidentMage.Arg[${_count},|]}].ID}
				/t ${ResidentMage.Arg[${_count},|]} weapons
				/call create_timer timer_ask_for_weapons${Spawn[pc ${ResidentMage.Arg[${_count},|]}].ID} ${tAsk_for_weapons}
			}
			
			| ask for ModRod - shard
			/if (${_request.Equal[shard]} && !${Bool[${timer_ask_for_rod${Spawn[pc ${ResidentMage.Arg[${_count},|]}].ID}}]} && !${Bool[${FindItem[Modulation Shard]}]}) {
				DEBUG Resident Mage: ${ResidentMage.Arg[${_count},|]} timer: timer_ask_for_rod${Spawn[pc ${ResidentMage.Arg[${_count},|]}].ID}
				/t ${ResidentMage.Arg[${_count},|]} shard
				/call create_timer timer_ask_for_rod${Spawn[pc ${ResidentMage.Arg[${_count},|]}].ID} ${tAsk_for_rod}
				
			}
		}
	/next _count	
	:exit
/return



|***
 *	DES: Res routines for the healer slobs.
 *	USE: /call check_for_corpse
 *	INI: 
 *  NOTE: looks for corpses.. rez them
 ***|
sub check_for_corpse()
	DEBUG ${cbug}check_for_corpse()\ax
	ISDEAD
	/if (${Me.PctMana} < 12) /goto :exit
	/if (${rezEmergency} && (!${Me.ItemReady[Token of Resurrection]} || ${Spawn[pc cleric radius 110].ID} || ${SafeZone})) /goto :exit
	
	| ${RezerList.Find[${Target.DisplayName}]}
	
	/declare _count 						int 		local
	/declare _tmpID 						int 		local 0
	/declare _divinerezCLR			string	local "Divine Resurrection"
	/declare _rezCLR						string	local "Blessing of Resurrection"
	/declare _rezDRU						string	local "Call of the Wild"
	/declare _rezSHM						string	local "Call of the Wild"
	/declare _rezPAL						string	local "Restore Life"
	/declare _rezEmergency			string	local "Token of Resurrection"
	
	| work in druid Rejuvenation of spirit. OOC 20 sec recast 96% rez

	| do we want to rez
	/if ((${rezAuto} || ${rezEmergency}) && ${SpawnCount[pccorpse radius 110]}) {
		/if (${Bool[${rezCombat}]} || ${rezEmergency}) {
			/if (${rezEmergency} || ${Me.AltAbilityReady[${_rez${Me.Class.ShortName}}]}) /goto :checkforcorpses
		} else /if (!${Bool[${rezCombat}]} && !${SpawnCount[npc radius ${combatRadius} playerstate 4]}) {
			/if (${Me.AltAbilityReady[${_rez${Me.Class.ShortName}}]}) /goto :checkforcorpses
		} 
	} else /if ((!${rezAuto} && !${rezEmergency}) || !${SpawnCount[pccorpse radius 110]}) {
		/goto :exit
	}

	:checkforcorpses
	UPDATEHUD "REZZING"
	| clerics first group or raid
	/if (${SpawnCount[pccorpse cleric raid radius 110]} && ${Raid.Members} || ${SpawnCount[pccorpse cleric group radius 110]} && ${Group.Members}) {
		/for _count 1 to ${SpawnCount[pccorpse cleric raid radius 110]}
		ISDEAD
		/if (!${timer_corpse_${NearestSpawn[${_count},pccorpse cleric raid radius 110].ID}}) {
			/varset _tmpID ${NearestSpawn[${_count},pccorpse cleric raid radius 110].ID}
			/if (${DivineRezCleric} && ${Me.AltAbilityReady[${_divinerezCLR}]}) {
				/call cast_rez ${_tmpID} ${_divinerezCLR}		
			} else /if (${Me.AltAbilityReady[${_rez${Me.Class.ShortName}}]}) {
				/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
			} else /if (${rezEmergency} && ${RezTokenList.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
				/call cast_rez ${_tmpID} ${_rezEmergency}
			}
		}
		/next _count
	} 
	
	| control toon
	/if (${SpawnCount[pccorpse radius 110 ${toonControl}]}) {
		/for _count 1 to ${SpawnCount[pccorpse radius 110 ${toonControl}]}
		ISDEAD
		/if (!${timer_corpse_${NearestSpawn[${_count},pccorpse radius 110 ${toonControl}].ID}}) {
			/varset _tmpID ${NearestSpawn[${_count},pccorpse radius 110 ${toonControl}].ID}
			/if (${rezEmergency}) {
				/call cast_rez ${_tmpID} ${_rezEmergency}
			} else {
				/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
			}
		}
		/next _count
	}
	
	| tanks
	/if (${SpawnCount[pccorpse raid radius 110 tank]} && ${Raid.Members}) {
		/for _count 1 to ${SpawnCount[pccorpse raid radius 110 tank]}
		ISDEAD
		/if (!${timer_corpse_${NearestSpawn[${_count},pccorpse raid radius 110 tank].ID}}) {
			/varset _tmpID ${NearestSpawn[${_count},pccorpse raid radius 110 tank].ID}
			/if (${rezEmergency} && ${RezTokenList.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
				/call cast_rez ${_tmpID} ${_rezEmergency}
			} else {
				/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
			}
		}
		/next _count
	} 
	
	| remaining raid members
	/if (${SpawnCount[pccorpse raid radius 110]} && ${Raid.Members}) {
		/for _count 1 to ${SpawnCount[pccorpse raid radius 110]}
		ISDEAD
		/if (!${timer_corpse_${NearestSpawn[${_count},pccorpse raid radius 110].ID}}) {
			/varset _tmpID ${NearestSpawn[${_count},pccorpse raid radius 110].ID}
			/if (${rezEmergency} && ${RezTokenList.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
				/call cast_rez ${_tmpID} ${_rezEmergency}
			} else {
				/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
			}
		}
		/next _count
	} 
	
	| guildies - all the time
	/if (${SpawnCount[pccorpse radius 110 guild]} && !${Raid.Members}) {
		/for _count 1 to ${SpawnCount[pccorpse radius 110 GUILD]}
		ISDEAD
		/if (!${timer_corpse_${NearestSpawn[${_count}, pccorpse  radius 110 guild].ID}}) {
			/varset _tmpID ${NearestSpawn[${_count}, pccorpse radius 110 guild].ID}
			/if (${rezEmergency} && ${RezTokenList.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
				/call cast_rez ${_tmpID} ${_rezEmergency}
			} else {
				/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
			}
		}
		/next _count
	}
	
	| group members out of raid
	/if (${SpawnCount[pccorpse radius 110 group]} && !${Raid.Members}) {
		/for _count 1 to ${SpawnCount[pccorpse radius 110 group]}
		ISDEAD
		/if (!${timer_corpse_${NearestSpawn[${_count}, pccorpse  radius 110 group].ID}}) {
			/varset _tmpID ${NearestSpawn[${_count}, pccorpse radius 110 group].ID}
			/if (${rezEmergency} && ${RezTokenList.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
				/call cast_rez ${_tmpID} ${_rezEmergency}
			} else {
				/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
			}
		}
		/next _count
	}	

	| outside guild and raid
	| needs /rezeveryone switch to be true
	/if (${SpawnCount[pccorpse radius 110]} && ${Bool[${rezEveryone}]}) {
		/for _count 1 to ${SpawnCount[pccorpse radius 110]}
		ISDEAD
		/if (!${timer_corpse_${NearestSpawn[${_count}, pccorpse radius 110].ID}}) {
			/varset _tmpID ${NearestSpawn[${_count}, pccorpse radius 110].ID}
			/if (${rezEmergency} && ${RezTokenList.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
				/call cast_rez ${_tmpID} ${_rezEmergency}
			} else {
				/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
			}
		}
		/next _count
	}	

	:exit
	UPDATEHUD
/return



|***
 *	DES: rez/call casting code.
 *	USE: /call cast_rez [ID] [rez aa|token]
 *	INI: 
 *  NOTE: 
 ***|
sub cast_rez(int _tmpID, string _rezAA)
	DEBUG ${cbug}cast_rez(${_tmpID}, ${_rezAA})\ax
	ISDEAD
	/squelch /target ID ${_tmpID} 
	/if (${Bool[${forcesafelist}]} && !${safelist.Find[${Target.DisplayName}]} || ${Select[${Me.Class.ShortName},DRU,SHM]}) /delay ${Int[${Math.Calc[${Math.Rand[${RandRezDelay}]}+1]}]}s
	/if (${Target.Distance} > 25 && ${Target.Distance} <= 110) /squelch /corpse
	/if (${Me.AltAbilityReady[${_rezAA}]} || (${Me.ItemReady[${_rezAA}]} && ${rezEmergency})) {	
		/if (${Select[${Me.Class.ShortName},CLR,PAL]}) NOUT Rezzing: ${cpc}${Spawn[${_tmpID}].DisplayName}\ax
		/if (${Select[${Me.Class.ShortName},DRU,SHM]}) NOUT Calling: ${cpc}${Spawn[${_tmpID}].DisplayName}\ax
		/if (${rezEmergency} && ${Target.ID}) NOUT ${owarning} ${sep} PANIC REZZ: ${cpc}${Spawn[${_tmpID}].DisplayName}\ax
		/if (${Target.ID}) ECHOCHANNEL Rezzing: ${Spawn[${_tmpID}].DisplayName}
		/if (!${rezEmergency}) /call MQ2Cast "${_rezAA}" alt -targetid|${_tmpID} -maxtries|3 
		/if (${rezEmergency}) {
			/squelch /target ID ${_tmpID} 
			/delay 1s ${Target.ID} == ${_tmpID}
			/call MQ2Cast "${_rezAA}" item -targetid|${_tmpID} -maxtries|3 
		}
		/if (${Select[${castReturn},CAST_SUCCESS]}) { 
			/call create_timer timer_corpse_${_tmpID} ${tRez_Retry}
		} else /if (${Select[${castReturn},CAST_CANCELLED,CAST_INTERRUPTED]}) {
			/call create_timer timer_corpse_${_tmpID} 7s
		} else {
			/if (${Select[${Me.Class.ShortName},CLR,PAL]}) /call create_timer timer_corpse_${_tmpID} 30s
			/if (${Select[${Me.Class.ShortName},DRU,SHM]}) /call create_timer timer_corpse_${_tmpID} 45s
		}
	}
	:exit
/return



|***
 *	DES: will set your group to combat ready with 1 key.
 *	USE: /here .. 
 *	INI: 
 *  NOTE: this is designed for a fast group set due to agro while traveling. This will not affect the sender of the command
 ***|
sub set_combat_here()
 	DEBUG ${cbug}set_combat_here()\ax
	UPDATEHUD "HERE"

	/if (!${Bool[${Group.Members}]}) /goto :exit
	NOUT ${owarning} ${sep} COMBAT.. Right NOW!

	| clear all pending events
	/doevents flush

	| clear all other modes
	/call mode_clear_all
	
	| reset timers for safety
	/if (${Bool[${BuffItem}]}) /call create_timer timer_check_buffs_item ${defaultTimerDelay}
	/if (${Bool[${BuffSelf}]}) /call create_timer timer_check_buffs_self ${defaultTimerDelay}
	/if (${Bool[${BuffRaid}]}) /call create_timer timer_check_buffs_raid ${defaultTimerDelay}
	/if (${Bool[${BuffBC}]}) /call create_timer timer_check_buffs_BC ${defaultTimerDelay}
	/if (${Bool[${BuffPet}]}) /call create_timer timer_check_pet_buffs ${defaultTimerDelay}
	/if (${Bool[${${Bool[${Aura1Buff}]} || ${Bool[${Aura2Buff}]}}]}) /call create_timer timer_check_aura ${defaultTimerDelay}
	/if (${Bool[${BuffTell}]}) /call create_timer timer_check_buffs_tell ${defaultTimerDelay}
	/if (${Me.Class.ShortName.Equal[CLR]}) /call create_timer timer_heal_ward ${tHealWard}

	/if (${Me.Casting.ID}) {
		/if (${Bool[${Plugin[mq2cast]}]}) /call Interrupt
		/stopcast
	}
	
	| stop 'all' the movement options
	| :loopkillstick
		ISDEAD
		| stop sticking to stuff
		/if (${Stick.Active}) /squelch /stick off
		/if (${tempsnapstuck}) /varset tempsnapstuck FALSE
		/if (${SetTie}) /call Event_SetVar NA SetTie Movement FALSE
		
		| stop advanced path
		/if (${AdvPath.Following}) /squelch /afollow off

		| stop moving
		/if (${Me.Moving}) {
			/keypress forward
			/keypress back
		}
 	| /if (${Stick.Active} || ${Me.Moving} || ${AdvPath.Following} || ${tempsnapstuck} || ${SetTie}) /goto :loopkillstick 	
 	
	| /call clear_combat
	/if (${usePet} && ${Me.Pet.ID} && ${Bool[${Me.Pet.Target}]}) /pet back off

	| make us visible 
	/if (${makeVisible} && ${Me.Invis}) /makemevisible
	
	| get shit out of our hands
	CLEARCURSOR TRUE
	
	| clear the current target
	/if (${clearTarget} && ${Target.ID}) /squelch /target clear
	
	| make sure we are turned on
	/if (!${AutoBot}) /call Event_SetVar NA AutoBot Common TRUE
	
	| wake the Main Assist up from its nap, set him in place and group assists
	/if (${Me.DisplayName.Equal[${GroupAssist}]}) {
		/if (!${HomeSet}) {
			/call Event_SetVar NA HomeSet Movement TRUE
			/call set_home
		}
		| cut the agro way down for safety
		/if (${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${MobAgro}]} > 2) {
			/if (${baseAgroRange} && ${MobAgro} > ${baseAgroRange}) /agro ${baseAgroRange}
		}	
		/call Event_SetVar NA setAgroMode Common TRUE 
		/call Event_SetVar NA setPullMode Common TRUE 
		/call Event_SetVar NA setPullMode Common TRUE 
		/zradius ${hereZRadius}	
	} 
	
	| set assists
	/if (!${Bool[${Raid.Members}]} && !${Select[${Me.Class.ShortName},CLR]} && !${setPullMode}) {
		/smartma G
 	} else /if (${Bool[${Raid.Members}]} && !${Select[${Me.Class.ShortName},CLR]} && ${setPullMode}) {
		/smartma 1
	}	else /if (${Select[${Me.Class.ShortName},CLR]} || ${setPullMode}) {
		/smartma X
	}
	
	| set class specific stuff
	| /combat ${combat${Me.Class.ShortName}}

	UPDATEHUD	
	:exit
/return



|******************************************************************
 *	LIST CONTROL FOR MEZ ROUTINES AND /TELL BUFFING
 ******************************************************************
 * USE CAUTION WHEN MAKING CHANGES HERE
 ******************************************************************|

|***
 *	DES: add to a list
 *	USE: /call list_add [mobList|addList] [ID]
 *	INI: 
 *  NOTE: 
 ***|
sub list_add(string _listName, string _tmpID)
	/declare _exit int local 0
	DEBUG ${cbug}list_add(list: ${_listName}, ID:${_tmpID})\ax
	/if (!${Bool[${${_listName}.Find[${_tmpID}]}]}) {
		/varset ${_listName} ${${_listName}}|${_tmpID}
		/varset _exit 1
		DEBUG addlist#:${addList.Count[|]} - List:${addList}
	}
	:exit
/return ${_exit}



|***
 *	DES: delete from a list
 *	USE: /call list_delete [LISTNAME] [ID|MOBNAME]
 *	INI: 
 *  NOTE: 
 ***|
sub list_delete(string _listName, string _tmpID)
	DEBUG ${cbug}list_delete(list: ${_listName}, ID:${_tmpID})\ax
	/if (${${_listName}.Equal[NULL]} || ${${_tmpID}.Equal[NULL]} || !${${_listName}.Find[${_tmpID}]}) /goto :exit

	/varset _tmpID |${_tmpID}
	/declare _a int local
	/declare _b int local
	/declare _strLeft string local
	/declare _strRight string local
	
	/varcalc _a ${${_listName}.Find[${_tmpID}]}-1
	/varcalc _b ${${_listName}.Length}-${_tmpID.Length}-${_a}
	/if (${_a} > 0) /varset _strLeft ${${_listName}.Left[${_a}]}
	/if (${_a} > 0) /varset _strRight ${If[${Select[${_b},0,NULL,FALSE,""]},,${${_listName}.Right[${_b}]}]} 
	/varset ${_listName} ${_strLeft}${_strRight}
	:exit
/return



|***
 *	DES: update a list
 *	USE: /call list_update [mobList|addList] [ID]
 *	INI: 
 *  NOTE: 
 ***|
sub list_update(string _listName, string _tmpID)
	DEBUG ${cbug}list_update(list: ${_listName}, ID:${_tmpID})\ax
	/declare _found bool local TRUE 

	/if (${${_listName}.Find[|]}) {
		/if (!${Spawn[${_tmpID}].Type.Equal[NPC]} && !${Spawn[${_tmpID}].Master.Type.Equal[NPC]}) /varset _found FALSE
		/if (${Spawn[${_tmpID}].Distance} > ${MobAgro}) /varset _found FALSE
		/if (${Spawn[${_tmpID}].Type.Equal[Corpse]}) /varset _found FALSE
		/if (!${Bool[${Spawn[${_tmpID}].ID}]}) /varset _found FALSE
		
		/if (!${_found}) {
			/call list_delete mobList ${_tmpID}
			/call list_delete addList ${_tmpID}
		}
	}
	:exit
/return



|***
 *	DES: checks for adds in the local area, calls updates to the needed lists.
 *	USE: /call check_for_adds
 *	INI: 
 *  NOTE: 
 ***|
sub check_for_adds()
/declare _tmpID				string	local
/declare _numMobs			int			local 1
:loopnextmob
	/if (${NearestSpawn[${_numMobs},npc radius ${MobAgro} targetable los playerstate 4].ID}) {
		/varset _tmpID ${NearestSpawn[${_numMobs},npc radius ${MobAgro} targetable ${If[${setIgnoreLOS},,los]} playerstate 4].ID}
		| /if (${Spawn[id ${_tmpID}].Type.Equal[NPC]} && ${Spawn[id ${_tmpID}].Distance}<=${MobAgro} && ${Spawn[id ${_tmpID}].LineOfSight} && !${ExcludeList.Find[${Spawn[id ${_tmpID}].DisplayName}]} && ${Spawn[${_tmpID}].DisplayName.Length}>1 && ${Spawn[${_tmpID}].Type.NotEqual[Untargetable]}) {
		/if (${Spawn[id ${_tmpID}].Type.Equal[NPC]} && ${Spawn[id ${_tmpID}].Distance}<=${MobAgro} && ${Spawn[id ${_tmpID}].LineOfSight} && !${ExcludeList.Find[${Spawn[id ${_tmpID}].DisplayName}]} && ${Spawn[id ${_tmpID}].Type.NotEqual[Untargetable]}) {
			/if (!${timer_mez_${_tmpID}}) {
				/if (${_tmpID} != ${Me.XTarget[1].ID}) /call list_add addList ${_tmpID}
				/call list_add mobList ${_tmpID}
				/if (${Macro.Return.Equal[1]}) {
					VOUT ${owarning} ADD ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax] ${sep} [${ctar}${_tmpID}\ax]
					/if (${Me.Casting.Name.NotEqual[${spellMez}]} && ${Me.Casting.ID}) {
						/if (${Bool[${Plugin[mq2cast]}]}) /call Interrupt
						/stopcast
					}
					/if (${MezzImmuneList.Find[${Spawn[${_tmpID}].DisplayName}]}) {
						NOUT ${owarning} IMMUNE TO\ax ${csp}${spellMez}\ax ${sep} [${ctar}${_tmpID}\ax]
						/call create_timer timer_mez_${_tmpID} 1.5m
					}          	
				}
			}		
		}
		/varcalc _numMobs ${_numMobs}+1
		/goto :loopnextmob
	}
/return



|***
 *	DES: preps the queue with the spell
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub queue_buff()
	DEBUG ${cbug}queue_buff()\ax
	ISDEAD
  /declare _count int local
	/if (${BCBuffCount}) {
		/for _count 1 to ${BCBuffCount}
			ISDEAD
			/if (${Me.AltAbility[${queueBuff${_count}}]}) /varset queueBuffID[${_count}] ${Me.AltAbility[${queueBuff${_count}}].Spell.ID}
			/if (${FindItem[${queueBuff${_count}}].ID}) /varset queueBuffID[${_count}] ${FindItem[${queueBuff${_count}}].Spell.ID}
			/if (${Me.Book[${queueBuff${_count}}]}) /varset queueBuffID[${_count}] ${Me.Book[${Me.Book[${queueBuff${_count}}]}].ID}
		/next _count
	}
/return



|***
 *	DES: adds /t for buffs from queue
 *	USE: uses INI [Buffs] 
 *	INI: 
 *  NOTE: 
 ***|
sub queue_buff_add(int _buffee, int _buffnumber)
	DEBUG ${cbug}queue_buff_add(${_buffee}, ${_buffnumber})\ax
  /declare _count				int local
  /declare _compare			int local
	ISDEAD
  /if (${queueCount} < ${queueCountMax} && !${Spawn[${_buffee}].Dead} && ${Spawn[${_buffee}].ID}) {
    /varset _compare 0
    /for _count 1 to ${queueCountMax}
      /if ((${_buffee}==${queueList[${_count},1]} || (${Select[${Spell[${queueBuffID[${queueList[${_count},2]}]}].TargetType},"Group v2","AE PC v2","Group v1","AE PC v1"]} && (${Group.Member[${Spawn[${_buffee}].DisplayName}].ID} || ${Group.Member[${Spawn[${_buffee}].Master.DisplayName}].ID}) && ${Group.Member[${Spawn[${queueList[${_count},1]}].DisplayName}].ID})) && ${_buffnumber}==${queueList[${_count},2]}) {
        /varset _compare 1
      }
    /next _count

    /if (!${_compare}) {
      /for _count 1 to ${queueCountMax}
        /if (!${queueList[${_count},1]}) {
          /varset queueList[${_count},1] ${_buffee}
          /varset queueList[${_count},2] ${_buffnumber}
          /varcalc queueCount ${queueCount}+1
          /goto :exit
        }
      /next _count
    }
  }
  :exit
/return



|***
 *	DES: removes /t for buffs from queue
 *	USE: uses INI [Buffs] 
 *	INI: 
 *  NOTE: 
 ***|
sub queue_buff_delete(int _buffee, int _buffnumber)
	DEBUG ${cbug}queue_buff_delete(${_buffee}, ${_buffnumber})\ax
	/declare _count 			int local
	/for _count 1 to ${queueCountMax}
		/if (((${Spawn[${queueList[${_count},1]}].State.Equal["DEAD"]} || !${Spawn[${queueList[${_count},1]}].ID}) || ((${queueList[${_count},1]}==${_buffee} || (!${_buffee} && (${Group.Member[${Spawn[${queueList[${_count},1]}].DisplayName}].ID} || ${Group.Member[${Spawn[${queueList[${_count},1]}].Master.DisplayName}].ID}))) && (${queueBuffID[${queueList[${_count},2]}]}==${_buffnumber} || ${queueList[${_count},2]}==${_buffnumber}))) && ${queueList[${_count},1]}) {
			DEBUG removing ${Spawn[${queueList[${_count},1]}].Name}  / ${Buff${queueList[${_count},2]}}
			/varset queueList[${_count},1] 0
			/varset queueList[${_count},2] 0
			/varcalc queueCount ${queueCount}-1
		}
	/next _count
/return



|***
 *	DES: Buff on /t requests
 *	USE: uses INI [Buffs] 
 *	INI: 
 *  NOTE: 
 ***|
sub check_buffs_tell()
	DEBUG ${cbug}check_buffs_tell()\ax
	ISDEAD
	UPDATEHUD "BUFF: Tell"
	/doevents
	/declare _count					int local
	/declare _tmpVAR				int local ${Math.Calc[${BuffCount}+1]}
	/declare _tmpID					int local 0

	/for _count 1 to ${queueCount}
		ISDEAD
		/doevents
		| /call check1offBuffs
		CHECKEXIT
		/if (${queueList[${_count},2]} < ${_tmpVAR} && ${Spawn[${queueList[${_count},1]}].ID} && !${Spawn[${queueList[${_count},1]}].Dead} && (${Spawn[${queueList[${_count},1]}].Distance} <  ${Spell[${Buff${queueList[${_count},2]}}].MyRange} || ${Spawn[${queueList[${_count},1]}].Distance} < ${Spell[${Buff${queueList[${_count},2]}}].AERange}) && ${Me.CurrentMana} > ${Spell[${Buff${_tmpVAR}}].Mana}) {
			/varset _tmpID ${queueList[${_count},1]}
			/varset _tmpVAR ${queueList[${_count},2]}
		}
	/next _count
	
	/if (${_tmpID}) /if ((${Me.SpellReady[${Buff${_tmpVAR}}]} || !${Me.Gem[${Buff${_tmpVAR}}]} || ${Spell[${Buff${_tmpVAR}}].RecastTime} < 30) || (${BuffGem${_tmpVAR}.Equal[item]} && ${FindItem[${Buff${_tmpVAR}}].TimerReady} < 30)) {
	| /if (${_tmpID}) /if ((${Me.SpellReady[${Buff${_tmpVAR}}]} || ${Spell[${Buff${_tmpVAR}}].RecastTime} < 30) || ${Me.ItemReady[${Buff${_tmpVAR}}]}) {
		/target id ${_tmpID}
		/delay 5s ${Target.ID}==${_tmpID}
		/if (${Target.ID}!=${_tmpID} || !${Target.ID}) /goto :exit
		/if (${Me.ItemReady[${Buff${_tmpVAR}}]}) /call core_cast "${Buff${_tmpVAR}}" item ${_tmpID}
		/call core_cast "${Buff${_tmpVAR}}" ${buffGem} ${_tmpID}

		/if (${Select[${castReturn},CAST_SUCCESS]}) /call queue_buff_delete ${_tmpID} ${_tmpVAR}
	}
	
	:exit
	/if (${AutoBot}) /call create_timer timer_check_buffs_tell ${tTell_Buff}
	UPDATEHUD
/return

|******************************************************************|



|***
 *	DES: creates/displays who has core set to manual mode in the HUD
 *	USE: /call broadcast_status
 *	INI: core.ini -> [transmit_status]
 *  NOTE: this only works for your group and on the same computer. it shares the settings in core.ini
 ***|
sub broadcast_status()
	/declare _count 			int 		local
	/declare _build				string 	local 

	| create your toon Core.INI entry
	/declare _INIstatus		string 		local ${Ini[${INICore},"transmit_status","${Me.DisplayName}"]}

	| adjust the INI to your current status for youself and others If it is different
	/if (${setBroadcast.Equal[TRUE]} && ${_INIstatus.NotEqual[DISABLED]}) {
		/if (${_INIstatus.NotEqual[${AutoBot}]}) /ini "${INICore}" "transmit_status" "${Me.DisplayName}" "${AutoBot}"
	}

	| creat the hud display list relative to who has AutoBot OFF and is a group member
	/for _count 1 to ${NetBots.Counts}
		/varset _INIstatus ${Ini[${INICore},"transmit_status","${NetBots.Client[${_count}]}"]}
		/if (${_INIstatus.Equal[DISABLED]}) /goto :next_count
		
		/if (!${Bool[${_INIstatus}]} && ${Group.Member[${NetBots.Client[${_count}]}].ID}) {
			/varset _build ${_build} ${If[${_INIstatus},,${NetBots.Client[${_count}].Left[3]}]}
		}
		
		:next_count
	/next _count

	/varset hudBroadcast ${_build}

	:exit
	/call create_timer timer_check_status ${tCheck_status}
/return ${_build}



|***
 *	DES: advanced loot routine(s)
 *	USE: /call do_loot
 *	INI: core.loot.ini
 *  NOTE: set a person in the group as master looter. Or, as the raid master looter.
 *	NOTE2: The event in this sectction is designed to handle lore items the toon already posseses. as of this, MQ has no property .Member value for LORE in ${AdvLoot
 ***|
#Event forcedloreleave "#*#already has #1# and it is lore#*#"
#Event forcedloreleave "#*#does not want #1#. It is either on their never list or they have selected No#*#"
sub Event_forcedloreleave(string line, string _itemName)
	DEBUG ${cbug}Event_forcedloreleave()\ax
	| /echo current iteration:${currentLootCount}
	| /declare _itemName string local
	| /if (${AdvLoot.SList[1].Name.Equal[${_itemName}]}) {
	| /if (${AdvLoot.SList[1].Name}) {

		/varset _itemName ${AdvLoot.SList[1].Name}
		VOUT Loot INI: ${cinfo}${_itemName}\ax -> Leave 
		/ini "${INILoot}" "${_itemName.Left[1]}" "${_itemName}" Leave
		| /echo "${_itemName}"  .. Leave
	| }
	/doevents flush
	:exit
/return

sub do_loot()
 	DEBUG ${cbug}do_loot()\ax
	ISDEAD
	/doevents
	/if (${Stick.Active} || !${Me.FreeInventory} || !${Me.UseAdvancedLooting}) /goto :exit

	| are we allowed to loot or not? i mean seriously.. make up your mind.
	/if (${Me.UseAdvancedLooting} && ${Raid.Members} && ${Raid.MasterLooter.ID} == ${Me.ID}) {
		/goto :permissiontoloot
	} else /if (${Me.UseAdvancedLooting} && !${Raid.Members} && ${Group.Members} && ${Group.MasterLooter.ID} == ${Me.ID}) {
		/goto :permissiontoloot
	} else /if (${Me.UseAdvancedLooting} && !${Raid.Members} && !${Group.Members}) {
		/goto :permissiontoloot
	} else {
		/goto :exit
	}
	
	:permissiontoloot
	UPDATEHUD "LOOT"

	/delay 1s !${AdvLoot.LootInProgress}
	/declare _scount int local 0
	/declare _pcount int local 0
	| Item To Process Name (the thingie in the list of advloot)
	/declare _advlItem string local
	| _advlChoice is what is listed to do with the items: Leave,Destroy,Keep,Sell,Announce
	/declare _advlChoice string local

	| /if (${Group.MasterLooter.ID}==${Me.ID}) {
	/if (${Raid.Members} || ${Group.Members}) {
		| Shared List Sorting
		/for _scount ${AdvLoot.SCount} downto 1
			/varset currentLootCount ${AdvLoot.SCount}
			/varset _advlItem ${AdvLoot.SList[${_scount}].Name}
			/varset _advlChoice ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"]}

			| Does it exist in the loot.ini (check to see if there is a choice for it)
			/if (${AdvLoot.SList[${_scount}].ID} && !${Select[${_advlChoice},Leave,Keep,Destroy,Sell,Announce,Ignore]}) {
				| Add item to ini if it not in there
				/if (${AdvLoot.SList[${_scount}].NoDrop} || ${FindItem[${_advlItem}].Lore} || ${FindItemBank[${_advlItem}].Lore}) {
					VOUT ${oadd}: ${_advlItem} = Leave [no drop/lore].
					/ini "${INILoot}" "${_advlItem.Left[1]}" "${_advlItem}" Leave
				} else {
					VOUT ${oadd}: ${_advlItem} = Keep [default].
					/ini "${INILoot}" "${_advlItem.Left[1]}" "${_advlItem}" Keep
				}
				| if we make a new entry, update shits.
				/varset _advlChoice ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"]}
			}  
			
			| now we do something with the item.
			
			| if we are going to leave the item or if it is lore AND we have one already. Conversly, we loot if we intend to keep or destroy the item
			/if (${Select[${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"]},Leave,Announce]} || ${FindItem[${_advlItem}].Lore} || ${FindItemBank[${_advlItem}].Lore}) {
				DEBUG SharedList: ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"]} is set to Leave.. 
				/if (${Select[${_advlChoice},Announce]}) ECHOCHANNEL Loot: Leaving ${_advlItem}
				/delay 1s !${AdvLoot.LootInProgress}
				/advloot shared ${_scount} leave
				/delay 1
				/goto :loopshareditem
			} else /if (${AdvLoot.SList[${_scount}].ID}) {
				DEBUG SharedList: ${_scount} ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"]} = ${_advlChoice}
				/if (${Select[${_advlChoice},Keep,Destroy,Sell]}) {
					/delay 1s !${AdvLoot.LootInProgress}
					/advloot shared ${_scount} giveto ${Me.Name}
					/delay 1
				}
			}
			:loopshareditem
		/next _scount
	}
	| Personal List sorting. ALSO for solo killing
	/if (${AdvLoot.PCount}) {
		/for _pcount ${AdvLoot.PCount} downto 1
			/varset _advlItem ${AdvLoot.PList[${_pcount}].Name}
			/varset _advlChoice ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"]}
			
			/if (${AdvLoot.PList[${_pcount}].ID} && !${Select[${_advlChoice},Leave,Keep,Destroy,Sell,Announce,Ignore]}) {
				/if (${Select[${_advlChoice},Announce]}) {
					ECHOCHANNEL Loot: Leaving ${_advlItem}
					/delay 1s !${AdvLoot.LootInProgress}
					/advloot personal ${_pcount} leave
				} else /if (${AdvLoot.PList[${_pcount}].NoDrop} || ${FindItem[${_advlItem}].Lore} || ${FindItemBank[${_advlItem}].Lore}) {
					VOUT Adding ${cinfo}${_advlItem}\ax = Leave loot file [no drop].
					/ini "${INILoot}" "${_advlItem.Left[1]}" "${_advlItem}" Leave
					/goto :looppersonalitem
				} else {
					VOUT Adding ${cinfo}${_advlItem}\ax = Keep loot file [default].
					/ini "${INILoot}" "${_advlItem.Left[1]}" "${_advlItem}" Keep
				}
				/varset _advlChoice ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"]}
			} 
			
			
			/if (${Select[${_advlChoice},Destroy]}) {
				/delay 1s !${AdvLoot.LootInProgress}
				/advloot personal ${_pcount} loot
				/delay 1s !${AdvLoot.LootInProgress}
				/delay 5
				/if (${FindItem[${_advlItem}].ID}) {
| evil delete code. had to change this. like > 9000 times
					:loopfind_advlItem
					/delay 5
					| /nomodkey /shift /itemnotify ${FindItem[=${_advlItem}].InvSlot} leftmouseup
					/nomodkey /shift /itemnotify in Pack${Math.Calc[${FindItem[=${_advlItem}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${_advlItem}].ItemSlot2}+1]} leftmouseup
					/delay 1s ${Cursor.Name.Equal[${_advlItem}]}
					/delay 5
					
					VOUT ${odestroy} ${sep} ${cinfo}${_advlItem}\ax
					/if (${Cursor.Name.Equal[${_advlItem}]}) /destroy
					/delay 1
					 /if (${Window[ConfirmationDialogBox].Open}) {
					 	/delay 1
					 	/nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
					 }
					/delay 3 !${Cursor.ID}
					/if (${FindItem[=${_advlItem}].ID}) /goto :loopfind_advlItem
| evil delete code end...
				}          
			} else /if (${Select[${_advlChoice},Leave,Announce]} || ${FindItem[${_advlItem}].Lore} || ${FindItemBank[${_advlItem}].Lore}) {
				/if (${Select[${_advlChoice},Announce]}) ECHOCHANNEL Loot: Leaving ${_advlItem}
				/delay 1s !${AdvLoot.LootInProgress}
				/advloot personal ${_pcount} leave
				/advloot shared ${_pcount} leave
				/goto :looppersonalitem
			} else /if (${Select[${_advlChoice},Keep,Sell]}) {
				DEBUG PList: ${_pcount} ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"]} = ${_advlChoice}
				/delay 1s !${AdvLoot.LootInProgress}
				/advloot personal ${_pcount} loot
				/delay 1
				/if (${Window[ConfirmationDialogBox].Open}) {
					/nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
					/delay 1
				}
			} 
			:looppersonalitem
		/next _pcount
	}        
	:exit
	UPDATEHUD
/return



|***
 *	DES: buy items from a vendor
 *	USE: 
 *	INI: 
 *  NOTE: you have to open the merchants window. this works on the loyalty vendor in PoK as well.
 ***|
sub vendor_buy(string _itemtoBuy, int _buyAmount)
	/if (!${Select[${Target.Type},npc,pet]} || !${Target.ID} || ${Target.Type.Equal[pet]} && !${Target.DisplayName.Find[familiar]}) /return
	/declare _count int local

	/declare _qty int local
	/declare _listItem int local
	/varcalc _qty ${_buyAmount}-${FindItemCount[${_itemtoBuy}]}
	/delay 3
	/if (${FindItemCount[${_itemtoBuy}]} >= ${_buyAmount}) {
		/echo I already have ${FindItemCount[${_itemtoBuy}]} ${_itemtoBuy}.
		/goto :exit
	}   

	| set item in the list window
	/if (${Target.ID} == ${Spawn[npc alerynril].ID}) {
		/varset _listItem ${Window[NewPointMerchantWnd].Child[NewPointMerchant_ItemList].List[=${_itemtoBuy}]}
	} else {
		/varset _listItem ${Window[MerchantWnd].Child[ItemList].List[=${_itemtoBuy},2]}}
	} 	

	/if (!${_listItem}) {
		VOUT ${ocaution} couldn't find ${_itemtoBuy}
		/goto :exit
	} else {
		/if (${Target.ID} == ${Spawn[npc alerynril].ID}) {
			/notify NewPointMerchantWnd NewPointMerchant_ItemList listselect ${_listItem}
		} else {
			/notify MerchantWnd ItemList listselect ${_listItem}
		}
		/delay 5       
	}	

	VOUT Buying ${citem}${_itemtoBuy}\ax 'till I get ${_buyAmount} 

	:loopbuyitems
	/doevents
	/if (${InventoryFull}) /goto :exit

	/if (${_qty} > 0) {
		/if (${_qty} > 19) {
			| /if (${InventoryFull}) /goto :exit
			/if (${Target.ID} == ${Spawn[npc alerynril].ID}) {
				/shiftkey /notify NewPointMerchantWnd NewPointMerchant_PurchaseButton leftmouseup
			} else {            	
				/shiftkey /notify merchantwnd MW_Buy_Button leftmouseup
			}	
			/delay 5 ${FindItemCount[B${_itemtoBuy}]} >= ${_qty}
			/varcalc _qty ${_buyAmount}-${FindItemCount[${_itemtoBuy}]} 
			/if (${_qty} <= 0) /goto :exit
			}
			/delay 5
			/if (${_qty} > 19) /goto :loopbuyitems
		}
		/if (${_qty} > 0 && ${_qty} < 20) {
			/for _count 1 to ${_qty}
				| /if (${Bool[${InventoryFull}]}) /goto :exit 
				/if (${Target.ID} == ${Spawn[npc alerynril].ID}) {
					/ctrlkey /notify NewPointMerchantWnd NewPointMerchant_PurchaseButton leftmouseup
				} else {            	
					/ctrlkey /notify merchantwnd MW_Buy_Button leftmouseup
				}
				/delay 1s ${FindItemCount[${_itemtoBuy}]} >= ${Math.Calc[${FindItemCount[${_itemtoBuy}]}+${_count}]}
				/echo Buying ${_itemtoBuy} ${_count} of ${_qty}
				/if (${_count} >= ${_qty}) /goto :exit
			/next _count
			/varcalc _qty ${_buyAmount}-${FindItemCount[${_itemtoBuy}]}

		}
	}
	:exit    
/return



|***
 *	DES: dell items to a vvendor
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub vendor_sell(string _itemtoSell)
	DEBUG ${cbug}vendor_sell(${_itemtoSell})}\ax
	/declare _pack 					int local
	/declare _slot 					int local
	/declare _lootItem 			string local
  /if (!${Window[MerchantWnd].Open}) /goto :exit

	/if (${_itemtoSell.Equal[lootfile]}) {
		| sell from top slots
		/for _pack 1 to 10
		| Check if top level inv slot is not a container/bag
		/if (${InvSlot[pack${_pack}].Item.Container} == 0) {
			| If top level inv slot is empty check for an item marked sell
			/if (${InvSlot[pack${_pack}].Item.ID}) {
				/varset _lootItem ${InvSlot[pack${_pack}].Item}
				/if (${Ini[${INILoot},"${_lootItem.Left[1]}","${_lootItem}"].Equal[Sell]}) /call vendor_sell_item "${_lootItem}"
			}
		}
		/next _pack   	
		
		| sell from bags
		/for _pack 1 to 10
			| Check if top level inv slot has a bag
			/if (${InvSlot[pack${_pack}].Item.Container} == 0) /next _pack
			| Set _pack 1 to number of bag slots and loop through slots looking for items marked sell
			/for _slot 1 to ${InvSlot[pack${_pack}].Item.Container}
				/if (${InvSlot[pack${_pack}].Item.Item[${_slot}].ID}) {
					/varset _lootItem ${InvSlot[pack${_pack}].Item.Item[${_slot}].Name}
					/if (${Ini[${INILoot},"${_lootItem.Left[1]}","${_lootItem}"].Equal[Sell]}) /call vendor_sell_item "${_lootItem}"
				}
			/next _slot
		/next _pack  	
		/doevents flush sellstuff
	} else {
		/call vendor_sell_item "${_itemtoSell}"
	}
	:exit
	VOUT ${onotice} ${sep} Done Selling.
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub vendor_sell_item(string _itemtoSell)
	DEBUG ${cbug}vendor_sell_item(${_itemtoSell})}\ax
  /if (!${Window[MerchantWnd].Open}) /goto :exit

	:loopsell
	VOUT Selling ${citem}${_itemtoSell}\ax
	/if (${InvSlot[pack${Select[${FindItem[=${_itemtoSell}].ItemSlot},23,24,25,26,27,28,29,30,31,32]}].Item.Container}) {
		/squelch /nomodkey /itemnotify in Pack${Math.Calc[${FindItem[=${_itemtoSell}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${_itemtoSell}].ItemSlot2}+1]} leftmouseup
	} else /if (${FindItem[=${_itemtoSell}].InvSlot}) {
		/squelch /nomodkey /itemnotify ${FindItem[=${_itemtoSell}].InvSlot} leftmouseup
	} else /if (!${FindItem[=${_itemtoSell}].InvSlot}) {
		VOUT ${onotice} ${sep} No ${citem}${_itemtoSell}\ax to sell.
		/goto :exit
	}
	
	/delay 5
	/nomodkey /shiftkey /notify merchantwnd MW_Sell_Button leftmouseup
	/delay 1s
	/if (${FindItemCount[=${_itemtoSell}]}) /goto :loopsell

	:exit
/return



|***
 *	DES: Make sure the AA is up all the time
 *	USE: /call check_AA_now
 *	INI: Toon.INI [NowAA] ->
 *  NOTE: This is hardcoded for only 2 skills. suck it up.
 ***|
sub check_AA_now()
	DEBUG ${cbug}check_AA_now()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit

	| nowAA1
	/if (${Bool[${AANow1}]} && ${Me.AltAbilityReady[${AANow1}]} && ${Spell[${AANow1}].Stacks}) {
		/if (!${Bool[${Me.Buff[${AANow1}]}]} && !${Bool[${Me.Song[${AANow1}]}]}) /call core_cast "${AANow1}" alt 0
	}

	| nowAA2
	/if (${Bool[${AANow2}]} && ${Me.AltAbilityReady[${AANow2}]} && ${Spell[${AANow2}].Stacks}) {
		/if (!${Bool[${Me.Buff[${AANow2}]}]} && !${Bool[${Me.Song[${AANow2}]}]}) /call core_cast "${AANow2}" alt 0
	}	
	:exit
/return



|***
 *	DES: Make sure the Disc is up all the time
 *	USE: /call check_disc_now
 *	INI: Toon.INI [NowDisc] ->
 *  NOTE: This is hardcoded for only 2 skills.
 ***|
sub check_disc_now()
	DEBUG ${cbug}check_disc_now()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit

	| nowDisc1
	/if (${Bool[${DiscNow1}]} && ${Me.CombatAbilityReady[${DiscNow1}]} && ${Spell[${DiscNow1}].Stacks}) {
		/if (!${Bool[${Me.Buff[${DiscNow1.Left[-7]}]}]} && !${Bool[${Me.Song[${DiscNow1.Left[-7]}]}]}) /call core_cast "${DiscNow1}" disc 0
	}

	| nowDisc2
	/if (${Bool[${DiscNow2}]} && ${Me.CombatAbilityReady[${DiscNow2}]} && ${Spell[${DiscNow2}].Stacks}) {
		/if (!${Bool[${Me.Buff[${DiscNow2.Left[-7]}]}]} && !${Bool[${Me.Song[${DiscNow2.Left[-7]}]}]}) /call core_cast "${DiscNow2}" disc 0
	}	
	:exit
/return



|***
 *	DES: last mob is dedded we need a new thing(s) to kill
 *	USE: /call get_target
 *	INI: 
 *  NOTE: got lost hunting for bitches. have your people call my people.
 *	SERIOUS: this will find a valid target to kill within the pullers defined agro range. bring it to the group and start teaching the gospel.
 ***|
sub get_target()
	DEBUG ${cbug}get_target()\ax
	/if (!${setPullMode}) /goto :exit
	ISDEAD
	/declare _tmpID int local
	/declare _count int local
	/declare _mobHP int local 100
	/declare _priorityMob bool local FALSE

	| search local ${combatRadius} range for mob	acquisition
	/if (${SpawnCount[npc ${If[${setIgnoreLOS},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${combatRadius} zradius ${zradius} targetable]}) {
		DEBUG Mob is close. Within:${combatRadius} 
		/for _count 1 to ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${combatRadius} zradius ${zradius} targetable]}
			ISDEAD
			/varset _tmpID ${NearestSpawn[${_count}, npc ${If[${setIgnoreLOS},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${combatRadius} zradius ${zradius} targetable].ID}
	
			/if (!${_priorityMob} && ${Spawn[id ${_tmpID}].Level} >= ${minMobPullLvl} && ${Spawn[id ${_tmpID}].PctHPs} <= ${_mobHP} && ${Spawn[id ${_tmpID}].Type.Equal[NPC]} && !${ExcludeList.Find[${Spawn[id ${_tmpID}].DisplayName}]} && ${Spawn[id ${_tmpID}].DisplayName.Length} > 1 || ${NoMezzList.Find[${Spawn[ID ${_tmpID}].DisplayName}]}) {
				/varset Target2Kill ${_tmpID}
				/varset _mobHP ${Spawn[id ${_tmpID}].PctHPs}
				/if (${NoMezzList.Find[${Spawn[ID ${_tmpID}].DisplayName}]} || ${NameList.Find[${Spawn[ID ${_tmpID}].DisplayName}]}) {
					/if (${NoMezzList.Find[${Spawn[ID ${_tmpID}].DisplayName}]}) VOUT Mez Immune: ${cbad}${Spawn[ID ${_tmpID}].DisplayName}\ax
					/if (${NameList.Find[${Spawn[ID ${_tmpID}].DisplayName}]}) VOUT Named: ${cbad}${Spawn[ID ${_tmpID}].DisplayName}\ax
					/varset _priorityMob TRUE
				}
			}
		/next _count

	| search full ${MobAgro} range for mob acquisition
	} else /if (${SpawnCount[npc ${If[${setIgnoreLOS},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${MobAgro} zradius ${zradius} targetable]} && !${Me.Buff[Resurrection Sickness].ID}) {
		DEBUG Mob is not close. Within:${MobAgro}
		/for _count 1 to ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${MobAgro} zradius ${zradius} targetable]}
			ISDEAD
			/varset _tmpID ${NearestSpawn[${_count}, npc ${If[${setIgnoreLOS},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${MobAgro} zradius ${zradius} targetable].ID}
			
			/if (${Spawn[id ${_tmpID}].Distance} < ${MobAgro} && ${Spawn[id ${_tmpID}].Type.Equal[NPC]} && (${Spawn[id ${_tmpID}].LineOfSight} && !${setIgnoreLOS}) && !${ExcludeList.Find[${Spawn[id ${_tmpID}].DisplayName}]} && ${Spawn[id ${_tmpID}].DisplayName.Length} > 1) {
				/varset Target2Kill ${_tmpID}
 			}
		/next _count	
		
	| lastly, kill all the evil pets. if the owner is dead.
	} else /if (${SpawnCount[pet ${If[${setIgnoreLOS},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${MobAgro} zradius ${zradius} targetable]} && !${SpawnCount[npc ${If[${setIgnoreLOS},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${combatRadius} zradius ${zradius} targetable]}) {
		DEBUG A pet is close. Within:${MobAgro}
		/for _count 1 to ${SpawnCount[pet ${If[${setIgnoreLOS},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${MobAgro} zradius ${zradius} targetable]}
			ISDEAD
			/varset _tmpID ${NearestSpawn[${_count}, pet ${If[${setIgnoreLOS},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${MobAgro} zradius ${zradius} targetable].ID}
			/if (${Spawn[${Spawn[${_tmpID}].Master}].Type.Equal[Corpse]} || !${Spawn[${Spawn[${_tmpID}].Master}].ID}) /varset Target2Kill ${_tmpID}
		/next _count	
	}
	
	/if (${Spawn[${Target2Kill}].Type.NotEqual[Aura]} && ${Target2Kill}) /target id ${Target2Kill}
	DEBUG Red Rover send [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax] right over. Distance:${If[${Spawn[${Target}].LineOfSight},${cgood}${Target.Distance}\ax,${cbad}${Target.Distance}\ax]}

	/call echos pull 
	| /if (${Target2Kill}) VOUT Pull ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax] ${If[${Spawn[${Target}].LineOfSight},${cgood}${Int[${Target.Distance}]}\ax,${cbad}${Int[${Target.Distance}]}\ax]}
	UPDATEHUD "PULL"

	DEBUG Disable /camp for pull
	/if (${setPullMode} && ${HomeSet}) /makecamp pause

	:looppull
	/if (${FindItemCount[${RangeItemSummon}]} < 5 && ${Bool[${RangeItemSummon}]}) /call check_ammo

	| if mob is over arrow range, start checking options to get him close
	/if (${Spawn[id ${Target2Kill}].Distance} > 31 && !${Select[${Me.Class.ShortName},MAG,NEC,BST,ENC]}) {
		DEBUG use arrow to pull mob over 31
		/target ID ${Target2Kill}
		/if (${Target.ID} && (${Select[${combat},MELEE,RANGE]})) /squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}

		/if (${Target.ID}) {
			/if (!${Spawn[${Target}].LineOfSight} && !${setIgnoreLOS}) {
				/delay 2s ${Spawn[${Target}].LineOfSight}
				/if (!${Spawn[${Target}].LineOfSight}) {
					/call clear_combat
					/goto :exit
				}
			}
			/doevents
			/if (${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${MobAgro} targetable]} && ${Me.State.NotEqual[STAND]}) /stand
			/if (${Me.Stunned}) /goto :exit
			| /if (${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${CombatDistance} targetable playerstate 4]}) /goto :exit
			/if (${Select[${Me.Class.ShortName},SHD]} && ${Me.AltAbilityReady[Hate's Attraction]}) {
				/if (${Target.Distance} < ${MobAgro} && ${Target.Distance} > 31 && ${Target.Distance} < ${Spell[Hate's Attraction].MyRange}) /call core_cast "Hate's Attraction" alt ${Target2Kill}
			} else /if (${Select[${Me.Class.ShortName},PAL]} && ${Me.AltAbilityReady[Divine Call]}) {
				/if (${Target.Distance} < ${MobAgro} && ${Target.Distance} > 31 && ${Target.Distance} < ${Spell[Divine Call].MyRange}) /call core_cast "Divine Call" alt ${Target2Kill}
			} else /if (${Select[${Me.Class.ShortName},WAR]} && ${Me.AltAbilityReady[Warlord's Grasp]}) {
				/if (${Target.Distance} < ${MobAgro} && ${Target.Distance} > 31 && ${Target.Distance} < ${Spell[Warlord's Grasp].MyRange}) /call core_cast "Warlord's Grasp" alt ${Target2Kill}
			}

			/if (${Spawn[ID ${Target2Kill}].Distance} > 31 && < ${MobAgro}) {
				/if (${Target.ID}) /range
			} else 	/if (${Spawn[id ${Target2Kill}].Distance} <= 31) {
				/if (${AutoBot}) /squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
				/if (${Select[${combat},RANGE]}) /goto :exit
				/squelch /stick id ${Target2Kill} ${CombatDistance} 
			}
		}

	| if the mob is less the arrow range, move to the bastard
	} else /if (${Spawn[id ${Target2Kill}].Distance} <= 31 && !${Select[${Me.Class.ShortName},MAG,NEC,BST,ENC]}) {
		DEBUG stick to mob <= 31
		/if (${AutoBot} && ${Select[${combat},MELEE,RANGE]}) /squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
		/if (${Select[${combat},RANGE]}) /goto :exit
		/squelch /stick ID ${Target2Kill} ${CombatDistance}

	| pet auto send to kill
	} else /if (${Target2Kill} && ${Spawn[id ${Target2Kill}].Distance} <= ${MobAgro} && ${Select[${Me.Class.ShortName},MAG,NEC,BST,ENC]}) {
		/target ID ${Target2Kill}
		/if (${Target.ID} && !${Bool[${Me.Pet.Target}]}) {
			/squelch /pet attack		
		}
	}
	
	/call is_target_dead ${Target2Kill}
	/if (!${Bool[${Macro.Return}]} && ${Spawn[id ${Target2Kill}].Distance} >= 31 && !${Select[${Me.Class.ShortName},MAG,NEC,BST,ENC]}) {
		/delay 1
		/goto :looppull
	}
	
	:exit
	UPDATEHUD
/return



|***
 *	DES: tells your BC people to hand in up to 4 items to your target. includes command giver
 *	USE: /bc handin |item1|item2|item3|item4
 *	INI: 
 *  NOTE: THIS HAS NOT BEEN TESTED!! you could be handing in your soul and not even know it.
 ***|
sub hand_in(string _sender, string _verbage)
	/if (${Spawn[pc ${_sender}].ID}) /assist ${_sender}
	/delay 1s ${Me.AssistComplete}
	/if (Target.Distance} > 100) /goto :exit
	/if (Target.Distance} < 100) /squelch /stick 10
	/delay 5
	/if (${Target.ID}) {

		| item 1
		/if (${FindItem[${_verbage.Arg[1,|]}].ID}) {
			/nomodkey /shift /itemnotify "${_verbage.Arg[2,|]}" leftmouseup
			/click left target
			/delay 5 !${Cursor.ID}
		} else {
			VOUT Didn't Find ${_verbage.Arg[1,|]}
		}

		| item 2
		/if (${FindItem[${_verbage.Arg[2,|]}].ID}) {
			/nomodkey /shift /itemnotify "${_verbage.Arg[3,|]}" leftmouseup
			/click left target
			/delay 5 !${Cursor.ID}
		} else {
			VOUT Didn't Find ${_verbage.Arg[2,|]}
		}

		| item 3
		/if (${FindItem[${_verbage.Arg[3,|]}].ID}) {
			/nomodkey /shift /itemnotify "${_verbage.Arg[4,|]}" leftmouseup
			/click left target
			/delay 5 !${Cursor.ID}
		} else {
			VOUT Didn't Find ${_verbage.Arg[3,|]}
		}

		| item 4
		/if (${FindItem[${_verbage.Arg[4,|]}].ID}) {
			/nomodkey /shift /itemnotify "${_verbage.Arg[5,|]}" leftmouseup
			/click left target
			/delay 5 !${Cursor.ID}
		} else {
			VOUT Didn't Find ${_verbage.Arg[4,|]}
		}

		/nomodkey /notify GiveWnd GVW_give_Button leftmouseup
	}
	: exit
/return



|***
 *	DES: will dump all worn or invnetory augmetns into an INI.
 *	USE: /echoaugments
 *	INI: core.augmentdump.ini
 *  NOTE: recursive of augments in items in bags.
 ***|
sub echo_augments()
	/declare _slot 			int local
	/declare _augslot 	int local
	/declare _bagslot 	int local
	
	| worn inventory skip powersource (21)
	/for _slot 0 to 22
		/if (${_slot} == 21) /goto :noWornItemAug
		/for _augslot 0 to ${Me.Inventory[${_slot}].Augs}
			/if (${Me.Inventory[${_slot}].AugSlot[${_augslot}].Empty}) /goto :noWornItemAug
			/ini "${INIAugDump}" "${Me.DisplayName} - Worn" "${Me.Inventory[${_slot}].Name} ${Me.Inventory[${_slot}].AugSlot[${_augslot}].Type}" "${Me.Inventory[${_slot}].AugSlot[${_augslot}].Name}"
		/next _augslot
		:noWornItemAug
	/next _slot	

	
	| bag inventory
	/for _slot 23 to 32
			| top slot augs
			/if (${Me.Inventory[${_slot}].Type.Equal[Augmentation]}) {
				/ini "${INIAugDump}" "${Me.DisplayName} - Inventory" "Slot:${_slot}" "${Me.Inventory[${_slot}].Name}"
			}

			| top slot item	with augs
			/if (${Me.Inventory[${_slot}].Augs}) {
				/for _augslot 0 to ${Me.Inventory[${_slot}].Augs}
					/if (!${Me.Inventory[${_slot}].AugSlot[${_augslot}].Empty}) /ini "${INIAugDump}" "${Me.DisplayName} - Inventory" "${Me.Inventory[${_slot}].Name} ${Me.Inventory[${_slot}].AugSlot[${_augslot}].Type}" "${Me.Inventory[${_slot}].AugSlot[${_augslot}].Name}"
				/next _augslot
			}
				
			| inside bags
			/if (${InvSlot[${_slot}].Item.Container}) {
				/for _bagslot 1 to ${InvSlot[${_slot}].Item.Container}
					/if (${Me.Inventory[${_slot}].Item[${_bagslot}].Type.Equal[Augmentation]}) {
						/ini "${INIAugDump}" "${Me.DisplayName} - Inventory" "${Me.Inventory[${_slot}].Name}:${_slot}" "${Me.Inventory[${_slot}].Item[${_bagslot}].Name}"
					}
					/if (${Me.Inventory[${_slot}].Item[${_bagslot}].Augs}) {
	
						/for _augslot 0 to ${Me.Inventory[${_slot}].Item[${_bagslot}].Augs}
							/if (!${Me.Inventory[${_slot}].Item[${_bagslot}].AugSlot[${_augslot}].Empty}) /ini "${INIAugDump}" "${Me.DisplayName} - Inventory" "${Me.Inventory[${_slot}].Item[${_bagslot}].Name} ${Me.Inventory[${_slot}].Item[${_bagslot}].AugSlot[${_augslot}].Type}" "${Me.Inventory[${_slot}].Item[${_bagslot}].AugSlot[${_augslot}].Name}"
						/next _augslot	
					}
				/next _bagslot
			}
		/next _slot
		VOUT Augments Dumped.
	:exit	 
/return



|***
 *	DES: will watch for a spawn in the zone
 *	USE: /watchspawn [NAME]
 *	INI: 
 *  NOTE: resets on start
 ***|
sub check_watch_spawn()
	| this maths dont work..
	| /declare _correctWatchCount int local ${Float[${Math.Calc[${SpawnCount[npc ${watchSpawn}]} - ${SpawnCount[npccorpse ${watchSpawn}]}]}].Int}
	
	/declare _correctWatchCount int local ${SpawnCount[npc ${watchSpawn}]}
	/varset hudwatchspawn ${_correctWatchCount}
	
	| count mismatch
	/if (${watchSpawnCount} != ${_correctWatchCount}) {
		| /if (${watchSpawnCount} < ${_correctWatchCount}) ECHOCHANNEL Spawn: ${_correctWatchCount}-${watchSpawn}
		/varset watchSpawnCount ${_correctWatchCount}
		/squelch /mapshow ${watchSpawn}
	}

	/call create_timer timer_check_watch_spawn ${tWatch_Spawn}
	:exit
/return



|***
 *	DES: finds an item. all BC toons will report back to ECHOCHANNEL item and location of they have it.
 *	USE: /whohas [ITEM] | /bc whohas [ITEM]
 *	INI: 
 *  NOTE: 
 ***|
sub check_who_has(string _sender, string _verbage)
	/declare _slot 			int local
	/declare _augslot 	int local
	/declare _bagslot 	int local

	| /if (${Me.DisplayName.Equal[${_sender}]}) /goto :exit
	/if (!${Bool[${echoChannel}]}) /goto :exit


	/if (${FindItem[=${_verbage}].ID} && !${FindItem[=${_verbage}].Type.Equal[Augmentation]}) {
		ECHOCHANNEL ${FindItem[=${_verbage}]}: Item Inventory
	} else /if (${FindItemBank[=${_verbage}].ID} && !${FindItemBank[=${_verbage}].Type.Equal[Augmentation]}) {
		ECHOCHANNEL ${FindItemBank[=${_verbage}]}: Item Bank
	}

	| augment search	
	| worn inventory skip powersource (21) unless your powersource does something mine doesn't?
	/for _slot 0 to 22
		/if (${_slot} == 21) /goto :noWornItemAug
		/for _augslot 0 to ${Me.Inventory[${_slot}].Augs}
			/if (${Me.Inventory[${_slot}].AugSlot[${_augslot}].Name.Equal[${_verbage}]}) ECHOCHANNEL ${Me.Inventory[${_slot}].AugSlot[${_augslot}].Name}: Worn Augment in ${Me.Inventory[${_slot}].Name} 
		/next _augslot
		:noWornItemAug
	/next _slot	
		
		
	| bag inventory
	/for _slot 23 to 32
		| top slot augs
		/if (${Me.Inventory[${_slot}].Name.Equal[${_verbage}]}) ECHOCHANNEL ${Me.Inventory[${_slot}].Name}: Inventory Augment in ${Me.Inventory[${_slot}].Name}

		| top slot item	with augs
		/if (${Me.Inventory[${_slot}].Augs}) {
			/for _augslot 0 to ${Me.Inventory[${_slot}].Augs}
				/if (${Me.Inventory[${_slot}].AugSlot[${_augslot}].Name.Equal[${_verbage}]}) ECHOCHANNEL ${Me.Inventory[${_slot}].AugSlot[${_augslot}].Name}: Inventory Item Augment in ${Me.Inventory[${_slot}].Name} 
			/next _augslot
		}

		| inside bags
		/if (${InvSlot[${_slot}].Item.Container}) {
			/for _bagslot 1 to ${InvSlot[${_slot}].Item.Container}
				/if (${Me.Inventory[${_slot}].Item[${_bagslot}].Name.Equal[${_verbage}]}) ECHOCHANNEL ${Me.Inventory[${_slot}].Item[${_bagslot}].Name}: Inventory Item in ${Me.Inventory[${_slot}].Name}

				| in items inside bags
				/if (${Me.Inventory[${_slot}].Item[${_bagslot}].Augs}) {
					/for _augslot 0 to ${Me.Inventory[${_slot}].Item[${_bagslot}].Augs}
						/if (${Me.Inventory[${_slot}].Item[${_bagslot}].AugSlot[${_augslot}].Name.Equal[${_verbage}]}) ECHOCHANNEL ${Me.Inventory[${_slot}].Item[${_bagslot}].AugSlot[${_augslot}].Name}: Inventory Item Augment in ${Me.Inventory[${_slot}].Item[${_bagslot}].Name}
					/next _augslot	
				}
			/next _bagslot
		}
	/next _slot
	
	:exit
/return



|***
 *	DES: Simply gives cash to an NPC. 
 *	USE: /givecoin [ammount] [stacks] [plat|gold|copper|silver]
 *	INI: NA
 *  NOTE: It's not dumb if it works.
 ***|
sub give_coin(int _ammount, int _stacks, string _coin)
	DEBUG ${cbug}give_coin(${_ammount}, ${_stacks}, ${ _coin})\ax
	| NOUT /givecoin [ammount] [stacks] [plat|gold|copper|silver]

	/declare _count 			int local 
	/declare _countloop 	int local 1
	/declare _coinID			int local 99

	| check target
	/if (!${Target.ID} || ${Taret.ID} == ${Me.ID} || ${Target.ID} == ${Pet.ID}) {
		VOUT ${owarning} ${sep} Need a target to give the coin too. Try again.
		/goto :exit
	}
	
	| check coin type
	/if (${_coin.Equal[plat]}) {
		/varset _coinID 0
	} else /if (${_coin.Equal[gold]}) {
		/varset _coinID 1
	} else /if (${_coin.Equal[silver]}) {
		/varset _coinID 2
	} else /if (${_coin.Equal[copper]}) {
		/varset _coinID 3
	} else /if (${_coin.Equal[NULL]}) {
		VOUT ${owarning} ${sep} Invalid coin type. Try again.
		/goto :exit
	}

	:loopgivemorecash
		/if (!${Window[InventoryWindow].Open}) /invoke ${Window[InventoryWindow].DoOpen}
		/if (!${Target.ID} || ${Taret.ID} == (${Me.ID} || ${Pet.ID})) /goto :exit
		/delay 2s
		/for _count 1 to ${_stacks}
			/ctrl /notify InventoryWindow IW_Money${_coinID} leftmouseup
			/delay 5
			/click left target 
			/delay 5 
		/next _count 
		/delay 3 
		/notify GiveWnd GVW_Give_Button leftmouseup 

		/delay 5 
		/varset _countloop ${Math.Calc[${_countloop}+1]}
		/if (${_countloop} < ${_ammount}) /goto :loopgivemorecash

	:exit
	VOUT ${onotice} ${sep} Done giving cash for bribes.
/return



|***
 *	DES: keeps SK/PAL harmonious up.. always..
 *	USE: /call check_harmonious
 *	INI: [spells] - harmonious
 *  NOTE: 103 spell EoK spell
 ***|
sub check_harmonious()
	DEBUG ${cbug}check_harmonious()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]} || ${SafeZone} || ${Me.Level} < 103) /goto :exit
	/if (!${Bool[${Me.Buff[${Harmonious}]}]} && ${Me.PctMana} > 10) {
		/call core_cast "${Harmonious}" 0 0
	}
	:exit
/return



|***
 *	DES: prep sub for all calsses to decide if they are going to be in combat and if its time to burn
 *	USE: /call prep_combat
 *	INI: NA
 *  NOTE:
 ***|
sub prep_combat()
	/declare _gokillstuff		bool local TRUE
	CHECKREZ
	CLEARCURSOR
	/if (${AutoBot} && !${Me.Invis}) {
		/call check1offBuffs
		/if (${Select[${combat},MELEE,RANGE,NUKE]}) {
			/call is_target_valid ${Target2Kill}
			/if (!${isValidTarget} || !${Spawn[${Target2Kill}].ID}) {
				/call clear_combat
				/varset _gokillstuff FALSE
			} else /if (${isValidTarget} && ${Spawn[${Target2Kill}].ID}) {
				/call is_toast_time
				/if (${Bool[${Macro.Return}]}) /call synergy_${Me.Class.ShortName}
			}
		} else /if (${Select[${combat},HEAL,DUAL]}) {
			/call is_toast_time
			/if (${Bool[${Macro.Return}]}) /call synergy_${Me.Class.ShortName}
		}
	} else /if (!${AutoBot}) {
		/if (${Target.ID} != ${Target2Kill}) /varset Target2Kill ${Target.ID}
		/if (!${Target.ID} || !${Spawn[${Target2Kill}].ID}) /varset _gokillstuff FALSE
		/if (${setToastMode}) /call synergy_${Me.Class.ShortName}
	}

	:exit
/return ${_gokillstuff}



 |***
 *	DES: Check if we need to engage toast routines. /combat heal/dual are treated differently
 *	USE: auto
 *	INI: 
 *  NOTE: we work off establised assist target for non healers only.
 ***|
sub is_toast_time()
	DEBUG ${cbug}is_toast_time()\ax
	/doevents
	ISDEAD
	/if (${SafeZone} && ${AutoBot}) /goto :exit

	/declare _toast 				bool 		local FALSE
	/declare _mobcount			int 		local 0
	/declare _count					int 		local 0
	/declare _mobID 				int 		local 0
	
	| if always set to toast, always toast while in combat
	/if (${AutoBot} && ${setToastMode} && ${Me.Combat}) /varset _toast TRUE
	
	| if the current target is a namer
	/if (${AutoBot} && ${AutoToast} && ${Bool[${Target2Kill}]} && ${Spawn[${Target2Kill}].PctHPs} <= ${whentoToast} && ${NameList.Find[${Target.DisplayName}]}) /varset _toast TRUE

	| if number of mobs in the local area reaches the /toastmobcount threashold
	/if (${AutoBot} && ${Bool[${toastMobCount}]} && ${SpawnCount[npc radius ${MobAgro} ${If[${setIgnoreLOS},,los]} targetable playerstate 4]} >= ${toastMobCount}) /varset _toast TRUE

	| healers mezers, and off combat routines simply check for a named in the local area to start a toast. no direct target of a named is required. simply be in the area.
	/if (${Select[${combat},HEAL,DUAL]}) {
		/varset _mobcount ${SpawnCount[npc radius ${MobAgro} playerstate 4]}
		/for _count 1 to ${_mobcount}
			/doevents
			ISDEAD
			/varset _mobID ${NearestSpawn[${_count}, npc radius ${MobAgro}].ID}
			/if (${AutoToast} && ${NameList.Find[${Spawn[id ${_mobID} radius ${MobAgro}].DisplayName}]}) /varset _toast TRUE
			/if (${Bool[${toastMobCount}]} && ${SpawnCount[${If[${setIgnoreLOS},,los]} npc radius ${MobAgro} targetable playerstate 4]} >= ${toastMobCount}) /varset _toast TRUE
		/next _count 	
	}
	:exit
/return ${_toast}



|***
 *	DES: Designed to check for the one-off crap.
 *	USE: /call check1offBuffs
 *	INI: 
 *  NOTE: THIS NEEDS TO BE WORKED OUT OF THE CODE.. MOVE THIS CRAP TO check_exit...
 ***|
sub check1offBuffs()
	ISDEAD
	/if (${Me.Buff[Holistic Health].ID}) /call cure_Anashti_DT
	/if (${Me.Buff[Pyroignition].ID}) /call CurePyroignitionDot
	/if (${Me.Buff[Withering Physicality].ID} || ${Me.Buff[Withering Limbs].ID} || ${Me.Buff[Withering Faith].ID}) /call CureDamselofDecay
	:exit
/return



|***
 *	DES: Summons Food
 *	USE: /call click_summon_food 
 *	INI: toon INI [Meal]->
 *  NOTE: 
 ***|
sub click_summon_food()
	DEBUG ${cbug}click_summon_food(${FindItemCount[${clickFoodItem}]} <= ${clickFoodCount})\ax
	ISDEAD
	CHECKTIE
	/call check_exit
	/if (${Bool[${Macro.Return}]} || !${Bool[${clickFoodDispenser}]}) /goto :exit
	
	/if (${FindItemCount[=${clickFoodItem}]} < ${clickFoodCount} && ${Me.ItemReady[${clickFoodDispenser}]}) {
		DEBUG click ${clickFoodDispenser}
		/call core_cast "${clickFoodDispenser}" item ${Me.ID}
		CLEARCURSOR TRUE
	}
	/call create_timer timer_click_food ${tClick_FoodDrink}
	:exit
/return



|***
 *	DES: Summons Drink
 *	USE: /call click_summon_drink
 *	INI: toon INI [Meal]->
 
 *  NOTE: 
 ***|
sub click_summon_drink()
	DEBUG ${cbug}click_summon_drink(${FindItemCount[${clickDrinkItem}]} <= ${clickDrinkCount})\ax
	ISDEAD
	CHECKTIE
	/call check_exit
	/if (${Bool[${Macro.Return}]} || !${Bool[${clickDrinkDispenser}]}) /goto :exit
	
	/if (${FindItemCount[=${clickDrinkItem}]} < ${clickDrinkCount} && ${Me.ItemReady[${clickDrinkDispenser}]}) {
		DEBUG click ${clickDrinkDispenser}
		/call core_cast "${clickDrinkDispenser}" item ${Me.ID}
		CLEARCURSOR TRUE
	}
	/call create_timer timer_click_drink ${tClick_FoodDrink}
	:exit
/return



