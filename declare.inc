|***
 *	declare.inc
 *	Core Declares common and class specific.
 *	All manual forced events.
 *	All created alias', binds and /bc commands
 *	All INI edits
 *
 ***|






|***
 *	DES: sets an outer variable
 *	USE: /call setDeclare [variable Name] [variable type] outer [value]
 *	INI: 
 *  NOTE: 
 ***|
sub setDeclare(string _name, string _type, string _section, string _value)
	| /echo \asetDeclare\ax(${_name.Left[1].Upper}${_name.Right[-1]} -> ${_type} -> ${_value})
	/if (${Defined[${_name}]}) {
		/varset ${_name} ${_value}
	} else {
		/declare ${_name} ${_type} outer ${_value}
	}
/return



|***
 *	DES: adjust the related class ini
 *	USE: /call writeINI [variable] [INISection] [value]
 *	INI: 
 *  NOTE: 
 ***|
sub writeINI(string _varName, string _section, string _value)
	DEBUG ${cbug}writeINI([${_section}] -> ${_varName.Left[1].Upper}${_varName.Right[-1]} -> ${_value})\ax 
	/if (${Defined[${_varName}]} && ${Ini[${INIClass},${_section},${_varName},"${_value}",noparse].NotEqual[${_value}]}) {
		/varset ${_varName} ${_value}
	 	/ini "${INIClass}" ${_section} "${_varName.Left[1].Upper}${_varName.Right[-1]}" "${_value}"
	}
/return



|***
 *	DES: adjust the related Core.INI
 *	USE: /call writeCoreINI [variable] [INISection] [value]
 *	INI: 
 *  NOTE: 
 ***|
sub writeCoreINI(string _varName, string _section, string _value)
	DEBUG ${cbug}writeCoreINI([${_section}] -> ${_varName.Left[1].Upper}${_varName.Right[-1]} -> ${_value})\ax 
	/if (${Defined[${_varName}]} && ${Ini[${INICore},${_section},${_varName},"${_value}",noparse].NotEqual[${_value}]}) {
		/varset ${_varName} ${_value}
	 	/ini "${INICore}" ${_section} "${_varName.Left[1].Upper}${_varName.Right[-1]}" "${_value}"
	}
/return



|***
 *	DES: adjust the related ini
 *	USE: /call Event_SetaliasVar NA [variable] [INISection] [value]
 *	INI: 
 *  NOTE: THIS IE BEING DONE AWAY WITH

#event SetaliasVar 			"[MQ2] - Core: #1# -> [#2#] -> #3#"
sub Event_SetaliasVar(string line, string _varName, string _section, string _value)
	/call writeINI ${_varName} ${_section} ${_value}
