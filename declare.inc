|***
 *	declare.inc
 *	Core Declares common and class specific.
 *	All manual forced events.
 *	All created alias', binds and /bc commands
 *	All INI edits
 *
 ***|


|***
 *	DES: adjust the related ini
 *	USE: /call writeINI [variable] [INISection] [value]
 *	INI: 
 *  NOTE: 
 ***|
sub writeINI(string _varName, string _section, string _value)
	DEBUG ${cbug}writeINI([${_section}] -> ${_varName.Left[1].Upper}${_varName.Right[-1]} -> ${_value})\ax 
	/if (${Defined[${_varName}]} && ${Ini[${INIClass},${_section},${_varName},"${_value}"].NotEqual[${_value}]}) {
		/varset ${_varName} ${_value}	
	 	/ini "${INIClass}" ${_section} "${_varName.Left[1].Upper}${_varName.Right[-1]}" "${_value}"
	 	| /ini ${INIClass} ${_section} ${_varName} "${_value}"
	}
/return



|***
 *	DES: adjust the related ini
 *	USE: /call Event_SetaliasVar NA [variable] [INISection] [value]
 *	INI: 
 *  NOTE: 
 ***|
#event SetaliasVar 			"[MQ2] - Core: #1# -> [#2#] -> #3#"
sub Event_SetaliasVar(string line, string _varName, string _section, string _value)
	/call writeINI ${_varName} ${_section} ${_value}
/return



|***
 *	DES: creates timers
 *	USE: /call declareTimer [NAME] [INT|BOOL|STRING|FLOAT] [INISECTION] [value] [timer_check_blah_blah]
 *	INI: 
 *  NOTE: 
 ***|
