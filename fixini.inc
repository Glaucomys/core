|***
 *	fixini.inc
 *	Core INI Utility
 *
 *
 ***|
 
 
 
 
|***
 *	DES: This will clear all store alises that core has created in your MacroQuest2.INI file. or those with matching names.
 *	USE: /core whitewash [NOECHO]
 *	INI: MacroQuest2.INI
 *  NOTE: IF you instance/sandbox MQ/EQ it is best you run this on all toons at the same time.
 ***|
sub whitewash(string _echo)
	/if (!${_echo.Equal[NOECHO]}) VOUT ${owarning} ${sep} Deleting all Core ${cinfo}${Float[${coreBuild}].Precision[2]}\ax aliases. ${cbad}Please Wait\ax
	/declare _count 				int local 0
	/declare _todelete[1] 	string local
	/declare _index 				int local 0
	/declare _increment			int local 1
	
	| core aliases
	/varset _todelete[1] |take|door|stack|here|sendpets|killmob|mirror|stein|stop|blurb|bcsay|echoaugments|autologin|rc|purify|buy|sell|on|off|zonereset|givecoin|oneoff|gtfo|lesson|clean|checkvar|bcsay
	
	/for _count 1 to ${_todelete.Size}
		/for _index 1 to ${_todelete[${_count}].Count[|]}
			/if (!${_echo.Equal[NOECHO]}) VOUT Deleting Alias: ${cinfo}${_todelete[${_count}].Arg[${_index},|]}\ax
			/squelch /alias /${_todelete[${_count}].Arg[${_index},|]} delete
			/varcalc _increment ${_increment}+1
		/next _index	
	/next _count
	
	/if (!${_echo.Equal[NOECHO]}) {
		/call echos ending
		/endm
	}
/return 
 
  
 
|***
 *	DES: adjust the related class ini
 *	USE: /call writeINI [variable] [INISection] [value]
 *	INI: 
 *  NOTE: 
 ***|
sub writeINI(string _varName, string _section, string _value)
	/if (${Defined[${_varName}]} && ${Ini[${INIClass},${_section},${_varName},"${_value}",noparse].NotEqual[${_value}]}) {
		/varset ${_varName} ${_value}
	 	/ini "${INIClass}" ${_section} "${_varName}" "${_value}"
	}
/return



|***
 *	DES: adjust the related Core.INI
 *	USE: /call writeCoreINI [variable] [INISection] [value]
 *	INI: 
 *  NOTE: 
 ***|
sub writeCoreINI(string _varName, string _section, string _value)
	/if (${Defined[${_varName}]} && ${Ini[${INICore},${_section},${_varName},"${_value}",noparse].NotEqual[${_value}]}) {
		/varset ${_varName} ${_value}
	 	/ini "${INICore}" ${_section} "${_varName}" "${_value}"
	}
/return



|***
 *	DES: INI controller: will createnew entries and read/adapt current entries for use in memory
 *	USE: /call setINIVar SKILLNAME int|bool|string [ini sub section] "variable value"
 *	INI: 
 *  NOTE: 
 ***|
sub setINIVar(string _name, string _type, string _section, string _value)

	/if (!${Bool[${_name}]} || !${Bool[${_type}]} || !${Bool[${_section}]}) {
		VOUT ${owarning} ${sep} INVALID INI SETTING/STRING [_name:${_name}/_type:${_type}/_section:${_section}] --\ax
		/return
	}
	
	| create the variable
	/if (!${Defined[${_name}]}) /declare ${_name} ${_type} outer
	
	/if (!${Bool[${Ini[${INIClass},${_section}].Find[${_name}]}]}) {
		/ini "${INIClass}" "${_section}" "${_name.Left[1].Upper}${_name.Right[-1]}" "${_value}"
		/varset ${_name} ${_value}
	} else {
		 /varset ${_name} ${Ini[${INIClass},${_section},${_name.Left[1].Upper}${_name.Right[-1]},"${_value}",noparse]}
	}

	/if (${Me.Book[${Me.Book[${${_name}} Rk. III]}].ID}) {
		/varset ${_name} ${Me.Book[${Me.Book[${${_name}} Rk. III]}].Name} 
	} else /if (${Me.Book[${Me.Book[${${_name}} Rk. II]}].ID}) {
		/varset ${_name} ${Me.Book[${Me.Book[${${_name}} Rk. II]}].Name}
	} else /if (${Me.CombatAbility[${${_name}} Rk. III]}) {
		/varset ${_name} ${${_name}} Rk. III
	} else /if (${Me.CombatAbility[${${_name}} Rk. II]}) {
		/varset ${_name} ${${_name}} Rk. II
	}
/return






