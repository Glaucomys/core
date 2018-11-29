|***
 *	pregen.inc
 *	Core INI Generation Utility
 *
 *
 ***|
 
 
 
 

|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_environment()

	| current build
	/call iniControl FALSE declare coreBuild									string	outer 20181129.00


	| Base Environment settings
	/call iniControl FALSE declare forceINIcheck							bool		outer TRUE
	/call iniControl FALSE declare castReturn 								string 	outer NULL
	/call iniControl FALSE declare combat											string	outer FALSE
	/call iniControl FALSE declare isValidTarget							bool		outer FALSE
	/call iniControl FALSE declare noInvis 										bool 		outer TRUE
	/call iniControl FALSE declare noFeigning 								bool 		outer TRUE
	/call iniControl FALSE declare Target2Kill								int			outer 0
	/call iniControl FALSE declare currentLootCount						int			outer	0
      
	| correctINICase will allow core to delete all INI entries with improper case structure and recreate 
	| them with the same data.
	| This is an incredibly invasive search and replace of EVERY INI entry in core.ini and the toons ini.
	| I do not advise leaving this TRUE. Use it and turn it back FALSE.
	/call iniControl FALSE declare correctINICase							bool		outer	FALSE

                                                            
	| lists for .. you know..lists and stuff.                 
	/call iniControl FALSE declare AllClassList								string 	outer "|WAR|SHD|PAL|BER|ROG|MNK|RNG|BST|BRD|NEC|ENC|WIZ|MAG|CLR|SHM|DRU"
	/call iniControl FALSE declare MeleeList    		  				string 	outer "|WAR|ROG|BST|MNK|SHD|PAL|BER|BRD|RNG"
	/call iniControl FALSE declare CastList										string 	outer "|BST|SHD|PAL|RNG|CLR|DRU|SHM|ENC|WIZ|NEC|MAG"
	/call iniControl FALSE declare PetList										string 	outer "|ENC|DRU|SHM|SHD|WIZ|BRD|BST|MAG|NEC"
	/call iniControl FALSE declare HealerList									string 	outer "|CLR|DRU|SHM|PAL"
	/call iniControl FALSE declare RezerList									string 	outer "|CLR|DRU|SHM|PAL|NEC"
	/call iniControl FALSE declare AASnareList								string 	outer "|RNG|DRU|SHD|NEC|ROG|WAR|WIZ|MNK|PAL|BER"
	/call iniControl FALSE declare AAEradicateList						string 	outer "|RNG|DRU|NEC|WIZ|ENC|MAG"
	/call iniControl FALSE core plist													string	Plugin "|MQ2Cast|MQ2Exchange|MQ2MoveUtils|MQ2Debuffs|MQ2DanNet"
                                                            
	                                                          
	| this is a makeshift /doevents to force binds to be cycled
	/call iniControl FALSE declare bindTrigger								bool		outer TRUE
	                                                          
                                                            
	| holder for the current sub for debugging purposes       
	/call iniControl FALSE declare currentdebugsub						string	outer FALSE


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
	/call iniControl FALSE declare cnum												string	outer "\a-t"
	/call iniControl FALSE declare ctar												string	outer "\a-w"
	/call iniControl FALSE declare cpc												string	outer "\a-w"
	/call iniControl FALSE declare csp												string	outer "\a-r"
	/call iniControl FALSE declare cbad												string	outer "\ar"
	/call iniControl FALSE declare cgood											string	outer "\ag"
	/call iniControl FALSE declare c-													string	outer "\at"
	/call iniControl FALSE declare cbug												string	outer "\at"
	/call iniControl FALSE declare cinfo											string	outer "\aw"
	/call iniControl FALSE declare citem											string	outer "\au"
	/call iniControl FALSE declare sep												string	outer "\at..\ax"
	/call iniControl FALSE declare sepO												string	outer "\at[\ax"
	/call iniControl FALSE declare sepC												string	outer "\at]\ax"
	/call iniControl FALSE declare oenabled										string	outer "${cgood}Enabled\ax"
	/call iniControl FALSE declare oon												string	outer "${cgood}on\ax"
	/call iniControl FALSE declare odisabled									string	outer "${cbad}DISABLED\ax"
	/call iniControl FALSE declare ooff												string	outer "${cbad}off\ax"
	/call iniControl FALSE declare owarning										string	outer "${cbad}WARNING\ax"
	/call iniControl FALSE declare ofatal											string	outer "${cbad}FATAL ERROR\ax ${sep} ${cbad}ENDING MACRO\ax"
	/call iniControl FALSE declare ocaution										string	outer "${cbad}CAUTION\ax"
	/call iniControl FALSE declare otrue											string	outer "${cgood}TRUE\ax"
	/call iniControl FALSE declare ofalse											string	outer "${cbad}FALSE\ax"
	/call iniControl FALSE declare odestroy										string	outer "${cbad}DESTROY\ax"
	/call iniControl FALSE declare oleave											string	outer "${cinfo}Leave\ax"
	/call iniControl FALSE declare oannounce									string	outer "\amAnnounce\ax"
	/call iniControl FALSE declare oadd												string	outer "${cgood}Add\ax"
	/call iniControl FALSE declare okeep											string	outer "${cgood}Keep\ax"
	/call iniControl FALSE declare onotice										string	outer "\ayNotice\ax"
 	/call iniControl FALSE declare ocombat										string	outer "\amCombat\ax"	
	/call iniControl FALSE declare oremove										string	outer "${cbad}Removing\ax"
	/call iniControl FALSE declare obroadcasted								string 	outer "\amBroadcasted\ax"
	| ? colors
	/call iniControl FALSE declare wikiswitch									string outer "[\a-wswitch\ax]"
	/call iniControl FALSE declare wikitarget									string outer "[\a-wTARGET\ax]"
	/call iniControl FALSE declare wikiname										string outer "[\a-wName\ax]"
	/call iniControl FALSE declare wikispire									string outer "[\ar0\ax\a-w|1|2|3\ax]"
	/call iniControl FALSE declare wikipct										string outer "[\a-w1,99\ax]"
	
	
	| please take a number and have a seat.
	/call iniControl FALSE declare bitchcount									int			outer	31
	/call iniControl FALSE declare bitch1											string	outer	"Seriously... Again..."
	/call iniControl FALSE declare bitch2											string	outer	"Did your parents ever ask you to run away from home?"
	/call iniControl FALSE declare bitch3											string	outer	"Aha, I see the Fuck-Up Fairy has visited us again!"
	/call iniControl FALSE declare bitch4											string	outer	"Will trade a Halfling for a rez, or three Gn0mes."
	/call iniControl FALSE declare bitch5											string	outer	"I'm sorry I bit you... pulled your hair... and punched you in the face..."
	/call iniControl FALSE declare bitch6											string	outer	"Go ahead, tell them everything you know. It'll only take 10 seconds."
	/call iniControl FALSE declare bitch7											string	outer	"Sometimes the first step to forgiveness, is realising the other person was born an idiot."
	/call iniControl FALSE declare bitch8											string	outer	"I'm getting Disc lag . . . again."
	/call iniControl FALSE declare bitch9											string	outer	"oh man....i figured out why i was lagging the other day...i look in my case and my kids knocked off my cpu fan."
	/call iniControl FALSE declare bitch10										string	outer	"I'm jealous of all the people that haven't met you!"
	/call iniControl FALSE declare bitch11										string	outer	"You're slower than a herd of turtles stampeding through peanut butter."
	/call iniControl FALSE declare bitch12										string	outer	"Oh good, my dog found a chainsaw."
	/call iniControl FALSE declare bitch13										string	outer	"You are not as bad as people say, you are much, much worse."
	/call iniControl FALSE declare bitch14										string	outer	"Stupidity is not a crime so you are free to go."
	/call iniControl FALSE declare bitch15										string	outer "AFK, playing Pokemon. More productive then this shit."
	/call iniControl FALSE declare bitch16										string	outer	"All I did was move the mouse over it."
	/call iniControl FALSE declare bitch17										string	outer	"You sound reasonable. It must be time to up my medication!"
	/call iniControl FALSE declare bitch18										string	outer	"I'll never forget the first time we met. Although, I'll keep trying."
	/call iniControl FALSE declare bitch19										string	outer	"Roses are red. Violets are blue. You're an idiot."
	/call iniControl FALSE declare bitch20										string	outer	"Sometimes, when I close my eyes, I... can't see you."
	/call iniControl FALSE declare bitch21										string 	outer "Shock me. Say something intelligent."
	/call iniControl FALSE declare bitch22										string 	outer "I'm typing this with my middle finger."
	/call iniControl FALSE declare bitch23										string 	outer "It takes courage just to be you. To get out of bed every single day, knowing full well, you gotta be you."
	/call iniControl FALSE declare bitch24										string 	outer "For someone with such a smart mouth you sure do have a lot of teeth."
	/call iniControl FALSE declare bitch25										string 	outer "Do you want a piece of gum?"
	/call iniControl FALSE declare bitch26										string 	outer "Good thing you're pretty."
	/call iniControl FALSE declare bitch27										string 	outer "You breathe through your mouth a lot don't you?"
	/call iniControl FALSE declare bitch28										string 	outer "Your mother was a hamster and your father smelt of elderberries!"
	/call iniControl FALSE declare bitch29										string 	outer "Don't worry about it too much. I did the same thing when I was eleven."
	/call iniControl FALSE declare bitch30										string 	outer "As the sun rose, Dawn crept across the lawn.. Searching for her car keys."
	/call iniControl FALSE declare bitch31										string 	outer "Thank you for nothing... You useless reptile."


	| expansions
	/call iniControl FALSE declare expansion1									string 	outer "The Ruins of Kunark"
	/call iniControl FALSE declare expansion2									string 	outer "The Scars of Velious"
	/call iniControl FALSE declare expansion3									string 	outer "The Shadows of Luclin"
	/call iniControl FALSE declare expansion4									string 	outer "The Planes of Power"
	/call iniControl FALSE declare expansion5									string 	outer "The Legacy of Ykesha"
	/call iniControl FALSE declare expansion6									string 	outer "Lost Dungeons of Norrath"
	/call iniControl FALSE declare expansion7									string 	outer "Gates of Discord"
	/call iniControl FALSE declare expansion8									string 	outer "Omens of War"
	/call iniControl FALSE declare expansion9									string 	outer "Dragons of Norrath"
	/call iniControl FALSE declare expansion10								string 	outer "Depths of Darkhollow"
	/call iniControl FALSE declare expansion11								string 	outer "Prophecy of Ro"
	/call iniControl FALSE declare expansion12								string 	outer "The Serpent's Spine"
	/call iniControl FALSE declare expansion13								string 	outer "The Buried Sea"
	/call iniControl FALSE declare expansion14								string 	outer "Secrets of Faydwer"
	/call iniControl FALSE declare expansion15								string 	outer "Seeds of Destruction"
	/call iniControl FALSE declare expansion16								string 	outer "Underfoot"
	/call iniControl FALSE declare expansion17								string 	outer "House of Thule"
	/call iniControl FALSE declare expansion18								string 	outer "Veil of Alaris"
	/call iniControl FALSE declare expansion19								string 	outer "Rain of Fear"
	/call iniControl FALSE declare expansion20								string 	outer "Call of the Forsaken"
	/call iniControl FALSE declare expansion21								string 	outer "The Darkened Sea"
	/call iniControl FALSE declare expansion22								string 	outer "The Broken Mirror"
	/call iniControl FALSE declare expansion23								string 	outer "Empires of Kunark"
	/call iniControl FALSE declare expansion24								string 	outer "Ring of Scale"
	/call iniControl FALSE declare expansion25								string 	outer ""

/return




|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_core()
	/declare _dcname string local ${MacroQuest.Server.Left[1].Upper}${MacroQuest.Server.Right[-1]}_${Me.Class.ShortName}_${Me.DisplayName}


	| HUD controls
	/call iniControl FALSE declare hudWarningLength						int			outer	0
	/call iniControl FALSE declare hudNoticeLength						int			outer	0
	/call iniControl FALSE declare hudTargetLength						int			outer	0
	/call iniControl FALSE declare hcType											string	outer "|time|update|broadcast|delete"
	/call iniControl FALSE declare hudNotice									string	outer FALSE
	/call iniControl FALSE declare hudWarning									string	outer	FALSE
	/call iniControl FALSE declare hudTarNotice								string	outer FALSE
	/call iniControl FALSE declare hudBroadcast								string	outer FALSE
	/call iniControl FALSE declare tmpMode										string	outer FALSE
	/call iniControl FALSE declare hudDeathCount							int			outer	0
	/call iniControl FALSE core HudName												string	General Core
	/call iniControl FALSE core HudType												string	General normal
	/call iniControl FALSE class HUDTime													bool 		Common TRUE
	/call iniControl FALSE class setBroadcast										string	Common TRUE	
	
	| communications

	/call iniControl FALSE declare commType										string	outer "|eqbc|dannet"
	/call iniControl FALSE core setCommunications							string	Comm "eqbc"	
	/call iniControl FALSE core EQBCServer										string	Comm 127.0.0.1
	/call iniControl FALSE core EQBCPort											string	Comm 2112
	                                                    			
	| DanNet settings                                   			
	/call iniControl FALSE core switchDanLocalEcho						bool		Comm FALSE
	/call iniControl FALSE core switchDanCommandEcho					bool		Comm FALSE
	/call iniControl FALSE core switchDanFullNames						bool		Comm FALSE


	| core control
	/call iniControl FALSE declare coreType 									string 	outer "|auto|checkcash|control|events|expadjust|expmaintain|explevel|fireworks|fixini|takeraidinvite|takegroupinvite|giveraidinvite|killcode|reset|rest|whitewash|zonereset"
	/call iniControl FALSE declare coreObscure								bool		outer FALSE
	/call iniControl FALSE class coreAuto				 													bool	 	Common FALSE
	/call iniControl FALSE core remoteKillKey									string	General "CHANGE ME" 
	
	
	/call iniControl FALSE core ${_dcname}										int			DeathCount 0
	/call iniControl FALSE core invitelist										string	No FALSE
	/call iniControl FALSE core rezlist												string	No FALSE
	/call iniControl FALSE core heallist											string	No FALSE
	/call iniControl FALSE core heallist											string	No FALSE
	/call iniControl FALSE core safelist											string	General "|FALSE"


	| misc environment switches/controls
	/call iniControl FALSE core minMobPullLvl									int			General 1
	/call iniControl FALSE core maxMobPullLvl									int			General 150
	/call iniControl FALSE core autoinihousekeeping						bool		General FALSE
	/call iniControl FALSE core smartasserrors								bool		General TRUE
	/call iniControl FALSE core forceNukeLoop									bool		General FALSE
	/call iniControl FALSE core autoAddNameList								bool		General TRUE
	/call iniControl FALSE core switchDispInfo								bool		General TRUE
	/call iniControl FALSE core switchDispBuild								bool		General TRUE
	/call iniControl FALSE core switchDispEXP									bool		General TRUE
	/call iniControl FALSE core switchAADiscnowManual					bool		General FALSE
	/call iniControl FALSE core switchDelayWhileZoning				bool		General FALSE
	/call iniControl FALSE core switchAutoRestFull						bool		General FALSE
	/call iniControl FALSE core switchModRodinRest						bool		General TRUE
	/call iniControl FALSE core switchBuffinRest							bool		General TRUE
	/call iniControl FALSE core switchUseCLSinHUD							bool		General FALSE
	/call iniControl FALSE core switchUseADVL									bool		General TRUE
	/call iniControl FALSE core switchGlyphOutsideRaid				bool		General FALSE
	/call iniControl FALSE core maintenance										string	General "|FALSE"
	/call iniControl FALSE core switchMQclearonCLS						bool		General FALSE
	/call iniControl FALSE core switchGuildinSafelist					bool		General FALSE
	/call iniControl FALSE core switchCheckGameCash						bool		General FALSE
	/call iniControl FALSE core switchPersistentModes					bool		General FALSE
	/call iniControl FALSE core switchNoTieonCombat						bool		General FALSE
	/call iniControl FALSE core switchDisableMQ2Melee					bool		General TRUE
	/call iniControl FALSE core switchIgnoreXpacControl				bool		General FALSE
	/call iniControl FALSE core switchBuffGemHold							bool		General TRUE
	/call iniControl FALSE core setMQFontSize									int			General 2
	/call iniControl FALSE core setMQCaptions									bool		General off
	/call iniControl FALSE core setAutoAssist									bool		General off
	/call iniControl FALSE core setMaxINIBuffCount						int			General 10
                                                      			
	/call iniControl FALSE core switchMealinManual						bool		General FALSE



	| broadcast switches
	| default on
	/call iniControl FALSE core broadcastZonereset						bool	Broadcast TRUE
	/call iniControl FALSE core broadcastStop									bool	Broadcast TRUE
	/call iniControl FALSE core broadcastHere									bool	Broadcast TRUE
                                                      			
	| default off                                       			
	/call iniControl FALSE core broadcastTake									bool	Broadcast FALSE
	/call iniControl FALSE core broadcastDoor									bool	Broadcast FALSE
	/call iniControl FALSE core broadcastInvis								bool	Broadcast FALSE
	/call iniControl FALSE core broadcastKillmob							bool	Broadcast FALSE
	/call iniControl FALSE core broadcastClean								bool	Broadcast FALSE
	/call iniControl FALSE core broadcastAnchor								bool	Broadcast FALSE


	| buff queue variables
	/call iniControl FALSE core queueCountMax						int			General 10
	/call iniControl FALSE declare queueCount            					int  		outer 0
	
	/if (!${Defined[queueBuffID]}) /declare queueBuffID[8] 												int  		outer
	/if (!${Defined[queueList]}) /declare queueList[${queueCountMax},2] 				 	int  		outer


	| hide switches for /hidecorpse command
	/call iniControl FALSE core hideNone											bool		hide FALSE
	/call iniControl FALSE core hideAll												bool		hide FALSE
	/call iniControl FALSE core hideAlways										bool		hide FALSE
	/call iniControl FALSE core hideLooted										bool		hide FALSE
	/call iniControl FALSE core hideAllButGroup								bool		hide FALSE
	/call iniControl FALSE core hideNPC												bool		hide FALSE
	/call iniControl FALSE core hideLooted										bool		hide FALSE
	/call iniControl FALSE core hideListed										bool		hide FALSE
	/call iniControl FALSE core hideListedUnlocked						bool		hide FALSE


	| set /crew stuffs
	/call iniControl FALSE core main													string	Group "|FALSE|FALSE"
	/call iniControl FALSE core main2													string	Group "|FALSE|FALSE"
	/call iniControl FALSE core ${MacroQuest.Server}Leader		string	Group FALSE
	/call iniControl FALSE core ${MacroQuest.Server}Assist		string	Group FALSE
	/call iniControl FALSE core ${MacroQuest.Server}Looter		string	Group FALSE
	
	/call iniControl FALSE core ResidentMage									string	Group "|FALSE|FALSE"
	/call iniControl FALSE core ResidentDruid						 			string	Group "|FALSE|FALSE"
	/call iniControl FALSE core ResidentWizard								string	Group "|FALSE|FALSE"
	

	| /here settings
	/call iniControl FALSE core makeVisible										bool		Here TRUE
	/call iniControl FALSE core clearTarget										bool		Here TRUE
	/call iniControl FALSE core defaultTimerDelay							string	Here 1m
	/call iniControl FALSE core baseAgroRange									int			Here 60
	/call iniControl FALSE core emptyHands										bool		Here TRUE
	/call iniControl FALSE core hereZRadius										int			Here 30


	| zone - core.zone.ini settings
	/call iniControl FALSE declare ZoneFileList								string 	outer "|NameList|ExcludeList|NoMezzList|RangeList|NoCastList|NoMeleeList|HuntMobList|CharmMobList|NoFireList|NoPoisonList|NoMagicList|NoCorruptionList|NoColdList|NoDiseaseList|HarvestList"
	/call iniControl FALSE declare currentZone								string	outer	FALSE
	/call iniControl FALSE declare RangeList		   						string 	outer FALSE
	/call iniControl FALSE declare ExcludeList    						string 	outer FALSE
	/call iniControl FALSE declare NameList       						string 	outer FALSE
	/call iniControl FALSE declare NoMeleeList     		  			string 	outer FALSE
	/call iniControl FALSE declare NoCastList       					string 	outer FALSE
	/call iniControl FALSE declare NoMezzList       					string 	outer FALSE
	/call iniControl FALSE declare NoDiseaseList       				string 	outer FALSE
	/call iniControl FALSE declare NoMagicList       					string 	outer FALSE
	/call iniControl FALSE declare NoPoisonList       				string 	outer FALSE
	/call iniControl FALSE declare NoFireList       					string 	outer FALSE
	/call iniControl FALSE declare NoCorruptionList      			string 	outer FALSE
	/call iniControl FALSE declare NoColdList       					string 	outer FALSE
	/call iniControl FALSE declare CharmMobList      					string 	outer FALSE
	/call iniControl FALSE declare SafeZone		       					string 	outer FALSE
	/call iniControl FALSE declare FullZoneName      					string 	outer FALSE
	/call iniControl FALSE declare HuntMobList       					string 	outer FALSE
	/call iniControl FALSE declare HarvestList       					string 	outer FALSE


	| general settings for mode item usage
	/call iniControl FALSE core modeDrinkofChoice				string	mode FALSE
	/call iniControl FALSE core modeOpenItem						string	mode FALSE