sub setTimer(string _name, string _type, string _section, string _value, string _my_timer)
	| DEBUG setTimer(name:${_name}, type:${_type}, INIsection:${_section}, value:${_value}, Timer_Name:${_my_timer})
	/if (!${Bool[${_name}]} || !${Bool[${_type}]} || !${Bool[${_section}]} || !${Bool[${_my_timer}]}) {
		/if (!${Bool[${_name}]}) OUT ${ocaution} ${sep} BAD variable Name. 
		/if (!${Bool[${_type}]}) OUT ${ocaution} ${sep} BAD variable Type. 
		/if (!${Bool[${_section}]}) OUT ${ocaution} ${sep} BAD INI Section.
		/if (!${Bool[${_my_timer}]}) OUT ${ocaution} ${sep} BAD Timer Name. 
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
sub DeclareIniVar(string _name, string _type, string _section, string _value)
	| /echo DeclareIniVar(_name:${_name}, type:${_type}, INIsection:${_section}, value:${_value})
	/if (!${Bool[${_name}]} || !${Bool[${_type}]} || !${Bool[${_section}]}) {
		NOUT ${owarning} ${sep} INVALID INI SETTING/STRING [name:${_name}/type:${_type}/section${_section}] --\ax
		/return
	}
	/if (!${Defined[${_name}]}) /declare ${_name} ${_type} outer
	/varset ${_name} ${Ini[${INIClass},${_section},${_name.Left[1].Upper}${_name.Right[-1]},"${_value}"]}
	/if (!${Bool[${Ini[${INIClass},${_section}].Find[${_name}]}]}) {
		/ini "${INIClass}" "${_section}" "${_name.Left[1].Upper}${_name.Right[-1]}" "${${_name}}" "${${_value}}"

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
	/varset ${name} ${Ini[${INICore},${section},${name.Left[1].Upper}${name.Right[-1]},"${value}"]}
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
		/if (${Bool[${Plugin[MQ2EQBC]}]} && ${EQBC.Connected}) /bc zonereset
	}
/return 
 


|***
 *	DES: check/create the zone base INI entries.
 *	USE: /call check_zone_INI
 *	INI: 
 *  NOTE: 
 ***| 
sub check_zone_INI(string _sender)
	DEBUG ${cbug}check_zone_INI()\ax 
	/if (${Bool[${_sender}]} && !${Spawn[pc ${_sender}].ID}) /return

	/declare _count 						int local	
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
	/declare coreBuild	 														int			outer 20170921

	/declare _count 																int 		local
	/declare combat																	string	outer
		
	/declare isValidTarget													bool		outer FALSE
	/declare canCast																bool		outer	TRUE

	| HUD controls
	/declare hudType																string	outer |time|update|broadcasts|delete
	/declare hudNotice															string	outer FALSE
	/declare hudWarning															string	outer	FALSE
	/declare hudTarNotice														string	outer FALSE
	/declare hudBroadcast														string	outer FALSE
	/declare tmpMode																string	outer FALSE
	/declare hudDeathCount													int			outer	0
		
	| /declare hudIV																	bool		outer FALSE
	| /declare hudIVU																	bool		outer FALSE
	
	/call DeclareCoreIniVar HudName									string	General Core
	/call DeclareCoreIniVar HudType									string	General normal
	/call DeclareIniVar HUDTime											bool 		Common TRUE
	/call DeclareIniVar setBroadcast								string	Common TRUE	


	| Base Environment settings
	/declare noInvis 																bool 		outer TRUE
	/declare noFeigning 														bool 		outer TRUE
	/declare Target2Kill														int			outer 0
	/declare UseRange																bool		outer FALSE
	/declare currentLootCount												int			outer	0
	/declare markerDead															bool		outer FALSE



	/call DeclareCoreIniVar plist										string	Plugin |MQ2Cast|MQ2EQBC|MQ2Exchange|MQ2MoveUtils|MQ2Netbots|MQ2Debuffs
	/call DeclareCoreIniVar EQBCServer							string	General 127.0.0.1
	/call DeclareCoreIniVar EQBCPort								string	General 2112
	/call DeclareCoreIniVar mqlog										string	General FALSE
	/call DeclareCoreIniVar minMobPullLvl						int			General 1
	/call DeclareCoreIniVar maxMobPullLvl						int			General 150
	/call DeclareIniVar verbose											bool		Common TRUE

		
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
	/declare ctar																		string	outer \a-w
	/declare cpc																		string	outer \a-w
	/declare csp																		string	outer \a-r
	/declare cbad																		string	outer \ar
	/declare cgood																	string	outer \ag
	/declare c-																			string	outer \at
	/declare cbug																		string	outer \at
	/declare cinfo																	string	outer \aw
	/declare citem																	string	outer \au
	/declare sep																		string	outer \at..\ax
	/declare sepO																		string	outer \at[\ax
	/declare sepC																		string	outer \at]\ax
	/declare oenabled																string	outer ${cgood}Enabled\ax
	/declare odisabled															string	outer ${cbad}DISABLED\ax
	/declare owarning																string	outer ${cbad}WARNING\ax
	/declare ofatal																	string	outer ${cbad}FATAL ERROR\ax ${sep} ${cbad}ENDING MACRO\ax
	/declare ocaution																string	outer ${cbad}CAUTION\ax
	/declare otrue																	string	outer ${cgood}TRUE\ax
	/declare ofalse																	string	outer ${cbad}FALSE\ax
	/declare odestroy																string	outer ${cbad}DESTROY\ax
	/declare oleave																	string	outer ${cinfo}Leave\ax
	/declare oannounce															string	outer \amAnnounce\ax
	/declare oadd																		string	outer ${cgood}Add\ax
	/declare okeep																	string	outer ${cgood}Keep\ax
	/declare onotice																string	outer \ayNotice\ax
	| /declare prelink												string	outer "3Rorcex^11000004^0^1^1362119591^10000^'"
 	| /declare postlink												string	outer ""	
 	/declare ocombat																string	outer "\amCombat\ax"	
	/declare oremove																string	outer "${cbad}Removing\ax"

 	 	
	| various lists for .. you know..lists and stuff.
	/declare MeleeList    		  										string 	outer |WAR|ROG|BST|MNK|SHD|PAL|BER|BRD|RNG
	/declare CastList																string 	outer |BST|SHD|PAL|RNG|CLR|DRU|SHM|ENC|WIZ|NEC|MAG
	/declare PetList																string 	outer |ENC|DRU|SHM|SHD|CLR|WIZ|RNG|BRD|BST|MAG
	/declare HealerList															string 	outer |CLR|DRU|SHM
	/declare RezerList															string 	outer |CLR|DRU|SHM|PAL
	/declare AllClassList														string 	outer |WAR|SHD|PAL|BER|ROG|MNK|RNG|BST|BRD|NEC|ENC|WIZ|MAG|CLR|SHM|DRU
	/declare AASnareList														string 	outer |RNG|DRU|SHD|NEC|ROG|WAR|WIZ|MNK|PAL
	/declare AAEradicateList												string 	outer |RNG|DRU|NEC|WIZ|ENC|MAG
		
		
	| core control
	/declare coreType 															string 	outer |auto|control|echo|expadjust|expmaintain|explevel|fixini|invite|killcode|dbcash|raidinvite|relay|reset|rest|verbose|whitewash
	/declare coreObscure														bool		outer FALSE
	/call DeclareIniVar relayTells									string	Common FALSE
	/call DeclareIniVar coreAuto				 						bool	 	Common FALSE
	/call DeclareCoreIniVar remoteKillKey						string	General "CHANGE ME" 
	/call DeclareCoreIniVar ${Me.DisplayName}				int			DeathCount 0
	/call DeclareCoreIniVar invitelist							string	No FALSE
	/call DeclareCoreIniVar safelist								string	General |FALSE|FALSE
	/call DeclareIniVar toonControl			 						string	Common FALSE
	/call DeclareIniVar autoRest 										int 		Common 90
	/call DeclareIniVar echoChannel									string	Common FALSE 
	/call DeclareIniVar AutoExpAdjust 							bool 		Common TRUE
	/call DeclareIniVar MaintExpLvl   							int 		Common 99
	/call DeclareIniVar MaxLevel      							int 		Common 105


	| OCD Shit - 
	/call DeclareCoreIniVar setMQFontSize						int			General 2
	/call DeclareCoreIniVar setMQCaptions						string	General off

	/call DeclareCoreIniVar setTGB									string	General on
	/call DeclareCoreIniVar setAutoAssist						string	General off
	
	| buff queue variables
	/call DeclareCoreIniVar queueCountMax						int			General 10
	/declare queueCount             							 	int  		outer 0
	/declare queueBuffID[8] 												int  		outer
	/declare queueList[${queueCountMax},2] 				 	int  		outer
		
	| /declare status_check_string						string	outer NULL
	
	/call DeclareCoreIniVar main										string	Group |FALSE|FALSE
	/call DeclareCoreIniVar GroupLeader							string	Group FALSE
	/call DeclareCoreIniVar GroupTank								string	Group FALSE
	/call DeclareCoreIniVar GroupAssist							string	Group FALSE
	/call DeclareCoreIniVar GroupPuller							string	Group FALSE
	/call DeclareCoreIniVar GroupLooter							string	Group FALSE
	/call DeclareCoreIniVar GroupMark								string	Group FALSE

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
	
	/declare bitchcount															int			outer	20	
	/declare bitch1																	string	outer	"Seriously... Again..."
	/declare bitch2																	string	outer	"Did your parents ever ask you to run away from home?"
	/declare bitch3																	string	outer	"Aha, I see the Fuck-Up Fairy has visited us again!"
	/declare bitch4																	string	outer	"Will trade a Halfling for a rez, or three Gn0mes."
	/declare bitch5																	string	outer	"I'm sorry I bit you... pulled your hair... and punched you in the face..."
	/declare bitch6																	string	outer	" Go ahead, tell them everything you know. It'll only take 10 seconds."
	/declare bitch7																	string	outer	"Sometimes the first step to forgiveness, is realising the other person was born an idiot."
	/declare bitch8																	string	outer	"I'm getting Disc lag . . . again."
	/declare bitch9																	string	outer	"oh man....i figured out why i was lagging the other day...i look in my case and my kids knocked off my cpu fan"
	/declare bitch10																string	outer	"I'm jealous of all the people that haven't met you!"
	/declare bitch11																string	outer	"You're slower than a herd of turtles stampeding through peanut butter."
	/declare bitch12																string	outer	"Oh good, my dog found a chainsaw."
	/declare bitch13																string	outer	"You are not as bad as people say, you are much, much worse."
	/declare bitch14																string	outer	"Stupidity is not a crime so you are free to go."
	/declare bitch15																string	outer "AFK, playing Pokemon. More productive then this shit."
	/declare bitch16																string	outer	"All I did was move the mouse over it."
	/declare bitch17																string	outer	"You sound reasonable. It must be time to up my medication!"
	/declare bitch18																string	outer	"I'll never forget the first time we met. Although, I'll keep trying."
	/declare bitch19																string	outer	"Roses are red. Violets are blue. You're an idiot."
	/declare bitch20																string	outer	"Sometimes, when I close my eyes, I... can't see you."
		
	| hide switches for /hidecorpse command
	/call DeclareCoreIniVar hideNone								bool		hide FALSE
	/call DeclareCoreIniVar hideAll									bool		hide FALSE
	/call DeclareCoreIniVar hideAlways							bool		hide FALSE
	/call DeclareCoreIniVar hideLooted							bool		hide FALSE
	/call DeclareCoreIniVar hideAllButGroup					bool		hide FALSE
	/call DeclareCoreIniVar hideNPC									bool		hide FALSE
	/call DeclareCoreIniVar hideLooted							bool		hide FALSE
	/call DeclareCoreIniVar hideLisdted							bool		hide FALSE
	/call DeclareCoreIniVar hideListedUnlocked			bool		hide FALSE
	
	
	| non-standard mode control
	/declare modeType																string	outer |stop|agro|drag|fish|forage|harvest|hunt|lush|petfarm|scorch|tradeskill
	/declare setMode																bool		outer FALSE
	/declare switchModeStop													bool		outer FALSE
	/declare switchModeAgro													bool		outer FALSE
	/declare setModeType														string	outer FALSE
	/declare setModeCount														int			outer 0
	/call DeclareIniVar setModeAgro									int			Common FALSE 
	/declare switchModeDrag													bool		outer FALSE 
	/declare switchModeForage												bool		outer FALSE
	/declare switchModePetfarm											bool		outer FALSE
	/declare switchModeScorch												bool		outer FALSE
	/declare switchModeHunt													bool		outer FALSE
	/declare switchModeFish													bool		outer FALSE
	/declare switchModeSkill												bool		outer FALSE
	/declare switchModeHarvest											bool		outer FALSE
	/declare switchModeTradeskill										bool		outer FALSE
	/declare switchModeLush													bool		outer FALSE

	| burns
	/declare burnType																string	outer |auto|count|engageat|force
	/declare burnForce															bool		outer FALSE
	/call DeclareIniVar burnAuto										bool		common FALSE
	/call DeclareIniVar burnEngageat								int			common 80
	/call DeclareIniVar burnCount										int			common 10
	

	| /declare 	pre 													string 	outer ${Ini[${INICore},fakelink,pre]}
	| /declare 	post 													string 	outer ${Ini[${INICore},fakelink,post]}
	
	
	| core.zone.ini lists
	/declare ZoneFileList														string 	outer |NameList|ExcludeList|NoMezzList|RangeList|NoCastList|NoMeleeList|HuntMobList|CharmMobList|NoFireList|NoPoisonList|NoMagicList|NoCorruptionList|NoColdList|NoDiseaseList|HarvestList
	/declare RangeList		   												string 	outer FALSE
	/declare ExcludeList    												string 	outer FALSE
	/declare NameList       												string 	outer FALSE
	/declare NoMeleeList     		  									string 	outer FALSE
	/declare NoCastList       											string 	outer FALSE
	/declare NoMezzList       											string 	outer FALSE
	/declare NoDiseaseList       										string 	outer FALSE
	/declare NoMagicList       											string 	outer FALSE
	/declare NoPoisonList       										string 	outer FALSE
	/declare NoFireList       											string 	outer FALSE
	/declare NoCorruptionList      			 						string 	outer FALSE
	/declare NoColdList       											string 	outer FALSE
	/declare CharmMobList      											string 	outer FALSE
	/declare SafeZone		       											string 	outer FALSE
	/declare FullZoneName      											string 	outer FALSE
	/declare HuntMobList       											string 	outer FALSE
	/declare HarvestList       											string 	outer FALSE
	

	/call DeclareIniVar autoControl			 						bool		Common FALSE 
	/call DeclareIniVar noCastWith									string	Common "|Quick Defense|Protection of Bayle|Interrogation Intimidation|Energy Charge|Cloud of Terror|Faith"


	/call DeclareIniVar shrinkItem 									string 	Common "Ring of the Ancients"
	/call DeclareIniVar IllusionItem								string	Common FALSE
	/call DeclareIniVar IllusionBuff								string	Common FALSE
	/call DeclareIniVar GTFO												string	Common FALSE
	/call DeclareIniVar switchAApicker							bool 		Common FALSE
	/call DeclareIniVar mount												string	Common FALSE
	/call DeclareIniVar RangeItem										string	Common FALSE		
	/call DeclareIniVar RangeItemSummon							string	Common FALSE
	/call DeclareIniVar autoInventoryItem						string	Common "|Summoned: Giant Modulation Shard"
	/call DeclareIniVar stoponDS										bool		Common TRUE		
	/call DeclareIniVar useGlyph										string	Common "Glyph of the Cataclysm"
	/call DeclareIniVar ResidentMage								string	Common |FALSE|FALSE
	/call DeclareIniVar ResidentrDruid							string	Common |FALSE|FALSE
	/call DeclareIniVar ResidentWizard							string	Common |FALSE|FALSE
	/call DeclareIniVar ClassChannel								int			Common FALSE	





	| spire control
	/declare typeSpire															string	outer |0|1|2|3
	/call DeclareIniVar spireRaid										int 	Buffs 0
	/call DeclareIniVar spireGroup									int 	Buffs 0
	

	| Cleric
	/declare CLRspire1															string	outer "Fundament: First Spire of Divinity"
	/declare CLRspire2															string	outer "Fundament: Second Spire of Divinity"
	/declare CLRspire3															string	outer "Fundament: Third Spire of Divinity"
	/declare CLRsynergyAA														string	outer "Templar's Synergy"

	| Druid
	/declare DRUspire1															string	outer "Fundament: First Spire of Nature"
	/declare DRUspire2															string	outer "Fundament: Second Spire of Nature"
	/declare DRUspire3															string	outer "Fundament: Third Spire of Nature"
	/declare DRUsynergyAA														string	outer "Preserver's Synergy"

	| Shaman
	/declare SHMspire1															string	outer "Fundament: First Spire of Ancestors"
	/declare SHMspire2															string	outer "Fundament: Second Spire of Ancestors"
	/declare SHMspire3															string	outer "Fundament: Third Spire of Ancestors"
	/declare SHMsynergyAA														string	outer "Luminary's Synergy"

	| Warrior
	/declare WARspire1													string	outer "Fundament: First Spire of the Warlord"
	/declare WARspire2													string	outer "Fundament: Second Spire of the Warlord"
	/declare WARspire3													string	outer "Fundament: Third Spire of the Warlord"
	/declare WARsynergyAA												string	outer "Myrmidon's Synergy"

	| Shadow Knight
	/declare SHDspire1													string	outer "Fundament: First Spire of the Reavers"
	/declare SHDspire2													string	outer "Fundament: Second Spire of the Reavers"
	/declare SHDspire3													string	outer "Fundament: Third Spire of the Reavers"
	/declare SHDsynergyAA												string	outer "Knight's Synergy"

	| Paladin
	/declare PALspire1													string	outer "Fundament: First Spire of Holiness"
	/declare PALspire2													string	outer "Fundament: Second Spire of Holiness"
	/declare PALspire3													string	outer "Fundament: Third Spire of Holiness"
	/declare PALsynergyAA												string	outer "Knight's Synergy"

	| Magician
	/declare MAGspire1													string	outer "Fundament: First Spire of the Elements"
	/declare MAGspire2													string	outer "Fundament: Second Spire of the Elements"
	/declare MAGspire3													string	outer "Fundament: Third Spire of the Elements"
	/declare MAGsynergyAA												string	outer "Conjurer's Synergy"

	| Enchanter	
	/declare ENCspire1													string	outer "Fundament: First Spire of Enchantment"
	/declare ENCspire2													string	outer "Fundament: Second Spire of Enchantment"
	/declare ENCspire3													string	outer "Fundament: Third Spire of Enchantment"
	/declare ENCIoG															string	outer "Illusions of Grandeur"
	/declare ENCMC															string	outer "Mental Contortion"
	/declare ENCsynergyAA												string	outer "Begulier's Synergy"
	
	| Wizard 
	/declare WIZspire1													string	outer "Fundament: First Spire of the Arcanum"
	/declare WIZspire2													string	outer "Fundament: Second Spire of the Arcanum"
	/declare WIZspire3													string	outer "Fundament: Third Spire of the Arcanum"
	/declare WIZmannaburn												string	outer "Manna Burn"
	/declare WIZsynergyAA												string	outer "Evoker's Synergy"

	| Necromancer
	/declare NECspire1													string	outer "Fundament: First Spire of Necromancy"
	/declare NECspire2													string	outer "Fundament: Second Spire of Necromancy"
	/declare NECspire3													string	outer "Fundament: Third Spire of Necromancy"
	/declare NECsynergyAA												string	outer "Defiler's Synergy"

	| Ranger
	/declare RNGspire1													string	outer "Fundament: First Spire of the Pathfinders"
	/declare RNGspire2													string	outer "Fundament: Second Spire of the Pathfinders"
	/declare RNGspire3													string	outer "Fundament: Third Spire of the Pathfinders"
	/declare RNGauspice													string	outer "Auspice of the Hunter"
	/declare RNGcheetah													string	outer "Scarlet Cheetah's Fang"
	/declare RNGgroupguardian										string	outer "Group Guardian of the Forest"
	/declare RNGsynergyAA												string	outer "Outrider's Synergy"

	| Bard
	/declare BRDspire1													string	outer "Fundament: First Spire of the Minstrel"
	/declare BRDspire2													string	outer "Fundament: Second Spire of the Minstrel"
	/declare BRDspire3													string	outer "Fundament: Third Spire of the Minstrel"
	/declare BRDsynergyAA												string	outer "Troubadour's Synergy"

	| Monk
	/declare MONspire1													string	outer "Fundament: First Spire of the Sensei"
	/declare MONspire2													string	outer "Fundament: Second Spire of the Sensei"
	/declare MONspire3													string	outer "Fundament: Third Spire of the Sensei"
	/declare MONsynergyAA												string	outer 

	| Beastlord
	/declare BSTspire1													string	outer "Fundament: First Spire of the Savage Lord"
	/declare BSTspire2													string	outer "Fundament: Second Spire of the Savage Lord"
	/declare BSTspire3													string	outer "Fundament: Third Spire of the Savage Lord"
	/declare BSTsynergyAA												string	outer "Primalist's Synergy"

	| Berzerker
	/declare BERspire1													string	outer "Fundament: First Spire of Savagery"
	/declare BERspire2													string	outer "Fundament: Second Spire of Savagery"
	/declare BERspire3													string	outer "Fundament: Third Spire of Savagery"
	/declare BERsynergyAA												string	outer "Fury's Synergy"

	| Rogue
	/declare ROGspire1													string	outer "Fundament: First Spire of the Rake"
	/declare ROGspire2													string	outer "Fundament: Second Spire of the Rake"
	/declare ROGspire3													string	outer "Fundament: Third Spire of the Rake"
	/declare ROGsynergyAA												string	outer "Blackguard's Synergy"




	| combat controls
	/declare ccType																	string	outer |agro|combatdistance|dual|engage|force|heal|MA1|MA2|melee|nuke|pull|range|saferadius|smartengage|zradius
	/call DeclareIniVar combat					 						string	Common melee
	/call DeclareIniVar MA1			 										string 	Common FALSE
	/call DeclareIniVar MA2			 										string 	Common FALSE
	/call DeclareIniVar MobAgro											int 		Common 10
	/call DeclareIniVar engageAt 										int 		Common 98
	/call DeclareIniVar smartengageAt 							int 		Common 98
	/call DeclareIniVar	useSmartAssist1							bool		Common FALSE 
	/call DeclareIniVar	useSmartAssist2							bool		Common FALSE
	/call DeclareIniVar	useSmartAssistG							bool		Common FALSE
	/call DeclareIniVar switchAgro									bool		Common FALSE 
	/call DeclareIniVar switchPull									bool		Common FALSE
	/call DeclareIniVar CombatDistance							int			Movement 15
	/call DeclareCoreIniVar combatRadius						string	General 30
	/call DeclareIniVar zradius											int			Common 50	
		
			
	| rez	control
  /declare 		rezType													string	outer |stop|combat|emergency|everyone|give|pct|range|take
  /declare 		rezPct													string	outer |10|20|35|50|60|75|90|93|96
	/declare 		flagCallNeedRez									bool		outer FALSE
	/declare		switchRez												bool		outer FALSE
	/call DeclareIniVar switchRezTake 							bool 		Heal TRUE 
	/call DeclareIniVar switchRezEmergency					bool		Heal FALSE
	/call DeclareIniVar minRezPct										int			Common 90
	/call DeclareIniVar switchRezGive	 							bool 		Heal FALSE 
	/call DeclareIniVar switchRezCombat 						bool 		Heal FALSE 
	/call DeclareIniVar switchRezEveryone 					bool 		Heal FALSE 
	/call DeclareIniVar maxRezRange				 					int 		Heal 60 

	| /if (${Select[${Me.Class.ShortName},CLR,PAL,DRU,NEC,SHM]}) {
	| }
	/call DeclareIniVar RezTokenList								string 	Heal |WAR|SHD|PAL|BER|ROG|MNK|RNG|BST|BRD|NEC|ENC|WIZ|MAG|CLR|SHM|DRU	
	
	| watch spawn stuff
	/declare hudwatchspawn													string	outer FALSE
	/declare watchSpawnCount												int			outer	FALSE
	/call DeclareIniVar watchSpawn									string	Common FALSE 
		
	| tie control
  /declare tcType																	string	outer |name|time|distance
	/call DeclareIniVar SetTieToon 									string 	Movement FALSE
	/declare SetTie       													bool 		outer FALSE
	/call DeclareIniVar SetTieDistance 							int 		Movement 15 
   
 	| Movement control
  /declare movementType														string	outer |arc|behind|fast|home|leash|radius
	/call DeclareIniVar HomeSet											bool		Movement FALSE
	/call DeclareIniVar HomeRadius									int 		Movement 5
	/call DeclareIniVar HomeLeash										int 		Movement 40
	/call DeclareIniVar HomeDelayMin								int 		Movement 100
	/call DeclareIniVar HomeDelayMax								int 		Movement 500	
	/call DeclareIniVar SetStickBehind 							bool 		Movement FALSE
	/call DeclareIniVar SetFaceFast									bool		Movement FALSE
	/call DeclareIniVar SetFaceArc									int			Movement 30
	/declare tempHomeSet														bool		outer FALSE	
	/declare tempsnapstuck													bool		outer	FALSE  
	
	| debuff switches by class	
  /declare deBuffType															string	outer |stop|mezz|malo|tash|cripple|slow|eradicate|snare
	/declare switchdeBuff														bool		outer FALSE
	/declare switchdeBuffStop												bool		outer FALSE
	/call DeclareIniVar switchdeBuffCripple					bool		Buffs FALSE
	/call DeclareIniVar switchdeBuffMezz						bool		Buffs FALSE
	/call DeclareIniVar switchdeBuffMalo						bool		Buffs FALSE
	/call DeclareIniVar switchdeBuffTash						bool		Buffs FALSE
	/call DeclareIniVar switchdeBuffSlow						bool		Buffs FALSE
	/call DeclareIniVar switchdeBuffEradicate				bool		Buffs FALSE
	/call DeclareIniVar switchdeBuffSnare						bool		Buffs FALSE
	
	/call DeclareIniVar doclickItem									int			Items 0 
	/call DeclareIniVar clickItem1									string	Items FALSE 
	/call DeclareIniVar clickItem2									string	Items FALSE 
	/call DeclareIniVar clickItem3									string	Items FALSE 
	/call DeclareIniVar clickItem4									string	Items FALSE 
		
	/call DeclareIniVar doclickItemBurn							int			Items 0 
	/call DeclareIniVar clickItemBurn1							string	Items FALSE 
	/call DeclareIniVar clickItemBurn2							string	Items FALSE 
	/call DeclareIniVar clickItemBurn3							string	Items FALSE 
	/call DeclareIniVar clickItemBurn4							string	Items FALSE 	
	
	/call DeclareIniVar Breastplate1		 						string	Items FALSE	
	/call DeclareIniVar Breastplate1effect					string	Items FALSE
	/call DeclareIniVar Breastplate2		 						string	Items FALSE
	/call DeclareIniVar Breastplate2effect					string	Items FALSE	
	/call DeclareIniVar Epic												string	Items FALSE	
	/call DeclareIniVar CircleofPower								string	Items FALSE	
	/call DeclareIniVar CircleofLife								string	Items FALSE	
	/call DeclareIniVar CircleofMana								string	Items FALSE	
	
	
	/call DeclareIniVar doclickOdds									int			Odds 0 
	/call DeclareIniVar clickOddItem1								string	Odds FALSE 
	/call DeclareIniVar clickOddItem2								string	Odds FALSE 
	/call DeclareIniVar clickOddItem3								string	Odds FALSE 
	/call DeclareIniVar clickOddItem4								string	Odds FALSE 	
	
	| aas that you want to always keep going
	/call DeclareIniVar AANow1											string	AANow FALSE 
	/call DeclareIniVar AANow2											string	AANow FALSE 

	| discs that you want to always keep going
	/call DeclareIniVar DiscNow1										string	DiscNow FALSE 
	/call DeclareIniVar DiscNow2										string	DiscNow FALSE 


	| overrides
  /declare overrideType												string	outer |stop|safezone|los|exclude|name|engageat|loot|safelist
	/declare switchOverride											bool		outer FALSE
	/declare switchOverrideStop									bool		outer FALSE
  /call DeclareIniVar switchOverrideLos						bool		Override FALSE
  | /call DeclareIniVar switchOverridePve					bool		Override FALSE
  /call DeclareIniVar switchOverrideSafezone			bool		Override FALSE
  /call DeclareIniVar switchOverrideExclude				bool		Override FALSE
  /call DeclareIniVar switchOverrideName					bool		Override FALSE
  /call DeclareIniVar switchOverrideEngageat			bool		Override FALSE
  /call DeclareIniVar switchOverrideLoot					bool		Override FALSE
  /call DeclareIniVar switchOverrideSafelist			bool		Override FALSE


	| General class combat skills
	/call DeclareIniVar kick												bool		meleedisc FALSE
	/call DeclareIniVar bash												bool		meleedisc FALSE
	/call DeclareIniVar taunt												bool		meleedisc FALSE
	/call DeclareIniVar disarm											bool		meleedisc FALSE
	/if (${Select[${Me.Class.ShortName},BER]}) {
		/call DeclareIniVar frenzy										bool		meleedisc FALSE
	}
	/if (${Select[${Me.Class.ShortName},ROG]}) {
		/declare flagSoS													bool	outer FALSE
		/call DeclareIniVar backstab									bool		meleedisc FALSE
	}
	/if (${Select[${Me.Class.ShortName},BRD]}) {
		/call DeclareIniVar seloskick									bool		meleedisc FALSE
	}
	/if (${Select[${Me.Class.ShortName},MNK]}) {
		/call DeclareIniVar dragonpunch								bool		meleedisc FALSE
		/call DeclareIniVar eaglestrike								bool		meleedisc FALSE
		/call DeclareIniVar tigerclaw									bool		meleedisc FALSE
		/call DeclareIniVar flyingkick								bool		meleedisc FALSE
		/call DeclareIniVar roundkick									bool		meleedisc FALSE
	}

	| food/drink
	/call DeclareIniVar clickFoodDispenser					string 	Meal FALSE
	/call DeclareIniVar clickFoodItem								string 	Meal FALSE
	/call DeclareIniVar clickFoodCount							int		 	Meal 0
	/call DeclareIniVar clickDrinkDispenser					string 	Meal FALSE
	/call DeclareIniVar clickDrinkItem							string 	Meal FALSE
	/call DeclareIniVar clickDrinkCount							int		 	Meal 0
	/call DeclareIniVar tClick_FoodDrink						string	Timer 10m
		/call create_timer timer_click_food ${tClick_FoodDrink}
		/call create_timer timer_click_drink ${tClick_FoodDrink}


	/if (${Me.Class.PetClass} || ${Select[${Me.Class.ShortName},ENC,DRU,SHM,SHD]}) {

		/call DeclareIniVar PetTank 									bool 		Pet FALSE
		/call DeclareIniVar PetTypeSpell 							string 	Pet FALSE
		/call DeclareIniVar useSummonWeps 						bool 		Pet TRUE
		/call DeclareIniVar EpicClick 								string 	Pet FALSE
		/call DeclareIniVar EpicPetBuff 							string 	Pet FALSE
		/call DeclareIniVar summonedWepSpell 					string 	Pet "Grant Thassis' Armaments"
		/call DeclareIniVar summonedWepClosedBag 			string 	Pet "Folded Pack of Thalassic Armaments"
		/call DeclareIniVar summonedFocusSpell 				string 	Pet "Grant Calix's Heirlooms"
		/call DeclareIniVar summonedArmorSpell 				string 	Pet "Grant Thassis' Plate"
		/call DeclareIniVar summonedMaskSpell 				string 	Pet "Grant Visor of Gobeker"
		/call DeclareIniVar summonedWepName						string 	Pet "Summoned: Thalassic Fireblade"
		/call DeclareIniVar shrinkPet 								string 	Pet	"Tiny Companion"
		/call DeclareIniVar PetHealSpell 							string 	Pet FALSE
		/call DeclareIniVar PetHealType 							string 	Pet "gem1"
		/call DeclareIniVar PetRenewalSpell 					string 	Pet FALSE
		/call DeclareIniVar PetRenewalType 						string 	Pet "gem2"
		/call DeclareIniVar PetIllusion		 						string 	Pet FALSE
		/call DeclareIniVar PetIllusionBuff						string 	Pet FALSE
		/call DeclareIniVar PetAAHeal									string 	Pet "Replenish Companion"
		/call DeclareIniVar PetBagSlotNumber					int 		Pet 8
		
		/call DeclareIniVar PetBuffCount 							int 		Pet 0
		
		| 10 pet buffs
		/for _count 1 to 10
			/call DeclareIniVar PetBuff${_count} 				string 	Pet FALSE
			/call DeclareIniVar PetBuff${_count}chk			string 	Pet FALSE
		/next _count
	}	
	
	/if (${Select[${Me.Class.ShortName},MAG,ENC,CLR,NEC,WIZ,SHM,DRU]}) { 
		/call DeclareIniVar TwincastSpell 						string 	Spells FALSE
		/call DeclareIniVar TwincastType 							string 	Spells FALSE
		/call DeclareInivar GatherSpell 							string 	Spells FALSE
		/call DeclareIniVar SilentCast	 							string	Spells FALSE
	}

	| buffs
	/call DeclareIniVar BuffRemove									string	Buffs "|Illusion: Gnoll Reaver|Illusion: Darkfell Gnoll Reaver|Illusion: Blackburrow Gnoll Reaver|Illusion: Mucktail Gnoll Reaver|Illusion: Direwind Gnoll Reaver|Illusion: Human Pirate|Illusion: Ogre Pirate"

  /declare 		buffType												string	outer |stop|self|item|combat|bc|raid|tell|pet
	/declare		switchBuff											bool		outer FALSE
	/declare		switchBuffStop									bool		outer FALSE
	| /declare		switchBuffType							string	outer FALSE

	/call DeclareIniVar switchBuffItem	 						bool		Buffs FALSE 
	/call DeclareIniVar switchBuffCombat 						bool		Buffs FALSE 
	/call DeclareIniVar switchBuffPet 							bool 		Pet FALSE 

	
	/if (${Me.Class.CanCast} || ${Select[${Me.Class.ShortName},ROG]}) {
		/call DeclareIniVar buffGem 									string 	Spells "gem12"
		/call DeclareIniVar GoMLevel									int			Spells 105
		/call DeclareIniVar GoMNuke										string	Spells FALSE
		/call DeclareIniVar GoMNuke2									string	Spells FALSE
		/call DeclareIniVar switchBuffSelf	 					bool		Buffs FALSE 
		/call DeclareIniVar switchBuffBc							bool		Buffs FALSE
		/call DeclareIniVar switchBuffRaid						bool		Buffs FALSE
		/call DeclareIniVar switchBuffTell						bool		Buffs FALSE

		| bc/tell buffs
		/declare BuffCount												int 		outer 15
		/for _count 1 to ${BuffCount}
			/call DeclareIniVar Buff${_count}							string 	Buffs FALSE
			/call DeclareIniVar Buff${_count}chk					string 	Buffs FALSE
			/call DeclareIniVar Buff${_count}self					string 	Buffs FALSE
			/call DeclareIniVar Buff${_count}Alias				string 	Buffs |FALSE|FALSE 
			/call DeclareIniVar BCBuff${_count}Class			string 	Buffs |WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM
			/call DeclareIniVar RaidBuff${_count}Class		string 	Buffs |WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM
			/call DeclareIniVar Buff${_count}noBCinRaid		bool	 	Buffs FALSE
		/next _count
	}

	/call DeclareIniVar Aura1Spell 									string 	Aura FALSE
	/call DeclareIniVar Aura1Buff 									string 	Aura FALSE  
	/call DeclareIniVar Aura2Spell 									string 	Aura FALSE
	/call DeclareIniVar Aura2Buff 									string 	Aura FALSE

	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,PAL]}) {
		/declare callID												int 		outer 0

		/call DeclareIniVar PetPoint 									int			Heal 80	
		/call DeclareIniVar CLRPoint		 							int 		Heal 80
		/call DeclareIniVar DRUPoint 									int 		Heal 70
		/call DeclareIniVar SHMPoint 									int 		Heal 60
		/call DeclareIniVar WIZPoint 									int 		Heal 75
		/call DeclareIniVar MAGPoint 									int 		Heal 75
		/call DeclareIniVar ENCPoint 									int 		Heal 80
		/call DeclareIniVar NECPoint 									int 		Heal 75
		/call DeclareIniVar WARPoint 									int 		Heal 70
		/call DeclareIniVar MNKPoint 									int 		Heal 60
		/call DeclareIniVar ROGPoint 									int 		Heal 60
		/call DeclareIniVar RNGPoint 									int 		Heal 90
		/call DeclareIniVar BSTPoint 									int 		Heal 70
		/call DeclareIniVar SHDPoint 									int 		Heal 90
		/call DeclareIniVar BRDPoint 									int 		Heal 65
		/call DeclareIniVar BERPoint 									int 		Heal 70
		/call DeclareIniVar PALPoint 									int 		Heal 90
		/call DeclareIniVar GoMpoint 									int 		Heal 80	
		/call DeclareIniVar XHealClass								string	Heal |WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM
		/call setTimer tXHeal													string	Timer 3m timer_set_xtarget
		/call DeclareIniVar toonControlXTslot					int			Heal 13
		/call DeclareIniVar toonControlProtect				bool		Heal TRUE
	}

	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,BST,PAL,RNG]}) {
 		/call DeclareIniVar cureCurse 								string 	Cure FALSE
		/call DeclareIniVar curePoison 								string 	Cure FALSE
		/call DeclareIniVar cureDisease 							string 	Cure FALSE
		/call DeclareIniVar cureCorrupt 							string 	Cure FALSE
	}
	
	| pet control
  /declare 		pcType											string	outer |pet|swarm|tank|type|build|bag
	/if (${Me.Class.PetClass} || ${Select[${Me.Class.ShortName},ENC,DRU,SHM,SHD,CLR,WIZ,RNG,BRD,BST]}) {
		/call DeclareIniVar PcPetengageat							int			Pet 99
		/call DeclareIniVar switchPcPet								bool 		Pet FALSE
		/call DeclareIniVar switchPcPettank						bool 		Pet FALSE
		/call DeclareIniVar PcPettype									string	Pet FALSE
	}
	/call DeclareIniVar switchPcSwarm								bool 		Pet FALSE	
	/call DeclareIniVar PcSwarmengageat							int			Pet 99
	/call DeclareIniVar sumFamiliar									string	Pet FALSE
	/call DeclareIniVar sumFamiliarBuff							string	Pet FALSE
	/call DeclareIniVar sumFamiliarLeave						bool		Pet TRUE	

	| debug
  /declare 		debugType														string	outer |stop|core|class|casting|rez|array|mode
  /declare		switchDebugStop											bool 		outer FALSE
	/call DeclareIniVar switchDebugCore							bool		debug FALSE 
	/call DeclareIniVar switchDebugClass						bool		debug FALSE 
	/call DeclareIniVar switchDebugCasting					bool		debug FALSE 
	/call DeclareIniVar switchDebugRez							bool		debug FALSE 
	/call DeclareIniVar switchDebugArray						bool		debug FALSE 
	/call DeclareIniVar switchDebugMode							bool		debug FALSE 
	| SET THIS TO TRUE TO ENABLE DEBUGING. ALTERNATIVE USE /varset debug TRUE
	| on a funny note: prepare your ass
	/declare debug																	bool		outer FALSE


	| onoff types
  /declare 		onoffType														string	outer |banestrike|horn|shrink

	/call DeclareIniVar switchBanestrike						bool		Common FALSE		
	/call DeclareIniVar switchInvites								bool 		Common TRUE
	/call DeclareIniVar switchRaidInvite						bool		Common FALSE
	/call DeclareIniVar switchShrink								bool		Common FALSE
	/call DeclareIniVar switchHorn									bool		Common FALSE

	| tradeskill control
  /declare 		tsType															string	outer |movebank
	
	| CLASS controls
  /declare brdType																string	outer |line|spiregroup|spireraid
  /declare berType																string	outer |spiregroup|spireraid
  /declare bstType																string	outer |spiregroup|spireraid
  /declare clrType																string	outer |aggressive|fade|groupheal|grouphot|healpoint|regen|spiregroup|spireraid|vp|xtradius|xttime|yaulp
  /declare druType																string	outer |groupheal|healpoint|spiregroup|spireraid|xtradius|xttime
  /declare encType																string	outer |spiregroup|spireraid
  /declare magType																string	outer |fade|mana|rods|spiregroup|spireraid
  /declare mnkType																string	outer |spiregroup|spireraid
  /declare necType																string	outer |spiregroup|spireraid
  /declare palType																string	outer |groupheal|harmonions|healpoint|mg|spiregroup|spireraid|stance|steel|stun|xtradius|xttime|yaulp
  /declare rogType																string	outer |poison|spiregroup|spireraid
  /declare rngType																string	outer |spice|spiregroup|spireraid|summer
  /declare shdType																string	outer |harmoniouns|skin|spiregroup|spireraid|stance
  /declare shmType																string	outer |groupheal|grouphot|healpoint|spiregroup|spireraid|xtradius|xttime
  /declare warType																string	outer |spiregroup|spireraid
  /declare wizType																string	outer |spiregroup|spireraid
  
  
	| cursor
  /declare cursorType															string	outer |auto|what|sell|keep|destroy|leave|announce|ignore|pause
	/call DeclareIniVar switchAutoCursor						bool		Common FALSE	
  
  | list control
	/declare listType																string	outer |safe|reset|remove|exclude|name|nomezz|nocast|range|nomelee|nomagic|nodisease|nofire|nopoison|nocold|nocorruption|huntmob

	| heals
  /declare healType																string	outer |stop|self|group|xtarget|pet|cure|override
  /declare switchHealStop													bool		outer	FALSE
  /declare switchHealOverride											bool		outer	0
	/declare switchHeal															bool		outer FALSE
	/call DeclareIniVar HealOverride								int			Heal 0
	/declare tmpHealOverride												float 	outer 99.99	
	/call DeclareIniVar switchHealSelf							bool 		Heal FALSE
	/call DeclareIniVar switchHealGroup							bool 		Heal FALSE
	/call DeclareIniVar switchHealXtarget						bool 		Heal FALSE
	/call DeclareIniVar switchHealPet								bool 		Pet FALSE
	/call DeclareIniVar switchHealCure							bool 		Heal FALSE

	/call DeclareIniVar cureCurseItem								string 	Cure FALSE
	/call DeclareIniVar curePoisonItem 							string 	Cure FALSE
	/call DeclareIniVar cureDiseaseItem							string 	Cure FALSE
	/call DeclareIniVar cureCorruptItem							string 	Cure FALSE
	/call DeclareIniVar noCureList	 								string 	Cure "|Sunset's Shadow|FALSE"
