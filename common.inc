|***
 *	common.inc
 *	Core shared Routines/Functions
 *
 *
 ***|




|***
 *	DES: Checks EQBCS connection and channel
 *	USE: /call check_one_mind
 *	INI: core.ini
 *  NOTE: 
 ***| 
sub check_one_mind()
	DEBUG ${cbug}check_one_mind()\ax
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
	DEBUG ${cbug}check_plugin()\ax
	/declare _count int local 0
	/declare _plug string local ${Ini[${INICore},plugin,plist]}

	/for _count 1 to ${_plug.Count[|]}
		/if (!${Bool[${Plugin[${_plug.Arg[${_count},|]}]}]})  {
			OUT Plugin \a-w${_plug.Arg[${_count},|]}\ax is missing. Loading it.
			/squelch /plugin ${_plug.Arg[${_count},|]}
		}
	/next _count	
/return



|***
 *	DES: echos misc stuff to EQBC
 *	USE: /call echos [type] "desc text" [VARIABLE] [Target ID] ["text2"]
 *	INI: 
 *  NOTE: 
 ***|
sub echos(string _type, string _verbage, string _tmpVariable, int _tmpID, string _verbage2)
	DEBUG ${cbug}echos(type ${_type}, verbage: ${_verbage}, tmp var: ${_tmpVariable})\ax
	AMIDEAD

	| on/off switch echos
	/if (${_type.Equal[switch]}) {
		VOUT ${_verbage} ${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}

	| control switches
	} else /if (${_type.Equal[setswitch]}) {
		VOUT ${_verbage} ${_verbage2} ${If[${Bool[${_tmpID}]},[\a-w${_tmpID}\ax] ,]}${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}


	| variable %s
	} else /if (${_type.Equal[number]}) {
		VOUT ${_verbage} ${sep} ${cinfo}${_tmpVariable}\ax

	| core startup
	} else /if (${_type.Equal[startup]}) {
		OUT Core Build ${sep} ${cinfo}${coreBuild}\ax

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

	| BC echo for switchechoBC
	} else /if (${_type.Equal[BC]}) {
		/bc ${_verbage} - [+r+]${_tmpVariable}[+x+]  ${If[${Bool[${_tmpID}]},- [+y+]${Spawn[${_tmpID}].DisplayName}[+x+],]} 

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

	| debug switches
	} else /if (${_type.Equal[debug]}) {
		VOUT DeBug ${sep} ${cinfo}${_verbage}\ax ${sep} ${If[${${_tmpVariable}},${oenabled},${odisabled}]}

	| petfarm
	} else /if (${_type.Equal[petfarm]}) {
		VOUT Pet ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax] ${sep} ${If[${Spawn[${Target2Kill}].LineOfSight},${cgood}${Spawn[${Target2Kill}].Distance}\ax,${cbad}${Spawn[${Target2Kill}].Distance}\ax]}

	| pull
	} else /if (${_type.Equal[pull]} && ${Target2Kill}) {
		VOUT Pull ${sep} [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax] ${sep} ${If[${Spawn[${Target}].LineOfSight},${cgood}${Int[${Target.Distance}]}\ax,${cbad}${Int[${Target.Distance}]}\ax]}

	| you suck
	} else /if (${_type.Equal[failboat]}) {
		/if (${smartasserrors}) {
			/call echo_bitch EQBC
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
	DEBUG ${cbug}check_engaged()\ax
	AMIDEAD
	/doevents
	/declare _isEngaged		bool local FALSE
	/if (${Me.Invis}) /varset _isEngaged TRUE
	/if (${coreAuto} && ${Select[${Me.Class.ShortName},ROG]} && ${switchSos}) /varset _isEngaged FALSE
	/if (${Bool[${Me.Casting}]} || ${Me.Casting.ID}) /varset _isEngaged TRUE
	/if (${Me.Moving} || ${Stick.Active}) /varset _isEngaged TRUE
	/if (${Me.Combat} || ${Me.AutoFire}) /varset _isEngaged TRUE
	/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${MobAgro} playerstate 4]} && !${switchBuffCombat}) /varset _isEngaged TRUE
	/if (((${switchPull} && ${switchAgro}) || ${Group.MainAssist.Name.Equal[${Me.DisplayName}]} || ${Group.MainTank.Name.Equal[${Me.DisplayName}]}) && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${CombatDistance}*2 playerstate 4]}) /varset _isEngaged TRUE
/return ${_isEngaged}



|***
 *	DES: give weapons to pet
 *	USE: /call give_to_pet
 *	INI: [Pet] - 
 *  NOTE: 
 ***|
sub give_to_pet()
	DEBUG ${cbug}give_to_pet()\ax Giving ${Cursor.ID} to ${Me.Pet.ID}
	AMIDEAD
	/doevents
	/if (${Target.ID} != ${Me.Pet.ID}) {
		/target ID ${Me.Pet.ID}
		/delay 1s ${Target.ID} == ${Me.Pet.ID}
	}
	:loopgiveitem
	AMIDEAD
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
	DEBUGCAST ${cbug}cast_summon_pet()\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${coreAuto} && ${switchPcPet} && ${Me.AltAbilityReady[Suspended Minion]}) {
		/call core_cast "Suspended Minion" alt 0 FALSE
		/delay 5s !${Me.Casting.ID}
		/delay 5
	}
	/if (${switchPcPet} && !${Me.Pet.ID})  {
		VOUT Building ${sep} ${csp}${PcPettype}\ax
		/call core_cast "${PcPettype}" ${buffGem} 0 FALSE
		/call create_timer timer_check_pet_buffs 20
		/delay 20s !${Me.Casting.ID}
		/pet hold on
		/pet ghold on
		/if (${switchPcPet} && ${Bool[${PetTank}]} && ${coreAuto}) { 
			/pet taunt on
		} else /if (${switchPcPet} && !${Bool[${PetTank}]} && ${coreAuto}) { 
			/pet taunt off
		}
		/call send_pet
	}
/return



|***
 *	DES: Shrinks pet.. wtf did you expect..
 *	USE: /call cast_shrink_pet
 *	INI: Pet - PetShrink
 *  NOTE: 
 ***|
sub cast_shrink_pet()
	DEBUGCAST ${cbug}cast_shrink_pet()\ax
	AMIDEAD
	CHECKEXIT
	/if (${check_engaged[]}) /return
	/if (${Me.Pet.ID} && ${Bool[${shrinkPet}]} && ${Me.Pet.Height} > 1.40) {
		/if (${Me.Book[${shrinkPet}]}) {
			/call core_cast "${shrinkPet}" ${buffGem} ${Me.Pet.ID} FALSE
		} else /if (${Me.ItemReady[${shrinkPet}]}) {
			/call core_cast "${shrinkPet}" item ${Me.Pet.ID} FALSE
		} else /if (${Me.AltAbilityReady[${shrinkPet}]}) {
			/call core_cast "${shrinkPet}" alt ${Me.Pet.ID} FALSE
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
	DEBUG ${cbug}check_pet()\ax
	AMIDEAD
	CHECKEXIT
	/if (!${Me.Pet.ID}) /call cast_summon_pet
	/if (${switchBuffPet} && !${timer_check_pet_buffs}) /call check_pet_buffs
	/if (${Me.Pet.ID} && ${useSummonWeps} && (!${Bool[${Me.Pet.Equipment[primary]}]} || !${Bool[${Me.Pet.Equipment[offhand]}]} || ${Me.Pet.Equipment[primary]} == 10855)) {
		/if (!${Select[${Me.Class.ShortName},MAG]} && !${Bool[${FindItem[${summonedWepClosedBag}]}]}) {
			/call tell_resident_mage weapons
		} else /if (${Select[${Me.Class.ShortName},MAG]} && !${Bool[${FindItem[${summonedWepClosedBag}]}]}) {
			/call cast_summon_weapons
		}
	}	

	AMIDEAD
	CHECKEXIT
	CHECKTIE
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
	DEBUG ${cbug}cast_summon_weapons()\ax
	AMIDEAD
	/doevents
	/declare _count int local 0
	
	/if (!${Me.Pet.ID}) /return
	/if (${Select[${Me.Class.ShortName},MAG]} && !${FindItem[${summonedWepClosedBag}].ID} && !${FindItem[${summonedWepName}].ID}) {
		DEBUG mage needs bag
		/call core_cast "${summonedWepSpell}" ${buffGem} ${Me.ID} FALSE
		/delay 20s !${Me.Casting.ID}
		/delay 5
		/call sort_cursor TRUE
	}
	/delay 5
	
	/if (${Bool[${FindItem[${summonedWepClosedBag}]}]}) {
		AMIDEAD
		DEBUG Found ${summonedWepClosedBag}
		DEBUG have a bag. best not in hands
		/while (${Cursor.ID}) {
			/autoinventory
			/delay 1s !${Cursor.ID}
		}
		DEBUG Exchange ${FindItem[${summonedWepClosedBag}].ID} pack${PetBagSlotNumber}
		/squelch /exchange ${FindItem[${summonedWepClosedBag}].ID} pack${PetBagSlotNumber}
		/call sort_cursor TRUE
		/delay 2s !${Cursor.ID}
		/delay 5
		/call core_cast "${summonedWepClosedBag}" item 0 FALSE
		DEBUG open the dumb bag
		/delay 3s !${Me.Casting.ID}
		/delay 5
		/delay 2s
		/call sort_cursor TRUE		
		/delay 5

		DEBUG Pouch of Quellious time
		/if (${Bool[${FindItem[Pouch of Quellious]}]}) {
			AMIDEAD
			DEBUG give the demanding pet his shit
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

			DEBUG destroy the bag
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
	DEBUG ${cbug}auto_adjust_EXP()\ax
	AMIDEAD
	CHECKEXIT
	/call create_timer timer_check_EXP ${tCheck_EXP}
	
	| set EXP cause we deaded too much
	/if (${Me.Level} <= ${MaxLevel} && ${Me.PctExp} <= ${MaintExpLvl}) {
		VOUT ${ocaution} ${sep} ${cinfo}${Me.PctExp}%\ax below Maintain EXP [${cinfo}${MaxLevel}\ax] ${sep} ${cinfo}${MaintExpLvl}%\ax
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
	DEBUG ${cbug}check_aura()\ax 
	/call create_timer timer_check_aura ${tCheck_Aura}
	/if (${coreAuto} && !${switchBuffCombat} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]}) /return
	AMIDEAD
	CHECKEXIT
	
	UPDATEHUD "CHECK: Aura"
	
	| Aura: berzerker, monk
	/if (${validate_class[FALSE, |BER|MNK]}) {
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
		/if (${coreAuto} && !${switchBuffCombat} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]}) /continue
		/if (${Bool[${Aura${_aura}Spell}]}) {
			/for _count 1 to 2	
				/if (${coreAuto} && !${switchBuffCombat} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]}) /continue
				/if (${Bool[${Aura${_aura}Buff}]}) {
					/if (${Bool[${Me.Aura[${_count}].Name.Equal[${Aura${_aura}Buff}]}]}) /varset _found TRUE
					/if (${Bool[${Me.Aura[${_count}].Name.Equal[${Aura${_aura}Spell}]}]}) /varset _found TRUE
					/if (${Bool[${Me.Aura[${_count}].Name.Equal[${Ini[${INIClass},Aura,Aura${_aura}Spell]}]}]}) /varset _found TRUE
					/if (${Bool[${Me.Aura[${_count}].Name.Equal[${Ini[${INIClass},Aura,Aura${_aura}Buff]}]}]}) /varset _found TRUE
				}
			/next _count
			/if (!${_found}) {
				/call core_cast "${Aura${_aura}Spell}" ${buffGem} ${Me.ID} FALSE
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
	DEBUG ${cbug}intialize_item_buffs()\ax
	AMIDEAD
	/doevents
	| VOUT Initializing Item Click Buffs
	/if (!${Defined[ClickItem]}) /declare ClickItem												int			outer 1
	/if (!${Defined[InvItem]}) /declare InvItem              							int			local 0
	/if (!${Defined[ContainerItem]}) /declare ContainerItem								int			local 0
	/if (!${Defined[UseItem]}) /declare UseItem[40]                  			string	outer 
	/if (!${Defined[SelfItem]}) /declare SelfItem[${UseItem.Size}]   			string 	outer 
	/if (!${Defined[SpellItem]}) /declare SpellItem[${UseItem.Size}]   		string	outer 
	/if (!${Defined[tmpClickName]}) /declare tmpClickName									string	outer ${Ini[${INIClass},ClickItems]}
	/if (!${Defined[tmpFoundSpell]}) /declare tmpFoundSpell								bool		local FALSE

	/for InvItem 1 to 32
		AMIDEAD
		/if (${Bool[${InvSlot[${InvItem}].Item.Container}]}) {
			/for ContainerItem 1 to ${InvSlot[${InvItem}].Item.Container}
				AMIDEAD
				/if (${Me.Inventory[${InvItem}].Item[${ContainerItem}].ID} && ${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell.ID} && ${Me.Inventory[${InvItem}].Item[${ContainerItem}].EffectType.Equal[Click Inventory]} && ${Me.Level}>=${Me.Inventory[${InvItem}].Item[${ContainerItem}].RequiredLevel}) {
					DEBUG Finding ${Me.Inventory[${InvItem}].Item[${ContainerItem}].Name}: -- [${If[${tmpClickName.Find[${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell}]},TRUE,FALSE]}]
					/if (${Bool[${tmpClickName.Find[${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell}]}]} && ${Bool[${Ini[${INIClass},ClickItems,${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell},TRUE]}]}) { 
					  /varset UseItem[${ClickItem}] TRUE
					  /varset SelfItem[${ClickItem}] ${Me.Inventory[${InvItem}].Item[${ContainerItem}].Name}
					  /varset SpellItem[${ClickItem}] ${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell}		  
					  /varcalc ClickItem ${ClickItem}+1
					  DEBUG Setting "${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell}" clickitem:${ClickItem}
					} else /if (!${tmpClickName.Find[${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell}]}) {
					 	/ini "${INIClass}" "ClickItems" "${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell}" "FALSE"
						DEBUG Creating "ClickItems" "${Me.Inventory[${InvItem}].Item[${ContainerItem}].Spell}"
					}
				}
			/next ContainerItem
		} else /if (!${Bool[${InvSlot[${InvItem}].Item.Container}]} && ${InvSlot[${InvItem}].Item.ID} && ${InvSlot[${InvItem}].Item.Spell.ID} && ${InvSlot[${InvItem}].Item.EffectType.Equal[Click Inventory]} && ${Me.Level}>=${InvSlot[${InvItem}].Item.RequiredLevel}) {
			DEBUG Finding ${Me.Inventory[${InvItem}].Name}: ${tmpClickName.Find[${Me.Inventory[${InvItem}].Spell.Name}]} -- [${If[${tmpClickName.Find[${Me.Inventory[${InvItem}].Spell}]},TRUE,FALSE]}]
			AMIDEAD
			/if (${Bool[${tmpClickName.Find[${Me.Inventory[${InvItem}].Spell}]}]} && ${Bool[${Ini[${INIClass},ClickItems,${Me.Inventory[${InvItem}].Spell},TRUE]}]}) { 
				/varset UseItem[${ClickItem}] TRUE		
				/varset SelfItem[${ClickItem}] ${InvSlot[${InvItem}].Item}
				/varset SpellItem[${ClickItem}] ${InvSlot[${InvItem}].Item.Spell}
				/varcalc ClickItem ${ClickItem}+1
			  DEBUG Setting "${Me.Inventory[${InvItem}].Spell}" clickitem:${ClickItem}
			} else /if (!${tmpClickName.Find[${Me.Inventory[${InvItem}].Spell}]}) {
				/ini "${INIClass}" "ClickItems" "${InvSlot[${InvItem}].Item.Spell}" "FALSE"
				DEBUG Creating "ClickItems" "${Me.Inventory[${InvItem}].Spell}"
			}
		}
	/next InvItem
/return



|***
 *	DES: Checks current buffs. if one is missing. fix it.
 *	USE: /ib and auto
 *	INI: 
 *  NOTE: 
 ***|
sub check_buffs_item()
	DEBUG ${cbug}check_buffs_items()\ax
	AMIDEAD
	CHECKEXIT
	UPDATEHUD "BUFF: Items"
	/declare _count int	local 0
	
	/if (${Me.State.NotEqual[Bind]} && !${Me.Moving}) {
		/for _count 1 to ${ClickItem}
			/doevents
			AMIDEAD
			/if (${UseItem[${_count}]}&& ${Me.FreeBuffSlots} >= 1 && ${FindItem[${SelfItem[${_count}]}].ID} && ${Cast.Ready[${SelfItem[${_count}]}]} && !${Me.Buff[${SpellItem[${_count}]}].ID} && ${Spell[${SpellItem[${_count}]}].Stacks}) { 
				/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop
				/call core_cast "${SelfItem[${_count}]}" item 0 FALSE
				/delay 5s !${Me.ItemReady[${SelfItem[${_count}]}]}
				/delay 10s !${Me.Casting.ID}
	  	}
  	/next _count
  }

	| CoTF progression illusion
  | /if (${Me.ItemReady["Gunthak Swabby's Eyepatch"]} && !${Bool[${Me.Buff[Gunthak]}]} && ${Spell[Gunthak Grit].Stacks}) /call core_cast "Gunthak Swabby's Eyepatch" item

 	AMIDEAD
	CHECKEXIT
  /if (${Me.ItemReady[${IllusionItem}]} && (!${Bool[${Me.Buff[${IllusionBuff}]}]}) && (${Spell[${IllusionBuff}].Stacks})) {
		VOUT Item ${sep} ${citem}${IllusionItem}\ax
 		/call MQ2Cast "${IllusionItem}" item 30s
 	}  	

	/if (!${Bool[${timer_Check_Familiar}]}) /call sort_familiar
 	
	/if (${Bool[${BuffRemove}]}) /call remove_unwanted_buffs	
	/if (${coreAuto} && ${switchShrink}) /call check_shrink
	/if (${coreAuto}) /call create_timer timer_check_buffs_item ${tItem_Buff}
	UPDATEHUD FALSE
/return



|***
 *	DES: Checks current buffs. NOT SONGS. IF one is found to match the INI listing per character, it is removed.
 *	USE: /call remove_unwanted_buffs
 *	INI: toon -> [SelfBuff] -> BuffRemove
 *  NOTE: 
***|
sub remove_unwanted_buffs()
	DEBUG ${cbug}remove_unwanted_buffs()${cbug}
	/declare _count int local 0
	/declare _remove string local ${BuffRemove}
	/for _count 1 to ${_remove.Count[|]}
		/doevents
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
	DEBUG ${cbug}check_buffs_self(switch:${switchBuffSelf}, timer: ${timer_check_buffs_self})\ax
	/if (${coreAuto} && !${timer_check_buffs_self}) /call create_timer timer_check_buffs_self ${tSelf_Buff}
	/if (${coreAuto} && !${switchBuffCombat} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]}) /return
	/if (${coreAuto} && ${watch_for_death[]}) /return 
| 	/if ((${watch_for_death[]} || ${flagCallNeedRez})) /return 
	/declare _count int local 0

	| check Unity AA buffs
	/if (${Bool[${${Me.Class.ShortName}AAUnity}]}) {
		DEBUG AA UNITY: ${${Me.Class.ShortName}AAUnity}
		/for _count 1 to 6
			/if (${coreAuto} && !${switchBuffCombat} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]}) /return
			UPDATEHUD "BUFF: AA Unity"
			AMIDEAD
			CHECKEXIT
			CHECKTIE

			/if (!${Me.AltAbilityReady[${${Me.Class.ShortName}AAUnity}]}) /break
			/if (${Me.CurrentMana} < ${Spell[${${Me.Class.ShortName}AAUnity}].Mana}) /break
			/if (!${Bool[${BuffAAUnity${_count}}]}) /continue
			/if (!${Spell[${BuffAAUnity${_count}}].Stacks}) /continue
			DEBUG _count: ${_count} - ${BuffAAUnity${_count}} = ${Me.Buff[${BuffAAUnity${_count}}].Duration}
			/if (${Bool[${BuffAAUnity${_count}}]} && !${Bool[${Me.Buff[${BuffAAUnity${_count}}].Duration}]}) {
				/call core_cast "${${Me.Class.ShortName}AAUnity}" alt 0 FALSE
				/break
			}
		/next _count
	}
		
	| check buffs
	/for _count 1 to ${BuffCount}
		/if (${coreAuto} && !${switchBuffCombat} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]}) /return
		UPDATEHUD "BUFF: Self"
		AMIDEAD
		CHECKEXIT
		CHECKTIE
		
		| check for non gold subs
		/if (!${Select[${Me.Subscription},GOLD]}) /varset Buff${_count}chk ${Ini[${INIClass},buffs,Buff${_count}chk]}
		
		/if (!${Bool[${Me.Buff[${Buff${_count}chk}].Duration}]} && ${Spell[${Buff${_count}}].Stacks} && ${Bool[${Buff${_count}self}]} && ${Bool[${Buff${_count}}]}) {
			/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop
			/if (!${Me.SpellReady[${Buff${_count}}]} && ${Cursor.ID}) CLEARCURSOR TRUE
 		  /if (${Me.Book[${Buff${_count}}]} && ${Me.CurrentMana} > ${Spell[${Buff${_count}}].Mana}) /call core_cast "${Buff${_count}}" ${buffGem} ${Me.ID} FALSE
			/if (${Me.AltAbility[${Buff${_count}}].ID}) /call core_cast "${Buff${_count}}" alt 0 FALSE
			/if (${Me.ItemReady[${Buff${_count}}]}) /call core_cast "${Buff${_count}}" item ${Me.ID} FALSE
		}
	/next _count
	/if (${Me.Class.ShortName.Equal[WIZ]}) /call sort_mancy_lineAA
	/if (!${Bool[${timer_Check_Familiar}]}) /call sort_familiar
	/if (${coreAuto} && ${switchShrink}) /call check_shrink
	/if (${Bool[${BuffRemove}]}) /call remove_unwanted_buffs	
	/if ((${Bool[${Aura1Spell}]} || ${Bool[${Aura2Spell}]}) && !${timer_check_aura}) /call check_aura
	/if (${Target.ID}) /squelch /target clear
	UPDATEHUD FALSE	
/return



|***
 *	DES: Checks pets buffs. If somethings missing, mem and cast it.
 *	USE: /pb and/or auto
 *	INI: 
 *  NOTE: this will do AA or Spell
 ***|
sub check_pet_buffs()
	DEBUG ${cbug}check_pet_buffs()\ax
	/if (!${Me.Pet.ID}) /return
	/if (${coreAuto} && (!${switchBuffCombat} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]})) /return
	/if (${coreAuto} || ${switchModePetfarm} || ${switchModeHunt}) /call create_timer timer_check_pet_buffs ${tPet_Buff}

	/declare _count int local 0
	/declare _spellType string local
	
	/for _count 1 to ${PetBuffCount}
		AMIDEAD
		CHECKEXIT
		CHECKTIE
		/if (${check_engaged[]}) /return
		/if (${coreAuto} && !${switchBuffCombat} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]}) /return
		UPDATEHUD "BUFF: Pet"
		/varset _spellType FALSE

		/if (!${Me.Pet.Buff[${PetBuff${_count}chk}]} && ${Me.CurrentMana} > ${Spell[${PetBuff${_count}}].Mana}) /varset _spellType ${buffGem}
		/if (!${Me.Pet.Buff[${PetBuff${_count}chk}]} && ${Me.AltAbilityReady[${PetBuff${_count}}]}) /varset _spellType alt
			
		/if (${Bool[${_spellType}]} && !${Me.Moving} ) {
			CLEARCURSOR TRUE
			/call core_cast "${PetBuff${_count}}" ${_spellType} ${Me.Pet.ID} FALSE
		}
	/next _count
	
	| chekck for pet illusion
	/if (${Bool[${PetIllusion}]} && !${Bool[${Me.Pet.Buff[${PetIllusionBuff}]}]} && ${Me.Pet.Distance} < 100 && ${Spell[${PetIllusionBuff}].Stacks}) /call MQ2Cast "${PetIllusion}" item -targetid|${Me.Pet.ID}
	| shrink the little bastards..
	/if (${Bool[${Me.Pet.ID}]}) /call cast_shrink_pet
	| pet epic click
	/if (${Me.ItemReady[${EpicClick}]} && !${Me.Pet.Buff[Elemental Conjunction]} && !${Me.Pet.Buff[${EpicPetBuff}]} && !${Me.Pet.Buff[Blessing of Unity]} && ${Spell[${EpicPetBuff}].Stacks}) /call core_cast "${EpicClick}" item 0 FALSE
	
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
	DEBUG ${cbug}check_no_cast_buffs()\ax
	AMIDEAD
	/doevents
	/declare _count int local 0
	/varset canCast TRUE
	/for _count 1 to ${noCastWith.Count[|]}
		AMIDEAD
		/doevents
		/if (${Bool[${Me.Buff[${noCastWith.Arg[${_count},|]}]}]} || ${Bool[${Target.Buff[${noCastWith.Arg[${_count},|]}]}]} || ${Bool[${Me.Song[${noCastWith.Arg[${_count},|]}]}]}) /varset canCast FALSE
	/next _count	
/return ${canCast}



|***
 *	DES: checks your toon and target for buffs that should prevent a thing
 *	USE: /call check_no_do_a_thing_buff
 *	INI: 
 *  NOTE: we assume return FALSE, as in no buffs found. if we find a buff, return TRUE
 ***|
sub check_no_do_a_thing_buff(string _type)
	DEBUG ${cbug}check_no_do_a_thing_buff(type: ${_type})\ax
	/if (!${Bool[${no${_type.Left[1].Upper}${_type.Right[-1].Lower}With}]}) /return FALSE
	/if (!${Bool[${_type}]}) /return FALSE
	/declare _count int local 0

	/for _count 1 to ${no${_type.Left[1].Upper}${_type.Right[-1].Lower}With.Count[|]}
		AMIDEAD
		/doevents

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
	DEBUG ${cbug}click_horn_of_unity()\ax
	/if (!${Bool[${switchHorn}]} || ${SafeZone} && !${switchOverrideSafezone} || ${Bool[${timer_zone_pause}]}) /return
	/if (${Me.ItemReady[Miniature Horn of Unity]} && !${Bool[${Me.Song[Blessing of Unity]}]} && (${Me.PctHPs} < 95*.${tmpHealOverride} || ${Me.PctMana} < 95*.${tmpHealOverride})) {
		/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop
		/call core_cast "Miniature Horn of Unity" item 0 FALSE
	}
/return



|***
 *	DES: EQBC Buff Routines
 *	USE: automatic for anyone sharing your EQBC
 *	INI: 
 *  NOTE: 
  ***|
