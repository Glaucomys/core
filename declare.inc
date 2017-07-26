|***
 *	declare.inc
 *	Core Declares common and class specific.
 *	All manual forced events.
 *	All created Alias' and /bc commands
 *	All INI edits
 *	most of the #bind(s)
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

	/declare versionCore				string	outer 3.7.0
	/declare versionCLR 				string 	outer 2.1.3
	/declare versionDRU 				string 	outer 2.0.1 
	/declare versionSHM 				string 	outer 1.5.1
	/declare versionWAR 				string 	outer 1.5.1 
	/declare versionPAL 				string 	outer 1.0.2 
	/declare versionSHD 				string 	outer 1.9.1 
	/declare versionWIZ 				string 	outer 1.8.0 
	/declare versionENC 				string 	outer 1.4.1
	/declare versionMAG					string 	outer 2.0.1
	/declare versionNEC 				string 	outer 0.5.1 
	/declare versionRNG 				string 	outer 2.2.2
	/declare versionBST 				string 	outer 1.0.1 
	/declare versionBRD 				string 	outer 1.5.1 
	/declare versionBER 				string 	outer 1.6.1
	/declare versionMNK 				string 	outer 0.5.0 
	/declare versionROG 				string 	outer 1.5.1 
/return



|***
 *	DES: Used to remove stale entries from the toons INI. 'cause I gotta keep changing shits.
 *	USE: /call fix_INI_declares
 *	INI: 
 *	NOTE: may look a little strange, but I leave the old variables here for history. Just in case.
 ***|
sub fix_INI_declares()
	/ini "${INIFile}" "Common" "SetModeInvis" NULL
	/ini "${INIFile}" "Common" "SetScorchedEarthMode" NULL
	/ini "${INIFile}" "Common" "SetSkillsMode" NULL
	/ini "${INIFile}" "Common" "SetFishMode" NULL
	/ini "${INIFile}" "Common" "SetForageMode" NULL
	/ini "${INIFile}" "Common" "SetDragMode" NULL
	/ini "${INIFile}" "Common" "Looter" NULL
	/ini "${INIFile}" "Common" "ForageFishInvis" NULL
	/ini "${INIFile}" "Common" "SetHunterMode" NULL
	/ini "${INIFile}" "Common" "SetHunterAgro" NULL
	/ini "${INIFile}" "Common" "SetPetFarmMode" NULL
	
	| mage
	/ini "${INIFile}" "SelfBuff" "ElementalForm" NULL
	/ini "${INIFile}" "SelfBuff" "ElementalFormCheck" NULL
	
	| timer	
	/ini "${INICore}" "Timer" "tPetFarm_delay" NULL
	
	


/return



|***
 *	DES: adjust the related ini
 *	USE: /call Event_SetVar NA [variable] [INISection] [value]
 *	INI: 
 *  NOTE: 
 ***|
#event SetVar 			"[MQ2] - Core: #1# -> [#2#] -> #3#"
sub Event_SetVar(string line, string _varName, string _section, string _value)
	/if (${Select[${_value},TRUE,ON]}) /varset _value TRUE
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
		/varset ${_name} ${_value}
		DEBUG Setting ${_name} ${_type} ${_scope} "${_value}" 
	} else /if (${Defined[${_name}]}) {
		DEBUG DEFINED/Setting:${_name} VALUE:${_value}
		/if (${_value.NotEqual[NULL]}) /varset ${_name} ${_value}
	}
/return



|* Start base INI controls *********************************************************************************************

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
			VOUT Adding ${ctar}${Target.DisplayName}\ax to ${_list}List
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




|* End base INI controls ***********************************************************************************************



|***
 *	DES: Base variable inits.
 *	USE: 
 *	INI: core.ini
 *  NOTE: 
 ***|
