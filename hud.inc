|***
 *	hud.inc
 *	Core HUD Include
 *	hard coded top left corner to surround lag-O-meter(tm)
 * 
 ***|



|***
 *	DES: Controlls for the HUD
 *	USE: /hud [time|update]
 *	INI: 
 *  NOTE:
 ***| 
#bind hud /hc
sub Bind_hud(string _hcType)
	DEBUG ${cbug}Bind_hud()\ax
	
	| adjust hud clock
	/if (${Bool[${_hcType.Find[time]}]}) {
		/varset HUDTime ${If[${HUDTime},FALSE,TRUE]}
		/if (${HUDTime}) VOUT Hud Showing ${cinfo}Real Life\ax time.
		/if (!${HUDTime}) VOUT Hud Showing ${cinfo}In Game\ax time.

	| reset the MQhud ini file
	} else /if (${Bool[${_hcType.Find[update]}]}) {
		/call HUDUpdate
		
	| update hud broadcasting	
	} else /if (${Bool[${_hcType.Find[broadcast]}]}) {
		/call Event_SetVar NA setBroadcast Common ${If[${Bool[${setBroadcast}]},FALSE,TRUE]}
		/if (${setBroadcast.Equal[FALSE]}) /ini "${INICore}" "transmit_status" "${Me.DisplayName}" "DISABLED"
		/if (${setBroadcast.Equal[TRUE]}) /ini "${INICore}" "transmit_status" "${Me.DisplayName}" "${AutoBot}"
		/call echos switch "Adjusting Broadcast Status" setBroadcast
	} else {
		
		/call echos listtype hc hud
	} 
/return












|***
 *	DES: HUD adjustment
 *	USE: 
 *	INI: 
 *  NOTE:
 ***|
