|***
 *	declare.inc
 *	Core Declares common and class specific.
 *	All manual forced events.
 *	All created Alias' and /bc commands
 *	All INI edits
 *
 ***|


|***
 *	DES: Version Control
 *	USE: 
 *	INI: 
 *  NOTE: work in progress
 ***|  
sub check_versions()
	DEBUG ${cbug}set_versions()\ax

	/declare versionCore				string	outer 3.3.0
	
	/declare versionCLR 				string 	outer 2.1.1
	/declare versionDRU 				string 	outer 2.0.1 
	/declare versionSHM 				string 	outer 1.5.1
	/declare versionWAR 				string 	outer 1.5.1 
	/declare versionPAL 				string 	outer 1.0.0 
	/declare versionSHD 				string 	outer 1.9.1 
	/declare versionWIZ 				string 	outer 1.7.1 
	/declare versionENC 				string 	outer 1.4.1
	/declare versionMAG					string 	outer 2.0.1
	/declare versionNEC 				string 	outer 0.5.1 
	/declare versionRNG 				string 	outer 2.2.1
	/declare versionBST 				string 	outer 1.0.1 
	/declare versionBRD 				string 	outer 1.5.1 
	/declare versionBER 				string 	outer 1.6.1
	/declare versionMNK 				string 	outer 0.5.0 
	/declare versionROG 				string 	outer 1.5.1 

	:exit
/return




|***
 *	DES: Used to remove stale entries from the toons INI. 'cause I gotta keep changing shits.
 *	USE: Auto
 *	INI: 
 *	NOTE: may look a little strange, but I leave the old variables here for history. Just in case.
 ***|
sub fix_INI_declares()
/return



|***
 *	DES: adjust the related ini
 *	USE: /call Event_SetVar NA [variable] [INISection] [value]
 *	INI: 
 *  NOTE: 
 ***|
#event SetVar 			"[MQ2] - Core: #1# -> [#2#] -> #3#"
sub Event_SetVar(string line, string _varName, string _section, string _value)
	/if (${Select[${_value},TRUE,ON,1]}) /varset _value TRUE
	/if (${Select[${_value},FALSE,OFF,0]}) /varset _value FALSE
	/varset ${_varName} ${_value}
	/ini "${INIFile}" ${_section} "${_varName.Left[1].Upper}${_varName.Right[-1]}" "${_value}"
	DEBUG changing INI -> [${_section}] -> ${_varName.Left[1].Upper}${_varName.Right[-1]} -> ${_value}
/return



|***
 *	DES: a /declare wrapper
 *	USE: /declare OPsMomisFat [int|bool|string] [outer|local] [3.14]
 *	INI: 
 *  NOTE: 
 ***|
sub DeclareVar(string _name, string _type, string _scope, string _value)
	/if (!${Defined[${_name}]}) {
		/declare ${_name} ${_type} ${_scope}
		/if (${_value.NotEqual[NULL]}) /varset ${_name} ${_value}
		DEBUG Setting ${_name} ${_type} ${_scope} "${_value}" 
	}
/return



|***** Start base INI controls *****|
|***
 *	DES: creates entries for the toon INI
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub DeclareIniVar(string name, string varType, string section, string value, string alias)
	DEBUG Setting ${name} ${Ini[${INIFile},${section},${name.Left[1].Upper}${name.Right[-1]},"${value}"]}
	/if (!${Bool[${name}]} || !${Bool[${varType}]} || !${Bool[${section}]}) {
		/echo ${cbad}-- INVALID INI SETTING/STRING [${name}/${varType}/${section}] --\ax
		/goto :exit
	}

	/if (!${Defined[${name}]}) /declare ${name} ${varType} outer
	/varset ${name} ${Ini[${INIFile},${section},${name.Left[1].Upper}${name.Right[-1]},"${value}"]}
	/if (!${Bool[${Ini[${INIFile},${section}].Find[${name}]}]}) {
		DEBUG Creating: INI -> [${section}] -> ${name} ${If[${Bool[${alias}]},[alias:\aw${alias}\ax],]}
		/ini "${INIFile}" "${section}" "${name.Left[1].Upper}${name.Right[-1]}" "${${name}}" "${${value}}"
	}

	/if (${Bool[${alias}]} && !${Bool[${Alias[${alias}]}]}) {
		DEBUG Creating: Alias \aw${alias}\ax 
		/squelch /alias ${alias} NOUT Core: \aw${name}\ax -> [${section}] -> 
	} else /if (${Bool[${alias}]} && ${Bool[${Alias[${alias}]}]}) {
		DEBUG Alias ${cinfo}${alias}\ax already exists
	}

	/if (${Me.Book[${Me.Book[${${name}} Rk. III]}].ID}) {
		DEBUG Setting ${name}: ${${name}} ${debugalias}
		/varset ${name} ${Me.Book[${Me.Book[${${name}} Rk. III]}].Name} 
	} else /if (${Me.Book[${Me.Book[${${name}} Rk. II]}].ID}) {
		DEBUG Setting ${name}: ${${name}} ${debugalias}
		/varset ${name} ${Me.Book[${Me.Book[${${name}} Rk. II]}].Name}
	} else /if (${Me.CombatAbility[${${name}} Rk. III]}) {
		DEBUG Setting ${name}: ${${name}} ${debugalias}
		/varset ${name} ${${name}} Rk. III
	} else /if (${Me.CombatAbility[${${name}} Rk. II]}) {
		DEBUG Setting ${name}: ${${name}} ${debugalias}
		/varset ${name} ${${name}} Rk. II
	}
	:exit
/return



|***
 *	DES: creates INI entries for core.ini
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub DeclareCoreIniVar(string name, string varType, string section, string value, string alias)
	/if (!${Bool[${name}]} || !${Bool[${varType}]} || !${Bool[${section}]}) {
		/echo ${cbad}-- INVALID INI SETTING/STRING [${name}/${varType}/${section}] --\ax
		/goto :exit
	}

	/if (!${Defined[${name}]}) /declare ${name} ${varType} outer
	/varset ${name} ${Ini[${INICore},${section},${name.Left[1].Upper}${name.Right[-1]},"${value}"]}
	/if (!${Bool[${Ini[${INICore},${section}].Find[${name}]}]}) {
		/ini "${INICore}" "${section}" "${name.Left[1].Upper}${name.Right[-1]}" "${${name}}" "${${value}}"
		DEBUG Creating: INI -> [${section}] -> ${name} ${If[${Bool[${alias}]},[alias:${cinfo}${alias}\ax],]}
	}
	DEBUG Setting ${name} ${Ini[${INICore},${section},${name.Left[1].Upper}${name.Right[-1]},"${value}"]}

	/if (${Bool[${alias}]} && !${Bool[${Alias[${alias}]}]}) {
		/squelch /alias ${alias} NOUT Core: ${cinfo}${name}\ax -> [${section}] -> 
		DEBUG Creating: Alias ${cinfo}${alias}\ax 
	} else /if (${Bool[${alias}]} && ${Bool[${Alias[${alias}]}]}) {
		DEBUG Alias ${cinfo}${alias}\ax already exists
	}
	:exit
/return
 


|***
 *	DES: Updates the list controls names
 *	USE: 
 *	INI: core.zone.ini
 *  NOTE: 
 ***|
sub update_zone_list(string _list)
	DEBUG  update_zone_list(${_list})
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
	/if (${Bool[${_sender}]} && !${Spawn[pc ${_sender}].ID}) /goto :exit

	/declare _count 				int local	
	/declare _listtype 			string local	
	/declare _list 					string local ${ZoneFileList}
	
	/for _count 1 to ${_list.Count[|]}
		/varset _listtype ${_list.Arg[${_count},|]}
		/if (!${Bool[${Ini[${INIZone},"${Zone.ShortName}","${_listtype}"]}]}) /ini "${INIZone}" "${Zone.ShortName}" "${_listtype}" ""
		/varset ${_listtype} ${Ini[${INIZone},"${Zone.ShortName}","${_listtype}"]}
	/next _count	
	
	| set safezone 
	/if (!${Bool[${Ini[${INIZone},"${Zone.ShortName}","SafeZone"]}]}) /ini "${INIZone}" "${Zone.ShortName}" "SafeZone" "FALSE"
	/varset SafeZone ${Ini[${INIZone},"${Zone.ShortName}","SafeZone"]}

 	:exit
/return 



#event setTargetList "[MQ2] - Set Target #1#!"
sub Event_setTargetList(string line, string _list)
	DEBUG  Event_setTargetList(${_list}List)
	/if (!${Target.ID}) {
		/call echos needtarget
		/goto :exit
	} 
	/if (${SafeZone}) {
		VOUT ${onotice} ${sep} Control Lists are not used in Safe Zones.
		/goto :exit
	}
		
	
	| validate the variable command
	/if (${Select[${_list},Exclude,NoMezz,Name,Range,NoCast,NoMelee,HuntMob,NoFire,NoPoison,NoMagic,NoCorruption,NoCold,NoDisease,CharmMob]}) {
		/goto :listadd
	} else /if (${Select[${_list},Remove]}) {
		/goto :listremove
	} else {
		/goto :exit
	}
		
	| add mobs
	:listadd
	/if (${Target.ID} && ${${_list}List.Find[${Target.DisplayName}]}) {
		/call echos listexists ${_list}List
	} else /if (${Target.ID} && !${Bool[${Spawn[pc ID ${Target.ID}]}]}) {
		/if (${${_list}List.Find[${Target.DisplayName}]}) {
			/call echos listexists ${_list}List
			/goto :exit
		} else /if (!${${_list}List.Find[${Target.DisplayName}]}) {
			/if (${Bool[${${_list}List}]}) /ini "${INIZone}" "${Zone.ShortName}" "${_list}List" "${${_list}List}|${Target.DisplayName}"
			/if (!${Bool[${${_list}List}]}) /ini "${INIZone}" "${Zone.ShortName}" "${_list}List" "|${Target.DisplayName}"
		}
		/if (${Bool[${Plugin[MQ2EQBC]}]} && ${EQBC.Connected}) /bc zonereset
	}
	/goto :exit
		
	| remove mobs
	:listremove
	/if (${Target.ID}) {
		/declare _mobName string local |${Target.DisplayName}
		VOUT Removing ${ctar}${Target.DisplayName}\ax ${sep} ${cinfo}${Zone.ShortName}\ax [\a-y${Zone.ID}\ax]
	}

	/declare _count 				int local	
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
		/if (!${${_listName}.Find[${_mobName}]}) /goto :nextcount
		/varcalc _a ${${_listName}.Find[${_mobName}]}-1
		/varcalc _b ${${_listName}.Length}-${_mobName.Length}-${_a}
		/if (${_a} > 0) /varset _strLeft ${${_listName}.Left[${_a}]}
		/if (${_a} >= 0) /varset _strRight ${If[${Select[${_b},NULL,FALSE,""]},,${${_listName}.Right[${_b}]}]} 
		/ini "${INIZone}" "${Zone.ShortName}" "${_listName}" "${If[${Bool[${_strLeft}]},${_strLeft},]}${If[${Bool[${_strRight}]},${_strRight},]}"

		:nextcount
	/next _count	
	/if (${Bool[${Plugin[MQ2EQBC]}]} && ${EQBC.Connected}) /bc zonereset	
	
	:exit
/return


#event HuntMobAgro			"[MQ2] HuntMobAgro: #1#"
sub Event_HuntMobAgro(string line, int _huntagro)
	/if (${_huntagro} <= 0 || !${_huntagro}) {
		/call Event_SetVar NA setHunterAgro Common 0
	} else /if (${_huntagro}) {
		/call Event_SetVar NA setHunterAgro Common ${_huntagro} 
	}
	/call echos hunter
/return

|***** End base INI controls *****|



|***
 *	DES: Base variable inits.
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_base()
	DEBUG ${cbug}base.Environment()\ax
	/declare core															bool	outer TRUE
	/declare _count														int		local
		
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
	/call DeclareCoreIniVar ctar								string	Output \a-w
	/call DeclareCoreIniVar cpc									string	Output \a-w
	/call DeclareCoreIniVar csp									string	Output \a-r
	/call DeclareCoreIniVar cbad								string	Output \ar
	/call DeclareCoreIniVar cgood								string	Output \ag
	/call DeclareCoreIniVar c-									string	Output \at
	/call DeclareCoreIniVar cbug								string	Output \at
	/call DeclareCoreIniVar cinfo								string	Output \aw
	/call DeclareCoreIniVar citem								string	Output \au
	/call DeclareCoreIniVar sep									string	Output \at..\ax
	/call DeclareCoreIniVar sepO								string	Output \at[\ax
	/call DeclareCoreIniVar sepC								string	Output \at]\ax
	/call DeclareCoreIniVar oenabled						string	Output "${cgood}Enabled\ax"
	/call DeclareCoreIniVar odisabled						string	Output "${cbad}DISABLED\ax"
	/call DeclareCoreIniVar owarning						string	Output "${cbad}WARNING\ax"
	/call DeclareCoreIniVar ofatal							string	Output "${cbad}FATAL ERROR\ax ${sep} ${cbad}ENDING MACRO\ax"
	/call DeclareCoreIniVar ocaution						string	Output "${cbad}CAUTION\ax"
	/call DeclareCoreIniVar otrue								string	Output "${cgood}TRUE\ax"
	/call DeclareCoreIniVar ofalse							string	Output "${cbad}FALSE\ax"
	/call DeclareCoreIniVar odestroy						string	Output "${cbad}DESTROY\ax"
	/call DeclareCoreIniVar oleave							string	Output "${cinfo}Leave\ax"
	/call DeclareCoreIniVar oannounce						string	Output "\amAnnounce\ax"
	/call DeclareCoreIniVar oadd								string	Output "${cgood}Add\ax"
	/call DeclareCoreIniVar okeep								string	Output "${cgood}Keep\ax"
	/call DeclareCoreIniVar onotice							string	Output "\ayNotice\ax"
	/call DeclareCoreIniVar prelink							string	Output "3Rorcex^11000004^0^1^1362119591^10000^'"
 	/call DeclareCoreIniVar postlink						string	Output ""	
		
	
	| SET THIS TO TRUE TO ENABLE DEBUGING. ALTERNATIVE USE /varset debug TRUE
	| on a funny note: prepare your ass for spam
	/declare debug															bool		outer FALSE

	/call DeclareIniVar verbose									bool		Common TRUE
	/call DeclareCoreIniVar EQBCServer					string	General 127.0.0.1
	/call DeclareCoreIniVar EQBCPort						string	General 2112
	/call DeclareCoreIniVar mqlog								string	General FALSE
	/call DeclareCoreIniVar HudName							string	General Core
	/call DeclareCoreIniVar HudType							string	General normal
	/call DeclareCoreIniVar minMobPullLvl				int			General 1
	/call DeclareCoreIniVar maxMobPullLvl				int			General 150
	/call DeclareCoreIniVar combatRadius				string	General 30 /combatradius
	/call DeclareCoreIniVar plist								string	Plugin |MQ2Cast|MQ2EQBC|MQ2Exchange|MQ2MoveUtils|MQ2Netbots|MQ2Debuffs
		


	/call DeclareCoreIniVar ${Me.DisplayName}		int			DeathCount 0
	/call DeclareCoreIniVar invitelist					string	No FALSE
	/call DeclareCoreIniVar safelist						string	General |FALSE|FALSE
	/call DeclareCoreIniVar forcesafelist				bool		General TRUE

	| OCD Shit - 
	/call DeclareCoreIniVar setMQFontSize				int			General 2
	/call DeclareCoreIniVar setMQCaptions				string	General off

	/call DeclareCoreIniVar setTGB							string	General on
	/call DeclareCoreIniVar setAutoAssist				string	General off
	/call DeclareCoreIniVar remoteKillKey				string	General "CHANGE ME" /killcode
	/call DeclareCoreIniVar RezTokenList				string 	General |WAR|SHD|PAL|BER|ROG|MNK|RNG|BST|BRD|NEC|ENC|WIZ|MAG|CLR|SHM|DRU
	
	| buff queue variables
	/call DeclareCoreIniVar queueCountMax				int			General 10
	/declare queueBuffID[8] 										int  		outer
	/declare queueList[${queueCountMax},2] 		 	int  		outer
	/declare queueCount             					  int  		outer 0
	| /declare DoBuffLowest         	   					int   outer 100
	
	
	/declare status_check_string								string	outer NULL
	
	/call DeclareCoreIniVar main								string	Group |FALSE|FALSE
	/call DeclareCoreIniVar GroupLeader					string	Group FALSE
	/call DeclareCoreIniVar GroupTank						string	Group FALSE
	/call DeclareCoreIniVar GroupAssist					string	Group FALSE
	/call DeclareCoreIniVar GroupPuller					string	Group FALSE
	/call DeclareCoreIniVar GroupLooter					string	Group FALSE
	/call DeclareCoreIniVar GroupMark						string	Group FALSE

	/call DeclareCoreIniVar combatWAR						string	Here MELEE
	/call DeclareCoreIniVar combatPAL						string	Here MELEE
	/call DeclareCoreIniVar combatSHD						string	Here MELEE
	/call DeclareCoreIniVar combatCLR						string	Here HEAL
	/call DeclareCoreIniVar combatSHM						string	Here HEAL
	/call DeclareCoreIniVar combatDRU						string	Here DUAL
	/call DeclareCoreIniVar combatMAG						string	Here NUKE
	/call DeclareCoreIniVar combatENC						string	Here DUAL
	/call DeclareCoreIniVar combatWIZ						string	Here NUKE
	/call DeclareCoreIniVar combatNEC						string	Here NUKE
	/call DeclareCoreIniVar combatMNK						string	Here MELEE
	/call DeclareCoreIniVar combatROG						string	Here MELEE
	/call DeclareCoreIniVar combatBER						string	Here MELEE
	/call DeclareCoreIniVar combatBST						string	Here MELEE
	/call DeclareCoreIniVar combatRNG						string	Here MELEE
	/call DeclareCoreIniVar combatBRD						string	Here Dual

	/call DeclareCoreIniVar makeVisible					bool		Here	TRUE
	/call DeclareCoreIniVar clearTarget					bool		Here	TRUE
	/call DeclareCoreIniVar defaultTimerDelay		string	Here	1m
	/call DeclareCoreIniVar baseAgroRange				int			Here	60
	/call DeclareCoreIniVar emptyHands					bool		Here	TRUE
	
	/call DeclareCoreIniVar count								int			bitch	20	
	/call DeclareCoreIniVar 1										string	bitch	"Seriously... Again..."
	/call DeclareCoreIniVar 2										string	bitch	"Can I get some heals over here?"
	/call DeclareCoreIniVar 3										string	bitch	"Aha, I see the Fuck-Up Fairy has visited us again!"
	/call DeclareCoreIniVar 4										string	bitch	"Will trade a Halfling for a rez, or three Gn0mes."
	/call DeclareCoreIniVar 5										string	bitch	"I'm sorry I bit you... pulled your hair... and punched you in the face..."
	/call DeclareCoreIniVar 6										string	bitch	"WTB HEALS!!"
	/call DeclareCoreIniVar 7										string	bitch	"Sometimes the first step to forgiveness, is realising the other person was born an idiot."
	/call DeclareCoreIniVar 8										string	bitch	"I'm getting Disc lag . . . again."
	/call DeclareCoreIniVar 9										string	bitch	"oh man....i figured out why i was lagging the other day...i look in my case and my kids knocked off my cpu fan"
	/call DeclareCoreIniVar 10									string	bitch	"I'm jealous of all the people that haven't met you!"
	/call DeclareCoreIniVar 11									string	bitch	"You're slower than a herd of turtles stampeding through peanut butter."
	/call DeclareCoreIniVar 12									string	bitch	"Oh good, my dog found a chainsaw."
	/call DeclareCoreIniVar 13									string	bitch	"You are not as bad as people say, you are much, much worse."
	/call DeclareCoreIniVar 14									string	bitch	"Stupidity is not a crime so you are free to go."
	/call DeclareCoreIniVar 15									string	bitch "AFK, playing Pokemon. More productive then this shit."
	/call DeclareCoreIniVar 16									string	bitch	"All I did was move my mouse over it."
	/call DeclareCoreIniVar 17									string	bitch	"You sound reasonable. It must be time to up my medication!"
	/call DeclareCoreIniVar 18									string	bitch	"YOU CAN'T KILL ME!"
	/call DeclareCoreIniVar 19									string	bitch	"Roses are red. Violets are blue. You're an idiot."
	/call DeclareCoreIniVar 20									string	bitch	"Sometimes, when I close my eyes, I... can't see."
	
	
	| hide switches for /hidecorpse command
	/call DeclareCoreIniVar hideNone						bool		hide FALSE
	/call DeclareCoreIniVar hideAll							bool		hide FALSE
	/call DeclareCoreIniVar hideAlways					bool		hide FALSE
	/call DeclareCoreIniVar hideLooted					bool		hide FALSE
	/call DeclareCoreIniVar hideAllButGroup			bool		hide FALSE
	/call DeclareCoreIniVar hideNPC							bool		hide FALSE
	/call DeclareCoreIniVar hideLooted					bool		hide FALSE
	/call DeclareCoreIniVar hideLisdted					bool		hide FALSE
	/call DeclareCoreIniVar hideListedUnlocked	bool		hide FALSE
	
	| Base Environment settings
	/declare noInvis 														bool 		outer TRUE
	/declare noFeigning 												bool 		outer TRUE
	/declare Target2Kill												int			outer 0
	/declare UseRange														bool		outer FALSE
	/declare currentLootCount										int			outer	0
	/declare markerDead													bool		outer FALSE
	
	/declare isValidTarget											bool		outer FALSE
	/declare combatMode													string	outer NULL
	/declare tmpMode														string	outer NULL
	/declare hudWarning													string	outer	NULL
	/declare hudNotice													string	outer	NULL	
	/declare hudTarNotice												string	outer	NULL
	/declare flagCallNeedRez										bool		outer FALSE

	/declare setToastMode												bool 		outer FALSE	
	/declare canCast														bool		outer	TRUE
	/declare ignoreEngageAt											bool		outer FALSE
	/declare tmpHealOverride										float 	outer 99.99	
	
	/declare 	pre 															string 	outer ${Ini[${INICore},fakelink,pre]}
	/declare 	post 															string 	outer ${Ini[${INICore},fakelink,post]}
	
	/declare hudBroadcast												string	outer
	
	/call DeclareVar INIBlurb										string	outer tc\core.blurb.ini
	/call DeclareVar INILoot										string  outer	tc\core.loot.ini
	/call DeclareVar INIZone		     						string 	outer tc\core.zone.ini
	/call DeclareVar INIAugDump     						string 	outer tc\core.augmentdump.ini
	
	| core.zone.ini lists
	/call DeclareVar RangeList		   						string 	outer NULL
	/call DeclareVar ExcludeList    						string 	outer NULL
	/call DeclareVar NameList       						string 	outer NULL
	/call DeclareVar NoMeleeList       					string 	outer NULL
	/call DeclareVar NoCastList       					string 	outer NULL
	/call DeclareVar NoMezzList       					string 	outer NULL
	/call DeclareVar NoDiseaseList       				string 	outer NULL
	/call DeclareVar NoMagicList       					string 	outer NULL
	/call DeclareVar NoPoisonList       				string 	outer NULL
	/call DeclareVar NoFireList       					string 	outer NULL
	/call DeclareVar NoCorruptionList       		string 	outer NULL
	/call DeclareVar NoColdList       					string 	outer NULL
	/call DeclareVar CharmMobList      					string 	outer NULL
	/call DeclareVar SafeZone		       					string 	outer NULL
	/call DeclareVar HuntMobList       					string 	outer NULL
	
	
	
	| various lists for .. you know..lists and stuff.
	/call DeclareVar MeleeList      						string 	outer |WAR|ROG|BST|MNK|SHD|PAL|BER|BRD|RNG
	/call DeclareVar CastList										string 	outer |BST|SHD|PAL|RNG|CLR|DRU|SHM|ENC|WIZ|NEC|MAG
	/call DeclareVar PetList										string 	outer |BST|SHD|DRU|SHM|ENC|NEC|MAG
	/call DeclareVar HealerList									string 	outer |CLR|DRU|SHM
	/call DeclareVar RezerList									string 	outer |CLR|DRU|SHM|PAL
	/call DeclareVar AllClassList								string 	outer |WAR|SHD|PAL|BER|ROG|MNK|RNG|BST|BRD|NEC|ENC|WIZ|MAG|CLR|SHM|DRU
	/call DeclareVar AASnareList								string 	outer |RNG|DRU|SHD|NEC|ROG|WAR|WIZ|MNK
	/call DeclareVar RezTokenList								string 	outer |WAR|SHD|PAL|BER|ROG|MNK|RNG|BST|BRD|NEC|ENC|WIZ|MAG|CLR|SHM|DRU
	/call DeclareVar ZoneFileList								string 	outer |ExcludeList|NoMezzList|NameList|RangeList|NoCastList|NoMeleeList|HuntMobList|NoFireList|NoPoisonList|NoMagicList|NoCorruptionList|NoColdList|NoDiseaseList|CharmMobList


	
	| Timers for toons
	/call DeclareIniVar tSelf_Buff							string	Timer 3m
	/call DeclareIniVar tPet_Buff								string	Timer 1m
	/call DeclareIniVar tItem_Buff							string	Timer 1m 
	/call DeclareIniVar tBC_Buff								string	Timer 20s   
	/call DeclareIniVar tRaid_Buff							string	Timer 20s   
	/call DeclareIniVar tClear_Cursor						string	Timer 5s   
	/call DeclareIniVar tCheck_Aura							string	Timer 3m   
	/call DeclareIniVar tRez_Retry							string	Timer 3m   
	/call DeclareIniVar	tBuff_Kill							string	Timer	1m	
	/call DeclareIniVar	tTell_Buff							string	Timer	10s	
	/call DeclareIniVar	tCheck_EQBC							string	Timer	10m	
	/call DeclareIniVar	tCheck_EXP							string	Timer	5m	
	/call DeclareIniVar	tCheck_Tie							string	Timer	5 /tietime
	/call DeclareIniVar	tCheck_Familiar					string	Timer	30s	
	/call DeclareIniVar	tCheck_Ammo							string	Timer	5s	
	/call DeclareIniVar	tCheck_snap_retry				string	Timer	15s	
	/call DeclareIniVar	tOdds_and_Ends					string	Timer	30m
	/call DeclareIniVar	tAuto_Control_Toon			string	Timer	10s
	/call DeclareIniVar	tAA_picker							string	Timer	5m
	/call DeclareIniVar	tWatch_Spawn						string	Timer	1s
	/call DeclareIniVar	tCast_Cure							string	Timer	30s
	/call DeclareIniVar	tZone_Pause							string	Timer	5s

	| mod rod recovered addicts
	/if (!${Select[${Me.Class.ShortName},MAG,WAR,ROG,BRD,MNK,BER]}) {
		/call DeclareIniVar	tAsk_for_rod						string	Timer	1m
	}
	/call DeclareCoreIniVar	tCheck_status				string	Timer	2s	