/return
 ***|


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
 *	DES: creates entries for the toon INI
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub setINIVar(string _name, string _type, string _section, string _value)
| sub DeclareIniVar(string _name, string _type, string _section, string _value)
	| /echo \atDeclareIniVar\ax(_name:\a-w${_name}\ax, _type:\a-w${_type}\ax, _section:\a-w${_section}\ax, _value:\a-w${_value}\ax)
	/if (!${Bool[${_name}]} || !${Bool[${_type}]} || !${Bool[${_section}]}) {
		VOUT ${owarning} ${sep} INVALID INI SETTING/STRING [_name:${_name}/_type:${_type}/_section:${_section}] --\ax
		/return
	}
	
	| create the variable
	/if (!${Defined[${_name}]}) /declare ${_name} ${_type} outer
	
	/if (!${Bool[${Ini[${INIClass},${_section}].Find[${_name}]}]}) {
		| /echo \arINI writing\ax: ${Me.DisplayName} "${_section}" "${_name.Left[1].Upper}${_name.Right[-1]}" "${_value}"
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
		| /if (${Bool[${Plugin[MQ2EQBC]}]} && ${EQBC.Connected}) /bc zonereset
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
	DEBUG ${cbug}check_zone_INI(_sender: ${_sender})\ax 
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
/return 



|***
 *	DES: class INI files for each toon
 *	USE: /call set_declares
 *	INI: 
 *  NOTE: 
 ***|
sub set_declares()
	/call setDeclare coreBuild											int			outer 20180324
	/call setINIVar classBuild											int 		Common 0
	/call setDeclare forceINIcheck									bool		outer TRUE
	/call setDeclare castReturn 										string 	outer NULL
	/declare _count 																int 		local 0
	/call setDeclare combat													string	outer
		
	/call setDeclare isValidTarget									bool		outer FALSE

	| HUD controls
	/call setDeclare hcType													string	outer "|time|update|broadcast|delete"
	/call setDeclare hudNotice											string	outer FALSE
	/call setDeclare hudWarning											string	outer	FALSE
	/call setDeclare hudTarNotice										string	outer FALSE
	/call setDeclare hudBroadcast										string	outer FALSE
	/call setDeclare tmpMode												string	outer FALSE
	/call setDeclare hudDeathCount									int			outer	0
		
	/call DeclareCoreIniVar HudName									string	General Core
	/call DeclareCoreIniVar HudType									string	General normal
	/call setINIVar HUDTime													bool 		Common TRUE
	/call setINIVar setBroadcast										string	Common TRUE	

	| Base Environment settings
	/call setDeclare noInvis 												bool 		outer TRUE
	/call setDeclare noFeigning 										bool 		outer TRUE
	/call setDeclare Target2Kill										int			outer 0
	/call setDeclare currentLootCount								int			outer	0

	/call DeclareCoreIniVar plist										string	Plugin "|MQ2Cast|MQ2EQBC|MQ2Exchange|MQ2MoveUtils|MQ2Netbots|MQ2Debuffs"
	/call DeclareCoreIniVar EQBCServer							string	General 127.0.0.1
	/call DeclareCoreIniVar EQBCPort								string	General 2112
	/call DeclareCoreIniVar mqlog										string	General FALSE
	/call DeclareCoreIniVar minMobPullLvl						int			General 1
	/call DeclareCoreIniVar maxMobPullLvl						int			General 150
	/call DeclareCoreIniVar autoinihousekeeping			string	General FALSE
	/call DeclareCoreIniVar smartasserrors					string	General TRUE
	/call DeclareCoreIniVar forceNukeLoop						string	General FALSE
 			
		|** Output Color Display 
			Default			\ax							Black				\ab
			Blue				\au							Brown				\a-o
			Cyan				??							Gray				\a-w
			Green				\ag							Pink				\am
			Orange			\ao							Purple			\ap
			Red					\ar							Teal				\at
			White				\aw							Yellow			\ay
			DarkGreen		\a-g						DarkMaroon	\a-m
			DarkPurple	\a-p						DarkRed			\a-r
			DarkCyan		\a-t						DarkBlue		\a-u
			DarkYellow	\a-y
		**|
	
	| visual outputs
	/call setDeclare cnum														string	outer "\a-t"
	/call setDeclare ctar														string	outer "\a-w"
	/call setDeclare cpc														string	outer "\a-w"
	/call setDeclare csp														string	outer "\a-r"
	/call setDeclare cbad														string	outer "\ar"
	/call setDeclare cgood													string	outer "\ag"
	/call setDeclare c-															string	outer "\at"
	/call setDeclare cbug														string	outer "\at"
	/call setDeclare cinfo													string	outer "\aw"
	/call setDeclare citem													string	outer "\au"
	/call setDeclare sep														string	outer "\at..\ax"
	/call setDeclare sepO														string	outer "\at[\ax"
	/call setDeclare sepC														string	outer "\at]\ax"
	/call setDeclare oenabled												string	outer "${cgood}Enabled\ax"
	/call setDeclare oon														string	outer "${cgood}on\ax"
	/call setDeclare odisabled											string	outer "${cbad}DISABLED\ax"
	/call setDeclare ooff														string	outer "${cbad}off\ax"
	/call setDeclare owarning												string	outer "${cbad}WARNING\ax"
	/call setDeclare ofatal													string	outer "${cbad}FATAL ERROR\ax ${sep} ${cbad}ENDING MACRO\ax"
	/call setDeclare ocaution												string	outer "${cbad}CAUTION\ax"
	/call setDeclare otrue													string	outer "${cgood}TRUE\ax"
	/call setDeclare ofalse													string	outer "${cbad}FALSE\ax"
	/call setDeclare odestroy												string	outer "${cbad}DESTROY\ax"
	/call setDeclare oleave													string	outer "${cinfo}Leave\ax"
	/call setDeclare oannounce											string	outer "\amAnnounce\ax"
	/call setDeclare oadd														string	outer "${cgood}Add\ax"
	/call setDeclare okeep													string	outer "${cgood}Keep\ax"
	/call setDeclare onotice												string	outer "\ayNotice\ax"
	| /declare prelink												string	outer "3Rorcex^11000004^0^1^1362119591^10000^'"
 	| /declare postlink												string	outer ""	
 	/call setDeclare ocombat												string	outer "\amCombat\ax"	
	/call setDeclare oremove												string	outer "${cbad}Removing\ax"

	| ? colors
	/call setDeclare wikiswitch											string outer "[\a-wswitch\ax]"
	/call setDeclare wikitarget											string outer "[\a-wTARGET\ax]"
	/call setDeclare wikiname												string outer "[\a-wName\ax]"
	/call setDeclare wikispire											string outer "[\ar0\ax\a-w|1|2|3\ax]"
	/call setDeclare wikipct												string outer "[\a-w1,99\ax]"


	| echo switches
	/call setDeclare echoType 											string 	outer "|alt|bc|channel|disc|item|melee|spell|relay|verbose"
	/call setINIVar switchEchoAlt										bool		Echo TRUE
	/call setINIVar switchEchoSpell									bool		Echo TRUE
	/call setINIVar switchEchoDisc									bool		Echo TRUE
	/call setINIVar switchEchoItem									bool		Echo TRUE
	/call setINIVar switchEchoMelee									bool		Echo TRUE
	/call setINIVar switchEchoBC										bool		Echo FALSE
	/call setINIVar echoChannel											string	Echo FALSE
	/call setINIVar relayTells											string	Echo FALSE
	/call setINIVar verbose													bool		Echo TRUE

	 	
	| lists for .. you know..lists and stuff.
	/call setDeclare AllClassList										string 	outer "|WAR|SHD|PAL|BER|ROG|MNK|RNG|BST|BRD|NEC|ENC|WIZ|MAG|CLR|SHM|DRU"
	/call setDeclare MeleeList    		  						string 	outer "|WAR|ROG|BST|MNK|SHD|PAL|BER|BRD|RNG"
	/call setDeclare CastList												string 	outer "|BST|SHD|PAL|RNG|CLR|DRU|SHM|ENC|WIZ|NEC|MAG"
	/call setDeclare PetList												string 	outer "|ENC|DRU|SHM|SHD|WIZ|BRD|BST|MAG"
	/call setDeclare HealerList											string 	outer "|CLR|DRU|SHM|PAL"
	/call setDeclare RezerList											string 	outer "|CLR|DRU|SHM|PAL|NEC"
	/call setDeclare AASnareList										string 	outer "|RNG|DRU|SHD|NEC|ROG|WAR|WIZ|MNK|PAL"
	/call setDeclare AAEradicateList								string 	outer "|RNG|DRU|NEC|WIZ|ENC|MAG"
		
		
	| core control
	/call setDeclare coreType 											string 	outer "|auto|control|events|expadjust|expmaintain|explevel|fireworks|fixini|invite|killcode|dbcash|raidinvite|reset|rest|whitewash"
	/call setDeclare coreObscure										bool		outer FALSE
	/call setINIVar coreAuto				 								bool	 	Common FALSE
	/call DeclareCoreIniVar remoteKillKey						string	General "CHANGE ME" 
	/call DeclareCoreIniVar ${MacroQuest.Server}.${Me.Class.ShortName}.${Me.DisplayName}				int			DeathCount 0
	/call DeclareCoreIniVar invitelist							string	No FALSE
	/call DeclareCoreIniVar rezlist									string	No FALSE
	/call DeclareCoreIniVar heallist								string	No FALSE
	/call DeclareCoreIniVar safelist								string	General "|FALSE|FALSE"

	/call DeclareCoreIniVar switchDispInfo					string	General TRUE
	/call DeclareCoreIniVar switchDispBuild					string	General TRUE
	/call DeclareCoreIniVar switchDispEXP						string	General TRUE
	/call DeclareCoreIniVar switchAADiscnowManual		string	General FALSE
	/call DeclareCoreIniVar switchAlwaysFaceTarget	string	General FALSE
	/call DeclareCoreIniVar switchDelayWhileZoning	string	General FALSE
	/call DeclareCoreIniVar switchAutoRestFull			string	General FALSE
	/call DeclareCoreIniVar switchModRodinRest			string	General TRUE
	/call DeclareCoreIniVar switchBuffinRest				string	General TRUE
	/call DeclareCoreIniVar switchUseCLSinHUD				string	General FALSE
	
	
	/call setINIVar toonControl			 								string	Common FALSE
	/call setINIVar autoRest 												int 		Common 90
	/call setINIVar AutoExpAdjust 									bool 		Common TRUE
	/call setINIVar MaintExpLvl   									int 		Common 99
	/call setINIVar MaxLevel      									int 		Common 110
	/call setINIVar coreEventsInc  									bool 		Common TRUE
	/call setINIVar switchAAFireworks								bool	 	Common FALSE


	| OCD Shit - 
	/call DeclareCoreIniVar setMQFontSize						int			General 2
	/call DeclareCoreIniVar setMQCaptions						string	General off
	/call DeclareCoreIniVar setTGB									string	General on
	/call DeclareCoreIniVar setAutoAssist						string	General off

	
	| buff queue variables
	/call DeclareCoreIniVar queueCountMax						int			General 10
	/call setDeclare queueCount            					int  		outer 0
	
	/if (!${Defined[queueBuffID]}) /declare queueBuffID[8] 												int  		outer
	/if (!${Defined[queueList]}) /declare queueList[${queueCountMax},2] 				 	int  		outer
	
		
	| set /crew stuffs
	/call DeclareCoreIniVar main													string	Group "|FALSE|FALSE"E
	/call DeclareCoreIniVar main2													string	Group "|FALSE|FALSE"
	/call DeclareCoreIniVar ${MacroQuest.Server}Leader		string	Group FALSE
	/call DeclareCoreIniVar ${MacroQuest.Server}Assist		string	Group FALSE
	/call DeclareCoreIniVar ${MacroQuest.Server}Looter		string	Group FALSE
	

	| /here settings
	/call DeclareCoreIniVar combatWAR								string	Here MELEE
	/call DeclareCoreIniVar combatPAL								string	Here MELEE
	/call DeclareCoreIniVar combatSHD								string	Here MELEE
	/call DeclareCoreIniVar combatCLR								string	Here HEAL
	/call DeclareCoreIniVar combatSHM								string	Here HEAL
	/call DeclareCoreIniVar combatDRU								string	Here DUAL
	/call DeclareCoreIniVar combatMAG								string	Here NUKE
	/call DeclareCoreIniVar combatENC								string	Here DUAL
	/call DeclareCoreIniVar combatWIZ								string	Here NUKE
	/call DeclareCoreIniVar combatNEC								string	Here NUKE
	/call DeclareCoreIniVar combatMNK								string	Here MELEE
	/call DeclareCoreIniVar combatROG								string	Here MELEE
	/call DeclareCoreIniVar combatBER								string	Here MELEE
	/call DeclareCoreIniVar combatBST								string	Here MELEE
	/call DeclareCoreIniVar combatRNG								string	Here MELEE
	/call DeclareCoreIniVar combatBRD								string	Here MELEE
	/call DeclareCoreIniVar makeVisible							bool		Here	TRUE
	/call DeclareCoreIniVar clearTarget							bool		Here	TRUE
	/call DeclareCoreIniVar defaultTimerDelay				string	Here	1m
	/call DeclareCoreIniVar baseAgroRange						int			Here	60
	/call DeclareCoreIniVar emptyHands							bool		Here	TRUE
	/call DeclareCoreIniVar hereZRadius							int			Here	30
	
	
	| please take a number and have a seat.
	/call setDeclare bitchcount											int			outer	29	
	/call setDeclare bitch1													string	outer	"Seriously... Again..."
	/call setDeclare bitch2													string	outer	"Did your parents ever ask you to run away from home?"
	/call setDeclare bitch3													string	outer	"Aha, I see the Fuck-Up Fairy has visited us again!"
	/call setDeclare bitch4													string	outer	"Will trade a Halfling for a rez, or three Gn0mes."
	/call setDeclare bitch5													string	outer	"I'm sorry I bit you... pulled your hair... and punched you in the face..."
	/call setDeclare bitch6													string	outer	"Go ahead, tell them everything you know. It'll only take 10 seconds."
	/call setDeclare bitch7													string	outer	"Sometimes the first step to forgiveness, is realising the other person was born an idiot."
	/call setDeclare bitch8													string	outer	"I'm getting Disc lag . . . again."
	/call setDeclare bitch9													string	outer	"oh man....i figured out why i was lagging the other day...i look in my case and my kids knocked off my cpu fan."
	/call setDeclare bitch10												string	outer	"I'm jealous of all the people that haven't met you!"
	/call setDeclare bitch11												string	outer	"You're slower than a herd of turtles stampeding through peanut butter."
	/call setDeclare bitch12												string	outer	"Oh good, my dog found a chainsaw."
	/call setDeclare bitch13												string	outer	"You are not as bad as people say, you are much, much worse."
	/call setDeclare bitch14												string	outer	"Stupidity is not a crime so you are free to go."
	/call setDeclare bitch15												string	outer "AFK, playing Pokemon. More productive then this shit."
	/call setDeclare bitch16												string	outer	"All I did was move the mouse over it."
	/call setDeclare bitch17												string	outer	"You sound reasonable. It must be time to up my medication!"
	/call setDeclare bitch18												string	outer	"I'll never forget the first time we met. Although, I'll keep trying."
	/call setDeclare bitch19												string	outer	"Roses are red. Violets are blue. You're an idiot."
	/call setDeclare bitch20												string	outer	"Sometimes, when I close my eyes, I... can't see you."
	/call setDeclare bitch21												string 	outer "Shock me. Say something intelligent."
	/call setDeclare bitch22												string 	outer "I'm typing this with my middle finger."
	/call setDeclare bitch23												string 	outer "It takes courage just to be you. To get out of bed every single day, knowing full well, you gotta be you."
	/call setDeclare bitch24												string 	outer "For someone with such a smart mouth you sure do have a lot of teeth."
	/call setDeclare bitch25												string 	outer "Do you want a piece of gum?"
	/call setDeclare bitch26												string 	outer "Good thing you're pretty."
	/call setDeclare bitch27												string 	outer "You breathe through your mouth a lot don't you?"
	/call setDeclare bitch28												string 	outer "Your mother was a hamster and your father smelt of elderberries!"
	/call setDeclare bitch29												string 	outer "Don't worry about it too much. I did the same thing when I was eleven."
	/call setDeclare bitch30												string 	outer 


	| expansions
	/call setDeclare expansion1											string 	outer "The Ruins of Kunark"
	/call setDeclare expansion2											string 	outer "The Scars of Velious"
	/call setDeclare expansion3											string 	outer "The Shadows of Luclin"
	/call setDeclare expansion4											string 	outer "The Planes of Power"
	/call setDeclare expansion5											string 	outer "The Legacy of Ykesha"
	/call setDeclare expansion6											string 	outer "Lost Dungeons of Norrath"
	/call setDeclare expansion7											string 	outer "Gates of Discord"
	/call setDeclare expansion8											string 	outer "Omens of War"
	/call setDeclare expansion9											string 	outer "Dragons of Norrath"
	/call setDeclare expansion10										string 	outer "Depths of Darkhollow"
	/call setDeclare expansion11										string 	outer "Prophecy of Ro"
	/call setDeclare expansion12										string 	outer "The Serpent's Spine"
	/call setDeclare expansion13										string 	outer "The Buried Sea"
	/call setDeclare expansion14										string 	outer "Secrets of Faydwer"
	/call setDeclare expansion15										string 	outer "Seeds of Destruction"
	/call setDeclare expansion16										string 	outer "Underfoot"
	/call setDeclare expansion17										string 	outer "House of Thule"
	/call setDeclare expansion18										string 	outer "Veil of Alaris"
	/call setDeclare expansion19										string 	outer "Rain of Fear"
	/call setDeclare expansion20										string 	outer "Call of the Forsaken"
	/call setDeclare expansion21										string 	outer "The Darkened Sea"
	/call setDeclare expansion22										string 	outer "The Broken Mirror"
	/call setDeclare expansion23										string 	outer "Empires of Kunark"
	/call setDeclare expansion24										string 	outer "Ring of Scale"


	| hide switches for /hidecorpse command
	/call DeclareCoreIniVar hideNone								bool		hide FALSE
	/call DeclareCoreIniVar hideAll									bool		hide FALSE
	/call DeclareCoreIniVar hideAlways							bool		hide FALSE
	/call DeclareCoreIniVar hideLooted							bool		hide FALSE
	/call DeclareCoreIniVar hideAllButGroup					bool		hide FALSE
	/call DeclareCoreIniVar hideNPC									bool		hide FALSE
	/call DeclareCoreIniVar hideLooted							bool		hide FALSE
	/call DeclareCoreIniVar hideListed							bool		hide FALSE
	/call DeclareCoreIniVar hideListedUnlocked			bool		hide FALSE
	
	
	| mode control
	/call setDeclare modeType												string	outer "|stop|agro|drag|drink|fish|forage|harvest|harvestcount|hunt|lush|petfarm|scorch|tradeskill|trainspell"
	/call setDeclare setMode												bool		outer FALSE
	/call setDeclare switchModeStop									bool		outer FALSE
	/call setDeclare switchModeAgro									bool		outer FALSE
	/call setDeclare setModeType										string	outer FALSE
	/call setDeclare setModeCount										int			outer 0
	/call setINIVar setModeAgro											int			Common FALSE 
	/call setINIVar modeHarvestCount								int			Common 999
	/call setDeclare switchModeDrag									bool		outer FALSE 
	/call setDeclare switchModeDrink								bool		outer FALSE 
	/call setDeclare switchModeForage								bool		outer FALSE
	/call setDeclare switchModePetfarm							bool		outer FALSE
	/call setDeclare switchModeScorch								bool		outer FALSE
	/call setDeclare switchModeHunt									bool		outer FALSE
	/call setDeclare switchModeFish									bool		outer FALSE
	/call setDeclare switchModeSkill								bool		outer FALSE
	/call setDeclare switchModeHarvest							bool		outer FALSE
	/call setDeclare switchModeHarvestcount					bool		outer FALSE
	/call setDeclare switchModeTradeskill						bool		outer FALSE
	/call setDeclare switchModeLush									bool		outer FALSE
	/call setDeclare switchModeTrainspell						bool		outer FALSE
	/call DeclareCoreIniVar modeDrinkofChoice				string	mode FALSE


	| burns
	/call setDeclare burnType												string	outer "|auto|count|engageat|force"
	/call setDeclare burnForce											bool		outer FALSE
	/call setINIVar burnAuto												bool		common FALSE
	/call setINIVar burnEngageat										int			common 80
	/call setINIVar burnCount												int			common 10
	

	| zone 
	/call setDeclare ZoneFileList										string 	outer "|NameList|ExcludeList|NoMezzList|RangeList|NoCastList|NoMeleeList|HuntMobList|CharmMobList|NoFireList|NoPoisonList|NoMagicList|NoCorruptionList|NoColdList|NoDiseaseList|HarvestList"
	/call setDeclare currentZone										string	outer	FALSE
	/call setDeclare RangeList		   								string 	outer FALSE
	/call setDeclare ExcludeList    								string 	outer FALSE
	/call setDeclare NameList       								string 	outer FALSE
	/call setDeclare NoMeleeList     		  					string 	outer FALSE
	/call setDeclare NoCastList       							string 	outer FALSE
	/call setDeclare NoMezzList       							string 	outer FALSE
	/call setDeclare NoDiseaseList       						string 	outer FALSE
	/call setDeclare NoMagicList       							string 	outer FALSE
	/call setDeclare NoPoisonList       						string 	outer FALSE
	/call setDeclare NoFireList       							string 	outer FALSE
	/call setDeclare NoCorruptionList      			 		string 	outer FALSE
	/call setDeclare NoColdList       							string 	outer FALSE
	/call setDeclare CharmMobList      							string 	outer FALSE
	/call setDeclare SafeZone		       							string 	outer FALSE
	/call setDeclare FullZoneName      							string 	outer FALSE
	/call setDeclare HuntMobList       							string 	outer FALSE
	/call setDeclare HarvestList       							string 	outer FALSE
	

	/call setINIVar autoControl			 								bool		Common FALSE 
	/call setINIVar noCastWith											string	Common "|Quick Defense|Protection of Bayle|Interrogation Intimidation|Energy Charge|Cloud of Terror|Faith"
	/call setDeclare canCast												bool		outer	TRUE
	/call setINIVar noMeleeWith											string	Common "|Itching Poison"
	/call setDeclare canMelee												bool		outer TRUE
	
	/call setINIVar shrinkItem 											string 	Common "Ring of the Ancients"
	/call setINIVar IllusionItem										string	Common FALSE
	/call setINIVar IllusionBuff										string	Common FALSE
	/call setINIVar GTFO														string	Common FALSE
	/call setINIVar switchAApicker									bool 		Common FALSE
	/call setINIVar RangeItem												string	Common "|FALSE|FALSE"
	/call setINIVar RangeItemSummon									string	Common "|FALSE|FALSE"
	/call setINIVar RangeSpell											string	Common FALSE
	/call setINIVar RangeClickItem									string	Common FALSE
	/call setINIVar RangeDisc												string	Common FALSE
			
	/call setINIVar autoInventoryItem								string	Common "|Summoned: Giant Modulation Shard"
	/call setINIVar stoponDS												bool		Common FALSE		
	/call setINIVar useGlyph												string	Common "Glyph of the Cataclysm"
	/call setINIVar ResidentMage										string	Common "|FALSE|FALSE"
	/call setINIVar ResidentrDruid									string	Common "|FALSE|FALSE"
	/call setINIVar ResidentWizard									string	Common "|FALSE|FALSE"
	/call setINIVar ClassChannel										int			Common FALSE	
	/call setINIVar switchSos												bool	 	Common FALSE


	| spire control
	/call setDeclare typeSpire											string	outer "|0|1|2|3"
	/call setDeclare ${Me.Class.ShortName}spire0		int		Buffs 0
	/call setINIVar spireRaid												int 	Buffs 0
	/call setINIVar spireGroup											int 	Buffs 0


	| Bard
	/call setDeclare BRDspire1											string	outer "Fundament: First Spire of the Minstrel"
	/call setDeclare BRDspire2											string	outer "Fundament: Second Spire of the Minstrel"
	/call setDeclare BRDspire3											string	outer "Fundament: Third Spire of the Minstrel"
	/call setDeclare BRDsynergyAA										string	outer "Troubadour's Synergy"
	/call setDeclare BRDEpic1												string	outer "Singing Short Sword"
	/call setDeclare BRDEpic15											string	outer "Prismatic Dragon Blade"
	/call setDeclare BRDEpic2												string	outer "Prismatic Dragon Blade"
	/call setDeclare BRDAlliance										string	outer "Alliance of Sticks and Stones"
	/call setDeclare BRDAAUnity											string	outer "FALSE"

	| Beastlord
	/call setDeclare BSTspire1											string	outer "Fundament: First Spire of the Savage Lord"
	/call setDeclare BSTspire2											string	outer "Fundament: Second Spire of the Savage Lord"
	/call setDeclare BSTspire3											string	outer "Fundament: Third Spire of the Savage Lord"
	/call setDeclare BSTsynergyAA										string	outer "Primalist's Synergy"
	/call setDeclare BSTEpic1												string	outer "Claw of the Savage Spirit"
	/call setDeclare BSTEpic15											string	outer "Savage Lord's Totem"
	/call setDeclare BSTEpic2												string	outer "Spiritcaller Totem of the Feral"
	/call setDeclare BSTAlliance										string	outer "Venomous Alliance"
	/call setDeclare BSTAAUnity											string	outer "Feralist's Unity"

	| Berzerker
	/call setDeclare BERspire1											string	outer "Fundament: First Spire of Savagery"
	/call setDeclare BERspire2											string	outer "Fundament: Second Spire of Savagery"
	/call setDeclare BERspire3											string	outer "Fundament: Third Spire of Savagery"
	/call setDeclare BERsynergyAA										string	outer "Fury's Synergy"
	/call setDeclare BEREpic1												string	outer "Kerasian Axe of Ire"
	/call setDeclare BEREpic15											string	outer "Raging Taelosian Alloy Axe"
	/call setDeclare BEREpic2												string	outer "Vengeful Taelosian Blood Axe"
	/call setDeclare BERAlliance										string	outer "Demolisher's Alliance"
	/call setDeclare BERAAUnity											string	outer FALSE

	| Cleric
	/call setDeclare CLRspire1											string	outer "Fundament: First Spire of Divinity"
	/call setDeclare CLRspire2											string	outer "Fundament: Second Spire of Divinity"
	/call setDeclare CLRspire3											string	outer "Fundament: Third Spire of Divinity"
	/call setDeclare CLRsynergyAA										string	outer "Templar's Synergy"
	/call setDeclare CLREpic1												string	outer "Water Sprinkler of Nem Ankh"
	/call setDeclare CLREpic15											string	outer "Harmony of the Soul"
	/call setDeclare CLREpic2												string	outer "Aegis of Superior Divinity"
	/call setDeclare CLRAlliance										string	outer "Divine Alliance"
	/call setDeclare CLRAAUnity											string	outer "Saint's Unity"

	| Druid
	/call setDeclare DRUspire1											string	outer "Fundament: First Spire of Nature"
	/call setDeclare DRUspire2											string	outer "Fundament: Second Spire of Nature"
	/call setDeclare DRUspire3											string	outer "Fundament: Third Spire of Nature"
	/call setDeclare DRUsynergyAA										string	outer "Preserver's Synergy"
	/call setDeclare DRUEpic1												string	outer "Nature Walker's Scimitar"
	/call setDeclare DRUEpic15											string	outer "Staff of Living Brambles"
	/call setDeclare DRUEpic2												string	outer "Staff of Everliving Brambles"
	/call setDeclare DRUAlliance										string	outer "Bosquetender's Alliance"
	/call setDeclare DRUAAUnity											string	outer "Wildtender's Unity"
	
	| Enchanter	
	/call setDeclare ENCspire1											string	outer "Fundament: First Spire of Enchantment"
	/call setDeclare ENCspire2											string	outer "Fundament: Second Spire of Enchantment"
	/call setDeclare ENCspire3											string	outer "Fundament: Third Spire of Enchantment"
	/call setDeclare ENCIoG													string	outer "Illusions of Grandeur"
	/call setDeclare ENCMC													string	outer "Mental Contortion"
	/call setDeclare ENCsynergyAA										string	outer "Begulier's Synergy"
	/call setDeclare ENCEpic1												string	outer "Staff of the Serpent"
	/call setDeclare ENCEpic15											string	outer "Oculus of Persuasion"
	/call setDeclare ENCEpic2												string	outer "Staff of Eternal Eloquence"
	/call setDeclare ENCAlliance										string	outer "Chromatic Alliance"
	/call setDeclare ENCAAUnity											string	outer "Orator's Unity"

	| Magician
	/call setDeclare MAGspire1											string	outer "Fundament: First Spire of the Elements"
	/call setDeclare MAGspire2											string	outer "Fundament: Second Spire of the Elements"
	/call setDeclare MAGspire3											string	outer "Fundament: Third Spire of the Elements"
	/call setDeclare MAGsynergyAA										string	outer "Conjurer's Synergy"
	/call setDeclare MAGEpic1												string	outer "Orb of Mastery"
	/call setDeclare MAGEpic15											string	outer "Staff of Elemental Essence"
	/call setDeclare MAGEpic2												string	outer "Focus of Primal Elements"
	/call setDeclare MAGAlliance										string	outer "Firebound Alliance"
	/call setDeclare MAGAAUnity											string	outer "Thaumaturge's Unity"

	| Monk
	/call setDeclare MONspire1											string	outer "Fundament: First Spire of the Sensei"
	/call setDeclare MONspire2											string	outer "Fundament: Second Spire of the Sensei"
	/call setDeclare MONspire3											string	outer "Fundament: Third Spire of the Sensei"
	/call setDeclare MONsynergyAA										string	outer "Calanin's Synergy"
	/call setDeclare MONEpic1												string	outer "Celestial Fists"
	/call setDeclare MONEpic15											string	outer "Fistwraps of Celestial Discipline"
	/call setDeclare MONEpic2												string	outer "Transcended Fistwraps of Immortality"
	/call setDeclare MONAlliance										string	outer "Doomwalker's Alliance"
	/call setDeclare MONAAUnity											string	outer FALSE
		
	| Necromancer
	/call setDeclare NECspire1											string	outer "Fundament: First Spire of Necromancy"
	/call setDeclare NECspire2											string	outer "Fundament: Second Spire of Necromancy"
	/call setDeclare NECspire3											string	outer "Fundament: Third Spire of Necromancy"
	/call setDeclare NECsynergyAA										string	outer "Defiler's Synergy"
	/call setDeclare NECEpic1												string	outer "Scythe of the Shadowed Soul"
	/call setDeclare NECEpic15											string	outer "Soulwhisper"
	/call setDeclare NECEpic2												string	outer "Deathwhisper"
	/call setDeclare NECAlliance										string	outer "Malevolent Alliance"
	/call setDeclare NECAAUnity											string	outer 

	| Paladin
	/call setDeclare PALspire1											string	outer "Fundament: First Spire of Holiness"
	/call setDeclare PALspire2											string	outer "Fundament: Second Spire of Holiness"
	/call setDeclare PALspire3											string	outer "Fundament: Third Spire of Holiness"
	/call setDeclare PALsynergyAA										string	outer "Knight's Synergy"
	/call setDeclare PALEpic1												string	outer "Fiery Defender"
	/call setDeclare PALEpic15											string	outer "Heartwood Blade"
	/call setDeclare PALEpic2												string	outer "Nightbane, Sword of the Valiant"
	/call setDeclare PALAlliance										string	outer "Holy Alliance"
	/call setDeclare PALAAUnity											string	outer "Divine Protector's Unity"

	| Ranger
	/call setDeclare RNGspire1											string	outer "Fundament: First Spire of the Pathfinders"
	/call setDeclare RNGspire2											string	outer "Fundament: Second Spire of the Pathfinders"
	/call setDeclare RNGspire3											string	outer "Fundament: Third Spire of the Pathfinders"
	/call setDeclare RNGauspice											string	outer "Auspice of the Hunter"
	/call setDeclare RNGcheetah											string	outer "Scarlet Cheetah's Fang"
	/call setDeclare RNGgroupguardian								string	outer "Group Guardian of the Forest"
	/call setDeclare RNGsynergyAA										string	outer "Outrider's Synergy"
	/call setDeclare RNGEpic1												string	outer "Earthcaller"
	/call setDeclare RNGEpic15											string	outer "Heartwood Blade"
	/call setDeclare RNGEpic2												string	outer "Aurora, the Heartwood Blade"
	/call setDeclare RNGAlliance										string	outer "Bosquestalker's Alliance"
	/call setDeclare RNGAAUnity											string	outer "Wildstalker's Unity (Azia)"

	| Rogue
	/call setDeclare ROGspire1											string	outer "Fundament: First Spire of the Rake"
	/call setDeclare ROGspire2											string	outer "Fundament: Second Spire of the Rake"
	/call setDeclare ROGspire3											string	outer "Fundament: Third Spire of the Rake"
	/call setDeclare ROGsynergyAA										string	outer "Blackguard's Synergy"
	/call setDeclare ROGEpic1												string	outer "Ragebringer"
	/call setDeclare ROGEpic15											string	outer "Fatestealer"
	/call setDeclare ROGEpic2												string	outer "Nightshade, Bane of Entropy"
	/call setDeclare ROGAlliance										string	outer "Poisonous Alliance"
	/call setDeclare ROGAAUnity											string	outer FALSE

	| Shadow Knight
	/call setDeclare SHDspire1											string	outer "Fundament: First Spire of the Reavers"
	/call setDeclare SHDspire2											string	outer "Fundament: Second Spire of the Reavers"
	/call setDeclare SHDspire3											string	outer "Fundament: Third Spire of the Reavers"
	/call setDeclare SHDsynergyAA										string	outer "Knight's Synergy"
	/call setDeclare SHDEpic1												string	outer "Innoruuk's Curse"
	/call setDeclare SHDEpic15											string	outer "Innoruuk's Voice"
	/call setDeclare SHDEpic2												string	outer "Innoruuk's Dark Blessing"
	/call setDeclare SHDAlliance										string	outer "Bloodletting Alliance"
	/call setDeclare SHDAAUnity											string	outer "Dark Lord's Unity (Beza)"

	| Shaman
	/call setDeclare SHMspire1											string	outer "Fundament: First Spire of Ancestors"
	/call setDeclare SHMspire2											string	outer "Fundament: Second Spire of Ancestors"
	/call setDeclare SHMspire3											string	outer "Fundament: Third Spire of Ancestors"
	/call setDeclare SHMsynergyAA										string	outer "Luminary's Synergy"
	/call setDeclare SHMEpic1												string	outer "Spear of Fate"
	/call setDeclare SHMEpic15											string	outer "Crafted Talisman of Fates"
	/call setDeclare SHMEpic2												string	outer "Blessed Spiritstaff of the Heyokah"
	/call setDeclare SHMAlliance										string	outer "Ancient Alliance"
	/call setDeclare SHMAAUnity											string	outer "Visionary's Unity"

	| Warrior
	/call setDeclare WARspire1											string	outer "Fundament: First Spire of the Warlord"
	/call setDeclare WARspire2											string	outer "Fundament: Second Spire of the Warlord"
	/call setDeclare WARspire3											string	outer "Fundament: Third Spire of the Warlord"
	/call setDeclare WARsynergyAA										string	outer "Myrmidon's Synergy"
	/call setDeclare WAREpic1												string	outer "Jagged Blade of War"
	/call setDeclare WAREpic15											string	outer "Champion's Sword of Eternal Power"
	/call setDeclare WAREpic2												string	outer "Kreljnok's Sword of Eternal Power"
	/call setDeclare WARAlliance										string	outer
	/call setDeclare WARAAUnity											string	outer FALSE

	| Wizard 
	/call setDeclare WIZspire1											string	outer "Fundament: First Spire of Arcanum"
	/call setDeclare WIZspire2											string	outer "Fundament: Second Spire of Arcanum"
	/call setDeclare WIZspire3											string	outer "Fundament: Third Spire of Arcanum"
	/call setDeclare WIZmannaburn										string	outer "Manna Burn"
	/call setDeclare WIZsynergyAA										string	outer "Evoker's Synergy"
	/call setDeclare WIZEpic1												string	outer "Staff of the Four"
	/call setDeclare WIZEpic15											string	outer "Staff of Prismatic Power"
	/call setDeclare WIZEpic2												string	outer "Staff of Phenomenal Power"
	/call setDeclare WIZAlliance										string	outer "Frostbound Alliance"
	/call setDeclare WIZAAUnity											string	outer "Etherealist's Unity"


	| set AA Unity Buffs
	/if (${Bool[${${Me.Class.ShortName}AAUnity}]}) {
		/for _count 1 to 7
			/call setINIVar BuffAAUnity${_count}		string	Buffs FALSE
		/next _count
	}


	| combat controls
	/call setDeclare ccType													string	outer "|agro|bandolier|circle|combatdistance|dual|engage|force|forceassist|heal|MA1|MA2|melee|nuke|nukedelay|pull|range|saferadius|smartengage|smartma|stopnuke|zradius"
	/call setINIVar combat					 								string	Common MELEE
	/call setINIVar MA1			 												string 	Common FALSE
	/call setINIVar MA2			 												string 	Common FALSE
	/call setINIVar MobAgro													int 		Common 60
	/call setINIVar engageAt 												int 		Common 98
	/call setINIVar smartengageAt 									int 		Common 98
	/call setINIVar	useSmartAssistG									bool		Common FALSE
	/call setINIVar	useSmartAssistR									bool		Common FALSE
	/call setINIVar	useForceAssist									bool		Common FALSE
	/call setINIVar switchAgro											bool		Common FALSE 
	/call setINIVar switchPull											bool		Common FALSE
	/call setINIVar CombatDistance									int			Movement 15
	/call setINIVar combatRadius										string	Common 30
	/call setINIVar zradius													int			Common 60	
	/call setINIVar switchBandolier									bool		meleedisc FALSE
	/call setINIVar stopnukeAt 											int 		Common 0
		
			
	| rez	control
  /call setDeclare rezType												string	outer "|stop|combat|emergency|everyone|give|pct|range|take"
  /call setDeclare rezPct													string	outer "|10|20|35|50|60|75|90|93|96"
	/call setDeclare flagCallNeedRez								bool		outer FALSE
	/call setDeclare switchAmIDead									bool		outer FALSE
	/call setDeclare validRezFound 									bool 		outer FALSE
	/call setDeclare rezWindowPct 									int 		outer 0
	/call setDeclare switchRez											bool		outer FALSE
	/call setDeclare switchRezPct										bool		outer FALSE
	/call setDeclare switchRezRange									bool		outer FALSE
	/call setDeclare switchRezStop									bool		outer FALSE
	/call setINIVar switchRezTake 									bool 		Heal TRUE 
	/call setINIVar switchRezEmergency							bool		Heal FALSE
	/call setINIVar minRezPct												int			Common 90
	/call setINIVar switchRezGive	 									bool 		Heal FALSE 
	/call setINIVar switchRezCombat 								bool 		Heal FALSE 
	/call setINIVar switchRezEveryone 							bool 		Heal FALSE 
	/call setINIVar maxRezRange				 							int 		Heal 60 
	/call setINIVar RezTokenList										string 	Heal "|WAR|SHD|PAL|BER|ROG|MNK|RNG|BST|BRD|NEC|ENC|WIZ|MAG|CLR|SHM|DRU"

	
	| watchspawn stuff
	/call setDeclare hudwatchspawn									string	outer FALSE
	/call setDeclare watchSpawnCount								int			outer	FALSE
	/call setINIVar watchSpawn											string	Common FALSE 

		
 	| Movement control
  /call setDeclare movementType										string	outer "|arc|behind|fast|gather|goto|home|leash|mount|radius"
	/call setINIVar HomeSet													bool		Movement FALSE
	/call setINIVar HomeRadius											int 		Movement 5
	/call setINIVar HomeLeash												int 		Movement 40
	/call setINIVar HomeDelayMin										int 		Movement 100
	/call setINIVar HomeDelayMax										int 		Movement 500	
	/call setINIVar SetStickBehind 									bool 		Movement FALSE
	/call setINIVar SetFaceFast											bool		Movement FALSE
	/call setINIVar SetFaceArc											int			Movement 30
	/call setINIVar mount														string	Common "Desert Kangon Saddle"
	/call setDeclare tempHomeSet										bool		outer FALSE	
	/call setDeclare tempsnapstuck									bool		outer	FALSE  

	| tie control
  /call setDeclare tcType													string	outer "|distance|lock|mode|name|time"
	/call setINIVar SetTieToon 											string 	Movement FALSE
	/call setDeclare SetTie       									bool 		outer FALSE
	/call setINIVar SetTieDistance 									int 		Movement 15 
	/call setINIVar SetTieLock	 										bool 		Movement FALSE
	/call setINIVar SetTieMode	 										string	Movement Stick


	
	| debuff switches	
  /call setDeclare deBuffType											string	outer "|stop|mezz|malo|tash|cripple|slow|eradicate|snare"
	/call setDeclare switchdeBuff										bool		outer FALSE
	/call setDeclare switchdeBuffStop								bool		outer FALSE
	/call setINIVar switchdeBuffCripple							bool		Buffs FALSE
	/call setINIVar switchdeBuffMezz								bool		Buffs FALSE
	/call setINIVar switchdeBuffMalo								bool		Buffs FALSE
	/call setINIVar switchdeBuffTash								bool		Buffs FALSE
	/call setINIVar switchdeBuffSlow								bool		Buffs FALSE
	/call setINIVar switchdeBuffEradicate						bool		Buffs FALSE
	/call setINIVar switchdeBuffSnare								bool		Buffs FALSE
	
	
	| clickie things
	/call setINIVar clickItem1											string	Items FALSE 
	/call setINIVar clickItem2											string	Items FALSE 
	/call setINIVar clickItem3											string	Items FALSE 
	/call setINIVar clickItem4											string	Items FALSE 
	/call setINIVar clickItem5											string	Items FALSE 
	/call setINIVar clickItem6											string	Items FALSE 
		
	/call setINIVar clickItemBurn1									string	Items FALSE 
	/call setINIVar clickItemBurn2									string	Items FALSE 
	/call setINIVar clickItemBurn3									string	Items FALSE 
	/call setINIVar clickItemBurn4									string	Items FALSE 	
	/call setINIVar clickItemBurn5									string	Items FALSE 	
	/call setINIVar clickItemBurn6									string	Items FALSE 	
	
	/call setINIVar Breastplate1		 								string	Items FALSE	
	/call setINIVar Breastplate1effect							string	Items FALSE
	/call setINIVar Breastplate2		 								string	Items FALSE
	/call setINIVar Breastplate2effect							string	Items FALSE	
	/call setINIVar useCircle												string	Items FALSE		
	/call setINIVar CircleofPower										string	Items FALSE	
	/call setINIVar CircleofLife										string	Items FALSE	
	/call setINIVar CircleofMana										string	Items FALSE	
	
	/call setINIVar clickOddItem1										string	Odds FALSE 
	/call setINIVar clickOddItem2										string	Odds FALSE 
	/call setINIVar clickOddItem3										string	Odds FALSE 
	/call setINIVar clickOddItem4										string	Odds FALSE 	

	
	| AAs that you want to always keep going
	/call setINIVar AANow1													string	AANow FALSE 
	/call setINIVar AANow2													string	AANow FALSE 


	| discs that you want to always keep going
	/call setINIVar DiscNow1												string	DiscNow FALSE 
	/call setINIVar DiscNow2												string	DiscNow FALSE 


	| overrides
  /call setDeclare overrideType										string	outer "|stop|engageat|exclude|loot|los|name|safelist|safezone"
	/call setDeclare switchOverride									bool		outer FALSE
	/call setDeclare switchOverrideStop							bool		outer FALSE
  /call setINIVar switchOverrideLos								bool		Override FALSE
  | /call setINIVar switchOverridePve							bool		Override FALSE
  /call setINIVar switchOverrideSafezone					bool		Override FALSE
  /call setINIVar switchOverrideExclude						bool		Override FALSE
  /call setINIVar switchOverrideName							bool		Override FALSE
  /call setINIVar switchOverrideEngageat					bool		Override FALSE
  /call setINIVar switchOverrideLoot							bool		Override FALSE
  /call setINIVar switchOverrideSafelist					bool		Override FALSE
  /call setDeclare switchOverrideClasscheck				bool		outer FALSE


	| General class combat skills
	/call setINIVar kick														bool		meleedisc FALSE
	/call setINIVar bash														bool		meleedisc FALSE
	/call setINIVar taunt														bool		meleedisc FALSE
	/call setINIVar disarm													bool		meleedisc FALSE
	/call setINIVar bandolierNormal									string  meleedisc FALSE
	/call setDeclare bandolierSwap									string 	outer ${bandolierNormal}
	/call setINIVar bandolierBash										string	meleedisc FALSE
	/call setINIVar bandolierBurn										string	meleedisc FALSE

	
	/if (${Select[${Me.Class.ShortName},BER]}) {
		/call setINIVar frenzy												bool		meleedisc FALSE
	}
	/if (${Select[${Me.Class.ShortName},ROG]}) {
		/call setDeclare flagSoS											bool		outer FALSE
		/call setINIVar backstab											bool		meleedisc FALSE
		/call setINIVar intimidation									bool		meleedisc FALSE
	}
	/if (${Select[${Me.Class.ShortName},BRD]}) {
		/call setINIVar intimidation									bool		meleedisc FALSE
	}
	/if (${Select[${Me.Class.ShortName},MNK,BST]}) {
		/call setINIVar dragonpunch										bool		meleedisc FALSE
		/call setINIVar eaglestrike										bool		meleedisc FALSE
		/call setINIVar tigerclaw											bool		meleedisc FALSE
		/call setINIVar flyingkick										bool		meleedisc FALSE
		/call setINIVar roundkick											bool		meleedisc FALSE
	}

	| food/drink
	/call setINIVar clickFoodDispenser							string 	Meal FALSE
	/call setINIVar clickFoodItem										string 	Meal FALSE
	/call setINIVar clickFoodCount									int		 	Meal 0
	/call setINIVar clickDrinkDispenser							string 	Meal FALSE
	/call setINIVar clickDrinkItem									string 	Meal FALSE
	/call setINIVar clickDrinkCount									int		 	Meal 0
	/call setINIVar tClick_FoodDrink								string	Timer 10m
	/call create_timer timer_click_food ${tClick_FoodDrink}
	/call create_timer timer_click_drink ${tClick_FoodDrink}


	/if (${Me.Class.PetClass} || ${Select[${Me.Class.ShortName},ENC,DRU,SHM,SHD]}) {
		/call setINIVar PetTank 											bool 		Pet FALSE
		/call setINIVar useSummonWeps 								bool 		Pet TRUE
		/call setINIVar EpicClick 										string 	Pet FALSE
		/call setINIVar EpicPetBuff 									string 	Pet FALSE
		/call setINIVar summonedWepSpell 							string 	Pet "Grant Thassis' Armaments"
		/call setINIVar summonedWepClosedBag 					string 	Pet "Folded Pack of Thalassic Armaments"
		/call setINIVar summonedFocusSpell 						string 	Pet "Grant Calix's Heirlooms"
		/call setINIVar summonedArmorSpell 						string 	Pet "Grant Thassis' Plate"
		/call setINIVar summonedMaskSpell 						string 	Pet "Grant Visor of Gobeker"
		/call setINIVar summonedWepName								string 	Pet "Summoned: Thalassic Fireblade"
		/call setINIVar shrinkPet 										string 	Pet	"Tiny Companion"
		/call setINIVar PetHealSpell 									string 	Pet FALSE
		/call setINIVar PetHealType 									string 	Pet "gem1"
		/call setINIVar PetRenewalSpell 							string 	Pet FALSE
		/call setINIVar PetRenewalType 								string 	Pet "gem2"
		/call setINIVar PetIllusion		 								string 	Pet FALSE
		/call setINIVar PetIllusionBuff								string 	Pet FALSE
		/call setINIVar PetAAHeal											string 	Pet "Replenish Companion"
		/call setINIVar PetBagSlotNumber							int 		Pet 8
		
		/call setINIVar PetBuffCount 									int 		Pet 0
		
		| 10 pet buffs
		/for _count 1 to 10
			/call setINIVar PetBuff${_count} 						string 	Pet FALSE
			/call setINIVar PetBuff${_count}chk					string 	Pet FALSE
		/next _count
	}	
	
	/if (${Select[${Me.Class.ShortName},MAG,ENC,CLR,NEC,WIZ,SHM,DRU]}) { 
		/call setINIVar TwincastSpell 								string 	Spells FALSE
		/call setINIVar GatherSpell 									string 	Spells FALSE
		/call setINIVar SilentCast	 									string	Spells FALSE
	}
	
	/if (!${Select[${Me.Class.ShortName},MON,BER,WAR]}) { 
		/call setINIVar switchNukeDelay								bool		Spells FALSE
	}	
	

	| buffs
	/call setINIVar BuffRemove											string	Buffs "|Illusion: Gnoll Reaver|Illusion: Darkfell Gnoll Reaver|Illusion: Blackburrow Gnoll Reaver|Illusion: Mucktail Gnoll Reaver|Illusion: Direwind Gnoll Reaver|Illusion: Human Pirate|Illusion: Ogre Pirate"
  /call setDeclare buffType												string	outer "|stop|self|item|combat|bc|raid|tell|pet|merc"
	/call setDeclare switchBuff											bool		outer FALSE
	/call setDeclare switchBuffStop									bool		outer FALSE
	/call setINIVar switchBuffItem	 								bool		Buffs FALSE 
	/call setINIVar switchBuffCombat 								bool		Buffs FALSE 
	/call setINIVar switchBuffPet 									bool 		Pet FALSE 
	/call setINIVar switchBuffSelf	 								bool		Buffs FALSE 
	/call setINIVar switchBuffBc										bool		Buffs FALSE
	/call setINIVar switchBuffRaid									bool		Buffs FALSE
	/call setINIVar switchBuffTell									bool		Buffs FALSE
	/call setINIVar switchBuffMerc									bool		Buffs FALSE

	
	/if (${Me.Class.CanCast} || ${Select[${Me.Class.ShortName},ROG]}) {
		/call setINIVar buffGem 											string 	Spells "gem8"
		/call setINIVar GoMLevel											int			Spells 105
		/call setINIVar GoMNuke												string	Spells FALSE
		/call setINIVar GoMNukeCondition							string	Spells FALSE
		/call setINIVar GoMNuke2											string	Spells FALSE
		/call setINIVar GoMNuke2Condition							string	Spells FALSE
	}
	
	| bc/tell buffs
	/call setDeclare BuffCount											int 		outer 15
	/for _count 1 to ${BuffCount}
		/call setINIVar Buff${_count}									string 	Buffs FALSE
		/call setINIVar Buff${_count}chk							string 	Buffs FALSE
		/call setINIVar Buff${_count}self							string 	Buffs FALSE
		/call setINIVar Buff${_count}Alias						string 	Buffs "|FALSE|FALSE" 
		/call setINIVar BCBuff${_count}Class					string 	Buffs "|WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM"
		/call setINIVar RaidBuff${_count}Class				string 	Buffs "|WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM"
		/call setINIVar Buff${_count}noBCinRaid				bool	 	Buffs FALSE
	/next _count


	/call setINIVar Aura1Spell 											string 	Aura FALSE
	/call setINIVar Aura1Buff 											string 	Aura FALSE  
	/call setINIVar Aura2Spell 											string 	Aura FALSE
	/call setINIVar Aura2Buff 											string 	Aura FALSE
	
	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,PAL]}) {
		/call setDeclare callID												int 		outer 0
		/call setINIVar PetPoint 											int			Heal 80	
		/call setINIVar CLRPoint		 									int 		Heal 80
		/call setINIVar DRUPoint 											int 		Heal 70
		/call setINIVar SHMPoint 											int 		Heal 60
		/call setINIVar WIZPoint 											int 		Heal 75
		/call setINIVar MAGPoint 											int 		Heal 75
		/call setINIVar ENCPoint 											int 		Heal 80
		/call setINIVar NECPoint 											int 		Heal 75
		/call setINIVar WARPoint 											int 		Heal 70
		/call setINIVar MNKPoint 											int 		Heal 60
		/call setINIVar ROGPoint 											int 		Heal 60
		/call setINIVar RNGPoint 											int 		Heal 90
		/call setINIVar BSTPoint 											int 		Heal 70
		/call setINIVar SHDPoint 											int 		Heal 90
		/call setINIVar BRDPoint 											int 		Heal 65
		/call setINIVar BERPoint 											int 		Heal 70
		/call setINIVar PALPoint 											int 		Heal 90
		/call setINIVar GoMpoint 											int 		Heal 80	
		/call setINIVar XHealClass										string	Heal "|WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM"
		/call setTimer tXHeal													string	Timer 3m timer_set_xtarget
		/call setINIVar toonControlXTslot							int			Heal 13
		/call setINIVar toonControlProtect						bool		Heal TRUE
	}

	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,BST,PAL,RNG]}) {
 		/call setINIVar cureCurse 										string 	Cure FALSE
		/call setINIVar curePoison 										string 	Cure FALSE
		/call setINIVar cureDisease 									string 	Cure FALSE
		/call setINIVar cureCorrupt 									string 	Cure FALSE
	}
	
	| pet control
  /call setDeclare 		pcType											string	outer "|pet|swarm|type|build|bag"
	/call setINIVar PcPetengageat										int			Pet 99
	| /call setINIVar switchPcTank										bool 		Pet FALSE
	/call setINIVar PcPettype												string	Pet FALSE
	/call setINIVar switchPcPet											bool 		Pet FALSE
	/call setINIVar switchPcSwarm										bool 		Pet FALSE	
	/call setINIVar PcSwarmengageat									int			Pet 99
	/call setINIVar sumFamiliar											string	Pet FALSE
	/call setINIVar sumFamiliarBuff									string	Pet FALSE
	/call setINIVar sumFamiliarLeave								bool		Pet TRUE	

	| debug
  /call setDeclare 		debugType										string	outer "|stop|core|class|casting|rez|array|mode"
  /call setDeclare		switchDebugStop							bool 		outer FALSE
	/call setINIVar switchDebugCore									bool		debug FALSE 
	/call setINIVar switchDebugClass								bool		debug FALSE 
	/call setINIVar switchDebugCasting							bool		debug FALSE 
	/call setINIVar switchDebugRez									bool		debug FALSE 
	/call setINIVar switchDebugArray								bool		debug FALSE 
	/call setINIVar switchDebugMode									bool		debug FALSE 
	| SET THIS TO TRUE TO ENABLE DEBUGING. ALTERNATIVE USE /varset debug TRUE
	| on a funny note: prepare your ass
	/call setDeclare debug													bool		outer FALSE


	| onoff types
  /call setDeclare 		onoffType										string	outer "|banestrike|horn|shrink"

	/call setINIVar switchBanestrike								bool		Common FALSE		
	/call setINIVar switchInvites										bool 		Common TRUE
	/call setINIVar switchRaidInvite								bool		Common FALSE
	/call setINIVar switchShrink										bool		Common FALSE
	/call setINIVar switchHorn											bool		Common FALSE

	| tradeskill control
  /call setDeclare 		tsType											string	outer "|movebank"
	
	| CLASS controls
  /call setDeclare brdType												string	outer "|healpoint|intimidkick|line|restsong|spiregroup|spireraid"
  /call setDeclare berType												string	outer "|ae|healpoint|spiregroup|spireraid"
  /call setDeclare bstType												string	outer "|fastheal|healpoint|spiregroup|spireraid"
  /call setDeclare clrType												string	outer "|aggressive|ch|fade|forcegroupheal|groupheal|grouphot|healpoint|lock|rampage|regen|spiregroup|spireraid|vp|xtradius|xttime|yaulp"
  /call setDeclare druType												string	outer "|groupheal|healpoint|rampage|spiregroup|spireraid|xtradius|xttime"
  /call setDeclare encType												string	outer "|charm|gather|healpoint|spiregroup|spireraid"
  /call setDeclare magType												string	outer "|fade|forcetwincast|gather|healpoint|mana|rods|spiregroup|spireraid"
  /call setDeclare mnkType												string	outer "|healpoint|spiregroup|spireraid"
  /call setDeclare necType												string	outer "|healpoint|spiregroup|spireraid"
  /call setDeclare palType												string	outer "|defcount|groupheal|harmonions|healpoint|mg|rampage|spiregroup|spireraid|stance|steel|stun|xtradius|xttime|yaulp"
  /call setDeclare rogType												string	outer "|ae|autosos|healpoint|poison|spiregroup|spireraid"
  /call setDeclare rngType												string	outer "|fastheal|healpoint|spice|spiregroup|spireraid|summer"
  /call setDeclare shdType												string	outer "|defcount|harmoniouns|healpoint|skin|spiregroup|spireraid|stance"
  /call setDeclare shmType												string	outer "|groupheal|grouphot|healpoint|rampage|spiregroup|spireraid|talisman|xtradius|xttime"
  /call setDeclare warType												string	outer "|healpoint|spiregroup|spireraid"
  /call setDeclare wizType												string	outer "|forcetwincast|gather|healpoint|line|manaburn|spiregroup|spireraid"
    
	| cursor
  /call setDeclare cursorType											string	outer "|announce|auto|destroy|ignore|keep|leave|pass|pause|sell|what"
	/call setINIVar switchAutoCursor								bool		Common TRUE	
  
  | list control
	/call setDeclare listType												string	outer "|safe|reset|remove|exclude|name|nomezz|nocast|range|nomelee|nomagic|nodisease|nofire|nopoison|nocold|nocorruption|huntmob"

	| heals
  /call setDeclare healType												string	outer "|stop|self|group|xtarget|pet|cure|override"
  /call setDeclare switchHealStop									bool		outer	FALSE
  /call setDeclare switchHealOverride							bool		outer	0
	/call setDeclare switchHeal											bool		outer FALSE
	/call setINIVar HealOverride										int			Heal 0
	/call setDeclare tmpHealOverride								float 	outer 99.99	
	/call setINIVar switchHealSelf									bool 		Heal FALSE
	/call setINIVar switchHealGroup									bool 		Heal FALSE
	/call setINIVar switchHealXtarget								bool 		Heal FALSE
	/call setINIVar switchHealPet										bool 		Heal FALSE
	/call setINIVar switchHealCure									bool 		Heal FALSE

	/call setINIVar cureCurseItem										string 	Cure FALSE
	/call setINIVar curePoisonItem 									string 	Cure FALSE
	/call setINIVar cureDiseaseItem									string 	Cure FALSE
	/call setINIVar cureCorruptItem									string 	Cure FALSE
	/call setINIVar noCureList	 										string 	Cure "|FALSE|FALSE"
	
	/call setDeclare clickHealItem									string outer "|Prayers of Life|Overflowing Urn of Life"
	
	

	| Mode stuffs
	| Train Spell spells
	/if (${Bool[${Me.Class.CanCast}]}) {
		/call setINIVar abjuration										string 	Mode FALSE
		/call setINIVar alteration										string 	Mode FALSE
		/call setINIVar conjuration										string 	Mode FALSE
		/call setINIVar divination										string 	Mode FALSE
		/call setINIVar evocation											string 	Mode FALSE
	}

	
	
/return
	