/return
	


sub declare_timers()
	/declare _count int local 0

	/call DeclareIniVar tRez_Retry									string	Timer 3m
	| /call DeclareIniVar	tBuff_Kill									string	Timer	1m
	/call DeclareIniVar	tCheck_snap_retry						string	Timer	15s
	| /call DeclareIniVar	tAA_picker									string	Timer	5m
	/call DeclareIniVar	tCheck_Runes								string	Timer	10s
	/call DeclareIniVar	tScorchedEarth_ignore				string	Timer	30s
	
	/call create_timer timer_auto_cursor_pause 10s
	/call create_timer timer_spellcast_nomob_ID 1
	/call create_timer timer_scribe_timer 1
	/call create_timer timer_try_door 1


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
	/call setTimer tCheck_Aura											string	Timer 3m 		timer_check_aura
	/call setTimer tTell_Buff												string	Timer	10s 	timer_check_buffs_tell
	/call setTimer tCheck_Ammo											string	Timer	5s 		timer_Check_Ammo
	/call setTimer tHunt_delay											string	Timer	30s 	timer_wait_for_mob_to_kill
	/call setTimer tAuto_Control_Toon								string	Timer	10s 	timer_auto_toon_control
	/call setTimer tZone_Pause											string	Timer	5s		timer_zone_pause
	/call setTimer tHUD_Death_Check									string	Timer	10s		timer_HUD_death_check
	


	| mod rod/pet weapon recovered addicts
	/if (!${Select[${Me.Class.ShortName},MAG,WAR,ROG,BRD,MNK,BER]}) {
		/for _count 1 to ${ResidentMage.Count[|]}
			/if (!${Bool[${ResidentMage.Arg[${_count},|]}]}) /continue
			/call setTimer	tAsk_for_rod									string	Timer	1m 		timer_ask_for_rod_${ResidentMage.Arg[${_count},|]}
			/call setTimer	tAsk_for_weapons							string	Timer	1m		timer_ask_for_weapons_${ResidentMage.Arg[${_count},|]}
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
	/call DeclareIniVar ServantNuke 								string 	Pet "Remorseless Servant"
	/call setTimer tCauldron_use										string 	Timer 360m	timer_caldron_use

	/call DeclareIniVar ParadoxSpell 								string 	Rods "Grant Icebound Paradox"
	/call DeclareIniVar ParadoxRodName 							string 	Rods "Summoned: Icebound Fragment"
	/call DeclareIniVar OrbSpell 										string 	Rods "Summon Blazing Orb"
	/call DeclareIniVar OrbName 										string 	Rods "Blazing Orb"
	/call DeclareIniVar SelfRodSpell 								string 	Rods "Wand of Dark Modulation"
	/call DeclareIniVar SelfRodName 								string 	Rods "Wand of Pelagic Modulation"
	/call DeclareIniVar ModRodSpell 								string 	Rods "Large Modulation Shard"
	/call DeclareIniVar ModRodName 									string 	Rods "Summoned: Large Modulation Shard"
	/call DeclareIniVar ElementRodSpell 						string 	Rods "Summon Mutinous Minion"
	/call DeclareIniVar ElementRodName 							string 	Rods "Summoned: Exigent Minion XXI"

	/call DeclareIniVar GatherSpell									string 	Spells "Gather Magnitude"
	/call DeclareIniVar DoNuke					 						int			Spells 2
	/call DeclareIniVar Nuke1 											string 	Spells "Spear of Blistersteel"
	/call DeclareIniVar Nuke2 											string 	Spells "Spear of Molten Shieldstone"
	/call DeclareIniVar Nuke3 											string 	Spells FALSE
	/call DeclareIniVar Nuke4 											string 	Spells FALSE
	/call DeclareIniVar NukeSalvo										string 	Spells "Storm of Many"
	/call DeclareIniVar DoDoT						 						int			Spells 0
	/call DeclareIniVar Cauldron				 						string	Spells "Cauldron of Countless Goods"
	/call DeclareIniVar CauldronDestroy 						string 	Spells "Tavon's Burnished Gemstone|Tavon's Polished Gemstone|Ether-Fused Shard|Imprint of the Enhanced Minion|Crystallized Sulfur|Worlu's Windcloak|Worlu's Prying Eyes|"
	/call DeclareIniVar useRain											bool 		Spells FALSE
	/call DeclareIniVar doRainNuke									int 		Spells 2
	/call DeclareIniVar RainNuke1 									string 	Spells "Rain of Cutlasses"
	/call DeclareIniVar RainNuke2 									string 	Spells "Coronal Rain"
	/call DeclareIniVar MaloSpell 									string 	Spells "Malosinete"
	/call DeclareIniVar AAFade											string	Buffs "Dimensional Shield"
	/call DeclareIniVar useAAFade										int			Buffs 50
	
	
/return
	


|***
 *	DES: Wizard vase variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_WIZ()
	/call DeclareIniVar GatherSpell									string 	Spells "Quietscent Harvest"
	/call DeclareIniVar GatherPercent			 					int			Spells 60
	/call DeclareIniVar DoNuke										 	int			Spells 5
	/call DeclareIniVar Nuke1 											string 	Spells "Cloudburst Stormstrike"
	/call DeclareIniVar Nuke2 											string 	Spells "Claw of the Flameweaver"
	/call DeclareIniVar Nuke3 											string 	Spells "Narendi's Fire"
	/call DeclareIniVar Nuke4 											string 	Spells "Etherial Skyblaze"
	/call DeclareIniVar Nuke5 											string 	Spells "Ethereal Rimeblast"
	/call DeclareIniVar Nuke6 											string 	Spells FALSE
	/call DeclareIniVar Nuke7 											string 	Spells FALSE
	/call DeclareIniVar Nuke8 											string 	Spells FALSE
	/call DeclareIniVar tempPet						 					string	Pet "Yulin's Pyroblade"
	
	
	/declare fay 																string	outer "Translocate: Fay"
	/declare dragonscale 												string	outer "Translocate: Dragonscale Hills"
	/declare northkarana 												string	outer "Translocate: North"
	/declare commonlands 												string	outer "Translocate: Common"
	/declare nektulos 													string	outer "Translocate: Nek"
	/declare qeyenos 														string	outer "Translocate: West"
	/declare northro 														string	outer "Translocate: Ro"
	/declare cazic 															string	outer "Translocate: Cazic"
	/declare undershore 												string	outer "Translocate: Undershore"
	/declare westkarana 												string	outer "Translocate: West Karana"
	/declare bloodfields 												string	outer "Translocate: Bloodfields"
	/declare wallofslaughter 										string	outer "Translocate: Slaughter"
	/declare shardslanding 											string	outer "Translocate: Shard's Landing"
	/declare pillars 														string	outer "Translocate Pillars of Alra"
	/declare sarith 														string	outer "Translocate Sarith"
	/declare dreadlands 												string	outer "Translocate: Combine"
	/declare lceanium														string	outer "Translocate: Lceanium"
	/declare dawnshroud 												string	outer "Translocate: Dawnshroud"
	/declare twilight 													string	outer "Translocate: Twilight"
	/declare grimling 													string	outer "Translocate: Grimling"
	/declare nexus 															string	outer "Translocate: Nexus"
	/declare bind 															string	outer "Translocate"
	/declare primary 														string	outer "Translocate: Primary Anchor"
	/declare secondary 													string	outer "Translocate: Secondary Anchor"
	/declare pushprimary 												string	outer "Primary Anchor Push"
	/declare pushsecondary 											string	outer "Secondary Anchor Push"
	/declare tempesttemple 											string	outer "Translocate: Tempest Temple"
	/declare katta															string	outer "Translocate: Katta Castrum"
	/declare tox 																string	outer "Translocate: Tox"
	/declare stonebrunt 												string	outer "Translocate: Stonebrunt"
	/declare icefall 														string	outer "Translocate: Icefall Glacier"
	/declare sunderock 													string	outer "Translocate: Sunderock Springs"
	/declare blightfire 												string	outer "Translocate: Blightfire Moors"
	/declare Barnidu 														string	outer "Translocate: Barindu"
	/declare Natimbi 														string	outer "Translocate: Natimbi"
	/declare grounds 														string	outer "Translocate: the Grounds"
	/declare potime 														string	outer "Translocate: Plane of Time"
	/declare brells 														string	outer "Translocate: Brell's Rest"
	/declare Arcstone 													string	outer "Translocate: Arcstone"
	/declare pok 																string	outer "Translocate: Knowledge"
	/declare cobaltscar 												string	outer "Translocate: Cobalt Scar"
	/declare wakening 													string	outer "Translocate: Wakening Lands"
	/declare greatdivide 												string	outer "Translocate: Great Divide"
	/declare iceclad 														string	outer "Translocate: Iceclad"		
	
	/call DeclareIniVar runeMagiWard	 							string	Rune "Doomscale Husk"
	/call DeclareIniVar runeShieldofFate						string	Rune "Shield of Consequence"
	/call DeclareIniVar runeCrystalwing							string	Rune "Armor of the Codex"
	/call DeclareIniVar runeGuard			 							string	Rune "Darkmist Guard"
/return	



|***
 *	DES: Snchanter base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_ENC()
	/declare mobList														string	outer
	/declare addList 														string 	outer
	| /if (!${Defined[aggroAnim]}) /declare aggroAnim string outer |5|8|12|17|18|42|44|80|106|129|144

	/call DeclareIniVar runeVeil			 							string	Rune "Veil of Mindshadow"
	/call DeclareIniVar runeEldritch	 							string	Rune "Eldritch Rune"
	/call DeclareIniVar runeBrimstone 							string	Rune "Brimstone Resolution"
	/call DeclareIniVar runeSpray			 							string	Rune "Glyph Spray"
	/call DeclareIniVar runeReactive	 							string	Rune "Reactive Rune"
	/call DeclareIniVar runePoly			 							string	Rune "Polyrefractive Rune"
	/call DeclareIniVar runeUnity			 							string	Rune FALSE
	/call DeclareIniVar runeWard			 							string	Rune "Ward of the Enticer"
	/call DeclareIniVar runeAuspice		 							string	Rune "Darkened Auspice"
	/call DeclareIniVar runeAuspiceToon							string  Rune |FALSE|FALSE
	/call DeclareIniVar runeMind										string  Rune "Mind Over Matter"
	/call DeclareIniVar runeEntAuspice							string	Rune "Enticer's Auspice"
	/call DeclareIniVar runeLegion									string	Rune "Legion of Lucem"
	/call DeclareIniVar runeShieldofFate						string	Rune "Shield of Consequence"

	/call DeclareIniVar mannaCrystal								string 	Spells "Azure Mind Crystal"
	/call DeclareIniVar healCrystal									string 	Spells "Sanguine Mind Crystal"
	
	/call DeclareIniVar spellCripple 								string 	Spells "Undermining Helix"
	/call DeclareIniVar spellSlow		 								string	Spells "Undermining Helix"		
	/call DeclareIniVar AASlow			 								string	Spells "Dreary Deeds"		
	/call DeclareIniVar spellTash 									string 	Spells "Decree of Tashan"

	/call DeclareIniVar spellMez										string 	Spells "Chaotic Bewildering"
	/call DeclareIniVar spellpbaeMez								string	Spells "Perilous Bafflement"
	/call DeclareIniVar spellStun										string	Spells "Color Concourse"
	/call DeclareIniVar spellCharm									string	Spells "Enticer's Command"
	
	/call DeclareIniVar DoNuke					 						int			Spells 3
	/call DeclareIniVar Nuke1					 							string	Spells "Chromatic Blink"
	/call DeclareIniVar Nuke2					 							string	Spells "Intellectual Appropriation"
	/call DeclareIniVar Nuke3					 							string	Spells "Mindsunder"
	/call DeclareIniVar Nuke4					 							string	Spells FALSE
	
	/call DeclareIniVar DoDoT								 				int			Spells 0
	/call DeclareIniVar DoT1					 							string	Spells FALSE
	/call DeclareIniVar DoT2					 							string	Spells FALSE
	/call DeclareIniVar DoT3					 							string	Spells FALSE
	/call DeclareIniVar DoT4					 							string	Spells FALSE
/return	



|***
 *	DES: Necromancer base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_NEC()
	/call setTimer tCheck_Blood											string	Timer 8s	timer_check_blood
	| /declare set																int			outer 1
	| /declare gem																int			outer 1
	
	| /call DeclareIniVar SpellSets 									int 		Spells 4
	| /call DeclareIniVar SpellSet1 									string 	Spells core1
	| /call DeclareIniVar SpellSet2 									string 	Spells core2
	| /call DeclareIniVar SpellSet3 									string 	Spells core3
	| /call DeclareIniVar SpellSet4 									string 	Spells core4

	/call DeclareIniVar spellMalo										string 	Spells "Scent of Terris"
	/call DeclareIniVar spellBlood									string 	Spells "Impose for Blood"

	/call DeclareIniVar DoDoT								 				int			Spells 0
	/call DeclareIniVar DoT1							 					string	Spells FALSE
	/call DeclareIniVar DoT1chk						 					string	Spells FALSE
	/call DeclareIniVar DoT2							 					string	Spells FALSE
	/call DeclareIniVar DoT2chk						 					string	Spells FALSE
	/call DeclareIniVar DoT3							 					string	Spells FALSE
	/call DeclareIniVar DoT3chk						 					string	Spells FALSE
	/call DeclareIniVar DoT4							 					string	Spells FALSE
	/call DeclareIniVar DoT4chk						 					string	Spells FALSE
	/call DeclareIniVar DoT5					 							string	Spells FALSE
	/call DeclareIniVar DoT5chk						 					string	Spells FALSE
	/call DeclareIniVar DoT6					 							string	Spells FALSE
	/call DeclareIniVar DoT6chk						 					string	Spells FALSE
	/call DeclareIniVar DoT7					 							string	Spells FALSE
	/call DeclareIniVar DoT7chk						 					string	Spells FALSE
	/call DeclareIniVar DoT8					 							string	Spells FALSE
	/call DeclareIniVar DoT8chk						 					string	Spells FALSE
	/call DeclareIniVar DoT9					 							string	Spells FALSE
	/call DeclareIniVar DoT9chk						 					string	Spells FALSE
	/call DeclareIniVar DoT10					 							string	Spells FALSE
	/call DeclareIniVar DoT10chk					 					string	Spells FALSE

	/call DeclareIniVar DoNuke					 						int			Spells 1
	/call DeclareIniVar Nuke1					 							string	Spells "Combust Bones"	
	/call DeclareIniVar Nuke2					 							string	Spells FALSE
	/call DeclareIniVar Nuke3					 							string	Spells FALSE
	/call DeclareIniVar Nuke4					 							string	Spells FALSE
	/call DeclareIniVar Nuke5					 							string	Spells FALSE
	/call DeclareIniVar Nuke6					 							string	Spells FALSE
/return	



|***
 *	DES: Shadowknight base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_SHD()
	/call setTimer tcheck_Skin											string 	Timer 45s timer_check_Steel
	/call setTimer tcheck_Stance										string 	Timer 3m timer_check_stance



	/declare mobAECount															int			outer 0
	/call DeclareIniVar Withstand 									string 	Discs "Repel"
	/call DeclareIniVar BladeStrike 								string 	Discs "Lacerating Blade"
	/call DeclareIniVar CrimsonBlade 								string 	Discs "Carmine Blade"
	/call DeclareIniVar DefensiveDisc 							string 	Discs "Doomscale Mantle"
	/call DeclareIniVar DefensiveDisc2 							string 	Discs "Grelleth's Carapace"
	/call DeclareIniVar DefensiveDisc3 							string 	Discs "Unholy Guardian Discipline"
	/call DeclareIniVar DiscRest			 							string	Discs "Breather"
	/call DeclareIniVar DiscFakeHate								string	Discs	"Unflinching Acrimony"
	/call DeclareIniVar DiscLeech										string	Discs	"Leechcurse Discipline"
	/call DeclareIniVar DiscRancor									string	Discs	"Reflexive Revulsion"


	/call DeclareIniVar ChallengeSpell							string 	Spells "Impose for Power"
	/call DeclareIniVar Hate1 											string 	Spells "Terror of Narus"
	/call DeclareIniVar Hate2 											string 	Spells "Terror of Poria"
	/call DeclareIniVar spellCripple								string 	Spells ""
	/call DeclareIniVar spellUndeadCripple					string 	Spells "Helix of the Undying"
	 
	/call DeclareIniVar AEHate1 										string 	Spells "Disgust"
	/call DeclareIniVar AEHate2 										string 	Spells "Repugnance"
	/call DeclareIniVar AELifeTap 									string 	Spells "Deceitful Deflection"
	
	/call DeclareIniVar DoNuke					 						int			Spells 2
	/call DeclareIniVar Nuke1 											string 	Spells "Touch of Holmein"
	/call DeclareIniVar Nuke2 											string 	Spells "Touch of Lutzen"
	/call DeclareIniVar Nuke3 											string 	Spells FALSE
	/call DeclareIniVar DoDoT								 				int			Spells 0
	/call DeclareIniVar DoT1							 					string	Spells FALSE
	/call DeclareIniVar DoT2							 					string	Spells FALSE
	/call DeclareIniVar DoT3							 					string	Spells FALSE
	 
	/call DeclareIniVar HpTapSpell 									string 	Spells "Touch of Lanys"
	/call DeclareIniVar HpTapSpellRecourse 					string 	Spells "Gift of Lanys"
	/call DeclareIniVar ManaTapSpell1 							string 	Spells "Bonemaw's Bite"
	/call DeclareIniVar ManaTapSpell2 							string 	Spells "Ancient: Bite of Muram"

	/call DeclareIniVar switchHarmonious 						bool	 	Buffs TRUE
	/call DeclareIniVar Harmonious 									string 	Buffs "Harmonious Disruption"
	/call DeclareIniVar switchStance 								bool	 	Buffs TRUE
	/call DeclareIniVar Stance 											string 	Buffs "Staunch Stance"
	/call DeclareIniVar switchSkin			 						bool	 	Buffs TRUE
	/call DeclareIniVar Skin			 									string 	Buffs "Vizat's Skin"

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
	/call setTimer tcheck_Stance										string 	Timer 7.5m timer_check_stance

	/declare mobAECount													int			outer 0 
	/declare groupHurt 													int 		outer 0
	/declare WorstHurt 													string 	outer FALSE
	| /declare WorstHurtID 										int 		outer
	/declare WorstHP 														int 		outer 100
	/declare WorstClass 												string 	outer FALSE
	| /declare MinorHurt											int    	outer 0 
	
  /call DeclareIniVar healWholeGroup							string 	Heal "Wave of Grief"
  /call DeclareIniVar splashGroup									string 	Heal "Wave of Grief"
  /call DeclareIniVar healPanic										string 	Heal FALSE
  /call DeclareIniVar splashPanic									string 	Heal "Aurora of Dayspring"
	| /declare rezID 													int 		outer
	/call DeclareIniVar useGroupHeal 								bool		Heal TRUE
	/call DeclareIniVar XTHealRadius								int			Heal 300
	
	/call DeclareIniVar doYaulp 										string	Buffs TRUE
	/call DeclareIniVar DiscRest			 							string	Discs "Breather"
	/call DeclareIniVar DiscFastRune	 							string	Discs "Repel"
	/call DeclareIniVar DiscArmor			 							string	Discs "Armor of Ardency"
	/call DeclareIniVar DiscGuard			 							string	Discs "Skalber Mantle"
	/call DeclareIniVar DiscUnflinching							string	Discs "Unflinching Affirmation"
	/call DeclareIniVar DiscJudgement								string	Discs "Inquisitors Judgement"
	/call DeclareIniVar DiscHate										string	Discs "Projection of Piety"
	/call DeclareIniVar DiscTwincast								string	Discs "Hand of Tunare"
	/call DeclareIniVar DiscReflexive								string	Discs "Reflexive Reverence"

	/call DeclareIniVar AALayonHands								string	Discs "Lay on Hands"
	/call DeclareIniVar AAHandofPiety								string	Discs "Hand of Piety"
	/call DeclareIniVar AAForceofDisruption					string	Discs "Force of Disruption"
	/call DeclareIniVar AADisruptivePersecution			string	Discs "Disruptive Persecution"
	/call DeclareIniVar AADivineStun								string	Discs "Divine Stun"

	/call DeclareIniVar AAAEBeacon									string	Discs "Beacon of the Righteous"
	/call DeclareIniVar AAAELodestar								string	Discs "Hallowed Lodestar"

	/call DeclareIniVar AAInquisitor								string	Discs "Armor of the Inquisitor"
	/call DeclareIniVar AAGroupInquisitor						string	Discs "Group Armor of the Inquisitor"

	/call DeclareIniVar AAJudgement									string	Discs "Inquisitor's Judgement"
	/call DeclareIniVar AAValorousRage							string	Discs "Valorous Rage"
	/call DeclareIniVar AAShield										string	Discs "Shield of Brilliance"
	/call DeclareIniVar chainStun										string	Discs FALSE
	
	/call DeclareIniVar DoNuke					 						int			Spells 5
	/call DeclareIniVar Nuke1												string 	Spells "Force of Ardency"
	/call DeclareIniVar Nuke2												string 	Spells "Ardent Force"
	/call DeclareIniVar Nuke3												string 	Spells "Force of the Darkened Sea"
	/call DeclareIniVar Nuke4												string 	Spells "Lesson of Grief"
	/call DeclareIniVar Nuke5							 					string	Spells "Protective Proclamation"

	/call DeclareIniVar DoDoT								 				int			Spells 0
	/call DeclareIniVar DoT1							 					string	Spells FALSE
	/call DeclareIniVar DoT2					 							string	Spells FALSE
	/call DeclareIniVar DoT3					 							string	Spells FALSE
	/call DeclareIniVar DoT4					 							string	Spells FALSE
	

	/call DeclareIniVar ChallengeSpell							string 	Spells "Impose for Honor"
	/call DeclareIniVar ValiantDefense							string 	Spells "Valiant Defense"
		
	/call DeclareIniVar Twincast										string 	Spells "Glorious Exoneration"
	
	/call DeclareIniVar switchHarmonious 						bool	 	Buffs TRUE
	/call DeclareIniVar Harmonious 									string 	Buffs "Harmonious Blessing"
	/call DeclareIniVar switchSteel			 						bool	 	Buffs TRUE
	/call DeclareIniVar Steel			 									string 	Buffs "Reinvigorating Steel"
	/call DeclareIniVar switchStance 								bool	 	Buffs TRUE
	/call DeclareIniVar Stance 											string 	Buffs "Staunch Stance"
	
	
	
	
	/call DeclareIniVar useMG												bool		Buffs TRUE
	/call DeclareIniVar useMGPct										int			Buffs 60
/return	



|***
 *	DES: Warrior base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_WAR()
	/call DeclareIniVar DiscInsult 									string 	Discs "Insult"
	/call DeclareIniVar DiscCyclone			 						string	Discs "Cyclone Roar"
	/call DeclareIniVar DiscWade				 						string	Discs "Wade In To Battle"
	/call DeclareIniVar DiscChallenge								string	Discs "Roar of Challenge"
	/call DeclareIniVar DiscShout				 						string	Discs "Tormenting Shout"
	/call DeclareIniVar DiscRest			 							string	Discs "Breather"
	/call DeclareIniVar DiscStoutDefense 						string	Discs "Stout Defense"
	/call DeclareIniVar DiscField				 						string	Discs "Field Protector"
	/call DeclareIniVar DiscFlash				 						string	Discs "Flash of Anger"
	/call DeclareIniVar DiscNoTime				 					string	Discs "Pain Doesn't Hurt"
	/call DeclareIniVar DiscAttention						 		string	Discs "Unflinching Attention"
	/call DeclareIniVar DiscPhantom							 		string	Discs "Phantom Aggressor"
	/call DeclareIniVar AAGutPunch 									string	Discs "Gut Punch"
	/call DeclareIniVar AAWarStomp 									string	Discs "War Stomp"
	/call DeclareIniVar AABraceforImpact 						string	Discs "Brace for Impact"
	/call DeclareIniVar AAMageHunter								string	Discs "Mark of the Mage Hunter"
	/call DeclareIniVar AAVhementRage 							string	Discs "Vhement Rage"
	/call DeclareIniVar AABladeGuardian 						string	Discs "Blade Guardian"
	/call DeclareIniVar AAKneeStrike 								string	Discs "Knee Strike"
	/call DeclareIniVar AAImpCommand								string	Discs "Imperator's Command"
	/call DeclareIniVar AAHeroicBlade 							string	Discs "Ward Sheol's Heroic Blade"
	/call DeclareIniVar AAResplendentGlory 					string	Discs "Resplendent Glory"
	/call DeclareIniVar AAHoldtheLine 							string	Discs "Hold the Line"
	/call DeclareIniVar AAAETaunt 									string	Discs "Area Taunt"
	/call DeclareIniVar AABlastofAnger							string	Discs "Blast of Anger"
	/call DeclareIniVar AARageoftheForsaken					string	Discs "Rage of the Forsaken"
	/call DeclareIniVar AAWarlordsTenacity					string	Discs "Warlords Tenacity"
	/call DeclareIniVar AAWarlordsBravery						string	Discs "Warlords Bravery"
	
	/call DeclareIniVar PanicHPPCT		 							int			Heal 25
/return	



|***
 *	DES: Berserkers base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_BER()
	/call DeclareIniVar DiscSnare 									string 	Discs FALSE
	/call DeclareIniVar SnareAxe 										string 	Discs FALSE
	/call DeclareIniVar DiscVolley 									string 	Discs "Brutal Volley"
	/call DeclareIniVar CryOfChaos 									string 	Discs "Ancient: Cry of Chaos"
	/call DeclareIniVar DiscBerserking 							string 	Discs "Berserking Discipline"
	/call DeclareIniVar DiscRest 										string 	Discs "Breather"
	/call DeclareIniVar DiscCombatFrenzy 						string 	Discs "Amplified Frenzy"
	/call DeclareIniVar DiscScream 									string 	Discs "Distressing Scream"
	/call DeclareIniVar ThrowingAxe 								string 	Discs "Demolishing Axe Throw"
	/call DeclareIniVar DiscJolt 										string 	Discs "Jarring Smite"
	/call DeclareIniVar SharedBloodLust 						string 	Discs "Shared Cruelty"
	/call DeclareIniVar SharedBloodLustRecourse			string 	Discs "Reflected Brutality II"
	/call DeclareIniVar DiscOverwhelmingFrenzy 			string 	Discs "Demolishing Frenzy"
	/call DeclareIniVar DiscCleavingAnger 					string 	Discs "Cleaving Acrimony Discipline"
	/call DeclareIniVar DiscSlapintheFace 					string 	Discs "Kick in the Shins"
	/call DeclareIniVar DiscAxeofRallos 						string 	Discs "Axe of Numicia"
	/call DeclareIniVar DiscVengeful 								string 	Discs "Avenging Flurry Discipline"
	/call DeclareIniVar DiscFesteringRage 					string 	Discs "Bubbling Rage"
	/call DeclareIniVar DiscCryHavoc 								string 	Discs "Cry Carnage"
	/call DeclareInivar DiscAugmentedFrenzy 				string 	Discs "Amplified Frenzy"
  /call DeclareInivar DiscOpenWound 							string 	Discs "Open Wound"
  /call DeclareInivar DiscFrenzyResolve						string 	Discs "Frenzied Resolve"
  /call DeclareInivar UntamedgRage								string 	Discs "Untamed Rage"
  /call DeclareInivar RecklessAbandon							string 	Discs "Reckless Abandon"
 	/call DeclareIniVar frenzy											bool		meleedisc FALSE
 	/call DeclareIniVar DiscDicho		 								string 	Discs "Dichotomic Rage"
 	/call DeclareIniVar DiscDisconcert							string 	Discs "Disconcerting Discipline"
 	/call DeclareIniVar DiscEnduranceTap						string 	Discs "Sapping Strikes"

	/declare OpenWoundEffect										string 	outer Open Wound
  /if (${OpenWoundDisc.Find[rk. III]}) {
		/varset OpenWoundEffect ${OpenWoundDisc.Left[-7]} effect III
	} else /if (${RaidBuffCast${x}.Find[rk. II]})  {
		/varset OpenWoundEffect ${OpenWoundDisc.Left[-6]} effect II
  }
	/declare AugmentedFrenzyEffect 							string outer Augmented Frenzy Effect
  /if (${AugmentedFrenzyDisc.Find[rk. III]}) {
		/varset AugmentedFrenzyEffect ${AugmentedFrenzyDisc.Left[-7]} effect III
	} else /if (${RaidBuffCast${x}.Find[rk. II]})  {
		/varset AugmentedFrenzyEffect ${AugmentedFrenzyDisc.Left[-6]} effect II
  } 
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

	/call DeclareIniVar DoNuke					 						int			Spells 5
	/call DeclareIniVar Nuke1					 							string	Spells "Anticipated Shots"	
	/call DeclareIniVar Nuke2					 							string	Spells "Focused Arrowrain"
	/call DeclareIniVar Nuke3					 							string	Spells "Heartsplit"
	/call DeclareIniVar Nuke4					 							string	Spells "Wildfire Boon"
	/call DeclareIniVar Nuke5					 							string	Spells "Wildfire Ash"
	/call DeclareIniVar Nuke6					 							string	Spells FALSE
	/call DeclareIniVar NukeSummer		 							string	Spells "Summer's Cyclone"
	/call DeclareIniVar switchNukeSummer		 				bool		Spells TRUE
	

	/call DeclareIniVar DoDoT								 				int			Spells 2
	/call DeclareIniVar DoT1					 							string	Spells "Blisterbeetle Swarm"
	/call DeclareIniVar DoT2					 							string	Spells "Vespid Swarm"
	/call DeclareIniVar DoT3					 							string	Spells FALSE
	/call DeclareIniVar DoT4					 							string	Spells FALSE
	/call DeclareIniVar DoTSwarmDS		 							string	Spells "Swarm of Glistenwings"
	/call DeclareIniVar DoTSwarmDSdeBuff 						string	Spells "Glistenwing Swarm"
	
	/call DeclareIniVar SlowHeal										string	Spells "Cloudfont" 
	/call DeclareIniVar FastHeal			 							string	Spells "Desperate Dousing" 
	/call DeclareIniVar FastHealPCT		 							int			Spells 25

	/call DeclareIniVar DiscMele			 							string	Spells "Copsestalker's Discipline"
	/call DeclareIniVar DiscRange			 							string	Spells "Pureshot Discipline"
	/call DeclareIniVar DiscRest			 							string	Spells "Breather"
	/call DeclareIniVar DiscJolt			 							string	Spells "Jolting Cut Kicks"
	/call DeclareIniVar DiscEnrage		 							string	Spells "Enraging Cut Kicks"
	/call DeclareIniVar DiscHeal			 							string	Spells "Reflexive Bladespurs"
	/call DeclareIniVar BladeStorm									string	Spells "Focused Storm of Blades"
	/call DeclareIniVar ElementalArrow							string	Spells "Elemental Arrow"
	/call DeclareIniVar GlacialArow									string	Spells "Glacial Arrow"
	/call DeclareIniVar VolatileArrow								string	Spells "Volatile Arrow"
	/call DeclareIniVar ArrowPoison			 						string	Spells "Poison Arrows"	
	/call DeclareIniVar ArrowFire			 							string	Spells "Flaming Arrows"	
	/call DeclareIniVar ArrowIce				 						string	Spells "Frost Arrows"	
	/call DeclareIniVar AuspiceTXT									string	Spells "AUSPICE (critSpell 33%, accuracy 230%, critHeal 33%, critMelee 225%, critDoT 33%, attack +165)"
	/call DeclareIniVar DiscSqual			 							string	Spells "Focused Squall of Blades"
/return	



|***
 *	DES: Druid base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_DRU()
	/call DeclareIniVar debuffRo			 							string	Spells "Blessing of Ro"	
	/call DeclareIniVar WoodTXT											string	Spells "SoWood cast."
	
	/call DeclareIniVar DoNuke					 						int			Spells 0
	/call DeclareIniVar Nuke1					 							string	Spells FALSE
	/call DeclareIniVar Nuke2							 					string	Spells FALSE
	/call DeclareIniVar Nuke3							 					string	Spells FALSE
	/call DeclareIniVar Nuke4							 					string	Spells FALSE
	/call DeclareIniVar DoDoT								 				int			Spells 0
	/call DeclareIniVar DoT1							 					string	Spells FALSE
	/call DeclareIniVar DoT2							 					string	Spells FALSE
	/call DeclareIniVar DoT3							 					string	Spells FALSE	
	/call DeclareIniVar DoT4					 							string	Spells FALSE	
	
	/declare MajorHurt 													int 		outer 0
	/declare WorstHurt 													string 	outer FALSE
	/declare WorstHurtID 												int 		outer
	/declare WorstHP 														int 		outer 100
	/declare WorstClass 												string 	outer FALSE
  /declare MinorHurt													int    	outer 0 
  /declare WorstHp 														int    	outer 100
  /declare pWorstHurt 												string 	outer ${Me.DisplayName}
  /declare pWorstHP														int			outer 100
  /declare pWorstHurtID												int 		outer ${Me.ID}
  /declare pWorstClass												string	outer FALSE
  /declare pMajorHurt													int			outer 0
	
	/call DeclareIniVar RandRezDelay								int 		Heal 10
	/call DeclareIniVar ProtectOutsideGroup					string	Heal |FALSE|FALSE
	/call DeclareIniVar ProtectInsideRaid						string	Heal |FALSE|FALSE
	/call DeclareIniVar ProtectNPCList							string	Heal |FALSE|FALSE
	/call DeclareIniVar useGroupHeal 								bool		Heal TRUE

	/call DeclareIniVar healPanic			 							string  Heal "Survival of the Serendipitous"
	/call DeclareIniVar healRejuvilation						string  Heal "Resurgence"
	/call DeclareIniVar healAdrenaline				 			string 	Heal "Daggerthorn Rush"
	/call DeclareIniVar healSanavida	 							string  Heal "Panavida"
	/call DeclareIniVar healWholeGroup							string 	Heal "Lunasoothe"
	/call DeclareIniVar XTHealRadius								int			Heal 300
	
	| ports	
	/declare looping														string	outer "Zephyr: Looping Planes"
	/declare steamfront													string	outer "Zephyr: Steamfront"
	/declare butcherblock												string	outer "Zephyr: Butcherblock"
	/declare shardslanding											string	outer "Zephyr: Shard's Landing"
	/declare pillars														string	outer "Zephyr: Pillars of Alra"
	/declare beast															string	outer "Zephyr: Beasts' Domain"
	/declare westkarna													string	outer "Zephyr: West Karana"
	/declare lavastorm													string	outer "Zephyr: Lavastorm"
	/declare undershore													string	outer "Zephyr: Undershore"
	/declare misty															string	outer "Zephyr: Misty"
	/declare ro																	string	outer "Zephyr: Ro"
	/declare feerrott														string	outer "Zephyr: Feerrott"
	/declare commonlands												string	outer "Zephyr: Commonlands"
	/declare surefall														string	outer "Zephyr: Surefall Glade"
	/declare karana															string	outer "Zephyr: Karana"
	/declare wallofslaughter 										string	outer "Zephyr: Slaughter"
	/declare bloodfields 												string	outer "Zephyr: Bloodfields"
	/declare lceanium														string	outer "Zephyr: Lceanium"
	/declare dreadlands													string	outer "Zephyr: Combines"
	/declare dawnshroud													string	outer "Zephyr: Dawnshroud"
	/declare twilight														string	outer "Zephyr: Twilight"
	/declare grimling														string	outer "Zephyr: Grimling"
	/declare nexus															string	outer "Zephyr: Nexus"
	/declare primarypush												string	outer "Primary Anchor Push"
	/declare secondarypush											string	outer "Secondary Anchor Push"
	/declare primary														string	outer "Zephyr: Primary Anchor"
	/declare secondary													string	outer "Zephyr: Secondary Anchor"
	/declare tempesttemple											string	outer "Zephyr: Tempest Temple"
	/declare buriedsea													string	outer "Zephyr: Buried Sea"
	/declare stonebrunt													string	outer "Zephyr: Stonebrunt"
	/declare tox																string	outer "Zephyr: Toxxulia"
	/declare direwind														string	outer "Zephyr: Direwind"
	/declare steppes														string	outer "Zephyr: Steppes"
	/declare blightfire													string	outer "Zephyr: Blightfire Moors"
	/declare barindu														string	outer "Zephyr: Barindu"
	/declare natimbi														string	outer "Zephyr: Natimbi"
	/declare grounds														string	outer "Zephyr: the Grounds"
	/declare potime															string	outer "Zephyr: Plane of Time"
	/declare brells															string	outer "Zephyr: Brell's Rest"
	/declare arcstone														string	outer "Zephyr: Arcstone"
	/declare knowledge													string	outer "Zephyr: Knowledge"
	/declare cobaltscar													string	outer "Zephyr: Cobalt Scar"
	/declare wakening														string	outer "Zephyr: Wakening Land"
	/declare greatdivide												string	outer "Zephyr: Great Divide"
	/declare iceclad														string	outer "Zephyr: Iceclad"	
/return	



|***
 *	DES: Shaman base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_SHM()
	/declare MajorHurt 													int 		outer 0
	/declare WorstHurt 													string 	outer FALSE
	/declare WorstHurtID 												int 		outer ${Me.ID}
	/declare WorstHP 														int 		outer 100
	/declare WorstClass 												string 	outer FALSE
  /declare MinorHurt													int    	outer 0 
  /declare WorstHp		 												int    	outer 100	
  
	/call DeclareIniVar AidTXT 											string 	Spells "ANCESTRAL AID Done (Str/Agi/Dex +300, HoT Heal +1840)"
	/call DeclareIniVar MaloSpell 									string 	Spells "Malosinete"
	/call DeclareIniVar AASlow			 								string	Spells "Turgur's Swarm"	
	/call DeclareIniVar spellSlow										string 	Spells FALSE
	/call DeclareIniVar spellAESlow 								string 	Spells "Srasku's Drowse"
	/call DeclareIniVar usePathosis 								bool	 	Spells TRUE
	
	/call DeclareIniVar TC1									 				string	Spells "Glacial Gift"	
	/call DeclareIniVar TC2									 				string	Spells "Frigid Gift"
	/call DeclareIniVar DoNuke							 				int			Spells 0
	/call DeclareIniVar Nuke1					 							string	Spells "Reefmaw's Bite"
	/call DeclareIniVar Nuke2					 							string	Spells FALSE
	/call DeclareIniVar Nuke3					 							string	Spells FALSE
	/call DeclareIniVar DoDoT						 						int			Spells 0
	/call DeclareIniVar DoT1					 							string	Spells FALSE
	/call DeclareIniVar DoT2					 							string	Spells FALSE
	/call DeclareIniVar DoT3					 							string	Spells FALSE
	/call DeclareIniVar spellCripple 								string 	Spells "Crippling Counterbias"

	/call DeclareIniVar CaniPercent				 					int			Spells 60 /cani
	/call DeclareIniVar spellCannibalization				string	Spells "Tribal Pact"

	/call DeclareIniVar useGroupHeal 								bool		Heal TRUE
	/call DeclareIniVar useGroupHoT									bool		Heal TRUE
	/call DeclareIniVar RandRezDelay								int 		Heal 10

  /call DeclareIniVar healSurge										string 	Heal "Spiritual Surge"
  /call DeclareIniVar healMain										string 	Heal "Krasir's Mending"
  /call DeclareIniVar healIntervention						string 	Heal "Historian's Intervention"
  /call DeclareIniVar healReckless								string 	Heal "Reckless Regeneration"
  /call DeclareIniVar healWholeGroup							string 	Heal "Krasir's Recourse"
  /call DeclareIniVar healWholeGroupHoT						string 	Heal "Shear of Renewal"

	/call DeclareIniVar ProtectOutsideGroup					string	Heal |FALSE|FALSE
	/call DeclareIniVar ProtectInsideRaid						string	Heal |FALSE|FALSE
	/call DeclareIniVar ProtectNPCList							string	Heal |FALSE|FALSE
	/call DeclareIniVar XTHealRadius								int			Heal 300

	/declare HealList 													string 	outer |${healSurge}|${healWholeGroup}|${healWholeGroupHoT}|${healMain}|${healIntervention}|${healReckless}
/return	



|***
 *	DES: Bard base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_BRD()
	/declare mobList														string 	outer
	/declare addList 														string 	outer
		
	/call DeclareIniVar ServantNukeAA1 							string 	Pet "Lyrical Prankster"
	/call DeclareIniVar ServantNukeAA2 							string 	Pet "Song of Stone"
	/call DeclareIniVar songServantNuke							string 	Pet "Plague of the Piper"
	
	/declare lineType																string	outer |caster|melee|mix|other|tank
	/call DeclareIniVar lineCurrent									string 	Song Tank
	/call DeclareIniVar lineTank										string	Song "|Chorus of Sionachie|War March of Jocelyn|Fjilnauk's Spiteful Lyric Rk. II|Fjilnauk's Song of Suffering|Doben's Spry Sonata Rk. II"
	/call DeclareIniVar lineCaster									string	Song "|Chorus of Sionachie|Nilsara's Aria|Fatesong of Jocelyn|Arcane Melody|Aria of Maetanrus"
	/call DeclareIniVar lineMelee										string	Song "|Chorus of Sionachie|War March of Jocelyn|Fjilnauk's Spiteful Lyric|Fjilnauk's Song of Suffering|Arcane Melody"
	/call DeclareIniVar lineMix											string	Song "|Chorus of Sionachie|War March of Jocelyn|Fjilnauk's Spiteful Lyric|Nilsara's Aria"
	/call DeclareIniVar lineOther										string	Song FALSE
	
	/call DeclareIniVar songDicho										string	Song "Dichotomic Psalm"
	
	/call DeclareIniVar DoNuke					 						int			Song 1
	/call DeclareIniVar Nuke1												string 	Song "Tsaph's Insult"
	/call DeclareIniVar Nuke2												string 	Song FALSE
	
	/call DeclareIniVar spellMez										string 	Song "Slumber of Silisia"
	/call DeclareIniVar spellpbaeMez								string	Song "Wave of Torpor"	
/return	


|***
 *	DES: Beastlord base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_BST()
	/call DeclareIniVar DiscForay 									string 	Spells "Pummel"
	/call DeclareIniVar DiscFocusClaws 							string 	Spells "Focused Clamor of Claws"
	/call DeclareIniVar DiscSavage 									string 	Spells "Savage Rage"
	/call DeclareiniVar DiscRuaabri 								string 	Spells "Ruaabri's Fury"	
	/call DeclareIniVar GrowlSpell 									string 	Spells "Growl of the Leopard"
 	/call DeclareIniVar ServantNuke		 							string 	Spells "Cry at the Moon"
  /call DeclareIniVar FeroSpell 									string 	Spells "Savage Ferocity"
  /call DeclareIniVar DiscEnduring								string 	Spells "Enduring Frenzy"
  /call DeclareIniVar DiscRending									string 	Spells "Reflexive Rending"
	
	/call DeclareIniVar GrowlSpell 									string 	Pet "Growl of the Leopard"
	/call DeclareIniVar TasteofBlood								string 	Pet "Taste of Blood"
	/call DeclareIniVar Aggression									string 	Pet "Sekmoset's Aggression"
	/call DeclareIniVar ProcHeal										string 	Pet "Mending Warder"
	/call DeclareIniVar Calm												string 	Pet "Companion's Calm Demeanor"
	/call DeclareIniVar DiscRest				 						string	Spells "Breather"
	/call DeclareIniVar ParagonTXT 									string 	Spells "PARAGON DONE (+5790 HP/tick, +2450 Mana/tick, +264 End/tick)"
	
	/call DeclareIniVar DoNuke							 				int			Spells 5
	/call DeclareIniVar Nuke1					 							string	Spells "Fozen Miasma"	
	/call DeclareIniVar Nuke2					 							string	Spells "Kromtus Lance"
	/call DeclareIniVar Nuke3					 							string	Spells "Visoracius' Maelstrom"
	/call DeclareIniVar Nuke4					 							string	Spells "Kromrif Lance"
	/call DeclareIniVar Nuke5					 							string	Spells "Krieg's Bite"
	/call DeclareIniVar Nuke6					 							string	Spells FALSE

	/call DeclareIniVar DoDoT						 						int			Spells 0
	/call DeclareIniVar DoT1							 					string	Spells FALSE
	/call DeclareIniVar DoT2					 							string	Spells FALSE
	/call DeclareIniVar DoT3					 							string	Spells FALSE
	/call DeclareIniVar DoT4					 							string	Spells FALSE
	
	/call DeclareIniVar FastHeal			 							string	Spells "Sabhattin's Mending" 
	/call DeclareIniVar FastHealPCT		 							int			Spells 60
	
	/call DeclareIniVar selfParagon		 							int			Spells 80
	/call DeclareIniVar groupParagon	 							int			Spells 70
	/call DeclareIniVar minConsumptionHP						int			Spells 50
/return	



|***
 *	DES: Monk base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_MNK()
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

	/call DeclareIniVar DiscRest				 						string 	Discs "Breather"
	/call DeclareIniVar DiscAggro 									string 	Discs "Disorientation"
	/call DeclareIniVar DiscAssault 								string 	Discs "Barrage"
	/call DeclareIniVar DiscBleed 									string 	Discs "Hack"	
	/call DeclareIniVar DiscPlay	 									string 	Discs "Knifeplay Discipline"	
	/call DeclareIniVar DiscPhantom									string 	Discs "Phantom Assassin"	
	/call DeclareIniVar DiscHack										string 	Discs "Jugular Hack"	
	/call DeclareIniVar DiscHiddenBlade							string 	Discs "Hidden Blade"	
	/call DeclareIniVar DiscBladePoison							string 	Discs "Reefcrawler Blade"	

	| disable for toast
	/call DeclareIniVar DiscVision 									string 	Discs "Thief's Vision"	

	| toast
	/call DeclareIniVar DiscPinPoint 								string 	Discs "Pinpoint Deficiencies"
	/call DeclareIniVar DiscMark 										string 	Discs "Wide-Eyed Mark"
	/call DeclareIniVar DiscEradicator 							string 	Discs "Executioner Discipline"
	/call DeclareIniVar DiscRazor 									string 	Discs "Razor's Edge Discipline"	
	/call DeclareIniVar DiscFrenzy 									string 	Discs "Frenzied Stabbing Discipline"	
	/call DeclareIniVar DiscTwistedChance						string 	Discs "Twisted Chance Discipline"
	
	/call DeclareIniVar clickPoisonBuff							string 	Poison "Bite of the Shissar Poison X"
	/call DeclareIniVar clickPoisonItem							string 	Poison "Consigned Bite of the Shissar XVIII"
	/call DeclareIniVar clickPoisonDispenser				string 	Poison "Selrach's Shadowscale Leggings"
	/call DeclareIniVar clickPoisonCount						int		 	Poison 60
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
	| /call setTimer tcheck_QM												string	Timer	10s timer_check_QM
		
	/declare WorstHurt 															string 	outer FALSE
	| /declare WorstHurtID 													int 		outer ${Me.ID}
	/declare WorstHP 																int 		outer 100
	/declare WorstClass 														string 	outer FALSE
	/declare MajorHurt 															int 		outer 0
	/declare MinorHurt															int    	outer 0 
	/declare groupHurt 															int 		outer 0

	
	/declare pWorstHurt 														string 	outer ${Me.DisplayName}
	/declare pWorstHP																int			outer 100
	/declare pWorstHurtID														int 		outer ${Me.ID}
	/declare pWorstClass														string	outer FALSE
	/declare pMajorHurt															int			outer 0
	
	/declare WorstManaID														int			outer
	/declare WorstManaPct														int			outer
	
	/call DeclareIniVar DivineRezCleric							bool 		Heal TRUE
	/call DeclareIniVar RandRezDelay								int 		Heal 5
	/call DeclareIniVar ProtectOutsideGroup					string	Heal |FALSE|FALSE
	/call DeclareIniVar ProtectInsideRaid						string	Heal |FALSE|FALSE
	/call DeclareIniVar ProtectNPCList							string	Heal |FALSE|FALSE
	/call DeclareIniVar useGroupHeal 								bool		Heal TRUE
	/call DeclareIniVar useGroupHoT									bool		Heal TRUE
	/call DeclareIniVar tempPet						 					string	Pet "Ardent Hammer of Zeal"
	/call DeclareIniVar setDivinetoGroupHeal				bool		Heal FALSE

  /call DeclareIniVar healWholeGroup							string 	Heal "Word of Greater Reformation"
  /call DeclareIniVar healWholeGroupHoT						string 	Heal "Ardent Acquittal"
  /call DeclareIniVar healRemedy1									string 	Heal "Spiritual Remedy"
  /call DeclareIniVar healRemedy2									string 	Heal "Graceful Remedy"
  /call DeclareIniVar healHoT 										string 	Heal "Ardent Elixir"
  /call DeclareIniVar healSplash									string 	Heal "Convalescent Splash"
  /call DeclareIniVar healIntervention1 					string 	Heal "Virtuous Intervention"
  /call DeclareIniVar healIntervention2 					string  Heal "Elysian Intervention"
  /call DeclareIniVar healIntervention3 					string  Heal "Mystical Intervention"
  /call DeclareIniVar healRenewal1	 							string  Heal "Fervid Renewal"
  /call DeclareIniVar healRenewal2	 							string  Heal "Fraught Renewal"
  /call DeclareIniVar healRenewal3	 							string  Heal "Fervent Renewal"
  /call DeclareIniVar healPanic			 							string  Heal "Fifteenth Emblem"
  /call DeclareIniVar healWard			 							string  Heal "Ward of Surety"
  /call DeclareIniVar healWardToon	 							string  Heal |FALSE|FALSE
  /call DeclareIniVar useCelestialRegen						bool	  Heal TRUE
  /call DeclareIniVar healIssuance								string  Heal "Issuance of Spirit"
	/call DeclareIniVar CRTXT												string	Heal "Celestial Regeneration (+11100HP / tic)"
	/call DeclareIniVar switchHealAggressive				string	Heal FALSE
	/call DeclareIniVar XTHealRadius								int			Heal 300
		
	/call DeclareIniVar doYaulp 										string	Buffs TRUE
	/call DeclareIniVar sumWeaponSpell							string	Buffs "Hammer of	Reverence"
	/call DeclareIniVar sumWeaponItem								string	Buffs "Hammer of	Reverence III"
	/call DeclareIniVar AAFade											string	Buffs "Divine Peace"
	/call DeclareIniVar useAAFade										int			Buffs 50

	/call DeclareIniVar useVP												bool		Buffs TRUE
	/call DeclareIniVar useVPPct										int			Buffs 40
	| /call DeclareIniVar useQM												bool		Buffs TRUE
	| /call DeclareIniVar useQMPct										int			Buffs 40

	/call DeclareIniVar DoNuke					 						int			Spells 0
	/call DeclareIniVar Nuke1					 							string	Spells FALSE	
	/call DeclareIniVar Nuke2					 							string	Spells FALSE
	/call DeclareIniVar Nuke3					 							string	Spells FALSE
	/call DeclareIniVar Nuke4					 							string	Spells FALSE
		
/return	



|***
 *	DES: This will clear all store alises that core has created in your MacroQuest2.INI file. or those with matching names.
 *	USE: /core whitewash
 *	INI: MacroQuest2.INI
 *  NOTE: IF you instance/sandbox MQ/EQ it is best you run this on all toons at the same time. Sorry it's slow as fuck.
 ***|
sub whitewash()
	OUT ${owarning} ${sep} Deleting all Core ${cinfo}${versionCore}\ax aliases. ${cbad}Please Wait\ax
	/declare _count 				int local
	/declare _todelete[11] 	string local
	/declare _index 				int local
	/declare _increment			int local 1
	
	| commands to remove forever
	| /varset _todelete[15] |healoverride|healself|healgroup|healxtarget|healpet|healcure|healaggressive|autoshrink|toast|companionswarm|companion|home|paragon|combatradius|HuntMobAgro
	| /varset _todelete[16] |spice|stop|setagro|smartma|setpull|HUDTime|banestrike|broadcast|ib|sb|pb|resetcore|verbose|buildpet|autorest|toastauto|primary|secondary|invis|info|whohas
	| /varset _todelete[17] |rezeveryone|rezcombat|rezauto|rezemergency|rezenable|core|invites|facefast|stickbehind|hudupdate|qmpct|qm|toastmobcount|toastengageat|combatdistance
	| /varset _todelete[18] |debuffmezz|debuffslow|debufftash|debuffmalo|debuffcripple|debufferadicate|debuffsnare|checkcash|agro|engageat|autorest|smartengageat|watchspawn|petengageat|petswarmengageat
	| /varset _todelete[19] |buffbc|bufftell|buffpet|buffitem|buffraid|buffraid|buffself|buffcombat|tie|ignorelos|forage|fish|train|petfarm|drag|ri|hunt|tietoon|tiedistance|tietime
	| /varset _todelete[20]	|listremove|listnomezz|listnocast|listexclude|listnamed|listnomelee|listrange|listnomagic|listnodisease|listnofire|listnopoison|listnocold|listnocorruption|listhuntmob
	| /varset _todelete[21] |lootleave|lootkeep|lootdestroy|lootsell|buy|sell|lootwhat|lootannounce|lootignore|pettype|pettank|whitewash|doyaulp|cr|ma1|ma2|info|huntagro|killcode|useeradicate
		
	| misc HUD commnads
	/varset _todelete[1] |control|smartengageat
	
	| assist
	/varset _todelete[2] |killmob

	| modes
	/varset _todelete[3] |combat|on|off

	| Misc commands
	/varset _todelete[4] |maxlevel|autoadjustexp|oneoff|userain|crew|autologin|usebuffkill
	/varset _todelete[5] |pathiosis|bcsay|blurb|clean|stack|stein|gtfo|door|echochannel
	/varset _todelete[6] |mirror|maintexplvl|autocontrol|echoaugments|relay|givecoin|stop

	| heal
	/varset _todelete[7] |usegrouphot|usegroupheal|rc|purify

	| pets
	/varset _todelete[8]	|sendpets

	| commands
	/varset _todelete[9] |harvest|gather|rod|redorod|rods|song
	/varset _todelete[10] |glyph|lesson|wood|cani

	| movement
	/varset _todelete[11]	|here|take|homeradius|homeleash

	
	/for _count 1 to ${_todelete.Size}
		DEBUG _count: ${_count}-${_todelete.Size}
		/for _index 1 to ${_todelete[${_count}].Count[|]}
			DEBUG _index: ${_count}-${_todelete.Size}: ${_index} ${_todelete[${_count}].Arg[${_index},|]}
			OUT Deleting Alias ${_increment}: ${cinfo}${_todelete[${_count}].Arg[${_index},|]}\ax
			/squelch /alias /${_todelete[${_count}].Arg[${_index},|]} delete
			/varcalc _increment ${_increment}+1
		/next _index	
	/next _count
	
	/call echos ending
	/endm
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
	/if (!${Bool[${Alias[/take]}]}) 					/squelch /alias /take 											/bc take
	/if (!${Bool[${Alias[/door]}]}) 					/squelch /alias /door 											/bc door
	/if (!${Bool[${Alias[/stack]}]}) 					/squelch /alias /stack 											/bcaa //invis
	/if (!${Bool[${Alias[/here]}]}) 					/squelch /alias /here 											/bca //setcombathere
	
  /if (!${Bool[${Alias[/sendpets]}]}) 			/squelch /alias /sendpets										OUT SendPets: 
	/if (!${Bool[${Alias[/killmob]}]}) 				/squelch /alias /killmob 										/bc killmob
	/if (!${Bool[${Alias[/mirror]}]}) 				/squelch /alias /mirror 										/bc mirror
	/if (!${Bool[${Alias[/stein]}]}) 					/squelch /alias /stein 											/bc stein
  /if (!${Bool[${Alias[/stop]}]}) 					/squelch /alias /stop 											/bc stop
	/if (!${Bool[${Alias[/blurb]}]}) 					/squelch /alias /blurb 											OUT Blurb: 
 	/if (!${Bool[${Alias[/clean]}]}) 					/squelch /alias /clean 											OUT Clean this mess up.
	/if (!${Bool[${Alias[/crew]}]}) 					/squelch /alias /crew 											OUT Crew: 
  /if (!${Bool[${Alias[/bcsay]}]}) 					/squelch /alias /bcsay 											/bc bcsay
  /if (!${Bool[${Alias[/echoaugments]}]}) 	/squelch /alias /echoaugments								OUT Echo Augment file!
  /if (!${Bool[${Alias[/autologin]}]}) 			/squelch /alias /autologin									/plugin mq2autologin unload

	| quick cleric cure commands
	/if (!${Bool[${Alias[/rc]}]}) 						/squelch /alias /rc 												/bc rc
	/if (!${Bool[${Alias[/purify]}]}) 				/squelch /alias /purify 										/bc purify
		
  /if (!${Bool[${Alias[/buy]}]}) 						/squelch /alias /buy												/echo Buy:
  /if (!${Bool[${Alias[/sell]}]}) 					/squelch /alias /sell												/echo Sell:

	/if (!${Bool[${Alias[/on]}]}) 						/noparse /squelch /alias /on       		    	/bca //varset coreAuto TRUE
	/if (!${Bool[${Alias[/off]}]}) 						/noparse /squelch /alias /off						    /bca //varset coreAuto FALSE
	/if (!${Bool[${Alias[/zonereset]}]}) 			/noparse /squelch /alias /zonereset			    /bc zonereset

	/if (!${Bool[${Alias[/givecoin]}]}) 			/noparse /squelch /alias /givecoin			    OUT Bribe Time:

	/if (!${Bool[${Alias[/oneoff]}]}) 				/noparse /squelch /alias /oneoff   					/bc oneoff

	/if (!${Bool[${Alias[/gtfo]}]}) 					/noparse /squelch /alias /gtfo    			 		/bc gtfo
	/if (!${Bool[${Alias[/lesson]}]}) 				/noparse /squelch /alias /lesson   			 		/bc lesson

	| /if (!${Bool[${Alias[/autocontrol]}]})		/noparse /squelch /alias /autocontrol				OUT Setting Automatic Control Toon swapping!

	|***
	/if (${Select[${Me.Class.ShortName},DRU]}) {
		/if (!${Bool[${Alias[/wood]}]}) 				/squelch /alias /wood 											/bc wood
	}
	/if (${Select[${Me.Class.ShortName},SHM]}) {
		/if (!${Bool[${Alias[/aid]}]}) 					/squelch /alias /aid 												/bc aid
	}
	***|
/return


#event Join "#1# invites you to join a #2#.#*#"
sub Event_Join(string line, string _sender, string _groupraid)
	DEBUG ${cbug}Event_Join(\ax${_sender} ${_groupraid}\ax${cbug})\ax
	/if (!${switchInvites}) /return
	CHECKSAFELIST
	OUT Accepting invite from ${cpc}${_sender}\ax for ${_groupraid}.
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
  /if (${_cmd.Equal[killmob]}) {
  	/varset switchOverrideEngageat ${If[${switchOverrideEngageat},FALSE,TRUE]}
		/call echos override "Engageat" switchOverrideEngageat
  }
  /if (${_cmd.Equal[mirror]} && ${Me.ItemReady[Mirror Fragment of Anashti Sul]}) /call core_cast "Mirror Fragment of Anashti Sul" item 0
  /if (${_cmd.Equal[stein]} && ${Me.ItemReady[Drunkard's Stein]}) /call core_cast "Drunkard's Stein" item 0
  /if (${_cmd.Equal[oneoff]}) /call check1offBuffs

  /if (${_cmd.Equal[lesson]} && ${Me.AltAbilityReady[Lesson of the Devoted]}) /call core_cast "Lesson of the Devoted" alt 0
  
	| commands to control all assist targets.  
  /if (${_cmd.Equal[sma1]} && !${switchPull} && !${Select[${Me.Class.ShortName},CLR]}) {
		/smartma 1
  } else /if (${_cmd.Equal[sma2]} && !${switchPull} && !${Select[${Me.Class.ShortName},CLR]}) {
		/smartma 2
  } else /if (${_cmd.Equal[smag]} && !${switchPull} && !${Select[${Me.Class.ShortName},CLR]}) {
		/smartma G
	} else /if (${_cmd.Equal[smax]}) {
		/smartma X
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
	}


	|***
	 *	DES: Tell toon to take doors
	 *	USE: /door, /bc door
	 ***|
	/if (${_cmd.Equal[door]}) {
		/call create_timer timer_try_door 4s
		:retrydoortarget
		/echo here
		/if (!${timer_try_door}) /return
		/echo here 2
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
	}

	|***
	* DES: Tell toon to campfire clickie
	* USE: /bc campfire
	***|
	/if (${_cmd.Equal[campfire]} && ${Me.ItemReady[Fellowship Registration Insignia]}) {
		/if (${Me.Invis}) /makemevisible
		/delay 1s !${Me.Invis}
		/call core_cast  "Fellowship Registration Insignia" item 0
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
		/return
	}

	| wis healers only
	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,PAL]}) {
		| radient Cure
		/if (${_cmd.Equal[rc]} &&  ${Me.AltAbilityReady[Radiant Cure]}) {
			/call core_cast "Radiant Cure" alt 0
			ECHOCHANNEL Radient Cure
		}
		
		| Purify Spirits
		/if (${_cmd.Equal[purify]} &&  && ${Me.AltAbilityReady[Purified Spirits]}) {
			/call core_cast "Purified Spirits" alt ${Spawn[pc ${_sender}].ID}
			ECHOCHANNEL Purified Spirits: ${Spawn[pc ${_sender}].Name}
		}
		/return
	}

/return



|***
 *	DES: contains the various BC commands witn ONE variable passed
 *	USE: /bc SOMETHINGORANOTHER
 *	INI: 
 *  NOTE: works off tells or broadcasts
 ***|
#event EQBC_say "<#1#> bcsay #2#"
#event EQBC_say "[#1#(msg)] bcsay #2#"
sub Event_EQBC_say(string line, string _sender, string _verbage)
	DEBUG _sender ${_sender} _verbage ${_verbage}
 	| have everyone say the same thing to the same target with a touch of random delay
 		/if (${_sender.NotEqual[${Me.DisplayName}]} && ${Bool[${Spawn[pc ${_sender} radius 40].ID}]}) {
			/assist ${_sender}
			/delay 5s ${Me.AssistComplete} 
			/if (!${Target.ID}) /return
		}
		/if (${Me.Invis}) {
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
	OUT Core Build ${cinfo}${coreBuild}\ax ${sep} ${cbad}STOPPED\ax for camp.
	/endm
	/squelch /makecamp off
	/squelch /moveto pause
	/squelch /stick off
/return

#event skillup "#*#You have become better at #1#! (#2#)"
sub Event_skillup(string line, string skillname, int skillup)
   OUT [${cinfo}${skillname}\ax] has increased to (${cinfo}${skillup}\ax)! [Max:${cinfo} ${Skill[${skillname}].SkillCap}\ax]
/return

#event task_update "Your task '#1#' has been updated."
sub Event_task_update(string line, string skillname)
   /bc [+g+]'[+x+][+y+]${skillname}[+x+][+g+]'[+x+] updated!
/return

#event noRaid "Your raid was disbanded."
sub Event_noRaid()
 		/if (${useSmartAssist1}) /call writeINI useSmartAssist1 Common FALSE
		/if (${useSmartAssist2}) /call writeINI useSmartAssist2 Common FALSE
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
			OUT ${cpc}${_sender}\ax is already in the raid.
			/return
		}
		/if (${invitelist.Find[${_sender}]}) {
			OUT ${cbad}NO\ax invites for ${cpc}${_sender}\ax.
			/return
		}
		/declare _timeDelay ${Math.Calc[${Math.Rand[40]}+20]}
		/varset _timeDelay ${Int[${_timeDelay}]}
		/delay 1m !${Raid.Locked}
		OUT Delay: ${cbug}${_timeDelay}\ax \ag- \ax ${cpc}${_sender}\ax requested a raid invite. [Raid: ${If[${Raid.Locked},${cbad}LOCKED\ax,${cgood}UNLOCKED\ax]}]
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
		OUT ${Ini[${INIBlurb},${exp.Upper}]}		
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
*	DES: Invites Crew 
*	USE: /crew [name1|name2|so on]
***|
#event Crew "[MQ2] - Crew: #1#"
sub Event_Crew(string line, string _clist)
	DEBUG ${cbug}Event_Crew(\ax${exp.Upper}${cbug})\ax
	/call set_crew ${_clist}
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
 *  NOTE: 
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
		OUT ${ocaution} ${sep} Invalid coin type. Try again.
		/return
	}

	/if (!${Window[InventoryWindow].Open}) /invoke ${Window[InventoryWindow].DoOpen}

	/declare _count 			int local 
	/declare _countloop 	int local 1

	:loopgivemorecash
		OUT Giving ${_coin} stack ${sep} ${_countloop} of ${_stacks}
		/if (!${Window[InventoryWindow].Open}) /return
		/if (!${Target.ID} || ${Taret.ID} == (${Me.ID} || ${Pet.ID})) /return
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

	OUT ${onotice} ${sep} Done giving cash for bribes.	How the mighty have fallen.
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
  	OUT ${owarning} ${sep} Recieved tell from ${cpc}${_sender}\ax to end current macro.
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
   /varset CantLootID ${Target.ID}