/return



|***
 *	DES: class INI files for each toon
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_common()
	DEBUG ${cbug}declare_common()\ax
	/declare _count 														int 		local

	/call DeclareIniVar AutoBot				 					bool	 	Common FALSE
	/call DeclareIniVar toonControl			 				string	Common FALSE /control 
	/call DeclareIniVar autoControl			 				bool		Common FALSE 
	/call DeclareIniVar combat					 				string	Common melee /combat 
	/call DeclareIniVar whentoToast							int 		Common 80 /toastengageat
	/call DeclareIniVar AutoToast								bool 		Common FALSE 
	/call DeclareIniVar MA1			 								string 	Common FALSE /ma1
	/call DeclareIniVar MA2			 								string 	Common FALSE /ma2
	
	/call DeclareIniVar MobAgro									int 		Common 10 /agro
	/call DeclareIniVar engageAt 								int 		Common 98 /engageat 
	/call DeclareIniVar smartengageAt 					int 		Common 98 /smartengageat 
	/call DeclareIniVar autoRest 								int 		Common 90 /autorest

	/call DeclareIniVar noCastWith							string	Common "|Quick Defense|Protection of Bayle|Interrogation Intimidation|Energy Charge|Cloud of Terror|Faith"
	| /call DeclareIniVar doDebuff			 					bool		Common TRUE
	/call DeclareIniVar HealOverride						int			Common 0	/healoverride
	/call DeclareIniVar echoChannel							string	Common FALSE /echochannel
	
	
	/call DeclareIniVar shrinkItem 							string 	Common "Ring of the Ancients"
	/call DeclareIniVar setAutoShrink						bool		Common FALSE
	/call DeclareIniVar	HornofUnity							bool		Common FALSE
	/call DeclareIniVar	useSmartAssist1					bool		Common FALSE 
	/call DeclareIniVar	useSmartAssist2					bool		Common FALSE
	/call DeclareIniVar	useSmartAssistG					bool		Common FALSE
	/call DeclareIniVar setRaidInvite						bool		Common FALSE
	/call DeclareIniVar setAgroMode							bool		Common FALSE 
	/call DeclareIniVar setPullMode							bool		Common FALSE
	/call DeclareIniVar setDragMode							bool		Common FALSE 
	/call DeclareIniVar setForageMode						bool		Common FALSE
	/call DeclareIniVar setPetFarmMode					bool		Common FALSE
	/call DeclareIniVar setHunterMode						bool		Common FALSE
	/call DeclareIniVar setHunterAgro						int			Common FALSE 
	/call DeclareIniVar setFishMode							bool		Common FALSE
	/call DeclareIniVar setSkillsMode						bool		Common FALSE
	/call DeclareIniVar IllusionItem						string	Common FALSE
	/call DeclareIniVar IllusionBuff						string	Common FALSE
	/call DeclareIniVar BuffItem	 							bool		Common TRUE 
	/call DeclareIniVar switchHealSelf					bool 		Heal FALSE
	/call DeclareIniVar switchInvites						bool 		Common TRUE
	/call DeclareIniVar HUDTime									bool 		Common TRUE
	/call DeclareIniVar GTFO										string	Common FALSE
	/call DeclareIniVar switchAApicker					bool 		Common FALSE
	/call DeclareIniVar mount										string	Common FALSE
	/call DeclareIniVar rezAuto 								bool 		Heal TRUE 
	/call DeclareIniVar rezEmergency						bool		Heal FALSE
	/call DeclareIniVar rezEveryone 						bool 		Heal FALSE 
	/call DeclareIniVar rezCombat 							bool 		Heal FALSE 
	/call DeclareIniVar rezEnable 							bool 		Heal FALSE 
	
	
	| /call DeclareIniVar BalanceHP     					bool   	Common FALSE /balance
	/call DeclareIniVar AutoExpAdjust 					bool 		Common TRUE /autoadjustexp
	/call DeclareIniVar MaintExpLvl   					int 		Common 99 /maintexplvl
	/call DeclareIniVar MaxLevel      					int 		Common 105 /maxlevel
	/call DeclareIniVar switchBanestrike				bool		Common FALSE		
	/call DeclareIniVar RangeItem								string	Common FALSE		
	/call DeclareIniVar RangeItemSummon					string	Common FALSE
	/call DeclareIniVar autoInventoryItem				string	Common 
	| /call DeclareIniVar autoDeleteItem					string	Common |FALSE|FALSE
	/call DeclareIniVar stoponDS								bool		Common TRUE		
	/call DeclareIniVar useGlyph								string	Common "Glyph of the Cataclysm"	/glyph
	/call DeclareIniVar toastMobCount						int			Common 10 /toastmobcount
	/call DeclareIniVar ResidentMage						string	Common |FALSE|FALSE
	/call DeclareIniVar ForageFishInvis					string	Common FALSE	
	/call DeclareIniVar ClassChannel						int			Common FALSE	
	/call DeclareIniVar setBroadcast						string	Common TRUE	
	/call DeclareIniVar relayTells							string	Common FALSE /relay	
	
	| watch spawn stuff
	/declare hudwatchspawn											string	outer FALSE
	/declare watchSpawnCount										int			outer	FALSE
	/call DeclareIniVar watchSpawn							string	Common FALSE /watchspawn
	

	| set debuff switches by class	
	| Mezz
	/if (${Select[${Me.Class.ShortName},ENC,BRD,NEC,MAG,DRU]}) {
		/call DeclareIniVar switchdeBuffMezz			bool		Common FALSE
	}
	| Malo	
	/if (${Select[${Me.Class.ShortName},SHM,MAG]}) {
		/call DeclareIniVar switchdeBuffMalo			bool		Common FALSE
	}  
	| Tash
	/if (${Select[${Me.Class.ShortName},ENC]}) {
		/call DeclareIniVar switchdeBuffTash			bool		Common FALSE
	}  
	| Cripple
	/call DeclareIniVar switchdeBuffCripple			bool		Common FALSE

	| Slow
	/if (${Select[${Me.Class.ShortName},SHM,MAG,NEC,BRD,ENC,BST]}) {
		/call DeclareIniVar switchdeBuffSlow			bool		Common FALSE
	}  
	| AA Eradicate Magic Line
	/if (${Me.Class.CanCast}) {
		/call DeclareIniVar switchdeBuffEradicate	bool	common FALSE
	}
	| AA Snare Line
	/if (${AASnareList.Find[${Me.Class.ShortName}]}) {
		/call DeclareIniVar switchdeBuffSnare			bool		Common FALSE
	} 
	
	/call DeclareIniVar doclickItem							int			Items 0 
	/call DeclareIniVar clickItem1							string	Items FALSE 
	/call DeclareIniVar clickItem2							string	Items FALSE 
	/call DeclareIniVar clickItem3							string	Items FALSE 
	/call DeclareIniVar clickItem4							string	Items FALSE 
		
	/call DeclareIniVar doclickItemToast				int			Items 0 
	/call DeclareIniVar clickItemToast1					string	Items FALSE 
	/call DeclareIniVar clickItemToast2					string	Items FALSE 
	/call DeclareIniVar clickItemToast3					string	Items FALSE 
	/call DeclareIniVar clickItemToast4					string	Items FALSE 	
	
	/call DeclareIniVar Breastplate1		 				string	Items FALSE	
	/call DeclareIniVar Breastplate1effect			string	Items FALSE
	/call DeclareIniVar Breastplate2		 				string	Items FALSE
	/call DeclareIniVar Breastplate2effect			string	Items FALSE	
	/call DeclareIniVar Epic										string	Items FALSE	
	/call DeclareIniVar CircleofPower						string	Items FALSE	
	/call DeclareIniVar CircleofLife						string	Items FALSE	
	/call DeclareIniVar CircleofMana						string	Items FALSE	
	
	
	/call DeclareIniVar doclickOdds							int			Odds 0 
	/call DeclareIniVar clickOddItem1						string	Odds FALSE 
	/call DeclareIniVar clickOddItem2						string	Odds FALSE 
	/call DeclareIniVar clickOddItem3						string	Odds FALSE 
	/call DeclareIniVar clickOddItem4						string	Odds FALSE 	
	
	| aas that you want to always keep going
	/call DeclareIniVar AANow1									string	AANow FALSE 
	/call DeclareIniVar AANow2									string	AANow FALSE 

	| discs that you want to always keep going
	/call DeclareIniVar DiscNow1								string	DiscNow FALSE 
	/call DeclareIniVar DiscNow2								string	DiscNow FALSE 


	| home camp / movement / tie	
	/call DeclareIniVar HomeSet									bool		Movement FALSE
	/call DeclareIniVar HomeRadius							int 		Movement 5 /homeradius
	/call DeclareIniVar HomeLeash								int 		Movement 40	/homeleash
	/call DeclareIniVar HomeDelayMin						int 		Movement 100
	/call DeclareIniVar HomeDelayMax						int 		Movement 500	
	/call DeclareIniVar CombatDistance					int			Movement 15	/combatdistance
	/declare tempHomeSet												bool		outer FALSE	
	/declare tempsnapstuck											bool		outer	FALSE
  /call DeclareIniVar setIgnoreLOS						bool		Movement FALSE
	/call DeclareIniVar SetTieToon 							string 	Movement FALSE /tietoon
	/call DeclareIniVar SetTie       						bool 		Movement FALSE
	/call DeclareIniVar SetTieDistance 					int 		Movement 15 /tiedistance
	/call DeclareIniVar SetStickBehind 					bool 		Movement FALSE
	/call DeclareIniVar SetFaceFast							bool		Movement FALSE


	| General class combat skills
	/call DeclareIniVar kick										bool		meleedisc FALSE
	/call DeclareIniVar bash										bool		meleedisc FALSE
	/call DeclareIniVar taunt										bool		meleedisc FALSE
	/call DeclareIniVar disarm									bool		meleedisc FALSE
	/if (${Select[${Me.Class.ShortName},BER]}) {
		/call DeclareIniVar frenzy								bool		meleedisc FALSE
	}
	/if (${Select[${Me.Class.ShortName},ROG]}) {
		/declare flagSoS													bool	outer FALSE
		/call DeclareIniVar backstab							bool		meleedisc FALSE
	}
	/if (${Select[${Me.Class.ShortName},BRD]}) {
		/call DeclareIniVar seloskick							bool		meleedisc FALSE
	}
	/if (${Select[${Me.Class.ShortName},MNK]}) {
		/call DeclareIniVar dragonpunch							bool		meleedisc FALSE
		/call DeclareIniVar eaglestrike							bool		meleedisc FALSE
		/call DeclareIniVar tigerclaw								bool		meleedisc FALSE
		/call DeclareIniVar flyingkick							bool		meleedisc FALSE
		/call DeclareIniVar roundkick								bool		meleedisc FALSE
	}
	
	/if (${Me.Class.PetClass} || ${Select[${Me.Class.ShortName},ENC,DRU,SHM,SHD,CLR,WIZ,RNG,BRD,BST]}) {
		/call DeclareIniVar sendPetPerc 					int			Pet 99 /petengageat
		/call DeclareIniVar usePet 								bool 		Pet FALSE
		/call DeclareIniVar sendPetSwarmPerc			int			Pet 99 /petswarmengageat
		/call DeclareIniVar usePetSwarm						bool 		Pet FALSE
		/call DeclareIniVar switchHealPet					bool 		Pet FALSE
	}
	
	/call DeclareIniVar sumFamiliar								string	Pet FALSE
	/call DeclareIniVar sumFamiliarBuff						string	Pet FALSE
	/call DeclareIniVar sumFamiliarLeave					bool		Pet TRUE


	/if (${Me.Class.PetClass} || ${Select[${Me.Class.ShortName},ENC,DRU,SHM,SHD]}) {
		/call DeclareIniVar	tAsk_for_weapons			string	Timer	10m

		/call DeclareIniVar PetTank 							bool 		Pet FALSE /pettank
		/call DeclareIniVar BuffPet 							bool 		Pet TRUE 
		/call DeclareIniVar useSummonWeps 				bool 		Pet TRUE
		/call DeclareIniVar PetTypeSpell 					string 	Pet FALSE /pettype
		/call DeclareIniVar EpicClick 						string 	Pet FALSE
		/call DeclareIniVar EpicPetBuff 					string 	Pet FALSE
		/call DeclareIniVar summonedWepSpell 			string 	Pet "Grant Thassis' Armaments"
		/call DeclareIniVar summonedWepClosedBag 	string 	Pet "Folded Pack of Thalassic Armaments"
		/call DeclareIniVar summonedFocusSpell 		string 	Pet "Grant Calix's Heirlooms"
		/call DeclareIniVar summonedArmorSpell 		string 	Pet "Grant Thassis' Plate"
		/call DeclareIniVar summonedMaskSpell 		string 	Pet "Grant Visor of Gobeker"
		/call DeclareIniVar summonedWepName				string 	Pet "Summoned: Thalassic Fireblade"
		/call DeclareIniVar shrinkPet 						string 	Pet	"Tiny Companion"
		/call DeclareIniVar PetHealSpell 					string 	Pet FALSE
		/call DeclareIniVar PetHealType 					string 	Pet "gem1"
		/call DeclareIniVar PetRenewalSpell 			string 	Pet FALSE
		/call DeclareIniVar PetRenewalType 				string 	Pet "gem2"
		/call DeclareIniVar PetIllusion		 				string 	Pet FALSE
		/call DeclareIniVar PetIllusionBuff				string 	Pet FALSE
		/call DeclareIniVar PetAAHeal							string 	Pet "Replenish Companion"
		/call DeclareIniVar PetBagSlotNumber			int 		Pet 8
		
		/call DeclareIniVar PetBuffCount 					int 		Pet 0
		
		| 10 pet buffs
		/for _count 1 to 10
			/call DeclareIniVar PetBuff${_count} 			string 	Pet FALSE
			/call DeclareIniVar PetBuff${_count}chk		string 	Pet FALSE
		/next _count
	}	
	
	/if (${Select[${Me.Class.ShortName},MAG,ENC,CLR,NEC,WIZ,SHM,DRU]}) { 
		/call DeclareIniVar RuneSpell 						string 	Spells FALSE
		/call DeclareIniVar TwincastSpell 				string 	Spells FALSE
		/call DeclareIniVar TwincastType 					string 	Spells FALSE
		/call DeclareInivar GatherSpell 					string 	Spells FALSE
		/call DeclareIniVar SilentCast	 					string	Spells FALSE
	}

	/call DeclareIniVar SelfBuffRemove					string	SelfBuff "|Illusion: Gnoll Reaver|Illusion: Darkfell Gnoll Reaver|Illusion: Blackburrow Gnoll Reaver|Illusion: Mucktail Gnoll Reaver|Illusion: Direwind Gnoll Reaver|Illusion: Human Pirate|Illusion: Ogre Pirate"
	
	/if (${Me.Class.CanCast} || ${Select[${Me.Class.ShortName},ROG]}) {
		/call DeclareIniVar buffGem 							string 	Spells "gem12"
		/call DeclareIniVar SpellSet 							string 	Spells core
		/call DeclareIniVar useBuffKill 					string	Spells FALSE /usebuffkill
		/call DeclareIniVar GoMLevel							int			Spells 105
		/call DeclareIniVar GoMNuke								string	Spells FALSE
		/call DeclareIniVar GoMNuke2							string	Spells FALSE
		| /call DeclareIniVar BuffAuto 							bool 		Buffs TRUE
		/call DeclareIniVar BuffCombat 						bool		Buffs TRUE 
		/call DeclareIniVar BuffSelf	 						bool		Buffs TRUE 
		/call DeclareIniVar BuffBC								bool		Buffs FALSE 
		/call DeclareIniVar BuffRaid							bool		Buffs FALSE
		/call DeclareIniVar BuffTell							bool		Buffs FALSE
		
		/call DeclareIniVar SelfBuffCount 				int 		SelfBuff 0
		| 10 self only buffs
		/for _count 1 to 10
			/call DeclareIniVar SelfBuff${_count}			string 	SelfBuff FALSE
			/call DeclareIniVar SelfBuff${_count}chk	string 	SelfBuff FALSE
		/next _count
		/call DeclareIniVar BuffCount 						int 		Buffs 0

		| 10 bc/tell buffs
		/for _count 1 to 10
			/call DeclareIniVar Buff${_count}							string 	Buffs FALSE
			/call DeclareIniVar Buff${_count}chk					string 	Buffs FALSE
			/call DeclareIniVar Buff${_count}Alias				string 	Buffs |FALSE|FALSE 
			/call DeclareIniVar BCBuff${_count}Class			string 	Buffs |WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM
			/call DeclareIniVar RaidBuff${_count}Class		string 	Buffs |WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM
			/call DeclareIniVar Buff${_count}noBCinRaid		bool	 	Buffs FALSE
		/next _count
	}

	/if (${Select[${Me.Class.ShortName},MAG,ENC,CLR,PAL,BRD,NEC,BER,DRU,MNK]}) {
		/call DeclareIniVar Aura1Spell 						string 	Aura FALSE
		/call DeclareIniVar Aura1Buff 						string 	Aura FALSE  
	}
	/if (${Select[${Me.Class.ShortName},CLR,ENC]}) {
		/call DeclareIniVar Aura2Spell 						string 	Aura FALSE
		/call DeclareIniVar Aura2Buff 						string 	Aura FALSE
	}

	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM]}) {
		/declare callID														int 		outer

		/call DeclareIniVar PetPoint 							int			Heal 80	
		/call DeclareIniVar CLRPoint 							int 		Heal 80
		/call DeclareIniVar DRUPoint 							int 		Heal 70
		/call DeclareIniVar SHMPoint 							int 		Heal 60
		/call DeclareIniVar WIZPoint 							int 		Heal 75
		/call DeclareIniVar MAGPoint 							int 		Heal 75
		/call DeclareIniVar ENCPoint 							int 		Heal 80
		/call DeclareIniVar NECPoint 							int 		Heal 75
		/call DeclareIniVar WARPoint 							int 		Heal 70
		/call DeclareIniVar MNKPoint 							int 		Heal 60
		/call DeclareIniVar ROGPoint 							int 		Heal 60
		/call DeclareIniVar RNGPoint 							int 		Heal 90
		/call DeclareIniVar BSTPoint 							int 		Heal 70
		/call DeclareIniVar SHDPoint 							int 		Heal 90
		/call DeclareIniVar BRDPoint 							int 		Heal 65
		/call DeclareIniVar BERPoint 							int 		Heal 70
		/call DeclareIniVar PALPoint 							int 		Heal 90
		/call DeclareIniVar MercWarPoint 					int 		Heal 70
		/call DeclareIniVar MercCLRPoint 					int 		Heal 80
		/call DeclareIniVar MercDPSpoint 					int 		Heal 80	
		/call DeclareIniVar XHealClass						string	Heal |WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM
		/call DeclareIniVar tXHeal								string	Timer 3m   
		| /call DeclareIniVar useRadiantCure				bool		Cure TRUE   
		/call DeclareIniVar toonControlXTslot			int			Heal 13
		/call DeclareIniVar toonControlProtect		bool		Heal TRUE
	}

	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,BST,PAL,RNG]}) {
 		/call DeclareIniVar cureCurse 						string 	Cure FALSE
		/call DeclareIniVar curePoison 						string 	Cure FALSE
		/call DeclareIniVar cureDisease 					string 	Cure FALSE
		/call DeclareIniVar cureCorrupt 					string 	Cure FALSE
		/call DeclareIniVar switchHealXTarget			bool 		Heal FALSE
		/call DeclareIniVar switchHealGroup				bool 		Heal FALSE
		/call DeclareIniVar switchHealCure				bool 		Heal FALSE
	}

	/call DeclareIniVar switchHealCure				bool 		Heal FALSE
	/call DeclareIniVar cureCurseItem					string 	Cure FALSE
	/call DeclareIniVar curePoisonItem 				string 	Cure FALSE
	/call DeclareIniVar cureDiseaseItem				string 	Cure FALSE
	/call DeclareIniVar cureCorruptItem				string 	Cure FALSE
	/call DeclareIniVar noCureList	 					string 	Cure "|Sunset's Shadow"

