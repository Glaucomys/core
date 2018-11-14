|***
 *	common.inc
 *	Core shared Routines/Functions
 *
 *
 ***|




|***
 *	DES: Checks EQBCS connection and channel
 *	USE: /call check_one_mind
 *	INI: core.ini->[communications]
 *  NOTE: 
 ***| 
sub check_one_mind()
	DEBUG check_one_mind()
	| if were not using eqbc.. go away..
	/if (!${setCommunications.Equal[eqbc]}) /return FALSE
	
	/call create_timer timer_check_EQBC ${tCheck_EQBC}
	/if (!${validate_plugin[FALSE, |mq2eqbc]}) /return
	/if (!${EQBC.Connected} && ${Bool[${EQBCServer}]}) {
		/bccmd connect ${EQBCServer} ${EQBCPort}
		/delay 15s !${EQBC.Connected}
	}
/return



|***
 *	DES: checks to make sure all needed plugins are loaded.
 *	USE: /call check_plugin
 *	INI: core.ini
 *  NOTE: 
 ***|
sub check_plugin()
	DEBUG check_plugin()
	/declare _count int local 0
	/declare _plug string local ${Ini[${INICore},plugin,plist]}

	/for _count 1 to ${_plug.Count[|]}
		/if (!${Bool[${Plugin[${_plug.Arg[${_count},|]}]}]}) {
			OUT Plugin \a-w${_plug.Arg[${_count},|]}\ax is missing. Loading it.
			/squelch /plugin ${_plug.Arg[${_count},|]}
		}
	/next _count
/return



|***
 *	DES: echos misc stuff to the MQ Window
 *	USE: /call echos [type] "desc text" [VARIABLE] [Target ID] ["text2"]
 *	INI: 
 *  NOTE: 
 ***|
sub echos(string _type, string _verbage, string _tmpVariable, int _tmpID, string _verbage2)
	AMIDEAD
	CHECKINPUTS
	DEBUG echos(${_type}, ${_verbage}, ${_tmpVariable}, ${_tmpID}, ${_verbage2})

	| on/off switch echos
	/if (${_type.Equal[switch]}) {
		VOUT ${_verbage} ${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}

	| control switches
	} else /if (${_type.Equal[setswitch]}) {
		VOUT ${_verbage} ${_verbage2} ${If[${Bool[${_tmpID}]},[\a-w${_tmpID}\ax] ,]}${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}


	| variable %s
	} else /if (${_type.Equal[number]}) {
		VOUT ${_verbage} ${sep} ${cinfo}${_tmpVariable}\ax

	| core Initialize
	} else /if (${_type.Equal[initialize]}) {
		OUT Initialize Core ${sep} ${cinfo}${coreBuild}\ax

	| zone initialization
	} else /if (${_type.Equal[zone]}) {
		/if (${Bool[${Ini[${INIZone},"${Zone.ShortName}","FullZoneName"]}]}) {
			VOUT Setting Zone ${sep} ${cinfo}${Zone.Name}\ax
		} else {
			VOUT Creating Zone ${sep} ${cinfo}${Zone.Name}\ax
		}

	| ending macro
	} else /if (${_type.Equal[ending]}) {
		VOUT ${owarning} ${sep} ${cbad}ENDING\ax Core Build ${sep} ${cinfo}${coreBuild}\ax

	| need target
	} else /if (${_type.Equal[needtarget]}) {
		VOUT ${onotice} ${sep} Bad target.

	| need cursor item
	} else /if (${_type.Equal[needcursor]}) {
		VOUT ${onotice} ${sep} need something on cursor.

	| cursor item
	} else /if (${_type.Equal[cursor]}) {
		VOUT Cursor ${sep} ${cinfo}${_verbage}\ax ${sep} ${_tmpVariable}

	| hunt
	} else /if (${_type.Equal[hunt]}) {
		VOUT Hunt ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax] ${sep} ${If[${Spawn[${_tmpID}].LineOfSight},${cgood}${Int[${Spawn[${_tmpID}].Distance}]}\ax,${cbad}${Int[${Spawn[${_tmpID}].Distance}]}\ax]}

	| mode switches
	} else /if (${_type.Equal[mode]}) {
		VOUT Mode ${sep} ${cinfo}${_verbage}\ax ${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}

	| petfarm
	} else /if (${_type.Equal[petfarm]}) {
		VOUT Pet ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax] ${sep} ${If[${Spawn[${Target2Kill}].LineOfSight},${cgood}${Spawn[${Target2Kill}].Distance}\ax,${cbad}${Spawn[${Target2Kill}].Distance}\ax]}

	| pull
	| } else /if (${_type.Equal[pull]} && ${Target2Kill}) {
	| 	VOUT Pull ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax] ${sep} ${If[${Spawn[${Target}].LineOfSight},${cgood}${Int[${Target.Distance}]}\ax,${cbad}${Int[${Target.Distance}]}\ax]}

	| grab (offtanks)
	| } else /if (${_type.Equal[grab]} && ${Target2Kill}) {
	| 	VOUT Grab ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax] ${sep} ${If[${Spawn[${Target}].LineOfSight},${cgood}${Int[${Target.Distance}]}\ax,${cbad}${Int[${Target.Distance}]}\ax]}
 
 	| you suck
	} else /if (${_type.Equal[failboat]}) {
		/if (${smartasserrors}) {
			/call echo_bitch MQWindow
		} else {
			VOUT ${cbad}Error\ax in command.
		}

	| info
	} else /if (${_type.Equal[wiki]}) {
		VOUT WIKI ${sep} ${cinfo}https://github.com/exspes007/core/wiki\ax

	| override switches
	}	else /if (${_type.Equal[override]}) {
		VOUT Override ${sep} ${cinfo}${_verbage}\ax ${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}

	| debuff switches
	}	else /if (${_type.Equal[debuff]}) {
		VOUT deBuff ${sep} ${cinfo}${_verbage}\ax ${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}

	| heal switches
	}	else /if (${_type.Equal[heal]}) {
		VOUT Heal ${sep} ${cinfo}${_verbage}\ax ${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}

	| buff echos
	}	else /if (${_type.Equal[buff]}) {
		VOUT Buff ${sep} ${cinfo}${_verbage}\ax ${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}

	| rez echos
	}	else /if (${_type.Equal[rez]}) {
		VOUT Rez ${sep} ${cinfo}${_verbage}\ax ${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}

	| arg list echos
	} else /if (${_type.Equal[listtype]}) {
		
		/declare _listout string local
		/declare _sep bool local FALSE
		/declare _count int local 0
		/for _count 1 to ${${_tmpVariable}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_tmpVariable}Type.Arg[${_count},|]}\ax
			/varset _sep TRUE
		/next _count
		VOUT /${_verbage.Lower} [${_listout}]
	}
/return



|***
 *	DES: Check to see if toon is engaged in any way.
 *	USE: /call check_engaged
 *	INI: 
 *  NOTE: 
 ***|
sub check_engaged()
	CHECKINPUTS
	DEBUG check_engaged()
	| rogue SOS .. 
	/if (${Select[${Me.Class.ShortName},ROG]}) {
		/if (${coreAuto} && ${switchSos}) /return FALSE
	}
	
	| invis
	/if (${Me.Invis}) /return TRUE
	
	| spawn count
	/if (!${switchBuffCombat}) {
		/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${MobAgro} playerstate 4]}) /return TRUE
	}

	| spell cast'in. songs as well
	/if (${Me.Casting.ID}) /return TRUE

	| movement
	/if (${Me.Moving}) /return TRUE
	| /if (${Stick.Active}) /return TRUE
	
	| combat
	/if (${Me.Combat}) /return TRUE
	/if (${Me.AutoFire}) /return TRUE
	
	| am i the puller or main agro with mobs in range?
	| /if (((${switchPull} && ${switchAgro}) || ${Group.MainAssist.Name.Equal[${Me.DisplayName}]} || ${Group.MainTank.Name.Equal[${Me.DisplayName}]}) && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${CombatDistance}*2 playerstate 4]}) /return TRUE
	/if (${switchPull} && ${switchAgro}) {
		/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${CombatDistance}*2 playerstate 4]}) /return TRUE
	}
	/if (${Group.MainAssist.Name.Equal[${Me.DisplayName}]}) {
		/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${CombatDistance}*2 playerstate 4]}) /return TRUE
	}
	| /if (${Group.MainTank.Name.Equal[${Me.DisplayName}]} && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${CombatDistance}*2 playerstate 4]}) /return TRUE

/return FALSE



|***
 *	DES: give weapons to pet
 *	USE: /call give_to_pet
 *	INI: [Pet] - 
 *  NOTE: 
 ***|
sub give_to_pet()
	DEBUG give_to_pet()
	/if (${Target.ID} != ${Me.Pet.ID}) {
		/target ID ${Me.Pet.ID}
		/delay 1s ${Target.ID} == ${Me.Pet.ID}
	}
	:loopgiveitem
	AMIDEAD
	CHECKINPUTS
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
/return



|***
 *	DES: Summon pet. I mean, it won't summon cheeze
 *	USE: /call cast_summon_pet
 *	INI: Pet - PcPettype
 *  NOTE: 
 ***|
sub cast_summon_pet()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	DEBUG cast_summon_pet()

	/if (${switchPcPet}) {
		/if (${coreAuto} && !${Me.Pet.ID}) {
			/if (${validate_cast[FALSE, alt, "Suspended Minion", ${Me.ID}]}) /call core_cast2 "Suspended Minion" alt 0 FALSE
			/delay 5s !${Me.Casting.ID}
			/delay 5
		}
	}
	
	| component check
	/if (${switchPcPet} && !${Me.Pet.ID}) {
		/declare _error bool local FALSE
		/if (${validate_class[FALSE, |ENC]} && !${FindItem[Tiny Dagger].ID}) /varset _error TRUE
		/if (${validate_class[FALSE, |MAG]} && !${FindItem[Malachite].ID}) /varset _error TRUE
		/if (${validate_class[FALSE, |NEC|SHD]} && !${FindItem[Bone Chips].ID}) /varset _error TRUE
		
		/if (${_error}) {
			OUT Missing pet component. 
			OUT Shutting pets off 'till you go buy some.
			OUT Don't forget to turn back on.
			/call writeINI switchPcPet Pet FALSE
			/return
		}
	}

	VOUT Building ${sep} ${csp}${PcPettype}\ax
	/if (!${Me.Pet.ID}) {
		/if (${validate_cast[FALSE, spell, "${PcPettype}", ${Me.ID}]}) /call core_cast2 "${PcPettype}" ${buffGem} 0 FALSE
		/call create_timer timer_check_buffs_pet 20
		/delay 20s !${Me.Casting.ID}
		/pet hold on
		/pet ghold on
	}
	
	/if (${coreAuto} && ${switchPcPet}) { 
		/if (${Bool[${PetTank}]}) { 
			/pet taunt on
		} else /if (!${Bool[${PetTank}]}) { 
			/pet taunt off
		}
	}
	/call send_pet
/return



|***
 *	DES: Shrinks pet.. wtf did you expect..
 *	USE: /call cast_shrink_pet
 *	INI: Pet - PetShrink
 *  NOTE: 
 ***|
sub cast_shrink_pet()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	DEBUG cast_shrink_pet()

	/if (${check_engaged[]}) /return
	/if (${Me.Pet.ID} && ${Bool[${shrinkPet}]} && ${Me.Pet.Height} > 1.40) {
		/if (${Me.Book[${shrinkPet}]}) {
			/if (${validate_cast[FALSE, spell, "${shrinkPet}", ${Me.Pet.ID}]}) /call core_cast2 "${shrinkPet}" ${buffGem} ${Me.Pet.ID} FALSE
		} else /if (${Me.ItemReady[${shrinkPet}]}) {
			/if (${validate_cast[FALSE, item, "${shrinkPet}", ${Me.Pet.ID}]}) /call core_cast2 "${shrinkPet}" item ${Me.Pet.ID} FALSE
		} else /if (${Me.AltAbilityReady[${shrinkPet}]}) {
			/if (${validate_cast[FALSE, alt, "${shrinkPet}", ${Me.Pet.ID}]}) /call core_cast2 "${shrinkPet}" alt ${Me.Pet.ID} FALSE
		}
	}
/return



|***
 *	DES: fixes pet for.. umm.. pet stuff.. yah that..
 *	USE: /call check_pet
 *	INI: 
 *  NOTE: chanter pets sword ID=10855
 ***|
sub check_pet()
	AMIDEAD
	CHECKEXIT
	DEBUG check_pet()

	/if (!${Me.Pet.ID}) /call cast_summon_pet
	/if (${switchBuffPet} && !${timer_check_buffs_pet}) /call check_buffs_pet
	/if (${Me.Pet.ID} && ${useSummonWeps} && (!${Bool[${Me.Pet.Equipment[primary]}]} || !${Bool[${Me.Pet.Equipment[offhand]}]} || ${Me.Pet.Equipment[primary]} == 10855)) {
		/if (!${Select[${Me.Class.ShortName},MAG]} && !${Bool[${FindItem[${summonedWepClosedBag}]}]}) {
			/call tell_resident_mage weapons
		} else /if (${Select[${Me.Class.ShortName},MAG]} && !${Bool[${FindItem[${summonedWepClosedBag}]}]}) {
			/call cast_summon_weapons
		}
	}	

	CHECKTIE
	CHECKINPUTS
	/if (${Me.Pet.ID} && ${useSummonWeps} && (!${Bool[${Me.Pet.Equipment[primary]}]} || ${Me.Pet.Equipment[primary]} == 10855) && (${Bool[${FindItem[${summonedWepClosedBag}]}]} || ${Bool[${FindItem[Pouch of Quellious]}]})) {
		CLEARCURSOR TRUE
		/if (${Bool[${FindItem[${summonedWepClosedBag}]}]} || ${Bool[${FindItem[Pouch of Quellious]}]}) /call cast_summon_weapons
	}
		
/return



|***
 *	DES: Summon and equip pet weapons
 *	USE: 
 *	INI: INI/Pet/useSummonWeps [TRUE|FALSE]
 *  NOTE: MAG,ENG,BST,NEC
 ***|
sub cast_summon_weapons()
	AMIDEAD
	CHECKINPUTS
	DEBUG cast_summon_weapons()

	/declare _count int local 0
	
	/if (!${Me.Pet.ID}) /return
	/if (${Select[${Me.Class.ShortName},MAG]} && !${FindItem[${summonedWepClosedBag}].ID} && !${FindItem[${summonedWepName}].ID}) {
		/if (${validate_cast[FALSE, spell, "${summonedWepSpell}", ${Me.ID}]}) /call core_cast2 "${summonedWepSpell}" ${buffGem} ${Me.ID} FALSE
		/delay 20s !${Me.Casting.ID}
		/delay 5
		/call sort_cursor TRUE
	}
	/delay 5
	
	/if (${Bool[${FindItem[${summonedWepClosedBag}]}]}) {
		AMIDEAD
		/while (${Cursor.ID}) {
			/autoinventory
			/delay 1s !${Cursor.ID}
		}
		/squelch /exchange ${FindItem[${summonedWepClosedBag}].ID} pack${PetBagSlotNumber}
		/call sort_cursor TRUE
		/delay 2s !${Cursor.ID}
		/delay 5
		/if (${validate_cast[FALSE, item, "${summonedWepClosedBag}", ${Me.ID}]}) /call core_cast2 "${summonedWepClosedBag}" item 0 FALSE
		/delay 3s !${Me.Casting.ID}
		/delay 5
		/delay 2s
		/call sort_cursor TRUE		
		/delay 5

		/if (${Bool[${FindItem[Pouch of Quellious]}]}) {
			AMIDEAD
			/nomodkey /itemnotify ${FindItem[Pouch of Quellious].InvSlot} rightmouseup
			/delay 1s
			/for _count 1 to 2
				/if (${Bool[${FindItem[${summonedWepName}]}]}) {
					/nomodkey /itemnotify ${FindItem[${summonedWepName}].InvSlot} leftmouseup
					/delay 3s ${Bool[${Cursor.ID}]}
					/delay 1s
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

			/while (${Bool[${FindItem[Pouch of Quellious]}]}) {
				/delay 1s
				/nomodkey /itemnotify ${FindItem[=Pouch of Quellious].InvSlot} leftmouseup
				/delay 1s ${Cursor.Name.Equal["Pouch of Quellious"]}
				/delay 1s
				/if (${Cursor.Name.Equal["Pouch of Quellious"]}) /destroy
			}
		}
		
		| cose the window
		/while (${Window[InventoryWindow].Open}) {
			AMIDEAD
			/nomodkey /keypress inventory
			/delay 1s !${Window[InventoryWindow].Open}
		}

	}
/return



|***
 *	DES: keep exp at a specified level then max aa
 *	USE: 
 *	INI: INI/common/MaintExpLvl
 *  NOTE: 
 ***|
sub auto_adjust_EXP()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	DEBUG auto_adjust_EXP()

	/call create_timer timer_check_EXP ${tCheck_EXP}
	
	| set EXP cause we deaded too much
	/if (${Me.Level} <= ${MaxLevel} && ${Me.PctExp} <= ${MaintExpLvl}) {
		/if (${switchDispEXP}) VOUT ${ocaution} ${sep} ${cinfo}${Me.PctExp}%\ax below Maintain EXP [${cinfo}${MaxLevel}\ax] ${sep} ${cinfo}${MaintExpLvl}%\ax
		/alternateadv off
	| set to AA cause the group healer is awesom	
	} else /if (${Me.Level} == ${MaxLevel} && ${Me.PctExp} >= ${MaintExpLvl} && ${Me.AAPoints} < ${Float[${Math.Calc[${Me.Level}*2]}].Int}) {
		| VOUT Maintain EXP [${cinfo}${MaxLevel}\ax] ${sep} ${cinfo}${MaintExpLvl}%\ax. AA:${cinfo}${Me.AAPoints} < ${Float[${Math.Calc[${Me.Level}*2]}].Int}\ax 
		/alternateadv on 100
	| set back to exp cause nothing else to buy
	} else /if (${Me.Level} == ${MaxLevel} && ${Me.PctExp} >= ${MaintExpLvl} && (${Me.AAPoints} == ${Float[${Math.Calc[${Me.Level}*2]}].Int})) {
		/alternateadv off
	}
/return



|***
 *	DES: Check for running auras, If they are duped, remove the second one
 *	USE: /call check_aura
 *	INI: 
 *  NOTE: 
 ***|
sub check_aura()
	CHECKCOMBATCAST
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	DEBUG check_aura()

	/call create_timer timer_check_aura ${tCheck_Aura}
	UPDATEHUD "BUFF Aura"
	
	| Aura: berzerker, monk, warrior
	/if (${validate_class[FALSE, |BER|MNK|WAR]}) {
		/if (${Bool[${Aura1Buff}]} && !${Bool[${Me.Aura[1].Name.Equal[${Aura1Buff}]}]}) {
			VOUT Disc ${sep} ${csp}${Aura1Spell}\ax
			/disc ${Aura1Spell}
			/delay 8s
		}
		/return
	}
	
	| Aura: Bard
	/if (${validate_class[FALSE, |BRD]}) {
		/if (${Bool[${Aura1Buff}]} && !${Bool[${Me.Aura[1].Name.Equal[${Aura1Buff}]}]}) {
			/call fix_gem_id "${buffGem}"
			/if (!${Defined[_gem]}) /declare _gem int local ${Macro.Return}
			/call mem_spell FALSE "${Aura1Buff}" "${_gem}" TRUE
			VOUT Pulse ${sep} ${csp}${Me.Gem[${_gem}].Name}\ax
			/melody ${_gem}
			/delay ${Spell[${Me.Gem[${_gem}].Name}].CastTime.Seconds}s !${Me.Casting.ID}
			/delay 2 

			:stoppulse
			/if ((${Bool[${Me.Aura[1].Name.Equal[${Aura1Buff}]}]} || ${Me.Song[${Me.Gem[${_gem}].ID}]}) && (${Me.Casting.ID} == ${Me.Gem[${_gem}].ID})) {
				/delay 2
				/melody ${_gem}	
				/goto :stoppulse
			}	
		}
		/return
	}

	/declare _count int local
	/declare _aura int local
	/declare _found bool false

	/for _aura 1 to 2
		CHECKCOMBATCAST
		/if (${Bool[${Aura${_aura}Spell}]}) {
			/for _count 1 to 2	
				CHECKCOMBATCAST
				/if (${Bool[${Aura${_aura}Buff}]}) {
					/if (${Bool[${Me.Aura[${_count}].Name.Equal[${Aura${_aura}Buff}]}]}) /varset _found TRUE
					/if (${Bool[${Me.Aura[${_count}].Name.Equal[${Aura${_aura}Spell}]}]}) /varset _found TRUE
					/if (${Bool[${Me.Aura[${_count}].Name.Equal[${Ini[${INIClass},Aura,Aura${_aura}Spell]}]}]}) /varset _found TRUE
					/if (${Bool[${Me.Aura[${_count}].Name.Equal[${Ini[${INIClass},Aura,Aura${_aura}Buff]}]}]}) /varset _found TRUE
				}
			/next _count
			/if (!${_found}) {
				/if (${validate_cast[FALSE, spell, "${Aura${_aura}Spell}", ${Me.ID}]}) /call core_cast2 "${Aura${_aura}Spell}" ${buffGem} ${Me.ID} FALSE
			} else /if (${_found}) {
				/varset _found FALSE
			}
		}
	/next _aura
	
	UPDATEHUD FALSE
/return



|***
 *	DES: looks for clickies in your inventory and adds them to toons INI
 *	USE: /call intialize_item_buffs
 *	INI: 
 *  NOTE: 
 ***|
sub intialize_item_buffs()
	AMIDEAD
	CHECKINPUTS
	DEBUG intialize_item_buffs()

	/declare _tmpSlot         														     						int			local 0
	/declare _tmpContItem																									int			local 0
	/declare _tmpClickItemList																						string	local ${Ini[${INIClass},ClickItems]}
	/if (!${Defined[ClickItem]}) /declare ClickItem												int			outer 1
	/if (!${Defined[UseItem]}) /declare UseItem[40]                  			string	outer 
	/if (!${Defined[SelfItem]}) /declare SelfItem[${UseItem.Size}]   			string 	outer 
	/if (!${Defined[SpellItem]}) /declare SpellItem[${UseItem.Size}]   		string	outer 

	/for _tmpSlot 1 to 32
		AMIDEAD
		/if (${Bool[${InvSlot[${_tmpSlot}].Item.Container}]}) {
			/for _tmpContItem 1 to ${InvSlot[${_tmpSlot}].Item.Container}
				AMIDEAD
				/if (${Me.Inventory[${_tmpSlot}].Item[${_tmpContItem}].ID} && ${Me.Inventory[${_tmpSlot}].Item[${_tmpContItem}].Spell.ID} && ${Me.Inventory[${_tmpSlot}].Item[${_tmpContItem}].EffectType.Equal[Click Inventory]} && ${Me.Level}>=${Me.Inventory[${_tmpSlot}].Item[${_tmpContItem}].RequiredLevel}) {
					/if (${Bool[${_tmpClickItemList.Find[${Me.Inventory[${_tmpSlot}].Item[${_tmpContItem}].Spell}]}]} && ${Bool[${Ini[${INIClass},ClickItems,${Me.Inventory[${_tmpSlot}].Item[${_tmpContItem}].Spell},TRUE]}]}) { 
					  /varset UseItem[${ClickItem}] TRUE
					  /varset SelfItem[${ClickItem}] ${Me.Inventory[${_tmpSlot}].Item[${_tmpContItem}].Name}
					  /varset SpellItem[${ClickItem}] ${Me.Inventory[${_tmpSlot}].Item[${_tmpContItem}].Spell}		  
					  /varcalc ClickItem ${ClickItem}+1
					} else /if (!${_tmpClickItemList.Find[${Me.Inventory[${_tmpSlot}].Item[${_tmpContItem}].Spell}]}) {
					 	/ini "${INIClass}" "ClickItems" "${Me.Inventory[${_tmpSlot}].Item[${_tmpContItem}].Spell}" "FALSE"
					}
				}
			/next _tmpContItem
		} else /if (!${Bool[${InvSlot[${_tmpSlot}].Item.Container}]} && ${InvSlot[${_tmpSlot}].Item.ID} && ${InvSlot[${_tmpSlot}].Item.Spell.ID} && ${InvSlot[${_tmpSlot}].Item.EffectType.Equal[Click Inventory]} && ${Me.Level}>=${InvSlot[${_tmpSlot}].Item.RequiredLevel}) {
			AMIDEAD
			/if (${Bool[${_tmpClickItemList.Find[${Me.Inventory[${_tmpSlot}].Spell}]}]} && ${Bool[${Ini[${INIClass},ClickItems,${Me.Inventory[${_tmpSlot}].Spell},TRUE]}]}) { 
				/varset UseItem[${ClickItem}] TRUE		
				/varset SelfItem[${ClickItem}] ${InvSlot[${_tmpSlot}].Item}
				/varset SpellItem[${ClickItem}] ${InvSlot[${_tmpSlot}].Item.Spell}
				/varcalc ClickItem ${ClickItem}+1
			} else /if (!${_tmpClickItemList.Find[${Me.Inventory[${_tmpSlot}].Spell}]}) {
				/ini "${INIClass}" "ClickItems" "${InvSlot[${_tmpSlot}].Item.Spell}" "FALSE"
			}
		}
	/next _tmpSlot
/return



|***
 *	DES: Checks current buffs. if one is missing. fix it.
 *	USE: /ib and auto
 *	INI: 
 *  NOTE: 
 ***|
sub check_buffs_item()
	AMIDEAD
	CHECKEXIT
	UPDATEHUD "BUFF Item"
	DEBUG check_buffs_item()

	/declare _count int	local 0
	
	/if (${Me.State.NotEqual[Bind]} && !${Me.Moving}) {
		/for _count 1 to ${ClickItem}
			/if (!${Me.FreeBuffSlots}) /continue
			/if (!${Me.ItemReady[${SelfItem[${_count}]}]}) /continue
			/if (${Me.Buff[${SpellItem[${_count}]}].ID}) /continue
			AMIDEAD
			CHECKTIE
			CHECKINPUTS
			/if (${UseItem[${_count}]}) { 
				/if (${Select[${Me.Class.ShortName},BRD]}) {
					/if (${Me.Casting.ID}) /call pulse_stop
				}
				/if (${validate_cast[FALSE, item, "${SelfItem[${_count}]}", ${Me.ID}]}) /call core_cast2 "${SelfItem[${_count}]}" item 0 FALSE
	  	}
  	/next _count
  }

 	AMIDEAD
	CHECKEXIT
  /if (${Me.ItemReady[${IllusionItem}]} && !${Bool[${Me.Buff[${IllusionBuff}]}]} && ${Spell[${IllusionBuff}].Stacks}) {
		VOUT Item ${sep} ${citem}${IllusionItem}\ax
 		/call MQ2Cast "${IllusionItem}" item 30s
 	}  	

	/if (!${Bool[${timer_Check_Familiar}]}) /call sort_familiar
 	
	/if (${Bool[${BuffRemove}]}) /call remove_unwanted_buffs	
	/if (${coreAuto}) {
		/if (${switchShrink}) /call check_shrink
		/call create_timer timer_check_buffs_item ${tItem_Buff}
	}
	UPDATEHUD FALSE
/return



|***
 *	DES: Checks current buffs. NOT SONGS. IF one is found to match the INI listing per character, it is removed.
 *	USE: /call remove_unwanted_buffs
 *	INI: toon -> [SelfBuff] -> BuffRemove
 *  NOTE: 
***|
sub remove_unwanted_buffs()
	CHECKINPUTS
	DEBUG remove_unwanted_buffs()

	/declare _count int local 0
	/declare _remove string local ${BuffRemove}
	/for _count 1 to ${_remove.Count[|]}
		AMIDEAD
		/if (${Bool[${Me.Buff[${_remove.Arg[${_count},|]}]}]})  {
			VOUT Buff ${sep} ${info}${_remove.Arg[${_count},|]}\ax ${sep} ${oremove}
			/invoke ${Me.Buff[${_remove.Arg[${_count},|]}].Remove}
		}
	/next _count		
/return



|***
 *	DES: Chekcs self buffs. If somethings missing, mem and cast it.
 *	USE: /sb and/or auto
 *	INI: 
 *  NOTE: 
 ***|
sub check_buffs_self()
	DEBUG check_buffs_self()
	/if (${coreAuto}) {
		/if (!${timer_check_buffs_self}) /call create_timer timer_check_buffs_self ${tSelf_Buff}
		/if (${watch_for_death[]}) /return 
	}
	/declare _count int local 0

	| check Unity AA buffs
	/if (${Bool[${${Me.Class.ShortName}AAUnity}]}) {
		/for _count 1 to 7
			UPDATEHUD "BUFF AA Unity"

			/if (!${Bool[${BuffAAUnity${_count}}]}) /continue
			/if (!${Me.AltAbilityReady[${${Me.Class.ShortName}AAUnity}]}) /break
			/if (${Me.CurrentMana} < ${Spell[${${Me.Class.ShortName}AAUnity}].Mana}) /break
			/if (!${Bool[${BuffAAUnity${_count}}]}) /continue
			/if (!${Spell[${BuffAAUnity${_count}}].Stacks}) /continue
			/if (!${Bool[${Me.Buff[${BuffAAUnity${_count}}].Duration}]}) {
				/if (${validate_cast[FALSE, alt, "${${Me.Class.ShortName}AAUnity}", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}AAUnity}" alt 0 FALSE
				/break
			}
		/next _count
	}
		
	| check self buffs
	/for _count 1 to ${setMaxINIBuffCount}
		CHECKCOMBATCAST
		UPDATEHUD "BUFF Self"
		AMIDEAD
		CHECKEXIT
		CHECKTIE
		CHECKINPUTS
		
		| is theere a buff there?
		/if (!${Bool[${Buff${_count}}]}) /continue		
		| self buff ?
		/if (!${Bool[${Buff${_count}self}]}) /continue		
		| stacks?
		/if (!${Spell[${Buff${_count}}].Stacks}) /continue		
		| already there?
		/if (${Bool[${Me.Buff[${Buff${_count}chk}].Duration}]}) /continue		
		| fail the condition?
		/if (!${cast_condition[Buff${_count}selfCondition]}) /return		
		
		| spell ready?				
		| /if (!${Me.SpellReady[${Buff${_count}}]}) /continue
		/if (${Cursor.ID}) CLEARCURSOR TRUE

		/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop

		/if (${validate_cast[FALSE, FIND, "${Buff${_count}}", ${Me.ID}]}) /call core_cast2 "${Buff${_count}}" FIND ${Me.ID} FALSE

	/next _count

	| check AA
	/if (${Bool[${AANow1}]} || ${Bool[${AANow2}]}) {
		/call check_AA_now
	}

	| check Discs
	/if (${Bool[${DiscNow1}]} || ${Bool[${DiscNow2}]}) {
		/call check_Disc_now
	}

	| wizard mancy line spells
	/if (${validate_class[FALSE, |WIZ]}) {
		/call sort_mancy_lineAA
	}

	/if (!${Bool[${timer_Check_Familiar}]}) /call sort_familiar

	/if (${coreAuto}) {
		| check shrink
		/if (${switchShrink}) /call check_shrink
	}

	| ditch unwanted buffs
	/if (${Bool[${BuffRemove}]}) {
		/call remove_unwanted_buffs	
	}

	| check auras
	/if (${Bool[${Aura1Spell}]} || ${Bool[${Aura2Spell}]}) {
		/if (!${timer_check_aura}) /call check_aura
	}

	/if (${Target.ID}) /squelch /target clear

	UPDATEHUD FALSE	
/return



|***
 *	DES: Checks pets buffs. If somethings missing, mem and cast it.
 *	USE: /pb and/or auto
 *	INI: 
 *  NOTE: this will do AA or Spell
 ***|
sub check_buffs_pet()
	/if (!${Me.Pet.ID}) /return
	CHECKINPUTS
	CHECKCOMBATCAST
	DEBUG check_buffs_pet()

	/if (${coreAuto} || ${switchModePetfarm} || ${switchModeHunt}) {
		/call create_timer timer_check_buffs_pet ${tPet_Buff}
	}

	/declare _count int local 0
	/declare _spellType string local
	
	/for _count 1 to ${PetBuffCount}
		AMIDEAD
		CHECKEXIT
		CHECKTIE
		/if (${check_engaged[]}) /return
		CHECKINPUTS
		CHECKCOMBATCAST
		UPDATEHUD "BUFF Pet"
		/if (${Me.Pet.Buff[${PetBuff${_count}chk}]}) /continue
		
		/if (${Me.Book[${PetBuff${_count}}]}) {
			/if (${validate_cast[FALSE, spell, "${PetBuff${_count}}", ${Me.Pet.ID}]}) /call core_cast2 "${PetBuff${_count}}" ${buffGem} ${Me.Pet.ID} FALSE
		} else /if (${Me.AltAbilityReady[${PetBuff${_count}}]}) {	
			/if (${validate_cast[FALSE, alt, "${PetBuff${_count}}", ${Me.Pet.ID}]}) /call core_cast2 "${PetBuff${_count}}" alt 0 FALSE
		} else /if (${Me.ItemReady[${PetBuff${_count}}]}) {
			/if (${validate_cast[FALSE, item, "${PetBuff${_count}}", ${Me.Pet.ID}]}) /call core_cast2 "${PetBuff${_count}}" item 0 FALSE		
		}

	/next _count
	
	| chekck for pet illusion
	/if (${Bool[${PetIllusion}]} && !${Bool[${Me.Pet.Buff[${PetIllusionBuff}]}]} && ${Me.Pet.Distance} < 100 && ${Spell[${PetIllusionBuff}].Stacks}) /call MQ2Cast "${PetIllusion}" item -targetid|${Me.Pet.ID}
	| shrink the little bastards..
	/if (${Bool[${Me.Pet.ID}]}) /call cast_shrink_pet
	| pet epic click
	/if (${Me.ItemReady[${EpicClick}]} && !${Me.Pet.Buff[Elemental Conjunction]} && !${Me.Pet.Buff[${EpicPetBuff}]} && !${Me.Pet.Buff[Blessing of Unity]} && ${Spell[${EpicPetBuff}].Stacks}) {
		/if (${validate_cast[FALSE, item, "${EpicClick}", ${Me.ID}]}) /call core_cast2 "${EpicClick}" item 0 FALSE
	}
	
	/if (${Target.ID}) /squelch /target clear
	UPDATEHUD FALSE	
/return



|***
 *	DES: checks your toon for buffs that prevent you from casting.
 *	USE: /call check_no_cast_buffs
 *	INI: 
 *  NOTE: this is by toon.
 ***|
sub check_no_cast_buffs()
	AMIDEAD
	DEBUG check_no_cast_buffs()

	/declare _count int local 0
	/for _count 1 to ${noCastWith.Count[|]}
		AMIDEAD
		CHECKINPUTS
		/if (${Bool[${Me.Buff[${noCastWith.Arg[${_count},|]}]}]}) /return FALSE
		/if (${Bool[${Target.Buff[${noCastWith.Arg[${_count},|]}]}]}) /return FALSE
		/if (${Bool[${Me.Song[${noCastWith.Arg[${_count},|]}]}]}) /return FALSE
	/next _count	
/return TRUE



|***
 *	DES: checks your toon and target for buffs that should prevent a thing
 *	USE: /call check_no_do_a_thing_buff
 *	INI: 
 *  NOTE: we assume return FALSE, as in no buffs found. if we find a buff, return TRUE
 ***|
sub check_no_do_a_thing_buff(string _type)
	DEBUG check_no_do_a_thing_buff(${_type})

	/if (!${Bool[${no${_type.Left[1].Upper}${_type.Right[-1].Lower}With}]}) /return FALSE
	/if (!${Bool[${_type}]}) /return FALSE
	/declare _count int local 0

	/for _count 1 to ${no${_type.Left[1].Upper}${_type.Right[-1].Lower}With.Count[|]}
		AMIDEAD
		CHECKINPUTS

		/if (${Bool[${Me.Buff[${no${_type.Left[1].Upper}${_type.Right[-1].Lower}With.Arg[${_count},|]}]}]}) /return TRUE
		/if (${Target.ID}) {
			/if (${Bool[${Target.Buff[${no${_type.Left[1].Upper}${_type.Right[-1].Lower}With.Arg[${_count},|]}]}]}) /return TRUE
		}
		/if (${Bool[${Me.Song[${no${_type.Left[1].Upper}${_type.Right[-1].Lower}With.Arg[${_count},|]}]}]}) /return TRUE
	
	/next _count	
/return FALSE



|***
 *	DES: checks and clicks minature horn of unity
 *	USE: /horn or /onoff horn
 *	INI: Toon -> [Common] -> switchHorn
 *  NOTE: 
 ***|
sub click_horn_of_unity()
	CHECKINPUTS
	DEBUG click_horn_of_unity()

	/if (!${Bool[${switchHorn}]} || ${SafeZone} && !${switchOverrideSafezone} || ${Bool[${timer_zone_pause}]}) /return
	/if (!${Bool[${Me.Song[Blessing of Unity]}]} && (${Me.PctHPs} < 95*.${tmpHealOverride} || ${Me.PctMana} < 95*.${tmpHealOverride})) {
		/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop
		/if (${validate_cast[FALSE, item, "Miniature Horn of Unity", ${Me.ID}]}) /call core_cast2 "Miniature Horn of Unity" item 0 FALSE
	}
/return



|***
 *	DES: Broadcasted Buff Routines
 *	USE: automatic any peers
 *	INI: 
 *  NOTE: 
  ***|
sub check_buffs_BC()
	CHECKCOMBATCAST
	DEBUG check_buffs_BC()

	/if (${coreAuto}) /call create_timer timer_check_buffs_BC ${tBC_Buff}
	UPDATEHUD "BUFF Broadcast"	

	/declare _toon int local 1
	/declare _buff int local 1
	/declare _toonName string local FALSE
	/declare _spellID int local 
	/declare _loop int local 1
	
	| how the fuck are we gonna count the loops?
	/if (${setCommunications.Equal[eqbc]}) {
		/declare _loopCount int local ${Int[${Math.Calc[${NetBots.Client.Count[ ]}+1]}]}
	} else /if (${setCommunications.Equal[dannet]}) {
		/declare _DNqout string local
		/declare _DNarray string local ${DanNet.Peers[all]}
		/declare _loopCount int local ${Int[${Math.Calc[${_DNarray.Count[|]}]}]}
	}
	

	| each toon
	/while (${_toon} <= ${_loopCount}) {
		
		| get the name to something usable
		/if (${setCommunications.Equal[eqbc]}) {
			/varset _toonName ${NetBots.Client[${_toon}]}
		} else /if (${setCommunications.Equal[dannet]}) {
			/if (${DanNet.FullNames}) {
				/varset _toonName ${_DNarray.Arg[${_toon},|].Right[-${Int[${Math.Calc[${MacroQuest.Server.Length}+1]}]}]}
			} else {
				/varset _toonName ${_DNarray.Arg[${_toon},|]}
			}
		}
	
		| healers ignore the rampage tank
		/if (${validate_class[FALSE, |CLR|PAL|DRU|SHM]}) {
			/if (${Bool[${RampageTank}]} && ${RampageTank.Equal[${_toonName}]}) /continue
		}


		| each buff. cycle all ${setMaxINIBuffCount}, used or not
		/for _buff 1 to ${setMaxINIBuffCount}
			CHECKCOMBATCAST
			
			| make sure the count is in the correct range
			/if (!${Range.Between[1,${setMaxINIBuffCount}:${Int[${_buff}]}]}) /continue
			AMIDEAD
			CHECKTIE
			CHECKINPUTS

			| no BC Buff
			/if (!${Bool[${Buff${_buff}}]}) /continue
			
			| bcbuff someone else bitches...
			/if (${_toonName.Equal[${Me.DisplayName}]}) /continue
	
			| no class to buff?
			/if (!${Bool[${BCBuff${_buff}Class}]}) /continue

			| no use this BC buff in raids
			/if (${Raid.Members} && ${Buff${_buff}noBCinRaid}) /continue

			| buffee is dead?			
			/if (!${Bool[${Spawn[pc ${_toonName}].ID}]} || ${Spawn[pc ${_toonName}].Dead}) /continue
			
			| targets class not listed
			/if (!${BCBuff${_buff}Class.Find[${Spawn[pc ${_toonName}].Class.ShortName}]}) /continue

			| bufeee out of range
			/if (${Spawn[pc ${_toonName}].Distance} > ${Spell[${Buff${_buff}}].MyRange}) /continue
			
			| buffee out of AE range
			/if (${Spell[${Buff${_buff}}].TargetType.Equal[Group V2]}) {
				/if (${Spawn[pc ${_toonName}].Distance} > ${Spell[${Buff${_buff}}].AERange}) /continue
			}

			| is the spell ready?
			/if (!${Me.SpellReady[${Buff${_buff}}]} && ${Me.Gem[${Buff${_buff}}]}) /continue

			| stacking?
			/if (${setCommunications.Equal[eqbc]}) {
				/if (!${NetBots[${NetBots.Client[${_toon}]}].Stacks[${Spell[${Buff${_buff}chk}].ID}]}) /continue
			
			} else /if (${setCommunications.Equal[dannet]}) {
				/dquery ${_toonName} -q "Me.FreeBuffSlots" -o _DNqout
				/if (${_DNqout.Equal[0]}) /continue
								
				/dquery ${_toonName} -q "Spell[${Buff${_buff}chk}].Stacks" -o _DNqout
				/if (!${_DNqout}) /continue

				| has the buff?
				/dquery ${_toonName} -q "Me.Buff[${Buff${_buff}chk}].Duration" -o _DNqout
				/if (${Bool[${_DNqout}]}) /continue
			}
			
			
			/if (${Me.CurrentMana} > ${Spell[${Buff${_buff}}].Mana}) { 
				| bards STFU
				/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop

				| can we cast the spell already				
				/if (${validate_cast[FALSE, spell, "${Buff${_buff}}", ${Spawn[pc ${_toonName}].ID}]}) /call core_cast2 "${Buff${_buff}}" ${buffGem} ${Spawn[pc ${_toonName}].ID} FALSE
			}
		/next _buff
		
		| keep the line moving please...
		/varcal _toon ${_toon}+1
	}	

	UPDATEHUD FALSE
/return



|***
 *	DES: make sure you have ammo
 *	USE: /call check_ammo
 *	INI: [common] -> RangeItem & RangeItemSummon
 *  NOTE:
 ***|
sub check_ammo()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	DEBUG check_ammo()

	/call create_timer timer_Check_Ammo ${tCheck_Ammo}
	/declare _count int local 0
	
	| enumerate the |'s
	/for _count 1 to ${RangeItem.Count[|]}

		| is there an entry for this loop?
		/if (!${Bool[${RangeItem.Arg[${_count},|]}]} || !${Bool[${RangeItemSummon.Arg[${_count},|]}]}) /continue

		| do we have enough of the summoned item
		/if (${FindItemCount[=${RangeItemSummon.Arg[${_count},|]}]} > ${RangeItemCount}) /continue
		
		/call sort_cursor TRUE
		
		| sort what type of item we need to get
		/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop
		/if (${Me.Book[${RangeItem.Arg[${_count},|]}]}) {
			/if (${validate_cast[FALSE, , "${RangeItem.Arg[${_count},|]}", ${Me.ID}]}) /call core_cast2 "${RangeItem.Arg[${_count},|]}" ${buffGem} ${Me.ID} FALSE
		} else /if (${Me.ItemReady[${RangeItem.Arg[${_count},|]}]}) {
			/if (${validate_cast[FALSE, item, "${RangeItem.Arg[${_count},|]}", ${Me.ID}]}) /call core_cast2 "${RangeItem.Arg[${_count},|]}" item 0 FALSE
		} else /if (${Me.AltAbilityReady[${RangeItem.Arg[${_count},|]}]}) {
			/if (${validate_cast[FALSE, alt, "${RangeItem.Arg[${_count},|]}", ${Me.ID}]}) /call core_cast2 "${RangeItem.Arg[${_count},|]}" alt ${Me.ID} FALSE
		} else /if (${Me.CombatAbilityReady[${RangeItem.Arg[${_count},|]}]} || ${Select[${Me.Class.ShortName},BER]}) {
			/if (${validate_cast[FALSE, disc, "${RangeItem.Arg[${_count},|]}", ${Me.ID}]}) /call core_cast2 "${RangeItem.Arg[${_count},|]}" disc ${Me.ID} FALSE
		}
	/next _count
/return



|***
 *	DES: Baby got back.
 *	USE: /call check_Shrink
 *	INI: [common] - shrinkItem
 *  NOTE: no, i don't know why the mage will not shrink.
 ***|
sub check_shrink()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	DEBUG check_shrink()

	/if (${Me.Height} > 2.04) {
		/if (${Me.Book[${shrinkItem}]}) {
			/if (${validate_cast[FALSE, spell, "${shrinkItem}", ${Me.ID}]}) /call core_cast2 "${shrinkItem}" ${buffGem} ${Me.ID} FALSE
		} else /if (${Me.ItemReady[${shrinkItem}]}) {
			/if (${validate_cast[FALSE, item, "${shrinkItem}", ${Me.ID}]}) /call core_cast2 "${shrinkItem}" item ${Me.ID} FALSE
		}
	}
/return



|***
 *	DES: create misc timer
 *	USE: /call create_timer [TIMER VARIABLE NAME] [DURATION]
 *	INI: [Timer]
 *  NOTE: this will define and populate if the variable does not exist. if it exists, it will adjust the time acordingly
 ***|
sub create_timer(string _my_timer, string _my_duration)
	DEBUG create_timer(${_my_timer}, ${_my_duration})
	/if (${verbose} && !${Bool[${_my_timer}]}) VOUT ${ocaution} ${sep} BAD TIMER CREATION. ${cinfo}[\ax${cbad}no name\ax${cinfo}]\ax
	/if (${verbose} && !${Bool[${_my_duration}]}) VOUT ${ocaution} ${sep} BAD TIMER CREATION.  ${sep} ${_my_timer}${sep} ${cinfo}[\ax${cbad}no duration\ax${cinfo}]\ax
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
	CHECKINPUTS
	DEBUG do_rest()
	/if (!${coreAuto} || ${switchAmIDead}) /return
	/if (${SetTieLock} && ${SetTie}) /return
	
	| clerics and pallys wait 'till yaulp gone
	/if (${Bool[${Me.Buff[Lesser Yaulp]}]} || ${Bool[${Me.Buff[Yaulp]}]}) /return
	

	/if (${HomeSet} && !${SpawnCount[npc radius ${CombatDistance}*2 playerstate 4 targetable]} && ((${Me.Y} != ${MakeCamp.AnchorY}) && (${Me.X} != ${MakeCamp.AnchorX}))) {
		/if (${switchPull}) /squelch /makecamp unpause
		/squelch /makecamp return
		/delay 3s ((${Me.Y} == ${MakeCamp.AnchorY}) && (${Me.X} == ${MakeCamp.AnchorX}))
	}

	/declare _needrest bool local FALSE
	/declare _resting bool local FALSE
	
	:loopneedmorerest
	/varset _needrest FALSE
	CHECKREZ
	/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${MobAgro} playerstate 4]} || !${coreAuto}) {
		/if (${Me.State.NotEqual[STAND]}) /stand
		/return
	}

	AMIDEAD
	CLEARCURSOR
	CHECKTIE
	CHECKINPUTS
	| check self click buffs
	/if (!${Me.Invis} && ${switchBuffinRest}) {
		/if (${switchBuffItem} && !${check_engaged[]} && !${timer_check_buffs_item}) /call check_buffs_item
		/if (${switchBuffTell} && !${timer_check_buffs_tell}) /call check_buffs_tell
	}
	
	/if (${switchUseADVL}) {
		/if (${coreAuto} && (${Bool[${Group.Members}]} && ${Group.MasterLooter.ID}==${Me.ID}) && ${Me.UseAdvancedLooting} && !${AdvLoot.LootInProgress} && !${Stick.Active}) /call do_loot
	}
	
	/if (${Bool[${autoRest}]} && !${Me.Casting.ID} && !${Me.Moving} && !${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${MobAgro} playerstate 4]}) {
			
		| Hybrid: RNG,SHD,BST,PAL,BRD
		/if ((${MeleeList.Find[${Me.Class.ShortName}]} && ${CastList.Find[${Me.Class.ShortName}]}) || ${Me.Class.ShortName.Equal[BRD]}) {

			| rest to full requirement
			/if (${switchAutoRestFull} && ${_resting}) {
				/if (${Me.PctEndurance} < 99*.${tmpHealOverride}) /varset _needrest TRUE
				/if (${Me.PctMana} < 99*.${tmpHealOverride}) /varset _needrest TRUE
			} else {
				/if (${Me.PctEndurance} < ${autoRest}*.${tmpHealOverride}) /varset _needrest TRUE
				/if (${Me.PctMana} < ${autoRest}*.${tmpHealOverride}) /varset _needrest TRUE
			}

			| bards suck...
			/if (${Me.Class.ShortName.Equal[BRD]}) {
				/call cast_AA_Rallying
				/if (${switchRestSong}) {
					/if (!${Bool[${Me.Song[${spellRestSong}]}]}) {
						/if (${Me.Casting.ID} != ${Spell[${spellRestSong}].ID}) {
							/call mem_spell FALSE "${spellRestSong}" ${buffGem} TRUE
							/melody ${Me.Gem[${spellRestSong}]}
							/delay 3s !${Me.Casting.ID}
						}
					}
				}
				/if (${switchModRod} && ${switchModRodinRest}) /call use_mod_rod
				/if (${Me.PctEndurance} < ${autoRest}*.${tmpHealOverride}) /varset _needrest TRUE
				/if (${Me.PctMana} < ${autoRest}*.${tmpHealOverride}) /varset _needrest TRUE
				
			} else {
				/if (${switchModRod} && ${switchModRodinRest}) /call use_mod_rod
				/call cast_stamina_recovery
			}
			/if (${Select[${Me.Class.ShortName},PAL]}) {
				/if (${switchHarmonious}) /call check_harmonious
				/if (${switchSteel} && !${timer_check_Steel}) /call check_steel	
			} else /if (${Select[${Me.Class.ShortName},SHD]}) {
				/if (${switchPcPet} && ${switchBuffinRest}) /call check_pet
				/if (${switchHarmonious}) /call check_harmonious
				/if (${switchSkin} && !${timer_check_Skin}) /call check_skin	
			} else /if (${Select[${Me.Class.ShortName},RNG]}) {
			} else /if (${Select[${Me.Class.ShortName},BST]}) {
			} 

		| Melee: WAR,MNK,ROG,BER
		} else /if (${MeleeList.Find[${Me.Class.ShortName}]} && !${CastList.Find[${Me.Class.ShortName}]}) {
			/if (${Me.PctEndurance} < ${autoRest}*.${tmpHealOverride}) /varset _needrest TRUE

			| rest to full requirement
			/if (${switchAutoRestFull}) {
				/if (${Me.PctEndurance} < 99*.${tmpHealOverride}) /varset _needrest TRUE
			}

			/call cast_stamina_recovery
			/if (${Select[${Me.Class.ShortName},WAR]}) {
			} else /if (${Select[${Me.Class.ShortName},MNK]}) {
			} else /if (${Select[${Me.Class.ShortName},ROG]}) {
				/if (${coreAuto} && ${switchSos}) /call automaticSoS
			} else /if (${Select[${Me.Class.ShortName},BER]}) {
			}
			
		| Caster: CLR,DRU,SHM,ENC,WIZ,NEC,MAG
		} else /if (!${MeleeList.Find[${Me.Class.ShortName}]} && ${CastList.Find[${Me.Class.ShortName}]}) {
			/if (${Me.PctMana} < ${autoRest}*.${tmpHealOverride}) {
				/varset _needrest TRUE
				/if (${switchModRod} && ${switchModRodinRest}) /call use_mod_rod
			}
			
			| rest to full requirement
			/if (${switchAutoRestFull}) {
				/if (${Me.PctMana} < 99*.${tmpHealOverride}) /varset _needrest TRUE
				/if (${switchModRod} && ${switchModRodinRest}) /call use_mod_rod
			}			

			/if (${Select[${Me.Class.ShortName},CLR]}) {
				/if (${switchOverrideCombat}) {
					/if ((${switchHealGroup} || ${switchHealXtarget}) && ${SpawnCount[npc radius ${MobAgro} playerstate 4]}) {
						/call check_heal_group_${Me.Class.ShortName}
					}
				}				
				
			} else /if (${Select[${Me.Class.ShortName},DRU]}) {
				/if (${switchHealGroup} && ${switchOverrideCombat}) /call check_heal_group_${Me.Class.ShortName}
			} else /if (${Select[${Me.Class.ShortName},SHM]}) {
				/if (${switchHealGroup} && ${switchOverrideCombat}) /call check_heal_group_${Me.Class.ShortName}
				/if (${Me.PctMana} < 60*.${tmpHealOverride} && !${flagCallNeedRez}) /call check_cani
			} else /if (${Select[${Me.Class.ShortName},ENC]}) {
			} else /if (${Select[${Me.Class.ShortName},WIZ]}) {
				/if (${switchBuffinRest}) /call sort_mancy_lineAA
			} else /if (${Select[${Me.Class.ShortName},NEC]}) {
				/if (${switchPcPet} && ${switchBuffinRest}) /call check_pet
			} else /if (${Select[${Me.Class.ShortName},MAG]}) {
				/if (${switchPcPet} && ${switchBuffinRest}) /call check_pet
			}
		} 			
	}
		
	/if (${_needrest}) {
		UPDATEHUD "Rest"
		/if (${Me.State.NotEqual[SIT]} && !${Bool[${Me.Mount.ID}]}) /sit
		/varset _resting TRUE
		/delay 1
		CHECKTIE
		AMIDEAD
		CHECKINPUTS
		/goto :loopneedmorerest
	} 
	
	UPDATEHUD FALSE