sub check_buffs_BC()
	DEBUG ${cbug}check_buffs_BC()\ax
	/if (${coreAuto} && (!${switchBuffCombat} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]})) /return
	/if (${coreAuto}) /call create_timer timer_check_buffs_BC ${tBC_Buff}
	UPDATEHUD "BUFF: BC"	

	/declare _toon int local 1
	/declare _buff int local 1

	| each toon
	DEBUG (${_toon} <= ${Int[${Math.Calc[${NetBots.Client.Count[ ]}+1]}]})
	/while (${_toon} <= ${Int[${Math.Calc[${NetBots.Client.Count[ ]}+1]}]}) {
		| healers ignore the rampage tank
		/if (${validate_class[FALSE, |CLR|PAL|DRU|SHM]}) {
			/if (${Bool[${RampageTank}]} && ${RampageTank.Equal[${NetBots[${NetBots.Client[${_toon}]}]}]}) /continue
		}
		DEBUG _toon ${_toon} / ${Int[${Math.Calc[${NetBots.Client.Count[ ]}+1]}]}
		AMIDEAD
		CHECKEXIT

		| each buff. cycle all ${BuffCount}, used or not
		/for _buff 1 to ${BuffCount}
			DEBUG _buff: ${_buff}
			/if (${coreAuto} && !${switchBuffCombat} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]}) /break
			
			| make sure the count is in the correct range
			/if (!${Range.Between[1,${BuffCount}:${Int[${_buff}]}]}) /continue
			/doevents
			AMIDEAD

			| no BC Buff
			/if (!${Bool[${Buff${_buff}}]}) /continue
	
			| no class to buff?
			/if (!${Bool[${BCBuff${_buff}Class}]}) /continue

			| no use this BC buff in raids
			/if (${Raid.Members} && ${Buff${_buff}noBCinRaid}) /continue
			DEBUG ${_buff} / ${BuffCount} raid - ${Raid.Members} && ${Buff${_buff}noBCinRaid}

			| buffee is dead?			
			/if (!${Bool[${Spawn[pc ${NetBots.Client[${_toon}]}].ID}]} || ${Spawn[pc ${NetBots.Client[${_toon}]}].Dead}) /continue
			
			| targets class not listed
			/if (!${BCBuff${_buff}Class.Find[${Spawn[pc ${NetBots.Client[${_toon}]}].Class.ShortName}]}) /continue
			DEBUG ${_buff} / ${BuffCount} class - ${Bool[${BCBuff${_buff}Class}]}

			| bufeee out of range
			/if (${Spawn[pc ${NetBots.Client[${_toon}]}].Distance} > ${Spell[${Buff${_buff}}].MyRange}) /continue

			| is the spell ready?
			/if (!${Me.SpellReady[${Buff${_buff}}]} && ${Me.Gem[${Buff${_buff}}]}) /continue
			DEBUG ${_buff} / ${BuffCount} ready -${Me.SpellReady[${Buff${_buff}}]} && ${Me.Gem[${Buff${_buff}}]}

			| spell stacks ?
			/if (!${NetBots[${NetBots.Client[${_toon}]}].Stacks[${Spell[${Buff${_buff}chk}].ID}]}) /continue
			DEBUG ${_buff} / ${BuffCount} stack - ${NetBots[${NetBots.Client[${_toon}]}].Stacks[${Spell[${Buff${_buff}chk}].ID}]}

			/if (${Me.CurrentMana} > ${Spell[${Buff${_buff}}].Mana}) { 
				| bards STFU
				/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop
  		  VSPELL BC Buff ${sep} ${csp}${Buff${_buff}}\ax ${sep} [${cpc}${NetBots.Client[${_toon}]}\ax]
				/call MQ2Cast "${Buff${_buff}}" ${buffGem} 10s -targetid|${NetBots[${NetBots.Client[${_toon}]}].ID} 	
			}
		/next _buff
		/varcalc _toon ${_toon}+1
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
	DEBUG ${cbug}check_ammo()\ax
	AMIDEAD
	CHECKEXIT
	/call create_timer timer_Check_Ammo ${tCheck_Ammo}
	/if (${Cursor.Name.Equal[${RangeItemSummon}]}) {
		/call sort_cursor TRUE
		/return
	}	
	
	/declare _count int local 0
	
	/for _count 1 to ${RangeItem.Count[|]}
		/if (!${Bool[${RangeItem.Arg[${_count},|]}]} || !${Bool[${RangeItemSummon.Arg[${_count},|]}]}) /continue

		/if (!${Bool[${FindItemCount[${RangeItemSummon.Arg[${_count},|]}]}]}) {
			/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop
			/if (${Me.Book[${RangeItem.Arg[${_count},|]}]}) /call core_cast "${RangeItem.Arg[${_count},|]}" ${buffGem} ${Me.ID} FALSE
			/if (${Me.ItemReady[${RangeItem.Arg[${_count},|]}]}) /call core_cast "${RangeItem.Arg[${_count},|]}" item ${Me.ID} FALSE
			/if (${Me.AltAbilityReady[${RangeItem.Arg[${_count},|]}]}) /call core_cast "${RangeItem.Arg[${_count},|]}" alt ${Me.ID} FALSE
			/if (${Me.CombatAbilityReady[${RangeItem.Arg[${_count},|]}]} || ${Select[${Me.Class.ShortName},BER]}) /call core_cast "${RangeItem.Arg[${_count},|]}" disc ${Me.ID} FALSE
			/delay 10s !${Me.Casting.ID}
			/call sort_cursor TRUE
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
	DEBUGCAST ${cbug}check_shrink()\ax
	AMIDEAD
	CHECKEXIT
	/if (${Me.Height} > 2.04 && ${Me.ItemReady[${shrinkItem}]}) {
		/if (${Me.Book[${shrinkItem}]}) /call core_cast "${shrinkItem}" ${buffGem} ${Me.ID} FALSE
		/if (${Me.ItemReady[${shrinkItem}]}) /call core_cast "${shrinkItem}" item ${Me.ID} FALSE
	}
/return



|***
 *	DES: create misc timer
 *	USE: /call create_timer [TIMER VARIABLE NAME] [DURATION]
 *	INI: [Timer]
 *  NOTE: this will define and populate if the variable does not exist. if it exists, it will adjust the time acordingly
 ***|
sub create_timer(string _my_timer, string _my_duration)
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
	DEBUG ${cbug}do_Rest(\ax${cinfo}manna:${Me.PctMana}% endurance:${Me.PctEndurance}%\ax${cbug})\ax
	/if (!${coreAuto} || ${switchAmIDead}) /return
	
	| clerics and pallys wait 'till yaulp gone
	/if (${Bool[${Me.Buff[Lesser Yaulp]}]} || ${Bool[${Me.Buff[Yaulp]}]}) /return
	
	AMIDEAD

	/if (${HomeSet} && !${SpawnCount[npc radius ${CombatDistance}*2 playerstate 4 targetable]} && ((${Me.Y} != ${MakeCamp.AnchorY}) && (${Me.X} != ${MakeCamp.AnchorX}))) {
		/if (${switchPull}) /squelch /makecamp unpause
		/squelch /makecamp return
		/delay 3s ((${Me.Y} == ${MakeCamp.AnchorY}) && (${Me.X} == ${MakeCamp.AnchorX}))
	}

	/declare _needrest bool local FALSE
	
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
	/doevents
	| check self click buffs
	/if (!${Me.Invis}) {
		/if (${switchBuffItem} && !${check_engaged[]} && !${timer_check_buffs_item}) /call check_buffs_item
		/if (${switchBuffTell} && !${timer_check_buffs_tell}) /call check_buffs_tell
	}

	/if (${coreAuto} && (${Bool[${Group.Members}]} && ${Group.MasterLooter.ID}==${Me.ID}) && ${Me.UseAdvancedLooting} && !${AdvLoot.LootInProgress} && !${Stick.Active}) /call do_loot
	
	/if (${Bool[${autoRest}]} && !${Me.Casting.ID} && !${Me.Moving} && !${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${MobAgro} playerstate 4]}) {
			
		| Hybrid: RNG,SHD,BST,PAL
		/if ((${MeleeList.Find[${Me.Class.ShortName}]} && ${CastList.Find[${Me.Class.ShortName}]}) || ${Me.Class.ShortName.Equal[BRD]}) {
			DEBUG REST RNG,SHD,BST,PAL,BRD
			/if (${Me.PctEndurance} < ${autoRest}*.${tmpHealOverride}) /varset _needrest TRUE
			/if (${Me.PctMana} < ${autoRest}*.${tmpHealOverride}) /varset _needrest TRUE
			/if (${Me.Class.ShortName.Equal[BRD]}) {
				
				DEBUG Rest Bard
				/call cast_AA_Rallying
					/if (${switchRestSong}) {
						/if (!${Bool[${Me.Song[${spellRestSong}]}]}) {
						/if (${Me.Casting.ID} != ${Spell[${spellRestSong}].ID}) {
							DEBUG song rest
							/call mem_spell FALSE "${spellRestSong}" ${buffGem} TRUE
							/melody ${Me.Gem[${spellRestSong}]}
							/delay 3s !${Me.Casting.ID}
						}
					}
				}
				
			} else {
				/call use_mod_rod
				/call cast_disc_breather
			}
			/if (${Select[${Me.Class.ShortName},PAL]}) {
			} else /if (${Select[${Me.Class.ShortName},SHD]}) {
				/if (${switchPcPet}) /call check_pet
			} else /if (${Select[${Me.Class.ShortName},RNG]}) {
			} else /if (${Select[${Me.Class.ShortName},BST]}) {
			} 

		| Melee: WAR,MNK,ROG,BER
		} else /if (${MeleeList.Find[${Me.Class.ShortName}]} && !${CastList.Find[${Me.Class.ShortName}]}) {
			DEBUG REST WAR,MNK,ROG,BER
			/if (${Me.PctEndurance} < ${autoRest}*.${tmpHealOverride}) /varset _needrest TRUE
			/call cast_disc_breather
			/if (${Select[${Me.Class.ShortName},WAR]}) {
			} else /if (${Select[${Me.Class.ShortName},MNK]}) {
			} else /if (${Select[${Me.Class.ShortName},ROG]}) {
				/if (${coreAuto} && ${switchSos}) /call automaticSoS
			} else /if (${Select[${Me.Class.ShortName},BER]}) {
			}
			
		| Caster: CLR,DRU,SHM,ENC,WIZ,NEC,MAG
		} else /if (!${MeleeList.Find[${Me.Class.ShortName}]} && ${CastList.Find[${Me.Class.ShortName}]}) {
			DEBUG REST CLR,DRU,SHM,ENC,WIZ,NEC,MAG
			/if (${Me.PctMana} < ${autoRest}*.${tmpHealOverride}) {
				/varset _needrest TRUE
				/call use_mod_rod
			}

			/if (${Select[${Me.Class.ShortName},CLR]}) {
				/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
			} else /if (${Select[${Me.Class.ShortName},DRU]}) {
				/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
			} else /if (${Select[${Me.Class.ShortName},SHM]}) {
				/if (${switchHealGroup}) /call check_heal_group_${Me.Class.ShortName}
				/if (${Me.PctMana} < 60*.${tmpHealOverride} && !${flagCallNeedRez}) /call check_cani
			} else /if (${Select[${Me.Class.ShortName},ENC]}) {
			} else /if (${Select[${Me.Class.ShortName},WIZ]}) {
				/call sort_mancy_lineAA
			} else /if (${Select[${Me.Class.ShortName},NEC]}) {
				/if (${switchPcPet}) /call check_pet
			} else /if (${Select[${Me.Class.ShortName},MAG]}) {
				/if (${switchPcPet}) /call check_pet
				/call Bind_setMAG mana
			}
		} 			
	}
		
	/if (${_needrest}) {
		DEBUG RESTING ${Me.PctMana} / ${Me.PctEndurance}
		UPDATEHUD "REST"
		/if (${Me.State.NotEqual[SIT]} && !${Bool[${Me.Mount.ID}]}) /sit
		/delay 1
		CHECKTIE
		AMIDEAD
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
	DEBUG ${cbug}set_home(\ax${HomeSet}${cbug})\ax
	/if (${HomeSet}) {
		/varset SetTie FALSE
		VOUT Radius: ${cinfo}${HomeRadius}\ax ${sep} Leash: ${cinfo}${HomeLeash}\ax
		VOUT ${cgood}Camp\ax established. Y: ${cinfo}${Me.Y}\ax ${sep} X: ${cinfo}${Me.X}\ax 
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
 *	USE: /stop or /alto or /squelch /bct ${Me} stop or /call set_stop [TRUE]
 *	INI: 
 *  NOTE: just because you stop something, does not mean you will not start it right back up again.
 ****|
sub set_stop(bool _noecho)
	DEBUG ${cbug}set_stop()\ax
	UPDATEHUD "STOPPING"
	/if (${Defined[combat]}) /varset combat ${If[${coreAuto},${Ini[${INIClass},Common,combat]},FALSE]}
	/if (!${Bool[${_noecho}]}) VOUT I was told to ${cbad}STOP\ax !! 
	/if (${Me.Casting.ID}) /stopcast
	| stop sticking to stuff
	/if (${Stick.Active}) /squelch /stick off
	/if (${Bool[${tempsnapstuck}]}) /varset tempsnapstuck FALSE
	/if (${Bool[${SetTie}]}) /varset SetTie FALSE
	
	| stop advanced path
	/if (${AdvPath.Following}) /squelch /afollow off

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
 *	USE: /call click_item [TRUE|FALSE] *if you want epic clicked (self only)
 *	INI: [Items] -> doclickItem
 *  NOTE: 
 ***|
sub click_item(bool _clickEpic)
	DEBUGCAST ${cbug}click_item()\ax
	| /if (${_clickEpic} && ${Me.ItemReady[${Epic}]} && ${Spell[${FindItem[${Epic}].Spell}].Stacks} && !${Me.Buff[${FindItem[${Epic}].Spell}].ID} && !${Me.Song[${FindItem[${Epic}].Spell}].ID} && ${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${MobAgro} playerstate 4 targetable]} >= 2) /call core_cast "${Epic}" item 0 FALSE

	/declare _count int local 0
	/for _count 1 to 6
		DEBUG _count:${_count} item:${clickItem${_count}} ready:${Me.ItemReady[${clickItem${_count}}]} 
		/if (!${Bool[${clickItem${_count}}]}) /continue
		/doevents
		AMIDEAD
		CHECKTIE
		/if (${Spawn[${Target.ID}].PctHPs} < 5*.${tmpHealOverride}) /break
		/if (${is_target_dead[${Target2Kill}]}) /break
		/if (${Me.ItemReady[${clickItem${_count}}]}) /call core_cast "${clickItem${_count}}" item 0 FALSE

	/next _count
/return



|***
 *	DES: how to handle cursor items
 *	USE: /call sort_cursor || CLEARCURSOR
 *	INI: core.loot.ini
 *  NOTE: 
 ***|
sub sort_cursor(bool _ignoreTimer)
	DEBUG ${cbug}sort_cursor(Ignore Timer: ${Bool[${_ignoreTimer}]})\ax
	/if (${timer_clear_cursor}) /return
	AMIDEAD
	/declare _cursorChoice string local 

	| we have faith in the all mighty AutoInventoryItem=
	/if (${Bool[${autoInventoryItem.Find[${Cursor.Name}]}]}) /autoinventory 
	/if (!${Bool[${Cursor.ID}]}) /return

	/if (!${Bool[${_ignoreTimer}]} && ${Bool[${autoInventoryItem.Find[${Cursor.Name}]}]}) {
		/if (${switchAutoCursor} && ${timer_clear_cursor}) /return
		/if (!${switchAutoCursor} && !${Bool[${autoInventoryItem.Find[${Cursor.Name}]}]}) /return
		/if (!${Bool[${Cursor.ID}]}) /return
	}
	
	| allow for absence of override for not in same zone as master looter
	/if (!${switchAutoCursor} && !${switchOverrideLoot}) /return
	
	
	/while (${Cursor.ID}) {
		DEBUG /while item on cursor
		/if (!${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}].Length}) /ini "${INILoot}" "${Cursor.Name.Left[1]}" "${Cursor.Name}" "Keep"
		/if (${Cursor.ID}) /varset _cursorChoice ${Ini[${INILoot},"${Cursor.Name.Left[1]}","${Cursor.Name}"]}
		/if (${Bool[${autoInventoryItem.Find[${Cursor.Name}]}]} || ${Select[${_cursorChoice},Keep,Sell,Announce]}) {
			/if (${Select[${_cursorChoice},Announce]}) ECHOCHANNEL - Cursor .. ${Cursor.Name}
			/if (${Cursor.ID}) VOUT Cursor ${sep} ${okeep} ${sep} ${cinfo}${Cursor.Name}\ax ${sep} (${cnum}${FindItemCount[${Cursor.Name}]}\ax)
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
 *  NOTE: for healers its /healxtarget
 ***|
sub set_xtarget()
	DEBUG ${cbug}set_xtarget()\ax 
	/if (!${validate_expansion[FALSE, |16]}) /return	
	/if (!${Select[${Me.Class.ShortName},SHM,CLR,PAL,DRU]}) {
		/call echo_bitch EQBC
		/if (${Defined[switchHealXtarget]}) /call writeINI switchHealXtarget Heal FALSE 
		/return
	}
	

	/call create_timer timer_set_xtarget ${tXHeal}

	VOUT Sorting XTargets ${sep} ${cinfo}${tXHeal}\ax
	/declare _x 				int 		local
	/declare _b 				int 		local
	/declare _count 		int			local 0
	
	| reset extended target slots	
	/for _x 1 to 13
		/if (${Bool[${toonControl}]} && ${Bool[${toonControlXTslot}]} == ${_x} && ${toonControlProtect}) /continue
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
	/if (!${Raid.Members} && ${Bool[${ProtectOutsideGroup}]}) {
		DEBUG Setting Protected Out of Group Players ${cinfo}${ProtectOutsideGroup}\ax
		/for _x 1 to ${ProtectOutsideGroup.Count[|]}
			DEBUG x:${_x} b:${_b} count:${ProtectOutsideGroup.Count[|]} spawn:${ProtectOutsideGroup.Arg[${_x},|]}
			/if (${Spawn[${ProtectOutsideGroup.Arg[${_x},|]}].Name.Equal[${toonControl}]}) /continue
			/if (${Spawn[${ProtectOutsideGroup.Arg[${_x},|]}].Distance} > ${XTHealRadius}) /continue
			/if (${_b} > 13 || !${Bool[${Spawn[${ProtectOutsideGroup.Arg[${_x},|]}]}]} || ${Group.Member[${ProtectOutsideGroup.Arg[${_x},|]}].ID}) /continue
			/if (${Bool[${Me.XTarget[${ProtectOutsideGroup.Arg[${_x},|]}]}]}) {
				/varcalc _b ${_b}+1
			} else /if (${Bool[${Spawn[${ProtectOutsideGroup.Arg[${_x},|]}]}]}) {
				/varcalc _b ${_b}+1
				/xtarget set ${_b} ${Spawn[${ProtectOutsideGroup.Arg[${_x},|]}].Name}
			}
		/next _x
	}				

	DEBUG Setting Raid Protected Classes ${cinfo}${XHealClass}\ax
	| set raid member classes to protect
	/if (${Raid.Members}) {
		/if (${Bool[${ProtectInsideRaid}]}) {
			DEBUG Setting Raid Protected Players ${cinfo}${ProtectInsideRaid}\ax
			/for _x 1 to ${ProtectInsideRaid.Count[|]}
				/if (${Spawn[${ProtectInsideRaid.Arg[${_x},|]}].Name.Equal[${toonControl}]}) /continue
				/if (${Spawn[${ProtectInsideRaid.Arg[${_x},|]}].Distance} > ${XTHealRadius}) /continue
				/if (${_b} > 13 || !${Bool[${Spawn[${ProtectInsideRaid.Arg[${_x},|]}]}]}) {
				} else /if (${Bool[${Me.XTarget[${ProtectInsideRaid.Arg[${_x},|]}]}]}) {
					/varcalc _b ${_b}+1
				} else /if (${Bool[${Spawn[${ProtectInsideRaid.Arg[${_x},|]}]}]}) {
					/varcalc _b ${_b}+1
					/xtarget set ${_b} ${Spawn[${ProtectInsideRaid.Arg[${_x},|]}].Name}
				}
			/next _x
		}
		|***
		| set raid protected classes radius ${XTHealRadius}
		/for _count 1 to ${_class.Count[|]}
			DEBUG _count: ${_count}
			/declare _${AllClassList.Arg[${_count},|]} int local ${SpawnCount[pc ${AllClassList.Arg[${_count},|]} raid radius ${XTHealRadius}]}
			/if (${_${AllClassList.Arg[${_count},|]}} && ${XHealClass.Find[${AllClassList.Arg[${_count},|]}]}) {
				/for _x 1 to ${_${AllClassList.Arg[${_count},|]}}
					DEBUG _x ${_x}
					DEBUG _b ${_b}
					/if (${_b} > 13) /break
					/if (${Bool[${Group.Member[${NearestSpawn[${_x}, pc ${AllClassList.Arg[${_count},|]} raid]}]}]} || ${Bool[${Me.XTarget[${NearestSpawn[${_x}, pc ${AllClassList.Arg[${_count},|]} raid].Name}]}]} || ${ProtectInsideRaid.Find[${NearestSpawn[${_x}, pc ${AllClassList.Arg[${_count},|]} raid].Name}]}) /continue 
					DEBUG /xtarget set ${_b} ${NearestSpawn[${_x}, pc ${AllClassList.Arg[${_count},|]} raid radius ${XTHealRadius}]}
					/varcalc _b ${_b}+1
					/xtarget set ${_b} ${NearestSpawn[${_x}, pc ${AllClassList.Arg[${_count},|]} raid radius ${XTHealRadius}]}
				/next _x
			}		
		/next _count	
		***|
	}
	
/return



|***
 *	DES: Sticks you to the target
 *	USE: /call stick_to_target [target ID]
 *	INI: na
 *  NOTE: for the love of whatever you find important, don't touch this ever again...
 ***|
sub stick_to_target()
	DEBUG ${cbug}stick_to_target()\ax Setting Stick to: [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax] @ ${CombatDistance}% agro distance
	/if (${Select[${combat},RANGE,NUKE,HEAL]} || !${coreAuto}) /return

	/if (!${validate_plugin[TRUE, |mq2moveutils]}) /return

	/if (${is_target_dead[${Target2Kill}]}) /return
	/doevents
	AMIDEAD	
	/if (${SetStickBehind}) {
		DEBUG ${cbug}stick_to_target(behind ${SetStickBehind})\ax coreAuto
		/call get_assist_target
		/if (${Target.ID} && ${Select[${combat},MELEE,RANGE]} && !${Range.Between[0,${SetFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
			
		/if (!${Stick.Active} && ${Target.ID} && ${isValidTarget} && ${SetStickBehind} && !${Stick.Behind} && !${timer_snap_recheckID_${Target2Kill}}) {
			/squelch /stick id ${Target2Kill} ${CombatDistance} snaproll
		} 

		:checkstuck
		/doevents
		AMIDEAD	
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
	} else {
		DEBUG ${cbug}stick_to_target()\ax
		/if (${switchModeHunt}) /varset isValidTarget TRUE
		:movetomob
		/doevents
		AMIDEAD	
		/if (${is_target_dead[${Target2Kill}]}) /return
		/if (!${Stick.Active} && ${Target.ID} && ${isValidTarget} && ${Spawn[${Target2Kill}].Distance} > ${CombatDistance}) {
			/if (${coreAuto} && ${Select[${combat},MELEE,RANGE]} && !${Range.Between[0,${SetFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
			/if (${Select[${combat},RANGE]}) /return
			DEBUG stick_to_target to mob. farther then:${CombatDistance}
			/squelch /stick id ${Target2Kill} ${CombatDistance} 
		}
		/doevents
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
	DEBUG ${cbug}get_assist_target()\ax
	AMIDEAD
	/if (!${coreAuto}) /return
	/if (${Group.MainAssist.Name.Equal[${Me.DisplayName}]}) /return
	
	| if no MA present. move along..	
	/if (!${Bool[${Spawn[${MA1}].ID}]} && !${Bool[${Spawn[${MA2}].ID}]} && !${Bool[${useSmartAssistR}]} && !${Bool[${useSmartAssistG}]}) {
		DEBUG ${cbad}NO MA\ax
		/delay 1
		/return
	} 	
	
	| cannnot assist yourself
	/if (${Spawn[pc ${MA1}].ID} == ${Me.ID}) /call Bind_setCombatControl MA1 FALSE
	/if (${Spawn[pc ${MA2}].ID} == ${Me.ID}) /call Bind_setCombatControl MA2 FALSE
	
	| if we dont have underfoot expansion, skip ALL smartassist go directly to jail. 
	/if (!${validate_expansion[FALSE, |16]}) /goto :skippingtoMAtarget

	| no, you still cannot assist yourself. get over it
	/if (${Spawn[pc ${Group.MainAssist}].ID} == ${Me.ID}) /call Bind_setCombatControl smartma X
	/if (${Spawn[pc ${Raid.MainAssist}].ID} == ${Me.ID}) /call Bind_setCombatControl smartma X

	| if in a group and no smartassist is set, skip to MA1/2
	| /if (!${Raid.Members} && ${Bool[${Group.Members}]} && !${Bool[${Group.MainAssist}]}) :skippingtoMAtarget
	
	| tar acquisition when using smart assist. set XTarget positions.
	/if (${Bool[${useSmartAssistR}]} || ${Bool[${useSmartAssistG}]}) {
		DEBUG Using SmartMA

		| if the raid/group roles are not set, move on tho MA1/2	
		/if (${Bool[${useSmartAssistG}]} && (!${Bool[${Group.MainAssist}]} || ${Bool[${Spawn[${Group.MainAssist}].Dead}]})) /goto :skippingtoMAtarget
	
		/if (${Me.XTarget[1].TargetType.NotEqual[Raid Assist 1 Target]} && ${Raid.Members} && ${Bool[${useSmartAssistR}]}) {
			/xtarget set 1 raidassist1target
			/xtarget set 2 Autohater
		} else /if (${Me.XTarget[1].TargetType.NotEqual[Group Assist Target]} && ${Bool[${Group.Members}]} && ${Bool[${useSmartAssistG}]}) {
			/xtarget set 1 groupassisttarget
		}
		
		| smart assist raid 1
		/if (${Bool[${Spawn[${Me.XTarget[1].ID}].Type.Equal[NPC]}]} && ${Bool[${useSmartAssistR}]}) {
			/if (${Me.XTarget[1].PctHPs} <= ${smartengageAt} || ${switchOverrideEngageat}) {
				DEBUG Setting SMA1
				/varset Target2Kill ${Me.XTarget[1].ID}
				/delay 1
				/if (${Spawn[id ${Target2Kill}].Distance} <= ${MobAgro} && ${Target.ID} != ${Target2Kill}) /target ID ${Target2Kill}
				/return
			}	
		}
		| smart assist group
		/if (${Bool[${Spawn[${Me.XTarget[1].ID}].Type.Equal[NPC]}]} && ${Bool[${useSmartAssistG}]}) {
			/if (${Me.XTarget[1].PctHPs} <= ${smartengageAt} || ${switchOverrideEngageat} || (${Select[${Me.Class.ShortName},ENC]} && ${Select[${combat},DUAL]})) {
				DEBUG Setting SMAG
				/varset Target2Kill ${Me.XTarget[1].ID}
				/delay 1
				/if (${Spawn[id ${Target2Kill}].Distance} <= ${MobAgro} && ${Target.ID} != ${Target2Kill}) /target ID ${Target2Kill}
				/return
			}	
		}
		/return
	} 

	| target acquisition when not using smart assist.
	:skippingtoMAtarget
	/if (${SpawnCount[npc radius ${MobAgro}]} && (${Bool[${MA1}]} || ${Bool[${MA2}]})) {
		| MA1
		/if (${Bool[${MA1}]} && ${Bool[${Spawn[${MA1}].ID}]} && !${Bool[${Spawn[${MA1}].Dead}]}) {
			DEBUG MA1 finding assist from ${MA1}
			/assist ${MA1}
			/delay 1s ${Me.AssistComplete}
			/if (!${Target2Kill} && ${Target.ID} && ${Spawn[${Target.ID}].Type.Equal[npc]}) {
				/varset Target2Kill ${Target.ID}
				DEBUG get_assist_target() target = ${Target2Kill}
			} else /if (${Target2Kill} != ${Target.ID} && ${Target2Kill} != 0 && ${Target.ID} && ${Spawn[${Target}].Type.Equal[npc]}) {
				/delay 1
				/varset Target2Kill 0
				DEBUG get_assist_target() ${Target2Kill} != ${Target.ID} CLEAR TARGET.. BAD
				/return
			}	
	
		| MA2
		} else /if (${Bool[${MA2}]} && ${Bool[${Spawn[${MA2}].ID}]} && !${Bool[${Spawn[${MA2}].Dead}]} && (!${Bool[${Spawn[${MA1}].ID}]} || ${Bool[${Spawn[${MA1}].Dead}]})) {
			/assist ${MA2}
			/delay 1s ${Me.AssistComplete}
			/if (!${Target2Kill} && ${Target.ID} && ${Spawn[${Target.ID}].Type.Equal[npc]}) {
				/varset Target2Kill ${Target.ID}
			} else /if (${Target2Kill} != ${Target.ID} && ${Target2Kill} != 0 && ${Target.ID} && ${Spawn[${Target}].Type.Equal[npc]}) {
				/delay 1
				/varset Target2Kill 0
				/return
			}	
		}
		/doevents
		AMIDEAD
		/if (${Target2Kill} == ${Target.ID} && ${Spawn[${Target}].Type.Equal[npc]}) {
			/if (${Target.PctHPs} > ${engageAt} && !${switchOverrideEngageat}) {
				/delay 3
				/goto :skippingtoMAtarget
			} 
		} else {
			/if (${Target.ID}) /squelch /target clear
			/return
		}
	}
/return



|***
 *	DES: Make sure that what ever we are targeting is something we want to kill.
 *	USE: /call is_target_valid [target ID]
 *	INI: 
 *  NOTE: in manual mode. anything is a valid target.
 ***|
sub is_target_valid(int _tmpID)
	DEBUG ${cbug}is_target_valid(\ax${_tmpID}${cbug})\ax checking: [${cinfo}${Spawn[${_tmpID}].DisplayName}\ax]
	AMIDEAD
	/doevents
	/if (!${coreAuto}) /return TRUE
	/if (!${Target.ID} || !${Bool[${Target2Kill}]} || (${SafeZone} && !${switchOverrideSafezone} && ${coreAuto})) {
		/varset isValidTarget FALSE
		/return FALSE
	}
	/varset isValidTarget TRUE

	| /if (!${validate_expansion[FALSE, |16]}) /goto :skippingtoMAtarget

	| check smart assists
	/if (${Target.ID} && ${coreAuto}) {
		/if (${useSmartAssistR} && ${Spawn[${Target2Kill}].ID} != ${Me.XTarget[1].ID}) {
			/call clear_combat
		} else /if (${useSmartAssistG} && ${Bool[${Group.MainAssist}]} && !${Bool[${Spawn[${Group.MainAssist}].Dead}]} && ${Spawn[${Target2Kill}].ID} != ${Me.XTarget[1].ID}) {
			/call clear_combat
		}
	}
	
	/if (${Spawn[${_tmpID}].PctHPs} > ${engageAt} && !${switchPull} && !${useSmartAssistR} && !${useSmartAssistG} && !${switchOverrideEngageat}) {
		/delay 1
		/varset isValidTarget FALSE
		/return FALSE
	} else /if (${Spawn[${_tmpID}].PctHPs} > ${smartengageAt} && !${switchPull} && (${useSmartAssistR} || ${useSmartAssistG}) && !${switchOverrideEngageat} ) {
		/delay 1
		/varset isValidTarget FALSE
		/return FALSE
	}
	/if (${ExcludeList.Find[${Spawn[${_tmpID}].DisplayName}]}) /varset isValidTarget FALSE
	/if (${Target.ID} && ${Bool[${Target.Mezzed.Duration}]} && !${switchPull}) /varset isValidTarget FALSE
	/if ((!${Spawn[${_tmpID}].LineOfSight} && !${switchOverrideLos}) || ${Spawn[${_tmpID}].Distance} > ${MobAgro} ) /varset isValidTarget FALSE
	/if (${Spawn[${_tmpID}].Type.Equal[PC]}) /varset isValidTarget FALSE
	/if (${Spawn[${Target.ID}].Type.Equal[PC]}) /varset isValidTarget FALSE
	/if (${_tmpID} == ${Me.ID} || ${_tmpID} == ${Me.Pet.ID}) /varset isValidTarget FALSE

	| /if (${coreAuto} && ${isValidTarget} && ${Select[${Me.Class.ShortName},ROG]} && ${switchSos}) /makemevisible
		
	DEBUG ${Spawn[${_tmpID}].DisplayName} [ID:${cinfo}${_tmpID}\ax] is a valid target: ${If[${isValidTarget},${cgood}TRUE\ax,${cbad}FALSE\ax]} ${If[${Spawn[${_tmpID}].LineOfSight},${cgood}LoS\ax,${cbad}LoS\ax]} ${If[${Spawn[${_tmpID}].Distance} <= ${MobAgro},${cgood}${Spawn[${_tmpID}].Distance}\ax,${cbad}${Spawn[${_tmpID}].Distance}\ax]}
/return ${isValidTarget}




|***
 *	DES: Seriously, do you need to ask?
 *	USE: mob dead = win!
 *	INI: 
 *  NOTE: ass.U.ME the mob is alive until, it is not.
 ***|
sub is_target_dead(int _tmpID)
	DEBUG ${cbug}is_target_dead(\ax${Spawn[${_tmpID}].DisplayName} [${_tmpID}]${cbug})\ax
	AMIDEAD
	/declare _dead bool local FALSE
	
	/if (${coreAuto}) {
		/if (!${Bool[${_tmpID}]}) /varset _tmpID ${Target2Kill}
	} else /if (!${coreAuto}) {
		/if (!${Bool[${_tmpID}]} && ${Target.ID}) /varset _tmpID ${Target.ID}
		/if (!${Bool[${_tmpID}]}) /return FALSE
	}
	
	| no target ID? shits dead 'till we get one to check..
	/if (!${Target.ID} && !${switchModeHunt}) /varset _dead TRUE
	/if (${_tmpID} == 0 || !${Bool[${_tmpID}]}) /varset _dead TRUE
	/if (${_dead}) /goto :dead

	| targets dead? were done here
	/if (${Target.Dead} || ${Target.Type.Equal[Corpse]}) /varset _dead TRUE
	/if (${_dead}) /goto :dead

	/if (${Bool[${Spawn[${_tmpID}].Type.Equal[CORPSE]}]} || ${Spawn[${Target.ID}].Type.Equal[CORPSE]}) /varset _dead TRUE
	/if (${Bool[${Spawn[${_tmpID}]].Dead}]} || ${Spawn[${Target.ID}].Dead}) /varset _dead TRUE
	/if (!${Spawn[${_tmpID}].ID} || !${Bool[${Spawn[ID ${_tmpID}]}]}) /varset _dead TRUE
	/if (${Spawn[${Target.ID}].Type.Equal[PCCORPSE]}) /varset _dead TRUE

	:dead
	/if (${_dead}) /call clear_combat
	DEBUG ${cbug}is_target_dead(\ax${ctar}${Spawn[${_tmpID}].DisplayName}\ax [${cinfo}${_tmpID}\ax]${cbug})\ax Dead: ${If[${_dead},${cgood}TRUE\ax,${cbad}FALSE\ax]}
/return ${_dead}



|***
 *	DES: Clear all combat variables
 *	USE: /call clear_combat
 *	INI: na
 *  NOTE: 
 ***|
sub clear_combat()
	DEBUG ${cbug}clear_combat()\ax
	| AMIDEAD
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
	/if (${isValidTarget}) /varset isValidTarget FALSE
	/if (${Stick.Active}) /squelch /stick off
	/if (${coreAuto} && ${Target.ID}) /squelch /target clear
	/if (!${coreAuto} && !${Spawn[${Target.ID}].Type.Equal[CORPSE]}) /squelch /target clear
	UPDATEHUD FALSE
/return TRUE



|***
 *	DES: Clears all defined timers for a specific ID
 *	USE: /call clear_timer [TARGETID]
 *	INI: na
 *  NOTE: timers that need clearing when the target is deaded
 ***|
sub clear_timer(int _tmpID)
	DEBUG	${cbug}clear_timers()\ax
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
	| if you have to ask
	/if (${Defined[timer_banestrike_${_tmpID}]}) /deletevar timer_banestrike_${_tmpID}
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
	DEBUG ${cbug}cast_condition(condition: ${_condition} ${sep} (${${_condition}})\ax
	/if (!${Bool[${${_condition}}]}) /return TRUE
	DEBUG ${_condition}: ${${_condition}}
	/if (${${_condition}}) /return TRUE
/return FALSE



|***
 *	DES: Cast nukes
 *	USE: /call cast_nuke
 *	INI: 
 *  NOTE: "I say we take off and nuke the entire site from orbit." --Ripley
 ***|
sub cast_nuke()
	DEBUGCAST ${cbug}cast_nuke()\ax
	/if (${timer_Nuke_Delay}) /return	
	/call get_assist_target
	/declare _count int local 0
	
	/for _count 1 to ${DoNuke}
		/if (!${Range.Between[1,${DoNuke}:${Int[${_count}]}]}) /continue
		DEBUG _count:${_count}, spell:${Nuke${_count}}, manna:${Me.CurrentMana} < ${Spell[${Nuke${_count}}].Mana}
		AMIDEAD
		CHECKEXIT
		
		/if (${is_target_dead[${Target2Kill}]}) /return
		/if (${Me.CurrentMana} < ${Spell[${Nuke${_count}}].Mana} || ${Me.CurrentMana} < 2) /continue 
		/if (!${cast_condition[Nuke${_count}Condition]}) /continue 
		
		/call core_cast "${Nuke${_count}}" 0 ${Target2Kill} cast_break
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
 *	DES: Cast DoT's
 *	USE: /call cast_DOT
 *	INI: 
 *  NOTE: 
 ***|
sub cast_DOT()
	DEBUGCAST ${cbug}cast_DOT(count ${DoDoT})\ax
	/call get_assist_target
	/declare _count int local 0
	/for _count 1 to ${DoDoT}
		/if (!${Range.Between[1,${DoDoT}:${Int[${_count}]}]}) /continue
		AMIDEAD
		CHECKEXIT
		/if (${is_target_dead[${Target2Kill}]}) /return
		/if (${Select[${Me.Subscription},SILVER]}) /varset DoT${_count}chk ${Ini[${INIClass},Spells,DoT${_count}chk]}
		/if (!${cast_condition[DoT${_count}Condition]}) /continue 
		/if (${Me.CurrentMana} < ${Spell[${DoT${_count}}].Mana} || ${Me.CurrentMana} < 2) /continue 

		/if ((${Target.BuffDuration[${DoT${_count}chk}].TotalSeconds} <= 3 && ${Spawn[${Target.Buff[${DoT${_count}chk}].Caster}].ID} == ${Me.ID}) || !${Target.Buff[${DoT${_count}chk}].ID}) {
			/call core_cast "${DoT${_count}}" 0  ${Target2Kill} cast_break
		}
	/next _count
/return



|***
 *	DES: casts AE Rain spells. Magician mostly...
 *	USE: /call cast_nuke_rain | /userain [TRUE|FALSE]
 *	INI: 
 *  NOTE: I'd suggest not using this. though it is kind of fun to watch
 ***|
sub cast_nuke_rain()
	DEBUGCAST ${cbug}cast_nuke_rain()${cinfo}
	/if (${timer_Nuke_Delay}) /return	
	/call get_assist_target
	/declare _count int local 0
	/for _count 1 to ${doRainNuke}
		AMIDEAD
		CHECKEXIT
		/if (${is_target_dead[${Target2Kill}]}) /return

		/if (${Spawn[${Target2Kill}].LineOfSight} && ${Me.SpellReady[${RainNuke${_count}}]} && ${Spawn[${Target2Kill}].Distance} <= ${Spell[${RainNuke${_count}}].MyRange} && ${Me.CurrentMana} >= ${Spell[${RainNuke${_count}}].Mana}) {
			/call core_cast "${RainNuke${_count}}" 0 ${Target2Kill} cast_break
			/if (${Select[${castReturn},CAST_SUCCESS]}) /call create_timer timer_Nuke_Delay ${tNuke_Delay}
		}
	/next _count
/return



|***
 *	DES: use Banestrike AA
 *	USE: /call cast_AA_banestrike [target ID]
 *	INI: 
 *  NOTE: slap all the mobs
 ***|
sub cast_AA_banestrike(int _tmpID)
	DEBUGCAST ${cbug}cast_AA_banestrike(ID:${_tmpID})\ax
	AMIDEAD
	CHECKEXIT
	/if (${is_target_dead[${_tmpID}]}) /return
	/if (!${is_target_valid[${_tmpID}]}) /return
	/if (${Bool[${_tmpID}]}) /call core_cast "Banestrike" alt ${_tmpID} FALSE
/return



|***
 *	DES: Returns the toons target to the intended kill ID.
 *	USE: /call get_target_2kill
 *	INI: 
 *  NOTE: sometimes people need focus.
***|
sub get_target_2kill()
	DEBUG ${cbug}get_target_2kill()\ax
	AMIDEAD
	CHECKEXIT
	/call is_target_dead
	/if (!${Macro.Return} && ${Target.ID} != ${Target2Kill} || ${Spawn[id ${Target2Kill}].Distance} > ${MobAgro}) {
		/target ID ${Target2Kill}
	}	
	/if (${coreAuto} && ${Target.Distance} > ${MobAgro}) /call clear_combat
/return



|***
 *	DES: Uses Entropy of Nature/EradicateMagic/Whatever on target. only once every 60 seconds.
 *	USE: /call cast_eradicate_magic [target ID] & /debuff eradicate
 *	INI: 
 *  NOTE: 
 ***|
sub cast_eradicate_magic(int _tmpID)
	DEBUGCAST ${cbug}cast_eradicate_magic()\ax
	/if (${timer_eradicate_${_tmpID}}) /return
	AMIDEAD
	CHECKEXIT

	/if (!${AAEradicateList.Find[${Me.Class.ShortName}]}) {
		VOUT ${onotice} ${sep} Class does not have the AA Eradicate.
		/call writeINI switchdeBuffEradicate Common FALSE
		/call echos switch "deBuff Eradicate" switchdeBuffEradicate	0 0
		/return
	}
	
	/if (!${Defined[_debuffEradicate]}) /declare _debuffEradicate string local
	/if (${validate_class[FALSE, |ENC|MAG|NEC|WIZ]}) /varset _debuffEradicate Eradicate Magic
	/if (${validate_class[FALSE, |RNG|DRU]}) /varset _debuffEradicate Entropy of Nature
	
	/if (${Me.AltAbilityReady[${_debuffEradicate}]} && ${Spawn[${_tmpID}].Distance} <= ${Spell[${_debuffEradicate}].MyRange}) {
		/call core_cast "${_debuffEradicate}" alt ${_tmpID} FALSE
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_RESISTED,CAST_RESIST,CAST_NOHOLD,CAST_TAKEHOLD]}) /call create_timer timer_eradicate_${_tmpID} 1m
	}
/return



|***
 *	DES: Uses class specific AA snares
 *	USE: /call cast_AA_snare [target ID] & /debuff snare
 *	INI: na
 *  NOTE: did I mention this is for classes with AA snares..
 ***|
sub cast_AA_snare(int _tmpID)
	DEBUG ${cbug}cast_AA_snare(ID: ${_tmpID})\ax
	/if (${timer_snare_${_tmpID}}) /return
	AMIDEAD
	CHECKEXIT

	/if (!${Select[${Me.Class.ShortName},RNG,DRU,WAR,ROG,NEC,SHD,WIZ,MNK,PAL]}) {
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
	/declare _debuffSnare string local
	/if (${validate_class[FALSE, |RNG|DRU]}) /varset _debuffSnare Entrap
	/if (${validate_class[FALSE, |WAR]}) /varset _debuffSnare Knee Strike
	/if (${validate_class[FALSE, |ROG]}) /varset _debuffSnare Ligament Slice
	/if (${validate_class[FALSE, |NEC|SHD]}) /varset _debuffSnare Encroaching Darkness
	/if (${validate_class[FALSE, |WIZ]}) /varset _debuffSnare Atol's Shackles	
	/if (${validate_class[FALSE, |MNK]}) /varset _debuffSnare Crippling Strike	
	
	| ber Binding Axe disc .. ?? 
	
	/if (${Me.AltAbilityReady[${_debuffSnare}]} && ${Spawn[${_tmpID}].Distance} <= ${combatRadius} && !${Target.Snared.ID}) {
		/if (!${validate_class[FALSE, |PAL]}) /call core_cast "${_debuffSnare}" alt ${_tmpID} FALSE
		
		| pally live or dead snare
		/if (${validate_class[FALSE, |PAL]} && ${Target.Body.Name.Equal[Undead]}) {
			/call core_cast "Halt the Dead" alt ${_tmpID} FALSE
		} /if (${validate_class[FALSE, |PAL]} && !${Target.Body.Name.Equal[Undead]}){
			/call core_cast "Shackles of Tunare" alt ${_tmpID} FALSE
		}
		
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_RESISTED,CAST_RESIST,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			DEBUG /call create_timer timer_snare_${_tmpID} ${Spell[${_debuffSnare}].Duration.TotalSeconds}s
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
	DEBUGCAST ${cbug}click_BP(${cinfo}${Breastplate1}, ${Breastplate2}\ax${cbug})\ax
	AMIDEAD
	CHECKEXIT
	/if (!${Me.ItemReady[${Breastplate1}]} && !${Me.ItemReady[${Breastplate2}]}) /return
	/if (${Bool[${Me.Song[${Breastplate1effect}]}]} || ${Bool[${Me.Buff[${Breastplate2effect}]}]}) /return
	/if (!${Bool[${Me.Song[${Breastplate1effect}]}]} && ${Me.ItemReady[${Breastplate2}]} && ${Spell[${Breastplate2effect}].Stacks}) {
		/call core_cast "${Breastplate2}" item 0 FALSE
	} else /if (!${Bool[${Me.Buff[${Breastplate2effect}]}]} && ${Me.ItemReady[${Breastplate1}]} && ${Spell[${Breastplate1effect}].Stacks}) {
		/call core_cast "${Breastplate1}" item 0 FALSE
	}
/return



|***
 *	DES: Uses a mod rod if you have one in your inventory
 *	USE: /call use_mod_rod
 *	INI: 
 *  NOTE: listed all i could think of. dont want to keep up with this in an INI.
 ***|
sub use_mod_rod()
	DEBUGCAST ${cbug}use_mod_rod()\ax 
	/if (${Select[${Me.Class.ShortName},ROG,BER,WAR,MNK,BRD]}) /return
	AMIDEAD
	CHECKEXIT
		
	| use all the rods!!
	/if (${Me.PctMana} < 70*.${tmpHealOverride}) {
		/if (${Me.ItemReady[Summoned: Glowing Modulation Shard]} && ${Me.CurrentHPs} > 45500) /call core_cast "Summoned: Glowing Modulation Shard" item 0 FALSE
		/if (${Me.ItemReady[Summoned: Giant Modulation Shard]} && ${Me.CurrentHPs} > 37500) /call core_cast "Summoned: Giant Modulation Shard" item 0 FALSE
		/if (${Me.ItemReady[Summoned: Large Modulation Shard]} && ${Me.CurrentHPs} > 23100) /call core_cast "Summoned: Large Modulation Shard" item 0 FALSE
		/if (${Me.ItemReady[Summoned: Medium Modulation Shard]} && ${Me.CurrentHPs} > 15100) /call core_cast "Summoned: Medium Modulation Shard" item 0 FALSE
		/if (${Me.ItemReady[Summoned: Small Modulation Shard]} && ${Me.CurrentHPs} > 7600) /call core_cast "Summoned: Small Modulation Shard" item 0 FALSE
		/if (${Me.ItemReady[Summoned: Modulating Rod]} && ${Me.CurrentHPs}>325) /call core_cast "Summoned: Modulating Rod" item 0 FALSE

		/if (${Me.ItemReady[Wand of Pelagic Transvergence]} && ${Me.CurrentHPs} > 12850) /call core_cast "Wand of Pelagic Transvergence" item 0 FALSE
		/if (${Me.ItemReady[Wand of Phantasmal Modulation]} && ${Me.CurrentHPs} > 11100) /call core_cast "Wand of Phantasmal Modulation" item 0 FALSE 
		/if (${Me.ItemReady[Wand of Pelagic Transvergence]} && ${Me.CurrentHPs} > 3594) /call core_cast "Wand of Pelagic Transvergence" item 0 FALSE
		/if (${Me.ItemReady[Wand of Phantasmal Transvergence]} && ${Me.CurrentHPs} > 3138) /call core_cast "Wand of Phantasmal Transvergence" item 0 FALSE
		/if (${Me.ItemReady[Wand of Temporal Mastery]} && ${Me.CurrentHPs} > 1686) /call core_cast "Wand of Temporal Mastery" item 0 FALSE
		/if (${Me.ItemReady[Wand of Elemental Transvergence]} && ${Me.CurrentHPs} > 920) /call core_cast "Wand of Elemental Transvergence" item 0 FALSE

		/if (${Me.ItemReady[Rod of Arcane Transvergence]} && ${Me.CurrentHPs} > 2530) /call core_cast "Rod of Arcane Transvergence" item 0 FALSE
		/if (${Me.ItemReady[Rod of Spectral Transvergence]} && ${Me.CurrentHPs} > 1720) /call core_cast "Rod of Spectral Transvergence" item 0 FALSE
		/if (${Me.ItemReady[Rod of Ethereal Transvergence]} && ${Me.CurrentHPs} > 1320) /call core_cast "Rod of Ethereal Transvergence" item 0 FALSE
		/if (${Me.ItemReady[Rod of Mechamagical Mastery]} && ${Me.CurrentHPs} > 1400) /call core_cast "Rod of Mechamagical Mastery" item 0 FALSE
		/if (${Me.ItemReady[Rod of Prime Transvergence]} && ${Me.CurrentHPs} > 1000) /call core_cast "Rod of Prime Transvergence" item 0 FALSE
		/if (${Me.ItemReady[Rod of Mystical Transvergence]} && ${Me.CurrentHPs} > 550) /call core_cast "Rod of Mystical Transvergence" item 0 FALSE
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
	DEBUG ${cbug}send_pet()${cinfo}
	/if (!${switchPcSwarm} && !${switchPcPet}) /return
	AMIDEAD
	CHECKEXIT
	/if (${Spawn[${Target2Kill}].Distance} > ${MobAgro} || ${Spawn[${Target.ID}].Type.Equal[corpse]} || ${Target.Dead}) /return
	
	/if (!${Target2Kill}) {
		/call get_assist_target
		/call is_target_valid ${Target2Kill}
		/if (!${isValidTarget}) /return
	}
	
	/target ID ${Target2Kill}
		
	/if (${switchPcPet} && ${Me.Pet.ID} &&  (${Target.PctHPs} <= ${PcPetengageat} || ${switchOverrideEngageat}) && !${Bool[${Me.Pet.Target}]}) {
		/call echos petfarm "filler" ${Target2Kill} 0 0
		/pet attack
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
	DEBUGCAST ${cbug}use_glyph(${useGlyph})\ax
	AMIDEAD
	/if (!${Bool[${useGlyph}]} || !${Raid.Members}) /return
	/if (!${Me.AltAbility[${useGlyph}].ID} && ${Me.AAPoints} >= ${AltAbility[${useGlyph}].Cost}) /alt buy ${AltAbility[${useGlyph}].ID}
	/if (${Me.AltAbilityReady[${useGlyph}]}) /call core_cast "${useGlyph}" alt 0 FALSE
	/if (!${Me.AltAbility[${useGlyph}].ID} && ${Me.AAPoints} >= ${AltAbility[${useGlyph}].Cost}) /alt buy ${AltAbility[${useGlyph}].ID}
/return



|***
 *	DES: spell/disc/aa/item cast wrapper
 *	USE: /call core_cast "SPELLNAME" [alt|disc|gem#|item|0|NULL] [MobID|0|NULL] [subname|FALSE]
 *	USE: /call core_cast "SPELLNAME" NULL NULL if you want to let the sub decide what you have to cast/use.
 *	INI: 
 *  NOTE: MQ2Cast ["spell name"|"item name"|"AA name"|"AA#"] [[item|slotname]|alt|gem#] [give up time][s|m] [custom subroutine name] [-targetid|###] [-maxtries|#] [-recast|#] [-setin|setname] [-bandolier|setname]
 *  NOTE2: How about we quit making changes to this....
 ***|
sub core_cast(string _spell, string _type, int _tmpID, string _sub, bool _forcewait)
	DEBUGCAST ${cbug}core_cast(\ax spell:${cinfo}${_spell}\ax, type:${cinfo}${_type}\ax, ID:${cinfo}${_tmpID}\ax, sub:${cinfo}${_sub}\ax ${cbug})\ax
	/doevents
	AMIDEAD
	/declare _bail string local FALSE
	
	| preemptive gtfo for this particular spell
	| /if (${Bool[${timer_${_tmpID}_${Spell[${_spell}].ID}}]}) /goto :cooldown
	
	| make sure we have our target 
	/if (${Bool[${_tmpID}]} && ${Target.ID} != ${_tmpID}) {
		DEBUGCAST fixing target to ID:${_tmpID} ${sep} ${Spawn[ID ${_tmpID}].DisplayName}
		/tar id ${_tmpID}
		/delay 1s ${Target.ID} == ${_tmpID}
	}
	
	| used to let /call core_cast "whatever" NULL NULL
	| figure out what the toon has and use that.
	/if (${_type.Equal[NULL]} && !${Bool[${_tmpID}]}) {
		DEBUGCAST fixing cast type and target id NULL/NULL
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

	| AA ready?
	/if (${Select[${_type},alt]} && !${Me.AltAbilityReady[${_spell}]}) /return FALSE
	| disc ready?
	/if (${Select[${_type},disc]} && !${Me.CombatAbilityReady[${_spell}]}) /return FALSE
	| item ready?
	/if (${Select[${_type},item]} && !${Me.ItemReady[${_spell}]}) /return FALSE
	| spell ready?
	/if (!${Select[${_type},alt,item,disc]} && !${Me.SpellReady[${_spell}]}) /varset _bail spell_not_ready
	| enough mana?
	| /if (!${Select[${_type},item,disc]} && ${Me.CurrentMana} < ${Spell[${_spell}].Mana}) /return FALSE
	/if (${Select[${_type},alt,0,${buffGem}]} && ${Me.CurrentMana} < ${Spell[${_spell}].Mana}) /return FALSE
	| check enough end for disc?
	/if (${Select[${_type},disc]} && ${Me.CurrentEndurance} < ${Spell[${_spell}].EnduranceCost}) /return FALSE
	
	| buff check cast self and target
	/if (${check_no_do_a_thing_buff[cast]}) /return FALSE
	
	| dead?
	/if (${Target.Type.Equal[Corpse]} || ${Spawn[${_tmpID}].Type.Equal[corpse]}) /varset _bail target_dead
	| no target
	/if (!${Select[${_type},item]} && ${Bool[${_tmpID}]} && !${Target.ID}) /varset _bail no_target
	| moving?
	/if (!${Select[${_type},alt,item,disc]} && ((${Stick.Active} && !${setModeType.Equal[hunt]} && !${setModeType.Equal[scorch]}) || ${Me.Moving} || ${AdvPath.Following})) /varset _bail im_moving
	| spell casting?
	/if (!${Select[${_type},alt,item,disc]} && ${Me.Casting.ID}) /varset _bail im_casting_spell
	| line of site valid?
	/if (${_tmpID} != 0 && !${switchOverrideLos} && !${Target.LineOfSight} && !${Spell[${_spell}].SpellType.Equal[Beneficial]}) /varset _bail bad_LoS
	| target in range?
	/if (${_tmpID} != 0 && (${Target.Distance} > ${Spell[${_spell}].MyRange} || ${Spawn[${_tmpID}].Distance} > ${Spell[${_spell}].Range})) /varset _bail out_of_range

	| spell stacks on self?
	/if (!${Select[${_type},alt,item,disc]} && !${Spell[${_spell}].Stacks} && ${Target.ID} == ${Me.ID}) /varset _bail does_not_stack

	| bailing on this spell. its either out of range, immune or something..
	DEBUGCAST ${owarning} ${sep} _bail = ${_bail} for ${_spell} type ${_type}
	/if (${Bool[${_bail}]} && !${Spell[${_spell}].SpellType.Equal[Beneficial]}) /goto :cooldown
	| /if (${Bool[${_bail}]}) /goto :cooldown
	
	| do not have the spell in the book... seriously why do we even try
	/if (!${Select[${_type},alt,item,disc]} && !${Bool[${Me.Book[${_spell}]}]} && !${Me.AltAbilityReady[${_spell}]}) {
		DEBUGCAST ${owarning} ${sep} _bail = ${_bail} for ${_spell} type ${_type}
		/varset _bail do_not_have
		/if (${Bool[${_bail}]}) /goto :cooldown
	}


	| skip needing a target
	/if (${_tmpID} == 0 || ${Spell[${_spell}].SpellType.Equal[Self]}) /goto :notarget
	
	/if (!${Spawn[${_tmpID}].ID} && !${timer_spellcast_nomob_ID}) {
		VOUT You tried to cast ${csp}${_spell}\ax on something that doesn't exist.
		/varset castReturn CAST_NOTREADY
		/call create_timer timer_spellcast_nomob_ID 60s
	}

	| restist types vs core.zone.inc
	/if (${Target.ID} && !${Spell[${_spell}].SpellType.Equal[Beneficial]} && ${_tmpID} != 0) {
		/if (${NoDiseaseList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Disease]}) /varset _bail immune_disease
		/if (${NoMagicList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Magic]}) /varset _bail immune_magic
		/if (${NoPoisonList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Poison]}) /varset _bail immune_poison
		/if (${NoFireList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Fire]}) /varset _bail immune_fire
		/if (${NoColdList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Cold]}) /varset _bail immune_cold
		/if (${NoCorruptionList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Corruption]}) /varset _bail immune_corruption
	}

	| bailing on this spell. its either out of range, immune or something..
	/if (${Bool[${_bail}]}) DEBUGCAST ${owarning} ${sep} _bail = ${_bail} immune too ${_spell}
	/if (${Bool[${_bail}]} && !${Spell[${_spell}].SpellType.Equal[Beneficial]} && (${Target.ID} || ${_tmpID})) /goto :cooldown

	:notarget
	AMIDEAD
	DEBUGCAST ${cbug}(\ax\awSPELL:${_spell}, TYPE:${_type}, ID:${_tmpID}, SUB:${_sub}\ax${cbug})\ax

	| Passed all checks. lets cast some shits..
	DEBUGCAST passed all checks

	| output echos
	| Spell
	/if (!${Select[${_type},alt,item,disc]}) {
		| /if (${_forcewait}) /call mem_spell FALSE "${_spell}" ${If[${Bool[${_type}]},${_type},gem${Me.Gem[${_spell}]}]} TRUE
		/if (${_tmpID} == 0) {
			VSPELL Spell ${sep} ${csp}${_spell}\ax
			/call MQ2Cast "${_spell}" ${If[${Bool[${_type}]},${_type},gem${Me.Gem[${_spell}]}]} -targetid|${_tmpID}
		} else /if (${_tmpID} != 0 && ${Target.ID}) {
			/if (${Me.Song[Gift of Mana (${GoMLevel})].ID}) /call cast_nuke_GoM ${_tmpID}
			DEBUGCAST MQ2Cast "${_spell}" ${If[${Bool[${_type}]},${_type},gem${Me.Gem[${_spell}]}]} -targetid|${_tmpID}
			VSPELL Spell ${sep} ${csp}${_spell}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
			/call MQ2Cast "${_spell}" ${If[${Bool[${_type}]},${_type},gem${Me.Gem[${_spell}]}]} ${If[${Bool[${_sub}]},${_sub},]} -targetid|${_tmpID}
		}
		VBC Spell "${_spell}" ${_tmpID} 0 
		/goto :cooldown
	| AA
	} else /if (${Select[${_type},alt]}) {
		/if (${_tmpID} == 0) {
			VALT AA ${sep} ${csp}${_spell}\ax
			/call MQ2Cast "${_spell}" alt 
		} else /if (${_tmpID} != 0 && ${Target.ID}) {
			VALT AA ${sep} ${csp}${_spell}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
			/call MQ2Cast "${_spell}" alt -targetid|${_tmpID}
		}
		VBC Alt "${_spell}" ${_tmpID} 0 
		/goto :cooldown
	| item
	} else /if (${Select[${_type},item]}) {
		/if (${_tmpID} == 0) {
			VITEM Item ${sep} ${citem}${_spell}\ax
			/call MQ2Cast "${_spell}" item
		} else /if (${_tmpID} != 0 && ${Target.ID}) {
			VITEM Item ${sep} ${citem}${_spell}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
			/call MQ2Cast "${_spell}" item -targetid|${_tmpID}
		}
		VBC Item "${_spell}" ${_tmpID} 0 
		/goto :cooldown
	| disc
	} else /if (${Select[${_type},disc]}) {
		/if (${_tmpID} == 0) VDISC Disc ${sep} ${csp}${_spell}\ax
		/if (${_tmpID} != 0 && ${Target.ID}) VDISC Disc ${sep} ${csp}${_spell}\ax ${sep} [${ctar}${Spawn[ID ${_tmpID}].DisplayName}\ax]
		VBC Disc "${_spell}" ${_tmpID} 0 
		/disc ${_spell}
		/delay 5s !${Me.Casting.ID}
		/goto :cooldown
	}
	DEBUGCAST ${csp}${_spell}\ax ${sep}${sep}${sep}${sep} ${sepO} ${castReturn} ${sepC}

	| cooldown
	:cooldown
	AMIDEAD
	/if (!${Select[${_type},alt,item,disc]}) {
		/delay 2 !${Me.SpellReady[${_spell}]}
	} else /if (${Select[${_type},item]}) {
		/delay 2 !${Me.ItemReady[${_spell}]}
	} else /if (${Select[${_type},alt]}) {
		/delay 2 !${Me.AltAbilityReady[${_spell}]}
	} else /if (${Select[${_type},disc]}) {
		/delay 2 !${Me.CombatAbilityReady[${_spell}]}
	}

	DEBUGCAST ${cbug}core_cast(\ax${cinfo}SPELL:${_spell})\ax ${sep} bail:${_bail}
/return ${Bool[${Macro.Return}]}



|***
 *	DES: will halt spell casting and force Gift of Manna nukes if setup
 *	USE: sub for use with MQ2Cast call
 *	INI: 
 *  NOTE: this is checked on most spell casts. prevents doing stupid shit.
 ***|
sub cast_break()
	DEBUGCAST ${cbug}cast_break()\ax
	AMIDEAD
	/doevents 
	/if (${Spell[${Me.Casting.ID}].Category.Equal[Transport]}) /return TRUE
	/if (${Spell[${Me.Casting.ID}].Category.Equal[Heal]}) /return TRUE
	
	/if (${Target.PctHPs} <= ${Int[${stopnukeAt}]}) /stopcast
	
	/if (${switchAmIDead}) /stopcast
	/if (${flagCallNeedRez}) /stopcast
		
	/if (${Target.Mezzed.ID} || !${Spawn[${Target2Kill}].ID} || !${Bool[${Target2Kill}]}) {
		/stopcast
	} else /if (${Bool[${Me.Song[Gift of Mana (${GoMLevel})]}]} && (${Bool[${GoMNuke}]} || ${Bool[${GoMNuke2}]})) {
		/stopcast
		/call cast_nuke_GoM ${Target2Kill}
	}
/return



|***
 *	DES: Cures/removes debuffs, we hope...
 *	USE: netbots are automatic.
 *	INI: 
 *  NOTE: 
 ***|
sub cast_cure()
	DEBUGCAST ${cbug}cast_cure()\ax
	/call create_timer timer_check_cast_cure ${tCast_Cure}
	AMIDEAD
	/doevents

	/if (!${validate_plugin[FALSE, |mq2debuffs]}) /return
	
	UPDATEHUD "CURE"

	| if i'm fine skip to netbots check
	/if (!${Debuff.Counters}) {
		/if (!${Bool[${Group.Members}]} || ${switchPull} || ${switchAgro}) /return
	} else /if (${Debuff.Counters}) {
		| cure self only with items
		
		/if (${Select[${Me.Class.ShortName},MNK]}) {
			/call core_cast "Purify Body" alt 0 FALSE
		} else /if (${Select[${Me.Class.ShortName},PAL,CLR,SHM,DRU]} && ${Me.AltAbilityReady[Radiant Cure]}) {
			/if (${Debuff.Cursed} || ${Debuff.Poisoned} || ${Debuff.Diseased}) /call core_cast "Radiant Cure" alt 0 FALSE
		}
		
		/if (${Debuff.Cursed} && ${Me.ItemReady[${cureCurseItem}]}) {
			/call core_cast "${cureCurseItem}" item ${Me.ID} FALSE
		} else /if (${Debuff.Corrupted} && ${Me.ItemReady[${cureCorruptItem}]}) {
			/call core_cast "${cureCorruptItem}" item ${Me.ID} FALSE
		} else /if (${Debuff.Poisoned} && ${Me.ItemReady[${curePoisonItem}]}) {
			/call core_cast "${curePoisonItem}" item ${Me.ID} FALSE
		} else /if (${Debuff.Diseased} && ${Me.ItemReady[${cureDiseaseItem}]}) {
			/call core_cast "${cureDiseaseItem}" item ${Me.ID} FALSE
		}
	}

	| /if (!${validate_plugin[FALSE, |mq2netbots]}) /return

	/declare _count int local 0
	/for _count 1 to ${Math.Calc[${NetBots.Client.Count[ ]}+1]}
		AMIDEAD
		/doevents 

		| are they debuffed in the zone and alive?
		/if (${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Count} <= 0 || !${Spawn[p${NetBots.Client[${_count}]}].ID} || ${Spawn[pc ${NetBots.Client[${_count}]}].Dead}) /continue

		| AA Radient Cure 
		| 100' 1 min
		/if (${validate_class[FALSE, |PAL|SHM|CLR|DRU]} && ${Me.AltAbilityReady[Radiant Cure]}) {
			/if (${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Count}) {
				/if (${Spawn[${NetBots.Client[${_count}]}].Distance} <= ${Spell[radiant cure].MyRange}) /call core_cast "Radiant Cure" alt 0 FALSE
			}
			/return
		}

		| AA Purify Soul
		| 200' 5 min
		/if (${validate_class[FALSE, |PAL|CLR|DRU]} && ${Me.AltAbilityReady[Purify Soul]}) {
			/if (${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Count}) {
				/if (${Spawn[pc ${NetBots.Client[${_count}]}].Distance} <= ${Spell[Purify Soul].MyRange}) /call core_cast "Purify Soul" alt ${Spawn[pc ${NetBots.Client[${_count}]}].ID} FALSE
			}
			/return
		}
		
		| AA Group Purify Soul . were in deep shit if we've gotten this far.
		| 0/100' 15 min
		/if (${validate_class[FALSE, |CLR|DRU]} && ${Me.AltAbilityReady[Group Purify Soul]}) {
			/if (${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Count}) {
				/if (${Spawn[pc ${NetBots.Client[${_count}]}].Distance} <= 100) /call core_cast "Group Purify Soul" alt 0 FALSE
			}
			/return
		}

		| cure with spell
		/if (${Bool[${cureCurse}]} && ${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Cursed} && ${Spawn[${NetBots.Client[${_count}]}].Distance} <= ${Spell[${cureCurse}].MyRange}) {
			/call core_cast "${cureCurse}" ${buffGem} ${Spawn[pc ${NetBots.Client[${_count}]}].ID} FALSE
		} else /if (${Bool[${cureCorrupt}]} && ${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Corrupted} && ${Spawn[pc ${NetBots.Client[${_count}]}].Distance} <= ${Spell[${cureCorrupt}].MyRange}) {
			/call core_cast "${cureCorrupt}" ${buffGem} ${Spawn[pc ${NetBots.Client[${_count}]}].ID} FALSE
		} else /if (${Bool[${curePoison}]} && ${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Poisoned} && ${Spawn[pc ${NetBots.Client[${_count}]}].Distance} <= ${Spell[${curePoison}].MyRange}) {
			/call core_cast "${curePoison}" ${buffGem} ${Spawn[pc ${NetBots.Client[${_count}]}].ID} FALSE
		} else /if (${Bool[${cureDisease}]} && ${Debuff[${NetBots[${NetBots.Client[${_count}]}].Buff}].Diseased} && ${Spawn[pc ${NetBots.Client[${_count}]}].Distance} <= ${Spell[${cureDisease}].MyRange}) {
			/call core_cast "${cureDisease}" ${buffGem} ${Spawn[pc ${NetBots.Client[${_count}]}].ID} FALSE
		}
	/next _count
	UPDATEHUD FALSE
/return



|***
 *	DES:targets and casts a free targeting area spell.
 *	USE: /call cast_free_target [SPELL] [TARGET ID]
 *	INI: 
 *  NOTE: i could never get this to work right...
 ***|
sub cast_free_target(string _tmpSpell, int _tmpID)
	DEBUGCAST ${cbug}cast_free_target()\ax
	/call core_cast "${_tmpSpell}" 0 ${_tmpID} FALSE
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
sub use_skill_melee(int _tmpID)
 	DEBUG ${cbug}use_skill_melee(ID ${_tmpID})\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	
	/if (!${Bool[${_tmpID}]}) /return	
	/if (${is_target_dead[${_tmpID}]}) /return
	
	/if (${Target.ID} != ${_tmpID}) {
		/target ID ${_tmpID}
		/delay 5 ${Target.ID} == ${_tmpID}
	}
	
	/if (${Target.ID} && ${Select[${combat},MELEE,RANGE]} && !${Range.Between[0,${SetFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${_tmpID} nolook ${If[${SetFaceFast},fast,]}

	| taunt /onoff agro
	/if (${Bool[${taunt}]} && ${switchAgro}) {
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
	/if (${validate_class[FALSE, |BRD|ROG]}) {
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
	/if (${validate_class[FALSE, |MON|BST]} && ${Spawn[${_tmpID}].Distance} <= 15) {
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
			VMELEE Melee ${sep} ${csp}Kick\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
			/if (${Target.ID}) /doability "Flying Kick"
		} else /if (${Bool[${roundkick}]} && !${Skill[Rounf Kick].Auto} && ${Me.AbilityReady[Round Kick]}) {
			VMELEE Melee ${sep} ${csp}Kick\ax ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax]
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
	DEBUGCAST ${cbug}cast_nuke_servant()\ax
	AMIDEAD
	CHECKEXIT
	/if (${switchPcSwarm} && ${Spawn[${Target2Kill}].LineOfSight} && (${Me.SpellReady[${ServantNuke}]} || ${Me.AltAbilityReady[${ServantNuke}]}) && ${Me.CurrentMana} >= ${Spell[${ServantNuke}].Mana} && ${Spawn[${Target2Kill}].Distance} < ${MobAgro}) {
		/if (${Me.SpellReady[${ServantNuke}]}) /call core_cast "${ServantNuke}" 0 ${Target2Kill} cast_break
		/if (${Me.AltAbilityReady[${ServantNuke}]}) /call core_cast "${ServantNuke}" alt ${Target2Kill} FALSE
	}
/return



|***
 *	DES: preps toon for combat modes
 *	USE: /call set_combat [MELEE|RANGE|NUKE|HEAL]
 *	INI: 
 *  NOTE: 
 ***|
sub set_combat(str _mode)
	DEBUG ${cbug}set_combat(${_mode})\ax
	AMIDEAD
	/if (${is_target_dead[${Target2Kill}]}) /return	
	/if (!${coreAuto} && ${Me.Invis}) /makemevisible
	UPDATEHUD "${_mode}"
	/if (${Me.State.NotEqual[STAND]}) /stand
	/if (${coreAuto} && ${Me.State.Equal[FEIGN]}) /squelch /stand
		
	/if (${Select[${_mode},MELEE]}) {
		/if (!${Range.Between[0,${SetFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
		/if (${coreAuto} && ${Bool[${Target.DSed}]} && ${stoponDS} && !${switchAgro}) {
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
		/if (!${Range.Between[0,${SetFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
		/if ((${Spawn[${Target2Kill}].LineOfSight} && !${switchOverrideLos}) && !${Me.AutoFire}) /squelch /autofire on
	} else /if (${Select[${_mode},NUKE]}) {
	} else /if (${Select[${_mode},HEAL]}) {
	}
/return



|***
 *	DES: Gift of Manna Nuke
 *	USE: /call cast_nuke_GoM
 *	INI: Toon -> [Spells] -> gomnuke, gomnuke2 gomlevel
 *  NOTE: 
 ***|
sub cast_nuke_GoM(int _tmpID)
	DEBUGCAST ${cbug}cast_nuke_GoM(ID: ${_tmpID})\ax
	| clerics have their own GOM routine in group heals
	/if (!${Select[${Me.Class.ShortName},CLR]}) {
		CHECKEXIT
		/if (${is_target_dead[${_tmpID}]}) /return
	}

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
 *	DES: Buff Raid Members in your /agro range
 *	USE: /call check_buffs_raid
 *	INI: uses INI [Buffs] 
 *  NOTE: 
 ***|
sub check_buffs_raid()
	DEBUG ${cbug}check_buffs_raid()\ax
	/if (${coreAuto}) /call create_timer timer_check_buffs_raid ${tRaid_Buff}
	/if (${coreAuto} && ${watch_for_death[]}) /return 

	| # people in raid
	/declare _i int local
	| # of buffs to cast
	/declare _x int local
	| targets buff count
	/declare _y int local
	
	/if (${Raid.Members} && ${Me.PctMana} > 10 && !${Me.Casting.ID}) {
		UPDATEHUD "BUFF: Raid"
		/for _i 1 to ${SpawnCount[raid radius ${MobAgro}]}
			DEBUG Checking Raid Buffs on [${cinfo}${Raid.Member[${_i}].DisplayName}\ax]
			/for _x 1 to ${BuffCount}
				/if (!${Bool[${RaidBuff${_x}Class}]} || !${Bool[${Buff${_x}}]}) /continue
				AMIDEAD
				CHECKEXIT
				/doevents
				CHECKTIE
				CLEARCURSOR TRUE
				/if (!${Bool[${timer_Raid_Buffs${Spell[${Buff${_x}}].ID}${Raid.Member[${_i}].ID}}]} && ${RaidBuff${_x}Class.Find[${Spawn[${Raid.Member[${_i}].ID}].Class.ShortName}]} && ${Spawn[${Raid.Member[${_i}].ID}].Type.Equal[pc]} && ${Spawn[${Raid.Member[${_i}].ID}].Distance} < ${MobAgro} && ${Me.PctMana} >= 20) {
					/if (${Stick.Active}) /squelch /stick off
					/target ID ${Raid.Member[${_i}].ID}
					/delay 1s ${Target.ID} == ${Raid.Member[${_i}].ID}
					/delay 1s ${Target.BuffCount}
					/delay 1s
					DEBUG X=${_x} class:${RaidBuff${_x}Class.Find[${Spawn[${Raid.Member[${_i}].ID}].Class.ShortName}]} .. spell:${Buff${_x}chk}
					/for _y 1 to ${Target.BuffCount}	
						AMIDEAD
						/if (!${Spell[${Target.Buff[${_y}]}].WillStack[${Spell[${Buff${_x}}]}]} && ${Target.Buff[${_y}].ID}) {
							/call create_timer timer_Raid_Buffs${Spell[${Buff${_x}chk}].ID}${Raid.Member[${_i}].ID} ${Spell[${Spell[${Buff${_x}chk}].ID}].Duration.TotalSeconds}s
							/return
						}
					/next _y
					/if (!${Target.Buff[${Buff${_x}chk}].ID}) {
						CLEARCURSOR TRUE
						VSPELL Raid Buffing ${sep} ${csp}${Buff${_x}}\ax on [${cpc}${Raid.Member[${_i}].DisplayName}\ax]
						/call MQ2Cast "${Buff${_x}}" ${buffGem} -targetid|${Raid.Member[${_i}].ID} 
					}
					/call create_timer timer_Raid_Buffs${Spell[${Buff${_x}chk}].ID}${Raid.Member[${_i}].ID} ${Spell[${Spell[${Buff${_x}chk}].ID}].Duration.TotalSeconds}s
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
 *  NOTE: need to remome the pet crap someday...
 ***|
sub cast_buffs_queue(string _sender, string _verbage)
	DEBUGCAST ${cbug}cast_buffs_queue(sender:${_sender}, request:${_verbage})\ax

  /declare _a int local 0
  /declare _i int local 0

  /for _i 1 to ${BuffCount}
    /for _a 1 to ${Buff${_i}Alias.Count[|]}
			AMIDEAD
   		/doevents
      /if (${_verbage.Find["${Buff${_i}Alias.Arg[${_a},|]}"]}) {
        | /if (${_verbage.Find["pet"]} || ${_verbage.Find[${Spawn[pc ${_sender}].Pet.DisplayName}]} || ${Spawn[pet ${_sender}].ID}) {
        |   VSPELL Buffing ${sep} ${cpc}${_sender}'s\ax Pet.
        |   /call queue_buff_add ${Spawn[pc ${_sender}].Pet.ID} ${_i}
        | } else {
          DEBUGCAST ${_sender} requested ${cinfo}${Buff${_i}}\ax
          /call queue_buff_add ${Spawn[pc ${_sender}].ID} ${_i}
        | }
      }
    /next _a
  /next _i
/return



 |***
 *	DES: will loosly tie to a specific toon. will moove close to the toon when it moves. set to timer [Timers] -> CheckTieTime
 *	USE: /tie [on/off toggle] requires /tc name [NAME] and /tc distance ##
 *	INI: 
 *  NOTE: 
 ***|
sub set_tie()
	DEBUG ${cbug}set_tie()\ax
	/if (${timer_check_Tie}) /return
	/if (${coreAuto}) /call create_timer timer_check_Tie ${tCheck_Tie}

	AMIDEAD
	/doevents
	| reasons to not tie to a toon
	/if (${SetTie} && ${Spawn[pc ${SetTieToon}].ID} && ${Spawn[pc ${SetTieToon}].Dead}) /return
	/if (${Spawn[pc ${SetTieToon}].Distance} <= ${SetTieDistance} || !${Spawn[pc ${SetTieToon}].ID}) {
		/if (${Stick.Active}) /squelch /stick off
		/if (${Me.Moving}) {
			/keypress forward
			/keypress back
		}
		/return
	}
	
	| turn off camp
	/if (${Bool[${MakeCamp}]}) {
		/squelch /makecamp off
		/call writeINI HomeSet Movement FALSE 
	}
	
	| all present and or accounted for SIR!
	/squelch /face ID ${Spawn[pc ${SetTieToon}].ID} ${If[${SetFaceFast},fast,]}
	/squelch /stick ID ${Spawn[pc ${SetTieToon}].ID} ${SetTieDistance} UW

	| FORCE EMULATE FOLLOW.. HATE DOING THIS
	| /while (${SetTie} && ${Spawn[pc ${SetTieToon}].ID} && ${SetTieLock}) {
	| 	AMIDEAD
	| 	/doevents
	| 	/delay 1
	| }


	:okweremoving
	AMIDEAD
	/doevents
	/if (${Spawn[pc ${SetTieToon}].Distance} <= ${SetTieDistance}) {
		/squelch /stick off
		/return
	}
	
	/if (!${Stick.Stopped} && ${Spawn[pc ${SetTieToon}].ID}) {
		/goto :okweremoving
	} else {
		/if (${Stick.Active}) /squelch /stick off
		/if (${Me.Moving}) {
			/keypress forward
			/keypress back
		}
	}

/return



 |***
 *	DES: Will sort out your familiar.
 *	USE: auto
 *	INI: 
 *  NOTE: /if (${Me.Pet.Body.ID}==31) /pet get lost
 ***|
sub sort_familiar()
	DEBUG ${cbug}sort_Familiar()\ax
	/call create_timer timer_Check_Familiar ${tCheck_Familiar}
	/doevents
	/call check_exit
	/if (${Macro.Return} || !${Bool[${sumFamiliar}]}) /return
	/if (!${Bool[${Me.Buff[${sumFamiliarBuff}].Duration}]}) /call core_cast "${sumFamiliar}" NULL NULL FALSE
	/if (${sumFamiliarLeave} && ${Me.Pet.Name.Find[familiar]} && ${Me.Pet.Body.ID} == 31) /pet get lost
/return



|***
 *	DES: Adjusts the XTarget for visual purposes to heal toonControl. *NO EFFECT ON HEALING
 *	USE: /call protect_always_xt & /control [toon name]
 *	INI: auto if toonControlXTslot is not FALSE in INI
 *  NOTE: 
 ***|
sub protect_always_xt()
	DEBUG ${cbug}protect_always_xt()\ax
	/if (!${validate_expansion[FALSE, |16]}) /return
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
	| /if (${Time.Date.Equal[${Ini[${INIClass},Common,InGameCashChecked,"01/01/2000"]}]}) /return
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
	/ini ${INIClass} "Common" "InGameCashChecked" "${Time.Date}"
/return



|***
*	DES: will click items and autoinventory the result on a timer.
*	USE: /call odds_and_ends
*	INI: [Odds]
*  NOTE: good for poisons, foods, whatevers.
***|
sub odds_and_ends()
	DEBUGCAST ${cbug}odds_and_ends()\ax
	CHECKEXIT
	/call create_timer timer_check_odds_and_ends ${tOdds_and_Ends}
	/declare _count int local
	/for _count 1 to 4
		/if (!${Bool[${clickOddItem${_count}}]}) /continue
		AMIDEAD
		CLEARCURSOR TRUE
		/doevents
		/if (${Me.ItemReady[${clickOddItem${_count}}]}) /call core_cast "${clickOddItem${_count}}" item 0 FALSE
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
	DEBUG ${cbug}check_exit()\ax

	| invis 
	/if (${Me.Invis}) /return TRUE
	
	| am i dead on the floor?
	/if (${Me.State.Equal[HOVER]} || ${Me.Hovering} || ${Me.Dead}) /return TRUE
	
	| did I just die and get sent to bind ?
	/if (${coreAuto} && ${Me.Buff[Revival Sickness].ID} && ${Zone.ID} == ${Me.ZoneBound.ID}) /return TRUE
	
	| /if (${coreAuto} && ${watch_for_death[]}) /return 
	
	/doevents

	| movement
	/if ((${Me.Moving} || ${Stick.Active}) && !${Select[${Me.Class.ShortName},BRD]}) /return TRUE

	| clear ToTD if it procs.
	/if (${coreAuto} && ${Select[${Me.Class.ShortName},CLR,SHM,BER]}) {
		/while (${Me.Song[Touch of the Divine].ID}) {
			/delay 3s
			/docommand /removebuff Touch of the Divine
			ECHOCHANNEL REMOVING .. Touch of the Divine
			/return TRUE
		}
	}

	| SELF BUFFS/SONGS
	/if (${Me.Song[Divine Aura].ID}) /return TRUE
	/if (${Me.Song[Bestowed Divine Aura Beza].ID} || ${Me.Song[Bestowed Divine Aura Azia].ID}) /return TRUE
	/if (${Me.Buff[Death Overcomes].ID}) /return TRUE
	/if (${Me.Buff[Fermata of Preservation].ID}) /return TRUE
	/if (${Me.Buff[Kazumi's Note of Preservation].ID}) /return TRUE

	| bad self buffs
	/if (${Me.Buff[Protection of Sul].ID}) /return TRUE
	/if (${Me.Buff[Bewilder].ID}) /return TRUE
	/if (${Me.Buff[Chaotic Bewildering].ID}) /return TRUE
	
	| SELF STATES
	/if (${Me.State.Equal[FEIGN]}) /return TRUE
	/if (${Me.Ducking}) /return TRUE
	/if (${Me.Stunned}) /return TRUE
	/if (${Me.Charmed.ID}) /return TRUE
	
	| check for ${noCastWith} buffs on self and target
	| /declare _count int local 0
	| /for _count 1 to ${noCastWith.Count[|]}
	| 	/if (${Bool[${Me.Buff[${noCastWith.Arg[${_count},|]}]}]} || ${Bool[${Target.Buff[${noCastWith.Arg[${_count},|]}]}]} || ${Bool[${Me.Song[${noCastWith.Arg[${_count},|]}]}]}) /return TRUE
	| /next _count	
	
	
	
	
	| check no cast list and mezz?
	
	/if (${coreAuto} && ${Target.ID} && ${Bool[${NoCastList.Find[${Target.DisplayName}]}]}) /return TRUE
	/if (${coreAuto} && ${Target.ID} && ${Bool[${Target.Mezzed}]} && !${switchPull}) /return TRUE

/return FALSE



|***
 *	DES: Updates Notice sction of HUD.
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub update_HUD_notices(string _mode)
	DEBUG ${cbug}update_HUD_notices()\ax
	/declare _count 				int 		local 0
	/declare _index 				int 		local 0
	/declare _sep 					bool 		local FALSE
	/declare _IV[3] 				string 	local FALSE
	/declare _IVU[1]				string 	local FALSE
	
	/varset hudNotice
	/varset hudWarning
	/varset hudTarNotice
	/if (!${timer_check_status}) /call broadcast_status
	/doevents	
	/if (${Bool[${_mode}]}) {
		/varset tmpMode "${_mode}"
	} else /if (!${Bool[${_mode}]} || ${_mode.Equal[NULL]}) {
		/varset tmpMode FALSE
	}
	
		/varset _IV[1] |Shared Perfected Camouflage|Shared Camouflage|Camouflage|Group Perfected Invisibility|Invisibility|Perfected Invisibility
		/varset _IV[2] |Phase March|Nethervoid March|Perfected Silent Presence|Cloak of Shadows|Cloak of Shadows I|Cloak of Shadows II|Shauri's Sonorous Clouding|Gather Umbra|Invisibility Cloak
		/varset _IV[3] |Perfected Natural Invisibility|Perfected Spirit Walk Invisibility|Perfected Group Spirit Walk Invisibility|Shauri's Sonorous Clouding

		/for _count 1 to ${_IV.Size}
			/for _index 1 to ${_IV[${_count}].Count[|]}
				/if (${Me.Buff[${_IV[${_count}].Arg[${_index},|]}].Name.Equal["${_IV[${_count}].Arg[${_index},|]}"]}) {
				/varset hudWarning ${hudWarning} IV
					/varset _sep TRUE
			}
			/next _index	
		/next _count

		| check for invis vs undead
		/varset _IVU[1] |Group Perfected Invisibility to Undead|Perfected Invisibility to Undead|Invisibility to Undead|Invisibility versus Undead|Shauri's Sonorous Clouding

		/for _count 1 to ${_IVU.Size}
			/for _index 1 to ${_IVU[${_count}].Count[|]}
				/if (${Me.Buff[${_IVU[${_count}].Arg[${_index},|]}].Name.Equal["${_IVU[${_count}].Arg[${_index},|]}"]}) {
					/varset hudWarning ${hudWarning} IVU 
					/varset _sep TRUE
				}
				/next _index	
		/next _count

		| override warnings
		/for _count 1 to ${overrideType.Count[|]}
			/if (${switchOverride${overrideType.Arg[${_count},|].Left[1].Upper}${overrideType.Arg[${_count},|].Right[-1].Lower}}) {
				/varset hudWarning ${hudWarning} ${overrideType.Arg[${_count},|].Left[1].Upper}${overrideType.Arg[${_count},|].Right[-1].Lower} 
				/varset _sep TRUE			
			}
		/next _count	
		
	
	| ** Notices for bottom of HUD	
	/varset _sep FALSE

	DEBUG HUD: Bottom Notices
	/if (${Select[${Me.Class.ShortName},BRD]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} ${lineCurrent}
		/varset _sep TRUE
	}	
	/if (${AdvPath.Playing}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} AdvPath
		/varset _sep TRUE
	}
	/if (${SafeZone}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} SafeZone
		/varset _sep TRUE
	}	
	/if (${Stick.Active}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} Stick
		/varset _sep TRUE
	}
	/if (${Select[${Me.Class.ShortName},MAG]}) {
		/if (${Bool[${useRain}]}) {
			/varset hudNotice ${hudNotice} ${If[${_sep},-,]} AE
			/varset _sep TRUE
		}	
	}
	/if (${Select[${Me.Class.ShortName},CLR,PAL,SHM,DRU]}) {
		/if (${Bool[${RampageTank}]}) {
			/varset hudNotice ${hudNotice} ${If[${_sep},-,]} RT[${RampageTank}]
			/varset _sep TRUE
		}	
	}

	| Mode Notice
	DEBUG HUD: Mode Notice
	/varset _sep FALSE
	/if (${Bool[${switchModeHunt}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} Hunt[${If[${setModeAgro},${setModeAgro},Zone]}]
		/varset _sep TRUE
	}		
	/if (${Bool[${switchModePetfarm}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} PetFarm
		/varset _sep TRUE
	}		
	/if (${Bool[${switchModeScorch}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} Scorch[${If[${setModeAgro},${setModeAgro},Zone]}]
		/varset _sep TRUE
	}		
	/if (${Bool[${switchModeHarvest}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} Harvest[${If[${setModeAgro},${setModeAgro},Zone]}]
		/varset _sep TRUE
	}	
	/if (${Bool[${switchModeDrag}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} Drag
		/varset _sep TRUE
	}
	/if (${Bool[${switchModeForage}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} Forage
		/varset _sep TRUE
	}
	/if (${Bool[${switchModeFish}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} Fish
		/varset _sep TRUE
	}
	/if (${Bool[${switchModeSkill}]}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} Skill Trainer
		/varset _sep TRUE
	}		
	/if (${Bool[${switchModeLush}]} || ${Me.Drunk}) {
		/varset hudNotice ${hudNotice} ${If[${_sep},-,]} Drunk[${Me.Drunk}]
		/varset _sep TRUE
	}		
		
	/varset _sep FALSE

	| WARNINGS FOR TOP OF HUD.. USE THIS SPARINGLY
	DEBUG HUD: WARNING
	/if (${switchAmIDead} || ${flagCallNeedRez}) {
		/varset hudWarning NEED REZ
	}	


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

/return



|***
 *	DES: AA Slow
 *	USE: /call cast_AA_slow [MOBID] [AA SLOW NAME]
 *	INI: 
 *  NOTE: 
 ***|
sub cast_AA_slow(int _tmpID, string _tmpSpell)
	DEBUG ${cbug}cast_AA_slow(${_tmpID}, ${_tmpSpell})\ax
	/if (${timer_slow_${_tmpID}}) /return
	AMIDEAD
	CHECKEXIT
	/doevents

	| check for overwrite slows
	/declare _tmpSlow string local ${Target.Slowed}
	/if (${Target.Slowed.Duration}) {
		/if (${_tmpSlow.Find[Undermining]}) /call create_timer timer_slow_${_tmpID} ${Target.Slowed.Duration.TotalSeconds}s
		/return
	}

	/declare _resistCount int local 0
	
	/if (${Spawn[${_tmpID}].Type.NotEqual[PC]} && ${Spawn[${_tmpID}].Distance} <= ${Spell[${_tmpSpell}].MyRange} && !${timer_slow_${_tmpID}}) {
		AMIDEAD
		/call core_cast "${_tmpSpell}" alt ${_tmpID} FALSE
		/if (${Select[${castReturn},CAST_SUCCESS,CAST_IMMUNE,CAST_NOHOLD,CAST_TAKEHOLD]}) {
			/call create_timer timer_slow_${_tmpID} ${Spell[${_tmpSpell}].Duration.TotalSeconds}s
		} else /if (${Select[${castReturn},CAST_RESISTED,CAST_RESIST]}) {
			/varcalc _resistCount ${_resistCount}+1
			/if (${_resistCount} >= 3) /call create_timer timer_slow_${_tmpID} ${Spell[${_tmpSpell}].Duration.TotalSeconds}m
		}
	}
/return



|***
 *	DES: will cast Temp pet
 *	USE: /call cast_temp_pet [MOBID] [Temp Spell Name]
 *	INI: 
 *  NOTE: 
 ***|
sub cast_temp_pet(int _tmpID, string _tmpSpell)
	DEBUGCAST ${cbug}cast_temp_pet(ID:${_tmpID}, Spell:${_tmpSpell})\ax
	AMIDEAD
	CHECKEXIT
	/if (!${switchPcPet} || ${Me.Pet.ID}) /return
	/if (${is_target_dead[${_tmpID}]}) /return
	/if (${Spawn[${_tmpID}].Distance} < ${MobAgro} && ${Spawn[${_tmpID}].PctHPs} <= ${sendPetPerc}) {
		VOUT Sending ${cpc}${_tmpSpell}\ax after [${If[${Bool[${Target.ID}]},${ctar}${Target.DisplayName}\ax,${cbad}BAD TARGET\ax]}] ${Target.Distance} ${If[${Spawn[${Target}].LineOfSight},${cgood}LoS\ax,${cbad}LoS\ax]}
		/call MQ2Cast "${_tmpSpell}" -targetid|${_tmpID}
	}
/return



|***
 *	DES: Cleric/Paladin AA Yaulp
 *	USE: /call cast_AA_yaulp
 *	INI: [selfbuff] - doYaulp
 *  NOTE: if doYaulp is TRUE, will use at 98% manna or less.
 ***|
sub cast_yaulp()
	DEBUGCAST ${cbug}cast_yaulp()\ax
	/if (${Me.Sitting} || !${Select[${Me.CombatState},COMBAT]}) /return
	AMIDEAD
	CHECKEXIT
	
	| use spell Yaulp
	/if (${Bool[${spellYaulp}]} && ${Me.SpellReady[${spellYaulp}]} && ${Spell[${spellYaulp}].Stacks}) {
		/if (${Me.PctMana} < ${useYaulpPct}*.${tmpHealOverride} || (${Me.Combat} && ${Select[${Me.Class.ShortName},PAL]})) /call core_cast "${spellYaulp}" 0 0 FALSE
	
	| use Donals Helm Yaulp
	} else /if (${Select[${Me.Class.ShortName},CLR]} && ${Me.ItemReady[Darkened Donal's Helm of Mourning]}) {
		/if (!${Spell[Yaulp XIV Rk. II].Stacks} || ${Bool[${Me.Buff[Lesser Yaulp]}]} || ${Bool[${Me.Buff[Yaulp]}]}) /return
		/if (${Me.PctMana} < ${useYaulpPct}*.${tmpHealOverride}) /call core_cast "Darkened Donal's Helm of Mourning" item 0 FALSE
	
	| use AA yaulp
	} else /if (!${FindItem[Darkened Donal's Helm of Mourning].ID} && ${Me.AltAbilityReady[Yaulp (Lesser)]}) {
		/if (!${Spell[Yaulp (Lesser)].Stacks} || ${Bool[${Me.Buff[Lesser Yaulp]}]} || ${Bool[${Me.Buff[Yaulp]}]}) /return
		/if (${Me.PctMana} < ${useYaulpPct}*.${tmpHealOverride} || (${Me.Combat} && ${Select[${Me.Class.ShortName},PAL]})) /call core_cast "Yaulp (Lesser)" alt 0 FALSE
	}
/return



|***
 *	DES: uses melee disc breather
 *	USE: /call cast_disc_breather
 *	INI: 
 *  NOTE: 
 ***|
sub cast_disc_breather()
 	DEBUGCAST ${cbug}cast_disc_breather(ready: ${Me.CombatAbilityReady[${DiscRest}]})\ax
 	/if (${SafeZone} && !${switchOverrideSafezone} || !${Me.CombatAbilityReady[${DiscRest}]} || ${switchAmIDead}) /return
	/if (${Me.PctEndurance} > 33) /return
	AMIDEAD
	/doevents

	/declare _endurancePoint int local 0
 	
	/if (${Me.CombatAbility[Breather Rk. III]}) /varset _endurancePoint 29
	/if (${Me.CombatAbility[Breather Rk. II]}) /varset _endurancePoint 25
	/if (${Me.CombatAbility[Breather]}) /varset _endurancePoint 21
	
	| rogue trigger breather while in combat
	/if (${validate_class[FALSE, |ROG]}) {
		/if (${Select[${Me.CombatState},COMBAT]} && ${Me.PctEndurance} < ${_endurancePoint} && ${Me.AltAbilityReady[Escape]}) {
			/call clear_combat
			/call set_stop TRUE
			/call core_cast "Escape" alt 0 FALSE
			/delay 1
			/call core_cast "${DiscRest}" disc 0 FALSE			
			/return
		}
	}

	| need to be out of combat to use
 	/if (!${Select[${Me.CombatState},RESTING,COOLDOWN]}) /return

 	DEBUG ${Me.PctEndurance} < ${_endurancePoint} && !${Bool[${Me.ActiveDisc}]}
 	/if (${Me.PctEndurance} < ${_endurancePoint} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscRest}" disc 0 FALSE
/return



|***
 *	DES: corrects the Gem## to simply a #
 *	USE: /call fix_gem_id [gem##]
 *	INI: 
 *  NOTE:
 ***|
sub fix_gem_id(string _spellGem)
	DEBUGCAST ${cbug}fix_gem_id(fix: ${_spellGem})\ax
	/declare _gem int local 0
	/if (${_spellGem.Length} == 1) {
		/varset _gem ${_spellGem.Right[1]}
	} else /if (${_spellGem.Length} == 2) {
		/varset _gem ${_spellGem.Right[2]} 
	} else /if (${_spellGem.Length} == 4) {
		/varset _gem ${_spellGem.Right[1]}
	} else /if (${_spellGem.Length} == 5) {
		/varset _gem ${_spellGem.Right[2]}
	}
	DEBUG fix_gem_id(return: ${_gem})
/return ${_gem}



|***
 *	DES: Memorize a spell/song
 *	USE: /call mem_spell [T|F echo memming] [spell name] [gem to mem it in] [TRUE|FALSE wait for recycle to cast]
 *	INI: 
 *  NOTE: 
 ***|
sub mem_spell(bool _echo, string _spellname, string _spellgem, bool _wait)
 	DEBUGCAST ${cbug}mem_spell("${_spellname}", gem:${_spellgem}, wait:${_wait})\ax
 	/if (${Me.Gem[${_spellname}]}) /return TRUE
	AMIDEAD

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
	DEBUGCAST ${_gem} "${Spell[${_spellname}].RankName}"
	/memspell ${_gem} "${Spell[${_spellname}].RankName}"
	/if (${_wait}) {
		:waitforgem
		/delay 10s ${Me.SpellReady[${_spellname}]}
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
 	DEBUG ${cbug}tell_resident_mage(request: ${_request})\ax
	/if (${Select[${Me.Class.ShortName},MAG]}) /return
	AMIDEAD
	CHECKEXIT
	/declare _count int local 0
	
	| cycle through the mages
	/for _count 1 to ${ResidentMage.Count[|]}
		DEBUG  mage: ${ResidentMage.Arg[${_count},|]}
		| skip if FALSE
		/if (!${Bool[${ResidentMage.Arg[${_count},|]}]}) /continue

		/if (${Spawn[pc ${ResidentMage.Arg[${_count},|]} zradius 150 radius 150].ID}) {

			| ask for weapons
			/if (${_request.Equal[weapons]} && !${Bool[${timer_ask_for_weapons_${ResidentMage.Arg[${_count},|]}}]} && !${Bool[${FindItem[${summonedWepClosedBag}]}]}) {
				DEBUG Resident Mage: ${ResidentMage.Arg[${_count},|]} timer: timer_ask_for_weapons_${ResidentMage.Arg[${_count},|]}
				/t ${ResidentMage.Arg[${_count},|]} weapons
				/call create_timer timer_ask_for_weapons_${ResidentMage.Arg[${_count},|]} ${tAsk_for_weapons}
			}
			
			| ask for ModRod - shard
			/if (${_request.Equal[shard]} && !${Bool[${timer_ask_for_rod_${ResidentMage.Arg[${_count},|]}}]} && !${Bool[${FindItem[Modulation Shard]}]}) {
				DEBUG Resident Mage: ${ResidentMage.Arg[${_count},|]} timer: timer_ask_for_rod_${ResidentMage.Arg[${_count},|]}
				/t ${ResidentMage.Arg[${_count},|]} shard
				/call create_timer timer_ask_for_rod_${ResidentMage.Arg[${_count},|]} ${tAsk_for_rod}
			}
		}
	/next _count	
/return



|***
 *	DES: Res routines for the healer slobs.
 *	USE: /call check_for_corpse
 *	INI: 
 *  NOTE: looks for corpses.. rez them
 ***|
sub check_for_corpse()
	DEBUGREZ ${cbug}check_for_corpse()\ax
	| AMIDEAD
	| /if (${watch_for_death[]}) /return
	
	/if (${Me.State.Equal[HOVER]} || ${Me.Hovering} || ${Me.Dead}) /return
	
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
	/if (${switchRezEmergency} && (!${Me.ItemReady[Token of Resurrection]} || ${Spawn[pc CLR radius ${maxRezRange}].ID})) /return FALSE
	
	| necro stuff
	/if (${Select[${Me.Class.ShortName},NEC]}) {
		/if (!${FindItem[Essence Emerald].ID}) /return FALSE
		/declare _rezNEC						string	local "Convergence"
	}

	| cleric stuff
	/if (${Select[${Me.Class.ShortName},CLR]}) {
		/declare _divinerezCLR			string	local "Divine Resurrection"
		/declare _rezCLR						string	local "Blessing of Resurrection"
	}
	
	| pally, shaman, druid stuff
	/if (${Select[${Me.Class.ShortName},PAL,SHM,DRU]}) {
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

	| make the damn variables.
	/for _count 1 to ${SpawnCount[pccorpse radius ${maxRezRange}]}
		/varset _tmpID ${NearestSpawn[${_count},pccorpse radius ${maxRezRange}].ID}
		
		| for the people that suck ass (not in a good way)
		/if (${rezlist.Find[${Spawn[${_tmpID}].Name}]}) {
			/if (!${Defined[timer_corpse_${_tmpID}]}) /declare timer_corpse_${_tmpID} timer outer 7200
			/continue
		}	
		
		/if (!${Defined[timer_corpse_${_tmpID}]}) /declare timer_corpse_${_tmpID} timer outer 1
	/next _count

	UPDATEHUD "REZZING"
	| clerics first group or raid
	/if (${SpawnCount[pccorpse cleric raid radius ${maxRezRange}]} && ${Raid.Members} || ${SpawnCount[pccorpse cleric group radius ${maxRezRange}]} && ${Bool[${Group.Members}]}) {
		/for _count 1 to ${SpawnCount[pccorpse cleric raid radius ${maxRezRange}]}
		AMIDEAD
		/if (!${timer_corpse_${NearestSpawn[${_count},pccorpse cleric raid radius ${maxRezRange}].ID}}) {
			/varset _tmpID ${NearestSpawn[${_count},pccorpse cleric raid radius ${maxRezRange}].ID}
			/if (${DivineRezCleric}) {
				/if (${Me.AltAbilityReady[${_divinerezCLR}]}) /call cast_rez ${_tmpID} ${_divinerezCLR}		
			} else /if (${Me.AltAbilityReady[${_rez${Me.Class.ShortName}}]}) {
				/call cast_rez ${_tmpID} ${_rez${Me.Class.ShortName}}
			} else /if (${switchRezEmergency} && ${RezTokenList.Find[${Spawn[${_tmpID}].Class.ShortName}]}) {
				/call cast_rez ${_tmpID} ${_rezEmergency}
			}
		}
		/next _count
	} 
	
	| control toon
	/if (${SpawnCount[pccorpse radius ${maxRezRange} ${toonControl}]}) {
		/for _count 1 to ${SpawnCount[pccorpse radius ${maxRezRange} ${toonControl}]}
		AMIDEAD
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
	
	| tanks
	/if (${SpawnCount[pccorpse raid radius ${maxRezRange} tank]} && ${Raid.Members}) {
		/for _count 1 to ${SpawnCount[pccorpse raid radius ${maxRezRange} tank]}
		AMIDEAD
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
	/if (${SpawnCount[pccorpse raid radius ${maxRezRange}]} && ${Raid.Members}) {
		/for _count 1 to ${SpawnCount[pccorpse raid radius ${maxRezRange}]}
		AMIDEAD
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
	
	| guildies - all the time
	/if (${SpawnCount[pccorpse radius ${maxRezRange} guild]} && !${Raid.Members}) {
		/for _count 1 to ${SpawnCount[pccorpse radius ${maxRezRange} GUILD]}
		AMIDEAD
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
	/if (${SpawnCount[pccorpse radius ${maxRezRange} group]} && !${Raid.Members}) {
		/for _count 1 to ${SpawnCount[pccorpse radius ${maxRezRange} group]}
		AMIDEAD
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
	/if (${SpawnCount[pccorpse radius ${maxRezRange}]} && ${switchRezEveryone}) {
		/for _count 1 to ${SpawnCount[pccorpse radius ${maxRezRange}]}
		AMIDEAD
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

	UPDATEHUD FALSE
/return



|***
 *	DES: rez/call casting code.
 *	USE: /call cast_rez [ID] [rez aa|token]
 *	INI: 
 *  NOTE: 
 ***|
sub cast_rez(int _tmpID, string _rezAA)
	DEBUGCAST ${cbug}cast_rez(${_tmpID}, ${_rezAA})\ax
	DEBUGREZ ${cbug}cast_rez(${_tmpID}, ${_rezAA})\ax
	/if (${Me.Dead}) /return
	/if (${timer_corpse_${_tmpID}}) /return
	AMIDEAD
	CHECKEXIT
		

	| delay res a sec if they are not on the safe list.
	/if (!${safelist.Find[${Spawn[${_tmpID}].DisplayName}]} || ${Select[${Me.Class.ShortName},DRU,SHM]}) /delay ${Int[${Math.Calc[${Math.Rand[${RandRezDelay}]}+1]}]}s

	| ignore the people we dont like
	/if (${Rezlist.Find[${Spawn[${_tmpID}].DisplayName}]}) {
		/call create_timer timer_corpse_${_tmpID}} 30m
		/return
	}
		
	| get the corpse close to us
	/if (${Spawn[${_tmpID}].Distance} > 15 && ${Spawn[${_tmpID}].Distance} <= ${maxRezRange}) {
		/squelch /target ID ${_tmpID}
		/squelch /corpse
	}
	
	| can we AA res or token?
	/if (${Me.AltAbilityReady[${_rezAA}]} || (${Me.ItemReady[${_rezAA}]} && ${switchRezEmergency})) {	

		| res the damn person
		/if (!${switchRezEmergency}) {
			/call core_cast "${_rezAA}" alt ${_tmpID} FALSE
		} else /if (${switchRezEmergency}) {
			/call core_cast "${_rezAA}" item ${_tmpID} FALSE
		}
	
		/if (${Select[${castReturn},CAST_SUCCESS]}) { 
			/call create_timer timer_corpse_${_tmpID} ${tRez_Retry}
			ECHOCHANNEL ${Me.Class.ShortName} Rez: ${Spawn[${_tmpID}].DisplayName}
		} else  /if (${Select[${castReturn},CAST_CANCELLED,CAST_INTERRUPTED]}) {
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



|***
 *	DES: will set your group to combat ready with 1 key.
 *	USE: /here .. 
 *	INI: 
 *  NOTE: this is designed for a fast group set due to agro while traveling. This will not affect the sender of the command
 ***|
sub set_combat_here()
 	DEBUG ${cbug}set_combat_here()\ax
 	AMIDEAD
	/if (!${Bool[${Group.Members}]}) /return
	UPDATEHUD "HERE"

	VOUT ${owarning} ${sep} COMBAT ${sep} Right NOW!
	
	| shut the bard up..
	/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop

	| clear all pending events
	/doevents flush

	| clear all other modes
	/call switch_clear_mode
	
	| reset timers for safety
	/if (${switchBuffItem}) /call create_timer timer_check_buffs_item ${defaultTimerDelay}
	/if (${switchBuffSelf}) /call create_timer timer_check_buffs_self ${defaultTimerDelay}
	/if (${switchBuffRaid}) /call create_timer timer_check_buffs_raid ${defaultTimerDelay}
	/if (${switchBuffBc}) /call create_timer timer_check_buffs_BC ${defaultTimerDelay}
	/if (${switchBuffPet}) /call create_timer timer_check_pet_buffs ${defaultTimerDelay}
	/if (${Bool[${Aura1Buff}]} || ${Bool[${Aura2Buff}]}) /call create_timer timer_check_aura ${defaultTimerDelay}
	/if (${switchBuffTell}) /call create_timer timer_check_buffs_tell ${defaultTimerDelay}
	
	/if (${Select[${Me.Class.ShortName},CLR]}) {
		/call create_timer timer_heal_ward ${tHealWard}
	}

	/if (${Me.Casting.ID}) /stopcast

	
	| stop 'all' the movement options
	AMIDEAD
	| stop sticking to stuff
	/if (${Stick.Active}) /squelch /stick off
	/if (${tempsnapstuck}) /varset tempsnapstuck FALSE
	/if (${SetTie}) /varset SetTie FALSE
	
	| stop advanced path
	/if (${AdvPath.Following}) /squelch /afollow off

	| stop moving
	/if (${Me.Moving}) {
		/keypress forward
		/keypress back
	}
 	
 	| fix zone
 	/call check_zone_INI FALSE
 	
	| /call clear_combat
	/if (${switchPcPet} && ${Me.Pet.ID} && ${Bool[${Me.Pet.Target}]}) /pet back off

	| make us visible 
	/if (${makeVisible} && ${Me.Invis}) /makemevisible
	
	| get shit out of our hands
	/call sort_cursor TRUE
	
	| clear the current target
	/if (${clearTarget} && ${Target.ID}) /squelch /target clear
	
	| make sure we are turned on  !?!?!
	/if (!${coreAuto}) /call writeINI coreAuto Common TRUE
	
	| wake the Main Assist up from its nap, set him in place and group assists
	/if (${Me.DisplayName.Equal[${GroupAssist}]}) {
		/if (!${HomeSet}) {
			/call writeINI HomeSet Movement TRUE
			/call set_home
		}
		| cut the agro way down for safety
		/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${MobAgro}]} > 2) {
			/if (${baseAgroRange} && ${MobAgro} > ${baseAgroRange}) /agro ${baseAgroRange}
		}	
		/if (!${switchAgro}) /call writeINI switchAgro Common TRUE 
		/if (!${switchPull}) /call writeINI switchPull Common TRUE 
		/if (${zradius} != ${hereZRadius}) /cc zradius ${hereZRadius}	
	} 
	
	| set assists
	/if (!${Bool[${Raid.Members}]} && !${Select[${Me.Class.ShortName},CLR]} && !${switchPull} && !${useSmartAssistG}) {
		/smartma G
 	} else /if (${Bool[${Raid.Members}]} && !${Select[${Me.Class.ShortName},CLR]} && ${switchPull} && !${useSmartAssistR}) {
		/smartma R
	}	else /if (${Select[${Me.Class.ShortName},CLR]} && (${useSmartAssistG} || ${useSmartAssistR}) || ${switchPull}) {
		/smartma X
	}
	
	| set class specific stuff
	| /combat ${combat${Me.Class.ShortName}}

	UPDATEHUD FALSE	
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
	DEBUGARRAY ${cbug}list_add(list: ${_listName}, ID:${_tmpID})\ax
	/if (!${Bool[${${_listName}.Find[${_tmpID}]}]}) {
		/varset ${_listName} ${${_listName}}|${_tmpID}
		/varset _exit 1
		DEBUGARRAY addlist#:${addList.Count[|]} - List:${addList}
	}
/return ${_exit}



|***
 *	DES: delete from a list
 *	USE: /call list_delete [LISTNAME] [ID|MOBNAME]
 *	INI: 
 *  NOTE: 
 ***|
sub list_delete(string _listName, string _tmpID)
	DEBUG ${cbug}list_delete(list: ${_listName}, ID:${_tmpID})\ax
	/if (${Bool[${${_listName}.Equal[NULL]}]}) /return
	/if (!${Bool[${_tmpID}]} || ${_tmpID} == 0) /return
	/if (!${Bool[${${_listName}.Find[${_tmpID}]}]}) /return

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
/return



|***
 *	DES: update a list
 *	USE: /call list_update [mobList|addList] [ID]
 *	INI: 
 *  NOTE: 
 ***|
sub list_update(string _listName, string _tmpID)
	DEBUGARRAY ${cbug}list_update(list: ${_listName}, ID:${_tmpID})\ax
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
/return



|***
 *	DES: checks for adds in the local area, calls updates to the needed lists.
 *	USE: /call check_for_adds
 *	INI: 
 *  NOTE: 
 ***|
sub check_for_adds()
	DEBUGARRAY ${cbug}check_for_adds\ax
	/declare _tmpID				string	local
	/declare _numMobs			int			local 1

	:loopnextmob
	/if (${NearestSpawn[${_numMobs},npc radius ${MobAgro} targetable los].ID}) {
	| /if (${NearestSpawn[${_numMobs},npc radius ${MobAgro} targetable los playerstate 4].ID}) {
		/varset _tmpID ${NearestSpawn[${_numMobs},npc radius ${MobAgro} targetable ${If[${switchOverrideLos},,los]} playerstate 4].ID}
			/if (${switchdeBuffEradicate} && !${Defined[timer_eradicate_${_tmpID}]}) /call create_timer timer_eradicate_${_tmpID} 1
			/if (${switchdeBuffTash} && !${Defined[timer_tash_${_tmpID}]}) /call create_timer timer_tash_${_tmpID} 1
			/if (${switchdeBuffSlow} && !${Defined[timer_slow_${_tmpID}]}) /call create_timer timer_slow_${_tmpID} 1
			/if (${switchdeBuffCripple} && !${Defined[timer_cripple_${_tmpID}]}) /call create_timer timer_cripple_${_tmpID} 1
			/if (${switchdeBuffMezz} && !${Defined[timer_mez_${_tmpID}]}) /call create_timer timer_mez_${_tmpID} 1
		| /if (${Spawn[id ${_tmpID}].Type.Equal[NPC]} && ${Spawn[id ${_tmpID}].Distance}<=${MobAgro} && ${Spawn[id ${_tmpID}].LineOfSight} && !${ExcludeList.Find[${Spawn[id ${_tmpID}].DisplayName}]} && ${Spawn[${_tmpID}].DisplayName.Length}>1 && ${Spawn[${_tmpID}].Type.NotEqual[Untargetable]}) {
		/if (${Spawn[id ${_tmpID}].Type.Equal[NPC]} && ${Spawn[id ${_tmpID}].Distance}<=${MobAgro} && ${Spawn[id ${_tmpID}].LineOfSight} && !${ExcludeList.Find[${Spawn[id ${_tmpID}].DisplayName}]} && ${Spawn[id ${_tmpID}].Type.NotEqual[Untargetable]}) {
			/if (!${timer_mez_${_tmpID}}) {
				/if (${_tmpID} != ${Me.XTarget[1].ID}) /call list_add addList ${_tmpID}
				/call list_add mobList ${_tmpID}
				/if (${Macro.Return.Equal[1]}) {
					VOUT ${owarning} ADD ${sep} [${ctar}${Spawn[${_tmpID}].DisplayName}\ax] ${sep} [${ctar}${_tmpID}\ax]
					/if (${Me.Casting.Name.NotEqual[${spellMez}]} && ${Me.Casting.ID}) {
						/if (${validate_plugin[FALSE, |mq2cast]}) /call Interrupt
						/stopcast
					}
					/if (${NoMezzList.Find[${Spawn[${_tmpID}].DisplayName}]}) {
						VOUT ${owarning} IMMUNE TO\ax ${csp}${spellMez}\ax ${sep} [${ctar}${_tmpID}\ax]
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
 *	DES: adds /t for buffs from queue
 *	USE: uses INI [Buffs] 
 *	INI: 
 *  NOTE: 
 ***|
sub queue_buff_add(int _buffee, int _buffnumber)
	DEBUGARRAY ${cbug}queue_buff_add(${_buffee}, ${_buffnumber})\ax
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
	DEBUGARRAY ${cbug}queue_buff_delete(${_buffee}, ${_buffnumber})\ax
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
	/if (${coreAuto} && !${switchBuffCombat} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]}) /return
	/if (${coreAuto} && ${watch_for_death[]}) /return 

	UPDATEHUD "BUFF: Tell"
	/doevents
	/declare _count					int local
	/declare _tmpVAR				int local ${Math.Calc[${BuffCount}]}
	/declare _tmpID					int local 0
	/if (${coreAuto}) /call create_timer timer_check_buffs_tell ${tTell_Buff}

	/for _count 1 to ${queueCount}
		/if (!${Range.Between[1,${queueCount}:${Int[${_count}]}]}) /continue
		/if (${coreAuto} && !${switchBuffCombat} && ${SpawnCount[npc radius ${combatRadius} playerstate 4 targetable]}) /break


		DEBUGARRAY Tell queue count: ${queueCount}
		AMIDEAD
		CHECKEXIT
		/if (${queueList[${_count},2]} < ${_tmpVAR} && ${Spawn[${queueList[${_count},1]}].ID} && !${Spawn[${queueList[${_count},1]}].Dead} && (${Spawn[${queueList[${_count},1]}].Distance} <  ${Spell[${Buff${queueList[${_count},2]}}].MyRange} || ${Spawn[${queueList[${_count},1]}].Distance} < ${Spell[${Buff${queueList[${_count},2]}}].AERange}) && ${Me.CurrentMana} > ${Spell[${Buff${_tmpVAR}}].Mana}) {
			/varset _tmpID ${queueList[${_count},1]}
			/varset _tmpVAR ${queueList[${_count},2]}
		}
	/next _count
	
	| /if (${_tmpID}) /if ((${Me.SpellReady[${Buff${_tmpVAR}}]} || !${Me.Gem[${Buff${_tmpVAR}}]} || ${Spell[${Buff${_tmpVAR}}].RecastTime} < 30) || (${BuffGem${_tmpVAR}.Equal[item]} && ${FindItem[${Buff${_tmpVAR}}].TimerReady} < 30)) {
	/if (${_tmpID}) /if ((${Me.SpellReady[${Buff${_tmpVAR}}]} || !${Me.Gem[${Buff${_tmpVAR}}]} || ${Spell[${Buff${_tmpVAR}}].RecastTime} < 30) || (${FindItem[${Buff${_tmpVAR}}].TimerReady} < 30)) {
	| /if (${_tmpID}) /if ((${Me.SpellReady[${Buff${_tmpVAR}}]} || ${Spell[${Buff${_tmpVAR}}].RecastTime} < 30) || ${Me.ItemReady[${Buff${_tmpVAR}}]}) {
		/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop
		/target id ${_tmpID}
		/delay 5s ${Target.ID}==${_tmpID}
		/if (${Target.ID}!=${_tmpID} || !${Target.ID}) /return
		/if (${Me.ItemReady[${Buff${_tmpVAR}}]}) /call core_cast "${Buff${_tmpVAR}}" item ${_tmpID} FALSE
		/call core_cast "${Buff${_tmpVAR}}" ${buffGem} ${_tmpID} FALSE

		/if (${Select[${castReturn},CAST_SUCCESS]}) /call queue_buff_delete ${_tmpID} ${_tmpVAR}
	}
	
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
	DEBUG ${cbug}broadcast_status()\ax
	/declare _count 			int 		local
	/declare _build				string 	local 

	| create your toon Core.INI entry
	/declare _INIstatus		string 		local ${Ini[${INICore},"transmit_status","${Me.DisplayName}"]}

	| adjust the INI to your current status for youself and others If it is different
	/if (${setBroadcast.Equal[TRUE]} && ${_INIstatus.NotEqual[DISABLED]}) {
		/if (${_INIstatus.NotEqual[${coreAuto}]}) /ini "${INICore}" "transmit_status" "${Me.DisplayName}" "${coreAuto}"
	}

	| creat the hud display list relative to who has coreAuto OFF and is a group member
	/for _count 1 to ${NetBots.Counts}
		/varset _INIstatus ${Ini[${INICore},"transmit_status","${NetBots.Client[${_count}]}"]}
		/if (${_INIstatus.Equal[DISABLED]}) /continue
		
		/if (!${Bool[${_INIstatus}]} && ${Group.Member[${NetBots.Client[${_count}]}].ID}) {
			/varset _build ${_build} ${If[${_INIstatus},,${NetBots.Client[${_count}].Left[3]}]}
		}
		
	/next _count

	/varset hudBroadcast ${_build}
	/call create_timer timer_check_status ${tCheck_status}
/return



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
	/varset _itemName ${AdvLoot.SList[1].Name}
	VOUT Cursor ${sep} ${cinfo}${_itemName}\ax -> ${oleave} 
	/ini "${INILoot}" "${_itemName.Left[1]}" "${_itemName}" Leave
	/doevents flush
/return

sub do_loot()
 	DEBUG ${cbug}do_loot()\ax
	AMIDEAD
	/doevents
	/if (${Stick.Active} || !${Me.FreeInventory} || !${Me.UseAdvancedLooting}) /return

	| I am not the raid loot master /sigh
	/if (${Raid.Members} && ${Raid.MasterLooter.ID} != ${Me.ID} && !${switchOverrideLoot}) /return
	/if (!${Raid.Members} && ${Bool[${Group.Members}]} && ${Group.MasterLooter.ID} != ${Me.ID} && !${switchOverrideLoot}) /return


	UPDATEHUD "LOOT"

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
			DEBUG ${_scount}/${AdvLoot.SCount} ${AdvLoot.SList[${_scount}].Name} = ${_advlChoice}, ${_advlPerson}


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
				DEBUG SharedList: ${_scount} ${_advlPerson}
					/delay 1s !${AdvLoot.LootInProgress}
					/advloot shared ${_scount} giveto ${_advlPerson}
					/delay 1
			
			| ditch the item if we need too
			} else /if (${Select[${_advlChoice},Leave,Announce]} || ${FindItem[${_advlItem}].Lore} || ${FindItemBank[${_advlItem}].Lore}) {
				DEBUG SharedList: ${_advlChoice} is set to Leave/Announce.. 
				/if (${Select[${_advlChoice},Announce]}) ECHOCHANNEL Loot: Leaving ${_advlItem}
				/delay 1s !${AdvLoot.LootInProgress}
				/advloot shared ${_scount} leave
				/delay 1
				/continue

			| or we keep it.
			} else /if (${AdvLoot.SList[${_scount}].ID}) {
				DEBUG SharedList: ${_scount} ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"]} = ${_advlChoice}
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
			DEBUG ADVL: ${_advlItem} - ${ctar}${_advlChoice}\ax
			
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
				/varset _advlChoice ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"]}
			} 
			
			
			/if (${Select[${_advlChoice},Destroy]}) {
				/delay 1s !${AdvLoot.LootInProgress}
				/advloot personal ${_pcount} loot
				DEBUG ADVL looting: ${_advlItem}
				/delay 1s !${AdvLoot.LootInProgress}
				/delay 5
				/if (${FindItem[${_advlItem}].ID}) {
					
| evil delete code. had to change this. like > 9000 times 
					:loopfind_advlItem
					/delay 5
					| /nomodkey /shift /itemnotify ${FindItem[=${_advlItem}].InvSlot} leftmouseup
					/nomodkey /shift /itemnotify in Pack${Math.Calc[${FindItem[=${_advlItem}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${_advlItem}].ItemSlot2}+1]} leftmouseup
					DEBUG ADVL finding: Pack${Math.Calc[${FindItem[=${_advlItem}].ItemSlot}-22]} - ${ctar}${_advlItem}\ax
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
				DEBUG PList: ${_pcount} ${Ini[${INILoot},"${_advlItem.Left[1]}","${_advlItem}"]} = ${_advlChoice}
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
	DEBUG ${cbug}vendor_buy(Buy: ${_itemtoBuy}, #:${_buyAmount})\ax
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
	/doevents
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
	DEBUG ${cbug}vendor_sell(sell: ${_itemtoSell})}\ax
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
	DEBUG ${cbug}vendor_sell_item(item: ${_itemtoSell})\ax
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
	DEBUG ${cbug}check_AA_now()\ax
	AMIDEAD
	CHECKEXIT

	| nowAA1
	/if (${Bool[${AANow1}]} && ${Me.AltAbilityReady[${AANow1}]} && ${Spell[${AANow1}].Stacks}) {
		/if (!${Bool[${Me.Buff[${AANow1}]}]} && !${Bool[${Me.Song[${AANow1}]}]}) /call core_cast "${AANow1}" alt 0 FALSE
	}

	| nowAA2
	/if (${Bool[${AANow2}]} && ${Me.AltAbilityReady[${AANow2}]} && ${Spell[${AANow2}].Stacks}) {
		/if (!${Bool[${Me.Buff[${AANow2}]}]} && !${Bool[${Me.Song[${AANow2}]}]}) /call core_cast "${AANow2}" alt 0 FALSE
	}	
/return



|***
 *	DES: Make sure the Disc is up all the time
 *	USE: /call check_disc_now
 *	INI: Toon.INI [NowDisc] ->
 *  NOTE: This is hardcoded for only 2 skills.
 ***|
sub check_disc_now()
	DEBUG ${cbug}check_disc_now()\ax
	AMIDEAD
	CHECKEXIT

	| nowDisc1
	/if (${Bool[${DiscNow1}]} && ${Me.CombatAbilityReady[${DiscNow1}]} && ${Spell[${DiscNow1}].Stacks}) {
		/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop
		/if (!${Bool[${Me.Buff[${DiscNow1.Left[-7]}]}]} && !${Bool[${Me.Song[${DiscNow1.Left[-7]}]}]}) /call core_cast "${DiscNow1}" disc 0 FALSE
	}
	| nowDisc2
	/if (${Bool[${DiscNow2}]} && ${Me.CombatAbilityReady[${DiscNow2}]} && ${Spell[${DiscNow2}].Stacks}) {
		/if (${Select[${Me.Class.ShortName},BRD]} && ${Me.Casting.ID}) /call pulse_stop
		/if (!${Bool[${Me.Buff[${DiscNow2.Left[-7]}]}]} && !${Bool[${Me.Song[${DiscNow2.Left[-7]}]}]}) /call core_cast "${DiscNow2}" disc 0 FALSE
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
	DEBUG ${cbug}get_target()\ax
	/if (!${switchPull} || !${coreAuto}) /return
	AMIDEAD
	/declare _tmpID int local
	/declare _tmpRange int local ${Math.Calc[${MobAgro}/10]}
	/declare _count int local
	/declare _mobHP int local 100
	/declare _priorityMob bool local FALSE

	| time to step the agro range out so we get mobs closer first	
	/while (${_tmpRange} <= ${MobAgro} && !${Target2Kill}) {
		DEBUG range search:${_tmpRange}

		| search local ${MobAgro} range in steps of 10 for mob acquisition
		/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${_tmpRange} zradius ${zradius} targetable]}) {
			DEBUG mob found within:${_tmpRange} 
			/for _count 1 to ${SpawnCount[npc ${If[${switchOverrideLos},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${_tmpRange} zradius ${zradius} targetable]}
				/if (!${switchPull} || !${coreAuto}) /return
				AMIDEAD
				/varset _tmpID ${NearestSpawn[${_count}, npc ${If[${switchOverrideLos},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${_tmpRange} zradius ${zradius} targetable].ID}
		
				/if (!${_priorityMob} && ${Spawn[id ${_tmpID}].Level} >= ${minMobPullLvl} && ${Spawn[id ${_tmpID}].PctHPs} <= ${_mobHP} && ${Spawn[id ${_tmpID}].Type.Equal[NPC]} && ${Spawn[id ${_tmpID}].Type.NotEqual[Mount]} && !${ExcludeList.Find[${Spawn[id ${_tmpID}].DisplayName}]} && ${Spawn[id ${_tmpID}].DisplayName.Length} > 1 || ${NoMezzList.Find[${Spawn[ID ${_tmpID}].DisplayName}]}) {
					/varset Target2Kill ${_tmpID}
					/varset _mobHP ${Spawn[id ${_tmpID}].PctHPs}
					/if (${NoMezzList.Find[${Spawn[ID ${_tmpID}].DisplayName}]} || ${NameList.Find[${Spawn[ID ${_tmpID}].DisplayName}]}) {
						/varset _priorityMob TRUE
						/break
					}
				}
			/next _count
		}
		/varcalc _tmpRange ${_tmpRange}+10
	}		
		
	| lastly, kill all the evil pets. if the owner is dead.
	/if (${SpawnCount[pet ${If[${switchOverrideLos},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${MobAgro} zradius ${zradius} targetable]} && !${SpawnCount[npc ${If[${switchOverrideLos},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${combatRadius} zradius ${zradius} targetable]}) {
		DEBUG Found a pet within:${MobAgro}
		/for _count 1 to ${SpawnCount[pet ${If[${switchOverrideLos},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${MobAgro} zradius ${zradius} targetable]}
			/if (!${switchPull} || !${coreAuto}) /return
			AMIDEAD
			/varset _tmpID ${NearestSpawn[${_count}, pet ${If[${switchOverrideLos},,los]} range ${minMobPullLvl} ${maxMobPullLvl} radius ${MobAgro} zradius ${zradius} targetable].ID}
			/if (${Spawn[${Spawn[${_tmpID}].Master}].Type.Equal[Corpse]} || !${Spawn[${Spawn[${_tmpID}].Master}].ID}) /varset Target2Kill ${_tmpID}
		/next _count	
	} 
		
	/if (${Spawn[${Target2Kill}].Type.NotEqual[Aura]} && ${Target2Kill}) /target id ${Target2Kill}
	DEBUG Red Rover send [${ctar}${Spawn[${Target2Kill}].DisplayName}\ax] right over. Distance:${If[${Spawn[${Target}].LineOfSight},${cgood}${Target.Distance}\ax,${cbad}${Target.Distance}\ax]}

	/call echos pull 0 0 0 0
	UPDATEHUD "PULL"

	DEBUG Disable /camp for pull
	/if (${switchPull} && ${HomeSet}) /squelch /makecamp pause

	:looppull
	CLEARCURSOR TRUE
	/if (!${switchPull} || !${coreAuto}) /return
	/if (${FindItemCount[${RangeItemSummon}]} < 5 && ${Bool[${RangeItemSummon}]}) /call check_ammo

	| if mob is over arrow range, start checking options to get him close
	/if (${Spawn[id ${Target2Kill}].Distance} > 31 && !${Select[${Me.Class.ShortName},MAG,NEC,BST,ENC]}) {
		DEBUG use arrow to pull mob over 31
		/target ID ${Target2Kill}
		DEBUG ID ${Target2Kill}
		/if (${Target.ID} && ${Select[${combat},MELEE,RANGE]} && !${Range.Between[0,${SetFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}

		/if (${Target.ID}) {
			
			| check target is LoS. if stays out of sight too long. reset
			/if (!${Spawn[${Target}].LineOfSight} && !${switchOverrideLos}) {
				/delay 2s ${Spawn[${Target}].LineOfSight}
				/if (!${Spawn[${Target}].LineOfSight}) {
					/call clear_combat
					/return
				}
			}
			
			| check targets range. if out of range too long. reset
			/if (${Spawn[${Target}].Distance} > ${MobAgro}) {
				/delay 2s ${Spawn[${Target}].Distance} < ${MobAgro}
				/if (${Spawn[${Target}].Distance} > ${MobAgro}) {
					/call clear_combat
					/return
				}
			}			
			
			/doevents
			/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${MobAgro} targetable]} && ${Me.State.NotEqual[STAND]}) /stand
			/if (${Me.Stunned}) /return
			/if (!${switchPull} || !${coreAuto}) /return
			
			/if (${Select[${Me.Class.ShortName},SHD]}) {
				/if (${Target.Distance} < ${MobAgro} && ${Target.Distance} > 31) /call core_cast "Hate's Attraction" alt ${Target2Kill} FALSE
			} else /if (${Select[${Me.Class.ShortName},PAL]}) {
				/if (${Target.Distance} < ${MobAgro} && ${Target.Distance} > 31 && ${Target.Distance} <= 45) /call core_cast "Divine Call" alt ${Target2Kill} FALSE
			} else /if (${Select[${Me.Class.ShortName},WAR]}) {
				/if (${Target.Distance} < ${MobAgro} && ${Target.Distance} > 31) /call core_cast "Warlord's Grasp" alt ${Target2Kill} FALSE
				/if (${Target.Distance} < 31 && ${Me.CurrentEndurance} > ${Spell[${DiscInsult}].EnduranceCost}) /call core_cast "${DiscInsult}" disc ${Target2Kill} FALSE
			}

			| get mob to us
			/if (${Spawn[ID ${Target2Kill}].Distance} > 31 && < ${MobAgro}) {
				/if (${Target.ID}) {
					
					| spell pull mob
					/if (${Bool[${Spell[${RangeSpell}]}]} && ${Bool[${Me.Gem[${RangeSpell}]}]} && ${Me.SpellReady[${RangeSpell}]}) {
						/call core_cast "${RangeSpell}" 0 ${Target2Kill} FALSE

					| item pull mob
					} else /if (${Me.ItemReady[${RangeClickItem}]}) {
						/call core_cast "${RangeClickItem}" item ${Target2Kill} FALSE

					| disc pull mob
					} else /if (${Me.CombatAbilityReady[${RangeDisc}]}) {
						/call core_cast "${RangeDisc}" disc ${Target2Kill} FALSE
		
					| range item pull mob
					} else {				
					 /range
					}
				}
				
			} else 	/if (${Spawn[ID ${Target2Kill}].Distance} <= 31) {
				/if (${coreAuto}) /squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
				/if (${Select[${combat},RANGE]}) /return
				/squelch /stick ID ${Target2Kill} ${CombatDistance} 
			}
		}

	| if the mob is less the arrow range, move to the bastard
	} else /if (${Spawn[ID ${Target2Kill}].Distance} <= 31 && !${Select[${Me.Class.ShortName},MAG,NEC,BST,ENC]}) {
		DEBUG stick to mob <= 31
		/if (${coreAuto} && ${Select[${combat},MELEE,RANGE]} && !${Range.Between[0,${SetFaceArc}:${Math.Abs[${Math.Calc[${Target.HeadingTo.Degrees}-${Me.Heading.Degrees}]}]}]}) /squelch /face ID ${Target2Kill} nolook ${If[${SetFaceFast},fast,]}
		/if (${Select[${combat},RANGE]}) /return
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
	
	UPDATEHUD FALSE
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
	DEBUG ${cbug}echo_augments()\ax
	/declare _slot 			int local
	/declare _augslot 	int local
	/declare _bagslot 	int local
	
	| worn inventory skip powersource (21)
	/for _slot 0 to 22
		/if (${_slot} == 21) /goto :noWornItemAug
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
	DEBUG ${cbug}check_watch_spawn(spawn: ${watchSpawn})\ax
	
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
	DEBUG ${cbug}give_coin(${_ammount}, ${_stacks}, ${_coin})\ax
	| VOUT /givecoin [ammount] [stacks] [plat|gold|copper|silver]

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
 *  NOTE: 103 spell EoK spell
 ***|
sub check_harmonious()
	DEBUGCAST ${cbug}check_harmonious()\ax
	AMIDEAD
	CHECKEXIT
	/if (!${switchHarmonious}) /return
	/if (!${Bool[${Me.Buff[${Harmonious}]}]} && ${Me.PctMana} > 10) /call core_cast "${Harmonious}" 0 0 FALSE
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
	/if (${coreAuto} && !${Me.Invis} || (${coreAuto} && ${Select[${Me.Class.ShortName},ROG]} && ${switchSos})) {
		/call check1offBuffs
		/if (${Select[${combat},MELEE,RANGE,NUKE]}) {
			/call is_target_valid ${Target2Kill}
			/if (!${isValidTarget} || !${Spawn[${Target2Kill}].ID}) {
				/call clear_combat
				/varset _gokillstuff FALSE
			} else /if (${isValidTarget} && ${Spawn[${Target2Kill}].ID}) {
				| /if (${is_burn_time[]}) /call synergy_${Me.Class.ShortName}
				/call is_burn_time
				/if (${Macro.Return}) /call synergy_${Me.Class.ShortName}
			}
		} else /if (${Select[${combat},HEAL,DUAL]}) {
			| /if (${is_burn_time[]}) /call synergy_${Me.Class.ShortName}
			/call is_burn_time
			/if (${Macro.Return}) /call synergy_${Me.Class.ShortName}
		}
	} else /if (!${coreAuto}) {
		/if (${Target.ID} != ${Target2Kill}) /varset Target2Kill ${Target.ID}
		/if (!${Target.ID} || !${Spawn[${Target2Kill}].ID}) /varset _gokillstuff FALSE
		/if (!${SafeZone} && ${burnForce} && ${Select[${Me.CombatState},COMBAT]}) {
			/call synergy_${Me.Class.ShortName}
		} else /if (${SafeZone} && ${burnForce} && ${Select[${Me.CombatState},ACTIVE]}) {
			/call synergy_${Me.Class.ShortName}
		}			

	}
	/if (${_gokillstuff} && (!${SafeZone} && !${switchOverrideSafezone})) /call set_base_target_timers ${Target2Kill}
/return ${_gokillstuff}



 |***
 *	DES: Check if we need to engage burn routines. /combat heal/dual are treated differently
 *	USE: auto
 *	INI: 
 *  NOTE: we work off establised assist target for non healers only.
 ***|
sub is_burn_time()
	DEBUG ${cbug}is_burn_time()\ax
	/doevents
	AMIDEAD
	/if (${SafeZone} && !${switchOverrideSafezone} && ${coreAuto}) /return FALSE
	
	| if always set to burn, always burn while in combat
	/if (${burnForce} && ${Me.Combat}) /return TRUE
	
	| if the current target is a namer
	/if (${coreAuto} && ${burnAuto} && ${Bool[${Target2Kill}]} && ${Spawn[${Target2Kill}].PctHPs} <= ${burnEngageat} && (${NameList.Find[${Target.DisplayName}]} && !${switchOverrideName})) /return TRUE

	| if number of mobs in the local area reaches the /burn count ## threashold
	/if (${coreAuto} && ${Bool[${burnCount}]} && ${SpawnCount[npc radius ${MobAgro} ${If[${switchOverrideLos},,los]} targetable playerstate 4]} >= ${burnCount}) /return TRUE

	| healers mezers, and off combat routines simply check for a named in the local area to start a burn. no direct target of a named is required. just be in the area.
	/declare _mobcount			int 		local 0
	/declare _count					int 		local 0
	/declare _mobID 				int 		local 0
	
	/if (${Select[${combat},HEAL,DUAL]}) {
		/varset _mobcount ${SpawnCount[npc radius ${MobAgro} playerstate 4]}
		/for _count 1 to ${_mobcount}
			/doevents
			AMIDEAD
			/varset _mobID ${NearestSpawn[${_count}, npc radius ${MobAgro}].ID}
			/if (${burnAuto} && ${NameList.Find[${Spawn[id ${_mobID} radius ${MobAgro}].DisplayName}]} && !${switchOverrideName}) /return TRUE
			/if (${Bool[${burnCount}]} && ${SpawnCount[npc radius ${MobAgro} ${If[${switchOverrideLos},,los]} targetable playerstate 4]} >= ${burnCount}) /return TRUE
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
	DEBUG ${cbug}click_summon_food(${FindItemCount[${clickFoodItem}]} <= ${clickFoodCount})\ax
	/call create_timer timer_click_food ${tClick_FoodDrink}
	AMIDEAD
	CHECKTIE
	CHECKEXIT
	
	/if (${FindItemCount[=${clickFoodItem}]} < ${clickFoodCount} && ${Me.ItemReady[${clickFoodDispenser}]}) {
		DEBUG click ${clickFoodDispenser}
		/call core_cast "${clickFoodDispenser}" item ${Me.ID} FALSE
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
	DEBUG ${cbug}click_summon_drink(${FindItemCount[${clickDrinkItem}]} <= ${clickDrinkCount})\ax
	CHECKTIE
	AMIDEAD
	CHECKEXIT
	/call create_timer timer_click_drink ${tClick_FoodDrink}
	
	/if (${FindItemCount[=${clickDrinkItem}]} < ${clickDrinkCount} && ${Me.ItemReady[${clickDrinkDispenser}]}) {
		DEBUG click ${clickDrinkDispenser}
		/call core_cast "${clickDrinkDispenser}" item ${Me.ID} FALSE
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
	DEBUG ${cbug}check_buff_routines(Force: ${_force})\ax
	/if (!${coreAuto} && !${_force}) /return
	/doevents
	AMIDEAD
	/if (!${Me.Invis} || (${Select[${Me.Class.ShortName},ROG]} && ${switchSos})) {
		
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
		  /if (${switchBuffRaid} && !${timer_check_buffs_raid}) /call check_buffs_raid
			/if (${switchBuffBc}  && !${timer_check_buffs_BC}) /call check_buffs_BC
			/if (${switchBuffPet} && ${Me.Pet.ID} && !${timer_check_pet_buffs}) /call check_pet_buffs
			/if (${switchBuffSelf} && (${Bool[${Aura1Spell}]} || ${Bool[${Aura2Spell}]}) && !${timer_check_aura}) /call check_aura
			/if (${switchBuffItem} && ${Select[${Me.Class.ShortName},ENC]}) /call check_crystals
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
	DEBUG ${cbug}check_for_item(verbage: ${_verbage})\ax
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
 *	DES: forces a reload of all INIs
 *	USE: /call core_reset
 *	INI: 
 *  NOTE:
 ***| 	
sub core_reset()
| /return
	DEBUG ${cbug}core_reset()\ax
	VOUT ${cgood}Reloading Core INIs\ax
	/call set_declares
	/call check_plugin
	/call echos startup 0 0 0 0
	/call declare_common
	/call declare_${Me.Class.ShortName}
	/call intialize_item_buffs
	/call set_alias
	/call Event_Zone
	/call base_preperation
	
	| /call clear_combat
	/if (${switchPcPet} && ${Me.Pet.ID} && ${Bool[${Me.Pet.Target}]}) /pet back off

	| get shit out of our hands
	CLEARCURSOR TRUE
	
	| clear modes
	/call switch_clear_mode
	
	| clear all debugs
	/call Bind_debug stop
	
	| clear the current target
	/if (${clearTarget} && ${Target.ID}) /squelch /target clear
	/call set_stop TRUE
	/call clear_combat
	
	| set assists
	/if (!${Bool[${Raid.Members}]} && !${Select[${Me.Class.ShortName},CLR]} && !${switchPull}) {
		/smartma G
 	} else /if (${Bool[${Raid.Members}]} && !${Select[${Me.Class.ShortName},CLR]} && ${switchPull}) {
		/smartma R
	}	else /if (${Select[${Me.Class.ShortName},CLR]} || ${switchPull}) {
		/smartma X
	}	
	
	| other misc resets
	/call writeINI watchSpawn Common False
	
	VOUT ${cgood}Done\ax
/return



|***
 *	DES: one line echo bitch
 *	USE: /call echo_bitch [EQBC|ECHO]
 *	INI: 
 *  NOTE:
 ***| 	
sub echo_bitch(string _out)
	DEBUG ${cbug}echo_bitch(type: ${_out})\ax
	/declare _bitch					string local 
	/declare _rnd						int local
	
	:loopgetrandom
	/varset _rnd ${Math.Rand[${bitchcount}]}
	/if (!${Range.Between[1,${bitchcount}:${_rnd}]}) /goto :loopgetrandom
	/varset _bitch ${bitch${_rnd}}

	/if (${_out.Find[EQBC]}) {
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
	DEBUG ${cbug}cast_AA_fade()\ax
	CHECKEXIT
	/doevents
	/if (${is_target_dead[${Target2Kill}]}) /return

	/if (${Me.AltAbilityReady[${AAFade}]}) {
		ECHOCHANNEL Fading Combat ${Me.PctHPs} <= ${AAFade}
		/call core_cast "${AAFade}" alt 0 FALSE
	}
/return



|***
 *	DES: keeps Paladin/Shadowknight Stance running
 *	USE: /call check_Stance
 *	INI: [buffs] - Stance
 *  NOTE: 
 ***|
sub check_stance()
	DEBUGCAST ${cbug}check_steel()\ax
	CHECKEXIT
	/doevents
	/if (!${switchStance}) /return
	
	/if (!${Bool[${Me.Buff[${Stance}]}]} && ${Me.SpellReady[${Stance}]}) {
		/call core_cast "${Stance}" 0 0 FALSE
		/call create_timer timer_check_Stance ${tcheck_Stance}
	}
/return

 
 
 |***
 *	DES: Circle of [Power|Life|Mana]
 *	USE: keep Circle of [Power|Life|Mana] going
 *	INI: TOON - [General] -> Circleof[Power|Life|Mana]=
 *	NOTE: these are all on the same timer. so u just get one
 ***|  
sub synergy_click_Circle()
	DEBUG ${cbug}synergy_click_Circle()\ax
	/if (!${Bool[${useCircle}]}) /return FALSE
	AMIDEAD
	CHECKEXIT	
	
	| circle of power
	/if (${useCircle.Equal[Power]}) {
		/if (${Me.ItemReady[${CircleofPower}]} && ${Spell[${FindItem[${CircleofPower}].Spell}].Stacks}) {
			/if (${Bool[${Me.Song[Circle of Power]}]} || ${Bool[${Me.Song[${RNGauspice}]}]}) /return
			/call core_cast "${CircleofPower}" item 0 FALSE
			/return
		}

	| circle of life
	} else /if (${useCircle.Equal[Life]}) {
		/if (${Me.ItemReady[${CircleofLife}]} && ${Spell[${FindItem[${CircleofLife}].Spell}].Stacks}) {
			/if (${Bool[${Me.Song[Circle of Life]}]}) /return
			/call core_cast "${CircleofLife}" item 0 FALSE
			/return
		}		

	| circle of Mana
	} else /if (${useCircle.Equal[Mana]}) {
		/if (${Me.ItemReady[${CircleofMana}]} && ${Spell[${FindItem[${CircleofMana}].Spell}].Stacks}) {
			/if (${Bool[${Me.Song[Circle of Mana]}]}) /return
			/call core_cast "${CircleofMana}" item 0 FALSE
			/return
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
	DEBUG ${cbug}click_item_burn()\ax
	/declare _count int local 0
	/for _count 1 to 6
		/if (!${Bool[${clickItemBurn${_count}}]}) /continue
		/doevents
		AMIDEAD
		CHECKTIE
		/if (${is_target_dead[${Target2Kill}]}) /break
		/if (${Me.ItemReady[${clickItemBurn${_count}}]}) /call core_cast "${clickItemBurn${_count}}" item 0 FALSE
	/next _count
/return  


|***
 *	DES: cast whatever spire you set for burns
 *	USE: /call cast_AA_spire
 *	INI: classINI - [Buffs] - spireGroup & spireRaid
 *  NOTE: 
 ***|
sub cast_AA_spire()
	AMIDEAD
	CHECKEXIT
	/if (${Bool[${Raid.Members}]} && ${Bool[${${Me.Class.ShortName}spire${spireRaid}}]}) {
		/if (${Me.AltAbilityReady[${${Me.Class.ShortName}spire${spireRaid}}]} && !${Bool[${Me.Buff[${${Me.Class.ShortName}spire${spireRaid}}]}]} && ${Spell[${${Me.Class.ShortName}spire${spireRaid}}].Stacks}) {
			/call core_cast "${${Me.Class.ShortName}spire${spireRaid}}" alt 0 FALSE
		}
	} else /if (!${Bool[${Raid.Members}]} && ${Bool[${${Me.Class.ShortName}spire${spireGroup}}]}) {
		/if (${Me.AltAbilityReady[${${Me.Class.ShortName}spire${spireGroup}}]} && !${Bool[${Me.Buff[${${Me.Class.ShortName}spire${spireGroup}}]}]} && ${Spell[${${Me.Class.ShortName}spire${spireGroup}}].Stacks}) {
			/call core_cast "${${Me.Class.ShortName}spire${spireGroup}}" alt 0 FALSE
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
 	DEBUGCLASS ${cbug}mem_spell_line(group: ${_line})\ax
	AMIDEAD
	CHECKEXIT
	/doevents
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
 *	USE: /call cast_challenge_${Me.Class.ShortName}
 *	INI: 
 *  NOTE:
 ***|
sub check_AA_agro()
	DEBUG ${cbug}check_AA_agro()\ax
	AMIDEAD
	
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
	DEBUGCLASS ${cbug}cast_challenge(ID: ${_tmpID})\ax
	AMIDEAD
	CHECKEXIT
	
	/if (${Me.SpellReady[${ChallengeSpell}]} && !${Target.Buff[${ChallengeSpell}].ID}) {
		/if (${switchAgro}) {
			/call core_cast "${ChallengeSpell}" 0 ${_tmpID} cast_break
		} else /if (!${switchAgro} && ${Target.Body.Name.Equal[Undead]} && ${switchdeBuffCripple}) {
			/call core_cast "${ChallengeSpell}" 0 ${_tmpID} cast_break
		}
	}
/return



|***
 *	DES: validates buffs are ready to be cast/can be cast
 *	USE: /call ${validate_buff[[spell|alt|disc|item], inisub, name]
 *	INI: 
 *  NOTE: will sort silver subscriptions as well.
 ***|
sub validate_buff(string _type, string _inisub, string _spell)
	DEBUG ${cbug}validate_buff(type:${_type}, ini:${_inisub}, name:${_spell})\ax

	| check spell
	/if (${_type.Equal[spell]}) {
		/if (!${Bool[${${_spell}}]}) /return FALSE
		/if (!${Bool[${Me.Book[${${_spell}}]}]}) /return FALSE
		/if (${Me.PctMana} < ${Spell[${_spell}].Mana}) /return FALSE
		| check for non gold subs
		/if (${Select[${Me.Subscription},SILVER]}) {
			/varset _spell ${Ini[${INIClass},${_inisub},${_spell}]}
			/if (${Bool[${Me.Buff[${_spell}].Duration}]}) /return FALSE
			/if (${Bool[${Me.Song[${_spell}].Duration}]}) /return FALSE
			/if (!${Spell[${_spell}].Stacks}) /return FALSE
		} else {
			/if (${Bool[${Me.Buff[${${_spell}}].Duration}]}) /return FALSE
			/if (${Bool[${Me.Song[${${_spell}}].Duration}]}) /return FALSE
			/if (!${Spell[${${_spell}}].Stacks}) /return FALSE
		}
		
	| check alt abilitie
	} else /if (${_type.Equal[alt]}) {
		/if (!${Me.AltAbilityReady[${${_spell}}]}) /return FALSE
		/if (${Bool[${Me.Buff[${${_spell}}]}]}) /return FALSE
		/if (${Me.PctMana} < ${Spell[${${_spell}}].Mana}) /return FALSE
	
	| check disc
	} else /if (${_type.Equal[disc]}) {
		/if (!${Bool[${${_spell}}]}) /return FALSE
		/if (!${Me.CombatAbilityReady[${${_spell}}]}) /return FALSE
		/if (${Spell[${${_spell}}].EnduranceCost} > ${Me.CurrentEndurance}) /return FALSE
		/if (${Bool[${Me.Song[${${_spell}}].Duration}]}) /return FALSE
		/if (${Bool[${Me.Buff[${_spell}].Duration}]}) /return FALSE

	| check item
	} else /if (${_type.Equal[item]}) {
	}

/return TRUE



|***
 *	DES: validates casting of "things on a target
  *	USE: /call ${validate_cast[[spell|alt|disc|item], inisub, name, ID]
 *	INI: 
 *  NOTE: will sort silver subscriptions as well.
 ***|
sub validate_cast(string _type, string _spell, int _tmpID)
	DEBUG ${cbug}validate_cast(type:${_type}, name:${_spell}, Id:${_tmpID})\ax
	
	/if (${Target.Type.Equal[Corpse]} || ${Spawn[${_tmpID}].Type.Equal[corpse]}) /return FALSE
	/if (${Bool[${_tmpID}]} && !${Target.ID}) /return FALSE

	/if (${_type.Equal[spell]}) {
		/if (!${Bool[${Me.Book[${_spell}]}]}) /return FALSE
		/if (!${Me.SpellReady[${_spell}]}) /return FALSE
		/if ((${Stick.Active} && !${setModeType.Equal[hunt]} && !${setModeType.Equal[scorch]}) || ${Me.Moving} || ${AdvPath.Following}) /return FALSE
		/if (${Me.Casting.ID}) /return FALSE
		/if (${_tmpID} != 0) {
			/if (!${switchOverrideLos} && !${Spawn[${_tmpID}].LineOfSight} && !${Spell[${_spell}].SpellType.Equal[Beneficial]}) /return FALSE
			/if (${Spawn[${_tmpID}].Distance} > ${Spell[${_spell}].Range}) /return FALSE
		}
		/if (${Me.CurrentMana} < ${Spell[${_spell}].Mana}) /return FALSE

		| spell stacks ?
		| /if (!${Select[${_type},alt,item,disc]} && !${Spell[${_spell}].Stacks}) /return FALSE

		| restist types vs core.zone.inc
		/if (!${Spell[${_spell}].SpellType.Equal[Beneficial]} && ${_tmpID} != 0) {
			/if (${NoDiseaseList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Disease]}) /return FALSE
			/if (${NoMagicList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Magic]}) /return FALSE
			/if (${NoPoisonList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Poison]}) /return FALSE
			/if (${NoFireList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Fire]}) /return FALSE
			/if (${NoColdList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Cold]}) /return FALSE
			/if (${NoCorruptionList.Find[${Target.DisplayName}]} && ${Spell[${_spell}].ResistType.Equal[Corruption]}) /return FALSE
		}
		
	| check alt ability
	} else /if (${_type.Equal[alt]}) {
		/if (!${Me.AltAbilityReady[${_spell}]}) /return FALSE
		/if (${Me.CurrentMana} < ${Spell[${_spell}].Mana}) /return FALSE
		/if (${_tmpID} != 0 && ${Spawn[${_tmpID}].Distance} > ${Spell[${_spell}].Range}) /return FALSE
	
	| check disc
	} else /if (${_type.Equal[disc]}) {
		/if (!${Me.CombatAbilityReady[${_spell}]}) /return FALSE
		/if (${Me.CurrentEndurance} < ${Spell[${_spell}].EnduranceCost}) /return FALSE
		/if (${_tmpID} != 0 && ${Spawn[${_tmpID}].Distance} > ${Spell[${_spell}].Range}) /return FALSE

	| check item
	} else /if (${_type.Equal[item]}) {
		/if (!${Me.ItemReady[${_spell}]}) /return FALSE
		/if (${_tmpID} != 0 && (${Target.Distance} > ${Spell[${_spell}].MyRange} || ${Spawn[${_tmpID}].Distance} > ${Spell[${_spell}].Range})) /return FALSE


	} else /if (${_type.Equal[pulse]}) {

	}

/return TRUE



|***
 *	DES: spell/disc/aa/item cast wrapper
 *	USE: /call core_cast "SPELLNAME" [alt|disc|gem#|item|0|NULL] [MobID|0|NULL] [subname|FALSE]
 *	USE: /call core_cast "SPELLNAME" NULL NULL if you want to let the sub decide what you have to cast/use.
 *	INI: 
 *  NOTE: MQ2Cast ["spell name"|"item name"|"AA name"|"AA#"] [[item|slotname]|alt|gem#] [give up time][s|m] [custom subroutine name] [-targetid|###] [-maxtries|#] [-recast|#] [-setin|setname] [-bandolier|setname]
 ***|
sub core_cast2(string _spell, string _type, int _tmpID, string _sub)
	DEBUG ${cbug}core_cast(\ax spell:${cinfo}${_spell}\ax, type:${cinfo}${_type}\ax, ID:${cinfo}${_tmpID}\ax, sub:${cinfo}${_sub}\ax ${cbug})\ax
	/doevents
	AMIDEAD
	/declare _bail string local FALSE
	
	| preemptive gtfo for this particular spell
	| /if (${Bool[${timer_${_tmpID}_${Spell[${_spell}].ID}}]}) /goto :cooldown
	
	| make sure we have our target 
	/if (${Bool[${_tmpID}]} && ${Target.ID} != ${_tmpID}) {
		DEBUGCAST fixing target to ID:${_tmpID} ${sep} ${Spawn[ID ${_tmpID}].DisplayName}
		/tar id ${_tmpID}
		/delay 1s ${Target.ID} == ${_tmpID}
	}
	
	| used to let /call core_cast "whatever" NULL NULL
	| figure out what the toon has and use that.
	/if (${_type.Equal[NULL]} && !${Bool[${_tmpID}]}) {
		DEBUGCAST fixing cast type and target id NULL/NULL
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
	
	
	| skip needing a target
	/if (${_tmpID} == 0 || ${Spell[${_spell}].SpellType.Equal[Self]}) /goto :notarget
	
	/if (!${Spawn[${_tmpID}].ID} && !${timer_spellcast_nomob_ID}) {
		VOUT You tried to cast ${csp}${_spell}\ax on something that doesn't exist.
		/varset castReturn CAST_NOTREADY
		/call create_timer timer_spellcast_nomob_ID 60s
	}


	| bailing on this spell. its either out of range, immune or something..
	/if (${Bool[${_bail}]}) DEBUGCAST ${owarning} ${sep} _bail = ${_bail} immune too ${_spell}
	/if (${Bool[${_bail}]} && !${Spell[${_spell}].SpellType.Equal[Beneficial]} && (${Target.ID} || ${_tmpID})) /goto :cooldown

	:notarget
	AMIDEAD
	DEBUGCAST ${cbug}(\ax\awSPELL:${_spell}, TYPE:${_type}, ID:${_tmpID}, SUB:${_sub}\ax${cbug})\ax

	| Passed all checks. lets cast some shits..
	DEBUGCAST passed all checks

	| output echos
	| Spell
	/if (!${Select[${_type},alt,item,disc]}) {
		/if (${_tmpID} == 0) {
			VOUT Spell ${sep} ${csp}${_spell}\ax
			/call MQ2Cast "${_spell}" ${If[${Bool[${_type}]},${_type},gem${Me.Gem[${_spell}]}]} -targetid|${_tmpID}
		} else /if (${_tmpID} != 0 && ${Target.ID}) {
			/if (${Me.Song[Gift of Mana (${GoMLevel})].ID}) /call cast_nuke_GoM ${_tmpID}
			DEBUGCAST MQ2Cast "${_spell}" ${If[${Bool[${_type}]},${_type},gem${Me.Gem[${_spell}]}]} -targetid|${_tmpID}
			VOUT Spell ${sep} ${csp}${_spell}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
			/call MQ2Cast "${_spell}" ${If[${Bool[${_type}]},${_type},gem${Me.Gem[${_spell}]}]} ${If[${Bool[${_sub}]},${_sub},]} -targetid|${_tmpID}
		}
		/goto :cooldown
	| AA
	} else /if (${Select[${_type},alt]}) {
		/if (${_tmpID} == 0) {
			VOUT AA ${sep} ${csp}${_spell}\ax
			/call MQ2Cast "${_spell}" alt 
		} else /if (${_tmpID} != 0 && ${Target.ID}) {
			VOUT AA ${sep} ${csp}${_spell}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
			/call MQ2Cast "${_spell}" alt -targetid|${_tmpID}
		}
		/goto :cooldown
	| item
	} else /if (${Select[${_type},item]}) {
		/if (${_tmpID} == 0) {
			VOUT Item ${sep} ${citem}${_spell}\ax
			/call MQ2Cast "${_spell}" item
		} else /if (${_tmpID} != 0 && ${Target.ID}) {
			VOUT Item ${sep} ${citem}${_spell}\ax ${sep} [${ctar}${Spawn[id ${_tmpID}].DisplayName}\ax]
			/call MQ2Cast "${_spell}" item -targetid|${_tmpID}
		}
		/goto :cooldown
	| disc
	} else /if (${Select[${_type},disc]}) {
		/if (${_tmpID} == 0) VOUT Disc ${sep} ${csp}${_spell}\ax
		/if (${_tmpID} != 0 && ${Target.ID}) VOUT Disc ${sep} ${csp}${_spell}\ax ${sep} [${ctar}${Spawn[ID ${_tmpID}].DisplayName}\ax]
		/disc ${_spell}
		/delay 5s !${Me.Casting.ID}
		/goto :cooldown
	}
	DEBUGCAST ${csp}${_spell}\ax ${sep}${sep}${sep}${sep} ${sepO} ${castReturn} ${sepC}

	| cooldown
	:cooldown
	AMIDEAD
	/if (!${Select[${_type},alt,item,disc]}) {
		/delay 2 !${Me.SpellReady[${_spell}]}
	} else /if (${Select[${_type},item]}) {
		/delay 2 !${Me.ItemReady[${_spell}]}
	} else /if (${Select[${_type},alt]}) {
		/delay 2 !${Me.AltAbilityReady[${_spell}]}
	} else /if (${Select[${_type},disc]}) {
		/delay 2 !${Me.CombatAbilityReady[${_spell}]}
	}

	DEBUGCAST ${cbug}core_cast(\ax${cinfo}SPELL:${_spell})\ax ${sep} bail:${_bail}
/return ${Bool[${Macro.Return}]}



|***
 *	DES: removes ranks from spell names. returns the name only
 *	USE: /call spell_remove_rank [spellname]
 *	INI: 
 *	NOTE:
 ***|
sub spell_remove_rank(string _spell)
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
	DEBUG ${cbug}set_base_install()\ax
	
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
	/call writeINI switchBuffCombat Buffs TRUE
	/call writeINI switchBuffItem Buff TRUE
	/call writeINI switchBuffSelf Buffs TRUE
	

	| class specific settings
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

/return



|***
 *	DES: Used to remove stale entries from the toons INI. 'cause I gotta keep changing shits.
 *	USE: /call fix_INI_declares
 *	INI: 
 *	NOTE: may look a little strange, but I leave the old variables here for history. Just in case.
 ***|
sub fix_INI_declares()
	/ini "${INIClass}" "Common" "echoChannel" NULL
	/ini "${INIClass}" "Common" "relayTells" NULL
	/ini "${INIClass}" "Common" "verbose" NULL

	/ini "${INIClass}" "Items" "doclickItem" NULL
	/ini "${INIClass}" "Items" "doclickItemBurn" NULL
	/ini "${INIClass}" "Odds" "doclickOdds" NULL

	/ini "${INIClass}" "Discs" "DefensiveDisc" NULL

	/ini "${INIClass}" "Discs" "DiscArmor" NULL
	/ini "${INIClass}" "Discs" "DiscGuard" NULL
	/ini "${INIClass}" "Discs" "DiscHoly" NULL

	/ini "${INIClass}" "Common" "useSmartAssist1" NULL
	/ini "${INIClass}" "Common" "useSmartAssist2" NULL
	
	/if (${Me.Class.ShortName.Equal[BRD]}) /ini "${INIClass}" "meleedisc" "seloskick" NULL

	/if (${Me.Class.ShortName.Equal[WIZ]}) {
		/ini "${INIClass}" "Spells" "DoNuke" NULL
		/ini "${INIClass}" "Spells" "Nuke1" NULL
		/ini "${INIClass}" "Spells" "Nuke2" NULL
		/ini "${INIClass}" "Spells" "Nuke3" NULL
		/ini "${INIClass}" "Spells" "Nuke4" NULL
		/ini "${INIClass}" "Spells" "Nuke5" NULL
		/ini "${INIClass}" "Spells" "Nuke6" NULL
		/ini "${INIClass}" "Spells" "Nuke7" NULL
		/ini "${INIClass}" "Spells" "Nuke8" NULL
	}


	| fix the range summon INI entries
	/if (!${RangeItem.Count[|]}) /call writeINI RangeItem Common |"${RangeItem}"
	/if (!${RangeItemSummon.Count[|]}) /call writeINI RangeItemSummon Common |"${RangeItemSummon}"		


	VOUT INI Housekeeping ${sep} ${cgood}DONE\ax
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
	AASnareList					|RNG|DRU|SHD|NEC|ROG|WAR|WIZ|MNK|PAL
	AAEradicateList			|RNG|DRU|NEC|WIZ|ENC|MAG
 
 ***|
sub validate_class(bool _error, string _class)
	DEBUG ${cbug}validate_class(error: ${_error}, class: ${_class})\ax
	/if (${switchOverrideClasscheck}) /return TRUE
	/declare _pass bool local FALSE
	/declare _count int local 0

	/for _count 1 to ${_class.Count[|]}
		AMIDEAD
		/if (${Me.Class.ShortName.Upper.Equal[${_class.Arg[${_count},|]}]}) {
			/varset _pass TRUE
			/break
		}
	/next _count

	/if (!${_pass} && ${_error}) /call echos failboat 0 0 0 0
/return ${_pass}



|***
 *	DES: is the plugin available ?
 *	USE: /if (${validate_plugin[BOOL, |plugin|plugin]) /dowhatever
 *	INI: 
 *	NOTE: cycle all plugins. return FALSE if any passed are not loaded
 ***|
sub validate_plugin(bool _error, string _plugin)
	DEBUG ${cbug}validate_plugin(error: ${_error}, plugin: ${_plugin})\ax
	
	/if (!${Bool[${_plugin}]}) VOUT ${owarning} ${sep} validate_plugin failure.
	/declare _plugcount int local 0	
	/declare _count int local 0

	/for _count 1 to ${_plugin.Count[|]}
		AMIDEAD
		DEBUG _count:${_plugin}, arg: ${_plugin.Arg[${_count},|]}
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
	DEBUG ${cbug}cast_slow(ID: ${_tmpID})\ax
	AMIDEAD
	/doevents
	CHECKEXIT
	/if (${is_target_dead[${_tmpID}]}) /return
	
	/squelch /target ID ${_tmpID}
	/delay 1s ${Target.ID} == ${_tmpID}
	
	| dont waste time if its slowed already
	/if (${Target.Slowed.Duration}) {
		/call create_timer timer_slow_${Target2Kill} ${Target.Slowed.Duration.TotalSeconds}s
		/return
	}
	UPDATEHUD "Slow"
	
	| necros only slow undead 
	/if (${validate_class[FALSE, |NEC]} && !${Target.Body.Name.Equal[Undead]}) /return

	| get to business
	/declare _spelltype string local FALSE
	/declare _readytype string local FALSE

	| spell not identified, using AA.
	/if (${spellSlow.Equal[NULL]} || !${Bool[${spellSlow}]} || !${Bool[${Me.Gem[${spellSlow}]}]}) {
		/if (${Select[${Me.Class.ShortName},ENC]}) /varset spellSlow Dreary Deeds
		/if (${Select[${Me.Class.ShortName},SHM]}) /varset spellSlow Turgur's Swarm
		/if (${Select[${Me.Class.ShortName},BST]}) /varset spellSlow Sha's Reprisal
		/if (${Select[${Me.Class.ShortName},NEC]}) /varset spellSlow Death's Malaise	
		/varset _spelltype alt
		/varset _readytype SpellReady
	} else /if (${Bool[${Me.Gem[${spellSlow}]}]}) {
		/varset _spelltype gem${Me.Gem[${spellSlow}]}
		/varset _readytype AltAbilityReady
	}	
	
	| check the things normal things	
	/if (${Spawn[${_tmpID}].Type.Equal[corpse]} || !${Spawn[${_tmpID}].ID} || !${Bool[${spellSlow}]}) /return
	/delay 2.25s ${Me.${_readytype}[${spellSlow}]}	
	/call core_cast "${spellSlow}" ${_spelltype} ${_tmpID} FALSE
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
	DEBUGCAST ${cbug}cast_malo(\ax${cinfo}${_tmpID}\ax${cbug})\ax
	AMIDEAD
	/doevents
	CHECKEXIT
	/if (${is_target_dead[${_tmpID}]}) /return
	
	/squelch /target ID ${_tmpID}
	/delay 1s ${Target.ID} == ${_tmpID}
	
	| dont waste time if its Maloed already
	/if (${Target.Maloed.Duration}) {
		/call create_timer timer_malo_${Target2Kill} ${Target.Maloed.Duration.TotalSeconds}s
		/return
	}
	UPDATEHUD "Malo"

	| can't touch this.... 
	/declare _spelltype string local FALSE
	/declare _readytype string local FALSE

	| spell not identified, using AA.
	/if (${spellMalo.Equal[NULL]} || !${Bool[${spellMalo}]} || !${Bool[${Me.Gem[${spellMalo}]}]}) {
		/if (${Select[${Me.Class.ShortName},MAG]}) /varset spellMalo Malosinete
		/if (${Select[${Me.Class.ShortName},SHM]}) /varset spellMalo Malosinete
		/if (${Select[${Me.Class.ShortName},NEC]}) /varset spellMalo Scent of Terris
		/varset _spelltype alt
		/varset _readytype SpellReady
	} else /if (${Bool[${Me.Gem[${spellMalo}]}]}) {
		/varset _spelltype gem${Me.Gem[${spellMalo}]}
		/varset _readytype AltAbilityReady
	}		
	
	| check the things normal things	
	/if (${Spawn[${_tmpID}].Type.Equal[corpse]} || !${Spawn[${_tmpID}].ID} || !${Bool[${spellMalo}]}) /return
	/delay 2.25s ${Me.${_readytype}[${spellMalo}]}	
	/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${MobAgro} playerstate 4]} > 2 && ${Select[${Me.Class.ShortName},MAG]}) {
		/call core_cast "Wind of Malosinete" alt ${_tmpID} FALSE
	} else {
		/call core_cast "${spellMalo}" ${_spelltype} ${_tmpID} FALSE
	}	
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
	DEBUG ${cbug}cast_cripple(ID: ${_tmpID})\ax 
	/if (${timer_cripple_${_tmpID}}) /return
	AMIDEAD
	CHECKEXIT
	/doevents
	CHECKTIE

	/squelch /target ID ${_tmpID}
	/delay 1s ${Target.ID} == ${_tmpID}

	| if its crippled, move on
	/if (${Target.Crippled.Duration}) {
		/call create_timer timer_cripple_${_tmpID} ${Target.Crippled.Duration.TotalSeconds}s
		/return
	}

	| get to business
	/declare _spelltype string local FALSE

	| spell not identified, using AA.
	/if (${spellCripple.Equal[NULL]} || !${Bool[${spellCripple}]} || !${Bool[${Me.Gem[${spellCripple}]}]}) {
		/if (${Select[${Me.Class.ShortName},ENC]}) /varset spellCripple Dreary Deeds
		/if (${Select[${Me.Class.ShortName},SHM]}) /varset spellCripple Crippling Spirit
		/if (${Select[${Me.Class.ShortName},WIZ]}) /varset spellCripple Lower Element
		/varset _spelltype alt
	} else /if (${Bool[${Me.Gem[${spellCripple}]}]}) {
		/varset _spelltype gem${Me.Gem[${spellCripple}]}
	}	

	| cripple spell already exists on
	/if (${Target.BuffDuration[${spellCripple}].TotalSeconds}) {
		/call create_timer timer_cripple_${_tmpID} ${Spell[${spellCripple}].Duration.TotalSeconds}s
		/return		
	}

	| check the things normal things	
	/if (${Spawn[${_tmpID}].Type.Equal[corpse]} || !${Spawn[${_tmpID}].ID} || !${Bool[${spellCripple}]}) /return

	UPDATEHUD "Cripple"	
	DEBUG CRIPPLE CASTING
	/delay 2.25s ${Me.SpellReady[${spellCripple}]}	
	/call core_cast "${spellCripple}" ${_spelltype} ${_tmpID} check_for_adds
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
	DEBUG ${cbug}set_base_target_timers(ID: ${_tmpID})\ax
	/if (${switchBanestrike} && !${Defined[timer_banestrike_${_tmpID}]}) /call create_timer timer_banestrike_${_tmpID} 1
	/if (${SetStickBehind} && !${Defined[timer_snap_recheckID_${_tmpID}]}) /call create_timer timer_snap_recheckID_${_tmpID} 1
	/if (${switchdeBuffEradicate} && !${Defined[timer_eradicate_${_tmpID}]}) /call create_timer timer_eradicate_${_tmpID} 1
	/if (${switchdeBuffTash} && !${Defined[timer_tash_${_tmpID}]}) /call create_timer timer_tash_${_tmpID} 1
	/if (${switchdeBuffSlow} && !${Defined[timer_slow_${_tmpID}]}) /call create_timer timer_slow_${_tmpID} 1
	/if (${switchdeBuffCripple} && !${Defined[timer_cripple_${_tmpID}]}) /call create_timer timer_cripple_${_tmpID} 1
	/if (${switchdeBuffMezz} && !${Defined[timer_mez_${_tmpID}]}) /call create_timer timer_mez_${_tmpID} 1
	/if (${switchdeBuffSnare} && !${Defined[timer_snare_${_tmpID}]}) /call create_timer timer_snare_${_tmpID} 1
	/if (${switchdeBuffMalo} && !${Defined[timer_malo_${_tmpID}]}) /call create_timer timer_malo_${_tmpID} 1
/return 
 
 
 
|***
 *	DES: cycles all debuff routines.
 *	USE: check_deBuffs
 *	INI: 
 *  NOTE: 
 ***|
sub check_deBuffs(int _tmpID)
	DEBUG ${cbug}check_deBuffs(ID:${_tmpID})\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	CHECKTIE
	
	/call set_base_target_timers ${Target2Kill}

	/if (${Target.ID} != ${_tmpID}) /squelch /target ID ${_tmpID}
	
	| tash
	/if (${switchdeBuffTash}) {
	 	/if (!${Bool[${timer_tash_${_tmpID}}]}) {
			/if (${validate_class[FALSE, |ENC]}) { 
				/call cast_tash ${_tmpID}
				/call check_for_adds
				/if (${addList.Count[|]}) /return
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
			/if (${validate_class[FALSE, |SHM]}) {
				/if (${SpawnCount[npc radius ${MobAgro} playerstate 4]} > 2) {
					/call core_cast "${spellAESlow}" 0 ${_tmpID} FALSE
				} else {
					/call cast_slow ${_tmpID}
				}
			} else {			
				/call cast_slow ${_tmpID}
			}
			/if (${validate_class[FALSE, |ENC]}) { 
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}
		}
	}
		
	| cripple
	/if (${switchdeBuffCripple}) {
		DEBUG check_deBuffs(ID:${_tmpID}) CRIPPLE 
		/if (!${Bool[${timer_cripple_${_tmpID}}]}) {		
			/if (!${validate_class[FALSE, |DRU|WIZ]}) { 
				/if (${Spell[${spellCripple}].ID} != ${Spell[${spellSlow}].ID}) /call cast_cripple ${_tmpID}
				/if (${validate_class[FALSE, |ENC]}) {
					/call check_for_adds
					/if (${addList.Count[|]}) /return
				}
			} else /if (${validate_class[FALSE, |DRU]}) {
				/call cast_AA_ro ${_tmpID}
			} else /if (${validate_class[FALSE, |WIZ]}) {
				DEBUG Wizzy Cripple
				/call cast_cripple ${_tmpID}
			}
		}
	}
	
	| eradicate magic AA line
	/if (${switchdeBuffEradicate} && ${validate_class[FALSE, ${AAEradicateList}]}) {
		/if (!${Bool[${timer_eradicate_${_tmpID}}]}) {
			/call cast_eradicate_magic ${_tmpID}
			/if (${validate_class[FALSE, |ENC]}) { 
				/call check_for_adds
				/if (${addList.Count[|]}) /return
			}
		}
	}
	
	| aa snare check
	/if (${switchdeBuffSnare} && ${validate_class[FALSE, ${AASnareList}]}) {
		/if (!${Bool[${timer_snare_${_tmpID}}]}) {
			/call cast_AA_snare ${_tmpID}
		}
	}

/return



|***
 *	DES: checks the build increments. if theres a varriance, will check INIs
 *	USE: check_deBuffs
 *	INI: 
 *  NOTE: 
 ***|
sub verify_build()
	DEBUG ${cbug}verify_build()\ax
	| if mismatch #s
	/if (${Int[${classBuild}]} != ${Int[${coreBuild}]}) {

		/if (${forceINIcheck}) {
			| clean INIs?
			/call fix_INI_declares	
			
			| fix INI version builds
			/call writeINI classBuild common ${coreBuild}
			
			| clean out all old aliases.
			/call whitewash TRUE
			
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
	DEBUG ${cbug}bandolier_swap(band: ${_band})\ax
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
 *  11	Prophecy of Ro													12	The Serpent's Spine
 *  13	The Buried Sea													14	Secrets of Faydwer
 *  15	Seeds of Destruction										16	Underfoot
 *  17	House of Thule													18	Veil of Alaris
 *  19	Rain of Fear														20	Call of the Forsaken
 *  21	The Darkened Sea												22	The Broken Mirror
 *  23	Empires of Kunark												24	Ring of Scale		 
 *   
 ***|
sub validate_expansion(bool _error, string _exp)
	DEBUG ${cbug}validate_expansion(error: ${_error}, expansion: ${_exp})\ax
	/declare _pass bool local TRUE
	/declare _count int local 0

	/for _count 1 to ${_exp.Count[|]}
		AMIDEAD
		DEBUG _count:${_count}, arg: ${_exp.Arg[${_count},|]}
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
	DEBUG ${cbug}validate_debuff(debuff: ${_deBuffType})\ax

	| check mezz switch
	/if (${Bool[${_deBuffType.Equal[mezz]}]} || ${switchdeBuffMezz}) {
		/if (!${validate_class[${_error}, |ENC|BRD|MAG|NEC]}) {
			/call writeINI switchdeBuffMezz Buffs FALSE  
			/return FALSE
		}
	}
	
	| check malo switch
	/if (${Bool[${_deBuffType.Equal[malo]}]} || ${switchdeBuffMalo}) {
		/if (!${validate_class[${_error}, |MAG|SHM]}) {
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
		/if (!${validate_class[${_error}, ${AASnareList}]}) {
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
DEBUGREZ ${cbug}watch_for_death()\ax

	| check for current flags. watch for rez box if I have them
	/if (${switchAmIDead}) /return TRUE
	/if (${flagCallNeedRez}) /return TRUE

	| check death states
	/if (${Me.State.Equal[HOVER]} || ${Me.Hovering} || ${Me.Dead}) {
		/if (${Me.Casting.ID}) /stopcast
		/varset switchAmIDead TRUE
	}
	
	| did I just die and get sent to bind ?
	/if (${Me.Buff[Resurrection Sickness].ID} && ${Zone.ID} == ${Me.ZoneBound.ID}) /varset switchAmIDead TRUE

	| if we died, shut shit down
	/if (${switchAmIDead}) {
		DEBUGREZ into rez, all event markers cleared. delay 2s to let script catch up.
		/doevents flush
		/delay 2s
		DEBUGREZ update death count
		/ini "${INICore}" "deathcount" "${MacroQuest.Server}.${Me.Class.ShortName}.${Me.DisplayName}" "${Int[${Math.Calc[${Ini[${INICore},deathcount,${MacroQuest.Server}.${Me.Class.ShortName}.${Me.DisplayName}]}+1]}]}"

		DEBUGREZ stop all the things(s)
		/call set_stop TRUE
		/call clear_combat
		/if (${Me.Casting.ID}) /stopcast
		/varset Target2Kill 0
		VBC Needs a rez: "${Me.DisplayName}" 0 0 
	}
	DEBUGREZ ${cbug}watch_for_death[${switchAmIDead}]\ax
/return ${switchAmIDead}	
	


|***
 *	DES: watches for the rez box if we have bben marked as dead or called.
 *	USE: /call  watch_for_rezbox()
 *	INI: 
 *  NOTE: 
 ***|
sub watch_for_rezbox()
DEBUGREZ ${cbug}watch_for_rezbox()\ax
	
	| dont rezz me
	/if (!${switchRezTake}) /return
	
	| not dead? GO AWAY
	/if (!${switchAmIDead} && !${flagCallNeedRez}) /return
		
	| bail if no rez box to check
	/if (!${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["Do you wish this?"]}]}) /return

	| is there a rez window?
	/if (${Window[ConfirmationDialogBox].Open}) {

	| /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["Do you wish this?"]}]}) {
		DEBUGREZ rezbox open. yay us.

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
			DEBUGREZ rezWindowPct = ${rezWindowPct}
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
			DEBUGREZ found call to corpse. setting flagCallNeedRez\ax
			/varset flagCallNeedRez TRUE
			/varset validRezFound TRUE
		} 
			
		| more waiting
		DEBUGREZ is valid or not?
		/if (${validRezFound}) {
			DEBUGREZ yes we want a rez
			/nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
			/delay 5
			DEBUGREZ sorting rez selection window
			/if (${Window[RespawnWnd].Open}) {
				/if (${Zone.ShortName.Equal[Powar]}) {
					DEBUGREZ option 1 in respawn window\ax
					/nomodkey /notify RespawnWnd RW_OptionsList listselect 1
				} else {
					DEBUGREZ option 2 in respawn window\ax
					/nomodkey /notify RespawnWnd RW_OptionsList listselect 2 
				}
				/delay 1s
				DEBUGREZ click RW_SelectButton
				/nomodkey /notify RespawnWnd RW_SelectButton leftmouseup
			}

			:cleardeadmarkers
				DEBUGREZ delays for rez status to clear
				/delay 2s !${Me.State.Equal[HOVER]}
				/delay 2s !${Me.Hovering}
				/delay 2s !${Me.Dead}
				| DEBUGREZ clear HUD tmpMode variable
				DEBUGREZ Exit rez, clear event marker
				| /if (${switchAmIDead} || ${tmpMode}) {
					/call set_stop TRUE
					| /varset tmpMode FALSE
					/varset switchAmIDead FALSE
					/varset validRezFound FALSE
					/varset rezWindowPct 0
					| /call create_timer clear_dead_event_marker 1s
					| /delay 5
					| /goto :cleardeadmarkers
				| }
			/return TRUE			
		} else /if (!${validRezFound}) {
			DEBUGREZ This rez sucks. and not in a good way...
			/nomodkey /notify ConfirmationDialogBox No_Button leftmouseup	
			/delay 5
			/return FALSE
		} 
	}

	DEBUGREZ ${cgood}EXIT\ax ${cbug}watch_for_rezbox((${Me.State.Equal[HOVER]} || ${Me.Hovering} || ${Me.Dead}) && ${switchRezTake} || ${switchAmIDead})\ax
	DEBUGREZ call flagCallNeedRez: ${flagCallNeedRez}
/return



|***
 *	DES: this is for waisting AAs. it does nothing more. if you expect it to do more, go away.
 *	USE: /call cast_AA_fireworks()
 *	INI: 
 *  NOTE: 
 ***|
sub cast_AA_fireworks()
	DEBUG ${cbug}cast_AA_fireworks(AA:${Me.AAPoints} Max:${Int[${Math.Calc[${Me.Level}*2]}]})\ax
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
		/call core_cast "Glyph of Fireworks II" alt 0 FALSE
	}
/return



 |***
 *	DES: click self healing items
 *	USE: /call click_item_heal
 *	INI: 
 *  NOTE: 
 ***|
sub click_item_heal()
	DEBUG ${cbug}click_item_heal()\ax
	CHECKEXIT
	AMIDEAD
	/declare _count int local 0
	
	/for _count 1 to ${clickHealItem.Count[|]}
		/if (${Me.PctHPs} >= ${${Me.Class.ShortName}Point}*.${tmpHealOverride}) /break
		/if (!${FindItem[${clickHealItem.Arg[${_count},|]}].ID}) /continue
		/if (!${Me.ItemReady[${clickHealItem.Arg[${_count},|]}]}) /continue
		
		/call core_cast "${clickHealItem.Arg[${_count},|]}" item 0 FALSE
	/next _count		
/return



 |***
 *	DES: make sure you have the correct zone INI info loaded
 *	USE: /call click_item_heal
 *	INI: 
 *  NOTE: 
 ***|
sub validate_zone(bool _error, bool _force)
	DEBUG ${cbug}validate_zone(error: ${_error}, force: ${_force})\ax
	/if (${timer_validate_zone}) /return
	/call create_timer timer_validate_zone ${tValidate_Zone}
	/declare _zoned bool local FALSE

	| zone matches. nothing to see here. move on
	/if (${Zone.ShortName.Equal[${currentZone}]}) {
		| do nothing at all

	| zone does not match. wtf..
	} else 	/if (${Zone.ShortName.NotEqual[${currentZone}]} || ${_force}) {
		/varset currentZone ${Zone.ShortName}
		/call echos zone 0 0 0 0 
		/call check_zone_INI FALSE
		/varset _zoned TRUE
		/call create_timer timer_zone_pause ${tZone_Pause}
		/doevents flush
	}

	| yah. we zoned and stuff
	/if (${_zoned}) {
		/if (${burnForce}) /varset burnForce FALSE
		/squelch /makecamp off
		/if (${HomeSet}) /call writeINI HomeSet Movement FALSE
		/call switch_clear_mode
		/if (${coreAuto} && ${switchShrink}) /call check_shrink
	}
/return



 |***
 *	DES: Buys an AA
 *	USE: /call cbuy_AA "AA NAME"
 *	INI: 
 *  NOTE: 
 ***|
sub buy_AA(String _name)
	DEBUG ${cbug}buy_AA(AA: ${_name})\ax
	/if (!${Bool[${_name}]}) /return

	/if (${AltAbility[${_name}].CanTrain}) {
		| Me.AltAbility will return NULL if at rank 0, which requires purchase via .Index
		/if (${Me.AltAbility[${_name}].CanTrain}) {
			/alt buy ${AltAbility[${_name}].NextIndex}
		} else {
			/alt buy ${AltAbility[${_name}].Index}
		}
	}
/return