/return
	
  
   
|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|   
sub declare_MAG()
	/call DeclareIniVar ServantNuke 						string 	Pet "Remorseless Servant"
	/call DeclareIniVar tCauldron_use						string 	Timer 360m

	/call DeclareIniVar ParadoxSpell 						string 	Rods "Grant Icebound Paradox"
	/call DeclareIniVar ParadoxRodName 					string 	Rods "Summoned: Icebound Fragment"
	/call DeclareIniVar OrbSpell 								string 	Rods "Summon Blazing Orb"
	/call DeclareIniVar OrbName 								string 	Rods "Blazing Orb"
	/call DeclareIniVar SelfRodSpell 						string 	Rods "Wand of Dark Modulation"
	/call DeclareIniVar SelfRodName 						string 	Rods "Wand of Pelagic Modulation"
	/call DeclareIniVar ModRodSpell 						string 	Rods "Large Modulation Shard"
	/call DeclareIniVar ModRodName 							string 	Rods "Summoned: Large Modulation Shard"
	/call DeclareIniVar ElementRodSpell 				string 	Rods "Summon Mutinous Minion"
	/call DeclareIniVar ElementRodName 					string 	Rods "Summoned: Exigent Minion XXI"

	/call DeclareIniVar GatherSpell							string 	Spells "Gather Magnitude"
	/call DeclareIniVar DoNuke					 				int			Spells 2
	/call DeclareIniVar Nuke1 									string 	Spells "Spear of Blistersteel"
	/call DeclareIniVar Nuke2 									string 	Spells "Spear of Molten Shieldstone"
	/call DeclareIniVar Nuke3 									string 	Spells FALSE
	/call DeclareIniVar Nuke4 									string 	Spells FALSE
	/call DeclareIniVar NukeSalvo								string 	Spells "Storm of Many"
	/call DeclareIniVar DoDoT						 				int			Spells 0
	/call DeclareIniVar Cauldron				 				string	Spells "Cauldron of Countless Goods"
	/call DeclareIniVar CauldronDestroy 				string 	Spells "Tavon's Burnished Gemstone|Tavon's Polished Gemstone|Ether-Fused Shard|Imprint of the Enhanced Minion|Crystallized Sulfur|Worlu's Windcloak|Worlu's Prying Eyes|"
	/call DeclareIniVar useRain									bool 		Spells FALSE /userain
	/call DeclareIniVar doRainNuke							int 		Spells 2
	/call DeclareIniVar RainNuke1 							string 	Spells "Rain of Cutlasses"
	/call DeclareIniVar RainNuke2 							string 	Spells "Coronal Rain"
	/call DeclareIniVar MaloSpell 							string 	Spells "Malosinete"
/return
	


|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_WIZ()
	| /call DeclareIniVar Familiar 								string 	Spells "Kerafyrm's Prismatic Familiar"
	/call DeclareIniVar GatherSpell							string 	Spells "Quietscent Harvest"
	/call DeclareIniVar GatherPercent			 			int			Spells 60 /gather
	/call DeclareIniVar DoNuke								 	int			Spells 5
	/call DeclareIniVar Nuke1 									string 	Spells "Cloudburst Stormstrike"
	/call DeclareIniVar Nuke2 									string 	Spells "Claw of the Flameweaver"
	/call DeclareIniVar Nuke3 									string 	Spells "Narendi's Fire"
	/call DeclareIniVar Nuke4 									string 	Spells "Etherial Skyblaze"
	/call DeclareIniVar Nuke5 									string 	Spells "Ethereal Rimeblast"
	/call DeclareIniVar Nuke6 									string 	Spells FALSE
	/call DeclareIniVar Nuke7 									string 	Spells FALSE
	/call DeclareIniVar Nuke8 									string 	Spells FALSE
	/call DeclareIniVar tempPet						 			string	Pet "Yulin's Pyroblade"
	
	/call DeclareIniVar fay 										string	Transport "Translocate: Fay"
	/call DeclareIniVar dragonscale 						string	Transport "Translocate: Dragonscale Hills"
	/call DeclareIniVar northkarana 						string	Transport "Translocate: North"
	/call DeclareIniVar commonlands 						string	Transport "Translocate: Common"
	/call DeclareIniVar nektulos 								string	Transport "Translocate: Nek"
	/call DeclareIniVar qeyenos 								string	Transport "Translocate: West"
	/call DeclareIniVar northro 								string	Transport "Translocate: Ro"
	/call DeclareIniVar cazic 									string	Transport "Translocate: Cazic"
	/call DeclareIniVar undershore 							string	Transport "Translocate: Undershore"
	/call DeclareIniVar westkarana 							string	Transport "Translocate: West Karana"
	/call DeclareIniVar bloodfields 						string	Transport "Translocate: Bloodfields"
	/call DeclareIniVar wallofslaughter 				string	Transport "Translocate: Slaughter"
	/call DeclareIniVar shardslanding 					string	Transport "Translocate: Shard's Landing"
	/call DeclareIniVar pillars 								string	Transport "Translocate Pillars of Alra"
	/call DeclareIniVar sarithc 								string	Transport "Translocate Sarith"
	/call DeclareIniVar dreadlands 							string	Transport "Translocate: Combine"
	/call DeclareIniVar lceanium								string	Transport "Translocate: Lceanium"
	/call DeclareIniVar dawnshroud 							string	Transport "Translocate: Dawnshroud"
	/call DeclareIniVar twilight 								string	Transport "Translocate: Twilight"
	/call DeclareIniVar grimling 								string	Transport "Translocate: Grimling"
	/call DeclareIniVar nexus 									string	Transport "Translocate: Nexus"
	/call DeclareIniVar bind 										string	Transport "Translocate"
	/call DeclareIniVar primary 								string	Transport "Translocate: Primary Anchor"
	/call DeclareIniVar secondary 							string	Transport "Translocate: Secondary Anchor"
	/call DeclareIniVar pushprimary 						string	Transport "Primary Anchor Push"
	/call DeclareIniVar pushsecondary 					string	Transport "Secondary Anchor Push"
	/call DeclareIniVar tempesttemple 					string	Transport "Translocate: Tempest Temple"
	/call DeclareIniVar katta										string	Transport "Translocate: Katta Castrum"
	/call DeclareIniVar tox 										string	Transport "Translocate: Tox"
	/call DeclareIniVar stonebrunt 							string	Transport "Translocate: Stonebrunt"
	/call DeclareIniVar icefall 								string	Transport "Translocate: Icefall Glacier"
	/call DeclareIniVar sunderdock 							string	Transport "Translocate: Sunderdock Springs"
	/call DeclareIniVar blightfire 							string	Transport "Translocate: Blightfire Moors"
	/call DeclareIniVar Barnidu 								string	Transport "Translocate: Barindu"
	/call DeclareIniVar Natimbi 								string	Transport "Translocate: Natimbi"
	/call DeclareIniVar grounds 								string	Transport "Translocate: the Grounds"
	/call DeclareIniVar potime 									string	Transport "Translocate: Plane of Time"
	/call DeclareIniVar brells 									string	Transport "Translocate: Brell's Rest"
	/call DeclareIniVar Arcstone 								string	Transport "Translocate: Arcstone"
	/call DeclareIniVar pok 										string	Transport "Translocate: Knowledge"
	/call DeclareIniVar cobaltscar 							string	Transport "Translocate: Cobalt Scar"
	/call DeclareIniVar wakening 								string	Transport "Translocate: Wakening Lands"
	/call DeclareIniVar greatdivide 						string	Transport "Translocate: Great Divide"
	/call DeclareIniVar iceclad 								string	Transport "Translocate: Iceclad"		
	
	
/return	




|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_MNK()
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_ENC()
	/declare mobList string outer
	/declare addList string outer
	| /if (!${Defined[aggroAnim]}) /declare aggroAnim string outer |5|8|12|17|18|42|44|80|106|129|144

	/call DeclareIniVar	tCheck_Runes						string	Timer	10s


	/call DeclareIniVar runeVeil			 					string	Rune "Veil of Mindshadow"
	/call DeclareIniVar runeEldritch	 					string	Rune "Eldritch Rune"
	/call DeclareIniVar runeBrimstone 					string	Rune "Brimstone Resolution"
	/call DeclareIniVar runeSpray			 					string	Rune "Glyph Spray"
	/call DeclareIniVar runeReactive	 					string	Rune "Reactive Rune"
	/call DeclareIniVar runePoly			 					string	Rune "Polyrefractive Rune"
	/call DeclareIniVar runeUnity			 					string	Rune FALSE
	/call DeclareIniVar runeWard			 					string	Rune "Ward of the Enticer"
	/call DeclareIniVar runeAuspice		 					string	Rune "Darkened Auspice"
	/call DeclareIniVar runeAuspiceToon					string  Rune |FALSE|FALSE
	/call DeclareIniVar runeMind								string  Rune "Mind Over Matter"
	/call DeclareIniVar runeEntAuspice					string	Rune "Enticer's Auspice"
	/call DeclareIniVar runeLegion							string	Rune "Legion of Lucem"

	/call DeclareIniVar mannaCrystal						string 	Spells "Azure Mind Crystal"
	/call DeclareIniVar healCrystal							string 	Spells "Sanguine Mind Crystal"
	
	/call DeclareIniVar spellCripple 						string 	Spells "Undermining Helix"
	/call DeclareIniVar spellSlow		 						string	Spells "Undermining Helix"		
	/call DeclareIniVar AASlow			 						string	Spells "Dreary Deeds"		
	/call DeclareIniVar spellTash 							string 	Spells "Decree of Tashan"

	/call DeclareIniVar spellMez								string 	Spells "Chaotic Bewildering"
	/call DeclareIniVar spellpbaeMez						string	Spells "Perilous Bafflement"
	/call DeclareIniVar spellStun								string	Spells "Color Concourse"
	/call DeclareIniVar spellCharm							string	Spells "Enticer's Command"
	
	/call DeclareIniVar CharmList 							string 	Pet FALSE
	
	/call DeclareIniVar DoNuke					 				int			Spells 3
	/call DeclareIniVar Nuke1					 					string	Spells "Chromatic Blink"
	/call DeclareIniVar Nuke2					 					string	Spells "Intellectual Appropriation"
	/call DeclareIniVar Nuke3					 					string	Spells "Mindsunder"
	/call DeclareIniVar Nuke4					 					string	Spells FALSE
	
	/call DeclareIniVar DoDoT						 				int			Spells 0
	/call DeclareIniVar DoT1					 					string	Spells FALSE
	/call DeclareIniVar DoT2					 					string	Spells FALSE
	/call DeclareIniVar DoT3					 					string	Spells FALSE
	/call DeclareIniVar DoT4					 					string	Spells FALSE
	
	
	
	
/return	



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_NEC()
	/call DeclareIniVar tCall_for_Blood					string	Timer 30s
		
	/declare set																int			outer 1
	/declare gem																int			outer 1
	
	/call DeclareIniVar SpellSets 							int 		Spells 4
	/call DeclareIniVar SpellSet1 							string 	Spells core1
	/call DeclareIniVar SpellSet2 							string 	Spells core2
	/call DeclareIniVar SpellSet3 							string 	Spells core3
	/call DeclareIniVar SpellSet4 							string 	Spells core4
	/call DeclareIniVar MaloSpell 							string 	Spells "Scent of Terris"
	/call DeclareIniVar CallforBlood						string 	Spells "Impose for Blood"

	/call DeclareIniVar dotR1-1	 								string 	Spells "Pyre of Jorobb"
	/call DeclareIniVar dotR1-2 								string 	Spells "Pyre of Marnek"	
	/call DeclareIniVar dotR1-3	 								string 	Spells "Ignite Synapses"
	/call DeclareIniVar dotR1-4	 								string 	Spells "Annihilation"
	/call DeclareIniVar dotR1-5	 								string 	Spells "Scourge of Fates"
	/call DeclareIniVar dotR1-6	 								string 	Spells "Glistenwing Venom"
	/call DeclareIniVar dotR1-7	 								string 	Spells "Plexipharia's Pallid Haze"
	/call DeclareIniVar dotR1-8	 								string 	Spells FALSE
	/call DeclareIniVar dotR1-9	 								string 	Spells "Dark Leech"
	/call DeclareIniVar dotR1-10 								string 	Spells "Smouldering Shadow"
	/call DeclareIniVar dotR1-11 								string 	Spells "Mortiferous Wounds"
	
	
	/call DeclareIniVar dotR2-9	 								string 	Spells "Coruscating Shadow"
	/call DeclareIniVar dotR2-10 								string 	Spells "Binaesa Venom"
	/call DeclareIniVar dotR2-11 								string 	Spells "Liquefaction"	
	
	/call DeclareIniVar dotR3-9	 								string 	Spells "Halstor's Pallid Haze"
	/call DeclareIniVar dotR3-10 								string 	Spells "Livianus' Decay"	
	/call DeclareIniVar dotR3-11 								string 	Spells "Grip of Jabaum"	
	
	/call DeclareIniVar dotR4-9	 								string 	Spells "Pyre of the Forsaken"
	/call DeclareIniVar dotR4-10 								string 	Spells "Ignite Thoughts"	
	/call DeclareIniVar dotR4-11 								string 	Spells "Dissolution"	

	/call DeclareIniVar DoDoT						 				int			Spells 0
	/call DeclareIniVar DoT1					 					string	Spells "Combust Bones"
	/call DeclareIniVar DoT2					 					string	Spells "Pyre of Jorobb"
	/call DeclareIniVar DoT3					 					string	Spells "Ignite Snypases"
	/call DeclareIniVar DoT4					 					string	Spells "Ilsaria's Swift Sickness"
	/call DeclareIniVar DoT5					 					string	Spells "Annihilation"
	/call DeclareIniVar DoT6					 					string	Spells FALSE
	/call DeclareIniVar DoT7					 					string	Spells FALSE
	/call DeclareIniVar DoT8					 					string	Spells FALSE
	/call DeclareIniVar DoT9					 					string	Spells FALSE
	/call DeclareIniVar DoT10					 					string	Spells FALSE

	/call DeclareIniVar DoNuke					 				int			Spells 0
	/call DeclareIniVar Nuke1					 					string	Spells FALSE	
	/call DeclareIniVar Nuke2					 					string	Spells FALSE
	/call DeclareIniVar Nuke3					 					string	Spells FALSE
	/call DeclareIniVar Nuke4					 					string	Spells FALSE
	/call DeclareIniVar Nuke5					 					string	Spells FALSE
	/call DeclareIniVar Nuke6					 					string	Spells FALSE

