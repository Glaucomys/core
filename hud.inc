|***
 *	hud.inc
 *	Core HUD Include
 *	hard coded top left corner to surround lag-O-meter(tm)
 * 
 ***|




|***
 *	DES: Controlls for the HUD
 *	USE: /hc [time|update|broadcast]
 *	INI: 
 *  NOTE:
 ***| 
#bind hud /hc
sub Bind_hud(string _hcType, string _verbage)

	/declare _sep bool local
	/declare _listout string local
	/declare _count int local 0
	/declare _bind string local hc	
	
	| get help
	/if (${_${_bind}Type.Equal[info]}) {
		/for _count 1 to ${${_bind}Type.Count[|]}
			VOUT /${_bind} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${${_bind}Type.Arg[${_count},|]}]}
		/next _count
		/return			
	} else /if (${Bool[${${_bind}Type.Find[${_hcType}]}]} && ${_${_bind}Verbage.Equal[info]}) {
		VOUT /${_bind} ${cinfo}${_${_bind}Type.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_${_bind}Type.Lower}]}
		/return
	}		
	
	| adjust hud clock
	/if (${Bool[${_${_bind}Type.Find[time]}]}) {
		/varset HUDTime ${If[${HUDTime},FALSE,TRUE]}
		/if (${HUDTime}) VOUT HUD Time ${sep} ${cinfo}IRL\ax
		/if (!${HUDTime}) VOUT HUD Time ${sep} ${cinfo}Game\ax

	| update hud broadcasting	
	} else /if (${Bool[${_${_bind}Type.Find[broadcast]}]}) {
		/call writeINI setBroadcast Common ${If[${Bool[${setBroadcast}]},FALSE,TRUE]}
		/if (${setBroadcast.Equal[FALSE]}) /ini "${INICore}" "transmit_status" "${Me.DisplayName}" "DISABLED"
		/if (${setBroadcast.Equal[TRUE]}) /ini "${INICore}" "transmit_status" "${Me.DisplayName}" "${coreAuto}"
		/call echos switch "Adjusting HUD Broadcast Status" setBroadcast

	| reset the MQhud ini file
	} else /if (${Bool[${_${_bind}Type.Find[update]}]}) {
		/if (${Bool[${_verbage}]} ) {
			/call HUDUpdate ${_verbage}
		} else {
			/varset _verbage Core
		 	/call HUDUpdate ${_verbage}
		}

	| delete the hud
	} else /if (${Bool[${_${_bind}Type.Find[delete]}]}) {

		/if (${Bool[${_verbage}]} ) {
			/ini ".\..\MQ2HUD.ini" "${_verbage}" NULL NULL
		} else {
			/varset _verbage Core
			/ini ".\..\MQ2HUD.ini" "core" NULL NULL
		}
		VOUT Deleting MQ2HUD.INI ${sep} ${cinfo}${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}\ax
		
	} else {
		/for _count 1 to ${${_bind}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
	} 
/return





|***
 *	DES: HUD adjustment
 *	USE: 
 *	INI: 
  *  NOTE:
 ***|
sub HUDUpdate(string _verbage)
	VOUT Updating MQ2HUD.INI ${sep} ${cinfo}${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}\ax
	/delay 5


	| main HUD for Core
	/if (${_verbage.Equal[core]}) {
		| Build
		HUDPATH "Core" "core1" "3,5,26,255,255,255,Build"
		HUDPATH "Core" "core2" "19,44,26,100,149,237,${coreBuild}"

		| last tell
		HUDPATH "Core" "tell1" "3,5,38,255,255,255,Tell"
		HUDPATH "Core" "tell2" "3,44,38,100,149,237,${MacroQuest.LastTell}"

		| control toon
		HUDPATH "Core" "control1" "19,5,50,255,255,255,${If[${Bool[${autoControl}]},,Control]}"

		HUDPATH "Core" "control2" "19,5,50,0,255,0,${If[${Bool[${autoControl}]},Control,]}"
		HUDPATH "Core" "control3" "19,44,50,100,149,237,${If[${Bool[${toonControl}]},${toonControl},]}"

		| loc Y X Z
		HUDPATH "Core" "loc1" "3,5,62,255,255,255,LOC"
		HUDPATH "Core" "loc2" "3,44,62,100,149,237,${Int[${Me.Y}]}, ${Int[${Me.X}]}, ${Int[${Me.Z}]}"

		| mode
		HUDPATH "Core" "mode0" "19,5,74,0,255,0,${If[${switchAutoCursor},Mode,]}"
		HUDPATH "Core" "mode1" "19,5,74,255,0,0,${If[!${switchAutoCursor},Mode,]}"	
		HUDPATH "Core" "mode2" "19,44,74,255,255,0,${If[${Bool[${tmpMode}]},${tmpMode},]}"
		HUDPATH "Core" "mode3" "19,44,74,25,25,25,${If[${Bool[${tmpMode}]},,]}"

		| Target / LOS
		HUDPATH "Core" "bodytype1" "3,5,85,100,149,237,[                       ]"
		HUDPATH "Core" "bodytype2" "3,10,86,255,255,255,${Target.Body}"
		HUDPATH "Core" "LoS1" "3,85,86,0,255,0,${If[${Bool[${Target}]},${If[${Target.LineOfSight},${Target.Distance} .. ID:${Target.ID},]},]}"
		HUDPATH "Core" "LoS2" "3,85,86,255,0,0,${If[${Bool[${Target}]},${If[${Target.LineOfSight},,${Target.Distance} .. ID:${Target.ID}]},]}"

		| current target info
		HUDPATH "Core" "target1" "3,70,97,100,149,237,${If[${Target.ID},[${Target.Class.ShortName} ${Target.Level}] ${Target.Name} ${If[${Bool[${Target.Guild}]},<${Target.Guild}>, ]}, ]}"
		
		| engage ##
		HUDPATH "Core" "engage1" "3,5,97,100,149,237,[     ][     ][     ]"

		| assist ##
		HUDPATH "Core" "engage2" "19,10,98,255,255,255,${If[(!${switchOverrideEngageat} && !${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssist3} && !${useSmartAssistG} && (${Bool[${MA1}]} || ${Bool[${MA2}]}),${engageAt},]}"
		HUDPATH "Core" "engage3" "19,10,98,255,255,255,${If[!${switchOverrideEngageat} && (${useSmartAssist1} || ${useSmartAssist2} || ${useSmartAssist3} || ${useSmartAssistG}),${smartengageAt},]}"
		HUDPATH "Core" "engage4" "19,10,98,25,25,25,${If[!${switchOverrideEngageat} && !${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssist3} && !${useSmartAssistG} && !${Bool[${MA1}]} && !${Bool[${MA2}]},##,]}"
		HUDPATH "Core" "engage7" "19,12,98,255,0,0,${If[(${switchOverrideEngageat},X,]}"

		| pet engage ##
		HUDPATH "Core" "pettarget1" "19,31,98,255,255,255,${If[!${switchOverrideEngageat} && ${switchPcPet},${PcPetengageat},]}"
		HUDPATH "Core" "pettarget2" "19,31,98,25,25,25,${If[!${switchOverrideEngageat} && !${switchPcPet},##,]}"
		HUDPATH "Core" "pettarget3" "19,33,98,255,0,0,${If[${switchOverrideEngageat},X,]}"

		| swarmpet engage ##
		HUDPATH "Core" "pettarget4" "19,52,98,255,255,255,${If[!${switchOverrideEngageat} && ${switchPcSwarm},${PcSwarmengageat},]}"
		HUDPATH "Core" "pettarget5" "19,52,98,25,25,25,${If[(!${switchOverrideEngageat}) && !${switchPcSwarm},##,]}"
		HUDPATH "Core" "pettarget6" "19,55,98,255,0,0,${If[${switchOverrideEngageat},X,]}"

		| target effect notice
		HUDPATH "Core" "tarnotice1" "3,5,109,100,149,237,[                                                                                                 ]"
		HUDPATH "Core" "tarnotice2" "19,10,110,255,255,255,${hudTarNotice}"
		
		| watchspawn counter
		HUDPATH "Core" "watchspawn1" "3,382,109,100,149,237,[       ]"
		HUDPATH "Core" "watchspawn2" "19,387,110,0,255,0,${If[${Bool[${watchSpawn}]},${hudwatchspawn},]}"
		HUDPATH "Core" "watchspawn3" "19,388,109,25,25,25,${If[${Bool[${watchSpawn}]},,----]}"

		| combat modes
		HUDPATH "Core" "bot1" "3,140,13,100,149,237,[              ][    ][     ]"
		HUDPATH "Core" "bot2" "19,145,14,0,255,0,${If[${coreAuto},A,]}"
		HUDPATH "Core" "bot3" "19,145,14,25,25,25,${If[${coreAuto},,A]}"
		HUDPATH "Core" "bot4" "19,153,14,0,255,0,${If[${switchCombatMelee},M,]}"
		HUDPATH "Core" "bot5" "19,153,14,25,25,25,${If[${switchCombatMelee},,M]}"
		HUDPATH "Core" "bot6" "19,161,14,0,255,0,${If[${switchCombatRange},R,]}"
		HUDPATH "Core" "bot7" "19,161,14,25,25,25,${If[${switchCombatRange},,R]}"
		HUDPATH "Core" "bot8" "19,169,14,0,255,0,${If[${switchCombatNuke},N,]}"
		HUDPATH "Core" "bot9" "19,169,14,25,25,25,${If[${switchCombatNuke},,N]}"
		HUDPATH "Core" "bot10" "19,177,14,0,255,0,${If[${switchCombatDoT},D,]}"
		HUDPATH "Core" "bot11" "19,177,14,25,25,25,${If[${switchCombatDoT},,D]}"

		| combat distance
		HUDPATH "Core" "CD1" "19,191,14,255,255,255,${CombatDistance}"
		
		| stick behind
		HUDPATH "Core" "stickback1" "19,210,14,0,255,0,${If[${SetStickBehind},B,]}"
		HUDPATH "Core" "stickback2" "19,210,14,25,25,25,${If[${SetStickBehind},,B]}"
		
		| face fast
		HUDPATH "Core" "facefast1" "19,218,14,0,255,0,${If[${SetFaceFast},F,]}"
		HUDPATH "Core" "facefast2" "19,218,14,25,25,25,${If[${SetFaceFast},,F]}"

		| stuff for camp status
		HUDPATH "Core" "camp1" "3,140,26,100,149,237,R          L"
		HUDPATH "Core" "camp2" "3,150,25,100,149,237,[     ]     [     ]"
		HUDPATH "Core" "camp3" "19,155,26,0,255,0,${If[${HomeSet},${HomeRadius},]}"
		HUDPATH "Core" "camp4" "19,190,26,0,255,0,${If[${HomeSet},${HomeLeash},]}"
		HUDPATH "Core" "camp5" "19,155,26,25,25,25,${If[${HomeSet},,##        ##]}"

		| tie info
		HUDPATH "Core" "tie1" "19,140,38,100,149,237,${SetTieMode}"
		HUDPATH "Core" "tie2" "3,165,37,100,149,237,[     ][     ]"
		HUDPATH "Core" "tie3" "19,170,38,255,255,255,${If[${SetTie},${SetTieDistance},]}"
		HUDPATH "Core" "tie4" "19,191,38,255,255,255,${If[${SetTie},${tCheck_Tie},]}"
		HUDPATH "Core" "tie5" "19,191,38,25,25,25,${If[${SetTie},,##]}"
		HUDPATH "Core" "tie6" "19,170,38,25,25,25,${If[${SetTie},,##]}"
		HUDPATH "Core" "tie7" "19,209,38,0,255,0,${If[${Bool[${SetTieToon}]},${If[${Bool[${Spawn[${SetTieToon}]}]},${SetTieToon},]},]}"
		HUDPATH "Core" "tie8" "19,209,38,255,0,0,${If[${Bool[${SetTieToon}]},${If[!${Bool[${Spawn[${SetTieToon}]}]},${If[${Bool[${SetTieToon}]},${SetTieToon},]},]},]}"
		HUDPATH "Core" "tie9" "19,209,37,25,25,25,${If[${Bool[${SetTieToon}]},,----]}"

		| burn a thing
		HUDPATH "Core" "burn1" "19,140,50,0,255,0,${If[${burnAuto},Burn,]}"
		HUDPATH "Core" "burn2" "19,140,50,255,0,0,${If[${burnAuto},,Burn]}"
		HUDPATH "Core" "burn3" "3,170,49,100,149,237,[     ][     ]"
		HUDPATH "Core" "burn4" "19,175,50,255,255,255,${If[!${burnForce} && ${burnCount},${burnCount},]}"
		HUDPATH "Core" "burn5" "19,175,50,25,25,25,${If[!${burnForce} && !${burnCount},##,]}"
		HUDPATH "Core" "burn6" "19,177,50,255,0,0,${If[${burnForce},X,]}"
		HUDPATH "Core" "burn7" "19,196,50,255,255,255,${If[!${burnForce} && ${burnEngageat},${burnEngageat},]}"
		HUDPATH "Core" "burn8" "19,196,50,25,25,25,${If[!${burnForce} && !${burnEngageat},##,]}"
		HUDPATH "Core" "burn9" "19,199,50,255,0,0,${If[${burnForce},X,]}"
			
		| rez
		HUDPATH "Core" "rez1" "3,140,62,100,149,237,Rez"
		HUDPATH "Core" "rez2" "19,162,61,255,0,0,${If[${switchRezTake},,[         ]]}"
		HUDPATH "Core" "rez3" "19,162,61,0,255,0,${If[${switchRezTake},[         ],]}"
		HUDPATH "Core" "rez4" "19,197,61,255,0,0,${If[${switchRezGive},,[      ]]}"
		HUDPATH "Core" "rez5" "19,197,61,0,255,0,${If[${switchRezGive},[      ][       ],]}"
		HUDPATH "Core" "rez6" "19,167,62,255,255,255,${If[${coreBuild},${hudDeathCount},]}"
		HUDPATH "Core" "rez7" "19,200,62,0,255,0,${If[${switchRezCombat},C,]}"
		HUDPATH "Core" "rez8" "19,200,62,25,25,25,${If[${switchRezCombat},,C]}"
		HUDPATH "Core" "rez9" "19,208,62,0,255,0,${If[${switchRezEveryone},E,]}"
		HUDPATH "Core" "rez10" "19,208,62,25,25,25,${If[${switchRezEveryone},,E]}"
		HUDPATH "Core" "rez11" "19,215,62,0,255,0,${If[${switchRezEmergency},!,]}"
		HUDPATH "Core" "rez12" "19,215,62,25,25,25,${If[${switchRezEmergency},,!]}"
		HUDPATH "Core" "rez13" "19,225,62,255,255,255,${If[${switchRezGive},${maxRezRange},]}"

		| HUD WARNING/OVERIDE notices
		HUDPATH "Core" "hudwarning0" "3,110,1,100,149,237,[                                                     ]"
		HUDPATH "Core" "hudwarning1" "19,115,2,240,0,0,${If[${Bool[${hudWarning}]},${hudWarning},]}"

		| group broadcast stauts
		HUDPATH "Core" "hudbroadcast0" "19,280,1,0,255,0,${If[${Bool[${setBroadcast}]},[                                         ],]}"
		HUDPATH "Core" "hudbroadcast1" "19,280,1,255,0,0,${If[${Bool[${setBroadcast}]},,[                                         ]]}"
		HUDPATH "Core" "hudbroadcast2" "19,285,2,100,149,237,${hudBroadcast}"

		| agro/puler/offtank status
		HUDPATH "Core" "agro0" "3,280,13,100,149,237,[         ][       ][       ][    ][      ]"
		HUDPATH "Core" "agro1" "19,285,14,0,255,0,${If[${switchAgro},A,]}"
		HUDPATH "Core" "agro2" "19,285,14,25,25,25,${If[${switchAgro},,A]}"
		HUDPATH "Core" "pull1" "19,294,14,0,255,0,${If[${switchPull},P,]}"
		HUDPATH "Core" "pull2" "19,294,14,25,25,25,${If[${switchPull},,P]}"	
		HUDPATH "Core" "ot1" "19,302,14,0,255,0,${If[${switchOffTank},O,]}"	
		HUDPATH "Core" "ot2" "19,302,14,25,25,25,${If[${switchOffTank},,O]}"	
		HUDPATH "Core" "agro4" "19,318,14,255,255,255,${MobAgro}"
		HUDPATH "Core" "zrad" "19,345,14,255,255,255,${zradius}"
		HUDPATH "Core" "cr1" "19,370,14,255,255,255,${combatRadius}"
		HUDPATH "Core" "spcount1" "19,389,14,255,255,255,${SpawnCount[npc agro ${MobAgro} targetable playerstate4]}"
		

		| ass..isting and stuff
		HUDPATH "Core" "ma11" "3,280,26,100,149,237,MA1"
		HUDPATH "Core" "ma12" "19,305,26,0,255,0,${If[${Bool[${MA1}]},${If[${Bool[${Spawn[${MA1}]}]},${MA1},]},]}"
		HUDPATH "Core" "ma13" "19,305,26,255,0,0,${If[${Bool[${MA1}]},${If[!${Bool[${Spawn[${MA1}]}]},${If[${Bool[${MA1}]},${MA1},]},]},]}"
		HUDPATH "Core" "ma14" "19,305,25,25,25,25,${If[${Bool[${MA1}]},,----]}"
		HUDPATH "Core" "ma21" "3,280,38,100,149,237,MA2"
		HUDPATH "Core" "ma22" "19,305,38,0,255,0,${If[${Bool[${MA2}]},${If[${Bool[${Spawn[${MA2}]}]},${MA2},]},]}"
		HUDPATH "Core" "ma23" "19,305,38,255,0,0,${If[${Bool[${MA2}]},${If[!${Bool[${Spawn[${MA2}]}]},${If[${Bool[${MA2}]},${MA2},]},]},]}"
		HUDPATH "Core" "ma24" "19,305,37,25,25,25,${If[${Bool[${MA2}]},,----]}"

		HUDPATH "Core" "ass1" "3,280,50,100,149,237,Assist"
		HUDPATH "Core" "ass2" "3,315,49,100,149,237,[           ]"
		HUDPATH "Core" "ass4" "19,320,50,0,255,0,${If[${useSmartAssistR},Raid,]}"
		HUDPATH "Core" "ass5" "19,320,50,0,255,0,${If[${useSmartAssist1},Raid1,]}"
		HUDPATH "Core" "ass6" "19,320,50,0,255,0,${If[${useSmartAssist2},Raid2,]}"
		HUDPATH "Core" "ass7" "19,320,50,0,255,0,${If[${useSmartAssist3},Raid3,]}"
		HUDPATH "Core" "ass8" "19,320,50,0,255,0,${If[${useSmartAssistG},Group,]}"
		HUDPATH "Core" "ass9" "19,320,50,0,255,0,${If[${useForceAssist},Force,]}"
		HUDPATH "Core" "ass10" "19,321,49,25,25,25,${If[!${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssist3} && !${useSmartAssistG} && !${useForceAssist},-------,]}"

		| buff switches
		HUDPATH "Core" "buff1" "3,280,62,100,149,237,Buff"
		HUDPATH "Core" "buff2" "3,305,61,100,149,237,[                         ]"
		HUDPATH "Core" "buff3" "19,310,62,0,255,0,${If[${switchBuffSelf},S,]}"
		HUDPATH "Core" "buff4" "19,310,62,25,25,25,${If[${switchBuffSelf},,S]} "
		HUDPATH "Core" "buff5" "19,319,62,0,255,0,${If[${switchBuffItem},I,]}"
		HUDPATH "Core" "buff6" "19,319,62,25,25,25,${If[${switchBuffItem},,I]} "
		HUDPATH "Core" "buff7" "19,324,62,0,255,0,${If[${switchBuffCombat},C,]}"
		HUDPATH "Core" "buff8" "19,324,62,25,25,25,${If[${switchBuffCombat},,C]} "
		HUDPATH "Core" "buff9" "19,332,62,0,255,0,${If[${switchBuffBc},B,]}"
		HUDPATH "Core" "buff10" "19,332,62,25,25,25,${If[${switchBuffBc},,B]}"
		HUDPATH "Core" "buff11" "19,341,62,0,255,0,${If[${switchBuffRaid},R,]}"
		HUDPATH "Core" "buff12" "19,341,62,25,25,25,${If[${switchBuffRaid},,R]}"
		HUDPATH "Core" "buff13" "19,349,62,0,255,0,${If[${switchBuffTell},T,]}"
		HUDPATH "Core" "buff14" "19,349,62,25,25,25,${If[${switchBuffTell},,T]}"
		HUDPATH "Core" "buff15" "19,357,62,0,255,0,${If[${switchBuffPet},P,]}"
		HUDPATH "Core" "buff16" "19,357,62,25,25,25,${If[${switchBuffPet},,P]}"
		HUDPATH "Core" "buff17" "19,365,62,0,255,0,${If[${switchBuffMerc},M,]}"
		HUDPATH "Core" "buff18" "19,365,62,25,25,25,${If[${switchBuffMerc},,M]}"
		HUDPATH "Core" "buff19" "19,374,62,0,255,0,${If[${switchBuffGroup},G,]}"
		HUDPATH "Core" "buff20" "19,374,62,25,25,25,${If[${switchBuffGroup},,G]}"

		| lazy ass controlls
		HUDPATH "Core" "rest0" "19,140,74,100,149,237,Rest"
		HUDPATH "Core" "rest2" "3,165,73,100,149,237,[     ]"
		HUDPATH "Core" "rest3" "19,170,74,255,255,255,${If[${autoRest},${autoRest},]}"
		HUDPATH "Core" "rest4" "19,170,74,25,25,25,${If[${autoRest},,##]}"

		| rest options
		HUDPATH "Core" "rest5" "3,187,73,100,149,237,[         ]"
		HUDPATH "Core" "rest6" "19,192,74,0,255,0,${If[${switchAutoRestFull},F,]}"
		HUDPATH "Core" "rest7" "19,192,74,25,25,25,${If[${switchAutoRestFull},,F]}"
		HUDPATH "Core" "rest8" "19,199,74,0,255,0,${If[${switchModRodinRest},M,]}"
		HUDPATH "Core" "rest9" "19,199,74,25,25,25,${If[${switchModRodinRest},,M]}"
		HUDPATH "Core" "rest10" "19,208,74,0,255,0,${If[${switchBuffinRest},B,]}"
		HUDPATH "Core" "rest11" "19,208,74,25,25,25,${If[${switchBuffinRest},,B]}"

		| deBuff switches
		HUDPATH "Core" "debuff0" "3,280,74,100,149,237,deBuff"
		HUDPATH "Core" "debuff1" "3,316,73,100,149,237,[                    ]"	
		HUDPATH "Core" "debuff2" "19,321,74,0,255,0,${If[${switchdeBuffMezz},Z,]}"
		HUDPATH "Core" "debuff3" "19,321,74,25,25,25,${If[${switchdeBuffMezz},,Z]}"	
		HUDPATH "Core" "debuff4" "19,329,74,0,255,0,${If[${switchdeBuffMalo},M,]}"
		HUDPATH "Core" "debuff5" "19,329,74,25,25,25,${If[${switchdeBuffMalo},,M]}"	
		HUDPATH "Core" "debuff6" "19,337,74,0,255,0,${If[${switchdeBuffTash},T,]}"
		HUDPATH "Core" "debuff7" "19,337,74,25,25,25,${If[${switchdeBuffTash},,T]}"	
		HUDPATH "Core" "debuff8" "19,345,74,0,255,0,${If[${switchdeBuffCripple},C,]}"
		HUDPATH "Core" "debuff9" "19,345,74,25,25,25,${If[${switchdeBuffCripple},,C]}"	
		HUDPATH "Core" "debuff10" "19,353,74,0,255,0,${If[${switchdeBuffSlow},S,]}"
		HUDPATH "Core" "debuff11" "19,353,74,25,25,25,${If[${switchdeBuffSlow},,S]}"		
		HUDPATH "Core" "debuff12" "19,361,74,0,255,0,${If[${switchdeBuffEradicate},E,]}"
		HUDPATH "Core" "debuff13" "19,361,74,25,25,25,${If[${switchdeBuffEradicate},,E]}"		
		HUDPATH "Core" "debuff14" "19,369,74,0,255,0,${If[${switchdeBuffSnare},R,]}"		
		HUDPATH "Core" "debuff15" "19,369,74,25,25,25,${If[${switchdeBuffSnare},,R]}"		
		
		| heal switches
		HUDPATH "Core" "heal2" "3,305,85,100,149,237,[     ][              ]"
		HUDPATH "Core" "heal3" "19,310,86,255,255,255,${If[${HealOverride},${HealOverride},]}"
		HUDPATH "Core" "heal4" "19,310,86,25,25,25,${If[${HealOverride},,##]}"
		HUDPATH "Core" "heal5" "19,331,86,0,255,0,${If[${switchHealSelf},S,]}"
		HUDPATH "Core" "heal6" "19,331,86,25,25,25,${If[${switchHealSelf},,S]}"
		HUDPATH "Core" "heal7" "19,339,86,0,255,0,${If[${switchHealGroup},G,]}"
		HUDPATH "Core" "heal8" "19,339,86,25,25,25,${If[${switchHealGroup},,G]}"
		HUDPATH "Core" "heal9" "19,347,86,0,255,0,${If[${switchHealXtarget},X,]}"
		HUDPATH "Core" "heal10" "19,347,86,25,25,25,${If[${switchHealXtarget},,X]}"
		HUDPATH "Core" "heal11" "19,355,86,0,255,0,${If[${switchHealPet},P,]}"
		HUDPATH "Core" "heal12" "19,355,86,25,25,25,${If[${switchHealPet},,P]}"
		HUDPATH "Core" "heal13" "19,363,86,0,255,0,${If[${switchHealCure},C,]}"
		HUDPATH "Core" "heal14" "19,363,86,25,25,25,${If[${switchHealCure},,C]}"
		HUDPATH "Core" "heal15" "19,280,86,100,149,237,${If[!${Select[${Me.Class.ShortName},CLR,SHM]},Heal,]}"
		HUDPATH "Core" "heal16" "19,280,86,255,0,0,${If[${Select[${Me.Class.ShortName},CLR,SHM]} && ${switchHealAggressive},Heal,]}"
		HUDPATH "Core" "heal17" "19,280,86,0,255,0,${If[${Select[${Me.Class.ShortName},CLR,SHM]} && !${switchHealAggressive},Heal,]}"

		| notice at bottom	
		HUDPATH "Core" "notice1" "3,5,131,100,149,237,[                                                                                                                   ]"
		HUDPATH "Core" "notice2" "19,10,132,255,255,255,${hudNotice}"

		HUDPATH "Core" "time1" "19,360,132,146,149,180,${If[${HUDTime} || !${coreBuild},${Time.Time24},${GameTime}]}"
		
	| elements HUD	
	} else /if (${_verbage.Equal[elements]}) {

		| target
		HUDPATH "Elements" "e.tar" "21,5,26,255,255,255,${Target.DisplayName}"


	}	else /if (${_verbage.Equal[debug]}) {

		| current line
		HUDPATH "Debug" "debug.cl1" "19,5,150,255,255,255,CL"
		HUDPATH "Debug" "debug.cl2" "19,30,150,100,149,237,${Macro.CurLine}"
		
		| runtime
		HUDPATH "Debug" "debug.rt1" "19,5,160,255,255,255,RT"
		HUDPATH "Debug" "debug.tr2" "19,30,160,100,149,237,${Macro.RunTime}"
	
		| target2kill
		HUDPATH "Debug" "debug.T2K1" "19,5,170,255,255,255,T2K"
		HUDPATH "Debug" "debug.T2K2" "19,30,170,100,149,237,${Target2Kill}"
		
		| zone
		HUDPATH "Debug" "debug.z" "19,330,150,100,149,237,${Zone.ShortName}"
		





	}
	
	
/return