sub declare_base()
	DEBUG ${cbug}base.Environment()\ax
	/call DeclareVar core													bool	outer TRUE
	/call DeclareVar _count												int		local
		
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
 	/call DeclareCoreIniVar ocombat							string	Output "\amCombat\ax"	
 	
 	
	| /call DeclareCoreIniVar CastRadius					int			Map 0	
	| /call DeclareCoreIniVar CastRadiusColor			string	Map 0 255 0
	

 	
		
	
	| SET THIS TO TRUE TO ENABLE DEBUGING. ALTERNATIVE USE /varset debug TRUE
	| on a funny note: prepare your ass for spam
	/call DeclareVar debug											bool		outer FALSE

	/call DeclareIniVar verbose									bool		Common TRUE
	/call DeclareCoreIniVar EQBCServer					string	General 127.0.0.1
	/call DeclareCoreIniVar EQBCPort						string	General 2112
	/call DeclareCoreIniVar mqlog								string	General FALSE
	/call DeclareCoreIniVar HudName							string	General Core
	/call DeclareCoreIniVar HudType							string	General normal
	/call DeclareCoreIniVar minMobPullLvl				int			General 1
	/call DeclareCoreIniVar maxMobPullLvl				int			General 150
	/call DeclareCoreIniVar combatRadius				string	General 30
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
	/call DeclareCoreIniVar queueCountMax																			int			General 10
	/call DeclareVar queueCount             					 												int  		outer 0
	/if (!${Defined[queueBuffID]}) /declare queueBuffID[8] 										int  		outer
	/if (!${Defined[queueList]}) /declare queueList[${queueCountMax},2] 		 		int  		outer
	
	
	/call DeclareVar status_check_string								string	outer NULL
	
	/call DeclareCoreIniVar main								string	Group |FALSE|FALSE
	/call DeclareCoreIniVar GroupLeader					string	Group FALSE
	/call DeclareCoreIniVar GroupTank						string	Group FALSE
	/call DeclareCoreIniVar GroupAssist					string	Group FALSE
	/call DeclareCoreIniVar GroupPuller					string	Group FALSE
	/call DeclareCoreIniVar GroupLooter					string	Group FALSE
	/call DeclareCoreIniVar GroupMark						string	Group FALSE


	| /here settings
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
	/call DeclareCoreIniVar hereZRadius				int			Here	30
	
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
	/call DeclareVar noInvis 										bool 		outer TRUE
	/call DeclareVar noFeigning 								bool 		outer TRUE
	/call DeclareVar Target2Kill								int			outer 0
	/call DeclareVar UseRange										bool		outer FALSE
	/call DeclareVar currentLootCount						int			outer	0
	/call DeclareVar markerDead									bool		outer FALSE
	
	| non-standard mode control
	/call DeclareVar modeTypes									string	outer "stop|fish|forage|drag|petfarm|scorch|hunt"
	/call DeclareVar setMode										bool		outer FALSE
	/call DeclareVar setModeType								string	outer FALSE
	/call DeclareIniVar setModeAgro							int			Common FALSE 
	/call DeclareVar setModeDrag								bool		outer FALSE 
	/call DeclareVar setModeForage							bool		outer FALSE
	/call DeclareVar setModePetfarm							bool		outer FALSE
	/call DeclareVar setModeScorch							bool		outer FALSE
	/call DeclareVar setModeHunt								bool		outer FALSE
	/call DeclareVar setModeFish								bool		outer FALSE
	/call DeclareVar setModeSkill								bool		outer FALSE
	
	
	
	/call DeclareVar isValidTarget							bool		outer FALSE
	/call DeclareVar combatMode									string	outer NULL
	/call DeclareVar tmpMode										string	outer NULL
	/call DeclareVar hudWarning									string	outer	NULL
	/call DeclareVar hudNotice									string	outer	NULL	
	/call DeclareVar hudTarNotice								string	outer	NULL
	/call DeclareVar flagCallNeedRez						bool		outer FALSE

	/call DeclareVar setToastMode								bool 		outer FALSE	
	/call DeclareVar canCast										bool		outer	TRUE
	/call DeclareVar ignoreEngageAt							bool		outer FALSE
	/call DeclareVar tmpHealOverride						float 	outer 99.99	
	
	/call DeclareVar 	pre 											string 	outer ${Ini[${INICore},fakelink,pre]}
	/call DeclareVar 	post 											string 	outer ${Ini[${INICore},fakelink,post]}
	
	/call DeclareVar hudBroadcast								string	outer
	
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
	/call DeclareIniVar	tCheck_Tie							string	Timer	5
	/call DeclareIniVar	tCheck_Familiar					string	Timer	30s	
	/call DeclareIniVar	tCheck_Ammo							string	Timer	5s	
	/call DeclareIniVar	tCheck_snap_retry				string	Timer	15s	
	/call DeclareIniVar	tOdds_and_Ends					string	Timer	30m
	/call DeclareIniVar	tAuto_Control_Toon			string	Timer	10s
	/call DeclareIniVar	tAA_picker							string	Timer	5m
	/call DeclareIniVar	tWatch_Spawn						string	Timer	1s
	/call DeclareIniVar	tCast_Cure							string	Timer	30s
	/call DeclareIniVar	tZone_Pause							string	Timer	5s
	/call DeclareIniVar	tHunt_delay							string	Timer	30s	
	/call DeclareIniVar	tCheck_Runes						string	Timer	10s
	/call DeclareIniVar	tScorchedEarth_delay		string	Timer	5s	
	/call DeclareIniVar	tScorchedEarth_fail			string	Timer	1m	
	/call DeclareIniVar	tScorchedEarth_ignore		string	Timer	30s	


	| mod rod recovered addicts
	/if (!${Select[${Me.Class.ShortName},MAG,WAR,ROG,BRD,MNK,BER]}) {
		/call DeclareIniVar	tAsk_for_rod						string	Timer	1m
	}
	/call DeclareCoreIniVar	tCheck_status				string	Timer	2s	
	/call DeclareIniVar	tPetfarm_delay					string	Timer	1s	

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
	/call DeclareIniVar whentoToast							int 		Common 80
	/call DeclareIniVar AutoToast								bool 		Common FALSE 
	/call DeclareIniVar MA1			 								string 	Common FALSE /ma1
	/call DeclareIniVar MA2			 								string 	Common FALSE /ma2
	
	/call DeclareIniVar MobAgro									int 		Common 10
	/call DeclareIniVar engageAt 								int 		Common 98
	/call DeclareIniVar smartengageAt 					int 		Common 98
	/call DeclareIniVar autoRest 								int 		Common 90

	/call DeclareIniVar noCastWith							string	Common "|Quick Defense|Protection of Bayle|Interrogation Intimidation|Energy Charge|Cloud of Terror|Faith"
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
	/call DeclareIniVar IllusionItem						string	Common FALSE
	/call DeclareIniVar IllusionBuff						string	Common FALSE
	/call DeclareIniVar BuffItem	 							bool		Common TRUE 
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
	/call DeclareIniVar switchHealSelf					bool 		Heal FALSE
	
	
	/call DeclareIniVar AutoExpAdjust 					bool 		Common TRUE /autoadjustexp
	/call DeclareIniVar MaintExpLvl   					int 		Common 99 /maintexplvl
	/call DeclareIniVar MaxLevel      					int 		Common 105 /maxlevel
	/call DeclareIniVar switchBanestrike				bool		Common FALSE		
	/call DeclareIniVar RangeItem								string	Common FALSE		
	/call DeclareIniVar RangeItemSummon					string	Common FALSE
	/call DeclareIniVar autoInventoryItem				string	Common "|Summoned: Giant Modulation Shard"
	/call DeclareIniVar stoponDS								bool		Common TRUE		
	/call DeclareIniVar useGlyph								string	Common "Glyph of the Cataclysm"	/glyph
	/call DeclareIniVar toastMobCount						int			Common 10
	/call DeclareIniVar ResidentMage						string	Common |FALSE|FALSE
	/call DeclareIniVar ClassChannel						int			Common FALSE	
	/call DeclareIniVar setBroadcast						string	Common TRUE	
	/call DeclareIniVar relayTells							string	Common FALSE /relay	
	/call DeclareIniVar zradius									int			Common 50	
	
	| watch spawn stuff
	/call DeclareVar hudwatchspawn											string	outer FALSE
	/call DeclareVar watchSpawnCount										int			outer	FALSE
	/call DeclareIniVar watchSpawn							string	Common FALSE 
	

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
	/call DeclareIniVar CombatDistance					int			Movement 15
	/call DeclareVar tempHomeSet								bool		outer FALSE	
	/call DeclareVar tempsnapstuck							bool		outer	FALSE
  /call DeclareIniVar setIgnoreLOS						bool		Movement FALSE
	/call DeclareIniVar SetTieToon 							string 	Movement FALSE
	/call DeclareIniVar SetTie       						bool 		Movement FALSE
	/call DeclareIniVar SetTieDistance 					int 		Movement 15 
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
		/call DeclareVar flagSoS									bool	outer FALSE
		/call DeclareIniVar backstab							bool		meleedisc FALSE
	}
	/if (${Select[${Me.Class.ShortName},BRD]}) {
		/call DeclareIniVar seloskick							bool		meleedisc FALSE
	}
	/if (${Select[${Me.Class.ShortName},MNK]}) {
		/call DeclareIniVar dragonpunch						bool		meleedisc FALSE
		/call DeclareIniVar eaglestrike						bool		meleedisc FALSE
		/call DeclareIniVar tigerclaw							bool		meleedisc FALSE
		/call DeclareIniVar flyingkick						bool		meleedisc FALSE
		/call DeclareIniVar roundkick							bool		meleedisc FALSE
	}
	
	/if (${Me.Class.PetClass} || ${Select[${Me.Class.ShortName},ENC,DRU,SHM,SHD,CLR,WIZ,RNG,BRD,BST]}) {
		/call DeclareIniVar sendPetPerc 					int			Pet 99
		/call DeclareIniVar usePet 								bool 		Pet FALSE
		/call DeclareIniVar sendPetSwarmPerc			int			Pet 99
		/call DeclareIniVar usePetSwarm						bool 		Pet FALSE
		/call DeclareIniVar switchHealPet					bool 		Pet FALSE
	}
	
	/call DeclareIniVar sumFamiliar							string	Pet FALSE
	/call DeclareIniVar sumFamiliarBuff					string	Pet FALSE
	/call DeclareIniVar sumFamiliarLeave				bool		Pet TRUE


	| food/drink
	/call DeclareIniVar clickFoodDispenser			string 	Meal FALSE
	/call DeclareIniVar clickFoodItem						string 	Meal FALSE
	/call DeclareIniVar clickFoodCount					int		 	Meal 0
	/call DeclareIniVar clickDrinkDispenser			string 	Meal FALSE
	/call DeclareIniVar clickDrinkItem					string 	Meal FALSE
	/call DeclareIniVar clickDrinkCount					int		 	Meal 0
	/call DeclareIniVar tClick_FoodDrink				string	Timer 10m





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
		/call DeclareVar callID										int 		outer

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
		/call DeclareIniVar GoMpoint 							int 		Heal 80	
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
	/call DeclareIniVar HuntNuke								string 	Spells "Cloudburst Stormstrike"
	
	
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
	
	/call DeclareIniVar runeMagiWard	 					string	Rune "Doomscale Husk"
	/call DeclareIniVar runeShieldofFate				string	Rune "Shield of Consequence"
	/call DeclareIniVar runeCrystalwing					string	Rune "Armor of the Codex"
	/call DeclareIniVar runeGuard			 					string	Rune "Darkmist Guard"
	
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
	/call DeclareVar mobList string outer
	/call DeclareVar addList string outer
	| /if (!${Defined[aggroAnim]}) /declare aggroAnim string outer |5|8|12|17|18|42|44|80|106|129|144

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
		
	/call DeclareVar set												int			outer 1
	/call DeclareVar gem												int			outer 1
	
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
	/call DeclareVar mobAECount									int	outer
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
	/call DeclareIniVar	tcheck_MG								string	Timer	10s

	/call DeclareVar mobAECount									int			outer
	/call DeclareVar MajorHurt 									int 		outer 0
	/call DeclareVar WorstHurt 									string 	outer NULL
	/call DeclareVar WorstHurtID 								int 		outer
	/call DeclareVar WorstHP 										int 		outer 100
	/call DeclareVar WorstClass 								string 	outer NULL
	/call DeclareVar MinorHurt									int    	outer 0 
	
	/call DeclareIniVar useGroupHeal 						bool		Heal TRUE /usegroupheal
  /call DeclareIniVar healWholeGroup					string 	Heal "Wave of Grief"
  /call DeclareIniVar healTouch								string 	Heal "Ardent Touch"
  /call DeclareIniVar healPanic								string 	Heal FALSE
|	/call DeclareVar rezID 											int 		outer
	/call DeclareVar callID											int 		outer

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
	
	/call DeclareIniVar doYaulp 								string	SelfBuff TRUE /doyaulp	
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

	/call DeclareIniVar DoDoT						 				int			Spells 0
	/call DeclareIniVar DoT1					 					string	Spells FALSE
	/call DeclareIniVar DoT2					 					string	Spells FALSE
	/call DeclareIniVar DoT3					 					string	Spells FALSE
	/call DeclareIniVar DoT4					 					string	Spells FALSE


	/call DeclareIniVar ChallengeSpell					string 	Spells "Impose for Honor"
	/call DeclareIniVar ValiantDefense					string 	Spells "Valiant Defense"
	
	/call DeclareIniVar Twincast								string 	Spells "Glorious Exoneration"
	
	/call DeclareIniVar Harmonious 							string 	Spells "Harmonious Blessing"
	
	/call DeclareIniVar useMG										bool		SelfBuff TRUE
	/call DeclareIniVar useMGPct								int			SelfBuff 60

	
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
	/call DeclareIniVar DiscCombatFrenzy 				string 	Discs "Amplified Frenzy"
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

	/call DeclareVar OpenWoundEffect						string 	outer Open Wound
  /if (${OpenWoundDisc.Find[rk. III]}) {
		/varset OpenWoundEffect ${OpenWoundDisc.Left[-7]} effect III
	} else /if (${RaidBuffCast${x}.Find[rk. II]})  {
		/varset OpenWoundEffect ${OpenWoundDisc.Left[-6]} effect II
  }
	/call DeclareVar AugmentedFrenzyEffect 			string outer Augmented Frenzy Effect
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

	/call DeclareIniVar DoDoT						 				int			Spells 2
	/call DeclareIniVar DoT1					 					string	Spells "Blisterbeetle Swarm"
	/call DeclareIniVar DoT2					 					string	Spells "Vespid Swarm"
	/call DeclareIniVar DoT3					 					string	Spells FALSE
	/call DeclareIniVar DoT4					 					string	Spells FALSE
	/call DeclareIniVar DoTSwarmDS		 					string	Spells "Swarm of Glistenwings"
	/call DeclareIniVar DoTSwarmDSdeBuff 				string	Spells "Glistenwing Swarm"
	
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
	
	/call DeclareVar MajorHurt 									int 		outer 0
	/call DeclareVar WorstHurt 									string 	outer NULL
	/call DeclareVar WorstHurtID 								int 		outer
	/call DeclareVar WorstHP 										int 		outer 100
	/call DeclareVar WorstClass 								string 	outer NULL
  /call DeclareVar MinorHurt									int    	outer 0 
  /call DeclareVar WorstHp 										int    	outer 100
  /call DeclareVar pWorstHurt 								string 	outer ${Me.DisplayName}
  /call DeclareVar pWorstHP										int			outer 100
  /call DeclareVar pWorstHurtID								int 		outer ${Me.ID}
  /call DeclareVar pWorstClass								string	outer NULL
  /call DeclareVar pMajorHurt									int			outer 0
	
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
	/call DeclareVar MajorHurt 									int 		outer 0
	/call DeclareVar WorstHurt 									string 	outer NULL
	/call DeclareVar WorstHurtID 								int 		outer
	/call DeclareVar WorstHP 										int 		outer 100
	/call DeclareVar WorstClass 								string 	outer NULL
  /call DeclareVar MinorHurt									int    	outer 0 
  /call DeclareVar WorstHp 										int    	outer 100	
  
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

	/call DeclareVar HealList 									string 	outer |${healSurge}|${healWholeGroup}|${healWholeGroupHoT}|${healMain}|${healIntervention}|${healReckless}