/return	



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_SHD()
	/declare mobAECount													int	outer
	/call DeclareIniVar Withstand 							string 	Discs "Repel"
	/call DeclareIniVar BladeStrike 						string 	Discs "Lacerating Blade"
	/call DeclareIniVar CrimsonBlade 						string 	Discs "Carmine Blade"
	/call DeclareIniVar DefensiveDisc 					string 	Discs "Doomscale Mantle"
	/call DeclareIniVar DefensiveDisc2 					string 	Discs "Grelleth's Carapace"
	/call DeclareIniVar DefensiveDisc3 					string 	Discs "Unholy Guardian Discipline"
	/call DeclareIniVar DiscRest			 					string	Discs "Breather"
	/call DeclareIniVar DiscFakeHate						string	Discs	"Unflinching Acrimony"
	/call DeclareIniVar DiscLeech								string	Discs	"Leechcurse Discipline"
	/call DeclareIniVar DiscRancor							string	Discs	"Reflexive Revulsion"


	/call DeclareIniVar ChallengeSpell					string 	Spells "Impose for Power"
	/call DeclareIniVar Hate1 									string 	Spells "Terror of Narus"
	/call DeclareIniVar Hate2 									string 	Spells "Terror of Poria"
	/call DeclareIniVar spellCripple						string 	Spells ""
	/call DeclareIniVar spellUndeadCripple			string 	Spells "Helix of the Undying"
	 
	/call DeclareIniVar AEHate1 								string 	Spells "Disgust"
	/call DeclareIniVar AEHate2 								string 	Spells "Repugnance"
	/call DeclareIniVar AELifeTap 							string 	Spells "Deceitful Deflection"
	
	/call DeclareIniVar DoNuke					 				int			Spells 2
	/call DeclareIniVar Nuke1 									string 	Spells "Touch of Holmein"
	/call DeclareIniVar Nuke2 									string 	Spells "Touch of Lutzen"
	/call DeclareIniVar Nuke3 									string 	Spells FALSE
	/call DeclareIniVar DoDoT						 				int			Spells 0
	/call DeclareIniVar DoT1					 					string	Spells FALSE
	/call DeclareIniVar DoT2					 					string	Spells FALSE
	/call DeclareIniVar DoT3					 					string	Spells FALSE
	 
	/call DeclareIniVar HpTapSpell 							string 	Spells "Touch of Lanys"
	/call DeclareIniVar HpTapSpellRecourse 			string 	Spells "Gift of Lanys"
	/call DeclareIniVar ManaTapSpell1 					string 	Spells "Bonemaw's Bite"
	/call DeclareIniVar ManaTapSpell2 					string 	Spells "Ancient: Bite of Muram"
	
	/call DeclareIniVar Harmonious 							string 	Spells "Harmonious Disruption"
/return	



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_PAL()
	/declare mobAECount													int			outer
	/declare MajorHurt 													int 		outer 0
	/declare WorstHurt 													string 	outer NULL
	/declare WorstHurtID 												int 		outer
	/declare WorstHP 														int 		outer 100
	/declare WorstClass 												string 	outer NULL
	/declare MinorHurt													int    	outer 0 
	
	/call DeclareIniVar useGroupHeal 						bool		Heal TRUE /usegroupheal
  /call DeclareIniVar healWholeGroup					string 	Heal "Wave of Grief"
  /call DeclareIniVar healTouch								string 	Heal "Ardent Touch"
  /call DeclareIniVar healPanic								string 	Heal FALSE
|	/declare rezID 															int 		outer
	/declare callID															int 		outer

	/call DeclareIniVar PetPoint 								int			Heal 80	
	/call DeclareIniVar CLRPoint 								int 		Heal 80
	/call DeclareIniVar DRUPoint 								int 		Heal 70
	/call DeclareIniVar SHMPoint 								int 		Heal 60
	/call DeclareIniVar WIZPoint 								int 		Heal 75
	/call DeclareIniVar MAGPoint 								int 		Heal 75
	/call DeclareIniVar ENCPoint 								int 		Heal 80
	/call DeclareIniVar NECPoint 								int 		Heal 75
	/call DeclareIniVar WARPoint 								int 		Heal 70
	/call DeclareIniVar MNKPoint 								int 		Heal 60
	/call DeclareIniVar ROGPoint 								int 		Heal 60
	/call DeclareIniVar RNGPoint 								int 		Heal 90
	/call DeclareIniVar BSTPoint 								int 		Heal 70
	/call DeclareIniVar SHDPoint 								int 		Heal 90
	/call DeclareIniVar BRDPoint 								int 		Heal 65
	/call DeclareIniVar BERPoint 								int 		Heal 70
	/call DeclareIniVar PALPoint 								int 		Heal 90
	
	/call DeclareIniVar doYaulp 								bool		SelfBuff TRUE /doyaulp	
	/call DeclareIniVar DiscRest			 					string	Discs "Breather"
	/call DeclareIniVar DiscFastRune	 					string	Discs "Repel"
	/call DeclareIniVar DiscArmor			 					string	Discs "Armor of Ardency"
	/call DeclareIniVar DiscGuard			 					string	Discs "Skalber Mantle"
	/call DeclareIniVar DiscUnflinching					string	Discs "Unflinching Affirmation"
	/call DeclareIniVar DiscJudgement						string	Discs "Inquisitors Judgement"
	/call DeclareIniVar DiscHate								string	Discs "Projection of Piety"
	/call DeclareIniVar DiscTwincast						string	Discs "Hand of Tunare"
	/call DeclareIniVar DiscReflexive						string	Discs "Reflexive Reverence"
	

	/call DeclareIniVar AALayonHands						string	Discs "Lay on Hands"
	/call DeclareIniVar AAHandofPiety						string	Discs "Hand of Piety"
	/call DeclareIniVar AAForceofDisruption			string	Discs "Force of Disruption"
	/call DeclareIniVar AADisruptivePersecution	string	Discs "Disruptive Persecution"
	/call DeclareIniVar AADivineStun						string	Discs "Divine Stun"

	/call DeclareIniVar AAAEBeacon							string	Discs "Beacon of the Righteous"
	/call DeclareIniVar AAAELodestar						string	Discs "Hallowed Lodestar"

	/call DeclareIniVar AAInquisitor						string	Discs "Armor of the Inquisitor"
	/call DeclareIniVar AAGroupInquisitor				string	Discs "Group Armor of the Inquisitor"

	/call DeclareIniVar AAJudgement							string	Discs "Inquisitor's Judgement"
	/call DeclareIniVar AAValorousRage					string	Discs "Valorous Rage"
	/call DeclareIniVar AAShield								string	Discs "Shield of Brilliance"
	

	/call DeclareIniVar DoNuke					 				int			Spells 5
	/call DeclareIniVar Nuke1										string 	Spells "Force of Ardency"
	/call DeclareIniVar Nuke2										string 	Spells "Ardent Force"
	/call DeclareIniVar Nuke3										string 	Spells "Force of the Darkened Sea"
	/call DeclareIniVar Nuke4										string 	Spells "Lesson of Grief"
	/call DeclareIniVar Nuke5					 					string	Spells "Protective Proclamation"


	/call DeclareIniVar ChallengeSpell					string 	Spells "Impose for Honor"
	/call DeclareIniVar ValiantDefense					string 	Spells "Valiant Defense"
	
	/call DeclareIniVar Twincast								string 	Spells "Glorious Exoneration"
	
	/call DeclareIniVar Harmonious 							string 	Spells "Harmonious Blessing"
	
/return	



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_WAR()
	/call DeclareIniVar DiscInsult 							string 	Discs "Insult"
	/call DeclareIniVar DiscCyclone			 				string	Discs "Cyclone Roar"
	/call DeclareIniVar DiscWade				 				string	Discs "Wade In To Battle"
	/call DeclareIniVar DiscChallenge						string	Discs "Roar of Challenge"
	/call DeclareIniVar DiscShout				 				string	Discs "Tormenting Shout"
	/call DeclareIniVar DiscRest			 					string	Discs "Breather"
	/call DeclareIniVar DiscStoutDefense 				string	Discs "Stout Defense"
	/call DeclareIniVar DiscField				 				string	Discs "Field Protector"
	/call DeclareIniVar DiscFlash				 				string	Discs "Flash of Anger"
	/call DeclareIniVar DiscNoTime				 			string	Discs "Pain Doesn't Hurt"
	/call DeclareIniVar DiscAttention				 		string	Discs "Unflinching Attention"
	/call DeclareIniVar DiscPhantom					 		string	Discs "Phantom Aggressor"
	/call DeclareIniVar AAGutPunch 							string	Discs "Gut Punch"
	/call DeclareIniVar AAWarStomp 							string	Discs "War Stomp"
	/call DeclareIniVar AABraceforImpact 				string	Discs "Brace for Impact"
	/call DeclareIniVar AAMageHunter						string	Discs "Mark of the Mage Hunter"
	/call DeclareIniVar AAVhementRage 					string	Discs "Vhement Rage"
	/call DeclareIniVar AABladeGuardian 				string	Discs "Blade Guardian"
	/call DeclareIniVar AAKneeStrike 						string	Discs "Knee Strike"
	/call DeclareIniVar AAImpCommand						string	Discs "Imperator's Command"
	/call DeclareIniVar AAHeroicBlade 					string	Discs "Ward Sheol's Heroic Blade"
	/call DeclareIniVar AAResplendentGlory 			string	Discs "Resplendent Glory"
	/call DeclareIniVar AAHoldtheLine 					string	Discs "Hold the Line"
	/call DeclareIniVar AAAETaunt 							string	Discs "Area Taunt"
	/call DeclareIniVar AABlastofAnger					string	Discs "Blast of Anger"
	/call DeclareIniVar AARageoftheForsaken			string	Discs "Rage of the Forsaken"
	/call DeclareIniVar AAWarlordsTenacity			string	Discs "Warlords Tenacity"
	/call DeclareIniVar AAWarlordsBravery				string	Discs "Warlords Bravery"
	
	/call DeclareIniVar PanicHPPCT		 					int			Heal 25
	
/return	



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_BER()
	/call DeclareIniVar DiscSnare 							string 	Discs FALSE
	/call DeclareIniVar SnareAxe 								string 	Discs FALSE
	/call DeclareIniVar DiscVolley 							string 	Discs "Brutal Volley"
	/call DeclareIniVar CryOfChaos 							string 	Discs "Ancient: Cry of Chaos"
	/call DeclareIniVar DiscBerserking 					string 	Discs "Berserking Discipline"
	/call DeclareIniVar DiscRest 								string 	Discs "Breather"
	/call DeclareIniVar DiscCombatFrenzy 				string 	Discs "Stormwild Frenzy"
	/call DeclareIniVar DiscScream 							string 	Discs "Distressing Scream"
	/call DeclareIniVar ThrowingAxe 						string 	Discs "Demolishing Axe Throw"
	/call DeclareIniVar DiscJolt 								string 	Discs "Jarring Smite"
	/call DeclareIniVar SharedBloodLust 				string 	Discs "Shared Cruelty"
	/call DeclareIniVar SharedBloodLustRecourse	string 	Discs "Reflected Brutality II"
	/call DeclareIniVar DiscOverwhelmingFrenzy 	string 	Discs "Demolishing Frenzy"
	/call DeclareIniVar DiscCleavingAnger 			string 	Discs "Cleaving Acrimony Discipline"
	/call DeclareIniVar DiscSlapintheFace 			string 	Discs "Kick in the Shins"
	/call DeclareIniVar DiscAxeofRallos 				string 	Discs "Axe of Numicia"
	/call DeclareIniVar DiscVengeful 						string 	Discs "Avenging Flurry Discipline"
	/call DeclareIniVar DiscFesteringRage 			string 	Discs "Bubbling Rage"
	/call DeclareIniVar DiscCryHavoc 						string 	Discs "Cry Carnage"
	/call DeclareInivar DiscAugmentedFrenzy 		string 	Discs "Amplified Frenzy"
  /call DeclareInivar DiscOpenWound 					string 	Discs "Open Wound"
  /call DeclareInivar DiscFrenzyResolve				string 	Discs "Frenzied Resolve"
  /call DeclareInivar UntamedgRage						string 	Discs "Untamed Rage"
  /call DeclareInivar RecklessAbandon					string 	Discs "Reckless Abandon"
 	/call DeclareIniVar frenzy									bool		meleedisc FALSE
 	/call DeclareIniVar DiscDicho		 						string 	Discs "Dichotomic Rage"
 	/call DeclareIniVar DiscDisconcert					string 	Discs "Disconcerting Discipline"
 	/call DeclareIniVar DiscEnduranceTap				string 	Discs "Sapping Strikes"

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
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_RNG()
	/call DeclareIniVar tNuke_Summer						string	Timer 3s

	/call DeclareIniVar DoNuke					 				int			Spells 5
	/call DeclareIniVar Nuke1					 					string	Spells "Anticipated Shots"	
	/call DeclareIniVar Nuke2					 					string	Spells "Focused Arrowrain"
	/call DeclareIniVar Nuke3					 					string	Spells "Heartsplit"
	/call DeclareIniVar Nuke4					 					string	Spells "Wildfire Boon"
	/call DeclareIniVar Nuke5					 					string	Spells "Wildfire Ash"
	/call DeclareIniVar Nuke6					 					string	Spells FALSE
	/call DeclareIniVar NukeSummer		 					string	Spells "Summer's Cyclone"

	/call DeclareIniVar DoDoT						 				int			Spells 3
	/call DeclareIniVar DoT1					 					string	Spells "Blisterbeetle Swarm"
	/call DeclareIniVar DoT2					 					string	Spells "Swarm of Glistenwings"
	/call DeclareIniVar DoT3					 					string	Spells "Vespid Swarm"
	/call DeclareIniVar DoT4					 					string	Spells FALSE
	/call DeclareIniVar SlowHeal								string	Spells "Cloudfont" 
	/call DeclareIniVar FastHeal			 					string	Spells "Desperate Dousing" 
	/call DeclareIniVar FastHealPCT		 					int			Spells 25

	/call DeclareIniVar DiscMele			 					string	Spells "Copsestalker's Discipline"
	/call DeclareIniVar DiscRange			 					string	Spells "Pureshot Discipline"
	/call DeclareIniVar DiscRest			 					string	Spells "Breather"
	/call DeclareIniVar DiscJolt			 					string	Spells "Jolting Cut Kicks"
	/call DeclareIniVar DiscEnrage		 					string	Spells "Enraging Cut Kicks"
	/call DeclareIniVar DiscHeal			 					string	Spells "Reflexive Bladespurs"
	/call DeclareIniVar BladeStorm							string	Spells "Focused Storm of Blades"
	/call DeclareIniVar ElementalArrow					string	Spells "Elemental Arrow"
	/call DeclareIniVar GlacialArow							string	Spells "Glacial Arrow"
	/call DeclareIniVar VolatileArrow						string	Spells "Volatile Arrow"
	/call DeclareIniVar ArrowPoison			 				string	Spells "Poison Arrows"	
	/call DeclareIniVar ArrowFire			 					string	Spells "Flaming Arrows"	
	/call DeclareIniVar ArrowIce				 				string	Spells "Frost Arrows"	
	/call DeclareIniVar AuspiceTXT							string	Spells "AUSPICE (critSpell 33%, accuracy 230%, critHeal 33%, critMelee 225%, critDoT 33%, attack +165)"
	/call DeclareIniVar DiscSqual			 					string	Spells "Focused Squall of Blades"
/return	



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_DRU()
	/call DeclareIniVar debuffRo			 					string	Spells "Blessing of Ro"	
	/call DeclareIniVar WoodTXT									string	Spells "SoWood cast."
	
	/call DeclareIniVar DoNuke					 				int			Spells 0
	/call DeclareIniVar Nuke1					 					string	Spells FALSE
	/call DeclareIniVar Nuke2					 					string	Spells FALSE
	/call DeclareIniVar Nuke3					 					string	Spells FALSE
	/call DeclareIniVar Nuke4					 					string	Spells FALSE
	/call DeclareIniVar DoDoT						 				int			Spells 0
	/call DeclareIniVar DoT1					 					string	Spells FALSE
	/call DeclareIniVar DoT2					 					string	Spells FALSE
	/call DeclareIniVar DoT3					 					string	Spells FALSE	
	/call DeclareIniVar DoT4					 					string	Spells FALSE	
	
	/declare MajorHurt 													int 		outer 0
	/declare WorstHurt 													string 	outer NULL
	/declare WorstHurtID 												int 		outer
	/declare WorstHP 														int 		outer 100
	/declare WorstClass 												string 	outer NULL
  /declare MinorHurt													int    	outer 0 
  /declare WorstHp 														int    	outer 100
  /declare pWorstHurt 												string 	outer ${Me.DisplayName}
  /declare pWorstHP														int			outer 100
  /declare pWorstHurtID												int 		outer ${Me.ID}
  /declare pWorstClass												string	outer NULL
  /declare pMajorHurt													int			outer 0
	
	/call DeclareIniVar RandRezDelay						int 		Heal 10
	/call DeclareIniVar ProtectOutsideGroup			string	Heal |NULL|NULL
	/call DeclareIniVar ProtectInsideRaid				string	Heal |NULL|NULL
	/call DeclareIniVar ProtectNPCList					string	Heal |NULL|NULL
	/call DeclareIniVar useGroupHeal 						bool		Heal TRUE /usegroupheal

	/call DeclareIniVar healPanic			 					string  Heal "Survival of the Serendipitous"
	/call DeclareIniVar healRejuvilation				string  Heal "Resurgence"
	/call DeclareIniVar healAdrenaline		 			string 	Heal "Daggerthorn Rush"
	/call DeclareIniVar healSanavida	 					string  Heal "Panavida"
	/call DeclareIniVar healWholeGroup					string 	Heal "Lunasoothe"
	
	
	| ports	
	/call DeclareIniVar looping									string	Transport "Zephyr: Looping Planes"
	/call DeclareIniVar steamfront							string	Transport "Zephyr: Steamfront"
	/call DeclareIniVar butcherblock						string	Transport "Zephyr: Butcherblock"
	/call DeclareIniVar shardslanding						string	Transport "Zephyr: Shard's Landing"
	/call DeclareIniVar pillars									string	Transport "Zephyr: Pillars of Alra"
	/call DeclareIniVar beast										string	Transport "Zephyr: Beasts' Domain"
	/call DeclareIniVar westkarna								string	Transport "Zephyr: West Karana"
	/call DeclareIniVar lavastorm								string	Transport "Zephyr: Lavastorm"
	/call DeclareIniVar undershore							string	Transport "Zephyr: Undershore"
	/call DeclareIniVar misty										string	Transport "Zephyr: Misty"
	/call DeclareIniVar ro											string	Transport "Zephyr: Ro"
	/call DeclareIniVar feerrott								string	Transport "Zephyr: Feerrott"
	/call DeclareIniVar commonlands							string	Transport "Zephyr: Commonlands"
	/call DeclareIniVar surefall								string	Transport "Zephyr: Surefall Glade"
	/call DeclareIniVar karana									string	Transport "Zephyr: Karana"
	/call DeclareIniVar wallofslaughter 				string	Transport "Zephyr: Slaughter"
	/call DeclareIniVar bloodfields 						string	Transport "Zephyr: Bloodfields"
	/call DeclareIniVar lceanium								string	Transport "Zephyr: Lceanium"
	/call DeclareIniVar dreadlands							string	Transport "Zephyr: Combines"
	/call DeclareIniVar dawnshroud							string	Transport "Zephyr: Dawnshroud"
	/call DeclareIniVar twilight								string	Transport "Zephyr: Twilight"
	/call DeclareIniVar grimling								string	Transport "Zephyr: Grimling"
	/call DeclareIniVar nexus										string	Transport "Zephyr: Nexus"
	/call DeclareIniVar primarypush							string	Transport "Primary Anchor Push"
	/call DeclareIniVar secondarypush						string	Transport "Secondary Anchor Push"
	/call DeclareIniVar primary									string	Transport "Zephyr: Primary Anchor"
	/call DeclareIniVar secondary								string	Transport "Zephyr: Secondary Anchor"
	/call DeclareIniVar tempesttemple						string	Transport "Zephyr: Tempest Temple"
	/call DeclareIniVar buriedsea								string	Transport "Zephyr: Buried Sea"
	/call DeclareIniVar stonebrunt							string	Transport "Zephyr: Stonebrunt"
	/call DeclareIniVar tox											string	Transport "Zephyr: Toxxulia"
	/call DeclareIniVar direwind								string	Transport "Zephyr: Direwind"
	/call DeclareIniVar steppes									string	Transport "Zephyr: Steppes"
	/call DeclareIniVar blightfire							string	Transport "Zephyr: Blightfire Moors"
	/call DeclareIniVar barindu									string	Transport "Zephyr: Barindu"
	/call DeclareIniVar natimbi									string	Transport "Zephyr: Natimbi"
	/call DeclareIniVar grounds									string	Transport "Zephyr: the Grounds"
	/call DeclareIniVar potime									string	Transport "Zephyr: Plane of Time"
	/call DeclareIniVar brells									string	Transport "Zephyr: Brell's Rest"
	/call DeclareIniVar arcstone								string	Transport "Zephyr: Arcstone"
	/call DeclareIniVar knowledge								string	Transport "Zephyr: Knowledge"
	/call DeclareIniVar cobaltscar							string	Transport "Zephyr: Cobalt Scar"
	/call DeclareIniVar wakening								string	Transport "Zephyr: Wakening Land"
	/call DeclareIniVar greatdivide							string	Transport "Zephyr: Great Divide"
	/call DeclareIniVar iceclad									string	Transport "Zephyr: Iceclad"	