|***
 *	DES: INI controller: will create new entries and read/adapt current entries for use in memory
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub iniControl(bool _error, string _file, string _name, string _type, string _section, string _value)
	/if (${_error}) SDEBUG passed -- file:${_file}, name:${_name}, type:${_type}, section:${_section}, value:${_value}

	/declare _tmpName string local
	/declare _count int local 0
	
	| set core.ini variables
	/if (${_file.Equal[core]}) {
		/varset _file INICore
	
	| set the class ini variables
	} else /if (${_file.Equal[class]}) {
		/varset _file INIClass
		
	| write to core ini only (small updates)
	} else /if (${_file.Equal[writecore]}) {
		/if (${Defined[${_name}]} && ${Ini[${INICore},${_section},${_name},"${_value}",noparse].NotEqual[${_value}]}) {
			/varset ${_name} ${_value}
		 	/ini "${INICore}" ${_section} ${_name} ${_value}
		}
		/return TRUE
		
	| write to class ini only (small updates)
	} else /if (${_file.Equal[writeclass]}) {
		/if (${Defined[${_name}]} && ${Ini[${INIClass},${_section},${_name},"${_value}",noparse].NotEqual[${_value}]}) {
			/varset ${_name} ${_value}
		 	/ini "${INIClass}" ${_section} "${_name}" "${_value}"
		}
		/return TRUE

	| just reset the variable.. and go away
	} else /if (${_file.Equal[declare]}) {
		/if (${Defined[${_name}]}) {
			/varset ${_name} ${_value}
		} else {
			/declare ${_name} ${_type} outer ${_value}
		}
		/return TRUE
		
	| bad operations
	} else {
		OUT ${owarning} bad INI in operation
		/return FALSE
	}

	| define the variable
	/if (!${Defined[${_name}]}) {
		/declare ${_name} ${_type} outer
	}

	| create the INI if it does not exist
	/if (!${Bool[${Ini[${${_file}},${_section}].Find[${_name}]}]}) {

		/if (!${Bool[${_value}]} && ${_value.NotEqual[FALSE]}) {
			/for _count 1 to ${Ini[${INIPreGen},${Me.Class.ShortName},${_name}].Count[|]}
				/if (${Me.Level} >= ${Ini[${INIPreGen},${Me.Class.ShortName},${_name}].Arg[${_count},|]}) {
					/ini "${${_file}}" "${_section}" "${_name}" "${Ini[${INIPreGen},${Me.Class.ShortName},${_name},noparse].Arg[${Math.Calc[${_count}+1]},|]}"
					/break
				}
				/varset _count ${Math.Calc[${_count}+1]}	
			/next _count
		} else {
			/ini "${${_file}}" "${_section}" "${_name}" "${_value}"
		}

		/varset ${_name} ${_value}


	| or read the INI if it does exist
	} else {
		
		/if (${correctINICase}) {
			/for _count 1 to ${Ini[${${_file}},${_section}].Count[|]}
				/varset _tmpName ${Ini[${${_file}},${_section}].Arg[${_count},|]}

				/if (${_tmpName.Equal[${_name}]} && ${_tmpName.NotEqualCS[${_name}]}) {
					/varset _value ${Ini[${${_file}},${_section},${_name},"${_value}",noparse]}
					
					OUT Fixing [${cnum}${_section}\ax] -> ${cnum}${_name}\ax
					| delete the ini entry
					/ini "${${_file}}" "${_section}" "${_name}" NULL	

					| rebuild the shit with proper case sensitive
					/ini "${${_file}}" "${_section}" "${_name}" "${_value}"

					| reset the varible data
					/varset ${_name} ${Ini[${${_file}},${_section},${_name},"${_value}",noparse]}
					/break
				}
		
			/next _count
		}
		
		/varset ${_name} ${Ini[${${_file}},${_section},${_name},"${_value}",noparse]}
	}
	
	/if (${_error}) {
		SDEBUG read -- file:${_file}, name:${_name}, type:${_type}, section:${_section}, data:${${_name}}
		/echo ${_name}
	}	
	

	| reset _name variable if it is a spell with a rank
	| we do this for the odd spells that do not have ranks that proc spells that do
	|**
	/if (${Me.Book[${Me.Book[${${_name}} Rk. III]}].ID}) {
		/varset ${_name} ${Me.Book[${Me.Book[${${_name}} Rk. III]}].Name} 
	} else /if (${Me.Book[${Me.Book[${${_name}} Rk. II]}].ID}) {
		/echo here
		/varset ${_name} ${Me.Book[${Me.Book[${${_name}} Rk. II]}].Name}
	} else /if (${Me.CombatAbility[${${_name}} Rk. III]}) {
		/varset ${_name} ${${_name}} Rk. III
	} else /if (${Me.CombatAbility[${${_name}} Rk. II]}) {
		/varset ${_name} ${${_name}} Rk. II
	}
	
	| /if (${Me.Book[${Spell[${${_name}}].RankName}]}) /varset ${_name} ${Spell[${_name}].RankName} 
	**|
	
	
	/if (${Me.Book[${${_name}} Rk. III]}) {
		/varset ${_name} ${${_name}} Rk. III
	} else /if (${Me.Book[${${_name}} Rk. II]}) {
		| /varset _name ${Spell[${_name}].RankName}
		/varset ${_name} ${${_name}} Rk. II
	} else /if (${Me.CombatAbility[${${_name}} Rk. III]}) {
		/varset ${_name} ${${_name}} Rk. III
	} else /if (${Me.CombatAbility[${${_name}} Rk. II]}) {
		/varset ${_name} ${${_name}} Rk. II
	}	

	
	
	/if (${_error}) {
		SDEBUG ${_name}:${${_name}}
		SDEBUG 
	}
	


