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
sub Bind_hud(string _hcType, string _hcVerbage)
	DEBUG ${cbug}Bind_hud()\ax

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

	| reset the MQhud ini file
	} else /if (${Bool[${_${_bind}Type.Find[update]}]}) {
		/call HUDUpdate
		
	| update hud broadcasting	
	} else /if (${Bool[${_${_bind}Type.Find[broadcast]}]}) {
		/call writeINI setBroadcast Common ${If[${Bool[${setBroadcast}]},FALSE,TRUE]}
		/if (${setBroadcast.Equal[FALSE]}) /ini "${INICore}" "transmit_status" "${Me.DisplayName}" "DISABLED"
		/if (${setBroadcast.Equal[TRUE]}) /ini "${INICore}" "transmit_status" "${Me.DisplayName}" "${coreAuto}"
		/call echos switch "Adjusting HUD Broadcast Status" setBroadcast

	| delete the hud
	} else /if (${Bool[${_${_bind}Type.Find[delete]}]}) {
		VOUT Deleteing HUD ${sep}
		/ini ".\..\MQ2HUD.ini" "core" NULL NULL
		VOUT Done ${sep}
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
sub HUDUpdate()
	DEBUG ${cbug}HUDUpdate()\ax
	VOUT Updating MQ2HUD.INI - > [core]
	/delay 5
	
	| Build
	/noparse /ini HUDPATH "core1" "3,5,26,255,255,255,Build"
	/noparse /ini HUDPATH "core2" "19,44,26,100,149,237,${coreBuild}"

	| last tell
	/noparse /ini HUDPATH "tell1" "3,5,38,255,255,255,Tell"
	/noparse /ini HUDPATH "tell2" "3,44,38,100,149,237,${MacroQuest.LastTell}"

	| control toon
	/noparse /ini HUDPATH "control1" "19,5,50,255,255,255,${If[${Bool[${autoControl}]},,Control]}"

	/noparse /ini HUDPATH "control2" "19,5,50,0,255,0,${If[${Bool[${autoControl}]},Control,]}"
	/noparse /ini HUDPATH "control3" "19,44,50,100,149,237,${If[${Bool[${toonControl}]},${toonControl},]}"

	| loc Y X Z
	/noparse /ini HUDPATH "loc1" "3,5,62,255,255,255,LOC"
	/noparse /ini HUDPATH "loc2" "3,44,62,100,149,237,${Int[${Me.Y}]}, ${Int[${Me.X}]}, ${Int[${Me.Z}]}"

	| mode
	/noparse /ini HUDPATH "mode0" "19,5,74,0,255,0,${If[${switchAutoCursor},Mode,]}"
	/noparse /ini HUDPATH "mode1" "19,5,74,255,0,0,${If[!${switchAutoCursor},Mode,]}"	
	/noparse /ini HUDPATH "mode2" "19,44,74,255,255,0,${If[${Bool[${tmpMode}]},${tmpMode},]}"
	/noparse /ini HUDPATH "mode3" "19,44,74,25,25,25,${If[${Bool[${tmpMode}]},,]}"

	| Target / LOS
	/noparse /ini HUDPATH "bodytype1" "3,5,85,100,149,237,[                       ]"
	/noparse /ini HUDPATH "bodytype2" "3,10,86,255,255,255,${Target.Body}"
	/noparse /ini HUDPATH "LoS1" "3,85,86,0,255,0,${If[${Bool[${Target}]},${If[${Target.LineOfSight},${Target.Distance} .. ID:${Target.ID},]},]}"
	/noparse /ini HUDPATH "LoS2" "3,85,86,255,0,0,${If[${Bool[${Target}]},${If[${Target.LineOfSight},,${Target.Distance} .. ID:${Target.ID}]},]}"

	| current target info
	| /noparse /ini HUDPATH "target1" "3,5,98,255,255,255,Target"
	/noparse /ini HUDPATH "target1" "3,70,97,100,149,237,${If[${Target.ID},[${Target.Class.ShortName} ${Target.Level}] ${Target.Name} ${If[${Bool[${Target.Guild}]},<${Target.Guild}>, ]}, ]}"
	
	| engage ##
	/noparse /ini HUDPATH "engage1" "3,5,97,100,149,237,[     ][     ][     ]"

	| assist ##
	/noparse /ini HUDPATH "engage2" "19,10,98,255,255,255,${If[(!${switchOverrideEngageat} && ${coreAuto} && !${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssist3} && !${useSmartAssistG} && (${Bool[${MA1}]} || ${Bool[${MA2}]}),${engageAt},]}"
	/noparse /ini HUDPATH "engage3" "19,10,98,255,255,255,${If[!${switchOverrideEngageat} && ${coreAuto} && (${useSmartAssist1} || ${useSmartAssist2} || ${useSmartAssist3} || ${useSmartAssistG}),${smartengageAt},]}"
	/noparse /ini HUDPATH "engage4" "19,10,98,25,25,25,${If[!${switchOverrideEngageat} && ${coreAuto} && !${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssist3} && !${useSmartAssistG} && !${Bool[${MA1}]} && !${Bool[${MA2}]},##,]}"
	/noparse /ini HUDPATH "engage6" "19,10,98,25,25,25,${If[!${coreAuto} && !${switchOverrideEngageat} && !${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssist3} && !${useSmartAssistG} && !${Bool[${MA1}]} && !${Bool[${MA2}]},##,]}"
	/noparse /ini HUDPATH "engage7" "19,12,98,255,0,0,${If[(${switchOverrideEngageat},X,]}"

	| pet engage ##
	/noparse /ini HUDPATH "pettarget1" "19,31,98,255,255,255,${If[${coreAuto} && !${switchOverrideEngageat} && ${switchPcPet},${PcPetengageat},]}"
	/noparse /ini HUDPATH "pettarget2" "19,31,98,25,25,25,${If[!${coreAuto} && !${switchOverrideEngageat} || !${switchPcPet},##,]}"
	/noparse /ini HUDPATH "pettarget3" "19,33,98,255,0,0,${If[${switchOverrideEngageat},X,]}"

	| swarmpet engage ##
	/noparse /ini HUDPATH "pettarget4" "19,52,98,255,255,255,${If[${coreAuto} && !${switchOverrideEngageat} && ${switchPcSwarm},${PcSwarmengageat},]}"
	/noparse /ini HUDPATH "pettarget5" "19,52,98,25,25,25,${If[(!${coreAuto} && !${switchOverrideEngageat}) || !${switchPcSwarm},##,]}"
	/noparse /ini HUDPATH "pettarget6" "19,55,98,255,0,0,${If[${switchOverrideEngageat},X,]}"

	| target effect notice
	/noparse /ini HUDPATH "tarnotice1" "3,5,109,100,149,237,[                                                                                                 ]"
	/noparse /ini HUDPATH "tarnotice2" "19,10,110,255,255,255,${hudTarNotice}"
	
	| watchspawn counter
	/noparse /ini HUDPATH "watchspawn1" "3,382,109,100,149,237,[       ]"
	/noparse /ini HUDPATH "watchspawn2" "19,387,110,0,255,0,${If[${Bool[${watchSpawn}]},${hudwatchspawn},]}"
	/noparse /ini HUDPATH "watchspawn3" "19,388,109,25,25,25,${If[${Bool[${watchSpawn}]},,----]}"

	| combat modes
	/noparse /ini HUDPATH "bot1" "3,140,13,100,149,237,[              ][    ][     ]"
	/noparse /ini HUDPATH "bot2" "19,145,14,0,255,0,${If[${coreAuto},A,]}"
	/noparse /ini HUDPATH "bot3" "19,145,14,25,25,25,${If[${coreAuto},,A]}"
	/noparse /ini HUDPATH "bot4" "19,153,14,0,255,0,${If[${switchCombatMelee},M,]}"
	/noparse /ini HUDPATH "bot5" "19,153,14,25,25,25,${If[${switchCombatMelee},,M]}"
	/noparse /ini HUDPATH "bot6" "19,161,14,0,255,0,${If[${switchCombatRange},R,]}"
	/noparse /ini HUDPATH "bot7" "19,161,14,25,25,25,${If[${switchCombatRange},,R]}"
	/noparse /ini HUDPATH "bot8" "19,169,14,0,255,0,${If[${switchCombatNuke},N,]}"
	/noparse /ini HUDPATH "bot9" "19,169,14,25,25,25,${If[${switchCombatNuke},,N]}"
	/noparse /ini HUDPATH "bot10" "19,177,14,0,255,0,${If[${switchCombatDoT},D,]}"
	/noparse /ini HUDPATH "bot11" "19,177,14,25,25,25,${If[${switchCombatDoT},,D]}"

	| combat distance
	/noparse /ini HUDPATH "CD1" "19,191,14,255,255,255,${CombatDistance}"
	
	| stick behind
	/noparse /ini HUDPATH "stickback1" "19,210,14,0,255,0,${If[${SetStickBehind},B,]}"
	/noparse /ini HUDPATH "stickback2" "19,210,14,25,25,25,${If[${SetStickBehind},,B]}"
	
	| face fast
	/noparse /ini HUDPATH "facefast1" "19,218,14,0,255,0,${If[${SetFaceFast},F,]}"
	/noparse /ini HUDPATH "facefast2" "19,218,14,25,25,25,${If[${SetFaceFast},,F]}"

	| stuff for camp status
	/noparse /ini HUDPATH "camp1" "3,140,26,100,149,237,R          L"
	/noparse /ini HUDPATH "camp2" "3,150,25,100,149,237,[     ]     [     ]"
	/noparse /ini HUDPATH "camp3" "19,155,26,0,255,0,${If[${HomeSet},${HomeRadius},]}"
	/noparse /ini HUDPATH "camp4" "19,190,26,0,255,0,${If[${HomeSet},${HomeLeash},]}"
	/noparse /ini HUDPATH "camp5" "19,155,26,25,25,25,${If[${HomeSet},,##        ##]}"

	| tie info
	/noparse /ini HUDPATH "tie1" "19,140,38,100,149,237,${SetTieMode}"
	/noparse /ini HUDPATH "tie2" "3,165,37,100,149,237,[     ][     ]"
	/noparse /ini HUDPATH "tie3" "19,170,38,255,255,255,${If[${SetTie},${SetTieDistance},]}"
	/noparse /ini HUDPATH "tie4" "19,191,38,255,255,255,${If[${SetTie},${tCheck_Tie},]}"
	/noparse /ini HUDPATH "tie5" "19,191,38,25,25,25,${If[${SetTie},,##]}"
	/noparse /ini HUDPATH "tie6" "19,170,38,25,25,25,${If[${SetTie},,##]}"
	/noparse /ini HUDPATH "tie7" "19,209,38,0,255,0,${If[${Bool[${SetTieToon}]},${If[${Bool[${Spawn[${SetTieToon}]}]},${SetTieToon},]},]}"
	/noparse /ini HUDPATH "tie8" "19,209,38,255,0,0,${If[${Bool[${SetTieToon}]},${If[!${Bool[${Spawn[${SetTieToon}]}]},${If[${Bool[${SetTieToon}]},${SetTieToon},]},]},]}"
	/noparse /ini HUDPATH "tie9" "19,209,37,25,25,25,${If[${Bool[${SetTieToon}]},,----]}"

	| burn a thing
	/noparse /ini HUDPATH "burn1" "19,140,50,0,255,0,${If[${burnAuto},Burn,]}"
	/noparse /ini HUDPATH "burn2" "19,140,50,255,0,0,${If[${burnAuto},,Burn]}"
	/noparse /ini HUDPATH "burn3" "3,170,49,100,149,237,[     ][     ]"
	/noparse /ini HUDPATH "burn4" "19,175,50,255,255,255,${If[!${burnForce} && ${burnCount},${burnCount},]}"
	/noparse /ini HUDPATH "burn5" "19,175,50,25,25,25,${If[!${burnForce} && !${burnCount},##,]}"
	/noparse /ini HUDPATH "burn6" "19,177,50,255,0,0,${If[${burnForce},X,]}"
	/noparse /ini HUDPATH "burn7" "19,196,50,255,255,255,${If[!${burnForce} && ${burnEngageat},${burnEngageat},]}"
	/noparse /ini HUDPATH "burn8" "19,196,50,25,25,25,${If[!${burnForce} && !${burnEngageat},##,]}"
	/noparse /ini HUDPATH "burn9" "19,199,50,255,0,0,${If[${burnForce},X,]}"
		
	| rez
	/noparse /ini HUDPATH "rez1" "3,140,62,100,149,237,Rez"
	/noparse /ini HUDPATH "rez2" "19,162,61,255,0,0,${If[${switchRezTake},,[         ]]}"
	/noparse /ini HUDPATH "rez3" "19,162,61,0,255,0,${If[${switchRezTake},[         ],]}"
	/noparse /ini HUDPATH "rez4" "19,197,61,255,0,0,${If[${switchRezGive},,[      ]]}"
	/noparse /ini HUDPATH "rez5" "19,197,61,0,255,0,${If[${switchRezGive},[      ][       ],]}"
	/noparse /ini HUDPATH "rez6" "19,167,62,255,255,255,${If[${coreBuild},${hudDeathCount},]}"
	/noparse /ini HUDPATH "rez7" "19,200,62,0,255,0,${If[${switchRezCombat},C,]}"
	/noparse /ini HUDPATH "rez8" "19,200,62,25,25,25,${If[${switchRezCombat},,C]}"
	/noparse /ini HUDPATH "rez9" "19,208,62,0,255,0,${If[${switchRezEveryone},E,]}"
	/noparse /ini HUDPATH "rez10" "19,208,62,25,25,25,${If[${switchRezEveryone},,E]}"
	/noparse /ini HUDPATH "rez11" "19,215,62,0,255,0,${If[${switchRezEmergency},!,]}"
	/noparse /ini HUDPATH "rez12" "19,215,62,25,25,25,${If[${switchRezEmergency},,!]}"
	/noparse /ini HUDPATH "rez13" "19,225,62,255,255,255,${If[${switchRezGive},${maxRezRange},]}"

	| HUD WARNING/OVERIDE notices
	/noparse /ini HUDPATH "hudwarning0" "3,110,1,100,149,237,[                                                     ]"
	/noparse /ini HUDPATH "hudwarning1" "19,115,2,240,0,0,${If[${Bool[${hudWarning}]},${hudWarning},]}"

	| group broadcast stauts
	/noparse /ini HUDPATH "hudbroadcast0" "19,280,1,0,255,0,${If[${Bool[${setBroadcast}]},[                                         ],]}"
	/noparse /ini HUDPATH "hudbroadcast1" "19,280,1,255,0,0,${If[${Bool[${setBroadcast}]},,[                                         ]]}"
	/noparse /ini HUDPATH "hudbroadcast2" "19,285,2,100,149,237,${hudBroadcast}"

	| agro notices and puler status
	/noparse /ini HUDPATH "agro0" "3,280,13,100,149,237,[       ][       ][       ][    ]"
	/noparse /ini HUDPATH "agro1" "19,286,14,0,255,0,${If[${switchAgro},A,]}"
	/noparse /ini HUDPATH "agro2" "19,286,14,25,25,25,${If[${switchAgro},,A]}"
	/noparse /ini HUDPATH "pull1" "19,295,14,0,255,0,${If[${switchPull},P,]}"
	/noparse /ini HUDPATH "pull2" "19,295,14,25,25,25,${If[${switchPull},,P]}"	
	/noparse /ini HUDPATH "agro4" "19,312,14,255,255,255,${MobAgro}"
	/noparse /ini HUDPATH "zrad" "19,339,14,255,255,255,${zradius}"
	/noparse /ini HUDPATH "cr1" "19,364,14,255,255,255,${combatRadius}"

	| ass..isting and stuff
	/noparse /ini HUDPATH "ma11" "3,280,26,100,149,237,MA1"
	/noparse /ini HUDPATH "ma12" "19,305,26,0,255,0,${If[${Bool[${MA1}]},${If[${Bool[${Spawn[${MA1}]}]},${MA1},]},]}"
	/noparse /ini HUDPATH "ma13" "19,305,26,255,0,0,${If[${Bool[${MA1}]},${If[!${Bool[${Spawn[${MA1}]}]},${If[${Bool[${MA1}]},${MA1},]},]},]}"
	/noparse /ini HUDPATH "ma14" "19,305,25,25,25,25,${If[${Bool[${MA1}]},,----]}"
	/noparse /ini HUDPATH "ma21" "3,280,38,100,149,237,MA2"
	/noparse /ini HUDPATH "ma22" "19,305,38,0,255,0,${If[${Bool[${MA2}]},${If[${Bool[${Spawn[${MA2}]}]},${MA2},]},]}"
	/noparse /ini HUDPATH "ma23" "19,305,38,255,0,0,${If[${Bool[${MA2}]},${If[!${Bool[${Spawn[${MA2}]}]},${If[${Bool[${MA2}]},${MA2},]},]},]}"
	/noparse /ini HUDPATH "ma24" "19,305,37,25,25,25,${If[${Bool[${MA2}]},,----]}"

	/noparse /ini HUDPATH "ass1" "3,280,50,100,149,237,Assist"
	/noparse /ini HUDPATH "ass2" "3,315,49,100,149,237,[           ]"
	/noparse /ini HUDPATH "ass4" "19,320,50,0,255,0,${If[${useSmartAssistR},Raid,]}"
	/noparse /ini HUDPATH "ass5" "19,320,50,0,255,0,${If[${useSmartAssist1},Raid1,]}"
	/noparse /ini HUDPATH "ass6" "19,320,50,0,255,0,${If[${useSmartAssist2},Raid2,]}"
	/noparse /ini HUDPATH "ass7" "19,320,50,0,255,0,${If[${useSmartAssist3},Raid3,]}"
	/noparse /ini HUDPATH "ass8" "19,320,50,0,255,0,${If[${useSmartAssistG},Group,]}"
	/noparse /ini HUDPATH "ass9" "19,320,50,0,255,0,${If[${useForceAssist},Force,]}"
	/noparse /ini HUDPATH "ass10" "19,321,49,25,25,25,${If[!${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssist3} && !${useSmartAssistG} && !${useForceAssist},-------,]}"

	| buff switches
	/noparse /ini HUDPATH "buff1" "3,280,62,100,149,237,Buff"
	/noparse /ini HUDPATH "buff2" "3,305,61,100,149,237,[                         ]"
	/noparse /ini HUDPATH "buff3" "19,310,62,0,255,0,${If[${switchBuffSelf},S,]}"
	/noparse /ini HUDPATH "buff4" "19,310,62,25,25,25,${If[${switchBuffSelf},,S]} "
	/noparse /ini HUDPATH "buff5" "19,319,62,0,255,0,${If[${switchBuffItem},I,]}"
	/noparse /ini HUDPATH "buff6" "19,319,62,25,25,25,${If[${switchBuffItem},,I]} "
	/noparse /ini HUDPATH "buff7" "19,324,62,0,255,0,${If[${switchBuffCombat},C,]}"
	/noparse /ini HUDPATH "buff8" "19,324,62,25,25,25,${If[${switchBuffCombat},,C]} "
	/noparse /ini HUDPATH "buff9" "19,332,62,0,255,0,${If[${switchBuffBc},B,]}"
	/noparse /ini HUDPATH "buff10" "19,332,62,25,25,25,${If[${switchBuffBc},,B]}"
	/noparse /ini HUDPATH "buff11" "19,341,62,0,255,0,${If[${switchBuffRaid},R,]}"
	/noparse /ini HUDPATH "buff12" "19,341,62,25,25,25,${If[${switchBuffRaid},,R]}"
	/noparse /ini HUDPATH "buff13" "19,349,62,0,255,0,${If[${switchBuffTell},T,]}"
	/noparse /ini HUDPATH "buff14" "19,349,62,25,25,25,${If[${switchBuffTell},,T]}"
	/noparse /ini HUDPATH "buff15" "19,357,62,0,255,0,${If[${switchBuffPet},P,]}"
	/noparse /ini HUDPATH "buff16" "19,357,62,25,25,25,${If[${switchBuffPet},,P]}"
	/noparse /ini HUDPATH "buff17" "19,365,62,0,255,0,${If[${switchBuffMerc},M,]}"
	/noparse /ini HUDPATH "buff18" "19,365,62,25,25,25,${If[${switchBuffMerc},,M]}"
	/noparse /ini HUDPATH "buff19" "19,374,62,0,255,0,${If[${switchBuffGroup},G,]}"
	/noparse /ini HUDPATH "buff20" "19,374,62,25,25,25,${If[${switchBuffGroup},,G]}"

	| lazy ass controlls
	/noparse /ini HUDPATH "rest0" "19,140,74,100,149,237,Rest"
	/noparse /ini HUDPATH "rest2" "3,165,73,100,149,237,[     ]"
	/noparse /ini HUDPATH "rest3" "19,170,74,255,255,255,${If[${autoRest},${autoRest},]}"
	/noparse /ini HUDPATH "rest4" "19,170,74,25,25,25,${If[${autoRest},,##]}"

	| rest options
	/noparse /ini HUDPATH "rest5" "3,187,73,100,149,237,[         ]"
	/noparse /ini HUDPATH "rest6" "19,192,74,0,255,0,${If[${switchAutoRestFull},F,]}"
	/noparse /ini HUDPATH "rest7" "19,192,74,25,25,25,${If[${switchAutoRestFull},,F]}"
	/noparse /ini HUDPATH "rest8" "19,199,74,0,255,0,${If[${switchModRodinRest},M,]}"
	/noparse /ini HUDPATH "rest9" "19,199,74,25,25,25,${If[${switchModRodinRest},,M]}"
	/noparse /ini HUDPATH "rest10" "19,208,74,0,255,0,${If[${switchBuffinRest},B,]}"
	/noparse /ini HUDPATH "rest11" "19,208,74,25,25,25,${If[${switchBuffinRest},,B]}"

	| deBuff switches
	/noparse /ini HUDPATH "debuff0" "3,280,74,100,149,237,deBuff"
	/noparse /ini HUDPATH "debuff1" "3,316,73,100,149,237,[                    ]"	
	/noparse /ini HUDPATH "debuff2" "19,321,74,0,255,0,${If[${switchdeBuffMezz},Z,]}"
	/noparse /ini HUDPATH "debuff3" "19,321,74,25,25,25,${If[${switchdeBuffMezz},,Z]}"	
	/noparse /ini HUDPATH "debuff4" "19,329,74,0,255,0,${If[${switchdeBuffMalo},M,]}"
	/noparse /ini HUDPATH "debuff5" "19,329,74,25,25,25,${If[${switchdeBuffMalo},,M]}"	
	/noparse /ini HUDPATH "debuff6" "19,337,74,0,255,0,${If[${switchdeBuffTash},T,]}"
	/noparse /ini HUDPATH "debuff7" "19,337,74,25,25,25,${If[${switchdeBuffTash},,T]}"	
	/noparse /ini HUDPATH "debuff8" "19,345,74,0,255,0,${If[${switchdeBuffCripple},C,]}"
	/noparse /ini HUDPATH "debuff9" "19,345,74,25,25,25,${If[${switchdeBuffCripple},,C]}"	
	/noparse /ini HUDPATH "debuff10" "19,353,74,0,255,0,${If[${switchdeBuffSlow},S,]}"
	/noparse /ini HUDPATH "debuff11" "19,353,74,25,25,25,${If[${switchdeBuffSlow},,S]}"		
	/noparse /ini HUDPATH "debuff12" "19,361,74,0,255,0,${If[${switchdeBuffEradicate},E,]}"
	/noparse /ini HUDPATH "debuff13" "19,361,74,25,25,25,${If[${switchdeBuffEradicate},,E]}"		
	/noparse /ini HUDPATH "debuff14" "19,369,74,0,255,0,${If[${switchdeBuffSnare},R,]}"		
	/noparse /ini HUDPATH "debuff15" "19,369,74,25,25,25,${If[${switchdeBuffSnare},,R]}"		
	
	| heal switches
	/noparse /ini HUDPATH "heal2" "3,305,85,100,149,237,[     ][              ]"
	/noparse /ini HUDPATH "heal3" "19,310,86,255,255,255,${If[${HealOverride},${HealOverride},]}"
	/noparse /ini HUDPATH "heal4" "19,310,86,25,25,25,${If[${HealOverride},,##]}"
	/noparse /ini HUDPATH "heal5" "19,331,86,0,255,0,${If[${switchHealSelf},S,]}"
	/noparse /ini HUDPATH "heal6" "19,331,86,25,25,25,${If[${switchHealSelf},,S]}"
	/noparse /ini HUDPATH "heal7" "19,339,86,0,255,0,${If[${switchHealGroup},G,]}"
	/noparse /ini HUDPATH "heal8" "19,339,86,25,25,25,${If[${switchHealGroup},,G]}"
	/noparse /ini HUDPATH "heal9" "19,347,86,0,255,0,${If[${switchHealXtarget},X,]}"
	/noparse /ini HUDPATH "heal10" "19,347,86,25,25,25,${If[${switchHealXtarget},,X]}"
	/noparse /ini HUDPATH "heal11" "19,355,86,0,255,0,${If[${switchHealPet},P,]}"
	/noparse /ini HUDPATH "heal12" "19,355,86,25,25,25,${If[${switchHealPet},,P]}"
	/noparse /ini HUDPATH "heal13" "19,363,86,0,255,0,${If[${switchHealCure},C,]}"
	/noparse /ini HUDPATH "heal14" "19,363,86,25,25,25,${If[${switchHealCure},,C]}"
	/noparse /ini HUDPATH "heal15" "19,280,86,100,149,237,${If[!${Select[${Me.Class.ShortName},CLR,SHM]},Heal,]}"
	/noparse /ini HUDPATH "heal16" "19,280,86,255,0,0,${If[${Select[${Me.Class.ShortName},CLR,SHM]} && ${switchHealAggressive},Heal,]}"
	/noparse /ini HUDPATH "heal17" "19,280,86,0,255,0,${If[${Select[${Me.Class.ShortName},CLR,SHM]} && !${switchHealAggressive},Heal,]}"

	| notice at bottom	
	/noparse /ini HUDPATH "notice1" "3,5,131,100,149,237,[                                                                                                                   ]"
	/noparse /ini HUDPATH "notice2" "19,10,132,255,255,255,${hudNotice}"

	/noparse /ini HUDPATH "time1" "19,360,132,146,149,180,${If[${HUDTime} || !${coreBuild},${Time.Time24},${GameTime}]}"
	
	VOUT MQ2HUD.INI update complete
/return