/return	



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_SHM()
	/declare MajorHurt 													int 		outer 0
	/declare WorstHurt 													string 	outer NULL
	/declare WorstHurtID 												int 		outer
	/declare WorstHP 														int 		outer 100
	/declare WorstClass 												string 	outer NULL
  /declare MinorHurt													int    	outer 0 
  /declare WorstHp 														int    	outer 100	
  
	/call DeclareIniVar AidTXT 									string 	Spells "ANCESTRAL AID Done (Str/Agi/Dex +300, HoT Heal +1840)"
	/call DeclareIniVar MaloSpell 							string 	Spells "Malosinete"
	/call DeclareIniVar AASlow			 						string	Spells "Turgur's Swarm"	
	/call DeclareIniVar spellSlow								string 	Spells FALSE
	/call DeclareIniVar spellAESlow 						string 	Spells "Srasku's Drowse"
	/call DeclareIniVar usePathosis 						bool	 	Spells TRUE /pathiosis
	
	/call DeclareIniVar TC1							 				string	Spells "Glacial Gift"	
	/call DeclareIniVar TC2							 				string	Spells "Frigid Gift"
	/call DeclareIniVar DoNuke					 				int			Spells 0
	/call DeclareIniVar Nuke1					 					string	Spells "Reefmaw's Bite"
	/call DeclareIniVar Nuke2					 					string	Spells FALSE
	/call DeclareIniVar Nuke3					 					string	Spells FALSE
	/call DeclareIniVar DoDoT						 				int			Spells 0
	/call DeclareIniVar DoT1					 					string	Spells FALSE
	/call DeclareIniVar DoT2					 					string	Spells FALSE
	/call DeclareIniVar DoT3					 					string	Spells FALSE
	/call DeclareIniVar spellCripple 						string 	Spells "Crippling Counterbias"

	/call DeclareIniVar CaniPercent				 			int			Spells 60 /cani
	/call DeclareIniVar spellCannibalization		string	Spells "Tribal Pact"

	/call DeclareIniVar useGroupHeal 						bool		Heal TRUE /usegroupheal
	/call DeclareIniVar useGroupHoT							bool		Heal TRUE /usegrouphot
	/call DeclareIniVar RandRezDelay						int 		Heal 10

  /call DeclareIniVar healSurge								string 	Heal "Spiritual Surge"
  /call DeclareIniVar healMain								string 	Heal "Krasir's Mending"
  /call DeclareIniVar healIntervention				string 	Heal "Historian's Intervention"
  /call DeclareIniVar healReckless						string 	Heal "Reckless Regeneration"
  /call DeclareIniVar healWholeGroup					string 	Heal "Krasir's Recourse"
  /call DeclareIniVar healWholeGroupHoT				string 	Heal "Shear of Renewal"

	/call DeclareIniVar ProtectOutsideGroup			string	Heal |NULL|NULL
	/call DeclareIniVar ProtectInsideRaid				string	Heal |NULL|NULL
	/call DeclareIniVar ProtectNPCList					string	Heal |NULL|NULL

	/declare HealList 													string 	outer |${healSurge}|${healWholeGroup}|${healWholeGroupHoT}|${healMain}|${healIntervention}|${healReckless}

/return	



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_BRD()
	/if (!${Defined[mobList]}) /declare mobList string outer
	/if (!${Defined[addList]}) /declare addList string outer

	/call DeclareIniVar ServantNukeAA1 					string 	Pet "Lyrical Prankster"
	/call DeclareIniVar ServantNukeAA2 					string 	Pet "Song of Stone"
	/call DeclareIniVar songServantNuke					string 	Pet "Plague of the Piper"
	
	/call DeclareIniVar songList								string 	Song TANK
	/call DeclareIniVar songTANKList						string	Song "|Chorus of Sionachie|War March of Jocelyn|Fjilnauk's Spiteful Lyric Rk. II|Fjilnauk's Song of Suffering|Doben's Spry Sonata Rk. II"
	/call DeclareIniVar songCASTERList					string	Song "|Chorus of Sionachie|Nilsara's Aria|Fatesong of Jocelyn|Arcane Melody|Aria of Maetanrus"
	/call DeclareIniVar songMELEEList						string	Song "|Chorus of Sionachie|War March of Jocelyn|Fjilnauk's Spiteful Lyric|Fjilnauk's Song of Suffering|Arcane Melody"
	/call DeclareIniVar songMIXList							string	Song "|Chorus of Sionachie|War March of Jocelyn|Fjilnauk's Spiteful Lyric|Nilsara's Aria"

	/call DeclareIniVar songDicho								string	Song "Dichotomic Psalm"
	
	/call DeclareIniVar DoNuke					 				int			Song 1
	/call DeclareIniVar Nuke1										string 	Song "Tsaph's Insult"
	/call DeclareIniVar Nuke2										string 	Song NULL
	
	/call DeclareIniVar spellMez								string 	Song "Slumber of Silisia"
	/call DeclareIniVar spellpbaeMez						string	Song "Wave of Torpor"
	
	
/return	


|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_BST()
|	/call DeclareIniVar useAASlow 							bool 		Spells TRUE /aaslow
	/call DeclareIniVar DiscForay 							string 	Spells "Pummel"
	/call DeclareIniVar DiscBestial							string 	Spells "Bestial Evulsing"
	/call DeclareIniVar DiscFocusClaws 					string 	Spells "Focused Clamor of Claws"
	/call DeclareIniVar DiscSavage 							string 	Spells "Savage Rage"
	/call DeclareiniVar DiscRuaabri 						string 	Spells "Ruaabri's Fury"	
	/call DeclareIniVar GrowlSpell 							string 	Spells "Growl of the Leopard"
 	/call DeclareIniVar ServantNuke		 					string 	Spells "Cry at the Moon"
  /call DeclareIniVar FeroSpell 							string 	Spells "Savage Ferocity"
  /call DeclareIniVar DiscEnduring						string 	Spells "Enduring Frenzy"
  /call DeclareIniVar DiscRending							string 	Spells "Reflexive Rending"
	
	/call DeclareIniVar GrowlSpell 							string 	Pet "Growl of the Leopard"
	/call DeclareIniVar TasteofBlood						string 	Pet "Taste of Blood"
	/call DeclareIniVar Aggression							string 	Pet "Sekmoset's Aggression"
	/call DeclareIniVar ProcHeal								string 	Pet "Mending Warder"
	/call DeclareIniVar Calm										string 	Pet "Companion's Calm Demeanor"
	/call DeclareIniVar DiscRest				 				string	Spells "Breather"
	/call DeclareIniVar ParagonTXT 							string 	Spells "PARAGON DONE (+5790 HP/tick, +2450 Mana/tick, +264 End/tick)"
	
	/call DeclareIniVar DoNuke					 				int			Spells 5
	/call DeclareIniVar Nuke1					 					string	Spells "Fozen Miasma"	
	/call DeclareIniVar Nuke2					 					string	Spells "Kromtus Lance"
	/call DeclareIniVar Nuke3					 					string	Spells "Visoracius' Maelstrom"
	/call DeclareIniVar Nuke4					 					string	Spells "Kromrif Lance"
	/call DeclareIniVar Nuke5					 					string	Spells "Krieg's Bite"
	/call DeclareIniVar Nuke6					 					string	Spells FALSE

	/call DeclareIniVar DoDoT						 				int			Spells 0
	/call DeclareIniVar DoT1					 					string	Spells FALSE
	/call DeclareIniVar DoT2					 					string	Spells FALSE
	/call DeclareIniVar DoT3					 					string	Spells FALSE
	/call DeclareIniVar DoT4					 					string	Spells FALSE
	
	/call DeclareIniVar FastHeal			 					string	Spells "Sabhattin's Mending" 
	/call DeclareIniVar FastHealPCT		 					int			Spells 60
	
	/call DeclareIniVar selfParagon		 					int			Spells 80
	/call DeclareIniVar groupParagon	 					int			Spells 70
	/call DeclareIniVar minConsumptionHP				int			Spells 50
		
/return	



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_MNK()
/return	



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_ROG()
	| keep on cooldown
	/call DeclareIniVar DiscRest				 				string Discs "Breather"
	/call DeclareIniVar DiscAggro 							string Discs "Disorientation"
	/call DeclareIniVar DiscAssault 						string Discs "Barrage"
	/call DeclareIniVar DiscBleed 							string Discs "Hack"	
	/call DeclareIniVar DiscPlay	 							string Discs "Knifeplay Discipline"	
	/call DeclareIniVar DiscPhantom							string Discs "Phantom Assassin"	
	/call DeclareIniVar DiscHack								string Discs "Jugular Hack"	
	/call DeclareIniVar DiscHiddenBlade					string Discs "Hidden Blade"	
	/call DeclareIniVar DiscBladePoison					string Discs "Reefcrawler Blade"	

	| disable for toast
	/call DeclareIniVar DiscVision 							string Discs "Thief's Vision"	

	| toast
	/call DeclareIniVar DiscPinPoint 						string Discs "Pinpoint Deficiencies"
	/call DeclareIniVar DiscMark 								string Discs "Wide-Eyed Mark"
	/call DeclareIniVar DiscEradicator 					string Discs "Executioner Discipline"
	/call DeclareIniVar DiscRazor 							string Discs "Razor's Edge Discipline"	
	/call DeclareIniVar DiscFrenzy 							string Discs "Frenzied Stabbing Discipline"	
	/call DeclareIniVar DiscTwistedChance				string Discs "Twisted Chance Discipline"
	
/return	



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_CLR()
	DEBUG ${cbug}declare_CLR()\ax
	/call DeclareIniVar tHealWard								string	Timer 185s
		
	/declare MajorHurt 													int 		outer 0
	/declare WorstHurt 													string 	outer NULL
	/declare WorstHurtID 												int 		outer
	/declare WorstHP 														int 		outer 100
	/declare WorstClass 												string 	outer NULL
	/declare MinorHurt													int    	outer 0 
	
	/declare pWorstHurt 												string 	outer ${Me.DisplayName}
	/declare pWorstHP														int			outer 100
	/declare pWorstHurtID												int 		outer ${Me.ID}
	/declare pWorstClass												string	outer NULL
	/declare pMajorHurt													int			outer 0
	
	/declare WorstManaID												int			outer 0
	/declare WorstManaPct												int			outer 0
	
	
	/call DeclareIniVar DivineRezCleric					bool 		Heal TRUE
	/call DeclareIniVar RandRezDelay						int 		Heal 5
	/call DeclareIniVar ProtectOutsideGroup			string	Heal |FALSE|FALSE
	/call DeclareIniVar ProtectInsideRaid				string	Heal |FALSE|FALSE
	/call DeclareIniVar ProtectNPCList					string	Heal |FALSE|FALSE
	/call DeclareIniVar useGroupHeal 						bool		Heal TRUE /usegroupheal
	/call DeclareIniVar useGroupHoT							bool		Heal TRUE /usegrouphot
	/call DeclareIniVar tempPet						 			string	Pet "Ardent Hammer of Zeal"
	/call DeclareIniVar setDivinetoGroupHeal		bool		Heal FALSE

  /call DeclareIniVar healWholeGroup					string 	Heal "Word of Greater Reformation"
  /call DeclareIniVar healWholeGroupHoT				string 	Heal "Ardent Acquittal"
  /call DeclareIniVar healRemedy1							string 	Heal "Spiritual Remedy"
  /call DeclareIniVar healRemedy2							string 	Heal "Graceful Remedy"
  /call DeclareIniVar healHoT 								string 	Heal "Ardent Elixir"
  /call DeclareIniVar healSplash							string 	Heal "Convalescent Splash"
  /call DeclareIniVar healIntervention1 			string 	Heal "Virtuous Intervention"
  /call DeclareIniVar healIntervention2 			string  Heal "Elysian Intervention"
  /call DeclareIniVar healIntervention3 			string  Heal "Mystical Intervention"
  /call DeclareIniVar healRenewal1	 					string  Heal "Fervid Renewal"
  /call DeclareIniVar healRenewal2	 					string  Heal "Fraught Renewal"
  /call DeclareIniVar healRenewal3	 					string  Heal "Fervent Renewal"
  /call DeclareIniVar healPanic			 					string  Heal "Fifteenth Emblem"
  /call DeclareIniVar healWard			 					string  Heal "Ward of Surety"
  /call DeclareIniVar healWardToon	 					string  Heal |FALSE|FALSE
  /call DeclareIniVar useCelestialRegen				bool	  Heal TRUE
  /call DeclareIniVar healIssuance						string  Heal "Issuance of Spirit"
	/call DeclareIniVar CRTXT										string	Heal "Celestial Regeneration (+11100HP / tic)"
	/call DeclareIniVar switchHealAggressive		string	Heal FALSE
		
	/call DeclareIniVar doYaulp 								bool		SelfBuff TRUE /doyaulp	
	/call DeclareIniVar sumWeaponSpell					string	SelfBuff "Hammer of	Reverence"
	/call DeclareIniVar sumWeaponItem						string	SelfBuff "Hammer of	Reverence III"
	/call DeclareIniVar useQM										bool		SelfBuff TRUE /qm
	/call DeclareIniVar useQMPct								int			SelfBuff 40 /qmpct

	| /call DeclareIniVar Spire										string	Spells "Fundament: Third Spire of Divinity"
		
/return	



|***
 *	DES: This will clear all store alises that core has created in your MacroQuest2.INI file. or those with matching names.
 *	USE: /whitewash
 *	INI: MacroQuest2.INI
 *  NOTE: IF you instance/sandbox MQ/EQ it is best you run this on all toons at the same time. Sorry its slow as fuck.
 ***|
#event whitewash "[MQ2] @@@ Core: Deleting all Core aliases.. PLEASE WAIT"
sub Event_whitewash()
	DEBUG ${cbug}Event_whitewash()\ax
	/call whitewash
/return

sub whitewash()
	NOUT ${owarning} ${sep} Deleting all Core ${cinfo}${versionCore}\ax aliases. ${cbad}Please wait.\ax
	/declare _count 				int local
	/declare _todelete[22] 	string local
	/declare _index 				int local
	/declare _increment			int local 1
	
	
	| commands to remove forever
	/varset _todelete[1] |setdebuff|battlerez|escort|dodebuff|autorez|followdistance|useentropy|debug|autobuff
	/varset _todelete[2] |sellstuff|depositstuff|MezzImmune|bitch|combatrez|xheal|burnmobcount|combatcall|usemalotash
	/varset _todelete[3] |nukemana|nukedelay|nukemode|meleeescort|BCMA|aaslow|balance|autotoast|port|cleanup|cure|buffauto
	/varset _todelete[4] |usebuffkill|fubar|follow|meleefollow|healfollow|nukefollow|looter|lootignore|lootsell|nomezz|nocast|exclude|named|nomelee|rmob|nomagic|nodisease|nofire|nopoison|nocold|nocorruption
	
	| misc HUD commnads
	/varset _todelete[5] |verbose|hudupdate|control|whitewash|smartengageat|balance|broadcast|HUDTime
	
	| assist
	/varset _todelete[6] |ma1|ma2|agro|engageat|autorest|smartma|combatdistance|killmob|combatradius|smartengageat|setagro

	| modes
	/varset _todelete[7] |forage|fish|invites|train|combat|setpull|petfarm|setpuller|on|off|core|drag|ri|hunt

	| Misc commands
	/varset _todelete[8] |maxlevel|autoadjustexp|oneoff|ds|userain|resetcore|crew|info|banestrike|watchspawn|huntagro|autologin
	/varset _todelete[9] |combatbuff|pathiosis|bcsay|blurb|clean|invis|stein|gtfo|door|echochannel|info|whohas
	/varset _todelete[10] |mirror|checkcash|autoshrink|maintexplvl|autocontrol|killcode|echoaugments|relay|givecoin

	| heal
	/varset _todelete[11] |healoverride|healself|healgroup|healxtarget|healpet|healcure|usegrouphot|usegroupheal|xtheal|rc|purify|healaggressive
	/varset _todelete[12] |rezeveryone|rezcombat|rezauto|rezemergency|rezenable

	| debuffing
	/varset _todelete[13] |debuffmezz|debuffslow|debufftash|debuffmalo|debuffcripple|debufferadicate|debuffsnare

	| buffing
	/varset _todelete[14] |buffbc|bufftell|buffpet|buffitem|buffraid|buffraid|buffself|buffcombat|ib|sb|pb

	| toast
	/varset _todelete[15] |toastmobcount|toastauto|toastengageat|toastauto|settoast|toast

	| loots
	/varset _todelete[16] |lootleave|lootkeep|lootdestroy|lootsell|buy|sell|lootwhat|lootannounce|lootignore

	| zone list control
	/varset _todelete[17]	|listremove|listnomezz|listnocast|listexclude|listnamed|listnomelee|listrange|listnomagic|listnodisease|listnofire|listnopoison|listnocold|listnocorruption|listhuntmob

	| pets
	/varset _todelete[18]	|petengageat|petswarmengageat|companionswarm|companion|pettype|buildpet|pettank|sendpets

	| class commands
	/varset _todelete[19] |harvest|gather|rod|redorod|rods|cr|issuance|spice|useeradicate|song
	/varset _todelete[20] |qmpct|qm|glyph|dorain|doyaulp|lesson|wood|paragon|cani

	| movement
	/varset _todelete[21]	|primary|secondary|here|halt|ignorelos|facefast|stickbehind|stop|take
	/varset _todelete[22] |tietoon|tie|tiedistance|tietime|home|homeradius|homeleash
	
	
	/for _count 5 to ${_todelete.Size}
		/for _index 1 to ${_todelete[${_count}].Count[|]}
			NOUT Deleting Alias ${If[${_count} > 4,${_increment}:,]} ${cinfo}${_todelete[${_count}].Arg[${_index},|]}\ax
			/squelch /alias /${_todelete[${_count}].Arg[${_index},|]} delete
			/if (${_count} > 4) /varcalc _increment ${_increment}+1
			/delay 1
		/next _index	
	/next _count
	
	/call echos ending
	/endm
	:exit
/return