/return TRUE




${Spell[Frozen Toxin Rk. II].ID}
${Spell[Frozen Toxin].RankName}

${Spell[Rake].RankName}


${Me.CombatAbility[${Spell[Rake].RankName}]}


Frozen Toxin


Frostbite Lance




${Me.Book[Frozen Toxin Rk. II]}

${Me.Book[Frozen Toxin]}












|***
 *	DES: creates INI entries for core.ini
 *	USE: 
 *	INI: 
 *  NOTE: 
  ***|
sub DeclareCoreIniVar(string name, string varType, string section, string value)
	/if (!${Bool[${name}]} || !${Bool[${varType}]} || !${Bool[${section}]}) {
		/echo ${cbad}-- INVALID INI SETTING/STRING [${name}/${varType}/${section}] --\ax
		/return
	}

	/if (!${Defined[${name}]}) /declare ${name} ${varType} outer
	/varset ${name} ${Ini[${INICore},${section},${name.Left[1].Upper}${name.Right[-1]},"${value}",noparse]}
	/if (!${Bool[${Ini[${INICore},${section}].Find[${name}]}]}) {
		/ini "${INICore}" "${section}" "${name.Left[1].Upper}${name.Right[-1]}" "${${name}}" "${${value}}"
	}
/return



|***
 *	DES: Updates the list controls names
 *	USE: 
 *	INI: core.zone.ini
 *  NOTE: 
 ***|
sub update_zone_list(string _list)
	/if (!${Target.ID}) {
		/call echos needtarget
	} else /if (${Target.ID} && ${${_list}.Find[${Target.DisplayName}]}) {
		/call echos listexists ${_list}
	} else /if (${Target.ID} && !${Bool[${Spawn[pc ID ${Target.ID}]}]} && !${${_list}.Find[${Target.DisplayName}]}) {
		/if (${Target.ID}) ${If[${Bool[${${_list}}]},/ini "${INIZone}" "${Zone.ShortName}" "${_list}" "${${_list}}|${Target.DisplayName}",/ini "${INIZone}" "${Zone.ShortName}" "${_list}" "|${Target.DisplayName}"]}
		/if (${validate_plugin[FALSE, |mq2eqbc]} && ${EQBC.Connected}) /bc zonereset
	}
/return 
 


|***
 *	DES: check/create the zone base INI entries.
 *	USE: /call check_zone_INI
 *	INI: 
 *  NOTE: 
 ***| 