/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_character()
	/declare _count 																int 		local 0
	/call iniControl FALSE class classBuild											string	Common 00.00

	| auto or manual control?
	/call iniControl FALSE class autoControl			 								bool		Common FALSE 


	/call iniControl FALSE class noCastWith											string	Common "|Quick Defense|Protection of Bayle|Interrogation Intimidation|Energy Charge|Cloud of Terror|Faith"
	/call iniControl FALSE class noMeleeWith											string	Common "|Itching Poison"
	
	/call iniControl FALSE class shrinkItem 											string 	Common "Ring of the Ancients"
	/call iniControl FALSE class IllusionItem										string	Common FALSE
	/call iniControl FALSE class IllusionBuff										string	Common FALSE
	/call iniControl FALSE class GTFO														string	Common FALSE
	/call iniControl FALSE class switchAApicker									bool 		Common FALSE
	
	| zerkers keep their own range item setup
	/if (!${Select[${Me.Class.ShortName},BER]}) {
		/call iniControl FALSE class RangeItem												string	Common "|FALSE|FALSE"
		/call iniControl FALSE class RangeItemSummon									string	Common "|FALSE|FALSE"
		/call iniControl FALSE class RangeItemCount									int			Common 5
	}
	/call iniControl FALSE class RangeSpell											string	Common FALSE
	/call iniControl FALSE class RangeClickItem									string	Common FALSE
	/call iniControl FALSE class RangeDisc												string	Common FALSE
			
	/call iniControl FALSE class autoInventoryItem								string	Common "|Summoned: Giant Modulation Shard"
	/call iniControl FALSE class stoponDS												bool		Common FALSE		
	/call iniControl FALSE class useGlyph												string	Common "Glyph of the Cataclysm"
	/call iniControl FALSE class switchSos												bool	 	Common FALSE


	| base lists for mob mez control
	/call iniControl FALSE declare mobList										string	outer
	/call iniControl FALSE declare addList 										string 	outer	

	| echo switches
	/call iniControl FALSE declare echoType 									string 	outer "|alt|bc|channel|disc|item|melee|spell|relay|verbose"
	/call iniControl FALSE class switchEchoAlt										bool		Echo TRUE
	/call iniControl FALSE class switchEchoSpell									bool		Echo TRUE
	/call iniControl FALSE class switchEchoDisc									bool		Echo TRUE
	/call iniControl FALSE class switchEchoItem									bool		Echo TRUE
	/call iniControl FALSE class switchEchoMelee									bool		Echo TRUE
	/call iniControl FALSE class switchEchoBC										bool		Echo FALSE
	/call iniControl FALSE class echoChannel											string	Echo FALSE
	/call iniControl FALSE class relayTells											string	Echo FALSE
	/call iniControl FALSE class verbose													bool		Echo TRUE
	/call iniControl FALSE class ClassChannel										int			Echo FALSE
	
	

	/call iniControl FALSE class toonControl			 								string	Common FALSE
	/call iniControl FALSE class autoRest 												int 		Common 90
	/call iniControl FALSE class AutoExpAdjust 									bool 		Common TRUE
	/call iniControl FALSE class MaintExpLvl   									int 		Common 99
	/call iniControl FALSE class MaxLevel      									int 		Common 110
	/call iniControl FALSE class coreEventsInc  									bool 		Common TRUE
	/call iniControl FALSE class switchAAFireworks								bool	 	Common FALSE

	/call iniControl FALSE class switchTakeGroupInvite						bool 		Common FALSE
	/call iniControl FALSE class switchTakeRaidInvite						bool 		Common FALSE
	/call iniControl FALSE class switchGiveRaidInvite						bool		Common FALSE
	
	
	| mode control
	/call iniControl FALSE declare modeType												string	outer "|stop|agro|drag|drink|fish|forage|harvest|harvestcount|hunt|lush|open|petfarm|scorch|skill|tradeskill|trainspell"
	/call iniControl FALSE declare setMode												bool		outer FALSE
	/call iniControl FALSE declare switchModeStop									bool		outer FALSE
	/call iniControl FALSE declare switchModeAgro									bool		outer FALSE
	/call iniControl FALSE declare setModeType										string	outer FALSE
	/call iniControl FALSE declare setModeCount										int			outer 0
	/call iniControl FALSE class setModeAgro											int			Common FALSE 
	/call iniControl FALSE class modeHarvestCount								int			Common 999
	/call iniControl FALSE declare switchModeDrag									bool		outer FALSE 
	/call iniControl FALSE declare switchModeDrink								bool		outer FALSE 
	/call iniControl FALSE declare switchModeForage								bool		outer FALSE
	/call iniControl FALSE declare switchModePetfarm							bool		outer FALSE
	/call iniControl FALSE declare switchModeScorch								bool		outer FALSE
	/call iniControl FALSE declare switchModeHunt									bool		outer FALSE
	/call iniControl FALSE declare switchModeFish									bool		outer FALSE
	/call iniControl FALSE declare switchModeSkill								bool		outer FALSE
	/call iniControl FALSE declare switchModeHarvest							bool		outer FALSE
	/call iniControl FALSE declare switchModeHarvestcount					bool		outer FALSE
	/call iniControl FALSE declare switchModeTradeskill						bool		outer FALSE
	/call iniControl FALSE declare switchModeLush									bool		outer FALSE
	/call iniControl FALSE declare switchModeTrainspell						bool		outer FALSE
	/call iniControl FALSE declare switchModeTrainspellSPC				bool		outer FALSE
	/call iniControl FALSE declare switchModeOpen									bool		outer FALSE	
	
	
	| burns
	/call iniControl FALSE declare burnType												string	outer "|auto|count|engageat|force"
	/call iniControl FALSE declare setBurnControl									bool		outer FALSE
	/call iniControl FALSE declare burnForce											bool		outer FALSE
	/call iniControl FALSE class burnAuto												bool		common FALSE
	/call iniControl FALSE class burnEngageat										int			common 80
	/call iniControl FALSE class burnCount												int			common 10
	

	| combat controls
	/call iniControl FALSE declare ccType													string	outer "|agro|alliance|bandolier|circle|combatdistance|engage|faderest|force|melee|meleedisc|range|nuke|dot|forceassist|nukedelay|offtank|pull|saferadius|smartengage|stopnuke|zradius"
	/call iniControl FALSE class MA1			 												string 	Common FALSE
	/call iniControl FALSE class MA2			 												string 	Common FALSE
	/call iniControl FALSE class MobAgro													int 		Common 60
	/call iniControl FALSE class engageAt 												int 		Common 98
	/call iniControl FALSE class smartengageAt 									int 		Common 98
	/call iniControl FALSE class	useSmartAssistG									bool		Common FALSE
	/call iniControl FALSE class	useSmartAssist1									bool		Common FALSE
	/call iniControl FALSE class	useSmartAssist2									bool		Common FALSE
	/call iniControl FALSE class	useSmartAssist3									bool		Common FALSE
	/call iniControl FALSE class	useForceAssist									bool		Common FALSE
	/call iniControl FALSE class switchAgro											bool		Common FALSE 
	/call iniControl FALSE class switchPull											bool		Common FALSE


	| rez	control
  /call iniControl FALSE declare rezType												string	outer "|stop|combat|emergency|everyone|give|pct|range|take"
  /call iniControl FALSE declare rezPct													string	outer "|10|20|35|50|60|75|90|93|96"
	/call iniControl FALSE class RezTokenList										string 	Heal "|WAR|SHD|PAL|BER|ROG|MNK|RNG|BST|BRD|NEC|ENC|WIZ|MAG|CLR|SHM|DRU"
	/call iniControl FALSE declare flagCallNeedRez								bool		outer FALSE
	/call iniControl FALSE declare switchAmIDead									bool		outer FALSE
	/call iniControl FALSE declare validRezFound 									bool 		outer FALSE
	/call iniControl FALSE declare rezWindowPct 									int 		outer 0
	/call iniControl FALSE declare switchRez											bool		outer FALSE
	/call iniControl FALSE declare switchRezPct										bool		outer FALSE
	/call iniControl FALSE declare switchRezRange									bool		outer FALSE
	/call iniControl FALSE declare switchRezStop									bool		outer FALSE
	/call iniControl FALSE class switchRezTake 									bool 		Heal TRUE 
	/call iniControl FALSE class switchRezEmergency							bool		Heal FALSE
	/call iniControl FALSE class minRezPct												int			Common 90
	/call iniControl FALSE class switchRezGive	 									bool 		Heal FALSE 
	/call iniControl FALSE class switchRezCombat 								bool 		Heal FALSE 
	/call iniControl FALSE class switchRezEveryone 							bool 		Heal FALSE 
	/call iniControl FALSE class maxRezRange				 							int 		Heal 60 
		

	| spire control
	/call iniControl FALSE declare typeSpire											string	outer "|0|1|2|3"
	/call iniControl FALSE declare ${Me.Class.ShortName}spire0		int			Buffs 0
	/call iniControl FALSE class spireRaid												int 		Buffs 0
	/call iniControl FALSE class spireGroup											int 		Buffs 0


	/call iniControl FALSE class switchOffTank										bool		Common FALSE
	
	/call iniControl FALSE class CombatDistance									int			Movement 15
	/call iniControl FALSE class combatRadius										string	Common 30
	/call iniControl FALSE class zradius													int			Common 60	
	/call iniControl FALSE class switchBandolier									bool		meleedisc FALSE
	/call iniControl FALSE class stopnukeAt 											int 		Spells 0
	/call iniControl FALSE class switchAlliance									string	Spells FALSE
	/call iniControl FALSE class spellAllianceTrigger						string	Spells FALSE
	/call iniControl FALSE class switchForceFadeRest							bool		Common FALSE
	
	/call iniControl FALSE class switchCombatMelee								bool		Common FALSE
	/call iniControl FALSE class switchCombatRange								bool		Common FALSE
	/call iniControl FALSE class switchCombatNuke								bool		Common FALSE
	/call iniControl FALSE class switchCombatDoT									bool		Common FALSE
	/call iniControl FALSE class	broadcastGroup									string	Common Core

	
	| watchspawn stuff
	/call iniControl FALSE declare hudwatchspawn									string	outer FALSE
	/call iniControl FALSE declare watchSpawnCount								int			outer	FALSE
	/call iniControl FALSE class watchSpawn											string	Common FALSE

		
 	| Movement control
  /call iniControl FALSE declare movementType										string	outer "|arc|behind|fast|gather|goto|home|leash|mount|radius"
	/call iniControl FALSE class HomeSet													bool		Movement FALSE
	/call iniControl FALSE class HomeRadius											int 		Movement 5
	/call iniControl FALSE class HomeLeash												int 		Movement 40
	/call iniControl FALSE class HomeDelayMin										int 		Movement 100
	/call iniControl FALSE class HomeDelayMax										int 		Movement 500	
	/call iniControl FALSE class SetStickBehind 									bool 		Movement FALSE
	/call iniControl FALSE class SetFaceFast											bool		Movement FALSE
	/call iniControl FALSE class SetFaceArc											int			Movement 30
	/call iniControl FALSE class mount														string	Common "Desert Kangon Saddle"
	/call iniControl FALSE declare tempHomeSet										bool		outer FALSE
	/call iniControl FALSE declare tempsnapstuck									bool		outer	FALSE


	| tie control
  /call iniControl FALSE declare tcType																string	outer "|distance|lock|mode|name|time"
	/call iniControl FALSE class SetTieToon 														string 	Movement FALSE
	/call iniControl FALSE declare SetTie       												bool 		outer FALSE
	/call iniControl FALSE class SetTieDistance 												int 		Movement 5
	/call iniControl FALSE class SetTieLock	 														bool 		Movement FALSE
	/call iniControl FALSE class SetTieMode	 														string	Movement Stick

	
	| debuff switches	
  /call iniControl FALSE declare deBuffType														string	outer "|stop|mezz|malo|tash|cripple|slow|eradicate|snare"
	/call iniControl FALSE declare switchdeBuff													bool		outer FALSE
	/call iniControl FALSE declare switchdeBuffStop											bool		outer FALSE
	/call iniControl FALSE class switchdeBuffCripple										bool		Buffs FALSE
	/call iniControl FALSE class switchdeBuffMezz												bool		Buffs FALSE
	/call iniControl FALSE class switchdeBuffMalo												bool		Buffs FALSE
	/call iniControl FALSE class switchdeBuffTash												bool		Buffs FALSE
	/call iniControl FALSE class switchdeBuffSlow												bool		Buffs FALSE
	/call iniControl FALSE class switchdeBuffEradicate									bool		Buffs FALSE
	/call iniControl FALSE class switchdeBuffSnare											bool		Buffs FALSE


	| unity buffs
	/for _count 1 to 7
		/call iniControl FALSE class BuffAAUnity${_count}									string	Buffs FALSE
	/next _count


	| clickie things
	/call iniControl FALSE class clickItem1															string	Items FALSE
	/call iniControl FALSE class clickItem2															string	Items FALSE
	/call iniControl FALSE class clickItem3															string	Items FALSE
	/call iniControl FALSE class clickItem4															string	Items FALSE
	/call iniControl FALSE class clickItem5															string	Items FALSE
	/call iniControl FALSE class clickItem6															string	Items FALSE
		                                                          				
	/call iniControl FALSE class clickItemBurn1													string	Items FALSE
	/call iniControl FALSE class clickItemBurn2													string	Items FALSE
	/call iniControl FALSE class clickItemBurn3													string	Items FALSE
	/call iniControl FALSE class clickItemBurn4													string	Items FALSE
	/call iniControl FALSE class clickItemBurn5													string	Items FALSE
	/call iniControl FALSE class clickItemBurn6													string	Items FALSE
	
	/call iniControl FALSE class Breastplate1		 												string	Items FALSE
	/call iniControl FALSE class Breastplate1Condition									string	Items FALSE
	/call iniControl FALSE class Breastplate1effect											string	Items FALSE
	/call iniControl FALSE class Breastplate2		 												string	Items FALSE
	/call iniControl FALSE class Breastplate2Condition									string	Items FALSE
	/call iniControl FALSE class Breastplate2effect											string	Items FALSE
	                                                              			
	/call iniControl FALSE class useCircle															string	Items FALSE
	/call iniControl FALSE class CircleofPower													string	Items FALSE
	/call iniControl FALSE class CircleofLife														string	Items FALSE
	/call iniControl FALSE class CircleofMana														string	Items FALSE
                                                                			
	/call iniControl FALSE class clickItemSwarm1												string	Items FALSE
	/call iniControl FALSE class clickItemSwarm2												string	Items FALSE
	/call iniControl FALSE class clickItemSwarm3												string	Items FALSE
	/call iniControl FALSE class clickItemSwarm4												string	Items FALSE
                                                                			
	/call iniControl FALSE class clickOddItem1													string	Odds FALSE
	/call iniControl FALSE class clickOddItem2													string	Odds FALSE
	/call iniControl FALSE class clickOddItem3													string	Odds FALSE
	/call iniControl FALSE class clickOddItem4													string	Odds FALSE
	
	
	| AAs that you want to always keep going
	/call iniControl FALSE class AANow1																	string	AANow FALSE
	/call iniControl FALSE class AANow2																	string	AANow FALSE


	| discs that you want to always keep going
	/call iniControl FALSE class DiscNow1																string	DiscNow FALSE
	/call iniControl FALSE class DiscNow2																string	DiscNow FALSE


	| overrides
  /call iniControl FALSE declare overrideType													string	outer "|stop|engageat|exclude|combat|loot|los|melee|name|range|safelist|safezone|trainspell"
	/call iniControl FALSE declare switchOverride												bool		outer FALSE
	/call iniControl FALSE declare switchOverrideStop										bool		outer FALSE
  /call iniControl FALSE class switchOverrideLos											bool		Override FALSE
  /call iniControl FALSE class switchOverrideSafezone									bool		Override FALSE
  /call iniControl FALSE class switchOverrideExclude									bool		Override FALSE
  /call iniControl FALSE class switchOverrideName											bool		Override FALSE
  /call iniControl FALSE class switchOverrideEngageat									bool		Override FALSE
  /call iniControl FALSE class switchOverrideRange										bool		Override FALSE
  /call iniControl FALSE class switchOverrideMelee										bool		Override FALSE
  /call iniControl FALSE class switchOverrideLoot											bool		Override FALSE
  /call iniControl FALSE class switchOverrideSafelist									bool		Override FALSE
  /call iniControl FALSE declare switchOverrideClasscheck							bool		outer FALSE
  /call iniControl FALSE class switchOverrideTrainspell								bool		Override FALSE
	/call iniControl FALSE class switchOverrideCombat										string	Override FALSE


	| General class combat skills
	/call iniControl FALSE class kick																		bool		meleedisc FALSE
	/call iniControl FALSE class bash																		bool		meleedisc FALSE
	/call iniControl FALSE class taunt																	bool		meleedisc FALSE
	/call iniControl FALSE class disarm																	bool		meleedisc FALSE
	/call iniControl FALSE class bandolierNormal												string  meleedisc FALSE
	/call iniControl FALSE declare bandolierSwap												string 	outer ${bandolierNormal}
	/call iniControl FALSE class bandolierBash													string	meleedisc FALSE
	/call iniControl FALSE class bandolierBurn													string	meleedisc FALSE
	/call iniControl FALSE class bandolierRange													string	meleedisc FALSE

	
	/if (${Select[${Me.Class.ShortName},BER]}) {
		/call iniControl FALSE class frenzy																bool		meleedisc FALSE
	}
	/if (${Select[${Me.Class.ShortName},ROG]}) {
		/call iniControl FALSE declare flagSoS														bool		outer FALSE
		/call iniControl FALSE class backstab															bool		meleedisc FALSE
	}
	/if (${Select[${Me.Class.ShortName},BRD,BER,MNK,ROG]}) {
		/call iniControl FALSE class intimidation													bool		meleedisc FALSE
	}
	/if (${Select[${Me.Class.ShortName},MNK,BST]}) {
		/call iniControl FALSE class dragonpunch													bool		meleedisc FALSE
		/call iniControl FALSE class eaglestrike													bool		meleedisc FALSE
		/call iniControl FALSE class tigerclaw														bool		meleedisc FALSE
		/call iniControl FALSE class flyingkick														bool		meleedisc FALSE
		/call iniControl FALSE class roundkick														bool		meleedisc FALSE
	}


	| food/drink
	/call iniControl FALSE class Food																		string 	Meal FALSE
	/call iniControl FALSE class FoodDispenser													string 	Meal FALSE
	/call iniControl FALSE class FoodCount															int		 	Meal 0
	/call iniControl FALSE class Drink																	string 	Meal FALSE
	/call iniControl FALSE class DrinkDispenser													string 	Meal FALSE
	/call iniControl FALSE class DrinkCount															int		 	Meal 0
	/call iniControl FALSE class tFoodDrink															string	Timer 10m
	/call create_timer timer_meal 1


	| pet crap
	/if (${Me.Class.PetClass} || ${Select[${Me.Class.ShortName},ENC,DRU,SHM,SHD]}) {
		/call iniControl FALSE class PetTank 															bool 		Pet FALSE
		/call iniControl FALSE class useSummonWeps 												bool 		Pet TRUE
		/call iniControl FALSE class EpicClick 														string 	Pet FALSE
		/call iniControl FALSE class EpicPetBuff 													string 	Pet FALSE
		/call iniControl FALSE class summonedWepSpell 										string 	Pet "Grant Thassis' Armaments"
		/call iniControl FALSE class summonedWepClosedBag 								string 	Pet "Folded Pack of Thalassic Armaments"
		/call iniControl FALSE class summonedFocusSpell 									string 	Pet "Grant Calix's Heirlooms"
		/call iniControl FALSE class summonedArmorSpell 									string 	Pet "Grant Thassis' Plate"
		/call iniControl FALSE class summonedMaskSpell 										string 	Pet "Grant Visor of Gobeker"
		/call iniControl FALSE class summonedWepName											string 	Pet "Summoned: Thalassic Fireblade"
		/call iniControl FALSE class shrinkPet 														string 	Pet	"Tiny Companion"
		/call iniControl FALSE class PetHealSpell 												string 	Pet FALSE
		/call iniControl FALSE class PetHealType 													string 	Pet "gem1"
		/call iniControl FALSE class PetRenewalSpell 											string 	Pet FALSE
		/call iniControl FALSE class PetRenewalType 											string 	Pet "gem2"
		/call iniControl FALSE class PetIllusion		 											string 	Pet FALSE
		/call iniControl FALSE class PetIllusionBuff											string 	Pet FALSE
		/call iniControl FALSE class PetAAHeal														string 	Pet "Replenish Companion"
		/call iniControl FALSE class PetBagSlotNumber											int 		Pet 8
		                                                            			
		/call iniControl FALSE class PetBuffCount 												int 		Pet 0
		
		| 10 pet buffs
		/for _count 1 to 10
			/call iniControl FALSE class PetBuff${_count} 									string 	Pet FALSE
			/call iniControl FALSE class PetBuff${_count}chk								string 	Pet FALSE
		/next _count
	}	
	
	/if (${Select[${Me.Class.ShortName},MAG,ENC,CLR,NEC,WIZ,SHM,DRU]}) {
		/call iniControl FALSE class TwincastSpell 												string 	Spells FALSE
		/call iniControl FALSE class GatherSpell 													string 	Spells FALSE
		/call iniControl FALSE class SilentCast	 													string	Spells FALSE
	}
	
	/if (!${Select[${Me.Class.ShortName},MNK,BER,WAR]}) {
		/call iniControl FALSE class switchNukeDelay											bool		Spells FALSE
	}	
	

	| buffs
	/call iniControl FALSE class BuffRemove															string	Buffs "|Illusion: Gnoll Reaver|Illusion: Darkfell Gnoll Reaver|Illusion: Blackburrow Gnoll Reaver|Illusion: Mucktail Gnoll Reaver|Illusion: Direwind Gnoll Reaver|Illusion: Human Pirate|Illusion: Ogre Pirate"
  /call iniControl FALSE declare buffType															string	outer "|stop|self|item|combat|bc|raid|tell|pet|merc|group|mgb"
	/call iniControl FALSE declare switchBuff														bool		outer FALSE
	/call iniControl FALSE declare switchBuffStop												bool		outer FALSE
	/call iniControl FALSE class switchBuffItem	 												bool		Buffs FALSE
	/call iniControl FALSE class switchBuffCombat 											bool		Buffs FALSE
	/call iniControl FALSE class switchBuffPet 													bool 		Pet FALSE
	/call iniControl FALSE class switchBuffSelf	 												bool		Buffs FALSE
	/call iniControl FALSE class switchBuffBc														bool		Buffs FALSE
	/call iniControl FALSE class switchBuffRaid													bool		Buffs FALSE
	/call iniControl FALSE class switchBuffTell													bool		Buffs FALSE
	/call iniControl FALSE class switchBuffMerc													bool		Buffs FALSE
	/call iniControl FALSE class switchBuffGroup												bool		Buffs FALSE
	/call iniControl FALSE class switchBuffMgb													bool		Buffs FALSE

	
	/if (${Me.Class.CanCast} || ${Select[${Me.Class.ShortName},ROG]}) {
		/call iniControl FALSE class buffGem 															string 	Spells "gem8"
		/call fix_gem_id "${buffGem}"
		/call iniControl FALSE declare buffNum														int 		local ${Macro.Return}
		
		| /call iniControl FALSE declare buffNum											
		/call iniControl FALSE class GoMLevel															int			Spells 105
		/call iniControl FALSE class GoMNuke															string	Spells FALSE
		/call iniControl FALSE class GoMNukeCondition											string	Spells FALSE
		/call iniControl FALSE class GoMNuke2															string	Spells FALSE
		/call iniControl FALSE class GoMNuke2Condition										string	Spells FALSE
	}

	
	| bc/tell buffs
	/call iniControl FALSE declare Buff0 																string 	outer FALSE
	/for _count 1 to ${setMaxINIBuffCount}                        			
		/call iniControl FALSE class Buff${_count}												string 	Buffs FALSE
		/call iniControl FALSE class Buff${_count}chk											string 	Buffs FALSE
		/call iniControl FALSE class Buff${_count}self										string 	Buffs FALSE
		/call iniControl FALSE class Buff${_count}selfCondition						string 	Buffs FALSE
		/call iniControl FALSE class Buff${_count}Alias										string 	Buffs "|FALSE" 
		/call iniControl FALSE class BCBuff${_count}Class									string 	Buffs "|WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM"
		/call iniControl FALSE class RaidBuff${_count}Class								string 	Buffs "|WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM"
		/call iniControl FALSE class ManualBuff${_count}Class							string 	Buffs "|WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM"
		/call iniControl FALSE class Buff${_count}noBCinRaid							bool	 	Buffs FALSE
	/next _count
	
	
	| MGB buff
	/if (${validate_class[FALSE, ${CastList}]}) {	
		/call iniControl FALSE class BuffMGBname													string 	Buffs FALSE	
		/call iniControl FALSE class BuffMGBchk														string 	Buffs FALSE	
		/call iniControl FALSE class BuffMGBalias													string 	Buffs "|FALSE"
		/call iniControl FALSE class BuffMGBcount													int		 	Buffs 18
		/call iniControl FALSE class BuffMGBmana													int		 	Buffs 40
	}                                                             			

	/call iniControl FALSE class Aura1Spell 														string 	Aura FALSE
	/call iniControl FALSE class Aura1Buff 															string 	Aura FALSE  
	/call iniControl FALSE class Aura2Spell 														string 	Aura FALSE
	/call iniControl FALSE class Aura2Buff 															string 	Aura FALSE
	
	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,PAL]}) {
		/call iniControl FALSE declare callID															int 		outer 0
		/call iniControl FALSE declare NULLPoint													int 		outer 0
		/call iniControl FALSE class PetPoint 														int			Heal 80
		/call iniControl FALSE class CLRPoint		 													int 		Heal 80
		/call iniControl FALSE class DRUPoint 														int 		Heal 70
		/call iniControl FALSE class SHMPoint 														int 		Heal 60
		/call iniControl FALSE class WIZPoint 														int 		Heal 75
		/call iniControl FALSE class MAGPoint 														int 		Heal 75
		/call iniControl FALSE class ENCPoint 														int 		Heal 80
		/call iniControl FALSE class NECPoint 														int 		Heal 75
		/call iniControl FALSE class WARPoint 														int 		Heal 70
		/call iniControl FALSE class MNKPoint 														int 		Heal 60
		/call iniControl FALSE class ROGPoint 														int 		Heal 60
		/call iniControl FALSE class RNGPoint 														int 		Heal 90
		/call iniControl FALSE class BSTPoint 														int 		Heal 70
		/call iniControl FALSE class SHDPoint 														int 		Heal 90
		/call iniControl FALSE class BRDPoint 														int 		Heal 65
		/call iniControl FALSE class BERPoint 														int 		Heal 70
		/call iniControl FALSE class PALPoint 														int 		Heal 90
		/call iniControl FALSE class GoMpoint 														int 		Heal 80	
		/call iniControl FALSE class XHealClass														string	Heal "|WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM"
		/call setTimer tXHeal																							string	Timer 3m timer_set_xtarget
		/call iniControl FALSE class toonControlXTslot										int			Heal 13
		/call iniControl FALSE class toonControlProtect										bool		Heal TRUE
	}

	/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM,BST,PAL,RNG]}) {
 		/call iniControl FALSE class cureCurse 														string 	Cure FALSE
		/call iniControl FALSE class curePoison 													string 	Cure FALSE
		/call iniControl FALSE class cureDisease 													string 	Cure FALSE
		/call iniControl FALSE class cureCorrupt 													string 	Cure FALSE
	}                                                             			
	                                                              			
	| pet control                                                 			
  /call iniControl FALSE declare 		pcType														string	outer "|pet|swarm|type|build|bag"
	/call iniControl FALSE class PcPetengageat													int			Pet 99
	| /call iniControl FALSE class switchPcTank													bool 		Pet FALSE
	/call iniControl FALSE class PcPettype															string	Pet FALSE
	/call iniControl FALSE class switchPcPet														bool 		Pet FALSE
	/call iniControl FALSE class switchPcSwarm													bool 		Pet FALSE	
	/call iniControl FALSE class PcSwarmengageat												int			Pet 99
	/call iniControl FALSE class sumFamiliar1														string	Pet FALSE
	/call iniControl FALSE class sumFamiliar1Buff												string	Pet FALSE
	/call iniControl FALSE class sumFamiliar1Leave											bool		Pet FALSE	
	/call iniControl FALSE class sumFamiliar2														string	Pet FALSE
	/call iniControl FALSE class sumFamiliar2Buff												string	Pet FALSE
	/call iniControl FALSE class sumFamiliar2Leave											bool		Pet FALSE	
	/call iniControl FALSE class sumFamiliar3														string	Pet FALSE
	/call iniControl FALSE class sumFamiliar3Buff												string	Pet FALSE
	/call iniControl FALSE class sumFamiliar3Leave											bool		Pet TRUE	

	| onoff types
  /call iniControl FALSE declare onoffType														string	outer "|horn|shrink"
	/call iniControl FALSE class switchShrink														bool		Common FALSE
	/call iniControl FALSE class switchHorn															bool		Common FALSE
                                                                			
	| tradeskill control                                          			
  /call iniControl FALSE declare 		tsType														string	outer "|movebank"
	                                                              			
	| cursor                                                      			
  /call iniControl FALSE declare cursorType														string	outer "|announce|auto|destroy|ignore|keep|leave|pass|pause|sell|what"
	/call iniControl FALSE class switchAutoCursor												bool		Common TRUE	
                                                                			
  | list control                                                			
	/call iniControl FALSE declare listType															string	outer "|safe|reset|remove|exclude|name|nomezz|nocast|range|nomelee|nomagic|nodisease|nofire|nopoison|nocold|nocorruption|huntmob"
                                                                			
	| heals                                                       			
  /call iniControl FALSE declare healType															string	outer "|stop|self|group|xtarget|pet|cure|override"
  /call iniControl FALSE declare switchHealStop												bool		outer	FALSE
  /call iniControl FALSE declare switchHealOverride										bool		outer	0
	/call iniControl FALSE declare switchHeal														bool		outer FALSE
	/call iniControl FALSE class HealOverride														int			Heal 0
	/call iniControl FALSE declare tmpHealOverride											float 	outer 99.99	
	/call iniControl FALSE class switchHealSelf													bool 		Heal FALSE
	/call iniControl FALSE class switchHealGroup												bool 		Heal FALSE
	/call iniControl FALSE class switchHealXtarget											bool 		Heal FALSE
	/call iniControl FALSE class switchHealPet													bool 		Heal FALSE
	/call iniControl FALSE class switchHealCure													bool 		Heal FALSE
                                                                			
	/call iniControl FALSE class cureCurseItem													string 	Cure FALSE
	/call iniControl FALSE class curePoisonItem 												string 	Cure FALSE
	/call iniControl FALSE class cureDiseaseItem												string 	Cure FALSE
	/call iniControl FALSE class cureCorruptItem												string 	Cure FALSE
	/call iniControl FALSE class noCureList	 														string 	Cure "|FALSE|FALSE"
	                                                              			
	/call iniControl FALSE declare clickHealItem												string outer "|Prayers of Life|Overflowing Urn of Life"
	                                                              			
	| Mode stuffs                                                 			
	| Train Spell spells                                          			
	/if (${Bool[${Me.Class.CanCast}]}) {                          			
		/call iniControl FALSE class abjuration														string 	Mode FALSE
		/call iniControl FALSE class alteration														string 	Mode FALSE
		/call iniControl FALSE class conjuration													string 	Mode FALSE
		/call iniControl FALSE class divination														string 	Mode FALSE
		/call iniControl FALSE class evocation														string 	Mode FALSE
	}