|************************************************************************************************************|
|* Created Events from this point on..                                                                       |
|************************************************************************************************************|
 
  
|***
 *	DES: stuf we do cause we are lazy.
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub set_alias()
	DEBUG ${cbug}set_alias()\ax Setting Aliases
	/if (!${Bool[${Alias[/info]}]}) 					/squelch /alias /info 											NOUT Core Instructions:
	| /if (!${Bool[${Alias[/port]}]}) 					/squelch /alias /port 											/bc port
	/if (!${Bool[${Alias[/take]}]}) 					/squelch /alias /take 											/bc take
	/if (!${Bool[${Alias[/door]}]}) 					/squelch /alias /door 											/bc door
	/if (!${Bool[${Alias[/invis]}]}) 					/squelch /alias /invis 											/bc invis
	
 	| zone list control
  /if (!${Bool[${Alias[/listremove]}]}) 				/squelch /alias /listremove							VOUT Set Target Remove!
  /if (!${Bool[${Alias[/listexclude]}]}) 				/squelch /alias /listexclude 						VOUT Set Target Exclude!
  /if (!${Bool[${Alias[/listnamed]}]}) 					/squelch /alias /listnamed 							VOUT Set Target Name!
  /if (!${Bool[${Alias[/listnomezz]}]}) 				/squelch /alias /listnomezz							VOUT Set Target NoMezz!
  /if (!${Bool[${Alias[/listnocast]}]}) 				/squelch /alias /listnocast 						VOUT Set Target NoCast!
  /if (!${Bool[${Alias[/listnomelee]}]}) 				/squelch /alias /listnomelee 						VOUT Set Target NoMelee!
  /if (!${Bool[${Alias[/listrange]}]}) 					/squelch /alias /listrange 							VOUT Set Target Range!
  /if (!${Bool[${Alias[/listnomagic]}]}) 				/squelch /alias /listnomagic 						VOUT Set Target NoMagic!
  /if (!${Bool[${Alias[/listnodisease]}]}) 			/squelch /alias /listnodisease 					VOUT Set Target NoDisease!
  /if (!${Bool[${Alias[/listnofire]}]}) 				/squelch /alias /listnofire 						VOUT Set Target NoFire!
  /if (!${Bool[${Alias[/listnopoison]}]}) 			/squelch /alias /listnopoison 					VOUT Set Target NoPoison!
  /if (!${Bool[${Alias[/listnocold]}]}) 				/squelch /alias /listnocold							VOUT Set Target NoCold!
  /if (!${Bool[${Alias[/listnocorruption]}]}) 	/squelch /alias /listnocorruption 			VOUT Set Target NoCorruption!
  /if (!${Bool[${Alias[/listhuntmob]}]}) 				/squelch /alias /listhuntmob 						VOUT Set Target HuntMob!
  /if (!${Bool[${Alias[/huntmobagro]}]}) 				/squelch /alias /huntmobagro						/echo HuntMobAgro:
  
   
  /if (!${Bool[${Alias[/sendpets]}]}) 			/squelch /alias /sendpets										NOUT SendPets: 
	/if (!${Bool[${Alias[/killmob]}]}) 				/squelch /alias /killmob 										/bc killmob
	/if (!${Bool[${Alias[/mirror]}]}) 				/squelch /alias /mirror 										/bc mirror
	/if (!${Bool[${Alias[/stein]}]}) 					/squelch /alias /stein 											/bc stein
  /if (!${Bool[${Alias[/stop]}]}) 					/squelch /alias /stop 											/bc stop
  /if (!${Bool[${Alias[/halt]}]}) 					/squelch /alias /halt 											/bc halt
	/if (!${Bool[${Alias[/ib]}]}) 						/squelch /alias /ib													NOUT Check Item Buffs.
	/if (!${Bool[${Alias[/sb]}]}) 						/squelch /alias /sb 												NOUT Check Self Buffs.
	/if (!${Bool[${Alias[/pb]}]}) 						/squelch /alias /pb 												NOUT Check Pet Buffs.
	/if (!${Bool[${Alias[/blurb]}]}) 					/squelch /alias /blurb 											NOUT Blurb: 
 	/if (!${Bool[${Alias[/clean]}]}) 					/squelch /alias /clean 											NOUT Clean this mess up.
 	/if (!${Bool[${Alias[/toast]}]}) 					/squelch /alias /toast 											NOUT Adjusting the toast dial !!
 	/if (!${Bool[${Alias[/whitewash]}]}) 			/squelch /alias /whitewash 									/echo \ar@@@\ax Core: Deleting all Core aliases.. \arPLEASE WAIT\ax
 	/if (!${Bool[${Alias[/smartma]}]}) 				/squelch /alias /smartma 										NOUT Adjusting Smart Assist !!
  /if (!${Bool[${Alias[/buildpet]}]}) 			/squelch /alias /buildpet 									NOUT Time for a new pet.
	/if (!${Bool[${Alias[/song]}]}) 					/squelch /alias /song 											NOUT Song: 
	/if (!${Bool[${Alias[/crew]}]}) 					/squelch /alias /crew 											NOUT Crew: 
	/if (!${Bool[${Alias[/here]}]}) 					/squelch /alias /here 											/bc here
  /if (!${Bool[${Alias[/broadcast]}]}) 			/squelch /alias /broadcast 									NOUT Set Broadcast Status!
  /if (!${Bool[${Alias[/bcsay]}]}) 					/squelch /alias /bcsay 											/bc bcsay
  /if (!${Bool[${Alias[/echoaugments]}]}) 	/squelch /alias /echoaugments								NOUT Echo Augment file!
  /if (!${Bool[${Alias[/whohas]}]}) 				/squelch /alias /whohas											/bc whohas
  /if (!${Bool[${Alias[/autologin]}]}) 			/squelch /alias /autologin									/plugin mq2autologin unload
   
	/if (!${Bool[${Alias[/buffraid]}]}) 			/squelch /alias /buffraid 									NOUT Set Raid Buffing!
	/if (!${Bool[${Alias[/buffbc]}]}) 				/squelch /alias /buffbc 										NOUT Set BC Buffing!
	/if (!${Bool[${Alias[/buffself]}]}) 			/squelch /alias /buffself 									NOUT Set Self Buffing!
	/if (!${Bool[${Alias[/buffitem]}]}) 			/squelch /alias /buffitem 									NOUT Set Item Buffing!
	/if (!${Bool[${Alias[/buffcombat]}]}) 		/squelch /alias /buffcombat 								NOUT Set Combat Buffing!
	/if (!${Bool[${Alias[/bufftell]}]}) 			/squelch /alias /bufftell 									NOUT Set /tell for Buffing!
	/if (!${Bool[${Alias[/buffpet]}]}) 				/squelch /alias /buffpet 										NOUT Set Pet Buffing!
	
	| healing thingies
	/if (!${Bool[${Alias[/healself]}]}) 			/squelch /alias /healself 									NOUT Set Self Healing!
	/if (!${Bool[${Alias[/healgroup]}]}) 			/squelch /alias /healgroup 									NOUT Set Group Healing!
	/if (!${Bool[${Alias[/healxtarget]}]}) 		/squelch /alias /healxtarget 								NOUT Set XTarget Healing!
	/if (!${Bool[${Alias[/healpet]}]}) 				/squelch /alias /healpet 										NOUT Set Pet Healing!
	/if (!${Bool[${Alias[/healcure]}]}) 			/squelch /alias /healcure 									NOUT Set Curing!
	/if (!${Bool[${Alias[/healaggressive]}]}) /squelch /alias /healaggressive							NOUT Set Aggressive Healing!
	
	| rez switches
	/if (!${Bool[${Alias[/rezcombat]}]}) 			/noparse /squelch /alias /rezcombat   		 	NOUT Adjusting Resurection in Combat!
	/if (!${Bool[${Alias[/rezeveryone]}]}) 		/noparse /squelch /alias /rezeveryone 		  NOUT Adjusting Resurection for Everyone!
	/if (!${Bool[${Alias[/rezauto]}]}) 				/noparse /squelch /alias /rezauto						NOUT Adjusting auto Rez/Calling!
	/if (!${Bool[${Alias[/rezemergency]}]}) 	/noparse /squelch /alias /rezemergency			NOUT Adjusting Emergency Rez!
	/if (!${Bool[${Alias[/rezenable]}]}) 			/noparse /squelch /alias /rezenable					NOUT Adjusting Resurrection/Call Casting!

	/if (!${Bool[${Alias[/rc]}]}) 						/squelch /alias /rc 												/bc rc
	/if (!${Bool[${Alias[/purify]}]}) 				/squelch /alias /purify 										/bc purify
	
	| debuff
	/if (!${Bool[${Alias[/debuffmezz]}]}) 			/squelch /alias /debuffmezz								NOUT Set deBuff Mezzing!
	/if (!${Bool[${Alias[/debuffmalo]}]}) 			/squelch /alias /debuffmalo								NOUT Set deBuff Malo!
	/if (!${Bool[${Alias[/debufftash]}]}) 			/squelch /alias /debufftash								NOUT Set deBuff Tash!
	/if (!${Bool[${Alias[/debuffcripple]}]}) 		/squelch /alias /debuffcripple						NOUT Set deBuff Cripple!
	/if (!${Bool[${Alias[/debuffslow]}]}) 			/squelch /alias /debuffslow								NOUT Set deBuff Slow!
	/if (!${Bool[${Alias[/debufferadicate]}]}) 	/squelch /alias /debufferadicate					NOUT Set deBuff Eradicate!
	/if (!${Bool[${Alias[/debuffsnare]}]}) 			/squelch /alias /debuffsnare							NOUT Set Snare!

		
	/if (!${Bool[${Alias[/resetcore]}]}) 			/squelch /alias /resetcore 									NOUT Setting Core to Defaults!
	/if (!${Bool[${Alias[/hudupdate]}]}) 			/squelch /alias /hudupdate 									NOUT Update the HUD!
	/if (!${Bool[${Alias[/home]}]}) 					/squelch /alias /home			 									NOUT Update Home Camp!

	/if (!${Bool[${Alias[/tie]}]}) 						/squelch /alias /tie			 									NOUT Set Tie!
	/if (!${Bool[${Alias[/stickbehind]}]}) 		/squelch /alias /stickbehind								NOUT Set Stick Behind!
	/if (!${Bool[${Alias[/facefast]}]}) 			/squelch /alias /facefast										NOUT Set Face Fast!

	/if (!${Bool[${Alias[/drag]}]}) 					/noparse /squelch /alias /drag							NOUT Setting Drag Mode!
	/if (!${Bool[${Alias[/forage]}]}) 				/noparse /squelch /alias /forage						NOUT Setting Forage Mode!
	/if (!${Bool[${Alias[/fish]}]}) 					/noparse /squelch /alias /fish							NOUT Setting Fish Mode!
	/if (!${Bool[${Alias[/train]}]}) 					/noparse /squelch /alias /train							NOUT Setting Train Skills Mode!
	/if (!${Bool[${Alias[/petfarm]}]}) 				/noparse /squelch /alias /petfarm						NOUT Setting Pet Farm Mode!
	/if (!${Bool[${Alias[/hunt]}]}) 					/noparse /squelch /alias /hunt							NOUT Setting Hunter Mode!

	/if (!${Bool[${Alias[/invites]}]}) 				/noparse /squelch /alias /invites						NOUT Setting Group / Raid Invite acceptance!

	/if (!${Bool[${Alias[/banestrike]}]}) 		/noparse /squelch /alias /banestrike				NOUT Setting Banestrike!


	/if (!${Bool[${Alias[/ri]}]}) 						/noparse /squelch /alias /ri								NOUT Adjusting Raid Invites!
	
	/if (!${Bool[${Alias[/setpuller]}]}) 			/noparse /squelch /alias /setpuller					NOUT Adjusting Puller!
	/if (!${Bool[${Alias[/setagro]}]}) 				/noparse /squelch /alias /setagro						NOUT Adjusting Agro!
	/if (!${Bool[${Alias[/core]}]}) 					/noparse /squelch /alias /core							NOUT Adjusting Core!
	
	| loot stuff
	/if (!${Bool[${Alias[/lootleave]}]}) 			/noparse /squelch /alias /lootleave					NOUT Set Loot Item Leave 
	/if (!${Bool[${Alias[/lootkeep]}]}) 			/noparse /squelch /alias /lootkeep					NOUT Set Loot Item Keep 
	/if (!${Bool[${Alias[/lootdestroy]}]}) 		/noparse /squelch /alias /lootdestroy				NOUT Set Loot Item Destroy
	/if (!${Bool[${Alias[/lootsell]}]}) 			/noparse /squelch /alias /lootsell					NOUT Set Loot Item Sell
	/if (!${Bool[${Alias[/lootignore]}]}) 		/noparse /squelch /alias /lootignore				NOUT Set Loot Item Ignore
	/if (!${Bool[${Alias[/lootwhat]}]}) 			/noparse /squelch /alias /lootwhat					NOUT Set Loot Item What
	/if (!${Bool[${Alias[/lootannounce]}]}) 	/noparse /squelch /alias /lootannounce			NOUT Set Loot Item Announce

  /if (!${Bool[${Alias[/buy]}]}) 						/squelch /alias /buy												/echo Buy:
  /if (!${Bool[${Alias[/sell]}]}) 					/squelch /alias /sell												/echo Sell:

	/if (!${Bool[${Alias[/on]}]}) 						/noparse /squelch /alias /on       		    	/bca //varset AutoBot TRUE
	/if (!${Bool[${Alias[/off]}]}) 						/noparse /squelch /alias /off						    /bca //varset AutoBot FALSE
	/if (!${Bool[${Alias[/zonereset]}]}) 			/noparse /squelch /alias /zonereset			    /bc zonereset

	/if (!${Bool[${Alias[/givecoin]}]}) 			/noparse /squelch /alias /givecoin			    NOUT Bribe Time:

	
	
	| force oneoff bad buff check
	/if (!${Bool[${Alias[/oneoff]}]}) 				/noparse /squelch /alias /oneoff   					/bc oneoff

	/if (!${Bool[${Alias[/checkcash]}]}) 			/noparse /squelch /alias /checkcash  			 	NOUT Check in game cash!

	/if (!${Bool[${Alias[/primary]}]}) 				/noparse /squelch /alias /primary						/bcaa //useitem Primary
	/if (!${Bool[${Alias[/secondary]}]}) 			/noparse /squelch /alias /secondary   		  /bcaa //useitem Secondary

	/if (!${Bool[${Alias[/hudtime]}]}) 				/noparse /squelch /alias /hudtime   		 	 	NOUT Adjusting HUD Time!
	/if (!${Bool[${Alias[/gtfo]}]}) 					/noparse /squelch /alias /gtfo    			 		/bc gtfo
	/if (!${Bool[${Alias[/lesson]}]}) 				/noparse /squelch /alias /lesson   			 		/bc lesson

	/if (!${Bool[${Alias[/autoshrink]}]})			/noparse /squelch /alias /autoshrink				NOUT Adjusting Automatic Shrink!
 	
	/if (!${Bool[${Alias[/ignorelos]}]})			/noparse /squelch /alias /ignorelos					NOUT Adjusting LoS Requirements!
	
 	
	/if (!${Bool[${Alias[/companion]}]})			/noparse /squelch /alias /companion					NOUT Adjusting Pet Usage!
	/if (!${Bool[${Alias[/companionswarm]}]})	/noparse /squelch /alias /companionswarm		NOUT Adjusting Pet Swarm Usage!
	/if (!${Bool[${Alias[/verbose]}]})				/noparse /squelch /alias /verbose						NOUT Setting VERBOSE Mode!
 	
	/if (!${Bool[${Alias[/autocontrol]}]})		/noparse /squelch /alias /autocontrol				NOUT Setting Automatic Control Toon swapping!
	
	/if (!${Bool[${Alias[/toastauto]}]})			/noparse /squelch /alias /toastauto					NOUT Adjusting Auto Toast on Named!

	/if (${Select[${Me.Class.ShortName},WAR]}) {
	} 
	/if (${Select[${Me.Class.ShortName},CLR]}) {
		/if (!${Bool[${Alias[/cr]}]}) 				/squelch /alias /cr 											/bc cr
		/if (!${Bool[${Alias[/issuance]}]}) 	/squelch /alias /issuance 								/bc issuance
	}
	/if (${Select[${Me.Class.ShortName},RNG]}) {
		/if (!${Bool[${Alias[/spice]}]}) 				/squelch /alias /spice 									/bc spice
	}
	/if (${Select[${Me.Class.ShortName},DRU]}) {
		/if (!${Bool[${Alias[/wood]}]}) 				/squelch /alias /wood 									/bc wood
	}
	/if (${Select[${Me.Class.ShortName},MAG]}) {
		/if (!${Bool[${Alias[/rods]}]}) 				/squelch /alias /rods 									/bc rods
	}
	/if (${Select[${Me.Class.ShortName},SHD]}) {
	}
	/if (${Select[${Me.Class.ShortName},ENC]}) {
	}
	/if (${Select[${Me.Class.ShortName},PAL]}) {
	}
	/if (${Select[${Me.Class.ShortName},BER]}) {
	}
	/if (${Select[${Me.Class.ShortName},SHM]}) {
		/if (!${Bool[${Alias[/aid]}]}) 					/squelch /alias /aid 										/bc aid
	}
	/if (${Select[${Me.Class.ShortName},NEC]}) {
	}
	/if (${Select[${Me.Class.ShortName},WIZ]}) {
	}
	/if (${Select[${Me.Class.ShortName},BRD]}) {
	}
	/if (${Select[${Me.Class.ShortName},BST]}) {
		/if (!${Bool[${Alias[/paragon]}]}) 			/squelch /alias /paragon 								/bc paragon
	}
	/if (${Select[${Me.Class.ShortName},ROG]}) {
	}
/return



#event Join "#1# invites you to join a #2#.#*#"
sub Event_Join(string line, string _sender, string _groupraid)
	DEBUG ${cbug}Event_Join(\ax${_sender} ${_groupraid}\ax${cbug})\ax
	CHECKSAFELIST
	/if (!${switchInvites}) /goto :exit
	VOUT Accepting invite from ${cpc}${_sender}\ax for ${_groupraid}.
	/if (${_groupraid.Equal[group]}) /invite
	/if (${_groupraid.Equal[raid]}) {
		/if (${Window[ConfirmationDialogbox].Child[CD_TextOutPut].Text.Find["join a raid"]}) {
			/nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
		} else {	
			/raidaccept	
		}
	}
	:exit
/return



|***
 *	DES: contains the various BC commands.
 *	USE: /bc SOMETHINGORANOTHER
 *	INI: 
 *  NOTE: works off tells or broadcasts
 ***| 