/return



|***
 *	DES: set Home camp spot
 *	USE: /call set_home
 *	INI: 
 *  NOTE: 
 ***|
sub set_home()
	DEBUG set_home()
	/if (${HomeSet}) {
		/varset SetTie FALSE
		VOUT ${cgood}Camp\ax R(${cnum}${HomeRadius}\ax) L(${cnum}${HomeLeash}\ax) ${sep} Y(${cnum}${Me.Y}\ax) X(${cnum}${Me.X}\ax)
		/squelch /makecamp on radius ${HomeRadius} leash ${HomeLeash} mindelay ${HomeDelayMin} maxdelay ${HomeDelayMax}		
	} else /if (!${HomeSet}) {
		VOUT ${ocaution} ${sep} No Camp established.
		/squelch /makecamp off
	}
	/squelch /moveto unpause
	/squelch /stick off
/return



|***
 *	DES: stops follow and re-establishes camp if needed. This stops everything.. seriously.. everything.
 *	USE: /stop or /alto or /squelch stop or /call set_stop [NOECHO]
 *	INI: 
 *  NOTE: just because you stop something, does not mean you will not start it right back up again.
 ****|
sub set_stop(string _echo)
	DEBUG set_stop(${_echo})
	UPDATEHUD "Stop"
	/varset combat FALSE
	/if (!${_echo.Equal[NOECHO]}) VOUT I was told to ${cbad}STOP\ax !! 
	
	| seriously. stop casting
	/if (${Select[${Me.Class.ShortName},BRD]}) {
		/call pulse_stop
	} else {
		/if (${Me.Casting.ID}) /stopcast
	}
	
	| stop sticking to stuff
	/if (${Bool[${Plugin[MQ2MoveUtils]}]}) {
		/if (${Stick.Active}) /squelch /stick off
	}
	/if (${Bool[${Plugin[MQ2Nav]}]}) {
		/if (${Navigation.Active}) /squelch /nav stop
	}	
	/if (${tempsnapstuck}) {
		/varset tempsnapstuck FALSE
	}
	/if (${SetTie}) {
		/varset SetTie FALSE
	}
	
	| stop advanced path
	
	/if (${Bool[${Plugin[MQ2ADVPath]}]}) {
		/if (${AdvPath.Following}) /squelch /afollow off
	}

	| stop moving
	/if (${Me.Moving}) {
		/keypress forward
		/keypress back
	}
 	 	
	| /call clear_combat
	/if (${Me.Pet.ID} && ${Bool[${Me.Pet.Target}]}) /pet back off
	/doevents flush
	/if (${Bool[${tempHomeSet}]}) {
		/call writeINI HomeSet Movement TRUE 
		/call set_home
	}

	UPDATEHUD FALSE
/return



|***
 *	DES: Clicks usefull 1 off items
 *	USE: /call click_item 
 *	INI: [Items] -> clickItem##
 *  NOTE: 
 ***|
sub click_item()
	DEBUG click_item()
	/declare _count int local 0
	/for _count 1 to 6
		/if (${Spawn[${Target.ID}].PctHPs} < 5*.${tmpHealOverride}) /break
		/if (${is_target_dead[${Target2Kill}]}) /break
		/if (!${Bool[${clickItem${_count}}]}) /continue
		AMIDEAD
		CHECKTIE
		CHECKINPUTS
		/if (${validate_cast[FALSE, item, "${clickItem${_count}}", ${Me.ID}]}) /call core_cast2 "${clickItem${_count}}" item 0 FALSE
	/next _count
/return



|***
 *	DES: Clicks swarmpet items
 *	USE: /call click_item_swarm [TARGETID]
 *	INI: [Items] -> clickItemSwarm##
 *  NOTE: 
 ***|
sub click_item_swarm(int _tmpID)
	DEBUG click_item_swarm(${_tmpID})
	/declare _count int local 0
	/for _count 1 to 4
		/if (${Spawn[${Target.ID}].PctHPs} < 5*.${tmpHealOverride}) /break
		/if (${is_target_dead[${_tmpID}]}) /break
		AMIDEAD
		CHECKTIE
		CHECKINPUTS
		/if (${validate_cast[FALSE, item, "${clickItemSwarm${_count}}", ${_tmpID}]}) /call core_cast2 "${clickItemSwarm${_count}}" item ${_tmpID} FALSE
	/next _count
/return



|***
 *	DES: how to handle cursor items
 *	USE: /call sort_cursor || CLEARCURSOR
 *	INI: core.loot.ini
 *  NOTE: 
 ***|
sub sort_cursor(bool _ignoreTimer)
	DEBUG sort_cursor(${_ignoreTimer})
	/if (!${Bool[${_ignoreTimer}]} && ${timer_clear_cursor}) /return
	/if (!${Cursor.ID}) /return
	AMIDEAD
	/declare _cursorChoice string local 

	| we have faith in the all mighty AutoInventoryItem=
	/if (${Bool[${autoInventoryItem.Find[${Cursor.Name}]}]}) /autoinventory 

	/if (!${Bool[${_ignoreTimer}]} && ${Bool[${autoInventoryItem.Find[${Cursor.Name}]}]}) {
		/if (${switchAutoCursor} && ${timer_clear_cursor}) /return
		/if (!${switchAutoCursor} && !${Bool[${autoInventoryItem.Find[${Cursor.Name}]}]}) /return
		/if (!${Cursor.ID}) /return
	}
	
	| allow for absence of override for not in same zone as master looter
	/if (!${switchAutoCursor} && !${switchOverrideLoot} && !${Bool[${_ignoreTimer}]}) /return
	
	
	/if (${Cursor.ID}) {
		CHECKINPUTS

		| corect the INI entry if not set for the new form
		/if (!${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}].Length}) {
			/ini "${INILoot}" "${Cursor.Name.Left[1]}" "${Cursor.Name}" "|Keep"
		} else /if (!${Ini[${INILoot},"${Cursor.Name.Left[1]}","${Cursor.Name}"].Count[|]}) {
			VOUT Fixing Loot: ${Cursor.Name}
			/ini "${INILoot}" "${Cursor.Name.Left[1]}" "${Cursor.Name}" |${Ini[${INILoot},"${Cursor.Name.Left[1]}","${Cursor.Name}"]}
		}		
		
		/if (${Cursor.ID}) /varset _cursorChoice ${Ini[${INILoot},"${Cursor.Name.Left[1]}","${Cursor.Name}"].Arg[1,|]}

		/if (${Bool[${autoInventoryItem.Find[${Cursor.Name}]}]} || ${Select[${_cursorChoice},Keep,Sell,Announce]}) {
			/if (${Select[${_cursorChoice},Announce]}) ECHOCHANNEL - Cursor .. ${Cursor.Name}
			/if (${Cursor.ID}) VOUT Cursor ${sep} ${okeep} ${sep} ${cinfo}${Cursor.Name}\ax ${sep} (${cnum}${FindItemCount[=${Cursor.Name}]}\ax)
			/if (${Cursor.ID}) /nomodkey /autoinventory
		} else /if (${Select[${_cursorChoice},Destroy]}) {
			/if (${Cursor.ID}) VOUT Cursor ${sep} ${odestroy} ${sep} ${cinfo}${Cursor.Name}\ax
			/if (${Cursor.ID} && ${_cursorChoice.Equal[Destroy]}) /nomodkey /destroy
		} else /if (${Select[${_cursorChoice},Leave]}) {
			/if (${Cursor.ID}) VOUT Cursor ${sep} ${oleave} ${sep} ${cinfo}${Cursor.Name}\ax
			/if (${Cursor.ID} && ${_cursorChoice.Equal[Leave]}) /nomodkey /drop
		} else /if (${Select[${_cursorChoice},Ignore]}) {
			/break
		}
		/delay 1
	} 
	/if (${switchAutoCursor}) /call create_timer timer_clear_cursor ${tClear_Cursor}
/return



|***
 *	DES: XTARGET controls
 *	USE: its in the toons INI
 *	INI: umm several different places.
 *  NOTE: for healers its /heal xtarget
 ***|
sub set_xtarget()
	DEBUG set_xtarget()
	/if (!${validate_expansion[FALSE, |16]}) /return	
	/if (!${Select[${Me.Class.ShortName},SHM,CLR,PAL,DRU]}) {
		/call echo_bitch MQWindow
		/if (${Defined[switchHealXtarget]}) /call writeINI switchHealXtarget Heal FALSE 
		/return
	}
	
	VOUT Sorting XTargets ${sep} ${cinfo}${tXHeal}\ax
	/declare _x 				int 		local
	/declare _b 				int 		local
	/declare _count 		int			local 0
	/declare _name			string	local

	| reset extended target slots	
	/for _count 1 to 13
		/if (${Bool[${toonControl}]} && ${Bool[${toonControlXTslot}]} == ${_count} && ${toonControlProtect}) /continue
		/xtarget set ${_count} Autohater
	/next _count
	
	/call create_timer timer_set_xtarget ${tXHeal}
		
	| bail if we are not healing the slackers and xtarget is clear
	/if (!${switchHealXtarget}) /return

	| set protection for NPCs
	/if (${Bool[${ProtectNPCList}]}) {
		/for _count 1 to ${ProtectNPCList.Count[|]}
			/varset _name ${Spawn[pc ${ProtectNPCList.Arg[${_x},|]}].DisplayName}

			/if (${_b} > 13 || !${Bool[${Spawn[${_name}]}]}) {
			} else /if (${Bool[${Me.XTarget[${_name}]}]}) {
				/varcalc _b ${_b}+1
			} else /if (${Bool[${Spawn[${_name}]}]}) {
				/varcalc _b ${_b}+1
				/xtarget set ${_b} ${Spawn[${_name}].Name}
			}
		/next _count
	}	

	| protect PCs when outside group
	/if (!${Raid.Members} && ${Bool[${ProtectOutsideGroup}]}) {
		/for _x 1 to ${ProtectOutsideGroup.Count[|]}
			/varset _name ${Spawn[pc ${ProtectOutsideGroup.Arg[${_x},|]}].DisplayName}
			/if (${_name.Equal[=${toonControl}]}) /continue
			/if (${Spawn[pc ${_name}].Distance} > ${XTHealRadius}) /continue
			/if (${Group.Member[${_name}].ID}) /continue
			/if (${_b} > 13 || !${Bool[${Spawn[pc ${_name}]}]}) /continue
			/if (${Bool[${Me.XTarget[${_name}]}]}) {
				/varcalc _b ${_b}+1
			} else /if (${Bool[${Spawn[pc ${_name}]}]}) {
				/varcalc _b ${_b}+1
				/xtarget set ${_b} ${Spawn[pc ${_name}].Name}
			}
		/next _x
	}				

	| set raid member classes to protect
	/if (${Raid.Members}) {
		/if (${Bool[${ProtectInsideRaid}]}) {
			/for _x 1 to ${ProtectInsideRaid.Count[|]}
			/varset _name ${Spawn[pc ${ProtectInsideRaid.Arg[${_x},|]}].DisplayName}

				/if (${Spawn[pc ${_name}].Name.Equal[${toonControl}]}) /continue
				/if (${Spawn[pc ${_name}].Distance} > ${XTHealRadius}) /continue
				/if (${Bool[${Group.Member[${_name}]}]}) /continue
				/if (${_b} > 13 || !${Bool[${Spawn[pc ${_name}]}]}) {
				} else /if (${Bool[${Me.XTarget[${_name}]}]}) {
					/varcalc _b ${_b}+1
				} else /if (${Bool[${Spawn[pc ${_name}]}]}) {
					/varcalc _b ${_b}+1
					/xtarget set ${_b} ${Spawn[pc ${_name}].Name}
				}
			/next _x
		}

		| set raid protected classes radius ${XTHealRadius}
		/for _count 1 to ${XHealClass.Count[|]}
			/declare _${AllClassList.Arg[${_count},|]} int local ${SpawnCount[pc ${AllClassList.Arg[${_count},|]} raid radius ${XTHealRadius}]}
			/if (${_${AllClassList.Arg[${_count},|]}} && ${XHealClass.Find[${AllClassList.Arg[${_count},|]}]}) {
				/for _x 1 to ${_${AllClassList.Arg[${_count},|]}}
					/if (${_b} > 13) /break
					/if (${Bool[${Group.Member[${NearestSpawn[${_x}, pc ${AllClassList.Arg[${_count},|]} raid]}]}]} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc ${AllClassList.Arg[${_count},|]} raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc ${AllClassList.Arg[${_count},|]} raid].Name}]}) /continue 
					/varcalc _b ${_b}+1
					/xtarget set ${_b} ${NearestSpawn[${_x}, pc ${AllClassList.Arg[${_count},|]} raid radius ${XTHealRadius}]}
				/next _x
			}		
		/next _count	

	}
	
/return



|***
 *	DES: Sticks you to the target
 *	USE: /call stick_to_target [target ID]
 *	INI: na
 *  NOTE: for the love of whatever you find important, don't touch this ever again...
 ***|