/return



|***
 *	DES: sets all timers used that are not specific to a class
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_timers()
	/declare _count int local 0

	/call iniControl FALSE class tRez_Retry											string	Timer 3m
	| /call iniControl FALSE class	tBuff_Kill											string	Timer	1m
	/call iniControl FALSE class	tCheck_snap_retry								string	Timer	15s
	| /call iniControl FALSE class	tAA_picker											string	Timer	5m
	/call iniControl FALSE class	tCheck_Runes										string	Timer	10s
	/call iniControl FALSE class	tScorchedEarth_ignore						string	Timer	30s
	
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

	/call setTimer tPet_Buff												string	Timer 1m 		timer_check_buffs_pet
	/call setTimer tSelf_Buff												string	Timer 3m 		timer_check_buffs_self
	/call setTimer tItem_Buff												string	Timer 1m 		timer_check_buffs_item
	/call setTimer tBC_Buff													string	Timer 20s 	timer_check_buffs_BC
	/call setTimer tRaid_Buff												string	Timer 20s 	timer_check_buffs_raid
	/call setTimer tMerc_Buff												string	Timer 40s 	timer_check_buffs_merc
	/call setTimer tGroup_Buff											string	Timer 5m	 	timer_check_buffs_group
	/call setTimer tTell_Buff												string	Timer	10s 	timer_check_buffs_tell
	/call setTimer tMGB_Buff												string	Timer	5m 		timer_check_buffs_mgb

	/call setTimer tCheck_Aura											string	Timer 3m 		timer_check_aura
	/call setTimer tCheck_Ammo											string	Timer	5s 		timer_Check_Ammo
	/call setTimer tHunt_delay											string	Timer	30s 	timer_wait_for_mob_to_kill
	/call setTimer tAuto_Control_Toon								string	Timer	10s 	timer_auto_toon_control
	/call setTimer tZone_Pause											string	Timer	5s		timer_zone_pause
	/call setTimer tHUD_Death_Check									string	Timer	10s		timer_HUD_death_check
	/call setTimer tNuke_Delay											string	Timer	1			timer_Nuke_Delay
	/call setTimer tWaste_Fireworks									string	Timer 15m		timer_Waste_Fireworks
	/call setTimer tValidate_Zone										string	Timer 10s		timer_validate_zone
	/call setTimer tMode_Open												string	Timer 1s		timer_mode_open


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
 *	DES: Berzerker base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_BER()
	/declare _count int local 0
                                        		
	| Axe Summoning
	/if (${Me.Level} >= 106)  {
		/call iniControl FALSE class RangeItem											string	Common "|Axe of the Mangler|Axe of the Demolisher"
		/call iniControl FALSE class RangeItemSummon								string	Common "|Axe of the Mangler|Axe of the Demolisher"
	} else /if (${Me.Level} >= 101)  {
		/call iniControl FALSE class RangeItem											string	Common "|Axe of the Demolisher"
		/call iniControl FALSE class RangeItemSummon								string	Common "|Axe of the Demolisher"
	} else /if (${Me.Level} >= 96)  {
		/call iniControl FALSE class RangeItem											string	Common "|Axe of the Brute"
		/call iniControl FALSE class RangeItemSummon								string	Common "|Axe of the Brute"
	} else /if (${Me.Level} >= 91)  {
		/call iniControl FALSE class RangeItem											string	Common "|Axe of the Sunderer"
		/call iniControl FALSE class RangeItemSummon								string	Common "|Axe of the Sunderer"
	} else /if (${Me.Level} >= 86)  {
		/call iniControl FALSE class RangeItem											string	Common "|Axe of the Savage"
		/call iniControl FALSE class RangeItemSummon								string	Common "|Axe of the Savage"
	} else /if (${Me.Level} >= 81)  {
		/call iniControl FALSE class RangeItem											string	Common "|Axe of the Eradicator"
		/call iniControl FALSE class RangeItemSummon								string	Common "|Axe of the Eradicator"
	} else /if (${Me.Level} >= 76)  {
		/call iniControl FALSE class RangeItem											string	Common "|Axe of the Decimator"
		/call iniControl FALSE class RangeItemSummon								string	Common "|Axe of the Decimator"
	} else /if (${Me.Level} >= 71)  {
		/call iniControl FALSE class RangeItem											string	Common "|Axe of the Annihilator"
		/call iniControl FALSE class RangeItemSummon								string	Common "|Axe of the Annihilator"
	} else /if (${Me.Level} >= 66)  {
		/call iniControl FALSE class RangeItem											string	Common "|Axe of the Destroyer"
		/call iniControl FALSE class RangeItemSummon								string	Common "|Axe of the Destroyer"
	} else {
		/call iniControl FALSE class RangeItem											string	Common "|FALSE"
		/call iniControl FALSE class RangeItemSummon								string	Common "|FALSE"
	}
		/call iniControl FALSE class RangeItemCount									int			Common 5


	/call iniControl FALSE declare BERspire1											string	outer "Fundament: First Spire of Savagery"
	/call iniControl FALSE declare BERspire2											string	outer "Fundament: Second Spire of Savagery"
	/call iniControl FALSE declare BERspire3											string	outer "Fundament: Third Spire of Savagery"
	/call iniControl FALSE declare BERsynergyAA										string	outer "Fury's Synergy"
	/call iniControl FALSE declare BEREpic1												string	outer "Kerasian Axe of Ire"
	/call iniControl FALSE declare BEREpic15											string	outer "Raging Taelosian Alloy Axe"
	/call iniControl FALSE declare BEREpic2												string	outer "Vengeful Taelosian Blood Axe"
	/call iniControl FALSE class BERAlliance											string	Discs "Demolisher's Alliance"
	/call iniControl FALSE declare BERAAUnity											string	outer FALSE
	/call iniControl FALSE declare BERAAFade											string	outer "Self Preservation"
	
	/call iniControl FALSE class switchAAFade											bool		Buffs FALSE
	/call iniControl FALSE class useAAFadePct											int			Buffs 50

	/call iniControl FALSE class AEMelee					 								bool	 	Discs FALSE
	/call iniControl FALSE class DiscSnare 												string 	Discs FALSE
	/call iniControl FALSE class SnareAxe 												string 	Discs FALSE

	| Volley Line
	/if (${Me.Level} >= 109)  {
		/call iniControl FALSE class DiscVolley 										string 	Discs "Mangling Volley"
	} else /if (${Me.Level} >= 104)  {
		/call iniControl FALSE class DiscVolley 										string 	Discs "Demolishing Volley"
	} else /if (${Me.Level} >= 99)  {
		/call iniControl FALSE class DiscVolley 										string 	Discs "Brutal Volley"
	} else /if (${Me.Level} >= 94)  {
		/call iniControl FALSE class DiscVolley 										string 	Discs "Sundering Volley"
	} else /if (${Me.Level} >= 89)  {
		/call iniControl FALSE class DiscVolley 										string 	Discs "Savage Volley"
	} else /if (${Me.Level} >= 84)  {
		/call iniControl FALSE class DiscVolley 										string 	Discs "Eradicator's Volley"
	} else /if (${Me.Level} >= 79)  {
		/call iniControl FALSE class DiscVolley 										string 	Discs "Decimator's Volley"
	} else /if (${Me.Level} >= 74)  {
		/call iniControl FALSE class DiscVolley 										string 	Discs "Annihilator's Volley"
	} else /if (${Me.Level} >= 69)  {
		/call iniControl FALSE class DiscVolley 										string 	Discs "Destroyer's Volley"
	} else /if (${Me.Level} >= 61)  {
		/call iniControl FALSE class DiscVolley 										string 	Discs "Rage Volley"
	} else {
		/call iniControl FALSE class DiscVolley			 								string	Discs FALSE
	}
	
	/call iniControl FALSE class CryOfChaos 											string 	Discs "Ancient: Cry of Chaos"
	
	| Berserking Line
	/if (${Me.Level} >= 100)  {
		/call iniControl FALSE class DiscBerserking									string 	Discs "Brutal Discipline"
	} else /if (${Me.Level} >= 95)  {
		/call iniControl FALSE class DiscBerserking 								string 	Discs "Sundering Discipline"
	} else /if (${Me.Level} >= 75)  {
		/call iniControl FALSE class DiscBerserking 								string 	Discs "Berserking Discipline"
	} else /if (${Me.Level} >= 60)  {
		/call iniControl FALSE class DiscBerserking 								string 	Discs "Burning Rage Discipline"
	} else /if (${Me.Level} >= 58)  {
		/call iniControl FALSE class DiscBerserking 								string 	Discs "Blind Rage Discipline"
	} else /if (${Me.Level} >= 57)  {
		/call iniControl FALSE class DiscBerserking 								string 	Discs "Focused Fury Discipline"
	} else /if (${Me.Level} >= 54)  {
		/call iniControl FALSE class DiscBerserking 								string 	Discs "Cleaving Rage Discipline"
	} else /if (${Me.Level} >= 53)  {
		/call iniControl FALSE class DiscBerserking 								string 	Discs "Inspired Anger Discipline"
	} else {
		/call iniControl FALSE class DiscBerserking			 						string	Discs FALSE
	}

	| rest disc
	/call iniControl FALSE class DiscRest		 														string	Discs

	| Combat Frenzy
	/if (${Me.Level} >= 106)  {
		/call iniControl FALSE class DiscCombatFrenzy 										string 	Discs "Mangler Frenzy"
	} else /if (${Me.Level} >= 101)  {                            			
		/call iniControl FALSE class DiscCombatFrenzy 										string 	Discs "Demolishing Frenzy"
	} else /if (${Me.Level} >= 96)  {                             			
		/call iniControl FALSE class DiscCombatFrenzy 										string 	Discs "Vanquishing Frenzy"
	} else /if (${Me.Level} >= 91)  {                             			
		/call iniControl FALSE class DiscCombatFrenzy 										string 	Discs "Conquering Frenzy"
	} else /if (${Me.Level} >= 86)  {                             			
		/call iniControl FALSE class DiscCombatFrenzy 										string 	Discs "Overwhelming Frenzy"
	} else /if (${Me.Level} >= 81)  {                             			
		/call iniControl FALSE class DiscCombatFrenzy 										string 	Discs "Overpowering Frenzy"
	} else {                                                      			
		/call iniControl FALSE class DiscCombatFrenzy			 								string	Discs FALSE
	}                                                             			
                                                                			
	| Disc Scream                                                 			
	/if (${Me.Level} >= 92)  {                                    			
		/call iniControl FALSE class DiscScream 													string 	Discs "Perturbing Scream"
	} else /if (${Me.Level} >= 87)  {                             			
		/call iniControl FALSE class DiscScream 													string 	Discs "Distracting Scream"
	} else /if (${Me.Level} >= 82)  {                             			
		/call iniControl FALSE class DiscScream 													string 	Discs "Distressing Scream"
	} else /if (${Me.Level} >= 77)  {                             			
		/call iniControl FALSE class DiscScream 													string 	Discs "Agitating Scream"
	} else /if (${Me.Level} >= 72)  {                             			
		/call iniControl FALSE class DiscScream 													string 	Discs "Unsettling Scream"
	}else {                                                       			
		/call iniControl FALSE class DiscScream			 											string	Discs FALSE
	}

	| Axe Throw
	/if (${Me.Level} >= 108)  {
		/call iniControl FALSE class ThrowingAxe 													string 	Discs "Mangling Axe Throw"
	} else /if (${Me.Level} >= 103)  {                          				
		/call iniControl FALSE class ThrowingAxe 													string 	Discs "Demolishing Axe Throw"
	} else /if (${Me.Level} >= 98)  {                           				
		/call iniControl FALSE class ThrowingAxe 													string 	Discs "Brutal Axe Throw"
	} else /if (${Me.Level} >= 93)  {                           				
		/call iniControl FALSE class ThrowingAxe 													string 	Discs "Spirited Axe Throw"
	} else /if (${Me.Level} >= 88)  {                           				
		/call iniControl FALSE class ThrowingAxe 													string 	Discs "Energetic Axe Throw"
	} else /if (${Me.Level} >= 83)  {                           				
		/call iniControl FALSE class ThrowingAxe 													string 	Discs "Vigorous Axe Throw"
	} else {                                                    				
		/call iniControl FALSE class ThrowingAxe			 										string	Discs FALSE
	}

	| Jolt Line
	/if (${Me.Level} >= 109)  {
		/call iniControl FALSE class DiscJolt 														string 	Discs "Jarring Jolt"
	} else /if (${Me.Level} >= 104)  {                            			
		/call iniControl FALSE class DiscJolt 														string 	Discs "Jarring Smite"
	} else /if (${Me.Level} >= 99)  {                             			
		/call iniControl FALSE class DiscJolt 														string 	Discs "Jarring Crush"
	} else /if (${Me.Level} >= 94)  {                             			
		/call iniControl FALSE class DiscJolt 														string 	Discs "Jarring Blow"
	} else /if (${Me.Level} >= 89)  {                             			
		/call iniControl FALSE class DiscJolt 														string 	Discs "Jarring Slam"
	} else /if (${Me.Level} >= 84)  {                             			
		/call iniControl FALSE class DiscJolt 														string 	Discs "Jarring Clash"
	} else /if (${Me.Level} >= 79)  {                             			
		/call iniControl FALSE class DiscJolt 														string 	Discs "Jarring Smash"
	} else /if (${Me.Level} >= 74)  {                             			
		/call iniControl FALSE class DiscJolt 														string 	Discs "Jarring Strike"
	} else /if (${Me.Level} >= 69)  {                             			
		/call iniControl FALSE class DiscJolt 														string 	Discs "Baffling Strike"
	} else /if (${Me.Level} >= 65)  {                             			
		/call iniControl FALSE class DiscJolt 														string 	Discs "Confusing Strike"
	} else /if (${Me.Level} >= 48)  {                             			
		/call iniControl FALSE class DiscJolt 														string 	Discs "Distracting Strike"
	} else /if (${Me.Level} >= 24)  {                             			
		/call iniControl FALSE class DiscJolt 														string 	Discs "Divertive Strike"
	} else {                                                      			
		/call iniControl FALSE class DiscJolt			 												string	Discs FALSE
	}
	
	| Bloodlust
	/if (${Me.Level} >= 110)  {
		/call iniControl FALSE class SharedBloodLust 											string 	Discs "Shared Ruthlessness"
		/call iniControl FALSE class SharedBloodLustRecourse							string 	Discs "Reflected Ruthlessness"
	} else /if (${Me.Level} >= 105)  {                            			
		/call iniControl FALSE class SharedBloodLust 											string 	Discs "Shared Cruelty"
		/call iniControl FALSE class SharedBloodLustRecourse							string 	Discs "Reflected Cruelty"
	} else /if (${Me.Level} >= 100)  {                            			
		/call iniControl FALSE class SharedBloodLust 											string 	Discs "Shared Viciousness"
		/call iniControl FALSE class SharedBloodLustRecourse							string 	Discs "Reflected Viciousness"
	} else /if (${Me.Level} >= 95)  {                             			
		/call iniControl FALSE class SharedBloodLust 											string 	Discs "Shared Savagery"
		/call iniControl FALSE class SharedBloodLustRecourse							string 	Discs "Reflected Savagery"
	} else /if (${Me.Level} >= 90)  {                             			
		/call iniControl FALSE class SharedBloodLust 											string 	Discs "Shared Brutality"
		/call iniControl FALSE class SharedBloodLustRecourse							string 	Discs "Reflected Brutality"
	} else /if (${Me.Level} >= 85)  {                             			
		/call iniControl FALSE class SharedBloodLust 											string 	Discs "Shared Bloodlust"
		/call iniControl FALSE class SharedBloodLustRecourse							string 	Discs "Reflected Bloodlust"
	} else {                                                      			
		/call iniControl FALSE class SharedBloodLust 											string 	Discs FALSE
		/call iniControl FALSE class SharedBloodLustRecourse							string 	Discs FALSE
	}
	
	| Overwhelming Frenzy Line
	/if (${Me.Level} >= 110)  {
		/call iniControl FALSE class DiscOverwhelmingFrenzy								string 	Discs "Torrid Frenzy"
	} else /if (${Me.Level} >= 105)  {                            			
		/call iniControl FALSE class DiscOverwhelmingFrenzy 							string 	Discs "Stormwild Frenzy"
	} else /if (${Me.Level} >= 100)  {                            			
		/call iniControl FALSE class DiscOverwhelmingFrenzy 							string 	Discs "Fearless Frenzy"
	} else /if (${Me.Level} >= 95)  {                             			
		/call iniControl FALSE class DiscOverwhelmingFrenzy 							string 	Discs "Steel Frenzy"
	} else /if (${Me.Level} >= 90)  {                             			
		/call iniControl FALSE class DiscOverwhelmingFrenzy 							string 	Discs "Fighting Frenzy"
	} else /if (${Me.Level} >= 85)  {                             			
		/call iniControl FALSE class DiscOverwhelmingFrenzy 							string 	Discs "Combat Frenzy"
	} else /if (${Me.Level} >= 80)  {                             			
		/call iniControl FALSE class DiscOverwhelmingFrenzy 							string 	Discs "Battle Frenzy"
	} else {                                                      			
		/call iniControl FALSE class DiscOverwhelmingFrenzy			 					string	Discs FALSE
	}

	| Cleaving Anger Line
	/if (${Me.Level} >= 86)  {
		/call iniControl FALSE class DiscCleavingAnger										string 	Discs "Cleaving Acrimony Discipline"
	} else /if (${Me.Level} >= 65)  {                             			
		/call iniControl FALSE class DiscCleavingAnger 										string 	Discs "Cleaving Anger Discipline"
	} else /if (${Me.Level} >= 54)  {                             			
		/call iniControl FALSE class DiscCleavingAnger 										string 	Discs "Cleaving Rage Discipline"
	} else {                                                      			
		/call iniControl FALSE class DiscCleavingAnger			 							string	Discs FALSE
	}                                                             			
                                                                			
	| Opening Attack Line                                         			
	/if (${Me.Level} >= 107)  {                                   			
		/call iniControl FALSE class DiscSlapintheFace										string 	Discs "Sucker Punch"
	} else /if (${Me.Level} >= 102)  {                            			
		/call iniControl FALSE class DiscSlapintheFace 										string 	Discs "Kick in the Shins"
	} else /if (${Me.Level} >= 97)  {                             			
		/call iniControl FALSE class DiscSlapintheFace 										string 	Discs "Punch in the Throat"
	} else /if (${Me.Level} >= 92)  {                             			
		/call iniControl FALSE class DiscSlapintheFace 										string 	Discs "Kick in the Teeth"
	} else /if (${Me.Level} >= 87)  {                             			
		/call iniControl FALSE class DiscSlapintheFace 										string 	Discs "Kick in the Shins"
	} else {                                                      			
		/call iniControl FALSE class DiscSlapintheFace			 							string	Discs FALSE
	}                                                             			
                                                                			
	| Axe of Line                                                 			
	/if (${Me.Level} >= 110)  {                                   			
		/call iniControl FALSE class DiscAxeofRallos											string 	Discs "Axe of Rekatok"
	} else /if (${Me.Level} >= 105)  {                            			
		/call iniControl FALSE class DiscAxeofRallos 											string 	Discs "Axe of Numicia"
	} else /if (${Me.Level} >= 102)  {                            			
		/call iniControl FALSE class DiscAxeofRallos 											string 	Discs "Axe of Aeons"
	} else /if (${Me.Level} >= 100)  {                            			
		/call iniControl FALSE class DiscAxeofRallos 											string 	Discs "Axe of Zurel"
	} else /if (${Me.Level} >= 95)  {                             			
		/call iniControl FALSE class DiscAxeofRallos 											string 	Discs "Axe of Illdaera"
	} else /if (${Me.Level} >= 90)  {                             			
		/call iniControl FALSE class DiscAxeofRallos 											string 	Discs "Axe of Graster"
	} else /if (${Me.Level} >= 85)  {                             			
		/call iniControl FALSE class DiscAxeofRallos 											string 	Discs "Axe of Rallos"
	} else {                                                      			
		/call iniControl FALSE class DiscAxeofRallos			 								string	Discs FALSE
	}

	| Flurry Line
	/if (${Me.Level} >= 89)  {
		/call iniControl FALSE class DiscVengeful													string 	Discs "Avenging Flurry Discipline"
	} else /if (${Me.Level} >= 60)  {                             			
		/call iniControl FALSE class DiscVengeful 												string 	Discs "Vengeful Flurry Discipline"
	} else {                                                      			
		/call iniControl FALSE class DiscVengeful			 										string	Discs FALSE
	}                                                             			
                                                                			
	| Rage Line                                                   			
	/if (${Me.Level} >= 102)  {                                   			
		/call iniControl FALSE class DiscFesteringRage										string 	Discs "Bubbling Rage"
	} else /if (${Me.Level} >= 98)  {                             			
		/call iniControl FALSE class DiscFesteringRage 										string 	Discs "Festering Rage"
	} else {                                                      			
		/call iniControl FALSE class DiscFesteringRage			 							string	Discs FALSE
	}                                                             			
                                                                			
	| Cry Havoc Line                                              			
	/if (${Me.Level} >= 98)  {                                    			
		/call iniControl FALSE class DiscCryHavoc													string 	Discs "Cry Carnage"
	} else /if (${Me.Level} >= 68)  {                             			
		/call iniControl FALSE class DiscCryHavoc 												string 	Discs "Cry Havoc"
	} else {                                                      			
		/call iniControl FALSE class DiscCryHavoc			 										string	Discs FALSE
	}                                                             			
                                                                			
	| Augmented Frenzy Line                                       			
	/if (${Me.Level} >= 109)  {                                   			
		/call iniControl FALSE class DiscAugmentedFrenzy									string 	Discs "Bolstered Frenzy"
		/call iniControl FALSE declare AugmentedFrenzyEffect 							string 	outer "Bolstered Frenzy Effect"
	} else /if (${Me.Level} >= 104)  {                            			
		/call iniControl FALSE class DiscAugmentedFrenzy 									string 	Discs "Amplified Frenzy"
		/call iniControl FALSE declare AugmentedFrenzyEffect 							string 	outer "Amplified Frenzy Effect"
	} else /if (${Me.Level} >= 99)  {                             			
		/call iniControl FALSE class DiscAugmentedFrenzy 									string 	Discs "Augmented Frenzy"
		/call iniControl FALSE declare AugmentedFrenzyEffect 							string 	outer "Augmented Frenzy Effect"
	} else {                                                      			
		/call iniControl FALSE class DiscAugmentedFrenzy			 						string	Discs FALSE
		/call iniControl FALSE declare AugmentedFrenzyEffect 							string 	outer FALSE
	}

	| Frenzied Line
	/if (${Me.Level} >= 94)  {
		/call iniControl FALSE class DiscFrenzyResolve										string 	Discs "Frenzied Resolve"
	} else {                                                      			
		/call iniControl FALSE class DiscFrenzyResolve			 							string	Discs FALSE
	}                                                             			
	                                                              			
	/call iniControl FALSE class UntamedgRage														string 	Discs "Untamed Rage"
	/call iniControl FALSE class RecklessAbandon												string 	Discs "Reckless Abandon"
 	/call iniControl FALSE class frenzy																	bool		meleedisc FALSE
                                                                			
 	| Dichotomic Line                                             			
	/if (${Me.Level} >= 101)  {                                   			
		/call iniControl FALSE class DiscDicho														string 	Discs "Dichotomic Rage"
	} else {                                                      			
		/call iniControl FALSE class DiscDicho			 											string	Discs FALSE
	}                                                             			
                                                                			
	| Disconcerting Line                                          			
	/if (${Me.Level} >= 108)  {                                   			
		/call iniControl FALSE class DiscDisconcert												string 	Discs "Mangling Discipline"
	} else /if (${Me.Level} >= 104)  {                            			
		/call iniControl FALSE class DiscDisconcert 											string 	Discs "Disconcerting Discipline"
	} else {                                                      			
		/call iniControl FALSE class DiscDisconcert			 									string	Discs FALSE
	}                                                             			
                                                                			
	| Endurance Tap Line                                          			
	/if (${Me.Level} >= 103)  {                                   			
		/call iniControl FALSE class DiscEnduranceTap											string 	Discs "Sapping Strikes"
	} else {                                                      			
		/call iniControl FALSE class DiscEnduranceTap			 								string	Discs FALSE
	}                                                             			
                                                                			
	| Pet Line                                                    			
	/if (${Me.Level} >= 100)  {                                   			
		/call iniControl FALSE class DiscPet															string 	Discs "Phantom Assailant"
	} else {                                                      			
		/call iniControl FALSE class DiscPet			 												string	Discs FALSE
	}                                                             			
                                                                			
	/call iniControl FALSE class ${Me.Class.ShortName}Point 						int 		Heal 60

	| Open Wound Line
	/if (${Me.Level} >= 106)  {
		/call iniControl FALSE class DiscOpenWound												string 	Discs "Ritual Scarification"
		/call iniControl FALSE declare OpenWoundEffect										string 	outer "Ritual Scarification"
	} else /if (${Me.Level} >= 96)  {                             			
		/call iniControl FALSE class DiscOpenWound 												string 	Discs "Open Wound"
		/call iniControl FALSE declare OpenWoundEffect										string 	outer "Open Wound"
	} else {                                                      			
		/call iniControl FALSE class DiscOpenWound			 									string	Discs FALSE
		/call iniControl FALSE declare OpenWoundEffect										string 	outer FALSE
	}                                                             			
                                                                			
	/call iniControl FALSE class buffGem 																string 	Spells FALSE
	/call iniControl FALSE class spellCripple 													string 	Spells FALSE