/return	



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_BRD()
	/call DeclareVar mobList string outer
	/call DeclareVar addList string outer

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
	/call DeclareIniVar tClick_Poison						string	Timer 30s

	/call DeclareIniVar DiscRest				 				string 	Discs "Breather"
	/call DeclareIniVar DiscAggro 							string 	Discs "Disorientation"
	/call DeclareIniVar DiscAssault 						string 	Discs "Barrage"
	/call DeclareIniVar DiscBleed 							string 	Discs "Hack"	
	/call DeclareIniVar DiscPlay	 							string 	Discs "Knifeplay Discipline"	
	/call DeclareIniVar DiscPhantom							string 	Discs "Phantom Assassin"	
	/call DeclareIniVar DiscHack								string 	Discs "Jugular Hack"	
	/call DeclareIniVar DiscHiddenBlade					string 	Discs "Hidden Blade"	
	/call DeclareIniVar DiscBladePoison					string 	Discs "Reefcrawler Blade"	

	| disable for toast
	/call DeclareIniVar DiscVision 							string 	Discs "Thief's Vision"	

	| toast
	/call DeclareIniVar DiscPinPoint 						string 	Discs "Pinpoint Deficiencies"
	/call DeclareIniVar DiscMark 								string 	Discs "Wide-Eyed Mark"
	/call DeclareIniVar DiscEradicator 					string 	Discs "Executioner Discipline"
	/call DeclareIniVar DiscRazor 							string 	Discs "Razor's Edge Discipline"	
	/call DeclareIniVar DiscFrenzy 							string 	Discs "Frenzied Stabbing Discipline"	
	/call DeclareIniVar DiscTwistedChance				string 	Discs "Twisted Chance Discipline"
	
	/call DeclareIniVar clickPoisonBuff					string 	Poison "Bite of the Shissar Poison X"
	/call DeclareIniVar clickPoisonItem					string 	Poison "Consigned Bite of the Shissar XVIII"
	/call DeclareIniVar clickPoisonDispenser		string 	Poison "Selrach's Shadowscale Leggings"
	/call DeclareIniVar clickPoisonCount				int		 	Poison 60
	
	
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
	/call DeclareIniVar	tcheck_QM								string	Timer	10s
		
	/call DeclareVar MajorHurt 									int 		outer 0
	/call DeclareVar WorstHurt 									string 	outer NULL
	/call DeclareVar WorstHurtID 								int 		outer
	/call DeclareVar WorstHP 										int 		outer 100
	/call DeclareVar WorstClass 								string 	outer NULL
	/call DeclareVar MinorHurt									int    	outer 0 
	
	/call DeclareVar pWorstHurt 								string 	outer ${Me.DisplayName}
	/call DeclareVar pWorstHP										int			outer 100
	/call DeclareVar pWorstHurtID								int 		outer ${Me.ID}
	/call DeclareVar pWorstClass								string	outer NULL
	/call DeclareVar pMajorHurt									int			outer 0
	
	/call DeclareVar WorstManaID								int			outer
	/call DeclareVar WorstManaPct								int			outer
	
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
		
	/call DeclareIniVar doYaulp 								string	SelfBuff TRUE /doyaulp	
	/call DeclareIniVar sumWeaponSpell					string	SelfBuff "Hammer of	Reverence"
	/call DeclareIniVar sumWeaponItem						string	SelfBuff "Hammer of	Reverence III"
	/call DeclareIniVar useQM										bool		SelfBuff TRUE
	/call DeclareIniVar useQMPct								int			SelfBuff 40

	| /call DeclareIniVar Spire										string	Spells "Fundament: Third Spire of Divinity"
		
/return	



|***
 *	DES: This will clear all store alises that core has created in your MacroQuest2.INI file. or those with matching names.
 *	USE: /whitewash
 *	INI: MacroQuest2.INI
 *  NOTE: IF you instance/sandbox MQ/EQ it is best you run this on all toons at the same time. Sorry it's slow as fuck.
 ***|
#event whitewash "[MQ2] @@@ Core: Deleting all Core aliases.. PLEASE WAIT"
sub Event_whitewash()
	DEBUG ${cbug}Event_whitewash()\ax
	/call whitewash
/return

sub whitewash()
	NOUT ${owarning} ${sep} Deleting all Core ${cinfo}${versionCore}\ax aliases. ${cbad}Please wait.\ax
	/declare _count 				int local
	/declare _todelete[19] 	string local
	/declare _index 				int local
	/declare _increment			int local 1
	
	
	| commands to remove forever
	
	/varset _todelete[15] |healoverride|healself|healgroup|healxtarget|healpet|healcure|healaggressive|autoshrink|toast|companionswarm|companion|home|paragon|combatradius|HuntMobAgro
	/varset _todelete[16] |spice|stop|setagro|smartma|setpull|HUDTime|banestrike|broadcast|ib|sb|pb|resetcore|verbose|buildpet|autorest|toastauto|primary|secondary|invis
	/varset _todelete[17] |rezeveryone|rezcombat|rezauto|rezemergency|rezenable|core|invites|facefast|stickbehind|hudupdate|qmpct|qm|toastmobcount|toastengageat|combatdistance
	/varset _todelete[18] |debuffmezz|debuffslow|debufftash|debuffmalo|debuffcripple|debufferadicate|debuffsnare|checkcash|agro|engageat|autorest|smartengageat|watchspawn|petengageat|petswarmengageat
	/varset _todelete[19] |buffbc|bufftell|buffpet|buffitem|buffraid|buffraid|buffself|buffcombat|tie|ignorelos|forage|fish|train|petfarm|drag|ri|hunt|tietoon|tiedistance|tietime
	
		
	| misc HUD commnads
	/varset _todelete[1] |control|whitewash|smartengageat
	
	| assist
	/varset _todelete[2] |ma1|ma2|killmob

	| modes
	/varset _todelete[3] |combat|on|off

	| Misc commands
	/varset _todelete[4] |maxlevel|autoadjustexp|oneoff|userain|crew|info|huntagro|autologin
	/varset _todelete[5] |pathiosis|bcsay|blurb|clean|stack|stein|gtfo|door|echochannel|info|whohas
	/varset _todelete[6] |mirror|maintexplvl|autocontrol|killcode|echoaugments|relay|givecoin|stop

	| heal
	/varset _todelete[7] |usegrouphot|usegroupheal|rc|purify

	/varset _todelete[8] |NULL

	| loots
	/varset _todelete[9] |lootleave|lootkeep|lootdestroy|lootsell|buy|sell|lootwhat|lootannounce|lootignore

	| zone list control
	/varset _todelete[10]	|listremove|listnomezz|listnocast|listexclude|listnamed|listnomelee|listrange|listnomagic|listnodisease|listnofire|listnopoison|listnocold|listnocorruption|listhuntmob

	| pets
	/varset _todelete[11]	|pettype|pettank|sendpets

	| class commands
	/varset _todelete[12] |harvest|gather|rod|redorod|rods|cr|issuance|useeradicate|song
	/varset _todelete[13] |glyph|doyaulp|lesson|wood|cani

	| movement
	/varset _todelete[14]	|here|take|homeradius|homeleash

	
	/for _count 1 to ${_todelete.Size}
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



|* Created Events ******************************************************************************************************
  