sub check_zone_INI(string _sender)
	/if (${Bool[${_sender}]} && !${Spawn[pc ${_sender}].ID}) /return

	/declare _count 						int local	0
	/declare _listtype 					string local	
	/declare _list 							string local ${ZoneFileList}
	/declare _tmpString					string local
	
	/if (${Bool[${Ini[${INIZone},"${Zone.ShortName}"]}]} && ${Bool[${Ini[${INIZone},"${Zone.ShortName}","FullZoneName"]}]}) {
		/varset FullZoneName ${Ini[${INIZone},"${Zone.ShortName}","FullZoneName"]}
		/varset SafeZone ${Ini[${INIZone},"${Zone.ShortName}","SafeZone"]}
		
		/for _count 1 to ${_list.Count[|]}
			/varset _listtype ${_list.Arg[${_count},|]}
			/if (!${Bool[${Ini[${INIZone},"${Zone.ShortName}","${_listtype}"]}]}) /ini "${INIZone}" "${Zone.ShortName}" "${_listtype}" ""
			/varset ${_listtype} ${Ini[${INIZone},"${Zone.ShortName}","${_listtype}"]}
		/next _count				

		/return
	} else /if (${Bool[${Ini[${INIZone},"${Zone.Name}"]}]} || !${Bool[${Ini[${INIZone},"${Zone.ShortName}"]}]} || !${Bool[${Ini[${INIZone},"${Zone.ShortName}","FullZoneName"]}]}) {
		| start swap. create a full zone name INI
		/if (!${Bool[${Ini[${INIZone},"${Zone.Name}","SafeZone"]}]} && ${Bool[${Ini[${INIZone},"${Zone.ShortName}","SafeZone"]}]}) {
			/ini "${INIZone}" "${Zone.Name}" "SafeZone" ${Ini[${INIZone},"${Zone.ShortName}","SafeZone"]}
		} else /if (!${Bool[${Ini[${INIZone},"${Zone.Name}","SafeZone"]}]} && !${Bool[${Ini[${INIZone},"${Zone.ShortName}","SafeZone"]}]}) {
			/ini "${INIZone}" "${Zone.Name}" "SafeZone" "FALSE"
		}
		
		/for _count 1 to ${_list.Count[|]}
			/varset _listtype ${_list.Arg[${_count},|]}
			
			| create full name
			/if (!${Bool[${Ini[${INIZone},"${Zone.Name}","${_listtype}"]}]} && ${Bool[${Ini[${INIZone},"${Zone.ShortName}","${_listtype}"]}]}) {
				/varset _tmpString ${Ini[${INIZone},"${Zone.ShortName}","${_listtype}"]}
				/ini "${INIZone}" "${Zone.Name}" "${_listtype}" "${_tmpString}"
			} else /if (!${Bool[${Ini[${INIZone},"${Zone.Name}","${_listtype}"]}]} && !${Bool[${Ini[${INIZone},"${Zone.ShortName}","${_listtype}"]}]}) {
				/ini "${INIZone}" "${Zone.Name}" "${_listtype}" ""
			}
		/next _count	

		| create new short name section entries	
		
		| delete the short name and start the recreation	
		/if (${Bool[${Ini[${INIZone},"${Zone.ShortName}"]}]} && ${Zone.ShortName.NotEqual[${Zone.Name}]}) /ini "${INIZone}" "${Zone.ShortName}" NULL NULL
		
		| create full zone name and load
		/ini "${INIZone}" "${Zone.ShortName}" "FullZoneName" "${Zone.Name}"
		/varset FullZoneName ${Ini[${INIZone},"${Zone.ShortName}","FullZoneName"]}		

		| create safezone and load 
		/ini "${INIZone}" "${Zone.ShortName}" "SafeZone" ${Ini[${INIZone},"${Zone.Name}","SafeZone"]}
		/varset SafeZone ${Ini[${INIZone},"${Zone.ShortName}","SafeZone"]}
		
		| create mob entries and load
		/for _count 1 to ${_list.Count[|]}
			/varset _listtype ${_list.Arg[${_count},|]}
			/if (${Bool[${Ini[${INIZone},"${Zone.Name}","${_listtype}"]}]}) {
				/varset _tmpString ${Ini[${INIZone},"${Zone.Name}","${_listtype}"]}
				/ini "${INIZone}" "${Zone.ShortName}" "${_listtype}" "${_tmpString}"
			} else /if (!${Bool[${Ini[${INIZone},"${Zone.Name}","${_listtype}"]}]} ) {
				/ini "${INIZone}" "${Zone.ShortName}" "${_listtype}" ""
			}
			/varset ${_listtype} ${Ini[${INIZone},"${Zone.ShortName}","${_listtype}"]}
		/next _count	
		
		| delete the full name and hope this shit works
		/if (${Bool[${Ini[${INIZone},"${Zone.Name}"]}]} && ${Zone.Name.NotEqual[${Zone.ShortName}]}) /ini "${INIZone}" "${Zone.Name}" NULL NULL
	}
	
	/if (${autoAddNameList}) {
		/if (!${Bool[${NameList}]}) /call fix_INI_declares namelist
	}
	
/return 


 
|***
 *	DES: Used to remove stale entries from the toons INI. 'cause I gotta keep changing shits.
 *	USE: /call fix_INI_declares
 *	INI: 
 *	NOTE: may look a little strange, but I leave the old variables here for history. Just in case.
 ***|
sub fix_INI_declares(string _verbage)
	/if (!${Bool[${_verbage}]}) {
		/call fix_ini_current
		VOUT INI Housekeeping ${sep} ${cgood}DONE\ax
	} else /if (${Bool[${_verbage.Equal[old]}]}) {
		/call fix_ini_old
		VOUT INI Housekeeping Old Data${sep} ${cgood}DONE\ax
	} else /if (${Bool[${_verbage.Equal[namelist]}]}) {
		/call fix_ini_namedlist
		VOUT INI Housekeeping ${Zone} ${sep} ${cgood}DONE\ax
	}

/return

 
 
 |***
 *	DES: adds named mobs to the core.zone.ini file
 *	USE: /core fixini namelist
 *	INI: 
 *  NOTE: 
 ***| 
sub fix_ini_namedlist()
	/varset currentdebugsub fix_ini_namedlist()
	/ini "${INIZone}" "${Zone.ShortName}" "NameList" "${Ini[tc\namelistdata.ini,${Zone},NameList]}"
	/varset NameList "${Ini[${INIZone},${Zone.ShortName},NameList]}"