/return		                                        		



|***
 *	DES: Bard base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_BRD()
	/declare _count int local 0
	/call setTimer tValidate_Lists																			string	Timer 15s timer_validate_lists
	/call iniControl FALSE declare BRDspire1														string	outer "Fundament: First Spire of the Minstrel"
	/call iniControl FALSE declare BRDspire2														string	outer "Fundament: Second Spire of the Minstrel"
	/call iniControl FALSE declare BRDspire3														string	outer "Fundament: Third Spire of the Minstrel"
	/call iniControl FALSE declare BRDsynergyAA													string	outer "Troubadour's Synergy"
	/call iniControl FALSE declare BRDEpic1															string	outer "Singing Short Sword"
	/call iniControl FALSE declare BRDEpic15														string	outer "Prismatic Dragon Blade"
	/call iniControl FALSE declare BRDEpic2															string	outer "Prismatic Dragon Blade"
	/call iniControl FALSE declare BRDAlliance													string	Song "Alliance of Sticks and Stones"
	/call iniControl FALSE declare BRDAAUnity														string	outer FALSE
	/call iniControl FALSE declare BRDAAFade														string	outer "Fading Memories"

	/call iniControl FALSE class switchAAFade														bool		Buffs FALSE
	/call iniControl FALSE class useAAFadePct														int			Buffs 50

	| Servant Nuke AA1
	/call iniControl FALSE class ServantNukeAA1 												string 	Pet
	
	| Servant Nuke AA2			
	/call iniControl FALSE class ServantNukeAA2 												string 	Pet
	
	| Servant Nuke song
	/call iniControl FALSE class songServantNuke												string 	Pet
	
	                                          		
	/call iniControl FALSE declare lineType															string	outer |caster|melee|mix|other|tank
	/call iniControl FALSE class lineCurrent														string 	Song Tank
	/call iniControl FALSE class lineTank																string	Song "|Chorus of Sionachie|War March of Jocelyn|Fjilnauk's Spiteful Lyric|Fjilnauk's Song of Suffering|Doben's Spry Sonata"
	/call iniControl FALSE class lineCaster															string	Song "|Chorus of Sionachie|Nilsara's Aria|Fatesong of Jocelyn|Arcane Melody|Aria of Maetanrus"
	/call iniControl FALSE class lineMelee															string	Song "|Chorus of Sionachie|War March of Jocelyn|Fjilnauk's Spiteful Lyric|Fjilnauk's Song of Suffering|Arcane Melody"
	/call iniControl FALSE class lineMix																string	Song "|Chorus of Sionachie|War March of Jocelyn|Fjilnauk's Spiteful Lyric|Nilsara's Aria"
	/call iniControl FALSE class lineOther															string	Song FALSE
	                                          		
	/call iniControl FALSE class songDicho															string	Song "Dichotomic Psalm"
	                                          		
	/call iniControl FALSE class DoNuke					 												int			Song 0
	/call iniControl FALSE class Nuke1																	string 	Song FALSE
	/call iniControl FALSE class Nuke2																	string 	Song FALSE
                                            		  										
	/call iniControl FALSE class DoDoT						 											int			Song 0
	/call iniControl FALSE class DoT1																		string 	Song FALSE
	/call iniControl FALSE class DoT2																		string 	Song FALSE
                                            		
	| Single Mez song
	/call iniControl FALSE class spellMez																string 	Song

	| AE Mez song
	/call iniControl FALSE class spellpbaeMez														string 	Song

	/call iniControl FALSE class spellSlow															string	Song "Requiem of Time"
	/call iniControl FALSE class switchRestSong													bool		Song FALSE
	/call iniControl FALSE class spellRestSong													string	Song FALSE
	/call iniControl FALSE class spellCripple 													string 	Spells FALSE
	/call iniControl FALSE class ${Me.Class.ShortName}Point		 					int 		Heal 80
	                                          		  										
	/call iniControl FALSE class switchIntimidKick 											bool	 	meleedisc FALSE
	/call iniControl FALSE class switchModRod														bool	 	Buffs TRUE
	
	/return