sub declare_timers()
	/declare _count int local 0

	/call setINIVar tRez_Retry											string	Timer 3m
	| /call setINIVar	tBuff_Kill											string	Timer	1m
	/call setINIVar	tCheck_snap_retry								string	Timer	15s
	| /call setINIVar	tAA_picker											string	Timer	5m
	/call setINIVar	tCheck_Runes										string	Timer	10s
	/call setINIVar	tScorchedEarth_ignore						string	Timer	30s
	
	/call create_timer timer_auto_cursor_pause 10s
	/call create_timer timer_spellcast_nomob_ID 1
	/call create_timer timer_scribe_timer 1
	/call create_timer timer_try_door 1

	/call setTimer tCheck_Runes											string	Timer	10s		timer_check_runes

	/call setTimer tScorchedEarth_fail							string	Timer	1m 		timer_scorched_earth_fail
	/call setTimer tCheck_Familiar									string	Timer	30s		timer_Check_Familiar
	/call setTimer tHarvest_delay										string	Timer	1s 		timer_Harvest_delay
	/call setTimer tScorchedEarth_delay							string	Timer	5s 		timer_ScorchedEarth_delay
	/call setTimer tCheck_EQBC											string	Timer	10m		timer_check_EQBC
	/call setTimer tOdds_and_Ends										string	Timer	30m 	timer_check_odds_and_ends
	/call setTimer tCheck_EXP												string	Timer	5m 		timer_check_EXP
	/call setTimer tCheck_Tie												string	Timer	5 		timer_check_Tie
	/call setTimer tWatch_Spawn											string	Timer	1s 		timer_check_watch_spawn
	/call setTimer tCast_Cure												string	Timer	30s 	timer_check_cast_cure
	/call setTimer tCheck_status										string	Timer	2s 		timer_check_status
	/call setTimer tPetfarm_delay										string	Timer	1s 		timer_Petfarm_delay	
	/call setTimer tClear_Cursor										string	Timer 5s 		timer_clear_cursor   
	/call setTimer tSelf_Buff												string	Timer 3m 		timer_check_buffs_self
	/call setTimer tPet_Buff												string	Timer 1m 		timer_check_pet_buffs
	/call setTimer tItem_Buff												string	Timer 1m 		timer_check_buffs_item
	/call setTimer tBC_Buff													string	Timer 20s 	timer_check_buffs_BC
	/call setTimer tRaid_Buff												string	Timer 20s 	timer_check_buffs_raid
	/call setTimer tMerc_Buff												string	Timer 20s 	timer_check_buffs_merc
	/call setTimer tCheck_Aura											string	Timer 3m 		timer_check_aura
	/call setTimer tTell_Buff												string	Timer	10s 	timer_check_buffs_tell
	/call setTimer tCheck_Ammo											string	Timer	5s 		timer_Check_Ammo
	/call setTimer tHunt_delay											string	Timer	30s 	timer_wait_for_mob_to_kill
	/call setTimer tAuto_Control_Toon								string	Timer	10s 	timer_auto_toon_control
	/call setTimer tZone_Pause											string	Timer	5s		timer_zone_pause
	/call setTimer tHUD_Death_Check									string	Timer	10s		timer_HUD_death_check
	/call setTimer tNuke_Delay											string	Timer	1			timer_Nuke_Delay
	/call setTimer tWaste_Fireworks									string	Timer 15m		timer_Waste_Fireworks
	/call setTimer tValidate_Zone										string	Timer 10s		timer_validate_zone


	| mod rod/pet weapon recovered addicts
	/if (!${Select[${Me.Class.ShortName},MAG,WAR,ROG,BRD,MNK,BER]}) {
		/for _count 1 to ${ResidentMage.Count[|]}
			/if (!${Bool[${ResidentMage.Arg[${_count},|]}]}) /continue
			/call setTimer	tAsk_for_rod								string	Timer	1m 		timer_ask_for_rod_${ResidentMage.Arg[${_count},|]}
			/call setTimer	tAsk_for_weapons						string	Timer	1m		timer_ask_for_weapons_${ResidentMage.Arg[${_count},|]}
		/next _count		
	}
/return	
	
  
   
|***
 *	DES: Magician base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|   
sub declare_MAG()
	/call setINIVar ServantNuke 										string 	Pet "Remorseless Servant"
	/call setTimer tCauldron_use										string 	Timer 360m timer_caldron_use
	/call setTimer tForce_Twincast									string	Timer 5s timer_force_twincast
	/varset timer_force_twincast 1

	/call setINIVar ParadoxSpell 										string 	Rods "Grant Icebound Paradox"
	/call setINIVar ParadoxRodName 									string 	Rods "Summoned: Icebound Fragment"
	/call setINIVar OrbSpell 												string 	Rods "Summon Blazing Orb"
	/call setINIVar OrbName 												string 	Rods "Blazing Orb"
	/call setINIVar SelfRodSpell 										string 	Rods "Wand of Dark Modulation"
	/call setINIVar SelfRodName 										string 	Rods "Wand of Pelagic Modulation"
	/call setINIVar ModRodSpell 										string 	Rods "Large Modulation Shard"
	/call setINIVar ModRodName 											string 	Rods "Summoned: Large Modulation Shard"
	/call setINIVar ElementRodSpell 								string 	Rods "Summon Mutinous Minion"
	/call setINIVar ElementRodName 									string 	Rods "Summoned: Exigent Minion XXI"

	/call setINIVar spellCripple 										string 	Spells FALSE
	/call setINIVar GatherSpell											string 	Spells "Gather Magnitude"
	/call setINIVar switchGather			 							bool		Spells TRUE
	/call setINIVar GatherPercent			 							int			Spells 60
	/call setINIVar DoNuke					 								int			Spells 2
	/call setINIVar Nuke1 													string 	Spells "Spear of Blistersteel"
	/call setINIVar Nuke1Condition 									string	Spells FALSE
	/call setINIVar Nuke2 													string 	Spells "Spear of Molten Shieldstone"
	/call setINIVar Nuke2Condition 									string	Spells FALSE
	/call setINIVar Nuke3 													string 	Spells FALSE
	/call setINIVar Nuke3Condition 									string	Spells FALSE
	/call setINIVar Nuke4 													string 	Spells FALSE
	/call setINIVar Nuke4Condition 									string	Spells FALSE
	/call setINIVar NukeSalvo												string 	Spells "Storm of Many"
	/call setINIVar NukeSalvoCondition 							string	Spells FALSE
	/call setINIVar DoDoT								 						int			Spells 0
	/call setINIVar Cauldron				 								string	Spells "Cauldron of Countless Goods"
	/call setINIVar useRain													bool 		Spells FALSE
	/call setINIVar doRainNuke											int 		Spells 2
	/call setINIVar RainNuke1 											string 	Spells "Rain of Cutlasses"
	/call setINIVar RainNuke2 											string 	Spells "Coronal Rain"
	/call setINIVar spellMalo 											string 	Spells "Malosinete"
	/call setINIVar AAFade													string	Buffs "Dimensional Shield"
	/call setINIVar useAAFade												int			Buffs 50
	/call setINIVar NukeTwincast										string	Spells "Chaotic Fire"
	/call setINIVar switchForceTwincast							bool		Spells TRUE

	/call setINIVar ${Me.Class.ShortName}Point 			int 		Heal 80
/return
	


|***
 *	DES: Wizard vase variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_WIZ()

	/declare _count int local 0
	/declare _list int local 0
	/declare _line string local NULL
	
	/call setTimer tForce_Twincast									string	Timer 3s timer_force_twincast
	/call setINIVar GatherSpell											string 	Spells "Musing Harvest"
	/call setINIVar switchGather					 					bool		Spells TRUE
	/call setINIVar GatherPercent			 							int			Spells 60
	
	/call setINIVar tempPet						 							string	Pet "Yulin's Pyroblade"
	/call setINIVar switchManaBurn		 							bool		Spells TRUE
	/call setINIVar spellCripple 										string 	Spells FALSE
	/call setINIVar spellConcussion									string 	Spells FALSE
	/call setINIVar spellConcussionPct							int 	Spells 0

	/call setDeclare lineType												string	outer "|off|fire|ice|magic|mix"
	/call setINIVar lineCurrent											string 	Spells Fire
	/call setINIVar lineFire												string	Spells "|Ethereal Braid|Ethereal Skyfire|Dragoflux's Fire|Chaos Burn"
	/call setINIVar lineIce													string	Spells "|Icefloe Cascade|Ethereal Icefloe|Rimeblast Cascade|Lure of Travenro"
	/call setINIVar lineMagic												string	Spells FALSE
	/call setINIVar lineMix													string	Spells FALSE

	| add conditions
	/for _list 1 to ${lineType.Count[|]}
		/varset _line ${lineType.Arg[${_list},|].Left[1].Upper}${lineType.Arg[${_list},|].Right[-1].Lower}
		/for _count 1 to 8	
			/call setINIVar line${_line}Condition${_count}		string	Spells FALSE
		/next _count
	/next _list	
	
	/call setINIVar lineAAFire											string	Spells "Pyromancy"
	/call setINIVar lineAAIce												string	Spells "Cryomancy"
	/call setINIVar lineAAMagic											string	Spells "Arcomancy"
	/call setINIVar lineAAMix												string	Spells FALSE
	
	/call setINIVar switchForceTwincast							bool		Spells TRUE
	/call setINIVar NukeTwincastFire								string	Spells "Claw of Qunard"
	/call setINIVar NukeTwincastIce									string	Spells "Claw of Travenro"
	/call setINIVar NukeTwincastMagic								string	Spells "Claw of Ellar"
	/call setINIVar NukeTwincastMix									string	Spells "Claw of Qunard"
	/call setINIVar NukeTwincastCondition 					string	Spells FALSE
	
	/call setDeclare fay 														string	outer "Translocate: Fay"
	/call setDeclare dragonscale 										string	outer "Translocate: Dragonscale Hills"
	/call setDeclare northkarana 										string	outer "Translocate: North"
	/call setDeclare commonlands 										string	outer "Translocate: Common"
	/call setDeclare nektulos 											string	outer "Translocate: Nek"
	/call setDeclare qeyenos 												string	outer "Translocate: West"
	/call setDeclare northro 												string	outer "Translocate: Ro"
	/call setDeclare cazic 													string	outer "Translocate: Cazic"
	/call setDeclare undershore 										string	outer "Translocate: Undershore"
	/call setDeclare westkarana 										string	outer "Translocate: West Karana"
	/call setDeclare bloodfields 										string	outer "Translocate: Bloodfields"
	/call setDeclare wallofslaughter 								string	outer "Translocate: Slaughter"
	/call setDeclare shardslanding 									string	outer "Translocate: Shard's Landing"
	/call setDeclare pillars 												string	outer "Translocate Pillars of Alra"
	/call setDeclare sarith 												string	outer "Translocate Sarith"
	/call setDeclare dreadlands 										string	outer "Translocate: Combine"
	/call setDeclare lceanium												string	outer "Translocate: Lceanium"
	/call setDeclare dawnshroud 										string	outer "Translocate: Dawnshroud"
	/call setDeclare twilight 											string	outer "Translocate: Twilight"
	/call setDeclare grimling 											string	outer "Translocate: Grimling"
	/call setDeclare nexus 													string	outer "Translocate: Nexus"
	/call setDeclare bind 													string	outer "Translocate"
	/call setDeclare primary 												string	outer "Translocate: Primary Anchor"
	/call setDeclare secondary 											string	outer "Translocate: Secondary Anchor"
	/call setDeclare pushprimary 										string	outer "Primary Anchor Push"
	/call setDeclare pushsecondary 									string	outer "Secondary Anchor Push"
	/call setDeclare tempesttemple 									string	outer "Translocate: Tempest Temple"
	/call setDeclare katta													string	outer "Translocate: Katta Castrum"
	/call setDeclare tox 														string	outer "Translocate: Tox"
	/call setDeclare stonebrunt 										string	outer "Translocate: Stonebrunt"
	/call setDeclare icefall 												string	outer "Translocate: Icefall Glacier"
	/call setDeclare sunderock 											string	outer "Translocate: Sunderock Springs"
	/call setDeclare blightfire 										string	outer "Translocate: Blightfire Moors"
	/call setDeclare Barnidu 												string	outer "Translocate: Barindu"
	/call setDeclare Natimbi 												string	outer "Translocate: Natimbi"
	/call setDeclare grounds 												string	outer "Translocate: the Grounds"
	/call setDeclare potime 												string	outer "Translocate: Plane of Time"
	/call setDeclare brells 												string	outer "Translocate: Brell's Rest"
	/call setDeclare Arcstone 											string	outer "Translocate: Arcstone"
	/call setDeclare pok 														string	outer "Translocate: Knowledge"
	/call setDeclare cobaltscar 										string	outer "Translocate: Cobalt Scar"
	/call setDeclare wakening 											string	outer "Translocate: Wakening Lands"
	/call setDeclare greatdivide 										string	outer "Translocate: Great Divide"
	/call setDeclare iceclad 												string	outer "Translocate: Iceclad"		
		
	/call setINIVar runeMagiWard	 									string	Rune "Doomscale Husk"
	/call setINIVar runeShieldofFate								string	Rune "Shield of Consequence"
	/call setINIVar runeCrystalwing									string	Rune "Armor of the Codex"
	/call setINIVar runeGuard			 									string	Rune "Darkmist Guard"

	/call setINIVar ${Me.Class.ShortName}Point 	int 		Heal 80
/return	



|***
 *	DES: Enchanter base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_ENC()
	/call setDeclare mobList												string	outer
	/call setDeclare addList 												string 	outer
	| /if (!${Defined[aggroAnim]}) /declare aggroAnim string outer "|5|8|12|17|18|42|44|80|106|129|144"
	
	/call setINIVar switchCharmPet									bool		Pet FALSE

	/call setINIVar switchGather			 							bool		Spells TRUE
	/call setINIVar GatherPercent			 							int			Spells 60

	/call setINIVar runeVeil			 									string	Rune "Veil of Mindshadow"
	/call setINIVar runeEldritch	 									string	Rune "Eldritch Rune"
	/call setINIVar runeBrimstone 									string	Rune "Brimstone Resolution"
	/call setINIVar runeSpray			 									string	Rune "Glyph Spray"
	/call setINIVar runeReactive	 									string	Rune "Reactive Rune"
	/call setINIVar runePoly			 									string	Rune "Polyrefractive Rune"
	/call setINIVar runeUnity			 									string	Rune FALSE
	/call setINIVar runeWard			 									string	Rune "Ward of the Enticer"
	/call setINIVar runeAuspice		 									string	Rune "Darkened Auspice"
	/call setINIVar runeAuspiceToon									string  Rune |FALSE|FALSE
	/call setINIVar runeMind												string  Rune "Mind Over Matter"
	/call setINIVar runeEntAuspice									string	Rune "Enticer's Auspice"
	/call setINIVar runeLegion											string	Rune "Legion of Lucem"
	/call setINIVar runeShieldofFate								string	Rune "Shield of Consequence"

	/call setINIVar mannaCrystal										string 	Spells "Azure Mind Crystal"
	/call setINIVar healCrystal											string 	Spells "Sanguine Mind Crystal"
	
	/call setINIVar spellCripple 										string 	Spells "Undermining Helix"
	/call setINIVar spellSlow		 										string	Spells "Undermining Helix"		
	/call setINIVar AASlow			 										string	Spells "Dreary Deeds"		
	/call setINIVar spellTash 											string 	Spells "Decree of Tashan"

	/call setINIVar spellMez												string 	Spells "Chaotic Bewildering"
	/call setINIVar spellpbaeMez										string	Spells "Perilous Bafflement"
	/call setINIVar spellStun												string	Spells "Color Concourse"
	/call setINIVar spellCharm											string	Spells "Enticer's Command"
	
	/call setINIVar DoNuke					 								int			Spells 3
	/call setINIVar Nuke1					 									string	Spells "Chromatic Blink"
	/call setINIVar Nuke1Condition 									string	Spells FALSE
	/call setINIVar Nuke2					 									string	Spells "Intellectual Appropriation"
	/call setINIVar Nuke2Condition 									string	Spells FALSE
	/call setINIVar Nuke3					 									string	Spells "Mindsunder"
	/call setINIVar Nuke3Condition 									string	Spells FALSE
	/call setINIVar Nuke4					 									string	Spells FALSE
	/call setINIVar Nuke4Condition 									string	Spells FALSE
	
	/call setINIVar DoDoT								 						int			Spells 0
	/call setINIVar DoT1							 							string	Spells FALSE
	/call setINIVar DoT1chk						 							string	Spells FALSE
	/call setINIVar DoT1Condition										string	Spells FALSE
	/call setINIVar DoT2							 							string	Spells FALSE
	/call setINIVar DoT2chk						 							string	Spells FALSE
	/call setINIVar DoT2Condition										string	Spells FALSE
	/call setINIVar DoT3							 							string	Spells FALSE
	/call setINIVar DoT3chk						 							string	Spells FALSE
	/call setINIVar DoT3Condition										string	Spells FALSE
	/call setINIVar DoT4							 							string	Spells FALSE
	/call setINIVar DoT4chk						 							string	Spells FALSE
	/call setINIVar DoT4Condition 									string	Spells FALSE

	/call setINIVar ${Me.Class.ShortName}Point 	int 		Heal 80
/return	



|***
 *	DES: Necromancer base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_NEC()
	/call setTimer tCheck_Blood											string	Timer 8s	timer_check_blood

	/call setINIVar spellMalo												string 	Spells "Scent of Terris"
	/call setINIVar spellBlood											string 	Spells "Impose for Blood"
	/call setINIVar spellSlow		 										string	Spells "Scent of Terris"
	/call setINIVar spellCripple 										string 	Spells FALSE
	
	/call setINIVar DoDoT								 						int			Spells 0
	/call setINIVar DoT1							 							string	Spells FALSE
	/call setINIVar DoT1chk						 							string	Spells FALSE
	/call setINIVar DoT1Condition 									string	Spells FALSE
	/call setINIVar DoT2							 							string	Spells FALSE
	/call setINIVar DoT2chk								 					string	Spells FALSE
	/call setINIVar DoT2Condition 									string	Spells FALSE
	/call setINIVar DoT3									 					string	Spells FALSE
	/call setINIVar DoT3chk								 					string	Spells FALSE
	/call setINIVar DoT3Condition 									string	Spells FALSE
	/call setINIVar DoT4							 							string	Spells FALSE
	/call setINIVar DoT4chk						 							string	Spells FALSE
	/call setINIVar DoT4Condition 									string	Spells FALSE
	/call setINIVar DoT5					 									string	Spells FALSE
	/call setINIVar DoT5chk						 							string	Spells FALSE
	/call setINIVar DoT5Condition 									string	Spells FALSE
	/call setINIVar DoT6					 									string	Spells FALSE
	/call setINIVar DoT6chk						 							string	Spells FALSE
	/call setINIVar DoT6Condition 									string	Spells FALSE
	/call setINIVar DoT7					 									string	Spells FALSE
	/call setINIVar DoT7chk								 					string	Spells FALSE
	/call setINIVar DoT7Condition 									string	Spells FALSE
	/call setINIVar DoT8					 									string	Spells FALSE
	/call setINIVar DoT8chk								 					string	Spells FALSE
	/call setINIVar DoT8Condition 									string	Spells FALSE
	/call setINIVar DoT9					 									string	Spells FALSE
	/call setINIVar DoT9chk								 					string	Spells FALSE
	/call setINIVar DoT9Condition 									string	Spells FALSE
	/call setINIVar DoT10					 									string	Spells FALSE
	/call setINIVar DoT10chk							 					string	Spells FALSE
	/call setINIVar DoT10Condition 									string	Spells FALSE

	/call setINIVar DoNuke							 						int			Spells 1
	/call setINIVar Nuke1					 									string	Spells "Combust Bones"	
	/call setINIVar Nuke1Condition 									string	Spells FALSE
	/call setINIVar Nuke2					 									string	Spells FALSE
	/call setINIVar Nuke2Condition 									string	Spells FALSE
	/call setINIVar Nuke3					 									string	Spells FALSE
	/call setINIVar Nuke3Condition 									string	Spells FALSE
	/call setINIVar Nuke4					 									string	Spells FALSE
	/call setINIVar Nuke4Condition 									string	Spells FALSE
	/call setINIVar Nuke5					 									string	Spells FALSE
	/call setINIVar Nuke5Condition 									string	Spells FALSE
	/call setINIVar Nuke6					 									string	Spells FALSE
	/call setINIVar Nuke6Condition 									string	Spells FALSE
/return	



|***
 *	DES: Shadowknight base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_SHD()
	/call setTimer tcheck_Skin											string 	Timer 45s timer_check_Skin
	/call setTimer tcheck_Stance										string 	Timer 3m timer_check_Stance



	/call setDeclare mobAECount											int			outer 0
	/call setINIVar Withstand 											string 	Discs "Repel"
	/call setINIVar BladeStrike 										string 	Discs "Lacerating Blade"
	/call setINIVar CrimsonBlade 										string 	Discs "Carmine Blade"
	/call setINIVar DiscRest			 									string	Discs "Breather"
	/call setINIVar DiscFakeHate										string	Discs	"Unflinching Acrimony"
	/call setINIVar DiscLeech												string	Discs	"Leechcurse Discipline"
	/call setINIVar DiscRancor											string	Discs	"Reflexive Revulsion"
	
	/call setINIVar DefensiveDisc1									string 	Discs "Krellnakor Mantle "
	/call setINIVar DefensiveDisc2 									string 	Discs "Tylix's Carapace"
	/call setINIVar DefensiveDisc3 									string 	Discs "Cursed Guardian Discipline"
	/call setINIVar defCount												int			Discs 2

	/call setINIVar ChallengeSpell									string 	Spells "Impose for Power"
	/call setINIVar Hate1 													string 	Spells "Terror of Narus"
	/call setINIVar Hate2 													string 	Spells "Terror of Poria"
	/call setINIVar spellCripple										string 	Spells FALSE
	/call setINIVar spellUndeadCripple							string 	Spells "Helix of the Undying"
	 
	/call setINIVar AEHate1 												string 	Spells "Disgust"
	/call setINIVar AEHate2 												string 	Spells "Repugnance"
	/call setINIVar AELifeTap 											string 	Spells "Deceitful Deflection"
	
	/call setINIVar DoNuke					 								int			Spells 2
	/call setINIVar Nuke1 													string 	Spells "Touch of Holmein"
	/call setINIVar Nuke1Condition 									string	Spells FALSE
	/call setINIVar Nuke2 													string 	Spells "Touch of Lutzen"
	/call setINIVar Nuke2Condition 									string	Spells FALSE
	/call setINIVar Nuke3 													string 	Spells FALSE
	/call setINIVar Nuke3Condition 									string	Spells FALSE
	/call setINIVar DoDoT										 				int			Spells 0
	/call setINIVar DoT1									 					string	Spells FALSE
	/call setINIVar DoT1chk								 					string	Spells FALSE
	/call setINIVar DoT1Condition 									string	Spells FALSE
	/call setINIVar DoT2									 					string	Spells FALSE
	/call setINIVar DoT2chk								 					string	Spells FALSE
	/call setINIVar DoT2Condition 									string	Spells FALSE
	/call setINIVar DoT3									 					string	Spells FALSE
	/call setINIVar DoT3chk								 					string	Spells FALSE
	/call setINIVar DoT3Condition 									string	Spells FALSE
	/call setINIVar DoT4									 					string	Spells FALSE
	/call setINIVar DoT4chk								 					string	Spells FALSE
	/call setINIVar DoT4Condition 									string	Spells FALSE
	 
	/call setINIVar spellCripple 										string 	Spells FALSE
	/call setINIVar switchHarmonious 								bool	 	Buffs TRUE
	/call setINIVar Harmonious 											string 	Buffs "Harmonious Disruption"
	/call setINIVar switchStance 										bool	 	Buffs TRUE
	/call setINIVar Stance 													string 	Buffs "Staunch Stance"
	/call setINIVar switchSkin			 								bool	 	Buffs TRUE
	/call setINIVar Skin			 											string 	Buffs "Vizat's Skin"

	/call setINIVar ${Me.Class.ShortName}Point 			int 		Heal 80
/return	

 

|***
 *	DES: Paladin base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_PAL()
	/call setTimer	tcheck_MG												string	Timer	10s timer_check_MG
	/call setTimer tcheck_Steel											string 	Timer 2m timer_check_Steel
	/call setTimer tcheck_Stance										string 	Timer 7.5m timer_check_Stance

	/call setDeclare mobAECount											int			outer 0 
	/call setDeclare groupHurt 											int 		outer 0
	/call setDeclare WorstHurt 											string 	outer FALSE
	| /call setDeclare WorstHurtID 										int 		outer
	/call setDeclare WorstHP 												int 		outer 100
	/call setDeclare WorstClass 										string 	outer FALSE
	| /call setDeclare MinorHurt											int    	outer 0 
	
  /call setINIVar healWholeGroup									string 	Heal "Wave of Grief"
  /call setINIVar splashGroup											string 	Heal "Wave of Grief"
  /call setINIVar healPanic												string 	Heal FALSE
  /call setINIVar splashPanic											string 	Heal "Aurora of Dayspring"
  /call setINIVar healPanic1											string 	Heal FALSE
  /call setINIVar healPanic2											string 	Heal FALSE
  /call setINIVar healTouch												string 	Heal "Ardent Touch"
	/call setINIVar useGroupHeal 										bool		Heal TRUE
	/call setINIVar XTHealRadius										int			Heal 300
	/call setINIVar RampageTank											string	Heal FALSE
	
	/call setINIVar doYaulp 												string	Buffs TRUE
	/call setINIVar spellYaulp 											string	Buffs FALSE
	/call setINIVar useYaulpPct											string	Buffs 98
	/call setINIVar DiscRest			 									string	Discs "Breather"
	/call setINIVar DiscFastRune	 									string	Discs "Repel"
		                                   	
	/call setINIVar DefensiveDisc1			 						string	Discs "Armor of Mercy"
	/call setINIVar DefensiveDisc2			 						string	Discs "Kar`Zok Mantle"
	/call setINIVar DefensiveDisc3			 						string	Discs "Blessed Guardian Discipline"
	/call setINIVar defCount												int			Discs 2
                                           	
	/call setINIVar DiscUnflinching									string	Discs "Unflinching Affirmation"
	/call setINIVar DiscJudgement										string	Discs "Inquisitors Judgement"
	/call setINIVar DiscHate												string	Discs "Projection of Piety"
	/call setINIVar DiscTwincast										string	Discs "Hand of Tunare"
	/call setINIVar DiscReflexive										string	Discs "Reflexive Reverence"
                                           	
	/call setINIVar AALayonHands										string	Discs "Lay on Hands"
	/call setINIVar AAHandofPiety										string	Discs "Hand of Piety"
	/call setINIVar AAForceofDisruption							string	Discs "Force of Disruption"
	/call setINIVar AADisruptivePersecution					string	Discs "Disruptive Persecution"
	/call setINIVar AADivineStun										string	Discs "Divine Stun"
                                           	
	/call setINIVar AAAEBeacon											string	Discs "Beacon of the Righteous"
	/call setINIVar AAAELodestar										string	Discs "Hallowed Lodestar"
                                           	
	/call setINIVar AAInquisitor										string	Discs "Armor of the Inquisitor"
	/call setINIVar AAGroupInquisitor								string	Discs "Group Armor of the Inquisitor"
                                           	
	/call setINIVar AAJudgement											string	Discs "Inquisitor's Judgement"
	/call setINIVar AAValorousRage									string	Discs "Valorous Rage"
	/call setINIVar AAShield												string	Discs "Shield of Brilliance"
	/call setINIVar chainStun												string	Discs FALSE
	                                         	
	/call setINIVar DoNuke					 								int			Spells 5
	/call setINIVar Nuke1														string 	Spells "Force of Ardency"
	/call setINIVar Nuke1Condition 									string	Spells FALSE
	/call setINIVar Nuke2														string 	Spells "Ardent Force"
	/call setINIVar Nuke2Condition 									string	Spells FALSE
	/call setINIVar Nuke3														string 	Spells "Force of the Darkened Sea"
	/call setINIVar Nuke3Condition 									string	Spells FALSE
	/call setINIVar Nuke4														string 	Spells "Lesson of Grief"
	/call setINIVar Nuke4Condition 									string	Spells FALSE
	/call setINIVar Nuke5							 							string	Spells "Protective Proclamation"
	/call setINIVar Nuke5Condition 									string	Spells FALSE
                                           	
	/call setINIVar DoDoT								 						int			Spells 0
	/call setINIVar DoT1							 							string	Spells FALSE
	/call setINIVar DoT1chk						 							string	Spells FALSE
	/call setINIVar DoT1Condition 									string	Spells FALSE
	/call setINIVar DoT2							 							string	Spells FALSE
	/call setINIVar DoT2chk						 							string	Spells FALSE
	/call setINIVar DoT2Condition 									string	Spells FALSE
	/call setINIVar DoT3							 							string	Spells FALSE
	/call setINIVar DoT3chk						 							string	Spells FALSE
	/call setINIVar DoT3Condition 									string	Spells FALSE
	/call setINIVar DoT4							 							string	Spells FALSE
	/call setINIVar DoT4chk						 							string	Spells FALSE
	/call setINIVar DoT4Condition 									string	Spells FALSE
                                          		
	/call setINIVar spellCripple 										string 	Spells FALSE
	/call setINIVar ChallengeSpell									string 	Spells "Impose for Honor"
	/call setINIVar ValiantDefense									string 	Spells "Valiant Defense"
		                                      		
	/call setINIVar Twincast												string 	Spells "Glorious Exoneration"
	                                        		
	/call setINIVar switchHarmonious 								bool	 	Buffs TRUE
	/call setINIVar Harmonious 											string 	Buffs "Harmonious Blessing"
	/call setINIVar switchSteel			 								bool	 	Buffs TRUE
	/call setINIVar Steel			 											string 	Buffs "Reinvigorating Steel"
	/call setINIVar switchStance 										bool	 	Buffs TRUE
	/call setINIVar Stance 													string 	Buffs "Staunch Stance"
	                                        		
	/call setINIVar useMG														bool		Buffs TRUE
	/call setINIVar useMGPct												int			Buffs 60
/return	



|***
 *	DES: Warrior base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_WAR()
	/call setINIVar DiscInsult 											string 	Discs "Insult"
	/call setINIVar DiscCyclone			 								string	Discs "Cyclone Roar"
	/call setINIVar DiscWade				 								string	Discs "Wade In To Battle"
	/call setINIVar DiscChallenge										string	Discs "Roar of Challenge"
	/call setINIVar DiscShout				 								string	Discs "Tormenting Shout"
	/call setINIVar DiscRest			 									string	Discs "Breather"
	/call setINIVar DiscStoutDefense 								string	Discs "Stout Defense"
	/call setINIVar DiscField				 								string	Discs "Field Protector"
	/call setINIVar DiscFlash				 								string	Discs "Flash of Anger"
	/call setINIVar DiscNoTime				 							string	Discs "Pain Doesn't Hurt"
	/call setINIVar DiscAttention								 		string	Discs "Unflinching Attention"
	/call setINIVar DiscPhantom									 		string	Discs "Phantom Aggressor"
	/call setINIVar AAGutPunch 											string	Discs "Gut Punch"
	/call setINIVar AAWarStomp 											string	Discs "War Stomp"
	/call setINIVar AABraceforImpact 								string	Discs "Brace for Impact"
	/call setINIVar AAMageHunter										string	Discs "Mark of the Mage Hunter"
	/call setINIVar AAVhementRage 									string	Discs "Vhement Rage"
	/call setINIVar AABladeGuardian 								string	Discs "Blade Guardian"
	/call setINIVar AAKneeStrike 										string	Discs "Knee Strike"
	/call setINIVar AAImpCommand										string	Discs "Imperator's Command"
	/call setINIVar AAHeroicBlade 									string	Discs "Ward Sheol's Heroic Blade"
	/call setINIVar AAResplendentGlory 							string	Discs "Resplendent Glory"
	/call setINIVar AAHoldtheLine 									string	Discs "Hold the Line"
	/call setINIVar AAAETaunt 											string	Discs "Area Taunt"
	/call setINIVar AABlastofAnger									string	Discs "Blast of Anger"
	/call setINIVar AARageoftheForsaken							string	Discs "Rage of the Forsaken"
	/call setINIVar AAWarlordsTenacity							string	Discs "Warlords Tenacity"
	/call setINIVar AAWarlordsBravery								string	Discs "Warlords Bravery"
	/call setINIVar defCount												int			Discs 2
	                                        		
	/call setINIVar PanicHPPCT		 									int			Heal 25
	/call setINIVar spellCripple 										string 	Spells FALSE
	
	/call setINIVar ${Me.Class.ShortName}Point 			int 		Heal 80