#event EQBC "<#1#> #2#"
#event EQBC "[#1#(msg)] #2#"
#event EQBC "<#1#> #2# #3#"
#event EQBC "[#1#(msg)] #2# #3#"
#event EQBC "<#1#> #2# #3# #4#"
#event EQBC "[#1#(msg)] #2# #3# #4#"
sub Event_EQBC(string line, string _sender, string _cmd, string _verbage, string _other)
	DEBUG cmd: ${_cmd} _sender ${_sender} _verbage ${_verbage}

	/if (${_cmd.Equal[horn]}) /call click_horn_of_unity 
	/if (${_cmd.Equal[whitewash]}) /call whitewash
	/if (${_cmd.Equal[stop]}) /call set_stop ${_sender}
  /if (${_cmd.Equal[setRaidInviteMode]}) /call setRaidInviteMode
  /if (${_cmd.Equal[smartma]}) /call Event_smartma
  /if (${_cmd.Equal[killmob]}) {
  	/varset ignoreEngageAt ${If[${ignoreEngageAt},FALSE,TRUE]}
		/call echos switch "Ignoring Engage at %" ignoreEngageAt
  }
  /if (${_cmd.Equal[mirror]} && ${Me.ItemReady[Mirror Fragment of Anashti Sul]}) /useitem "Mirror Fragment of Anashti Sul"
  /if (${_cmd.Equal[stein]} && ${Me.ItemReady[Drunkard's Stein]}) /useitem "Drunkard's Stein"
  /if (${_cmd.Equal[oneoff]}) /call check1offBuffs
  /if (${_cmd.Equal[spice]} && ${Select[${Me.Class.ShortName},RNG]}) /call cast_auspice
  /if (${_cmd.Equal[paragon]} && ${Select[${Me.Class.ShortName},BST]}) /call cast_paragon
  /if (${_cmd.Equal[rods]} && ${Select[${Me.Class.ShortName},MAG]}) /call cast_rods
  /if (${_cmd.Equal[aid]} && ${Select[${Me.Class.ShortName},SHM]}) /call doAid
  /if (${_cmd.Equal[cr]} && ${Select[${Me.Class.ShortName},CLR,DRU,PAL]}) /call cast_celest_regen
  /if (${_cmd.Equal[issuance]} && ${Select[${Me.Class.ShortName},CLR]}) /call cast_issuance_heal
  
  /if (${_cmd.Equal[lesson]}) {
  	/if (${Me.AltAbilityReady[Lesson of the Devoted]}) /call core_cast "Lesson of the Devoted" alt 0
  	/goto :exit
  }
  

	/if (${_cmd.Equal[handin]}) /call hand_in "${_sender}" "${_verbage}"
  
  | see who has the item and report back
	/if (${_cmd.Equal[whohas]}) /call check_who_has "${_sender}" "${_verbage}"
 
 	| buy things from a normal vendor
	| /if (${_cmd.Equal[buyitem]} && ${Spawn[pc ${_sender}].ID} == ${Me.ID}) /call buy_item ${_verbage} ${_other}
 

	| have everyone say the same thing to the same target with a touch of random delay
  /if (${_cmd.Equal[bcsay]}) {
 		/if (${_sender.NotEqual[${Me.DisplayName}]} && ${Spawn[pc ${_sender} radius 80].ID}) {
			/assist ${_sender}
			/delay 5s ${Me.AssistComplete} 
			/if (!${Target.ID}) //goto :exit
		}
		/if (${Me.Invis}) {
			/makemevisible
			/delay 1s !${Me.Invis}
		}
		/say ${_verbage}
	}
  
  | Halts physical movement routines. Nothing else.
  /if (${_cmd.Equal[halt]}) {
 		VOUT ${onotice} ${sep} HALTING ALL Movement!\ax
	 	/if (${Bool[${MakeCamp.Status}]}) {
			/makecamp off
			/call Event_SetVar NA HomeSet Movement FALSE 
		} 	
		/call Event_SetVar NA SetTie Movement FALSE
		:loopkillstick
			/stick off
			/squelch /afollow off
			/keypress forward
			/keypress back
			/delay 1
			/varset tempsnapstuck FALSE
		 	/if (${Stick.Active} || ${Me.Moving} || ${AdvPath.Following}) /goto :loopkillstick
	 	/goto :exit
  }
  
    
	| commands to control all assist targets.  
  /if (${_cmd.Equal[sma1]}) {
		/if (${useSmartAssist2}) /call Event_SetVar NA useSmartAssist2 Common FALSE
		/if (${useSmartAssistG}) /call Event_SetVar NA useSmartAssistG Common FALSE
 		/call Event_SetVar NA useSmartAssist1 Common TRUE
		/goto :exit
  }
  /if (${_cmd.Equal[sma2]}) {
 		/if (${useSmartAssist1}) /call Event_SetVar NA useSmartAssist1 Common FALSE
		/call Event_SetVar NA useSmartAssist2 Common TRUE
		/if (${useSmartAssistG}) /call Event_SetVar NA useSmartAssistG Common FALSE
		/goto :exit
  }
  /if (${_cmd.Equal[smag]}) {
 		/if (${useSmartAssist1}) /call Event_SetVar NA useSmartAssist1 Common FALSE
		/if (${useSmartAssist2}) /call Event_SetVar NA useSmartAssist2 Common FALSE
		/call Event_SetVar NA useSmartAssistG Common TRUE
 		/goto :exit
	}
	/if (${_cmd.Equal[smax]}) {
 		/if (${useSmartAssist1}) /call Event_SetVar NA useSmartAssist1 Common FALSE
		/if (${useSmartAssist2}) /call Event_SetVar NA useSmartAssist2 Common FALSE
		/if (${useSmartAssistG}) /call Event_SetVar NA useSmartAssistG Common FALSE
 		/goto :exit
	}
	
  
 
	/if (${_cmd.Equal[MELEE]}) {
		/if (!${Target.ID} || ${Target.ID}==${Me.ID}) {
			/varset combat FALSE
			/call echos needtarget
			/goto :exit
		}
		/varset isValidTarget TRUE
		/varset combat MELEE
		/varset Target2Kill ${Target.ID}		
	}
	/if (${_cmd.Equal[RANGE]}) {
		/if (!${Target.ID} || ${Target.ID}==${Me.ID}) {
			/varset combat FALSE
			/call echos needtarget
			/goto :exit
		} else /if (${Target.Distance} < 30) {
			/stick off
			NOUT ${cbad}Target too close.\ax
		}
		/varset isValidTarget TRUE
		/varset combat RANGE
		/varset Target2Kill ${Target.ID}		
	}
	/if (${_cmd.Equal[NUKE]}) {
		/if (!${Target.ID} || ${Target.ID}==${Me.ID} || ${Target.ID}==${Me.Pet.ID}) {
			/varset combat FALSE
			/call echos needtarget
			/goto :exit
		}
		/varset isValidTarget TRUE
		/varset combat NUKE
		/varset Target2Kill ${Target.ID}		
	}
	/if (${_cmd.Equal[HEAL]}) {
	}
	/if (${_cmd.Equal[DUAL]}) {
		/if (!${Target.ID} || ${Target.ID}==${Me.ID} || ${Target.ID}==${Me.Pet.ID}) {
			/varset combat FALSE
			/call echos needtarget
			/goto :exit
		}
		/varset isValidTarget TRUE
		/varset combat DUAL
		/varset Target2Kill ${Target.ID}	
	}


	| group bail stuff
  /if (${_cmd.Equal[gtfo]}) {
  	/if (${Bool[${GTFO}]}) {
  		/call sort_cursor TRUE
  		/call set_stop
  		/docommand /makemevisible
  		/if (${Me.ItemReady[${GTFO}]}) /call MQ2Cast "${GTFO}" item
   		/if (${Me.SpellReady[${GTFO}]}) /call MQ2Cast "${GTFO}"
   		/if (${Me.AltAbility[${GTFO}].ID}) /call MQ2Cast "${GTFO}" alt
  	}
  }


	| if you would be so kind as to stop running around like asses and kill the shit attackign us, that would be great.
  /if (${_cmd.Equal[here]} && !${Me.DisplayName.Equal[${_sender}]}) NOUT Set Combat Here!


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
		/if (!${timer_try_door}) /goto :exitdoor
		/doortarget 
		/delay 1
		/if (!${Switch.ID}) /goto :retrydoortarget
		:retryopendoor
		/if (!${timer_try_door}) /goto :exitdoor
		/if (!${Switch.Open}) {
			/click left door
			/delay 1s
			/doevents flush
		}
		:exitdoor
	}



	|***
	* DES: Tell toon to campfire clickie
	* USE: /bc campfire
	***|
	/if (${_cmd.Equal[campfire]} && ${Me.ItemReady[Fellowship Registration Insignia]}) {
		/if (${Me.Invis}) /makemevisible
		/delay 1s !${Me.Invis}
		/call core_cast  "Fellowship Registration Insignia" item
	}



	|***
	* DES: reset the core.zone.ini for all toons in BC if you make a change to the file.
	* USE: /bc zonereset
	***|
	/if (${_cmd.Equal[zonereset]}) {
		/if (!${Spawn[pc ${_sender}].ID}) /goto :exit

		/call echos zone
		/call check_zone_INI ${_sender}
		/if (${AutoBot} && ${setAutoShrink}) /call check_shrink
		/goto :exit
	}



	| wis healers only
	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,PAL]}) {
		| radient Cure
		/if (${_cmd.Equal[rc]} &&  ${Me.AltAbilityReady[Radiant Cure]}) {
			/call core_cast "Radiant Cure" alt ${Me.ID}
			ECHOCHANNEL Radient Cure
		}
		
		
		| Purify Spirits
		/if (${_cmd.Equal[purify]} &&  && ${Me.AltAbilityReady[Purified Spirits]}) {
			/call core_cast "Purified Spirits" alt ${Spawn[pc ${_sender}].ID}
			ECHOCHANNEL Purified Spirits: ${Spawn[pc ${_sender}].Name}
		}
		/goto :exit
	}


	|***
	 *	DES: Cast group invis. Stacks invis if right classes are in group
	 *	USE: /bc invis, /invis 
	 ***|
	/if (${_cmd.Equal[invis]} && (${Me.AltAbilityReady[Group Perfected Invisibility]} || ${Me.AltAbilityReady[Group Perfected Invisibility to Undead]})) {

		
		| /delay ${Math.Rand[5]}
		
		/if (${Select[${Me.Class.ShortName},WIZ,MAG,ENC]}) {
			/call core_cast "Group Perfected Invisibility" alt 0
		} else /if (${Select[${Me.Class.ShortName},PAL,CLR,SHD,NEC]}) {
			/delay 3
			/call core_cast "Group Perfected Invisibility to Undead" alt 0
		}
   	/delay 1s



|**
		/call MQ2Cast "Group Perfected Invisibility To Undead" alt
		/if (${Select[${Me.Class.ShortName},WIZ,MAG]}) {
			/echo invis
			| /delay 2
			/call MQ2Cast "Group Perfected Invisibility" alt
		}
		/delay 1s

	**|


   	/goto :exit
  }
	
	:exit
/return




#event camping "It will take you about 30 seconds to prepare your camp."
sub Event_camping(string line)
	DEBUG ${cbug}Event_camping()\ax
	/call clear_combat
	VOUT Core Macro ${cinfo}${versionCore}\ax ${sep} ${cbad}STOPPED\ax for camp.
	/endm
	/squelch /makecamp off
	/squelch /moveto pause
	/squelch /stick off
/return

#event skillup "#*#You have become better at #1#! (#2#)"
sub Event_skillup(string line, string skillname, int skillup)
   VOUT [${cinfo}${skillname}\ax] has increased to (${cinfo}${skillup}\ax)! [Max:${cinfo} ${Skill[${skillname}].SkillCap}\ax]
/return

#event task_update "Your task '#1#' has been updated."
sub Event_task_update(string line, string skillname)
   /bc [+g+]'[+x+][+y+]${skillname}[+x+][+g+]'[+x+] updated!
|   /if (${verbose}) /popup ${skillname} updated!
/return

#event noRaid "Your raid was disbanded."
sub Event_noRaid()
 		/if (${useSmartAssist1}) /call Event_SetVar NA useSmartAssist1 Common FALSE
		/if (${useSmartAssist2}) /call Event_SetVar NA useSmartAssist2 Common FALSE
		/if (${setRaidInvite}) /call Event_SetVar NA setRaidInvite Common FALSE
|		/call Event_SetVar NA useSmartAssistG Common TRUE
/return



|***
	DES: This is for lazy people like me that have to invite people to raids and dont wanna do the work.
	USE: /ri
***|
#event raidinvite             "#1# tells you#*#'#*#RI#*#'"
#event raidinvite             "#1# tells you#*#'#*#R I#*#'"
#event raidinvite             "#1# tells you#*#'#*#LFG#*#'"
#event raidinvite             "#1# tells you#*#'#*#LFR#*#'"
#event raidinvite             "#1# tells you#*#'#*#invite#*#'"

sub Event_raidinvite(string line, string _sender)
	/if (${setRaidInvite}) {
		/if (${Bool[${Raid.Member[${_sender}]}]}) {
			VOUT ${cpc}${_sender}\ax is already in the raid.
			/goto :exit
		}
		/if (${invitelist.Find[${_sender}]}) {
			VOUT \arNO\ax invites for ${cpc}${_sender}\ax.
			/goto :exit
		}
		/declare _timeDelay ${Math.Calc[${Math.Rand[40]}+20]}
		/varset _timeDelay ${Int[${_timeDelay}]}
		/delay 1m !${Raid.Locked}
		VOUT Delay: ${cbug}${_timeDelay}\ax \ag- \ax ${cpc}${_sender}\ax requested a raid invite. [Raid: ${If[${Raid.Locked},${cbad}LOCKED\ax,${cgood}UNLOCKED\ax]}]
		/delay 1m !${Raid.Locked}
		/delay ${_timeDelay}
		/raidinvite ${_sender}

	}
	:exit
/return



|**************************************************
 * Forced Events 
 **************************************************|
 
 
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
		/goto :exit
	}
	/if (${explist.Find[${exp}]} && ${event.Equal[LIST]}) {
		NOUT ${Ini[${INIBlurb},${exp.Upper}]}		
		/goto :exitEvent_Blurb
	}
	/if (${explist.Find[${exp}]} && ${Bool[${event}]}) {
		/varset eventlist ${Ini[${INIBlurb},${exp.Upper}]}
		/if (${eventlist.Find[${event}]}) {
				/varset blurb ${Ini[${INIBlurb},${exp.Upper},${event.Upper}]}
				/rs ${blurb}
		}
	}
	:exit
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

	:exit
/return 

 |***
 *	DES: Invites Crew 
 *	USE: /crew [name1|name2|so on]
 ***|
#event Crew "[MQ2] - Crew: #1#"
sub Event_Crew(string line, string _clist)
	DEBUG ${cbug}Event_Crew(\ax${exp.Upper}${cbug})\ax
	/call set_crew ${_clist}
	:exit
/return 

#event HUDTime "[MQ2] - Adjusting HUD Time!"
sub Event_HUDTime(string line)
	/varset HUDTime ${If[${HUDTime},FALSE,TRUE]}
	/if (${HUDTime}) VOUT Showing ${cinfo}Real Life\ax time.
	/if (!${HUDTime}) VOUT Showing ${cinfo}In Game\ax time.
/return

#event boradcaststatus "[MQ2] - Set Broadcast Status!"
sub Event_boradcaststatus(string line)
	/call Event_SetVar NA setBroadcast Common ${If[${Bool[${setBroadcast}]},FALSE,TRUE]}
	/if (${setBroadcast.Equal[FALSE]}) /ini "${INICore}" "transmit_status" "${Me.DisplayName}" "DISABLED"
	/if (${setBroadcast.Equal[TRUE]}) /ini "${INICore}" "transmit_status" "${Me.DisplayName}" "${AutoBot}"
	/call echos switch "Adjusting Broadcast Status" setBroadcast
/return


#event autocontrol "[MQ2] - Setting Automatic Control Toon swapping!"
sub Event_autocontrol(string line)
	/call Event_SetVar NA autoControl Common ${If[${autoControl},FALSE,TRUE]}
	/call echos switch "Auto Adjusting Control Toon" autoControl
/return

#event echoaugment "[MQ2] - Echo Augment file!"
sub Event_echoaugment(string line)
	/call echo_augments
/return

#event companion "[MQ2] - Adjusting Pet Usage!"
sub Event_companion(string line)
	/call Event_SetVar NA usePet Pet ${If[${usePet},FALSE,TRUE]}
	/call echos switch "Using Pets" usePet
	/if (!${usePet} && ${Pet.ID}) {
		/if (${Me.AltAbilityReady[Suspended Minion]}) {
			/call core_cast "Suspended Minion" alt 0
		} else {
			/pet leave
		}
	}
/return

#event companionswarm "[MQ2] - Adjusting Pet Swarm Usage!"
sub Event_companionswarm(string line)
	/call Event_SetVar NA usePetSwarm Pet ${If[${usePetSwarm},FALSE,TRUE]}
	/call echos switch "Using Pet Swarms" usePetSwarm
/return

#event verbose "[MQ2] - Setting VERBOSE Mode!"
sub Event_verbose(string line)
	/call Event_SetVar NA verbose Common ${If[${verbose},FALSE,TRUE]}
	/call echos switch "VERBOSE MODE" verbose TRUE
/return
   
#event gamecash "[MQ2] - Check in game cash!"
sub Event_gamecash(string line)
	/call check_free_ingame_cash
/return
 
#event toast "[MQ2] - Adjusting the toast dial !!"
sub Event_toast(string line)
	/varset setToastMode ${If[${setToastMode},FALSE,TRUE]}
	/call echos switch "Constant Toasting" setToastMode
/return
 
#event rezCombat "[MQ2] - Adjusting Resurection in Combat!"
sub Event_rezCombat()
	/call Event_SetVar NA rezCombat Heal ${If[${rezCombat},FALSE,TRUE]} 
	/call echos switch "Rez During Combat" rezCombat
/return

#event rezEveryone "[MQ2] - Adjusting Resurection for Everyone!"
sub Event_rezEveryone()
	/call Event_SetVar NA rezEveryone Heal ${If[${rezEveryone},FALSE,TRUE]} 
	/call echos switch "Rez Everyone" rezEveryone
/return

#event rezEnable "[MQ2] - Adjusting Resurrection/Call Casting!"
sub Event_rezEnable()
	/if (${Select[${Me.Class.ShortName},WAR,MNK,BST,ROG,ENC,WIZ,MAG,NEC,SHD,RNG,BRD]}) {
		/call Event_SetVar NA rezEnable Heal FALSE
		VOUT No rez for joo!
		/goto :exit
	}
	/call Event_SetVar NA rezEnable Heal ${If[${rezEnable},FALSE,TRUE]} 
	:exit
	/call echos switch "Automatic Casting if Resurrection/Call" rezEnable
/return

#event rezAuto "[MQ2] - Adjusting auto Rez/Calling!"
sub Event_rezAuto()
	/call Event_SetVar NA rezAuto Heal ${If[${rezAuto},FALSE,TRUE]} 
	/call echos switch "Automatic Rez" rezAuto
/return

#event setRIMode "[MQ2] - Adjusting Raid Invites!"
sub Event_setRIMode()
	/call Event_SetVar NA setRaidInvite Common ${If[${setRaidInvite},FALSE,TRUE]} 
	/call echos switch "Invites for Raids" setRaidInvite
/return

#event rezEmergency "[MQ2] - Adjusting Emergency Rez!"
sub Event_rezEmergency()
	/call Event_SetVar NA rezEmergency Heal ${If[${rezEmergency},FALSE,TRUE]} 
	/call echos switch "Emergency Rez Mode" rezEmergency
/return

#event setDragMode "[MQ2] - Setting Drag Mode!"
sub Event_setDragMode()
	/call Event_SetVar NA setDragMode Common ${If[${setDragMode},FALSE,TRUE]} 
	/call echos switch "Drag Mode" setDragMode
/return

#event setForageMode "[MQ2] - Setting Forage Mode!"
sub Event_setForageMode()
	/call Event_SetVar NA setForageMode Common ${If[${setForageMode},FALSE,TRUE]} 
	/call echos switch "Forage Mode" setForageMode
/return

#event setFishMode "[MQ2] - Setting Fish Mode!"
sub Event_setFishMode()
	/call Event_SetVar NA setFishMode Common ${If[${setFishMode},FALSE,TRUE]} 
	/call echos switch "Fish Mode" setFishMode
/return

#event setHuntMode "[MQ2] - Setting Hunter Mode!"
sub Event_setHuntMode()
	/call Event_SetVar NA setHunterMode Common ${If[${setHunterMode},FALSE,TRUE]} 
	/call echos hunter
/return

#event setPetFarmMode "[MQ2] - Setting Pet Farm Mode!"
sub Event_setPetFarmMode()
	/if (!${Select[${Me.Class.ShortName},MAG,ENC,NEC,BST]}) {
		VOUT My daughter says you're a nooblet.
		/goto :exit
	}
	/call Event_SetVar NA setPetFarmMode Common ${If[${setPetFarmMode},FALSE,TRUE]} 
	:exit
	/call echos switch "Pet Farm Mode" setPetFarmMode
/return

#event setSkillsMode "[MQ2] - Setting Train Skills Mode!"
sub Event_setSkillsMode()
	/call Event_SetVar NA setSkillsMode Common ${If[${setSkillsMode},FALSE,TRUE]} 
/return

#event HomeCamp "[MQ2] - Update Home Camp!"
sub Event_HomeCamp()
	/call Event_SetVar NA HomeSet Movement ${If[${HomeSet},FALSE,TRUE]}  
	/call set_home
	| /call echos switch "Home" HomeSet
/return

#event setPuller "[MQ2] - Adjusting Puller!"
sub Event_setPuller()
	/call Event_SetVar NA setPullMode Common ${If[${setPullMode},FALSE,TRUE]} 
	/if (${useSmartAssist1}) /call Event_SetVar NA useSmartAssist1 Common FALSE
	/if (${useSmartAssist2}) /call Event_SetVar NA useSmartAssist2 Common FALSE
	/if (${useSmartAssistG}) /call Event_SetVar NA useSmartAssistG Common FALSE 
	/call echos switch "Pull Mode" setPullMode
/return

#event setAgro "[MQ2] - Adjusting Agro!"
sub Event_setAgro()
	/call Event_SetVar NA setAgroMode Common ${If[${setAgroMode},FALSE,TRUE]}  
	/call echos switch "Agro Mode" setAgroMode
/return

#event checkOneOff "[MQ2] - Force OneOff Check!"
sub Event_checkOneOff()
	/call check1offBuffs
/return

|**
#event SetCuring "[MQ2] - Adjusting Curing!"
sub Event_SetCuring()
	/if (!${Bool[${Plugin[MQ2netbots]}]} || !${Bool[${Plugin[MQ2debuffs]}]}) {
		/if (!${Bool[${Plugin[MQ2netbots]}]}) VOUT you need the plugn ${cinfo}MQ2netbots\ax for curing
		/if (!${Bool[${Plugin[MQ2debuffs]}]}) VOUT you need the plugn ${cinfo}MQ2debuffs\ax for curing
		/call Event_SetVar NA useCure Cure FALSE  
		/call echos switch "Curing" useCure
	} else {
		/call Event_SetVar NA useCure Cure ${If[${useCure},FALSE,TRUE]}  
		/call echos switch "Curing" useCure
	}
/return
**|

#event takeInvites "[MQ2] - Setting Group / Raid Invite acceptance!"
sub Event_takeInvites()
	/call Event_SetVar NA switchInvites Common ${If[${switchInvites},FALSE,TRUE]}  
	/call echos switch "Auto Accept Invites" switchInvites
/return

#event banestrike "[MQ2] - Setting Banestrike!"
sub Event_banestrike()
	/call Event_SetVar NA switchBanestrike Common ${If[${switchBanestrike},FALSE,TRUE]}  
	/call echos switch "Use AA Banestrike" switchBanestrike
/return

#event buffself "[MQ2] - Set Self Buffing!"
sub Event_buffself()
	/call Event_SetVar NA BuffSelf Buffs ${If[${BuffSelf},FALSE,TRUE]}
	/call echos switch "Buff Self" BuffSelf
/return

#event buffitem "[MQ2] - Set Item Buffing!"
sub Event_buffitem()
	/call Event_SetVar NA BuffItem Common ${If[${BuffItem},FALSE,TRUE]}
	/call echos switch "Buff Item" BuffItem
/return

#event buffbc "[MQ2] - Set BC Buffing!"
sub Event_buffbc()
	/if (!${Bool[${Plugin[MQ2netbots]}]}) {
		VOUT you need the plugn ${cinfo}MQ2netbots\ax for curing
		/call Event_SetVar NA BuffBC Buffs FALSE
		/call echos switch "Buff BC" BuffBC
	} else {
		/call Event_SetVar NA BuffBC Buffs ${If[${BuffBC},FALSE,TRUE]}
		/call echos switch "Buff BC" BuffBC
	}
/return

#event buffcombat "[MQ2] - Set Combat Buffing!"
sub Event_buffcombat()
	/call Event_SetVar NA BuffCombat Buffs ${If[${BuffCombat},FALSE,TRUE]}
	/call echos switch "Buff Combat [range ${combatRadius}]" BuffCombat
/return