|***
 *	DES: Beastlord base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_BST()
	/declare _count int local 0

	/call iniControl FALSE declare BSTspire1											string	outer "Fundament: First Spire of the Savage Lord"
	/call iniControl FALSE declare BSTspire2											string	outer "Fundament: Second Spire of the Savage Lord"
	/call iniControl FALSE declare BSTspire3											string	outer "Fundament: Third Spire of the Savage Lord"
	/call iniControl FALSE declare BSTsynergyAA										string	outer "Primalist's Synergy"
	/call iniControl FALSE declare BSTEpic1												string	outer "Claw of the Savage Spirit"
	/call iniControl FALSE declare BSTEpic15											string	outer "Savage Lord's Totem"
	/call iniControl FALSE declare BSTEpic2												string	outer "Spiritcaller Totem of the Feral"
	/call iniControl FALSE class BSTAlliance											string	Spells "Venomous Alliance"
	/call iniControl FALSE declare BSTAAUnity											string	outer "Feralist's Unity"
	/call iniControl FALSE declare BSTAAFade											string	outer FALSE
	
	/call iniControl FALSE class switchAAFade										bool		Buffs FALSE
	/call iniControl FALSE class useAAFadePct										int			Buffs 50

	/call iniControl FALSE class DiscForay 											string 	Spells "Pummel"
	/call iniControl FALSE class DiscFocusClaws 									string 	Spells "Focused Clamor of Claws"
	/call iniControl FALSE class DiscSavage 											string 	Spells "Savage Rage"
	/call iniControl FALSE class DiscRuaabri 										string 	Spells "Ruaabri's Fury"	
	/call iniControl FALSE class GrowlSpell 											string 	Spells "Growl of the Leopard"
 	/call iniControl FALSE class ServantNuke		 									string 	Spells "Cry at the Moon"
  /call iniControl FALSE class FeroSpell 											string 	Spells "Savage Ferocity"
  /call iniControl FALSE class DiscEnduring										string 	Spells "Enduring Frenzy"
  /call iniControl FALSE class DiscRending											string 	Spells "Reflexive Rending"
	                                           		
	/call iniControl FALSE class GrowlSpell 											string 	Pet "Growl of the Leopard"
	/call iniControl FALSE class TasteofBlood										string 	Pet "Taste of Blood"

	/call iniControl FALSE class ProcHeal												string 	Pet "Mending Warder"
	/call iniControl FALSE class Calm														string 	Pet "Companion's Calm Demeanor"
	/call iniControl FALSE class ParagonTXT 											string 	Spells "PARAGON DONE (+5790 HP/tick, +2450 Mana/tick, +264 End/tick)"
	                                           		

	                                           		
	/call iniControl FALSE class FastHeal			 									string	Spells "Sabhattin's Mending" 

	/call iniControl FALSE class switchSelfParagon		 						bool		Spells TRUE
	/call iniControl FALSE class selfParagonPct 									int			Spells 80
	/call iniControl FALSE class switchConsumeSpirit	 						bool		Spells TRUE
	/call iniControl FALSE class ConsumeSpiritPct								int			Spells 80
	/call iniControl FALSE class switchGroupParagon		 					bool		Spells TRUE
	/call iniControl FALSE class groupParagonPct 								int			Spells 50
                                          		
	/call iniControl FALSE class spellSlow		 										string	Spells "Sha's Reprisal"		
	/call iniControl FALSE class spellCripple 										string 	Spells "Incapacitate"
                                             		
	/call iniControl FALSE class ${Me.Class.ShortName}Point 			int 		Heal 80
	/call iniControl FALSE class switchModRod										bool	 	Buffs TRUE



	| rest disc
	/call iniControl FALSE class DiscRest		 											string	Discs


	| Nukes
	/call iniControl FALSE class DoNuke					 								int			Spells 0
	/for _count 1 to 6
		/call iniControl FALSE class Nuke${_count}								string	Spells FALSE
		/call iniControl FALSE class Nuke${_count}Condition				string	Spells FALSE
	/next _count
	
	| DoTs
	/call iniControl FALSE class DoDoT								 						int			Spells 0
	/for _count 1 to 6
		/call iniControl FALSE class DoT${_count}									string	Spells FALSE
		/call iniControl FALSE class DoT${_count}chk								string	Spells FALSE
		/call iniControl FALSE class DoT${_count}Condition					string	Spells FALSE
	/next _count