/return	



|***
 *	DES: Berserkers base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_BER()
	/call setINIVar AEMelee					 								bool	 	Discs FALSE
	/call setINIVar DiscSnare 											string 	Discs FALSE
	/call setINIVar SnareAxe 												string 	Discs FALSE
	/call setINIVar DiscVolley 											string 	Discs "Brutal Volley"
	/call setINIVar CryOfChaos 											string 	Discs "Ancient: Cry of Chaos"
	/call setINIVar DiscBerserking 									string 	Discs "Berserking Discipline"
	/call setINIVar DiscRest 												string 	Discs "Breather"
	/call setINIVar DiscCombatFrenzy 								string 	Discs "Amplified Frenzy"
	/call setINIVar DiscScream 											string 	Discs "Distressing Scream"
	/call setINIVar ThrowingAxe 										string 	Discs "Demolishing Axe Throw"
	/call setINIVar DiscJolt 												string 	Discs "Jarring Smite"
	/call setINIVar SharedBloodLust 								string 	Discs "Shared Cruelty"
	/call setINIVar SharedBloodLustRecourse					string 	Discs "Reflected Brutality II"
	/call setINIVar DiscOverwhelmingFrenzy 					string 	Discs "Demolishing Frenzy"
	/call setINIVar DiscCleavingAnger 							string 	Discs "Cleaving Acrimony Discipline"
	/call setINIVar DiscSlapintheFace 							string 	Discs "Kick in the Shins"
	/call setINIVar DiscAxeofRallos 								string 	Discs "Axe of Numicia"
	/call setINIVar DiscVengeful 										string 	Discs "Avenging Flurry Discipline"
	/call setINIVar DiscFesteringRage 							string 	Discs "Bubbling Rage"
	/call setINIVar DiscCryHavoc 										string 	Discs "Cry Carnage"
	/call setINIVar DiscAugmentedFrenzy 						string 	Discs "Amplified Frenzy"
  /call setINIVar DiscOpenWound 									string 	Discs "Open Wound"
  /call setINIVar DiscFrenzyResolve								string 	Discs "Frenzied Resolve"
  /call setINIVar UntamedgRage										string 	Discs "Untamed Rage"
  /call setINIVar RecklessAbandon									string 	Discs "Reckless Abandon"
 	/call setINIVar frenzy													bool		meleedisc FALSE
 	/call setINIVar DiscDicho		 										string 	Discs "Dichotomic Rage"
 	/call setINIVar DiscDisconcert									string 	Discs "Disconcerting Discipline"
 	/call setINIVar DiscEnduranceTap								string 	Discs "Sapping Strikes"
 	/call setINIVar DiscPet													string 	Discs "Phantom Assailant"
	/call setINIVar ${Me.Class.ShortName}Poin	 			int 		Heal 60

	/call setDeclare OpenWoundEffect								string 	outer Open Wound
  /if (${OpenWoundDisc.Find[rk. III]}) {
		/varset OpenWoundEffect ${OpenWoundDisc.Left[-7]} effect III
	} else /if (${RaidBuffCast${x}.Find[rk. II]})  {
		/varset OpenWoundEffect ${OpenWoundDisc.Left[-6]} effect II
  }
	/call setDeclare AugmentedFrenzyEffect 					string outer Augmented Frenzy Effect
  /if (${AugmentedFrenzyDisc.Find[rk. III]}) {
		/varset AugmentedFrenzyEffect ${AugmentedFrenzyDisc.Left[-7]} effect III
	} else /if (${RaidBuffCast${x}.Find[rk. II]})  {
		/varset AugmentedFrenzyEffect ${AugmentedFrenzyDisc.Left[-6]} effect II
  } 
    
	/call setINIVar spellCripple 										string 	Spells FALSE
/return	



|***
 *	DES: Ranger base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_RNG()
	/call setTimer tNuke_Summer											string	Timer 3s timer_cast_nuke_summer
	/call create_timer timer_Elemental_Arrow 1

	/call setINIVar DoNuke					 								int			Spells 5
	/call setINIVar Nuke1					 									string	Spells "Anticipated Shots"	
	/call setINIVar Nuke1Condition 									string	Spells FALSE
	/call setINIVar Nuke2					 									string	Spells "Focused Arrowrain"
	/call setINIVar Nuke2Condition 									string	Spells FALSE
	/call setINIVar Nuke3					 									string	Spells "Heartsplit"
	/call setINIVar Nuke3Condition 									string	Spells FALSE
	/call setINIVar Nuke4					 									string	Spells "Wildfire Boon"
	/call setINIVar Nuke4Condition 									string	Spells FALSE
	/call setINIVar Nuke5					 									string	Spells "Wildfire Ash"
	/call setINIVar Nuke5Condition 									string	Spells FALSE
	/call setINIVar Nuke6					 									string	Spells FALSE
	/call setINIVar Nuke6Condition 									string	Spells FALSE
	/call setINIVar NukeSummer		 									string	Spells "Summer's Cyclone"
	/call setINIVar NukeSummerCondition							string	Spells FALSE
	/call setINIVar switchNukeSummer		 						bool		Spells TRUE
	                                        		
                                          		
	/call setINIVar DoDoT								 						int			Spells 0
	/call setINIVar DoT1							 							string	Spells FALSE
	/call setINIVar DoT1chk						 							string	Spells FALSE
	/call setINIVar DoT1Condition										string	Spells FALSE
	/call setINIVar DoT2							 							string	Spells FALSE
	/call setINIVar DoT2chk						 							string	Spells FALSE
	/call setINIVar DoT2Condition										string	Spells FALSE
	/call setINIVar DoT3							 							string	Spells FALSE
	/call setINIVar DoT3chk						 							string	Spells FALSE
	/call setINIVar DoT3Condition										string	Spells FALSE
	/call setINIVar DoT4							 							string	Spells FALSE
	/call setINIVar DoT4chk						 							string	Spells FALSE
	/call setINIVar DoT4Condition 									string	Spells FALSE
	/call setINIVar DoTSwarmDS		 									string	Spells "Swarm of Glistenwings"
	/call setINIVar DoTSwarmDSdeBuff 								string	Spells "Glistenwing Swarm"
	                                        		
	/call setINIVar spellCripple 										string 	Spells FALSE
	/call setINIVar SlowHeal												string	Spells "Cloudfont" 
	/call setINIVar FastHeal			 									string	Spells "Desperate Dousing" 
	/call setINIVar FastHealPCT		 									int			Spells 25
                                          		
	/call setINIVar DiscMele			 									string	Spells "Windstalkers's Discipline"
	/call setINIVar DiscRange			 									string	Spells "Pureshot Discipline"
	/call setINIVar DiscRest			 									string	Spells "Breather"
	/call setINIVar DiscJolt			 									string	Spells "Jolting Wheel Kicks"
	/call setINIVar DiscEnrage		 									string	Spells "Enraging Wheel Kicks"
	/call setINIVar DiscHeal			 									string	Spells "Reflexive Bladespurs"
	/call setINIVar BladeStorm											string	Spells "Focused Squall of Blades"
	/call setINIVar ElementalArrow									string	Spells "Elemental Arrow"
	/call setINIVar GlacialArow											string	Spells "Glacial Arrow"
	/call setINIVar VolatileArrow										string	Spells "Volatile Arrow"
	/call setINIVar ArrowPoison			 								string	Spells "Poison Arrows"	
	/call setINIVar ArrowFire			 									string	Spells "Flaming Arrows"	
	/call setINIVar ArrowIce				 								string	Spells "Frost Arrows"	
	/call setINIVar AuspiceTXT											string	Spells "AUSPICE (critSpell 33%, accuracy 230%, critHeal 33%, critMelee 225%, critDoT 33%, attack +165)"
	/call setINIVar DiscSqual			 									string	Spells "Focused Squall of Blades"
	                                        		
	/call setINIVar AADefensive1 										string	Spells "Outrider's Evasion"
	/call setINIVar AADefensive2 										string	Spells "Protection of the Spirit Wolf"
	/call setINIVar AADefensive3 										string	Spells "Bulwark of the Brownies"

	/call setINIVar ${Me.Class.ShortName}Point		 	int 		Heal 80
/return	



|***
 *	DES: Druid base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_DRU()
	/call setINIVar debuffRo			 									string	Spells "Blessing of Ro"	
	/call setINIVar WoodTXT													string	Spells "SoWood cast."
                                        		
	/call setINIVar spellCripple 										string 	Spells FALSE
	/call setINIVar DoNuke					 								int			Spells 0
	/call setINIVar Nuke1					 									string	Spells FALSE
	/call setINIVar Nuke1Condition 									string	Spells FALSE
	/call setINIVar Nuke2							 							string	Spells FALSE
	/call setINIVar Nuke2Condition 									string	Spells FALSE
	/call setINIVar Nuke3							 							string	Spells FALSE
	/call setINIVar Nuke3Condition 									string	Spells FALSE
	/call setINIVar Nuke4							 							string	Spells FALSE
	/call setINIVar Nuke4Condition 									string	Spells FALSE
	/call setINIVar DoDoT								 						int			Spells 0
	/call setINIVar DoT1							 							string	Spells FALSE
	/call setINIVar DoT1chk						 							string	Spells FALSE
	/call setINIVar DoT1Condition 									string	Spells FALSE
	/call setINIVar DoT2							 							string	Spells FALSE
	/call setINIVar DoT2chk						 							string	Spells FALSE
	/call setINIVar DoT2Condition 									string	Spells FALSE
	/call setINIVar DoT3							 							string	Spells FALSE
	/call setINIVar DoT3chk						 							string	Spells FALSE
	/call setINIVar DoT3Condition 									string	Spells FALSE
	/call setINIVar DoT4							 							string	Spells FALSE
	/call setINIVar DoT4chk						 							string	Spells FALSE
	/call setINIVar DoT4Condition 									string	Spells FALSE
	
	/call setDeclare MajorHurt 											int 		outer 0
	/call setDeclare WorstHurt 											string 	outer FALSE
	/call setDeclare WorstHurtID 										int 		outer
	/call setDeclare WorstHP 												int 		outer 100
	/call setDeclare WorstClass 										string 	outer FALSE
  /call setDeclare MinorHurt											int    	outer 0 
  /call setDeclare WorstHp 												int    	outer 100
  /call setDeclare pWorstHurt 										string 	outer ${Me.DisplayName}
  /call setDeclare pWorstHP												int			outer 100
  /call setDeclare pWorstHurtID										int 		outer ${Me.ID}
  /call setDeclare pWorstClass										string	outer FALSE
  /call setDeclare pMajorHurt											int			outer 0
	
	/call setINIVar RandRezDelay										int 		Heal 10
	/call setINIVar ProtectOutsideGroup							string	Heal "|FALSE|FALSE"
	/call setINIVar ProtectInsideRaid								string	Heal "|FALSE|FALSE"
	/call setINIVar ProtectNPCList									string	Heal "|FALSE|FALSE"
	/call setINIVar useGroupHeal 										bool		Heal TRUE
                                           	
	/call setINIVar healPanic			 									string  Heal "Survival of the Serendipitous"
	/call setINIVar healRejuvilation								string  Heal "Resurgence"
	/call setINIVar healAdrenaline				 					string 	Heal "Daggerthorn Rush"
	/call setINIVar healSanavida	 									string  Heal "Panavida"
	/call setINIVar healWholeGroup									string 	Heal "Lunasoothe"
	/call setINIVar XTHealRadius										int			Heal 300
	/call setINIVar RampageTank											string	Heal FALSE
	
	| ports	
	/call setDeclare looping												string	outer "Zephyr: Looping Planes"
	/call setDeclare steamfront											string	outer "Zephyr: Steamfront"
	/call setDeclare butcherblock										string	outer "Zephyr: Butcherblock"
	/call setDeclare shardslanding									string	outer "Zephyr: Shard's Landing"
	/call setDeclare pillars												string	outer "Zephyr: Pillars of Alra"
	/call setDeclare beast													string	outer "Zephyr: Beasts' Domain"
	/call setDeclare westkarna											string	outer "Zephyr: West Karana"
	/call setDeclare lavastorm											string	outer "Zephyr: Lavastorm"
	/call setDeclare undershore											string	outer "Zephyr: Undershore"
	/call setDeclare misty													string	outer "Zephyr: Misty"
	/call setDeclare ro															string	outer "Zephyr: Ro"
	/call setDeclare feerrott												string	outer "Zephyr: Feerrott"
	/call setDeclare commonlands										string	outer "Zephyr: Commonlands"
	/call setDeclare surefall												string	outer "Zephyr: Surefall Glade"
	/call setDeclare karana													string	outer "Zephyr: Karana"
	/call setDeclare wallofslaughter 								string	outer "Zephyr: Slaughter"
	/call setDeclare bloodfields 										string	outer "Zephyr: Bloodfields"
	/call setDeclare lceanium												string	outer "Zephyr: Lceanium"
	/call setDeclare dreadlands											string	outer "Zephyr: Combines"
	/call setDeclare dawnshroud											string	outer "Zephyr: Dawnshroud"
	/call setDeclare twilight												string	outer "Zephyr: Twilight"
	/call setDeclare grimling												string	outer "Zephyr: Grimling"
	/call setDeclare nexus													string	outer "Zephyr: Nexus"
	/call setDeclare primarypush										string	outer "Primary Anchor Push"
	/call setDeclare secondarypush									string	outer "Secondary Anchor Push"
	/call setDeclare primary												string	outer "Zephyr: Primary Anchor"
	/call setDeclare secondary											string	outer "Zephyr: Secondary Anchor"
	/call setDeclare tempesttemple									string	outer "Zephyr: Tempest Temple"
	/call setDeclare buriedsea											string	outer "Zephyr: Buried Sea"
	/call setDeclare stonebrunt											string	outer "Zephyr: Stonebrunt"
	/call setDeclare tox														string	outer "Zephyr: Toxxulia"
	/call setDeclare direwind												string	outer "Zephyr: Direwind"
	/call setDeclare steppes												string	outer "Zephyr: Steppes"
	/call setDeclare blightfire											string	outer "Zephyr: Blightfire Moors"
	/call setDeclare barindu												string	outer "Zephyr: Barindu"
	/call setDeclare natimbi												string	outer "Zephyr: Natimbi"
	/call setDeclare grounds												string	outer "Zephyr: the Grounds"
	/call setDeclare potime													string	outer "Zephyr: Plane of Time"
	/call setDeclare brells													string	outer "Zephyr: Brell's Rest"
	/call setDeclare arcstone												string	outer "Zephyr: Arcstone"
	/call setDeclare knowledge											string	outer "Zephyr: Knowledge"
	/call setDeclare cobaltscar											string	outer "Zephyr: Cobalt Scar"
	/call setDeclare wakening												string	outer "Zephyr: Wakening Land"
	/call setDeclare greatdivide										string	outer "Zephyr: Great Divide"
	/call setDeclare iceclad												string	outer "Zephyr: Iceclad"	
/return	



|***
 *	DES: Shaman base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_SHM()
	/call setDeclare MajorHurt 											int 		outer 0
	/call setDeclare WorstHurt 											string 	outer FALSE
	/call setDeclare WorstHurtID 										int 		outer ${Me.ID}
	/call setDeclare WorstHP 												int 		outer 100
	/call setDeclare WorstClass 										string 	outer FALSE
  /call setDeclare MinorHurt											int    	outer 0 
  /call setDeclare WorstHp		 										int    	outer 100	

	/call setTimer tTalisman												string	Timer 30s timer_cast_talisman
	/call setINIVar spellTalisman	 									string	Spells "Talisman of the Leopard"
	/call setINIVar switchTalisman			 						bool		Spells TRUE
                                          		
	/call setINIVar AidTXT 													string 	Spells "ANCESTRAL AID Done (Str/Agi/Dex +300, HoT Heal +1840)"
	/call setINIVar spellMalo 											string 	Spells FALSE
	/call setINIVar spellCripple 										string 	Spells FALSE
	/call setINIVar spellSlow												string 	Spells FALSE
	/call setINIVar spellAESlow 										string 	Spells "Srasku's Drowse"
	                                        		
	/call setINIVar TC1									 						string	Spells "Glacial Gift"	
	/call setINIVar TC2									 						string	Spells "Frigid Gift"
	/call setINIVar DoNuke							 						int			Spells 0
	/call setINIVar Nuke1					 									string	Spells FALSE
	/call setINIVar Nuke1Condition 									string	Spells FALSE
	/call setINIVar Nuke2					 									string	Spells FALSE
	/call setINIVar Nuke2Condition 									string	Spells FALSE
	/call setINIVar Nuke3					 									string	Spells FALSE
	/call setINIVar Nuke3Condition 									string	Spells FALSE
	/call setINIVar DoDoT						 								int			Spells 0
	/call setINIVar DoT1							 							string	Spells FALSE
	/call setINIVar DoT1chk						 							string	Spells FALSE
	/call setINIVar DoT1Condition 									string	Spells FALSE
	/call setINIVar DoT2							 							string	Spells FALSE
	/call setINIVar DoT2chk						 							string	Spells FALSE
	/call setINIVar DoT2Condition 									string	Spells FALSE
	/call setINIVar DoT3							 							string	Spells FALSE
	/call setINIVar DoT3chk						 							string	Spells FALSE
	/call setINIVar DoT3Condition 									string	Spells FALSE
	/call setINIVar DoT4							 							string	Spells FALSE
	/call setINIVar DoT4chk						 							string	Spells FALSE
	/call setINIVar DoT4Condition 									string	Spells FALSE
                                          		
	/call setINIVar CaniPercent				 							int			Spells 60
	/call setINIVar spellCannibalization						string	Spells "Tribal Pact"
                                          		
	/call setINIVar useGroupHeal 										bool		Heal TRUE
	/call setINIVar useGroupHoT											bool		Heal TRUE
	/call setINIVar RandRezDelay										int 		Heal 10
                                          		
  /call setINIVar healSurge												string 	Heal "Spiritual Surge"
  /call setINIVar healMain												string 	Heal "Krasir's Mending"
  /call setINIVar healIntervention1								string 	Heal "Historian's Intervention"
  /call setINIVar healReckless										string 	Heal "Reckless Regeneration"
  /call setINIVar healWholeGroup									string 	Heal "Krasir's Recourse"
  /call setINIVar healWholeGroupHoT								string 	Heal "Shear of Renewal"
                                          		
	/call setINIVar ProtectOutsideGroup							string	Heal "|FALSE|FALSE"
	/call setINIVar ProtectInsideRaid								string	Heal "|FALSE|FALSE"
	/call setINIVar ProtectNPCList									string	Heal "|FALSE|FALSE"
	/call setINIVar XTHealRadius										int			Heal 300
	/call setINIVar RampageTank											string	Heal FALSE

	/call setDeclare HealList 											string 	outer "|${healSurge}|${healWholeGroup}|${healWholeGroupHoT}|${healMain}|${healIntervention1}|${healReckless}"
/return	



|***
 *	DES: Bard base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_BRD()
	/call setDeclare mobList												string 	outer
	/call setDeclare addList 												string 	outer
		
	/call setINIVar ServantNukeAA1 									string 	Pet "Lyrical Prankster"
	/call setINIVar ServantNukeAA2 									string 	Pet "Song of Stone"
	/call setINIVar songServantNuke									string 	Pet "Plague of the Piper"
	                                          		
	/call setDeclare lineType												string	outer |caster|melee|mix|other|tank
	/call setINIVar lineCurrent											string 	Song Tank
	/call setINIVar lineTank												string	Song "|Chorus of Sionachie|War March of Jocelyn|Fjilnauk's Spiteful Lyric Rk. II|Fjilnauk's Song of Suffering|Doben's Spry Sonata Rk. II"
	/call setINIVar lineCaster											string	Song "|Chorus of Sionachie|Nilsara's Aria|Fatesong of Jocelyn|Arcane Melody|Aria of Maetanrus"
	/call setINIVar lineMelee												string	Song "|Chorus of Sionachie|War March of Jocelyn|Fjilnauk's Spiteful Lyric|Fjilnauk's Song of Suffering|Arcane Melody"
	/call setINIVar lineMix													string	Song "|Chorus of Sionachie|War March of Jocelyn|Fjilnauk's Spiteful Lyric|Nilsara's Aria"
	/call setINIVar lineOther												string	Song FALSE
	                                          		
	/call setINIVar songDicho												string	Song "Dichotomic Psalm"
	                                          		
	/call setINIVar DoNuke					 								int			Song 0
	/call setINIVar Nuke1														string 	Song FALSE
	/call setINIVar Nuke2														string 	Song FALSE
                                            		
	/call setINIVar DoDoT						 								int			Song 0
	/call setINIVar DoT1														string 	Song FALSE
	/call setINIVar DoT2														string 	Song FALSE
                                            		
	/call setINIVar spellMez												string 	Song "Slumber of Silisia"
	/call setINIVar spellpbaeMez										string	Song "Wave of Torpor"
	/call setINIVar spellSlow												string	Song "Requiem of Time"
	/call setINIVar switchRestSong									bool		Song FALSE
	/call setINIVar spellRestSong										string	Song FALSE
	/call setINIVar spellCripple 										string 	Spells FALSE
	/call setINIVar ${Me.Class.ShortName}Point		 	int 		Heal 80
	                                          		
	/call setINIVar switchIntimidKick 							bool	 	meleedisc FALSE
	
	
/return	


|***
 *	DES: Beastlord base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_BST()
	/call setINIVar DiscForay 											string 	Spells "Pummel"
	/call setINIVar DiscFocusClaws 									string 	Spells "Focused Clamor of Claws"
	/call setINIVar DiscSavage 											string 	Spells "Savage Rage"
	/call setINIVar DiscRuaabri 										string 	Spells "Ruaabri's Fury"	
	/call setINIVar GrowlSpell 											string 	Spells "Growl of the Leopard"
 	/call setINIVar ServantNuke		 									string 	Spells "Cry at the Moon"
  /call setINIVar FeroSpell 											string 	Spells "Savage Ferocity"
  /call setINIVar DiscEnduring										string 	Spells "Enduring Frenzy"
  /call setINIVar DiscRending											string 	Spells "Reflexive Rending"
	                                           		
	/call setINIVar GrowlSpell 											string 	Pet "Growl of the Leopard"
	/call setINIVar TasteofBlood										string 	Pet "Taste of Blood"
	/call setINIVar Aggression											string 	Pet "Sekmoset's Aggression"
	/call setINIVar ProcHeal												string 	Pet "Mending Warder"
	/call setINIVar Calm														string 	Pet "Companion's Calm Demeanor"
	/call setINIVar DiscRest				 								string	Spells "Breather"
	/call setINIVar ParagonTXT 											string 	Spells "PARAGON DONE (+5790 HP/tick, +2450 Mana/tick, +264 End/tick)"
	                                           		
	/call setINIVar DoNuke							 						int			Spells 5
	/call setINIVar Nuke1					 									string	Spells "Fozen Miasma"	
	/call setINIVar Nuke1Condition 									string	Spells FALSE
	/call setINIVar Nuke2					 									string	Spells "Kromtus Lance"
	/call setINIVar Nuke2Condition 									string	Spells FALSE
	/call setINIVar Nuke3					 									string	Spells "Visoracius' Maelstrom"
	/call setINIVar Nuke3Condition 									string	Spells FALSE
	/call setINIVar Nuke4					 									string	Spells "Kromrif Lance"
	/call setINIVar Nuke4Condition 									string	Spells FALSE
	/call setINIVar Nuke5					 									string	Spells "Krieg's Bite"
	/call setINIVar Nuke5Condition 									string	Spells FALSE
	/call setINIVar Nuke6					 									string	Spells FALSE
	/call setINIVar Nuke6Condition 									string	Spells FALSE
                                             		
	/call setINIVar DoDoT						 								int			Spells 0
	/call setINIVar DoT1							 							string	Spells FALSE
	/call setINIVar DoT1chk						 							string	Spells FALSE
	/call setINIVar DoT1Condition 									string	Spells FALSE
	/call setINIVar DoT2							 							string	Spells FALSE
	/call setINIVar DoT2chk						 							string	Spells FALSE
	/call setINIVar DoT2Condition 									string	Spells FALSE
	/call setINIVar DoT3							 							string	Spells FALSE
	/call setINIVar DoT3chk						 							string	Spells FALSE
	/call setINIVar DoT3Condition 									string	Spells FALSE
	/call setINIVar DoT4							 							string	Spells FALSE
	/call setINIVar DoT4chk						 							string	Spells FALSE
	/call setINIVar DoT4Condition 									string	Spells FALSE
	                                           		
	/call setINIVar FastHeal			 									string	Spells "Sabhattin's Mending" 
	/call setINIVar FastHealPCT		 									int			Spells 60
	                                           		
	/call setINIVar selfParagon		 									int			Spells 80
	/call setINIVar groupParagon	 									int			Spells 70
	/call setINIVar minConsumptionHP								int			Spells 50
	/call setINIVar spellSlow		 										string	Spells "Sha's Reprisal"		
	/call setINIVar spellCripple 										string 	Spells "Incapacitate"
                                             		
	/call setINIVar ${Me.Class.ShortName}Point 			int 		Heal 80
/return	



|***
 *	DES: Monk base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_MNK()
	/call setINIVar spellCripple 										string 	Spells FALSE

/return	



|***
 *	DES: Rogue base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_ROG()
	| keep on cooldown
	/call setTimer tClick_Poison										string	Timer 30s 	timer_click_poison
	/call setINIVar AEMelee					 								bool	 	Discs FALSE
                                             		
	/call setINIVar DiscRest				 								string 	Discs "Breather"
	/call setINIVar DiscAggro 											string 	Discs "Disorientation"
	/call setINIVar DiscAssault 										string 	Discs "Barrage"
	/call setINIVar DiscBleed 											string 	Discs "Hack"	
	/call setINIVar DiscPlay	 											string 	Discs "Knifeplay Discipline"	
	/call setINIVar DiscPhantom											string 	Discs "Phantom Assassin"	
	/call setINIVar DiscHack												string 	Discs "Jugular Hack"	
	/call setINIVar DiscHiddenBlade									string 	Discs "Hidden Blade"	
	/call setINIVar DiscBladePoison									string 	Discs "Reefcrawler Blade"	
	/call setINIVar DiscSneakAttack									string 	Discs "Daggergash"	
	/call setINIVar DiscShadowHunter								string 	Discs "Shadow-Hunter's Dagger"	
	/call setINIVar DiscDisassociative							string 	Discs "Disassociative Puncture"	
	/call setINIVar DiscAspBleeder									string 	Discs "Aceleus Discipline"	
	/call setINIVar DiscBlinding										string 	Discs "Blinding Brilliance"	
                                             		
	| disable for burn                         		
	/call setINIVar DiscVision 											string 	Discs "Thief's Vision"	
                                             		
	| burn                                     		
	/call setINIVar DiscPinPoint 										string 	Discs "Pinpoint Deficiencies"
	/call setINIVar DiscMark 												string 	Discs "Wide-Eyed Mark"
	/call setINIVar DiscEradicator 									string 	Discs "Executioner Discipline"
	/call setINIVar DiscRazor 											string 	Discs "Razor's Edge Discipline"	
	/call setINIVar DiscFrenzy 											string 	Discs "Frenzied Stabbing Discipline"	
	/call setINIVar DiscTwistedChance								string 	Discs "Twisted Chance Discipline"
                                             		
	/call setINIVar TwistedShank										string 	Discs "Twisted Shank"
	/call setINIVar AbsorbingAgent									string 	Discs "Absorbing Agent"
	                                           		
	/call setINIVar clickPoisonBuff									string 	Poison |Bite of the Shissar Poison X|Tallon's Tactic Poison IV|Tallon's Tactic Poison V|Vallon's Tactic Poison IV|Vallon's Tactic Poison V|Mana Poison Coat|Hero's Toxin Poison I|Hero's Toxin Poison II
	/call setINIVar clickPoisonItem									string 	Poison Consigned Bite of the Shissar XVIII
	/call setINIVar clickPoisonDispenser						string 	Poison Selrach's Shadowscale Leggings
	/call setINIVar clickPoisonCount								int		 	Poison 60
                                             		
	/call setINIVar spellCripple 										string 	Spells FALSE
                                             		
	/call setINIVar ${Me.Class.ShortName}Point 			int 		Heal 80
/return	



|***
 *	DES: Cleric base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_CLR()
	DEBUG ${cbug}declare_CLR()\ax
	/call setTimer tHealWard												string	Timer 185s timer_heal_ward
	/call setINIVar switchLockMA										bool 		Heal FALSE
		
	/call setDeclare WorstHurt 											string 	outer FALSE
	/call setDeclare WorstHP 												int 		outer 100
	/call setDeclare WorstClass 										string 	outer FALSE
	/call setDeclare MajorHurt 											int 		outer 0
	/call setDeclare MinorHurt											int    	outer 0 
	/call setDeclare groupHurt 											int 		outer 0

	/call setDeclare pWorstHurt 										string 	outer ${Me.DisplayName}
	/call setDeclare pWorstHP												int			outer 100
	/call setDeclare pWorstHurtID										int 		outer ${Me.ID}
	/call setDeclare pWorstClass										string	outer FALSE
	/call setDeclare pMajorHurt											int			outer 0
	
	/call setINIVar DivineRezCleric									bool 		Heal TRUE
	/call setINIVar RandRezDelay										int 		Heal 5
	/call setINIVar ProtectOutsideGroup							string	Heal |FALSE|FALSE
	/call setINIVar ProtectInsideRaid								string	Heal |FALSE|FALSE
	/call setINIVar ProtectNPCList									string	Heal |FALSE|FALSE
	/call setINIVar useGroupHeal 										bool		Heal TRUE
	/call setINIVar useGroupHoT											bool		Heal TRUE
	/call setINIVar tempPet						 							string	Pet "Ardent Hammer of Zeal"
	/call setINIVar setDivinetoGroupHeal						bool		Heal FALSE
                                            		
  /call setINIVar healWholeGroup									string 	Heal "Word of Greater Reformation"
  /call setINIVar healWholeGroup2									string 	Heal "Syllable of Convalescence"
  /call setINIVar healWholeGroup3									string 	Heal "Word of Convalescence"
  /call setINIVar healWholeGroupHoT								string 	Heal "Elixir of the Seas"
  /call setINIVar healRemedy1											string 	Heal "Merciful Remedy"
  /call setINIVar healRemedy2											string 	Heal "Spiritual Remedy"
  /call setINIVar healRemedy3											string 	Heal "Graceful Remedy"
  /call setINIVar healHoT 												string 	Heal "Ardent Elixir"
  /call setINIVar healSplash											string 	Heal "Convalescent Splash"
  /call setINIVar healIntervention1 							string  Heal "Merciful Intervention"
  /call setINIVar healIntervention2 							string  Heal "Mystical Intervention"
  /call setINIVar healIntervention3 							string 	Heal "Virtuous Intervention"
  /call setINIVar healIntervention4 							string  Heal "Elysian Intervention"
  /call setINIVar healRenewal1	 									string  Heal "Fervid Renewal"
  /call setINIVar healRenewal2	 									string  Heal "Fraught Renewal"
  /call setINIVar healRenewal3	 									string  Heal "Fervent Renewal"
  /call setINIVar healPanic			 									string  Heal "Fifteenth Emblem"
  /call setINIVar healWard			 									string  Heal "Ward of Surety"
  /call setINIVar healWardToon	 									string  Heal |FALSE|FALSE
  /call setINIVar useCelestialRegen								bool	  Heal TRUE
  /call setINIVar healIssuance										string  Heal "Issuance of Spirit"
	/call setINIVar CRTXT														string	Heal "Celestial Regeneration (+11100HP / tic)"
	/call setINIVar XTHealRadius										int			Heal 300
	/call setINIVar switchForceGroupHeal						bool		Heal FALSE
	/call setINIVar RampageTank											string	Heal FALSE
	/call setINIVar switchHealAggressive						string	Heal FALSE
	/call setINIVar switchCompleteHeal							string	Heal FALSE
	/call setINIVar CompleteHealpct									int 		Heal 70	
	                                          		
	/call setINIVar doYaulp 												string	Buffs TRUE
	/call setINIVar spellYaulp 											string	Buffs FALSE
	/call setINIVar useYaulpPct											string	Buffs 98
	/call setINIVar sumWeaponSpell									string	Buffs "Hammer of	Reverence"
	/call setINIVar sumWeaponItem										string	Buffs "Hammer of	Reverence III"
	/call setINIVar AAFade													string	Buffs "Divine Peace"
	/call setINIVar useAAFade												int			Buffs 50
                                            		
	/call setINIVar useVP														bool		Buffs TRUE
	/call setINIVar useVPPct												int			Buffs 40
                                            		
	/call setINIVar spellCripple 										string 	Spells FALSE
                                            		
	/call setINIVar DoNuke					 								int			Spells 0
	/call setINIVar Nuke1					 									string	Spells FALSE	
	/call setINIVar Nuke1Condition 									string	Spells FALSE
	/call setINIVar Nuke2					 									string	Spells FALSE
	/call setINIVar Nuke2Condition 									string	Spells FALSE
	/call setINIVar Nuke3					 									string	Spells FALSE
	/call setINIVar Nuke3Condition 									string	Spells FALSE
	/call setINIVar Nuke4					 									string	Spells FALSE
	/call setINIVar Nuke4Condition 									string	Spells FALSE
	/call setINIVar DoDoT								 						int			Spells 0
	/call setINIVar DoT1							 							string	Spells FALSE
	/call setINIVar DoT1chk						 							string	Spells FALSE
	/call setINIVar DoT1Condition										string	Spells FALSE
	/call setINIVar DoT2							 							string	Spells FALSE
	/call setINIVar DoT2chk						 							string	Spells FALSE
	/call setINIVar DoT2Condition										string	Spells FALSE
	/call setINIVar DoT3							 							string	Spells FALSE
	/call setINIVar DoT3chk						 							string	Spells FALSE
	/call setINIVar DoT3Condition										string	Spells FALSE
	/call setINIVar DoT4							 							string	Spells FALSE
	/call setINIVar DoT4chk						 							string	Spells FALSE
	/call setINIVar DoT4Condition 									string	Spells FALSE