/return 
 
 

 |***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub fix_ini_current()
	/declare _tmpVar string local


	| core.ini entries adjusted here EQBCPort
	
	
		| fix the deathcount variable structure
		/if (${Bool[${Ini[${INICore},deathcount,"${MacroQuest.Server.Left[1].Upper}${MacroQuest.Server.Right[-1]}.${Me.Class.ShortName}.${Me.DisplayName}"]}]}) {
			/varset _tmpVar ${Ini[${INICore},deathcount,"${MacroQuest.Server.Left[1].Upper}${MacroQuest.Server.Right[-1]}.${Me.Class.ShortName}.${Me.DisplayName}"]}
			/ini "${INICore}" "deathcount" "${MacroQuest.Server.Left[1].Upper}${MacroQuest.Server.Right[-1]}.${Me.Class.ShortName}.${Me.DisplayName}" NULL
			/ini "${INICore}" "deathcount" "${MacroQuest.Server.Left[1].Upper}${MacroQuest.Server.Right[-1]}_${Me.Class.ShortName}_${Me.DisplayName}" ${_tmpVar}
			
			/varset ${MacroQuest.Server.Left[1].Upper}${MacroQuest.Server.Right[-1]}_${Me.Class.ShortName}_${Me.DisplayName} ${_tmpVar}
			
		}

		| 20180926	
		/ini "${INICore}" "General" "setTGB" NULL
		/ini "${INICore}" "General" "EQBCServer" NULL
		/ini "${INICore}" "General" "EQBCPort" NULL
	
		| 20180822
		| /call DeclareCoreIniVar mqlog										string	General FALSE
		/ini "${INICore}" "General" "mqlog" NULL


		| 20180429
		/ini "${INICore}" "General" "switchAlwaysFaceTarget" NULL
		/ini "${INICore}" "Here" "combatWAR" NULL
		/ini "${INICore}" "Here" "combatPAL" NULL
		/ini "${INICore}" "Here" "combatSHD" NULL
		/ini "${INICore}" "Here" "combatCLR" NULL
		/ini "${INICore}" "Here" "combatSHM" NULL
		/ini "${INICore}" "Here" "combatDRU" NULL
		/ini "${INICore}" "Here" "combatMAG" NULL
		/ini "${INICore}" "Here" "combatENC" NULL
		/ini "${INICore}" "Here" "combatWIZ" NULL
		/ini "${INICore}" "Here" "combatNEC" NULL
		/ini "${INICore}" "Here" "combatROG" NULL
		/ini "${INICore}" "Here" "combatBER" NULL
		/ini "${INICore}" "Here" "combatBST" NULL
		/ini "${INICore}" "Here" "combatRNG" NULL
		/ini "${INICore}" "Here" "combatBRD" NULL
		/ini "${INICore}" "Here" "combatMNK" NULL
		
	
	| toon INI non specific class --------------------------------------------------------------------------------------
	
	
		| 201811 food redesign
		/ini "${INIClass}" "Meal" "clickFoodDispenser" NULL	
		/ini "${INIClass}" "Meal" "clickFoodItem" NULL	
		/ini "${INIClass}" "Meal" "clickFoodCount" NULL	
		/ini "${INIClass}" "Meal" "clickDrinkDispenser" NULL	
		/ini "${INIClass}" "Meal" "clickDrinkItem" NULL	
		/ini "${INIClass}" "Meal" "clickDrinkCount" NULL	
		/ini "${INIClass}" "Timer" "tClick_FoodDrink" NULL	


	
		| 201811
		/ini "${INIClass}" "Spells" "DiscRest" NULL	
		/ini "${INIClass}" "Spells" "DiscFastRune" NULL	
		/ini "${INIClass}" "Spells" "DefensiveDisc1" NULL	
		/ini "${INIClass}" "Spells" "DefensiveDisc2" NULL	
		/ini "${INIClass}" "Spells" "DefensiveDisc3" NULL	
	
	
	
		| 202180921
		/ini "${INIClass}" "Override" "switchOverrideRangecombat" NULL	
		/ini "${INIClass}" "Override" "switchOverrideMeleecombat" NULL	
	
		| remove the resident classes	
		/ini "${INIClass}" "Common" "ResidentMage" NULL	
		/ini "${INIClass}" "Common" "ResidentDruid" NULL	
		/ini "${INIClass}" "Common" "ResidentWizard" NULL	

		/ini "${INIClass}" "Common" "verbose" NULL	
		/ini "${INIClass}" "Override" "switchOverrideCombatheal" NULL	

		| 20180831.00
		/ini "${INIClass}" "debug" NULL NULL
		
		| 20180718.00 -- removal of banestrike
		/ini "${INIClass}" "Common" "switchBanestrike" NULL	
		/ini "${INIClass}" "Common" "BanestrikeCondition" NULL	
		
		| 20180624.00
		/ini "${INIClass}" "Common" "InGameCashChecked" NULL	


		| 20180611.00
		/ini "${INIClass}" "Common" "stopnukeAt" NULL	
		/ini "${INIClass}" "Itme" "Epic" NULL		
		

		| 20180522.00
		/ini "${INIClass}" "Common" "useSmartAssistR" NULL	
	
		| fix the range summon INI entries
		/if (!${RangeItem.Count[|]}) /call writeINI RangeItem Common |"${RangeItem}"
		/if (!${RangeItemSummon.Count[|]}) /call writeINI RangeItemSummon Common |"${RangeItemSummon}"		
	
		| 20180429
		/ini "${INIClass}" "Common" "combat" NULL	

		| 20180506
		/ini "${INIClass}" "Common" "ClassChannel" NULL	

	| class Specific
	
		| BERZERKER
		/if (${Select[${Me.Class.ShortName},BER]}) {
			
		| BARD
		} else /if (${Select[${Me.Class.ShortName},BRD]}) {
			
		| BEASTLORD
		} else /if (${Select[${Me.Class.ShortName},BST]}) {
			
			| 20180630
			/ini "${INIClass}" "Spells" "selfParagon" NULL
			/ini "${INIClass}" "Spells" "groupParagon" NULL
			/ini "${INIClass}" "Spells" "FastHealPCT" NULL
			/ini "${INIClass}" "Spells" "minConsumptionHP" NULL
			
		| CLERIC
		} else /if (${Select[${Me.Class.ShortName},CLR]}) {
			
			| corrections to AA fading and checks
			/ini "${INIClass}" "Buffs" "AAFade" NULL
			/ini "${INIClass}" "Buffs" "useAAFade" NULL
			/ini "${INIClass}" "Buffs" "useVP" NULL

			| remove cleric QM from 20 sep 2017 patch
			/ini "${INIClass}" "Buffs" "useQM" NULL
			/ini "${INIClass}" "Buffs" "useQMPct" NULL
			/ini "${INIClass}" "Timer" "tcheck_QM" NULL

		| DRUID
		} else /if (${Select[${Me.Class.ShortName},DRU]}) {

			| adjust AA fade controls
			/ini "${INIClass}" "Buffs" "AAFade" NULL
			/ini "${INIClass}" "Buffs" "useAAFade" NULL

			
		| ENCHANTER
		} else /if (${Select[${Me.Class.ShortName},ENC]}) {
			
		| MAGICIAN
		} else /if (${Select[${Me.Class.ShortName},MAG]}) {

			| adjust AA fade controls
			/ini "${INIClass}" "Buffs" "AAFade" NULL
			/ini "${INIClass}" "Buffs" "useAAFade" NULL

			
			| 20180527
			/ini "${INIClass}" "Spells" "DoNuke" NULL
			/ini "${INIClass}" "Spells" "Nuke1" NULL
			/ini "${INIClass}" "Spells" "Nuke1Condition" NULL
			/ini "${INIClass}" "Spells" "Nuke2" NULL
			/ini "${INIClass}" "Spells" "Nuke2Condition" NULL
			/ini "${INIClass}" "Spells" "Nuke3" NULL
			/ini "${INIClass}" "Spells" "Nuke3Condition" NULL
			/ini "${INIClass}" "Spells" "Nuke4" NULL
			/ini "${INIClass}" "Spells" "Nuke4Condition" NULL
			/ini "${INIClass}" "Spells" "NukeTwincast" NULL
			
			
		| MONK
		} else /if (${Select[${Me.Class.ShortName},MNK]}) {
		
		| NECROMANCER
		} else /if (${Select[${Me.Class.ShortName},NEC]}) {
			
		| PALADIN
		} else /if (${Select[${Me.Class.ShortName},PAL]}) {
			
			| 20181118
			/ini "${INIClass}" "Discs" "DiscJudgement" NULL

			/ini "${INIClass}" "Discs" "DiscHate" NULL
			/ini "${INIClass}" "Discs" "DiscTwincast" NULL
			
			| 20180602
			/ini "${INIClass}" "Heal" "healPanic" NULL
			/ini "${INIClass}" "Heal" "healWholeGroup" NULL

		| RANGER
		} else /if (${Select[${Me.Class.ShortName},RNG]}) {
			
			| 20181128
			/ini "${INIClass}" "Spells" "DiscRange" NULL
			/ini "${INIClass}" "Spells" "DiscMele" NULL
			/ini "${INIClass}" "Spells" "DiscJolt" NULL
			/ini "${INIClass}" "Spells" "DiscEnrage" NULL
			/ini "${INIClass}" "Spells" "DiscHeal" NULL
			/ini "${INIClass}" "Spells" "BladeStorm" NULL
			/ini "${INIClass}" "Spells" "DiscSqual" NULL



			/ini "${INIClass}" "Spells" "SlowHeal" NULL
			/ini "${INIClass}" "Spells" "FastHeal" NULL
			/ini "${INIClass}" "Spells" "FastHealPCT" NULL
			
			| 20180429
			/ini "${INIClass}" "Spells" "ElementalArrow" NULL
			/ini "${INIClass}" "Spells" "GlacialArow" NULL
			/ini "${INIClass}" "Spells" "VolatileArrow" NULL
			/ini "${INIClass}" "Spells" "ArrowPoison" NULL
			/ini "${INIClass}" "Spells" "ArrowFire" NULL
			/ini "${INIClass}" "Spells" "ArrowIce" NULL

		} else /if (${Select[${Me.Class.ShortName},ROG]}) {

		| SHADOWKNIGHT
		} else /if (${Select[${Me.Class.ShortName},SHD]}) {

			| 20181118
			/ini "${INIClass}" "Spells" "spellUndeadCripple" NULL




		| SHAMAN
		} else /if (${Select[${Me.Class.ShortName},SHM]}) {
			
			/ini "${INIClass}" "Heal" "healReckless" NULL
			
			| 20180501
			/ini "${INIClass}" "Spells" "CaniPercent" NULL

		| WARRIOR
		} else /if (${Select[${Me.Class.ShortName},WAR]}) {
			
			| 20180501
			/ini "${INIClass}" "Discs" "AAVhementRage" NULL

		| WIZARD
		} else /if (${Select[${Me.Class.ShortName},WIZ]}) {
		}