sub stick_to_target()
	AMIDEAD	
	CHECKINPUTS
	DEBUG stick_to_target()
	/if (!${switchCombatMelee}) || !${coreAuto}) /return
	/if (!${validate_plugin[TRUE, |mq2moveutils]}) /return

	| if the stick behind switch is on. we do that over ranged decision combat
	/if (${SetStickBehind}) {
		/call get_assist_target
		/if (${Target.ID} && !${Range.Between[0,${SetFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
			
		/if (!${Stick.Active} && ${Target.ID} && ${isValidTarget} && ${SetStickBehind} && !${Stick.Behind} && !${timer_snap_recheckID_${Target2Kill}} && ${switchCombatMelee}) {
			/squelch /stick id ${Target2Kill} ${CombatDistance} snaproll
		} 

		:checkstuck
		AMIDEAD	
		CHECKINPUTS
		/if (${is_target_dead[${Target2Kill}]}) /return
		/if (${MoveUtils.Stuck}) {
			/delay 1
			/squelch /stick off
			/call create_timer timer_snap_recheckID_${Target2Kill} ${tCheck_snap_retry}
			/return
		}	
			
		/if (${Spawn[${Target2Kill}].Distance} < ${CombatDistance} && !${SetStickBehind} && !${timer_snap_recheckID_${Target2Kill}}) {
			/squelch /stick off
			/return
		}
		/if (!${Stick.Behind} && ${SetStickBehind} && !${timer_snap_recheckID_${Target2Kill}}) /goto :checkstuck

	} else /if (!${SetStickBehind}) {
		/if (${switchModeHunt}) /varset isValidTarget TRUE
		:movetomob
		AMIDEAD	
		CHECKINPUTS
		/if (${is_target_dead[${Target2Kill}]}) /return
		/if (!${Stick.Active} && ${Target.ID} && ${isValidTarget} && ${Spawn[${Target2Kill}].Distance} > ${CombatDistance} && ${switchCombatMelee}) {
			/if (${coreAuto}) {
				/if (!${Range.Between[0,${SetFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
			}

			/if (${coreAuto}) /squelch /stick id ${Target2Kill} ${CombatDistance} 
		}
		/delay 1
		/if (${Spawn[${Target2Kill}].Distance} > ${CombatDistance}) /goto :movetomob
		/if (${Spawn[${Target2Kill}].Distance} < ${CombatDistance}) /squelch /stick off
		/return
	}
	
	/squelch /stick off
/return



|***
 *	DES: Acquire target, get bitches. This will establish ${Target2Kill} for everyone.
 *	USE: /call get_assist_target
 *	INI: 
 *  NOTE: 
 ***|
sub get_assist_target()
	AMIDEAD
	CHECKINPUTS
	DEBUG get_assist_target()

	/if (!${coreAuto}) /return
	/if (${Group.MainAssist.Name.Equal[${Me.DisplayName}]} && !${useForceAssist}) /return
	/if (${useForceAssist} && ${Target2Kill} == ${Me.ID}) /return
	
	| if no MA present. move along..	
	/if (!${Bool[${Spawn[pc ${MA1}].ID}]} && !${Bool[${Spawn[pc ${MA2}].ID}]} && !${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssist3} && !${useSmartAssistG} && !${useForceAssist}) {
		/delay 1
		/return
	} 	
	
	| cannnot assist yourself
	/if (${Spawn[pc ${MA1}].ID} == ${Me.ID}) /call Bind_setCombatControl MA1 FALSE
	/if (${Spawn[pc ${MA2}].ID} == ${Me.ID}) /call Bind_setCombatControl MA2 FALSE
	
	| if we dont have underfoot expansion, skip ALL smartassist go directly to jail. 
	/if (!${validate_expansion[FALSE, |16]}) /goto :skippingtoMAtarget
	/if (${useForceAssist}) /goto :skippingtoMAforcedtarget

	| no, you still cannot assist yourself. get over it
	/if (${Group.MainAssist.Spawn.ID} == ${Me.ID}) /call validate_assist NOECHO X
	/if (${Spawn[=${Raid.MainAssist} pc].ID} == ${Me.ID}) /call validate_assist NOECHO X

	| tar acquisition when using smart assist. set XTarget positions.
	/if (${useSmartAssistG} || ${useSmartAssist1} || ${useSmartAssist2} || ${useSmartAssist3}) {

		| if the raid/group roles are not set or dead, move on tho MA1/2	
		/if (${useSmartAssistG}) {
			/if (!${Bool[${Group.MainAssist}]}) /goto :skippingtoMAtarget
			/if (${Bool[${Spawn[${Group.MainAssist}].Dead}]}) /goto :skippingtoMAtarget
		} else /if (${useSmartAssist1} || ${useSmartAssist2} || ${useSmartAssist3}) {		
		}	
	
		| set the 1st xtarget slot to the assists target
		/if (${useSmartAssistG}) {
			/if (${Me.XTarget[1].TargetType.NotEqual[Group Assist Target]} && ${Group.Members}) /xtarget set 1 groupassisttarget
		} else /if (${useSmartAssist1}) {
			/if (${Me.XTarget[1].TargetType.NotEqual[Raid Assist 1 Target]} && ${Raid.Members}) /xtarget set 1 raidassist1target
		} else /if (${useSmartAssist2}) {
			/if (${Me.XTarget[1].TargetType.NotEqual[Raid Assist 2 Target]} && ${Raid.Members}) /xtarget set 1 raidassist2target
		} else /if (${useSmartAssist3}) {
			/if (${Me.XTarget[1].TargetType.NotEqual[Raid Assist 3 Target]} && ${Raid.Members}) /xtarget set 1 raidassist3target
		}
		
		| smart assist group
		/if (${useSmartAssistG}) {
			| /if (${Bool[${Spawn[${Me.XTarget[1].ID}].Type.Equal[NPC]}]}) {
				/if (${Me.XTarget[1].PctHPs} <= ${smartengageAt} || ${switchOverrideEngageat} || ${switchdeBuffMezz}) {
					/varset Target2Kill ${Me.XTarget[1].ID}
					/delay 1
					/if (${Spawn[id ${Target2Kill}].Distance} <= ${MobAgro} && ${Target.ID} != ${Target2Kill}) {
						/if (${switchCombatMelee} || ${switchCombatRange} || ${switchCombatNuke} || ${switchCombatDoT}) /target ID ${Target2Kill}
					}
					/return
				}
			| }

		| smart assist raid
		} else /if (${useSmartAssist1} || ${useSmartAssist2} || ${useSmartAssist3}) {
			| /if (${Bool[${Spawn[${Me.XTarget[1].ID}].Type.Equal[NPC]}]}) {
				/if (${Me.XTarget[1].PctHPs} <= ${smartengageAt} || ${switchOverrideEngageat}) {
					/varset Target2Kill ${Me.XTarget[1].ID}
					/delay 1
					/if (${Spawn[id ${Target2Kill}].Distance} <= ${MobAgro} && ${Target.ID} != ${Target2Kill}) {
						/if (${switchCombatMelee} || ${switchCombatRange} || ${switchCombatNuke} || ${switchCombatDoT}) /target ID ${Target2Kill}
					}
					/return
				}	
			| }
		}

		/return
	} 

	| target acquisition when not using manual assist.
	:skippingtoMAtarget
	/if (${SpawnCount[npc radius ${MobAgro}]} && (${Bool[${MA1}]} || ${Bool[${MA2}]})) {
		| MA1
		/if (${Bool[${MA1}]} && ${Bool[${Spawn[${MA1}].ID}]} && !${Bool[${Spawn[${MA1}].Dead}]}) {
			/if (${switchCombatMelee} || ${switchCombatRange} || ${switchCombatNuke} || ${switchCombatDoT}) /assist ${MA1}
			/delay 1s ${Me.AssistComplete}
			| /if (!${Target2Kill} && ${Target.ID} && ${Spawn[${Target.ID}].Type.Equal[npc]}) {
			/if (!${Target2Kill} && ${Target.ID}) {
				/varset Target2Kill ${Target.ID}
			} else /if (${Target2Kill} != ${Target.ID} && ${Target2Kill} != 0 && ${Target.ID} && ${Spawn[${Target}].Type.Equal[npc]}) {
				/delay 1
				/varset Target2Kill 0
				/return
			}	
	
		| MA2
		} else /if (${Bool[${MA2}]} && ${Bool[${Spawn[${MA2}].ID}]} && !${Bool[${Spawn[${MA2}].Dead}]} && (!${Bool[${Spawn[${MA1}].ID}]} || ${Bool[${Spawn[${MA1}].Dead}]})) {
			/if (${switchCombatMelee} || ${switchCombatRange} || ${switchCombatNuke} || ${switchCombatDoT}) /assist ${MA2}
			/delay 1s ${Me.AssistComplete}
			| /if (!${Target2Kill} && ${Target.ID} && ${Spawn[${Target.ID}].Type.Equal[npc]}) {
			/if (!${Target2Kill} && ${Target.ID}) {
				/varset Target2Kill ${Target.ID}
			} else /if (${Target2Kill} != ${Target.ID} && ${Target2Kill} != 0 && ${Target.ID} && ${Spawn[${Target}].Type.Equal[npc]}) {
				/delay 1
				/varset Target2Kill 0
				/return
			}	
			
		} 		
		AMIDEAD
		CHECKINPUTS
		| /if (${Target2Kill} == ${Target.ID} && ${Spawn[${Target}].Type.Equal[npc]}) {
		/if (${Target2Kill} == ${Target.ID}) {
			/if (${Target.PctHPs} > ${engageAt} && !${switchOverrideEngageat}) {
				/delay 3
				/goto :skippingtoMAtarget
			} 
		} else {
			/if (${Target.ID}) /squelch /target clear
			/return
		}
	}
	
	| target acquisition when not using manual Forced assist.
	:skippingtoMAforcedtarget	
	/if (${Bool[${useForceAssist}]} && ${Target2Kill} != 0) {
		
		/if (${Spawn[${Target2Kill}].ID}) {
			/if (${Target.ID} != ${Target2Kill} && ${Spawn[${Target2Kill}].ID}) {
				/target ID ${Target2Kill}
			}
 		} else /if (!${Spawn[${Target2Kill}].ID}) {
 		
 		}
		
		AMIDEAD
		CHECKINPUTS
		| /if (${Target2Kill} && ${Spawn[${Target}].Type.Equal[npc]}) {
		/if (${Target2Kill}) {
			/if (${Spawn[${Target2Kill}].PctHPs} > ${engageAt} && !${switchOverrideEngageat}) {
				/delay 3
				/goto :skippingtoMAforcedtarget
			} 
		} else {
			/if (${Target.ID}) /squelch /target clear
			/return
		}
	}
/return



|***
 *	DES: Seriously, do you need to ask?
 *	USE: mob dead = win!
 *	INI: 
 *  NOTE: ass.U.ME the mob is alive until, it is not.
 ***|
sub is_target_dead(int _tmpID)
	AMIDEAD
	DEBUG is_target_dead(${_tmpID})

	/declare _dead bool local FALSE
	
	/if (!${coreAuto}) {
		/if (!${Bool[${_tmpID}]} && ${Target.ID}) /varset _tmpID ${Target.ID}
		/if (!${Bool[${_tmpID}]}) {
			/call clear_combat
			/return TRUE
		}
	}
	
	| no target ID? shits dead 'till we get one to check..
	/if (!${Target.ID} && !${switchModeHunt}) {
		/varset _dead TRUE
	} else /if (${_tmpID} == 0 || !${Bool[${_tmpID}]}) {
		/varset _dead TRUE
	}

	| targets dead? were done here
	/if (${Target.Dead}) {
		/varset _dead TRUE
	| } else /if (${Spawn[_tmpID].ID}) {
	| 	/varset _dead TRUE
	} else /if (${Target.Type.Equal[Corpse]} || ${Spawn[${Target.ID}].Type.Equal[CORPSE]}) {
		/varset _dead TRUE
	} else /if (${Spawn[${_tmpID}]].Dead} || ${Spawn[${Target.ID}].Dead}) {
		/varset _dead TRUE
	| } else /if (!${Spawn[${_tmpID}].ID} || !${Bool[${Spawn[ID ${_tmpID}]}]}) {
	} else /if (!${Spawn[${_tmpID}].ID}) {
		/varset _dead TRUE
	} else /if (${Spawn[${Target.ID}].Type.Equal[PCCORPSE]}) {
		/varset _dead TRUE
	}

	/if (${_dead}) {
		/call clear_combat
	}
/return ${_dead}



|***
 *	DES: Clear all combat variables
 *	USE: /call clear_combat
 *	INI: na
 *  NOTE: 
 ***|
sub clear_combat()
	CHECKINPUTS
	DEBUG clear_combat()

	/if (${validate_class[FALSE, |BRD]}) {
		/call pulse_stop
		/if (${validate_plugin[FALSE, |mq2twist]}) /squelch /twist clear
	}
	/if (${Me.AutoFire}) /squelch /autofire
	/if (${Me.Combat}) /squelch /attack off
	/if (${coreAuto} && ${Bool[${Me.Casting}]} && (!${Spell[${Me.Casting.ID}].Category.Equal[Transport]} || !${Spell[${Me.Casting.ID}].Category.Equal[Heals]})) /stopcast
	/if (${Bool[${Target2Kill}]}) {
		/call clear_timer ${Target2Kill}
		/varset Target2Kill 0
	}
	
	/if (${setBurnControl}) /varset setBurnControl FALSE
	/if (${isValidTarget}) /varset isValidTarget FALSE
	/if (${Stick.Active}) /squelch /stick off
	/if (${coreAuto} && ${Target.ID}) /squelch /target clear
	/if (!${coreAuto} && !${Spawn[${Target.ID}].Type.Equal[CORPSE]}) /squelch /target clear
/return TRUE



|***
 *	DES: Clears all defined timers for a specific ID
 *	USE: /call clear_timer [TARGETID]
 *	INI: na
 *  NOTE: timers that need clearing when the target is deaded
 ***|
sub clear_timer(int _tmpID)
DEBUG clear_timer(${_tmpID})
	CHECKINPUTS
	/if (!${_tmpID}) /return

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
	| stick to mobs ass check
	/if (${Defined[timer_snap_recheckID_${_tmpID}]}) /deletevar timer_snap_recheckID_${_tmpID}
/return



|***
 *	DES: conditions to check for each cast
 *	USE: /if (!${cast_condition[CONDITION]}) /return ??
 *	INI: 
 *  NOTE: DO NOT PASS VARIABLE CONTAINER. JUST PASS ITS' INI DECLARE NAME
 ***|
sub cast_condition(str _condition)
	DEBUG cast_condition(${_condition})
	CHECKINPUTS	
	
	| is the base variable string set to FALSE in the INI? if so. we do not need to check and consider valid
	/if (${${_condition}.Equal[FALSE]}) {
		/return TRUE

	| if there is a condition and it does evaluate as TRUE.
	} else /if (${${_condition}}) {
		/return TRUE
		
	| when in doubt, blame your little sister.	
	} 

/return FALSE



|***
 *	DES: Cast nukes
 *	USE: /call cast_nuke
 *	INI: 
 *  NOTE: "I say we take off and nuke the entire site from orbit." --Ripley
 ***|
sub cast_nuke()
	DEBUG cast_nuke()
	/if (${timer_Nuke_Delay}) /return	
	/call get_assist_target
	/declare _count int local 0
	
	/for _count 1 to ${DoNuke}
		/if (${is_target_dead[${Target2Kill}]}) /return
		/if (!${Range.Between[1,${DoNuke}:${Int[${_count}]}]}) /continue
		/if (!${cast_condition[Nuke${_count}Condition]}) /continue

		AMIDEAD
		CHECKEXIT
		CHECKINPUTS
		/if (${is_target_dead[${Target2Kill}]}) /return
		/if (${Me.CurrentMana} < ${Spell[${Nuke${_count}}].Mana} || ${Me.PctMana} < 2) /continue 
		
		/if (${validate_cast[FALSE, spell, "${Nuke${_count}}", ${Target2Kill}]}) /call core_cast2 "${Nuke${_count}}" 0 ${Target2Kill} cast_break
		/if (${Select[${castReturn},CAST_SUCCESS]}) /call create_timer timer_Nuke_Delay ${tNuke_Delay}

		| toons ini switch. maintain the loop of nuked till all are cycled.
		/if (${forceNukeLoop}) {
			/delay 10s !${Me.Casting.ID}
			/delay 2.5s
			/delay ${timer_Nuke_Delay}
		}
	/next _count
/return



|***
 *	DES: Cast nuke Lines
 *	USE: /call cast_nuke
 *	INI: 
 *  NOTE: "The only good bug is a dead bug" -- Would you like to know more?
 ***|
sub cast_nuke_line()
	DEBUG cast_nuke_line()
	/if (${lineCurrent.Equal[off]}) /return
	/if (${timer_Nuke_Delay}) /return	
	/call get_assist_target
	/declare _count int local 0
	/declare _list string local ${line${lineCurrent}}
	
	/for _count 1 to ${_list.Count[|]}
		/if (!${Range.Between[1,${_list.Count[|]}:${Int[${_count}]}]}) /continue
		AMIDEAD
		CHECKEXIT
		CHECKINPUTS
		
		/if (${is_target_dead[${Target2Kill}]}) /return
		/if (${Me.CurrentMana} < ${Spell[${_list.Arg[${_count},|]}].Mana} || ${Me.PctMana} < 2) /break 
		/if (!${cast_condition[line${lineCurrent}Condition${_count}]}) /continue 
		
		/if (${validate_cast[FALSE, spell, "${Spell[${_list.Arg[${_count},|]}].RankName}", ${Target2Kill}]}) /call core_cast2 "${Spell[${_list.Arg[${_count},|]}].RankName}" 0 ${Target2Kill} cast_break
		/if (${Select[${castReturn},CAST_SUCCESS]}) /call create_timer timer_Nuke_Delay ${tNuke_Delay}

		| toons ini switch. maintain the loop of nuked till all are cycled.
		/if (${forceNukeLoop}) {
			/delay 10s !${Me.Casting.ID}
			/delay 2.5s
			/delay ${timer_Nuke_Delay}
		}
	/next _count
/return



|***
 *	DES: memorize spell line
 *	USE: /call mem_nuke_line [] 
 *	INI: [Spell] -> line[fire|ice|magic|mix]
 *  NOTE:
 ***|
sub mem_nuke_line(string _line)
	DEBUG mem_nuke_line(${_line})
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/declare _index 				int 		local 0
	/declare _list		 			string 	local ${line${lineCurrent}}
	
	| mem twincast spell
	/if (!${Me.Gem[1].Name.Equal[${NukeTwincast${lineCurrent}}]} && ${Bool[${NukeTwincast${lineCurrent}}]}) {
		/call mem_spell TRUE "${NukeTwincast${lineCurrent}}" 1 FALSE
	}	
	
	| mem remainder line of spells
	/for _index 1 to ${_list.Count[|]}
		/if (${Me.Gem[${_index}].Name.Equal[${Spell[${_list.Arg[${_index},|]}].RankName}]}) /continue
		/call mem_spell TRUE "${Spell[${_list.Arg[${_index},|]}].RankName}" ${Int[${Math.Calc[${_index}+1]}]} FALSE
	/next _index	
	VOUT Line ${sep} ${cinfo}${lineCurrent}\ax memorization completed.
/return



|***
 *	DES: Cast DoT's
 *	USE: /call cast_DOT
 *	INI: 
 *  NOTE: 
 ***|
sub cast_DOT()
	DEBUG cast_DOT()
	/call get_assist_target
	/declare _count int local 0
	/for _count 1 to ${DoDoT}
		/if (!${Range.Between[1,${DoDoT}:${Int[${_count}]}]}) /continue
		AMIDEAD
		CHECKEXIT
		CHECKINPUTS
		/if (${is_target_dead[${Target2Kill}]}) /return
		| /if (${Select[${Me.Subscription},SILVER]}) /varset DoT${_count}chk ${Ini[${INIClass},Spells,DoT${_count}chk]}
		/if (!${cast_condition[DoT${_count}Condition]}) /continue 
		/if (${Me.CurrentMana} < ${Spell[${DoT${_count}}].Mana} || ${Me.PctMana} < 2) /continue 

		/if (${Target.BuffDuration[${DoT${_count}chk}].TotalSeconds} >= 3 && ${Spawn[${Target.Buff[${DoT${_count}chk}].Caster}].ID} == ${Me.ID}) /continue
		/if (${Target.Buff[${DoT${_count}chk}].ID}) /continue
		/if (${validate_cast[FALSE, spell, "${DoT${_count}}", ${Target2Kill}]}) /call core_cast2 "${DoT${_count}}" 0 ${Target2Kill} cast_break
	/next _count
/return



|***
 *	DES: casts AE Rain spells. Magician mostly...
 *	USE: /call cast_nuke_rain | /userain [TRUE|FALSE]
 *	INI: 
 *  NOTE: I'd suggest not using this. though it is kind of fun to watch
 ***|
sub cast_nuke_rain()
	DEBUG cast_nuke_rain()
	/if (${timer_Nuke_Delay}) /return	
	/call get_assist_target
	/declare _count int local 0
	/for _count 1 to ${doRainNuke}
		AMIDEAD
		CHECKEXIT
		CHECKINPUTS
		/if (${is_target_dead[${Target2Kill}]}) /return

		/if (${Spawn[${Target2Kill}].LineOfSight} && ${Me.SpellReady[${RainNuke${_count}}]} && ${Spawn[${Target2Kill}].Distance} <= ${Spell[${RainNuke${_count}}].MyRange} && ${Me.CurrentMana} >= ${Spell[${RainNuke${_count}}].Mana}) {
			/if (${validate_cast[FALSE, spell, "${RainNuke${_count}}", ${Target2Kill}]}) /call core_cast2 "${RainNuke${_count}}" 0 ${Target2Kill} cast_break
			/if (${Select[${castReturn},CAST_SUCCESS]}) /call create_timer timer_Nuke_Delay ${tNuke_Delay}
		}
	/next _count
/return



|***
 *	DES: Returns the toons target to the intended kill ID.
 *	USE: /call get_target_2kill
 *	INI: 
 *  NOTE: sometimes people need focus.
***|
sub get_target_2kill()
	DEBUG get_target_2kill()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS

	/if (${is_target_dead[${Target2Kill}]}) /return
	/if (${Target.ID} != ${Target2Kill} && ${Spawn[id ${Target2Kill}].Distance} <= ${MobAgro}) {
		/target ID ${Target2Kill}
	}	
	| /if (${coreAuto} && ${Target.Distance} > ${MobAgro}) /call clear_combat
/return



|***
 *	DES: Uses Entropy of Nature/EradicateMagic/Whatever on target. only once every 60 seconds.
 *	USE: /call cast_AA_eradicate_magic [target ID] & /debuff eradicate
 *	INI: 
 *  NOTE: 
 ***|
sub cast_AA_eradicate_magic(int _tmpID)
	DEBUG cast_AA_eradicate_magic(${_tmpID})
	/if (${timer_eradicate_${_tmpID}}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS

	/if (!${AAEradicateList.Find[${Me.Class.ShortName}]}) {
		VOUT ${onotice} ${sep} Class does not have the AA Eradicate.
		/call writeINI switchdeBuffEradicate Common FALSE
		/call echos switch "deBuff Eradicate" switchdeBuffEradicate	0 0
		/return
	}
	
	/if (!${Defined[_debuffEradicate]}) /declare _debuffEradicate string local
	/if (${validate_class[FALSE, |ENC|MAG|NEC|WIZ]}) /varset _debuffEradicate Eradicate Magic
	/if (${validate_class[FALSE, |RNG|DRU]}) /varset _debuffEradicate Entropy of Nature
	
	/if (${validate_cast[FALSE, alt, "${_debuffEradicate}", ${_tmpID}]}) /call core_cast2 "${_debuffEradicate}" alt ${_tmpID} FALSE
	/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_RESISTED,CAST_RESIST,CAST_NOHOLD,CAST_TAKEHOLD]}) /call create_timer timer_eradicate_${_tmpID} 1m

/return



|***
 *	DES: Uses class specific AA snares
 *	USE: /call cast_AA_snare [target ID] & /debuff snare
 *	INI: na
 *  NOTE: did I mention this is for classes with AA snares..
 ***|
sub cast_AA_snare(int _tmpID)
	DEBUG cast_AA_snare(${_tmpID})
	/if (${timer_snare_${_tmpID}}) /return
	AMIDEAD
	CHECKEXIT

	/if (!${Select[${Me.Class.ShortName},RNG,DRU,WAR,ROG,NEC,SHD,WIZ,MNK,PAL,BER]}) {
		VOUT ${onotice} ${sep} Not an AA snaring class. Delete toon and reroll a ranger.
		/call writeINI switchdeBuffSnare Common FALSE  
		/call echos debuff "Snare" switchdeBuffSnare 0 0	
		/return
	}
	
	| if its snared, move on
	/if (${Target.Snared.ID}) {
		/call create_timer timer_snare_${Target2Kill} 1m
		/return
	}
		
	| set AA for snaring
	/if (${Select[${Me.Class.ShortName},BER]}) {
		/declare _debuffSnare string local Leg Strike	
	} else /if (${Select[${Me.Class.ShortName},DRU,RNG]}) {
		/declare _debuffSnare string local Entrap
	} else /if (${Select[${Me.Class.ShortName},MNK]}) {
		/declare _debuffSnare string local Crippling Strike
	} else /if (${Select[${Me.Class.ShortName},NEC,SHD]}) {
		/declare _debuffSnare string local Encroaching Darkness
	} else /if (${Select[${Me.Class.ShortName},ROG]}) {
		/declare _debuffSnare string local Ligament Slice
	} else /if (${Select[${Me.Class.ShortName},WAR]}) {
		/declare _debuffSnare string local Call of Challenge
	} else /if (${Select[${Me.Class.ShortName},WIZ]}) {
		/declare _debuffSnare string local Atol's Shackles
	}	


	
	/if (!${Target.Snared.ID}) {

		| paladins
		/if (${validate_class[FALSE, |PAL]}) {
			/if (${Target.Body.Name.Equal[Undead]}) {
				/if (${validate_cast[FALSE, alt, "Halt the Dead", ${_tmpID}]}) /call core_cast2 "Halt the Dead" alt ${_tmpID} FALSE
			} else /if (!${Target.Body.Name.Equal[Undead]}) {
				/if (${validate_cast[FALSE, alt, "Shackles of Tunare", ${_tmpID}]}) /call core_cast2 "Shackles of Tunare" alt ${_tmpID} FALSE
			}

		| berzerkers
		} else /if (${validate_class[FALSE, |BER]}) {
			/if (${validate_cast[FALSE, disc, "${_debuffSnare}", ${_tmpID}]}) /call core_cast2 "${_debuffSnare}" disc ${_tmpID} FALSE

		| everyone else
		} else {
			/if (${validate_cast[FALSE, alt, "${_debuffSnare}", ${_tmpID}]}) /call core_cast2 "${_debuffSnare}" alt ${_tmpID} FALSE
		}

		
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_RESISTED,CAST_RESIST,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_snare_${_tmpID} 1m
		} else /if (${Select[${castReturn},CAST_IMMUNE]}) {
			/call create_timer timer_snare_${_tmpID} 4m
		}
	} else /if (${Target.Snared.ID} && !${timer_snare_${_tmpID}}) {
		/call create_timer timer_snare_${_tmpID} 30s
	}
/return



|***
 *	DES: Users class specific Breastplates. Will alternate 2 if you have both. 
 *	USE: /call click_BP
 *	INI: 
 *  NOTE: 
 ***|
sub click_BP()
	DEBUG click_BP()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	
	| is at least one ready?
	/if (!${Me.ItemReady[${Breastplate1}]} && !${Me.ItemReady[${Breastplate2}]}) /return
	
	| do I have one of the effects?
	/if (${Bool[${Me.Song[${Breastplate1effect}]}]} || ${Bool[${Me.Buff[${Breastplate2effect}]}]}) /return
	
	/if (${Me.ItemReady[${Breastplate1}]}) {
		/if (!${cast_condition[Breastplate1Condition]}) /return
		/if (!${Bool[${Me.Buff[${Breastplate2effect}]}]} && ${Spell[${Breastplate1effect}].Stacks}) {
			/if (${validate_cast[FALSE, item, "${Breastplate1}", ${Me.ID}]}) /call core_cast2 "${Breastplate1}" item 0 FALSE
		}
	} else /if (${Me.ItemReady[${Breastplate2}]}) {
		/if (!${cast_condition[Breastplate2Condition]}) /return
		/if (!${Bool[${Me.Song[${Breastplate1effect}]}]} && ${Spell[${Breastplate2effect}].Stacks}) {
			/if (${validate_cast[FALSE, item, "${Breastplate2}", ${Me.ID}]}) /call core_cast2 "${Breastplate2}" item 0 FALSE
		}
	}
/return



|***
 *	DES: Uses a mod rod if you have one in your inventory
 *	USE: /call use_mod_rod
 *	INI: 
 *  NOTE: listed all i could think of. dont want to keep up with this in an INI.
 ***|
sub use_mod_rod()
	DEBUG use_mod_rod()
	/if (${Select[${Me.Class.ShortName},ROG,BER,WAR,MNK]}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
		
	| use all the rods!!
	/if (${Me.PctMana} < 70*.${tmpHealOverride}) {
		/if (${Me.ItemReady[Summoned: Glowing Modulation Shard]} && ${Me.CurrentHPs} > 45500) { 
			/if (${validate_cast[FALSE, item, "Summoned: Glowing Modulation Shard", ${Me.ID}]}) /call core_cast2 "Summoned: Glowing Modulation Shard" item 0 FALSE
		} else /if (${Me.ItemReady[Summoned: Giant Modulation Shard]} && ${Me.CurrentHPs} > 37500) {
			/if (${validate_cast[FALSE, item, "Summoned: Giant Modulation Shard", ${Me.ID}]}) /call core_cast2 "Summoned: Giant Modulation Shard" item 0 FALSE
		} else /if (${Me.ItemReady[Summoned: Large Modulation Shard]} && ${Me.CurrentHPs} > 23100) {
			/if (${validate_cast[FALSE, item, "Summoned: Large Modulation Shard", ${Me.ID}]}) /call core_cast2 "Summoned: Large Modulation Shard" item 0 FALSE
		} else /if (${Me.ItemReady[Summoned: Medium Modulation Shard]} && ${Me.CurrentHPs} > 15100) {
			/if (${validate_cast[FALSE, item, "Summoned: Medium Modulation Shard", ${Me.ID}]}) /call core_cast2 "Summoned: Medium Modulation Shard" item 0 FALSE
		} else /if (${Me.ItemReady[Summoned: Small Modulation Shard]} && ${Me.CurrentHPs} > 7600) {
			/if (${validate_cast[FALSE, item, "Summoned: Small Modulation Shard", ${Me.ID}]}) /call core_cast2 "Summoned: Small Modulation Shard" item 0 FALSE
		} else /if (${Me.ItemReady[Summoned: Modulating Rod]} && ${Me.CurrentHPs} > 325) {
			/if (${validate_cast[FALSE, item, "Summoned: Modulating Rod", ${Me.ID}]}) /call core_cast2 "Summoned: Modulating Rod" item 0 FALSE
		} else /if (${Me.ItemReady[Wand of Pelagic Transvergence]} && ${Me.CurrentHPs} > 12850) {
			/if (${validate_cast[FALSE, item, "Wand of Pelagic Transvergence", ${Me.ID}]}) /call core_cast2 "Wand of Pelagic Transvergence" item 0 FALSE
		} else /if (${Me.ItemReady[Wand of Phantasmal Modulation]} && ${Me.CurrentHPs} > 11100) {
			/if (${validate_cast[FALSE, item, "Wand of Phantasmal Modulation", ${Me.ID}]}) /call core_cast2 "Wand of Phantasmal Modulation" item 0 FALSE 
		} else /if (${Me.ItemReady[Wand of Pelagic Transvergence]} && ${Me.CurrentHPs} > 3594) {
			/if (${validate_cast[FALSE, item, "Wand of Pelagic Transvergence", ${Me.ID}]}) /call core_cast2 "Wand of Pelagic Transvergence" item 0 FALSE
		} else /if (${Me.ItemReady[Wand of Phantasmal Transvergence]} && ${Me.CurrentHPs} > 3138) {
			/if (${validate_cast[FALSE, item, "Wand of Phantasmal Transvergence", ${Me.ID}]}) /call core_cast2 "Wand of Phantasmal Transvergence" item 0 FALSE
		} else /if (${Me.ItemReady[Wand of Temporal Mastery]} && ${Me.CurrentHPs} > 1686) {
			/if (${validate_cast[FALSE, item, "Wand of Temporal Mastery", ${Me.ID}]}) /call core_cast2 "Wand of Temporal Mastery" item 0 FALSE
		} else /if (${Me.ItemReady[Wand of Elemental Transvergence]} && ${Me.CurrentHPs} > 920) {
			/if (${validate_cast[FALSE, item, "Wand of Elemental Transvergence", ${Me.ID}]}) /call core_cast2 "Wand of Elemental Transvergence" item 0 FALSE
		} else /if (${Me.ItemReady[Rod of Arcane Transvergence]} && ${Me.CurrentHPs} > 2530) {
			/if (${validate_cast[FALSE, item, "Rod of Arcane Transvergence", ${Me.ID}]}) /call core_cast2 "Rod of Arcane Transvergence" item 0 FALSE
		} else /if (${Me.ItemReady[Rod of Spectral Transvergence]} && ${Me.CurrentHPs} > 1720) {
			/if (${validate_cast[FALSE, item, "Rod of Spectral Transvergence", ${Me.ID}]}) /call core_cast2 "Rod of Spectral Transvergence" item 0 FALSE
		} else /if (${Me.ItemReady[Rod of Ethereal Transvergence]} && ${Me.CurrentHPs} > 1320) {
			/if (${validate_cast[FALSE, item, "Rod of Ethereal Transvergence", ${Me.ID}]}) /call core_cast2 "Rod of Ethereal Transvergence" item 0 FALSE
		} else /if (${Me.ItemReady[Rod of Mechamagical Mastery]} && ${Me.CurrentHPs} > 1400) {
			/if (${validate_cast[FALSE, item, "Rod of Mechamagical Mastery", ${Me.ID}]}) /call core_cast2 "Rod of Mechamagical Mastery" item 0 FALSE
		} else /if (${Me.ItemReady[Rod of Prime Transvergence]} && ${Me.CurrentHPs} > 1000) {
			/if (${validate_cast[FALSE, item, "Rod of Prime Transvergence", ${Me.ID}]}) /call core_cast2 "Rod of Prime Transvergence" item 0 FALSE
		} else /if (${Me.ItemReady[Rod of Mystical Transvergence]} && ${Me.CurrentHPs} > 550) {
			/if (${validate_cast[FALSE, item, "Rod of Mystical Transvergence", ${Me.ID}]}) /call core_cast2 "Rod of Mystical Transvergence" item 0 FALSE
		}
	}
	
	| ask for a rod cause i'm fu'kn lazy...
	/if (!${Select[${Me.Class.ShortName},MAG]} && !${Bool[${FindItem[Modulation Shard]}]}) {
		/call tell_resident_mage shard
	}	
/return



|***
 *	DES: Send Pet to attack
 *	USE: /call send_pet
 *	INI: na
 *  NOTE: 
 ***| 
sub send_pet(int _tmpID)
	DEBUG send_pet(${_tmpID})
	/if (!${coreAuto}) /return
	/if (!${switchPcPet}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${Spawn[${Target2Kill}].Distance} > ${MobAgro} || ${Spawn[${Target.ID}].Type.Equal[corpse]} || ${Target.Dead}) /return
	
	/if (!${Target2Kill}) {
		/call get_assist_target
		/if (!${validate_target[FALSE, ${Target2Kill}]}) /return
	}

	/target ID ${Target2Kill}
	
	/if (${switchPcPet}) {
		/if (${Me.Pet.ID} && (${Target.PctHPs} <= ${PcPetengageat} || ${switchOverrideEngageat}) && !${Bool[${Me.Pet.Target}]}) {
			/call echos petfarm "filler" ${Target2Kill} 0 0
			/pet attack
		}
	}
	
	| send in the swarm
	/if (${switchPcSwarm} && (${Target.PctHPs} <= ${PcSwarmengageat} || ${switchOverrideEngageat})) /pet swarm
/return



|***
 *	DES: Will use a glyph and buy it if able.
 *	USE: /call use_glyph
 *	INI: 
 *  NOTE: 
 ***|
sub use_glyph()
	DEBUG use_glyph()
	AMIDEAD
	CHECKINPUTS
	/if (!${Bool[${useGlyph}]} || !${Raid.Members} && !${switchGlyphOutsideRaid}) /return
	/if (!${Me.AltAbility[${useGlyph}].ID} && ${Me.AAPoints} >= ${AltAbility[${useGlyph}].Cost}) /alt buy ${AltAbility[${useGlyph}].ID}
	/if (${validate_cast[FALSE, alt, "${useGlyph}", ${Me.ID}]}) /call core_cast2 "${useGlyph}" alt 0 FALSE
	/if (!${Me.AltAbility[${useGlyph}].ID} && ${Me.AAPoints} >= ${AltAbility[${useGlyph}].Cost}) /alt buy ${AltAbility[${useGlyph}].ID}
/return



|***
 *	DES: will halt spell casting and force Gift of Manna nukes if setup
 *	USE: sub for use with MQ2Cast call
 *	INI: 
 *  NOTE: this is checked on most spell casts. prevents doing stupid shit.
 ***|
sub cast_break()
	DEBUG cast_break()
	AMIDEAD
	CHECKINPUTS
	/if (${Me.Zoning}) /stopcast

	/if (${Spell[${Me.Casting.ID}].Category.Equal[Transport]}) /return TRUE
	/if (${Spell[${Me.Casting.ID}].Category.Equal[Heal]}) /return TRUE
	
	/if (${Target.PctHPs} <= ${Int[${stopnukeAt}]}) /stopcast
	
	/if (${switchAmIDead}) /stopcast
	/if (${flagCallNeedRez}) /stopcast
	
		
	/if (${Target.Mezzed.ID} || !${Spawn[${Target2Kill}].ID} || !${Bool[${Target2Kill}]}) {
		/stopcast
	} else /if (${Bool[${Me.Song[Gift of Mana (${GoMLevel})]}]} && ${Bool[${combat}]} && (${Bool[${GoMNuke}]} || ${Bool[${GoMNuke2}]})) {
		/stopcast
		/call cast_nuke_GoM ${Target2Kill}
	}
/return



|***
 *	DES: Cures/removes debuffs, we hope...
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub cast_cure()
	DEBUG cast_cure()
	/call create_timer timer_check_cast_cure ${tCast_Cure}
	AMIDEAD
	CHECKINPUTS

	/if (!${validate_plugin[FALSE, |mq2debuffs]}) /return
	
	UPDATEHUD "HEAL Cure"

	| if i'm fine skip to everyone else
	/if (!${Debuff.Counters}) {
		/if (!${Bool[${Group.Members}]} || ${switchPull} || ${switchAgro}) /return
	} else /if (${Debuff.Counters}) {
		
		| Monk
		/if (${Select[${Me.Class.ShortName},MNK]}) {
			/if (${validate_cast[FALSE, alt, "Purify Body", ${Me.ID}]}) /call core_cast2 "Purify Body" alt 0 FALSE
		| paladin, Cleric, Shaman, Druid
		} else /if (${Select[${Me.Class.ShortName},PAL,CLR,SHM,DRU]} && ${Me.AltAbilityReady[Radiant Cure]}) {
			/if (${Debuff.Curses} || ${Debuff.Poisons} || ${Debuff.Diseases}) {
				/if (${validate_cast[FALSE, alt, "Radiant Cure", ${Me.ID}]}) /call core_cast2 "Radiant Cure" alt 0 FALSE
			}
			| paladin splash
			/if (${Select[${Me.Class.ShortName},PAL]}) {
				/call Bind_pal_splash
			}
			
		| Necromancer
		} else /if (${Select[${Me.Class.ShortName},NEC]} && ${Me.AltAbilityReady[Embrace the Decay]}) {
			/if (${Debuff.Curses} || ${Debuff.Poisons} || ${Debuff.Diseases} || ${Debuff.Corruptions}) {
				/if (${validate_cast[FALSE, alt, "Embrace the Decay", ${Me.ID}]}) /call core_cast2 "Embrace the Decay" alt 0 FALSE
			}
			
			
			
		}
		
		/if (${Debuff.Curses} && ${Me.ItemReady[${cureCurseItem}]}) {
			/if (${validate_cast[FALSE, item, "${cureCurseItem}", ${Me.ID}]}) /call core_cast2 "${cureCurseItem}" item ${Me.ID} FALSE
		}
		
		/if (${Debuff.Corruptions} && ${Me.ItemReady[${cureCorruptItem}]}) {
			/if (${validate_cast[FALSE, item, "${cureCorruptItem}", ${Me.ID}]}) /call core_cast2 "${cureCorruptItem}" item ${Me.ID} FALSE
		}
		
		/if (${Debuff.Poisons} && ${Me.ItemReady[${curePoisonItem}]}) {
			/if (${validate_cast[FALSE, item, "${curePoisonItem}", ${Me.ID}]}) /call core_cast2 "${curePoisonItem}" item ${Me.ID} FALSE
		}
		
		/if (${Debuff.Diseases} && ${Me.ItemReady[${cureDiseaseItem}]}) {
			/if (${validate_cast[FALSE, item, "${cureDiseaseItem}", ${Me.ID}]}) /call core_cast2 "${cureDiseaseItem}" item ${Me.ID} FALSE
		}
	}


	/declare _count int local 0

	/if (${setCommunications.Equal[eqbc]}) {
		/for _count 1 to ${Math.Calc[${NetBots.Client.Count[ ]}+1]}
			AMIDEAD
			CHECKINPUTS

			| are they debuffed in the zone and alive?
			/if (${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Count} <= 0 || !${Spawn[p${NetBots.Client[${_count}]}].ID} || ${Spawn[pc ${NetBots.Client[${_count}]}].Dead}) /continue

			| AA Radient Cure 
			| 100' 1 min
			/if (${validate_class[FALSE, |PAL|SHM|CLR|DRU]} && ${Me.AltAbilityReady[Radiant Cure]}) {
				/if (${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Count}) {
					/if (${validate_cast[FALSE, alt, "Radiant Cure", ${Me.ID}]}) /call core_cast2 "Radiant Cure" alt 0 FALSE
				}
				/return
			}

			| AA Purify Soul
			| 200' 5 min
			/if (${validate_class[FALSE, |PAL|CLR|DRU]} && ${Me.AltAbilityReady[Purify Soul]}) {
				/if (${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Count}) {
					/if (${validate_cast[FALSE, alt, "Purify Soul", ${Spawn[pc ${NetBots.Client[${_count}]}].ID}]}) /call core_cast2 "Purify Soul" alt ${Spawn[pc ${NetBots.Client[${_count}]}].ID} FALSE
				}
				
				| paladin splash
				/if (${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Count} && ${Select[${Me.Class.ShortName},PAL]}{
					/call Bind_pal_splash
				}				
				/return
			}
			
			| AA Group Purify Soul . were in deep shit if we've gotten this far.
			| 0/100' 15 min
			/if (${validate_class[FALSE, |CLR|DRU]} && ${Me.AltAbilityReady[Group Purify Soul]}) {
				/if (${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Count}) {
					/if (${validate_cast[FALSE, alt, "Group Purify Soul", ${Me.ID}]} && ${Spawn[pc ${NetBots.Client[${_count}]}].Distance} <= 100) /call core_cast2 "Group Purify Soul" alt 0 FALSE
				}
				/return
			}

			| cure with spell
			/if (${Bool[${cureCurse}]} && ${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Cursed}) {
				/if (${validate_cast[FALSE, spell, "${cureCurse}", ${Spawn[pc ${NetBots.Client[${_count}]}].ID}]}) /call core_cast2 "${cureCurse}" ${buffGem} ${Spawn[pc ${NetBots.Client[${_count}]}].ID} FALSE
			} else /if (${Bool[${cureCorrupt}]} && ${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Corrupted}) {
				/if (${validate_cast[FALSE, spell, "${cureCorrupt}", ${Spawn[pc ${NetBots.Client[${_count}]}].ID}]}) /call core_cast2 "${cureCorrupt}" ${buffGem} ${Spawn[pc ${NetBots.Client[${_count}]}].ID} FALSE
			} else /if (${Bool[${curePoison}]} && ${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Poisoned}) {
				/if (${validate_cast[FALSE, spell, "${curePoison}", ${Spawn[pc ${NetBots.Client[${_count}]}].ID}]}) /call core_cast2 "${curePoison}" ${buffGem} ${Spawn[pc ${NetBots.Client[${_count}]}].ID} FALSE
			} else /if (${Bool[${cureDisease}]} && ${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Diseased}) {
				/if (${validate_cast[FALSE, spell, "${cureDisease}", ${Spawn[pc ${NetBots.Client[${_count}]}].ID}]}) /call core_cast2 "${cureDisease}" ${buffGem} ${Spawn[pc ${NetBots.Client[${_count}]}].ID} FALSE
			}

		/next _count	
	} else /if (${setCommunications.Equal[dannet]}) {
		/declare _toonName string local
		/declare _DNqout string local
		/declare _DNarray string local ${DanNet.Peers[all]}
		

		| each toon
		/for _count 1 to ${Int[${Math.Calc[${_DNarray.Count[|]}]}]})
			AMIDEAD
			CHECKINPUTS
			
			| get the name to something usable
			/if (${DanNet.FullNames}) {
				/varset _toonName ${_DNarray.Arg[${_count},|].Right[-${Int[${Math.Calc[${MacroQuest.Server.Length}+1]}]}]}
			} else {
				/varset _toonName ${_DNarray.Arg[${_count},|]}
			}
					
			| toon here?
			/if (!${Spawn[pc ${_toonName}].ID}) /continue
				
			| toon dead?
			/if (${Spawn[pc ${_toonName}].Dead}) /continue
		
			| already checked me
			/if (${_toonName.Equal[${Me.DisplayName}]}) /continue		

			| are they debuffed?
			/dquery ${_toonName} -q "Debuff" -o _DNqout
			/if (!${_DNqout}) /continue		


			| AA Radient Cure 
			| 100' 1 min
			/if (${validate_class[FALSE, |PAL|SHM|CLR|DRU]} && ${Me.AltAbilityReady[Radiant Cure]}) {
				/if (${validate_cast[FALSE, alt, "Radiant Cure", ${Me.ID}]}) /call core_cast2 "Radiant Cure" alt 0 FALSE
				/continue
			}

			| AA Purify Soul
			| 200' 5 min
			/if (${validate_class[FALSE, |PAL|CLR|DRU]} && ${Me.AltAbilityReady[Purify Soul]}) {
				/if (${validate_cast[FALSE, alt, "Purify Soul", ${Spawn[pc ${_toonName}].ID}]}) /call core_cast2 "Purify Soul" alt ${Spawn[pc ${_toonName}].ID} FALSE
				/continue
			}
			
			| AA Group Purify Soul . were in deep shit if we've gotten this far.
			| 0/100' 15 min
			/if (${validate_class[FALSE, |CLR|DRU]} && ${Me.AltAbilityReady[Group Purify Soul]}) {
				/if (${validate_cast[FALSE, alt, "Group Purify Soul", ${Me.ID}]} && ${Spawn[pc ${_toonName}].Distance} <= 100) /call core_cast2 "Group Purify Soul" alt 0 FALSE
				/continue
			}
			
			| paladin splash 50' range
			/if (${Select[${Me.Class.ShortName},PAL]}) {
				/call Bind_pal_splash
			}
			
			| cure with spell
			
			/if (${Bool[${cureCurse}]}) {
				/dquery ${_toonName} -q "Debuff.Curses" -o _DNqout
				/if (${_DNqout} && ${validate_cast[FALSE, spell, "${cureCurse}", ${Spawn[pc ${_toonName}].ID}]}) /call core_cast2 "${cureCurse}" ${buffGem} ${Spawn[pc ${_toonName}].ID} FALSE
			}
			
			/if (${Bool[${cureCorrupt}]}) {
				/dquery ${_toonName} -q "Debuff.Corruptions" -o _DNqout
				/if (${_DNqout} && ${validate_cast[FALSE, spell, "${cureCorrupt}", ${Spawn[pc ${_toonName}]}]}) /call core_cast2 "${cureCorrupt}" ${buffGem} ${Spawn[pc ${_toonName}].ID} FALSE
			}
			
			/if (${Bool[${curePoison}]}) {
				/dquery ${_toonName} -q "Debuff.Poisons" -o _DNqout
				/if (${_DNqout} && ${validate_cast[FALSE, spell, "${curePoison}", ${Spawn[pc ${_toonName}].ID}]}) /call core_cast2 "${curePoison}" ${buffGem} ${Spawn[pc ${_toonName}].ID} FALSE
			}
			
			/if (${Bool[${cureDisease}]}) {
				/dquery ${_toonName} -q "Debuff.Diseases" -o _DNqout
				/if (${_DNqout} && ${validate_cast[FALSE, spell, "${cureDisease}", ${Spawn[pc ${_toonName}].ID}]}) /call core_cast2 "${cureDisease}" ${buffGem} ${Spawn[pc ${_toonName}].ID} FALSE
			}

		/next _count
		
	}
	
	UPDATEHUD FALSE
/return



|***
 *	DES:targets and casts a free targeting area spell.
 *	USE: /call cast_free_target [SPELL] [TARGET ID]
 *	INI: 
 *  NOTE: i could never get this to work right...
 ***|
sub cast_free_target(string _tmpSpell, int _tmpID)
	DEBUG cast_free_target(${_tmpSpell}, ${_tmpID})
	/if (${validate_cast[FALSE, spell, "${_tmpSpell}", ${_tmpID}) /call core_cast2 "${_tmpSpell}" 0 ${_tmpID} FALSE
	/squelch /face ID ${_tmpID} ${If[${SetFaceFast},fast,]}
	/delay 5
	/click left center
	/delay 4s !${Me.Casting.ID}
/return



|***
 *	DES: use base melee discs
 *	USE: /call use_skill_melee TARGET
 *	INI: toon -> [meleedisc] set the skill to TRUE to use.
 *  NOTE: 
 ***|
sub use_skill_melee(int _tmpID)
	DEBUG use_skill_melee(${_tmpID})
 	/if (!${switchCombatMelee}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	
	/if (!${Bool[${_tmpID}]}) /return	
	/if (${is_target_dead[${_tmpID}]}) /return
	
	/if (${Target.ID} != ${_tmpID}) {
		/target ID ${_tmpID}
		/delay 5 ${Target.ID} == ${_tmpID}
	}
	
	/if (${coreAuto}) {
		/if (${Target.ID} && !${Range.Between[0,${SetFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${_tmpID} nolook ${If[${SetFaceFast},fast,]}
	}

	| taunt /onoff agro
	| /if (${validate_class[FALSE, |WAR|PAL|SHD|RNG]}) {
	/if (${taunt} && ${switchAgro}) {
		/if (${Me.AbilityReady[Taunt]} && ${Spawn[${_tmpID}].Distance} <= 18 && ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) {
			VMELEE Melee ${sep} ${csp}Taunting\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability Taunt
		}
	}
	
	| disarm
	/if (${Bool[${disarm}]} && !${Skill[Disarm].Auto}) {
		/if (${Me.AbilityReady[Disarm]} && ${Spawn[${_tmpID}].Distance} <= 15 && ${switchAgro}) {
			VMELEE Melee ${sep} ${csp}Disarm\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability Disarm
		}
	}
		
	| kick
	/if (${Bool[${kick}]} && !${Skill[Kick].Auto}) {
	 	/if (!${Select[${Me.Class.ShortName},MNK]} && ${Me.AbilityReady[Kick]} && ${Spawn[${_tmpID}].Distance} <= 15) {
			VMELEE Melee ${sep} ${csp}Kick\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability Kick
		}
	}
	
	| bash
	/if (${Bool[${bash}]} && !${Skill[Bash].Auto}) {
		/if (${Me.AbilityReady[Bash]} && ${Spawn[${_tmpID}].Distance} <= 15) {
			VMELEE Melee ${sep} ${csp}Bash\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Bool[${switchBandolier}]} && ${Bool[${bandolierBash}]} && ${Bool[${bandolierNormal}]}) {
				/if (${Target.ID}) {
					/call bandolier_swap Bash
					/doability Bash
					/call bandolier_swap Normal
				}
			} else /if (!${Bool[${switchBandolier}]}) {
				/if (${Target.ID}) /doability Bash
			}
		}
	}	
	
	| Intimidation Bard/Rogue only skill
	/if (${validate_class[FALSE, |BRD|ROG|BER]}) {
		/if (${Bool[${intimidation}]}) {
			/if (${Me.AbilityReady[Intimidation]} && ${Spawn[${_tmpID}].Distance} <= 15) {
				VMELEE Melee ${sep} ${csp}Intimidation\ax ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax]
				/if (${Target.ID}) /doability Intimidation		
			}
		}
	}

	| Zerker only skill
	/if (${validate_class[FALSE, |BER]}) {
		/if (${Bool[${frenzy}]} && && !${Skill[Frenzy].Auto}) {
			/if (${Me.AbilityReady[Frenzy]} && ${Spawn[${_tmpID}].Distance} <= 13) {
				VMELEE Melee ${sep} ${csp}Frenzy\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
				/if (${Target.ID}) /doability Frenzy
			}
		}
	}

	| rogue only skill
	/if (${validate_class[FALSE, |ROG]}) { 
		/if (${Bool[${backstab}]} && && !${Skill[Backstab].Auto}) {
			/if (${Me.AbilityReady[Backstab]} && ${Spawn[${_tmpID}].Distance} <= 13) {
				VMELEE Melee ${sep} ${csp}Backstab\ax ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax]
				/if (${Target.ID}) /doability Backstab
			}
		}
	}
	
	| monk/bst only skill 
	/if (${validate_class[FALSE, |MNK|BST]} && ${Spawn[${_tmpID}].Distance} <= 15) {
		| fist
		/if (${Bool[${dragonpunch}]} && !${Skill[Dragon Punch].Auto} && ${Me.AbilityReady[Dragon Punch]}) {
			VMELEE Melee ${sep} ${csp}Dragon Punch\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability "Dragon Punch"
		} else /if (${Bool[${eaglestrike}]} && !${Skill[Eagle Strike].Auto} && ${Me.AbilityReady[Eagle Strike]}) {
			VMELEE Melee ${sep} ${csp}Eagle Strike\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability "Eagle Strike"
		} else /if (${Bool[${tigerclaw}]} && !${Skill[Tiger Claw].Auto} && ${Me.AbilityReady[Tiger Claw]}) {
			VMELEE Melee ${sep} ${csp}Tiger Claw\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability "Tiger Claw"
		}
		| kick
	 	/if (${Bool[${kick}]} && !${Skill[Kick].Auto} && ${Me.AbilityReady[Kick]}) {
			VMELEE Melee ${sep} ${csp}Kick\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability Kick
		} else /if (${Bool[${flyingkick}]} && !${Skill[Flying Kick].Auto} && ${Me.AbilityReady[Flying Kick]}) {
			VMELEE Melee ${sep} ${csp}Flying Kick\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability "Flying Kick"
		} else /if (${Bool[${roundkick}]} && !${Skill[Round Kick].Auto} && ${Me.AbilityReady[Round Kick]}) {
			VMELEE Melee ${sep} ${csp}Round Kick\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability "Round Kick"
		}		
	}
/return
 


 |***
 *	DES: single swarm pet
 *	USE: /call cast_nuke_servant
 *	INI: na
 *  NOTE: 
 ***|
sub cast_nuke_servant()
	DEBUG cast_nuke_servant()
	/if (!${switchPcSwarm}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${validate_cast[FALSE, spell, "${ServantNuke}", ${Target2Kill}]}) /call core_cast2 "${ServantNuke}" 0 ${Target2Kill} cast_break
	/if (${validate_cast[FALSE, alt, "${ServantNuke}", ${Target2Kill}]}) /call core_cast2 "${ServantNuke}" alt ${Target2Kill} FALSE
/return



|***
 *	DES: preps toon for combat modes
 *	USE: /call set_combat []
 *	INI: 
 *  NOTE: 
 ***|
sub set_combat(str _mode)
	DEBUG set_combat(${_mode})
	AMIDEAD
	CHECKINPUTS
	/if (${is_target_dead[${Target2Kill}]}) /return	
	/if (!${coreAuto} && ${Me.Invis}) /makemevisible
	UPDATEHUD "${_mode}"
	/if (${Me.State.NotEqual[STAND]}) /stand
	/if (${coreAuto} && ${Me.State.Equal[FEIGN]}) /squelch /stand
		
	| combat	
	/if (${Bool[${_mode}]}) {
		| /if ((${switchCombatMelee} || ${switchCombatRange}) && ${coreAuto} && !${Range.Between[0,${SetFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
		
		/if (${coreAuto}) {
			/if (${switchCombatMelee} || ${switchCombatRange}) {
				/if (!${Range.Between[0,${SetFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
			}
		}


		| stop melee on DS ??
		/if (${coreAuto} && ${Bool[${Target.DSed}]} && ${stoponDS} && !${switchAgro}) {
			/call set_stop NOECHO
			/call clear_combat
		} else {
			
			|**
			/if (${Target.Distance} > 31 && ${switchCombatRange} && !${SetStickBehind}) {
				/if ((${Spawn[${Target2Kill}].LineOfSight} && !${switchOverrideLos}) && !${Me.AutoFire}) /squelch /autofire on
				/if (${coreAuto} && ${switchCombatMelee} && !${RangeList.Find[${Target.DisplayName}]}) /call stick_to_target ${Target2Kill}
			} else /if ((${Target.Distance} <= 31 && ${switchCombatMelee}) || (${SetStickBehind} && ${Target.Distance} <= ${MobAgro} && ${switchCombatMelee})) {
				/if (${NoMeleeList.Find[${Target.DisplayName}]}) {
					/squelch /attack off
				} else {
					/squelch /attack on
					/if (${coreAuto} && ${switchCombatMelee}) /call stick_to_target ${Target2Kill}
				}
			}
			**|

			| range all the things
			/if (${Target.Distance} > 31 && ${switchCombatRange} && !${SetStickBehind}) {
				
				| no need to move were are at a good distance
				/if (${Stick.Active}) /squelch /stick off		
				
				/if (${RangeList.Find[${Target.DisplayName}]}) {
					/if (${Me.AutoFire}) /squelch /autofire
				} else {
					/if (${Spawn[${Target2Kill}].LineOfSight} && !${Me.AutoFire}) /squelch /autofire
				}
				
			| adjust range id melee is diabled to keep close to target.
			} else /if (${Target.Distance} < 31 && ${switchCombatRange} && !${SetStickBehind} && !${switchCombatMelee}) {
				/if (${coreAuto} && !${Stick.Active} && ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) /stick moveback 38
				
			| melee shit?
			} else /if ((${Target.Distance} <= 31 && ${switchCombatMelee}) || (${SetStickBehind} && ${Target.Distance} <= ${MobAgro} && ${switchCombatMelee})) {
				/if (${NoMeleeList.Find[${Target.DisplayName}]}) {
					/squelch /attack off
				} else {
					/if (${Me.AutoFire}) /squelch /autofire
					/squelch /attack on
					/if (${coreAuto}) /call stick_to_target ${Target2Kill}
				}
			}
		}
	}
/return



|***
 *	DES: Gift of Manna Nuke
 *	USE: /call cast_nuke_GoM
 *	INI: Toon -> [Spells] -> gomnuke, gomnuke2 gomlevel
 *  NOTE: 
 ***|
sub cast_nuke_GoM(int _tmpID)
	DEBUG cast_nuke_GoM(${_tmpID})

	| clerics have their own GOM routine in group heals
	/if (${Select[${Me.Class.ShortName},CLR]}) /return

	CHECKEXIT
	CHECKINPUTS

	/while (${Me.Casting.ID} && (${Me.Gem[${GoMNuke}]} || ${Me.Gem[${GoMNuke2}]})) {
		/delay 1
	}
	
	/if (${Me.SpellReady[${GoMNuke}]}) {
		/if (${Me.CurrentMana} < ${Spell[${GoMNuke}].Mana} || ${Me.CurrentMana} < 2) /return 

		/target ID ${_tmpID}
		/if (!${cast_condition[GoMNukeCondition]}) /return
		VSPELL GoM ${sep} ${csp}${GoMNuke}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
		/call MQ2Cast "${GoMNuke}" gem${Me.Gem[${GoMNuke}]} -targetid|${_tmpID}
	} else /if (${Me.SpellReady[${GoMNuke2}]}) {
		/if (${Me.CurrentMana} < ${Spell[${GoMNuke2}].Mana} || ${Me.CurrentMana} < 2) /return 
		/target ID ${_tmpID}
		/if (!${cast_condition[GoMNuke2Condition]}) /return
		VSPELL GoM Backup ${sep} ${csp}${GoMNuke2}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
		/call MQ2Cast "${GoMNuke2}" gem${Me.Gem[${GoMNuke2}]} -targetid|${_tmpID}
	}
/return



|***
 *	DES: buffs mercs in your group
 *	USE: /call check_buffs_manual [mercenary|raid]
 *	INI: uses INI [Buffs] 
 *  NOTE: 
 ***|
sub check_buffs_manual(string _type)
	DEBUG check_buffs_manual(${_type})
	/if (!${coreAuto}) /return
	CHECKCOMBATCAST
	/if (${watch_for_death[]}) /return 

	| # people in group
	/declare _i int local
	| # of buffs to cast
	/declare _x int local
	| targets buff count
	/declare _y int local
	| declare type types
	/declare _GroupRaid string local NULL
	/declare _PCMercenary string local NULL
	/declare _INIline string local NULL
	
	| set type types..
	/if (${_type.Equal[raid]}) {
		/call create_timer timer_check_buffs_raid ${tRaid_Buff}
		/varset _GroupRaid Raid
		/varset _PCMercenary pc
		/varset _INIline Raid
	} else /if (${_type.Equal[group]}) {
		/call create_timer timer_check_buffs_group ${tGroup_Buff}
		/varset _GroupRaid Group
		/varset _PCMercenary pc
		/varset _INIline Manual
	} else /if (${_type.Equal[mercenary]}) {
		/call create_timer timer_check_buffs_merc ${tMerc_Buff}
		/varset _GroupRaid Group
		/varset _PCMercenary mercenary
		/varset _INIline Manual
	} else {
		/return
	}
		
	/if (${${_GroupRaid}.Members} && ${Me.PctMana} > 10 && !${Me.Casting.ID}) {
		UPDATEHUD "BUFF ${_type}"
		/for _i 0 to ${${_GroupRaid}.Members}
			
			| do not buff self with this mode!!
			/if (${${_GroupRaid}.Member[${_i}].DisplayName.Equal[${Me.DisplayName}]}) /continue
			
			| is the right type of member
			/if (${Spawn[${${_GroupRaid}.Member[${_i}].ID}].Type.NotEqual[${_PCMercenary}]}) /continue
			
			/for _x 1 to ${setMaxINIBuffCount}
				/if (!${Bool[${Buff${_x}}]}) /continue
				/if (!${Bool[${${_INIline}Buff${_x}Class}]}) /continue
				AMIDEAD
				CHECKEXIT
				CHECKTIE
				CLEARCURSOR TRUE
				
				/if (!${${_INIline}Buff${_x}Class.Find[${Spawn[${${_GroupRaid}.Member[${_i}].ID}].Class.ShortName}]}) /continue 
				/if (!${Spawn[${${_GroupRaid}.Member[${_i}].ID}].Type.Equal[${_PCMercenary}]}) /continue
				/if (${Spawn[${${_GroupRaid}.Member[${_i}].ID}].Distance} > ${MobAgro} && ${Me.PctMana} <= 20) /continue
	
				| stop what you're doing and get the target
				/if (${Stick.Active}) /squelch /stick off
				/target ID ${${_GroupRaid}.Member[${_i}].ID}
				/delay 1s ${Target.ID} == ${${_GroupRaid}.Member[${_i}].ID}
				/delay 1s ${Target.BuffCount}
				/delay 1s

				| check the buffs, if it doesnt stack mark it and dont come back
				/for _y 1 to ${Target.BuffCount}	
					AMIDEAD
					/if (${timer_manual_buffs${Spell[${Buff${_x}chk}].ID}${${_GroupRaid}.Member[${_i}].ID}}) /continue
					
					/if (!${Spell[${Target.Buff[${_y}]}].WillStack[${Spell[${Buff${_x}}]}]} || ${Target.Buff[${_y}].ID}) {
						/call create_timer timer_manual_buffs${Spell[${Buff${_x}chk}].ID}${${_GroupRaid}.Member[${_i}].ID} ${Spell[${Spell[${Buff${_x}chk}].ID}].Duration.TotalSeconds}s
					} else {
						/call create_timer timer_manual_buffs${Spell[${Buff${_x}chk}].ID}${${_GroupRaid}.Member[${_i}].ID} 1
						/delay 2
					}
				/next _y

				/if (${Bool[${timer_manual_buffs${Spell[${Buff${_x}}].ID}${Group.Member[${_i}].ID}}]}) /continue
				
				| if target doesnt have the buff, hook em up
				/if (!${Target.Buff[${Buff${_x}chk}].ID}) {
					CLEARCURSOR TRUE
					VSPELL ${_GroupRaid} Buffing ${sep} ${csp}${Buff${_x}}\ax on [${cpc}${${_GroupRaid}.Member[${_i}].DisplayName}\ax]
					/call MQ2Cast "${Buff${_x}}" ${buffGem} -targetid|${${_GroupRaid}.Member[${_i}].ID} 
				}

				
			/next _x	
		/next _i
		UPDATEHUD FALSE
	}
/return



|***
 *	DES: preps the queue with target toon
 *	USE: /call cast_buffs_queue [toon name] [buff word]
 *	INI: 
 *  NOTE:
 ***|
sub cast_buffs_queue(string _sender, string _verbage)
	DEBUG cast_buffs_queue(${_sender}, ${_verbage})
	CHECKINPUTS

	/declare _a int local 0
	/declare _i int local 0

	/for _i 1 to ${setMaxINIBuffCount}
		/for _a 1 to ${Buff${_i}Alias.Count[|]}
			AMIDEAD
			/if (${_verbage.Find["${Buff${_i}Alias.Arg[${_a},|]}"]}) {
				/call queue_buff_add ${Spawn[pc ${_sender}].ID} ${_i}
			}
		/next _a
	/next _i
/return



 |***
 *	DES: will loosly tie to a specific toon. will moove close to the toon when it moves. set to timer [Timers] -> CheckTieTime
 *	USE: /tie [on/off toggle] requires /tc name [NAME] and /tc distance ##
 *	INI: 
 *  NOTE: 
 **|
sub set_tie()
	DEBUG set_tie()

	| FORCE EMULATE FOLLOW.. HATE DOING THIS
	/if (${SetTieLock}) {
		/if (${SetTie} && ${Spawn[pc ${SetTieToon}].ID}) {
			/if (!${Stick.Active}) {
				/squelch /face ID ${Spawn[pc ${SetTieToon}].ID} ${If[${SetFaceFast},fast,]}
				/squelch /stick ID ${Spawn[pc ${SetTieToon}].ID} ${SetTieDistance} UW
			}
			AMIDEAD
			CHECKINPUTS
			/delay 1
		}
		/return
	}
	
	/if (${timer_check_Tie}) /return
	/if (${coreAuto}) /call create_timer timer_check_Tie ${tCheck_Tie}

	AMIDEAD
	CHECKINPUTS

	| reasons to not tie to a toon
	/if (${SetTie} && ${Spawn[pc ${SetTieToon}].ID} && ${Spawn[pc ${SetTieToon}].Dead}) /return
	/if (${Me.Combat} && ${switchNoTieonCombat}) /return
	/if (${Spawn[pc ${SetTieToon}].Distance} <= ${SetTieDistance} || !${Spawn[pc ${SetTieToon}].ID}) {
		
		| no nav plugin
		/if (${SetTieMode.Equal[nav]} && ${Bool[${Plugin[MQ2Nav]}]}) {
		
		| no moveutils plugin
		} else /if (${SetTieMode.Equal[stick]} && ${Bool[${Plugin[MQ2MoveUtils]}]}) {
			/if (${Stick.Active}) /squelch /stick off
			/if (${Me.Moving}) {
				/keypress forward
				/keypress back
			}
			/return
		}
	}
		
	| turn off camp
	/if (${Bool[${MakeCamp}]}) {
		/squelch /makecamp off
		/call writeINI HomeSet Movement FALSE 
	}
		
	| MQ2Nav mode for stick
	/if (${Bool[${Plugin[MQ2Nav]}]} && ${SetTieMode.Equal[nav]}) {

		/if (${Navigation.MeshLoaded}) {
			/if (!${Navigation.Active} && ${Spawn[pc ${SetTieToon}].Distance} > ${SetTieDistance}) {
				/squelch /face ID ${Spawn[pc ${SetTieToon}].ID} ${If[${SetFaceFast},fast,]}
				/squelch /nav spawn pc ${SetTieToon}
		
				:navtime

			} else /if (${Spawn[pc ${SetTieToon}].Distance} <= ${SetTieDistance}) {
				/if (${Navigation.Active}) {
					/squelch /nav stop
				}
			}

		} else /if (!${Navigation.MeshLoaded}) {
			VOUT NO Nav Mesh for this zone. Returning to Stick.
			/call Bind_setTieControl mode stick
		}
		
	| MQ2MoveUtils mode for stick
	} else /if (${Bool[${Plugin[MQ2MoveUtils]}]} && ${SetTieMode.Equal[stick]}) {
		| all present and or accounted for SIR!
		/if (${Spawn[pc ${SetTieToon}].Distance} > ${SetTieDistance}) {
			/squelch /face ID ${Spawn[pc ${SetTieToon}].ID} ${If[${SetFaceFast},fast,]}
			/squelch /stick ID ${Spawn[pc ${SetTieToon}].ID} ${SetTieDistance} UW
		}

		:okweresticking
		AMIDEAD
		CHECKINPUTS
		/if (${Spawn[pc ${SetTieToon}].Distance} <= ${SetTieDistance}) {
			/squelch /stick off
			/return
		}
		
		/if (!${Stick.Stopped} && ${Spawn[pc ${SetTieToon}].ID}) {
			/goto :okweresticking
		} else {
			/if (${Stick.Active}) /squelch /stick off
			/if (${Me.Moving}) {
				/keypress forward
				/keypress back
			}
		}
	
	}

/return



 |***
 *	DES: Will sort out your familiar.
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub sort_familiar()
	DEBUG sort_familiar()
	/call create_timer timer_Check_Familiar ${tCheck_Familiar}
	CHECKINPUTS
	/if (${check_exit[]}) /return
	/declare _count int local 0
	
	/for _count 1 to 3
		/if (${Bool[${Me.Buff[${sumFamiliar${_count}Buff}].Duration}]}) /continue
		/if (!${Bool[${sumFamiliar${_count}}]}) /continue
				
		/if (${Me.Book[${sumFamiliar${_count}}]}) {
			/if (${validate_cast[FALSE, spell, "${sumFamiliar${_count}}", ${Me.ID}]}) /call core_cast2 "${sumFamiliar${_count}}" ${buffGem} ${Me.Pet.ID} FALSE
		} else /if (${Me.AltAbilityReady[${sumFamiliar${_count}}]}) {	
			/if (${validate_cast[FALSE, alt, "${sumFamiliar${_count}}", ${Me.ID}]}) /call core_cast2 "${sumFamiliar${_count}}" alt 0 FALSE
		} else /if (${Me.ItemReady[${sumFamiliar${_count}}]}) {
			/if (${validate_cast[FALSE, item, "${sumFamiliar${_count}}", ${Me.ID}]}) /call core_cast2 "${sumFamiliar${_count}}" item 0 FALSE		
		}	
			
		/delay 5
		/if (${sumFamiliar${_count}Leave}) /familiar get lost
		/delay 5

	/next _count
	
/return



|***
 *	DES: Adjusts the XTarget for visual purposes to heal toonControl. *NO EFFECT ON HEALING
 *	USE: /call protect_always_xt & /control [toon name]
 *	INI: auto if toonControlXTslot is not FALSE in INI
 *  NOTE: 
 ***|
sub protect_always_xt()
	DEBUG protect_always_xt()
	/if (!${validate_expansion[FALSE, |16]}) /return
	CHECKINPUTS
	/if (${Bool[${toonControl}]} && ${Bool[${toonControlXTslot}]} && ${toonControlProtect} && ${Bool[${Spawn[pc ${toonControl}].ID}]} && !${Bool[${Spawn[${toonControl}].Dead}]}) {
		/if (${Group.Member[${toonControl}].ID}) {
			/xtarget set ${toonControlXTslot} Autohater
		} else /if (!${Group.Member[${toonControl}].ID}) {
			/xtarget set ${toonControlXTslot} ${toonControl}
		}
	} else /if ((!${Bool[${toonControl}]} || !${Bool[${Spawn[pc ${toonControl}].ID}]}) && ${Bool[${toonControlXTslot}]}) {
		/xtarget set ${toonControlXTslot} Autohater
	}
/return



|***
*	DES: check and claim Free Station Cash
*	USE: manual
*	Credit: v1.0 - Initial release on Macroquest VIP forums - Dewey2461 on 7-7-2014 [butchered by me]
*	INI: 
*  NOTE: 
***|
sub check_free_ingame_cash()
	DEBUG check_free_ingame_cash()
	
	| if nothing to claim. no reason to stick around.. like a bad house guest... go away..
	/if (${Window[MKPW_ClaimWindow].Child[MKPW_ClaimClickHereBtn].Text.EqualCS[Claimed]}) /return
	/declare _current_funds string local 0

	/if (${Window[MKPW_ClaimWindow].Child[MKPW_ClaimClickHereBtn].Text.EqualCS[Claim]}) {
		| open market window
		/if (!${Window[MarketplaceWnd]}) /market

		| get current funds
		/varset _current_funds ${Window[MarketplaceWnd].Child[MKPW_AvailableFundsUpper].Text}
		
		| click claim button
		/notify MKPW_ClaimWindow MKPW_ClaimClickHereBtn leftmouseup
		/delay 1s
		/delay 10s ${Window[MarketplaceWnd].Child[MKPW_AvailableFundsUpper].Text.NotEqual[${_current_funds}]}
		/varset _current_funds ${Window[MarketplaceWnd].Child[MKPW_AvailableFundsUpper].Text}
		VOUT In Game Cash Collected [${cnum}${_current_funds}\ax]   

		| close market window
		/if (${Window[MarketplaceWnd]}) /market

	}

/return



|***
*	DES: will click items and autoinventory the result on a timer.
*	USE: /call odds_and_ends
*	INI: [Odds]
*  NOTE: good for poisons, whatevers.
***|
sub odds_and_ends()
	DEBUG odds_and_ends()
	CHECKEXIT
	/call create_timer timer_check_odds_and_ends ${tOdds_and_Ends}
	/declare _count int local
	/for _count 1 to 4
		/if (!${Bool[${clickOddItem${_count}}]}) /continue
		AMIDEAD
		CLEARCURSOR TRUE
		CHECKINPUTS
		/if (${validate_cast[FALSE, item, "${clickOddItem${_count}}", ${Me.ID}]}) /call core_cast2 "${clickOddItem${_count}}" item 0 FALSE
	/next _count
	/call sort_cursor TRUE
/return



|***
 *	DES: If the followng criteria are met, the calling function will be skipped.
 *	USE: /call check_exit [TRUE|NULL] 
 *	INI: 
 *  NOTE: All reasons to not attack a target..
 ***|
sub check_exit()
	DEBUG check_exit()
	
	| invis 
	/if (${Me.Invis}) /return TRUE
	
	| am i zoning?
	/if (${Me.Zoning}) /return TRUE
	
	| am i dead on the floor?
	/if (${Me.State.Equal[HOVER]} || ${Me.Hovering} || ${Me.Dead}) /return TRUE
	
	| did I just die and get sent to bind ?
	/if (${coreAuto} && ${Me.Buff[Revival Sickness].ID} && ${Zone.ID} == ${Me.ZoneBound.ID}) /return TRUE
		
	| movement
	/if ((${Me.Moving} || ${Stick.Active}) && !${Select[${Me.Class.ShortName},BRD]}) /return TRUE

	| clear ToTD if it procs.
	/if (${coreAuto} && ${Select[${Me.Class.ShortName},CLR,SHM,BER]}) {
		/if (${Me.Song[Touch of the Divine].ID}) {
			/delay 3s
			/docommand /removebuff Touch of the Divine
			ECHOCHANNEL REMOVING .. Touch of the Divine
			/return TRUE
		}
	}
	
	| invulnerable debuff?
	/if (${Debuff.Invulnerable}) /return TRUE	
	
	| SELF BUFFS/SONGS
	/if (${Me.Song[Divine Aura].ID}) /return TRUE
	/if (${Me.Song[Divine Barrier].ID}) /return TRUE
	/if (${Me.Song[Sacred Barrier].ID}) /return TRUE
	/if (${Me.Song[Touch of the Divine].ID}) /return TRUE
	/if (${Me.Song[Bestowed Divine Aura Beza].ID} || ${Me.Song[Bestowed Divine Aura Azia].ID}) /return TRUE

	| bad self buffs
	/if (${Me.Buff[Bewilder].ID}) /return TRUE
	/if (${Me.Buff[Chaotic Bewildering].ID}) /return TRUE
	
	| SELF STATES
	/if (${Me.State.Equal[FEIGN]}) /return TRUE
	/if (${Me.Ducking}) /return TRUE
	/if (${Me.Stunned}) /return TRUE
	/if (${Me.Charmed.ID}) /return TRUE
	
	| check no cast list and mezz?
	/if (${coreAuto} && ${Target.ID}) {
		/if (${Bool[${NoCastList.Find[${Target.DisplayName}]}]}) /return TRUE
		/if (${Bool[${Target.Mezzed}]} && !${switchPull}) /return TRUE
	}

/return FALSE



|***
 *	DES: Updates Notice sction of HUD.
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub update_HUD_notices(string _mode)
	DEBUG update_HUD_notices(${_mode})
	CHECKINPUTS
	/if (${Me.Zoning}) /return
	
	/declare _count 					int 		local 0
	/declare _index 					int 		local 0
	/declare _sep 						bool 		local FALSE
	/declare _IV[4] 					string 	local FALSE
	/declare _IVU[1]					string 	local FALSE
	/declare _tmphudWarning		string	local  
	/declare _tmphudNotice		string	local  
	/declare _tmptarNotice		string	local  
	
	/if (!${timer_check_status}) /call broadcast_status
	
	| current mode display
	/if (${Bool[${_mode}]}) {
		/varset tmpMode "${_mode}"
	} else /if (!${Bool[${_mode}]} || ${_mode.Equal[NULL]}) {
		/varset tmpMode FALSE
	}

	|**  HUD warning notices.. the red stuff up top **|
	
	| rez warning
	/if (${switchAmIDead} || ${flagCallNeedRez}) {
		/varset _tmphudWarning ${_tmphudWarning} NEED REZ
	}	
			
	/varset _IV[1] |Shared Perfected Camouflage|Shared Camouflage|Camouflage|Group Perfected Invisibility I|Invisibility|Perfected Invisibility
	/varset _IV[2] |Phase March|Nethervoid March|Perfected Silent Presence|Cloak of Shadows|Cloak of Shadows I|Cloak of Shadows II|Gather Umbra|Invisibility Cloak
	/varset _IV[3] |Perfected Natural Invisibility|Perfected Spirit Walk Invisibility|Perfected Group Spirit Walk Invisibility
	/varset _IV[4] |Shauri's Sonorous Clouding|Shauri's Sonorous Clouding I|Shauri's Sonorous Clouding II|Shauri's Sonorous Clouding III|Selo's Song of Travel

	/for _count 1 to ${_IV.Size}
		/for _index 1 to ${_IV[${_count}].Count[|]}
			/if (${Me.Buff[${_IV[${_count}].Arg[${_index},|]}].Name.Equal[${_IV[${_count}].Arg[${_index},|]}]}) {
				/varset _tmphudWarning ${_tmphudWarning} IV
				/break
			}
		/next _index	
	/next _count

	| check for invis vs undead
	/varset _IVU[1] |Group Perfected Invisibility to Undead|Perfected Invisibility to Undead|Invisibility to Undead|Invisibility versus Undead|Shauri's Sonorous Clouding

	/for _count 1 to ${_IVU.Size}
		/for _index 1 to ${_IVU[${_count}].Count[|]}
			/if (${Me.Buff[${_IVU[${_count}].Arg[${_index},|]}].Name.Equal[${_IVU[${_count}].Arg[${_index},|]}]}) {
				/varset _tmphudWarning ${_tmphudWarning} IVU 
				/break
			}
			/next _index	
	/next _count

	| override warnings
	/for _count 1 to ${overrideType.Count[|]}
		/if (${switchOverride${overrideType.Arg[${_count},|].Left[1].Upper}${overrideType.Arg[${_count},|].Right[-1].Lower}}) {
			/varset _tmphudWarning ${_tmphudWarning} ${overrideType.Arg[${_count},|].Left[1].Upper}${overrideType.Arg[${_count},|].Right[-1].Lower} 
		}
	/next _count	
	
	| finaly something to update.. maybe..
	/if (${_tmphudWarning.Length} != ${hudWarningLength}) {
		/varset hudWarningLength ${_tmphudWarning.Length}
		/varset hudWarning ${_tmphudWarning}
	} 		
		
	
	|**  HUD bottom notices.. safe zone, modes, stuff like that  **|
  CHECKINPUTS
	
	/if (${Select[${Me.Class.ShortName},BRD]}) {
		/varset _tmphudNotice ${_tmphudNotice} ${lineCurrent}
	}	

	/if (${Bool[${Plugin[MQ2ADVPath]}]}) {
		/if (${AdvPath.Playing}) {
			/varset _tmphudNotice ${_tmphudNotice} AdvPath
		}
	}
	
	/if (${SafeZone}) {
		/varset _tmphudNotice ${_tmphudNotice} SafeZone
	}	
	/if (${Stick.Active}) {
		/varset _tmphudNotice ${_tmphudNotice} Stick
	}
	/if (${Select[${Me.Class.ShortName},MAG]}) {
		/if (${Bool[${useRain}]}) {
			/varset _tmphudNotice ${_tmphudNotice} AE
		}	
	}
	/if (${Select[${Me.Class.ShortName},CLR,PAL,SHM,DRU]}) {
		/if (${Bool[${RampageTank}]}) {
			/varset _tmphudNotice ${_tmphudNotice} RT[${RampageTank}]
		}	
	}

	| Mode Notice
	/if (${switchModeHunt}) {
		/varset _tmphudNotice ${_tmphudNotice} Hunt[${setModeAgro}]
	}		
	/if (${switchModePetfarm}) {
		/varset _tmphudNotice ${_tmphudNotice} PetFarm
	}		
	/if (${switchModeScorch}) {
		/varset _tmphudNotice ${_tmphudNotice} Scorch[${setModeAgro}]
	}		
	/if (${switchModeHarvest}) {
		/varset _tmphudNotice ${_tmphudNotice} Harvest[${setModeAgro}]
	}	
	/if (${switchModeDrag}) {
		/varset _tmphudNotice ${_tmphudNotice} Drag
	}
	/if (${switchModeForage}) {
		/varset _tmphudNotice ${_tmphudNotice} Forage
	}
	/if (${switchModeFish}) {
		/varset _tmphudNotice ${_tmphudNotice} Fish
	}
	/if (${switchModeSkill}) {
		/varset _tmphudNotice ${_tmphudNotice} Skill Trainer
	}		
	/if (${switchModeLush} || ${Me.Drunk}) {
		/varset _tmphudNotice ${_tmphudNotice} Drunk[${Me.Drunk}]
	}		

	| spit out the verbage
	/if (${_tmphudNotice.Length} != ${hudNoticeLength}) {
		/varset hudNoticeLength ${_tmphudNotice.Length}
		/varset hudNotice ${_tmphudNotice}
	} 	
		
	| ** target debuffs notices
	/varset _sep FALSE

	/if (${Bool[${Spawn[${Target.ID}].Master}]}) {
		/varset _tmptarNotice ${_tmptarNotice} Pet
	}	
	/if (${Bool[${Target.Maloed}]}) {
		/varset _tmptarNotice ${_tmptarNotice} Malo
	}	
	/if (${Bool[${Target.Tashed}]}) {
		/varset _tmptarNotice ${_tmptarNotice} Tash
	}	
	/if (${Bool[${Target.Snared}]}) {
		/varset _tmptarNotice ${_tmptarNotice} Snare
	}	
	/if (${Bool[${Target.Buff[Mental Contortion]}]}) {
		/varset _tmptarNotice ${_tmptarNotice} MC
	}	
	/if (${Bool[${Target.Slowed}]}) {
		/varset _tmptarNotice ${_tmptarNotice} Slow
	}	
	/if (${Bool[${Target.DSed}]}) {
		/varset _tmptarNotice ${_tmptarNotice} DS
	}	
	
	| target things and stuff
	/if (${_tmptarNotice.Length} != ${hudTargetLength}) {
		/varset hudTargetLength ${_tmptarNotice.Length}
		/varset hudTarNotice ${_tmptarNotice}
	} 
/return



|***
 *	DES: AA Slow
 *	USE: /call cast_AA_slow [MOBID] [AA SLOW NAME]
 *	INI: 
 *  NOTE: 

sub cast_AA_slow(int _tmpID, string _tmpSpell)
	| DEBUG ${cbug}cast_AA_slow(${_tmpID}, ${_tmpSpell})\ax
	/if (${timer_slow_${_tmpID}}) /return
	AMIDEAD
	CHECKEXIT

	| check for overwrite slows
	/declare _tmpSlow string local ${Target.Slowed}
	/if (${Target.Slowed.Duration}) {
		/if (${_tmpSlow.Find[Undermining]}) /call create_timer timer_slow_${_tmpID} ${Target.Slowed.Duration.TotalSeconds}s
		/return
	}

	/declare _resistCount int local 0
	
	/if (${Spawn[${_tmpID}].Type.NotEqual[PC]} && ${Spawn[${_tmpID}].Distance} <= ${Spell[${_tmpSpell}].MyRange} && !${timer_slow_${_tmpID}}) {
		AMIDEAD
		/if (${validate_cast[FALSE, alt, "${_tmpSpell}", ${_tmpID}]}) /call core_cast2 "${_tmpSpell}" alt ${_tmpID} FALSE
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_slow_${_tmpID} ${Spell[${_tmpSpell}].Duration.TotalSeconds}s
		} else /if (${Select[${castReturn},CAST_RESISTED,CAST_RESIST]}) {
			/varcalc _resistCount ${_resistCount}+1
			/if (${_resistCount} >= 3) /call create_timer timer_slow_${_tmpID} ${Spell[${_tmpSpell}].Duration.TotalSeconds}m
		}
	}
/return
 ***|


|***
 *	DES: will cast Temp pet
 *	USE: /call cast_temp_pet [MOBID] [Temp Spell Name]
 *	INI: 
 *  NOTE: 
 ***|
sub cast_temp_pet(int _tmpID, string _tmpSpell)
	DEBUG cast_temp_pet(${_tmpID}, ${_tmpSpell})
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${_tmpID}]}) /return
	/if (${validate_cast[FALSE, spell, "${_tmpSpell}", ${_tmpID}]}) /call core_cast2 "${_tmpSpell}" 0 ${_tmpID} FALSE
/return



|***
 *	DES: Cleric/Paladin AA Yaulp
 *	USE: /call cast_yaulp
 *	INI: [selfbuff] - doYaulp
 *  NOTE: if doYaulp is TRUE, will use at 98% manna or less.
 *	NOTE: Pallys keep that shit up ALL the time
 ***|
sub cast_yaulp()
	DEBUG cast_yaulp()
	/if (${Me.Sitting}) /return
	/if (${Me.Buff[Lesser Yaulp].ID} || ${Me.Buff[Yaulp].ID}) /return
	| /if (${Me.PctMana} > ${useYaulpPct}*.${tmpHealOverride}) /return
	/if (!${Select[${Me.CombatState},COMBAT]}) /return
	AMIDEAD
	SIMPLEEXIT
	
	| use spell Yaulp
	/if (${Bool[${spellYaulp}]}) {
		/if (${Me.SpellReady[${spellYaulp}]} && ${Spell[${spellYaulp}].Stacks}) {
			/if (${validate_cast[FALSE, spell, "${spellYaulp}", ${Me.ID}]}) /call core_cast2 "${spellYaulp}" 0 0 FALSE
		}
	
	| use Donals Helm Yaulp (cleric only)
	} else /if (${Select[${Me.Class.ShortName},CLR]} && ${Me.ItemReady[Darkened Donal's Helm of Mourning]}) {
		/if (!${Spell[Yaulp XIV Rk. II].Stacks}) /return
		/if (${validate_cast[FALSE, item, "Darkened Donal's Helm of Mourning", ${Me.ID}]}) /call core_cast2 "Darkened Donal's Helm of Mourning" item 0 FALSE
	
	| use AA yaulp
	} else /if (!${FindItem[Darkened Donal's Helm of Mourning].ID} && ${Me.AltAbilityReady[Yaulp (Lesser)]}) {
		/if (!${Spell[Yaulp (Lesser)].Stacks} || ${Bool[${Me.Buff[Lesser Yaulp]}]} || ${Bool[${Me.Buff[Yaulp]}]}) /return
			/if (${validate_cast[FALSE, alt, "Yaulp (Lesser)", ${Me.ID}]}) /call core_cast2 "Yaulp (Lesser)" alt 0 FALSE

	}
/return



|***
 *	DES: uses melee rest disc
 *	USE: /call cast_stamina_recovery
 *	INI: 
 *  NOTE: 
 ***|
sub cast_stamina_recovery()
	DEBUG cast_stamina_recovery()
 	/if (${Defined[DiscRest]}) {
		/if (!${Bool[${DiscRest}]}) /return
	 	/if (!${Me.CombatAbilityReady[${DiscRest}]}) /return
 	} else /if (!${Defined[DiscRest]}) {
 		/return
 	}
 	/if (${SafeZone} && !${switchOverrideSafezone} || !${Me.CombatAbilityReady[${DiscRest}]} || ${switchAmIDead}) /return
	/if (${Me.PctEndurance} > 33) /return
	AMIDEAD
	CHECKINPUTS
	
	/declare _endurancePoint int local 0

	/if (${Me.CombatAbility[Breather Rk. III]}) /varset _endurancePoint 29
	/if (${Me.CombatAbility[Breather Rk. II]}) /varset _endurancePoint 25
	/if (${Me.CombatAbility[Breather]}) /varset _endurancePoint 21
	
	| Force Fade to get rest dfor stamina while in combat
	/if (${switchForceFadeRest} && ${Bool[${${Me.Class.ShortName}AAFade}]}) {
		/if (${Select[${Me.CombatState},COMBAT]} && ${Me.PctEndurance} < ${_endurancePoint} && ${Me.AltAbilityReady[${${Me.Class.ShortName}AAFade}]}) {
			/call clear_combat
			/call set_stop NOECHO
			/if (${validate_cast[FALSE, alt, "${${Me.Class.ShortName}AAFade}", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}AAFade}" alt 0 FALSE
			/delay 3
			/if (${validate_cast[FALSE, disc, "${DiscRest}", ${Me.ID}]}) /call core_cast2 "${DiscRest}" disc 0 FALSE
			/return
		}
	}

	| need to be out of combat to use
 	/if (!${Select[${Me.CombatState},RESTING,COOLDOWN,ACTIVE]}) /return

 	/if (${Me.PctEndurance} < ${_endurancePoint}) {
 		/if (${validate_cast[FALSE, disc, "${DiscRest}", ${Me.ID}]}) /call core_cast2 "${DiscRest}" disc 0 FALSE
 	}
/return



|***
 *	DES: corrects the Gem## to simply a #
 *	USE: /call fix_gem_id [gem##]
 *	INI: 
 *  NOTE:
 ***|
sub fix_gem_id(string _spellGem)
	DEBUG fix_gem_id(${_spellGem})
	/declare _gem int local 0
	/if (${_spellGem.Length} == 1 || ${_spellGem.Length} == 4) {
		/varset _gem ${_spellGem.Right[1]}
	} else /if (${_spellGem.Length} == 2 || ${_spellGem.Length} == 5) {
		/varset _gem ${_spellGem.Right[2]} 
	}
/return ${_gem}



|***
 *	DES: Memorize a spell/song
 *	USE: /call mem_spell [T|F echo memming] [spell name] [gem to mem it in] [TRUE|FALSE wait for recycle to cast]
 *	INI: 
 *  NOTE: 
 ***|
sub mem_spell(bool _echo, string _spellname, string _spellgem, bool _wait)
	DEBUG mem_spell(${_echo}, ${_spellname}, ${_spellgem}, ${_wait})
 	/if (${Me.Gem[${_spellname}]}) /return TRUE
	AMIDEAD
	CHECKINPUTS

	| do we even have the spell?
	/if (!${Me.Book[${_spellname}]}) {
		/if (!${Me.Book[${_spellname}]} && !${timer_mem_spell_timer}) {
			/call create_timer timer_mem_spell_timer 60s
			VOUT ${onotice} ${sep} Bad Spell Name.
		}
		/return FALSE
	}

	| just the facts ma'am
	/call fix_gem_id "${_spellgem}"
	/declare _gem int local ${Macro.Return}
	
	/if (${_echo}) VOUT Mem ${sep} ${csp}${Spell[${_spellname}].RankName}\ax
	/memspell ${_gem} "${Spell[${_spellname}].RankName}"
	/if (${_wait}) {
		| :waitforgem
		/delay 1m ${Me.SpellReady[${_spellname}]}
		/delay 4
		/if (!${Me.SpellReady[${_spellname}]}) /goto :waitforgem
	} else /if (!${_wait}) {
		/delay 3s ${Me.SpellReady[${_spellname}]}
	}
/return TRUE



|***
 *	DES: sends a tell to the mages in the ResidentMage for shit you need.
 *	USE: /call tell_resident_mage [shard|weapons]
 *	INI: [common] -> ResidentMage
 *  NOTE: This will send the same tell to all mages in the list. welcome to ignore
 ***|
sub tell_resident_mage(string _request)
	DEBUG tell_resident_mage(${_request})
	/if (${Select[${Me.Class.ShortName},MAG]}) /return FALSE
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	
	/declare _toon string local FALSE
	/if (${Bool[${get_resident[mage, 150]}]}) {
		/varset _toon ${get_resident[mage, 150]}
	} else {
		/return FALSE
	}

	| ask for weapons
	/if (${validate_class[FALSE, ${PetList}]}) {
		/if (${_request.Equal[weapons]} && !${timer_ask_for_weapons_${_toon} && !${FindItem[${summonedWepClosedBag}].ID}) {
			/t ${_toon} weapons
			/call create_timer timer_ask_for_weapons_${_toon} ${tAsk_for_weapons}
		}
	}
	
	| ask for ModRod - shard
	/if (${_request.Equal[shard]} && !${timer_ask_for_rod_${_toon}} && !${FindItem[Modulation Shard].ID}) {
		/t ${_toon} shard
		/call create_timer timer_ask_for_rod_${_toon} ${tAsk_for_rod}
	}
/return TRUE

${timer_ask_for_rod_Ieve}

|***
 *	DES: Res routines for the healer slobs.
 *	USE: /call check_for_corpse
 *	INI: 
 *  NOTE: looks for corpses.. rez them
 ***|
sub check_for_corpse()
	DEBUG check_for_corpse()
	/if (${Me.State.Equal[HOVER]} || ${Me.Hovering} || ${Me.Dead}) /return FALSE
	
	| fuck your corpse
	/if (!${switchRezGive}) /return	FALSE
	| no one dead
	/if (!${SpawnCount[pccorpse radius ${maxRezRange}]}) /return FALSE
	| no combat rez
	/if (!${switchRezCombat} && ${SpawnCount[npc radius ${combatRadius} playerstate 4]}) /return FALSE
	| no safe zone ressing
	/if (${SafeZone} && !${switchOverrideSafezone}) /return FALSE
	| not enough manna for AA rez
	/if (${Me.PctMana} < 8 && !${switchRezEmergency}) /return FALSE
	| no emergency tokens AND dont use them when a cleric is around
	/if (${switchRezEmergency}) {
		/if (!${Me.ItemReady[Token of Resurrection]}) /return FALSE
		/if (${Spawn[pc CLR radius ${maxRezRange}].ID}) /return FALSE
	}
	
	| disable rezemergency if you dont have tokens
	/if (${switchRezEmergency} && !${FindItem[Token of Resurrection].ID}) {
		/varset switchRezEmergency FALSE
		ECHOCHANNEL ${Me.DisplayName} needs Rez Tokens....
	}
		
	| necro stuff
	/if (${Select[${Me.Class.ShortName},NEC]}) {
		/if (!${FindItem[Essence Emerald].ID}) /return FALSE
		/declare _rezNEC						string	local "Convergence"

	| cleric stuff
	} else /if (${Select[${Me.Class.ShortName},CLR]}) {
		/declare _divinerezCLR			string	local "Divine Resurrection"
		/declare _rezCLR						string	local "Blessing of Resurrection"
	
	| pally, shaman, druid stuff
	} else /if (${Select[${Me.Class.ShortName},PAL,SHM,DRU]}) {
		/if (${Select[${Me.CombatState},COMBAT]}) {
			/declare _rezDRU						string	local "Call of the Wild"
			/declare _rezSHM						string	local "Call of the Wild"
			/declare _rezPAL						string	local "Gift of Resurrection"
		} else {
			/declare _rezDRU						string	local "Rejuvenation of Spirit"
			/declare _rezSHM						string	local "Rejuvenation of Spirit"
			/declare _rezPAL						string	local "Restore Life"
		}
	}

	/if (${switchRezEmergency}) {		
		/declare _rezEmergency			string	local "Token of Resurrection"
	}

	/declare _count 						int 		local 0
	/declare _tmpID 						int 		local 0

	| make the damn variables
	/for _count 1 to ${SpawnCount[pccorpse radius ${maxRezRange}]}
		/varset _tmpID ${NearestSpawn[${_count},pccorpse radius ${maxRezRange}].ID}
		
		| for the people that suck ass (not in a good way)
		/if (${rezlist.Find[${Spawn[${_tmpID}].Name}]}) {
			/if (!${Defined[timer_corpse_${_tmpID}]}) /declare timer_corpse_${_tmpID} timer outer 7200
			/continue
		}	
		
		/if (!${Defined[timer_corpse_${_tmpID}]}) /declare timer_corpse_${_tmpID} timer outer 1
	/next _count

	UPDATEHUD "Resurrect"
	
	
	| clerics first group or raid
	/if (${SpawnCount[pccorpse cleric raid radius ${maxRezRange}]} && ${Raid.Members} || ${SpawnCount[pccorpse cleric group radius ${maxRezRange}]}) {
		/for _count 1 to ${SpawnCount[pccorpse cleric radius ${maxRezRange}]}
		AMIDEAD
		/if (!${timer_corpse_${NearestSpawn[${_count},pccorpse cleric radius ${maxRezRange}].ID}}) {
			/varset _tmpID ${NearestSpawn[${_count},pccorpse cleric radius ${maxRezRange}].ID}
			/if (${Select[${Me.Class.ShortName},CLR]} && ${Me.AltAbilityReady[${_divinerezCLR}]}) {
				/if (${DivineRezCleric}) /call cast_rez ${_tmpID} ${_divinerezCLR}		
			} else /if (${Select[${Me.Class.ShortName},CLR,DRU,PAL,SHM,NEC]} && ${Me.AltAbilityReady[${_rez${Me.Class.ShortName}}]}) {
				/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
			} else /if (${switchRezEmergency} && ${RezTokenList.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
				/call cast_rez ${_tmpID} ${_rezEmergency}
			}
		}
		/next _count
	} 
	
	| control toon
	/if (${Bool[${toonControl}]}) {
		/if (${SpawnCount[pccorpse name ${toonControl} radius ${maxRezRange}]}) {
			/for _count 1 to ${SpawnCount[pccorpse radius ${maxRezRange} ${toonControl}]}
			/if (!${timer_corpse_${NearestSpawn[${_count},pccorpse radius ${maxRezRange} ${toonControl}].ID}}) {
				/varset _tmpID ${NearestSpawn[${_count},pccorpse radius ${maxRezRange} ${toonControl}].ID}
				/if (${switchRezEmergency}) {
					/call cast_rez ${_tmpID} ${_rezEmergency}
				} else {
					/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
				}
			}
			/next _count
		}
	}
	
	| tanks
	/if (${Raid.Members}) {
		/if (${SpawnCount[pccorpse raid tank radius ${maxRezRange}]}) {
			/for _count 1 to ${SpawnCount[pccorpse raid radius ${maxRezRange} tank]}
			/if (!${timer_corpse_${NearestSpawn[${_count},pccorpse raid radius ${maxRezRange} tank].ID}}) {
				/varset _tmpID ${NearestSpawn[${_count},pccorpse raid radius ${maxRezRange} tank].ID}
				/if (${switchRezEmergency} && ${RezTokenList.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
					/call cast_rez ${_tmpID} ${_rezEmergency}
				} else {
					/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
				}
			}
			/next _count
		} 
	
	| remaining raid members
		/if (${SpawnCount[pccorpse raid radius ${maxRezRange}]}) {
			/for _count 1 to ${SpawnCount[pccorpse raid radius ${maxRezRange}]}
			/if (!${timer_corpse_${NearestSpawn[${_count},pccorpse raid radius ${maxRezRange}].ID}}) {
				/varset _tmpID ${NearestSpawn[${_count},pccorpse raid radius ${maxRezRange}].ID}
				/if (${switchRezEmergency} && ${RezTokenList.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
					/call cast_rez ${_tmpID} ${_rezEmergency}
				} else {
					/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
				}
			}
			/next _count
		} 
	}
	
	| guildies - all the time
	/if (${SpawnCount[pccorpse guild radius ${maxRezRange}]}) {
		/for _count 1 to ${SpawnCount[pccorpse radius ${maxRezRange} GUILD]}
		/if (!${timer_corpse_${NearestSpawn[${_count}, pccorpse radius ${maxRezRange} guild].ID}}) {
			/varset _tmpID ${NearestSpawn[${_count}, pccorpse radius ${maxRezRange} guild].ID}
			/if (${switchRezEmergency} && ${RezTokenList.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
				/call cast_rez ${_tmpID} ${_rezEmergency}
			} else {
				/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
			}
		}
		/next _count
	}
	
	| group members out of raid
	/if (${SpawnCount[pccorpse group radius ${maxRezRange}]}) {
		/for _count 1 to ${SpawnCount[pccorpse radius ${maxRezRange} group]}
		/if (!${timer_corpse_${NearestSpawn[${_count}, pccorpse radius ${maxRezRange} group].ID}}) {
			/varset _tmpID ${NearestSpawn[${_count}, pccorpse radius ${maxRezRange} group].ID}
			/if (${switchRezEmergency} && ${RezTokenList.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
				/call cast_rez ${_tmpID} ${_rezEmergency}
			} else {
				/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
			}
		}
		/next _count
	}	

	| outside guild and raid
	| needs /rez everyone switch to be true
	/if (${switchRezEveryone}) {
		/if (${SpawnCount[pccorpse radius ${maxRezRange}]}) {
			/for _count 1 to ${SpawnCount[pccorpse radius ${maxRezRange}]}
			/if (!${timer_corpse_${NearestSpawn[${_count}, pccorpse radius ${maxRezRange}].ID}}) {
				/varset _tmpID ${NearestSpawn[${_count}, pccorpse radius ${maxRezRange}].ID}
				/if (${switchRezEmergency} && ${RezTokenList.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
					/call cast_rez ${_tmpID} ${_rezEmergency}
				} else {
					/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
				}
			}
			/next _count
		}	
	}

	UPDATEHUD FALSE
/return



|***
 *	DES: rez/call casting code.
 *	USE: /call cast_rez [ID] [rez aa|token]
 *	INI: 
 *  NOTE: 
 ***|
sub cast_rez(int _tmpID, string _rezAA)
	DEBUG cast_rez(${_tmpID}, ${_rezAA})
	/if (${Me.Dead}) /return
	/if (${timer_corpse_${_tmpID}}) /return
	AMIDEAD
	CHECKEXIT

	| delay res a sec if they are not on the safe list.
	/if (!${validate_safelist[${Spawn[pccorpse ${_tmpID}].DisplayName}]} || ${Select[${Me.Class.ShortName},DRU,SHM,PAL,NEC]}) {
		/delay ${Int[${Math.Calc[${Math.Rand[${RandRezDelay}]}+1]}]}s
	}
	| /if (!${safelist.Find[${Spawn[pccorpse ${_tmpID}].DisplayName}]} || ${Select[${Me.Class.ShortName},DRU,SHM,PAL,NEC]}) /delay ${Int[${Math.Calc[${Math.Rand[${RandRezDelay}]}+1]}]}s

	| ignore the people we dont like
	/if (${rezlist.Find[${Spawn[pccorpse ${_tmpID}].DisplayName}]}) {
		/call create_timer timer_corpse_${_tmpID}} 30m
		/return
	}
		
	| get the corpse close to us
	/if (${Spawn[pccorpse ${_tmpID}].Distance} > ${combatRadius}*2 && ${Spawn[pccorpse ${_tmpID}].Distance} <= ${maxRezRange}) {
		/squelch /corpse
	}

	
	| can we AA res or token?
	/if (${Me.AltAbilityReady[${_rezAA}]} || (${Me.ItemReady[Token of Resurrection]} && ${switchRezEmergency})) {	

		| res the damn person
		
		| AA rez
		/if (!${switchRezEmergency}) {
			| /if (!${Spawn[pccorpse ${_tmpID}].ID}) /return
			/delay 5s !${Me.Casting.ID}
			/target ID ${_tmpID}
			/delay 1s ${Target.ID} == ${_tmpID}

			/if (${validate_cast[FALSE, alt, "${_rezAA}", ${Me.ID}]}) /call core_cast2 "${_rezAA}" alt ${_tmpID} FALSE

		| token rez
		} else /if (${switchRezEmergency}) {
			/delay 5s !${Me.Casting.ID}
			/target ID ${_tmpID}
			/delay 1s ${Target.ID} == ${_tmpID}
			/if (${validate_cast[FALSE, item, "Token of Resurrection", ${Me.ID}]}) /call core_cast2 "Token of Resurrection" item ${_tmpID} FALSE
		}
	
		| if cast was good, set the timers.
		/if (${Select[${castReturn},CAST_SUCCESS]}) { 
			/call create_timer timer_corpse_${_tmpID} ${tRez_Retry}
			ECHOCHANNEL ${Me.Class.ShortName} Rez: ${Spawn[${_tmpID}].DisplayName}
		} else /if (${Select[${castReturn},CAST_CANCELLED,CAST_INTERRUPTED]}) {
			/call create_timer timer_corpse_${_tmpID} 7s
		} else {
			/if (${Select[${Me.Class.ShortName},CLR]}) {
				/call create_timer timer_corpse_${_tmpID} 30s
			} else /if (${Select[${Me.Class.ShortName},DRU,SHM,PAL]}) {
				/call create_timer timer_corpse_${_tmpID} 45s
			} else {
				/call create_timer timer_corpse_${_tmpID} 1m
			}
		}
		
	}
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
	DEBUG list_add(${_listName}, ${_tmpID})
	/declare _exit int local 0
	/if (!${Bool[${${_listName}.Find[${_tmpID}]}]}) {
		/varset ${_listName} ${${_listName}}|${_tmpID}
		/varset _exit 1
	}
/return ${_exit}



|***
 *	DES: delete from a list
 *	USE: /call list_delete [LISTNAME] [ID|MOBNAME]
 *	INI: 
 *  NOTE: 
 ***|
sub list_delete(string _listName, string _tmpID)
	DEBUG list_delete(${_listName}, ${_tmpID})
	/if (${Bool[${${_listName}.Equal[NULL]}]}) /return
	/if (!${Bool[${_tmpID}]}) /return
	/declare _count int local 0
	/declare _tmpList string local

	/varset _tmpID |${_tmpID}
	/declare _a int local
	/declare _b int local
	/declare _strLeft string local
	/declare _strRight string local
	
	/varcalc _a ${${_listName}.Find[${_tmpID}]}-1
	/varcalc _b ${${_listName}.Length}-${_tmpID.Length}-${_a}
	/if (${_a} > 0) /varset _strLeft ${${_listName}.Left[${_a}]}
	/if (${_a} > 0) /varset _strRight ${If[${Select[${_b},0,NULL,FALSE,""]},,${${_listName}.Right[${_b}]}]} 
	
	|**
	/for _count 1 to ${${_listName}.Count[|]}
		/if (${${${_listName}.Arg[${_count},|]}.Equal[FALSE]}) /continue
		/if (${_tmpID.Find[${${_listName}.Arg[${_count},|]}]}) /continue
		/varset ${_listName} ${_tmpList}|${${_listName}.Arg[${_count},|]}
	/next _count	
	**|
	
	/varset ${_listName} ${_strLeft}${_strRight}
/return



|***
 *	DES: update a list
 *	USE: /call list_update [mobList|addList] [ID]
 *	INI: 
 *  NOTE: 
 ***|
sub list_update(string _listName, string _tmpID)
	DEBUG list_update(${_listName}, ${_tmpID})
	/declare _remove bool local FALSE 

	/if (${${_listName}.Find[${_tmpID}]}) {
		
		| not an NPC or its a pet
		/if (!${Spawn[${_tmpID}].Type.Equal[NPC]} && !${Spawn[${_tmpID}].Master.Type.Equal[NPC]}) /varset _remove TRUE
		
		| no spawn in zone
		/if (!${Spawn[${_tmpID}].ID}) /varset _remove TRUE

		| mob out of range
		/if (${Spawn[${_tmpID}].Distance} > ${MobAgro}) /varset _remove TRUE
		
		| NPC corpse
		/if (${Spawn[npccorpse ${_tmpID}].ID}) /varset _remove TRUE
		/if (${Spawn[${_tmpID}].Type.Equal[Corpse]}) /varset _remove TRUE

		
		/if (${_remove}) {
			/call list_delete ${_listName} ${_tmpID}
		}
	}
/return



|***
 *	DES: checks for adds in the local area, calls updates to the needed lists.
 *	USE: /call check_for_adds
 *	INI: 
 *  NOTE: 
 ***|
sub check_for_adds()
	DEBUG check_for_adds()
	/declare _tmpID				string	local
	/declare _numMobs			int			local 1

	:loopnextmob
	/if (${NearestSpawn[${_numMobs},npc radius ${MobAgro} targetable los].ID}) {
		/varset _tmpID ${NearestSpawn[${_numMobs},npc radius ${MobAgro} targetable ${If[${switchOverrideLos},,los]} playerstate 4].ID}
			/if (${switchdeBuffEradicate} && !${Defined[timer_eradicate_${_tmpID}]}) /call create_timer timer_eradicate_${_tmpID} 1
			| /if (${switchdeBuffTash} && !${Defined[timer_tash_${_tmpID}]}) /call create_timer timer_tash_${_tmpID} 1
			/if (!${Defined[timer_tash_${_tmpID}]}) /call create_timer timer_tash_${_tmpID} 1
			/if (${switchdeBuffSlow} && !${Defined[timer_slow_${_tmpID}]}) /call create_timer timer_slow_${_tmpID} 1
			/if (${switchdeBuffCripple} && !${Defined[timer_cripple_${_tmpID}]}) /call create_timer timer_cripple_${_tmpID} 1
			/if (${switchdeBuffMezz} && !${Defined[timer_mez_${_tmpID}]}) /call create_timer timer_mez_${_tmpID} 1
		/if (${Spawn[id ${_tmpID}].Type.Equal[NPC]} && ${Spawn[id ${_tmpID}].Distance}<=${MobAgro} && ${Spawn[id ${_tmpID}].LineOfSight} && !${ExcludeList.Find[${Spawn[id ${_tmpID}].DisplayName}]} && ${Spawn[id ${_tmpID}].Type.NotEqual[Untargetable]}) {
			/if (!${timer_mez_${_tmpID}}) {
				/if (${_tmpID} != ${Me.XTarget[1].ID}) /call list_add addList ${_tmpID}
				/call list_add mobList ${_tmpID}
				/if (${Macro.Return.Equal[1]}) {
					| VOUT ${owarning} ADD ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax] ${sep} [${ctar}${_tmpID}\ax]
					/if (${Me.Casting.Name.NotEqual[${spellMez}]} && ${Me.Casting.ID}) {
						/if (${validate_plugin[FALSE, |mq2cast]}) /call Interrupt
						/stopcast
					}
					/if (${NoMezzList.Find[${Spawn[${_tmpID}].DisplayName}]}) {
						VOUT ${owarning} IMMUNE TO\ax ${csp}${spellMez}\ax ${sep} [${ctar}${_tmpID}\ax]
						/call create_timer timer_mez_${_tmpID} 1.5m
					}          	
				}
			} else /if (${timer_mez_${_tmpID}}) {
				/call list_delete mobList ${_tmpID}
			}
		}
		/varcalc _numMobs ${_numMobs}+1
		/goto :loopnextmob
	}
/return



|***
 *	DES: adds /t for buffs from queue
 *	USE: uses INI [Buffs] 
 *	INI: 
 *  NOTE: 
 ***|
sub queue_buff_add(int _buffee, int _buffnumber)
	DEBUG queue_buff_add(${_buffee}, ${_buffnumber})
  /declare _count				int local
  /declare _compare			int local
	AMIDEAD
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
          /break
        }
      /next _count
    }
  }
/return



|***
 *	DES: removes /t for buffs from queue
 *	USE: uses INI [Buffs] 
 *	INI: 
 *  NOTE: 
 ***|
sub queue_buff_delete(int _buffee, int _buffnumber)
	DEBUG queue_buff_delete(${_buffee}, ${_buffnumber})
	/declare _count 			int local
	/for _count 1 to ${queueCountMax}
		/if (((${Spawn[${queueList[${_count},1]}].State.Equal["DEAD"]} || !${Spawn[${queueList[${_count},1]}].ID}) || ((${queueList[${_count},1]}==${_buffee} || (!${_buffee} && (${Group.Member[${Spawn[${queueList[${_count},1]}].DisplayName}].ID} || ${Group.Member[${Spawn[${queueList[${_count},1]}].Master.DisplayName}].ID}))) && (${queueBuffID[${queueList[${_count},2]}]}==${_buffnumber} || ${queueList[${_count},2]}==${_buffnumber}))) && ${queueList[${_count},1]}) {
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
 *  NOTE: queue counts start at 1, but they are empty.
 ***|
sub check_buffs_tell()
	DEBUG check_buffs_tell()
	CHECKINPUTS
	UPDATEHUD "BUFF tell"
	/declare _count					int local
	/declare _tmpVAR				int local ${Math.Calc[${setMaxINIBuffCount}]}
	/declare _tmpID					int local 0

	/if (${coreAuto}) {
		/call create_timer timer_check_buffs_tell ${tTell_Buff}
	}
	
	/for _count 1 to ${queueCount}
		| make sure we are at least in the correct range
		/if (!${Range.Between[1,${queueCount}:${Int[${_count}]}]}) /continue
	
		| queues default to 1 but empty. if empty., go away
		/if (${queueCount} < 1 || ${queueCount} == 0 || ${queueList[${_count},1]} == 0) /return
	
		CHECKCOMBATCAST

		| target dead or not in zone? fuck their request 
		/if (${Spawn[${queueList[${_count},1]}].Dead} || !${Spawn[${queueList[${_count},1]}].ID}) {
			/call queue_buff_delete ${queueList[${_count},1]} ${queueList[${_count},2]}
			/return
		}

		AMIDEAD
		CHECKEXIT
		CHECKINPUTS
		/varset _tmpID ${queueList[${_count},1]}
		/varset _tmpVAR ${queueList[${_count},2]}
		| /if (${_tmpID}) /break
	/next _count

	| no target. were out of here
	/if (!${_tmpID}) /return

	| even more checks for items/spells
	/if (${FindItem[${Buff${_tmpVAR}}].ID}) {
		/if (${FindItem[${Buff${_tmpVAR}}].TimerReady} > 20) /return
	} else {
		/if (!${Me.SpellReady[${Buff${_tmpVAR}}]} && ${Me.Gem[${Buff${_tmpVAR}}]}) /return
	}	

	| bards are a pita..
	/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop
	/target ID ${_tmpID}
	/delay 5s ${Target.ID} == ${_tmpID}
	/if (${Target.ID} != ${_tmpID} || !${Target.ID}) /return
	
	| do something.. seriously we made it this far..
	/if (${FindItem[${Buff${_tmpVAR}}].ID}) {
		/if (${validate_cast[FALSE, item, "${Buff${_tmpVAR}}", ${_tmpID}]}) /call core_cast2 "${Buff${_tmpVAR}}" item ${_tmpID} FALSE
	} else /if (${Me.AltAbility[${Buff${_tmpVAR}}]}) {
		/if (${validate_cast[FALSE, alt, "${Buff${_tmpVAR}}", ${_tmpID}]}) /call core_cast2 "${Buff${_tmpVAR}}" alt ${_tmpID} FALSE
	} else {
		/if (${validate_cast[FALSE, spell, "${Buff${_tmpVAR}}", ${_tmpID}]}) /call core_cast2 "${Buff${_tmpVAR}}" ${buffGem} ${_tmpID} FALSE
	}

	| clear that request form the queue. we no longer like that person
	/if (${Select[${castReturn},CAST_SUCCESS]}) /call queue_buff_delete ${_tmpID} ${_tmpVAR}
	
	UPDATEHUD FALSE
/return


|******************************************************************|



|***
 *	DES: creates/displays who has core set to manual mode in the HUD
 *	USE: /call broadcast_status
 *	INI: core.ini -> [transmit_status]
 *  NOTE: this only works for your group and on the same computer. it shares the settings in core.ini
 ***|
sub broadcast_status()
	DEBUG broadcast_status()
	/declare _count int local
	/declare _build string local 
	/declare _INIstatusName string local
	/declare _toonName string local FALSE
	
	| create your toon Core.INI entry
	/declare _INIstatus string local ${Ini[${INICore},"transmit_status","${Me.DisplayName}"]}

	| adjust the INI to your current status for youself and others If it is different
	/if (${setBroadcast.Equal[TRUE]} && ${_INIstatus.NotEqual[DISABLED]}) {
		/if (${_INIstatus.NotEqual[${coreAuto}]}) /ini "${INICore}" "transmit_status" "${Me.DisplayName}" "${coreAuto}"
	}

	| creat the hud display list relative to who has coreAuto OFF and is a group member
	
	/if (${setCommunications.Equal[eqbc]}) {
		/for _count 1 to ${NetBots.Counts}
			/varset _INIstatus ${Ini[${INICore},"transmit_status","${NetBots.Client[${_count}]}"]}
			/if (${_INIstatus.Equal[DISABLED]}) /continue
			
			/varset _INIstatusName ${NetBots.Client[${_count}]}
			
			/if (!${Bool[${_INIstatus}]} && ${Group.Member[${_INIstatusName}].ID}) {
				
				| do we want to use CLS or toon name?
				/if (!${switchUseCLSinHUD}) {
					/varset _build ${_build} ${If[${_INIstatus},,${_INIstatusName.Left[3]}]}
				} else /if (${switchUseCLSinHUD}) {
					/varset _build ${_build} ${If[${_INIstatus},,${Spawn[pc ${_INIstatusName}].Class.ShortName}]}
				}
			}
		/next _count
		
	} else /if (${setCommunications.Equal[dannet]}) {
		/for _count 1 to ${DanNet.PeerCount}
			
			/if (${DanNet.FullNames}) {
				/varset _toonName ${DanNet.Peers[all].Arg[${_count},|].Right[-${Int[${Math.Calc[${MacroQuest.Server.Length}+1]}]}]}
			} else {
				/varset _toonName ${DanNet.Peers[all].Arg[${_count},|]}
			}

			/varset _INIstatus ${Ini[${INICore},"transmit_status","${_toonName}"]}
			/if (${_INIstatus.Equal[DISABLED]}) /continue
			
			/varset _INIstatusName ${_toonName}
			
			/if (!${Bool[${_INIstatus}]} && ${Group.Member[${_INIstatusName}].ID}) {
				
				| do we want to use CLS or toon name?
				/if (!${switchUseCLSinHUD}) {
					/varset _build ${_build} ${If[${_INIstatus},,${_INIstatusName.Left[3]}]}
				} else /if (${switchUseCLSinHUD}) {
					/varset _build ${_build} ${If[${_INIstatus},,${Spawn[pc ${_INIstatusName}].Class.ShortName}]}
				}
			}
		/next _count
	}

	/varset hudBroadcast ${_build}
	/call create_timer timer_check_status ${tCheck_status}
/return



|***
 *	DES: advanced loot routine(s)
 *	USE: auto
 *	INI: core.loot.ini
 *  NOTE: set a person in the group as master looter. Or, as the raid master looter.
 *	NOTE2: The event in this sectction is designed to handle lore items the toon already posseses. as of this, MQ has no property .Member value for LORE in ${AdvLoot
 ***|
#Event forcedloreleave "#*#already has #1# and it is lore#*#"
#Event forcedloreleave "#*#does not want #1#. It is either on their never list or they have selected No#*#"
sub Event_forcedloreleave(string line, string _itemName)
	/if (!${switchUseADVL}) /return
	/varset _itemName ${AdvLoot.SList[1].Name}
	VOUT Cursor ${sep} ${cinfo}${_itemName}\ax -> ${oleave} 
	/ini "${INILoot}" "${_itemName.Left[1]}" "${_itemName}" Leave
	/doevents flush
/return

sub do_loot()
	DEBUG do_loot()
	AMIDEAD
	CHECKINPUTS
	/if (!${switchUseADVL}) /return
	/if (${Stick.Active} || !${Me.FreeInventory} || !${Me.UseAdvancedLooting}) /return

	| I am not the raid loot master /sigh
	/if (${Raid.Members} && ${Raid.MasterLooter.ID} != ${Me.ID} && !${switchOverrideLoot}) /return
	/if (!${Raid.Members} && ${Bool[${Group.Members}]} && ${Group.MasterLooter.ID} != ${Me.ID} && !${switchOverrideLoot}) /return

	UPDATEHUD "Loot"

	/delay 2s !${AdvLoot.LootInProgress}
	/if (${Me.UseAdvancedLooting} && ${Group.MasterLooter.Name.Equal[${Me.DisplayName}]} && !${switchAutoCursor}) /varset switchAutoCursor TRUE
	
	/declare _scount int local 0
	/declare _pcount int local 0
	
	| Item To Process Name (the thingie in the list of advloot)
	/declare _advlItem string local
	
	| _advlChoice is what is listed to do with the items: Leave,Destroy,Keep,Sell,Announce
	/declare _advlChoice string local
	
	| if we want to give it to someone else
	/declare _advlPerson string local

	/if (${Raid.Members} || ${Bool[${Group.Members}]}) {
		| Shared List Sorting
		/for _scount ${AdvLoot.SCount} downto 1
			/varset _advlItem ${AdvLoot.SList[${_scount}].Name}
			
			| corect the INI entry if not set for the new form
			/if (!${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"].Count[|]}) {
				VOUT Fixing Loot: ${_advlItem}
				/ini "${INILoot}" "${_advlItem.Left[1]}" "${_advlItem}" |${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"]}
			}
			/varset _advlChoice ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"].Arg[1,|]}
			/varset _advlPerson ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"].Arg[2,|]}


			| Does it exist in the loot.ini (check to see if there is a choice for it)
			/if (${AdvLoot.SList[${_scount}].ID} && !${Select[${_advlChoice},Leave,Keep,Destroy,Sell,Announce,Ignore]}) {
				/echo ${_advlChoice}
				| Add item to ini if it not in there
				/if (${AdvLoot.SList[${_scount}].NoDrop} || ${FindItem[${_advlItem}].Lore} || ${FindItemBank[${_advlItem}].Lore}) {
					VOUT ${oadd} ${sep} ${_advlItem} = ${oleave} [no drop/lore].
					/ini "${INILoot}" "${_advlItem.Left[1]}" "${_advlItem}" |Leave
				} else {
					VOUT ${oadd} ${sep} ${_advlItem} = ${okeep} [default].
					/ini "${INILoot}" "${_advlItem.Left[1]}" "${_advlItem}" |Keep
				}
				| if we make a new entry, update.
				/varset _advlChoice ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"].Arg[1,|]}
			}  
			
			| now we do something with the item.
			
			| if we are going to leave the item or if it is lore AND we have one already. Conversly, we loot if we intend to keep or destroy the item
			| pass the item off if there is a person listed and in group
			/if (${Bool[${_advlPerson}]} && ${Group.Member[${_advlPerson}].ID}) {
					/delay 1s !${AdvLoot.LootInProgress}
					/advloot shared ${_scount} giveto ${_advlPerson}
					/delay 1
			
			| ditch the item if we need too
			} else /if (${Select[${_advlChoice},Leave,Announce]} || ${FindItem[${_advlItem}].Lore} || ${FindItemBank[${_advlItem}].Lore}) {
				/if (${Select[${_advlChoice},Announce]}) ECHOCHANNEL Loot: Leaving ${_advlItem}
				/delay 1s !${AdvLoot.LootInProgress}
				/advloot shared ${_scount} leave
				/delay 1
				/continue

			| or we keep it.
			} else /if (${AdvLoot.SList[${_scount}].ID}) {
				/if (${Select[${_advlChoice},Keep,Destroy,Sell]}) {
					/delay 1s !${AdvLoot.LootInProgress}
					/advloot shared ${_scount} giveto ${Me.Name}
					/delay 1
				}
			}
		/next _scount
	}
	
	| Personal List sorting. ALSO for solo killing
	/if (${AdvLoot.PCount}) {
		/for _pcount ${AdvLoot.PCount} downto 1
			/varset _advlItem ${AdvLoot.PList[${_pcount}].Name}
			/varset _advlChoice ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"].Arg[1,|]}
			
			/if (${AdvLoot.PList[${_pcount}].ID} && !${Select[${_advlChoice},Leave,Keep,Destroy,Sell,Announce,Ignore]}) {
				/if (${Select[${_advlChoice},Announce]}) {
					ECHOCHANNEL Loot: Leaving ${_advlItem}
					/delay 1s !${AdvLoot.LootInProgress}
					/advloot personal ${_pcount} leave
				} else /if (${AdvLoot.PList[${_pcount}].NoDrop} || ${FindItem[${_advlItem}].Lore} || ${FindItemBank[${_advlItem}].Lore}) {
					VOUT Cursor ${sep} ${cinfo}${_advlItem}\ax = ${oleave} loot file [no drop].
					/ini "${INILoot}" "${_advlItem.Left[1]}" "${_advlItem}" |Leave
					| /goto :looppersonalitem
				} else {
					VOUT Cursor ${sep} ${cinfo}${_advlItem}\ax = ${okeep} loot file [default].
					/ini "${INILoot}" "${_advlItem.Left[1]}" "${_advlItem}" |Keep
				}
				/varset _advlChoice ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"].Arg[1,|]}
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
					/call sort_cursor TRUE
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
				/continue
			} else /if (${Select[${_advlChoice},Keep,Sell]}) {
				/delay 1s !${AdvLoot.LootInProgress}
				/advloot personal ${_pcount} loot
				/delay 1
				/if (${Window[ConfirmationDialogBox].Open}) {
					/nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
					/delay 1
				}
			} 
		/next _pcount
	}        
	UPDATEHUD FALSE
/return



|***
 *	DES: buy items from a vendor
 *	USE: 
 *	INI: 
 *  NOTE: you have to open the merchants window. this works on the loyalty vendor in PoK as well.
 ***|
sub vendor_buy(string _itemtoBuy, int _buyAmount)
	DEBUG vendor_buy(${_itemtoBuy}, ${_buyAmount})
	/if (!${Select[${Target.Type},npc,pet]} || !${Target.ID} || ${Target.Type.Equal[pet]} && !${Target.DisplayName.Find[familiar]}) /return
	/declare _count int local 0
	/declare _qty int local 0
	/declare _listItem int local 0

	/varcalc _qty ${_buyAmount}-${FindItemCount[${_itemtoBuy}]}
	/delay 3
	/if (${FindItemCount[${_itemtoBuy}]} >= ${_buyAmount}) {
		VOUT I already have ${_itemtoBuy} ${sep} (${cnum}${FindItemCount[${_itemtoBuy}]}\ax)
		/return
	}   

	| set item in the list window
	/if (${Target.ID} == ${Spawn[npc alerynril].ID}) {
		/varset _listItem ${Window[NewPointMerchantWnd].Child[NewPointMerchant_ItemList].List[=${_itemtoBuy}]}
	} else {
		/varset _listItem ${Window[MerchantWnd].Child[ItemList].List[=${_itemtoBuy},2]}}
	} 	

	/if (!${_listItem}) {
		VOUT ${ocaution} ${sep} Couldn't find ${_itemtoBuy}
		/return
	} else {
		/if (${Target.ID} == ${Spawn[npc alerynril].ID}) {
			/notify NewPointMerchantWnd NewPointMerchant_ItemList listselect ${_listItem}
		} else {
			/notify MerchantWnd ItemList listselect ${_listItem}
		}
		/delay 5       
	}	

	VOUT Buying ${sep} ${citem}${_itemtoBuy}\ax 'till I get ${_buyAmount} 

	:loopbuyitems
	CHECKINPUTS
	/if (${InventoryFull}) /return

	/if (${_qty} > 0) {
		/if (${_qty} > 19) {
			| /if (${InventoryFull}) /return
			/if (${Target.ID} == ${Spawn[npc alerynril].ID}) {
				/shiftkey /notify NewPointMerchantWnd NewPointMerchant_PurchaseButton leftmouseup
			} else {            	
				/shiftkey /notify merchantwnd MW_Buy_Button leftmouseup
			}	
			/delay 5 ${FindItemCount[B${_itemtoBuy}]} >= ${_qty}
			/varcalc _qty ${_buyAmount}-${FindItemCount[${_itemtoBuy}]} 
			/if (${_qty} <= 0) /return
			}
			/delay 5
			/if (${_qty} > 19) /goto :loopbuyitems
		}
		/if (${_qty} > 0 && ${_qty} < 20) {
			/for _count 1 to ${_qty}
				| /if (${Bool[${InventoryFull}]}) /return 
				/if (${Target.ID} == ${Spawn[npc alerynril].ID}) {
					/ctrlkey /notify NewPointMerchantWnd NewPointMerchant_PurchaseButton leftmouseup
				} else {            	
					/ctrlkey /notify merchantwnd MW_Buy_Button leftmouseup
				}
				/delay 1s ${FindItemCount[${_itemtoBuy}]} >= ${Math.Calc[${FindItemCount[${_itemtoBuy}]}+${_count}]}
				/echo Buying ${_itemtoBuy} ${_count} of ${_qty}
				/if (${_count} >= ${_qty}) /return
			/next _count
			/varcalc _qty ${_buyAmount}-${FindItemCount[${_itemtoBuy}]}

		}
	}
/return



|***
 *	DES: finds the items to sell to a vendor
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub vendor_sell(string _itemtoSell)
	DEBUG vendor_sell(${_itemtoSell})
	/declare _pack 					int local
	/declare _slot 					int local
	/declare _lootItem 			string local
  /if (!${Window[MerchantWnd].Open}) /return

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
			/if (${InvSlot[pack${_pack}].Item.Container} == 0) /goto :nextpack
			| Set _pack 1 to number of bag slots and loop through slots looking for items marked sell
			/for _slot 1 to ${InvSlot[pack${_pack}].Item.Container}
				/if (${InvSlot[pack${_pack}].Item.Item[${_slot}].ID}) {
					/varset _lootItem ${InvSlot[pack${_pack}].Item.Item[${_slot}].Name}
					/if (${Ini[${INILoot},"${_lootItem.Left[1]}","${_lootItem}"].Equal[Sell]}) /call vendor_sell_item "${_lootItem}"
				}
			/next _slot
			:nextpack
		/next _pack  	
		/doevents flush sellstuff
	} else {
		/call vendor_sell_item "${_itemtoSell}"
	}
	VOUT ${onotice} ${sep} Done Selling.
/return



|***
 *	DES: sells the actual item to the vendor
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub vendor_sell_item(string _itemtoSell)
	DEBUG vendor_sell_item(${_itemtoSell})
  /if (!${Window[MerchantWnd].Open}) {
  	VOUT Go and open a merchant window.
  	/return
  }

	/while (${FindItemCount[=${_itemtoSell}]}) {
		VOUT Selling ${sep} ${citem}${_itemtoSell}\ax ${sep} (${cnum}${FindItemCount[${_itemtoSell}]}\ax)
		/if (${InvSlot[pack${Select[${FindItem[=${_itemtoSell}].ItemSlot},23,24,25,26,27,28,29,30,31,32]}].Item.Container}) {
			/squelch /nomodkey /itemnotify in Pack${Math.Calc[${FindItem[=${_itemtoSell}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${_itemtoSell}].ItemSlot2}+1]} leftmouseup
		} else /if (${FindItem[=${_itemtoSell}].InvSlot}) {
			/squelch /nomodkey /itemnotify ${FindItem[=${_itemtoSell}].InvSlot} leftmouseup
		} else /if (!${FindItem[=${_itemtoSell}].InvSlot}) {
			VOUT ${onotice} ${sep} No ${citem}${_itemtoSell}\ax to sell.
			/break
		}
		/delay 5
		/nomodkey /shiftkey /notify merchantwnd MW_Sell_Button leftmouseup
		/delay 1s
	}
/return



|***
 *	DES: Make sure the AA is up all the time
 *	USE: /call check_AA_now
 *	INI: Toon.INI [NowAA] ->
 *  NOTE: This is hardcoded for only 2 skills. suck it up.
 ***|
sub check_AA_now()
	DEBUG check_AA_now()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS

	/if (${Bool[${AANow1}]}) {
		/if (${validate_cast[FALSE, alt, "${AANow1}", ${Me.ID}]}) /call core_cast2 "${AANow1}" alt 0 FALSE
	}

	/if (${Bool[${AANow2}]}) {
		/if (${validate_cast[FALSE, alt, "${AANow2}", ${Me.ID}]}) /call core_cast2 "${AANow2}" alt 0 FALSE
	}
/return



|***
 *	DES: Make sure the Disc is up all the time
 *	USE: /call check_disc_now
 *	INI: Toon.INI [NowDisc] ->
 *  NOTE: This is hardcoded for only 2 skills.
 ***|
sub check_disc_now()
	DEBUG check_disc_now()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS

	| nowDisc1
	/if (${Bool[${DiscNow1}]} && ${Me.CombatAbilityReady[${DiscNow1}]} && ${Spell[${DiscNow1}].Stacks}) {
		/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop
		/if (!${Bool[${Me.Buff[${DiscNow1.Left[-7]}]}]} && !${Bool[${Me.Song[${DiscNow1.Left[-7]}]}]}) {
			/if (${validate_cast[FALSE, disc, "${DiscNow1}", ${Me.ID}]}) /call core_cast2 "${DiscNow1}" disc 0 FALSE
		}
	}
	| nowDisc2
	/if (${Bool[${DiscNow2}]} && ${Me.CombatAbilityReady[${DiscNow2}]} && ${Spell[${DiscNow2}].Stacks}) {
		/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop
		/if (!${Bool[${Me.Buff[${DiscNow2.Left[-7]}]}]} && !${Bool[${Me.Song[${DiscNow2.Left[-7]}]}]}) {
			/if (${validate_cast[FALSE, disc, "${DiscNow2}", ${Me.ID}]}) /call core_cast2 "${DiscNow2}" disc 0 FALSE
		}
	}	
/return



|***
 *	DES: last mob is dedded we need a new thing(s) to kill
 *	USE: /call get_target
 *	INI: 
 *  NOTE: got lost hunting for bitches. have your people call my people.
 *	SERIOUS: this will find a valid target to kill within the pullers defined agro range. bring it to the group and start teaching the gospel.
 ***|
sub get_target()
	DEBUG get_target()
	/if (!${switchPull} || !${coreAuto}) /return
	AMIDEAD
	CHECKINPUTS
	/declare _tmpID int local
	/declare _tmpAgroRange int local ${Math.Calc[${MobAgro}/10]}
	/declare _tmpCombatRange int local 31
	/declare _count int local
	/declare _mobHP int local 102
	/declare _priorityMob bool local FALSE
	
	/if (!${switchOffTank}) {
		UPDATEHUD "Pull"		
	} else /if (${switchOffTank})
		UPDATEHUD "Grab"		
	}
	
	| time to step the agro range out so we get mobs closer first	
	/while (${_tmpAgroRange} <= ${MobAgro} && !${Target2Kill}) {
	
		| search local ${MobAgro} range in steps of 10 for mob acquisition
		/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${_tmpAgroRange} zradius ${zradius} targetable]}) {
			/for _count 1 to ${SpawnCount[npc ${If[${switchOverrideLos},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${_tmpAgroRange} zradius ${zradius} targetable]}
				/if (!${switchPull} || !${coreAuto}) /return
				AMIDEAD
				/varset _tmpID ${NearestSpawn[${_count}, npc ${If[${switchOverrideLos},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${_tmpAgroRange} zradius ${zradius} targetable].ID}
				
				| these things are our friends. dont kill them
				/if (${Select[${Spawn[id ${_tmpID}].Type},AURA,BANNER,CAMPFIRE,CORPSE,CHEST,ITEM,TRIGGER,TRAP,TIMER,MOUNT,Mercenary]}) /continue
				/if (${ExcludeList.Find[${Spawn[id ${_tmpID}].DisplayName}]}) /continue
				
				| CYA for shit MQ will pick up in the environment that does not have a name
				/if (${Spawn[id ${_tmpID}].DisplayName.Length} < 1) /continue

				| get the lowest HP mob if we can
				/if (${Spawn[id ${_tmpID}].PctHPs} <= ${_mobHP}) {
					/varset _mobHP ${Spawn[id ${_tmpID}].PctHPs}
					/varset Target2Kill ${_tmpID}
				}

				| is this a priority mob?
				/if (${NoMezzList.Find[${Spawn[id ${_tmpID}].DisplayName}]} || ${NameList.Find[${Spawn[id ${_tmpID}].DisplayName}]}) {

					| off tanks have to sort by a different position
					/if (${switchOffTank} && ${Me.XTarget[1].ID} == ${_tmpID}) /continue 

					/varset _priorityMob TRUE
					/break
				}					
		
			/next _count
		}
		
		| offtank has nothing special to do? back to your damn job!
		/if (${switchOffTank} && !${_priorityMob}) {
			/call get_assist_target
			/return
		}
		
		/if (${_priorityMob}) /break
		/varcalc _tmpAgroRange ${_tmpAgroRange}+${CombatDistance}
	}
		
	| lastly, kill all the evil pets. if the owner is dead.
	/if (!${_priorityMob}) {
		/if (${SpawnCount[pet ${If[${switchOverrideLos},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${MobAgro} zradius ${zradius} targetable]} && !${SpawnCount[npc ${If[${switchOverrideLos},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${combatRadius} zradius ${zradius} targetable]}) {
			/for _count 1 to ${SpawnCount[pet ${If[${switchOverrideLos},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${MobAgro} zradius ${zradius} targetable]}
				/if (!${switchPull} || !${coreAuto}) /return
				AMIDEAD
				/varset _tmpID ${NearestSpawn[${_count}, pet ${If[${switchOverrideLos},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${MobAgro} zradius ${zradius} targetable].ID}
				/if (${Spawn[id ${_tmpID}].Master.Type.Equal[Corpse]} || !${Spawn[id ${_tmpID}].Master.ID}) /varset Target2Kill ${_tmpID}
			/next _count	
		} 
	}

	/if (${Target2Kill}) {
		| Disable /camp for pull
		/if (${HomeSet}) {
			/squelch /makecamp pause
		}
	
		| target the pull
		/if (${Target.ID} != ${Target2Kill}) /target ID ${Target2Kill}
		
		/varset _tmpCombatRange ${Spawn[id ${Target2Kill}].MaxRangeTo}
	}
		
	| get the mob to us
	/while (!${is_target_dead[${Target2Kill}]} && ${Spawn[id ${Target2Kill}].Distance} >= ${_tmpCombatRange} && !${Select[${Me.Class.ShortName},MAG,NEC,BST,ENC]} && ${Spawn[ID ${Target2Kill}].Distance} <= ${MobAgro}) { 
		CLEARCURSOR TRUE
		CHECKINPUTS
		AMIDEAD

		/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${MobAgro} targetable]} && ${Me.State.NotEqual[STAND]}) /stand
		/if (${Me.Stunned}) /return
		/if (!${switchPull} || !${coreAuto}) /return
		
		| keep us facing the target just enough...
		/if (${Target.ID} && (${switchCombatMelee} || ${switchCombatRange})) {
			/if (!${Range.Between[0,${SetFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
		}
			
		| check target is LoS. if stays out of sight too long. reset
		/if (!${Spawn[${Target}].LineOfSight} && !${switchOverrideLos}) {
			/delay 2s ${Spawn[${Target}].LineOfSight}
			/if (!${Spawn[${Target}].LineOfSight}) {
				/call clear_combat
				/return
			}
		
		| check targets range. if out of range too long. reset
		} else /if (${Spawn[${Target}].Distance} > ${MobAgro}) {
			/delay 2s ${Spawn[${Target}].Distance} < ${MobAgro}
			/if (${Spawn[${Target}].Distance} > ${MobAgro}) {
				/call clear_combat
				/return
			}
		}
		
		| shadowknight attraction
		/if (${Select[${Me.Class.ShortName},SHD]}) {
			/if (${switchAttraction} && ${Target.Distance} < ${MobAgro} && ${Target.Distance} > 40) {
				/if (${validate_cast[FALSE, alt, "Hate's Attraction", ${Target2Kill}]}) /call core_cast2 "Hate's Attraction" alt ${Target2Kill} FALSE
			}
		
		| paladin attraction
		} else /if (${Select[${Me.Class.ShortName},PAL]}) {
			/if (${switchAttraction} && ${Target.Distance} < ${MobAgro} && ${Target.Distance} > 40) {
				/if (${validate_cast[FALSE, alt, "Divine Call", ${Target2Kill}]}) /call core_cast2 "Divine Call" alt ${Target2Kill} FALSE
			}
			
		| warrior attraction
		} else /if (${Select[${Me.Class.ShortName},WAR]}) {
			/if (${switchAttraction} && ${Target.Distance} < ${MobAgro} && ${Target.Distance} > 40) {
				/if (${validate_cast[FALSE, alt, "Warlord's Grasp", ${Target2Kill}]}) /call core_cast2 "Warlord's Grasp" alt ${Target2Kill} FALSE
			}
		}

		| spell pull mob
		/if (${Bool[${Spell[${RangeSpell}]}]} && ${Bool[${Me.Gem[${RangeSpell}]}]}) {
			/if (${validate_cast[FALSE, spell, "${RangeSpell}", ${Target2Kill}]}) /call core_cast2 "${RangeSpell}" 0 ${Target2Kill} FALSE

		| item pull mob
		} else /if (${Me.ItemReady[${RangeClickItem}]}) {
			/if (${validate_cast[FALSE, item, "${RangeClickItem}", ${Target2Kill}]}) /call core_cast2 "${RangeClickItem}" item ${Target2Kill} FALSE

		| disc pull mob
		} else /if (${Me.CombatAbilityReady[${RangeDisc}]}) {
			/if (${validate_cast[FALSE, disc, "${RangeDisc}", ${Target2Kill}]}) /call core_cast2 "${RangeDisc}" disc ${Target2Kill} FALSE

		| pew pew
		} else {
			/if (${Spawn[ID ${Target2Kill}].Distance} > 31) {
				/for _count 1 to ${RangeItem.Count[|]}
					/if (!${Bool[${FindItemCount[${RangeItemSummon}]}]} && ${Bool[${RangeItemSummon}]}) /call check_ammo
				/next _count
				/range
			}
		}

		/if (${Spawn[ID ${Target2Kill}].Distance} < 32 && ${Spawn[ID ${Target2Kill}].Distance} > ${CombatDistance}) {
			/if (${switchCombatMelee}) /squelch /stick ID ${Target2Kill} ${CombatDistance}
		}


		/delay 1
		/if (${is_target_dead[${Target2Kill}]}) /break
	}
		
	| stick to the mob		
	/if (${Spawn[ID ${Target2Kill}].Distance} <= ${_tmpCombatRange} && !${Select[${Me.Class.ShortName},MAG,NEC,BST,ENC]} && !${Me.Rooted.ID}) {
		/if (${switchCombatMelee}) /squelch /stick ID ${Target2Kill} ${CombatDistance}
		/return

	| pet auto send to kill
	} else /if (${Target2Kill} && ${Spawn[id ${Target2Kill}].Distance} <= ${MobAgro} && ${Select[${Me.Class.ShortName},MAG,NEC,BST,ENC]}) {
		/if (${Target.ID} != ${Target2Kill}) /target ID ${Target2Kill}
		/if (${Target.ID} && !${Bool[${Me.Pet.Target}]}) {
			/squelch /pet attack		
		}
	}

	UPDATEHUD FALSE
/return



|***
 *	DES: tells your BC people to hand in up to 4 items to your target. includes command giver
 *	USE: /bc handin |item1|item2|item3|item4
 *	INI: 
 *  NOTE: THIS HAS NOT BEEN TESTED!! you could be handing in your soul and not even know it.
 ***|
sub hand_in(string _sender, string _verbage)
	DEBUG hand_in(${_sender}, ${_verbage})
	/if (${Spawn[pc ${_sender}].ID}) /assist ${_sender}
	/delay 1s ${Me.AssistComplete}
	/if (Target.Distance} > 100) /return
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
/return



|***
 *	DES: will dump all worn or invnetory augments into an INI.
 *	USE: /echoaugments
 *	INI: core.augmentdump.ini
 *  NOTE: recursive of augments in items in bags.
 ***|
sub echo_augments()
	DEBUG echo_augments()
	/declare _slot 			int local
	/declare _augslot 	int local
	/declare _bagslot 	int local
	
	| worn inventory skip powersource (21)
	/for _slot 0 to 22
		/if (${_slot} == 21) /continue
		/for _augslot 0 to ${Me.Inventory[${_slot}].Augs}
			/if (${Me.Inventory[${_slot}].AugSlot[${_augslot}].Empty}) /continue
			/ini "${INIAugDump}" "${Me.DisplayName} - Worn" "${Me.Inventory[${_slot}].Name} ${Me.Inventory[${_slot}].AugSlot[${_augslot}].Type}" "${Me.Inventory[${_slot}].AugSlot[${_augslot}].Name}"
		/next _augslot
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
/return



|***
 *	DES: will watch for a spawn in the zone
 *	USE: /watchspawn [NAME]
 *	INI: 
 *  NOTE: resets on start
 ***|
sub check_watch_spawn()
	DEBUG check_watch_spawn()
	
	/declare _correctWatchCount int local ${SpawnCount[npc ${watchSpawn}]}
	/varset hudwatchspawn ${_correctWatchCount}
	
	| count mismatch
	/if (${watchSpawnCount} != ${_correctWatchCount}) {
		/varset watchSpawnCount ${_correctWatchCount}
		/squelch /mapshow ${watchSpawn}
	}

	/call create_timer timer_check_watch_spawn ${tWatch_Spawn}
/return



|***
 *	DES: Simply gives cash to an NPC. 
 *	USE: /givecoin [ammount] [stacks] [plat|gold|copper|silver]
 *	INI: NA
 *  NOTE: It's not dumb if it works.
 ***|
sub give_coin(int _ammount, int _stacks, string _coin)
	DEBUG give_coin(${_ammount}, ${_stacks}, ${_coin})
	/declare _count 			int local 
	/declare _countloop 	int local 1
	/declare _coinID			int local 99

	| check target
	/if (!${Target.ID} || ${Taret.ID} == ${Me.ID} || ${Target.ID} == ${Pet.ID}) {
		VOUT ${owarning} ${sep} Need a target to give the coin too. Try again.
		/return
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
		/return
	}

	/while (${_countloop} < ${_ammount}) {
		/if (!${Window[InventoryWindow].Open}) /invoke ${Window[InventoryWindow].DoOpen}
		/if (!${Target.ID} || ${Taret.ID} == (${Me.ID} || ${Pet.ID})) /return
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
	}

	VOUT ${onotice} ${sep} Done giving cash for bribes.
/return



|***
 *	DES: keeps SK/PAL harmonious up.. always..
 *	USE: /call check_harmonious
 *	INI: [spells] - harmonious
 *  NOTE: 103 spell EoK
 ***|
sub check_harmonious()
	DEBUG check_harmonious()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (!${switchHarmonious}) /return
	/if (!${Bool[${Me.Buff[${Harmonious}]}]} && ${Me.PctMana} > 10) {
		/if (${validate_cast[FALSE, spell, "${Harmonious}", ${Me.ID}]}) /call core_cast2 "${Harmonious}" 0 0 FALSE
	}
/return



|***
 *	DES: prep sub for all calsses to decide if they are going to be in combat and if its time to burn
 *	USE: /call prep_combat
 *	INI: NA
 *  NOTE:
**|
sub prep_combat()
	DEBUG prep_combat()
	/if (${Me.Zoning}) /return FALSE
  CHECKINPUTS
	CLEARCURSOR
	
	
	|**
	
	/if (${switchOverrideCombat}) {
		
		| healers.. mostly
		/if (${coreAuto}) {

			| healer override check
			/if (${switchHealGroup} || ${switchHealXtarget}) {
				/if (!${SafeZone} && !${switchOverrideSafezone}) /call set_base_target_timers ${Target2Kill}
				/if (${validate_target[FALSE, ${Target2Kill}]}) /varset isValidTarget TRUE
				/varset combat COMBAT
				/if (${is_burn_time[]}) /varset setBurnControl TRUE
				/return TRUE
			}
			
		| healers.. mostly.. in manual
		 } else /if (!${coreAuto}) {
		 	/varset combat FALSE
		 	/return FALSE		
		}
			
		
		
		
		
	} else /if (!${switchOverrideCombat}) {
		

		/if (!${coreAuto}) {
			/if (${Target.ID}) {
				/if (${Target.ID} == ${Me.ID}) /return FALSE
				| /if (!${Spawn[${Target2Kill}].ID}) /varset Target2Kill ${Target.ID}
				| /if (!${SafeZone} && !${switchOverrideSafezone}) /call set_base_target_timers ${Target2Kill}
				| /varset combat COMBAT
				/if (${Bool[${combat}]}) {
					/if (${is_burn_time[]}) /varset setBurnControl TRUE
				}
				/return TRUE
			| } else /if (!${Target.ID} || !${Spawn[${Target2Kill}].ID}) {
			} else /if (!${Target.ID}) {
				/if (${Bool[${combat}]} || ${isValidTarget}) /call clear_combat
				| /varset combat FALSE
				/return FALSE
			}
		}


		
		
		
		
	}
	
	**|
	
		
	| manual mode
	/if (!${coreAuto} && !${switchOverrideCombat}) {
		/if (${Target.ID}) {
			/if (${Target.ID} == ${Me.ID}) /return FALSE
			| /if (!${Spawn[${Target2Kill}].ID}) /varset Target2Kill ${Target.ID}
			| /if (!${SafeZone} && !${switchOverrideSafezone}) /call set_base_target_timers ${Target2Kill}
			| /varset combat COMBAT
			/if (${Bool[${combat}]}) {
				/if (${is_burn_time[]}) /varset setBurnControl TRUE
			}
			/return TRUE
		| } else /if (!${Target.ID} || !${Spawn[${Target2Kill}].ID}) {
		} else /if (!${Target.ID}) {
			/if (${Bool[${combat}]} || ${isValidTarget}) /call clear_combat
			| /varset combat FALSE
			/return FALSE
		}
	
	| healers.. mostly
	} else /if (${coreAuto} && ${switchOverrideCombat}) {

		| healer override check
		/if (${switchHealGroup} || ${switchHealXtarget}) {
			/if (!${SafeZone} && !${switchOverrideSafezone}) /call set_base_target_timers ${Target2Kill}
			/if (${validate_target[FALSE, ${Target2Kill}]}) /varset isValidTarget TRUE
			/varset combat COMBAT
			/if (${is_burn_time[]}) /varset setBurnControl TRUE
			/return TRUE
		}
		
	| healers.. mostly.. in manual
	 } else /if (!${coreAuto} && ${switchOverrideCombat}) {
	 	/varset combat FALSE
	 	/return FALSE

	| offtank check for mobs
	} else /if (${coreAuto} && ${switchOffTank}) {
		/call get_target
		/if (${Me.XTarget[1].ID} != ${Target2Kill}) {
			/varset combat COMBAT
			/return TRUE
		}
	
	| automatic mode
	} else /if (${coreAuto}) {
		/if (!${Me.Invis} || (${coreAuto} && ${Select[${Me.Class.ShortName},ROG]} && ${switchSos})) {
			/if (${validate_target[FALSE, ${Target2Kill}]}) /varset isValidTarget TRUE
			/if (!${isValidTarget} || !${Spawn[${Target2Kill}].ID}) {
				/call clear_combat
				/return FALSE

			} else /if (${isValidTarget} && ${Spawn[${Target2Kill}].ID}) {
				/if (!${SafeZone} && !${switchOverrideSafezone}) /call set_base_target_timers ${Target2Kill}
				/varset combat COMBAT
				/if (${is_burn_time[]}) /varset setBurnControl TRUE
				/return TRUE
				
			}
		}
	}
/return FALSE



|***
 *	DES: do we want to exit the class while loop
 *	USE: ${exit_class_loop[]}
 *	INI: 
 *  NOTE: assume no exit (FALSE)
 ***|
sub exit_class_loop()
	DEBUG exit_class_loop()

	/if (${coreAuto}) {	
		/if (${switchOverrideCombat}) /return TRUE
		/if (!${Spawn[${Target2Kill}].ID}) /return TRUE
		/if (${is_target_dead[${Target2Kill}]}) /return TRUE
	} else /if (!${coreAuto}) {
		/if (!${Target.ID}) /return TRUE
		| /if (${Target.ID} && !${Bool[${combat}]}) /return TRUE
		/if (${Bool[${combat}]}) {
			/if (${is_target_dead[${Target2Kill}]}) /return TRUE
		}
	}
/return FALSE



 |***
 *	DES: Check if we need to engage burn routines. /combat heal/dual are treated differently
 *	USE: auto
 *	INI: 
 *  NOTE: we work off establised assist target for non healers only.
 ***|
sub is_burn_time()
	DEBUG is_burn_time()
	/if (${SafeZone} && !${switchOverrideSafezone} && ${coreAuto}) /return FALSE
	
	| if always set to burn, always burn while in combat
	/if (${burnForce} && ${Me.Combat}) /return TRUE
	
	| remainder requires automated modes
	/if (!${coreAuto}) /return FALSE
	
	| if the current target is a namer
	/if (${burnAuto} && ${Target2Kill} && ${Spawn[${Target2Kill}].PctHPs} <= ${burnEngageat} && (${NameList.Find[${Target.DisplayName}]} && !${switchOverrideName})) /return TRUE
	

	| if number of mobs in the local area reaches the /burn count ## threashold
	/if (${burnCount}) {
		/if (${SpawnCount[npc radius ${MobAgro} targetable playerstate 4]} >= ${burnCount}) /return TRUE
	}
	
	| healers mezers, and off combat routines simply check for a named in the local area to start a burn. no direct target of a named is required. just be in the area.
	/if (${validate_class[FALSE, |CLR|DRU|SHM|ENC]}) /return FALSE
	/declare _count					int 		local 0
	/declare _mobID 				int 		local 0
	
	/if (${Bool[${combat}]}) {
		/for _count 1 to ${SpawnCount[npc radius ${MobAgro} playerstate 4]}
			/varset _mobID ${NearestSpawn[${_count}, npc radius ${MobAgro}].ID}
			/if (${burnAuto} && ${NameList.Find[${Spawn[id ${_mobID} radius ${MobAgro}].DisplayName}]} && !${switchOverrideName}) /return TRUE
			/if (${burnCount}) {
				/if (${SpawnCount[npc radius ${MobAgro} ${If[${switchOverrideLos},,los]} targetable playerstate 4]} >= ${burnCount}) /return TRUE
			}
		/next _count 	
	}
/return FALSE



|***
 *	DES: Designed to check for the one-off crap.
 *	USE: /call check1offBuffs
 *	INI: 
 *  NOTE: THIS NEEDS TO BE WORKED OUT OF THE CODE.. MOVE TO check_exit...
 ***|
sub check1offBuffs()
	DEBUG check1offBuffs()
	AMIDEAD
	/if (${Me.Buff[Holistic Health].ID}) /call cure_Anashti_DT
	| /if (${Me.Buff[Pyroignition].ID}) /call CurePyroignitionDot
	/if (${Me.Buff[Withering Physicality].ID} || ${Me.Buff[Withering Limbs].ID} || ${Me.Buff[Withering Faith].ID}) /call CureDamselofDecay
/return



|***
 *	DES: Summons Food
 *	USE: /call click_summon_food 
 *	INI: toon INI [Meal]->
 *  NOTE: 
 ***|
sub click_summon_food()
	DEBUG click_summon_food()
	/call create_timer timer_click_food ${tClick_FoodDrink}
	AMIDEAD
	CHECKTIE
	CHECKEXIT
	CHECKINPUTS
	
	/if (${FindItemCount[=${clickFoodItem}]} < ${clickFoodCount}) {
		/if (${validate_cast[FALSE, item, "${clickFoodDispenser}", ${Me.ID}]}) /call core_cast2 "${clickFoodDispenser}" item ${Me.ID} FALSE
		CLEARCURSOR TRUE
	}
/return



|***
 *	DES: Summons Drink
 *	USE: /call click_summon_drink
 *	INI: toon INI [Meal]->
 *  NOTE: 
 ***|
sub click_summon_drink()
	DEBUG click_summon_drink()
	CHECKTIE
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/call create_timer timer_click_drink ${tClick_FoodDrink}
	
	/if (${FindItemCount[=${clickDrinkItem}]} < ${clickDrinkCount}) {
		/if (${validate_cast[FALSE, item, "${clickDrinkDispenser}", ${Me.ID}]}) /call core_cast2 "${clickDrinkDispenser}" item ${Me.ID} FALSE
		CLEARCURSOR TRUE
	}
/return



|***
 *	DES: check all buff routines
 *	USE: /call check_buff_routines [TRUE|FALSE] TRUE forces the check even when in manual
 *	INI: 
 *  NOTE: 
 ***|
sub check_buff_routines(bool _force)
	DEBUG check_buff_routines(${_force})
	/if (!${coreAuto} && !${_force}) /return
	AMIDEAD
	CHECKINPUTS
	/if (!${Me.Invis} || (${Select[${Me.Class.ShortName},ROG]} && ${switchSos})) {
		
		/if (${switchBuffGemHold}) {
			/declare _tmpSpell string local FALSE
			/if (${Bool[${Me.Gem[${fix_gem_id[${buffGem}]}]}]})  {
				/varset _tmpSpell ${Me.Gem[${fix_gem_id[${buffGem}]}]}
			}		
		}
		
		| check tell request for buffs
	  /if (${switchBuffTell} && !${timer_check_buffs_tell}) /call check_buffs_tell
		
		| class specific buff checks
		/if (${Select[${Me.Class.ShortName},BER]}) {
		} else /if (${Select[${Me.Class.ShortName},BRD]}) {
		} else /if (${Select[${Me.Class.ShortName},BST]}) {
		} else /if (${Select[${Me.Class.ShortName},CLR]}) {
		} else /if (${Select[${Me.Class.ShortName},DRU]}) {
		} else /if (${Select[${Me.Class.ShortName},ENC]}) {
			/if (!${timer_check_runes}) /call check_rune_${Me.Class.ShortName}
 			/if (${switchBuffItem}) /call check_crystals
		} else /if (${Select[${Me.Class.ShortName},MAG]}) {
			/call Bind_setMAG rods
		} else /if (${Select[${Me.Class.ShortName},MNK]}) {
		} else /if (${Select[${Me.Class.ShortName},NEC]}) {
		} else /if (${Select[${Me.Class.ShortName},PAL]}) {
			/if (${switchHarmonious}) /call check_harmonious
			/if (${switchSteel} && !${timer_check_Steel}) /call check_steel	
			/if (${switchStance} && (!${timer_check_Stance} || !${Bool[${Me.Buff[${Stance}]}]})) /call check_stance
		} else /if (${Select[${Me.Class.ShortName},RNG]}) {
		} else /if (${Select[${Me.Class.ShortName},ROG]}) {
		} else /if (${Select[${Me.Class.ShortName},SHD]}) {
			/if (${switchHarmonious}) /call check_harmonious
			/if (${switchSkin} && !${timer_check_Skin}) /call check_skin
			/if (${switchStance} && (!${timer_check_Stance} || !${Bool[${Me.Buff[${Stance}]}]})) /call check_stance
		} else /if (${Select[${Me.Class.ShortName},SHM]}) {
		} else /if (${Select[${Me.Class.ShortName},WAR]}) {
		} else /if (${Select[${Me.Class.ShortName},WIZ]}) {
			/if (!${timer_check_runes}) /call check_rune_${Me.Class.ShortName}
		}

		| check main routines
		/if (!${check_engaged[]})  {
			/if (${switchBuffItem} && !${timer_check_buffs_item}) /call check_buffs_item
			/if (${switchBuffSelf} && !${timer_check_buffs_self}) /call check_buffs_self
		  /if (${switchBuffRaid} && !${timer_check_buffs_raid}) /call check_buffs_manual raid
			/if (${switchBuffBc}  && !${timer_check_buffs_BC}) /call check_buffs_BC
			/if (${switchBuffPet} && ${Me.Pet.ID} && !${timer_check_buffs_pet}) /call check_buffs_pet
			/if (${switchBuffSelf} && (${Bool[${Aura1Spell}]} || ${Bool[${Aura2Spell}]}) && !${timer_check_aura}) /call check_aura
			/if (${switchBuffMerc} && !${timer_check_buffs_merc}) /call check_buffs_manual mercenary
			/if (${switchBuffGroup} && !${timer_check_buffs_group}) /call check_buffs_manual group
		}

		/if (${switchBuffGemHold}) {
			/if (${Bool[${_tmpSpell}]}) {
				/call mem_spell TRUE "${_tmpSpell}" ${buffGem} FALSE	
				/varset _tmpSpell FALSE
			}
		}
		
	}
/return



|***
 *	DES: finds an item
 *	USE: /call check_for_item "ITEM NAME"  .. use quotes
 *	INI: 
 *  NOTE: 
 ***|
sub check_for_item(string _verbage)
	DEBUG check_for_item(${_verbage})
	/declare _slot 			int local
	/declare _augslot 	int local
	/declare _bagslot 	int local
	/declare _found			bool FALSE

	VOUT Searching ${sep} ${cinfo}${_verbage}\ax

| check bank for shits
	/for _slot 1 to 28
	| -- search top bank containers
		/if (${Me.Bank[${_slot}].Container}) {
			/for _bagslot 1 to ${Me.Bank[${_slot}].Container}
	| -- ITEMS: search inside bank containers top slots
				/if (${Me.Bank[${_slot}].Item[${_bagslot}].Name.Equal[${_verbage}]}) {
					/varset _found TRUE
					VOUT Bank Bag:${cinfo}${_slot}\ax ${sep} Bag Slot:${cinfo}${_bagslot}\ax ${If[${Me.Bank[${_slot}].Item[${_bagslot}].Stack} > 1,${sep} Stack:${cgood}${Me.Bank[${_slot}].Item[${_bagslot}].Stack}\ax,]}	
				}
	| -- AUG: search aug slots in items in bags Augments in items inside bags
				/if (${Me.Bank[${_slot}].Item[${_bagslot}].Augs}) {
					/for _augslot 0 to ${Me.Bank[${_slot}].Item[${_bagslot}].Augs}
						/if (${Me.Bank[${_slot}].Item[${_bagslot}].AugSlot[${_augslot}].Name.Equal[${_verbage}]}) {
							/varset _found TRUE
							VOUT Bank Bag:${cinfo}${_slot}\ax ${sep} Slot:${cinfo}${_bagslot}\ax ${sep} Aug:${cinfo}${Int[${Math.Calc[${_augslot}+1]}]}\ax
						}
					/next _augslot	
				}
			/next _bagslot
	| -- AUG/ITEM: search top bank slots
		} else /if (!${Me.Bank[${_slot}].Container}) {
			/if (${Me.Bank[${_slot}].Name.Equal[${_verbage}]}) {
				/varset _found TRUE
				VOUT Bank Slot:${cinfo}${_slot}\ax ${If[${Me.Bank[${_slot}].Stack} > 1,${sep} Stack:${cgood}${Me.Bank[${_slot}].Stack}\ax,]}	
			}		
	| -- AUG: search top bank slot items with aug slots
			/if (${Me.Bank[${_slot}].Augs}) {
				/for _augslot 0 to ${Me.Bank[${_slot}].Augs}
					/if (${Me.Bank[${_slot}].AugSlot[${_augslot}].Name.Equal[${_verbage}]}) {
						/varset _found TRUE
						VOUT Bank Slot:${cinfo}${_slot}\ax ${sep} Aug:${cinfo}${Int[${Math.Calc[${_augslot}+1]}]}\ax
					}
				/next _augslot
			}				
		}
	/next _slot	
	
| Augment search worn items (skip powersource)
	/for _slot 0 to 22
		/if (${_slot} == 21) /next _slot
		/for _augslot 0 to ${Me.Inventory[${_slot}].Augs}
			/if (${Me.Inventory[${_slot}].AugSlot[${_augslot}].Name.Equal[${_verbage}]}) {
				/varset _found TRUE
				VOUT ${cinfo}${Me.Inventory[${_slot}].Name}\ax ${sep} Aug:${cinfo}${Int[${Math.Calc[${_augslot}+1]}]}\ax
			}
		/next _augslot
	/next _slot	
		
| Check Inventory
	| -- search top inventory slots
	/for _slot 23 to 32
	| -- search inside bags top slots
		/if (${InvSlot[${_slot}].Item.Container}) {
			/for _bagslot 1 to ${InvSlot[${_slot}].Item.Container}
				/if (${Me.Inventory[${_slot}].Item[${_bagslot}].Name.Equal[${_verbage}]}) {
					/varset _found TRUE
					VOUT Inventory Bag:${cinfo}${Int[${Math.Calc[${_slot}-22]}]}\ax ${sep} Slot:${cinfo}${_bagslot}\ax ${If[${Me.Inventory[${_slot}].Item[${_bagslot}].Stack} > 1,${sep} Stack:${cgood}${Me.Inventory[${_slot}].Item[${_bagslot}].Stack}\ax,]}	
				}
	| -- search aug slots in items in bags
				/if (${Me.Inventory[${_slot}].Item[${_bagslot}].Augs}) {
					/for _augslot 0 to ${Me.Inventory[${_slot}].Item[${_bagslot}].Augs}
						/if (${Me.Inventory[${_slot}].Item[${_bagslot}].AugSlot[${_augslot}].Name.Equal[${_verbage}]}) {
							/varset _found TRUE
							VOUT Inventory Bag:${cinfo}${Int[${Math.Calc[${_slot}-22]}]}\ax ${sep} Slot:${cinfo}${_bagslot}\ax ${sep} Aug:${cinfo}${Int[${Math.Calc[${_augslot}+1]}]}\ax
						}
					/next _augslot	
				}
			/next _bagslot
		} else /if (!${InvSlot[${_slot}].Item.Container}) {
	| -- search top inventory slots
			/if (${Me.Inventory[${_slot}].Name.Equal[${_verbage}]}) {
				/varset _found TRUE
				VOUT Inventory Slot:${cinfo}${Int[${Math.Calc[${_slot}-22]}]}\ax ${If[${Me.Inventory[${_slot}].Stack} > 1,${sep} Stack:${cgood}${Me.Inventory[${_slot}].Stack}\ax,]}	
			}
	| -- search top slot items with aug slots
			/if (${Me.Inventory[${_slot}].Augs}) {
				/for _augslot 0 to ${Me.Inventory[${_slot}].Augs}
					/if (${Me.Inventory[${_slot}].AugSlot[${_augslot}].Name.Equal[${_verbage}]}) {
						/varset _found TRUE
						VOUT Inventory Slot:${cinfo}${Int[${Math.Calc[${_slot}-22]}]}\ax ${sep} Aug:${cinfo}${Int[${Math.Calc[${_augslot}+1]}]}\ax
					}
				/next _augslot
			}
		}
	/next _slot
	
	/if (!${_found}) VOUT ${cbad}Not Found\ax ${sep} ${cinfo}${_verbage}\ax
/return



|***
 *	DES: one line echo bitch
 *	USE: /call echo_bitch [MQWindow|ECHO]
 *	INI: 
 *  NOTE:
 ***| 	
sub echo_bitch(string _out)
	DEBUG echo_bitch(${_out})
	/declare _bitch					string local 
	/declare _rnd						int local
	
	:loopgetrandom
	/varset _rnd ${Math.Rand[${bitchcount}]}
	/if (!${Range.Between[1,${bitchcount}:${_rnd}]}) /goto :loopgetrandom
	/varset _bitch ${bitch${_rnd}}

	/if (${_out.Find[MQWindow]}) {
		VOUT \a-g${_bitch}\ax
	} else /if (${_out.Find[ECHO]}) {
		ECHOCHANNEL ${_bitch}
	} 
/return



|***
 *	DES: cast classes AA fade
 *	USE: /call cast_AA_fade
 *	INI: 
 *  NOTE:
 ***|
sub cast_AA_fade()
	DEBUG cast_AA_fade()
	CHECKEXIT
	
	| does the class even have a fade?
	/if (!${Bool[${${Me.Class.ShortName}AAFade}]}) /return

	| made it this far. may as well see if the shit works
	/if (${Me.AltAbilityReady[${${Me.Class.ShortName}AAFade}]}) {
		ECHOCHANNEL Fading ${Me.PctHPs} <= ${useAAFadePct} casting .. ${${Me.Class.ShortName}AAFade}
		/if (${validate_cast[FALSE, alt, "${${Me.Class.ShortName}AAFade}", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}AAFade}" alt 0 FALSE
	}
/return



|***
 *	DES: keeps Paladin/Shadowknight Stance running
 *	USE: /call check_Stance
 *	INI: [buffs] - Stance
 *  NOTE: 
 ***|
sub check_stance()
	DEBUG check_stance()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/call create_timer timer_check_Stance ${tcheck_Stance}
	/if (${validate_cast[FALSE, spell, "${Stance}", ${Me.ID}]}) /call core_cast2 "${Stance}" 0 0 FALSE
/return



 |***
 *	DES: Circle of [Power|Life|Mana]
 *	USE: keep Circle of [Power|Life|Mana] going
 *	INI: TOON - [General] -> Circleof[Power|Life|Mana]=
 *	NOTE: these are all on the same timer. so u just get one
 ***|  
sub synergy_click_Circle()
	DEBUG synergy_click_Circle()
	/if (!${Bool[${useCircle}]}) /return FALSE
	AMIDEAD
	CHECKEXIT	
	CHECKINPUTS
	
	| circle of power
	/if (${useCircle.Equal[Power]}) {
		/if (${Bool[${Me.Song[Circle of Power]}]}) /return
		/if (${Me.ItemReady[${CircleofPower}]} && ${Spell[${FindItem[${CircleofPower}].Spell}].Stacks}) {
			/if (${validate_cast[FALSE, item, "${CircleofPower}", ${Me.ID}]}) /call core_cast2 "${CircleofPower}" item 0 FALSE
		}

	| circle of life
	} else /if (${useCircle.Equal[Life]}) {
		/if (${Bool[${Me.Song[Circle of Life]}]}) /return
		/if (${Me.ItemReady[${CircleofLife}]} && ${Spell[${FindItem[${CircleofLife}].Spell}].Stacks}) {
			/if (${validate_cast[FALSE, item, "${CircleofLife}", ${Me.ID}]}) /call core_cast2 "${CircleofLife}" item 0 FALSE
		}		

	| circle of Mana
	} else /if (${useCircle.Equal[Mana]}) {
		/if (${Bool[${Me.Song[Circle of Mana]}]}) /return
		/if (${Me.ItemReady[${CircleofMana}]} && ${Spell[${FindItem[${CircleofMana}].Spell}].Stacks}) {
			/if (${validate_cast[FALSE, item, "${CircleofMana}", ${Me.ID}]}) /call core_cast2 "${CircleofMana}" item 0 FALSE
		}
	}
/return TRUE



|***
 *	DES: Clicks usefull 1 off items during a Burn
 *	USE: 
 *	INI: set [doclickItemBurn] to 0 to disable
 *  NOTE: 
 ***|
sub click_item_burn()
	DEBUG click_item_burn()
	/declare _count int local 0
	/for _count 1 to 6
		/if (!${Bool[${clickItemBurn${_count}}]}) /continue
		/if (${is_target_dead[${Target2Kill}]}) /break
		AMIDEAD
		CHECKINPUTS
		/if (${validate_cast[FALSE, item, "${clickItemBurn${_count}}", ${Me.ID}]}) /call core_cast2 "${clickItemBurn${_count}}" item 0 FALSE
	/next _count
/return



|***
 *	DES: cast whatever spire you set for burns
 *	USE: /call cast_AA_spire
 *	INI: classINI - [Buffs] - spireGroup & spireRaid
 *  NOTE: 
 ***|
sub cast_AA_spire()
	DEBUG cast_AA_spire()
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${Raid.Members} && ${Bool[${${Me.Class.ShortName}spire${spireRaid}}]}) {
		/if (!${Bool[${Me.Buff[${${Me.Class.ShortName}spire${spireRaid}}]}]} && ${Spell[${${Me.Class.ShortName}spire${spireRaid}}].Stacks}) {
			/if (${validate_cast[FALSE, alt, "${${Me.Class.ShortName}spire${spireRaid}}", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}spire${spireRaid}}" alt 0 FALSE
		}
	} else /if (!${Raid.Members} && ${Bool[${${Me.Class.ShortName}spire${spireGroup}}]}) {
		/if (!${Bool[${Me.Buff[${${Me.Class.ShortName}spire${spireGroup}}]}]} && ${Spell[${${Me.Class.ShortName}spire${spireGroup}}].Stacks}) {
			/if (${validate_cast[FALSE, alt, "${${Me.Class.ShortName}spire${spireGroup}}", ${Me.ID}]}) /call core_cast2 "${${Me.Class.ShortName}spire${spireGroup}}" alt 0 FALSE
		}
	}
/return
	


|***
 *	DES: memorize spell lines
 *	USE: /call mem_spell_line []
 *	INI: [Spells] -> line[]
 *  NOTE:
 ***|
sub mem_spell_line(string _line)
	DEBUG mem_spell_line(${_line})
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/declare _count 				int local
	/declare _index 				int local	
	/declare _list[1] 			string ${line${lineCurrent}}
	/declare _currentSpell 	string NULL
	
	/for _count 1 to ${_list.Size}
		/for _index 1 to ${_list[${_count}].Count[|]}
			/varset _currentSpell ${Spell[${_list[${_count}].Arg[${_index},|]}].RankName}
			/if (${Me.Gem[${_index}].Name.Equal[${_currentSong}]}) /continue
			/call mem_spell TRUE "${_currentSpell}" "${_index}" FALSE
		/next _index	
	/next _count	
	VOUT Line ${sep} ${cinfo}${lineCurrent}\ax memorization complete.
/return



|***
 *	DES: passive toggle AA Agro switches
 *	USE: 
 *	INI: 
 *  NOTE:
 ***|
sub check_AA_agro()
	DEBUG check_AA_agro()
	AMIDEAD
	CHECKINPUTS
	
	| turn agro ON
	/if (${switchAgro}) {

		| adjust bold attacks AA
		/if (${Me.AltAbilityReady[Bold Attacks: Disabled]}) {
			/call MQ2Cast "Bold Attacks: Disabled" alt
			VALT AA ${sep} ${cinfo}Bold Attacks\ax ${sep} ${oenabled}
		}

		| SHD Soul Flay
		/if (${Select[${Me.Class.ShortName},SHD]}) {
			/if (${Me.AltAbilityReady[Soul Flay: Disabled]}) {
				/call MQ2Cast "Soul Flay: Disabled" alt
				VALT AA ${sep} ${cinfo}Soul Flay\ax ${sep} ${oenabled}
			}			
		}
		
		| ranger ferocious kicks
		/if (${Select[${Me.Class.ShortName},RNG]}) {
			/if (${Me.AltAbilityReady[Ferocious Kick Subtlety: Enabled]}) {
				/call MQ2Cast "Ferocious Kick Subtlety: Enabled" alt
				VALT AA ${sep} ${cinfo}Ferocious Kick Subtlety\ax ${sep} ${odisabled}
			}
		}

	| turn agro OFF
	} else /if (!${switchAgro}) {

		| adjust bold attacks AA
		/if (${Me.AltAbilityReady[Bold Attacks: Enabled]}) {
			/call MQ2Cast "Bold Attacks: Enabled" alt
			VALT AA ${sep} ${cinfo}Bold Attacks\ax ${sep} ${odisabled}
		}

		| SHD Soul Flay
		/if (${Select[${Me.Class.ShortName},SHD]}) {
			/if (${Me.AltAbilityReady[Soul Flay: Enabled]}) {
				/call MQ2Cast "Soul Flay: Enabled" alt
				VALT AA ${sep} ${cinfo}Soul Flay\ax ${sep} ${odisabled}
			}			
		}

		| ranger ferocious kicks
		/if (${Select[${Me.Class.ShortName},RNG]}) {
			/if (${Me.AltAbilityReady[Ferocious Kick Subtlety: Disabled]}) {
				/call MQ2Cast "Ferocious Kick Subtlety: Disabled" alt
				VALT AA ${sep} ${cinfo}Ferocious Kick Subtlety\ax ${sep} ${oenabled}
			}
		}

	}
/return



|***
 *	DES: Challenge Spells for paladin/shadowknight
 *	USE: /call cast_challenge TARGET
 *	INI: 
 *  NOTE:
 ***|
sub cast_challenge(int _tmpID)
	DEBUG cast_challenge(${_tmpID})
	/if (${Target.Buff[${ChallengeSpell}].ID}) /return
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${validate_cast[FALSE, spell, "${ChallengeSpell}", ${_tmpID}]}) /call core_cast2 "${ChallengeSpell}" 0 ${_tmpID} cast_break
/return



|***
 *	DES: validates casting of "things on a target
  *	USE: /call ${validate_cast[[TRUE|FALSE], [spell|alt|disc|item|pulse|FIND], name, targetID]
 *	INI: 
 *  NOTE: will sort silver subscriptions as well.
 *  This is designed tomake sure whatever we are attempting to cast, is castable. be it a disc, item, spell, booger flick.. whatever.. IS IT READY TO BE USED?
 ***|
sub validate_cast(bool _error, string _type, string _spell, int _tmpID)
	DEBUG validate_cast(${_error}, ${_type}, ${_spell}, ${_tmpID})
	
	/if (${_error}) SDEBUG type:${_type}, name:${_spell}, ID:${_tmpID} 
	
	| _spell set to false
	/if (!${Bool[${_spell}]}) /return FALSE
	
	| is the issued target dead?
	/if (${_tmpID}) {
		/if (!${Spawn[${_tmpID}].ID}) /return FALSE
	}

	| exit in manual when force stop issued to release the target..
	/if (!${coreAuto}) {
		/if (${_tmpID} != ${Me.ID} && (!${Target.ID} || ${Target.Dead})) {
			/if (!${Spell[${_spell}].SpellType.Equal[Beneficial]}) /return FALSE
		}
	}

	CHECKINPUTS
	
	/if (${Me.Zoning}) /return FALSE
	
	| what type is it?
	/if (${_type.Equal[FIND]}) {
		/if (${Me.Book[${_spell}]}) {
			/varset _type spell
		} else /if (${Me.AltAbilityReady[${_spell}]}) {	
			/varset _type alt
		} else /if (${Me.CombatAbilityReady[${_spell}]}) {
			/varset _type disc
		} else /if (${Me.ItemReady[${_spell}]}) {
			/varset _type item
		}
	}
			
	| check spell
	/if (${_type.Equal[spell]}) {
		/if (!${Bool[${Me.Book[${_spell}]}]}) /return FALSE
		/if (!${Me.SpellReady[${_spell}]} && ${Spell[${_spell}].SpellType.Equal[Utility Beneficial]}) /return FALSE
		/if (!${Me.SpellReady[${_spell}]} && !${Spell[${_spell}].SpellType.Equal[Beneficial]} && !${Spell[${_spell}].Category.Equal[Heals]}) /return FALSE
		/if (${Stick.Active} && !${setModeType.Equal[hunt]} && !${setModeType.Equal[scorch]}) /return FALSE
		/if (${Me.Moving}) /return FALSE
		/if (${Bool[${Plugin[MQ2ADVPath]}]}) {
			/if (${AdvPath.Following}) /return FALSE
		}
		/if (${Me.Casting.ID}) /return FALSE
		/if (!${Spell[${_spell}].Category.Equal[Heals]}) {
			/if (!${Me.SpellReady[${_spell}]} && !${Spell[${_spell}].SpellType.Equal[Beneficial]}) /return FALSE
			/if (!${switchOverrideLos} && !${Spawn[${_tmpID}].LineOfSight} && !${Spell[${_spell}].SpellType.Equal[Beneficial]}) /return FALSE
		}
		/if (${Spell[${_spell}].Range}) {
			/if (${Spawn[${_tmpID}].Distance} > ${Spell[${_spell}].MyRange}) /return FALSE
		}
		/if (${Spell[${_spell}].TargetType.Equal[Group V2]}) {
			/if (${Spawn[${_tmpID}].Distance} > ${Spell[${_spell}].AERange}) /return FALSE
		}
	
		/if (${Me.CurrentMana} < ${Spell[${_spell}].Mana}) /return FALSE

		| restist types vs core.zone.inc
		/if (!${Spell[${_spell}].SpellType.Equal[Beneficial]}) {
			/if (${NoDiseaseList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Disease]}) /return FALSE
			/if (${NoMagicList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Magic]}) /return FALSE
			/if (${NoPoisonList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Poison]}) /return FALSE
			/if (${NoFireList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Fire]}) /return FALSE
			/if (${NoColdList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Cold]}) /return FALSE
			/if (${NoCorruptionList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Corruption]}) /return FALSE
		}
		
		| check for stacking issues
		/if (${_tmpID} == ${Me.ID}) {
			/if (${Spell[${_spell}].SpellType.Equal[Beneficial]} && !${Spell[${_spell}].Category.Equal[Heals]}) {
				/if (!${Spell[${_spell}].Stacks}) /return FALSE
				/if (${Me.Buff[${_spell}].ID}) /return FALSE
				/if (${Me.Song[${_spell}].ID}) /return FALSE		
			}
		}
		
	| check alt ability
	} else /if (${_type.Equal[alt]}) {
		/if (!${Me.AltAbilityReady[${_spell}]}) /return FALSE
		/if (${Me.CurrentMana} < ${Spell[${_spell}].Mana}) /return FALSE
		/if (${Spell[${_spell}].Range}) {
			/if (${Spawn[${_tmpID}].Distance} > ${Spell[${_spell}].MyRange}) /return FALSE
		}		
		| stacking
		/if (${Spell[${_spell}].SpellType.Equal[Beneficial]} && (${_tmpID} == ${Me.ID})) {
			/if (!${Spell[${_spell}].Stacks}) /return FALSE
			/if (${Me.Buff[${_spell}].ID}) /return FALSE
			/if (${Me.Song[${_spell}].ID}) /return FALSE		
		}
	
	| check disc
	} else /if (${_type.Equal[disc]}) {
		/if (!${Bool[${_spell}]}) /return FALSE
		/if (!${Me.CombatAbilityReady[${_spell}]}) /return FALSE
		/if (${Me.CurrentEndurance} < ${Spell[${_spell}].EnduranceCost}) /return FALSE
		/if (${Spell[${_spell}].Range}) {
			/if (${Spawn[${_tmpID}].Distance} > ${Spell[${_spell}].MyRange}) /return FALSE
		}
		/if (${Bool[${Me.Song[${_spell}].Duration}]}) /return FALSE
		/if (${Bool[${Me.Buff[${_spell}].Duration}]}) /return FALSE		
					
		| stacking
		/if (${Spell[${_spell}].SpellType.Equal[Beneficial]} && (${_tmpID} == ${Me.ID})) {
			/if (!${Spell[${_spell}].Stacks}) /return FALSE
		}

	| check disc that requires running disc slot
	} else /if (${_type.Equal[activedisc]}) {
		/if (${_error}) OUT ${cbug}validate_cast(error:${_error}, type:${_type}, name:${_spell}, RunningDisc:${Bool[${Me.ActiveDisc}]})\ax
		/if (!${Bool[${_spell}]}) /return FALSE
		/if (${Bool[${Me.ActiveDisc}]}) /return FALSE
		/if (!${Me.CombatAbilityReady[${_spell}]}) /return FALSE
		/if (${Me.CurrentEndurance} < ${Spell[${_spell}].EnduranceCost}) /return FALSE

	| check item
	} else /if (${_type.Equal[item]}) {
		/if (${_error}) OUT ${cbug}validate_cast(error:${_error}, type:${_type}, name:"${_spell}", ID:${_tmpID})\ax
		/if (!${Me.ItemReady[=${_spell}]}) /return FALSE
		/if (${_tmpID} != ${Me.ID}) {
			/if (${Spawn[${_tmpID}].Distance} > ${Spell[${FindItem[=${_spell}].Spell}].MyRange} && ${Spell[${FindItem[=${_spell}].Spell}].Range} != 0) /return FALSE
		}

		/if (${_tmpID} == ${Me.ID}) {
			/if (${Me.Buff[${FindItem[=${_spell}].Spell}].ID}) /return FALSE
			/if (${Me.Song[${FindItem[=${_spell}].Spell}].ID}) /return FALSE
		}
					
		| stacking
		/if (${Spell[${FindItem[=${_spell}].Spell}].SpellType.Equal[Beneficial]} && (${_tmpID} == ${Me.ID})) {
			/if (!${Spell[${FindItem[=${_spell}].Spell}].Stacks}) /return FALSE
		}
		
	| epic clicks
	} else /if (${_type.Equal[epicitem]}) {
		/if (${_error}) OUT ${cbug}validate_cast(error:${_error}, type:${_type}, name:${${Me.Class.ShortName}Epic${_spell}}, ID:${_tmpID})\ax
		/if (!${Me.ItemReady[=${${Me.Class.ShortName}Epic${_spell}}]}) /return FALSE
		/if (${_tmpID} != ${Me.ID}) {
			/if (${Spawn[${_tmpID}].Distance} > ${Spell[${FindItem[=${${Me.Class.ShortName}Epic${_spell}}].Spell}].MyRange} && ${Spell[${FindItem[=${${Me.Class.ShortName}Epic${_spell}}].Spell}].Range} != 0) /return FALSE
		}
		/if (${Me.Buff[${FindItem[=${${Me.Class.ShortName}Epic${_spell}}].Spell}].ID}) /return FALSE
		/if (${Me.Song[${FindItem[=${${Me.Class.ShortName}Epic${_spell}}].Spell}].ID}) /return FALSE
		
		/if (${Spell[${FindItem[=${${Me.Class.ShortName}Epic${_spell}}].Spell}].SpellType.Equal[Beneficial]} && (${Target.ID} == ${Me.ID})) {
			/if (!${Spell[${FindItem[=${${Me.Class.ShortName}Epic${_spell}}].Spell}].Stacks}) /return FALSE
		}

	| check bard shit.. gotta be difficult..
	} else /if (${_type.Equal[pulse]}) {

	}
/return TRUE



|***
 *	DES: spell/disc/aa/item cast wrapper
 *	USE: /call core_cast2 "SPELLNAME" [alt|activedisc|disc|gem#|item|epicitem|pulse] [TargetID|0 (for castless skills)] [subname|FALSE]
 *	INI: 
 *  NOTE: MQ2Cast ["spell name"|"item name"|"AA name"|"AA#"] [[item|slotname]|alt|gem#] [give up time][s|m] [custom subroutine name] [-targetid|###] [-maxtries|#] [-recast|#] [-setin|setname] [-bandolier|setname]
 ***|
sub core_cast2(string _spell, string _type, int _tmpID, string _sub)
	AMIDEAD
	CHECKINPUTS
	DEBUG core_cast2(${_spell}, ${_type}, ${_tmpID}, ${_sub})
	
	| make sure we have our target 
	/if (${Bool[${_tmpID}]}) {
			/if (${Target.ID} != ${_tmpID}) {
			/target ID ${_tmpID}
			/delay 1s ${Target.ID} == ${_tmpID}
		}
	}
	
	| what type is it?
	/if (${_type.Equal[FIND]}) {
		/if (${Me.Book[${_spell}]}) {
			/varset _type ${buffGem}
		} else /if (${Me.AltAbilityReady[${_spell}]}) {	
			/varset _type alt
		} else /if (${Me.CombatAbilityReady[${_spell}]}) {
			/varset _type disc
		} else /if (${Me.ItemReady[${_spell}]}) {
			/varset _type item
		}
	} 	

	| spell
	/if (!${Select[${_type},alt,item,disc,pulse]}) {
		/if (${_tmpID} == 0) {
			VSPELL Spell ${sep} ${csp}${_spell}\ax
			/call MQ2Cast "${_spell}" ${If[${Bool[${_type}]},${_type},gem${Me.Gem[${_spell}]}]} ${If[${Bool[${_sub}]},${_sub},]}
		} else /if (${_tmpID} != 0 && ${Target.ID}) {

			| /if (${Me.Song[Gift of Mana (${GoMLevel})].ID}) /call cast_nuke_GoM ${_tmpID}
			/if (${Me.Song[Gift of Mana].ID}) /call cast_nuke_GoM ${_tmpID}

			VSPELL Spell ${sep} ${csp}${_spell}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
			/call MQ2Cast "${_spell}" ${If[${Bool[${_type}]},${_type},gem${Me.Gem[${_spell}]}]} ${If[${Bool[${_sub}]},${_sub},]} -targetid|${_tmpID}
		}

		| bc output if enabled
		VBC Spell "${_spell}" ${_tmpID} 0 		

	| AA
	} else /if (${Select[${_type},alt]}) {
		/if (${_tmpID} == 0) {
			VALT AA ${sep} ${csp}${_spell}\ax
			/call MQ2Cast "${_spell}" alt 
		} else /if (${_tmpID} != 0 && ${Target.ID}) {
			VALT AA ${sep} ${csp}${_spell}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
			/call MQ2Cast "${_spell}" alt -targetid|${_tmpID}
		}

		| bc output if enabled
		VBC Alt "${_spell}" ${_tmpID} 0 

	| item
	} else /if (${Select[${_type},item,epicitem]}) {
		/if (${_tmpID} == 0) {
			VITEM Item ${sep} ${citem}${_spell}\ax
			/call MQ2Cast "${_spell}" item
		} else /if (${_tmpID} != 0 && ${Target.ID}) {
			VITEM Item ${sep} ${citem}${_spell}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
			/call MQ2Cast "${_spell}" item -targetid|${_tmpID}
		}

	| disc
	} else /if (${Select[${_type},disc,activedisc]}) {
		/if (${_tmpID} == 0) VDISC Disc ${sep} ${csp}${_spell}\ax
		/if (${_tmpID} != 0 && ${Target.ID}) VDISC Disc ${sep} ${csp}${_spell}\ax ${sep} [${ctar}${Spawn[ID ${_tmpID}].DisplayName}\ax]
		VBC Disc "${_spell}" ${_tmpID} 0 
		/disc ${_spell}
		/delay 5s !${Me.Casting.ID}

	| song	
	} else /if (${Select[${_type},pulse]}) {
	}

	| cooldown the system needs the tiony inturrupt to register the cast routine and greout for spells
	AMIDEAD
	CHECKINPUTS
	/if (!${Select[${_type},alt,item,disc,pulse]}) {
		/delay 2 !${Me.SpellReady[${_spell}]}
	} else /if (${Select[${_type},item,epicitem]}) {
		/delay 2 !${Me.ItemReady[${_spell}]}
	} else /if (${Select[${_type},alt]}) {
		/delay 2 !${Me.AltAbilityReady[${_spell}]}
	} else /if (${Select[${_type},disc,activedisc]}) {
		/delay 2 !${Me.CombatAbilityReady[${_spell}]}
	} else /if (${Select[${_type},pulse]}) {
		/delay 2 !${Me.SpellReady[${_spell}]}
	}

/return FALSE



|***
 *	DES: removes ranks from spell names. returns the name only
 *	USE: /call spell_remove_rank [spellname]
 *	INI: 
 *	NOTE:
 ***|
sub spell_remove_rank(string _spell)
	DEBUG spell_remove_rank(${_spell})
	/if (${Spell[${_spell}].Rank} == 3) {
		/varset _spell ${_spell.Left[-8]}	
	} else /if (${Spell[${_spell}].Rank} == 2) 
		/varset _spell ${_spell.Left[-7]}	
	} else /if (${Spell[${_spell}].Rank} == 1) 
		/varset _spell ${_spell}
	} else /if (${Spell[${_spell}].Rank} == 0) 
		/varset _spell ${_spell}
	}
/return ${_spell}



|***
 *	DES: set base settings for first usage
 *	USE: 
 *	INI: 
 *	NOTE: 
 ***|
sub set_base_install()
	DEBUG set_base_install()
	VOUT Setting base environment for first time use.

	| basics
	/call writeINI verbose Common TRUE
	/call writeINI autoRest Common 80

	| set tie stuff
	/call writeINI tCheck_Tie Timer 5
	/call writeINI SetTieDistance Movement 5
	
	| set burns
	/call writeINI burnAuto Common TRUE
	/call writeINI burnCount Common 5
	/call writeINI burnEngageat Common 98
	
	| combat stuff
	/call writeINI MobAgro Common 80
	/call writeINI switchPull Common FALSE
	/call writeINI MobAgro Common FALSE
	/call writeINI engageAt Common 98
	/call writeINI smartengageAt Common98
	/call writeINI CombatDistance Movement 15
	/call writeINI combatRadius Common 30
	/call writeINI zradius Common 30
	
	| movement
	/call writeINI SetStickBehind Movement FALSE
	/call writeINI SetFaceFast Movement TRUE
	/call writeINI SetFaceArc Movement 30	
	
	| buffs
	/call writeINI switchBuffCombat Buffs FALSE
	/call writeINI switchBuffItem Buff TRUE
	/call writeINI switchBuffSelf Buffs TRUE
	

	| class specific settings
	|**
	/if (${Select[${Me.Class.ShortName},BER]}) {
		/call writeINI combat Common MELEE
	} else /if (${Select[${Me.Class.ShortName},BRD]}) {
		/call writeINI combat Common MELEE
	} else /if (${Select[${Me.Class.ShortName},BST]}) {
		/call writeINI combat Common MELEE
	} else /if (${Select[${Me.Class.ShortName},CLR]}) {
		/call writeINI combat Common HEAL
	} else /if (${Select[${Me.Class.ShortName},DRU]}) {
		/call writeINI combat Common DUAL
	} else /if (${Select[${Me.Class.ShortName},ENC]}) {
		/call writeINI combat Common DUAL
	} else /if (${Select[${Me.Class.ShortName},MAG]}) {
		/call writeINI combat Common NUKE
	} else /if (${Select[${Me.Class.ShortName},MNK]}) {
		/call writeINI combat Common MELEE
	} else /if (${Select[${Me.Class.ShortName},NEC]}) {
		/call writeINI combat Common NUKE
	} else /if (${Select[${Me.Class.ShortName},PAL]}) {
		/call writeINI combat Common MELEE
	} else /if (${Select[${Me.Class.ShortName},RNG]}) {
		/call writeINI combat Common MELEE
	} else /if (${Select[${Me.Class.ShortName},ROG]}) {
		/call writeINI combat Common MELEE
		/call writeINI SetStickBehind Movement TRUE
		/override los
	} else /if (${Select[${Me.Class.ShortName},SHD]}) {
		/call writeINI combat Common MELEE
	} else /if (${Select[${Me.Class.ShortName},SHM]}) {
		/call writeINI combat Common HEAL
	} else /if (${Select[${Me.Class.ShortName},WAR]}) {
		/call writeINI combat Common MELEE
	} else /if (${Select[${Me.Class.ShortName},WIZ]}) {
		/call writeINI combat Common NUKE
	}
	**|

/return



|***
 *	DES: check specific classes ONLY
 *	USE: /if (${validate_class[BOOL, |CLS|CLS]) /dowhatever
 *	INI: 
 *	NOTE: 
 
	AllClassList				|WAR|SHD|PAL|BER|ROG|MNK|RNG|BST|BRD|NEC|ENC|WIZ|MAG|CLR|SHM|DRU
	MeleeList 					|WAR|ROG|BST|MNK|SHD|PAL|BER|BRD|RNG
	CastList						|BST|SHD|PAL|RNG|CLR|DRU|SHM|ENC|WIZ|NEC|MAG
	PetList							|ENC|DRU|SHM|SHD|WIZ|BRD|BST|MAG
	HealerList					|CLR|DRU|SHM|PAL
	RezerList						|CLR|DRU|SHM|PAL|NEC
	AASnareList					|RNG|DRU|SHD|NEC|ROG|WAR|WIZ|MNK|PAL|BER
	AAEradicateList			|RNG|DRU|NEC|WIZ|ENC|MAG
 
 ***|
sub validate_class(bool _error, string _class)
	DEBUG validate_class(${_error}, ${_class})
	/if (${switchOverrideClasscheck}) /return TRUE
	/declare _pass bool local FALSE
	/declare _count int local 0

	/for _count 1 to ${_class.Count[|]}
		/if (${Me.Class.ShortName.Upper.Equal[${_class.Arg[${_count},|]}]}) {
			/varset _pass TRUE
			/break
		}
	/next _count

	/if (${_error}) {
		/if (!${_pass}) /call echos failboat 0 0 0 0
	}
/return ${_pass}



|***
 *	DES: is the plugin available ?
 *	USE: /if (${validate_plugin[BOOL, |plugin|plugin]) /dowhatever
 *	INI: 
 *	NOTE: cycle all plugins. return FALSE if any passed are not loaded
 ***|
sub validate_plugin(bool _error, string _plugin)
	DEBUG validate_plugin(${_error}, ${_plugin})
	/if (!${Bool[${_plugin}]}) VOUT ${owarning} ${sep} validate_plugin failure.
	/declare _plugcount int local 0	
	/declare _count int local 0

	/for _count 1 to ${_plugin.Count[|]}
		AMIDEAD
		/if (${Bool[${Plugin[${_plugin.Arg[${_count},|]}]}]}) {
			/varcalc _plugcount ${_plugcount}+1
		} else /if (!${Bool[${Plugin[${_plugin.Arg[${_count},|]}]}]} && ${_error}) {
			 VOUT Plugin ${cinfo}$ ${_plugin.Arg[${_count},|]}\ax is not loaded.
		}
	/next _count
	
	/if (${_plugcount} == ${_plugin.Count[|]}) {
		/return TRUE
	} else /if (${_plugcount} != ${_plugin.Count[|]}) {
		/return FALSE
	}
/return FALSE



|***
 *	DES: slows the target
 *	USE: /call cast_slow [ID] | /debuff slow
 *	INI: 
 *  NOTE: 
 ***|
sub cast_slow(int _tmpID)
	DEBUG cast_slow(${_tmpID})
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${_tmpID}]}) /return
	
	/squelch /target ID ${_tmpID}
	/delay 1s ${Target.ID} == ${_tmpID}

	| necros only slow undead 
	/if (${validate_class[FALSE, |NEC]} && ${Target.Body.Name.NotEqual[Undead]}) /return
	
	| dont waste time if its slowed already
	/if (${Target.Slowed.Duration} && ${Target.Slowed.Name.NotEqual[Tendon Slice]}) {
		/call create_timer timer_slow_${Target2Kill} ${Target.Slowed.Duration.TotalSeconds}s
		/return
	}
	
	| check the things	
	/if (${Spawn[${_tmpID}].Type.Equal[corpse]} || !${Spawn[${_tmpID}].ID}) /return
	
	UPDATEHUD "Slow"	
	
	| if the spell is identified in the INI and memed we will use that over the AA
	/if (${Bool[${Me.Gem[${spellSlow}]}]}) {
		/delay 2.25s ${Me.SpellReady[${spellSlow}]}	
		/if (${validate_cast[FALSE, spell, "${spellSlow}", ${_tmpID}]}) /call core_cast2 "${spellSlow}" 0 ${_tmpID} FALSE

	| spell not identified, using AA.
	} else /if (${spellSlow.Equal[NULL]} || !${Bool[${spellSlow}]} || !${Bool[${Me.Gem[${spellSlow}]}]}) {
		/if (${Select[${Me.Class.ShortName},ENC]}) /varset spellSlow Slowing Helix
		/if (${Select[${Me.Class.ShortName},SHM]}) /varset spellSlow Turgur's Swarm
		/if (${Select[${Me.Class.ShortName},BST]}) /varset spellSlow Sha's Reprisal
		/if (${Select[${Me.Class.ShortName},NEC]}) /varset spellSlow Death's Malaise	

		/delay 2.25s ${Me.AltAbilityReady[${spellSlow}]}	
		/if (${validate_cast[FALSE, alt, "${spellSlow}", ${_tmpID}]}) /call core_cast2 "${spellSlow}" alt ${_tmpID} FALSE
	}
	
	| set time based upn cast return	
	/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
		/call create_timer timer_slow_${_tmpID} ${Spell[${spellSlow}].Duration.TotalSeconds}s
	} else /if (${Select[${castReturn},CAST_RESISTED,CAST_RESIST]}) {
		/call create_timer timer_slow_${_tmpID} 15s
	}
	
	UPDATEHUD FALSE
/return



|***
 *	DES: casts Malo or Tash depending on the class
 *	USE:  /call cast_malo [target ID]
 *	INI: 
 *  NOTE: 
 ***|
sub cast_malo(int _tmpID)
	DEBUG cast_malo(${_tmpID})
	AMIDEAD
	CHECKEXIT
	CHECKINPUTS
	/if (${is_target_dead[${_tmpID}]}) /return
	
	/squelch /target ID ${_tmpID}
	/delay 1s ${Target.ID} == ${_tmpID}
	
	| check the things .. normal things	
	/if (${Spawn[${_tmpID}].Type.Equal[corpse]} || !${Spawn[${_tmpID}].ID}) /return

	| dont waste time if its Maloed already
	/if (${Target.Maloed.Duration}) {
		/call create_timer timer_malo_${Target2Kill} ${Target.Maloed.Duration.TotalSeconds}s
		/return
	}
	UPDATEHUD "Malo"

	| now the spell 
	/if (${Bool[${Me.Gem[${spellMalo}]}]}) {
		/delay 2.25s ${Me.SpellReady[${spellMalo}]}	
		/if (${validate_cast[FALSE, spell, "${spellMalo}", ${_tmpID}]}) /call core_cast2 "${spellMalo}" 0 ${_tmpID} FALSE
		
	| fall back to AA	
	} else /if (${spellMalo.Equal[NULL]} || !${Bool[${spellMalo}]} || !${Bool[${Me.Gem[${spellMalo}]}]}) {
		
		| magician and shaman
		/if (${Select[${Me.Class.ShortName},MAG,SHM]} && ${SpawnCount[npc radius ${MobAgro} playerstate 4]} >= 2 && ${Me.AltAbilityReady[Wind of Malaise]}) {
			/varset spellMalo Wind of Malaise
		} else /if (${Select[${Me.Class.ShortName},MAG,SHM]} && (${SpawnCount[npc radius ${MobAgro} playerstate 4]} == 1 || !${Me.AltAbilityReady[Wind of Malaise]})) {
			/varset spellMalo Malaise
		}

		| necromancer		
		/if (${Select[${Me.Class.ShortName},NEC]}) /varset spellMalo Scent of Terris

		/delay 2.25s ${Me.AltAbilityReady[${spellMalo}]}	
		/if (${validate_cast[FALSE, alt, "${spellMalo}", ${_tmpID}]}) /call core_cast2 "${spellMalo}" alt ${_tmpID} FALSE
	
	}
	
	| set the timers	
	/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
		/call create_timer timer_malo_${_tmpID} ${Spell[${spellMalo}].Duration.TotalSeconds}s
	} else /if (${Select[${castReturn},CAST_RESISTED,CAST_RESIST]}) {
		/call create_timer timer_malo_${_tmpID} 15s
	}
	
	UPDATEHUD FALSE	
/return



|***
 *	DES: cripples the target
 *	USE: /call cast_cripple [ID] | /debuff cripple
 *	INI: 
 *  NOTE: not for wizards.
 ***|
sub cast_cripple(int _tmpID)
	DEBUG cast_cripple(${_tmpID})
	/if (${timer_cripple_${_tmpID}}) /return
	/if (${Spawn[${_tmpID}].Type.Equal[corpse]} || !${Spawn[${_tmpID}].ID}) /return

	AMIDEAD
	CHECKEXIT
	CHECKTIE
	CHECKINPUTS

	/squelch /target ID ${_tmpID}
	/delay 1s ${Target.ID} == ${_tmpID}

	| if its crippled, move on
	/if (${Target.Crippled.Duration} || ${Target.BuffDuration[${spellCripple}].TotalSeconds}) {
		/call create_timer timer_cripple_${_tmpID} ${Target.Crippled.Duration.TotalSeconds}s
		/return
	}

	UPDATEHUD "Cripple"	

	| spell first
	/if (${Bool[${Me.Gem[${spellCripple}]}]}) {
		/delay 2.25s ${Me.SpellReady[${spellCripple}]}	
		/if (${Select[${Me.Class.ShortName},ENC]}) {
			/if (${validate_cast[FALSE, spell, "${spellCripple}", ${_tmpID}]})  /call core_cast2 "${spellCripple}" 0 ${_tmpID} check_for_adds
		} else {
			/if (${validate_cast[FALSE, spell, "${spellCripple}", ${_tmpID}]})  /call core_cast2 "${spellCripple}" 0 ${_tmpID}
		}
		
	| fall back to alt ability	
	} else /if (${Select[${Me.Class.ShortName},ENC,WIZ,SHM]} && (!${Bool[${Me.Gem[${spellCripple}]}]} || ${spellCripple.Equal[NULL]} || !${Bool[${spellCripple}]})) {
		/if (${Select[${Me.Class.ShortName},ENC]}) /varset spellCripple Slowing Helix
		/if (${Select[${Me.Class.ShortName},SHM]}) /varset spellCripple Crippling Spirit
		/if (${Select[${Me.Class.ShortName},WIZ]}) /varset spellCripple Lower Element
		
		/if (${Select[${Me.Class.ShortName},ENC]}) {
			/if (${validate_cast[FALSE, alt, "${spellCripple}", ${_tmpID}]})  /call core_cast2 "${spellCripple}" alt ${_tmpID} check_for_adds
		} else {
			/if (${validate_cast[FALSE, alt, "${spellCripple}", ${_tmpID}]})  /call core_cast2 "${spellCripple}" alt ${_tmpID}
		}		
		
	}

	| set timers
	/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
		/call create_timer timer_cripple_${_tmpID} ${Spell[${spellCripple}].Duration.TotalSeconds}s
	} else /if (${Select[${castReturn},CAST_RESISTED,CAST_RESIST]}) {
		/call create_timer timer_cripple_${_tmpID} 15s
	}
	UPDATEHUD FALSE
/return



|***
 *	DES: sets a base line for all target2kill timers
 *	USE: /call set_base_target_timers
 *	INI: 
 *  NOTE: we use this to prevent undeclared variables since targets change .. a lot.
 ***|
sub set_base_target_timers(int _tmpID)
DEBUG set_base_target_timers(${_tmpID})
	CHECKINPUTS
	
	| movement stick behind
	/if (${SetStickBehind}) {
		/if (!${Defined[timer_snap_recheckID_${_tmpID}]}) /call create_timer timer_snap_recheckID_${_tmpID} 1
	}
	
	| class eradicate buffs AA
	/if (${switchdeBuffEradicate}) {
		/if (!${Defined[timer_eradicate_${_tmpID}]}) /call create_timer timer_eradicate_${_tmpID} 1
	}
	
	/if (${switchdeBuffTash}) {
		/if (!${Defined[timer_tash_${_tmpID}]}) /call create_timer timer_tash_${_tmpID} 1
	}
	
	/if (${switchdeBuffSlow}) {
		/if (!${Defined[timer_slow_${_tmpID}]}) /call create_timer timer_slow_${_tmpID} 1
	}
	
	/if (${switchdeBuffCripple}) {
		/if (!${Defined[timer_cripple_${_tmpID}]}) /call create_timer timer_cripple_${_tmpID} 1
	}
	
	/if (${switchdeBuffMezz}) {
		/if (!${Defined[timer_mez_${_tmpID}]}) /call create_timer timer_mez_${_tmpID} 1
	}
	
	/if (${switchdeBuffSnare}) {
		/if (!${Defined[timer_snare_${_tmpID}]}) /call create_timer timer_snare_${_tmpID} 1
	}
	/if (${switchdeBuffMalo}) {
		/if (!${Defined[timer_malo_${_tmpID}]}) /call create_timer timer_malo_${_tmpID} 1
	}
/return 
 
 
 
|***
 *	DES: cycles all debuff routines.
 *	USE: check_deBuffs
 *	INI: 
 *  NOTE: 
 ***|
sub check_deBuffs(int _tmpID)
	DEBUG check_deBuffs(${_tmpID})
	AMIDEAD
	CHECKEXIT
	CHECKTIE
	CHECKINPUTS
	
	/call set_base_target_timers ${Target2Kill}

	| /if (${Target.ID} != ${_tmpID}) /squelch /target ID ${_tmpID}
	
	| tash
	/if (${switchdeBuffTash}) {
	 	/if (!${Bool[${timer_tash_${_tmpID}}]}) {
			/if (${Select[${Me.Class.ShortName},ENC]}) { 
				/call cast_tash ${_tmpID}
			} else {
			}
		}
	}
	
	| malo
	/if (${switchdeBuffMalo}) {
		/if (!${Bool[${timer_malo_${_tmpID}}]}) {
			/call cast_malo ${_tmpID}
		}
	}
	
	| slow. spell if spellSlow defined otherwise AA
	/if (${switchdeBuffSlow}) {
		/if (!${Bool[${timer_slow_${_tmpID}}]} && !${validate_class[FALSE, |BRD]}) {
			/if (${Select[${Me.Class.ShortName},SHM]}) {
				/if (${SpawnCount[npc radius ${MobAgro} playerstate 4]} > 2) {
					/if (${validate_cast[FALSE, spell, "${spellAESlow}", ${_tmpID}]}) /call core_cast2 "${spellAESlow}" 0 ${_tmpID} FALSE
				} else {
					/call cast_slow ${_tmpID}
				}
			} else {			
				/call cast_slow ${_tmpID}
			}
			/if (${Select[${Me.Class.ShortName},ENC]}) { 
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}
		}
	}
		
	| cripple
	/if (${switchdeBuffCripple}) {
		
		/if (${Select[${Me.Class.ShortName},DRU]}) {
			/call cast_AA_ro ${_tmpID}	
		}	else /if (${Select[${Me.Class.ShortName},ENC]}) { 
			/if (!${Bool[${timer_cripple_${_tmpID}}]}) {		
				/if (${Spell[${spellCripple}].ID} != ${Spell[${spellSlow}].ID}) /call cast_cripple ${_tmpID}
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}
		} else {
			/call cast_cripple ${_tmpID}
		}
	}
	
	| eradicate magic AA line
	/if (${switchdeBuffEradicate} && ${validate_class[FALSE, ${AAEradicateList}]}) {
		/if (!${Bool[${timer_eradicate_${_tmpID}}]}) {
			/call cast_AA_eradicate_magic ${_tmpID}
			/if (${Select[${Me.Class.ShortName},ENC]}) { 
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}
		}
	}
	
	| aa snare
	/if (${switchdeBuffSnare} && ${validate_class[FALSE, ${AASnareList}]}) {
		/if (!${Bool[${timer_snare_${_tmpID}}]}) {
			/call cast_AA_snare ${_tmpID}
		}
	}
/return



|***
 *	DES: checks the build increments. if theres a varriance, will check INIs
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub verify_build()
	| if mismatch #s
	/if (${classBuild.NotEqual[${coreBuild}]}) {

		/if (${forceINIcheck}) {
			| clean INIs. do not force old clean
			/call fix_INI_declares FALSE
			
			| fix INI version builds
			/call writeINI classBuild common ${coreBuild}
			
			| clean out all old aliases.
			/call whitewash NOECHO
			
			| re establish aliases
			/call set_alias
		}
	}
/return



|***
 *	DES: swaps the bandolier if needed
 *	USE: /call bandolier_swap BANDTYPE
 *	INI: 
 *  NOTE: 
 ***|
sub bandolier_swap(string _band)
	DEBUG bandolier_swap(${_band})
	/if (${bandolierSwap.Equal[${bandolier${_band}}]}) /return TRUE
	/if (!${switchBandolier}) /return FALSE
	/if (!${Bool[${bandolierNormal}]}) /return FALSE
	/if (!${Bool[${bandolier${_band}}]}) /return FALSE
	/delay 1
	/bandolier Activate ${bandolier${_band}}
	/varset bandolierSwap ${bandolier${_band}}
/return TRUE



|***
 *	DES: check to see that you have a specific expansion
 *	USE: /if (!${validate_expansion[TRUE/FALSE, |#|#|#]}) /return
 *	INI: 
 *  NOTE: check for any FALSE returns. if even one, bail as bad.
 *  
 *  1	The Ruins of Kunark												2	The Scars of Velious
 *  3	The Shadows of Luclin											4	The Planes of Power
 *  5	The Legacy of Ykesha											6	Lost Dungeons of Norrath
 *  7	Gates of Discord													8	Omens of War
 *  9	Dragons of Norrath												10	Depths of Darkhollow
 *  11 Prophecy of Ro														12 The Serpent's Spine
 *  13 The Buried Sea														14 Secrets of Faydwer
 *  15 Seeds of Destruction											16 Underfoot
 *  17 House of Thule														18 Veil of Alaris
 *  19 Rain of Fear															20 Call of the Forsaken
 *  21 The Darkened Sea													22 The Broken Mirror
 *  23 Empires of Kunark												24 Ring of Scale		 
 *  25 The Burning Lands
 *   
 ***|
sub validate_expansion(bool _error, string _exp)
	DEBUG validate_expansion(${_error}, ${_exp})
	/if (${switchIgnoreXpacControl}) /return TRUE
	/declare _pass bool local TRUE
	/declare _count int local 0

	/for _count 1 to ${_exp.Count[|]}
		AMIDEAD
		/if (!${Me.HaveExpansion[${_exp.Arg[${_count},|]}]}) {
			/varset _pass FALSE
			/break
		}
	/next _count

	/if (!${_pass} && ${_error}) OUT ${onotice} ${sep} Expansion ${cinfo}${expansion${_count}}\ax required for this feature.
/return ${_pass}



|***
 *	DES: check and adjust the debuff switches
 *	USE: /call validate_debufs [T|F echo error] [debuff switch|NULL]
 *	INI: 
 *  NOTE: 
 ***|
sub validate_debuff(bool _error, string _deBuffType)
	DEBUG validate_debuff(${_error}, ${_deBuffType})
	| check mezz switch
	/if (${Bool[${_deBuffType.Equal[mezz]}]} || ${switchdeBuffMezz}) {
		/if (!${validate_class[${_error}, |ENC|BRD|MAG|NEC]}) {
			/call writeINI switchdeBuffMezz Buffs FALSE  
			/return FALSE
		}
	}
	
	| check malo switch
	/if (${Bool[${_deBuffType.Equal[malo]}]} || ${switchdeBuffMalo}) {
		/if (!${validate_class[${_error}, |MAG|SHM|NEC]}) {
			/call writeINI switchdeBuffMalo Buffs FALSE  
			/return FALSE
		}
	}
	
	| check tash switch
	/if (${Bool[${_deBuffType.Equal[tash]}]} || ${switchdeBuffTash}) {
		/if (!${validate_class[${_error}, |ENC]}) {
			/call writeINI switchdeBuffTash Buffs FALSE  
			/return FALSE
		}
	}
	
	| check cripple switch
	/if (${Bool[${_deBuffType.Equal[cripple]}]} || ${switchdeBuffCripple}) {
		/if (!${validate_class[${_error}, |ENC|SHM|DRU|MAG|NEC|SHD|BST|WIZ]}) {
			/call writeINI switchdeBuffCripple Buffs FALSE  
			/return FALSE
		}
	}
	
	| check slow swich
	/if (${Bool[${_deBuffType.Equal[slow]}]} || ${switchdeBuffSlow}) {
		/if (!${validate_class[${_error}, |ENC|SHM|BRD|NEC|BST]}) {
			/call writeINI switchdeBuffSlow Buffs FALSE  
			/return FALSE
		}
	}
	
	| check eradicate switch
	/if (${Bool[${_deBuffType.Equal[eradicate]}]} || ${switchdeBuffEradicate}) {
		/if (!${validate_class[${_error}, ${AAEradicateList}]}) {
			/call writeINI switchdeBuffEradicate Buffs FALSE  
			/return FALSE
		}
	}
	
	| check snare switch
	/if (${Bool[${_deBuffType.Equal[snare]}]} || ${switchdeBuffSnare}) {
		/if (!${validate_class[${_error}, ${AASnareList}|BER]}) {
			/call writeINI switchdeBuffSnare Buffs FALSE  
			/return FALSE
		}
	}
/return TRUE



|***
 *	DES: watches to see if you are deaded. if so.. theres a couple things we need to do.
 *	USE: /if (${watch_for_death[]}) /do a thing
 *	INI: 
 *  NOTE: returns switchAmIDead TRUE if you died
 ***|
sub watch_for_death()
	DEBUG watch_for_death()
	CHECKINPUTS

	| check for current flags. watch for rez box if I have them
	/if (${switchAmIDead}) {
		/return TRUE
	} else /if (${flagCallNeedRez}) {
		/return TRUE

	| check death states
	} else /if (${Me.State.Equal[HOVER]} || ${Me.Hovering} || ${Me.Dead}) {
		/if (${Me.Casting.ID}) /stopcast
		/varset switchAmIDead TRUE
	
	| did I just die and get sent to bind ?
	} else /if (${Me.Buff[Resurrection Sickness].ID} && ${Zone.ID} == ${Me.ZoneBound.ID}) {
		/varset switchAmIDead TRUE
	}

	| if we died, shut shit down
	/if (${switchAmIDead}) {
		/doevents flush
		/delay 2s
		| update death count
		/ini "${INICore}" "deathcount" "${MacroQuest.Server}.${Me.Class.ShortName}.${Me.DisplayName}" "${Int[${Math.Calc[${Ini[${INICore},deathcount,${MacroQuest.Server}.${Me.Class.ShortName}.${Me.DisplayName}]}+1]}]}"

		| stop all the things(s)
		/call set_stop NOECHO
		/call clear_combat
		/if (${Me.Casting.ID}) /stopcast
		/varset Target2Kill 0
		/if (${coreAuto}) ECHOCHANNEL Needs a rez .. "${Me.DisplayName}"
	}
/return ${switchAmIDead}
	


|***
 *	DES: watches for the rez box if we have bben marked as dead or called.
 *	USE: /call  watch_for_rezbox()
 *	INI: 
 *  NOTE: 
 ***|
sub watch_for_rezbox()
	DEBUG watch_for_rezbox(${minRezPct})
	CHECKINPUTS
	
	| dont rezz me
	/if (!${switchRezTake}) /return
	
	| not dead? GO AWAY
	/if (!${switchAmIDead} && !${flagCallNeedRez}) /return
		
	| bail if no rez box to check
	/if (!${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["Do you wish this?"]}]}) /return

	| is there a rez window?
	/if (${Window[ConfirmationDialogBox].Open}) {

	| /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["Do you wish this?"]}]}) {

		| get rez window % to something we can use			
		/if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["percent)"]}]}) {
			/if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(10 percent)"]}]}) { 
				/varset rezWindowPct 10
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(20 percent)"]}]}) {
				/varset rezWindowPct 20
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(35 percent)"]}]}) { 
				/varset rezWindowPct 35
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(50 percent)"]}]}) { 
				/varset rezWindowPct 50
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(60 percent)"]}]}) { 
				/varset rezWindowPct 60
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(75 percent)"]}]}) { 
				/varset rezWindowPct 75
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(90 percent)"]}]}) { 
				/varset rezWindowPct 90
			} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(96 percent)"]}]}) { 
				/varset rezWindowPct 96
			}
		}
			
		| we do not accept any 0% rezes
		/if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(0 percent)"]}]}) {
			/varset validRezFound FALSE
		| 10% rez and higher
		} else /if (${minRezPct} == 10 && ${Select[${rezWindowPct},10,20,35,50,60,75,90,93,96]}) {
			/varset validRezFound TRUE
		| 20% rez and higher
		} else /if (${minRezPct} == 20 && ${Select[${rezWindowPct},20,35,50,60,75,90,93,96]}) {
			/varset validRezFound TRUE
		| 45% rez and higher
		} else /if (${minRezPct} == 35 && ${Select[${rezWindowPct},35,50,60,75,90,93,96]}) {
			/varset validRezFound TRUE
		| 50% rez and higher
		} else /if (${minRezPct} == 50 && ${Select[${rezWindowPct},50,60,75,90,93,96]}) {
			/varset validRezFound TRUE
		| 60% rez and higher
		} else /if (${minRezPct} == 60 && ${Select[${rezWindowPct},60,75,90,93,96]}) {
			/varset validRezFound TRUE
		| 75% rez and higher
		} else /if (${minRezPct} == 75 && ${Select[${rezWindowPct},75,90,93,96]}) {
			/varset validRezFound TRUE
		| 90% rez and higher
		} else /if (${minRezPct} == 90 && ${Select[${rezWindowPct},90,93,96]}) {
			/varset validRezFound TRUE
		| 93% rez
		} else /if (${minRezPct} == 93 && ${Select[${rezWindowPct},93,96]}) {
			/varset validRezFound TRUE
		| 96% rez
		} else /if (${minRezPct} == 96 && ${Select[${rezWindowPct},96]}) {
			/varset validRezFound TRUE
		| 100% rez.. we take these	
		} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(100 percent)"]}]}) {
			/varset validRezFound TRUE
		| we take call to corpse as well
		} else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["you will still be able to get a resurrection later"]}]}) {
			/varset flagCallNeedRez TRUE
			/varset validRezFound TRUE
		} 
			
		| more waiting
		/if (${validRezFound}) {
			/nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
			/delay 5
			/if (${Window[RespawnWnd].Open}) {
				/if (${Zone.ShortName.Equal[Powar]}) {
					/nomodkey /notify RespawnWnd RW_OptionsList listselect 1
				} else {
					/nomodkey /notify RespawnWnd RW_OptionsList listselect 2 
				}
				/delay 1s
				/nomodkey /notify RespawnWnd RW_SelectButton leftmouseup
				
			}

			| Clear ALL THE MARKERS!
			/delay 2s !${Me.State.Equal[HOVER]}
			/delay 2s !${Me.Hovering}
			/delay 2s !${Me.Dead}
			/call set_stop NOECHO
			/varset switchAmIDead FALSE
			/varset validRezFound FALSE
			/varset rezWindowPct 0
			/return TRUE			
		} else /if (!${validRezFound}) {
			/nomodkey /notify ConfirmationDialogBox No_Button leftmouseup	
			/delay 5
			/return FALSE
		} 
	}
/return



|***
 *	DES: this is for waisting AAs. it does nothing more. if you expect it to do more, go away.
 *	USE: /call cast_AA_fireworks()
 *	INI: 
 *  NOTE: 
 ***|
sub cast_AA_fireworks()
	DEBUG cast_AA_fireworks()
	/if (!${switchAAFireworks} || ${Raid.Members}) /return
	/call create_timer timer_Waste_Fireworks ${tWaste_Fireworks}
	/if (${Me.AAPoints} < 20) /return

	| show me the money
	/if (${Me.AAPoints} >= 120 && !${Me.AltAbilityReady[Glyph of Fireworks II]}) {
		/call buy_AA "Glyph of Fireworks II"
		/call create_timer timer_Waste_Fireworks 1m
		/return
	} else /if (${Me.AltAbilityReady[Glyph of Fireworks II]}) {
		VOUT 'yo .. hold my beer ..
		/if (${validate_cast[FALSE, alt, "Glyph of Fireworks II", ${Me.ID}]}) /call core_cast2 "Glyph of Fireworks II" alt 0 FALSE
	}
/return



 |***
 *	DES: click self healing items
 *	USE: /call click_item_heal
 *	INI: 
 *  NOTE: 
 ***|
sub click_item_heal()
	DEBUG click_item_heal()
	CHECKEXIT
	AMIDEAD
	/declare _count int local 0
	
	/for _count 1 to ${clickHealItem.Count[|]}
		/if (${Me.PctHPs} >= ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /break
		/if (!${FindItem[${clickHealItem.Arg[${_count},|]}].ID}) /continue
		/if (!${Me.ItemReady[${clickHealItem.Arg[${_count},|]}]}) /continue
		
		/if (${validate_cast[FALSE, item, "${clickHealItem.Arg[${_count},|]}", ${Me.ID}]}) /call core_cast2 "${clickHealItem.Arg[${_count},|]}" item 0 FALSE
	/next _count		
/return



 |***
 *	DES: make sure you have the correct zone INI info loaded
 *	USE: /call validate_zone True|False True|False
 *	INI: 
 *  NOTE: 
 ***|
sub validate_zone(bool _error, bool _force)
	DEBUG validate_zone(${_error}, ${_force})
	/if (${timer_validate_zone} && !${_force}) /return
	/call create_timer timer_validate_zone ${tValidate_Zone}
	/declare _zoned bool local FALSE

	| zone matches. nothing to see here. move on
	/if (${Zone.ShortName.Equal[${currentZone}]} && !${_force}) {
		| do nothing at all
		/return

	| zone does not match. wtf..
	} else /if (${Zone.ShortName.NotEqual[${currentZone}]} || ${_force}) {
		/varset currentZone ${Zone.ShortName}
		/call echos zone 0 0 0 0
		/call check_zone_INI FALSE
		/varset _zoned TRUE
		/call create_timer timer_zone_pause ${tZone_Pause}
		/doevents flush
	
		| check for mesh
		/if (${Bool[${Plugin[MQ2Nav]}]}) {
			/if (!${Navigation.MeshLoaded}) VOUT ${owarning} ${sep} NO Mesh Loaded
		}	
		
	}
	| yah. we zoned and stuff
	/if (${_zoned}) {
		/if (${burnForce}) /varset burnForce FALSE
		/squelch /makecamp off
		/if (${HomeSet}) /call writeINI HomeSet Movement FALSE
		/if (!${switchPersistentModes}) /call switch_clear_mode
		/if (${coreAuto} && ${switchShrink}) /call check_shrink
	}
/return



 |***
 *	DES: Buys an AA
 *	USE: /call buy_AA "AA NAME"
 *	INI: 
 *  NOTE: 
 ***|
sub buy_AA(string _name)
	DEBUG buy_AA(${_name})
	/if (!${Bool[${_name}]}) /return

	/if (${AltAbility[${_name}].CanTrain}) {
		/if (${Me.AltAbility[${_name}].CanTrain}) {
			/alt buy ${AltAbility[${_name}].NextIndex}
		} else {
			/alt buy ${AltAbility[${_name}].Index}
		}
	}
/return



 |***
 *	DES: asssit controlls
 *	USE: /call validate_assist [ECHO] [x|g|1|2|3|f|ma1|ma2] [NAME]
 *	INI: 
 *  NOTE: 
 ***|
sub validate_assist(string _echo, string _assist, string _name)
	DEBUG validate_assist(${_echo}, ${_assist}, ${_name})

	/declare _assistMode string local |x|g|1|2|3|ma1|ma2|f
	/declare _assM string local
	/declare _count int local 0
	/declare _listout string local
	/declare _sep bool local FALSE

	| disable all assists
	/if (${_assist.Equal[x]}) {
		/call writeINI MA1 Common FALSE
		/call writeINI MA2 Common FALSE
 		/call writeINI useSmartAssist1 Common FALSE
 		/call writeINI useSmartAssist2 Common FALSE
 		/call writeINI useSmartAssist3 Common FALSE
		/call writeINI useSmartAssistG Common FALSE
		/call writeINI useForceAssist Common FALSE
		/xtarget set 1 Autohater
	
	| group assist
	} else /if (${_assist.Equal[g]}) {
 		/call writeINI useSmartAssist1 Common FALSE
 		/call writeINI useSmartAssist2 Common FALSE
 		/call writeINI useSmartAssist3 Common FALSE
		/call writeINI useSmartAssistG Common TRUE
		/call writeINI useForceAssist Common FALSE
		/xtarget set 1 groupassisttarget	

	| raid assist 1
	} else /if (${_assist.Equal[1]}) {
 		/call writeINI useSmartAssist1 Common TRUE
 		/call writeINI useSmartAssist2 Common FALSE
 		/call writeINI useSmartAssist3 Common FALSE
		/call writeINI useSmartAssistG Common FALSE
		/call writeINI useForceAssist Common FALSE
		/xtarget set 1 raidassist1target

	| raid assist 2
	} else /if (${_assist.Equal[2]}) {
 		/call writeINI useSmartAssist1 Common FALSE
 		/call writeINI useSmartAssist2 Common TRUE
 		/call writeINI useSmartAssist3 Common FALSE
		/call writeINI useSmartAssistG Common FALSE
		/ca5ll writeINI useForceAssist Common FALSE
		/xtarget set 1 raidassist2target

	| raid assist 3
	} else /if (${_assist.Equal[3]}) {
 		/call writeINI useSmartAssist1 Common FALSE
 		/call writeINI useSmartAssist2 Common FALSE
 		/call writeINI useSmartAssist3 Common TRUE
		/call writeINI useSmartAssistG Common FALSE
		/call writeINI useForceAssist Common FALSE
		/xtarget set 1 raidassist3target
				
	| set assist to forced from driver
	} else /if (${_assist.Equal[f]}) {
		/call writeINI MA1 Common FALSE
		/call writeINI MA2 Common FALSE
 		/call writeINI useSmartAssist1 Common FALSE
 		/call writeINI useSmartAssist2 Common FALSE
 		/call writeINI useSmartAssist3 Common FALSE
		/call writeINI useSmartAssistG Common FALSE
		/call writeINI useForceAssist Common TRUE
		/xtarget set 1 Autohater
		
	| set assist to MA1
	} else /if (${_assist.Equal[ma1]}) {
		/if (${Bool[${_name}]}) {
			/call writeINI MA1 Common ${_name.Left[1].Upper}${_name.Right[-1].Lower}
		} else /if (!${Bool[${_name}]})  {
			/call writeINI MA1 Common FALSE
		} 				
		/xtarget set 1 Autohater
		
	| set assist to MA2
	} else /if (${_assist.Equal[ma2]}) {
		/if (${Bool[${_name}]}) {
			/call writeINI MA2 Common ${_name.Left[1].Upper}${_name.Right[-1].Lower}
		} else /if (!${Bool[${_name}]})  {
			/call writeINI MA2 Common FALSE
		} 				
		/xtarget set 1 Autohater
	} 

	/if (${_echo.Equal[NOECHO]}) /return
	/if (${switchMQclearonCLS}) /mqclear

	/for _count 1 to ${_assistMode.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_assistMode.Arg[${_count},|]}\ax

		/if (${_assistMode.Arg[${_count},|].Equal[g]}) /varset _listout ${_listout}(${If[${useSmartAssistG},${oon}\ax,${ooff}]})	
		/if (${_assistMode.Arg[${_count},|].Equal[1]}) /varset _listout ${_listout}(${If[${useSmartAssist1},${oon}\ax,${ooff}]})	
		/if (${_assistMode.Arg[${_count},|].Equal[2]}) /varset _listout ${_listout}(${If[${useSmartAssist2},${oon}\ax,${ooff}]})	
		/if (${_assistMode.Arg[${_count},|].Equal[3]}) /varset _listout ${_listout}(${If[${useSmartAssist3},${oon}\ax,${ooff}]})	
		/if (${_assistMode.Arg[${_count},|].Equal[ma1]}) /varset _listout ${_listout}(${If[${Bool[${MA1}]},${cnum}${MA1}\ax,${ooff}]})	
		/if (${_assistMode.Arg[${_count},|].Equal[ma2]}) /varset _listout ${_listout}(${If[${Bool[${MA2}]},${cnum}${MA2}\ax,${ooff}]})	
		/varset _sep TRUE
	/next _count
	VOUT /setassist [${_listout}]


/return



|***
 *	DES: cast or trigger alliance on target
 *	USE: /call trigger_alliance TARGETID
 *	INI: 
 *  NOTE:
 ***|
sub trigger_alliance(int _tmpID)
	DEBUG trigger_alliance(${_tmpID})
	| we need raid peoples
	/if (!${Raid.Members}) /return
		
	| alliance time
	/if (!${Target.BuffDuration[${${Me.Class.ShortName}Alliance}].TotalSeconds}) {
		/if (${validate_cast[FALSE, spell, "${Spell[${${Me.Class.ShortName}Alliance}].RankName}", ${_tmpID}]}) /call core_cast3 "${Spell[${${Me.Class.ShortName}Alliance}].RankName}" 0 ${_tmpID} FALSE
		/return
	}

	| my alliance. were done here
	/if (${Spawn[${Target.Buff[${${Me.Class.ShortName}Alliance}].Caster}].ID} == ${Me.ID}) /return

	| if theres no forced casted trigger, gtfo
	/if (!${Bool[${${Me.Class.ShortName}AllianceTrigger}]}) /return

	/declare _count int local 0

	| is one of the trigger spells ready?
	/for _count 1 to ${spellAllianceTrigger.Count[|]}
		/if (${Me.SpellReady[${Spell[${spellAllianceTrigger.Arg[${_count},|]}].RankName}]}) {
			/if (${validate_cast[FALSE, spell, "${Spell[${spellAllianceTrigger.Arg[${_count},|]}].RankName}", ${_tmpID}]}) /call core_cast2 "${Spell[${spellAllianceTrigger.Arg[${_count},|]}].RankName}" 0 ${_tmpID} FALSE
			/continue
		}
	/next _count
/return



|***
 *	DES: sets an outer variable
 *	USE: /call setDeclare [variable Name] [variable type] outer [value]
 *	INI: 
 *  NOTE: 
 ***|
sub setDeclare(string _name, string _type, string _section, string _value)
	/if (${Defined[${_name}]}) {
		/varset ${_name} ${_value}
	} else {
		/declare ${_name} ${_type} outer ${_value}
	}
/return



|***
 *	DES: creates timers
 *	USE: /call declareTimer [NAME] [INT|BOOL|STRING|FLOAT] [INISECTION] [value] [timer_check_blah_blah]
 *	INI: 
 *  NOTE: 
 ***|
sub setTimer(string _name, string _type, string _section, string _value, string _my_timer)
	/if (!${Bool[${_name}]} || !${Bool[${_type}]} || !${Bool[${_section}]} || !${Bool[${_my_timer}]}) {
		/if (!${Bool[${_name}]}) VOUT ${ocaution} ${sep} BAD variable Name. 
		/if (!${Bool[${_type}]}) VOUT ${ocaution} ${sep} BAD variable Type. 
		/if (!${Bool[${_section}]}) VOUT ${ocaution} ${sep} BAD INI Section.
		/if (!${Bool[${_my_timer}]}) VOUT ${ocaution} ${sep} BAD Timer Name. 
		/return
	}

	/if (!${Defined[${_name}]}) /declare ${_name} ${_type} outer
	/varset ${_name} ${Ini[${INIClass},${_section},${_name},"${_value}"]}
	/if (!${Bool[${Ini[${INIClass},${_section}].Find[${_name}]}]}) {
		/ini "${INIClass}" "${_section}" "${_name.Left[1].Upper}${_name.Right[-1]}" "${_value}"
	} 

	/call create_timer ${_my_timer} 1
/return



|***
 *	DES: active AA Shield Flash when HP reach a specific %
 *	USE: /if (${useShieldFlash}) /call cast_AA_shield_flash
 *	INI: 
 *  NOTE: 
 ***|
sub cast_AA_shield_flash()
	DEBUG cast_AA_shield_flash()
	SIMPLEEXIT
	/if (!${coreAuto}) /return
	/if (${Me.PctHPs}*.${tmpHealOverride} > ${useShieldFlashPct}) /return
	/if (${validate_cast[FALSE, alt, "Shield Flash", ${Me.ID}]}) /call core_cast2 "Shield Flash" alt 0 FALSE
/return



|***
 *	DES: checks senders anme or guild status to the safelist ins core.ini
 *	USE: ${validate_safelist[${_sender}]}
 *	INI: core.ini
 *  NOTE: we assume no one is trustable
 ***|
sub validate_safelist(string _sender)
	DEBUG validate_safelist(${_sender})
	| override and accept everyone
	/if (${switchOverrideSafelist}) /return TRUE

	| basic safe list check
	/if (${safelist.Find[${_sender}]}) /return TRUE

	| guild safe list check
	/if (${switchGuildinSafelist} && ${Bool[${Me.Guild}]}) {
		/if (${Spawn[PC ${_sender}].Guild.Equal[${Me.Guild}]}) /return TRUE
	}
/return FALSE



|***
 *	DES: Make sure that what ever we are targeting is something we want to kill.
 *	USE: /if (${validate_target[FALSE, target ID]}
 *	INI: 
 *  NOTE: in manual mode. anything is a valid target.
**|
sub validate_target(bool _error, int _tmpID)
	DEBUG validate_target(${_error}, ${_tmpID})
	/if (!${coreAuto}) /return TRUE
	AMIDEAD
	CHECKINPUTS
		
	/if (!${Target.ID}) /return FALSE
	/if (!${Bool[${_tmpID}]}) /return FALSE
	/if (${SafeZone} && !${switchOverrideSafezone}) /return FALSE

	| check smart assists
	/if (!${useForceAssist}) {
		
		/if (${Target.ID}) {
			/if ((${useSmartAssist1} || ${useSmartAssist2} || ${useSmartAssist3}) && ${Spawn[${_tmpID}].ID} != ${Me.XTarget[1].ID}) {
				/call clear_combat
				/return FALSE
			} else /if (${useSmartAssistG} && ${Group.MainAssist.ID} && !${Bool[${Spawn[${Group.MainAssist}].Dead}]} && ${Spawn[${_tmpID}].ID} != ${Me.XTarget[1].ID}) {
				/call clear_combat
				/return FALSE
			}
		}
		
		/if (!${switchOverrideEngageat} && !${switchPull}) {
			/if (${Spawn[${_tmpID}].PctHPs} > ${engageAt} && !${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssist3} && !${useSmartAssistG}) {
				/delay 1
				/return FALSE
			} else /if (${Spawn[${_tmpID}].PctHPs} > ${smartengageAt} && (${useSmartAssist1} || ${useSmartAssist2} || ${useSmartAssist3}|| ${useSmartAssistG})) {
				/delay 1
				/return FALSE
			}
		}
	}
	
	| shit we really shouldnt't be attacking
	/if (${Select[${Spawn[${_tmpID}].Type},AURA,BANNER,CAMPFIRE,CORPSE,CHEST,ITEM,TRIGGER,TRAP,TIMER,MOUNT,Mercenary]}) /return FALSE
	/if (${ExcludeList.Find[${Spawn[${_tmpID}].DisplayName}]}) /return FALSE
	/if (${Target.ID} && ${Bool[${Target.Mezzed.Duration}]} && !${switchPull}) /return FALSE
	/if ((!${Spawn[${_tmpID}].LineOfSight} && !${switchOverrideLos}) || ${Spawn[${_tmpID}].Distance} > ${MobAgro}) /return FALSE
	/if (${Spawn[${_tmpID}].Type.Equal[PC]}) /return FALSE
	/if (${Target.Type.Equal[PC]}) /varset isValidTarget FALSE
	/if (${_tmpID} == ${Me.ID} || ${_tmpID} == ${Me.Pet.ID}) /return FALSE

	| /if (${Select[${Me.Class.ShortName},ROG]} && ${switchSos}) /makemevisible

/return TRUE



|***
 *	DES: displays the port and builds the command to the porting class
 *	USE: /port 
 *	INI: 
 *  NOTE: expect sarcasim if you cannot spell
 ***| 
#bind port_list /port
sub Bind_port_list(string _class, string _location)
	DEBUG Bind_port_list(${_class}, ${_location})
	/declare _listout 				string 	local
	/declare _listouttype			string 	local
	/declare _sep 						bool 		local FALSE
	/declare _country 				int 		local 0
	/declare _count 					int 		local 0
	/declare _state 					int 		local 0
	/declare _portclass 			string 	local |druid|wizard
	| /declare _porttype	 			string 	local |tl|group
	/declare _continent 			string 	local |alaris|antonica|discord|faydwer|kunark|luclin|odus|serpentsspine|taelosia|planes|other
	/declare _tell						bool		local FALSE

	| druid port list
	/declare _portDruidAlaris 											string local |beast|pillars|shardslanding
	/declare _portDruidAntonica  										string local |commonlands|feerrott|karana|lavastorm|misty|ro|surefall|undershore|westkarna
	/declare _portDruidDiscord 											string local |bloodfields|wallofslaughter
	/declare _portDruidFaydwer 											string local |butcherblock|looping|steamfront
	/declare _portDruidKunark 											string local |dreadlands|lceanium
	/declare _portDruidLuclin 											string local |dawnshroud|grimling|nexus|twilight
	/declare _portDruidOdus 												string local |buriedsea|stonebrunt|tempesttemple|tox
	/declare _portDruidSerpentsspine							 	string local |blightfire|direwind|steppes
	/declare _portDruidTaelosia 										string local |barindu|natimbi
	/declare _portDruidPlanes 											string local |grounds|potime|brells|arcstone|knowledge
	/declare _portDruidVelious 											string local |cobaltscar|wakening|greatdivide|iceclad
	/declare _portDruidOther 												string local |primarypush|secondarypush|primary|secondary

	| wizard port list
	/declare _portWizardAlaris 											string local |pillars|sarith 
	/declare _portWizardAntonica 										string local |northkarana|commonlands|nektulos|qeyenos|northro|cazic|undershore|westkarana
	/declare _portWizardDiscord 										string local |wallofslaughter|bloodfields
	/declare _portWizardFaydwer 										string local |fay|dragonscale
	/declare _portWizardKunark 											string local |lceanium|dreadlands|skyfire
	/declare _portWizardLuclin 											string local |dawnshroud|twilight|grimling
	/declare _portWizardOdus 												string local |tempesttemple|katta
	/declare _portWizardSerpentsspine							 	string local |tox|stonebrunt|icefall|sunderock|blightfire
	/declare _portWizardTaelosia 										string local |barindu|natimbi
	/declare _portWizardPlanes 											string local |grounds|potime|brells|arcstone|pok
	/declare _portWizardVelious 										string local |cobaltscar|wakening|greatdivide|iceclad
	/declare _portWizardOther 											string local |bind|nexus|primary|secondary|primarypush|secondarypush
			
	| sort the class
	/if (${Bool[${_class.Find[druid]}]}) {
		/varset _class Druid
	} else /if (${Bool[${_class.Find[wizard]}]}) {
		/varset _class Wizard
	} else {
		| build class types
		/for _count 1 to ${_portclass.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_portclass.Arg[${_count},|]}\ax
			/varset _sep TRUE
		/next _count
		/varset _sep FALSE
		
		|** build port types
		/for _count 1 to ${_porttype.Count[|]}
			/varset _listouttype ${_listouttype} ${If[${_sep},${sep},]} ${cinfo}${_porttype.Arg[${_count},|]}\ax
			/varset _sep TRUE
		/next _count
		**|

		| VOUT /port [${_listout}] [${cinfo}location\ax] [${_listouttype}]
		VOUT /port [${_listout}] [${cinfo}location\ax]
		/return
	}
	
  | echo locations if class correct
	/if (${Bool[${_class}]} && !${Bool[${_location}]}) {
		/for _country 1 to ${_continent.Count[|]}
			/for _state 1 to ${_port${_class}${_continent.Arg[${_country},|].Left[1].Upper}${_continent.Arg[${_country},|].Right[-1].Lower}.Count[|]}
				/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_port${_class}${_continent.Arg[${_country},|].Left[1].Upper}${_continent.Arg[${_country},|].Right[-1].Lower}.Arg[${_state},|]}\ax
				/varset _sep TRUE
			/next _state
			VOUT ${_continent.Arg[${_country},|].Left[1].Upper}${_continent.Arg[${_country},|].Right[-1].Lower} [${_listout}] 
			/varset _listout
			/varset _sep
		/next _country

	| request port when class and location are present
	} else /if (${Bool[${_class}]} && ${Bool[${_location}]}) {
		/if (${setCommunications.Equal[eqbc]}) {
			/bct ${get_resident[${_class}, 100]} /taxi_service "${Me.DisplayName}" "${_location}"
		} else /if (${setCommunications.Equal[dannet]}) {
			/dex ${get_resident[${_class}, 100]} /taxi_service "${Me.DisplayName}" "${_location}"
		}
	}

/return



|***
 *	DES: zephyr/TL requesting toon
 *	USE: this is built by the /port bind request
 *	INI: 
 *  NOTE: 
 ***|
#bind taxi_service /taxi_service
sub Bind_taxi_service(string _sender, string _location)

	| pick a reason to not do something.. whatever you like
	/if (!${Select[${Me.Class.ShortName},DRU,WIZ]}) /return
	/if (!${Defined[${_location}]}) {
		/t ${_sender} ${_location} is not a place. Please quit bothering me.
		/return
	}
	CHECKSAFELIST
	/if (${Spawn[pc ${_sender}].Distance} > 100) /return
	
	| send 'em on their way with a valid location request
	/call mem_spell TRUE "${${_location}}" ${buffGem} TRUE
	/t ${_sender} Please enjoy your flight. ${${_location}} on its way.
	/if (${validate_cast[FALSE, spell, "${${_location}}", ${Spawn[pc ${_sender}].ID}]}) /call core_cast2 "${${_location}}" ${buffGem} ${Spawn[pc ${_sender}].ID}]} FALSE

/return



|***
 *	DES: returns a valid resident if there is one
 *	USE: ${get_resident[|mage|wizard|druid, 100]}
 *	INI: core.ini->[general]->residentmage, residentdruid, residentwizard
 *  NOTE: 
 ***|
sub get_resident(string _class, int _distance)
	DEBUG get_resident(${_class}, ${_distance})
	
	/varset _class ${_class.Left[1].Upper}${_class.Right[-1].Lower}
	/declare _name string local FALSE
	/declare _count int local 0
	
	/for _count 1 to ${Resident${_class}.Count[|]}
		| SDEBUG  ${_class}, ${_distance} --  ${Resident${_class}.Count[|]}:${Resident${_class}.Arg[${_count},|]}
		/if (!${Bool[${Resident${_class}.Arg[${_count},|]}]}) /continue
		/if (!${Bool[${Spawn[${Resident${_class}.Arg[${_count},|]}].ID}]}) /continue
		/if (${Spawn[${Resident${_class}.Arg[${_count},|]}].Dead}) /continue
		/if (${Spawn[${Resident${_class}.Arg[${_count},|]}].Distance} > ${_distance}) /continue
		/if (${setCommunications.Equal[eqbc]}) {
			/if (!${Spawn[${Resident${_class}.Arg[${_count},|]}].InZone}) /continue
		}
		| /echo _name ${Resident${_class}.Arg[${_count},|].Left[1].Upper}${Resident${_class}.Arg[${_count},|].Right[-1].Lower}
		/varset _name ${Resident${_class}.Arg[${_count},|].Left[1].Upper}${Resident${_class}.Arg[${_count},|].Right[-1].Lower}
		/if (${Bool[${_name}]}) /break
	/next _count	

/return ${_name}



|***
 *	DES: Checks external inputs: #events and forces bind checks
 *	USE: CHECKINPUTS
 *	INI: 
 *  NOTE: 
 ***|
sub check_inputs()
	DEBUG check_inputs()
	| set a non-use variable to force bind checks. better this then an echo
	/varset bindTrigger ${If[${bindTrigger},FALSE,TRUE]}
	
	| check the event queue
	/doevents
	
/return TRUE