/return



|***
 *	DES: This is a cover all for the null slot issue when swapping gear.
 *	USE: NA
 *	INI: NA
 *  NOTE: 
 ***| 	
#event NullSlot "#*#Invalid item slot 'null#*#"
sub Event_NullSlot
	OUT ${owarning} ${sep} Opening all bags to correct null slot
	/keypress OPEN_INV_BAGS
	/timed 10 /keypress CLOSE_INV_BAGS
/return



|***
 *	DES: Used whenever we zone. Preps toon for mob lists, movement, etc.
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***| 
#event Zone "LOADING, PLEASE WAIT..."
sub Event_Zone()
	/call echos zone 0 0 0 0 
	/call check_zone_INI
	/if (${burnForce}) /varset burnForce FALSE
	/makecamp off
	/if (${HomeSet}) /call writeINI HomeSet Movement FALSE
	/call switch_clear_mode
	/if (${coreAuto} && ${switchShrink}) /call check_shrink
	
	/call create_timer timer_zone_pause ${tZone_Pause}
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
		OUT Need an item to buy, in single qoutes.
	} else /if (${_buyAmount} < 1) {
		OUT Need a valid quantity. 
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
 *	DES: Adjusts the smart assist through Raid 1 then Raid 2 then Group then off
 *	USE: /smartma [1|2|X|G]
 *	INI: 
 *  NOTE: this is a four way toggle
 ***| 	
#bind smartMA /smartma
sub Bind_smartMA(string _assist)
	DEBUG ${cbug}Event_smartma()\ax
	/call clear_combat

	| Raid Assist 1
	/if ((${_assist.Equal[1]} || (!${Bool[${_assist}]} && !${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG} && ${Bool[${Raid.Members}]})) && !${Select[${Me.Class.ShortName},CLR]} && !${switchPull}) {
		DEBUG ((${_assist} || (!${Bool[${_assist}]} && !${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG} && ${Bool[${Raid.Members}]})) && !${Select[${Me.Class.ShortName},CLR]} && !${switchPull})
 		/if (!${useSmartAssist1}) /call writeINI useSmartAssist1 Common TRUE
		/if (${useSmartAssist2}) /call writeINI useSmartAssist2 Common FALSE
		/if (${useSmartAssistG}) /call writeINI useSmartAssistG Common FALSE
		/xtarget set 1 Raidassist1target
		OUT Setting Assist ${sep} ${cinfo}Raid 1\ax

	| Raid Assist 2
	} else /if ((${_assist.Equal[2]} || (!${Bool[${_assist}]} && ${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG} && ${Bool[${Raid.Members}]})) && !${Select[${Me.Class.ShortName},CLR]} && !${switchPull}) {
		DEBUG ((${_assist} || (!${Bool[${_assist}]} && ${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG} && ${Bool[${Raid.Members}]})) && !${Select[${Me.Class.ShortName},CLR]} && !${switchPull})
 		/if (${useSmartAssist1}) /call writeINI useSmartAssist1 Common FALSE
		/if (!${useSmartAssist2}) /call writeINI useSmartAssist2 Common TRUE
		/if (${useSmartAssistG}) /call writeINI useSmartAssistG Common FALSE
		/xtarget set 1 Raidassist1target
		OUT Setting Assist ${sep} ${cinfo}Raid 2\ax

	| NO Assist
	} else /if ((${_assist.Equal[X]} || (!${Bool[${_assist}]} && !${useSmartAssist1} && !${useSmartAssist2} && ${useSmartAssistG}) || (!${Bool[${Raid.Members}]} && !${Bool[${Group.Members}]})) || ${Select[${Me.Class.ShortName},CLR]} || ${switchPull}) {
		DEBUG ((${_assist} || (!${Bool[${_assist}]} && !${useSmartAssist1} && !${useSmartAssist2} && ${useSmartAssistG}) || (!${Bool[${Raid.Members}]} && !${Bool[${Group.Members}]})) || ${Select[${Me.Class.ShortName},CLR]} || ${switchPull})
 		/if (${useSmartAssist1}) /call writeINI useSmartAssist1 Common FALSE
		/if (${useSmartAssist2}) /call writeINI useSmartAssist2 Common FALSE
		/if (${useSmartAssistG}) /call writeINI useSmartAssistG Common FALSE
		/if (${Bool[${MA1}]}) /call writeINI MA1 Common FALSE
		/if (${Bool[${MA2}]}) /call writeINI MA2 Common FALSE
		/xtarget set 1 Autohater
		OUT Setting Assist ${sep} ${odisabled}\ax

	| Goup Main Assist
	} else /if ((${_assist.Equal[G]} || (!${Bool[${_assist}]} && !${useSmartAssist1} && ${useSmartAssist2} && !${useSmartAssistG}) || (!${Bool[${Raid.Members}]} && ${Bool[${Group.Members}]} && !${useSmartAssistG})) && !${Select[${Me.Class.ShortName},CLR]} && !${switchPull}) {
		DEBUG ((${_assist} || (!${Bool[${_assist}]} && !${useSmartAssist1} && ${useSmartAssist2} && !${useSmartAssistG}) || (!${Bool[${Raid.Members}]} && ${Bool[${Group.Members}]} && !${useSmartAssistG})) && !${Select[${Me.Class.ShortName},CLR]} && !${switchPull})
 		/if (${useSmartAssist1}) /call writeINI useSmartAssist1 Common FALSE
		/if (${useSmartAssist2}) /call writeINI useSmartAssist2 Common FALSE
		/if (!${useSmartAssistG}) /call writeINI useSmartAssistG Common TRUE
		/xtarget set 1 Groupassisttarget
		OUT Setting Assist ${sep} ${cinfo}Group\ax
	}
/return



|***
 *	DES: stack invis
 *	USE: /invis
 *	INI: 
 *  NOTE:
 ***| 
#bind cast_AA_invis /invis
sub Bind_cast_AA_invis()
	/if (${Me.AltAbilityReady[Group Perfected Invisibility]} && ${Select[${Me.Class.ShortName},WIZ,MAG,ENC]}) {
		/call MQ2Cast "Group Perfected Invisibility" alt -targetid|${Me.ID}
		ECHOCHANNEL AA: Group Perfected Invisibility
	}  
	/if (${Me.AltAbilityReady[Group Perfected Invisibility to Undead]} && ${Select[${Me.Class.ShortName},CLR,SHD,NEC]}) {
		/delay 3
		/call MQ2Cast "Group Perfected Invisibility to Undead" alt -targetid|${Me.ID}
		ECHOCHANNEL AA: Group Perfected Invisibility to Undead
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
		/if (${Me.Casting.ID}) /stopcast
		/call core_cast "Primary Anchor Transport Device" item 0
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
		/if (${Me.Casting.ID}) /stopcast
		/call core_cast "Secondary Anchor Transport Device" item 0
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
		OUT ${sep} ${cinfo}/watchspawn\ax requires a taret or mob partial name. 
		OUT ${sep} NPC and CORPSE is also acceptable.
		/if (!${Bool[${_spawn}]}) /call writeINI watchSpawn Common FALSE
	}
/return


	
 |***
 *	DES: entry point for finding something in inventory or bank
 *	USE: /findathing
 *	INI: 
 *  NOTE: you gotta tell it what you want to find. no i'm not kidding, it doesn't just magically know. 
 ***|
#bind findathing /findathing
sub Bind_findathing(string _verbage)
	/call check_for_item "${_verbage}"
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
	/declare _count int local 0
	
	/if (${Bool[${overrideType.Find[${_overrideType}]}]} && ${_overrideType.NotEqual[stop]}) {
		/echo switchOverride${_overrideType.Left[1].Upper}${_overrideType.Right[-1].Lower}
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
sub Bind_deBuff(string _debuffType)
	DEBUG ${cbug}Bind_deBuff(type: ${_debuffType})\ax
	/declare _count int local 0
	
	/if (${Bool[${deBuffType.Find[${_debuffType}]}]} && ${_debuffType.NotEqual[stop]}) {
		/if (${Defined[switchdeBuff${_debuffType.Left[1].Upper}${_debuffType.Right[-1].Lower}]}) {
			/call writeINI switchdeBuff${_debuffType.Left[1].Upper}${_debuffType.Right[-1].Lower} Buffs ${If[${switchdeBuff${_debuffType.Left[1].Upper}${_debuffType.Right[-1].Lower}},FALSE,TRUE]}  
		}
	} else /if (${Bool[${deBuffType.Equal[${_debuffType}]}]} && ${_debuffType.Equal[stop]}) {
		/for _count 1 to ${deBuffType.Count[|]}
			/if (${switchdeBuff${deBuffType.Arg[${_count},|].Left[1].Upper}${deBuffType.Arg[${_count},|].Right[-1].Lower}}) {
				/call writeINI switchdeBuff${deBuffType.Arg[${_count},|].Left[1].Upper}${deBuffType.Arg[${_count},|].Right[-1].Lower} Buffs FALSE  
			}
		/next _count
	} 
	/declare _listout string local
	/declare _sep bool local FALSE
	/for _count 1 to ${deBuffType.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${switchdeBuff${deBuffType.Arg[${_count},|].Left[1].Upper}${deBuffType.Arg[${_count},|].Right[-1].Lower}},${cgood},${cinfo}]}${deBuffType.Arg[${_count},|]}\ax
		/varset _sep TRUE
	/next _count
	OUT /debuff [${_listout}]			


	| clear stickbehind. it will force bad targeting fot the mezzer.
	/if (${SetStickBehind} && ${switchdeBuffMezz}) {
		/call writeINI SetStickBehind Movement FALSE  
		/call echos switch "Stick behind target"  SetStickBehind 0 0
	}
/return



|***
 *	DES: set various heal switches
 *	USE: /heal []
 *	INI: 
 *  NOTE:
 ***| 	
#bind heal /heal
sub Bind_heal(string _healType, int _healInt)
	DEBUG ${cbug}Bind_heal(type: ${_healType})\ax
	/declare _count		int local
	
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
	| heal override
	} else /if (${_healType.Equal[override]}) {
		/if (${Range.Between[0,99:${Int[${_healInt}]}]}) {
			/call writeINI HealOverride Heal ${_healInt} 
			OUT Heal ${sep} ${cinfo}override\ax ${sep} ${If[${_healInt},${_healInt},${odisabled}]}
		} else {
			OUT /heal override [${cinfo}${HealOverride}\ax] allowable range ${cinfo}0 - 99\ax
		}
	}
	/if (${Select[${Me.Class.ShortName},DRU,CLR,SHM]} && ${_healType.Equal[self]}) OUT Heal Classes [CLR,DRU,SHM] use /heal group. Not /heal self.
	/declare _listout string local
	/declare _sep bool local
	/for _count 1 to ${healType.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${switchHeal${healType.Arg[${_count},|].Left[1].Upper}${healType.Arg[${_count},|].Right[-1].Lower}},${cgood},${cinfo}]}${healType.Arg[${_count},|]}\ax
		/varset _sep TRUE
	/next _count
	OUT /heal [${_listout}] [${cinfo}##\ax]				
/return
	


|***
 *	DES: set various buff switches
 *	USE: /buff []
 *	INI: 
 *  NOTE:
 ***| 	
#bind buff /buff
sub Bind_buff(string _buffType)
	DEBUG ${cbug}Bind_buff(type: ${_buffType})\ax
	/declare _count int local 0
	
	/if (${_buffType.Equal[stop]}) {
		/for _count 1 to ${buffType.Count[|]}
			/if (${switchBuff${buffType.Arg[${_count},|].Left[1].Upper}${buffType.Arg[${_count},|].Right[-1].Lower}}) {
				/call writeINI switchBuff${buffType.Arg[${_count},|].Left[1].Upper}${buffType.Arg[${_count},|].Right[-1].Lower} Buffs FALSE  
			}
		/next _count
	} else /if (${_buffType.Equal[self]}) {
		/call writeINI switchBuffSelf Buffs ${If[${switchBuffSelf},FALSE,TRUE]}
	} else /if (${_buffType.Equal[item]}) {
		/call writeINI switchBuffItem Buffs ${If[${switchBuffItem},FALSE,TRUE]}
	} else /if (${_buffType.Equal[combat]}) {
		/call writeINI switchBuffCombat Buffs ${If[${switchBuffCombat},FALSE,TRUE]}
	} else /if (${_buffType.Equal[bc]}) {
		/call writeINI switchBuffBc Buffs ${If[${switchBuffBc},FALSE,TRUE]}
	} else /if (${_buffType.Equal[raid]}) {
		/call writeINI switchBuffRaid Buffs ${If[${switchBuffRaid},FALSE,TRUE]}
	} else /if (${_buffType.Equal[tell]}) {
		/call writeINI switchBuffTell Buffs ${If[${switchBuffTell},FALSE,TRUE]}
	} else /if (${_buffType.Equal[pet]}) {
		/if (${Select[${Me.Class.ShortName},NEC,ENC,SHM,DRU,BST,SHD,MAG]}) {
			/call writeINI switchBuffPet Pet ${If[${switchBuffPet},FALSE,TRUE]}
		} else {
			/call writeINI switchBuffPet Pet FALSE
			/call echos failboat 0 0 0 0
		}
	}
	/declare _listout string local
	/declare _sep bool local
	/for _count 1 to ${buffType.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${switchBuff${buffType.Arg[${_count},|].Left[1].Upper}${buffType.Arg[${_count},|].Right[-1].Lower}},${cgood},${cinfo}]}${buffType.Arg[${_count},|]}\ax
		/varset _sep TRUE
	/next _count
	OUT /buff [${_listout}]				