/return



|***
 *	DES: Cleric base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_CLR()
	/declare _count int local 0
	
	/call iniControl FALSE declare CLRspire1														string	outer "Fundament: First Spire of Divinity"
	/call iniControl FALSE declare CLRspire2														string	outer "Fundament: Second Spire of Divinity"
	/call iniControl FALSE declare CLRspire3														string	outer "Fundament: Third Spire of Divinity"
	/call iniControl FALSE declare CLRsynergyAA													string	outer "Templar's Synergy"
	/call iniControl FALSE declare CLREpic1															string	outer "Water Sprinkler of Nem Ankh"
	/call iniControl FALSE declare CLREpic15														string	outer "Harmony of the Soul"
	/call iniControl FALSE declare CLREpic2															string	outer "Aegis of Superior Divinity"
	/call iniControl FALSE class CLRAlliance														string	Spells "Divine Alliance"
	/call iniControl FALSE declare CLRAAUnity														string	outer "Saint's Unity"
	/call iniControl FALSE declare CLRAAFade														string	outer "Divine Peace"
	
	/call iniControl FALSE class switchAAFade														bool		Buffs FALSE
	/call iniControl FALSE class useAAFadePct														int			Buffs 50

	/call setTimer	tHealWard																						string	Timer 185s timer_heal_ward
	/call iniControl FALSE class switchLockMA														bool 		Heal FALSE
	/call iniControl FALSE class switchModRod														bool	 	Buffs TRUE
	/call iniControl FALSE class switchVP																bool		Buffs TRUE
	/call iniControl FALSE class useVPPct																int			Buffs 40
	/call iniControl FALSE class spellCripple 													string 	Spells FALSE

	/call iniControl FALSE class BreastplateHeal		 										string	Items FALSE
	/call iniControl FALSE class BreastplateHealCondition								string	Items FALSE		

	/call iniControl FALSE declare hurtID		 														string 	outer FALSE
	/call iniControl FALSE declare hurtPctHP														int 		outer 100
	/call iniControl FALSE declare hurtCLS		 													string 	outer FALSE
	/call iniControl FALSE declare hurtCount 														int 		outer 0
	
	/call iniControl FALSE class DivineRezCleric												bool 		Heal TRUE
	/call iniControl FALSE class RandRezDelay														int 		Heal 5
	/call iniControl FALSE class ProtectOutsideGroup										string	Heal "|FALSE"
	/call iniControl FALSE class ProtectInsideRaid											string	Heal "|FALSE"
	/call iniControl FALSE class ProtectNPCList													string	Heal "|FALSE"
	/call iniControl FALSE class useGroupHeal 													bool		Heal TRUE
	/call iniControl FALSE class useGroupHoT														bool		Heal TRUE
	/call iniControl FALSE class setDivinetoGroupHeal										bool		Heal FALSE

  /call iniControl FALSE class useCelestialRegen											bool	  Heal TRUE
	/call iniControl FALSE class CRTXT																	string	Heal "Celestial Regeneration"
	
	/call iniControl FALSE class XTHealRadius														int			Heal 300
	/call iniControl FALSE class switchForceGroupHeal										bool		Heal FALSE
	/call iniControl FALSE class RampageTank														string	Heal FALSE
	/call iniControl FALSE class switchHealAggressive										bool		Heal FALSE
	/call iniControl FALSE class switchCompleteHeal											bool		Heal FALSE
	/call iniControl FALSE class CompleteHealpct												int 		Heal 70	

	| group heals (not cures) *mostly                                            		
	/call iniControl FALSE class healWholeGroup 												string  Heal
  /call iniControl FALSE class healWholeGroup2												string 	Heal FALSE
  /call iniControl FALSE class healWholeGroup3												string 	Heal FALSE

	| group HoT
  /call iniControl FALSE class healWholeGroupHoT 											string  Heal

	| remedies
  /call iniControl FALSE class healRemedy1														string 	Heal
  /call iniControl FALSE class healRemedy2														string 	Heal
	/call iniControl FALSE class healRemedy3														string 	Heal
                                                                			
  /call iniControl FALSE class healHoT 																string 	Heal FALSE
                                                                			
	| target AE splash heals                                      			
  /call iniControl FALSE class healSplash 														string  Heal

	| interventions
  /call iniControl FALSE class healIntervention1 											string  Heal
  /call iniControl FALSE class healIntervention2 											string  Heal
  /call iniControl FALSE class healIntervention3 											string 	Heal
  /call iniControl FALSE class healIntervention4 											string  Heal

	| renewals
  /call iniControl FALSE class healRenewal1 													string  Heal
  /call iniControl FALSE class healRenewal2 													string  Heal
  /call iniControl FALSE class healRenewal3 													string 	Heal

	| panic heals
  /call iniControl FALSE class healPanic 															string  Heal
                                                              				
  | wards                                                     				
  /call iniControl FALSE class healWard			 													string  Heal
  /call iniControl FALSE class healWardToon	 													string  Heal "|FALSE"

	| healing pet shield
	/call iniControl FALSE class switchIssuance													bool		Heal FALSE
	/call iniControl FALSE class issuanceMobCount												int			Heal 4
  /call iniControl FALSE class healIssuance 													string  Heal
	
	| yodeling anyone?                                       		
	/call iniControl FALSE class doYaulp 																bool		Buffs TRUE
	/call iniControl FALSE class useYaulpPct														int			Buffs 98
  /call iniControl FALSE class spellYaulp 														string  Buffs
	
	| hammer of ass kickery	
  /call iniControl FALSE class sumWeaponSpell 												string  Buffs
  /call iniControl FALSE class sumWeaponItem 													string  Buffs
	
	| cute hammer pet
  /call iniControl FALSE class tempPet 																string  Pet
	| /call iniControl FALSE class tempPetCondition 										string  Pet FALSE
	
	| Nukes
	/call iniControl FALSE class DoNuke					 												int			Spells 0
	/for _count 1 to 6                                            			
		/call iniControl FALSE class Nuke${_count}												string	Spells FALSE
		/call iniControl FALSE class Nuke${_count}Condition								string	Spells FALSE
	/next _count                                                  			
	                                                              			
	| DoTs                                                        			
	/call iniControl FALSE class DoDoT								 									int			Spells 0
	/for _count 1 to 6                                            			
		/call iniControl FALSE class DoT${_count}													string	Spells FALSE
		/call iniControl FALSE class DoT${_count}chk											string	Spells FALSE
		/call iniControl FALSE class DoT${_count}Condition								string	Spells FALSE
	/next _count
	
/return		                                        		



|***
 *	DES: Druid base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_DRU()
	/declare _count int local 0

	/call iniControl FALSE declare DRUspire1														string	outer "Fundament: First Spire of Nature"
	/call iniControl FALSE declare DRUspire2														string	outer "Fundament: Second Spire of Nature"
	/call iniControl FALSE declare DRUspire3														string	outer "Fundament: Third Spire of Nature"
	/call iniControl FALSE declare DRUsynergyAA													string	outer "Preserver's Synergy"
	/call iniControl FALSE declare DRUEpic1															string	outer "Nature Walker's Scimitar"
	/call iniControl FALSE declare DRUEpic15														string	outer "Staff of Living Brambles"
	/call iniControl FALSE declare DRUEpic2															string	outer "Staff of Everliving Brambles"
	/call iniControl FALSE class DRUAlliance														string	Spells "Bosquetender's Alliance"
	/call iniControl FALSE declare DRUAAUnity														string	outer "Wildtender's Unity"
	/call iniControl FALSE declare DRUAAFade														string	outer "Veil of the Underbrush"
                                                                			
	/call iniControl FALSE class switchAAFade														bool		Buffs FALSE
	/call iniControl FALSE class useAAFadePct														int			Buffs 50
                                                                			
	/call iniControl FALSE class switchModRod														bool	 	Buffs TRUE
	                                                              			
	/call iniControl FALSE class debuffRo			 													string	Spells "Blessing of Ro"	
	/call iniControl FALSE class WoodTXT																string	Spells "SoWood cast."
                                        		                    			
	/call iniControl FALSE class spellCripple 													string 	Spells FALSE
	/call iniControl FALSE class AAWolfSpirit														string 	Buffs FALSE
                                                                			
	/call iniControl FALSE class XTHealRadius														int			Heal 300
	/call iniControl FALSE class RampageTank														string	Heal FALSE
	/call iniControl FALSE declare hurtID		 														string 	outer FALSE
	/call iniControl FALSE declare hurtPctHP														int 		outer 100
	/call iniControl FALSE declare hurtCLS		 													string 	outer FALSE
	/call iniControl FALSE declare hurtCount 														int 		outer 0
	/call iniControl FALSE class RandRezDelay														int 		Heal 10
	/call iniControl FALSE class ProtectOutsideGroup										string	Heal "|FALSE"
	/call iniControl FALSE class ProtectInsideRaid											string	Heal "|FALSE"
	/call iniControl FALSE class ProtectNPCList													string	Heal "|FALSE"
	/call iniControl FALSE class useGroupHeal 													bool		Heal TRUE
	/call iniControl FALSE class useGroupHoT														bool		Heal TRUE
                                           	
	
	
	
	
	/call iniControl FALSE class healPanic			 									string  Heal "Survival of the Serendipitous"
	
	
	/call iniControl FALSE class healRejuvilation								string  Heal "Resurgence"
	
	
	/call iniControl FALSE class healAdrenaline				 					string 	Heal "Daggerthorn Rush"
	
	
	
	/call iniControl FALSE class healSanavida	 									string  Heal "Panavida"
	
	
	/call iniControl FALSE class healWholeGroup									string 	Heal "Lunasoothe"







	| temp HP grow line
	/call iniControl FALSE class switchGrowth														bool  Spells FALSE
 	/if (${Me.Level} >= 107) {                                					
	  /call iniControl FALSE class spellGrowth 													string  Spells "Savage Growth"
	} else /if (${Me.Level} >= 102) {	                        					
	  /call iniControl FALSE class spellGrowth 													string  Spells "Ferocious Growth"
	} else /if (${Me.Level} >= 97) {	                        					
	  /call iniControl FALSE class spellGrowth 													string  Spells "Rampant Growth"
	} else /if (${Me.Level} >= 92) {	                        					
	  /call iniControl FALSE class spellGrowth 													string  Spells "Unfettered Growth"
	} else /if (${Me.Level} >= 87) {	                        					
	  /call iniControl FALSE class spellGrowth 													string  Spells "Untamed Growth"
	} else /if (${Me.Level} >= 82) {	                        					
	  /call iniControl FALSE class spellGrowth 													string  Spells "Wild Growth"
	} else {	                                                					
	  /call iniControl FALSE class spellGrowth 													string  Spells FALSE
	}                                                         					
	/call iniControl FALSE class spellGrowthToon												string  Spells |FALSE









	| Nukes
	/call iniControl FALSE class DoNuke					 								int			Spells 0
	/for _count 1 to 6
		/call iniControl FALSE class Nuke${_count}									string	Spells FALSE
		/call iniControl FALSE class Nuke${_count}Condition				string	Spells FALSE
	/next _count
	
	| DoTs
	/call iniControl FALSE class DoDoT								 						int			Spells 0
	/for _count 1 to 6
		/call iniControl FALSE class DoT${_count}									string	Spells FALSE
		/call iniControl FALSE class DoT${_count}chk								string	Spells FALSE
		/call iniControl FALSE class DoT${_count}Condition					string	Spells FALSE
	/next _count





















	| ports	
	/call iniControl FALSE declare looping															string	outer "Zephyr: Looping Planes"
	/call iniControl FALSE declare steamfront														string	outer "Zephyr: Steamfront"
	/call iniControl FALSE declare butcherblock													string	outer "Zephyr: Butcherblock"
	/call iniControl FALSE declare shardslanding												string	outer "Zephyr: Shard's Landing"
	/call iniControl FALSE declare pillars															string	outer "Zephyr: Pillars of Alra"
	/call iniControl FALSE declare beast																string	outer "Zephyr: Beasts' Domain"
	/call iniControl FALSE declare westkarna														string	outer "Zephyr: West Karana"
	/call iniControl FALSE declare lavastorm														string	outer "Zephyr: Lavastorm"
	/call iniControl FALSE declare undershore														string	outer "Zephyr: Undershore"
	/call iniControl FALSE declare misty																string	outer "Zephyr: Misty"
	/call iniControl FALSE declare ro																		string	outer "Zephyr: Ro"
	/call iniControl FALSE declare feerrott															string	outer "Zephyr: Feerrott"
	/call iniControl FALSE declare commonlands													string	outer "Zephyr: Commonlands"
	/call iniControl FALSE declare surefall															string	outer "Zephyr: Surefall Glade"
	/call iniControl FALSE declare karana																string	outer "Zephyr: Karana"
	/call iniControl FALSE declare wallofslaughter 											string	outer "Zephyr: Slaughter"
	/call iniControl FALSE declare bloodfields 													string	outer "Zephyr: Bloodfields"
	/call iniControl FALSE declare lceanium															string	outer "Zephyr: Lceanium"
	/call iniControl FALSE declare dreadlands														string	outer "Zephyr: Combines"
	/call iniControl FALSE declare dawnshroud														string	outer "Zephyr: Dawnshroud"
	/call iniControl FALSE declare twilight															string	outer "Zephyr: Twilight"
	/call iniControl FALSE declare grimling															string	outer "Zephyr: Grimling"
	/call iniControl FALSE declare nexus																string	outer "Zephyr: Nexus"
	/call iniControl FALSE declare primarypush													string	outer "Primary Anchor Push"
	/call iniControl FALSE declare secondarypush												string	outer "Secondary Anchor Push"
	/call iniControl FALSE declare primary															string	outer "Zephyr: Primary Anchor"
	/call iniControl FALSE declare secondary														string	outer "Zephyr: Secondary Anchor"
	/call iniControl FALSE declare tempesttemple												string	outer "Zephyr: Tempest Temple"
	/call iniControl FALSE declare buriedsea														string	outer "Zephyr: Buried Sea"
	/call iniControl FALSE declare stonebrunt														string	outer "Zephyr: Stonebrunt"
	/call iniControl FALSE declare tox																	string	outer "Zephyr: Toxxulia"
	/call iniControl FALSE declare direwind															string	outer "Zephyr: Direwind"
	/call iniControl FALSE declare steppes															string	outer "Zephyr: Steppes"
	/call iniControl FALSE declare blightfire														string	outer "Zephyr: Blightfire Moors"
	/call iniControl FALSE declare barindu															string	outer "Zephyr: Barindu"
	/call iniControl FALSE declare natimbi															string	outer "Zephyr: Natimbi"
	/call iniControl FALSE declare grounds															string	outer "Zephyr: the Grounds"
	/call iniControl FALSE declare potime																string	outer "Zephyr: Plane of Time"
	/call iniControl FALSE declare brells																string	outer "Zephyr: Brell's Rest"
	/call iniControl FALSE declare arcstone															string	outer "Zephyr: Arcstone"
	/call iniControl FALSE declare knowledge														string	outer "Zephyr: Knowledge"
	/call iniControl FALSE declare cobaltscar														string	outer "Zephyr: Cobalt Scar"
	/call iniControl FALSE declare wakening															string	outer "Zephyr: Wakening Land"
	/call iniControl FALSE declare greatdivide													string	outer "Zephyr: Great Divide"
	/call iniControl FALSE declare iceclad															string	outer "Zephyr: Iceclad"	
	
	
	
	