/return	



|***
 *	DES: This will clear all store alises that core has created in your MacroQuest2.INI file. or those with matching names.
 *	USE: /core whitewash
 *	INI: MacroQuest2.INI
 *  NOTE: IF you instance/sandbox MQ/EQ it is best you run this on all toons at the same time. Sorry it's slow as fuck.
 ***|
sub whitewash(bool _silent)
	/if (!${_silent}) VOUT ${owarning} ${sep} Deleting all Core ${cinfo}${coreBuild}\ax aliases. ${cbad}Please Wait\ax
	/declare _count 				int local 0
	/declare _todelete[1] 	string local
	/declare _index 				int local 0
	/declare _increment			int local 1
	
	| retired aliases
	| /varset _todelete[xx] |healoverride|healself|healgroup|healxtarget|healpet|healcure|healaggressive|autoshrink|toast|companionswarm|companion|home|paragon|combatradius|HuntMobAgro
	| /varset _todelete[xx] |spice|stop|setagro|smartma|setpull|HUDTime|banestrike|broadcast|ib|sb|pb|resetcore|verbose|buildpet|autorest|toastauto|primary|secondary|invis|info|whohas
	| /varset _todelete[xx] |rezeveryone|rezcombat|rezauto|rezemergency|rezenable|core|invites|facefast|stickbehind|hudupdate|qmpct|qm|toastmobcount|toastengageat|combatdistance
	| /varset _todelete[xx] |debuffmezz|debuffslow|debufftash|debuffmalo|debuffcripple|debufferadicate|debuffsnare|checkcash|agro|engageat|autorest|smartengageat|watchspawn|petengageat|petswarmengageat
	| /varset _todelete[xx] |buffbc|bufftell|buffpet|buffitem|buffraid|buffraid|buffself|buffcombat|tie|ignorelos|forage|fish|train|petfarm|drag|ri|hunt|tietoon|tiedistance|tietime
	| /varset _todelete[xx]	|listremove|listnomezz|listnocast|listexclude|listnamed|listnomelee|listrange|listnomagic|listnodisease|listnofire|listnopoison|listnocold|listnocorruption|listhuntmob
	| /varset _todelete[xx] |lootleave|lootkeep|lootdestroy|lootsell|buy|sell|lootwhat|lootannounce|lootignore|pettype|pettank|whitewash|doyaulp|cr|ma1|ma2|info|huntagro|killcode|useeradicate
	| /varset _todelete[xx] |control|smartengageat|killmob|combat|on|off|maxlevel|autoadjustexp|oneoff|userain|crew|autologin|usebuffkill|pathiosis|bcsay|blurb|clean|stack|stein|gtfo|door|echochannel|core
	| /varset _todelete[xx] |mirror|maintexplvl|autocontrol|echoaugments|relay|givecoin|stop|usegrouphot|usegroupheal|rc|purify|sendpets|harvest|gather|rod|redorod|rods|song|glyph|lesson|wood|cani
	| /varset _todelete[xx] |here|take|homeradius|homeleash
		
	| core aliases
	/varset _todelete[1] |take|door|stack|here|sendpets|killmob|mirror|stein|stop|blurb|bcsay|echoaugments|autologin|rc|purify|buy|sell|on|off|zonereset|givecoin|oneoff|gtfo|lesson

	
	/for _count 1 to ${_todelete.Size}
		DEBUG _count: ${_count}-${_todelete.Size}
		/for _index 1 to ${_todelete[${_count}].Count[|]}
			DEBUG _index: ${_count}-${_todelete.Size}: ${_index} ${_todelete[${_count}].Arg[${_index},|]}
			/if (!${_silent}) VOUT Deleting Alias ${_increment}: ${cinfo}${_todelete[${_count}].Arg[${_index},|]}\ax
			/squelch /alias /${_todelete[${_count}].Arg[${_index},|]} delete
			/varcalc _increment ${_increment}+1
		/next _index	
	/next _count
	
	/if (!${_silent}) {
		/call echos ending
		/endm
	}
/return



|* Created Events ******************************************************************************************************
  
|***
 *	DES: stuf we do cause we are lazy.
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub set_alias()
	DEBUG ${cbug}set_alias()\ax Setting Aliases
	/if (!${Bool[${Alias[/take]}]}) 								/squelch /alias /take 											/bc take
	/if (!${Bool[${Alias[/door]}]}) 								/squelch /alias /door 											/bc door
	/if (!${Bool[${Alias[/stack]}]}) 								/squelch /alias /stack 											/bcaa //invis
	/if (!${Bool[${Alias[/here]}]}) 								/squelch /alias /here 											/bca //setcombathere
	                                          			
  /if (!${Bool[${Alias[/sendpets]}]}) 						/squelch /alias /sendpets										VOUT SendPets: 
	/if (!${Bool[${Alias[/killmob]}]}) 							/squelch /alias /killmob 										/bc killmob
	/if (!${Bool[${Alias[/mirror]}]}) 							/squelch /alias /mirror 										/bc mirror
	/if (!${Bool[${Alias[/stein]}]}) 								/squelch /alias /stein 											/bc stein
  /if (!${Bool[${Alias[/stop]}]}) 								/squelch /alias /stop 											/bc stop
	/if (!${Bool[${Alias[/blurb]}]}) 								/squelch /alias /blurb 											VOUT Blurb: 
 	/if (!${Bool[${Alias[/clean]}]}) 								/squelch /alias /clean 											VOUT Clean this mess up.
  /if (!${Bool[${Alias[/bcsay]}]}) 								/squelch /alias /bcsay 											/bc bcsay
  /if (!${Bool[${Alias[/echoaugments]}]}) 				/squelch /alias /echoaugments								VOUT Echo Augment file!
  /if (!${Bool[${Alias[/autologin]}]}) 						/squelch /alias /autologin									/plugin mq2autologin unload
                                            			
	| quick cleric cure commands              			
	/if (!${Bool[${Alias[/rc]}]}) 									/squelch /alias /rc 												/bc rc
	/if (!${Bool[${Alias[/purify]}]}) 							/squelch /alias /purify 										/bc purify
		                                        			
  /if (!${Bool[${Alias[/buy]}]}) 									/squelch /alias /buy												/echo Buy:
  /if (!${Bool[${Alias[/sell]}]}) 								/squelch /alias /sell												/echo Sell:
                                            			
	/if (!${Bool[${Alias[/on]}]}) 									/noparse /squelch /alias /on       		    	/bca //core auto on
	/if (!${Bool[${Alias[/off]}]}) 									/noparse /squelch /alias /off						    /bca //core auto off
	/if (!${Bool[${Alias[/zonereset]}]}) 						/noparse /squelch /alias /zonereset			    /bc zonereset
                                            			
	/if (!${Bool[${Alias[/givecoin]}]}) 						/noparse /squelch /alias /givecoin			    VOUT Bribe Time:
                                            			
	/if (!${Bool[${Alias[/oneoff]}]}) 							/noparse /squelch /alias /oneoff   					/bc oneoff
                                            			
	/if (!${Bool[${Alias[/gtfo]}]}) 								/noparse /squelch /alias /gtfo    			 		/bc gtfo
/return


#event Join "#1# invites you to join a #2#.#*#"
sub Event_Join(string line, string _sender, string _groupraid)
	DEBUG ${cbug}Event_Join(\ax${_sender} ${_groupraid}\ax${cbug})\ax
	/if (!${switchInvites}) /return
	CHECKSAFELIST
	VOUT Accepting invite from ${cpc}${_sender}\ax for ${_groupraid}.
	/if (${_groupraid.Equal[group]}) /invite
	/if (${_groupraid.Equal[raid]}) {
		/if (${Window[ConfirmationDialogbox].Child[CD_TextOutPut].Text.Find["join a raid"]}) {
			/nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
		} else {	
			/raidaccept	
		}
	}
/return



|***
 *	DES: contains the various BC commands witn NO variables passed
 *	USE: /bc SOMETHINGORANOTHER
 *	INI: 
 *  NOTE: works off tells or broadcasts
 ***| 
#event EQBC "<#1#> #2#"
#event EQBC "[#1#(msg)] #2#"
sub Event_EQBC(string line, string _sender, string _cmd)
	DEBUG cmd: ${_cmd} _sender ${_sender}

	/if (${_cmd.Equal[stop]}) /call set_stop FALSE
  /if (${_cmd.Equal[killmob]}) /call Bind_override engageat
 
   | 	/varset switchOverrideEngageat ${If[${switchOverrideEngageat},FALSE,TRUE]}
		| /call echos override "Engageat" switchOverrideEngageat
 
  
  /if (${_cmd.Equal[mirror]} && ${Me.ItemReady[Mirror Fragment of Anashti Sul]}) /call core_cast "Mirror Fragment of Anashti Sul" item 0 FALSE
  /if (${_cmd.Equal[stein]} && ${Me.ItemReady[Drunkard's Stein]}) /call core_cast "Drunkard's Stein" item 0 FALSE
  /if (${_cmd.Equal[oneoff]}) /call check1offBuffs

 
	| commands to control all assist targets.  
  /if (${_cmd.Equal[sma1]} && !${switchPull}) {
		/call Bind_setCombatControl smartma R
  } else /if (${_cmd.Equal[smag]} && !${switchPull}) {
		/call Bind_setCombatControl smartma G
	} else /if (${_cmd.Equal[smax]}) {
		/call Bind_setCombatControl smartma X
	}
 

	| group bail stuff
  /if (${_cmd.Equal[gtfo]}) {
  	/if (${Bool[${GTFO}]}) {
  		/call sort_cursor TRUE
  		/call set_stop TRUE
  		/call clear_combat
  		/docommand /makemevisible
  		/if (${Me.ItemReady[${GTFO}]}) /call MQ2Cast "${GTFO}" item 0
   		/if (${Me.SpellReady[${GTFO}]}) /call MQ2Cast "${GTFO}" 0 0
   		/if (${Me.AltAbility[${GTFO}].ID}) /call MQ2Cast "${GTFO}" alt 0
  	}
  }

	|***
	 *	DES: Tell toon to take ports: translocate, banner, Guild Hall TL, Task Acceptance
	 *	USE: /take, /bc take
	 ***|
	/if (${_cmd.Equal[port]} || ${_cmd.Equal[take]}) {
		| take ports
		/if (${Window[ConfirmationDialogbox].Child[CD_TextOutPut].Text.Find[${Spawn[${Me}].Guild}]} || ${Window[ConfirmationDialogbox].Child[CD_TextOutPut].Text.Find["Guild Hall"]}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
		/if (${Window[ConfirmationDialogbox].Child[CD_TextOutPut].Text.Find["be translocated by"]}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
		/if (${Window[LargeDialogWindow].Child[LDW_TextBox].Text.Find["Would you like to travel"]}) /nomodkey /notify LargeDialogWindow LDW_YesButton leftmouseup 

		| take taskadds/missions/HA
		/if (${Window[LargeDialogWindow].Child[LDW_TextBox].Text.Find["Would you like to take on the challenge"]}) /nomodkey /notify LargeDialogWindow LDW_YesButton leftmouseup 
		/if (${Window[ConfirmationDialogBox].Child[CD_TextOutPut].Text.Find["Join the expedition"]}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup 
		/if (${Window[LargeDialogWindow].Child[LDW_TextBox].Text.Find["You have a heroic adventure"]}) /nomodkey /notify LargeDialogWindow LDW_YesButton leftmouseup
		/if (${Window[ConfirmationDialogbox].Child[CD_TextOutPut].Text.Find["has asked you to join the shared task"]}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
		
		| trade window
		/if (${Window[Tradewnd].HisTradeReady}) {
			/nomodkey /notify Tradewnd TRDW_Trade_Button leftmouseup
			/delay 1s ${Cursor.ID}
			/call sort_cursor TRUE
		}		
		/return TRUE
	}

	|***
	 *	DES: Tell toon to take doors
	 *	USE: /door, /bc door
	 ***|
	/if (${_cmd.Equal[door]}) {
		/call create_timer timer_try_door 4s
		:retrydoortarget
		/if (!${timer_try_door}) /return
		/doortarget 
		/delay 1
		/if (!${Switch.ID}) /goto :retrydoortarget
		:retryopendoor
		/if (!${timer_try_door}) /return
		/if (!${Switch.Open}) {
			/click left door
			/delay 1s
			/doevents flush
		}
		/return TRUE
	}

	|***
	* DES: Tell toon to campfire clickie
	* USE: /bc campfire
	***|
	/if (${_cmd.Equal[campfire]} && ${Me.ItemReady[Fellowship Registration Insignia]}) {
		/if (${Me.Invis}) /makemevisible
		/delay 1s !${Me.Invis}
		/if (${Me.Casting.ID}) /stopcast
		/delay 1s !${Me.Casting.ID}
		| /call core_cast  "Fellowship Registration Insignia" item 0 FALSE
		/useitem Fellowship Registration Insignia
		/return
	}

	|***
	* DES: reset the core.zone.ini for all toons in BC if you make a change to the file.
	* USE: /bc zonereset
	***|
	/if (${_cmd.Equal[zonereset]}) {
		/if (!${Spawn[pc ${_sender}].ID}) /return
		/call echos zone 0 0 0 0
		/call check_zone_INI ${_sender}
		/if (${coreAuto} && ${switchShrink}) /call check_shrink
		/return TRUE
	}

	|***
	* DES: casts AA radient cure
	* USE: /rc | /bc rc
	***|
	/if (${_cmd.Equal[rc]}) {
		/if (!${Select[${Me.Class.ShortName},CLR,DRU,SHM,PAL]}) /return FALSE
		/if (!${Me.AltAbilityReady[Radiant Cure]}) /return FALSE
		/if (${Me.Casting.ID}) /stopcast
		/delay 1s !${Me.Casting.ID}
		/call core_cast "Radiant Cure" alt 0 FALSE
		ECHOCHANNEL Radiant Cure
		/return TRUE
	}
		
	|***
	* DES: casts AA purified spirits
	* USE: /purify | /bc purify
	***|
	/if (${_cmd.Equal[purify]}) {
		/if (!${Select[${Me.Class.ShortName},CLR,DRU,SHM]}) /return FALSE
		/if (!${Me.AltAbilityReady[Purified Spirits]}) /return FALSE
		/if (${Me.Casting.ID}) /stopcast
		/delay 1s !${Me.Casting.ID}
		/call core_cast "Purified Spirits" alt ${Spawn[pc ${_sender}].ID} FALSE
		ECHOCHANNEL Purified Spirits: ${Spawn[pc ${_sender}].Name}
		/return TRUE
	}
	
	|***
	* DES: Magician AA Call of Heroe
	* USE: /g coh
	***|
	/if (${_cmd.Equal[coh]} && ${Select[${Me.Class.ShortName},MAG]}) {
		CHECKSAFELIST
	  /delay 15s ${Me.AltAbilityReady[Call of the Hero]}
	  /if (${Me.Invis}) /makemevisible
	  ECHOCHANNEL Calling ... ${_sender}
	 	/call core_cast "Call of the Hero" alt ${Spawn[pc ${_sender}].ID} FALSE
		/return TRUE
	}
	
/return



|***
 *	DES: contains the various BC commands witn ONE variable passed
 *	USE: /bcsay SOMETHINGORANOTHER
 *	INI: 
 *  NOTE: works off tells or broadcasts
 ***|
#event EQBC_say "<#1#> bcsay #2#"
#event EQBC_say "[#1#(msg)] bcsay #2#"
sub Event_EQBC_say(string line, string _sender, string _verbage)
	DEBUG _sender ${_sender} _verbage ${_verbage}
 	| have everyone say the same thing to the same target with a touch of random delay
	/if (${_sender.NotEqual[${Me.DisplayName}]} && ${Bool[${Spawn[pc ${_sender} radius 60].ID}]}) {
		/assist ${_sender}
		/delay 5s ${Me.AssistComplete} 
		/if (!${Target.ID}) /return
	}
	/while (${Me.Invis}) {
		/makemevisible
		/delay 1s !${Me.Invis}
	}
	/say ${_verbage}
	/delay 1s		
/return

	
#event camping "It will take you about 30 seconds to prepare your camp."
sub Event_camping(string line)
	DEBUG ${cbug}Event_camping()\ax
	/call clear_combat
	VOUT Core Build ${cinfo}${coreBuild}\ax ${sep} ${cbad}STOPPED\ax for camp.
	/endm
	/squelch /makecamp off
	/squelch /moveto pause
	/squelch /stick off
/return

#event skillup "#*#You have become better at #1#! (#2#)"
sub Event_skillup(string line, string skillname, int skillup)
   VOUT [${cinfo}${skillname}\ax] has increased to (${cnum}${skillup}\ax)! [Max:${cnum}${Skill[${skillname}].SkillCap}\ax]
/return

#event task_update "Your task '#1#' has been updated."
sub Event_task_update(string line, string skillname)
   /bc [+g+]'[+x+][+y+]${skillname}[+x+][+g+]'[+x+] updated!
/return

#event noRaid "Your raid was disbanded."
sub Event_noRaid()
 		/if (${useSmartAssistR}) /call writeINI useSmartAssistR Common FALSE
		/if (${switchRaidInvite}) /call writeINI switchRaidInvite Common FALSE
/return

|***
	DES: This is for lazy people like me that have to invite people to raids and dont wanna do the work.
	USE:
***|
#event raidinvite             "#1# tells you#*#'#*#RI#*#'"
#event raidinvite             "#1# tells you#*#'#*#R I#*#'"
#event raidinvite             "#1# tells you#*#'#*#LFG#*#'"
#event raidinvite             "#1# tells you#*#'#*#LFR#*#'"
#event raidinvite             "#1# tells you#*#'#*#invite#*#'"

sub Event_raidinvite(string line, string _sender)
	/if (${switchRaidInvite}) {
		/if (${Bool[${Raid.Member[${_sender}]}]}) {
			VOUT ${cpc}${_sender}\ax is already in the raid.
			/return
		}
		/if (${invitelist.Find[${_sender}]}) {
			VOUT ${cbad}NO\ax invites for ${cpc}${_sender}\ax.
			/return
		}
		/declare _timeDelay ${Math.Calc[${Math.Rand[40]}+20]}
		/varset _timeDelay ${Int[${_timeDelay}]}
		/delay 1m !${Raid.Locked}
		VOUT Delay: ${cbug}${_timeDelay}\ax \ag- \ax ${cpc}${_sender}\ax requested a raid invite. [Raid: ${If[${Raid.Locked},${cbad}LOCKED\ax,${cgood}UNLOCKED\ax]}]
		/delay 1m !${Raid.Locked}
		/delay ${_timeDelay}
		/raidinvite ${_sender}

	}
/return

|***
*	DES: blurb events for raids. 
*	USE: /blurb EXPANSION EVENT
***|
#event Blurb "[MQ2] - Blurb: #1# #2#"
#event Blurb "[MQ2] - Blurb: #1#"

sub Event_Blurb(string line, string exp, string event)
	DEBUG ${cbug}Event_Blurb(\ax${exp.Upper}, ${event.Upper}${cbug})\ax
	/declare blurb 			string 	local NULL
	/declare explist 		string 	local ${Ini[${INIBlurb}]}
	/declare eventlist	string	local	NULL
	
	/if (${exp.Equal[LIST]}) {
		/echo ${explist}
		/return
	}
	/if (${explist.Find[${exp}]} && ${event.Equal[LIST]}) {
		VOUT ${Ini[${INIBlurb},${exp.Upper}]}		
	}
	/if (${explist.Find[${exp}]} && ${Bool[${event}]}) {
		/varset eventlist ${Ini[${INIBlurb},${exp.Upper}]}
		/if (${eventlist.Find[${event}]}) {
				/varset blurb ${Ini[${INIBlurb},${exp.Upper},${event.Upper}]}
				/rs ${blurb}
		}
	}
/return

|***
*	DES: Song Sets for Bards. 
*	USE: /song [melee|caster|tank|mix]
***|
#event Song "[MQ2] - Song: #1#"
sub Event_Song(string line, string _group)
	DEBUG ${cbug}Event_Song(\ax${exp.Upper}${cbug})\ax

	/if (${Select[${Me.Class.ShortName},BRD]}) {
		/call mem_song_set ${_group}
	}
/return 



|***
 *	DES: dumps all your augments to a INI file
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***| 	
#event echoaugment "[MQ2] - Echo Augment file!"
sub Event_echoaugment()
	/call echo_augments
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: this is being done away with.. slowly..
 ***| 	
#event checkOneOff "[MQ2] - Force OneOff Check!"
sub Event_checkOneOff()
	/call check1offBuffs
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***| 	
#event bribetime "[MQ2] - Bribe Time: #1# #2# #3#"
sub Event_bribetime(string line, int _amount, int _stacks, string _coin)
	DEBUG ${cbug}Event_bribetime(${_amount}, ${_stacks}, ${_coin})\ax
	/doevents flush
	/declare _coinID			int local 99

	| validate coin type
	/if (${_coin.Equal[plat]}) {
		/varset _coinID 0
	} else /if (${_coin.Equal[gold]}) {
		/varset _coinID 1
	} else /if (${_coin.Equal[silver]}) {
		/varset _coinID 2
	} else /if (${_coin.Equal[copper]}) {
		/varset _coinID 3
	} else /if (${_coinID} == 99) {
		VOUT ${ocaution} ${sep} Invalid coin type. Try again.
		/return
	}

	/if (!${Window[InventoryWindow].Open}) /invoke ${Window[InventoryWindow].DoOpen}

	/declare _count 			int local 
	/declare _countloop 	int local 1

	:loopgivemorecash
		VOUT Giving ${_coin} stack ${sep} ${_countloop} of ${_stacks}
		/if (!${Window[InventoryWindow].Open}) /return
		/if (!${Target.ID} || ${Target.ID} == (${Me.ID} || ${Pet.ID})) /return
		/delay 2s
		/for _count 1 to ${_stacks}
			AMIDEAD

			DEBUG stack count: ${_count}/${_stacks}
			/ctrl /notify InventoryWindow IW_Money${_coinID} leftmouseup
			/delay 5
			/click left target 
			/delay 5 
		/next _count 
		/delay 3 
		/notify GiveWnd GVW_Give_Button leftmouseup 

		/delay 5 
		/varset _countloop ${Math.Calc[${_countloop}+1]}
		/if (${_countloop} < ${_amount}) /goto :loopgivemorecash

	VOUT ${onotice} ${sep} Done giving cash for bribes.	How the mighty have fallen.
/return 
 
 
  
|***
 *	DES: Tells for specific things.. i guess?
 *	USE: /t uselesstoon IwantThisBuff
 *	INI: 
 *  NOTE:
 ***| 	
#event Talk "#1# tells you, '#2#'"
sub Event_Talk(string line, string _sender, string _chatText)
|**
	/declare _ignoreRelay		 	string 	local |${relayTells}|bank|${Me.Pet.DisplayName}|${Me}
	/declare _count 					int 		local
	/declare _relay						bool		local TRUE
	
	/varset _relay ${If[${_ignoreRelay.Find[${_sender}]},TRUE,]}
	/varset _relay ${If[${_ignoreRelay.Find[${_chatText}]},TRUE,]}

	| relay tells

	/echo ..
	/echo R:${_relay}

	/echo S: ${_sender}
	/echo ${If[${_sender.Find[Banker]},TRUE,FALSE]}
	/echo C: ${_chatText}
	/echo ${If[${_ignoreRelay.Find[${_chatText}]},TRUE,FALSE]}
	/echo ..
	**|
	
	
	/if (${Bool[${relayTells}]} && !${_sender.Find[Banker]} && !${_sender.Find[${Me}]} && !${_sender.Find[${relayTells}]} && !${_sender.Find[${Me.Pet.DisplayName}]} ) {
		/tell ${relayTells} ${_sender} told me: ${_chatText}
	}

	CHECKSAFELIST

	| Tells for buffs.
	/if (${switchBuffTell} && ${Spawn[${_sender}].ID} && ${_sender.NotEqual[${Me.DisplayName}]}) {
    /call cast_buffs_queue "${_sender}" "${_chatText}"
  }
  
	| Remote kill execution. CHANGE THE CORE.INI
  /if (${Bool[${remoteKillKey}]} && ${_chatText.Equal[${remoteKillKey}]}) {
  	/call set_stop FALSE
  	/call clear_combat
  	VOUT ${owarning} ${sep} Recieved tell from ${cpc}${_sender}\ax to end current macro.
  	/endm
  }
  
  | take a tell to reform your group.
  /if (${_chatText.Equal[formgroup]}) /call set_crew main
  /if (${_chatText.Equal[disbandgroup]}) /bcaa //disband
  /if (${_chatText.Equal[makemeleader]}) /makeleader ${_sender}
  /if (${_chatText.Equal[makememasterlooter]}) /delegategmasterlooter ${_sender}
  
  | raid chat
  /if (${_chatText.Equal[raidinviteme]} && ${Me.DisplayName.Equal[${Raid.Leader}]} && !${Raid.Locked}) /raidinvite ${_sender}
  /if (${_chatText.Equal[makemeraidleader]} && ${Me.DisplayName.Equal[${Raid.Leader}]} && !${Raid.Locked}) /makeraidleader ${_sender}
  /if (${_chatText.Equal[makemedzleader]}) /dzmakeleader ${_sender}
  /if (${_chatText.Equal[makemetaskleader]}) /taskmakeleader ${_sender}
  
/return



|***
 *	DES: entry point for prep here command
 *	USE: /here 
 *	INI: 
 *  NOTE: 
 ***| 	
#bind setCombatHere /setcombathere 
sub Bind_setCombatHere()
	/call set_combat_here
/return



|***
 *	DES: 
 *	USE: NA
 *	INI: NA
 *  NOTE: STILL NEEDED????
 ***| 	
#Event CantLoot "#*#may not loot this corpse#*#"
sub Event_CantLoot
   DEBUG Event_CantLoot Can't loot ${Target.DisplayName} ${Target.ID}
   | /varset CantLootID ${Target.ID}
/return



|***
 *	DES: This is a cover all for the null slot issue when swapping gear.
 *	USE: NA
 *	INI: NA
 *  NOTE: 
 ***| 	
#event NullSlot "#*#Invalid item slot 'null#*#"
sub Event_NullSlot
	VOUT ${owarning} ${sep} Opening all bags to correct null slot
	/keypress OPEN_INV_BAGS
	/timed 10 /keypress CLOSE_INV_BAGS
/return



|***
 *	DES: catch when a zone happens
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***| 
#event Zone "You have entered #*#" 
#event Zone "LOADING, PLEASE WAIT..."
sub Event_Zone(string _line)
	DEBUG ${cbug}Event_Zone(line: ${_line})\ax

	| force a delay while zoning
	/if (${switchDelayWhileZoning} && ${Me.Zoning}) {
		/while (${Me.Zoning}) {
			/delay 2s
		}	
	}

	/call validate_zone FALSE TRUE
/return 



|***
 *	DES: Buy things from vendors
 *	USE: /echo Buy: 'itemname' ##
 *	INI: 
 *  NOTE: single quotes
 ***| 
#event buystuff	"[MQ2] Buy: '#1#' #2#"
sub Event_buystuff(string line, string _itemtoBuy, int _buyAmount)
	DEBUG (string:${_itemtoBuy}, int:${_buyAmount})
	/if (${_itemtoBuy.Equal[NULL]}) {
		VOUT Need an item to buy, in single qoutes.
	} else /if (${_buyAmount} < 1) {
		VOUT Need a valid quantity. 
	} else {
		/call vendor_buy "${_itemtoBuy}" ${_buyAmount}
	}
/return



|***
 *	DES: Buy things from vendors
 *	USE: /echo Sell: 'itemname' or loot
 *	INI: 
 *  NOTE: single quotes.. yes, the item name has to be in single quotes
 ***| 
#event sellstuff	"[MQ2] Sell: '#1#'"
sub Event_sellstuff(string line, string _itemtoSell)
	DEBUG ${cbug}Event_sellstuff(string: ${_itemtoSell})\ax
	/call vendor_sell "${_itemtoSell}"
/return



|***
 *	DES: ocd shit
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***| 	
#event clean "[MQ2] - Clean this mess up."
sub Event_clean()
	DEBUG ${cbug}Event_clean()\ax
	/if (${Cursor.ID}) /call sort_cursor TRUE
	/if (${hideAll}) /hidecorpse all
	/if (${hideAlways}) /hidecorpse always
	/if (${hideLooted}) /hidecorpse looted
	/if (${hideAllButGroup}) /hidecorpse allbutgroup
	/if (${hideNPC}) /hidecorpse npc
	/if (${hideListed}) /hidecorpse listed
	/if (${hideListedUnlocked}) /hidecorpse listedunlocked  
/return



|***
 *	DES: rez emote
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***| 	
#event resurrection_exp_gain "You regain some experience from resurrection."
sub Event_resurrection_exp_gain()
	DEBUGREZ delays for rez status to clear
	/delay 2s !${Me.State.Equal[HOVER]}
	/delay 2s !${Me.Hovering}
	/delay 2s !${Me.Dead}
	DEBUGREZ clear HUD tmpMode variable
	/varset tmpMode FALSE
	DEBUGREZ Exit rez, clear event marker
	/call set_stop TRUE
	/call clear_combat
	/varset switchAmIDead FALSE
/return



|***
 *	DES: force self item buffing
 *	USE: /ib
 *	INI: 
 *  NOTE:
 ***| 	
#bind itembuffs /ib
sub Bind_itembuffs()
	/call check_buffs_item
	/if (${Select[${Me.Class.ShortName},ENC]}) /call check_crystals
	/if (${Select[${Me.Class.ShortName},MAG]}) /call Bind_setMAG rods
/return



|***
 *	DES: force self spell buffing
 *	USE: /sb
 *	INI: 
 *  NOTE:
 ***| 	
#bind selfbuffs /sb
sub Bind_selfbuffs()
	/call check_buffs_self
	/if (${Select[${Me.Class.ShortName},SHD,PAL]}) /call check_harmonious
	/if (${Select[${Me.Class.ShortName},WIZ,ENC]}) /call check_rune_${Me.Class.ShortName}
	/if (${Bool[${AANow1}]} || ${Bool[${AANow2}]}) /call check_AA_now
	/if (${Bool[${DiscNow1}]} || ${Bool[${DiscNow2}]}) /call check_Disc_now
	/if (${Bool[${Aura1Spell}]} || ${Bool[${Aura2Spell}]}) /call check_aura
/return



|***
 *	DES: alternate to /bc stop. self only
 *	USE: /alto
 *	INI: 
 *  NOTE:
 ***| 	
#bind setstop /alto
sub Bind_setstop()
	/call set_stop FALSE
	/call clear_combat
/return



|***
 *	DES: stack invis
 *	USE: /invis
 *	INI: 
 *  NOTE:
 ***| 
#bind cast_AA_invis /invis
sub Bind_cast_AA_invis()
	/if (${Me.AltAbilityReady[Shauri's Sonorous Clouding]} && ${Select[${Me.Class.ShortName},BRD]}) {
		/call pulse_stop
		/call MQ2Cast "Shauri's Sonorous Clouding" alt -targetid|${Me.ID}
		ECHOCHANNEL AA: Shauri's Sonorous Clouding
		/return
	}
	/if (${Me.AltAbilityReady[Group Perfected Invisibility]} && ${Select[${Me.Class.ShortName},WIZ,MAG,ENC]}) {
		/call MQ2Cast "Group Perfected Invisibility" alt -targetid|${Me.ID}
		ECHOCHANNEL AA: Group Perfected Invisibility
		/return
	}  
	/if (${Me.AltAbilityReady[Group Perfected Invisibility to Undead]} && ${Select[${Me.Class.ShortName},CLR,SHD,NEC,PAL]}) {
		/delay 3
		/call MQ2Cast "Group Perfected Invisibility to Undead" alt -targetid|${Me.ID}
		ECHOCHANNEL AA: Group Perfected Invisibility to Undead
		/return
	}
	/if (${Me.AltAbilityReady[Group Silent Presence]} && ${Select[${Me.Class.ShortName},SHM]}) {
		/delay 5
		/call MQ2Cast "Group Silent Presence" alt -targetid|${Me.ID}
		ECHOCHANNEL AA: Group Silent Presence
		/return
	}	
/return		



|***
 *	DES: use primary anchor.
 *	USE: /primary
 *	INI: 
 *  NOTE:
 ***| 
#bind click_primary_anchor /primary
sub Bind_click_primary_anchor()
	/if (${Me.ItemReady[Primary Anchor Transport Device]}) {
		/if (${Me.Invis}) /makemevisible
		/if (${Me.Casting.ID}) /stopcast
		/call core_cast "Primary Anchor Transport Device" item 0 FALSE
	}
/return



|***
 *	DES: use secondary anchor
 *	USE: /secondary
 *	INI: 
 *  NOTE:
 ***| 
#bind click_secondary_anchor /secondary
sub Bind_click_secondary_anchor()
	/if (${Me.ItemReady[Secondary Anchor Transport Device]}) {
		/if (${Me.Invis}) /makemevisible
		/if (${Me.Casting.ID}) /stopcast
		/call core_cast "Secondary Anchor Transport Device" item 0 FALSE
	}
/return



|***
 *	DES: keeps an eye out for a mob
 *	USE: /watchspawn
 *	INI: 
 *  NOTE:
 ***| 
#bind watchspawn /watchspawn
sub Bind_watchspawn(string _spawn)
	/if (${Bool[${_spawn}]}) {
		/call writeINI watchSpawn Common "${_spawn}"
	} else /if (!${Bool[${_spawn}]} && ${Target.ID}) {
		/call writeINI watchSpawn Common "${Target.DisplayName}"
	} else /if (!${Bool[${_spawn}]} && !${Target.ID}) {
		VOUT ${sep} ${cinfo}/watchspawn\ax requires a target or mob partial name. 
		VOUT ${sep} NPC and CORPSE is also acceptable.
		/if (!${Bool[${_spawn}]}) /call writeINI watchSpawn Common FALSE
	}
	
	/if (${_spawn.Equal[FALSE]}) /mapshow reset
/return
  
  

|***
 *	DES: enable override
 *	USE: /override []
 *	INI: NA
 *  NOTE: starts/ends overrides
 ***|
#bind override /override
sub Bind_override(string _overrideType)
	DEBUG ${cbug}Bind_override(type: ${_overrideType})\ax
	/declare _count		int local
	/declare _bind string local override
	
	
	| override class checks. not documented.
	/if (${_overrideType.Equal[classcheck]}) {
		/varset switchOverrideClasscheck TRUE
		OUT ${owarning} Overriding class checks.
		/return
	}

	| get help
	/if (${${_bind}Type.Find[${_${_bind}Type}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_${_bind}Type.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_${_bind}Type.Lower}]}
		/return
	}	else /if (${_${_bind}Type.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Arg[${_count},|]}]}
		/next _count
		/return
	}	
	/if (${Bool[${overrideType.Find[${_overrideType}]}]} && ${_overrideType.NotEqual[stop]}) {
		| /echo switchOverride${_overrideType.Left[1].Upper}${_overrideType.Right[-1].Lower}
		/if (${Defined[switchOverride${_overrideType.Left[1].Upper}${_overrideType.Right[-1].Lower}]}) {
			/call writeINI switchOverride${_overrideType.Left[1].Upper}${_overrideType.Right[-1].Lower} Override ${If[${switchOverride${_overrideType.Left[1].Upper}${_overrideType.Right[-1].Lower}},FALSE,TRUE]}  
		} 
	} else /if (${_overrideType.Equal[stop]}) {
		/for _count 1 to ${overrideType.Count[|]}
			/if (${switchOverride${overrideType.Arg[${_count},|].Left[1].Upper}${overrideType.Arg[${_count},|].Right[-1].Lower}}) {
				/call writeINI switchOverride${overrideType.Arg[${_count},|].Left[1].Upper}${overrideType.Arg[${_count},|].Right[-1].Lower} Override FALSE  
			}
		/next _count
	}
	/declare _listout string local
	/declare _sep bool local FALSE
	/for _count 1 to ${overrideType.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${switchOverride${overrideType.Arg[${_count},|].Left[1].Upper}${overrideType.Arg[${_count},|].Right[-1].Lower}},${cgood},${cinfo}]}${overrideType.Arg[${_count},|]}\ax
		/varset _sep TRUE
	/next _count
	VOUT /override [${_listout}]		
/return  



|***
 *	DES: set various debuff things
 *	USE: /debuff []
 *	INI: 
 *  NOTE:
 ***| 	
#bind deBuff /debuff
sub Bind_deBuff(string _deBuffType, string _verbage)
	DEBUG ${cbug}Bind_deBuff(type: ${_deBuffType})\ax
	/declare _count int local 0
	/declare _bind string local deBuff
	/declare _listout string local
	/declare _sep bool local FALSE

	| get help
	/if (${${_bind}Type.Find[${_${_bind}Type}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_${_bind}Type.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_${_bind}Type.Lower}]}
		/return
	}	else /if (${_${_bind}Type.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Arg[${_count},|]}]}
		/next _count
		/return
	}
	
	| can i do this debuff?
	/if (${Bool[${_deBuffType.Equal[mezz]}]}) {
		/if (!${validate_debuff[TRUE, mezz]}) /return
	} else /if (${Bool[${_deBuffType.Equal[malo]}]}) {
		/if (!${validate_debuff[TRUE, malo]}) /return
	} else /if (${Bool[${_deBuffType.Equal[tash]}]}) {
		/if (!${validate_debuff[TRUE, tash]}) /return
	} else /if (${Bool[${_deBuffType.Equal[cripple]}]}) {
		/if (!${validate_debuff[TRUE, cripple]}) /return
	} else /if (${Bool[${_deBuffType.Equal[slow]}]}) {
		/if (!${validate_debuff[TRUE, slow]}) /return
	} else /if (${Bool[${_deBuffType.Equal[eradicate]}]}) {
		/if (!${validate_debuff[TRUE, eradicate]}) /return
	} else /if (${Bool[${_deBuffType.Equal[snare]}]}) {
		/if (!${validate_debuff[TRUE, snare]}) /return
	}
								
	/if (${Bool[${deBuffType.Find[${_deBuffType}]}]} && ${_deBuffType.NotEqual[stop]}) {
		/if (${Defined[switchdeBuff${_deBuffType.Left[1].Upper}${_deBuffType.Right[-1].Lower}]}) {
			/call writeINI switchdeBuff${_deBuffType.Left[1].Upper}${_deBuffType.Right[-1].Lower} Buffs ${If[${switchdeBuff${_deBuffType.Left[1].Upper}${_deBuffType.Right[-1].Lower}},FALSE,TRUE]}  
		}
	} else /if (${Bool[${deBuffType.Equal[${_deBuffType}]}]} && ${_deBuffType.Equal[stop]}) {
		/for _count 1 to ${deBuffType.Count[|]}
			/if (${switchdeBuff${deBuffType.Arg[${_count},|].Left[1].Upper}${deBuffType.Arg[${_count},|].Right[-1].Lower}}) {
				/call writeINI switchdeBuff${deBuffType.Arg[${_count},|].Left[1].Upper}${deBuffType.Arg[${_count},|].Right[-1].Lower} Buffs FALSE  
			}
		/next _count
	} 
	/for _count 1 to ${deBuffType.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${switchdeBuff${deBuffType.Arg[${_count},|].Left[1].Upper}${deBuffType.Arg[${_count},|].Right[-1].Lower}},${cgood},${cinfo}]}${deBuffType.Arg[${_count},|]}\ax
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]			


	| clear stickbehind. it will force bad targeting fot the mezzer.
	/if (${SetStickBehind} && ${switchdeBuffMezz}) {
		/call writeINI SetStickBehind Movement FALSE  
		VOUT /${_bind} behind ${sep} ${SetStickBehind}
	}