#event buffraid "[MQ2] - Set Raid Buffing!"
sub Event_buffraid()
	/call Event_SetVar NA BuffRaid Buffs ${If[${BuffRaid},FALSE,TRUE]}
	/call echos switch "Buff Raid" BuffRaid
/return

#event bufftell "[MQ2] - Set /tell for Buffing!"
sub Event_bufftell()
	/call Event_SetVar NA BuffTell Buffs ${If[${BuffTell},FALSE,TRUE]}
	/call echos switch "Buff Tell" BuffTell
/return

#event buffpet "[MQ2] - Set Pet Buffing!"
sub Event_buffpet()
	/if (${Me.Class.PetClass} || ${Select[${Me.Class.ShortName},ENC,DRU,SHM,SHD]}) /call Event_SetVar NA BuffPet Pet ${If[${BuffPet},FALSE,TRUE]}  
	/call echos switch "Buff Pet" BuffPet
/return

#event healself "[MQ2] - Set Self Healing!"
sub Event_healself()
	/if (${Select[${Me.Class.ShortName},DRU,CLR,SHM,PAL]}) {
		VOUT Heal Classes [CLR,DRU,SHM,PAL] use /healgroup. Not /healself.
		/call Event_SetVar NA switchHealSelf Heal FALSE 
		/call echos switch "Heal Self" switchHealSelf
	} else {
		/call Event_SetVar NA switchHealSelf Heal ${If[${switchHealSelf},FALSE,TRUE]}  
		/call echos switch "Heal Self" switchHealSelf
	}
/return

#event healgroup "[MQ2] - Set Group Healing!"
sub Event_healgroup()
	/call Event_SetVar NA switchHealGroup Heal ${If[${switchHealGroup},FALSE,TRUE]}  
	/call echos switch "Heal Group" switchHealGroup
/return

#event healxtarget "[MQ2] - Set XTarget Healing!"
sub Event_healxtarget()
	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,PAL]}) /call Event_SetVar NA switchHealXTarget Heal ${If[${switchHealXTarget},FALSE,TRUE]}
	/call echos switch "Heal XTarget" switchHealXTarget
/return

#event healPet "[MQ2] - Set Pet Healing!"
sub Event_healpet()
	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,MAG]}) /call Event_SetVar NA switchHealPet Pet ${If[${switchHealPet},FALSE,TRUE]}  
	/call echos switch "Heal Pet" switchHealPet
/return

#event healCure "[MQ2] - Set Curing!"
sub Event_healCure()
	/call Event_SetVar NA switchHealCure Heal ${If[${switchHealCure},FALSE,TRUE]}  
	/call echos switch "Curing" switchHealCure
/return

#event healAggressive "[MQ2] - Set Aggressive Healing!"
sub Event_healAggressive()
	/call Event_SetVar NA switchHealAggressive Heal ${If[${switchHealAggressive},FALSE,TRUE]}  
	/call echos switch "Aggressive Healing" switchHealAggressive
/return

#event itembuffs "[MQ2] - Check Item Buffs."
sub Event_itembuffs()
	/call check_buffs_item
/return

#event selfbuffs "[MQ2] - Check Self Buffs."
sub Event_selfbuffs()
	DEBUG ${cbug}Event_selfbuffs()\ax
	/call check_buffs_self
/return

#event petbuffs "[MQ2] - Check Pet Buffs."
sub Event_petbuffs()
	DEBUG ${cbug}Event_petbuffs()\ax
	/call check_pet_buffs
/return

#event buildpet "[MQ2] - Time for a new pet."
sub Event_buildpet()
	DEBUG ${cbug}Event_buildpet()\ax
	/call check_pet
/return

#event clean "[MQ2] - Clean this mess up."
sub Event_clean()
	DEBUG ${cbug}Event_clean()\ax
	/call sort_cursor TRUE
	| none is kind of counterproductive..
	| /if (${hideNone}) /hidecorpse none
	
	/if (${hideAll}) /hidecorpse all
	/if (${hideAlways}) /hidecorpse always
	/if (${hideLooted}) /hidecorpse looted
	/if (${hideAllButGroup}) /hidecorpse allbutgroup
	/if (${hideNPC}) /hidecorpse npc
	/if (${hideListed}) /hidecorpse listed
	/if (${hideListedUnlocked}) /hidecorpse listedunlocked  
/return

#event setCore "[MQ2] - Adjusting Core!"
sub Event_setCore()
	/call Event_SetVar NA AutoBot Common ${If[${AutoBot},FALSE,TRUE]}  
	/call set_stop TRUE
	/call echos switch "Automatic Mode" AutoBot
/return

#event setAutoShrink "[MQ2] - Adjusting Automatic Shrink!"
sub Event_setAutoShrink()
	/call Event_SetVar NA setAutoShrink Common ${If[${setAutoShrink},FALSE,TRUE]}  
	/call echos switch "Automatic Shrink" setAutoShrink
/return

#event setIgnoreLOS "[MQ2] - Adjusting LoS Requirements!"
sub Event_setIgnoreLOS()
	/call Event_SetVar NA setIgnoreLOS Movement ${If[${setIgnoreLOS},FALSE,TRUE]}  
	/call echos switch "Ignoring Line of Sight" setIgnoreLOS
	/if (${setIgnoreLOS}) NOUT ${owarning} ${sep} THIS IS ${cbad}NOT\ax ADVISED. You will have pathing issues.
/return	
 _amount 
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
		/goto :exit
	}

	/if (!${Window[InventoryWindow].Open}) /invoke ${Window[InventoryWindow].DoOpen}

	/declare _count 			int local 
	/declare _countloop 	int local 1

	:loopgivemorecash
		VOUT Giving ${_coin} stack ${sep} ${_countloop} of ${_stacks}
		/if (!${Window[InventoryWindow].Open}) /goto :exit
		/if (!${Target.ID} || ${Taret.ID} == (${Me.ID} || ${Pet.ID})) /goto :exit
		/delay 2s
		/for _count 1 to ${_stacks}
			ISDEAD
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
	:exit
/return 
 
 
  
|***
 *	DES: Tell buffing
 *	USE: /t uselesstoon IwantThisBuff
 *	INI: 
 *  NOTE:
 ***| 	
#event Talk "#1# tells you, '#2#'"
sub Event_Talk(string line, string _sender, string _chatText)
|**
	/declare _ignoreRelay		 	string 	local |${relayTells}|bank
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
	
	
	/if (${Bool[${relayTells}]} && !${_sender.Find[Banker]} && !${_sender.Find[${Me}]} && !${_sender.Find[${relayTells}]}) {
		/tell ${relayTells} ${_sender} told me: ${_chatText}
	}

	CHECKSAFELIST

	| Tells for buffs.
	/if (${BuffTell} && ${Spawn[${_sender}].ID} && ${_sender.NotEqual[${Me.DisplayName}]}) {
    /call cast_buffs_queue "${_sender}" "${_chatText}"
  }
  
	| Remote kill execution. CHANGE THE CORE.INI
  /if (${Bool[${remoteKillKey}]} && ${_chatText.Equal[${remoteKillKey}]}) {
  	/call set_stop
  	NOUT ${owarning} ${sep} Recieved tell from ${cpc}${_sender}\ax to end current macro.
  	/endm
  }
  
  | take a tell to reform your group.
  /if (${_chatText.Equal[formgroup]}) /call set_crew main
  /if (${_chatText.Equal[disbandgroup]}) /bcaa //disband
  
  :exit
/return



|***
 *	DES: will reset the majority of core to its defaults.
 *	USE: 
 *	INI: 
 *  NOTE: this needs updating
 ***|
sub Event_resetcore(string line)
	/call Event_SetVar NA HealOverride Common 0
	/call Event_SetVar NA autoRest Common 90
	/call Event_SetVar NA AutoBot Common FALSE
	/call Event_SetVar NA setDragMode Common FALSE
	/call Event_SetVar NA setForageMode Common FALSE
	/call Event_SetVar NA setFishMode Common FALSE
	/call Event_SetVar NA setSkillsMode Common FALSE
	
 	/call Event_SetVar NA useSmartAssist1 Common FALSE
	/call Event_SetVar NA useSmartAssist2 Common FALSE
	/call Event_SetVar NA useSmartAssistG Common FALSE
	/call Event_SetVar NA engageAt Common 98
	/call Event_SetVar NA smartengageAt Common 98
	/if (${usePet}) /call Event_SetVar NA sendPetPerc Pet 98
	/if (${usePet}) /call Event_SetVar NA PetTank Pet FALSE
	/call Event_SetVar NA MA1 Common FALSE
	/call Event_SetVar NA MA2 Common FALSE
	/call Event_SetVar NA SetTie Movement FALSE 
	/call Event_SetVar NA SetTieToon Movement FALSE 
/return

#event setTieMode "[MQ2] - Set Tie!"
sub Event_setTieMode()
	/if (!${Bool[${SetTieToon}]}) {
		NOUT ${owarning} You need to set someone to be close too ${cinfo}/tietoon\ax [NAME].
		/goto :exit
	}
	/call Event_SetVar NA SetTie Movement ${If[${SetTie},FALSE,TRUE]} 
	/if (${Bool[${SetTie}]}) /call set_tie
	/if (!${Bool[${SetTie}]}) /stick off
	/call echos switch "Tie to ${cpc}${SetTieToon}" SetTie TRUE

	:exit
/return

#event setStickBehind "[MQ2] - Set Stick Behind!"
sub Event_SetStickBehind()
	/call Event_SetVar NA SetStickBehind Movement ${If[${SetStickBehind},FALSE,TRUE]}  
	/call echos switch "Stick behind target"  SetStickBehind

	| disable mezzing with stickbehind enabled.
	/if (${switchdeBuffMezz} && ${SetStickBehind}) {
		/call Event_SetVar NA switchdeBuffMezz Common FALSE  
		/call echos switch "deBuff  Mezz" switchdeBuffMezz
	}
/return

#event setFaceFast "[MQ2] - Set Face Fast!"
sub Event_SetFaceFast()
	/call Event_SetVar NA SetFaceFast Movement ${If[${SetFaceFast},FALSE,TRUE]}  
	/call echos switch "Face Fast"  SetFaceFast
	/if (${SetFaceFast}) VOUT ${cbad}WARNING\ax ${sep} This is a MQ2 Dead Giveaway. Use with caution.
/return

#event smartma "[MQ2] - Adjusting Smart Assist !!"
sub Event_smartma(string line)
	DEBUG ${cbug}Event_smartma()\ax
	/if (!${Bool[${Raid.Members}]} && !${Bool[${Group.Members}]}) {
		VOUT Try joining some people.
 		/call Event_SetVar NA useSmartAssist1 Common FALSE
		/call Event_SetVar NA useSmartAssist2 Common FALSE
		/call Event_SetVar NA useSmartAssistG Common FALSE
		/goto :exit
	} else /if (!${Bool[${Raid.Members}]} && ${Bool[${Group.Members}]} && !${useSmartAssistG}) {
 		/call Event_SetVar NA useSmartAssist1 Common FALSE
		/call Event_SetVar NA useSmartAssist2 Common FALSE
		/call Event_SetVar NA useSmartAssistG Common TRUE
		/goto :exit
	}
	/call clear_combat
	/call Event_SetVar NA setPullMode Common FALSE
	/if (!${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG} && ${Bool[${Raid.Members}]}) {
 		/call Event_SetVar NA useSmartAssist1 Common TRUE
		/call Event_SetVar NA useSmartAssist2 Common FALSE
		/call Event_SetVar NA useSmartAssistG Common FALSE
		VOUT Assisting Raid Assist 1
	} else /if (${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG} && ${Bool[${Raid.Members}]}) {
 		/call Event_SetVar NA useSmartAssist1 Common FALSE
		/call Event_SetVar NA useSmartAssist2 Common TRUE
		/call Event_SetVar NA useSmartAssistG Common FALSE
		VOUT Assisting Raid Assist 2
	} else /if (!${useSmartAssist1} && ${useSmartAssist2} && !${useSmartAssistG}) {
 		/call Event_SetVar NA useSmartAssist1 Common FALSE
		/call Event_SetVar NA useSmartAssist2 Common FALSE
		/call Event_SetVar NA useSmartAssistG Common TRUE
		VOUT Assisting Group Assist
	} else /if (!${useSmartAssist1} && !${useSmartAssist2} && ${useSmartAssistG}) {
 		/call Event_SetVar NA useSmartAssist1 Common FALSE
		/call Event_SetVar NA useSmartAssist2 Common FALSE
		/call Event_SetVar NA useSmartAssistG Common FALSE
		VOUT Exiting SmartAssist Mode
	}
	:exit
/return

#event SendPets "[MQ2] - SendPets: #1#"
sub Event_SendPets(string line, int _tmpID)
	DEBUG ${cbug}Event_SendPets()\ax
	/if (${usePet} && ${Pet.ID}) {
		VOUT Sending ${cpc}${Pet.DisplayName}\ax
		/target	ID ${_tmpID}
		/pet attack
	}
	:exit
/return

#event deBuffMezz "[MQ2] - Set deBuff Mezzing!"
sub Event_deBuffMezz()
	/call Event_SetVar NA switchdeBuffMezz Common ${If[${switchdeBuffMezz},FALSE,TRUE]}  
	/call echos switch "deBuff  Mezz" switchdeBuffMezz

	| clear stickbehind. it will force bad targeting fot the mezzer.
	/if (${SetStickBehind} && ${switchdeBuffMezz}) {
		/call Event_SetVar NA SetStickBehind Movement FALSE  
		/call echos switch "Stick behind target"  SetStickBehind
	}
/return

#event deBuffMalo "[MQ2] - Set deBuff Malo!"
sub Event_deBuffMalo()
	/call Event_SetVar NA switchdeBuffMalo Common ${If[${switchdeBuffMalo},FALSE,TRUE]}  
	/call echos switch "deBuff  Malo" switchdeBuffMalo
/return

#event deBuffTash "[MQ2] - Set deBuff Tash!"
sub Event_deBuffTash()
	/call Event_SetVar NA switchdeBuffTash Common ${If[${switchdeBuffTash},FALSE,TRUE]}  
	/call echos switch "deBuff  Tash" switchdeBuffTash
/return

#event deBuffCripple "[MQ2] - Set deBuff Cripple!"
sub Event_deBuffCripple()
	/call Event_SetVar NA switchdeBuffCripple Common ${If[${switchdeBuffCripple},FALSE,TRUE]}  
	/call echos switch "deBuff  Cripple" switchdeBuffCripple
/return

#event deBuffSlow "[MQ2] - Set deBuff Slow!"
sub Event_deBuffSlow()
	/call Event_SetVar NA switchdeBuffSlow Common ${If[${switchdeBuffSlow},FALSE,TRUE]}  
	/call echos switch "deBuff Slow" switchdeBuffSlow
/return

#event deBuffEradicate "[MQ2] - Set deBuff Eradicate!"
sub Event_deBuffEradicate()
	/if (${Select[${Me.Class.ShortName},ENC,MAG,NEC,WIZ,RNG,DRU]}) /call Event_SetVar NA switchdeBuffEradicate Common ${If[${switchdeBuffEradicate},FALSE,TRUE]}  
	/call echos switch "deBuff Eradicate" switchdeBuffEradicate
/return
	
#event deBuffSnare "[MQ2] - Set Snare!"
sub Event_deBuffSnare()
	/if (${AASnareList.Find[${Me.Class.ShortName}]}) {
		/call Event_SetVar NA switchdeBuffSnare Common ${If[${switchdeBuffSnare},FALSE,TRUE]}  
	} else {
		VOUT ${onotice} ${sep} Not an AA snaring class. Get bent.
		/call Event_SetVar NA switchdeBuffSnare Common FALSE  
	}
	/call echos switch "deBuff Snare" switchdeBuffSnare
/return
	
#event toastauto "[MQ2] - Adjusting Auto Toast on Named!"
sub Event_toastauto()
	/call Event_SetVar NA AutoToast Common ${If[${AutoToast},FALSE,TRUE]}  
	/call echos switch "Auto Toast" AutoToast
/return

#event setCombatHere "[MQ2] - Set Combat Here!"
sub Event_setCombatHere()
	/call set_combat_here
/return

#event setMount "[MQ2] - Adjusting Mount!"
sub Event_setMount()

/return





| Loot Events

#Event CantLoot "#*#may not loot this corpse#*#"
sub Event_CantLoot
   DEBUG Event_CantLoot Can't loot ${Target.DisplayName} ${Target.ID}
   /varset CantLootID ${Target.ID}
	:exit
/return

#event EditIniItem "[MQ2] - Set Loot Item #1#"
sub Event_EditIniItem(string line, _action)
	/if (${_action.Equal[What]} && ${Cursor.ID}) {
		VOUT ${Cursor.Name} is set to: ${cinfo}${Ini[${INILoot},${Cursor.Name.Left[1]},${Cursor.Name}]}\ax
		/goto :exit
	}

	/if (!${Cursor.ID}) {
		/call echos needcursor
	} else /if (${Cursor.ID}) {
		VOUT Loot INI: ${cinfo}${Cursor.Name}\ax -> ${_action}
		/ini "${INILoot}" "${Cursor.Name.Left[1]}" "${Cursor.Name}" "${_action}"
		/if (${_action.Equal[Destroy]}) /destroy
	}
	:exit
/return

#event NullSlot "#*#Invalid item slot 'null#*#"
sub Event_NullSlot
	VOUT ${owarning}: Opening all bags to correct null slot
	/keypress OPEN_INV_BAGS
	/timed 10 /keypress CLOSE_INV_BAGS
/return



|***
 *	DES: zephyr/TL requesting toon.
 *	USE: /t [toon] send [location]
 *	INI: 
 *  NOTE: 
 ***|
#event send 			"#1# tells you, 'send #2#'"
sub Event_send(string line, string _sender, string _location)
	CHECKSAFELIST
	/if (!${Select[${Me.Class.ShortName},DRU,WIZ]} || ${Spawn[pc ${_sender}].Distance} > 125) /goto :exit
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
			/varset _portList[2] Antonica: northkarna, commonlands, nektulos, qeyenos, northro, cazic, undershore, westkarana
			/varset _portList[3] Discord: wallofslaughter, bloodfields
			/varset _portList[4] Faydwer: fay, dragonscale
			/varset _portList[5] Kunark: lceanium, dreadlands
			/varset _portList[6] Luclin: dawnshroud, twilight, grimling
			/varset _portList[7] Misc: bind, nexus, primary, secondary, primarypush, secondarypush
			/varset _portList[8] Odus: tempesttemple, katta
			/varset _portList[9] Serpent's Spine: tox, stonebrunt, icefall, sunderdock, blightfire
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
	:exit
/return



|***
 *	DES: Used whenever we zone. Preps toon for mob lists, movement, etc.
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***| 
#event Zone "LOADING, PLEASE WAIT..."
sub Event_Zone()
	/call echos zone
	/call check_zone_INI
	/if (${setToastMode}) /call Event_SetVar NA setToastMode Common FALSE
	/makecamp off
	/if (${HomeSet}) /call Event_SetVar NA HomeSet Movement FALSE
	/if (${setDragMode}) /call Event_SetVar NA setDragMode Common FALSE 
	/if (${setForageMode}) /call Event_SetVar NA setForageMode Common FALSE 
	/if (${setFishMode}) /call Event_SetVar NA setFishMode Common FALSE 
	/if (${setSkillsMode}) /call Event_SetVar NA setSkillsMode Common FALSE 
	/if (${setPetFarmMode}) /call Event_SetVar NA setPetFarmMode Common FALSE 
	/if (${AutoBot} && ${setAutoShrink}) /call check_shrink
	
	/call create_timer timer_zone_pause ${tZone_Pause}
 	:exit
/return 



|***
 *	DES: Death checks by event
 *	USE: 
 *	INI: 
 *  NOTE: we force this event when recieving calls to corpse so the toon will still accept the next rez/
 ***| 
| #event markerDead "#*#Returning to Bind Location#*#"
| #event markerDead "You have been slain by#*#"
| #event markerDead "You died."
sub Event_markerDead()
	/varset markerDead TRUE
	/doevents flush
	:exit
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
		NOUT Need an item to buy, in single qoutes.
		/goto :exit
	}
	/if (${_buyAmount} < 1) {
		NOUT Need a valid quantity. 
		/goto :exit
	} 
	/call vendor_buy "${_itemtoBuy}" ${_buyAmount}
	:exit
/return



|***
 *	DES: Buy things from vendors
 *	USE: /echo Sell: 'itemname' or loot
 *	INI: 
 *  NOTE: single quotes.. yes, the item name has to be in single quotes
 ***| 
#event sellstuff	"[MQ2] Sell: '#1#'"
sub Event_sellstuff(string line, string _itemtoSell)
	DEBUG (string:${_itemtoSell})
	/call vendor_sell "${_itemtoSell}"
	:exit
/return