/return		                                        		



|***
 *	DES: Enchanter base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_ENC()
	/declare _count int local 0

	/call setTimer tValidate_Lists																string	Timer 15s timer_validate_lists

	/call iniControl FALSE declare ENCspire1											string	outer "Fundament: First Spire of Enchantment"
	/call iniControl FALSE declare ENCspire2											string	outer "Fundament: Second Spire of Enchantment"
	/call iniControl FALSE declare ENCspire3											string	outer "Fundament: Third Spire of Enchantment"
	/call iniControl FALSE declare ENCIoG													string	outer "Illusions of Grandeur"
	/call iniControl FALSE declare ENCMC													string	outer "Mental Contortion"
	/call iniControl FALSE declare ENCsynergyAA										string	outer "Begulier's Synergy"
	/call iniControl FALSE declare ENCEpic1												string	outer "Staff of the Serpent"
	/call iniControl FALSE declare ENCEpic15											string	outer "Oculus of Persuasion"
	/call iniControl FALSE declare ENCEpic2												string	outer "Staff of Eternal Eloquence"
	/call iniControl FALSE class ENCAlliance											string	Spells "Chromatic Alliance"
	/call iniControl FALSE declare ENCAAUnity											string	outer "Orator's Unity"
	/call iniControl FALSE declare ENCAAFade											string	outer "Self Stasis"

	/call iniControl FALSE class switchAAFade										bool		Buffs FALSE
	/call iniControl FALSE class useAAFadePct										int			Buffs 50
	/call iniControl FALSE class switchAABeamofSlumber						bool		Spells FALSE
	
	| /if (!${Defined[aggroAnim]}) /declare aggroAnim string outer "|5|8|12|17|18|42|44|80|106|129|144"
	
	/call iniControl FALSE class switchCharmPet									bool		Pet FALSE

	/call iniControl FALSE class switchGather			 							bool		Spells TRUE
	/call iniControl FALSE class GatherPercent			 							int			Spells 60

	/call iniControl FALSE class runeVeil			 									string	Rune "Veil of Mindshadow"
	
	
	
	/call iniControl FALSE class runeEldritch	 									string	Rune "Eldritch Rune"
	
	
	/call iniControl FALSE class runeBrimstone 									string	Rune ""
| 109 Brimstone Durability
| 104 Brimstone Resolution
| 99 Brimstone Eminence

	
	
	/call iniControl FALSE class runeSpray			 									string	Rune "Glyph Spray"
	/call iniControl FALSE class runeReactive	 									string	Rune "Reactive Rune"
	/call iniControl FALSE class runePoly			 									string	Rune "Polyrefractive Rune"
	/call iniControl FALSE class runeUnity			 									string	Rune FALSE
	/call iniControl FALSE class runeWard			 									string	Rune "Ward of the Enticer"
	
	/call iniControl FALSE class runeAuspice		 									string	Rune ""
| 109 Tenebrous Auspice
| 104 Darkened Auspice
| 99 Umbral Auspice

	/call iniControl FALSE class runeAuspiceToon									string  Rune |FALSE



	
	/call iniControl FALSE class runeMind												string  Rune "Mind Over Matter"
	
	/call iniControl FALSE class runeEntAuspice									string	Rune "Enticer's Auspice"
	
	/call iniControl FALSE class runeLegion											string	Rune "Legion of Lucem"
	
	/call iniControl FALSE class runeShieldofFate								string	Rune "Shield of Consequence"


	| aa crystal items
	/call iniControl FALSE class mannaCrystal										string 	Spells "Azure Mind Crystal"
	/call iniControl FALSE class healCrystal											string 	Spells "Sanguine Mind Crystal"
	
	/call iniControl FALSE class spellCripple 										string 	Spells "Undermining Helix"
	/call iniControl FALSE class spellSlow		 										string	Spells "Undermining Helix"		
	/call iniControl FALSE class AASlow			 										string	Spells "Dreary Deeds"
	
	
	/call iniControl FALSE class spellTash 											string 	Spells "Decree of Tashan"

	/call iniControl FALSE class spellMez												string 	Spells "Chaotic Bewildering"
	
	/call iniControl FALSE class spellpbaeMez										string	Spells "Perilous Bafflement"
	
	/call iniControl FALSE class spellStun												string	Spells "Color Concourse"
	
	/call iniControl FALSE class spellCharm											string	Spells "Enticer's Command"
	
	
	
	
	
	
	
	/call iniControl FALSE class ${Me.Class.ShortName}Point 			int 		Heal 80
	/call iniControl FALSE class switchModRod										bool	 	Buffs TRUE
















	| Nukes
	/call iniControl FALSE class DoNuke					 								int			Spells 0
	/for _count 1 to 6
		/call iniControl FALSE class Nuke${_count}									string	Spells FALSE
		/call iniControl FALSE class Nuke${_count}Condition				string	Spells FALSE
	/next _count	
	
	| DoTs
	/call iniControl FALSE class DoDoT								 						int			Spells 0
	/for _count 1 to 6
		/call iniControl FALSE class DoT${_count}									string	Spells FALSE
		/call iniControl FALSE class DoT${_count}chk								string	Spells FALSE
		/call iniControl FALSE class DoT${_count}Condition					string	Spells FALSE
	/next _count
	





/return		                                        		


|***
 *	DES: Magician base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_MAG()
	/declare _count int local 0
/return		                                        		


|***
 *	DES: Monk base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_MNK()
	/declare _count int local 0
/return		                                        		


|***
 *	DES: Necro base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_NEC()
	/declare _count int local 0
/return		                                        		



|***
 *	DES: Paladin base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_PAL(int _lvl)
	/declare _count int local 0

	/call iniControl FALSE declare PALspire1														string	outer "Fundament: First Spire of Holiness"
	/call iniControl FALSE declare PALspire2														string	outer "Fundament: Second Spire of Holiness"
	/call iniControl FALSE declare PALspire3														string	outer "Fundament: Third Spire of Holiness"
	/call iniControl FALSE declare PALsynergyAA													string	outer "Knight's Synergy"
	/call iniControl FALSE declare PALEpic1															string	outer "Fiery Defender"
	/call iniControl FALSE declare PALEpic15														string	outer "Heartwood Blade"
	/call iniControl FALSE declare PALEpic2															string	outer "Nightbane, Sword of the Valiant"
	/call iniControl FALSE class PALAlliance														string	Spells "Holy Alliance"
	/call iniControl FALSE declare PALAAUnity														string	outer "Divine Protector's Unity"
	/call iniControl FALSE declare PALAAFade														string	outer "Balefire Burst"
	
	/call iniControl FALSE class switchAAFade														bool		Buffs FALSE
	/call iniControl FALSE class useAAFadePct														int			Buffs 50

	/call setTimer tcheck_MG																						string	Timer	10s timer_check_MG
	/call setTimer tcheck_Steel																					string 	Timer 2m timer_check_Steel
	/call setTimer tcheck_Stance																				string 	Timer 7.5m timer_check_Stance

	/call iniControl FALSE declare mobAECount														int			outer 0 
	
	/call iniControl FALSE declare hurtID		 														string 	outer FALSE
	/call iniControl FALSE declare hurtPctHP														int 		outer 100
	/call iniControl FALSE declare hurtCLS		 													string 	outer FALSE
	/call iniControl FALSE declare hurtCount 														int 		outer 0
	
  /call iniControl FALSE class splashGroup											string 	Heal "Wave of Expiation"
  /call iniControl FALSE class splashPanic											string 	Heal "Aurora of Morninglight"
  /call iniControl FALSE class splashCure											string 	Heal "Splash of Depuration"
  /call iniControl FALSE class healPanic1											string 	Heal "Expiation"
  /call iniControl FALSE class healPanic2											string 	Heal "Grief"
  /call iniControl FALSE class healTouch												string 	Heal "Merciful Touch"
	/call iniControl FALSE class useGroupHeal 										bool		Heal TRUE
	/call iniControl FALSE class XTHealRadius										int			Heal 300
	/call iniControl FALSE class RampageTank											string	Heal FALSE
	/call iniControl FALSE class RandRezDelay										int 		Heal 5
	/call iniControl FALSE class defCount																int			Discs 2


	| yaulp attack buff
	/call iniControl FALSE class doYaulp 																bool		Buffs FALSE
	/call iniControl FALSE class useYaulpPct														int			Buffs 98
	/call iniControl FALSE class spellYaulp			 												string	Buffs

	| rest disc
	/call iniControl FALSE class DiscRest		 														string	Discs
	
	| fast rune
	/call iniControl FALSE class DiscFastRune			 											string	Discs

	| armor disc
	/call iniControl FALSE class DefensiveDisc1			 										string	Discs

	| mantle disc
	/call iniControl FALSE class DefensiveDisc2			 										string	Discs

	| guardian disc
	/call iniControl FALSE class DefensiveDisc3			 										string	Discs


	| agro disc                                          	
	/call iniControl FALSE class DiscUnflinching									string	Discs

	/call iniControl FALSE class DiscReflexive										string	Discs "Reflexive Reverence"
	/call iniControl FALSE class AAHatePet												string	Discs "Projection of Piety"
	/call iniControl FALSE class DiscTwincast										string	Discs "Hand of Tunare"
	/call iniControl FALSE class AALayonHands										string	Discs "Lay on Hands"
	/call iniControl FALSE class AAHandofPiety										string	Discs "Hand of Piety"
	/call iniControl FALSE class AAForceofDisruption							string	Discs "Force of Disruption"
	/call iniControl FALSE class AADisruptivePersecution					string	Discs "Disruptive Persecution"
	/call iniControl FALSE class AADivineStun										string	Discs "Divine Stun"
                                           	
	/call iniControl FALSE class AAAEBeacon											string	Discs "Beacon of the Righteous"
	/call iniControl FALSE class AAAELodestar										string	Discs "Hallowed Lodestar"
                                           	
	/call iniControl FALSE class AAInquisitor										string	Discs "Armor of the Inquisitor"
	/call iniControl FALSE class AAGroupInquisitor								string	Discs "Group Armor of the Inquisitor"
                                           	
	/call iniControl FALSE class AAJudgement											string	Discs "Inquisitor's Judgement"
	/call iniControl FALSE class AAValorousRage									string	Discs "Valorous Rage"
	/call iniControl FALSE class AAShield												string	Discs "Shield of Brilliance"
	/call iniControl FALSE class chainStun												string	Discs FALSE
	/call iniControl FALSE class AEMelee					 								bool	 	Discs FALSE
	                                         	
	| Nukes
	/call iniControl FALSE class DoNuke					 												int			Spells 0
	/for _count 1 to 6
		/call iniControl FALSE class Nuke${_count}												string	Spells FALSE
		/call iniControl FALSE class Nuke${_count}Condition								string	Spells FALSE
	/next _count                                           	

	| DoTs
	/call iniControl FALSE class DoDoT								 									int			Spells 0
	/for _count 1 to 6
		/call iniControl FALSE class DoT${_count}													string	Spells FALSE
		/call iniControl FALSE class DoT${_count}chk											string	Spells FALSE
		/call iniControl FALSE class DoT${_count}Condition								string	Spells FALSE
	/next _count
	
	/call iniControl FALSE class spellCripple 													string 	Spells FALSE
	
	| single target challenge spell
	/call iniControl FALSE class ChallengeSpell			 										string	Spells

	| def proc spell
	/call iniControl FALSE class ValiantDefense			 										string	Spells

	| steel weapon heal proc
	/call iniControl FALSE class Steel			 														string 	Buffs
	/call iniControl FALSE class switchSteel			 											bool	 	Buffs FALSE
	

	| stance temp HP buff
	/call iniControl FALSE class Stance			 														string	Buffs
	/call iniControl FALSE class switchStance 													bool	 	Buffs FALSE

	/call iniControl FALSE class Twincast																string 	Spells "Glorious Exoneration"
	/call iniControl FALSE class switchModRod														bool	 	Buffs FALSE
	/call iniControl FALSE class Harmonious 														string 	Buffs "Harmonious Blessing"
	/call iniControl FALSE class switchHarmonious 											bool	 	Buffs FALSE

	/call iniControl FALSE class useMG																	bool		Buffs TRUE
	/call iniControl FALSE class useMGPct																int			Buffs 60
	/call iniControl FALSE class useShieldFlash													bool		Discs FALSE
	/call iniControl FALSE class useShieldFlashPct											int			Discs 20
	/call iniControl FALSE class useLoH																	bool		Heal FALSE
	/call iniControl FALSE class useLoHPct															int			Heal 10
	/call iniControl FALSE class switchAttraction												bool		Discs FALSE

/return		                                        		




|***
 *	DES: Ranger base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_RNG()
	/declare _count int local 0
	
	/call iniControl FALSE declare RNGspire1											string	outer "Fundament: First Spire of the Pathfinders"
	/call iniControl FALSE declare RNGspire2											string	outer "Fundament: Second Spire of the Pathfinders"
	/call iniControl FALSE declare RNGspire3											string	outer "Fundament: Third Spire of the Pathfinders"
	/call iniControl FALSE declare RNGauspice											string	outer "Auspice of the Hunter"
	/call iniControl FALSE declare RNGcheetah											string	outer "Scarlet Cheetah's Fang"
	/call iniControl FALSE declare RNGgroupguardian								string	outer "Group Guardian of the Forest"
	/call iniControl FALSE declare RNGsynergyAA										string	outer "Outrider's Synergy"
	/call iniControl FALSE declare RNGEpic1												string	outer "Earthcaller"
	/call iniControl FALSE declare RNGEpic15											string	outer "Heartwood Blade"
	/call iniControl FALSE declare RNGEpic2												string	outer "Aurora, the Heartwood Blade"
	/call iniControl FALSE class RNGAlliance											string	Spells "Bosquestalker's Alliance"
	/call iniControl FALSE declare RNGAAUnity											string	outer "Wildstalker's Unity (Azia)"
	/call iniControl FALSE declare RNGAAFade											string	outer "Cover Tracks"
	
	/call iniControl FALSE class switchAAFade										bool		Buffs FALSE
	/call iniControl FALSE class useAAFadePct										int			Buffs 50

	/call iniControl FALSE class AuspiceTXT												string	Spells "AUSPICE Lit"
	/call iniControl FALSE class spellCripple 										string 	Spells FALSE

	/call iniControl FALSE class AADefensive1 										string	Spells "Outrider's Evasion"
	/call iniControl FALSE class AADefensive2 										string	Spells "Protection of the Spirit Wolf"
	/call iniControl FALSE class AADefensive3 										string	Spells "Bulwark of the Brownies"
	/call iniControl FALSE class switchModRod										bool	 	Buffs TRUE
	/call iniControl FALSE class ${Me.Class.ShortName}Point		 	int 		Heal 80

	| Nukes
	/call iniControl FALSE class DoNuke					 								int			Spells 0
	/for _count 1 to 6
		/call iniControl FALSE class Nuke${_count}									string	Spells FALSE
		/call iniControl FALSE class Nuke${_count}Condition				string	Spells FALSE
	/next _count
                                          		

	| Summer's line
	/call setTimer tNuke_Summer																					string	Timer 3s timer_cast_nuke_summer
	/call iniControl FALSE class NukeSummer		 													string	Spells
	/call iniControl FALSE class NukeSummerCondition										string	Spells FALSE
	/call iniControl FALSE class switchNukeSummer		 										bool		Spells TRUE

	| DoTs
	/call iniControl FALSE class DoDoT								 						int			Spells 0
	/for _count 1 to 6
		/call iniControl FALSE class DoT${_count}									string	Spells FALSE
		/call iniControl FALSE class DoT${_count}chk								string	Spells FALSE
		/call iniControl FALSE class DoT${_count}Condition					string	Spells FALSE
	/next _count

	| ToT DS DoT / debuff
	/call iniControl FALSE class DoTSwarmDS		 										string	Spells
	/call iniControl FALSE class DoTSwarmDSdeBuff 								string	Spells
	
	| slow heal
  /call iniControl FALSE class heal1	 													string  Heal

	| fast heal
  /call iniControl FALSE class heal2				 										string  Heal

	| bow disc
	/call iniControl FALSE class DiscRange			 									string	Discs

	| rest disc
	/call iniControl FALSE class DiscRest		 											string	Discs
	
	| melee disc
	/call iniControl FALSE class DiscMele			 										string	Discs

	| agro/anti agro kicks
	/call iniControl FALSE class DiscJolt			 										string	Discs
	/call iniControl FALSE class DiscEnrage		 										string	Discs

	| AE group heal disc
	/call iniControl FALSE class DiscHeal			 										string	Discs

	| ae melee disc
	/call iniControl FALSE class BladeStorm			 									string	Discs

	| focused melee disc
	/call iniControl FALSE class DiscSqual			 									string	Discs