|***
 *	DES: stuf we do cause we are lazy.
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub set_alias()
	DEBUG ${cbug}set_alias()\ax Setting Aliases
	/if (!${Bool[${Alias[/info]}]}) 					/squelch /alias /info 											NOUT Core Instructions:
	/if (!${Bool[${Alias[/take]}]}) 					/squelch /alias /take 											/bc take
	/if (!${Bool[${Alias[/door]}]}) 					/squelch /alias /door 											/bc door
	/if (!${Bool[${Alias[/stack]}]}) 					/squelch /alias /stack 											/bcaa //invis
	
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
   
  /if (!${Bool[${Alias[/sendpets]}]}) 			/squelch /alias /sendpets										NOUT SendPets: 
	/if (!${Bool[${Alias[/killmob]}]}) 				/squelch /alias /killmob 										/bc killmob
	/if (!${Bool[${Alias[/mirror]}]}) 				/squelch /alias /mirror 										/bc mirror
	/if (!${Bool[${Alias[/stein]}]}) 					/squelch /alias /stein 											/bc stein
  /if (!${Bool[${Alias[/stop]}]}) 					/squelch /alias /stop 											/bc stop
	/if (!${Bool[${Alias[/blurb]}]}) 					/squelch /alias /blurb 											NOUT Blurb: 
 	/if (!${Bool[${Alias[/clean]}]}) 					/squelch /alias /clean 											NOUT Clean this mess up.
 	/if (!${Bool[${Alias[/whitewash]}]}) 			/squelch /alias /whitewash 									/echo \ar@@@\ax Core: Deleting all Core aliases.. \arPLEASE WAIT\ax
	/if (!${Bool[${Alias[/song]}]}) 					/squelch /alias /song 											NOUT Song: 
	/if (!${Bool[${Alias[/crew]}]}) 					/squelch /alias /crew 											NOUT Crew: 
	/if (!${Bool[${Alias[/here]}]}) 					/squelch /alias /here 											/bc here
  /if (!${Bool[${Alias[/bcsay]}]}) 					/squelch /alias /bcsay 											/bc bcsay
  /if (!${Bool[${Alias[/echoaugments]}]}) 	/squelch /alias /echoaugments								NOUT Echo Augment file!
  /if (!${Bool[${Alias[/whohas]}]}) 				/squelch /alias /whohas											/bc whohas
  /if (!${Bool[${Alias[/autologin]}]}) 			/squelch /alias /autologin									/plugin mq2autologin unload

	/if (!${Bool[${Alias[/rc]}]}) 						/squelch /alias /rc 												/bc rc
	/if (!${Bool[${Alias[/purify]}]}) 				/squelch /alias /purify 										/bc purify
		
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

	/if (!${Bool[${Alias[/oneoff]}]}) 				/noparse /squelch /alias /oneoff   					/bc oneoff

	/if (!${Bool[${Alias[/gtfo]}]}) 					/noparse /squelch /alias /gtfo    			 		/bc gtfo
	/if (!${Bool[${Alias[/lesson]}]}) 				/noparse /squelch /alias /lesson   			 		/bc lesson

	/if (!${Bool[${Alias[/autocontrol]}]})		/noparse /squelch /alias /autocontrol				NOUT Setting Automatic Control Toon swapping!



	
	/if (${Select[${Me.Class.ShortName},CLR]}) {
		/if (!${Bool[${Alias[/cr]}]}) 				/squelch /alias /cr 											/bc cr
		| /if (!${Bool[${Alias[/issuance]}]}) 	/squelch /alias /issuance 								/bc issuance
	}
	/if (${Select[${Me.Class.ShortName},DRU]}) {
		/if (!${Bool[${Alias[/wood]}]}) 				/squelch /alias /wood 									/bc wood
	}
	/if (${Select[${Me.Class.ShortName},MAG]}) {
		/if (!${Bool[${Alias[/rods]}]}) 				/squelch /alias /rods 									/bc rods
	}
	/if (${Select[${Me.Class.ShortName},SHM]}) {
		/if (!${Bool[${Alias[/aid]}]}) 					/squelch /alias /aid 										/bc aid
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
	/if (${_cmd.Equal[stop]}) /call set_stop FALSE
  /if (${_cmd.Equal[setRaidInviteMode]}) /call setRaidInviteMode
  /if (${_cmd.Equal[killmob]}) {
  	/varset ignoreEngageAt ${If[${ignoreEngageAt},FALSE,TRUE]}
		/call echos switch "Ignoring Engage at %" ignoreEngageAt
  }
  /if (${_cmd.Equal[mirror]} && ${Me.ItemReady[Mirror Fragment of Anashti Sul]}) /useitem "Mirror Fragment of Anashti Sul"
  /if (${_cmd.Equal[stein]} && ${Me.ItemReady[Drunkard's Stein]}) /useitem "Drunkard's Stein"
  /if (${_cmd.Equal[oneoff]}) /call check1offBuffs
  | MGB Class AA casts
  /if (${_cmd.Equal[paragon]} && ${Select[${Me.Class.ShortName},BST]}) /call cast_AA_paragon
  /if (${_cmd.Equal[rods]} && ${Select[${Me.Class.ShortName},MAG]}) /call cast_rods
  /if (${_cmd.Equal[aid]} && ${Select[${Me.Class.ShortName},SHM]}) /call doAid
  /if (${_cmd.Equal[cr]} && ${Select[${Me.Class.ShortName},CLR]}) /call cast_celest_regen
  /if (${_cmd.Equal[wood]} && ${Select[${Me.Class.ShortName},DRU]}) /call cast_AA_spirit_wood
  | /if (${_cmd.Equal[issuance]} && ${Select[${Me.Class.ShortName},CLR]}) /call cast_issuance_heal
  /if (${_cmd.Equal[spice]} && ${Select[${Me.Class.ShortName},RNG]}) /call cast_AA_auspice
  
  /if (${_cmd.Equal[lesson]}) {
  	/if (${Me.AltAbilityReady[Lesson of the Devoted]}) /call core_cast "Lesson of the Devoted" alt 0
  	/goto :exit
  }
  
	/if (${_cmd.Equal[handin]}) /call hand_in "${_sender}" "${_verbage}"
  
  | see who has the item and report back
	/if (${_cmd.Equal[whohas]}) /call check_who_has "${_sender}" "${_verbage}"
 
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
   
    
	| commands to control all assist targets.  
  /if (${_cmd.Equal[sma1]} && !${setPullMode} && !${Select[${Me.Class.ShortName},CLR]}) {
		/smartma 1
  }
  /if (${_cmd.Equal[sma2]} && !${setPullMode} && !${Select[${Me.Class.ShortName},CLR]}) {

		/smartma 2
  }
  /if (${_cmd.Equal[smag]} && !${setPullMode} && !${Select[${Me.Class.ShortName},CLR]}) {
		/smartma G
	}
	/if (${_cmd.Equal[smax]}) {
		/smartma X
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
  		/call set_stop TRUE
  		/call clear_combat
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
/return

#event noRaid "Your raid was disbanded."
sub Event_noRaid()
 		/if (${useSmartAssist1}) /call Event_SetVar NA useSmartAssist1 Common FALSE
		/if (${useSmartAssist2}) /call Event_SetVar NA useSmartAssist2 Common FALSE
		/if (${setRaidInvite}) /call Event_SetVar NA setRaidInvite Common FALSE
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
	/if (${setRaidInvite}) {
		/if (${Bool[${Raid.Member[${_sender}]}]}) {
			VOUT ${cpc}${_sender}\ax is already in the raid.
			/goto :exit
		}
		/if (${invitelist.Find[${_sender}]}) {
			VOUT ${cbad}NO\ax invites for ${cpc}${_sender}\ax.
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



|* Forced Events ******************************************************************************************************* 
 
 
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


|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***| 	
#event autocontrol "[MQ2] - Setting Automatic Control Toon swapping!"
sub Event_autocontrol()
	/call Event_SetVar NA autoControl Common ${If[${autoControl},FALSE,TRUE]}
	/call echos switch "Auto Adjusting Control Toon" autoControl
/return



|***
 *	DES: 
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
	/if (${BuffTell} && ${Spawn[${_sender}].ID} && ${_sender.NotEqual[${Me.DisplayName}]}) {
    /call cast_buffs_queue "${_sender}" "${_chatText}"
  }
  
	| Remote kill execution. CHANGE THE CORE.INI
  /if (${Bool[${remoteKillKey}]} && ${_chatText.Equal[${remoteKillKey}]}) {
  	/call set_stop FALSE
  	/call clear_combat
  	NOUT ${owarning} ${sep} Recieved tell from ${cpc}${_sender}\ax to end current macro.
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
  
  :exit
/return



|***
 *	DES: 
 *	USE: /here or /bc here
 *	INI: 
 *  NOTE: 
 ***| 	
#event setCombatHere "[MQ2] - Set Combat Here!"
sub Event_setCombatHere()
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
	:exit
/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***| 	
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



|***
 *	DES: This is a cover all for the null slot issue when swapping gear.
 *	USE: NA
 *	INI: NA
 *  NOTE: 
 ***| 	
#event NullSlot "#*#Invalid item slot 'null#*#"
sub Event_NullSlot
	NOUT ${owarning} ${sep} Opening all bags to correct null slot
	/keypress OPEN_INV_BAGS
	/timed 10 /keypress CLOSE_INV_BAGS
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
	/call mode_clear_all
	/if (${AutoBot} && ${setAutoShrink}) /call check_shrink
	
	/call create_timer timer_zone_pause ${tZone_Pause}
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
	} else /if (${_buyAmount} < 1) {
		NOUT Need a valid quantity. 
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
	DEBUG (string:${_itemtoSell})
	/call vendor_sell "${_itemtoSell}"
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
	/if (${usePet} && ${Pet.ID}) {
		VOUT Sending ${cpc}${Pet.DisplayName}\ax
		/target	ID ${_tmpID}
		/pet attack
	}
/return



|***
 *	DES: 
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




|* #bind(s) ************************************************************************************************************

|***
 *	DES: forces the character into melee combat if there is a target.
 *	USE: /combatmelee
 *	INI: 
 *  NOTE: manual mode
 ***| 	
#bind combatmelee /combatmelee
sub Bind_combatmelee()
	/call sort_combatmelee
/return

sub sort_combatmelee()
	/if (${Target.ID}) {
		/varset isValidTarget TRUE
		/varset combat MELEE
		/varset Target2Kill ${Target.ID}
		VOUT ${ocombat} Melee ${sep} [${ctar}${Spawn[ID ${Target.ID}].DisplayName}\ax]
	} else {
		/varset combat FALSE
		NOUT ${onotice} ${sep} You need a target.
		/call echos needtarget
	}
/return



|***
 *	DES: 
 *	USE: /combatrange
 *	INI: 
 *  NOTE:
 ***| 	
#bind combatrange /combatrange
sub Bind_combatrange()
	/if (${Target.ID} && ${Target.Distance} > 30) {
		/varset isValidTarget TRUE
		/varset combat RANGE
		/varset Target2Kill ${Target.ID}	
		VOUT ${ocombat} Range ${sep} [${ctar}${Spawn[ID ${Target.ID}].DisplayName}\ax]
	} else /if (!${Target.ID} || ${Target.ID} == ${Me.ID} || ${Target.Distance} < 31) {
		/varset combat FALSE
		/call echos needtarget
	} 
/return



|***
 *	DES: 
 *	USE: /combatnuke
 *	INI: 
 *  NOTE:
 ***| 	
#bind combatnuke /combatnuke
sub Bind_combatnuke()
	/if (${Target.ID} && ${Target.ID} != ${Me.ID} && ${Target.ID} != ${Me.Pet.ID}) {
		/varset isValidTarget TRUE
		/varset combat NUKE
		/varset Target2Kill ${Target.ID}
		VOUT ${ocombat} Nuke ${sep} [${ctar}${Spawn[ID ${Target.ID}].DisplayName}\ax]
	} else 	/if (!${Target.ID} || ${Target.ID} == ${Me.ID} || ${Target.ID} == ${Me.Pet.ID}) {
		/varset combat FALSE
		/call echos needtarget
	} 
/return



|***
 *	DES: 
 *	USE: /hudtime
 *	INI: 
 *  NOTE:
 ***| 	
#bind HUDTime /hudtime
sub Bind_HUDTime(string line)
	/varset HUDTime ${If[${HUDTime},FALSE,TRUE]}
	/if (${HUDTime}) VOUT Showing ${cinfo}Real Life\ax time.
	/if (!${HUDTime}) VOUT Showing ${cinfo}In Game\ax time.
/return



|***
 *	DES: 
 *	USE: /hudbroadcast
 *	INI: 
 *  NOTE:
 ***| 	
#bind setBroadcast /hudbroadcast
sub Bind_setBroadcast()
	/call Event_SetVar NA setBroadcast Common ${If[${Bool[${setBroadcast}]},FALSE,TRUE]}
	/if (${setBroadcast.Equal[FALSE]}) /ini "${INICore}" "transmit_status" "${Me.DisplayName}" "DISABLED"
	/if (${setBroadcast.Equal[TRUE]}) /ini "${INICore}" "transmit_status" "${Me.DisplayName}" "${AutoBot}"
	/call echos switch "Adjusting Broadcast Status" setBroadcast
/return



|***
 *	DES: 
 *	USE: /companionpet
 *	INI: 
 *  NOTE:
 ***| 	
#bind companion /companionpet
sub Bind_companion()
	/call Event_SetVar NA usePet Pet ${If[${usePet},FALSE,TRUE]}
	/call echos switch "Use Pet" usePet
	/if (!${usePet} && ${Pet.ID}) {
		/if (${Me.AltAbilityReady[Suspended Minion]}) {
			/call core_cast "Suspended Minion" alt 0
		} else {
			/pet leave
		}
	}
/return



|***
 *	DES: 
 *	USE: /companionswarm
 *	INI: 
 *  NOTE:
 ***| 	
#bind companionswarm /companionswarm
sub Bind_companionswarm()
	/call Event_SetVar NA usePetSwarm Pet ${If[${usePetSwarm},FALSE,TRUE]}
	/call echos switch "Using Pet Swarms" usePetSwarm
/return



|***
 *	DES: 
 *	USE: /verbose
 *	INI: 
 *  NOTE:
 ***| 	
#bind verbose /verbose
sub Bind_verbose()
	/call Event_SetVar NA verbose Common ${If[${verbose},FALSE,TRUE]}
	/call echos switch "Verbose EQBC Echo" verbose
/return
   
   
   
|***
 *	DES: 
 *	USE: /checkcash
 *	INI: 
 *  NOTE:
 ***| 	
#bind gamecash /checkcash
sub Bind_gamecash()
	/call check_free_ingame_cash
/return
 
 
 
|***
 *	DES: 
 *	USE: /toastnow
 *	INI: 
 *  NOTE:
 ***| 	
#bind toast_now /toastnow
sub Bind_toast_now()
	/varset setToastMode ${If[${setToastMode},FALSE,TRUE]}
	/call echos switch "Constant Toasting" setToastMode
/return
 


|***
 *	DES: 
 *	USE: /toastmobcount
 *	INI: 
 *  NOTE:
 ***| 	
#bind toast_mobcount /toastmobcount
sub Bind_toast_mobcount(int _toastmobcount)
 	/if (${Range.Between[1,99:${_toastmobcount}]}) {
		/call Event_SetVar NA toastMobCount Common ${_toastmobcount} 
		/call echos number "Toast # Mob Enagage At" ${_toastmobcount}
	} else {
		NOUT /toastmobcount [${cinfo}##\ax] of mobs in range ${cinfo}1 - 99\ax
	}
/return 
 

 
 |***
 *	DES: 
 *	USE: /toastengageat
 *	INI: 
 *  NOTE:
 ***| 	
#bind toast_engageat /toastengageat
sub Bind_toast_engageat(int _toastengageat)
 	/if (${Range.Between[1,99:${_toastengageat}]}) {
		/call Event_SetVar NA whentoToast Common ${_toastengageat} 
		/call echos number "Toast % Health Enagage At" ${_toastengageat}
	} else {
		NOUT /toastengageat [${cinfo}##\ax] % of mobs health to engage toast at ${cinfo}1 - 99\ax
	}
/return 
 
 
 
|***
 *	DES: 
 *	USE: /rezcombat
 *	INI: 
 *  NOTE:
 ***| 	
#bind rezCombat /rezcombat
sub Bind_rezCombat()
	/call Event_SetVar NA rezCombat Heal ${If[${rezCombat},FALSE,TRUE]} 
	/call echos switch "Rez During Combat" rezCombat
/return



|***
 *	DES: 
 *	USE: /rezeveryone
 *	INI: 
 *  NOTE:
 ***| 	
#bind rezEveryone /rezeveryone
sub Bind_rezEveryone()
	/call Event_SetVar NA rezEveryone Heal ${If[${rezEveryone},FALSE,TRUE]} 
	/call echos switch "Rez Everyone" rezEveryone
/return



|***
 *	DES: 
 *	USE: /rezenable
 *	INI: 
 *  NOTE:
 ***| 	
#bind rezEnable /rezenable
sub Bind_rezEnable()
	/if (${Select[${Me.Class.ShortName},WAR,MNK,BST,ROG,ENC,WIZ,MAG,NEC,SHD,RNG,BRD]}) {
		/call Event_SetVar NA rezEnable Heal FALSE
		VOUT No rez for joo!
		/goto :exit
	}
	/call Event_SetVar NA rezEnable Heal ${If[${rezEnable},FALSE,TRUE]} 
	:exit
	/call echos switch "Automatic Casting if Resurrection/Call" rezEnable
/return



|***
 *	DES: 
 *	USE: /rezauto
 *	INI: 
 *  NOTE:
 ***| 	
#bind rezAuto /rezauto
sub Bind_rezAuto()
	/call Event_SetVar NA rezAuto Heal ${If[${rezAuto},FALSE,TRUE]} 
	/call echos switch "Automatic Rez" rezAuto
/return



|***
 *	DES: 
 *	USE: /ri
 *	INI: 
 *  NOTE:
 ***| 	
#bind setRIMode /ri
sub Event_setRIMode()
	/call Event_SetVar NA setRaidInvite Common ${If[${setRaidInvite},FALSE,TRUE]} 
	/call echos switch "Invites for Raids" setRaidInvite
/return



|***
 *	DES: 
 *	USE: /rezemergency
 *	INI: 
 *  NOTE:
 ***| 	
#bind rezEmergency /rezemergency
sub Bind_rezEmergency()
	/call Event_SetVar NA rezEmergency Heal ${If[${rezEmergency},FALSE,TRUE]} 
	/call echos switch "Emergency Rez Mode" rezEmergency
/return



|***
 *	DES: 
 *	USE: /home
 *	INI: 
 *  NOTE:
 ***| 	
#bind HomeCamp /home
sub Bind_HomeCamp()
	/call Event_SetVar NA HomeSet Movement ${If[${HomeSet},FALSE,TRUE]}  
	/call set_home
/return



|***
 *	DES: 
 *	USE: /setpuller
 *	INI: 
 *  NOTE:
 ***| 	
#bind setPuller /setpuller
sub Bind_setPuller()
	/call Event_SetVar NA setPullMode Common ${If[${setPullMode},FALSE,TRUE]} 
	/call echos switch "Pull Mode" setPullMode
	/if (${setPullMode}) /smartma X
/return



|***
 *	DES: 
 *	USE: /setagro
 *	INI: 
 *  NOTE:
 ***| 	
#bind setAgro /setagro
sub Bind_setAgro()
	/call Event_SetVar NA setAgroMode Common ${If[${setAgroMode},FALSE,TRUE]}  
	/call echos switch "Agro Mode" setAgroMode
/return



|***
 *	DES: 
 *	USE: /invites
 *	INI: 
 *  NOTE:
 ***| 	
#bind takeInvites /invites
sub Bind_takeInvites()
	/call Event_SetVar NA switchInvites Common ${If[${switchInvites},FALSE,TRUE]}  
	/call echos switch "Auto Accept Invites" switchInvites
/return



|***
 *	DES: 
 *	USE: /banestrike
 *	INI: 
 *  NOTE:
 ***| 	
#bind banestrike /banestrike
sub Bind_banestrike()
	/call Event_SetVar NA switchBanestrike Common ${If[${switchBanestrike},FALSE,TRUE]}  
	/call echos switch "Use AA Banestrike" switchBanestrike
/return



|***
 *	DES: 
 *	USE: /buffself
 *	INI: 
 *  NOTE:
 ***| 	
#bind buffself /buffself
sub Bind_buffself()
	/call Event_SetVar NA BuffSelf Buffs ${If[${BuffSelf},FALSE,TRUE]}
	/call echos switch "Buff Self" BuffSelf
/return



|***
 *	DES: 
 *	USE: /buffitem
 *	INI: 
 *  NOTE:
 ***| 	
#bind buffitem /buffitem
sub Bind_buffitem()
	/call Event_SetVar NA BuffItem Common ${If[${BuffItem},FALSE,TRUE]}
	/call echos switch "Buff Item" BuffItem
/return



|***
 *	DES: 
 *	USE: /buffbc
 *	INI: 
 *  NOTE:
 ***| 	
#bind buffbc /buffbc
sub Bind_buffbc()
	/if (!${Bool[${Plugin[MQ2netbots]}]}) {
		VOUT you need the plugn ${cinfo}MQ2netbots\ax for curing
		/call Event_SetVar NA BuffBC Buffs FALSE
	} else {
		/call Event_SetVar NA BuffBC Buffs ${If[${BuffBC},FALSE,TRUE]}
	}
		/call echos switch "Buff BC" BuffBC
/return



|***
 *	DES: 
 *	USE: /buffcombat
 *	INI: 
 *  NOTE:
 ***| 	
#bind buffcombat /buffcombat
sub Bind_buffcombat()
	/call Event_SetVar NA BuffCombat Buffs ${If[${BuffCombat},FALSE,TRUE]}
	/call echos switch "Buff Combat [range ${combatRadius}]" BuffCombat
/return



|***
 *	DES: 
 *	USE: /buffraid
 *	INI: 
 *  NOTE:
 ***| 	
#bind buffraid /buffraid
sub Bind_buffraid()
	/call Event_SetVar NA BuffRaid Buffs ${If[${BuffRaid},FALSE,TRUE]}
	/call echos switch "Buff Raid" BuffRaid
/return



|***
 *	DES: 
 *	USE: /bufftell
 *	INI: 
 *  NOTE:
 ***| 	
#bind bufftell /bufftell
sub Bind_bufftell()
	/call Event_SetVar NA BuffTell Buffs ${If[${BuffTell},FALSE,TRUE]}
	/call echos switch "Buff Tell" BuffTell
/return



|***
 *	DES: 
 *	USE: /buffpet
 *	INI: 
 *  NOTE:
 ***| 	
#bind buffpet /buffpet
sub Bind_buffpet()
	/if (${Me.Class.PetClass} || ${Select[${Me.Class.ShortName},ENC,DRU,SHM,SHD]}) /call Event_SetVar NA BuffPet Pet ${If[${BuffPet},FALSE,TRUE]}  
	/call echos switch "Buff Pet" BuffPet
/return



|***
 *	DES: 
 *	USE: /healself
 *	INI: 
 *  NOTE:
 ***| 	
#bind healself /healself
sub Bind_healself()
	/if (${Select[${Me.Class.ShortName},DRU,CLR,SHM,PAL]}) {
		VOUT Heal Classes [CLR,DRU,SHM,PAL] use /healgroup. Not /healself.
		/call Event_SetVar NA switchHealSelf Heal FALSE 
	} else {
		/call Event_SetVar NA switchHealSelf Heal ${If[${switchHealSelf},FALSE,TRUE]}  
	}
	/call echos switch "Heal Self" switchHealSelf
/return



|***
 *	DES: 
 *	USE: /healgroup
 *	INI: 
 *  NOTE:
 ***| 	
#bind healgroup /healgroup
sub Bind_healgroup()
	/call Event_SetVar NA switchHealGroup Heal ${If[${switchHealGroup},FALSE,TRUE]}  
	/call echos switch "Heal Group" switchHealGroup
/return



|***
 *	DES: 
 *	USE: /healxtarget
 *	INI: 
 *  NOTE:
 ***| 	
#bind healxtarget /healxtarget
sub Bind_healxtarget()
	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,PAL]}) /call Event_SetVar NA switchHealXTarget Heal ${If[${switchHealXTarget},FALSE,TRUE]}
	/call echos switch "Heal XTarget" switchHealXTarget
/return



|***
 *	DES: 
 *	USE: /healpet
 *	INI: 
 *  NOTE:
 ***| 	
#bind healpet /healpet
sub Bind_healpet()
	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,MAG]}) /call Event_SetVar NA switchHealPet Pet ${If[${switchHealPet},FALSE,TRUE]}  
	/call echos switch "Heal Pet" switchHealPet
/return



|***
 *	DES: 
 *	USE: /healcure
 *	INI: 
 *  NOTE:
 ***| 	
#bind healcure /healcure
sub Bind_healCure()
	/call Event_SetVar NA switchHealCure Heal ${If[${switchHealCure},FALSE,TRUE]}  
	/call echos switch "Curing" switchHealCure
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
	/if (${Bool[${AANow1}]} || ${Bool[${AANow2}]}) /call check_AA_now
	/if (${Bool[${DiscNow1}]} || ${Bool[${DiscNow2}]}) /call check_Disc_now
/return



|***
 *	DES: force self pet buffing
 *	USE: /pb
 *	INI: 
 *  NOTE:
 ***| 	
#bind petbuffs /pb
sub Bind_petbuffs()
	/call check_pet_buffs
/return



|***
 *	DES: force build a new pet
 *	USE: /buildpet
 *	INI: 
 *  NOTE:
 ***| 	
#bind buildpet /buildpet
sub Bind_buildpet()
	/call check_pet
/return



|***
 *	DES: turns automatic mode on or off
 *	USE: /core
 *	INI: 
 *  NOTE:
 ***| 	
#bind setCore /core
sub Bind_setCore()
	/call sort_core
/return

sub sort_core()
	/call Event_SetVar NA AutoBot Common ${If[${AutoBot},FALSE,TRUE]}  
	/if (!${AutoBot}) { 
		/call set_stop TRUE
		/call clear_combat
	}
	/call echos switch "Automatic" AutoBot
/return



|***
 *	DES: enable or disable auto shrinking 
 *	USE: /autoshrink
 *	INI: 
 *  NOTE:
 ***| 	
#bind setAutoShrink /autoshrink
sub Bind_setAutoShrink()
	/call Event_SetVar NA setAutoShrink Common ${If[${setAutoShrink},FALSE,TRUE]}  
	/call echos switch "Automatic Shrink" setAutoShrink
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
 *	DES: enable/disable Line of Site checking
 *	USE: /ignorelos
 *	INI: 
 *  NOTE:
 ***| 	
#bind setIgnoreLOS /ignorelos
sub Bind_setIgnoreLOS()
	/call Event_SetVar NA setIgnoreLOS Movement ${If[${setIgnoreLOS},FALSE,TRUE]}  
	/call echos switch "Ignoring Line of Sight" setIgnoreLOS
	/if (${setIgnoreLOS}) NOUT ${owarning} ${sep} THIS IS ${cbad}NOT\ax ADVISED. You will have pathing issues.
/return	



|***
 *	DES: 
 *	USE: /tietoon [NAME]
 *	INI: 
 *  NOTE:
 ***| 
#bind setTieToon /tietoon
sub Bind_setTieToon(string _settietoon)
	/if (${_settietoon.NotEqual[${Me.DisplayName}]}) {	
		/call Event_SetVar NA SetTieToon Movement ${_settietoon}	
		/call echos	string "Set Tie Toon" _settietoon
	}
/return



|***
 *	DES: 
 *	USE: /tietime [##]
 *	INI: 
 *  NOTE:
 ***| 	
#bind setTietime /tietime
sub Bind_setTietime(int _setTietime)
	/if (${Range.Between[5,50:${_setTietime}]}) {
		/call Event_SetVar NA tCheck_Tie Timer ${_setTietime} 
	} else {
		NOUT /tietime [${cinfo}##\ax] allowable range is ${cinfo}5 - 50\ax
	}
	/call echos number "Tie Check Time" ${_setTietime}
/return



|***
 *	DES: 
 *	USE: /tiedistance [##]
 *	INI: 
 *  NOTE:
 ***| 	
#bind SetTieDistance /tiedistance
sub Bind_SetTieDistance(int _SetTieDistance)
	/if (${Range.Between[1,99:${_SetTieDistance}]}) {
		/call Event_SetVar NA SetTieDistance Movement ${_SetTieDistance} 
	} else {
		NOUT /tiedistance [${cinfo}##\ax] allowable range is ${cinfo}1 - 99\ax
	}
		/call echos number "Tie Distance" ${_SetTieDistance}
/return



|***
 *	DES: enable/disable Tie
 *	USE: /tie [NAME]
 *	INI: 
 *  NOTE:
 ***| 	
#bind setTieMode /tie
sub Bind_setTieMode(string _settietoon)

	| do we want to change the /tietoon [NAME]
	/if (${Bool[${_settietoon}]} && ${SetTieToon.NotEqual[${_settietoon}]}) /tietoon ${_settietoon}
	
	| is tietoon set or in zone?
	/if (!${Bool[${SetTieToon}]} || !${Bool[${Spawn[pc ${SetTieToon}].ID}]}) {
		/if (!${Bool[${SetTieToon}]}) {
			NOUT ${owarning} ${sep} You need to set someone to be close too ${cinfo}/tietoon\ax [NAME].
			/call Event_SetVar NA SetTie Movement FALSE 
			/call echos switch "Tie" SetTie
		}
		/if (!${Bool[${Spawn[pc ${SetTieToon}].ID}]}) {
			NOUT ${onotice} ${sep} ${cpc}${SetTieToon}\ax is not in same zone.
			/call Event_SetVar NA SetTie Movement FALSE 
			/call echos switch "Tie to ${cpc}${SetTieToon}\ax" SetTie 
		}
	| adjust the switch
	} else {
		/call Event_SetVar NA SetTie Movement ${If[${SetTie},FALSE,TRUE]} 
		/call echos switch "Tie to ${cpc}${SetTieToon}\ax" SetTie
	}
/return

sub sort_setTieMode(string _settietoon)
	| do we want to change the /tietoon [NAME]
	/if (${Bool[${_settietoon}]} && ${SetTieToon.NotEqual[${_settietoon}]}) /tietoon ${_settietoon}
	
	| is tietoon set or in zone?
	/if (!${Bool[${SetTieToon}]} || !${Bool[${Spawn[pc ${SetTieToon}].ID}]}) {
		/if (!${Bool[${SetTieToon}]}) {
			NOUT ${owarning} ${sep} You need to set someone to be close too ${cinfo}/tietoon\ax [NAME].
			/call Event_SetVar NA SetTie Movement FALSE 
			/call echos switch "Tie" SetTie
		}
		/if (!${Bool[${Spawn[pc ${SetTieToon}].ID}]}) {
			NOUT ${onotice} ${sep} ${cpc}${SetTieToon}\ax is not in same zone.
			/call Event_SetVar NA SetTie Movement FALSE 
			/call echos switch "Tie to ${cpc}${SetTieToon}\ax" SetTie 
		}
	| adjust the switch
	} else {
		/call Event_SetVar NA SetTie Movement ${If[${SetTie},FALSE,TRUE]} 
		/call echos switch "Tie to ${cpc}${SetTieToon}\ax" SetTie
	}
/return




|***
 *	DES: 
 *	USE: /stickbehind
 *	INI: 
 *  NOTE:
 ***| 	
#bind SetStickBehind /stickbehind
sub Bind_SetStickBehind()
	/call Event_SetVar NA SetStickBehind Movement ${If[${SetStickBehind},FALSE,TRUE]}  
	/call echos switch "Stick behind target"  SetStickBehind

	| disable mezzing with stickbehind enabled.
	/if (${switchdeBuffMezz} && ${SetStickBehind}) {
		/if (${Defined[switchdeBuffMezz]}) /call Event_SetVar NA switchdeBuffMezz Common FALSE  
		/call echos switch "deBuff  Mezz" switchdeBuffMezz
	}
/return



|***
 *	DES: 
 *	USE: /facefast
 *	INI: 
 *  NOTE:
 ***| 	
#bind SetFaceFast /facefast
sub Bind_SetFaceFast()
	/call Event_SetVar NA SetFaceFast Movement ${If[${SetFaceFast},FALSE,TRUE]}  
	/call echos switch "Face Fast"  SetFaceFast
	/if (${SetFaceFast}) NOUT ${owarning} ${sep} This is a MQ2 Dead Giveaway. Use with caution.
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
	/if ((${_assist.Equal[1]} || (!${Bool[${_assist}]} && !${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG} && ${Bool[${Raid.Members}]})) && !${Select[${Me.Class.ShortName},CLR]} && !${setPullMode}) {
		DEBUG ((${_assist} || (!${Bool[${_assist}]} && !${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG} && ${Bool[${Raid.Members}]})) && !${Select[${Me.Class.ShortName},CLR]} && !${setPullMode})
 		/if (!${useSmartAssist1}) /call Event_SetVar NA useSmartAssist1 Common TRUE
		/if (${useSmartAssist2}) /call Event_SetVar NA useSmartAssist2 Common FALSE
		/if (${useSmartAssistG}) /call Event_SetVar NA useSmartAssistG Common FALSE
		/xtarget set 1 Raidassist1target
		VOUT Setting Assist ${sep} ${cinfo}Raid 1\ax
	| Raid Assist 2
	} else /if ((${_assist.Equal[2]} || (!${Bool[${_assist}]} && ${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG} && ${Bool[${Raid.Members}]})) && !${Select[${Me.Class.ShortName},CLR]} && !${setPullMode}) {
		DEBUG ((${_assist} || (!${Bool[${_assist}]} && ${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG} && ${Bool[${Raid.Members}]})) && !${Select[${Me.Class.ShortName},CLR]} && !${setPullMode})
 		/if (${useSmartAssist1}) /call Event_SetVar NA useSmartAssist1 Common FALSE
		/if (!${useSmartAssist2}) /call Event_SetVar NA useSmartAssist2 Common TRUE
		/if (${useSmartAssistG}) /call Event_SetVar NA useSmartAssistG Common FALSE
		/xtarget set 1 Raidassist1target
		VOUT Setting Assist ${sep} ${cinfo}Raid 2\ax
	| NO Assist
	} else /if ((${_assist.Equal[X]} || (!${Bool[${_assist}]} && !${useSmartAssist1} && !${useSmartAssist2} && ${useSmartAssistG}) || (!${Bool[${Raid.Members}]} && !${Bool[${Group.Members}]})) || ${Select[${Me.Class.ShortName},CLR]} || ${setPullMode}) {
		DEBUG ((${_assist} || (!${Bool[${_assist}]} && !${useSmartAssist1} && !${useSmartAssist2} && ${useSmartAssistG}) || (!${Bool[${Raid.Members}]} && !${Bool[${Group.Members}]})) || ${Select[${Me.Class.ShortName},CLR]} || ${setPullMode})
 		/if (${useSmartAssist1}) /call Event_SetVar NA useSmartAssist1 Common FALSE
		/if (${useSmartAssist2}) /call Event_SetVar NA useSmartAssist2 Common FALSE
		/if (${useSmartAssistG}) /call Event_SetVar NA useSmartAssistG Common FALSE
		/if (${Bool[${MA1}]}) /call Event_SetVar NA MA1 Common FALSE
		/if (${Bool[${MA2}]}) /call Event_SetVar NA MA2 Common FALSE
		| /if (${setPullMode}) /call echos switch "Pull Mode" setPullMode
		/xtarget set 1 Autohater
		VOUT Setting Assist ${sep} ${odisabled}\ax
	| Goup Main Assist
	} else /if ((${_assist.Equal[G]} || (!${Bool[${_assist}]} && !${useSmartAssist1} && ${useSmartAssist2} && !${useSmartAssistG}) || (!${Bool[${Raid.Members}]} && ${Bool[${Group.Members}]} && !${useSmartAssistG})) && !${Select[${Me.Class.ShortName},CLR]} && !${setPullMode}) {
		DEBUG ((${_assist} || (!${Bool[${_assist}]} && !${useSmartAssist1} && ${useSmartAssist2} && !${useSmartAssistG}) || (!${Bool[${Raid.Members}]} && ${Bool[${Group.Members}]} && !${useSmartAssistG})) && !${Select[${Me.Class.ShortName},CLR]} && !${setPullMode})
 		/if (${useSmartAssist1}) /call Event_SetVar NA useSmartAssist1 Common FALSE
		/if (${useSmartAssist2}) /call Event_SetVar NA useSmartAssist2 Common FALSE
		/if (!${useSmartAssistG}) /call Event_SetVar NA useSmartAssistG Common TRUE
		/xtarget set 1 Groupassisttarget
		VOUT Setting Assist ${sep} ${cinfo}Group\ax
	}
/return



|***
 *	DES: 
 *	USE: /debuffmezz
 *	INI: 
 *  NOTE:
 ***| 	
#bind deBuffMezz /debuffmezz
sub Bind_deBuffMezz()
	/call Event_SetVar NA switchdeBuffMezz Common ${If[${switchdeBuffMezz},FALSE,TRUE]}  
	/call echos switch "deBuff  Mezz" switchdeBuffMezz

	| clear stickbehind. it will force bad targeting fot the mezzer.
	/if (${SetStickBehind} && ${switchdeBuffMezz}) {
		/call Event_SetVar NA SetStickBehind Movement FALSE  
		/call echos switch "Stick behind target"  SetStickBehind
	}
	:exit
/return



|***
 *	DES: 
 *	USE: /debuffmalo
 *	INI: 
 *  NOTE:
 ***| 	
#bind deBuffMalo /debuffmalo
sub Bind_deBuffMalo()
	/if (${Select[${Me.Class.ShortName},SHM,MAG]}) {
		/call Event_SetVar NA switchdeBuffMalo Common ${If[${switchdeBuffMalo},FALSE,TRUE]}  
 	} else {
		VOUT ${onotice} ${sep} Not an AA Malo class.
		/if (${Defined[switchdeBuffMalo]}) /call Event_SetVar NA switchdeBuffMalo Common FALSE  
	}
	/call echos switch "deBuff Malo" switchdeBuffMalo	
/return



|***
 *	DES: 
 *	USE: /debufftash
 *	INI: 
 *  NOTE:
 ***| 	
#bind deBuffTash /debufftash
sub Bind_deBuffTash()
	/call Event_SetVar NA switchdeBuffTash Common ${If[${switchdeBuffTash},FALSE,TRUE]}  
	/call echos switch "deBuff  Tash" switchdeBuffTash
/return



|***
 *	DES: 
 *	USE: /debuffcripple
 *	INI: 
 *  NOTE:
 ***| 	
#bind deBuffCripple /debuffcripple
sub Bind_deBuffCripple()
	/call Event_SetVar NA switchdeBuffCripple Common ${If[${switchdeBuffCripple},FALSE,TRUE]}  
	/call echos switch "deBuff  Cripple" switchdeBuffCripple
/return



|***
 *	DES: 
 *	USE: /debuffslow
 *	INI: 
 *  NOTE:
 ***| 	
#bind deBuffSlow /debuffslow
sub Bind_deBuffSlow()
	/call Event_SetVar NA switchdeBuffSlow Common ${If[${switchdeBuffSlow},FALSE,TRUE]}  
	/call echos switch "deBuff Slow" switchdeBuffSlow
/return



|***
 *	DES: 
 *	USE: /debufferadicate
 *	INI: 
 *  NOTE:
 ***| 	
#bind deBuffEradicate /debufferadicate
sub Bind_deBuffEradicate()
	/if (${Select[${Me.Class.ShortName},ENC,MAG,NEC,WIZ,RNG,DRU]}) {
		/call Event_SetVar NA switchdeBuffEradicate Common ${If[${switchdeBuffEradicate},FALSE,TRUE]}  
	} else {
		VOUT ${onotice} ${sep} Not an AA Eradicate class. Please come back later.
		/if (${Defined[switchdeBuffEradicate]}) /call Event_SetVar NA switchdeBuffEradicate Common FALSE  
	}
	/call echos switch "deBuff Eradicate" switchdeBuffEradicate
/return
	
	
	
|***
 *	DES: 
 *	USE: /debuffsnare
 *	INI: 
 *  NOTE:
 ***| 	
#bind deBuffSnare /debuffsnare
sub Bind_deBuffSnare()
	/if (${AASnareList.Find[${Me.Class.ShortName}]}) {
		/call Event_SetVar NA switchdeBuffSnare Common ${If[${switchdeBuffSnare},FALSE,TRUE]}  
	} else {
		VOUT ${onotice} ${sep} Not an AA snaring class. Please come back later.
		/if (${Defined[switchdeBuffSnare]}) /call Event_SetVar NA switchdeBuffSnare Common FALSE  
	}
	/call echos switch "deBuff Snare" switchdeBuffSnare
/return
	
	
	
|***
 *	DES: 
 *	USE: /toastauto
 *	INI: 
 *  NOTE:
 ***| 	
#bind toastauto /toastauto
sub Bind_toastauto()
	/call Event_SetVar NA AutoToast Common ${If[${AutoToast},FALSE,TRUE]}  
	/call echos switch "Auto Toast" AutoToast
/return



|***
 *	DES: 
 *	USE: /autorest [##]
 *	INI: 
 *  NOTE:
 ***| 	
#bind autoRest /autorest
sub Bind_autoRest(int _autoRest)
	/if (${Range.Between[1,99:${_autoRest}]}) {
		/call Event_SetVar NA autoRest Common ${_autoRest}  
		/call echos number "Auto Rest" ${If[${_autoRest},${_autoRest},${autoRest}]} 
	} else {
		NOUT /autorest [${cinfo}##\ax] allowable range is ${cinfo}1 - 99\ax
	}
/return



|***
 *	DES: 
 *	USE: /agro [##]
 *	INI: 
 *  NOTE:
 ***| 	
#bind MobAgro /agro
sub Bind_MobAgro(int _MobAgro)
	/if (${Range.Between[1,400:${_MobAgro}]}) {
		/call Event_SetVar NA MobAgro Common ${_MobAgro} 
		/call echos number "Mob Agro Range" ${_MobAgro}
	} else {
		NOUT /agro [${cinfo}##\ax] allowable range is ${cinfo}1 - 400\ax
	}
/return

	
	
|***
 *	DES: 
 *	USE: /engageat [##]
 *	INI: 
 *  NOTE:
 ***| 	
#bind engageAt /engageat
sub Bind_engageAt(int _engageAt)
	/if (${Range.Between[1,99:${_engageAt}]}) {
		/call Event_SetVar NA engageAt Common ${_engageAt} 
`	/call echos number "MA1/2 Enagage At" ${_engageAt}
	} else {
		NOUT /engageat [${cinfo}##\ax] of mobs HP range ${cinfo}1 - 99\ax
	}
/return	
	


|***
 *	DES: 
 *	USE: /smartengageat [##]
 *	INI: 
 *  NOTE:
 ***| 	
#bind smartengageAt /smartengageat
sub Bind_smartengageAt(int _smartengageAt)
	/if (${Range.Between[1,99:${_smartengageAt}]}) {
		/call Event_SetVar NA smartengageAt Common ${_smartengageAt} 
		/call echos number "Smart Enagage At" ${_smartengageAt}
	} else {
		NOUT /smartengageat [${cinfo}##\ax] of mobs HP range ${cinfo}1 - 99\ax
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
		| /delay ${Math.Rand[5]}
		/if (${Me.AltAbilityReady[Group Perfected Invisibility]} && ${Select[${Me.Class.ShortName},WIZ,MAG,ENC]}) {
			/call MQ2Cast "Group Perfected Invisibility" alt -targetid|${Me.ID}
			ECHOCHANNEL AA: Group Perfected Invisibility
		}  
		/if (${Me.AltAbilityReady[Group Perfected Invisibility to Undead]} && ${Select[${Me.Class.ShortName},PAL,CLR,SHD,NEC]}) {
			/delay 3
			/call MQ2Cast "Group Perfected Invisibility to Undead" alt -targetid|${Me.ID}
			ECHOCHANNEL AA: Group Perfected Invisibility to Undead
		}

	:exit
/return		
	


|***
 *	DES: force 1 toast routine
 *	USE: /toastforce
 *	INI: 
 *  NOTE:
 ***| 
#bind toast_force /toastforce
sub Bind_toast_force()
	/call synergy_${Me.Class.ShortName}
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
		/call core_cast "Primary Anchor Transport Device" item
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
		/call core_cast "Secondary Anchor Transport Device" item
	}
/return



|***
 *	DES: 
 *	USE: /zradius ##
 *	INI: 
 *  NOTE:
 ***| 	
#bind zradius /zradius
sub Bind_zradius(int _zradius)
	/if (${Range.Between[1,200:${_zradius}]}) {
		/call Event_SetVar NA zradius Common ${_zradius} 
		/call echos number "Set ZRadius At" ${_zradius}
	} else {
		NOUT /zradius [${cinfo}##\ax] of mobs HP range ${cinfo}1 - 200\ax
	}
/return



|***
 *	DES: forces a reload of all INIs
 *	USE: /resetcore
 *	INI: 
 *  NOTE:
 ***| 	
#bind resetcore /resetcore
sub Bind_resetcore()
	
	NOUT ${cgood}Reloading of all INIs\ax
	/call declare_base
	/call check_plugin
	/call echos startup
	/call declare_common
	/call declare_${Me.Class.ShortName}
	/call synergy_core
	/call intialize_item_buffs
	/call set_alias
	/call Event_Zone
	/call base_preperation
	
	| /call clear_combat
	/if (${usePet} && ${Me.Pet.ID} && ${Bool[${Me.Pet.Target}]}) /pet back off

	| get shit out of our hands
	CLEARCURSOR TRUE
	
	| clear modes
	/call mode_clear_all
	
	| clear the current target
	/if (${clearTarget} && ${Target.ID}) /squelch /target clear
	/call set_stop TRUE
	/call clear_combat
	
	| set assists
	/if (!${Bool[${Raid.Members}]} && !${Select[${Me.Class.ShortName},CLR]} && !${setPullMode}) {
		/smartma G
 	} else /if (${Bool[${Raid.Members}]} && !${Select[${Me.Class.ShortName},CLR]} && ${setPullMode}) {
		/smartma 1
	}	else /if (${Select[${Me.Class.ShortName},CLR]} || ${setPullMode}) {
		/smartma X
	}	
	
	| other misc resets
	/call Event_SetVar NA watchSpawn Common False
	
	NOUT ${cgood}Done\ax
/return



|***
 *	DES: 
 *	USE: /watchspawn
 *	INI: 
 *  NOTE:
 ***| 
#bind watchspawn /watchspawn
sub Bind_watchspawn(string _spawn)
	/if (${Bool[${_spawn}]}) {
		/call Event_SetVar NA watchSpawn Common "${_spawn}"
	} else /if (!${Bool[${_spawn}]} && ${Target.ID}) {
		/call Event_SetVar NA watchSpawn Common "${Target.DisplayName}"
	} else /if (!${Bool[${_spawn}]} && !${Target.ID}) {
		VOUT ${sep} ${cinfo}/watchspawn\ax requires a taret or mob partial name. 
		VOUT ${sep} NPC and CORPSE is also acceptable.
		/if (!${Bool[${_spawn}]}) /call Event_SetVar NA watchSpawn Common FALSE
	}
/return



|***
 *	DES: 
 *	USE: /combatradius
 *	INI: 
 *  NOTE:
 ***| 
#bind combatRadius /combatradius
sub Bind_combatRadius(string _combatRadius)
	/if (${Range.Between[1,99:${_combatRadius}]}) {
		/call Event_SetVar NA combatRadius Common ${_combatRadius}
		/call echos number "Fake Safe Combat Radius" ${_combatRadius}
	} else {
		NOUT /combatradius [${cinfo}##\ax] of mobs HP range ${cinfo}1 - 99\ax
	}
/return



|***
 *	DES: 
 *	USE: /combatdistance
 *	INI: 
 *  NOTE:
 ***| 
#bind CombatDistance /combatdistance
sub Bind_CombatDistance(string _CombatDistance)
	/if (${Range.Between[1,99:${_CombatDistances}]}) {
		/call Event_SetVar NA CombatDistance Common ${_CombatDistance}
		/call echos number "Mele Combat Distance" ${_CombatDistance}
	} else {
		NOUT /combatradius [${cinfo}##\ax] of mobs HP range ${cinfo}1 - 99\ax
	}
/return



|***
 *	DES: forces checking for runes and spellguards
 *	USE: /rb
 *	INI: 
 *  NOTE:
 ***| 
#bind check_rune /rb
sub Bind_check_rune()
	/if (${Select[${Me.Class.ShortName},WIZ,ENC]}) /call check_rune_${Me.Class.ShortName}
/return


	
|***
 *	DES: 
 *	USE: /sendpetperc
 *	INI: 
 *  NOTE:
 ***| 
#bind sendPetPerc /petengageat
sub Bind_sendPetPerc(string _sendpetperc)
	/if (${Range.Between[1,99:${_sendpetperc}]}) {
		/call Event_SetVar NA sendPetPerc Common ${_sendpetperc}
		/call echos number "Pet Engage" ${_sendpetperc}
	} else {
		NOUT /sendpetperc [${cinfo}##\ax] valid range range ${cinfo}1 - 99\ax
	}
/return	


	
|***
 *	DES: 
 *	USE: /petswarmengageat
 *	INI: 
 *  NOTE:
 ***| 
#bind sendPetSwarmPerc /petswarmengageat
sub Bind_sendPetSwarmPerc(string _sendpetswarmperc)
	/if (${Range.Between[1,99:${_sendpetswarmperc}]}) {
		/call Event_SetVar NA sendPetSwarmPerc Common ${_sendpetswarmperc}
		/call echos number "Swarm Pet Engage" ${_sendpetswarmperc}
	} else {
		NOUT /sendpetswarmperc [${cinfo}##\ax] valid range range ${cinfo}1 - 99\ax
	}
/return	