sub HUDUpdate(string Line)
   
	NOUT This will only take a second \at...\ax
	/ini ".\..\MQ2HUD.ini" "core" NULL NULL
	/delay 1s
	
	/noparse /ini HUDPATH "core1" "3,5,26,255,255,255,Core"
	/noparse /ini HUDPATH "core2" "3,44,26,100,149,237,${If[${versionCore},${versionCore},]}"
	/noparse /ini HUDPATH "core3" "3,84,26,100,149,237,${If[${version${Me.Class.ShortName}},${version${Me.Class.ShortName}},]}"
	/noparse /ini HUDPATH "core4" "3,44,26,255,0,0,${If[${versionCore},,OFF]}"

	| last tell
	/noparse /ini HUDPATH "tell1" "3,5,38,255,255,255,Tell"
	/noparse /ini HUDPATH "tell2" "3,44,38,100,149,237,${MacroQuest.LastTell}"

	| control toon
	/noparse /ini HUDPATH "control1" "3,5,50,255,255,255,${If[${Bool[${autoControl}]},,Control]}"
	/noparse /ini HUDPATH "control2" "3,5,50,0,255,0,${If[${Bool[${autoControl}]},Control,]}"
	/noparse /ini HUDPATH "control3" "3,44,50,100,149,237,${If[${Bool[${toonControl}]},${toonControl},]}"

	| loc Y X Z
	/noparse /ini HUDPATH "loc1" "3,5,62,255,255,255,LOC"
	/noparse /ini HUDPATH "loc2" "3,44,62,100,149,237,${Int[${Me.Y}]}, ${Int[${Me.X}]}, ${Int[${Me.Z}]}"

	| mode
	/noparse /ini HUDPATH "mode0" "3,5,74,0,255,0,${If[${switchAutoCursor} && !${timer_auto_cursor_pause},Mode,]}"
	/noparse /ini HUDPATH "mode1" "3,5,74,255,0,0,${If[!${switchAutoCursor} || (${switchAutoCursor} && ${timer_auto_cursor_pause}),Mode,]}"	
	/noparse /ini HUDPATH "mode2" "3,44,74,255,255,0,${If[${Bool[${tmpMode}]},${tmpMode},]}"
	/noparse /ini HUDPATH "mode3" "3,44,74,25,25,25,${If[${Bool[${tmpMode}]},,NONE]}"

	| distance and LOS
	/noparse /ini HUDPATH "LoS1" "3,5,86,255,255,255,LoS"
	/noparse /ini HUDPATH "LoS2" "3,44,86,0,255,0,${If[${Bool[${Target}]},${If[${Target.LineOfSight},dis:${Target.Distance} ID:${Target.ID},]},]}"
	/noparse /ini HUDPATH "LoS3" "3,44,86,255,0,0,${If[${Bool[${Target}]},${If[${Target.LineOfSight},,dis:${Target.Distance} ID:${Target.ID}]},]}"

	| current target info
	/noparse /ini HUDPATH "target1" "3,5,98,255,255,255,Target"
	/noparse /ini HUDPATH "target2" "3,103,97,100,149,237,${If[${Target.ID},[${Target.Class.ShortName} ${Target.Level}] ${Target.Name} ${If[${Bool[${Target.Guild}]},<${Target.Guild}>, ]}, ]}"
	/noparse /ini HUDPATH "target3" "3,39,97,100,149,237,[     ][     ][     ]"

	| engage ##
	/noparse /ini HUDPATH "engage1" "3,44,98,255,255,255,${If[(!${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG} && !${switchOverrideEngageat} && (${Bool[${MA1}]} || ${Bool[${MA2}]}),${engageAt},]}"
	/noparse /ini HUDPATH "engage2" "3,46,98,255,0,0,${If[(!${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG} && ${switchOverrideEngageat} && (${Bool[${MA1}]} || ${Bool[${MA2}]}),X,]}"
	/noparse /ini HUDPATH "engage3" "3,44,98,255,255,255,${If[(${useSmartAssist1} || ${useSmartAssist2} || ${useSmartAssistG}) && !${switchOverrideEngageat},${smartengageAt},]}"
	/noparse /ini HUDPATH "engage4" "3,46,98,255,0,0,${If[(${useSmartAssist1} || ${useSmartAssist2} || ${useSmartAssistG}) && ${switchOverrideEngageat},X,]}"
	/noparse /ini HUDPATH "engage5" "3,44,98,25,25,25,${If[${useSmartAssist1} || ${useSmartAssist2} || ${useSmartAssistG} || ${Bool[${MA1}]} || ${Bool[${MA2}]},,##]}"

	| pet engage ##
	/noparse /ini HUDPATH "pettarget1" "3,65,98,255,255,255,${If[${switchPcPet} && !${switchOverrideEngageat},${PcPetengageat},]}"
	/noparse /ini HUDPATH "pettarget2" "3,67,98,255,0,0,${If[${switchPcPet} && ${switchOverrideEngageat},X,]}"
	/noparse /ini HUDPATH "pettarget3" "3,65,98,25,25,25,${If[${switchPcPet},,##]}"

	| swarmpet engage ##
	/noparse /ini HUDPATH "pettarget4" "3,86,98,255,255,255,${If[${switchPcSwarm} && !${switchOverrideEngageat},${PcSwarmengageat},]}"
	/noparse /ini HUDPATH "pettarget5" "3,88,98,255,0,0,${If[${switchPcSwarm} && ${switchOverrideEngageat},X,]}"
	/noparse /ini HUDPATH "pettarget6" "3,86,98,25,25,25,${If[${switchPcSwarm},,##]}"

	| target effect notice
	/noparse /ini HUDPATH "tarnotice1" "3,39,109,100,149,237,[                                                                                                        ][       ]"
	/noparse /ini HUDPATH "tarnotice2" "3,45,110,255,255,255,${hudTarNotice}"
	/noparse /ini HUDPATH "watchspawn1" "3,362,110,0,255,0,${If[${Bool[${watchSpawn}]},${hudwatchspawn},]}"
	/noparse /ini HUDPATH "watchspawn2" "3,362,110,25,25,25,${If[${Bool[${watchSpawn}]},,###]}"

	| combat modes
	/noparse /ini HUDPATH "bot1" "3,140,13,100,149,237,[              ][    ][     ]"
	/noparse /ini HUDPATH "bot2" "3,145,14,0,255,0,${If[${AutoBot} && ${combat.Equal[MELEE]},M,]}"
	/noparse /ini HUDPATH "bot3" "3,145,14,25,25,25,${If[!${AutoBot} || (!${AutoBot} && !${combat.Equal[MELEE]}) || (${AutoBot} && !${combat.Equal[MELEE]}),M,]}"
	/noparse /ini HUDPATH "bot4" "3,153,14,0,255,0,${If[${AutoBot} && ${combat.Equal[RANGE]},R,]}"
	/noparse /ini HUDPATH "bot5" "3,153,14,25,25,25,${If[!${AutoBot} || (!${AutoBot} && !${combat.Equal[RANGE]}) || (${AutoBot} && !${combat.Equal[RANGE]}),R,]}"
	/noparse /ini HUDPATH "bot6" "3,161,14,0,255,0,${If[${AutoBot} && ${combat.Equal[HEAL]},H,]}"
	/noparse /ini HUDPATH "bot7" "3,161,14,25,25,25,${If[!${AutoBot} || (!${AutoBot} && !${combat.Equal[HEAL]}) || (${AutoBot} && !${combat.Equal[HEAL]}),H,]}"
	/noparse /ini HUDPATH "bot8" "3,169,14,0,255,0,${If[${AutoBot} && ${combat.Equal[NUKE]},N,]}"
	/noparse /ini HUDPATH "bot9" "3,169,14,25,25,25,${If[!${AutoBot} || (!${AutoBot} && !${combat.Equal[NUKE]}) || (${AutoBot} && !${combat.Equal[NUKE]}),N,]}"
	/noparse /ini HUDPATH "bot10" "3,177,14,0,255,0,${If[${AutoBot} && ${combat.Equal[DUAL]},D,]}"
	/noparse /ini HUDPATH "bot11" "3,177,14,25,25,25,${If[!${AutoBot} || (!${AutoBot} && !${combat.Equal[DUAL]}) || (${AutoBot} && !${combat.Equal[DUAL]}),D,]}"
	
	| combat distance
	/noparse /ini HUDPATH "CD1" "3,191,14,255,255,255,${CombatDistance}"
	
	| stick behind
	/noparse /ini HUDPATH "stickback1" "3,210,14,0,255,0,${If[${SetStickBehind},B,]}"
	/noparse /ini HUDPATH "stickback2" "3,210,14,25,25,25,${If[${SetStickBehind},,B]}"
	
	| face fast
	/noparse /ini HUDPATH "facefast1" "3,218,14,0,255,0,${If[${SetFaceFast},F,]}"
	/noparse /ini HUDPATH "facefast2" "3,218,14,25,25,25,${If[${SetFaceFast},,F]}"

	| stuff for camp status
	/noparse /ini HUDPATH "camp1" "3,140,26,100,149,237,R          L"
	/noparse /ini HUDPATH "camp2" "3,150,25,100,149,237,[     ]     [     ]"
	/noparse /ini HUDPATH "camp3" "3,155,26,0,255,0,${If[${HomeSet},${HomeRadius},]}"
	/noparse /ini HUDPATH "camp4" "3,190,26,0,255,0,${If[${HomeSet},${HomeLeash},]}"
	/noparse /ini HUDPATH "camp5" "3,155,26,25,25,25,${If[${HomeSet},,##        ##]}"

	| tie info
	/noparse /ini HUDPATH "tie1" "3,140,38,100,149,237,Tie"
	/noparse /ini HUDPATH "tie2" "3,157,37,100,149,237,[     ][     ]"
	/noparse /ini HUDPATH "tie3" "3,162,38,255,255,255,${If[${SetTie},${SetTieDistance},]}"
	/noparse /ini HUDPATH "tie4" "3,183,38,255,255,255,${If[${SetTie},${tCheck_Tie},]}"
	/noparse /ini HUDPATH "tie5" "3,183,38,25,25,25,${If[${SetTie},,##]}"
	/noparse /ini HUDPATH "tie6" "3,162,38,25,25,25,${If[${SetTie},,##]}"
	/noparse /ini HUDPATH "tie7" "3,201,38,0,255,0,${If[${Bool[${SetTieToon}]},${If[${Bool[${Spawn[${SetTieToon}]}]},${SetTieToon},]},]}"
	/noparse /ini HUDPATH "tie8" "3,201,38,255,0,0,${If[${Bool[${SetTieToon}]},${If[!${Bool[${Spawn[${SetTieToon}]}]},${If[${Bool[${SetTieToon}]},${SetTieToon},]},]},]}"
	/noparse /ini HUDPATH "tie9" "3,201,38,25,25,25,${If[${Bool[${SetTieToon}]},,NONE]}"

	| toasting preferences
	/noparse /ini HUDPATH "toast1" "3,140,50,0,255,0,${If[${AutoToast} && !${switchToastMode},Toast,]}"
	/noparse /ini HUDPATH "toast2" "3,140,50,255,255,0,${If[${switchToastMode},Toast,]}"
	/noparse /ini HUDPATH "toast3" "3,140,50,255,0,0,${If[${switchToastMode},,Toast]}"
	/noparse /ini HUDPATH "toast4" "3,170,49,100,149,237,[     ][     ]"
	/noparse /ini HUDPATH "toast5" "3,175,50,255,255,255,${If[${toastMobCount},${toastMobCount},]}"
	/noparse /ini HUDPATH "toast6" "3,175,50,25,25,25,${If[${toastMobCount},,##]}"
	/noparse /ini HUDPATH "toast7" "3,196,50,255,255,255,${If[${whentoToast},${whentoToast},]}"
	/noparse /ini HUDPATH "toast8" "3,196,50,25,25,25,${If[${whentoToast},,##]}"
		
	| rez
	/noparse /ini HUDPATH "rez1" "3,140,62,100,149,237,Rez"
	/noparse /ini HUDPATH "rez2" "3,162,61,255,0,0,${If[${switchRezTake},,[         ]]}"
	/noparse /ini HUDPATH "rez3" "3,162,61,0,255,0,${If[${switchRezTake},[         ],]}"
	/noparse /ini HUDPATH "rez4" "3,197,61,255,0,0,${If[${switchRezGive},,[      ]]}"
	/noparse /ini HUDPATH "rez5" "3,197,61,0,255,0,${If[${switchRezGive},[      ],]}"
	/noparse /ini HUDPATH "rez6" "3,167,62,255,255,255,${If[${versionCore},${Int[${Ini[${INICore},deathcount,${Me.DisplayName}]}]},]}"
	/noparse /ini HUDPATH "rez7" "3,200,62,0,255,0,${If[${switchRezCombat},C,]}"
	/noparse /ini HUDPATH "rez8" "3,200,62,25,25,25,${If[${switchRezCombat},,C]}"
	/noparse /ini HUDPATH "rez9" "3,208,62,0,255,0,${If[${switchRezEveryone},E,]}"
	/noparse /ini HUDPATH "rez10" "3,208,62,25,25,25,${If[${switchRezEveryone},,E]}"
	/noparse /ini HUDPATH "rez11" "3,215,62,0,255,0,${If[${switchRezEmergency},!,]}"
	/noparse /ini HUDPATH "rez12" "3,215,62,25,25,25,${If[${switchRezEmergency},,!]}"

	| HUD WARNING/OVERIDE notices
	/noparse /ini HUDPATH "hudwarning0" "3,110,1,100,149,237,[                                                   ]"
	/noparse /ini HUDPATH "hudwarning1" "3,115,2,240,0,0,${If[${Bool[${hudWarning}]},${hudWarning},]}"

	| group broadcast stauts
	/noparse /ini HUDPATH "hudbroadcast0" "3,280,1,0,255,0,${If[${Bool[${setBroadcast}]},[                                         ],]}"
	/noparse /ini HUDPATH "hudbroadcast1" "3,280,1,255,0,0,${If[${Bool[${setBroadcast}]},,[                                         ]]}"
	/noparse /ini HUDPATH "hudbroadcast2" "3,285,2,100,149,237,${hudBroadcast}"

	| agro notices and puler status
	/noparse /ini HUDPATH "agro0" "3,280,13,100,149,237,[       ][       ][       ][    ]"
	/noparse /ini HUDPATH "agro1" "3,286,14,0,255,0,${If[${switchAgroMode},A,]}"
	/noparse /ini HUDPATH "agro2" "3,286,14,255,0,0,${If[${switchAgroMode},,A]}"
	/noparse /ini HUDPATH "pull1" "3,295,14,0,255,0,${If[${switchPullMode},P,]}"
	/noparse /ini HUDPATH "pull2" "3,295,14,255,0,0,${If[${switchPullMode},,P]}"	
	/noparse /ini HUDPATH "agro4" "3,312,14,255,255,255,${MobAgro}"
	/noparse /ini HUDPATH "zrad" "3,339,14,255,255,255,${zradius}"
	/noparse /ini HUDPATH "cr1" "3,364,14,100,149,237,${combatRadius}"

	| ass..isting and stuff
	/noparse /ini HUDPATH "ma11" "3,280,26,100,149,237,MA1"
	/noparse /ini HUDPATH "ma12" "3,305,26,0,255,0,${If[${Bool[${MA1}]},${If[${Bool[${Spawn[${MA1}]}]},${MA1},]},]}"
	/noparse /ini HUDPATH "ma13" "3,305,26,255,0,0,${If[${Bool[${MA1}]},${If[!${Bool[${Spawn[${MA1}]}]},${If[${Bool[${MA1}]},${MA1},]},]},]}"
	/noparse /ini HUDPATH "ma14" "3,305,26,25,25,25,${If[${Bool[${MA1}]},,NONE]}"
	/noparse /ini HUDPATH "ma21" "3,280,38,100,149,237,MA2"
	/noparse /ini HUDPATH "ma22" "3,305,38,0,255,0,${If[${Bool[${MA2}]},${If[${Bool[${Spawn[${MA2}]}]},${MA2},]},]}"
	/noparse /ini HUDPATH "ma23" "3,305,38,255,0,0,${If[${Bool[${MA2}]},${If[!${Bool[${Spawn[${MA2}]}]},${If[${Bool[${MA2}]},${MA2},]},]},]}"
	/noparse /ini HUDPATH "ma24" "3,305,38,25,25,25,${If[${Bool[${MA2}]},,NONE]}"
	/noparse /ini HUDPATH "sma1" "3,280,50,100,149,237,Smart MA"
	/noparse /ini HUDPATH "sma2" "3,329,49,100,149,237,[    ]"
	/noparse /ini HUDPATH "sma3" "3,334,50,255,255,255,${If[${useSmartAssist1},1,]}${If[${useSmartAssist2},2,]}${If[${useSmartAssistG},G,]}"
	/noparse /ini HUDPATH "sma4" "3,334,50,25,25,25,${If[!${useSmartAssist1} && !${useSmartAssist2} && !${useSmartAssistG},X,]}"

	| buff switches
	/noparse /ini HUDPATH "buff1" "3,280,62,100,149,237,Buff"
	/noparse /ini HUDPATH "buff2" "3,305,61,100,149,237,[                   ]"
	/noparse /ini HUDPATH "buff3" "3,310,62,0,255,0,${If[${switchBuffSelf},S,]}"
	/noparse /ini HUDPATH "buff4" "3,310,62,25,25,25,${If[${switchBuffSelf},,S]} "
	/noparse /ini HUDPATH "buff5" "3,319,62,0,255,0,${If[${switchBuffItem},I,]}"
	/noparse /ini HUDPATH "buff6" "3,319,62,25,25,25,${If[${switchBuffItem},,I]} "
	/noparse /ini HUDPATH "buff7" "3,324,62,0,255,0,${If[${switchBuffCombat},C,]}"
	/noparse /ini HUDPATH "buff8" "3,324,62,25,25,25,${If[${switchBuffCombat},,C]} "
	/noparse /ini HUDPATH "buff9" "3,332,62,0,255,0,${If[${switchBuffBc},B,]}"
	/noparse /ini HUDPATH "buff10" "3,332,62,25,25,25,${If[${switchBuffBc},,B]}"
	/noparse /ini HUDPATH "buff11" "3,341,62,0,255,0,${If[${switchBuffRaid},R,]}"
	/noparse /ini HUDPATH "buff12" "3,341,62,25,25,25,${If[${switchBuffRaid},,R]}"
	/noparse /ini HUDPATH "buff13" "3,349,62,0,255,0,${If[${switchBuffTell},T,]}"
	/noparse /ini HUDPATH "buff14" "3,349,62,25,25,25,${If[${switchBuffTell},,T]}"
	/noparse /ini HUDPATH "buff15" "3,357,62,0,255,0,${If[${switchBuffPet},P,]}"
	/noparse /ini HUDPATH "buff16" "3,357,62,25,25,25,${If[${switchBuffPet},,P]}"

	| lazy ass controlls
	/noparse /ini HUDPATH "rest0" "3,140,74,100,149,237,Rest"
	/noparse /ini HUDPATH "rest1" "3,165,73,100,149,237,[     ]"
	/noparse /ini HUDPATH "rest2" "3,170,74,255,255,255,${If[${autoRest},${autoRest},]}"
	/noparse /ini HUDPATH "rest3" "3,170,74,25,25,25,${If[${autoRest},,##]}"
	
	| DE-buff switches
	/noparse /ini HUDPATH "debuff0" "3,280,74,100,149,237,deBuff"
	/noparse /ini HUDPATH "debuff1" "3,316,73,100,149,237,[                    ]"	
	/noparse /ini HUDPATH "debuff2" "3,321,74,0,255,0,${If[${switchdeBuffMezz},Z,]}"
	/noparse /ini HUDPATH "debuff3" "3,321,74,25,25,25,${If[${switchdeBuffMezz},,Z]}"	
	/noparse /ini HUDPATH "debuff4" "3,329,74,0,255,0,${If[${switchdeBuffMalo},M,]}"
	/noparse /ini HUDPATH "debuff5" "3,329,74,25,25,25,${If[${switchdeBuffMalo},,M]}"	
	/noparse /ini HUDPATH "debuff6" "3,337,74,0,255,0,${If[${switchdeBuffTash},T,]}"
	/noparse /ini HUDPATH "debuff7" "3,337,74,25,25,25,${If[${switchdeBuffTash},,T]}"	
	/noparse /ini HUDPATH "debuff8" "3,345,74,0,255,0,${If[${switchdeBuffCripple},C,]}"
	/noparse /ini HUDPATH "debuff9" "3,345,74,25,25,25,${If[${switchdeBuffCripple},,C]}"	
	/noparse /ini HUDPATH "debuff10" "3,353,74,0,255,0,${If[${switchdeBuffSlow},S,]}"
	/noparse /ini HUDPATH "debuff11" "3,353,74,25,25,25,${If[${switchdeBuffSlow},,S]}"		
	/noparse /ini HUDPATH "debuff12" "3,361,74,0,255,0,${If[${switchdeBuffEradicate},E,]}"
	/noparse /ini HUDPATH "debuff13" "3,361,74,25,25,25,${If[${switchdeBuffEradicate},,E]}"		
	/noparse /ini HUDPATH "debuff14" "3,369,74,0,255,0,${If[${switchdeBuffSnare},R,]}"		
	/noparse /ini HUDPATH "debuff15" "3,369,74,25,25,25,${If[${switchdeBuffSnare},,R]}"		
	
	| heal switches
	/noparse /ini HUDPATH "heal2" "3,305,85,100,149,237,[     ][              ]"
	/noparse /ini HUDPATH "heal3" "3,310,86,255,255,255,${If[${HealOverride},${HealOverride},]}"
	/noparse /ini HUDPATH "heal4" "3,310,86,25,25,25,${If[${HealOverride},,##]}"
	/noparse /ini HUDPATH "heal5" "3,331,86,0,255,0,${If[${switchHealSelf},S,]}"
	/noparse /ini HUDPATH "heal6" "3,331,86,25,25,25,${If[${switchHealSelf},,S]}"
	/noparse /ini HUDPATH "heal7" "3,339,86,0,255,0,${If[${switchHealGroup},G,]}"
	/noparse /ini HUDPATH "heal8" "3,339,86,25,25,25,${If[${switchHealGroup},,G]}"
	/noparse /ini HUDPATH "heal9" "3,347,86,0,255,0,${If[${switchHealXtarget},X,]}"
	/noparse /ini HUDPATH "heal10" "3,347,86,25,25,25,${If[${switchHealXtarget},,X]}"
	/noparse /ini HUDPATH "heal11" "3,355,86,0,255,0,${If[${switchHealPet},P,]}"
	/noparse /ini HUDPATH "heal12" "3,355,86,25,25,25,${If[${switchHealPet},,P]}"
	/noparse /ini HUDPATH "heal13" "3,363,86,0,255,0,${If[${switchHealCure},C,]}"
	/noparse /ini HUDPATH "heal14" "3,363,86,25,25,25,${If[${switchHealCure},,C]}"
	/noparse /ini HUDPATH "heal15" "3,280,86,100,149,237,${If[!${Select[${Me.Class.ShortName},CLR]},Heal,]}"
	/noparse /ini HUDPATH "heal16" "3,280,86,255,0,0,${If[${Select[${Me.Class.ShortName},CLR]} && ${switchHealAggressive},Heal,]}"
	/noparse /ini HUDPATH "heal17" "3,280,86,0,255,0,${If[${Select[${Me.Class.ShortName},CLR]} && !${switchHealAggressive},Heal,]}"

	| notice at bottom	
	/noparse /ini HUDPATH "notice1" "3,5,131,100,149,237,[                                                                                                                   ]"
	/noparse /ini HUDPATH "notice2" "3,10,132,255,255,255,${hudNotice}"

	/noparse /ini HUDPATH "time1" "3,360,132,146,149,180,${If[${HUDTime} || !${versionCore},${Time.Time24},${GameTime}]}"

	:exit
/return