/return




|***
 *	DES: cursor control
 *	USE: /cursor []
 *	INI: 
 *  NOTE: 
 ***| 
#bind cursor /cursor
sub Bind_cursor(string _cursorType, string _time)
	DEBUG ${cbug}Bind_cursor(type: ${_cursorType})\ax
	/declare _properType string local
	
	/if (${_cursorType.Equal[auto]}) {
		/call writeINI switchAutoCursor Common ${If[${switchAutoCursor},FALSE,TRUE]}  
		OUT Cursor ${sep} ${cinfo}Auto\ax ${sep} ${If[${switchAutoCursor},${oenabled},${odisabled}]}
		/if (${timer_auto_cursor_pause}) /varset timer_auto_cursor_pause 1
	} else /if (${_cursorType.Equal[pause]}) {
		/call create_timer timer_auto_cursor_pause ${If[${Bool[${_time}]},${_time},30s]}
		OUT Cursor ${sep} ${cinfo}Pause\ax ${sep} ${If[${Bool[${_time}]},${_time},30s]}
	} else /if (${Cursor.ID} && ${Bool[${cursorType.Find[${_cursorType}]}]} && ${_cursorType.Equal[what]}) {
		/call echos cursor "${Cursor.Name}" ${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}]} 0 0
	} else /if (${Cursor.ID} && ${Bool[${cursorType.Find[${_cursorType}]}]} && ${_cursorType.NotEqual[what]} && ${_cursorType.NotEqual[auto]}) {
		/varset _properType ${_cursorType.Left[1].Upper}${_cursorType.Right[-1].Lower}
		/ini "${INILoot}" "${Cursor.Name.Left[1]}" "${Cursor.Name}" "${_properType}"
		/call echos cursor "${Cursor.Name}" ${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}]} 0 0
		/if (${Cursor.ID} && ${_cursorType.Equal[Destroy]}) /destroy
	} else {	
		/call echos listtype cursor cursor 0 0
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
	
	/if (${Bool[${rezType.Find[${_rezType}]}]} && ${_rezType.NotEqual[stop]} && ${_rezType.NotEqual[pct]} && ${_rezType.NotEqual[range]}) {
		/if (${Defined[switchRez${_rezType.Left[1].Upper}${_rezType.Right[-1].Lower}]}) {
			/call writeINI switchRez${_rezType.Left[1].Upper}${_rezType.Right[-1].Lower} Heal ${If[${switchRez${_rezType.Left[1].Upper}${_rezType.Right[-1].Lower}},FALSE,TRUE]}
			/call echos rez "${_rezType.Left[1].Upper}${_rezType.Right[-1].Lower}" switchRez${_rezType.Left[1].Upper}${_rezType.Right[-1].Lower} 0 0
		} 
	
	| turn off all rez controls	
	} else /if (${_rezType.Equal[stop]}) {
		/for _count 1 to ${rezType.Count[|]}
			DEBUG switchRez${rezType.Arg[${_count},|].Left[1].Upper}${rezType.Arg[${_count},|].Right[-1].Lower}
			/if (${switchRez${rezType.Arg[${_count},|].Left[1].Upper}${rezType.Arg[${_count},|].Right[-1].Lower}}) {
				/call writeINI switchRez${rezType.Arg[${_count},|].Left[1].Upper}${rezType.Arg[${_count},|].Right[-1].Lower} Heal FALSE
				/call echos rez "${rezType.Arg[${_count},|].Left[1].Upper}${rezType.Arg[${_count},|].Right[-1].Lower}" switchRez${rezType.Arg[${_count},|].Left[1].Upper}${rezType.Arg[${_count},|].Right[-1].Lower} 0 0
			}
		/next _count
		
	| set rez accept percent	
	} else /if (${_rezType.Equal[pct]}) {
		/if (${Range.Between[10,96:${Int[${_verbage}]}]}) {
			/if (${rezPct.Find[${_verbage}]}) /call writeINI minRezPct Common ${_verbage}
		}
		/declare _listout string local
		/declare _sep bool local FALSE
		/for _count 1 to ${rezPct.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${minRezPct} == ${rezPct.Arg[${_count},|]},${cgood},${cinfo}]}${rezPct.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		OUT /rez pct [${_listout}]
	} else /if (${_rezType.Equal[range]}) {
		/if (${Range.Between[1,110:${Int[${_verbage}]}]}) {
			/call writeINI maxRezRange Heal ${_verbage}
			| OUT /rez range [${cinfo}${_verbage}\ax]
		} else {
			OUT /rez range [${cinfo}${maxRezRange}\ax] allowable range is ${cinfo}1 - 110\ax
			/return
		}
	} 
	/declare _listout string local
	/declare _sep bool local FALSE
	/for _count 1 to ${rezType.Count[|]}
		/if (${rezType.Arg[${_count},|].Equal[pct]}) {
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}pct[${cgood}${minRezPct}\ax]\ax
		} else /if (${rezType.Arg[${_count},|].Equal[range]}) {
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}range[${cgood}${maxRezRange}\ax]\ax
		} else {
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${switchRez${rezType.Arg[${_count},|].Left[1].Upper}${rezType.Arg[${_count},|].Right[-1].Lower}},${cgood},${cinfo}]}${rezType.Arg[${_count},|]}\ax
		}
		/varset _sep TRUE
	/next _count
	OUT /rez [${_listout}] [${cinfo}##\ax]]			
/return




|***
 *	DES: list control
 *	USE: /lc []
 *	INI: core.zone.ini
 *  NOTE: 
 ***| 
#bind listControl /lc
sub Bind_listControl(string _list)
	DEBUG ${cbug}Bind_listControl (control: ${_list})\ax
	/declare _properType 			string local
	/declare _count 					int local 0
	/declare _listchanged 		bool local FALSE
	/declare _validName				bool local FALSE

	| disable all commands but safezone while in a safezone
	/if (${SafeZone} && ${_list.NotEqual[safe]} && ${_list.NotEqual[reset]}) {
		OUT ${onotice} ${sep} List Control is disabled while in a Safe Zone.
		/return
	}

	| require a target. one that is not a PC
	/if ((!${Target.ID} || ${Bool[${Spawn[pc ID ${Target.ID}]}]}) && ${_list.NotEqual[safe]} && ${_list.NotEqual[reset]}) {
		DEBUG NO /lc Target
		/call echos needtarget 0 0 0 0
	} 

	| validate _name and correct the case
	/if (${Bool[${_list}]} && ${_list.NotEqual[safe]} && ${_list.NotEqual[reset]}) {
		/for _count 1 to ${listType.Count[|]}
			DEBUG Match: ${listType.Arg[${_count},|]} ${If[${Bool[${_list.Find[${listType.Arg[${_count},|]}]}]},${cgood}==\ax,${cbad}!=\ax]} ${_list}
			
			/if (${Bool[${_list.Find[${listType.Arg[${_count},|]}]}]}) {
				/if (${_list.Left[2].Equal[no]}) {
					DEBUG ${_list.Left[2].Left[1].Upper}${_list.Left[2].Right[-1].Lower}${_list.Right[-2].Left[1].Upper}${_list.Right[-2].Right[-1].Lower}
					/varset _list ${_list.Left[2].Left[1].Upper}${_list.Left[2].Right[-1].Lower}${_list.Right[-2].Left[1].Upper}${_list.Right[-2].Right[-1].Lower}
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
		/call echos switch "Safe Zone ${sep} ${cinfo}${Zone.Name}\ax" SafeZone 0 0
		/varset _listchanged TRUE
	} else /if (${_list.Equal[reset]}) {
		DEBUG Reset Zone
		/call echos zone 0 0 0 0
		/call check_zone_INI ${Me.DisplayName}
	} else /if (${_list.Equal[remove]}) {
		DEBUG remove from INI
		/declare _mobName string local |${Target.DisplayName}
		OUT List ${_list} ${sep} ${ctar}${Target.DisplayName}\ax ${sep} ${cinfo}${Zone.Name}\ax

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
			OUT ${onotice} ${sep} [${ctar}${Target.DisplayName}\ax] already exists in ${sep} ${cinfo}${Zone.Name}\ax
		} else /if (!${${_list}List.Find[${Target.DisplayName}]}) {
			DEBUG Adding
			OUT List ${_list} ${sep} ${ctar}${Target.DisplayName}\ax ${sep} ${cinfo}${Zone.Name}\ax
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
sub Bind_debug(string _debugType)
	DEBUG ${cbug}Bind_debug(Debug: ${_debugType})\ax
	/declare _count int local 0
	/if (${Bool[${debugType.Find[${_debugType}]}]} && ${_debugType.NotEqual[stop]}) {
		/if (${Defined[switchDebug${_debugType.Left[1].Upper}${_debugType.Right[-1].Lower}]}) {
			/call writeINI switchDebug${_debugType.Left[1].Upper}${_debugType.Right[-1].Lower} debug ${If[${switchDebug${_debugType.Left[1].Upper}${_debugType.Right[-1].Lower}},FALSE,TRUE]}  
		} 
	} else /if (${_debugType.Equal[stop]}) {
		/for _count 1 to ${debugType.Count[|]}
			/if (${switchDebug${debugType.Arg[${_count},|].Left[1].Upper}${debugType.Arg[${_count},|].Right[-1].Lower}}) {
				/call writeINI switchDebug${debugType.Arg[${_count},|].Left[1].Upper}${debugType.Arg[${_count},|].Right[-1].Lower} debug FALSE
			}
		/next _count	
	}
	/declare _listout string local
	/declare _sep bool local FALSE
	/for _count 1 to ${debugType.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${switchDebug${debugType.Arg[${_count},|].Left[1].Upper}${debugType.Arg[${_count},|].Right[-1].Lower}},${cgood},${cinfo}]}${debugType.Arg[${_count},|]}\ax
		/varset _sep TRUE
	/next _count
	OUT /debug [${_listout}]		
/return  



#bind petControl /pc
sub Bind_petControl(string _pcType, string _verbage, string _pcString)
	DEBUG ${cbug}Bind_petControl(type: ${_pcType}, #: ${_verbage})\ax

	| build and buff
	/if (${Bool[${_pcType.Find[build]}]}) {
		/call check_pet
		/if (${Me.Pet.ID}) /call check_pet_buffs	

	| pet/engage#
	} else /if (${Bool[${_pcType.Equal[pet]}]}) {
		/if (!${Bool[${_verbage}]}) {
			/call writeINI switchPcPet Pet ${If[${switchPcPet},FALSE,TRUE]}
			/call echos switch "Use Pet" switchPcPet 0 0
			/if (!${switchPcPet} && ${Pet.ID}) {
				/if (${Me.AltAbilityReady[Suspended Minion]}) {
					/call core_cast "Suspended Minion" alt 0
				} else {
					/pet leave
				}
			}
		} else /if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI PcPetengageat Pet ${_verbage}
			/call echos number "Pet Engage" ${_verbage} 0 0
		} else /if (!${Bool[${_pcNum}]} || !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			OUT /pc pet [${cinfo}${PcPetengageat}\ax] valid range ${cinfo}1 - 99\ax
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
			OUT /pc swarm [${cinfo}${PcSwarmengageat}\ax] valid range ${cinfo}1 - 99\ax
		}

		
	| pc tank
	} else /if (${Bool[${_pcType.Equal[tank]}]}) {
		/call writeINI switchPcPettank Pet ${If[${switchPcPettank},FALSE,TRUE]}
		/call echos switch "Pet Tank" switchPcPettank	0 0
		
	| pc type
	} else /if (${Bool[${_pcType.Equal[type]}]}) {
		/call writeINI PetTypeSpell Pet ${_pcString}
	
		
		
	| pc bag ##
	} else /if (${Bool[${_pcType.Equal[bag]}]}) {
		/if (${Range.Between[1,10:${Int[${_verbage}]}]}) {
			/call writeINI PetBagSlotNumber Pet ${_verbage}
			/call echos number "Pet Weapons Slot" ${_verbage} 0 0
		} else {
			OUT /pc bag [${cinfo}${PetBagSlotNumber}\ax] valid range range ${cinfo}1 - 10\ax
		}


	} else {
		/call echos listtype pc pc 0 0
	} 

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
		OUT Sending ${cpc}${Pet.DisplayName}\ax
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

	| set time to toon name
	/if (${Bool[${_tcType.Equal[name]}]}) {
		/if (${SetTieToon.NotEqual[${_verbage}]} && ${_verbage.NotEqual[${Me.DisplayName}]}) {	
			/call writeINI SetTieToon Movement ${_verbage}	
			/call echos	string "Set Tie Toon" _verbage 0 0
		}
	| set tie time
	} else /if (${Bool[${_tcType.Equal[time]}]}) {
		/if (${Range.Between[5,50:${Int[${_verbage}]}]}) {
			/call writeINI tCheck_Tie Timer ${_verbage} 
		/call echos number "Tie Check Time" ${_verbage} 0 0
		} else {
			OUT /tc time [${cinfo}${tCheck_Tie}\ax] allowable range is ${cinfo}5 - 50\ax
		}
		
	| set tie distance		
	} else /if (${Bool[${_tcType.Equal[distance]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI SetTieDistance Movement ${_verbage} 
			/call echos number "Tie Distance" ${_verbage}	0 0	
		} else {
			OUT /tc distance [${cinfo}${SetTieDistance}\ax] allowable range is ${cinfo}1 - 99\ax
		}
	} else {
		/call echos listtype tc tc 0 0
	} 
/return



|***
 *	DES: enable/disable Tie
 *	USE: /tie
 *	INI: 
 *  NOTE:
 ***| 	
#bind setTie /tie
sub Bind_setTie()
	/if (${Bool[${Spawn[pc ${SetTieToon}].ID}]}) /varset SetTie ${If[${SetTie},FALSE,TRUE]}  
	/if (!${Bool[${Spawn[pc ${SetTieToon}].ID}]}) /varset SetTie FALSE 
	OUT Tie to ${cpc}${SetTieToon}\ax ${sep} ${If[${SetTie},${oenabled},${odisabled}]}
/return



|***
 *	DES: Burn control
 *	USE: /burn []
 *	INI: 
 *  NOTE:
 ***| 	
#bind burn /burn
sub Bind_burn(string _burnType, int _burnNum)
	DEBUG ${cbug}Bind_burn()\ax

	| Burn Automatic on/off
	/if (${Bool[${_burnType.Find[auto]}]}) {
		/call writeINI burnAuto Common ${If[${burnAuto},FALSE,TRUE]}  
		/call echos switch "Burn Auto" burnAuto 0 0

	| Mob count #
	} else /if (${Bool[${_burnType.Equal[count]}]}) {
	 	/if (${Range.Between[1,99:${Int[${_burnNum}]}]}) {
			/call writeINI burnCount Common ${_burnNum} 
			/call echos number "Burn # Mob Count" ${_burnNum} 0 0
		} else {
			OUT /burn count [${cinfo}##\ax] of mobs in range [${cbug}${burnCount}\ax] ${cinfo}1 - 99\ax
		}

	| Burn Engage at %
	} else /if (${Bool[${_burnType.Equal[engageatat]}]}) {
	 	/if (${Range.Between[1,99:${Int[${_burnNum}]}]}) {
			/call writeINI burnEngageat Common ${_burnNum} 
			/call echos number "Burn health enagage %" ${_burnNum} 0 0
		} else {
			OUT /burn engageat [${cinfo}##\ax] % of mobs health to engage Burn [${cbug}${burnEngageat}\ax] ${cinfo}1 - 99\ax
		}

	| force constant burning
	} else /if (${Bool[${_burnType.Equal[force]}]}) {
		/varset burnForce ${If[${burnForce},FALSE,TRUE]}
		/call echos switch "Constant Burn" burnForce 0 0

	} else {
		/call echos listtype burn burn 0 0
	} 

/return



|***
 *	DES: core controls
 *	USE: /core [] [TEXT]
 *	INI: 
 *  NOTE:
 ***|
#bind setCore /core
sub Bind_setCore(string _coreType, string _coreVerbage)
	DEBUG ${cbug}Bind_setCore(type:${_coreType}, verbage:${_coreVerbage})\ax

	| core auto/manual
	/if (${Bool[${_coreType.Equal[auto]}]}) {
		/call writeINI coreAuto Common ${If[${coreAuto},FALSE,TRUE]}
		/varset combat ${If[${coreAuto},${Ini[${INIClass},Common,combat]},FALSE]}  
		/if (!${coreAuto}) { 
			/call set_stop TRUE
			/call clear_combat
		}
		OUT Core ${sep} ${cinfo}Automatic\ax ${sep} ${If[${coreAuto},${oenabled},${odisabled}]}
			
	| enable/disable verbose EQBC Echos
	} else /if (${Bool[${_coreType.Equal[verbose]}]}) {
		/call writeINI verbose Common ${If[${verbose},FALSE,TRUE]}
		OUT Core ${sep} ${cinfo}Verbose\ax ${sep} ${If[${verbose},${oenabled},${odisabled}]}

	| enable/disable obscure target names ## not documented
	} else /if (${Bool[${_coreType.Equal[obscure]}]}) {
		/varset coreObscure ${If[${coreObscure},FALSE,TRUE]}
		OUT Core ${sep} ${cinfo}Obscure Names\ax ${sep} ${If[${coreObscure},${oenabled},${odisabled}]}

	| set the kill code
	} else /if (${Bool[${_coreType.Equal[killcode]}]}) {
		/if (${Bool[${_coreVerbage}]}) {
			/call DeclareCoreIniVar NA remoteKillKey General ${_coreVerbage} 
			OUT Core ${sep} ${cinfo}Remote Kill Code\ax ${sep} ${_coreVerbage}
		} else {
			OUT Current killcode: ${cinfo}${remoteKillKey}\ax
		}

	| runs the whitewash routine to clear all aliases
	} else /if (${Bool[${_coreType.Equal[whitewash]}]}) {
		/call whitewash
		
	} else /if (${Bool[${_coreType.Equal[rest]}]}) {
		/declare _num int local ${_coreVerbage}
		/if (${Range.Between[0,99:${Int[${_num}]}]}) {
			/call writeINI autoRest Common ${_num}  
			/call echos number "Auto Rest" ${If[${_num},${_num},${autoRest}]} 0 0
		} 		

	| set control toon
	} else /if (${Bool[${_coreType.Equal[control]}]}) {
		/if (${Bool[${_coreVerbage}]}) {
			/call writeINI toonControl Common ${_coreVerbage.Left[1].Upper}${_coreVerbage.Right[-1].Lower}
		} else /if (!${Bool[${_coreVerbage}]}) {
			/call writeINI toonControl Common FALSE
		}
		OUT Control Toon ${sep} ${If[${Bool[${toonControl}]},${cgood}${toonControl}\ax,${odisabled}]}

	| enable/disable relay tells
	} else /if (${Bool[${_coreType.Equal[relay]}]}) {
		/if (${Bool[${_coreVerbage}]}) {
			/call writeINI relayTells Common ${_coreVerbage.Left[1].Upper}${_coreVerbage.Right[-1].Lower}
		} else /if (!${Bool[${_coreVerbage}]}) {
			/call writeINI relayTells Common FALSE
		}
		OUT Relay Tells ${sep} ${If[${Bool[${relayTells}]},${cgood}${relayTells}\ax,${odisabled}]}
		
	| reset all core INIs
	} else /if (${Bool[${_coreType.Equal[reset]}]}) {
		/call core_reset


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

	| set echo channel
	} else /if (${Bool[${_coreType.Equal[echo]}]}) {
		/declare _num int local ${_coreVerbage}
		/if (${Range.Between[1,9:${Int[${_num}]}]}) {
			/call writeINI echoChannel Common ${_num} 0 0
		} 
		OUT /core echo [${cinfo}${echoChannel}\ax] set Echo Channel ${cinfo}1 - 9\ax


	| exp max level
	} else /if (${Bool[${_coreType.Equal[expmaintain]}]}) {
		/declare _num int local ${_coreVerbage}
		/if (${Range.Between[1,99:${Int[${_num}]}]}) {
			/call writeINI MaintExpLvl Common ${_num} 
			/call echos number "Maintain $ of EXP in level" ${_num} 0 0
		} else {
			OUT /core expmaintain [${cinfo}${MaintExpLvl}\ax] EXP % range ${cinfo}1 - 99\ax
		}	

	| exp maintain level
	} else /if (${Bool[${_coreType.Equal[explevel]}]}) {
		/declare _num int local ${_coreVerbage}
		/if (${Range.Between[1,105:${Int[${_num}]}]}) {
			/call writeINI MaxLevel Common ${_num} 
			/call echos number "Max EXP Level" ${_num} 0 0
		} else {
			OUT /core explevel [${cinfo}${MaxLevel}\ax] set Max Level ${cinfo}1 - 105\ax
		}	

	| exp auto adjust
 	} else /if (${Bool[${_coreType.Equal[expadjust]}]}) {
		/call writeINI AutoExpAdjust Common ${If[${AutoExpAdjust},FALSE,TRUE]} 
		/call echos switch "Auto Adjust EXP/AA" AutoExpAdjust 0 0

	| delete old INI entries
 	} else /if (${Bool[${_coreType.Find[fixini]}]}) {
		/call fix_INI_declares

	} else {
		/call echos listtype core core 0 0 
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
	
	/if (${Bool[${onoffType.Equal[${_onoffType}]}]}) {
		/if (${Defined[switch${_onoffType.Left[1].Upper}${_onoffType.Right[-1].Lower}]}) {
			/call writeINI switch${_onoffType.Left[1].Upper}${_onoffType.Right[-1].Lower} Common ${If[${switch${_onoffType.Left[1].Upper}${_onoffType.Right[-1].Lower}},FALSE,TRUE]}  
			/call echos switch "${_onoffType.Left[1].Upper}${_onoffType.Right[-1].Lower}" switch${_onoffType.Left[1].Upper}${_onoffType.Right[-1].Lower} 0 0
		} 
	} else {
		/call echos listtype onoff onoff 0 0
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
	/declare _controls string local |buff|burn|cc|core|cursor|debuff|hc|heal|lc|movement|onoff|override|pc|port|rez|tc|tie|ts|${Me.Class.ShortName}
	
	/for _count 1 to ${_controls.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}/${_controls.Arg[${_count},|].Lower}\ax
		/varset _sep TRUE
	/next _count
	/echo
	/call echos info 0 0 0 0
	OUT ${_listout}
	/echo
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

	| tradeskill commands
	/if (${Bool[${_tsType.Find[movebank]}]}) {
		/declare _slot 				int local
		/declare _bagslot 		int local
		/for _slot 23 to 26
			/if (${InvSlot[${_slot}].Item.Container}) {
				/for _bagslot 1 to ${InvSlot[${_slot}].Item.Container}
					/if (${Me.Inventory[${_slot}].Item[${_bagslot}].StackSize} > 100 && ${Window[BigBankWnd].Open}) {
						OUT  Moving ${sep} ${cinfo}${Me.Inventory[${_slot}].Item[${_bagslot}].Name}\ax ${sep} [${ctar}${FindItemCount[${Me.Inventory[${_slot}].Item[${_bagslot}].Name}]}\ax]
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
							OUT  Moving ${sep} ${cinfo}${Me.Inventory[${_slot}].Item[${_bagslot}].Name}\ax ${sep} [${ctar}${FindItemCount[${Me.Inventory[${_slot}].Item[${_bagslot}].Name}]}\ax]
							| /nomodkey /itemnotify in Pack${Math.Calc[${FindItem[=${Me.Inventory[${_slot}].Item[${_bagslot}].Name}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${Me.Inventory[${_slot}].Item[${_bagslot}].Name}].ItemSlot2}+1]} 
							/click left ${Me.Inventory[${_slot}].Item[${_bagslot}].Name} 

							/nomodkey /shift /notify RealEstateItemsWnd REIW_Move_Closet_Button leftmouseup
							
						}
					/next _bagslot
				}
			/next _slot
		} else {
			OUT Open Realestate Items Window
		}

| /click left ${Me.Inventory[1].Item[23].Name} 
| /nomodkey /itemnotify in Pack${Math.Calc[${FindItem[=${Me.Inventory[23].Item[1].Name}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${Me.Inventory[23].Item[1].Name}].ItemSlot2}+1]} leftmouseup
| /nomodkey /itemnotify in Pack${Math.Calc[${FindItem[=${Me.Inventory[23].Item[1].Name}].ItemSlot}-22]} ${Math.Calc[${FindItem[=${Me.Inventory[23].Item[1].Name}].ItemSlot2}+1]} rightmouseup
| ${Window[RealEstateItemsWnd].Open}
| /nomodkey /itemnotify "${Me.Inventory[23].Name}" rightmouseup
| /nomodkey /shift /notify RealEstateItemsWnd REIW_Move_Closet_Button leftmouseup

	} else {
		/call echos listtype ts ts 0 0 0
	} 
/return

	

|***
 *	DES: core controls
 *	USE: /cc [auto|agro|force|melee|range|nuke|verbose|killcode|reset|whitewash|MA1|MA2|saferadius|zradius] [TEXT]
 *	INI: 
 *  NOTE:
 ***|
#bind setCombatControl /cc
sub Bind_setCombatControl(string _combatType, string _combatVerbage)
	DEBUG ${cbug}Bind_setCombatControl(type:${_combatType}, verbage:${_combatVerbage})\ax

	| force manual combat routines
	/if (${Bool[${_combatType.Equal[force]}]}) {
		/if (${Bool[${_combatVerbage}]}) {
			/if (${Target.ID} && (${Target.ID} != ${Me.ID}) && (${Target.ID} != ${Me.Pet.ID})) {
				| engage Melee combat
				/if (${Bool[${_combatVerbage.Equal[melee]}]}) {
					/varset isValidTarget TRUE
					/varset combat MELEE
					/varset Target2Kill ${Target.ID}
					OUT Combat ${sep} ${cinfo}Melee\ax ${sep} [${ctar}${Spawn[ID ${Target.ID}].DisplayName}\ax]
				| engage Range combat
				} else /if (${Bool[${_combatVerbage.Equal[range]}]}) {
					/if (${Target.Distance} > 31) {
						/varset isValidTarget TRUE
						/varset combat RANGE
						/varset Target2Kill ${Target.ID}	
						OUT Combat ${sep} ${cinfo}Range\ax ${sep} [${ctar}${Spawn[ID ${Target.ID}].DisplayName}\ax]
					} 
				| engage Nuke combat
				} else /if (${Bool[${_combatVerbage.Equal[nuke]}]}) {
					/varset isValidTarget TRUE
					/varset combat NUKE
					/varset Target2Kill ${Target.ID}
					OUT Combat ${sep} ${cinfo}Nuke\ax ${sep} [${ctar}${Spawn[ID ${Target.ID}].DisplayName}\ax]
				}
			}
			 
			| echo options
			} else {
				/declare _tmpVariable string local |melee|range|nuke
				/declare _listout string local
				/declare _sep bool local FALSE
				/declare _count int local 0 

				/for _count 1 to ${_tmpVariable.Count[|]}
					/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_tmpVariable.Arg[${_count},|]}\ax
					/varset _sep TRUE
				/next _count
				OUT /cc force [${_listout}]	
				
			}

	| set Melee combat
	} else /if (${Bool[${_combatType.Equal[melee]}]}) {
		/call writeINI combat Common ${_combatType.Upper}
		OUT Core Auto Combat ${sep} ${cinfo}${combat}\ax

	| set Range combat
	} else /if (${Bool[${_combatType.Equal[range]}]}) {
		/call writeINI combat Common ${_combatType.Upper}
		OUT Core Auto Combat ${sep} ${cinfo}${combat}\ax

	| set Nuke combat
	} else /if (${Bool[${_combatType.Equal[nuke]}]}) {
		/call writeINI combat Common ${_combatType.Upper}
		OUT Core Auto Combat ${sep} ${cinfo}${combat}\ax

	| set Heal combat
	} else /if (${Bool[${_combatType.Equal[heal]}]}) {
		/call writeINI combat Common ${_combatType.Upper}
		OUT Core Auto Combat ${sep} ${cinfo}${combat}\ax

	| set Dual combat
	} else /if (${Bool[${_combatType.Equal[dual]}]}) {
		/call writeINI combat Common ${_combatType.Upper}
		OUT Core Auto Combat ${sep} ${cinfo}${combat}\ax

	| set Pull
	} else /if (${Bool[${_combatType.Equal[pull]}]}) {
		/call writeINI switchPull Common ${If[${switchPull},FALSE,TRUE]}
		OUT Combat ${sep} ${cinfo}Pull\ax ${sep} ${If[${switchPull},${oenabled},${odisabled}]}
		/if (${switchPull}) {
			/if (${useSmartAssist1} || ${useSmartAssist2} || ${useSmartAssistG}) /smartma X
			/if (${switchPull} && ${Bool[${MA1}]}) /call writeINI MA1 Common FALSE
			/if (${switchPull} && ${Bool[${MA2}]}) /call writeINI MA2 Common FALSE
		}

	| agro/agro range ##
	} else /if (${Bool[${_combatType.Equal[agro]}]}) {
		/declare _num int local ${_combatVerbage}
		/if (${Bool[${_combatVerbage}]} && ${Range.Between[1,400:${Int[${_num}]}]}) {
			/call writeINI MobAgro Common ${_num} 
			/call echos number "Combat Agro Range" ${_num} 0 0
		} else /if (${Bool[${_combatVerbage}]} && !${Range.Between[1,400:${Int[${_num}]}]})  {
			OUT /cc agro [${cinfo}${MobAgro}\ax] allowable range is ${cinfo}1 - 400\ax
		} else /if (!${Bool[${_combatVerbage}]}) {
			/call writeINI switchAgro Common ${If[${switchAgro},FALSE,TRUE]}
			OUT Combat ${sep} ${cinfo}Agro\ax ${sep} ${If[${switchAgro},${oenabled},${odisabled}]}
		}
		
		/call check_AA_agro		
		
			

	| set out of group Main assist 1 or 2
	} else /if (${Bool[${_combatType.Equal[ma1]}]}) {
		/if (${Bool[${_combatVerbage}]}) {
			/call writeINI MA1 Common ${_combatVerbage.Left[1].Upper}${_combatVerbage.Right[-1].Lower}
		} else /if (!${Bool[${_combatVerbage}]})  {
			/call writeINI MA1 Common FALSE
		} 				
		OUT MA1 ${sep} ${If[${Bool[${MA1}]},${cinfo}${MA1}\ax,${odisabled}]}
	} else /if (${Bool[${_combatType.Equal[ma2]}]}) {		
		/if (${Bool[${_combatVerbage}]}) {
			/call writeINI MA2 Common ${_combatVerbage.Left[1].Upper}${_combatVerbage.Right[-1].Lower}
		} else /if (!${Bool[${_combatVerbage}]})  {
			/call writeINI MA2 Common FALSE
		} 				
		OUT MA2 ${sep} ${If[${Bool[${MA2}]},${cinfo}${MA2}\ax,${odisabled}]}	

	| MA1/2 engage at %
	} else /if (${Bool[${_combatType.Equal[engage]}]}) {
		/declare _num int local ${_combatVerbage}
		/if (${Range.Between[1,100:${Int[${_num}]}]}) {
			/call writeINI engageAt Common ${_num}  
			/call echos number "MA Engage at" ${If[${_num},${_num},${engageAt}]} 0 0
		} else {
			OUT MA1/2 Engage [${cinfo}${engageAt}\ax] % of mobs health to engage ${cinfo}1 - 100\ax
		}

	| Smart Assist engage at %
		/declare _num int local ${_combatVerbage}
		/if (${Range.Between[1,100:${Int[${_num}]}]}) {
			/call writeINI smartengageAt Common ${_num}  
			/call echos number "Smart Assist Engage at" ${If[${_num},${_num},${smartengageAt}]} 0 0
		} else {
			OUT Smart Engage [${cinfo}${smartengageAt}\ax] % of mobs health to engage ${cinfo}1 - 100\ax
		}

	| set melee combat distance to target
	} else /if (${Bool[${_combatType.Equal[combatdistance]}]}) {
		/declare _num int local ${_combatVerbage}
		/if (${Range.Between[1,50:${Int[${_num}]}]}) {
			/call writeINI CombatDistance Movement ${_num}
			/call echos number "Melee Combat Distance" ${_num} 0 0
		} else {
			OUT Melee Combat Distance [${cinfo}${CombatDistance}\ax] minimum range to melee mob ${cinfo}1 - 50\ax
		}

	| set fake safe radius
	} else /if (${Bool[${_combatType.Equal[saferadius]}]}) {
		/declare _num int local ${_combatVerbage}
		/if (${Range.Between[1,99:${Int[${_num}]}]}) {
			/call writeINI combatRadius Common ${_num}
			/call echos number "Fake Safe Combat Radius" ${_num} 0 0
		} else {
			OUT /cc saferadius [${cinfo}${combatRadius}\ax] fake safe combat radius ${cinfo}1 - 50\ax
		}
	
	| set Z radius
	} else /if (${Bool[${_combatType.Equal[zradius]}]}) {
		/declare _num int local ${_combatVerbage}
		/if (${Range.Between[1,200:${Int[${_num}]}]}) {
			/call writeINI zradius Common ${_num} 
			/call echos number "Set ZRadius" ${_num} 0 0
		} else {
			OUT /cc zradius [${cinfo}${zradius}\ax] of mobs Z Radius range ${cinfo}1 - 200\ax
		}	
	} else {
		/call echos listtype cc cc 0 TEXT|## 0
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
		OUT /port [${_listout}] [${cinfo}location\ax]
		/return
	}
	
  | echo locations if class correct
	/if (${Bool[${_class}]} && !${Bool[${_location}]}) {
		/for _country 1 to ${_continent.Count[|]}
			/for _state 1 to ${_port${_class}${_continent.Arg[${_country},|].Left[1].Upper}${_continent.Arg[${_country},|].Right[-1].Lower}.Count[|]}
				/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_port${_class}${_continent.Arg[${_country},|].Left[1].Upper}${_continent.Arg[${_country},|].Right[-1].Lower}.Arg[${_state},|]}\ax
				/varset _sep TRUE
			/next _state
			OUT ${_continent.Arg[${_country},|].Left[1].Upper}${_continent.Arg[${_country},|].Right[-1].Lower} [${_listout}] 
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
			:nextcount
		/next _count
	}
	
	| send 'em on their way with a valid location request
	/if (${Defined[${_location}]}) {
		/t ${_sender} Please enjoy your flight. ${${_location}} on its way.
		/call scribe "${${_location}}" ${buffGem} TRUE
		/call core_cast "${${_location}}" 0 ${Spawn[pc ${_sender}].ID}
	} 
/return



|***
 *	DES: movement controls
 *	USE: /movement [] [##]
 *	INI: 
 *  NOTE:
 ***|
#bind setMovementControls /movement
sub Bind_setMovementControls(string _movementType, string _movementVerbage)
	DEBUG ${cbug}Bind_setMovementControls(type:${_mcType}, verbage:${_mcVerbage})\ax

	| set home on or off
	/if (${Bool[${_movementType.Equal[home]}]}) {
		/call writeINI HomeSet Movement ${If[${HomeSet},FALSE,TRUE]}  
		/call set_home

	| set stick behind
	} else /if (${Bool[${_movementType.Equal[behind]}]}) {
		/call writeINI SetStickBehind Movement ${If[${SetStickBehind},FALSE,TRUE]}  
		/call echos switch "Stick Behind Target"  SetStickBehind 0 0

		| disable mezzing with stickbehind enabled.
		/if (${switchdeBuffMezz} && ${SetStickBehind}) {
			/if (${Defined[switchdeBuffMezz]}) /call writeINI switchdeBuffMezz Buffs FALSE  
			/call echos switch "deBuff Mezz" switchdeBuffMezz 0 0
		}

	| set face fast
	} else /if (${Bool[${_movementType.Equal[fast]}]}) {
		/call writeINI SetFaceFast Movement ${If[${SetFaceFast},FALSE,TRUE]}  
		/call echos switch "Face Fast" SetFaceFast 0 0
		/if (${SetFaceFast}) OUT ${owarning} ${sep} This is a MQ2 Dead Giveaway ${sep} ${owarning}

	| set home radius
	} else /if (${Bool[${_movementType.Equal[radius]}]}) {
		/declare _num int local ${_movementVerbage}
		/if (${Range.Between[1,200:${Int[${_num}]}]}) {
			/call writeINI HomeRadius Movement ${_num} 
			/call echos number "Home Radius" ${_num} 0 0
		} else {
			OUT /movement radius [${cinfo}${HomeRadius}\ax] Home Radius range ${cinfo}1 - 200\ax
		}

	| set leash range
	} else /if (${Bool[${_movementType.Equal[leash]}]}) {
		/declare _num int local ${_movementVerbage}
		/if (${Range.Between[1,200:${Int[${_num}]}]}) {
			/call writeINI HomeLeash Movement ${_num} 
			/call echos number "Leash to Home" ${_num} 0 0
		} else {
			OUT /movement leash [${cinfo}${HomeLeash}\ax] Home Leash range ${cinfo}1 - 200\ax
		}	

	| set facing arc
	} else /if (${Bool[${_movementType.Equal[arc]}]}) {
		/declare _num int local ${_movementVerbage}
		/if (${Range.Between[1,45:${Int[${_num}]}]}) {
			/call writeINI SetFaceArc Movement ${_num} 
			/call echos number "Facing Arc Buffer" ${_num} 0 0
		} else {
			OUT /movement arc [${cinfo}${SetFaceArc}\ax] Facing Movement buffer range ${cinfo}1 - 45\ax
		}	

	} else {
		/call echos listtype movement movement 0 0 0
	} 


/return