/return



|***
 *	DES: set various heal switches
 *	USE: /heal []
 *	INI: 
 *  NOTE:
 ***| 	
#bind heal /heal
sub Bind_heal(string _healType, string _verbage)
	DEBUG ${cbug}Bind_heal(type: ${_healType})\ax
	/declare _count		int local
	/declare _bind string local heal
	/declare _listout string local
	/declare _sep bool local

	| get help
	/if (${${_bind}Type.Find[${_${_bind}Type}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_${_bind}Type.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_${_bind}Type.Lower}]}
		/return
	}	else /if (${_${_bind}Type.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Arg[${_count},|]}]}
		/next _count
		/return
	}
	
	| can I do this heal
	/if (${Bool[${_healType.Equal[self]}]}) {
		/if (${validate_class[FALSE, |CLR|SHM|DRU]}) {
			VOUT Heal Classes [\a-wCLR,DRU,SHM\ax] use /heal group. Not /heal self.
			/call writeINI switchHealSelf Heal FALSE
			/return
		}
	} else /if (${Bool[${_healType.Equal[group]}]}) {
		/if (!${validate_class[TRUE, ${HealerList}]}) /return
	} else /if (${Bool[${_healType.Equal[xtarget]}]}) {
		/if (!${validate_class[TRUE, ${HealerList}]}) /return
	} else /if (${Bool[${_healType.Equal[pet]}]}) {
		/if (!${validate_class[TRUE, |CLR|DRU|SHM|MAG|BST|NEC]}) /return
	}
				
	/if (${Bool[${healType.Find[${_healType}]}]} && ${_healType.NotEqual[stop]} && ${_healType.NotEqual[override]}) {
		/if (${Defined[switchHeal${_healType.Left[1].Upper}${_healType.Right[-1].Lower}]}) {
			/call writeINI switchHeal${_healType.Left[1].Upper}${_healType.Right[-1].Lower} Heal ${If[${switchHeal${_healType.Left[1].Upper}${_healType.Right[-1].Lower}},FALSE,TRUE]}  
		} 
		
	| stop all healing
	} else /if (${_healType.Equal[stop]}) {
		/for _count 1 to ${healType.Count[|]}
			/if (${switchHeal${healType.Arg[${_count},|].Left[1].Upper}${healType.Arg[${_count},|].Right[-1].Lower}}) {
				/call writeINI switchHeal${healType.Arg[${_count},|].Left[1].Upper}${healType.Arg[${_count},|].Right[-1].Lower} Heal FALSE  
			}
		/next _count


	| heal xtarget correction
	} else /if (${_healType.Equal[xtarget]}) {
		/if (!${validate_expansion[TRUE, |16]}) {
			/call writeINI switchHealXtarget Heal FALSE 
			/return
		}
		
	| heal override
	} else /if (${_healType.Equal[override]}) {
		/if (${Range.Between[0,99:${Int[${_verbage}]}]}) {
			/call writeINI HealOverride Heal ${_verbage} 
			VOUT /${_bind} override ${sep} ${If[${_verbage},${_verbage},${odisabled}]}
		} else {
			VOUT /${_bind} override(${cnum}${HealOverride}\ax) allowable range ${cinfo}0 - 99\ax
			/return
		}
		/return
	}

	/for _count 1 to ${healType.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${switchHeal${healType.Arg[${_count},|].Left[1].Upper}${healType.Arg[${_count},|].Right[-1].Lower}},${cgood},${cinfo}]}${healType.Arg[${_count},|]}\ax
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]				
/return
	


|***
 *	DES: set various buff switches
 *	USE: /buff []
 *	INI: 
 *  NOTE:
 ***| 	
#bind buff /buff
sub Bind_buff(string _buffType, string _verbage)
	DEBUG ${cbug}Bind_buff(type: ${_buffType})\ax
	/declare _count int local 0
	/declare _bind string local buff
	/declare _listout string local
	/declare _sep bool local

	| get help
	/if (${${_bind}Type.Find[${_${_bind}Type}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_${_bind}Type.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_${_bind}Type.Lower}]}
		/return
	}	else /if (${_${_bind}Type.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Arg[${_count},|]}]}
		/next _count
		/return
	}
	
	| stop	
	/if (${_buffType.Equal[stop]}) {
		/for _count 1 to ${buffType.Count[|]}
			/if (${switchBuff${buffType.Arg[${_count},|].Left[1].Upper}${buffType.Arg[${_count},|].Right[-1].Lower}}) {
				/call writeINI switchBuff${buffType.Arg[${_count},|].Left[1].Upper}${buffType.Arg[${_count},|].Right[-1].Lower} Buffs FALSE  
			}
		/next _count
	| self
	} else /if (${_buffType.Equal[self]}) {
		/call writeINI switchBuffSelf Buffs ${If[${switchBuffSelf},FALSE,TRUE]}
	| item
	} else /if (${_buffType.Equal[item]}) {
		/call writeINI switchBuffItem Buffs ${If[${switchBuffItem},FALSE,TRUE]}
	| combat
	} else /if (${_buffType.Equal[combat]}) {
		/call writeINI switchBuffCombat Buffs ${If[${switchBuffCombat},FALSE,TRUE]}
	| bc
	} else /if (${_buffType.Equal[bc]}) {
		/call writeINI switchBuffBc Buffs ${If[${switchBuffBc},FALSE,TRUE]}
	| raid
	} else /if (${_buffType.Equal[raid]}) {
		/call writeINI switchBuffRaid Buffs ${If[${switchBuffRaid},FALSE,TRUE]}
	| mercenaries
	} else /if (${_buffType.Equal[merc]}) {
		/call writeINI switchBuffMerc Buffs ${If[${switchBuffMerc},FALSE,TRUE]}
	| tell
	} else /if (${_buffType.Equal[tell]}) {
		/call writeINI switchBuffTell Buffs ${If[${switchBuffTell},FALSE,TRUE]}
	| pet
	} else /if (${_buffType.Equal[pet]}) {
		/if (!${validate_class[TRUE, ${PetList}]}) {
			/call writeINI switchBuffPet Pet FALSE
			/return
		}
		/call writeINI switchBuffPet Pet ${If[${switchBuffPet},FALSE,TRUE]}
	}
	/for _count 1 to ${buffType.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${switchBuff${buffType.Arg[${_count},|].Left[1].Upper}${buffType.Arg[${_count},|].Right[-1].Lower}},${cgood},${cinfo}]}${buffType.Arg[${_count},|]}\ax
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]				
/return



|***
 *	DES: cursor control
 *	USE: /cursor []
 *	INI: 
 *  NOTE: 
 ***| 
#bind cursor /cursor
sub Bind_cursor(string _cursorType, string _verbage)
	DEBUG ${cbug}Bind_cursor(type: ${_cursorType})\ax
	/declare _properType 	string 	local
	/declare _count 			int 		local 0
	/declare _bind 				string 	local cursor
	/declare _passto 			string 	local NULL

	| get help
	/if (${${_bind}Type.Find[${_${_bind}Type}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_${_bind}Type.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_${_bind}Type.Lower}]}
		/return
	}	else /if (${_${_bind}Type.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Arg[${_count},|]}]}
		/next _count
		/return
	}
		
	| set cursot ro auto enable/disable
	/if (${_cursorType.Equal[auto]}) {
		/call writeINI switchAutoCursor Common ${If[${switchAutoCursor},FALSE,TRUE]}  
		VOUT /cursor ${_cursorType.Lower} ${sep} ${If[${switchAutoCursor},${oenabled},${odisabled}]}
		/if (${timer_auto_cursor_pause}) /varset timer_auto_cursor_pause 1

	| pause cursor controlls
	} else /if (${_cursorType.Equal[pause]}) {
		/call create_timer timer_auto_cursor_pause ${If[${Bool[${_verbage}]},${_verbage},30s]}
		VOUT /cursor ${_cursorType.Lower} ${sep} ${cinfo}${If[${Bool[${_verbage}]},${_verbage},30s]}\ax


	| set item to be passed to another
	} else /if (${Cursor.ID} && ${_cursorType.Equal[pass]}) {

		/if (${Bool[${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}].Arg[1,|]}]}) {
			/varset _properType ${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}].Arg[1,|]}
		} else {
			/varset _properType Keep
		}

		| /varset _properType ${_cursorType.Left[1].Upper}${_cursorType.Right[-1].Lower}
	
		/if (${Bool[${_verbage}]}) {
			/varset _passto ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		}

		/ini "${INILoot}" "${Cursor.Name.Left[1]}" "${Cursor.Name}" |${_properType}|${_passto}

		VOUT /cursor ${_cursorType.Lower} ${sep} \a-w${Cursor.Name}\ax=${cinfo}${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}]}\ax
		






	| echo what the item is set too
	} else /if (${Cursor.ID} && ${Bool[${cursorType.Find[${_cursorType}]}]} && ${_cursorType.Equal[what]}) {
		VOUT /cursor ${_cursorType.Lower} ${sep} \a-w${Cursor.Name}\ax=${cinfo}${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}]}\ax

	| remainder of the commands
	} else /if (${Cursor.ID} && ${Bool[${cursorType.Find[${_cursorType}]}]} && ${_cursorType.NotEqual[what]} && ${_cursorType.NotEqual[auto]}) {
		/varset _properType ${_cursorType.Left[1].Upper}${_cursorType.Right[-1].Lower}
		/if (${Bool[${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}].Arg[2,|]}]}) {
			/varset _passto ${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}].Arg[2,|]}
		}
		/ini "${INILoot}" "${Cursor.Name.Left[1]}" "${Cursor.Name}" |${_properType}${If[${Bool[${_passto}]},|${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}].Arg[2,|]},]}
		VOUT /cursor ${_cursorType.Lower} ${sep} \a-w${Cursor.Name}\ax=${cinfo}${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}]}\ax
		/if (${Cursor.ID} && ${_cursorType.Equal[Destroy]}) /destroy

	} else {	
		/call echos listtype cursor cursor 0 0 0
	}

/return



|***
 *	DES: set rez controls
 *	USE: /rez []
 *	INI: 
 *  NOTE:
 ***| 	
#bind rez /rez
sub Bind_rez(string _rezType, string _verbage)
	DEBUG ${cbug}Bind_rez(type: ${_rezType}, pct: ${_verbage})\ax
	/declare _count int local 0
	/declare _listout string local
	/declare _sep bool local FALSE
	/declare _bind string local rez

	| get help
	/if (${${_bind}Type.Find[${_${_bind}Type}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_${_bind}Type.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_${_bind}Type.Lower}]}
		/return
	}	else /if (${_${_bind}Type.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Arg[${_count},|]}]}
		/next _count
		/return
	}
		
	/if (${Bool[${rezType.Find[${_rezType}]}]} && ${_rezType.NotEqual[stop]} && ${_rezType.NotEqual[pct]} && ${_rezType.NotEqual[range]}) {
		/if (${Defined[switchRez${_rezType.Left[1].Upper}${_rezType.Right[-1].Lower}]}) {
			/call writeINI switchRez${_rezType.Left[1].Upper}${_rezType.Right[-1].Lower} Heal ${If[${switchRez${_rezType.Left[1].Upper}${_rezType.Right[-1].Lower}},FALSE,TRUE]}
		} 
	
	| turn off all rez controls	
	} else /if (${_rezType.Equal[stop]}) {
		/for _count 1 to ${rezType.Count[|]}
			DEBUG switchRez${rezType.Arg[${_count},|].Left[1].Upper}${rezType.Arg[${_count},|].Right[-1].Lower}
			/if (${switchRez${rezType.Arg[${_count},|].Left[1].Upper}${rezType.Arg[${_count},|].Right[-1].Lower}}) {
				/call writeINI switchRez${rezType.Arg[${_count},|].Left[1].Upper}${rezType.Arg[${_count},|].Right[-1].Lower} Heal FALSE
			}
		/next _count
		
	| set rez accept percent	
	} else /if (${_rezType.Equal[pct]}) {
		/if (${Range.Between[10,96:${Int[${_verbage}]}]}) {
			/if (${rezPct.Find[${_verbage}]}) /call writeINI minRezPct Common ${_verbage}
		}
		/varset _sep FALSE
		/for _count 1 to ${rezPct.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${minRezPct} == ${rezPct.Arg[${_count},|]},${cgood},${cinfo}]}${rezPct.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} pct [${_listout}]
		/return
	} else /if (${_rezType.Equal[range]}) {
		/if (${Range.Between[1,110:${Int[${_verbage}]}]}) {
			/call writeINI maxRezRange Heal ${_verbage}
		} else {
			VOUT /${_bind} range(${cnum}${maxRezRange}\ax) allowable range is ${cinfo}1 - 110\ax
			/return
		}
	} 

	/varset _sep FALSE
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${switchRez${${_bind}Type.Arg[${_count},|].Left[1].Upper}${${_bind}Type.Arg[${_count},|].Right[-1].Lower}},${cgood},${cinfo}]}${${_bind}Type.Arg[${_count},|]}\ax
		/if (${${_bind}Type.Arg[${_count},|].Equal[pct]}) /varset _listout ${_listout}(${cnum}${minRezPct}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[range]}) /varset _listout ${_listout}(${cnum}${maxRezRange}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /rez [${_listout}]	
/return




|***
 *	DES: list control
 *	USE: /lc []
 *	INI: core.zone.ini
 *  NOTE: 
 ***| 
#bind listControl /lc
sub Bind_listControl(string _list, string _verbage)
	DEBUG ${cbug}Bind_listControl (control: ${_list}, verbage: ${_verbage})\ax
	/declare _properType 			string 	local
	/declare _count 					int 		local 0
	/declare _listchanged 		bool 		local FALSE
	/declare _validName				bool 		local FALSE
	/declare _bind 						string 	local lc
	

	| get help
	/if (${listType.Find[${_list}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_list.Lower}\ax ${sep} ${Ini[${INIWiki},lc,${_list.Lower}]}
		/return
	}	else /if (${_list.Equal[info]}) {
		/for _count 1 to ${listType.Count[|]}
			VOUT /${_bind} ${cinfo}${listType.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},lc,${listType.Arg[${_count},|]}]}
		/next _count
		/return
	}

	| make sure we are using the correct zone file
	/call validate_zone FALSE TRUE
	
	| disable all commands but safezone while in a safezone
	/if (${SafeZone} && ${_list.NotEqual[safe]} && ${_list.NotEqual[reset]}) {
		VOUT ${onotice} ${sep} List Control is disabled while in a Safe Zone.
		/return
	}

	| require a target. one that is not a PC
	/if ((!${Target.ID} || ${Bool[${Spawn[pc ID ${Target.ID}]}]}) && ${_list.NotEqual[safe]} && ${_list.NotEqual[reset]}) {
		DEBUG NO /${_bind} Target
		/call echos needtarget 0 0 0 0
		/return
	} 

	| validate _list and correct the case
	/if (${Bool[${_list}]} && ${_list.NotEqual[safe]} && ${_list.NotEqual[reset]}) {
		/for _count 1 to ${listType.Count[|]}
			DEBUG Match: ${listType.Arg[${_count},|]} ${If[${Bool[${_list.Find[${listType.Arg[${_count},|]}]}]},${cgood}==\ax,${cbad}!=\ax]} ${_list}
			
			/if (${Bool[${_list.Find[${listType.Arg[${_count},|]}]}]}) {
				/if (${_list.Left[2].Equal[no]}) {
					DEBUG ${_list.Left[2].Left[1].Upper}${_list.Left[2].Right[-1].Lower}${_list.Right[-2].Left[1].Upper}${_list.Right[-2].Right[-1].Lower}
					/varset _list ${_list.Left[2].Left[1].Upper}${_list.Left[2].Right[-1].Lower}${_list.Right[-2].Left[1].Upper}${_list.Right[-2].Right[-1].Lower}
				}	else /if (${_list.Left[4].Equal[hunt]}) {
					/varset _list ${_list.Left[4].Left[1].Upper}${_list.Left[4].Right[-1].Lower}${_list.Right[3].Left[1].Upper}${_list.Right[3].Right[-1].Lower}
				} else /if (${_list.Left[2].NotEqual[no]}) {
					/varset _list ${_list.Left[1].Upper}${_list.Right[-1].Lower}
				}
				/varset _validName TRUE
				DEBUG ProperList: ${cinfo}${_list}\ax
				/goto :validname
			}	
		/next _count
		/if (!${_validName}) /call echos listtype lc list 0 0
	} else /if (!${Bool[${_list}]} && ${_list.NotEqual[safe]} && ${_list.NotEqual[reset]}) {
		/call echos listtype lc list 0 0
	}

	:validname
	DEBUG :validname
	/if (${_list.Equal[safe]}) {
		DEBUG SAFE zone adjustment
		/if (${Ini[${INIZone},"${Zone.ShortName}","SafeZone"]}) {
			/ini "${INIZone}" "${Zone.ShortName}" "SafeZone" "FALSE"
		} else /if (!${Ini[${INIZone},"${Zone.ShortName}","SafeZone"]}) {
			/ini "${INIZone}" "${Zone.ShortName}" "SafeZone" "TRUE"
		}
		/varset SafeZone ${Ini[${INIZone},"${Zone.ShortName}","SafeZone"]}
		VOUT /${_bind} ${_list} ${sep} ${If[${SafeZone},${oenabled},${odisabled}]}
		/varset _listchanged TRUE
	} else /if (${_list.Equal[reset]}) {
		DEBUG Reset Zone
		VOUT /${_bind} ${_list} ${sep} ${cinfo}${Zone.Name}\ax
		/varset _listchanged TRUE
		| /call check_zone_INI ${Me.DisplayName}
	} else /if (${_list.Equal[remove]}) {
		DEBUG remove from INI
		/declare _mobName string local |${Target.DisplayName}
		VOUT /${_bind} ${_list} ${sep} ${ctar}${Target.DisplayName}\ax

		/declare _listName 			string local	
		/declare _INIlist				string local ${ZoneFileList}
		/declare _a 						int local
		/declare _b 						int local
		/declare _strLeft 			string local
		/declare _strRight 			string local
		
		/for _count 1 to ${_INIlist.Count[|]}
			/varset _listName ${_INIlist.Arg[${_count},|]}
			/varset _strLeft FALSE
			/varset _strRight FALSE
			/if (${${_listName}.Find[${_mobName}]}) {
				/varcalc _a ${${_listName}.Find[${_mobName}]}-1
				/varcalc _b ${${_listName}.Length}-${_mobName.Length}-${_a}
				/if (${_a} > 0) /varset _strLeft ${${_listName}.Left[${_a}]}
				/if (${_a} >= 0) /varset _strRight ${If[${Select[${_b},NULL,FALSE,""]},,${${_listName}.Right[${_b}]}]} 
				/ini "${INIZone}" "${Zone.ShortName}" "${_listName}" "${If[${Bool[${_strLeft}]},${_strLeft},]}${If[${Bool[${_strRight}]},${_strRight},]}"
			}
		/next _count	
		/varset _listchanged TRUE

	} else /if (${Bool[${listType.Find[${_list}]}]} && ${_list.NotEqual[remove]} && ${_list.NotEqual[safe]} && ${_list.NotEqual[reset]}) {
		DEBUG Exists
		/if (${${_list}List.Find[${Target.DisplayName}]}) {
			VOUT ${onotice} ${sep} [${ctar}${Target.DisplayName}\ax] already exists in ${sep} ${cinfo}${Zone.Name}\ax
		} else /if (!${${_list}List.Find[${Target.DisplayName}]}) {
			DEBUG Adding |${Target.DisplayName} to ${${_list}List}
			VOUT List ${_list} ${sep} ${ctar}${Target.DisplayName}\ax ${sep} ${cinfo}${Zone.Name}\ax
			/if (${Bool[${${_list}List}]}) /ini "${INIZone}" "${Zone.ShortName}" "${_list}List" "${${_list}List}|${Target.DisplayName}"
			/if (!${Bool[${${_list}List}]}) /ini "${INIZone}" "${Zone.ShortName}" "${_list}List" "|${Target.DisplayName}"
			/varset _listchanged TRUE
		}
	} else /if (${Bool[${_list}]}) {
		/call echos listtype lc list 0 0
	}

	| do we need to reset the zone
	/if (${_listchanged}) {
		/if (${Bool[${Plugin[MQ2EQBC]}]} && ${EQBC.Connected}) {
			/bc zonereset
		} else {
			/zonereset
		}
	}
/return



|***
 *	DES: debug switches
 *	USE: /debug []
 *	INI: NA
 *  NOTE: starts/ends debugs
 ***|
