|***
 *	declare.inc
 *	Core Declares common and class specific.
 *	All manual forced events.
 *	All created alias', binds and /dg commands
 *
 ***|




|***
 *	DES: class INI files for each toon
 *	USE: /call set_declares
 *	INI: 
 *  NOTE: 
 ***|
sub set_declares()
	/call setDeclare coreBuild											string	outer 20181019.00
	/call setINIVar classBuild											string	Common 00.00
	/call setDeclare forceINIcheck									bool		outer TRUE
	/call setDeclare castReturn 										string 	outer NULL
	/declare _count 																int 		local 0
	/call setDeclare combat													string	outer FALSE
		
	/call setDeclare isValidTarget									bool		outer FALSE

	| HUD controls
	/call setDeclare hudWarningLength								int			outer	0
	/call setDeclare hudNoticeLength								int			outer	0
	/call setDeclare hudTargetLength								int			outer	0
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
	
	
	| communications
	/call setDeclare commType												string	outer "|eqbc|dannet"
	/call DeclareCoreIniVar setCommunications				string	Comm "eqbc"	
	/call DeclareCoreIniVar EQBCServer							string	Comm 127.0.0.1
	/call DeclareCoreIniVar EQBCPort								string	Comm 2112
	
	| DanNet settings
	/call DeclareCoreIniVar switchDanLocalEcho			bool		Comm FALSE
	/call DeclareCoreIniVar switchDanCommandEcho		bool		Comm FALSE
	/call DeclareCoreIniVar switchDanFullNames			bool		Comm FALSE
	

	| Base Environment settings
	/call setDeclare noInvis 												bool 		outer TRUE
	/call setDeclare noFeigning 										bool 		outer TRUE
	/call setDeclare Target2Kill										int			outer 0
	/call setDeclare currentLootCount								int			outer	0

	/call DeclareCoreIniVar plist										string	Plugin "|MQ2Cast|MQ2Exchange|MQ2MoveUtils|MQ2Debuffs|MQ2DanNet"
	/call DeclareCoreIniVar minMobPullLvl						int			General 1
	/call DeclareCoreIniVar maxMobPullLvl						int			General 150
	/call DeclareCoreIniVar autoinihousekeeping			string	General FALSE
	/call DeclareCoreIniVar smartasserrors					string	General TRUE
	/call DeclareCoreIniVar forceNukeLoop						string	General FALSE
	/call DeclareCoreIniVar autoAddNameList					string	General TRUE

	/call DeclareCoreIniVar switchDispInfo					string	General TRUE
	/call DeclareCoreIniVar switchDispBuild					string	General TRUE
	/call DeclareCoreIniVar switchDispEXP						string	General TRUE
	/call DeclareCoreIniVar switchAADiscnowManual		string	General FALSE
	/call DeclareCoreIniVar switchDelayWhileZoning	string	General FALSE
	/call DeclareCoreIniVar switchAutoRestFull			string	General FALSE
	/call DeclareCoreIniVar switchModRodinRest			string	General TRUE
	/call DeclareCoreIniVar switchBuffinRest				string	General TRUE
	/call DeclareCoreIniVar switchUseCLSinHUD				string	General FALSE
	/call DeclareCoreIniVar switchUseADVL						string	General TRUE
	/call DeclareCoreIniVar switchGlyphOutsideRaid	string	General FALSE
	/call DeclareCoreIniVar maintenance							string	General "|FALSE|FALSE"
	/call DeclareCoreIniVar switchMQclearonCLS			string	General FALSE
	/call DeclareCoreIniVar switchGuildinSafelist		string	General FALSE
	/call DeclareCoreIniVar switchCheckGameCash			string	General FALSE
	/call DeclareCoreIniVar switchPersistentModes		string	General FALSE 			
	/call DeclareCoreIniVar switchNoTieonCombat			string	General FALSE 			

	| broadcast switches
	| default on
	/call DeclareCoreIniVar broadcastZonereset			bool	Broadcast TRUE
	/call DeclareCoreIniVar broadcastStop						bool	Broadcast TRUE
	/call DeclareCoreIniVar broadcastHere						bool	Broadcast TRUE

	| default off
	/call DeclareCoreIniVar broadcastTake						bool	Broadcast FALSE
	/call DeclareCoreIniVar broadcastDoor						bool	Broadcast FALSE
	/call DeclareCoreIniVar broadcastInvis					bool	Broadcast FALSE
	/call DeclareCoreIniVar broadcastKillmob				bool	Broadcast FALSE
	/call DeclareCoreIniVar broadcastClean					bool	Broadcast FALSE
	/call DeclareCoreIniVar broadcastAnchor					bool	Broadcast FALSE

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
 	/call setDeclare ocombat												string	outer "\amCombat\ax"	
	/call setDeclare oremove												string	outer "${cbad}Removing\ax"
	/call setDeclare obroadcasted										string 	outer "\amBroadcasted\ax"

	| ? colors
	/call setDeclare wikiswitch											string outer "[\a-wswitch\ax]"
	/call setDeclare wikitarget											string outer "[\a-wTARGET\ax]"
	/call setDeclare wikiname												string outer "[\a-wName\ax]"
	/call setDeclare wikispire											string outer "[\ar0\ax\a-w|1|2|3\ax]"
	/call setDeclare wikipct												string outer "[\a-w1,99\ax]"


	| base lists for mob mez control
	/call setDeclare mobList												string	outer
	/call setDeclare addList 												string 	outer	

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
	/call setINIVar ClassChannel										int			Echo FALSE

	 	
	| lists for .. you know..lists and stuff.
	/call setDeclare AllClassList										string 	outer "|WAR|SHD|PAL|BER|ROG|MNK|RNG|BST|BRD|NEC|ENC|WIZ|MAG|CLR|SHM|DRU"
	/call setDeclare MeleeList    		  						string 	outer "|WAR|ROG|BST|MNK|SHD|PAL|BER|BRD|RNG"
	/call setDeclare CastList												string 	outer "|BST|SHD|PAL|RNG|CLR|DRU|SHM|ENC|WIZ|NEC|MAG"
	/call setDeclare PetList												string 	outer "|ENC|DRU|SHM|SHD|WIZ|BRD|BST|MAG|NEC"
	/call setDeclare HealerList											string 	outer "|CLR|DRU|SHM|PAL"
	/call setDeclare RezerList											string 	outer "|CLR|DRU|SHM|PAL|NEC"
	/call setDeclare AASnareList										string 	outer "|RNG|DRU|SHD|NEC|ROG|WAR|WIZ|MNK|PAL"
	/call setDeclare AAEradicateList								string 	outer "|RNG|DRU|NEC|WIZ|ENC|MAG"
	
	| this is a makeshift /doevents to force binds to be cycled.
	/call setDeclare bindTrigger										bool		outer TRUE
	
	| holder for the current sub for debugging purposes
	/call setDeclare currentdebugsub								string	outer FALSE
	
		
	| core control
	/call setDeclare coreType 											string 	outer "|auto|checkcash|control|events|expadjust|expmaintain|explevel|fireworks|fixini|takeraidinvite|takegroupinvite|giveraidinvite|killcode|reset|rest|whitewash|zonereset"
	/call setDeclare coreObscure										bool		outer FALSE
	/call setINIVar coreAuto				 								bool	 	Common FALSE
	/call DeclareCoreIniVar remoteKillKey						string	General "CHANGE ME" 
	/call DeclareCoreIniVar ${MacroQuest.Server}.${Me.Class.ShortName}.${Me.DisplayName}				int			DeathCount 0
	/call DeclareCoreIniVar invitelist							string	No FALSE
	/call DeclareCoreIniVar rezlist									string	No FALSE
	/call DeclareCoreIniVar heallist								string	No FALSE
	/call DeclareCoreIniVar safelist								string	General "|FALSE|FALSE"

	/call setINIVar toonControl			 								string	Common FALSE
	/call setINIVar autoRest 												int 		Common 90
	/call setINIVar AutoExpAdjust 									bool 		Common TRUE
	/call setINIVar MaintExpLvl   									int 		Common 99
	/call setINIVar MaxLevel      									int 		Common 110
	/call setINIVar coreEventsInc  									bool 		Common TRUE
	/call setINIVar switchAAFireworks								bool	 	Common FALSE

	/call setINIVar switchTakeGroupInvite						bool 		Common FALSE
	/call setINIVar switchTakeRaidInvite						bool 		Common FALSE
	/call setINIVar switchGiveRaidInvite						bool		Common FALSE


	| OCD Shit - 
	/call DeclareCoreIniVar setMQFontSize						int			General 2
	/call DeclareCoreIniVar setMQCaptions						string	General off
	/call DeclareCoreIniVar setAutoAssist						string	General off

	
	| buff queue variables
	/call DeclareCoreIniVar queueCountMax						int			General 10
	/call setDeclare queueCount            					int  		outer 0
	
	/if (!${Defined[queueBuffID]}) /declare queueBuffID[8] 												int  		outer
	/if (!${Defined[queueList]}) /declare queueList[${queueCountMax},2] 				 	int  		outer
	
		
	| set /crew stuffs
	/call DeclareCoreIniVar main													string	Group "|FALSE|FALSE"
	/call DeclareCoreIniVar main2													string	Group "|FALSE|FALSE"
	/call DeclareCoreIniVar ${MacroQuest.Server}Leader		string	Group FALSE
	/call DeclareCoreIniVar ${MacroQuest.Server}Assist		string	Group FALSE
	/call DeclareCoreIniVar ${MacroQuest.Server}Looter		string	Group FALSE
	
	/call DeclareCoreIniVar ResidentMage									string	Group "|FALSE|FALSE"
	/call DeclareCoreIniVar ResidentDruid						 			string	Group "|FALSE|FALSE"
	/call DeclareCoreIniVar ResidentWizard								string	Group "|FALSE|FALSE"
	
	

	| /here settings
	/call DeclareCoreIniVar makeVisible							bool		Here TRUE
	/call DeclareCoreIniVar clearTarget							bool		Here TRUE
	/call DeclareCoreIniVar defaultTimerDelay				string	Here 1m
	/call DeclareCoreIniVar baseAgroRange						int			Here 60
	/call DeclareCoreIniVar emptyHands							bool		Here TRUE
	/call DeclareCoreIniVar hereZRadius							int			Here 30
	
	
	| please take a number and have a seat.
	/call setDeclare bitchcount											int			outer	31
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
	/call setDeclare bitch30												string 	outer "As the sun rose, Dawn crept across the lawn.. Searching for her car keys."
	/call setDeclare bitch31												string 	outer "Thank you for nothing... You useless reptile."


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
	/call setDeclare expansion25										string 	outer ""


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
	/call setDeclare modeType												string	outer "|stop|agro|drag|drink|fish|forage|harvest|harvestcount|hunt|lush|open|petfarm|scorch|skill|tradeskill|trainspell"
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
	/call setDeclare switchModeTrainspellSPC				bool		outer FALSE
	/call setDeclare switchModeOpen									bool		outer FALSE

	/call DeclareCoreIniVar modeDrinkofChoice				string	mode FALSE
	/call DeclareCoreIniVar modeOpenItem						string	mode FALSE


	| burns
	/call setDeclare burnType												string	outer "|auto|count|engageat|force"
	/call setDeclare setBurnControl									bool		outer FALSE
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
	/call setINIVar noMeleeWith											string	Common "|Itching Poison"
	| /call setDeclare canMelee												bool		outer TRUE
	
	/call setINIVar shrinkItem 											string 	Common "Ring of the Ancients"
	/call setINIVar IllusionItem										string	Common FALSE
	/call setINIVar IllusionBuff										string	Common FALSE
	/call setINIVar GTFO														string	Common FALSE
	/call setINIVar switchAApicker									bool 		Common FALSE
	/call setINIVar RangeItem												string	Common "|FALSE|FALSE"
	/call setINIVar RangeItemSummon									string	Common "|FALSE|FALSE"
	/call setINIVar RangeItemCount									int			Common 5
	/call setINIVar RangeSpell											string	Common FALSE
	/call setINIVar RangeClickItem									string	Common FALSE
	/call setINIVar RangeDisc												string	Common FALSE
			
	/call setINIVar autoInventoryItem								string	Common "|Summoned: Giant Modulation Shard"
	/call setINIVar stoponDS												bool		Common FALSE		
	/call setINIVar useGlyph												string	Common "Glyph of the Cataclysm"
	/call setINIVar switchSos												bool	 	Common FALSE


	| spire control
	/call setDeclare typeSpire											string	outer "|0|1|2|3"
	/call setDeclare ${Me.Class.ShortName}spire0		int			Buffs 0
	/call setINIVar spireRaid												int 		Buffs 0
	/call setINIVar spireGroup											int 		Buffs 0


	| unity buffs
	/for _count 1 to 7
		/call setINIVar BuffAAUnity${_count}					string	Buffs FALSE
	/next _count


	| combat controls
	/call setDeclare ccType													string	outer "|agro|alliance|bandolier|circle|combatdistance|engage|faderest|force|melee|meleedisc|range|nuke|dot|forceassist|nukedelay|offtank|pull|saferadius|smartengage|stopnuke|zradius"
	/call setINIVar MA1			 												string 	Common FALSE
	/call setINIVar MA2			 												string 	Common FALSE
	/call setINIVar MobAgro													int 		Common 60
	/call setINIVar engageAt 												int 		Common 98
	/call setINIVar smartengageAt 									int 		Common 98
	/call setINIVar	useSmartAssistG									bool		Common FALSE
	/call setINIVar	useSmartAssist1									bool		Common FALSE
	/call setINIVar	useSmartAssist2									bool		Common FALSE
	/call setINIVar	useSmartAssist3									bool		Common FALSE
	/call setINIVar	useForceAssist									bool		Common FALSE
	/call setINIVar switchAgro											bool		Common FALSE 
	/call setINIVar switchPull											bool		Common FALSE

	/call setINIVar switchOffTank										bool		Common FALSE
	
	/call setINIVar CombatDistance									int			Movement 15
	/call setINIVar combatRadius										string	Common 30
	/call setINIVar zradius													int			Common 60	
	/call setINIVar switchBandolier									bool		meleedisc FALSE
	/call setINIVar stopnukeAt 											int 		Spells 0
	/call setINIVar switchAlliance									bool		Spells FALSE
	/call setINIVar spellAllianceTrigger						string	Spells |FALSE|FALSE
	/call setINIVar switchForceFadeRest							bool		Common FALSE
	
	/call setINIVar switchCombatMelee								bool		Common FALSE
	/call setINIVar switchCombatRange								bool		Common FALSE
	/call setINIVar switchCombatNuke								bool		Common FALSE
	/call setINIVar switchCombatDoT									bool		Common FALSE
	/call setINIVar	broadcastGroup									string	Common Core
	
			
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
	/call setINIVar SetTieDistance 									int 		Movement 5
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
	/call setINIVar Breastplate1Condition						string	Items FALSE
	/call setINIVar Breastplate1effect							string	Items FALSE
	/call setINIVar Breastplate2		 								string	Items FALSE
	/call setINIVar Breastplate2Condition						string	Items FALSE
	/call setINIVar Breastplate2effect							string	Items FALSE
	
	/call setINIVar useCircle												string	Items FALSE
	/call setINIVar CircleofPower										string	Items FALSE
	/call setINIVar CircleofLife										string	Items FALSE
	/call setINIVar CircleofMana										string	Items FALSE

	/call setINIVar clickItemSwarm1									string	Items FALSE
	/call setINIVar clickItemSwarm2									string	Items FALSE
	/call setINIVar clickItemSwarm3									string	Items FALSE
	/call setINIVar clickItemSwarm4									string	Items FALSE

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
  | /call setDeclare overrideType										string	outer "|stop|engageat|exclude|combatheal|loot|los|melee|name|range|safelist|safezone|trainspell"
  /call setDeclare overrideType										string	outer "|stop|engageat|exclude|combat|loot|los|name|safelist|safezone|trainspell"
	/call setDeclare switchOverride									bool		outer FALSE
	/call setDeclare switchOverrideStop							bool		outer FALSE
  /call setINIVar switchOverrideLos								bool		Override FALSE
  /call setINIVar switchOverrideSafezone					bool		Override FALSE
  /call setINIVar switchOverrideExclude						bool		Override FALSE
  /call setINIVar switchOverrideName							bool		Override FALSE
  /call setINIVar switchOverrideEngageat					bool		Override FALSE
  | /call setINIVar switchOverrideRangecombat				bool		Override FALSE
  | /call setINIVar switchOverrideMeleecombat				bool		Override FALSE
  /call setINIVar switchOverrideLoot							bool		Override FALSE
  /call setINIVar switchOverrideSafelist					bool		Override FALSE
  /call setDeclare switchOverrideClasscheck				bool		outer FALSE
  /call setINIVar switchOverrideTrainspell				bool		Override FALSE
	/call setINIVar switchOverrideCombat						string	Override FALSE


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
	}
	/if (${Select[${Me.Class.ShortName},BRD,BER,MNK,ROG]}) {
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
	
	/if (!${Select[${Me.Class.ShortName},MNK,BER,WAR]}) {
		/call setINIVar switchNukeDelay								bool		Spells FALSE
	}	
	

	| buffs
	/call setINIVar BuffRemove											string	Buffs "|Illusion: Gnoll Reaver|Illusion: Darkfell Gnoll Reaver|Illusion: Blackburrow Gnoll Reaver|Illusion: Mucktail Gnoll Reaver|Illusion: Direwind Gnoll Reaver|Illusion: Human Pirate|Illusion: Ogre Pirate"
  /call setDeclare buffType												string	outer "|stop|self|item|combat|bc|raid|tell|pet|merc|group"
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
	/call setINIVar switchBuffGroup									bool		Buffs FALSE

	
	/if (${Me.Class.CanCast} || ${Select[${Me.Class.ShortName},ROG]}) {
		/call setINIVar buffGem 											string 	Spells "gem8"
		/call fix_gem_id "${buffGem}"
		/call setDeclare buffNum											 int 		local ${Macro.Return}
		
		| /call setDeclare buffNum											
		/call setINIVar GoMLevel											int			Spells 105
		/call setINIVar GoMNuke												string	Spells FALSE
		/call setINIVar GoMNukeCondition							string	Spells FALSE
		/call setINIVar GoMNuke2											string	Spells FALSE
		/call setINIVar GoMNuke2Condition							string	Spells FALSE
	}
	
	| bc/tell buffs
	/call setDeclare Buff0 													string 	outer FALSE
	/call setDeclare BuffCount											int 		outer 15
	/for _count 1 to ${BuffCount}
		/call setINIVar Buff${_count}									string 	Buffs FALSE
		/call setINIVar Buff${_count}chk							string 	Buffs FALSE
		/call setINIVar Buff${_count}self							string 	Buffs FALSE
		/call setINIVar Buff${_count}Alias						string 	Buffs "|FALSE|FALSE" 
		/call setINIVar BCBuff${_count}Class					string 	Buffs "|WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM"
		/call setINIVar RaidBuff${_count}Class				string 	Buffs "|WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM"
		/call setINIVar ManualBuff${_count}Class			string 	Buffs "|WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM"
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
	/call setINIVar sumFamiliar1										string	Pet FALSE
	/call setINIVar sumFamiliar1Buff								string	Pet FALSE
	/call setINIVar sumFamiliar1Leave								bool		Pet FALSE	
	/call setINIVar sumFamiliar2										string	Pet FALSE
	/call setINIVar sumFamiliar2Buff								string	Pet FALSE
	/call setINIVar sumFamiliar2Leave								bool		Pet FALSE	
	/call setINIVar sumFamiliar3										string	Pet FALSE
	/call setINIVar sumFamiliar3Buff								string	Pet FALSE
	/call setINIVar sumFamiliar3Leave								bool		Pet TRUE	

	| onoff types
  /call setDeclare onoffType											string	outer "|horn|shrink"
	/call setINIVar switchShrink										bool		Common FALSE
	/call setINIVar switchHorn											bool		Common FALSE

	| tradeskill control
  /call setDeclare 		tsType											string	outer "|movebank"
	
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
	


 |***
 *	DES: sets all timers used that are not specific to a class
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
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

	/call setTimer tPet_Buff												string	Timer 1m 		timer_check_buffs_pet
	/call setTimer tSelf_Buff												string	Timer 3m 		timer_check_buffs_self
	/call setTimer tItem_Buff												string	Timer 1m 		timer_check_buffs_item
	/call setTimer tBC_Buff													string	Timer 20s 	timer_check_buffs_BC
	/call setTimer tRaid_Buff												string	Timer 20s 	timer_check_buffs_raid
	/call setTimer tMerc_Buff												string	Timer 40s 	timer_check_buffs_merc
	/call setTimer tGroup_Buff											string	Timer 5m	 	timer_check_buffs_group
	/call setTimer tTell_Buff												string	Timer	10s 	timer_check_buffs_tell

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
 *	DES: Magician base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|   
sub declare_MAG()
	/call setDeclare MAGspire1											string	outer "Fundament: First Spire of the Elements"
	/call setDeclare MAGspire2											string	outer "Fundament: Second Spire of the Elements"
	/call setDeclare MAGspire3											string	outer "Fundament: Third Spire of the Elements"
	/call setDeclare MAGsynergyAA										string	outer "Conjurer's Synergy"
	/call setDeclare MAGEpic1												string	outer "Orb of Mastery"
	/call setDeclare MAGEpic15											string	outer "Staff of Elemental Essence"
	/call setDeclare MAGEpic2												string	outer "Focus of Primal Elements"
	/call setDeclare MAGAlliance										string	outer "Firebound Alliance"
	/call setDeclare MAGAAUnity											string	outer "Thaumaturge's Unity"
	/call setDeclare MAGAAFade											string	outer "Drape of Shadows"

	/call setINIVar MAGAAGroupCOH										string	outer "Call of the Heroes"

	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50
	
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
	/call setINIVar NukeSalvo												string 	Spells "Storm of Many"
	/call setINIVar NukeSalvoCondition 							string	Spells FALSE
	/call setINIVar DoDoT								 						int			Spells 0
	/call setINIVar Cauldron				 								string	Spells "Cauldron of Countless Goods"
	/call setINIVar useRain													bool 		Spells FALSE
	/call setINIVar doRainNuke											int 		Spells 2
	/call setINIVar RainNuke1 											string 	Spells "Rain of Cutlasses"
	/call setINIVar RainNuke2 											string 	Spells "Coronal Rain"
	/call setINIVar spellMalo 											string 	Spells "Malosinete"
	/call setINIVar switchModRod										bool	 	Buffs TRUE

	/call setINIVar ${Me.Class.ShortName}Point 			int 		Heal 80
	
	/declare _count int local 0
	/declare _list int local 0
	/declare _line string local NULL
	
	/call setDeclare lineType												string	outer "|off|fire|magic|mix"
	/call setINIVar lineCurrent											string 	Spells Fire
	/call setINIVar lineFire												string	Spells "|Spear of Molten Arcronite|Bolt of Skyfire"
	/call setINIVar lineMagic												string	Spells FALSE
	/call setINIVar lineMix													string	Spells FALSE

	| add conditions
	/for _list 1 to ${lineType.Count[|]}
		/varset _line ${lineType.Arg[${_list},|].Left[1].Upper}${lineType.Arg[${_list},|].Right[-1].Lower}
		/for _count 1 to 4
			/call setINIVar line${_line}Condition${_count}		string	Spells FALSE
		/next _count
	/next _list		
	
	/call setINIVar switchForceTwincast							bool		Spells TRUE
	/call setINIVar NukeTwincastFire								string	Spells "Chaotic Inferno"
	/call setINIVar NukeTwincastMagic								string	Spells FALSE
	/call setINIVar NukeTwincastMix									string	Spells FALSE
	/call setINIVar NukeTwincastCondition 					string	Spells FALSE	
	
	
	
/return
	


|***
 *	DES: Wizard vase variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_WIZ()
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
	/call setDeclare WIZAAFade											string	outer "A Hole in Space"

	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50

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
	/call setINIVar spellConcussionPct							int 		Spells 0

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
	
	
	| faydwer ports
	/call setDeclare fay 														string	outer "Translocate: Fay"
	/call setDeclare fay_group 											string	outer "Fay Portal"
	/call setDeclare dragonscale 										string	outer "Translocate: Dragonscale Hills"
	/call setDeclare dragonscale_group 							string	outer "Dragonscale Hills Portal"

	| antonica Ports
	/call setDeclare northkarana 										string	outer "Translocate: North"
	/call setDeclare northkarana_group 							string	outer "North Portal"
	/call setDeclare commonlands 										string	outer "Translocate: Common"
	/call setDeclare commonlands_group 							string	outer "Common Portal"
	/call setDeclare nektulos 											string	outer "Translocate: Nek"
	/call setDeclare nektulos_group 								string	outer "Nek Portal"
	/call setDeclare qeyenos 												string	outer "Translocate: West"
	/call setDeclare qeyenos_group 									string	outer "West Portal"
	/call setDeclare northro 												string	outer "Translocate: Ro"
	/call setDeclare northro_group 									string	outer "Ro Portal"
	/call setDeclare cazic 													string	outer "Translocate: Cazic"
	/call setDeclare cazic_group 										string	outer "Cazic Portal"
	/call setDeclare undershore 										string	outer "Translocate: Undershore"
	/call setDeclare undershore_group 							string	outer "Undershore Portal"
	/call setDeclare westkarana 										string	outer "Translocate: West Karana"
	/call setDeclare westkarana_group 							string	outer "West Karana Portal"

	| discord ports 
	/call setDeclare bloodfields 										string	outer "Translocate: Bloodfields"
	/call setDeclare bloodfields_group 							string	outer "Bloodfields Portal"
	/call setDeclare wallofslaughter 								string	outer "Translocate: Slaughter"
	/call setDeclare wallofslaughter_group 					string	outer "Slaughter Portal"

	| alaris ports
	/call setDeclare shardslanding 									string	outer "Translocate: Shard's Landing"
	/call setDeclare shardslanding_group 						string	outer "Shard's Landing Portal"
	/call setDeclare pillars 												string	outer "Translocate Pillars of Alra"
	/call setDeclare pillars_group 									string	outer "Alra Portal"
	/call setDeclare sarith 												string	outer "Translocate Sarith"
	/call setDeclare sarith_group 									string	outer "Sarith Portal"

	| Kunark ports
	/call setDeclare dreadlands 										string	outer "Translocate: Combine"
	/call setDeclare dreadlands_group 							string	outer "Combine Portal"
	/call setDeclare lceanium												string	outer "Translocate: Lceanium"
	/call setDeclare lceanium_group									string	outer "Lceanium Portal"
	/call setDeclare skyfire 												string	outer "Translocate: Skyfire"
	/call setDeclare skyfire_group 									string	outer "Skyfire Portal"	

	| liclin ports
	/call setDeclare dawnshroud 										string	outer "Translocate: Dawnshroud"
	/call setDeclare dawnshroud_group 							string	outer "Dawnshroud Portal"
	/call setDeclare twilight 											string	outer "Translocate: Twilight"
	/call setDeclare twilight_group 								string	outer "Twilight Portal"
	/call setDeclare grimling 											string	outer "Translocate: Grimling"
	/call setDeclare grimling_group 								string	outer "Grimling Portal"
	/call setDeclare nexus 													string	outer "Translocate: Nexus"
	/call setDeclare nexus_group 										string	outer "Nexus Portal"

	| odus ports
	/call setDeclare tempesttemple 									string	outer "Translocate: Tempest Temple"
	/call setDeclare tempesttemple_group 						string	outer "Tempest Temple Portal"
	/call setDeclare katta													string	outer "Translocate: Katta Castrum"
	/call setDeclare katta_group										string	outer "Katta Castrum Portal"
	/call setDeclare tox 														string	outer "Translocate: Tox"
	/call setDeclare tox_group 											string	outer "Tox Portal"
	/call setDeclare stonebrunt 										string	outer "Translocate: Stonebrunt"
	/call setDeclare stonebrunt_group 							string	outer "Stonebrunt Portal"

	| serpent's spine ports
	/call setDeclare icefall 												string	outer "Translocate: Icefall Glacier"
	/call setDeclare icefall_group 									string	outer "Icefall Glacier Portal"
	/call setDeclare sunderock 											string	outer "Translocate: Sunderock Springs"
	/call setDeclare sunderock_group 								string	outer "Sunderock Springs Portal"
	/call setDeclare blightfire 										string	outer "Translocate: Blightfire Moors"
	/call setDeclare blightfire_group 							string	outer "Blightfire Moors Portal"

	| taelosia ports
	/call setDeclare barnidu 												string	outer "Translocate: Barindu"
	/call setDeclare barnidu_group 									string	outer "Barindu Portal"
	/call setDeclare natimbi 												string	outer "Translocate: Natimbi"
	/call setDeclare natimbi_group 									string	outer "Natimbi Portal"

	| planes ports
	/call setDeclare grounds 												string	outer "Translocate: the Grounds"
	/call setDeclare grounds_group 									string	outer "Grounds Portal"
	/call setDeclare potime 												string	outer "Translocate: Plane of Time"
	/call setDeclare potime_group 									string	outer "Plane of Time Portal"
	/call setDeclare brells 												string	outer "Translocate: Brell's Rest"
	/call setDeclare brells_group 									string	outer "Brell's Rest Portal"
	/call setDeclare arcstone 											string	outer "Translocate: Arcstone"
	/call setDeclare arcstone_group 								string	outer "Arcstone Portal"
	/call setDeclare pok 														string	outer "Translocate: Knowledge"
	/call setDeclare pok_group 											string	outer "Knowledge Portal"

	| velious ports
	/call setDeclare cobaltscar 										string	outer "Translocate: Cobalt Scar"
	/call setDeclare cobaltscar_group 							string	outer "Cobalt Scar Portal"
	/call setDeclare wakening 											string	outer "Translocate: Wakening Lands"
	/call setDeclare wakening_group 								string	outer "Wakening Lands Portal"
	/call setDeclare greatdivide 										string	outer "Translocate: Great Divide"
	/call setDeclare greatdivide_group 							string	outer "Great Divide Portal"
	/call setDeclare iceclad 												string	outer "Translocate: Iceclad"
	/call setDeclare iceclad_group 									string	outer "Iceclad Portal"
	
	| misc ports	
	/call setDeclare bind 													string	outer "Translocate"
	/call setDeclare bind_group											string	outer "Teleport"
	/call setDeclare primary 												string	outer "Translocate: Primary Anchor"
	/call setDeclare primary_group 									string	outer "Primary Anchor Portal"
	/call setDeclare secondary 											string	outer "Translocate: Secondary Anchor"
	/call setDeclare secondary_group								string	outer "Secondary Anchor Portal"
	/call setDeclare pushprimary 										string	outer "Primary Anchor Push"
	/call setDeclare pushprimary_group 							string	outer "Teleport Primary Anchor"
	/call setDeclare pushsecondary 									string	outer "Secondary Anchor Push"
	/call setDeclare pushsecondary_group						string	outer "Teleport Secondary Anchor"
	
	/call setINIVar runeMagiWard	 									string	Rune "Doomscale Husk"
	/call setINIVar runeShieldofFate								string	Rune "Shield of Consequence"
	/call setINIVar runeCrystalwing									string	Rune "Armor of the Codex"
	/call setINIVar runeGuard			 									string	Rune "Darkmist Guard"

	/call setINIVar ${Me.Class.ShortName}Point 			int 		Heal 80
	/call setINIVar switchModRod										bool	 	Buffs TRUE
/return	



|***
 *	DES: Enchanter base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_ENC()
	/call setTimer tValidate_Lists									string	Timer 15s timer_validate_lists

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
	/call setDeclare ENCAAFade											string	outer "Self Stasis"

	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50
	/call setINIVar switchAABeamofSlumber						bool		Spells FALSE
	
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

	/call setINIVar ${Me.Class.ShortName}Point 			int 		Heal 80
	/call setINIVar switchModRod										bool	 	Buffs TRUE
/return	



|***
 *	DES: Necromancer base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_NEC()
	/call setDeclare NECspire1											string	outer "Fundament: First Spire of Necromancy"
	/call setDeclare NECspire2											string	outer "Fundament: Second Spire of Necromancy"
	/call setDeclare NECspire3											string	outer "Fundament: Third Spire of Necromancy"
	/call setDeclare NECsynergyAA										string	outer "Defiler's Synergy"
	/call setDeclare NECEpic1												string	outer "Scythe of the Shadowed Soul"
	/call setDeclare NECEpic15											string	outer "Soulwhisper"
	/call setDeclare NECEpic2												string	outer "Deathwhisper"
	/call setDeclare NECAlliance										string	outer "Malevolent Alliance"
	/call setDeclare NECAAUnity											string	outer "Mortifier's Unity"
	/call setDeclare NECAAFade											string	outer FALSE

	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50

	/call setTimer tCheck_Blood											string	Timer 8s	timer_check_blood

	/call setINIVar spellMalo												string 	Spells "Scent of Terris"
	/call setINIVar spellBlood											string 	Spells "Impose for Blood"
	/call setINIVar spellSlow		 										string	Spells "Scent of Terris"
	/call setINIVar spellCripple 										string 	Spells FALSE
	/call setINIVar switchGather					 					bool		Spells TRUE
	/call setINIVar GatherPercent			 							int			Spells 60
	
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

	/call setINIVar DoNuke							 						int			Spells 0
	/call setINIVar Nuke1					 									string	Spells FALSE	
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
	/call setINIVar switchModRod										bool	 	Buffs TRUE
/return	



|***
 *	DES: Shadowknight base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_SHD()
	/call setTimer tcheck_vot												string Timer 1215s timer_check_vot

	/call setDeclare SHDspire1											string	outer "Fundament: First Spire of the Reavers"
	/call setDeclare SHDspire2											string	outer "Fundament: Second Spire of the Reavers"
	/call setDeclare SHDspire3											string	outer "Fundament: Third Spire of the Reavers"
	/call setDeclare SHDsynergyAA										string	outer "Knight's Synergy"
	/call setDeclare SHDEpic1												string	outer "Innoruuk's Curse"
	/call setDeclare SHDEpic15											string	outer "Innoruuk's Voice"
	/call setDeclare SHDEpic2												string	outer "Innoruuk's Dark Blessing"
	/call setDeclare SHDAlliance										string	outer "Bloodletting Alliance"
	/call setDeclare SHDAAUnity											string	outer "Dark Lord's Unity (Beza)"
	/call setDeclare SHDAAFade											string	outer "Death Peace"
	
	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50

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
	/call setINIVar AEMelee					 								bool	 	Discs FALSE

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
	/call setINIVar Nuke4 													string 	Spells FALSE
	/call setINIVar Nuke4Condition 									string	Spells FALSE
	/call setINIVar Nuke5 													string 	Spells FALSE
	/call setINIVar Nuke5Condition 									string	Spells FALSE
	/call setINIVar Nuke6 													string 	Spells FALSE
	/call setINIVar Nuke6Condition 									string	Spells FALSE
	
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
	/call setINIVar Skin			 											string 	Buffs "Tylix's Skin"
	/call setINIVar switchVoT				 								bool	 	Buffs TRUE
	/call setINIVar switchModRod										bool	 	Buffs TRUE

	/call setINIVar ${Me.Class.ShortName}Point 			int 		Heal 80
	/call setINIVar useShieldFlash									bool		Discs FALSE
	/call setINIVar useShieldFlashPct								int			Discs 20

/return	

 

|***
 *	DES: Paladin base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_PAL()
	/call setDeclare PALspire1											string	outer "Fundament: First Spire of Holiness"
	/call setDeclare PALspire2											string	outer "Fundament: Second Spire of Holiness"
	/call setDeclare PALspire3											string	outer "Fundament: Third Spire of Holiness"
	/call setDeclare PALsynergyAA										string	outer "Knight's Synergy"
	/call setDeclare PALEpic1												string	outer "Fiery Defender"
	/call setDeclare PALEpic15											string	outer "Heartwood Blade"
	/call setDeclare PALEpic2												string	outer "Nightbane, Sword of the Valiant"
	/call setDeclare PALAlliance										string	outer "Holy Alliance"
	/call setDeclare PALAAUnity											string	outer "Divine Protector's Unity"
	/call setDeclare PALAAFade											string	outer "Balefire Burst"
	
	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50

	/call setTimer tcheck_MG												string	Timer	10s timer_check_MG
	/call setTimer tcheck_Steel											string 	Timer 2m timer_check_Steel
	/call setTimer tcheck_Stance										string 	Timer 7.5m timer_check_Stance

	/call setDeclare mobAECount											int			outer 0 
	
	/call setDeclare hurtID		 											string 	outer FALSE
	/call setDeclare hurtPctHP											int 		outer 100
	/call setDeclare hurtCLS		 										string 	outer FALSE
	/call setDeclare hurtCount 											int 		outer 0
	
  /call setINIVar splashGroup											string 	Heal "Wave of Expiation"
  /call setINIVar splashPanic											string 	Heal "Aurora of Morninglight"
  /call setINIVar healPanic1											string 	Heal "Expiation"
  /call setINIVar healPanic2											string 	Heal "Grief"
  /call setINIVar healTouch												string 	Heal "Merciful Touch"
	/call setINIVar useGroupHeal 										bool		Heal TRUE
	/call setINIVar XTHealRadius										int			Heal 300
	/call setINIVar RampageTank											string	Heal FALSE
	
	/call setINIVar doYaulp 												bool		Buffs TRUE
	/call setINIVar spellYaulp 											string	Buffs FALSE
	/call setINIVar useYaulpPct											int			Buffs 98
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
	/call setINIVar AEMelee					 								bool	 	Discs FALSE
	                                         	
	/call setINIVar DoNuke					 								int			Spells 5
	/call setINIVar Nuke1														string 	Spells "Force of Ardency"
	/call setINIVar Nuke1Condition 									string	Spells FALSE
	/call setINIVar Nuke2														string 	Spells "Ardent Force"
	/call setINIVar Nuke2Condition 									string	Spells FALSE
	/call setINIVar Nuke3														string 	Spells "Force of the Darkened Sea"
	/call setINIVar Nuke3Condition 									string	Spells FALSE
	/call setINIVar Nuke4														string 	Spells "Lesson of Grief"
	/call setINIVar Nuke4Condition 									string	Spells FALSE
	/call setINIVar Nuke5							 							string	Spells "Protective Devotion"
	/call setINIVar Nuke5Condition 									string	Spells "!${Me.Song[Protective Devotion].ID}"
	/call setINIVar Nuke6							 							string	Spells FALSE
	/call setINIVar Nuke6Condition 									string	Spells FALSE
                                           	
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
	                                        		
	/call setINIVar switchModRod										bool	 	Buffs TRUE
	/call setINIVar switchHarmonious 								bool	 	Buffs TRUE
	/call setINIVar Harmonious 											string 	Buffs "Harmonious Blessing"
	/call setINIVar switchSteel			 								bool	 	Buffs TRUE
	/call setINIVar Steel			 											string 	Buffs "Reinvigorating Steel"
	/call setINIVar switchStance 										bool	 	Buffs TRUE
	/call setINIVar Stance 													string 	Buffs "Staunch Stance"
	                                        		
	/call setINIVar useMG														bool		Buffs TRUE
	/call setINIVar useMGPct												int			Buffs 60
	/call setINIVar useShieldFlash									bool		Discs FALSE
	/call setINIVar useShieldFlashPct								int			Discs 20
	/call setINIVar useLoH													bool		Heal FALSE
	/call setINIVar useLoHPct												int			Heal 10

/return	



|***
 *	DES: Warrior base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_WAR()
	/call setDeclare WARspire1											string	outer "Fundament: First Spire of the Warlord"
	/call setDeclare WARspire2											string	outer "Fundament: Second Spire of the Warlord"
	/call setDeclare WARspire3											string	outer "Fundament: Third Spire of the Warlord"
	/call setDeclare WARsynergyAA										string	outer "Myrmidon's Synergy"
	/call setDeclare WAREpic1												string	outer "Jagged Blade of War"
	/call setDeclare WAREpic15											string	outer "Champion's Sword of Eternal Power"
	/call setDeclare WAREpic2												string	outer "Kreljnok's Sword of Eternal Power"
	/call setDeclare WARAlliance										string	outer FALSE
	/call setDeclare WARAAUnity											string	outer FALSE
	/call setDeclare WARAAFade											string	outer FALSE
	
	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50
	/call setINIVar AEMelee					 								bool	 	Discs FALSE

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
	/call setINIVar AAVehementRage 									string	Discs "Vehement Rage"
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
	/call setDeclare BERspire1											string	outer "Fundament: First Spire of Savagery"
	/call setDeclare BERspire2											string	outer "Fundament: Second Spire of Savagery"
	/call setDeclare BERspire3											string	outer "Fundament: Third Spire of Savagery"
	/call setDeclare BERsynergyAA										string	outer "Fury's Synergy"
	/call setDeclare BEREpic1												string	outer "Kerasian Axe of Ire"
	/call setDeclare BEREpic15											string	outer "Raging Taelosian Alloy Axe"
	/call setDeclare BEREpic2												string	outer "Vengeful Taelosian Blood Axe"
	/call setDeclare BERAlliance										string	outer "Demolisher's Alliance"
	/call setDeclare BERAAUnity											string	outer FALSE
	/call setDeclare BERAAFade											string	outer FALSE
	
	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50

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
	/call setINIVar ${Me.Class.ShortName}Point 			int 		Heal 60
	/call setDeclare OpenWoundEffect								string 	outer "Open Wound"
	/call setDeclare AugmentedFrenzyEffect 					string 	outer "Augmented Frenzy Effect"
	/call setINIVar buffGem 												string 	Spells FALSE
   
	/call setINIVar spellCripple 										string 	Spells FALSE
/return	



|***
 *	DES: Ranger base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_RNG()
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
	/call setDeclare RNGAAFade											string	outer "Cover Tracks"
	
	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50

	/call setTimer tNuke_Summer											string	Timer 3s timer_cast_nuke_summer

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

	/call setINIVar ${Me.Class.ShortName}Point		 	int 		Heal 80
  /call setINIVar heal1	 													string  Heal "Cloudfont"
  /call setINIVar heal2					 									string  Heal "Desperate Dousing"
                                          		
	/call setINIVar DiscMele			 									string	Spells "Windstalkers's Discipline"
	/call setINIVar DiscRange			 									string	Spells "Pureshot Discipline"
	/call setINIVar DiscRest			 									string	Spells "Breather"
	/call setINIVar DiscJolt			 									string	Spells "Jolting Wheel Kicks"
	/call setINIVar DiscEnrage		 									string	Spells "Enraging Wheel Kicks"
	/call setINIVar DiscHeal			 									string	Spells "Reflexive Bladespurs"
	/call setINIVar BladeStorm											string	Spells "Focused Squall of Blades"
	/call setINIVar AuspiceTXT											string	Spells "AUSPICE (critSpell 33%, accuracy 230%, critHeal 33%, critMelee 225%, critDoT 33%, attack +165)"
	/call setINIVar DiscSqual			 									string	Spells "Focused Squall of Blades"
	                                        		
	/call setINIVar AADefensive1 										string	Spells "Outrider's Evasion"
	/call setINIVar AADefensive2 										string	Spells "Protection of the Spirit Wolf"
	/call setINIVar AADefensive3 										string	Spells "Bulwark of the Brownies"
	/call setINIVar switchModRod										bool	 	Buffs TRUE

/return	



|***
 *	DES: Druid base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_DRU()
	/call setDeclare DRUspire1											string	outer "Fundament: First Spire of Nature"
	/call setDeclare DRUspire2											string	outer "Fundament: Second Spire of Nature"
	/call setDeclare DRUspire3											string	outer "Fundament: Third Spire of Nature"
	/call setDeclare DRUsynergyAA										string	outer "Preserver's Synergy"
	/call setDeclare DRUEpic1												string	outer "Nature Walker's Scimitar"
	/call setDeclare DRUEpic15											string	outer "Staff of Living Brambles"
	/call setDeclare DRUEpic2												string	outer "Staff of Everliving Brambles"
	/call setDeclare DRUAlliance										string	outer "Bosquetender's Alliance"
	/call setDeclare DRUAAUnity											string	outer "Wildtender's Unity"
	/call setDeclare DRUAAFade											string	outer "Veil of the Underbrush"

	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50

	/call setINIVar switchModRod										bool	 	Buffs TRUE
	
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

	/call setDeclare hurtID		 											string 	outer FALSE
	/call setDeclare hurtPctHP											int 		outer 100
	/call setDeclare hurtCLS		 										string 	outer FALSE
	/call setDeclare hurtCount 											int 		outer 0

	/call setINIVar RandRezDelay										int 		Heal 10
	/call setINIVar ProtectOutsideGroup							string	Heal "|FALSE|FALSE"
	/call setINIVar ProtectInsideRaid								string	Heal "|FALSE|FALSE"
	/call setINIVar ProtectNPCList									string	Heal "|FALSE|FALSE"
	/call setINIVar useGroupHeal 										bool		Heal TRUE
	/call setINIVar useGroupHoT											bool		Heal TRUE
                                           	
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
	/call setDeclare SHMspire1											string	outer "Fundament: First Spire of Ancestors"
	/call setDeclare SHMspire2											string	outer "Fundament: Second Spire of Ancestors"
	/call setDeclare SHMspire3											string	outer "Fundament: Third Spire of Ancestors"
	/call setDeclare SHMsynergyAA										string	outer "Luminary's Synergy"
	/call setDeclare SHMEpic1												string	outer "Spear of Fate"
	/call setDeclare SHMEpic15											string	outer "Crafted Talisman of Fates"
	/call setDeclare SHMEpic2												string	outer "Blessed Spiritstaff of the Heyokah"
	/call setDeclare SHMAlliance										string	outer "Ancient Alliance"
	/call setDeclare SHMAAUnity											string	outer "Visionary's Unity"
	/call setDeclare SHMAAFade											string	outer FALSE
	
	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50
	
	/call setDeclare hurtID		 											string 	outer FALSE
	/call setDeclare hurtPctHP											int 		outer 100
	/call setDeclare hurtCLS		 										string 	outer FALSE
	/call setDeclare hurtCount 											int 		outer 0

	/call setTimer 	tTalisman												string	Timer 30s timer_cast_talisman
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
                                          		
	/call setINIVar CanniPctHPs				 							int			Spells 60
	/call setINIVar CanniPctManna				 						int			Spells 60
	/call setINIVar spellCannibalization						string	Spells "Tribal Pact"

	/call setINIVar switchSurge											bool		Heal TRUE
	/call setINIVar switchHealAggressive						bool		Heal FALSE
                                          		
	/call setINIVar useGroupHeal 										bool		Heal TRUE
	/call setINIVar useGroupHoT											bool		Heal TRUE
	/call setINIVar RandRezDelay										int 		Heal 10
                                          		
  /call setINIVar healSurge												string 	Heal "Spiritual Surge"
  /call setINIVar healMain												string 	Heal "Krasir's Mending"
  /call setINIVar healIntervention1								string 	Heal "Historian's Intervention"
  /call setINIVar healReckless1										string 	Heal "Reckless Regeneration"
  /call setINIVar healReckless2										string 	Heal FALSE
  /call setINIVar healReckless3										string 	Heal FALSE
  /call setINIVar healWholeGroup									string 	Heal "Krasir's Recourse"
  /call setINIVar healWholeGroupHoT								string 	Heal "Shear of Renewal"
                                          		
	/call setINIVar ProtectOutsideGroup							string	Heal "|FALSE|FALSE"
	/call setINIVar ProtectInsideRaid								string	Heal "|FALSE|FALSE"
	/call setINIVar ProtectNPCList									string	Heal "|FALSE|FALSE"
	/call setINIVar XTHealRadius										int			Heal 300
	/call setINIVar RampageTank											string	Heal FALSE
	/call setINIVar switchModRod										bool	 	Buffs TRUE

/return	



|***
 *	DES: Bard base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_BRD()
	/call setTimer tValidate_Lists									string	Timer 15s timer_validate_lists
	/call setDeclare BRDspire1											string	outer "Fundament: First Spire of the Minstrel"
	/call setDeclare BRDspire2											string	outer "Fundament: Second Spire of the Minstrel"
	/call setDeclare BRDspire3											string	outer "Fundament: Third Spire of the Minstrel"
	/call setDeclare BRDsynergyAA										string	outer "Troubadour's Synergy"
	/call setDeclare BRDEpic1												string	outer "Singing Short Sword"
	/call setDeclare BRDEpic15											string	outer "Prismatic Dragon Blade"
	/call setDeclare BRDEpic2												string	outer "Prismatic Dragon Blade"
	/call setDeclare BRDAlliance										string	outer "Alliance of Sticks and Stones"
	/call setDeclare BRDAAUnity											string	outer FALSE
	/call setDeclare BRDAAFade											string	outer "Fading Memories"

	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50

	/call setINIVar ServantNukeAA1 									string 	Pet "Lyrical Prankster"
	/call setINIVar ServantNukeAA2 									string 	Pet "Song of Stone"
	/call setINIVar songServantNuke									string 	Pet "Plague of the Piper"
	                                          		
	/call setDeclare lineType												string	outer |caster|melee|mix|other|tank
	/call setINIVar lineCurrent											string 	Song Tank
	/call setINIVar lineTank												string	Song "|Chorus of Sionachie|War March of Jocelyn|Fjilnauk's Spiteful Lyric|Fjilnauk's Song of Suffering|Doben's Spry Sonata"
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
	/call setINIVar switchModRod										bool	 	Buffs TRUE
	
	
/return	


|***
 *	DES: Beastlord base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_BST()
	/call setDeclare BSTspire1											string	outer "Fundament: First Spire of the Savage Lord"
	/call setDeclare BSTspire2											string	outer "Fundament: Second Spire of the Savage Lord"
	/call setDeclare BSTspire3											string	outer "Fundament: Third Spire of the Savage Lord"
	/call setDeclare BSTsynergyAA										string	outer "Primalist's Synergy"
	/call setDeclare BSTEpic1												string	outer "Claw of the Savage Spirit"
	/call setDeclare BSTEpic15											string	outer "Savage Lord's Totem"
	/call setDeclare BSTEpic2												string	outer "Spiritcaller Totem of the Feral"
	/call setDeclare BSTAlliance										string	outer "Venomous Alliance"
	/call setDeclare BSTAAUnity											string	outer "Feralist's Unity"
	/call setDeclare BSTAAFade											string	outer FALSE
	
	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50

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
	| /call setINIVar Aggression											string 	Pet "Sekmoset's Aggression"
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

	/call setINIVar switchSelfParagon		 						bool		Spells TRUE
	/call setINIVar selfParagonPct 									int			Spells 80
	/call setINIVar switchConsumeSpirit	 						bool		Spells TRUE
	/call setINIVar ConsumeSpiritPct								int			Spells 80
	/call setINIVar switchGroupParagon		 					bool		Spells TRUE
	/call setINIVar groupParagonPct 								int			Spells 50
                                          		
	/call setINIVar spellSlow		 										string	Spells "Sha's Reprisal"		
	/call setINIVar spellCripple 										string 	Spells "Incapacitate"
                                             		
	/call setINIVar ${Me.Class.ShortName}Point 			int 		Heal 80
	/call setINIVar switchModRod										bool	 	Buffs TRUE
/return	



|***
 *	DES: Monk base variables
 *	USE: 
 *	INI: 
 *  NOTE: i need to go back on my drugs
 ***|
sub declare_MNK()
	/call setDeclare MNKspire1											string	outer "Fundament: First Spire of the Sensei"
	/call setDeclare MNKspire2											string	outer "Fundament: Second Spire of the Sensei"
	/call setDeclare MNKspire3											string	outer "Fundament: Third Spire of the Sensei"
	/call setDeclare MNKsynergyAA										string	outer "Calanin's Synergy"
	/call setDeclare MNKEpic1												string	outer "Celestial Fists"
	/call setDeclare MNKEpic15											string	outer "Fistwraps of Celestial Discipline"
	/call setDeclare MNKEpic2												string	outer "Transcended Fistwraps of Immortality"
	/call setDeclare MNKAlliance										string	outer "Doomwalker's Alliance"
	/call setDeclare MNKAAUnity											string	outer FALSE
	/call setDeclare MNKAAFade											string	outer "Imitate Death"
	
	/call setINIVar spellCripple 										string 	Spells FALSE
	/call setINIVar switchMNKTrainMelee							bool		Common FALSE
	/call setINIVar buffGem 												string 	Spells "gem8"

	/call setINIVar setFDAgroPct										int			Melee FALSE
	/call setINIVar setAgroPct											int			Melee 95
	/call setINIVar setFDPct												int			Melee 25
 	/call setINIVar DiscRest												string	Melee "Breather"
 	
	/call setINIVar AEMelee					 								bool	 	Discs FALSE
	
	/call setINIVAR DiscDicho												string	Melee "Dichotomic Form"
	/call setINIVAR DiscBalance											string	Melee "Tiger's Poise"
	/call setINIVAR DiscMonkey											string	Melee "Drunken Monkey Style"
	/call setINIVAR DiscHeel												string	Melee "Heel of Zagali"
	/call setINIVAR DiscAshenhand										string	Melee "Ironfist Discipline"
	/call setINIVAR DiscCrane												string	Melee "Crane Stance"
	/call setINIVAR DiscDoubleAttack								string	Melee "Fists of Wu"
	/call setINIVAR DiscFists												string	Melee "Firestorm of Fists"
	/call setINIVAR DiscSynergy											string	Melee "Firewalker's Synergy"
	/call setINIVAR DiscFang												string	Melee "Hoshkar's Fang"
	/call setINIVAR DiscCurse												string	Melee "Curse of the Thirteen Fingers"
	/call setINIVAR DiscThrowing										string	Melee "Doomwalker's Precision Strike"
	/call setINIVAR DiscShuriken										string	Melee "Vigorous Shuriken"
	/call setINIVAR DiscStorm												string	Melee "Eye of the Storm"
	/call setINIVAR DiscRest												string	Melee "Breather"
	/call setINIVAR DiscDelayDeath									string	Melee "Refuse Death"  	
	
	/call setINIVAR AASilent												string	Melee "Silent Strikes"
	/call setINIVAR AAThunder												string	Melee "Infusion of Thunder"
	/call setINIVAR AAFocus													string	Melee "Focused Destructive Force"
	/call setINIVAR AATwinStrike										string	Melee "Five Point Palm"
	/call setINIVAR AAChant													string	Melee "Swift Tail's Chant"
	/call setINIVAR AAZanFi													string	Melee "Zan Fi's Whistle"
	/call setINIVAR AAWasp													string	Melee "Two-Finger Wasp Touch"	
	
	/call setINIVar ${Me.Class.ShortName}Point 			int 		Heal 80
/return	



|***
 *	DES: Rogue base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub declare_ROG()
	/call setDeclare ROGspire1											string	outer "Fundament: First Spire of the Rake"
	/call setDeclare ROGspire2											string	outer "Fundament: Second Spire of the Rake"
	/call setDeclare ROGspire3											string	outer "Fundament: Third Spire of the Rake"
	/call setDeclare ROGsynergyAA										string	outer "Blackguard's Synergy"
	/call setDeclare ROGEpic1												string	outer "Ragebringer"
	/call setDeclare ROGEpic15											string	outer "Fatestealer"
	/call setDeclare ROGEpic2												string	outer "Nightshade, Blade of Entropy"
	/call setDeclare ROGAlliance										string	outer "Poisonous Alliance"
	/call setDeclare ROGAAUnity											string	outer FALSE
	/call setDeclare ROGAAFade											string	outer "Escape"
	
	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50

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
	                                           		
	/call setINIVar clickPoisonBuff									string 	Poison "|Bite of the Shissar Poison X|Tallon's Tactic Poison IV|Tallon's Tactic Poison V|Vallon's Tactic Poison IV|Vallon's Tactic Poison V|Mana Poison Coat|Hero's Toxin Poison I|Hero's Toxin Poison II"
	/call setINIVar clickPoisonItem									string 	Poison "Consigned Bite of the Shissar XVIII"
	/call setINIVar clickPoisonDispenser						string 	Poison "Selrach's Shadowscale Leggings"
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
	/call setDeclare CLRspire1											string	outer "Fundament: First Spire of Divinity"
	/call setDeclare CLRspire2											string	outer "Fundament: Second Spire of Divinity"
	/call setDeclare CLRspire3											string	outer "Fundament: Third Spire of Divinity"
	/call setDeclare CLRsynergyAA										string	outer "Templar's Synergy"
	/call setDeclare CLREpic1												string	outer "Water Sprinkler of Nem Ankh"
	/call setDeclare CLREpic15											string	outer "Harmony of the Soul"
	/call setDeclare CLREpic2												string	outer "Aegis of Superior Divinity"
	/call setDeclare CLRAlliance										string	outer "Divine Alliance"
	/call setDeclare CLRAAUnity											string	outer "Saint's Unity"
	/call setDeclare CLRAAFade											string	outer "Divine Peace"
	
	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50

	/call setTimer	tHealWard												string	Timer 185s timer_heal_ward
	/call setINIVar switchLockMA										bool 		Heal FALSE
		
	/call setDeclare hurtID		 											string 	outer FALSE
	/call setDeclare hurtPctHP											int 		outer 100
	/call setDeclare hurtCLS		 										string 	outer FALSE
	/call setDeclare hurtCount 											int 		outer 0
	
	/call setINIVar DivineRezCleric									bool 		Heal TRUE
	/call setINIVar RandRezDelay										int 		Heal 5
	/call setINIVar ProtectOutsideGroup							string	Heal "|FALSE|FALSE"
	/call setINIVar ProtectInsideRaid								string	Heal "|FALSE|FALSE"
	/call setINIVar ProtectNPCList									string	Heal "|FALSE|FALSE"
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
  /call setINIVar healHoT 												string 	Heal FALSE
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
  /call setINIVar healWardToon	 									string  Heal "|FALSE|FALSE"
  /call setINIVar useCelestialRegen								bool	  Heal TRUE
  /call setINIVar healIssuance										string  Heal "Issuance of Spirit"
	/call setINIVar CRTXT														string	Heal "Celestial Regeneration"
	/call setINIVar XTHealRadius										int			Heal 300
	/call setINIVar switchForceGroupHeal						bool		Heal FALSE
	/call setINIVar RampageTank											string	Heal FALSE
	/call setINIVar switchHealAggressive						bool		Heal FALSE
	/call setINIVar switchCompleteHeal							bool		Heal FALSE
	/call setINIVar CompleteHealpct									int 		Heal 70	
	/call setINIVar switchIssuance									bool		Heal FALSE
	/call setINIVar issuanceMobCount								int			Heal 4
	                                          		
	/call setINIVar doYaulp 												bool		Buffs TRUE
	/call setINIVar spellYaulp 											string	Buffs FALSE
	/call setINIVar useYaulpPct											INT			Buffs 98
	/call setINIVar sumWeaponSpell									string	Buffs "Hammer of Reverence"
	/call setINIVar sumWeaponItem										string	Buffs "Hammer of Reverence III"
	/call setINIVar switchModRod										bool	 	Buffs TRUE
                                            		
	/call setINIVar switchVP												bool		Buffs TRUE
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
	
	/call setINIVar BreastplateHeal		 							string	Items FALSE
	/call setINIVar BreastplateHealCondition				string	Items FALSE	

/return	



|* Created Events ******************************************************************************************************
  
|***
 *	DES: stuf we do cause we are lazy.
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub set_alias()
  /if (!${Bool[${Alias[/sendpets]}]}) 						/squelch /alias /sendpets										VOUT SendPets: 
	/if (!${Bool[${Alias[/blurb]}]}) 								/squelch /alias /blurb 											VOUT Blurb: 
  /if (!${Bool[${Alias[/echoaugments]}]}) 				/squelch /alias /echoaugments								VOUT Echo Augment file!
                                            			
  /if (!${Bool[${Alias[/buy]}]}) 									/squelch /alias /buy												/echo Buy:
  /if (!${Bool[${Alias[/sell]}]}) 								/squelch /alias /sell												/echo Sell:
                                            			
	/if (!${Bool[${Alias[/on]}]}) 									/noparse /squelch /alias /on       		    	/broadcast_on
	/if (!${Bool[${Alias[/off]}]}) 									/noparse /squelch /alias /off						    /broadcast_off
                                            			
	/if (!${Bool[${Alias[/givecoin]}]}) 						/noparse /squelch /alias /givecoin			    VOUT Bribe Time:
                                            			
 	| utility
  /if (!${Bool[${Alias[/autologin]}]}) 						/squelch /alias /autologin									/plugin mq2autologin unload
	
/return



|***
 *	DES: raid and group invite accept controls
 *	USE: EVENTS
 *	INI: 
 *  NOTE:
 ***| 
#event Join "#1# invites you to join a #2#.#*#"
sub Event_Join(string line, string _sender, string _groupraid)
	CHECKSAFELIST

	| group invites
	/if (${_groupraid.Equal[group]}) {
		/if (!${switchTakeGroupInvite}) {
			/return
		} else /if (${switchTakeGroupInvite}) {
			VOUT Accepting invite from ${cpc}${_sender}\ax for ${_groupraid}.
			/invite
		}
	}
	
	| raid invites
	/if (${_groupraid.Equal[raid]}) {
		/delay 1s
		/if (!${switchTakeRaidInvite}) {
			/if (${Window[ConfirmationDialogbox].Child[CD_TextOutPut].Text.Find["join a raid"]}) {
				/nomodkey /notify ConfirmationDialogBox No_Button leftmouseup
				/return
			}
			/raiddecline
		} else /if (${switchTakeRaidInvite}) {
			VOUT Accepting invite from ${cpc}${_sender}\ax for ${_groupraid}.
			/if (${Window[ConfirmationDialogbox].Child[CD_TextOutPut].Text.Find["join a raid"]}) {
				/nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
				/return
			}
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
#event broadcast_command "<#1#> #2#"
#event broadcast_command "[#1#(msg)] #2#"
sub Event_broadcast_command(string line, string _sender, string _cmd)

	/if (${_cmd.Equal[stop]}) /call set_stop FALSE
 
	|***
	* DES: casts AA radient cure
	* USE: /rc | /bc rc
	***|
	/if (${_cmd.Equal[rc]}) {
		/if (!${Select[${Me.Class.ShortName},CLR,DRU,SHM,PAL]}) /return FALSE
		/if (!${Me.AltAbilityReady[Radiant Cure]}) /return FALSE
		/stopcast
		/delay 1s !${Me.Casting.ID}
		/if (${validate_cast[FALSE, alt, "Radiant Cure", ${Me.ID}]}) /call core_cast2 "Radiant Cure" alt 0 FALSE
		ECHOCHANNEL Radiant Cure
		/return
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
		/if (${validate_cast[FALSE, alt, "Purified Spirits", ${Spawn[pc ${_sender}].ID}]}) /call core_cast2 "Purified Spirits" alt ${Spawn[pc ${_sender}].ID} FALSE
		ECHOCHANNEL Purified Spirits: ${Spawn[pc ${_sender}].Name}
		/return
	}
	
/return

	
#event camping "It will take you about 30 seconds to prepare your camp."
sub Event_camping(string line)
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
	/if (${Me.Class.ShortName.Equal[MNK]}) {
		/if (${switchMNKTrainMelee}) /call melee_train_MNK
  }
   
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
	/if (!${switchGiveRaidInvite}) /return
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
/return

|***
*	DES: blurb events for raids. 
*	USE: /blurb EXPANSION EVENT
***|
#event Blurb "[MQ2] - Blurb: #1# #2#"
#event Blurb "[MQ2] - Blurb: #1#"

sub Event_Blurb(string line, string exp, string event)
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
 *	DES: khajiit has wares
 *	USE: gives coins to a NPC..
 *	INI: 
 *  NOTE: 
 ***| 	
#event bribetime "[MQ2] - Bribe Time: #1# #2# #3#"
sub Event_bribetime(string line, int _amount, int _stacks, string _coin)
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
#event Talk "#1# tells you,#*# '#2#'"

sub Event_Talk(string line, string _sender, string _chatText)
	
	/if (${Bool[${relayTells}]} && !${_sender.Find[Banker]} && !${_sender.Find[${Me}]} && !${_sender.Find[${relayTells}]} && !${_sender.Find[${Me.Pet.DisplayName}]} ) {
		/tell ${relayTells} ${_sender} told me: ${_chatText}
	}

	CHECKSAFELIST

	| Tells for buffs.
	/if (${switchBuffTell} && ${Spawn[${_sender}].ID} && ${_sender.NotEqual[${Me.DisplayName}]}) {
    /call cast_buffs_queue "${_sender}" "${_chatText}"
    /return
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
  /if (${_chatText.Equal[makemeleader]}) /makeleader ${_sender}
  /if (${_chatText.Equal[makememasterlooter]}) /delegategmasterlooter ${_sender}
  
  | raid chat
  /if (${_chatText.Equal[raidinviteme]} && ${Me.DisplayName.Equal[${Raid.Leader}]} && !${Raid.Locked}) /raidinvite ${_sender}
  /if (${_chatText.Equal[makemeraidleader]} && ${Me.DisplayName.Equal[${Raid.Leader}]} && !${Raid.Locked}) /makeraidleader ${_sender}
  /if (${_chatText.Equal[makemedzleader]}) /dzmakeleader ${_sender}
  /if (${_chatText.Equal[makemetaskleader]}) /taskmakeleader ${_sender}
  
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
| #event Zone "You have entered #*#" 
#event Zone "LOADING, PLEASE WAIT..."
sub Event_Zone(string _line)

	| force a delay while zoning
	:stillfuckingzoningOMG
	/if (${switchDelayWhileZoning} && ${Me.Zoning}) {
		/delay 45s !${Me.Zoning}
	}

	/if (${Me.Zoning}) {
		/goto :stillfuckingzoningOMG
	} else /if (!${Me.Zoning}) {
		/call validate_zone FALSE TRUE
	}
/return FALSE



|***
 *	DES: Buy things from vendors
 *	USE: /echo Buy: 'itemname' ##
 *	INI: 
 *  NOTE: single quotes
 ***| 
#event buystuff	"[MQ2] Buy: '#1#' #2#"
sub Event_buystuff(string line, string _itemtoBuy, int _buyAmount)
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
	/call vendor_sell "${_itemtoSell}"
/return



|***
 *	DES: rez emote
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***| 	
#event resurrection_exp_gain "You regain some experience from resurrection."
sub Event_resurrection_exp_gain()
	/delay 2s !${Me.State.Equal[HOVER]}
	/delay 2s !${Me.Hovering}
	/delay 2s !${Me.Dead}
	/varset tmpMode FALSE
	/call set_stop NOECHO
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

	/declare _tmpSpell string local FALSE
	/if (${Bool[${Me.Gem[${fix_gem_id[${buffGem}]}]}]})  {
		/varset _tmpSpell ${Me.Gem[${fix_gem_id[${buffGem}]}]}
	}
	
	/call check_buffs_self
	/if (${Select[${Me.Class.ShortName},SHD,PAL]}) /call check_harmonious
	/if (${Select[${Me.Class.ShortName},WIZ,ENC]}) /call check_rune_${Me.Class.ShortName}
	/if (${Bool[${AANow1}]} || ${Bool[${AANow2}]}) /call check_AA_now
	/if (${Bool[${DiscNow1}]} || ${Bool[${DiscNow2}]}) /call check_Disc_now
	/if (${Bool[${Aura1Spell}]} || ${Bool[${Aura2Spell}]}) /call check_aura
	
	/if (${Bool[${_tmpSpell}]}) {
		/call mem_spell TRUE "${_tmpSpell}" ${buffGem} FALSE	
		/varset _tmpSpell FALSE
	}	
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
 *	DES: use primary anchor.
 *	USE: /primary
 *	INI: 
 *  NOTE:
 ***| 
#bind click_primary_anchor /primary
sub Bind_click_primary_anchor()
	/if (${Me.ItemReady[Primary Anchor Transport Device]}) {
		/if (${Me.Invis}) /makemevisible
		/if (!${switchPersistentModes}) /call switch_clear_mode
		/call set_stop NOECHO
		/call clear_combat
		/if (${validate_cast[FALSE, item, "Primary Anchor Transport Device", ${Me.ID}]}) /call core_cast2 "Primary Anchor Transport Device" item 0 FALSE
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
		/if (!${switchPersistentModes}) /call switch_clear_mode
		/call set_stop NOECHO
		/call clear_combat
		/if (${validate_cast[FALSE, item, "Secondary Anchor Transport Device", ${Me.ID}]}) /call core_cast2 "Secondary Anchor Transport Device" item 0 FALSE
	}
/return



|***
 *	DES: will use which ever anchor is available
 *	USE: /anchor
 *	INI: 
 *  NOTE:
 ***| 
#bind anchor /anchor
sub Bind_anchor()
	/if (${broadcastAnchor}) {
		/if (${setCommunications.Equal[eqbc]}) {
			/bca //prisec_anchor
		} else /if (${setCommunications.Equal[dannet]}) {
			/dgexecute /prisec_anchor
		}
	} 
	/call Bind_prisec_anchor
/return

|***
 *	DES: sub for /anchor
 ***| 
#bind prisec_anchor /prisec_anchor
sub Bind_prisec_anchor()
	/if (${Me.ItemReady[Primary Anchor Transport Device]}) {
		/call Bind_click_primary_anchor
		/return
	} else /if (${Me.ItemReady[Secondary Anchor Transport Device]}) {
		/call Bind_click_secondary_anchor
		/return			
	}

/return



|***
 *	DES: use Drunkard's Stein
 *	USE: /stein
 *	INI: 
 *  NOTE:
 ***| 
#bind click_drunkards_stein /stein
sub Bind_click_drunkards_stein()
	/if (!${Me.ItemReady[Drunkard's Stein]}) /return
		/if (${Me.Invis}) /makemevisible
		/if (!${switchPersistentModes}) /call switch_clear_mode
		/call set_stop NOECHO
		/call clear_combat
	/if (${validate_cast[FALSE, item, "Drunkard's Stein", ${Me.ID}]}) /call core_cast2 "Drunkard's Stein" item 0 FALSE
/return



|***
 *	DES: use Mirror Fragment of Anashti Sul
 *	USE: /mirror
 *	INI: 
 *  NOTE:
 ***| 
#bind click_mirror_fragment /mirror
sub Bind_click_mirror_fragment()
	/if (!${Me.ItemReady[Mirror Fragment of Anashti Sul]}) /return
		/if (${Me.Invis}) /makemevisible
		/if (!${switchPersistentModes}) /call switch_clear_mode
		/call set_stop NOECHO
		/call clear_combat
	/if (${validate_cast[FALSE, item, "Mirror Fragment of Anashti Sul", ${Me.ID}]}) /call core_cast2 "Mirror Fragment of Anashti Sul" item 0 FALSE
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
sub Bind_override(string _overrideType, string _verbage)
	
	/declare _count		int local
	/declare _bind string local override
	/declare _listout string local
	/declare _sep bool local FALSE
		
	| override class checks. not documented.
	/if (${_overrideType.Equal[classcheck]}) {
		/varset switchOverrideClasscheck TRUE
		OUT ${owarning} Overriding class checks ${sep} ur gonna break shit...
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

	/if (${switchMQclearonCLS}) /mqclear
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

	/if (${switchMQclearonCLS}) /mqclear
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
	
	/declare _count		int local
	/declare _bind 		string local heal
	/declare _listout string local
	/declare _sep 		bool local

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

	/if (${switchMQclearonCLS}) /mqclear
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
	| group
	} else /if (${_buffType.Equal[group]}) {
		/call writeINI switchBuffGroup Buffs ${If[${switchBuffGroup},FALSE,TRUE]}
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
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax		
		/if (${${_bind}Type.Arg[${_count},|].Equal[pct]}) /varset _listout ${_listout}(${cnum}${minRezPct}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[range]}) /varset _listout ${_listout}(${cnum}${maxRezRange}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[combat]}) /varset _listout ${_listout}(${If[${switchRezCombat},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[emergency]}) /varset _listout ${_listout}(${If[${switchRezEmergency},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[everyone]}) /varset _listout ${_listout}(${If[${switchRezEveryone},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[give]}) /varset _listout ${_listout}(${If[${switchRezGive},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[take]}) /varset _listout ${_listout}(${If[${switchRezTake},${oon},${ooff}]})
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

	| require a target. one that is not a PC
	/if ((!${Target.ID} || ${Bool[${Spawn[pc ID ${Target.ID}]}]}) && ${_list.NotEqual[safe]} && ${_list.NotEqual[reset]}) {
		/call echos needtarget 0 0 0 0
		/return
	} 

	| make sure we are using the correct zone file
	/call validate_zone FALSE TRUE
	
	| disable all commands but safezone while in a safezone
	/if (${SafeZone} && ${_list.NotEqual[safe]} && ${_list.NotEqual[reset]}) {
		VOUT ${onotice} ${sep} List Control is disabled while in a Safe Zone.
		/return
	}

	| validate _list and correct the case
	/if (${Bool[${_list}]} && ${_list.NotEqual[safe]} && ${_list.NotEqual[reset]}) {
		/for _count 1 to ${listType.Count[|]}
			
			/if (${Bool[${_list.Find[${listType.Arg[${_count},|]}]}]}) {
				/if (${_list.Left[2].Equal[no]}) {
					/varset _list ${_list.Left[2].Left[1].Upper}${_list.Left[2].Right[-1].Lower}${_list.Right[-2].Left[1].Upper}${_list.Right[-2].Right[-1].Lower}
				}	else /if (${_list.Left[4].Equal[hunt]}) {
					/varset _list ${_list.Left[4].Left[1].Upper}${_list.Left[4].Right[-1].Lower}${_list.Right[3].Left[1].Upper}${_list.Right[3].Right[-1].Lower}
				} else /if (${_list.Left[2].NotEqual[no]}) {
					/varset _list ${_list.Left[1].Upper}${_list.Right[-1].Lower}
				}
				/varset _validName TRUE
				/goto :validname
			}	
		/next _count
		/if (!${_validName}) /call echos listtype lc list 0 0
	} else /if (!${Bool[${_list}]} && ${_list.NotEqual[safe]} && ${_list.NotEqual[reset]}) {
		/call echos listtype lc list 0 0
	}

	:validname
	/if (${_list.Equal[safe]}) {
		/if (${Ini[${INIZone},"${Zone.ShortName}","SafeZone"]}) {
			/ini "${INIZone}" "${Zone.ShortName}" "SafeZone" "FALSE"
		} else /if (!${Ini[${INIZone},"${Zone.ShortName}","SafeZone"]}) {
			/ini "${INIZone}" "${Zone.ShortName}" "SafeZone" "TRUE"
		}
		/varset SafeZone ${Ini[${INIZone},"${Zone.ShortName}","SafeZone"]}
		VOUT /${_bind} ${_list} ${sep} ${If[${SafeZone},${oenabled},${odisabled}]}
		/varset _listchanged TRUE
	} else /if (${_list.Equal[reset]}) {
		VOUT /${_bind} ${_list} ${sep} ${cinfo}${Zone.Name}\ax
		/varset _listchanged TRUE
		| /call check_zone_INI ${Me.DisplayName}
	} else /if (${_list.Equal[remove]}) {
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
		/if (${${_list}List.Find[${Target.DisplayName}]}) {
			VOUT ${onotice} ${sep} [${ctar}${Target.DisplayName}\ax] already exists in ${sep} ${cinfo}${Zone.Name}\ax
		} else /if (!${${_list}List.Find[${Target.DisplayName}]}) {
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
		/if (${setCommunication.Equal[eqbc]}) {
			/bcaa //core zonereset			
		} else /if (${setCommunication.Equal[dannet]}) {
			/dgaexecute /core zonereset
		} else {
			/core zonereset
		}
	}
/return



|***
 *	DES: pet control
 *	USE: /pc []
 *	INI: mostly [pet] entries  from the toons INI
 *  NOTE:
 ***|
#bind petControl /pc
sub Bind_petControl(string _pcType, string _verbage)

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
		/if (${Me.Pet.ID}) /call check_buffs_pet
		/return

	| pet/engage#
	} else /if (${Bool[${_pcType.Equal[pet]}]}) {
		/if (!${validate_class[TRUE, ${PetList}]}) /return
		/if (!${Bool[${_verbage}]}) {
			/call writeINI switchPcPet Pet ${If[${switchPcPet},FALSE,TRUE]}
			/if (!${switchPcPet} && ${Pet.ID}) {
				/if (${Me.AltAbilityReady[Suspended Minion]}) {
					/if (${validate_cast[FALSE, alt, "Suspended Minion", ${Me.ID}]}) /call core_cast2 "Suspended Minion" alt 0 FALSE
				} else /if (${Me.AltAbilityReady[Companion's Suspension]}) {
					/if (${validate_cast[FALSE, alt, "Companion's Suspension", ${Me.ID}]}) /call core_cast2 "Companion's Suspension" alt 0 FALSE
				} else {
					/pet leave
				}
			}
		} else /if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI PcPetengageat Pet ${_verbage}
		} else /if (!${Bool[${_pcNum}]} || !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} pet(${cnum}${PcPetengageat}\ax) valid range ${cinfo}1 - 99\ax
			/return
		}
		
	| swarm/engage#
	} else /if (${Bool[${_pcType.Equal[swarm]}]}) {
		/if (!${Bool[${_verbage}]}) {
			/call writeINI switchPcSwarm Pet ${If[${switchPcSwarm},FALSE,TRUE]}
		} else /if (${Bool[${_verbage}]} && ${Range.Between[1,99:${Int[${_verbage}]}]}) {			
			/call writeINI PcSwarmengageat Pet ${_verbage}
		} else /if (!${Bool[${_verbage}]} || !${Range.Between[1,99:${Int[${_verbage}]}]}) {
			VOUT /${_bind} swarm(${cnum}${PcSwarmengageat}\ax) valid range ${cinfo}1 - 99\ax
			/return
		}
		
	| pc tank  This is not working atm. sorry...
	} else /if (${Bool[${_pcType.Equal[tank]}]}) {
		/return
		/if (!${validate_class[TRUE, ${PetList}]}) /return
		/call writeINI switchPcPettank Pet ${If[${switchPcPettank},FALSE,TRUE]}
		/call echos switch "Pet Tank" switchPcPettank	0 0
		/return
		
	| pc type
	} else /if (${Bool[${_pcType.Equal[type]}]}) {
		/if (!${validate_class[TRUE, ${PetList}]}) /return
		/call writeINI PcPettype Pet ${_verbage}
		
	| pc bag ##
	} else /if (${Bool[${_pcType.Equal[bag]}]}) {
		/if (!${validate_class[TRUE, ${PetList}]}) /return
		/if (${Range.Between[1,10:${Int[${_verbage}]}]}) {
			/call writeINI PetBagSlotNumber Pet ${_verbage}
		} else {
			VOUT /${_bind} bag(${cnum}${PetBagSlotNumber}\ax] valid range range ${cinfo}1 - 10\ax
			/return
		}

	}
	
	/if (${switchMQclearonCLS}) /mqclear
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax		
		/if (${${_bind}Type.Arg[${_count},|].Equal[pet]}) /varset _listout ${_listout}(${If[${switchPcPet},${cnum}${PcPetengageat}\ax,${cbad}${PcPetengageat}\ax]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[swarm]}) /varset _listout ${_listout}(${If[${switchPcSwarm},${cnum}${PcSwarmengageat}\ax,${cbad}${PcSwarmengageat}\ax]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[type]}) /varset _listout ${_listout}(${If[${Bool[${PcPettype}]},${cnum}${PcPettype}\ax,${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[bag]}) /varset _listout ${_listout}(${If[${Bool[${PetBagSlotNumber}]},${cnum}${PetBagSlotNumber}\ax,${ooff}]})
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return



|***
 *	DES: sets controlls for /tie
 *	USE: /tc [name|time|distance]
 *	INI: 
 *  NOTE:
 ***| 
#bind setTieControl /tc
sub Bind_setTieControl(string _tcType, string _verbage)
	
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
	/if (${switchMQclearonCLS}) /mqclear
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax		
		/if (${${_bind}Type.Arg[${_count},|].Equal[name]}) /varset _listout ${_listout}(${cnum}${If[${Bool[${SetTieToon}]},${SetTieToon},${ooff}]}\ax)
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

	| Mob count #
	} else /if (${Bool[${_burnType.Equal[count]}]}) {
	 	/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI burnCount Common ${_verbage} 
		} else {
			VOUT /${_bind} count(${cnum}${burnCount}\ax] when # of mobs in range ${cinfo}1 - 99\ax
			/return
		}

	| Burn Engage at %
	} else /if (${Bool[${_burnType.Equal[engageat]}]}) {
	 	/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI burnEngageat Common ${_verbage} 
		} else {
			VOUT /${_bind} engageat(${cnum}${burnEngageat}\ax] % of mobs health to engage burn ${cinfo}1 - 99\ax
			/return
		}

	| force constant burning
	} else /if (${Bool[${_burnType.Equal[force]}]}) {
		/varset burnForce ${If[${burnForce},FALSE,TRUE]}

	} 
	
	/if (${switchMQclearonCLS}) /mqclear
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
		/if (${${_bind}Type.Arg[${_count},|].Equal[auto]}) /varset _listout ${_listout}(${cnum}${If[${Bool[${burnAuto}]},${oon},${ooff}]}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[force]}) /varset _listout ${_listout}(${cnum}${If[${Bool[${burnForce}]},${oon},${ooff}]}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[engageat]}) /varset _listout ${_listout}(${cnum}${burnEngageat}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[count]}) /varset _listout ${_listout}(${cnum}${burnCount}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return



|***
 *	DES: core controls
 *	USE: /core [] [TEXT]
 *	INI: 
 *  NOTE:
 ***|
#bind setCore /core
sub Bind_setCore(string _coreType, string _verbage)

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
		/varset combat ${If[${coreAuto},COMBAT,FALSE]}  
		/if (!${coreAuto}) { 
			/call set_stop NOECHO
			/call clear_combat
		}
		VOUT /${_bind} auto ${sep} ${If[${coreAuto},${oenabled},${odisabled}]}
		/return
		
	| check in game cash
	} else /if (${Bool[${_coreType.Equal[checkcash]}]}) {
		/if (${Window[MKPW_ClaimWindow].Child[MKPW_ClaimClickHereBtn].Text.EqualCS[Claimed]}) {
			VOUT No cash to collect, current: ${cnum}${Window[MarketplaceWnd].Child[MKPW_AvailableFundsUpper].Text}\ax
			/return
		} else {
			/call check_free_ingame_cash
		}		

	| enable/disable obscure target names ## not documented
	} else /if (${Bool[${_coreType.Equal[obscure]}]}) {
		/varset coreObscure ${If[${coreObscure},FALSE,TRUE]}
		VOUT /${_bind} obscure ${sep} ${If[${coreObscure},${oenabled},${odisabled}]}
		/return

	| enable/disable using events.inc file
	} else /if (${Bool[${_coreType.Equal[events]}]}) {
		/call writeINI coreEventsInc Common ${If[${coreEventsInc},FALSE,TRUE]}
		| VOUT /${_bind} events ${sep} ${If[${coreEventsInc},${oenabled},${odisabled}]}

	| set the kill code
	} else /if (${Bool[${_coreType.Equal[killcode]}]}) {
		/if (${Bool[${_verbage}]}) {
			/call DeclareCoreIniVar NA remoteKillKey General ${_verbage} 
			VOUT /${_bind} ${cinfo}killcode\ax ${sep} ${_verbage}
		} else {
			VOUT /${_bind} killcode ${sep} (${cnum}${remoteKillKey}\ax)
		}
		/return

	| runs the whitewash routine to clear all aliases
	} else /if (${Bool[${_coreType.Equal[whitewash]}]}) {
		/call whitewash
		/return

	| set rest %		
	} else /if (${Bool[${_coreType.Equal[rest]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI autoRest Common ${_verbage}  
		} else /if (${Range.Between[0,0:${Int[${_verbage}]}]}) {
			/call writeINI autoRest Common 0  
		}

	| reset core
	} else /if (${Bool[${_coreType.Equal[reset]}]}) {
		/call initialize_core ${_verbage}
		/return
		
	| reset zone load INI
	} else /if (${Bool[${_coreType.Equal[zonereset]}]}) {
		/call validate_zone FALSE TRUE
		/if (${switchShrink}) {
			/if (${coreAuto}) /call check_shrink
		}		
		/return
		
	| set control toon
	} else /if (${Bool[${_coreType.Equal[control]}]}) {
		/if (${Bool[${_verbage}]}) {
			/call writeINI toonControl Common ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		} else /if (!${Bool[${_verbage}]}) {
			/call writeINI toonControl Common FALSE
		}
		VOUT /${_bind} control ${sep} ${If[${Bool[${toonControl}]},${cgood}${toonControl}\ax,${odisabled}]}
		/if (!${Bool[${toonControl}]}) /xtarget set ${toonControlXTslot} Autohater
		/return

	| enable wasiting fireworks for those bastards with too much AA on their hands
	} else /if (${Bool[${_coreType.Equal[fireworks]}]}) {
		/call writeINI switchAAFireworks Common ${If[${switchAAFireworks},FALSE,TRUE]}  
		/if (!${switchAAFireworks}) /call create_timer timer_Waste_Fireworks 1
		
	| take invites to raid/group
	} else /if (${Bool[${_coreType.Equal[invite]}]}) {
		/call writeINI switchInvites Common ${If[${switchInvites},FALSE,TRUE]}  

	| take raid invites
	} else /if (${Bool[${_coreType.Equal[takeraidinvite]}]}) {
		/call writeINI switchTakeRaidInvite Common ${If[${switchTakeRaidInvite},FALSE,TRUE]} 

	| take group invites
	} else /if (${Bool[${_coreType.Equal[takegroupinvite]}]}) {
		/call writeINI switchTakeGroupInvite Common ${If[${switchTakeGroupInvite},FALSE,TRUE]} 
	
	| invite people to your raid
	} else /if (${Bool[${_coreType.Equal[giveraidinvite]}]}) {
		/call writeINI switchGiveRaidInvite Common ${If[${switchGiveRaidInvite},FALSE,TRUE]} 

	| exp max level
	} else /if (${Bool[${_coreType.Equal[expmaintain]}]}) {
		/if (${Range.Between[1,99:${Int[${_verbage}]}]}) {
			/call writeINI MaintExpLvl Common ${_verbage} 
		} else {
			VOUT /${_bind} expmaintain(${cnum}${MaintExpLvl}\ax) allowable range ${cinfo}1 - 99\ax
			/return
		}	

	| exp maintain level
	} else /if (${Bool[${_coreType.Equal[explevel]}]}) {
		/if (${Range.Between[1,110:${Int[${_verbage}]}]}) {
			/call writeINI MaxLevel Common ${_verbage} 
		} else {
			VOUT /${_bind} explevel(${cnum}${MaxLevel}\ax) allowable range ${cinfo}1 - 110\ax
			/return
		}	

	| exp auto adjust
 	} else /if (${Bool[${_coreType.Equal[expadjust]}]}) {
		/call writeINI AutoExpAdjust Common ${If[${AutoExpAdjust},FALSE,TRUE]} 
		VOUT /${_bind} expadjust ${sep} ${If[${AutoExpAdjust},${oenabled},${odisabled}]}

	| delete old INI entries
 	} else /if (${Bool[${_coreType.Find[fixini]}]}) {
		/call fix_INI_declares ${_verbage}
		/return
	}

	/if (${switchMQclearonCLS}) /mqclear	
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
		/if (${${_bind}Type.Arg[${_count},|].Equal[explevel]}) /varset _listout ${_listout}(${cnum}${MaxLevel}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[expmaintain]}) /varset _listout ${_listout}(${cnum}${MaintExpLvl}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[rest]}) /varset _listout ${_listout}(${If[${autoRest},${cnum}${autoRest}\ax,${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[expadjust]}) /varset _listout ${_listout}(${If[${AutoExpAdjust},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[giveraidinvite]}) /varset _listout ${_listout}(${If[${switchGiveRaidInvite},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[takeraidinvite]}) /varset _listout ${_listout}(${If[${switchTakeRaidInvite},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[takegroupinvite]}) /varset _listout ${_listout}(${If[${switchTakeGroupInvite},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[fireworks]}) /varset _listout ${_listout}(${If[${switchAAFireworks},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[events]}) /varset _listout ${_listout}(${If[${coreEventsInc},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[auto]}) /varset _listout ${_listout}(${If[${coreAuto},${oon},${ooff}]})
		
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return



|***
 *	DES: set on/off switches
 *	USE: /onoff [|horn|shrink]
 *	INI: NA
 *  NOTE: enable/disable many switches
 ***|
#bind onoff /onoff
sub Bind_onoff(string _onoffType)
	/if (${switchMQclearonCLS}) /mqclear
	
	/declare _count int local 0
	/declare _listout string local
	/declare _sep bool local FALSE
	/declare _bind string local onoff
	
	/if (${Bool[${${_bind}Type.Find[${_${_bind}Type}]}]}) {
		/if (${Defined[switch${_onoffType.Left[1].Upper}${_onoffType.Right[-1].Lower}]}) {
			/call writeINI switch${_${_bind}Type.Left[1].Upper}${_${_bind}Type.Right[-1].Lower} Common ${If[${switch${_${_bind}Type.Left[1].Upper}${_${_bind}Type.Right[-1].Lower}},FALSE,TRUE]}  
		} 
	}
	
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax
		/if (${${_bind}Type.Arg[${_count},|].Equal[horn]}) /varset _listout ${_listout}(${If[${switchHorn},${oon},${ooff}]})	
		/if (${${_bind}Type.Arg[${_count},|].Equal[shrink]}) /varset _listout ${_listout}(${If[${switchShrink},${oon},${ooff}]})

		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
/return  



|***
 *	DES: i dunno... information maybe?
 *	USE: /info
 *	INI: NA
 *  NOTE: list all switches for help
 ***|
#bind info /info
sub Bind_info()
	/if (${switchMQclearonCLS}) /mqclear
	
	/declare _listout string local
	/declare _sep bool local
	/declare _count int local
	/declare _controls string local |buff|burn|campfire|cc|comm|core|cursor|debuff|echos|grab|hc|heal|lc|mode|movement|onoff|override|pc|rez|setassist|tc|tie|ts|${Me.Class.ShortName}
	
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
		/if (${switchMQclearonCLS}) /mqclear

		/for _count 1 to ${${_bind}Type.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax		
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
	} 
/return



|***
 *	DES: sets the assist
 *	USE: /setassist [X|G|1|2|3|F|MA1|MA2|NULL]
 *	INI: 
 *  NOTE: 
 ***| 	
#bind setAssist /setassist
sub Bind_setAssist(string _verbage, _name)
	/if (${switchMQclearonCLS}) /mqclear

	/declare _count int local 0
	/declare _bind string local setassist
	/declare _listout string local
	/declare _sep bool local FALSE
	/declare _setassistType string local |x|g|1|2|3|ma1|ma2|f

	| get help
	/if (${_setassistType.Find[${_verbage}]} && ${_name.Equal[info]}) {
		VOUT /setassist ${cinfo}${_verbage.Lower}\ax ${sep} ${Ini[${INIWiki},${_bind},${_verbage.Lower}]}
		/return
	}	else /if (${_verbage.Equal[info]}) {
		/for _count 1 to ${_setassistType.Count[|]}
			VOUT /setassist ${cinfo}${_setassistType.Arg[${_count},|]}\ax ${sep} ${Ini[${INIWiki},${_bind},${_setassistType.Arg[${_count},|]}]}
		/next _count
		/return
	}

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
	
	/declare _count int local 0
	/declare _bind string local cc
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

	| enable / disable forced assist
	/if  (${Bool[${_ccType.Equal[forceassist]}]}) {
		/call validate_assist FALSE force
		VOUT /${_bind} forceassist ${sep} ${If[${useForceAssist},${oenabled},${odisabled}]}

	| force manual combat routines
	} else /if (${Bool[${_ccType.Equal[force]}]}) {
		/if (${Bool[${_verbage}]}) {
			/if (${Target.ID} && (${Target.ID} != ${Me.ID}) && (${Target.ID} != ${Me.Pet.ID})) {

				| engage base combat
				/if (${Bool[${_verbage.Equal[combat]}]}) {
					/varset isValidTarget TRUE
					/varset combat COMBAT
					/varset Target2Kill ${Target.ID}
					OUT /${_bind} force combat ${sep} [${ctar}${Target.DisplayName}\ax]

				| force assist for other peer toons
				} else /if (${Bool[${_verbage.Equal[assist]}]}) {
					/if (${setCommunications.Equal[eqbc]}) {
						/bca //varset Target2Kill ${Target.ID}						
					} else /if (${setCommunications.Equal[dannet]}) {
						/dgexecute /varset Target2Kill ${Target.ID}						
					}
					OUT /${_bind} force assist ${sep} [${ctar}${Target.DisplayName}\ax]
				}
			}
			 
			| echo options
			} else {
				/declare _tmpVariable string local |combat
				/for _count 1 to ${_tmpVariable.Count[|]}
					/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_tmpVariable.Arg[${_count},|]}\ax
					/varset _sep TRUE
				/next _count
				VOUT /${_bind} force [${_listout}]	
			}
			/return

	| set bandolier swapping
	} else /if (${Bool[${_ccType.Equal[bandolier]}]}) {
		/call writeINI switchBandolier meleedisc ${If[${switchBandolier},FALSE,TRUE]}
		VOUT /${_bind} bandolier ${sep} ${cinfo}${combat}\ax
		/return

	| set stop nuke %
	} else /if (${Bool[${_ccType.Equal[stopnuke]}]}) {
		/if (${Range.Between[0,99:${Int[${_verbage}]}]}) {
			/call writeINI stopnukeAt Spells ${_verbage} 
		} else {
			VOUT /${_bind} stopnuke(${cnum}${stopnukeAt}\ax) allowable range is ${cinfo}0 - 99\ax
		} 		
		/return
		
	| set circle of to be used
	} else /if (${Bool[${_ccType.Equal[circle]}]}) {
		/if (${Bool[${_verbage.Equal[power]}]} || ${Bool[${_verbage.Equal[life]}]} || ${Bool[${_verbage.Equal[mana]}]}) {
			/call writeINI useCircle Items ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
			VOUT /${_bind} circle ${sep} ${cinfo}${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}\ax
		} else /if (!${Bool[${_verbage}]}) {
			VOUT /${_bind} circle allowable options [${cinfo}power, life, mana\ax] 
		} 
		/return

	| set Pull
	} else /if (${Bool[${_ccType.Equal[pull]}]}) {
		/call writeINI switchPull Common ${If[${switchPull},FALSE,TRUE]}
		VOUT /${_bind} pull ${sep} ${If[${switchPull},${oenabled},${odisabled}]}
		/if (${switchPull}) /call validate_assist NOECHO x
		/return
		
	| set Off Tank
	} else /if (${Bool[${_ccType.Equal[offtank]}]}) {
		/call writeINI switchOffTank Common ${If[${switchOffTank},FALSE,TRUE]}
		VOUT /${_bind} offtank ${sep} ${If[${switchOffTank},${oenabled},${odisabled}]}
		/return
		
	| delay in nuking	
	} else /if (${Bool[${_ccType.Equal[nukedelay]}]}) {
		/if (${Range.Between[1,100:${Int[${_verbage}]}]}) {
			/call writeINI tNuke_Delay Timer ${_verbage} 
			VOUT /${_bind} nukedelay ${sep} ${cinfo}${_verbage}\ax
		} else {
			VOUT /${_bind} nukedelay(${cnum}${tNuke_Delay}\ax) allowable range is ${cinfo}1 - 100\ax
		} 	
		/return
		
	| force fade to rest
	} else /if (${Bool[${_ccType.Equal[faderest]}]}) {
		/if (!${validate_class[TRUE, |WAR|PAL|SHD|ROG|BER|RNG|MON|BST]}) /return
		/call writeINI switchForceFadeRest Common ${If[${switchForceFadeRest},FALSE,TRUE]}

	| agro/agro range ##
	} else /if (${Bool[${_ccType.Equal[agro]}]}) {
		/if (${Range.Between[1,400:${Int[${_verbage}]}]}) {
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
		/return


	|** set out of group Manual Assist 1 or 2
	} else /if (${Bool[${_ccType.Equal[ma1]}]}) {
		/call validate_assist FALSE ma1 ${_verbage}
		VOUT /${_bind} ma1 ${sep} ${If[${Bool[${MA1}]},${cinfo}${MA1}\ax,${odisabled}]}
		/return
		
	} else /if (${Bool[${_ccType.Equal[ma2]}]}) {		
		/call validate_assist FALSE ma2 ${_verbage}
		VOUT /${_bind} ma2 ${sep} ${If[${Bool[${MA2}]},${cinfo}${MA2}\ax,${odisabled}]}	
		/return
	**|

	| MA1/2 engage at %
	} else /if (${Bool[${_ccType.Equal[engage]}]}) {
		/if (${Range.Between[1,100:${Int[${_verbage}]}]}) {
			/call writeINI engageAt Common ${_verbage}  
			VOUT /${_bind} engage ${sep} ${cinfo}${engageAt}\ax
		} else {
			VOUT /${_bind} engage(${cnum}${engageAt}\ax) allowable range ${cinfo}1 - 99\ax
		}
		/return

	| Smart Assist engage at %
	} else /if (${Bool[${_ccType.Equal[smartengage]}]}) {
		/if (!${validate_expansion[TRUE, |16]}) /return
		/if (${Range.Between[1,100:${Int[${_verbage}]}]}) {
			/call writeINI smartengageAt Common ${_verbage}  
			VOUT /${_bind} smartengage ${sep} ${cinfo}${smartengageAt}\ax
		} else {
			VOUT /${_bind} smartengage(${cnum}${smartengageAt}\ax) allowable range ${cinfo}1 - 99\ax
		}
		/return

	| set melee combat distance to target
	} else /if (${Bool[${_ccType.Equal[combatdistance]}]}) {
		/if (${Range.Between[1,50:${Int[${_verbage}]}]}) {
			/call writeINI CombatDistance Movement ${_verbage}
			VOUT /${_bind} combatdistance ${sep} ${cinfo}${_verbage}\ax
		} else {
			VOUT /${_bind} combatdistance(${cnum}${CombatDistance}\ax] minimum range to melee mob ${cinfo}1 - 50\ax
		}
		/return

	| set fake safe radius
	} else /if (${Bool[${_ccType.Equal[saferadius]}]}) {
		/if (${Range.Between[1,50:${Int[${_verbage}]}]}) {
			/call writeINI combatRadius Common ${_verbage}
			VOUT /${_bind} saferadius ${sep} ${cinfo}${combatRadius}\ax
		} else {
			VOUT /${_bind} saferadius(${cnum}${combatRadius}\ax] allowable radius ${cinfo}1 - 50\ax
		}
		/return
	
	| set Z radius
	} else /if (${Bool[${_ccType.Equal[zradius]}]}) {
		/if (${Range.Between[1,200:${Int[${_verbage}]}]}) {
			/call writeINI zradius Common ${_verbage} 
		} else {
			VOUT /${_bind} zradius(${cnum}${zradius}\ax) allowable range ${cinfo}1 - 200\ax
			/return
		}	
		
	| alliance on or off... yada yada		
	} else /if (${Bool[${_ccType.Equal[alliance]}]}) {
		/call writeINI switchAlliance Spells ${If[${switchAlliance},FALSE,TRUE]}

	| enable combat: melee, range, nuke, dot
	} else /if (${Select[${_ccType},melee,range,nuke,dot]}) {

		| enable combat: melee
		/if (${Bool[${_ccType.Equal[melee]}]}) {
			/call writeINI switchCombatMelee Common ${If[${switchCombatMelee},FALSE,TRUE]}

		| enable combat: range
		} else /if (${Bool[${_ccType.Equal[range]}]}) {
			/call writeINI switchCombatRange Common ${If[${switchCombatRange},FALSE,TRUE]}

		| enable combat: nuke
		} else /if (${Bool[${_ccType.Equal[nuke]}]}) {
			/call writeINI switchCombatNuke Common ${If[${switchCombatNuke},FALSE,TRUE]}

		| enable combat: dot
		} else /if (${Bool[${_ccType.Equal[dot]}]}) {
			/call writeINI switchCombatDoT Common ${If[${switchCombatDoT},FALSE,TRUE]}
		}

		/for _count 1 to ${${_bind}Type.Count[|]}
			/if (${${_bind}Type.Arg[${_count},|].Equal[melee]}) /varset _listout ${_listout} ${cinfo}melee\ax(${If[${switchCombatMelee},${oon},${ooff}]})	
			/if (${${_bind}Type.Arg[${_count},|].Equal[range]}) /varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}range\ax(${If[${switchCombatRange},${oon},${ooff}]})	
			/if (${${_bind}Type.Arg[${_count},|].Equal[nuke]}) /varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}nuke\ax(${If[${switchCombatNuke},${oon},${ooff}]})	
			/if (${${_bind}Type.Arg[${_count},|].Equal[dot]}) /varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}dot\ax(${If[${switchCombatDoT},${oon},${ooff}]})	
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} [${_listout}]
		/return
		
	| melee discs
	} else /if (${Bool[${_ccType.Equal[meleedisc]}]}) {
		/if (${Bool[${_verbage.Equal[kick]}]} && ${Me.Skill[kick]}) /call writeINI kick meleedisc ${If[${kick},FALSE,TRUE]}
		/if (${Bool[${_verbage.Equal[bash]}]} && ${Me.Skill[bash]}) /call writeINI bash meleedisc ${If[${bash},FALSE,TRUE]}
		/if (${Bool[${_verbage.Equal[taunt]}]} && ${Me.Skill[taunt]}) /call writeINI taunt meleedisc ${If[${taunt},FALSE,TRUE]}
		/if (${Bool[${_verbage.Equal[disarm]}]} && ${Me.Skill[disarm]}) /call writeINI disarm meleedisc ${If[${disarm},FALSE,TRUE]}
		/if (${Bool[${_verbage.Equal[intimidation]}]} && ${Me.Skill[intimidation]}) /call writeINI intimidation meleedisc ${If[${intimidation},FALSE,TRUE]}
		/if (${Bool[${_verbage.Equal[backstab]}]} && ${Me.Skill[backstab]}) /call writeINI backstab meleedisc ${If[${backstab},FALSE,TRUE]}
		/if (${Bool[${_verbage.Equal[frenzy]}]} && ${Me.Skill[frenzy]}) /call writeINI frenzy meleedisc ${If[${frenzy},FALSE,TRUE]}
		/if (${Bool[${_verbage.Equal[dragonpunch]}]} && ${Me.Skill[dragon punch]}) /call writeINI dragonpunch meleedisc ${If[${dragonpunch},FALSE,TRUE]}
		/if (${Bool[${_verbage.Equal[eaglestrike]}]} && ${Me.Skill[eagle strike]}) /call writeINI eaglestrike meleedisc ${If[${eaglestrike},FALSE,TRUE]}
		/if (${Bool[${_verbage.Equal[tigerclaw]}]} && ${Me.Skill[tiger claw]}) /call writeINI tigerclaw meleedisc ${If[${tigerclaw},FALSE,TRUE]}
		/if (${Bool[${_verbage.Equal[flyingkick]}]} && ${Me.Skill[flying kick]}) /call writeINI flyingkick meleedisc ${If[${flyingkick},FALSE,TRUE]}
		/if (${Bool[${_verbage.Equal[roundkick]}]} && ${Me.Skill[round kick]}) /call writeINI roundkick meleedisc ${If[${roundkick},FALSE,TRUE]}
		/if (${Bool[${_verbage.Equal[slam]}]} && ${Me.Skill[slam]}) /call writeINI slam meleedisc ${If[${slam},FALSE,TRUE]}


		/declare _meleeskill string local |backstab|bash|disarm|dragonpunch|eaglestrike|flyingkick|frenzy|intimidation|kick|roundkick|slam|taunt|tigerclaw
		/for _count 1 to ${_meleeskill.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_meleeskill.Arg[${_count},|]}\ax		
			/if (${_meleeskill.Arg[${_count},|].Equal[kick]} && ${Me.Skill[kick]}) /varset _listout ${_listout}(${If[${kick},${oon},${ooff}]})	
			/if (${_meleeskill.Arg[${_count},|].Equal[bash]} && ${Me.Skill[bash]}) /varset _listout ${_listout}(${If[${bash},${oon},${ooff}]})	
			/if (${_meleeskill.Arg[${_count},|].Equal[taunt]} && ${Me.Skill[taunt]}) /varset _listout ${_listout}(${If[${taunt},${oon},${ooff}]})	
			/if (${_meleeskill.Arg[${_count},|].Equal[disarm]} && ${Me.Skill[disarm]}) /varset _listout ${_listout}(${If[${disarm},${oon},${ooff}]})	
			/if (${_meleeskill.Arg[${_count},|].Equal[intimidation]} && ${Me.Skill[intimidation]}) /varset _listout ${_listout}(${If[${intimidation},${oon},${ooff}]})	
			/if (${_meleeskill.Arg[${_count},|].Equal[frenzy]} && ${Me.Skill[frenzy]}) /varset _listout ${_listout}(${If[${frenzy},${oon},${ooff}]})	
			/if (${_meleeskill.Arg[${_count},|].Equal[backstab]} && ${Me.Skill[backstab]}) /varset _listout ${_listout}(${If[${backstab},${oon},${ooff}]})	
			/if (${_meleeskill.Arg[${_count},|].Equal[dragonpunch]} && ${Me.Skill[dragon punch]}) /varset _listout ${_listout}(${If[${dragonpunch},${oon},${ooff}]})	
			/if (${_meleeskill.Arg[${_count},|].Equal[eaglestrike]} && ${Me.Skill[eagle strike]}) /varset _listout ${_listout}(${If[${eaglestrike},${oon},${ooff}]})	
			/if (${_meleeskill.Arg[${_count},|].Equal[tigerclaw]} && ${Me.Skill[tiger claw]}) /varset _listout ${_listout}(${If[${tigerclaw},${oon},${ooff}]})	
			/if (${_meleeskill.Arg[${_count},|].Equal[flyingkick]} && ${Me.Skill[flyin gkick]}) /varset _listout ${_listout}(${If[${flyingkick},${oon},${ooff}]})	
			/if (${_meleeskill.Arg[${_count},|].Equal[roundkick]} && ${Me.Skill[round kick]}) /varset _listout ${_listout}(${If[${roundkick},${oon},${ooff}]})	
			/if (${_meleeskill.Arg[${_count},|].Equal[slam]} && ${Me.Skill[slam]}) /varset _listout ${_listout}(${If[${slam},${oon},${ooff}]})	
			/varset _sep TRUE
		/next _count
		VOUT /${_bind} meleedisc [${_listout}]
		/return
	}

	/if (${switchMQclearonCLS}) /mqclear
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax		
		/if (${${_bind}Type.Arg[${_count},|].Equal[zradius]}) /varset _listout ${_listout}(${cnum}${zradius}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[smartengage]}) /varset _listout ${_listout}(${cnum}${smartengageAt}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[engage]}) /varset _listout ${_listout}(${cnum}${engageAt}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[nukedelay]}) /varset _listout ${_listout}(${cnum}${tNuke_Delay}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[agro]}) /varset _listout ${_listout}(${If[${switchAgro},${cnum}${MobAgro}\ax,${cbad}${MobAgro}\ax]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[combatdistance]}) /varset _listout ${_listout}(${cnum}${CombatDistance}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[saferadius]}) /varset _listout ${_listout}(${cnum}${combatRadius}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[stopnuke]}) /varset _listout ${_listout}(${cnum}${stopnukeAt}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[circle]}) /varset _listout ${_listout}(${cnum}${useCircle}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[alliance]}) /varset _listout ${_listout}(${If[${switchAlliance},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[pull]}) /varset _listout ${_listout}(${If[${switchPull},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[MA1]}) /varset _listout ${_listout}(${If[${Bool[${MA1}]},${cnum}${MA1}\ax,${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[MA2]}) /varset _listout ${_listout}(${If[${Bool[${MA2}]},${cnum}${MA2}\ax,${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[faderest]}) /varset _listout ${_listout}(${If[${switchForceFadeRest},${oon},${ooff}]})	
		/if (${${_bind}Type.Arg[${_count},|].Equal[melee]}) /varset _listout ${_listout}(${If[${switchCombatMelee},${oon},${ooff}]})	
		/if (${${_bind}Type.Arg[${_count},|].Equal[range]}) /varset _listout ${_listout}(${If[${switchCombatRange},${oon},${ooff}]})	
		/if (${${_bind}Type.Arg[${_count},|].Equal[nuke]}) /varset _listout ${_listout}(${If[${switchCombatNuke},${oon},${ooff}]})	
		/if (${${_bind}Type.Arg[${_count},|].Equal[dot]}) /varset _listout ${_listout}(${If[${switchCombatDoT},${oon},${ooff}]})	
		/if (${${_bind}Type.Arg[${_count},|].Equal[offtank]}) /varset _listout ${_listout}(${If[${switchOffTank},${oon},${ooff}]})	
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
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
 *	DES: movement controls
 *	USE: /movement []
 *	INI: 
 *  NOTE:
 ***|
#bind setMovementControls /movement
sub Bind_setMovementControls(string _movementType, string _verbage)
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
				/dexecute ${_clist.Arg[${_count},|]} /nav spawn pc ${Me.DisplayName}
				
			}

		/next _count		
		/return
		
	| set leash range
	} else /if (${_${_bind}Type.Equal[leash]}) {
		/if (${Range.Between[1,200:${Int[${_verbage}]}]}) {
			/if (${HomeRadius} > ${_verbage}) {
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
			/if (${validate_cast[FALSE, item, "${mount}", ${Me.ID}]}) /call core_cast2 "${mount}" item 0 FALSE
		}
		
	}

	/if (${switchMQclearonCLS}) /mqclear
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax		
		/if (${${_bind}Type.Arg[${_count},|].Equal[fast]}) /varset _listout ${_listout}(${If[${SetFaceFast},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[behind]}) /varset _listout ${_listout}(${If[${SetStickBehind},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[home]}) /varset _listout ${_listout}(${If[${HomeSet},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[arc]}) /varset _listout ${_listout}(${cnum}${SetFaceArc}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[leash]}) /varset _listout ${_listout}(${cnum}${HomeLeash}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[radius]}) /varset _listout ${_listout}(${cnum}${HomeRadius}\ax)
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]
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
	| bail if now crey type listed
	/if (!${Bool[${_crewNumber}]}) /return
	

	/declare _count int local 0
	/declare _clist string local ${Ini[${INICore},Group,${_crewNumber}]}
	
	| disband raid if exists
	/if (${Raid.Members}) {
		/for _count 1 to ${_clist.Count[|]}
			/if (${Me.Name.Equal[${_clist.Arg[${_count},|]}]}) /continue
			/dexecute ${_clist.Arg[${_count},|]} /raiddisband
			/delay 2
		/next _count	
		/if (${Raid.Members}) /raiddisband
	}
	
	| drop group. we dont care if you are in one or not.
	/for _count 1 to ${_clist.Count[|]}
		/dexecute ${_clist.Arg[${_count},|]} /disband
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

	| set echo channel
	} else /if (${Bool[${_echoType.Equal[channel]}]}) {
		/if (${Range.Between[1,9:${Int[${_verbage}]}]}) {
			/call writeINI echoChannel Echo ${_verbage} 0 0
		} 
		VOUT /${_bind}s channel(${cnum}${echoChannel}\ax)

	| enable/disable verbose BC Echos
	} else /if (${Bool[${_echoType.Equal[verbose]}]}) {
		/call writeINI verbose Echo ${If[${verbose},FALSE,TRUE]}

	| enable/disable BC relay
	} else /if (${Bool[${_echoType.Equal[bc]}]}) {
		/call writeINI switchEchoBC Echo ${If[${switchEchoBC},FALSE,TRUE]}

	| enable/disable relay tells
	} else /if (${Bool[${_echoType.Equal[relay]}]}) {
		/if (${Bool[${_verbage}]}) {
			/call writeINI relayTells Echo ${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}
		} else /if (!${Bool[${_verbage}]}) {
			/call writeINI relayTells Echo FALSE
		}
	
	}
	
 	/if (${switchMQclearonCLS}) /mqclear
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax		
		/if (${${_bind}Type.Arg[${_count},|].Equal[channel]}) /varset _listout ${_listout}(${cnum}${echoChannel}\ax)
		/if (${${_bind}Type.Arg[${_count},|].Equal[bc]}) /varset _listout ${_listout}(${If[${switchEchoBC},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[relay]}) /varset _listout ${_listout}(${If[${switchEchoBC},${cinfo}${_verbage.Left[1].Upper}${_verbage.Right[-1].Lower}\ax,${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[verbose]}) /varset _listout ${_listout}(${If[${verbose},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[alt]}) /varset _listout ${_listout}(${If[${switchEchoAlt},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[spell]}) /varset _listout ${_listout}(${If[${switchEchoSpell},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[disc]}) /varset _listout ${_listout}(${If[${switchEchoDisc},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[item]}) /varset _listout ${_listout}(${If[${switchEchoItem},${oon},${ooff}]})
		/if (${${_bind}Type.Arg[${_count},|].Equal[melee]}) /varset _listout ${_listout}(${If[${switchEchoMelee},${oon},${ooff}]})
		/varset _sep TRUE
	/next _count
	VOUT /${_bind}s [${_listout}]
/return



|***
 *	DES: random insult
 *	USE: /oof
 *	INI: 
 *  NOTE: not documented. why should i share everything!?!
 ***|
#bind oof /oof
sub Bind_oof()
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
 	/em looks around for the biggest fucking rock to hit ${_pronoun} head against.
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
  /if (${validate_cast[FALSE, alt, "Lesson of the Devoted", ${Me.ID}]}) /call core_cast2 "Lesson of the Devoted" alt 0 FALSE
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
 *	DES: navigates to the name/target
 *	USE: /navto [NAME]
 *	INI: 
 *  NOTE: 
 ***|
#bind nav_to_target /navto
sub Bind_nav_to_target(string _name)
	/call Bind_setMovementControls goto ${_name}
/return



|***
 *	DES: nclicks yes on a window option for some form of travel
 *	USE: /take
 *	INI: 
 *  NOTE: 
 ***|
#bind broadcast_take /take
sub Bind_broadcast_take()
	/if (${broadcastTake}) {
		/if (${setCommunications.Equal[eqbc]}) {
			/bca //click_window_option
		} else /if (${setCommunications.Equal[dannet]}) {
			/dgexecute /click_window_option
		}
	}
	/call Bind_click_window_option
/return

|***
 *	DES: take window option sub
 ***|
#bind click_window_option /click_window_option
sub Bind_click_window_option()

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
/return



|***
 *	DES: nclicks yes on a window option for some form of travel
 *	USE: /invis
 *	INI: 
 *  NOTE: 
 ***|
#bind broadcast_invis /invis
sub Bind_broadcast_invis()
	/if (${setCommunications.Equal[eqbc]}) {
		/bca //cast_AA_invis
	} else /if (${setCommunications.Equal[dannet]}) {
		/dgexecute /cast_AA_invis
	}	
	/call Bind_cast_AA_invis
/return

|***
 *	DES: sub for invis command
 ***| 
#bind cast_AA_invis /cast_AA_invis
sub Bind_cast_AA_invis()

	| fucking bards
	/if (${Me.AltAbilityReady[Shauri's Sonorous Clouding]} && ${Select[${Me.Class.ShortName},BRD]}) {
		/call pulse_stop
		/call MQ2Cast "Shauri's Sonorous Clouding" alt -targetid|${Me.ID}
		ECHOCHANNEL AA: Shauri's Sonorous Clouding
		/return
		
	| Invis vs Normies  casters	
	} else /if (${Me.AltAbilityReady[Group Perfected Invisibility]} && ${Select[${Me.Class.ShortName},WIZ,MAG,ENC]}) {
		/call MQ2Cast "Group Perfected Invisibility" alt -targetid|${Me.ID}
		ECHOCHANNEL AA: Group Perfected Invisibility
		/return
	
	| Invis Vs undead casters
	} else /if (${Me.AltAbilityReady[Group Perfected Invisibility to Undead]} && ${Select[${Me.Class.ShortName},CLR,SHD,NEC,PAL]}) {
		/delay 3
		/call MQ2Cast "Group Perfected Invisibility to Undead" alt -targetid|${Me.ID}
		ECHOCHANNEL AA: Group Perfected Invisibility to Undead
		/return
		
	| shaman	
	} else /if (${Me.AltAbilityReady[Group Silent Presence]} && ${Select[${Me.Class.ShortName},SHM]}) {
		/delay 5
		/call MQ2Cast "Group Silent Presence" alt -targetid|${Me.ID}
		ECHOCHANNEL AA: Group Silent Presence
		/return
	
	| rangers and druids
	}	else /if (${Me.AltAbilityReady[Shared Camouflage]} && ${Select[${Me.Class.ShortName},RNG,DRU]}) {
		/return
		/delay 15
		/call MQ2Cast "Shared Camouflage" alt -targetid|${Me.ID}
		ECHOCHANNEL AA: Shared Camouflage
		/return
	}
/return



|***
 *	DES: clicks a door or lever to open or zone
 *	USE: /take
 *	INI: 
 *  NOTE: 
 ***|
#bind broadcast_door /enter
sub Bind_broadcast_door()
	/if (${broadcastDoor}) {
		/if (${setCommunications.Equal[eqbc]}) {
			/bca //click_door
		} else /if (${setCommunications.Equal[dannet]}) {
			/dgexecute /click_door
		}
	}
	OUT /enter ${sep} ${obroadcasted}
	/call Bind_click_door
/return

|***
 *	DES: sub for door/lever clicking
 ***|
#bind click_door /click_door
sub Bind_click_door()

	/call create_timer timer_try_door 4s

	:retrydoortarget
	/if (!${timer_try_door}) /return
	/doortarget 
	/delay 1
	/if (!${Switch.ID}) /goto :retrydoortarget

	/if (!${timer_try_door}) /return

	/if (!${Switch.Open}) {
		/click left door
		/delay 1s
		/doevents flush
	}
/return



|***
 *	DES: deal with corpses
 *	USE: /cls
 *	INI: 
 *  NOTE: 
 ***|
#bind cls /cls
sub Bind_cls()
	/call Bind_clear_corpse_sub
	/if (${broadcastClean}) {
		/if (${Bool[${Plugin[MQ2Dannet]}]}) {
			/dgaexecute /clear_corpse_sub
		}
	} 
/return

|***
 *	DES: sub for dealing with corpses
 ***|
#bind  clear_corpse_sub /clear_corpse_sub
sub Bind_clear_corpse_sub()
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
 *	DES: will set your group to combat ready with 1 key.
 *	USE: /here  
 *	INI: 
 *  NOTE: this is designed for a fast group set due to agro while traveling. This will not affect the sender of the command
 ***|
#bind broadcast_here /here
sub Bind_broadcast_here()
	/if (${broadcastHere}) {
		/if (${setCommunications.Equal[eqbc]}) {
			/bca //set_combat_here
		}	else /if (${setCommunications.Equal[dannet]}) {
			/dgexecute /set_combat_here
		}
	}
	OUT /here ${sep} ${obroadcasted}
/return

|***
 *	DES: sub for /here command
 ***|
#bind set_combat_here /set_combat_here 
sub Bind_set_combat_here()
 	AMIDEAD
	UPDATEHUD "Combat Now"

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
	/if (${switchBuffPet}) /call create_timer timer_check_buffs_pet ${defaultTimerDelay}
	/if (${switchBuffMerc}) /call create_timer timer_check_buffs_merc ${defaultTimerDelay}
	/if (${switchBuffGroup}) /call create_timer timer_check_buffs_group ${defaultTimerDelay}
	/if (${Bool[${Aura1Buff}]} || ${Bool[${Aura2Buff}]}) /call create_timer timer_check_aura ${defaultTimerDelay}
	/if (${switchBuffTell}) /call create_timer timer_check_buffs_tell ${defaultTimerDelay}
	
	/if (${Select[${Me.Class.ShortName},CLR]}) {
		/call create_timer timer_heal_ward ${tHealWard}
	}

	/if (${Me.Casting.ID}) /stopcast
	
	| stop 'all' the movement options
	AMIDEAD
	| stop sticking to stuff
	/if (${tempsnapstuck}) /varset tempsnapstuck FALSE
	/if (${SetTie}) /varset SetTie FALSE
	
	| stop advanced path
	/if (${Bool[${Plugin[MQ2ADVPath]}]}) {
		/if (${AdvPath.Following}) /squelch /afollow off
	}

	| stop stick
	/if (${Bool[${Plugin[MQ2MoveUtils]}]}) {
		/if (${Stick.Active}) /squelch /stick off
	}

	| stop mq2nav
	/if (${Bool[${Plugin[MQ2Nav]}]}) {
		/if (${Navigation.Active}) /squelch /nav stop
	}	

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
	/if (${Me.DisplayName.Equal[${${MacroQuest.Server}Assist}]}) {
		/if (!${HomeSet}) {
			/call writeINI HomeSet Movement TRUE
			/call set_home
		}
		| cut the agro way down for safety
		/if (${SpawnCount[npc ${If[${switchOverrideLos},,los]} radius ${MobAgro}]} > 2) {
			/if (${baseAgroRange} && ${MobAgro} > ${baseAgroRange}) /agro ${baseAgroRange}
		}	
		/if (!${switchAgro}) /call writeINI switchAgro Common TRUE 
		/if (!${switchPull} && !${useForceAssist}) /call writeINI switchPull Common TRUE 
		/if (${zradius} != ${hereZRadius}) /cc zradius ${hereZRadius}	
	} 

	UPDATEHUD FALSE	
/return



|***
 *	DES: sets all toons to override all engaeg HP% and attack as soon as there is a vlaid target in range
 *	USE: /killmob
 *	INI: 
 *  NOTE: this is just a simple broadcast of .override engageat. for like when your in a hurry and shit.
 ***|
#bind killmob /killmob
sub Bind_killmob()
	/call Bind_override engageat
	/if (${broadcastKillmob}) {
		/if (${setCommunications.Equal[eqbc]}) {
			/bca //override engageat
		} else /if (${setCommunications.Equal[dannet]}) {
			/dgexecute /override engageat
		}
	} 	
	OUT /killmob ${sep} ${obroadcasted}
/return



|***
 *	DES: look for undefined variables
 *	USE: /checkvar
 *	INI: 
 *  NOTE: this is a utility for finding errors in code. nothing more
 ***|
#bind checkvar /checkvar
sub Bind_checkvar()
	/mqclear
	/invoke ${Macro.Undeclared}
/return



|***
 *	DES: make adjustments for the drivetoon
 *	USE: /imincharge
 *	INI: 
 *  NOTE: This is a work in progress.
 ***|
#bind imincharge /imincharge
sub Bind_imincharge()
	| /call imincharge
	
	| set my tie name off
	/call writeINI SetTieToon Movement FALSE

	| set everyone elses /tie defaults
	/if (${setCommunications.Equal[eqbc]}) {
		/bca //tc name ${Me.DisplayName}
	} else /if (${setCommunications.Equal[dannet]}) {
		/dgexecute /tc name ${Me.DisplayName}
	}
	OUT I'm the damn boss ${sep} ${obroadcasted}
/return
	
	
	
|***
 *	DES: have everyone get your target and repeate what you say.
 *	USE: /bcsay VERBAGE
 *	INI: 
 *  NOTE:
 ***|
#bind bcsay /bcsay
sub Bind_bcsay(string _verbage, string _sender, int _tmpID)
	| SDEBUG bcsay v:"${_verbage}" s:${_sender} id:${_tmpID}
	
	/if (${Me.Invis}) /makemevisible
	
	| sender
	/if (!${Bool[${_sender}]}) {
		| need a target
		/if (!${Target.ID}) /return
		
		| speak you peace
		/say ${_verbage}
		
		| tell everyone else to speak up
		
		/if (${setCommunications.Equal[eqbc]}) {
			/bca //bcsay "${_verbage}" ${Me.DisplayName} ${Target.ID}
		} else /if (${setCommunications.Equal[dannet]}) {
			/dgexecute /bcsay "${_verbage}" ${Me.DisplayName} ${Target.ID}
		}
		| no not come back here
		/return
	}

	| everyone else
	/if (${_sender.NotEqual[${Me.DisplayName}]}) {
		/if (${Bool[${Spawn[pc ${_sender} radius 60].ID}]}) {
			/target ID ${_tmpID}
			/delay 10s ${Target.ID} == ${_tmpID}
			/if (${Target.ID} == ${_tmpID}) {
				/delay ${Int[${Math.Calc[${Math.Rand[20]}+10]}]}
				/say ${_verbage}
				/delay 1s
			}
		}
	}
	
/return



|***
 *	DES: for when you really gotta go..
 *	USE: /gtfo
 *	INI: 
 *  NOTE:
 ***|
#bind gtfo /gtfo
sub Bind_gtfo()

	/if (${setCommunications.Equal[eqbc]}) {
		/bca //get_the_fuck_out
	} else /if (${setCommunications.Equal[dannet]}) {
		/dgexecute /get_the_fuck_out
	}
	OUT Get The Fuck Out ${sep} ${obroadcasted}
	/call Bind_get_the_fuck_out
/return

#bind get_the_fuck_out /get_the_fuck_out
sub Bind_get_the_fuck_out()
	/call sort_cursor TRUE
	/call set_stop NOECHO
	/call clear_combat
	/docommand /makemevisible

	/if (${Me.ItemReady[${GTFO}]}) {
		/if (${validate_cast[FALSE, item, "${GTFO}", ${Me.ID}]}) /call core_cast2 "${GTFO}" item 0 FALSE
	} else /if (${Me.SpellReady[${GTFO}]}) {
		/if (${validate_cast[FALSE, spell, "${GTFO}", ${Me.ID}]}) /call core_cast2 "${GTFO}" 0 0 FALSE
	} else /if (${Me.AltAbility[${GTFO}].ID}) {
		/if (${validate_cast[FALSE, alt, "${GTFO}", ${Me.ID}]}) /call core_cast2 "${GTFO}" alt 0 FALSE
	}
/return



|***
 *	DES: turns everyone off
 *	USE: /off  
 *	INI: 
 *  NOTE: this will show those whiney bastards
 ***|
#bind broadcast_off /broadcast_off
sub Bind_broadcast_off()
	/if (${setCommunications.Equal[eqbc]}) {
		/bca //core auto off
	}	else /if (${setCommunications.Equal[dannet]}) {
		/dgexecute /core auto off
	}
	OUT /core auto off ${sep} ${obroadcasted}
/return



|***
 *	DES: turns everyone on.. 
 *	USE: /on  
 *	INI: 
 *  NOTE: never judge anothers kink.. never know what you may like??!
 ***|
#bind broadcast_on /broadcast_on
sub Bind_broadcast_on()
	/if (${setCommunications.Equal[eqbc]}) {
		/bca //core auto on
	}	else /if (${setCommunications.Equal[dannet]}) {
		/dgexecute /core auto on
	}
	OUT /core auto on ${sep} ${obroadcasted}
/return



|***
 *	DES: fellowship campfire controls
 *	USE: /campfire  
 *	INI: 
 *  NOTE: 
 ***|
#bind campfire /campfire
sub Bind_campfire(string _verbage)
	/declare _count int local 0
	/declare _bind string local campfire
	/declare _listout string local
	/declare _sep bool local FALSE
	/declare campfireType string local |drop|port

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

	| drop a campfire
	/if (${_verbage.Equal[drop]}) {
		
		| need 3 people close
		/if (${SpawnCount[fellowship radius 50]} < 3) /return
		
		/squelch /windowstate FellowshipWnd open
		/delay 10 !${Window[FellowshipWnd].Open}
		/nomodkey /notify FellowshipWnd FP_Subwindows tabselect 2
		
		| skip this if we dont have a fire
		/if (${Me.Fellowship.Campfire}) {
			/nomodkey /notify FellowshipWnd FP_DestroyCampsite leftmouseup
			/delay 5s ${Window[ConfirmationDialogBox].Open}
			/if (${Window[ConfirmationDialogBox].Open}) /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup
			/delay 5s !${Me.Fellowship.Campfire}
		}
		/nomodkey /notify FellowshipWnd FP_RefreshList leftmouseup
		/delay 8
		/nomodkey /notify FellowshipWnd FP_CampsiteKitList listselect 1
		/delay 8
		/nomodkey /notify FellowshipWnd FP_CreateCampsite leftmouseup
		/delay 5s ${Me.Fellowship.Campfire}
		/if (${Window[FellowshipWnd].Open}) /squelch /windowstate FellowshipWnd close
		
	| click campfire insignia	
	} else /if (${_verbage.Equal[port]}) {
		/if (${Me.Invis}) /makemevisible
		/delay 1s !${Me.Invis}
  	/call set_stop NOECHO
		/delay 1s !${Me.Casting.ID}
		/if (${validate_cast[FALSE, item, "Fellowship Registration Insignia", ${Me.ID}]}) /call core_cast2 "Fellowship Registration Insignia" item 0 FALSE
		/return
	}
	
	/if (${switchMQclearonCLS}) /mqclear
	/for _count 1 to ${${_bind}Type.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${${_bind}Type.Arg[${_count},|]}\ax		
		/varset _sep TRUE
	/next _count
	VOUT /${_bind} [${_listout}]	
	
/return



|***
 *	DES: changes your communications settings from eqbc/dannet or back.
 *	USE: /comm [eqbc|dannet]
 *	INI: core.ini
 *  NOTE: this is a broadcasted update.
 ***|
#bind comm /comm
sub Bind_comm(string _commType)

	/declare _count int local 0
	/declare _sep bool local
	/declare _listout string local

	
	/if (${_commType.Equal[eqbc]}) {
		/if (!${validate_plugin[FALSE, |MQ2EQBC|MQ2NetBots]}) {
			OUT ${owarning} install plugin ${cinfo}MQ2EQBC\ax and ${cinfo}MQ2NetBots\ax first.
			/return
		}		
		/call writeCoreINI setCommunications Comm eqbc
		/bcaa //varset setCommunications eqbc
	} else /if (${_commType.Equal[dannet]}) {
		/if (!${validate_plugin[FALSE, |MQ2DanNet]}) {
			OUT ${owarning} install plugin ${cinfo}MQ2DanNet\ax first.
			/return
		}
		/call writeCoreINI setCommunications Comm dannet
		/dgaexecute /varset setCommunications dannet
	}
	/declare _Types string local |eqbc|dannet
	/for _count 1 to ${_Types.Count[|]}
		/varset _listout ${_listout} ${If[${_sep},${sep} ,]} 
		/if (${_Types.Arg[${_count},|].Equal[eqbc]}) /varset _listout ${_listout}${If[${setCommunications.Equal[eqbc]},${cgood}eqbc\ax,${cinfo}eqbc\ax]}
		/if (${_Types.Arg[${_count},|].Equal[dannet]}) /varset _listout ${_listout}${If[${setCommunications.Equal[dannet]},${cgood}dannet\ax,${cinfo}dannet\ax]}
		/varset _sep TRUE
	/next _count
	VOUT /comm [${_listout}]

/return



|***
 *	DES: Request Call of the Hero [aa|group]
 *	USE: /coh [group]
 *	INI: 
 *  NOTE: 
 ***|
#bind request_coh /coh
sub Bind_request_coh(string _verbage)
	
	/declare _mage string local
	/declare _sep bool local
	/declare _count int local 0
	
	/if (${setCommunications.Equal[eqbc]}) {
		/if (${_verbage.Equal[group]}) {
			/bct ${_mage} /call_of_the_hero "${Me.DisplayName}" "${_verbage}"
		} else {
			/bct ${_mage} /call_of_the_hero "${Me.DisplayName}"
		}
		
	}	else /if (${setCommunications.Equal[dannet]}) {
		
		/if (${_verbage.Equal[group]}) {
			/dex ${get_resident[mage, 99999]} /call_of_the_hero "${Me.DisplayName}" "${_verbage}"
			/return
		} else {
			/dex ${get_resident[mage, 99999]} /call_of_the_hero "${Me.DisplayName}"
			/return
		}
	}	
/return