/return
                                     		


|***
 *	DES: Rogue base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_ROG()
	/declare _count int local 0
/return



|***
 *	DES: Shadowknight base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_SHD()
	/declare _count int local 0
	
	/call setTimer tcheck_vot												string Timer 1215s timer_check_vot

	/call iniControl FALSE declare SHDspire1											string	outer "Fundament: First Spire of the Reavers"
	/call iniControl FALSE declare SHDspire2											string	outer "Fundament: Second Spire of the Reavers"
	/call iniControl FALSE declare SHDspire3											string	outer "Fundament: Third Spire of the Reavers"
	/call iniControl FALSE declare SHDsynergyAA										string	outer "Knight's Synergy"
	/call iniControl FALSE declare SHDEpic1												string	outer "Innoruuk's Curse"
	/call iniControl FALSE declare SHDEpic15											string	outer "Innoruuk's Voice"
	/call iniControl FALSE declare SHDEpic2												string	outer "Innoruuk's Dark Blessing"
	/call iniControl FALSE class SHDAlliance											string	Spells "Bloodletting Alliance"
	/call iniControl FALSE declare SHDAAUnity											string	outer "Dark Lord's Unity (Beza)"
	/call iniControl FALSE declare SHDAAFade											string	outer "Death Peace"
	
	/call iniControl FALSE class switchAAFade										bool		Buffs FALSE
	/call iniControl FALSE class useAAFadePct										int			Buffs 50

	/call setTimer tcheck_Skin											string 	Timer 45s timer_check_Skin
	/call setTimer tcheck_Stance										string 	Timer 3m timer_check_Stance

	/call iniControl FALSE declare mobAECount											int			outer 0

	| fast rune
	/if (${Me.Level} >= 108) {
		/call iniControl FALSE class Withstand			 								string	Discs "Spurn"
	} else /if (${Me.Level} >= 103) {
		/call iniControl FALSE class Withstand			 								string	Discs "Repel"
	} else /if (${Me.Level} >= 98) {
		/call iniControl FALSE class Withstand			 								string	Discs "Reprove"
	} else /if (${Me.Level} >= 93) {
		/call iniControl FALSE class Withstand			 								string	Discs "Renounce"
	} else /if (${Me.Level} >= 83) {
		/call iniControl FALSE class Withstand			 								string	Discs "Withstand"
	} else {
		/call iniControl FALSE class Withstand			 								string	Discs FALSE
	}	

	/call iniControl FALSE class BladeStrike 										string 	Discs "Lacerating Blade"

	| blade DPS
	/if (${Me.Level} >= 110) {
		/call iniControl FALSE class CrimsonBlade			 						string	Discs "Claret Blad"
	} else /if (${Me.Level} >= 105) {
		/call iniControl FALSE class CrimsonBlade			 						string	Discs "Carmine Blade"
	} else /if (${Me.Level} >= 95) {
		/call iniControl FALSE class CrimsonBlade			 						string	Discs "Crimson Blade"
	} else {
		/call iniControl FALSE class CrimsonBlade			 						string	Discs FALSE
	}	
	
	| rest disc
	/call iniControl FALSE class DiscRest		 										string	Discs

	| acromony
	/if (${Me.Level} >= 109) {
		/call iniControl FALSE class DiscFakeHate			 						string	Discs "Unyielding Acrimony"
	} else /if (${Me.Level} >= 104) {
		/call iniControl FALSE class DiscFakeHate			 						string	Discs "Unflinching Acrimony"
	} else /if (${Me.Level} >= 99) {
		/call iniControl FALSE class DiscFakeHate			 						string	Discs "Unbroken Acrimony"
	} else /if (${Me.Level} >= 94) {
		/call iniControl FALSE class DiscFakeHate			 						string	Discs "Undivided Acrimony"
	} else {
		/call iniControl FALSE class DiscFakeHate			 						string	Discs FALSE
	}	
	
	/call iniControl FALSE class DiscLeech												string	Discs	"Leechcurse Discipline"
	
	| rancor	
	/if (${Me.Level} >= 104) {
		/call iniControl FALSE class DiscRancor				 						string	Discs "Reflexive Revulsion"
	} else /if (${Me.Level} >= 100) {
		/call iniControl FALSE class DiscRancor				 						string	Discs "Reflexive Rancor"
	} else {
		/call iniControl FALSE class DiscRancor				 						string	Discs FALSE
	}	

	| defensive mantle
	/if (${Me.Level} >= 108) {
		/call iniControl FALSE class DefensiveDisc1			 					string	Discs "Krellnakor Mantle"
	} else /if (${Me.Level} >= 103) {
		/call iniControl FALSE class DefensiveDisc1			 					string	Discs "Doomscale Mantle"
	} else /if (${Me.Level} >= 98) {
		/call iniControl FALSE class DefensiveDisc1			 					string	Discs "Bonebrood Mantle"
	} else /if (${Me.Level} >= 93) {
		/call iniControl FALSE class DefensiveDisc1			 					string	Discs "Recondite Mantle"
	} else /if (${Me.Level} >= 83) {
		/call iniControl FALSE class DefensiveDisc1			 					string	Discs "Malarian Mantle"
	} else {
		/call iniControl FALSE class DefensiveDisc1			 					string	Discs FALSE
	}	

	| defensive carpace
	/if (${Me.Level} >= 108)  {
		/call iniControl FALSE class DefensiveDisc2			 					string	Discs "Tylix's Carapace"
	} else /if (${Me.Level} >= 103)  {
		/call iniControl FALSE class DefensiveDisc2								string	Discs "Vizat's Carpace"
	} else /if (${Me.Level} >= 98)  {
		/call iniControl FALSE class DefensiveDisc2								string	Discs "Grelleth's Carpace"
	} else /if (${Me.Level} >= 93)  {
		/call iniControl FALSE class DefensiveDisc2		 						string	Discs "Sholothian Carpace"
	} else {
		/call iniControl FALSE class DefensiveDisc2		 						string	Discs FALSE
	}	

	| defensive guardian
	/if (${Me.Level} >= 107) {
		/call iniControl FALSE class DefensiveDisc3			 					string	Discs "Cursed Guardian Discipline"
	} else /if (${Me.Level} >= 97) {
		/call iniControl FALSE class DefensiveDisc3			 					string	Discs "Unholy Guardian Discipline"
	} else {
		/call iniControl FALSE class DefensiveDisc3			 					string	Discs FALSE
	}		

	/call iniControl FALSE class defCount												int			Discs 2
	/call iniControl FALSE class AEMelee					 								bool	 	Discs FALSE

	| challenge	
	/if (${Me.Level} >= 107) {
		/call iniControl FALSE class ChallengeSpell			 					string	Spells "Refute for Power"
	} else /if (${Me.Level} >= 102) {
		/call iniControl FALSE class ChallengeSpell								string	Spells "Impose for Power"
	} else /if (${Me.Level} >= 97) {
		/call iniControl FALSE class ChallengeSpell								string	Spells "Demand for Power"
	} else /if (${Me.Level} >= 92) {
		/call iniControl FALSE class ChallengeSpell		 						string	Spells "Provocation for Power"
	} else /if (${Me.Level} >= 82) {
		/call iniControl FALSE class ChallengeSpell		 						string	Spells "Charge for Power"
	} else /if (${Me.Level} >= 77) {
		/call iniControl FALSE class ChallengeSpell		 						string	Spells "Trial for Power"
	} else /if (${Me.Level} >= 77) {
		/call iniControl FALSE class ChallengeSpell		 						string	Spells "Challenge for Power"
	} else {
		/call iniControl FALSE class ChallengeSpell		 						string	Spells FALSE
	}	
	
	/call iniControl FALSE class spellCripple										string 	Spells FALSE
	/call iniControl FALSE class switchModRod										bool	 	Buffs TRUE
	/call iniControl FALSE class switchHarmonious 								bool	 	Buffs TRUE
	/call iniControl FALSE class Harmonious 											string 	Buffs "Harmonious Disruption"

	| single hate
	/if (${Me.Level} >= 106)  {
		/call iniControl FALSE class Hate1 												string 	Spells "Terror of Kra`Du"
		/call iniControl FALSE class Hate2 												string 	Spells "Terror of Narus"
	} else /if (${Me.Level} >= 101) {
		/call iniControl FALSE class Hate1 												string 	Spells "Terror of Narus"
		/call iniControl FALSE class Hate2 												string 	Spells "Terror of Poira"
	} else /if (${Me.Level} >= 96) {
		/call iniControl FALSE class Hate1 												string 	Spells "Terror of Poira"
		/call iniControl FALSE class Hate2 												string 	Spells "Terror of Desalin"
	} else /if (${Me.Level} >= 91) {
		/call iniControl FALSE class Hate1 												string 	Spells "Terror of Desalin"
		/call iniControl FALSE class Hate2 												string 	Spells "Terror of Rerekalen"
	} else /if (${Me.Level} >= 86) {
		/call iniControl FALSE class Hate1 												string 	Spells "Terror of Rerekalen"
		/call iniControl FALSE class Hate2 												string 	Spells "Terror of Jelvalak"
	} else /if (${Me.Level} >= 81) {
		/call iniControl FALSE class Hate1 												string 	Spells "Terror of Jelvalak"
		/call iniControl FALSE class Hate2 												string 	Spells "Terror of the Soulbleeder"
	} else /if (${Me.Level} >= 76) {
		/call iniControl FALSE class Hate1 												string 	Spells "Terror of the Soulbleeder"
		/call iniControl FALSE class Hate2 												string 	Spells "Terror of Vergalid"
	} else /if (${Me.Level} >= 71) {
		/call iniControl FALSE class Hate1 												string 	Spells "Terror of Vergalid"
		/call iniControl FALSE class Hate2 												string 	Spells "Terror of Discord"
	} else /if (${Me.Level} >= 67) {
		/call iniControl FALSE class Hate1 												string 	Spells "Terror of Discord"
		/call iniControl FALSE class Hate2 												string 	Spells "Terror of Thule"
	} else /if (${Me.Level} >= 63) {
		/call iniControl FALSE class Hate1 												string 	Spells "Terror of Thule"
		/call iniControl FALSE class Hate2 												string 	Spells "Terrir of Terris"
	} else /if (${Me.Level} >= 59) {
		/call iniControl FALSE class Hate1 												string 	Spells "Terrir of Terris"
		/call iniControl FALSE class Hate2 												string 	Spells "Terror of Death"
	} else /if (${Me.Level} >= 53) {
		/call iniControl FALSE class Hate1 												string 	Spells "Terror of Death"
		/call iniControl FALSE class Hate2 												string 	Spells "Terror of Shadows"
	} else /if (${Me.Level} >= 42) {
		/call iniControl FALSE class Hate1 												string 	Spells "Terror of Shadows"
		/call iniControl FALSE class Hate2 												string 	Spells "Terror of Darkness"
	} else /if (${Me.Level} >= 33) {
		/call iniControl FALSE class Hate1 												string 	Spells "Terror of Darkness"
		/call iniControl FALSE class Hate2 												string 	Spells FALSE
	} else {
		/call iniControl FALSE class Hate1 												string 	Spells FALSE
		/call iniControl FALSE class Hate2 												string 	Spells FALSE
	}

	| AE Hate	
	/if (${Me.Level} >= 109) {
		/call iniControl FALSE class AEHate1 											string 	Spells "Revulsion"
		/call iniControl FALSE class AEHate2 											string 	Spells "Disgust"
	} else /if (${Me.Level} >= 104) {
		/call iniControl FALSE class AEHate1 											string 	Spells "Disgust"
		/call iniControl FALSE class AEHate2 											string 	Spells "Repugnance"
	} else /if (${Me.Level} >= 99) {
		/call iniControl FALSE class AEHate1 											string 	Spells "Repugnance"
		/call iniControl FALSE class AEHate2 											string 	Spells "Abhorrence"
	} else /if (${Me.Level} >= 94) {
		/call iniControl FALSE class AEHate1 											string 	Spells "Abhorrence"
		/call iniControl FALSE class AEHate2 											string 	Spells "Loathing"
	} else /if (${Me.Level} >= 89) {
		/call iniControl FALSE class AEHate1 											string 	Spells "Loathing"
		/call iniControl FALSE class AEHate2 											string 	Spells "Burst of Spite"
	} else /if (${Me.Level} >= 84) {
		/call iniControl FALSE class AEHate1 											string 	Spells "Burst of Spite"
		/call iniControl FALSE class AEHate2 											string 	Spells "Revile"
	} else /if (${Me.Level} >= 79) {
		/call iniControl FALSE class AEHate1 											string 	Spells "Revile"
		/call iniControl FALSE class AEHate2 											string 	Spells "Vilify"
	} else /if (${Me.Level} >= 74) {
		/call iniControl FALSE class AEHate1 											string 	Spells "Vilify"
		/call iniControl FALSE class AEHate2 											string 	Spells "Dread Gaze"
	} else /if (${Me.Level} >= 69) {
		/call iniControl FALSE class AEHate1 											string 	Spells "Dread Gaze"
		/call iniControl FALSE class AEHate2 											string 	Spells FALSE
	} else {
		/call iniControl FALSE class AEHate1 											string 	Spells FALSE
		/call iniControl FALSE class AEHate2 											string 	Spells FALSE
	}	

	/call iniControl FALSE class AELifeTap 											string 	Spells "Deceitful Deflection"
	/call iniControl FALSE class AAHatePet												string	Discs "Projection of Doom"

	| stance	
	/call iniControl FALSE class switchStance 										bool	 	Buffs TRUE
	/if (${Me.Level} >= 109) {
		/call iniControl FALSE class Stance			 									string	Buffs "Defiant Stance"
	} else /if (${Me.Level} >= 104) {
		/call iniControl FALSE class Stance												string	Buffs "Staunch Stance"
	} else /if (${Me.Level} >= 99) {
		/call iniControl FALSE class Stance												string	Buffs "Steadfast Stance"
	} else /if (${Me.Level} >= 94) {
		/call iniControl FALSE class Stance		 										string	Buffs "Stoic Stance"
	} else /if (${Me.Level} >= 89) {
		/call iniControl FALSE class Stance		 										string	Buffs "Stubborn Stance"
	} else /if (${Me.Level} >= 84) {
		/call iniControl FALSE class Stance		 										string	Buffs "Steely Stance"
	} else {
		/call iniControl FALSE class Stance		 										string	Buffs FALSE
	}	

	| skin rune buff	
	/call iniControl FALSE class switchSkin			 								bool	 	Buffs TRUE
	/if (${Me.Level} >= 110) {
		/call iniControl FALSE class Skin			 										string	Buffs "Tylix's Skin"
	} else /if (${Me.Level} >= 105) {
		/call iniControl FALSE class Skin													string	Buffs "Vizat's Skin"
	} else /if (${Me.Level} >= 100) {
		/call iniControl FALSE class Skin													string	Buffs "Grelleth's Skin"
	} else /if (${Me.Level} >= 95) {
		/call iniControl FALSE class Skin		 											string	Buffs "Sholothian Skin"
	} else /if (${Me.Level} >= 90) {
		/call iniControl FALSE class Skin		 											string	Buffs "Gorgon Skin"
	} else /if (${Me.Level} >= 85) {
		/call iniControl FALSE class Skin		 											string	Buffs "Malarian Skin"
	} else /if (${Me.Level} >= 80) {
		/call iniControl FALSE class Skin		 											string	Buffs "Umbrial Skin"
	} else /if (${Me.Level} >= 70) {
		/call iniControl FALSE class Skin		 											string	Buffs "Decrepit Skin"
	} else {
		/call iniControl FALSE class Skin		 											string	Buffs FALSE
	}	

	/call iniControl FALSE class switchVoT				 								bool	 	Buffs TRUE
	/call iniControl FALSE class ${Me.Class.ShortName}Point 			int 		Heal 80
	/call iniControl FALSE class useShieldFlash									bool		Discs FALSE
	/call iniControl FALSE class useShieldFlashPct								int			Discs 20
	/call iniControl FALSE class switchAttraction								bool		Discs TRUE

	| Nukes
	/call iniControl FALSE class DoNuke					 								int			Spells 0
	/for _count 1 to 6
		/call iniControl FALSE class Nuke${_count}									string	Spells FALSE
		/call iniControl FALSE class Nuke${_count}Condition				string	Spells FALSE
	/next _count	
	
	| DoTs
	/call iniControl FALSE class DoDoT								 						int			Spells 0
	/for _count 1 to 6
		/call iniControl FALSE class DoT${_count}									string	Spells FALSE
		/call iniControl FALSE class DoT${_count}chk								string	Spells FALSE
		/call iniControl FALSE class DoT${_count}Condition					string	Spells FALSE
	/next _count
	
/return



|***
 *	DES: Shaman base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_SHM()
	/declare _count int local 0
/return



|***
 *	DES: Warrior base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_WAR()
	/declare _count int local 0
/return



|***
 *	DES: Wizard base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_WIZ()
	/declare _count int local 0
/return