#bind debug /debug
sub Bind_debug(string _debugType, string _verbage)
	DEBUG ${cbug}Bind_debug(Debug: ${_debugType})\ax
	/declare _count int local 0
	/declare _bind string local debug
	/declare _listout string local
	/declare _sep bool local FALSE

	| get help
	/if (${${_bind}Type.Find[${_${_bind}Type}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_${_bind}Type.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_${_bind}Type.Lower}]}
		/return
	}	else /if (${_${_bind}Type.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Arg[${_count},|]}]}
		/next _count
		/return
	}
			
	/if (${Bool[${${_bind}Type.Find[${_${_bind}Type}]}]} && ${_${_bind}Type.NotEqual[stop]}) {
		/if (${Defined[switchDebug${_${_bind}Type.Left[1].Upper}${_${_bind}Type.Right[-1].Lower}]}) {
			/call writeINI switchDebug${_${_bind}Type.Left[1].Upper}${_${_bind}Type.Right[-1].Lower} ${_bind} ${If[${switchDebug${_${_bind}Type.Left[1].Upper}${_${_bind}Type.Right[-1].Lower}},FALSE,TRUE]}  
		} 
	} else /if (${_${_bind}Type.Equal[stop]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			/if (${switchDebug${${_bind}Type.Arg[${_count},|].Left[1].Upper}${${_bind}Type.Arg[${_count},|].Right[-1].Lower}}) {
				/call writeINI switchDebug${${_bind}Type.Arg[${_count},|].Left[1].Upper}${${_bind}Type.Arg[${_count},|].Right[-1].Lower} debug FALSE
			}
		/next _count	
	}
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${switchDebug${${_bind}Type.Arg[${_count},|].Left[1].Upper}${${_bind}Type.Arg[${_count},|].Right[-1].Lower}},${cgood},${cinfo}]}${${_bind}Type.Arg[${_count},|]}\ax
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]		
/return  



|***
 *	DES: pet control
 *	USE: /pc []
 *	INI: NA
 *  NOTE:
 ***|
#bind petControl /pc
sub Bind_petControl(string _pcType, string _verbage)
	DEBUG ${cbug}Bind_petControl(type: ${_pcType}, #: ${_verbage})\ax

	/declare _count int local 0
	/declare _bind string local pc
	/declare _listout string local
	/declare _sep bool local FALSE

	| get help
	/if (${${_bind}Type.Find[${_${_bind}Type}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_${_bind}Type.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_${_bind}Type.Lower}]}
		/return
	}	else /if (${_${_bind}Type.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Arg[${_count},|]}]}
		/next _count
		/return
	}
	| build and buff
	/if (${Bool[${_pcType.Find[build]}]}) {
		/if (!${validate_class[TRUE, ${PetList}]}) /return
		/call writeINI switchPcPet Pet TRUE
		/call check_pet
		/if (${Me.Pet.ID}) /call check_pet_buffs	

	| pet/engage#
	} else /if (${Bool[${_pcType.Equal[pet]}]}) {
		/if (!${validate_class[TRUE, ${PetList}]}) /return
		/if (!${Bool[${_verbage}]}) {
			/call writeINI switchPcPet Pet ${If[${switchPcPet},FALSE,TRUE]}
			/call echos switch "Use Pet" switchPcPet 0 0
			/if (!${switchPcPet} && ${Pet.ID}) {
				/if (${Me.AltAbilityReady[Suspended Minion]}) {
					/call core_cast "Suspended Minion" alt 0 FALSE
				} else /if (${Me.AltAbilityReady[Companion's Suspension]}) {
					/call core_cast "Companion's Suspension" alt 0 FALSE
				} else {
					/pet leave
				}
			}
		} else /if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI PcPetengageat Pet ${_verbage}
			/call echos number "Pet Engage" ${_verbage} 0 0
		} else /if (!${Bool[${_pcNum}]} || !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} pet(${cnum}${PcPetengageat}\ax] valid range ${cinfo}1 - 99\ax
		}
		
	| swarm/engage#
	} else /if (${Bool[${_pcType.Equal[swarm]}]}) {
		/if (!${Bool[${_verbage}]}) {
			/call writeINI switchPcSwarm Pet ${If[${switchPcSwarm},FALSE,TRUE]}
			/call echos switch "Use Pet Swarm" switchPcSwarm 0 0
		} else /if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {			
			/call writeINI PcSwarmengageat Pet ${_verbage}
			/call echos number "Swarm Pet Engage" ${_verbage} 0 0
		} else /if (!${Bool[${_verbage}]} || !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} swarm(${cnum}${PcSwarmengageat}\ax] valid range ${cinfo}1 - 99\ax
		}
		
	| pc tank
	} else /if (${Bool[${_pcType.Equal[tank]}]}) {
		/if (!${validate_class[TRUE, ${PetList}]}) /return
		/call writeINI switchPcPettank Pet ${If[${switchPcPettank},FALSE,TRUE]}
		/call echos switch "Pet Tank" switchPcPettank	0 0
		
	| pc type
	} else /if (${Bool[${_pcType.Equal[type]}]}) {
		/if (!${validate_class[TRUE, ${PetList}]}) /return
		/call writeINI PcPettype Pet ${_verbage}
		
	| pc bag ##
	} else /if (${Bool[${_pcType.Equal[bag]}]}) {
		/if (!${validate_class[TRUE, ${PetList}]}) /return
		/if (${Range.Between[1,10:${Int[${_verbage}]}]}) {
			/call writeINI PetBagSlotNumber Pet ${_verbage}
			| /call echos number "Pet Weapons Slot" ${_verbage} 0 0
		} else {
			VOUT /${_bind} bag(${cnum}${PetBagSlotNumber}\ax] valid range range ${cinfo}1 - 10\ax
		}

	}
	
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax		
		/if (${validate_class[FALSE, ${PetList}]} && ${${_bind}Type.Arg[${_count},|].Equal[bag]}) /varset _listout ${_listout}(${cnum}${PetBagSlotNumber}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[swarm]}) /varset _listout ${_listout}(${cnum}${PcSwarmengageat}\ax)
		/if (${validate_class[FALSE, ${PetList}]} && ${${_bind}Type.Arg[${_count},|].Equal[pet]}) /varset _listout ${_listout}(${cnum}${PcPetengageat}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***| 	
#event SendPets "[MQ2] - SendPets: #1#"
sub Event_SendPets(string line, int _tmpID)
	DEBUG ${cbug}Event_SendPets()\ax
	/if (${switchPcPet} && ${Pet.ID}) {
		VOUT Sending ${cpc}${Pet.DisplayName}\ax
		/target	ID ${_tmpID}
		/pet attack
	}
/return



|***
 *	DES: sets controlls for /tie
 *	USE: /tc [name|time|distance]
 *	INI: 
 *  NOTE:
 ***| 
#bind setTieControl /tc
sub Bind_setTieControl(string _tcType, string _verbage)
	DEBUG ${cbug}Bind_setsetTieControl()\ax
	/declare _count int local 0
	/declare _bind string local tc
	/declare _listout string local
	/declare _sep bool local FALSE

	| get help
	/if (${${_bind}Type.Find[${_${_bind}Type}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_${_bind}Type.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Lower}]}
		/return
	}	else /if (${_${_bind}Type.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Arg[${_count},|]}]}
		/next _count
		/return
	}
	
	| set time to toon name
	/if (${Bool[${_tcType.Equal[name]}]}) {
		/if (${SetTieToon.NotEqual[${_verbage}]} && ${_verbage.NotEqual[${Me.DisplayName}]}) {	
			/call writeINI SetTieToon Movement ${_verbage}	
		}

	| forced tied
	| we like it rough
	} else /if (${Bool[${_tcType.Equal[lock]}]}) {
		/call writeINI SetTieLock Movement ${If[${SetTieLock},FALSE,TRUE]}
		/if (${SetTieLock}) /call writeINI SetTieMode Movement Stick

	| set tie time
	} else /if (${Bool[${_tcType.Equal[time]}]}) {
		/if (${Range.Between[5,50:${Int[${_verbage}]}]}) {
			/call writeINI tCheck_Tie Timer ${_verbage} 
		} else {
			VOUT /${_bind} time(${cnum}${tCheck_Tie}\ax) allowable range is ${cinfo}5 - 50\ax
			/return
		}
		
	| set tie mode
	} else /if (${Bool[${_tcType.Equal[mode]}]}) {
		/if (${_verbage.Equal[stick]}) {
			/call writeINI SetTieMode Movement Stick
		} else /if (${_verbage.Equal[nav]}) {
			/call writeINI SetTieMode Movement Nav
		} 
		/declare _tcModeType string local |stick|nav
		/for _count 1 to ${_tcModeType.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]}
			/if (${_tcModeType.Arg[${_count},|].Equal[stick]}) /varset _listout ${_listout}${If[${SetTieMode.Equal[stick]},${cgood}stick\ax,${cinfo}stick\ax]}
			/if (${_tcModeType.Arg[${_count},|].Equal[nav]}) /varset _listout ${_listout}${If[${SetTieMode.Equal[nav]},${cgood}nav\ax,${cinfo}nav\ax]}
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} mode [${_listout}]
		/return
		
	| set tie distance		
	} else /if (${Bool[${_tcType.Equal[distance]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI SetTieDistance Movement ${_verbage} 
		} else {
			VOUT /${_bind} distance(${cnum}${SetTieDistance}\ax) allowable range is ${cinfo}1 - 99\ax
			/return
		}
	}
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax		
		/if (${${_bind}Type.Arg[${_count},|].Equal[name]}) /varset _listout ${_listout}(${cnum}${If[${Bool[${SetTieToon}]},${SetTieToon},NA]}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[time]}) /varset _listout ${_listout}(${cnum}${tCheck_Tie}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[distance]}) /varset _listout ${_listout}(${cnum}${SetTieDistance}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[lock]}) /varset _listout ${_listout}(${If[${SetTieLock},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[mode]}) /varset _listout ${_listout}(${cnum}${SetTieMode}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]

/return



|***
 *	DES: enable/disable Tie
 *	USE: /tie [switch|on|off]
 *	INI: 
 *  NOTE:
 ***| 	
#bind setTie /tie
sub Bind_setTie(string _tieType)

	| tie toon not in zone?
	/if (!${Bool[${Spawn[pc ${SetTieToon}].ID}]}) {
		/varset SetTie FALSE 
		/return
	}

	/if (${_tieType.Equal[on]}) {
		/if (${SetTie}) /return
		/if (${Bool[${Spawn[pc ${SetTieToon}].ID}]}) /varset SetTie TRUE
	} else /if (${_tieType.Equal[off]}) {
		/if (!${SetTie}) /return
		/if (${Bool[${Spawn[pc ${SetTieToon}].ID}]}) /varset SetTie FALSE
	} else {
		/if (${Bool[${Spawn[pc ${SetTieToon}].ID}]}) /varset SetTie ${If[${SetTie},FALSE,TRUE]}  
	}
	
	VOUT /tie ${sep} ${If[${SetTie},${oenabled},${odisabled}]} ${If[${SetTieLock},${sep} (${cgood}Locked\ax),]}
/return



|***
 *	DES: Burn control
 *	USE: /burn []
 *	INI: 
 *  NOTE:
 ***| 	
#bind burn /burn
sub Bind_burn(string _burnType, string _verbage)
	DEBUG ${cbug}Bind_burn()\ax
	/declare _count int local 0
	/declare _bind string local burn
	/declare _listout string local
	/declare _sep bool local FALSE

	| get help
	/if (${${_bind}Type.Find[${_${_bind}Type}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_${_bind}Type.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_${_bind}Type.Lower}]}
		/return
	}	else /if (${_${_bind}Type.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Arg[${_count},|]}]}
		/next _count
		/return
	}

	| Burn Automatic on/off
	/if (${Bool[${_burnType.Find[auto]}]}) {
		/call writeINI burnAuto Common ${If[${burnAuto},FALSE,TRUE]}  
		| VOUT /${_bind} auto ${sep} ${If[${burnAuto},${oenabled},${odisabled}]}

	| Mob count #
	} else /if (${Bool[${_burnType.Equal[count]}]}) {
	 	/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI burnCount Common ${_verbage} 
			| VOUT /${_bind} count ${sep} ${cinfo}${_verbage}\ax
		} else {
			VOUT /${_bind} count(${cnum}${burnCount}\ax] when # of mobs in range ${cinfo}1 - 99\ax
		}

	| Burn Engage at %
	} else /if (${Bool[${_burnType.Equal[engageat]}]}) {
	 	/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI burnEngageat Common ${_verbage} 
		} else {
			VOUT /${_bind} engageat(${cnum}${burnEngageat}\ax] % of mobs health to engage burn ${cinfo}1 - 99\ax
		}

	| force constant burning
	} else /if (${Bool[${_burnType.Equal[force]}]}) {
		/varset burnForce ${If[${burnForce},FALSE,TRUE]}
		| VOUT /${_bind} force ${sep} ${If[${burnForce},${oenabled},${odisabled}]}

	} 
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]}
		/if (${${_bind}Type.Arg[${_count},|].Equal[auto]}) /varset _listout ${_listout} ${If[${burnAuto},${cgood}auto\ax,${cinfo}auto\ax]}
		/if (${${_bind}Type.Arg[${_count},|].Equal[force]}) /varset _listout ${_listout} ${If[${burnForce},${cgood}force\ax,${cinfo}force\ax]}
		/if (${${_bind}Type.Arg[${_count},|].Equal[engageat]}) /varset _listout ${_listout} ${cinfo}engageat\ax(${cnum}${burnEngageat}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[count]}) /varset _listout ${_listout} ${cinfo}count\ax(${cnum}${burnCount}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
	} 
/return



|***
 *	DES: core controls
 *	USE: /core [] [TEXT]
 *	INI: 
 *  NOTE:
 ***|