/return 



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub fix_ini_old()
		
	/ini "${INIClass}" "Common" "SetModeInvis" NULL
	/ini "${INIClass}" "Common" "SetScorchedEarthMode" NULL
	/ini "${INIClass}" "Common" "SetSkillsMode" NULL
	/ini "${INIClass}" "Common" "SetFishMode" NULL
	/ini "${INIClass}" "Common" "SetForageMode" NULL
	/ini "${INIClass}" "Common" "SetDragMode" NULL
	/ini "${INIClass}" "Common" "Looter" NULL
	/ini "${INIClass}" "Common" "ForageFishInvis" NULL
	/ini "${INIClass}" "Common" "SetHunterMode" NULL
	/ini "${INIClass}" "Common" "SetHunterAgro" NULL
	/ini "${INIClass}" "Common" "SetPetFarmMode" NULL
	/ini "${INIClass}" "Movement" "SetIgnoreLOS" NULL
	/ini "${INICore}" "General" "QPath" NULL
	/ini "${INICore}" "Timer" "tPetFarm_delay" NULL
	/ini "${INIClass}" "Spells" "HuntNuke" NULL
	/ini "${INIClass}" "Common" "setRaidInvite" NULL
	/ini "${INIClass}" "Common" "setAgroMode" NULL
	/ini "${INIClass}" "Common" "setPullMode" NULL
	/ini "${INIClass}" "Common" "setToastMode" NULL
	/ini "${INIClass}" "Common" "switchdeBuffMezz" NULL
	/ini "${INIClass}" "Common" "switchdeBuffMalo" NULL
	/ini "${INIClass}" "Common" "switchdeBuffTash" NULL
	/ini "${INIClass}" "Common" "switchdeBuffSlow" NULL
	/ini "${INIClass}" "Common" "switchdeBuffEradicate" NULL
	/ini "${INIClass}" "Common" "switchdeBuffSnare" NULL
	/ini "${INIClass}" "Common" "switchdeBuffCripple" NULL
	/ini "${INIClass}" "Common" "setToastMode" NULL
	/ini "${INIClass}" "Common" "BuffCombat" NULL
	/ini "${INIClass}" "Common" "BuffSelf" NULL
	/ini "${INIClass}" "Common" "BuffBC" NULL
	/ini "${INIClass}" "Common" "BuffRaid" NULL
	/ini "${INIClass}" "Common" "BuffTell" NULL
	/ini "${INIClass}" "Common" "BuffPet" NULL
	/ini "${INIClass}" "Common" "BuffItem" NULL
	/ini "${INIClass}" "Common" "BuffAuto" NULL	
	/ini "${INIClass}" "Buffs" "BuffCombat" NULL
	/ini "${INIClass}" "Buffs" "BuffSelf" NULL
	/ini "${INIClass}" "Buffs" "BuffBC" NULL
	/ini "${INIClass}" "Buffs" "BuffRaid" NULL
	/ini "${INIClass}" "Buffs" "BuffTell" NULL
	/ini "${INIClass}" "Buffs" "BuffPet" NULL
	/ini "${INIClass}" "Buffs" "BuffItem" NULL	
	/ini "${INIClass}" "Buffs" "BuffAuto" NULL	
	/ini "${INIClass}" "Spells" "useBuffKill" NULL
	/ini "${INIClass}" "Buffs" "BuffCount" NULL
	/ini "${INIClass}" "Spells" "SpellSet" NULL
	/ini "${INIClass}" "Spells" "Spire" NULL
	/ini "${INIClass}" "Common" "autoCursor" NULL	
	/ini "${INIClass}" "Common" "switchPullMode" NULL	
	/ini "${INIClass}" "Common" "switchAgroMode" NULL	
	/ini "${INIClass}" "Heal" "rezAuto" NULL	
	/ini "${INIClass}" "Heal" "rezEmergency" NULL	
	/ini "${INIClass}" "Heal" "rezEveryone" NULL	
	/ini "${INIClass}" "Heal" "rezCombat" NULL	
	/ini "${INIClass}" "Heal" "rezEnable" NULL	
	/ini "${INIClass}" "Heal" "switchRezAtuo" NULL	
	/ini "${INIClass}" "Heal" "switchRezEnable" NULL	
	/ini "${INIClass}" "Buffs" "switchRezEmergency" NULL	
	/ini "${INIClass}" "Buffs" "switchRezAuto" NULL	
	/ini "${INIClass}" "Buffs" "switchRezCombat" NULL	
	/ini "${INIClass}" "Buffs" "switchRezEveryone" NULL	
	/ini "${INICore}" "General" "RezTokenList" NULL	
	/ini "${INIClass}" "Pet" "PetTank" NULL
	/ini "${INIClass}" "Pet" "sendPetPerc" NULL
	/ini "${INIClass}" "Pet" "usePet" NULL
	/ini "${INIClass}" "Pet" "sendPetSwarmPerc" NULL
	/ini "${INIClass}" "Pet" "usePetSwarm" NULL
	/ini "${INIClass}" "Movement" "SetTie" NULL
	/ini "${INIClass}" "Common" "UseBanestrike" NULL
	/ini "${INIClass}" "Common" "ModeNuke" NULL
	/ini "${INIClass}" "Common" "AutoDeleteItem" NULL
	/ini "${INIClass}" "Common" "CombatBuff" NULL
	/ini "${INIClass}" "Common" "BalanceHP" NULL
	/ini "${INIClass}" "Common" "DoDebuff" NULL
	/ini "${INIClass}" "Common" "Mount" NULL
	/ini "${INIClass}" "Common" "HealOverride" NULL
	/ini "${INIClass}" "Spells" "RuneSpell" NULL
	/ini "${INIClass}" "Spells" "RuneType" NULL
	/ini "${INIClass}" "Spells" "CauldronDestroy" NULL
	/ini "${INIClass}" "Common" "AutoSit" NULL
	/ini "${INIClass}" "Common" "AutoBuff" NULL
	/ini "${INIClass}" "Common" "UseCharm" NULL
	/ini "${INIClass}" "Common" "SetTankMode" NULL
	/ini "${INIClass}" "Common" "AutoBot" NULL
	/ini "${INIClass}" "Common" "forcesafelist" NULL
	/ini "${INIClass}" "SelfBuff" NULL NULL
	/ini "${INIClass}" "Transport" NULL NULL
	/ini "${INICore}" "bitch" NULL NULL
	/ini "${INICore}" "Synergy" NULL NULL
	/ini "${INICore}" "Output" NULL NULL
	/ini "${INIClass}" "Common" "setAutoShrink" NULL
	/ini "${INIClass}" "Common" "HornofUnity" NULL
	/ini "${INIClass}" "Heal" "MercWarPoint" NULL
	/ini "${INIClass}" "Heal" "MercCLRPoint" NULL
	/ini "${INIClass}" "Heal" "MercDPSpoint" NULL
	/ini "${INIClass}" "Heal" "healTouch" NULL
	/ini "${INIClass}" "Common" "ToastMobCount" NULL
	/ini "${INIClass}" "Common" "WhentoToast" NULL
	/ini "${INIClass}" "Common" "AutoToast" NULL
	/ini "${INIClass}" "Common" "MyDeathCount" NULL
	/ini "${INIClass}" "Common" "BCMA" NULL
	/ini "${INIClass}" "Timer" "AutoInvTime" NULL
	/ini "${INIClass}" "Common" "BuffAuto" NULL
	/ini "${INIClass}" "Common" "BuffCombat" NULL
	/ini "${INIClass}" "Common" "BuffSelf" NULL
	/ini "${INIClass}" "Timer" "SummerTime" NULL
	/ini "${INIClass}" "Timer" "CheckAmmo" NULL
	/ini "${INIClass}" "Timer" "CheckTieTime" NULL
	/ini "${INIClass}" "Timer" "CheckFamiliar" NULL
	/ini "${INIClass}" "Common" "EQBCServer" NULL
	/ini "${INIClass}" "Common" "EQBCPort" NULL
	/ini "${INIClass}" "Common" "EQBCChannel" NULL
	/ini "${INIClass}" "Timer" "ClearCursorTime" NULL
	/ini "${INIClass}" "Timer" "tCheckTieTime" NULL
	/ini "${INIClass}" "Timer" "SelfBuffTime" NULL
	/ini "${INIClass}" "Timer" "ItemBuffTime" NULL
	/ini "${INIClass}" "Timer" "CheckEXPTime" NULL
	/ini "${INIClass}" "Timer" "TellBuffTime" NULL
	/ini "${INIClass}" "Timer" "BCBuffTime" NULL
	/ini "${INIClass}" "Timer" "RaidBuffTime" NULL
	/ini "${INIClass}" "Timer" "CheckEQCBTime" NULL
	/ini "${INIClass}" "Timer" "AuraTime" NULL
	/ini "${INIClass}" "Timer" "PetBuffTime" NULL
	/ini "${INIClass}" "Timer" "RezRetryTime" NULL
	/ini "${INIClass}" "Timer" "BitchTime" NULL
	/ini "${INIClass}" "Timer" "BuffKillTime" NULL
	/ini "${INIClass}" "Timer" "XHealTime" NULL
	/ini "${INIClass}" "Common" "setPetFarmMode" NULL
	/ini "${INIClass}" "Common" "autoRez" NULL
	/ini "${INIClass}" "Heal" "CombatCall" NULL
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
	/ini "${INIClass}" "Common" "switchInvites" NULL
	/ini "${INIClass}" "Common" "switchRaidInvite" NULL
		
	/if (${Me.Class.ShortName.Equal[BRD]}) {
		/ini "${INIClass}" "meleedisc" "seloskick" NULL
	}

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
			
/return