#bind setCore /core
sub Bind_setCore(string _coreType, string _verbage)
	DEBUG ${cbug}Bind_setCore(type:${_coreType}, verbage:${_verbage})\ax

	/declare _count int local 0
	/declare _bind string local core
	/declare _listout string local
	/declare _sep bool local FALSE

	| get help
	/if (${${_bind}Type.Find[${_${_bind}Type}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_${_bind}Type.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_${_bind}Type.Lower}]}
		/return
	}	else /if (${_${_bind}Type.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Arg[${_count},|]}]}
		/next _count
		/return
	}

	| core auto/manual
	/if (${Bool[${_coreType.Equal[auto]}]}) {
		
		/if (${Bool[${_verbage.Equal[on]}]}) {
			/call writeINI coreAuto Common TRUE
		} else /if (${Bool[${_verbage.Equal[off]}]}) {
			/call writeINI coreAuto Common FALSE
		} else {
			/call writeINI coreAuto Common ${If[${coreAuto},FALSE,TRUE]}
		}
		/varset combat ${If[${coreAuto},${Ini[${INIClass},Common,combat]},FALSE]}  
		/if (!${coreAuto}) { 
			/call set_stop TRUE
			/call clear_combat
		}
		VOUT /${_bind} auto ${sep} ${If[${coreAuto},${oenabled},${odisabled}]}

	| enable/disable obscure target names ## not documented
	} else /if (${Bool[${_coreType.Equal[obscure]}]}) {
		/varset coreObscure ${If[${coreObscure},FALSE,TRUE]}
		VOUT /${_bind} obscure ${sep} ${If[${coreObscure},${oenabled},${odisabled}]}

	| enable/disable using events.inc file
	} else /if (${Bool[${_coreType.Equal[events]}]}) {
		/call writeINI coreEventsInc Common ${If[${coreEventsInc},FALSE,TRUE]}
		VOUT /${_bind} events ${sep} ${If[${coreEventsInc},${oenabled},${odisabled}]}

	| set the kill code
	} else /if (${Bool[${_coreType.Equal[killcode]}]}) {
		/if (${Bool[${_verbage}]}) {
			/call DeclareCoreIniVar NA remoteKillKey General ${_verbage} 
			VOUT /${_bind} ${cinfo}killcode\ax ${sep} ${_verbage}
		} else {
			VOUT /${_bind} killcode ${sep} (${cnum}${remoteKillKey}\ax)
		}

	| runs the whitewash routine to clear all aliases
	} else /if (${Bool[${_coreType.Equal[whitewash]}]}) {
		/call whitewash
		
	} else /if (${Bool[${_coreType.Equal[rest]}]}) {
		/if (${Range.Between[0,99:${Int[${_verbage}]}]}) {
			/call writeINI autoRest Common ${_verbage}  
			| /call echos setswitch /${_bind} autoRest ${_verbage} ${_${_bind}Type}
			| /call echos number "Auto Rest" ${If[${_verbage},${_verbage},${autoRest}]} 0 0
			VOUT /${_bind} rest ${sep} ${If[${AutoExpAdjust},${oenabled},${odisabled}]}
			
		} 
		/call echos setswitch /${_bind} autoRest ${autoRest} ${_${_bind}Type}
		VOUT /${_bind} rest ${sep} ${If[${AutoExpAdjust},${oenabled},${odisabled}]}


	| reset core
	} else /if (${Bool[${_coreType.Equal[reset]}]}) {
		/call initialize_core


	| set control toon
	} else /if (${Bool[${_coreType.Equal[control]}]}) {
		/if (${Bool[${_verbage}]}) {
			/call writeINI toonControl Common ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		} else /if (!${Bool[${_verbage}]}) {
			/call writeINI toonControl Common FALSE
		}
		VOUT /${_bind} control ${sep} ${If[${Bool[${toonControl}]},${cgood}${toonControl}\ax,${odisabled}]}

	| reset all core INIs
	} else /if (${Bool[${_coreType.Equal[reset]}]}) {
		| /call core_reset

	| enable wasiting fireworks for those bastards with too much AA on their hands
	} else /if (${Bool[${_coreType.Equal[fireworks]}]}) {
		/call writeINI switchAAFireworks Common ${If[${switchAAFireworks},FALSE,TRUE]}  
		VOUT /${_bind} fireworks ${sep} ${If[${switchAAFireworks},${oenabled},${odisabled}]}
		/if (!${switchAAFireworks}) /call create_timer timer_Waste_Fireworks 1

		
	| take invites to raid/group
	} else /if (${Bool[${_coreType.Equal[invite]}]}) {
		/call writeINI switchInvites Common ${If[${switchInvites},FALSE,TRUE]}  
		/call echos switch "Auto Accept Invites for [${ctar}Group\ax|${ctar}Raid\ax]" switchInvites 0 0

	| accepts requests to add to the raid
	} else /if (${Bool[${_coreType.Equal[raidinvite]}]}) {
		/call writeINI switchRaidInvite Common ${If[${switchRaidInvite},FALSE,TRUE]} 
		/call echos switch "Accept request for invites to [${ctar}Raid\ax]" switchRaidInvite 0 0

	| claim market cash
	} else /if (${Bool[${_coreType.Equal[dbcash]}]}) {
		/call check_free_ingame_cash

	| exp max level
	} else /if (${Bool[${_coreType.Equal[expmaintain]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI MaintExpLvl Common ${_verbage} 
			VOUT /${_bind} expmaintain(${cnum}${MaintExpLvl}\ax)
		} else {
			VOUT /${_bind} expmaintain(${cnum}${MaintExpLvl}\ax) allowable range ${cinfo}1 - 99\ax
		}	

	| exp maintain level
	} else /if (${Bool[${_coreType.Equal[explevel]}]}) {
		/if (${Range.Between[1,110:${Int[${_verbage}]}]}) {
			/call writeINI MaxLevel Common ${_verbage} 
			VOUT /${_bind} explevel(${cnum}${MaxLevel}\ax)
		} else {
			VOUT /${_bind} explevel(${cnum}${MaxLevel}\ax) allowable range ${cinfo}1 - 110\ax
		}	

	| exp auto adjust
 	} else /if (${Bool[${_coreType.Equal[expadjust]}]}) {
		/call writeINI AutoExpAdjust Common ${If[${AutoExpAdjust},FALSE,TRUE]} 
		VOUT /${_bind} expadjust ${sep} ${If[${AutoExpAdjust},${oenabled},${odisabled}]}

	| delete old INI entries
 	} else /if (${Bool[${_coreType.Find[fixini]}]}) {
		/call fix_INI_declares

	} else {
		/for _count 1 to ${${_bind}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
			/if (${${_bind}Type.Arg[${_count},|].Equal[explevel]}) /varset _listout ${_listout}(${cnum}${MaxLevel}\ax)
			/if (${${_bind}Type.Arg[${_count},|].Equal[expmaintain]}) /varset _listout ${_listout}(${cnum}${MaintExpLvl}\ax)
			/if (${${_bind}Type.Arg[${_count},|].Equal[rest]}) /varset _listout ${_listout}(${cnum}${autoRest}\ax)
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
	} 
/return



|***
 *	DES: set on/off switches
 *	USE: /onoff [banestrike|horn|shrink]
 *	INI: NA
 *  NOTE: enable/disable many switches
 ***|
#bind onoff /onoff
sub Bind_onoff(string _onoffType)
	DEBUG ${cbug}Bind_onoff(type: ${_onoffType})\ax
	/declare _count int local 0
	/declare _listout string local
	/declare _sep bool local FALSE
	/declare _bind string local onoff
	
	/if (${Bool[${${_bind}Type.Find[${_${_bind}Type}]}]}) {
		/if (${Defined[switch${_onoffType.Left[1].Upper}${_onoffType.Right[-1].Lower}]}) {
			/call writeINI switch${_${_bind}Type.Left[1].Upper}${_${_bind}Type.Right[-1].Lower} Common ${If[${switch${_${_bind}Type.Left[1].Upper}${_${_bind}Type.Right[-1].Lower}},FALSE,TRUE]}  
			/call echos switch "${_${_bind}Type.Left[1].Upper}${_${_bind}Type.Right[-1].Lower}" switch${_${_bind}Type.Left[1].Upper}${_${_bind}Type.Right[-1].Lower} 0 0
		} 
	} else {
		/for _count 1 to ${${_bind}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax		
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
	}
/return  



|***
 *	DES: i dunno... information maybe?
 *	USE: /info
 *	INI: NA
 *  NOTE: list all switches for help
 ***|
#bind info /info
sub Bind_info()
	DEBUG ${cbug}Bind_info()\ax
	/declare _listout string local
	/declare _sep bool local
	/declare _count int local
	/declare _controls string local |buff|burn|cc|core|cursor|debuff|echos|grab|hc|heal|lc|mode|movement|onoff|override|pc|port|rez|setassist|tc|tie|ts|${Me.Class.ShortName}
	
	/for _count 1 to ${_controls.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}/${_controls.Arg[${_count},|].Lower}\ax
		/varset _sep TRUE
	/next _count
	/if (${switchDispInfo}) /call echos wiki 0 0 0 0
	VOUT ${_listout}
	/if (${switchDispInfo}) VOUT /command ${cinfo}info\ax for details
/return  	



|***
 *	DES: tradeskill controls
 *	USE: /ts [movebank|movehouse]
 *	INI: 
 *  NOTE:
 ***|
#bind ts /ts
sub Bind_ts(string _tsType)
	DEBUG ${cbug}Bind_ts(type:${_tsType})\ax

	/declare _listout string local
	/declare _sep bool local
	/declare _count int local
	/declare _bind string local ts

	| tradeskill commands
	/if (${Bool[${_tsType.Find[movebank]}]}) {
		/declare _slot 				int local
		/declare _bagslot 		int local
		/for _slot 23 to 26
			/if (${InvSlot[${_slot}].Item.Container}) {
				/for _bagslot 1 to ${InvSlot[${_slot}].Item.Container}
					/if (${Me.Inventory[${_slot}].Item[${_bagslot}].StackSize} > 100 && ${Window[BigBankWnd].Open}) {
						VOUT  Moving ${sep} ${cinfo}${Me.Inventory[${_slot}].Item[${_bagslot}].Name}\ax ${sep} [${ctar}${FindItemCount[${Me.Inventory[${_slot}].Item[${_bagslot}].Name}]}\ax]
						/nomodkey /shift /itemnotify in Pack${Math.Calc[${FindItem[=${Me.Inventory[${_slot}].Item[${_bagslot}].Name}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${Me.Inventory[${_slot}].Item[${_bagslot}].Name}].ItemSlot2}+1]} leftmouseup
						/delay 5 ${Cursor.ID}
						/notify BigBankWnd bigb_autobutton leftmouseup
						/delay 5 !${Cursor.ID}
					}
				/next _bagslot
			}
		/next _slot

	} else /if (${Bool[${_tsType.Find[movehouse]}]}) {
		/if (${Window[RealEstateItemsWnd].Open}) {
			/declare _slot 				int local
			/declare _bagslot 		int local
			/for _slot 23 to 26
				/if (${InvSlot[${_slot}].Item.Container}) {
					/for _bagslot 1 to ${InvSlot[${_slot}].Item.Container}
						/nomodkey /itemnotify "${Me.Inventory[${_slot}].Name}" rightmouseup
						/if (${Me.Inventory[${_slot}].Item[${_bagslot}].StackSize} > 100 && ${Window[RealEstateItemsWnd].Open}) {
							VOUT  Moving ${sep} ${cinfo}${Me.Inventory[${_slot}].Item[${_bagslot}].Name}\ax ${sep} [${ctar}${FindItemCount[${Me.Inventory[${_slot}].Item[${_bagslot}].Name}]}\ax]
							| /nomodkey /itemnotify in Pack${Math.Calc[${FindItem[=${Me.Inventory[${_slot}].Item[${_bagslot}].Name}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${Me.Inventory[${_slot}].Item[${_bagslot}].Name}].ItemSlot2}+1]} 
							/click left ${Me.Inventory[${_slot}].Item[${_bagslot}].Name} 

							/nomodkey /shift /notify RealEstateItemsWnd REIW_Move_Closet_Button leftmouseup
							
						}
					/next _bagslot
				}
			/next _slot
		} else {
			VOUT Open Realestate Items Window
		}

	} else {
		/for _count 1 to ${${_bind}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax		
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
	} 
/return



|***
 *	DES: simple shortcut to the  /cc smartma command
 *	USE: /setassist [X|R,Raid|G,Group|F,Force|1,MA1|2,MA2|NULL]
 *	INI: 
 *  NOTE: 
 ***| 	
#bind setAssist /smartma
#bind setAssist /setassist
sub Bind_setAssist(string _verbage, _name)
	DEBUG ${cbug}Bind_setAssist(assist: ${_verbage}, name: ${_name})\ax
	/call validate_assist FALSE ${_verbage} ${If[${Bool[${_name}]},${_name},]}
/return



|***
 *	DES: combat controls
 *	USE: /cc []
 *	INI: 
 *  NOTE:
 ***|
#bind setCombatControl /cc
sub Bind_setCombatControl(string _ccType, string _verbage)
	DEBUG ${cbug}Bind_setCombatControl(type:${_ccType}, verbage:${_verbage})\ax

	/declare _count int local 0
	/declare _bind string local cc
	/declare _listout string local
	/declare _sep bool local FALSE
	/declare _combatModes string local |melee|range|nuke|heal|dual

	| get help
	/if (${${_bind}Type.Find[${_${_bind}Type}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_${_bind}Type.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_${_bind}Type.Lower}]}
		/return
	}	else /if (${_${_bind}Type.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Arg[${_count},|]}]}
		/next _count
		/return
	}

	| enable / disable forced assist
	/if  (${Bool[${_ccType.Equal[forceassist]}]}) {
		/call validate_assist FALSE force
		| /call writeINI useForceAssist Common ${If[${useForceAssist},FALSE,TRUE]}
 		| /if (${useSmartAssistR}) /call writeINI useSmartAssistR Common FALSE
		| /if (${useSmartAssistG}) /call writeINI useSmartAssistG Common FALSE
		VOUT /${_bind} forceassist ${sep} ${If[${useForceAssist},${oenabled},${odisabled}]}
	
	| force manual combat routines
	} else /if (${Bool[${_ccType.Equal[force]}]}) {
		/if (${Bool[${_verbage}]}) {
			/if (${Target.ID} && (${Target.ID} != ${Me.ID}) && (${Target.ID} != ${Me.Pet.ID})) {
				| engage Melee combat
				/if (${Bool[${_verbage.Equal[melee]}]}) {
					/varset isValidTarget TRUE
					/varset combat MELEE
					/varset Target2Kill ${Target.ID}
				| engage Range combat
				} else /if (${Bool[${_verbage.Equal[range]}]}) {
					/if (${Target.Distance} > 31) {
						/varset isValidTarget TRUE
						/varset combat RANGE
						/varset Target2Kill ${Target.ID}	
					} 
				| engage Nuke combat
				} else /if (${Bool[${_verbage.Equal[nuke]}]}) {
					/varset isValidTarget TRUE
					/varset combat NUKE
					/varset Target2Kill ${Target.ID}

				| force assist for other eqbc toons
				} else /if (${Bool[${_verbage.Equal[assist]}]}) {
					/if (!${validate_plugin[FALSE, |mq2eqbc]} || (!${EQBC.Connected} && ${Bool[${EQBCServer}]})) {
						VOUT ${owarning} ${sep} EQBC Error
						/return
					}		
					DEBUG passing targetID: ${Target.ID}			
					/bca //varset Target2Kill ${Target.ID}
				}
				VOUT /${_bind} force ${cinfo}${_verbage}\ax ${sep} [${ctar}${Spawn[ID ${Target.ID}].DisplayName}\ax]
			}
			 
			| echo options
			} else {
				/declare _tmpVariable string local |melee|range|nuke
				/for _count 1 to ${_tmpVariable.Count[|]}
					/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_tmpVariable.Arg[${_count},|]}\ax
					/varset _sep TRUE
				/next _count
				VOUT /${_bind} force [${_listout}]	
			}

	| set bandolier swapping
	} else /if (${Bool[${_ccType.Equal[bandolier]}]}) {
		/call writeINI switchBandolier meleedisc ${If[${switchBandolier},FALSE,TRUE]}
		VOUT /${_bind} bandolier ${sep} ${cinfo}${combat}\ax

	| set stop nuke %
	} else /if (${Bool[${_ccType.Equal[stopnuke]}]}) {
		/if (${Range.Between[0,99:${Int[${_verbage}]}]}) {
			/call writeINI stopnukeAt Common ${_verbage} 
			VOUT /${_bind} stopnuke ${sep} ${cinfo}${_verbage}%\ax
		} else {
			VOUT /${_bind} stopnuke(${cnum}${stopnukeAt}\ax) allowable range is ${cinfo}0 - 99\ax
			/return
		} 			

	| set circle of to be used
	} else /if (${Bool[${_ccType.Equal[circle]}]}) {
		/if (${Bool[${_verbage.Equal[power]}]} || ${Bool[${_verbage.Equal[life]}]} || ${Bool[${_verbage.Equal[mana]}]}) {
			/call writeINI useCircle Items ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
			VOUT /${_bind} circle ${sep} ${cinfo}${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}\ax
		} else /if (!${Bool[${_verbage}]}) {
			VOUT /${_bind} circle allowable options [${cinfo}power, life, mana\ax] 
			/return
		} 

	| set automated combat
	} else /if (${Bool[${_ccType.Equal[melee]}]} || ${Bool[${_ccType.Equal[range]}]} || ${Bool[${_ccType.Equal[nuke]}]} || ${Bool[${_ccType.Equal[heal]}]} || ${Bool[${_ccType.Equal[dual]}]}) {
		/if (${combat.NotEqual[${_ccType}]}) /call writeINI combat Common ${_ccType.Upper}
		/for _count 1 to ${_combatModes.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${combat.Equal[${_combatModes.Arg[${_count},|]}]},${cgood}${combat.Upper}\ax,${cinfo}${_combatModes.Arg[${_count},|]}\ax]}   
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]	
				
	| set Pull
	} else /if (${Bool[${_ccType.Equal[pull]}]}) {
		/call writeINI switchPull Common ${If[${switchPull},FALSE,TRUE]}
		VOUT /${_bind} pull ${sep} ${If[${switchPull},${oenabled},${odisabled}]}
		/if (${switchPull}) /call validate_assist FALSE x
		
	| delay in nuking	
	} else /if (${Bool[${_ccType.Equal[nukedelay]}]}) {
		/if (${Range.Between[1,100:${Int[${_verbage}]}]}) {
			/call writeINI tNuke_Delay Timer ${_verbage} 
			VOUT /${_bind} nukedelay ${sep} ${cinfo}${_verbage}\ax
		} else {
			VOUT /${_bind} nukedelay(${cnum}${tNuke_Delay}\ax) allowable range is ${cinfo}1 - 100\ax
			/return
		} 	

	| agro/agro range ##
	} else /if (${Bool[${_ccType.Equal[agro]}]}) {
		/if (${Bool[${_verbage}]} && ${Range.Between[1,400:${Int[${_verbage}]}]}) {
			/call writeINI MobAgro Common ${_verbage} 
			VOUT /${_bind} agro ${sep} ${cinfo}${_verbage}\ax
		} else /if (${Bool[${_verbage}]} && !${Range.Between[1,400:${Int[${_verbage}]}]})  {
			VOUT /${_bind} agro(${cnum}${MobAgro}\ax) allowable range is ${cinfo}1 - 400\ax
			/return
		} else /if (!${Bool[${_verbage}]}) {
			/call writeINI switchAgro Common ${If[${switchAgro},FALSE,TRUE]}
			VOUT /${_bind} agro ${sep} ${If[${switchAgro},${oenabled},${odisabled}]}
		}
		/call check_AA_agro		


	| set out of group Manual Assist 1 or 2
	} else /if (${Bool[${_ccType.Equal[ma1]}]}) {
		/call validate_assist FALSE ma1 ${_verbage}
		|**
		/if (${Bool[${_verbage}]}) {
			/call writeINI MA1 Common ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		} else /if (!${Bool[${_verbage}]})  {
			/call writeINI MA1 Common FALSE
		} 				
		**|
		VOUT /${_bind} ma1 ${sep} ${If[${Bool[${MA1}]},${cinfo}${MA1}\ax,${odisabled}]}
	} else /if (${Bool[${_ccType.Equal[ma2]}]}) {		
		/call validate_assist FALSE ma2 ${_verbage}
		|**
		/if (${Bool[${_verbage}]}) {
			/call writeINI MA2 Common ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		} else /if (!${Bool[${_verbage}]})  {
			/call writeINI MA2 Common FALSE
		} 				
		**|
		VOUT /${_bind} ma2 ${sep} ${If[${Bool[${MA2}]},${cinfo}${MA2}\ax,${odisabled}]}	
		

	| MA1/2 engage at %
	} else /if (${Bool[${_ccType.Equal[engage]}]}) {
		/if (${Range.Between[1,100:${Int[${_verbage}]}]}) {
			/call writeINI engageAt Common ${_verbage}  
			VOUT /${_bind} engage ${sep} ${cinfo}${engageAt}\ax
		} else {
			VOUT /${_bind} engage(${cnum}${engageAt}\ax) allowable range ${cinfo}1 - 99\ax
			/return
		}

	| Smart Assist engage at %
	} else /if (${Bool[${_ccType.Equal[smartengage]}]}) {
		/if (!${validate_expansion[TRUE, |16]}) /return
		/if (${Range.Between[1,100:${Int[${_verbage}]}]}) {
			/call writeINI smartengageAt Common ${_verbage}  
			VOUT /${_bind} smartengage ${sep} ${cinfo}${smartengageAt}\ax
		} else {
			VOUT /${_bind} smartengage(${cnum}${smartengageAt}\ax) allowable range ${cinfo}1 - 99\ax
			/return
		}

	| set melee combat distance to target
	} else /if (${Bool[${_ccType.Equal[combatdistance]}]}) {
		/if (${Range.Between[1,50:${Int[${_verbage}]}]}) {
			/call writeINI CombatDistance Movement ${_verbage}
			VOUT /${_bind} combatdistance ${sep} ${cinfo}${_verbage}\ax
		} else {
			VOUT /${_bind} combatdistance(${cnum}${CombatDistance}\ax] minimum range to melee mob ${cinfo}1 - 50\ax
		}

	| set fake safe radius
	} else /if (${Bool[${_ccType.Equal[saferadius]}]}) {
		/if (${Range.Between[1,50:${Int[${_verbage}]}]}) {
			/call writeINI combatRadius Common ${_verbage}
			VOUT /${_bind} saferadius ${sep} ${cinfo}${combatRadius}\ax
		} else {
			VOUT /${_bind} saferadius(${cnum}${combatRadius}\ax] allowable radius ${cinfo}1 - 50\ax
		}
	
	| set Z radius
	} else /if (${Bool[${_ccType.Equal[zradius]}]}) {
		/if (${Range.Between[1,200:${Int[${_verbage}]}]}) {
			/call writeINI zradius Common ${_verbage} 
			VOUT /${_bind} zradius ${sep} ${cinfo}${zradius}\ax
			/call echos number "Set ZRadius" ${_verbage} 0 0
		} else {
			VOUT /${_bind} zradius(${cnum}${zradius}\ax) allowable range ${cinfo}1 - 200\ax
			/return
		}	
		
	| set smart assist
	} else /if (${Bool[${_ccType.Equal[smartma]}]}) {
		/if (!${validate_expansion[TRUE, |16]}) {
			OUT Use /cc MA1 [NAME] or MA2 [NAME]
			/return
		}
		/call clear_combat

		| Raid Assist Role
		/if ((${_verbage.Equal[r]} || (!${Bool[${_verbage}]} && !${useSmartAssistR} && ${Bool[${Raid.Members}]})) && !${switchPull}) {
			DEBUG ((${_verbage} || (!${Bool[${_verbage}]} && !${useSmartAssistR} && !${useSmartAssistG} && ${Bool[${Raid.Members}]})) && !${Select[${Me.Class.ShortName},CLR]} && !${switchPull})
			/call validate_assist FALSE raid
	 		| /if (!${useSmartAssistR}) /call writeINI useSmartAssistR Common TRUE
			| /if (${useSmartAssistG}) /call writeINI useSmartAssistG Common FALSE
			| /if (${useForceAssist}) /call writeINI useForceAssist Common FALSE
			| /xtarget set 1 Raidassist1target

		| Goup Assist role
		} else /if ((${_verbage.Equal[g]} || (!${Bool[${_verbage}]} && !${useSmartAssistR} && !${useSmartAssistG}) || (!${Bool[${Raid.Members}]} && ${Bool[${Group.Members}]} && !${useSmartAssistG})) && !${switchPull}) {
			DEBUG ((${_verbage} || (!${Bool[${_verbage}]} && !${useSmartAssistR} && !${useSmartAssistG}) || (!${Bool[${Raid.Members}]} && ${Bool[${Group.Members}]} && !${useSmartAssistG})) && !${switchPull})
			/call validate_assist FALSE group
	 		| /if (${useSmartAssistR}) /call writeINI useSmartAssistR Common FALSE
			| /if (!${useSmartAssistG}) /call writeINI useSmartAssistG Common TRUE
			| /if (${useForceAssist}) /call writeINI useForceAssist Common FALSE
			| /xtarget set 1 Groupassisttarget

		| NO Assist
		} else /if ((${_verbage.Equal[x]} || (!${Bool[${_verbage}]} && (${useSmartAssistR} || ${useSmartAssistG})) || (!${Bool[${Raid.Members}]} && !${Bool[${Group.Members}]})) || ${switchPull}) {
			DEBUG ((${_verbage} || (!${Bool[${_verbage}]} && !${useSmartAssistR} && ${useSmartAssistG}) || (!${Bool[${Raid.Members}]} && !${Bool[${Group.Members}]})) || ${switchPull})
			/call validate_assist FALSE x
	 		| /if (${useSmartAssistR}) /call writeINI useSmartAssistR Common FALSE
			| /if (${useSmartAssistG}) /call writeINI useSmartAssistG Common FALSE
			| /if (${useForceAssist}) /call writeINI useForceAssist Common FALSE
			| /xtarget set 1 Autohater
		}

		| output result
		/declare _assistMode string local |R|G
		/declare _assM string local
		
		/for _count 1 to ${_assistMode.Count[|]}
			/varset _assM ${useSmartAssist${_assistMode.Arg[${_count},|]}}
			DEBUG mode: ${_assistMode.Arg[${_count},|]} -- ${_assM}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${_assM},${cgood}${_assistMode.Arg[${_count},|]}\ax,${cinfo}${_assistMode.Arg[${_count},|]}\ax]}
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} smartma [${_listout}]


	} else {
		/for _count 1 to ${${_bind}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax		
			/if (${${_bind}Type.Arg[${_count},|].Equal[zradius]}) /varset _listout ${_listout}(${cnum}${zradius}\ax)
			/if (${${_bind}Type.Arg[${_count},|].Equal[smartengage]}) /varset _listout ${_listout}(${cnum}${smartengageAt}\ax)
			/if (${${_bind}Type.Arg[${_count},|].Equal[engage]}) /varset _listout ${_listout}(${cnum}${engageAt}\ax)
			/if (${${_bind}Type.Arg[${_count},|].Equal[nukedelay]}) /varset _listout ${_listout}(${cnum}${tNuke_Delay}\ax)
			/if (${${_bind}Type.Arg[${_count},|].Equal[agro]}) /varset _listout ${_listout}(${cnum}${MobAgro}\ax)
			/if (${${_bind}Type.Arg[${_count},|].Equal[combatdistance]}) /varset _listout ${_listout}(${cnum}${CombatDistance}\ax)
			/if (${${_bind}Type.Arg[${_count},|].Equal[saferadius]}) /varset _listout ${_listout}(${cnum}${combatRadius}\ax)
			/if (${${_bind}Type.Arg[${_count},|].Equal[stopnuke]}) /varset _listout ${_listout}(${cnum}${stopnukeAt}\ax)
			/if (${${_bind}Type.Arg[${_count},|].Equal[circle]}) /varset _listout ${_listout}(${cnum}${useCircle}\ax)
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
	} 
/return	
	


|***
 *	DES: shortcut for the agro command
 *	USE: /agro [##]
 *	INI: 
 *  NOTE:
 ***| 	
#bind MobAgro /agro
sub Bind_MobAgro(int _MobAgro)
	/call Bind_setCombatControl agro ${_MobAgro}
/return



|***
 *	DES: 
 *	USE: /port 
 *	INI: 
 *  NOTE:
 ***| 
#bind port_list /port
sub Bind_port_list(string _class, string _location)
	/declare _listout 				string 	local
	/declare _sep 						bool 		local FALSE
	/declare _country 				int 		local 0
	/declare _count 					int 		local 0
	/declare _state 					int 		local 0
	/declare _portclass 			string 	local |druid|wizard
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
	/declare _portWizardKunark 											string local |lceanium|dreadlands
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
		/for _count 1 to ${_portclass.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_portclass.Arg[${_count},|]}\ax
			/varset _sep TRUE
		/next _count
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
		/for _count 1 to ${Resident${_class}.Count[|]}
			/if (${Spawn[pc ${Resident${_class}.Arg[${_count},|]} zradius 80 radius 100].ID}) {
				/t ${Resident${_class}.Arg[${_count},|]} send ${_location}
				/varset _tell TRUE
			}		
		/next _count
		/if (!${_tell}) /call echos failboat 0 0 0 0
	}

/return



|***
 *	DES: zephyr/TL requesting toon.
 *	USE: /t [toon] send [location]
 *	INI: 
 *  NOTE: 
 ***|
#event send "#1# tells you, 'send #2#'"
sub Event_send(string line, string _sender, string _location)
	CHECKSAFELIST
	/if (!${Select[${Me.Class.ShortName},DRU,WIZ]} || ${Spawn[pc ${_sender}].Distance} > 125) /return
	/declare _portList[14]		string local
	/declare _count 					int local 0

	/if (${_location.Equal[where]}) {
		/if (${Select[${Me.Class.ShortName},DRU]}) {
			/varset _portList[1] Alaris: shardslanding, pillars, beast
			/varset _portList[2] Antonica: westkarna, lavastorm, undershore, misty, ro, feerrott, commonlands, surefall, karana
			/varset _portList[3] Discord: wallofslaughter, bloodfields
			/varset _portList[4] Faydwer: looping, steamfront, butcherblock
			/varset _portList[5] Kunark: lceanium, dreadlands
			/varset _portList[6] Luclin: dawnshroud, twilight, grimling, nexus
			/varset _portList[7] Misc: primarypush, secondarypush, primary, secondary
			/varset _portList[8] Odus: tempesttemple, buriedsea, stonebrunt, tox
			/varset _portList[9] Serpent's Spine: direwind, steppes, blightfire
			/varset _portList[10] Taelosia: barindu, natimbi
			/varset _portList[11] The Planes: grounds, potime, brells, arcstone, knowledge
			/varset _portList[12] 
			/varset _portList[13] Velious: cobaltscar, wakening, greatdivide, iceclad
			/varset _portList[14] --Port List End
		} else /if (${Select[${Me.Class.ShortName},WIZ]}) {
			/varset _portList[1] Alaris: pillars, sarith 
			/varset _portList[2] Antonica: northkarana, commonlands, nektulos, qeyenos, northro, cazic, undershore, westkarana
			/varset _portList[3] Discord: wallofslaughter, bloodfields
			/varset _portList[4] Faydwer: fay, dragonscale
			/varset _portList[5] Kunark: lceanium, dreadlands
			/varset _portList[6] Luclin: dawnshroud, twilight, grimling
			/varset _portList[7] Misc: bind, nexus, primary, secondary, primarypush, secondarypush
			/varset _portList[8] Odus: tempesttemple, katta
			/varset _portList[9] Serpent's Spine: tox, stonebrunt, icefall, sunderock, blightfire
			/varset _portList[10] Taelosia: barindu, natimbi
			/varset _portList[11] The Planes: grounds, potime, brells, arcstone, pok
			/varset _portList[12] 
			/varset _portList[13] Velious: cobaltscar, wakening, greatdivide, iceclad
			/varset _portList[14] --Port List End
		}
		| feed the response back to sender		
		/for _count 1 to ${_portList.Size}
			/t ${_sender} ${_portList[${_count}]}
			/delay 8
		/next _count
	}
	
	| send 'em on their way with a valid location request
	/if (${Defined[${_location}]}) {
		/t ${_sender} Please enjoy your flight. ${${_location}} on its way.
		/call scribe ${${_location}} ${buffGem} TRUE
		/call core_cast ${${_location}} 0 ${Spawn[pc ${_sender}].ID} FALSE
	} 
/return



|***
 *	DES: movement controls
 *	USE: /movement []
 *	INI: 
 *  NOTE:
 ***|
#bind setMovementControls /movement
sub Bind_setMovementControls(string _movementType, string _verbage)
	DEBUG ${cbug}Bind_setMovementControls(type:${_movementType}, verbage:${_verbage})\ax
	
	/if (!${validate_plugin[FALSE, |MQ2MoveUtils]}) /return

	/declare _count int local 0
	/declare _bind string local movement
	/declare _listout string local
	/declare _sep bool local FALSE

	| get help
	/if (${${_bind}Type.Find[${_${_bind}Type}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_${_bind}Type.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_${_bind}Type.Lower}]}
		/return
	}	else /if (${_${_bind}Type.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Arg[${_count},|]}]}
		/next _count
		/return
	}

	| set home on or off
	/if (${Bool[${_${_bind}Type.Equal[home]}]}) {
		/call writeINI HomeSet Movement ${If[${HomeSet},FALSE,TRUE]}  
		/call set_home
		/return
		
	| navigate to the Target or NAME		
	} else /if (${Bool[${_${_bind}Type.Equal[goto]}]}) {
		/if (!${validate_plugin[TRUE, |MQ2Nav]}) /return
		/if (!${Target.ID} && !${Bool[${_verbage}]}) /return
		/if (${Bool[${_verbage}]}) {
			/nav spawn ID ${_verbage}
		} else /if (${Target.ID}) {
			/nav spawn ${Target.DisplayName}
		}
		/return		

	| set stick behind
	} else /if (${Bool[${_${_bind}Type.Equal[behind]}]}) {
		/call writeINI SetStickBehind Movement ${If[${SetStickBehind},FALSE,TRUE]}  
		VOUT /${_bind} behind ${sep} ${If[${SetStickBehind},${oenabled},${odisabled}]}

		| disable mezzing with stickbehind enabled.
		/if (${switchdeBuffMezz} && ${SetStickBehind}) {
			/if (${Defined[switchdeBuffMezz]}) /call writeINI switchdeBuffMezz Buffs FALSE  
			/VOUT /${_bind} behind ${sep} ${If[${switchdeBuffMezz},${oenabled},${odisabled}]}
		}
		/return

	| set face fast
	} else /if (${Bool[${_${_bind}Type.Equal[fast]}]}) {
		/call writeINI SetFaceFast Movement ${If[${SetFaceFast},FALSE,TRUE]}  
		VOUT /${_bind} fast ${sep} ${If[${SetFaceFast},${oenabled},${odisabled}]}
		/if (${SetFaceFast}) VOUT ${owarning} ${sep} This is a MQ2 Dead Giveaway ${sep} ${owarning}
		/return

	| set home radius
	} else /if (${Bool[${_${_bind}Type.Equal[radius]}]}) {
		/if (${Range.Between[1,200:${Int[${_verbage}]}]}) {
			/if (${_verbage} < ${HomeLeash}) {
				VOUT ${onotice} ${sep} Radius must be greater then leash(${cnum}${HomeLeash}\ax)
				/return
			}
			/call writeINI HomeRadius Movement ${_verbage} 
		} else {
			VOUT /${_bind} radius(${cnum}${HomeRadius}\ax) allowable range ${cinfo}1 - 200\ax
			/return
		}
		
		
	| gather your crew
	} else /if (${_${_bind}Type.Equal[gather]}) {
		/if (!${validate_plugin[TRUE, |MQ2Nav]}) /return
		/declare _clist string local ${Ini[${INICore},Group,${_verbage}]}
	

		/for _count 1 to ${_clist.Count[|]}
			/if (${_clist.Arg[${_count},|].Equal[${Me.DisplayName}]}) /continue
			/if (${Bool[${Spawn[pc ${_clist.Arg[${_count},|]}]}]}) {
				VOUT Gathering ${sep} ${cinfo}${_clist.Arg[${_count},|]}\ax
				/bct ${_clist.Arg[${_count},|]} //nav spawn pc ${Me.DisplayName}
				
			}

		/next _count		
		/return
		
	| set leash range
	} else /if (${_${_bind}Type.Equal[leash]}) {
		/if (${Range.Between[1,200:${Int[${_verbage}]}]}) {
			/if (${HomeRadius} < ${_verbage}) {
				VOUT ${onotice} ${sep} Leash must be less then radius(${cnum}${HomeRadius}\ax)
				/return
			}
			/call writeINI HomeLeash Movement ${_verbage} 
		} else {
			VOUT /${_bind} leash(${cnum}${HomeLeash}\ax) allowable range ${cinfo}1 - 200\ax
			/return
		}	

	| set facing arc
	} else /if (${Bool[${_${_bind}Type.Equal[arc]}]}) {
		/if (${Range.Between[1,45:${Int[${_verbage}]}]}) {
			/call writeINI SetFaceArc Movement ${_verbage} 
		} else {
			VOUT /${_bind} arc(${cnum}${SetFaceArc}\ax) allowable range ${cinfo}1 - 45\ax
			/return
		}	
		
	| i need mounts because i like to annoy people in the raid
	} else /if (${Bool[${_${_bind}Type.Equal[mount]}]}) {
		/if (!${Bool[${_verbage}]}) {
			VOUT /${_bind} mount [${cinfo}on\ax ${sep} ${cinfo}off\ax]
		} else /if (${Bool[${_verbage.Equal[off]}]}) {
			/if (!${Bool[${Me.Mount.ID}]}) {
				/return
			} else /if (${Bool[${Me.Mount.ID}]}) {
				/dismount
				/return
			}
		} else /if (${Bool[${_verbage.Equal[on]}]}) {
			/if (${Bool[${Me.Mount.ID}]}) /return
			/if (${Zone.Indoor}) {
				VOUT Go outside..
				/return
			}			
			/if (${Me.ItemReady[${mount}]}) /call core_cast "${mount}" item 0 FALSE
		}
		
	}
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} 
		| ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax		

		/if (${${_bind}Type.Arg[${_count},|].Equal[mount]}) /varset _listout ${_listout} ${cinfo}mount\ax
		/if (${${_bind}Type.Arg[${_count},|].Equal[gather]}) /varset _listout ${_listout} ${cinfo}gather\ax
		/if (${${_bind}Type.Arg[${_count},|].Equal[fast]}) /varset _listout ${_listout} ${If[${SetFaceFast},${cgood}fast\ax,${cinfo}fast\ax]}
		/if (${${_bind}Type.Arg[${_count},|].Equal[behind]}) /varset _listout ${_listout} ${If[${SetStickBehind},${cgood}behind\ax,${cinfo}behind\ax]}
		/if (${${_bind}Type.Arg[${_count},|].Equal[goto]}) /varset _listout ${_listout} ${cinfo}goto\ax
		/if (${${_bind}Type.Arg[${_count},|].Equal[home]}) /varset _listout ${_listout} ${If[${HomeSet},${cgood}home\ax,${cinfo}home\ax]}


		/if (${${_bind}Type.Arg[${_count},|].Equal[arc]}) /varset _listout ${_listout} ${cinfo}arc\ax(${cnum}${SetFaceArc}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[leash]}) /varset _listout ${_listout} ${cinfo}leash\ax(${cnum}${HomeLeash}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[radius]}) /varset _listout ${_listout} ${cinfo}radius\ax(${cnum}${HomeRadius}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
	} 
/return



|***
 *	DES: builds your group
 *	USE: /crew [main|main2|...] {you can rename these} 
 *	INI: core.ini [group] - main|idiotgroup|rejects|....
 *  NOTE: you can rename the groups. i'd suggest using main as your primary. it is referenced elsewhere.
 *	/grouproles set [NAME] [#]
 *	1=MainTank .. 					${GroupTank}
 *	2=MainAssist ..		 			${GroupAssist}
 *	3=Puller .. 						${GroupPuller}
 *	4=MarkNpc .. 						${GroupMark}
 *	5=MasterLooter .. 			${GroupLooter}
 *	/makeleader [NAME] .. 	${GroupLeader}
 ***|
#bind crew /crew
sub Bind_crew(string _crewNumber)
 	DEBUG ${cbug}Bind_crew(${Ini[${INICore},Group,_crewNumber]})\ax

	/declare _count int local 0
	/declare _clist string local ${Ini[${INICore},Group,${_crewNumber}]}
	
	| disband raid if exists
	/if (${Raid.Members}) {
		/for _count 1 to ${_clist.Count[|]}
			/if (${Me.Name.Equal[${_clist.Arg[${_count},|]}]}) /continue
			| /if (${Bool[${Raid.Member[${_clist.Arg[${_count},|]}]}]}) /bct ${_clist.Arg[${_count},|]} //raiddisband
			/bct ${_clist.Arg[${_count},|]} //raiddisband
			/delay 2
		/next _count	
		/if (${Raid.Members}) /raiddisband
	}
	
	| drop group. we dont care if you are in one or not.
	/for _count 1 to ${_clist.Count[|]}
		/bct ${_clist.Arg[${_count},|]} //disband
		/delay 1
	/next _count			
	
	/delay 2s

	| invite the members in the list
	/for _count 1 to ${_clist.Count[|]}
		AMIDEAD
		/if (!${Bool[${Group.Member[${_clist.Arg[${_count},|]}]}]}) {
			VOUT Inviting ${sep} ${cpc}${_clist.Arg[${_count},|]}\ax
			/invite ${_clist.Arg[${_count},|]}
		}	
	/next _count
		
	/delay 1s
	/delay 12s ${Group.GroupSize} == ${_clist.Count[|]}
	/if (${Group.GroupSize} != ${_clist.Count[|]}) {
		VOUT ${owarning} ${sep} Group failed to form properly.
		/return
	} else {
		VOUT Group formed.
	}
	
	| set Group Main Assist
	/if (${Bool[${${MacroQuest.Server}Assist}]} && !${Group.Member[${${MacroQuest.Server}Assist}].Offline}) {
		/delay 5
		/if (${Group.Member[${${MacroQuest.Server}Assist}].Index}) /grouproles set ${${MacroQuest.Server}Assist} 2
	}

	| set Group Master Looter
	/if (${Bool[${${MacroQuest.Server}Looter}]} && !${Group.Member[${${MacroQuest.Server}Looter}].Offline}) {
		/delay 5
		/if (${Group.Member[${${MacroQuest.Server}Looter}].Index}) /grouproles set ${${MacroQuest.Server}Looter} 5
	}

	| set Group leader ... last to make sure all roles are set
	/if (${Bool[${${MacroQuest.Server}Leader}]} && !${Group.Member[${${MacroQuest.Server}Leader}].Offline}) {
		/delay 5
		/if (${Group.Member[${Group.Leader}].Index} != ${Group.Member[${${MacroQuest.Server}Leader}].Index}) /makeleader ${${MacroQuest.Server}Leader}
	}
	
	VOUT Leader:${cinfo}${${MacroQuest.Server}Leader}\ax ${sep} Assist:${cinfo}${${MacroQuest.Server}Assist}\ax ${sep} Looter:${cinfo}${${MacroQuest.Server}Looter}\ax
/return



|***
 *	DES: echo controls
 *	USE: /echos []
 *	INI: 
 *  NOTE:
 ***|
#bind echos /echos
sub Bind_echos(string _echoType, string _verbage)
 	DEBUG ${cbug}Bind_echos(type ${_echoType}, verbage ${_verbage})\ax

	/declare _count int local 0
	/declare _bind string local echo
	/declare _listout string local
	/declare _sep bool local FALSE
	/declare _baseEchos string local |alt|disc|item|spell

	| get help
	/if (${${_bind}Type.Find[${_${_bind}Type}]} && ${_verbage.Equal[info]}) {
		VOUT /${_bind.Lower}s ${cinfo}${_${_bind}Type}\ax ${sep} ${Ini[${INIWiki},${_bind},${_${_bind}Type}]}
		/return
	}	else /if (${_${_bind}Type.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind.Lower}s ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind.Lower}Type.Arg[${_count},|]}]}
		/next _count
		/return
	}
	
	| base echos
	/if (${Bool[${_baseEchos.Find[${_echoType}]}]}) {
		/call writeINI switchEcho${_echoType.Left[1].Upper}${_echoType.Right[-1].Lower} Echo ${If[${switchEcho${_echoType.Left[1].Upper}${_echoType.Right[-1].Lower}},FALSE,TRUE]}  
		| /call echos switch "Echo ${sep} ${_echoType.Left[1].Upper}${_echoType.Right[-1].Lower}" switchEcho${_echoType.Left[1].Upper}${_echoType.Right[-1].Lower} 0 0
		VOUT /${_bind}s ${_echoType.Left[1].Upper}${_echoType.Right[-1].Lower} ${sep} ${If[${switchEcho${_echoType.Left[1].Upper}${_echoType.Right[-1].Lower}},${oenabled},${odisabled}]}

	| set echo channel
	} else /if (${Bool[${_echoType.Equal[channel]}]}) {
		/if (${Range.Between[1,9:${Int[${_verbage}]}]}) {
			/call writeINI echoChannel Echo ${_verbage} 0 0
		} 
		VOUT /${_bind}s channel(${cnum}${echoChannel}\ax)

	| enable/disable verbose EQBC Echos
	} else /if (${Bool[${_echoType.Equal[verbose]}]}) {
		/call writeINI verbose Echo ${If[${verbose},FALSE,TRUE]}
		OUT /${_bind.Lower}s verbose ${sep} ${If[${verbose},${oenabled},${odisabled}]}

	| enable/disable BC relay
	} else /if (${Bool[${_echoType.Equal[bc]}]}) {
		/call writeINI switchEchoBC Echo ${If[${switchEchoBC},FALSE,TRUE]}
		VOUT /${_bind}s BC ${sep} ${If[${switchEchoBC},${oenabled},${odisabled}]}

	| enable/disable relay tells
	} else /if (${Bool[${_echoType.Equal[relay]}]}) {
		/if (${Bool[${_verbage}]}) {
			/call writeINI relayTells Echo ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		} else /if (!${Bool[${_verbage}]}) {
			/call writeINI relayTells Echo FALSE
		}
		VOUT /${_bind} ${cinfo}relay\ax ${sep} ${If[${Bool[${relayTells}]},${cgood}${relayTells}\ax,${odisabled}]}
	
	} else {
		/for _count 1 to ${${_bind}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax		
			/if (${${_bind}Type.Arg[${_count},|].Equal[channel]}) /varset _listout ${_listout}(${cnum}${echoChannel}\ax)

			/varset _sep TRUE
		/next _count
		VOUT /${_bind}s [${_listout}]
	} 
/return



|***
 *	DES: random insult
 *	USE: /oof
 *	INI: 
 *  NOTE:
 ***|
#bind oof /oof
sub Bind_oof(string _echoType, string _verbage)
 	DEBUG ${cbug}Bind_oof(type ${_echoType}, verbage ${_verbage})\ax
 	
 	/declare _pronoun 			string 	local FALSE
 	/declare _bitch					string 	local 
	/declare _rnd						int 		local 0
 	
 	/if (${Select[${Me.Gender},male]}) {
 		/varset _pronoun his
 	} else /if (${Select[${Me.Gender},female]}) {
 		/varset _pronoun her
 	} else /if (${Select[${Me.Gender},neuter]}) {
 		/varset _pronoun its'
 	}
 	
	:loopgetrandom
	/varset _rnd ${Math.Rand[${bitchcount}]}
	/if (!${Range.Between[1,${bitchcount}:${_rnd}]}) /goto :loopgetrandom
 	/delay 1
 	/em  looks around for the biggest fucking rock to hit ${_pronoun} head against.
	/say ${bitch${_rnd}}
/return



|***
 *	DES: lesson of the devoted
 *	USE: /lesson
 *	INI: 
 *  NOTE:
 ***|
#bind lesson /lesson
sub Bind_lesson()
 	DEBUG ${cbug}Bind_lesson()\ax
  /if (!${Me.AltAbilityReady[Lesson of the Devoted]}) /return
  /call core_cast "Lesson of the Devoted" alt 0 FALSE
/return



|***
 *	DES: grabs a ground spawn.
 *	USE: /grab NAME
 *	INI: 
 *  NOTE: max range of 20
 ***|
#bind grab /grab
sub Bind_grab(string _name)

	| typed something wrong
	/if (${_name.Equal[NULL]} || !${Bool[${_name}]} || ${_name.Equal[info]} ) {
		VOUT /grab ${cinfo}NAME\ax of mapfilter spawn name you want to pick up.
		/return
	}
	
	| trash pick up.
	/if (${Ground[${_name}].Distance3D} > 20) {
		VOUT ${cinfo}${_name}\ax is ${Ground[${_name}].Distance3D}. Needs to be within 20.
		/return
	} else /if (${Ground[${_name}].Distance3D} <= 20 ) {
		/invoke ${Ground[${_name}].Grab}
		/delay 1s ${Cursor.ID}
		VOUT /grab ${sep} ${cinfo}${Cursor.Name}\ax
	}
/return




|***
 *	DES: navigates tot he name.target
 *	USE: /navto [NAME]
 *	INI: 
 *  NOTE: max range of 20
 ***|
#bind nav_to_target /navto
sub Bind_nav_to_target (string _name)
	/call Bind_setMovementControls goto ${_name}
/return