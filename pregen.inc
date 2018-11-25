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
	/call setDeclare coreBuild											string	outer 20181125.00


	| Base Environment settings
	/call setDeclare forceINIcheck									bool		outer TRUE
	/call setDeclare castReturn 										string 	outer NULL
	/call setDeclare combat													string	outer FALSE
	/call setDeclare isValidTarget									bool		outer FALSE
	/call setDeclare noInvis 												bool 		outer TRUE
	/call setDeclare noFeigning 										bool 		outer TRUE
	/call setDeclare Target2Kill										int			outer 0
	/call setDeclare currentLootCount								int			outer	0


	| lists for .. you know..lists and stuff.
	/call setDeclare AllClassList										string 	outer "|WAR|SHD|PAL|BER|ROG|MNK|RNG|BST|BRD|NEC|ENC|WIZ|MAG|CLR|SHM|DRU"
	/call setDeclare MeleeList    		  						string 	outer "|WAR|ROG|BST|MNK|SHD|PAL|BER|BRD|RNG"
	/call setDeclare CastList												string 	outer "|BST|SHD|PAL|RNG|CLR|DRU|SHM|ENC|WIZ|NEC|MAG"
	/call setDeclare PetList												string 	outer "|ENC|DRU|SHM|SHD|WIZ|BRD|BST|MAG|NEC"
	/call setDeclare HealerList											string 	outer "|CLR|DRU|SHM|PAL"
	/call setDeclare RezerList											string 	outer "|CLR|DRU|SHM|PAL|NEC"
	/call setDeclare AASnareList										string 	outer "|RNG|DRU|SHD|NEC|ROG|WAR|WIZ|MNK|PAL|BER"
	/call setDeclare AAEradicateList								string 	outer "|RNG|DRU|NEC|WIZ|ENC|MAG"
	/call DeclareCoreIniVar plist										string	Plugin "|MQ2Cast|MQ2Exchange|MQ2MoveUtils|MQ2Debuffs|MQ2DanNet"

	
	| this is a makeshift /doevents to force binds to be cycled.
	/call setDeclare bindTrigger										bool		outer TRUE
	

	| holder for the current sub for debugging purposes
	/call setDeclare currentdebugsub								string	outer FALSE


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

/return




|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_core()
	/declare _count 																int 		local 0


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





	| misc environment switches/controls
	/call DeclareCoreIniVar minMobPullLvl						int			General 1
	/call DeclareCoreIniVar maxMobPullLvl						int			General 150
	/call DeclareCoreIniVar autoinihousekeeping			bool		General FALSE
	/call DeclareCoreIniVar smartasserrors					bool		General TRUE
	/call DeclareCoreIniVar forceNukeLoop						bool		General FALSE
	/call DeclareCoreIniVar autoAddNameList					bool		General TRUE
	/call DeclareCoreIniVar switchDispInfo					bool		General TRUE
	/call DeclareCoreIniVar switchDispBuild					bool		General TRUE
	/call DeclareCoreIniVar switchDispEXP						bool		General TRUE
	/call DeclareCoreIniVar switchAADiscnowManual		bool		General FALSE
	/call DeclareCoreIniVar switchDelayWhileZoning	bool		General FALSE
	/call DeclareCoreIniVar switchAutoRestFull			bool		General FALSE
	/call DeclareCoreIniVar switchModRodinRest			bool		General TRUE
	/call DeclareCoreIniVar switchBuffinRest				bool		General TRUE
	/call DeclareCoreIniVar switchUseCLSinHUD				bool		General FALSE
	/call DeclareCoreIniVar switchUseADVL						bool		General TRUE
	/call DeclareCoreIniVar switchGlyphOutsideRaid	bool		General FALSE
	/call DeclareCoreIniVar maintenance							string	General "|FALSE"
	/call DeclareCoreIniVar switchMQclearonCLS			bool		General FALSE
	/call DeclareCoreIniVar switchGuildinSafelist		bool		General FALSE
	/call DeclareCoreIniVar switchCheckGameCash			bool		General FALSE
	/call DeclareCoreIniVar switchPersistentModes		bool		General FALSE
	/call DeclareCoreIniVar switchNoTieonCombat			bool		General FALSE
	/call DeclareCoreIniVar switchDisableMQ2Melee		bool		General TRUE
	/call DeclareCoreIniVar switchIgnoreXpacControl	bool		General FALSE
	/call DeclareCoreIniVar switchBuffGemHold				bool		General TRUE
	/call DeclareCoreIniVar setMQFontSize						int			General 2
	/call DeclareCoreIniVar setMQCaptions						bool		General off
	/call DeclareCoreIniVar setAutoAssist						bool		General off
	/call DeclareCoreIniVar setMaxINIBuffCount			int			General 10
	/call DeclareCoreIniVar switchMealinManual			bool		General FALSE


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


	| buff queue variables
	/call DeclareCoreIniVar queueCountMax						int			General 10
	/call setDeclare queueCount            					int  		outer 0
	
	/if (!${Defined[queueBuffID]}) /declare queueBuffID[8] 												int  		outer
	/if (!${Defined[queueList]}) /declare queueList[${queueCountMax},2] 				 	int  		outer


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


	| zone - core.zone.ini settings
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


	| general settings for mode item usage
	/call DeclareCoreIniVar modeDrinkofChoice				string	mode FALSE
	/call DeclareCoreIniVar modeOpenItem						string	mode FALSE

/return



|***
 *	DES: 
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_character()
	/declare _count 																int 		local 0
	/call setINIVar classBuild											string	Common 00.00

	| auto or manual control?
	/call setINIVar autoControl			 								bool		Common FALSE 


	/call setINIVar noCastWith											string	Common "|Quick Defense|Protection of Bayle|Interrogation Intimidation|Energy Charge|Cloud of Terror|Faith"
	/call setINIVar noMeleeWith											string	Common "|Itching Poison"
	
	/call setINIVar shrinkItem 											string 	Common "Ring of the Ancients"
	/call setINIVar IllusionItem										string	Common FALSE
	/call setINIVar IllusionBuff										string	Common FALSE
	/call setINIVar GTFO														string	Common FALSE
	/call setINIVar switchAApicker									bool 		Common FALSE
	
	| zerkers keep their own range item setup
	/if (!${Select[${Me.Class.ShortName},BER]}) {
		/call setINIVar RangeItem												string	Common "|FALSE|FALSE"
		/call setINIVar RangeItemSummon									string	Common "|FALSE|FALSE"
		/call setINIVar RangeItemCount									int			Common 5
	}
	/call setINIVar RangeSpell											string	Common FALSE
	/call setINIVar RangeClickItem									string	Common FALSE
	/call setINIVar RangeDisc												string	Common FALSE
			
	/call setINIVar autoInventoryItem								string	Common "|Summoned: Giant Modulation Shard"
	/call setINIVar stoponDS												bool		Common FALSE		
	/call setINIVar useGlyph												string	Common "Glyph of the Cataclysm"
	/call setINIVar switchSos												bool	 	Common FALSE


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
	
	
	| burns
	/call setDeclare burnType												string	outer "|auto|count|engageat|force"
	/call setDeclare setBurnControl									bool		outer FALSE
	/call setDeclare burnForce											bool		outer FALSE
	/call setINIVar burnAuto												bool		common FALSE
	/call setINIVar burnEngageat										int			common 80
	/call setINIVar burnCount												int			common 10
	

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


	| rez	control
  /call setDeclare rezType												string	outer "|stop|combat|emergency|everyone|give|pct|range|take"
  /call setDeclare rezPct													string	outer "|10|20|35|50|60|75|90|93|96"
	/call setINIVar RezTokenList										string 	Heal "|WAR|SHD|PAL|BER|ROG|MNK|RNG|BST|BRD|NEC|ENC|WIZ|MAG|CLR|SHM|DRU"
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
		

	| spire control
	/call setDeclare typeSpire											string	outer "|0|1|2|3"
	/call setDeclare ${Me.Class.ShortName}spire0		int			Buffs 0
	/call setINIVar spireRaid												int 		Buffs 0
	/call setINIVar spireGroup											int 		Buffs 0


	/call setINIVar switchOffTank										bool		Common FALSE
	
	/call setINIVar CombatDistance									int			Movement 15
	/call setINIVar combatRadius										string	Common 30
	/call setINIVar zradius													int			Common 60	
	/call setINIVar switchBandolier									bool		meleedisc FALSE
	/call setINIVar stopnukeAt 											int 		Spells 0
	/call setINIVar switchAlliance									string	Spells FALSE
	/call setINIVar spellAllianceTrigger						string	Spells FALSE
	/call setINIVar switchForceFadeRest							bool		Common FALSE
	
	/call setINIVar switchCombatMelee								bool		Common FALSE
	/call setINIVar switchCombatRange								bool		Common FALSE
	/call setINIVar switchCombatNuke								bool		Common FALSE
	/call setINIVar switchCombatDoT									bool		Common FALSE
	/call setINIVar	broadcastGroup									string	Common Core

	
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


	| unity buffs
	/for _count 1 to 7
		/call setINIVar BuffAAUnity${_count}					string	Buffs FALSE
	/next _count


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
  /call setDeclare overrideType										string	outer "|stop|engageat|exclude|combat|loot|los|melee|name|range|safelist|safezone|trainspell"
	/call setDeclare switchOverride									bool		outer FALSE
	/call setDeclare switchOverrideStop							bool		outer FALSE
  /call setINIVar switchOverrideLos								bool		Override FALSE
  /call setINIVar switchOverrideSafezone					bool		Override FALSE
  /call setINIVar switchOverrideExclude						bool		Override FALSE
  /call setINIVar switchOverrideName							bool		Override FALSE
  /call setINIVar switchOverrideEngageat					bool		Override FALSE
  /call setINIVar switchOverrideRange							bool		Override FALSE
  /call setINIVar switchOverrideMelee							bool		Override FALSE
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
	/call setINIVar bandolierRange									string	meleedisc FALSE

	
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
	/call setINIVar Food														string 	Meal FALSE
	/call setINIVar FoodDispenser										string 	Meal FALSE
	/call setINIVar FoodCount												int		 	Meal 0
	/call setINIVar Drink														string 	Meal FALSE
	/call setINIVar DrinkDispenser									string 	Meal FALSE
	/call setINIVar DrinkCount											int		 	Meal 0
	/call setINIVar tFoodDrink											string	Timer 10m
	/call create_timer timer_meal 1


	| pet crap
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
  /call setDeclare buffType												string	outer "|stop|self|item|combat|bc|raid|tell|pet|merc|group|mgb"
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
	/call setINIVar switchBuffMgb										bool		Buffs FALSE

	
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
	/for _count 1 to ${setMaxINIBuffCount}
		/call setINIVar Buff${_count}									string 	Buffs FALSE
		/call setINIVar Buff${_count}chk							string 	Buffs FALSE
		/call setINIVar Buff${_count}self							string 	Buffs FALSE
		/call setINIVar Buff${_count}selfCondition		string 	Buffs FALSE
		/call setINIVar Buff${_count}Alias						string 	Buffs "|FALSE" 
		/call setINIVar BCBuff${_count}Class					string 	Buffs "|WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM"
		/call setINIVar RaidBuff${_count}Class				string 	Buffs "|WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM"
		/call setINIVar ManualBuff${_count}Class			string 	Buffs "|WAR|PAL|SHD|BRD|RNG|BST|BER|MNK|ROG|WIZ|MAG|ENC|NEC|CLR|DRU|SHM"
		/call setINIVar Buff${_count}noBCinRaid				bool	 	Buffs FALSE
	/next _count
	
	
	| MGB buff
	/if (${validate_class[FALSE, ${CastList}]}) {	
		/call setINIVar BuffMGBname										string 	Buffs FALSE	
		/call setINIVar BuffMGBchk										string 	Buffs FALSE	
		/call setINIVar BuffMGBalias									string 	Buffs "|FALSE"
		/call setINIVar BuffMGBcount									int		 	Buffs 18
		/call setINIVar BuffMGBmana										int		 	Buffs 40
	}
	
	
	
	

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
sub pregen_timers()
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
		/call setINIVar RangeItem											string	Common "|Axe of the Mangler|Axe of the Demolisher"
		/call setINIVar RangeItemSummon								string	Common "|Axe of the Mangler|Axe of the Demolisher"
		/call setINIVar RangeItemCount								int			Common 5
	} else /if (${Me.Level} >= 101)  {
		/call setINIVar RangeItem											string	Common "|Axe of the Demolisher"
		/call setINIVar RangeItemSummon								string	Common "|Axe of the Demolisher"
		/call setINIVar RangeItemCount								int			Common 5
	} else /if (${Me.Level} >= 96)  {
		/call setINIVar RangeItem											string	Common "|Axe of the Brute"
		/call setINIVar RangeItemSummon								string	Common "|Axe of the Brute"
		/call setINIVar RangeItemCount								int			Common 5
	} else /if (${Me.Level} >= 91)  {
		/call setINIVar RangeItem											string	Common "|Axe of the Sunderer"
		/call setINIVar RangeItemSummon								string	Common "|Axe of the Sunderer"
		/call setINIVar RangeItemCount								int			Common 5
	} else /if (${Me.Level} >= 86)  {
		/call setINIVar RangeItem											string	Common "|Axe of the Savage"
		/call setINIVar RangeItemSummon								string	Common "|Axe of the Savage"
		/call setINIVar RangeItemCount								int			Common 5
	} else /if (${Me.Level} >= 81)  {
		/call setINIVar RangeItem											string	Common "|Axe of the Eradicator"
		/call setINIVar RangeItemSummon								string	Common "|Axe of the Eradicator"
		/call setINIVar RangeItemCount								int			Common 5
	} else /if (${Me.Level} >= 76)  {
		/call setINIVar RangeItem											string	Common "|Axe of the Decimator"
		/call setINIVar RangeItemSummon								string	Common "|Axe of the Decimator"
		/call setINIVar RangeItemCount								int			Common 5
	} else /if (${Me.Level} >= 71)  {
		/call setINIVar RangeItem											string	Common "|Axe of the Annihilator"
		/call setINIVar RangeItemSummon								string	Common "|Axe of the Annihilator"
		/call setINIVar RangeItemCount								int			Common 5
	} else /if (${Me.Level} >= 66)  {
		/call setINIVar RangeItem											string	Common "|Axe of the Destroyer"
		/call setINIVar RangeItemSummon								string	Common "|Axe of the Destroyer"
		/call setINIVar RangeItemCount								int			Common 5
	} else {
		/call setINIVar RangeItem											string	Common "|FALSE|FALSE"
		/call setINIVar RangeItemSummon								string	Common "|FALSE|FALSE"
		/call setINIVar RangeItemCount								int			Common 5
	}

	/call setDeclare BERspire1											string	outer "Fundament: First Spire of Savagery"
	/call setDeclare BERspire2											string	outer "Fundament: Second Spire of Savagery"
	/call setDeclare BERspire3											string	outer "Fundament: Third Spire of Savagery"
	/call setDeclare BERsynergyAA										string	outer "Fury's Synergy"
	/call setDeclare BEREpic1												string	outer "Kerasian Axe of Ire"
	/call setDeclare BEREpic15											string	outer "Raging Taelosian Alloy Axe"
	/call setDeclare BEREpic2												string	outer "Vengeful Taelosian Blood Axe"
	/call setINIVar BERAlliance											string	Discs "Demolisher's Alliance"
	/call setDeclare BERAAUnity											string	outer FALSE
	/call setDeclare BERAAFade											string	outer "Self Preservation"
	
	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50

	/call setINIVar AEMelee					 								bool	 	Discs FALSE
	/call setINIVar DiscSnare 											string 	Discs FALSE
	/call setINIVar SnareAxe 												string 	Discs FALSE

	| Volley Line
	/if (${Me.Level} >= 109)  {
		/call setINIVar DiscVolley 										string 	Discs "Mangling Volley"
	} else /if (${Me.Level} >= 104)  {
		/call setINIVar DiscVolley 										string 	Discs "Demolishing Volley"
	} else /if (${Me.Level} >= 99)  {
		/call setINIVar DiscVolley 										string 	Discs "Brutal Volley"
	} else /if (${Me.Level} >= 94)  {
		/call setINIVar DiscVolley 										string 	Discs "Sundering Volley"
	} else /if (${Me.Level} >= 89)  {
		/call setINIVar DiscVolley 										string 	Discs "Savage Volley"
	} else /if (${Me.Level} >= 84)  {
		/call setINIVar DiscVolley 										string 	Discs "Eradicator's Volley"
	} else /if (${Me.Level} >= 79)  {
		/call setINIVar DiscVolley 										string 	Discs "Decimator's Volley"
	} else /if (${Me.Level} >= 74)  {
		/call setINIVar DiscVolley 										string 	Discs "Annihilator's Volley"
	} else /if (${Me.Level} >= 69)  {
		/call setINIVar DiscVolley 										string 	Discs "Destroyer's Volley"
	} else /if (${Me.Level} >= 61)  {
		/call setINIVar DiscVolley 										string 	Discs "Rage Volley"
	} else {
		/call setINIVar DiscVolley			 							string	Discs FALSE
	}
	
	/call setINIVar CryOfChaos 											string 	Discs "Ancient: Cry of Chaos"
	
	| Berserking Line
	/if (${Me.Level} >= 100)  {
		/call setINIVar DiscBerserking								string 	Discs "Brutal Discipline"
	} else /if (${Me.Level} >= 95)  {
		/call setINIVar DiscBerserking 								string 	Discs "Sundering Discipline"
	} else /if (${Me.Level} >= 75)  {
		/call setINIVar DiscBerserking 								string 	Discs "Berserking Discipline"
	} else /if (${Me.Level} >= 60)  {
		/call setINIVar DiscBerserking 								string 	Discs "Burning Rage Discipline"
	} else /if (${Me.Level} >= 58)  {
		/call setINIVar DiscBerserking 								string 	Discs "Blind Rage Discipline"
	} else /if (${Me.Level} >= 57)  {
		/call setINIVar DiscBerserking 								string 	Discs "Focused Fury Discipline"
	} else /if (${Me.Level} >= 54)  {
		/call setINIVar DiscBerserking 								string 	Discs "Cleaving Rage Discipline"
	} else /if (${Me.Level} >= 53)  {
		/call setINIVar DiscBerserking 								string 	Discs "Inspired Anger Discipline"
	} else {
		/call setINIVar DiscBerserking			 					string	Discs FALSE
	}

	| rest disc
	/if (${Me.Level} >= 101)  {
		/call setINIVar DiscRest			 								string	Discs "Breather"
	} else /if (${Me.Level} >= 96)  {
		/call setINIVar DiscRest			 								string	Discs "Rest"
	} else /if (${Me.Level} >= 91)  {
		/call setINIVar DiscRest			 								string	Discs "Reprive"
	} else /if (${Me.Level} >= 86)  {
		/call setINIVar DiscRest			 								string	Discs "Respite"
	} else {
		/call setINIVar DiscRest			 								string	Discs FALSE
	}

	| Combat Frenzy
	/if (${Me.Level} >= 106)  {
		/call setINIVar DiscCombatFrenzy 							string 	Discs "Mangler Frenzy"
	} else /if (${Me.Level} >= 101)  {
		/call setINIVar DiscCombatFrenzy 							string 	Discs "Demolishing Frenzy"
	} else /if (${Me.Level} >= 96)  {
		/call setINIVar DiscCombatFrenzy 							string 	Discs "Vanquishing Frenzy"
	} else /if (${Me.Level} >= 91)  {
		/call setINIVar DiscCombatFrenzy 							string 	Discs "Conquering Frenzy"
	} else /if (${Me.Level} >= 86)  {
		/call setINIVar DiscCombatFrenzy 							string 	Discs "Overwhelming Frenzy"
	} else /if (${Me.Level} >= 81)  {
		/call setINIVar DiscCombatFrenzy 							string 	Discs "Overpowering Frenzy"
	} else {
		/call setINIVar DiscCombatFrenzy			 				string	Discs FALSE
	}

	| Disc Scream
	/if (${Me.Level} >= 92)  {
		/call setINIVar DiscScream 										string 	Discs "Perturbing Scream"
	} else /if (${Me.Level} >= 87)  {
		/call setINIVar DiscScream 										string 	Discs "Distracting Scream"
	} else /if (${Me.Level} >= 82)  {
		/call setINIVar DiscScream 										string 	Discs "Distressing Scream"
	} else /if (${Me.Level} >= 77)  {
		/call setINIVar DiscScream 										string 	Discs "Agitating Scream"
	} else /if (${Me.Level} >= 72)  {
		/call setINIVar DiscScream 										string 	Discs "Unsettling Scream"
	}else {
		/call setINIVar DiscScream			 							string	Discs FALSE
	}

	| Axe Throw
	/if (${Me.Level} >= 108)  {
		/call setINIVar ThrowingAxe 									string 	Discs "Mangling Axe Throw"
	} else /if (${Me.Level} >= 103)  {
		/call setINIVar ThrowingAxe 									string 	Discs "Demolishing Axe Throw"
	} else /if (${Me.Level} >= 98)  {
		/call setINIVar ThrowingAxe 									string 	Discs "Brutal Axe Throw"
	} else /if (${Me.Level} >= 93)  {
		/call setINIVar ThrowingAxe 									string 	Discs "Spirited Axe Throw"
	} else /if (${Me.Level} >= 88)  {
		/call setINIVar ThrowingAxe 									string 	Discs "Energetic Axe Throw"
	} else /if (${Me.Level} >= 83)  {
		/call setINIVar ThrowingAxe 									string 	Discs "Vigorous Axe Throw"
	} else {
		/call setINIVar ThrowingAxe			 							string	Discs FALSE
	}

	| Jolt Line
	/if (${Me.Level} >= 109)  {
		/call setINIVar DiscJolt 											string 	Discs "Jarring Jolt"
	} else /if (${Me.Level} >= 104)  {
		/call setINIVar DiscJolt 											string 	Discs "Jarring Smite"
	} else /if (${Me.Level} >= 99)  {
		/call setINIVar DiscJolt 											string 	Discs "Jarring Crush"
	} else /if (${Me.Level} >= 94)  {
		/call setINIVar DiscJolt 											string 	Discs "Jarring Blow"
	} else /if (${Me.Level} >= 89)  {
		/call setINIVar DiscJolt 											string 	Discs "Jarring Slam"
	} else /if (${Me.Level} >= 84)  {
		/call setINIVar DiscJolt 											string 	Discs "Jarring Clash"
	} else /if (${Me.Level} >= 79)  {
		/call setINIVar DiscJolt 											string 	Discs "Jarring Smash"
	} else /if (${Me.Level} >= 74)  {
		/call setINIVar DiscJolt 											string 	Discs "Jarring Strike"
	} else /if (${Me.Level} >= 69)  {
		/call setINIVar DiscJolt 											string 	Discs "Baffling Strike"
	} else /if (${Me.Level} >= 65)  {
		/call setINIVar DiscJolt 											string 	Discs "Confusing Strike"
	} else /if (${Me.Level} >= 48)  {
		/call setINIVar DiscJolt 											string 	Discs "Distracting Strike"
	} else /if (${Me.Level} >= 24)  {
		/call setINIVar DiscJolt 											string 	Discs "Divertive Strike"
	} else {
		/call setINIVar DiscJolt			 								string	Discs FALSE
	}
	
	| Bloodlust
	/if (${Me.Level} >= 110)  {
		/call setINIVar SharedBloodLust 							string 	Discs "Shared Ruthlessness"
		/call setINIVar SharedBloodLustRecourse				string 	Discs "Reflected Ruthlessness"
	} else /if (${Me.Level} >= 105)  {
		/call setINIVar SharedBloodLust 							string 	Discs "Shared Cruelty"
		/call setINIVar SharedBloodLustRecourse				string 	Discs "Reflected Cruelty"
	} else /if (${Me.Level} >= 100)  {
		/call setINIVar SharedBloodLust 							string 	Discs "Shared Viciousness"
		/call setINIVar SharedBloodLustRecourse				string 	Discs "Reflected Viciousness"
	} else /if (${Me.Level} >= 95)  {
		/call setINIVar SharedBloodLust 							string 	Discs "Shared Savagery"
		/call setINIVar SharedBloodLustRecourse				string 	Discs "Reflected Savagery"
	} else /if (${Me.Level} >= 90)  {
		/call setINIVar SharedBloodLust 							string 	Discs "Shared Brutality"
		/call setINIVar SharedBloodLustRecourse				string 	Discs "Reflected Brutality"
	} else /if (${Me.Level} >= 85)  {
		/call setINIVar SharedBloodLust 							string 	Discs "Shared Bloodlust"
		/call setINIVar SharedBloodLustRecourse				string 	Discs "Reflected Bloodlust"
	} else {
		/call setINIVar SharedBloodLust 							string 	Discs FALSE
		/call setINIVar SharedBloodLustRecourse				string 	Discs FALSE
	}
	
	| Overwhelming Frenzy Line
	/if (${Me.Level} >= 110)  {
		/call setINIVar DiscOverwhelmingFrenzy				string 	Discs "Torrid Frenzy"
	} else /if (${Me.Level} >= 105)  {
		/call setINIVar DiscOverwhelmingFrenzy 				string 	Discs "Stormwild Frenzy"
	} else /if (${Me.Level} >= 100)  {
		/call setINIVar DiscOverwhelmingFrenzy 				string 	Discs "Fearless Frenzy"
	} else /if (${Me.Level} >= 95)  {
		/call setINIVar DiscOverwhelmingFrenzy 				string 	Discs "Steel Frenzy"
	} else /if (${Me.Level} >= 90)  {
		/call setINIVar DiscOverwhelmingFrenzy 				string 	Discs "Fighting Frenzy"
	} else /if (${Me.Level} >= 85)  {
		/call setINIVar DiscOverwhelmingFrenzy 				string 	Discs "Combat Frenzy"
	} else /if (${Me.Level} >= 80)  {
		/call setINIVar DiscOverwhelmingFrenzy 				string 	Discs "Battle Frenzy"
	} else {
		/call setINIVar DiscOverwhelmingFrenzy			 	string	Discs FALSE
	}

	| Cleaving Anger Line
	/if (${Me.Level} >= 86)  {
		/call setINIVar DiscCleavingAnger							string 	Discs "Cleaving Acrimony Discipline"
	} else /if (${Me.Level} >= 65)  {
		/call setINIVar DiscCleavingAnger 						string 	Discs "Cleaving Anger Discipline"
	} else /if (${Me.Level} >= 54)  {
		/call setINIVar DiscCleavingAnger 						string 	Discs "Cleaving Rage Discipline"
	} else {
		/call setINIVar DiscCleavingAnger			 				string	Discs FALSE
	}

	| Opening Attack Line
	/if (${Me.Level} >= 107)  {
		/call setINIVar DiscSlapintheFace							string 	Discs "Sucker Punch"
	} else /if (${Me.Level} >= 102)  {
		/call setINIVar DiscSlapintheFace 						string 	Discs "Kick in the Shins"
	} else /if (${Me.Level} >= 97)  {
		/call setINIVar DiscSlapintheFace 						string 	Discs "Punch in the Throat"
	} else /if (${Me.Level} >= 92)  {
		/call setINIVar DiscSlapintheFace 						string 	Discs "Kick in the Teeth"
	} else /if (${Me.Level} >= 87)  {
		/call setINIVar DiscSlapintheFace 						string 	Discs "Kick in the Shins"
	} else {
		/call setINIVar DiscSlapintheFace			 				string	Discs FALSE
	}

	| Axe of Line
	/if (${Me.Level} >= 110)  {
		/call setINIVar DiscAxeofRallos								string 	Discs "Axe of Rekatok"
	} else /if (${Me.Level} >= 105)  {
		/call setINIVar DiscAxeofRallos 							string 	Discs "Axe of Numicia"
	} else /if (${Me.Level} >= 102)  {
		/call setINIVar DiscAxeofRallos 							string 	Discs "Axe of Aeons"
	} else /if (${Me.Level} >= 100)  {
		/call setINIVar DiscAxeofRallos 							string 	Discs "Axe of Zurel"
	} else /if (${Me.Level} >= 95)  {
		/call setINIVar DiscAxeofRallos 							string 	Discs "Axe of Illdaera"
	} else /if (${Me.Level} >= 90)  {
		/call setINIVar DiscAxeofRallos 							string 	Discs "Axe of Graster"
	} else /if (${Me.Level} >= 85)  {
		/call setINIVar DiscAxeofRallos 							string 	Discs "Axe of Rallos"
	} else {
		/call setINIVar DiscAxeofRallos			 					string	Discs FALSE
	}

	| Flurry Line
	/if (${Me.Level} >= 89)  {
		/call setINIVar DiscVengeful									string 	Discs "Avenging Flurry Discipline"
	} else /if (${Me.Level} >= 60)  {
		/call setINIVar DiscVengeful 									string 	Discs "Vengeful Flurry Discipline"
	} else {
		/call setINIVar DiscVengeful			 						string	Discs FALSE
	}

	| Rage Line
	/if (${Me.Level} >= 102)  {
		/call setINIVar DiscFesteringRage							string 	Discs "Bubbling Rage"
	} else /if (${Me.Level} >= 98)  {
		/call setINIVar DiscFesteringRage 						string 	Discs "Festering Rage"
	} else {
		/call setINIVar DiscFesteringRage			 				string	Discs FALSE
	}

	| Cry Havoc Line
	/if (${Me.Level} >= 98)  {
		/call setINIVar DiscCryHavoc									string 	Discs "Cry Carnage"
	} else /if (${Me.Level} >= 68)  {
		/call setINIVar DiscCryHavoc 									string 	Discs "Cry Havoc"
	} else {
		/call setINIVar DiscCryHavoc			 						string	Discs FALSE
	}

	| Augmented Frenzy Line
	/if (${Me.Level} >= 109)  {
		/call setINIVar DiscAugmentedFrenzy						string 	Discs "Bolstered Frenzy"
		/call setDeclare AugmentedFrenzyEffect 				string 	outer "Bolstered Frenzy Effect"
	} else /if (${Me.Level} >= 104)  {
		/call setINIVar DiscAugmentedFrenzy 					string 	Discs "Amplified Frenzy"
		/call setDeclare AugmentedFrenzyEffect 				string 	outer "Amplified Frenzy Effect"
	} else /if (${Me.Level} >= 99)  {
		/call setINIVar DiscAugmentedFrenzy 					string 	Discs "Augmented Frenzy"
		/call setDeclare AugmentedFrenzyEffect 				string 	outer "Augmented Frenzy Effect"
	} else {
		/call setINIVar DiscAugmentedFrenzy			 			string	Discs FALSE
		/call setDeclare AugmentedFrenzyEffect 				string 	outer FALSE
	}

	| Frenzied Line
	/if (${Me.Level} >= 94)  {
		/call setINIVar DiscFrenzyResolve							string 	Discs "Frenzied Resolve"
	} else {
		/call setINIVar DiscFrenzyResolve			 				string	Discs FALSE
	}
	
	/call setINIVar UntamedgRage										string 	Discs "Untamed Rage"
	/call setINIVar RecklessAbandon									string 	Discs "Reckless Abandon"
 	/call setINIVar frenzy													bool		meleedisc FALSE

 	| Dichotomic Line
	/if (${Me.Level} >= 101)  {
		/call setINIVar DiscDicho											string 	Discs "Dichotomic Rage"
	} else {
		/call setINIVar DiscDicho			 								string	Discs FALSE
	}

	| Disconcerting Line
	/if (${Me.Level} >= 108)  {
		/call setINIVar DiscDisconcert								string 	Discs "Mangling Discipline"
	} else /if (${Me.Level} >= 104)  {
		/call setINIVar DiscDisconcert 								string 	Discs "Disconcerting Discipline"
	} else {
		/call setINIVar DiscDisconcert			 					string	Discs FALSE
	}

	| Endurance Tap Line
	/if (${Me.Level} >= 103)  {
		/call setINIVar DiscEnduranceTap							string 	Discs "Sapping Strikes"
	} else {
		/call setINIVar DiscEnduranceTap			 				string	Discs FALSE
	}

	| Pet Line
	/if (${Me.Level} >= 100)  {
		/call setINIVar DiscPet												string 	Discs "Phantom Assailant"
	} else {
		/call setINIVar DiscPet			 									string	Discs FALSE
	}

	/call setINIVar ${Me.Class.ShortName}Point 			int 		Heal 60

	| Open Wound Line
	/if (${Me.Level} >= 106)  {
		/call setINIVar DiscOpenWound									string 	Discs "Ritual Scarification"
		/call setDeclare OpenWoundEffect							string 	outer "Ritual Scarification"
	} else /if (${Me.Level} >= 96)  {
		/call setINIVar DiscOpenWound 								string 	Discs "Open Wound"
		/call setDeclare OpenWoundEffect							string 	outer "Open Wound"
	} else {
		/call setINIVar DiscOpenWound			 						string	Discs FALSE
		/call setDeclare OpenWoundEffect							string 	outer FALSE
	}

	/call setINIVar buffGem 												string 	Spells FALSE
	/call setINIVar spellCripple 										string 	Spells FALSE

/return		                                        		



|***
 *	DES: Bard base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_BRD()
	/declare _count int local 0
/return



|***
 *	DES: Beastlord base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_BST()
	/declare _count int local 0
/return



|***
 *	DES: Cleric base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_CLR()
	/declare _count int local 0
	
	/call setDeclare CLRspire1											string	outer "Fundament: First Spire of Divinity"
	/call setDeclare CLRspire2											string	outer "Fundament: Second Spire of Divinity"
	/call setDeclare CLRspire3											string	outer "Fundament: Third Spire of Divinity"
	/call setDeclare CLRsynergyAA										string	outer "Templar's Synergy"
	/call setDeclare CLREpic1												string	outer "Water Sprinkler of Nem Ankh"
	/call setDeclare CLREpic15											string	outer "Harmony of the Soul"
	/call setDeclare CLREpic2												string	outer "Aegis of Superior Divinity"
	/call setINIVar CLRAlliance											string	Spells "Divine Alliance"
	/call setDeclare CLRAAUnity											string	outer "Saint's Unity"
	/call setDeclare CLRAAFade											string	outer "Divine Peace"
	
	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50

	/call setTimer	tHealWard												string	Timer 185s timer_heal_ward
	/call setINIVar switchLockMA										bool 		Heal FALSE
	/call setINIVar switchModRod										bool	 	Buffs TRUE
	/call setINIVar switchVP												bool		Buffs TRUE
	/call setINIVar useVPPct												int			Buffs 40
	/call setINIVar spellCripple 										string 	Spells FALSE

	/call setINIVar BreastplateHeal		 							string	Items FALSE
	/call setINIVar BreastplateHealCondition				string	Items FALSE		

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
	/call setINIVar setDivinetoGroupHeal						bool		Heal FALSE

  /call setINIVar useCelestialRegen								bool	  Heal TRUE
	/call setINIVar CRTXT														string	Heal "Celestial Regeneration"
	
	/call setINIVar XTHealRadius										int			Heal 300
	/call setINIVar switchForceGroupHeal						bool		Heal FALSE
	/call setINIVar RampageTank											string	Heal FALSE
	/call setINIVar switchHealAggressive						bool		Heal FALSE
	/call setINIVar switchCompleteHeal							bool		Heal FALSE
	/call setINIVar CompleteHealpct									int 		Heal 70	

	| group heals (not cures) *mostly                                            		
	/if (${Me.Level} >= 108) {
	  /call setINIVar healWholeGroup 								string  Heal "Syllable of Mending"
	} else /if (${Me.Level} >= 103) {	
	  /call setINIVar healWholeGroup 								string  Heal "Syllable of Convalescence"
	} else /if (${Me.Level} >= 98) {	
	  /call setINIVar healWholeGroup 								string  Heal "Syllable of Renewal"
	} else /if (${Me.Level} >= 96) {	
	  /call setINIVar healWholeGroup 								string  Heal "Word of Renewal"
	} else /if (${Me.Level} >= 91) {	
	  /call setINIVar healWholeGroup 								string  Heal "Word of Recuperation"
	} else /if (${Me.Level} >= 86) {	
	  /call setINIVar healWholeGroup 								string  Heal "Word of Awakening"
	} else /if (${Me.Level} >= 80) {	
	  /call setINIVar healWholeGroup 								string  Heal "Word of Vivacity"
	} else /if (${Me.Level} >= 69) {	
	  /call setINIVar healWholeGroup 								string  Heal "Word of Vivification"
	} else /if (${Me.Level} >= 64) {	
	  /call setINIVar healWholeGroup 								string  Heal "Word of Replenishment"
	} else /if (${Me.Level} >= 60) {	
	  /call setINIVar healWholeGroup 								string  Heal "Word of Redemption"
	} else /if (${Me.Level} >= 57) {	
	  /call setINIVar healWholeGroup 								string  Heal "Word of Restoration"
	} else /if (${Me.Level} >= 52) {	
	  /call setINIVar healWholeGroup 								string  Heal "Word of Vigor"
	} else /if (${Me.Level} >= 45) {	
	  /call setINIVar healWholeGroup 								string  Heal "Word of Healing"
	} else /if (${Me.Level} >= 30) {	
	  /call setINIVar healWholeGroup 								string  Heal "Word of Health"
	} else {	
	  /call setINIVar healWholeGroup 								string  Heal FALSE
	}
  /call setINIVar healWholeGroup2									string 	Heal FALSE
  /call setINIVar healWholeGroup3									string 	Heal FALSE

	| group HoT
 	/if (${Me.Level} >= 110) {
	  /call setINIVar healWholeGroupHoT 						string  Heal "Elixir of Wulthan"
	} else /if (${Me.Level} >= 105) {	
	  /call setINIVar healWholeGroupHoT 						string  Heal "Elixir of the Seas"
	} else /if (${Me.Level} >= 100) {	
	  /call setINIVar healWholeGroupHoT 						string  Heal "Elixir of the Acquittal"
	} else /if (${Me.Level} >= 95) {	
	  /call setINIVar healWholeGroupHoT 						string  Heal "Elixir of the Beneficent"
	} else /if (${Me.Level} >= 90) {	
	  /call setINIVar healWholeGroupHoT 						string  Heal "Elixir of the Ardent"
	} else /if (${Me.Level} >= 85) {	
	  /call setINIVar healWholeGroupHoT 						string  Heal "Elixir of Expiation"
	} else /if (${Me.Level} >= 80) {	
	  /call setINIVar healWholeGroupHoT 						string  Heal "Elixir of Atonment"
	} else /if (${Me.Level} >= 75) {	
	  /call setINIVar healWholeGroupHoT 						string  Heal "Elixir of Redemption"
	} else /if (${Me.Level} >= 70) {	
	  /call setINIVar healWholeGroupHoT 						string  Heal "Elixir of Divinity"
	} else /if (${Me.Level} >= 60) {	
	  /call setINIVar healWholeGroupHoT 						string  Heal "Etheral Elixir"
	} else {	
	  /call setINIVar healWholeGroupHoT 						string  Heal FALSE
	}

	| remedies
 	/if (${Me.Level} >= 106) {
	  /call setINIVar healRemedy1										string 	Heal "Merciful Remedy"
	  /call setINIVar healRemedy2										string 	Heal "Spiritual Remedy"
		/call setINIVar healRemedy3										string 	Heal "Graceful Remedy"
	} else /if (${Me.Level} >= 101) {	
	  /call setINIVar healRemedy1										string 	Heal "Spiritual Remedy"
	  /call setINIVar healRemedy2										string 	Heal "Graceful Remedy"
		/call setINIVar healRemedy3										string 	Heal "Faithful Remedy"
	} else /if (${Me.Level} >= 96) {	
	  /call setINIVar healRemedy1										string 	Heal "Graceful Remedy"
	  /call setINIVar healRemedy2										string 	Heal "Faithful Remedy"
		/call setINIVar healRemedy3										string 	Heal "Earnest Remedy"
	} else /if (${Me.Level} >= 91) {	
	  /call setINIVar healRemedy1										string 	Heal "Faithful Remedy"
	  /call setINIVar healRemedy2										string 	Heal "Earnest Remedy"
		/call setINIVar healRemedy3										string 	Heal "Devout Remedy"
	} else /if (${Me.Level} >= 86) {	
	  /call setINIVar healRemedy1										string 	Heal "Earnest Remedy"
	  /call setINIVar healRemedy2										string 	Heal "Devout Remedy"
		/call setINIVar healRemedy3										string 	Heal "Solemn Remedy"
	} else /if (${Me.Level} >= 81) {	
	  /call setINIVar healRemedy1										string 	Heal "Devout Remedy"
	  /call setINIVar healRemedy2										string 	Heal "Solemn Remedy"
		/call setINIVar healRemedy3										string 	Heal "Sacred Remedy"
	} else /if (${Me.Level} >= 76) {	
	  /call setINIVar healRemedy1										string 	Heal "Solemn Remedy"
	  /call setINIVar healRemedy2										string 	Heal "Sacred Remedy"
		/call setINIVar healRemedy3										string 	Heal "Pious Remedy"
	} else /if (${Me.Level} >= 71) {	
	  /call setINIVar healRemedy1										string 	Heal "Sacred Remedy"
	  /call setINIVar healRemedy2										string 	Heal "Pious Remedy"
		/call setINIVar healRemedy3										string 	Heal "Supernal Remedy"
	} else /if (${Me.Level} >= 66) {	
	  /call setINIVar healRemedy1										string 	Heal "Pious Remedy"
	  /call setINIVar healRemedy2										string 	Heal "Supernal Remedy"
		/call setINIVar healRemedy3										string 	Heal "Ethereal Remedy"
	} else /if (${Me.Level} >= 61) {	
	  /call setINIVar healRemedy1										string 	Heal "Supernal Remedy"
	  /call setINIVar healRemedy2										string 	Heal "Ethereal Remedy"
		/call setINIVar healRemedy3										string 	Heal FALSE
	} else /if (${Me.Level} >= 59) {	
	  /call setINIVar healRemedy1										string 	Heal "Ethereal Remedy"
	  /call setINIVar healRemedy2										string 	Heal FALSE
		/call setINIVar healRemedy3										string 	Heal FALSE
	} else {	
	  /call setINIVar healRemedy1										string 	Heal FALSE
	  /call setINIVar healRemedy2										string 	Heal FALSE
		/call setINIVar healRemedy3										string 	Heal FALSE
	}

  /call setINIVar healHoT 												string 	Heal FALSE

	| target AE splash heals
 	/if (${Me.Level} >= 110) {
	  /call setINIVar healSplash 										string  Heal "Mending Splash"
	} else /if (${Me.Level} >= 105) {	
	  /call setINIVar healSplash 										string  Heal "Convalescent Splash"
	} else /if (${Me.Level} >= 100) {	
	  /call setINIVar healSplash 										string  Heal "Reforming Splash"
	} else /if (${Me.Level} >= 95) {	
	  /call setINIVar healSplash 										string  Heal "Rejuvenating Splash"
	} else /if (${Me.Level} >= 90) {	
	  /call setINIVar healSplash 										string  Heal "Healing Splash"
	} else {	
	  /call setINIVar healSplash 										string  Heal FALSE
	}

	| interventions
	/if (${Me.Level} >= 108) {
	  /call setINIVar healIntervention1 						string  Heal "Merciful Intervention"
	  /call setINIVar healIntervention2 						string  Heal "Mystical Intervention"
	  /call setINIVar healIntervention3 						string 	Heal "Virtuous Intervention"
	  /call setINIVar healIntervention4 						string  Heal "Elysian Intervention"
	} else /if (${Me.Level} >= 103)  {
	  /call setINIVar healIntervention1 						string  Heal "Mystical Intervention"
	  /call setINIVar healIntervention2 						string  Heal "Virtuous Intervention"
	  /call setINIVar healIntervention3 						string 	Heal "Elysian Intervention"
	  /call setINIVar healIntervention4 						string  Heal "Celestial Intervention"
	} else /if (${Me.Level} >= 98)  {
	  /call setINIVar healIntervention1 						string  Heal "Virtuous Intervention"
	  /call setINIVar healIntervention2 						string  Heal "Elysian Intervention"
	  /call setINIVar healIntervention3 						string 	Heal "Celestial Intervention"
	  /call setINIVar healIntervention4 						string  Heal "Holy Intervention"
	} else /if (${Me.Level} >= 93)  {
	  /call setINIVar healIntervention1 						string  Heal "Elysian Intervention"
	  /call setINIVar healIntervention2 						string  Heal "Celestial Intervention"
	  /call setINIVar healIntervention3 						string 	Heal "Holy Intervention"
	  /call setINIVar healIntervention4 						string  Heal FALSE
	} else /if (${Me.Level} >= 88)  {
	  /call setINIVar healIntervention1 						string  Heal "Celestial Intervention"
	  /call setINIVar healIntervention2 						string  Heal "Holy Intervention"
	  /call setINIVar healIntervention3 						string 	Heal FALSE
	  /call setINIVar healIntervention4 						string  Heal FALSE
	} else /if (${Me.Level} >= 83)  {
	  /call setINIVar healIntervention1 						string  Heal "Holy Intervention"
	  /call setINIVar healIntervention2 						string  Heal FALSE
	  /call setINIVar healIntervention3 						string 	Heal FALSE
	  /call setINIVar healIntervention4 						string  Heal FALSE
	} else {
	  /call setINIVar healIntervention1 						string  Heal FALSE
	  /call setINIVar healIntervention2 						string  Heal FALSE
	  /call setINIVar healIntervention3 						string 	Heal FALSE
	  /call setINIVar healIntervention4 						string  Heal FALSE
	}

	| renewals
	/if (${Me.Level} >= 110) {
	  /call setINIVar healRenewal1 									string  Heal "Furial Renewal"
	  /call setINIVar healRenewal2 									string  Heal "Fervid Renewal"
	  /call setINIVar healRenewal3 									string 	Heal "Fraught Renewal"
	} else /if (${Me.Level} >= 105) {	
	  /call setINIVar healRenewal1 									string  Heal "Fervid Renewal"
	  /call setINIVar healRenewal2 									string  Heal "Fraught Renewal"
	  /call setINIVar healRenewal3 									string 	Heal "Fervent Renewal"
	} else /if (${Me.Level} >= 100) {	
	  /call setINIVar healRenewal1 									string  Heal "Fraught Renewal"
	  /call setINIVar healRenewal2 									string  Heal "Fervent Renewal"
	  /call setINIVar healRenewal3 									string 	Heal "Frenetic Renewal"
	} else /if (${Me.Level} >= 95) {	
	  /call setINIVar healRenewal1 									string  Heal "Fervent Renewal"
	  /call setINIVar healRenewal2 									string  Heal "Frenetic Renewal"
	  /call setINIVar healRenewal3 									string 	Heal "Desperate Renewal"
	} else /if (${Me.Level} >= 85) {	
	  /call setINIVar healRenewal1 									string  Heal "Frenetic Renewal"
	  /call setINIVar healRenewal2 									string  Heal "Desperate Renewal"
	  /call setINIVar healRenewal3 									string 	Heal FALSE
	} else /if (${Me.Level} >= 70) {	
	  /call setINIVar healRenewal1 									string  Heal "Desperate Renewal"
	  /call setINIVar healRenewal2 									string  Heal FALSE
	  /call setINIVar healRenewal3 									string 	Heal FALSE
	} else {	
	  /call setINIVar healRenewal1 									string  Heal FALSE
	  /call setINIVar healRenewal2 									string  Heal FALSE
	  /call setINIVar healRenewal3 									string 	Heal FALSE
	}

	| panic heals
 	/if (${Me.Level} >= 107) {
	  /call setINIVar healPanic 										string  Heal "Sixteenth Serenity"
	} else /if (${Me.Level} >= 97) {	
	  /call setINIVar healPanic 										string  Heal "Fifteenth Emblem"
	} else /if (${Me.Level} >= 93) {	
	  /call setINIVar healPanic 										string  Heal "Fourteenth Catalyst"
	} else /if (${Me.Level} >= 87) {	
	  /call setINIVar healPanic 										string  Heal "Thirteenth Salve"
	} else /if (${Me.Level} >= 82) {	
	  /call setINIVar healPanic 										string  Heal "Twelfth Night"
	} else /if (${Me.Level} >= 77) {	
	  /call setINIVar healPanic 										string  Heal "Eleventh-Hour"
	} else {	
	  /call setINIVar healPanic 										string  Heal FALSE
	}
   
  | wards
 	/if (${Me.Level} >= 107) {
	  /call setINIVar healWard			 								string  Heal "Ward of Assurance"
	} else /if (${Me.Level} >= 102) {	
	  /call setINIVar healWard			 								string  Heal "Ward of Surety"
	} else /if (${Me.Level} >= 97) {	
	  /call setINIVar healWard			 								string  Heal "Ward of Certitude"
	} else {	
	  /call setINIVar healWard			 								string  Heal FALSE
	}
  /call setINIVar healWardToon	 									string  Heal "|FALSE|FALSE"

	| healing pet shield
	/call setINIVar switchIssuance									bool		Heal FALSE
	/call setINIVar issuanceMobCount								int			Heal 4
	/if (${Me.Level} >= 108) {
	  /call setINIVar healIssuance 									string  Heal "Issuance of Mercy"
	} else /if (${Me.Level} >= 103) {	
	  /call setINIVar healIssuance 									string  Heal "Issuance of Spirit"
	} else /if (${Me.Level} >= 98) {	
	  /call setINIVar healIssuance 									string  Heal "Issuance of Grace"
	} else /if (${Me.Level} >= 93) {	
	  /call setINIVar healIssuance 									string  Heal "Issuance of Faith"
	} else {	
	  /call setINIVar healIssuance 									string  Heal FALSE
	}
	
	| yodeling anyone?                                       		
	/call setINIVar doYaulp 												bool		Buffs TRUE
	/call setINIVar useYaulpPct											int			Buffs 98
 	/if (${Me.Level} >= 106) {
	  /call setINIVar spellYaulp 										string  Buffs "Yaulp XV"
	} else /if (${Me.Level} >= 101) {	
	  /call setINIVar spellYaulp 										string  Buffs "Yaulp XIV"
	} else /if (${Me.Level} >= 96) {	
	  /call setINIVar spellYaulp 										string  Buffs "Yaulp XIII"
	} else /if (${Me.Level} >= 91) {	
	  /call setINIVar spellYaulp 										string  Buffs "Yaulp XII"
	} else /if (${Me.Level} >= 86) {	
	  /call setINIVar spellYaulp 										string  Buffs "Yaulp XI"
	} else /if (${Me.Level} >= 81) {	
	  /call setINIVar spellYaulp 										string  Buffs "Yaulp X"
	} else /if (${Me.Level} >= 76) {	
	  /call setINIVar spellYaulp 										string  Buffs "Yaulp IX"
	} else /if (${Me.Level} >= 71) {	
	  /call setINIVar spellYaulp 										string  Buffs "Yaulp VIII"
	} else /if (${Me.Level} >= 69) {	
	  /call setINIVar spellYaulp 										string  Buffs "Yaulp VII"
	} else /if (${Me.Level} >= 65) {	
	  /call setINIVar spellYaulp 										string  Buffs "Yaulp VI"
	} else /if (${Me.Level} >= 56) {	
	  /call setINIVar spellYaulp 										string  Buffs "Yaulp V"
	} else /if (${Me.Level} >= 53) {	
	  /call setINIVar spellYaulp 										string  Buffs "Yaulp IV"
	} else /if (${Me.Level} >= 41) {	
	  /call setINIVar spellYaulp 										string  Buffs "Yaulp III"
	} else /if (${Me.Level} >= 16) {	
	  /call setINIVar spellYaulp 										string  Buffs "Yaulp II"
	} else /if (${Me.Level} >= 1) {	
	  /call setINIVar spellYaulp 										string  Buffs "Yaulp"
	} 
	
	| hammer of ass kickery	
 	/if (${Me.Level} >= 107) {
	  /call setINIVar sumWeaponSpell 								string  Buffs "Hammer of Repudation"
	  /call setINIVar sumWeaponItem 								string  Buffs "Hammer of Repudation"
	} else /if (${Me.Level} >= 97) {	
	  /call setINIVar sumWeaponSpell 								string  Buffs "Hammer of Reverence"
	  /call setINIVar sumWeaponItem 								string  Buffs "Hammer of Reverence III"
	} else /if (${Me.Level} >= 68) {	
	  /call setINIVar sumWeaponSpell 								string  Buffs "Hammer of Reproach"
	  /call setINIVar sumWeaponItem 								string  Buffs "Hammer of Reproach"
	} else /if (${Me.Level} >= 63) {	
	  /call setINIVar sumWeaponSpell 								string  Buffs "Hammer of Damnation"
	  /call setINIVar sumWeaponItem 								string  Buffs "Hammer of Damnation"
	} else /if (${Me.Level} >= 60) {	
	  /call setINIVar sumWeaponSpell 								string  Buffs "Hammer of Souls"
	  /call setINIVar sumWeaponItem 								string  Buffs "Hammer of Souls"
	} else /if (${Me.Level} >= 58) {	
	  /call setINIVar sumWeaponSpell 								string  Buffs "Hammer of Divinity"
	  /call setINIVar sumWeaponItem 								string  Buffs "Hammer of Divinity"
	} else /if (${Me.Level} >= 56) {	
	  /call setINIVar sumWeaponSpell 								string  Buffs "Hammer of Judgment"
	  /call setINIVar sumWeaponItem 								string  Buffs "Hammer of Judgment"
	} else /if (${Me.Level} >= 40) {	
	  /call setINIVar sumWeaponSpell 								string  Buffs "Hammer of Requital"
	  /call setINIVar sumWeaponItem 								string  Buffs "Hammer of Requital"
	} else /if (${Me.Level} >= 20) {	
	  /call setINIVar sumWeaponSpell 								string  Buffs "Hammer of Striking"
	  /call setINIVar sumWeaponItem 								string  Buffs "Hammer of Striking"
	} else /if (${Me.Level} >= 7) {	
	  /call setINIVar sumWeaponSpell 								string  Buffs "Hammer of Wrath"
	  /call setINIVar sumWeaponItem 								string  Buffs "Hammer of Wrath"
	} else {	
	  /call setINIVar sumWeaponSpell 								string  Buffs FALSE
	  /call setINIVar sumWeaponItem 								string  Buffs FALSE
	}
	
	| cute hammer pet
 	/if (${Me.Level} >= 109) {
	  /call setINIVar tempPet 											string  Pet "Unyielding Hammer of Zeal"
	} else /if (${Me.Level} >= 104) {	
	  /call setINIVar tempPet 											string  Pet "Ardent Hammer of Zeal"
	} else /if (${Me.Level} >= 99) {	
	  /call setINIVar tempPet 											string  Pet "Infallible Hammer of Reverence"
	} else /if (${Me.Level} >= 94) {	
	  /call setINIVar tempPet 											string  Pet "Imfallible Hammer of Zeal"
	} else /if (${Me.Level} >= 89) {	
	  /call setINIVar tempPet 											string  Pet "Devout Hammer of Zeal"
	} else /if (${Me.Level} >= 84) {	
	  /call setINIVar tempPet 											string  Pet "Unwavering Hammer of Zeal"
	} else /if (${Me.Level} >= 79) {	
	  /call setINIVar tempPet 											string  Pet "Unflinching Hammer of Zeal"
	} else /if (${Me.Level} >= 68) {	
	  /call setINIVar tempPet 											string  Pet "Unswerving Hammer of Retribution"
	} else /if (${Me.Level} >= 54) {	
	  /call setINIVar tempPet 											string  Pet "Unswerving Hammer of Faith"
	} else {	
	  /call setINIVar tempPet 											string  Pet FALSE
	}
	| /call setINIVar tempPetCondition 								string  Pet FALSE
	
	
	
	| Nukes
	/call setINIVar DoNuke					 								int			Spells 0
	/for _count 1 to 6
		/call setINIVar Nuke${_count}									string	Spells FALSE
		/call setINIVar Nuke${_count}Condition				string	Spells FALSE
	/next _count
	
	| DoTs
	/call setINIVar DoDoT								 						int			Spells 0
	/for _count 1 to 6
		/call setINIVar DoT${_count}									string	Spells FALSE
		/call setINIVar DoT${_count}chk								string	Spells FALSE
		/call setINIVar DoT${_count}Condition					string	Spells FALSE
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

	/call setDeclare DRUspire1											string	outer "Fundament: First Spire of Nature"
	/call setDeclare DRUspire2											string	outer "Fundament: Second Spire of Nature"
	/call setDeclare DRUspire3											string	outer "Fundament: Third Spire of Nature"
	/call setDeclare DRUsynergyAA										string	outer "Preserver's Synergy"
	/call setDeclare DRUEpic1												string	outer "Nature Walker's Scimitar"
	/call setDeclare DRUEpic15											string	outer "Staff of Living Brambles"
	/call setDeclare DRUEpic2												string	outer "Staff of Everliving Brambles"
	/call setINIVar DRUAlliance											string	Spells "Bosquetender's Alliance"
	/call setDeclare DRUAAUnity											string	outer "Wildtender's Unity"
	/call setDeclare DRUAAFade											string	outer "Veil of the Underbrush"

	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50

	/call setINIVar switchModRod										bool	 	Buffs TRUE
	
	/call setINIVar debuffRo			 									string	Spells "Blessing of Ro"	
	/call setINIVar WoodTXT													string	Spells "SoWood cast."
                                        		
	/call setINIVar spellCripple 										string 	Spells FALSE
	/call setINIVar AAWolfSpirit										string 	Buffs FALSE


	


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
	













	| Nukes
	/call setINIVar DoNuke					 								int			Spells 0
	/for _count 1 to 6
		/call setINIVar Nuke${_count}									string	Spells FALSE
		/call setINIVar Nuke${_count}Condition				string	Spells FALSE
	/next _count
	
	| DoTs
	/call setINIVar DoDoT								 						int			Spells 0
	/for _count 1 to 6
		/call setINIVar DoT${_count}									string	Spells FALSE
		/call setINIVar DoT${_count}chk								string	Spells FALSE
		/call setINIVar DoT${_count}Condition					string	Spells FALSE
	/next _count





















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
 *	DES: Enchanter base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_ENC()
	/declare _count int local 0

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
	/call setINIVar ENCAlliance											string	Spells "Chromatic Alliance"
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
	
	
	/call setINIVar runeBrimstone 									string	Rune ""
| 109 Brimstone Durability
| 104 Brimstone Resolution
| 99 Brimstone Eminence

	
	
	/call setINIVar runeSpray			 									string	Rune "Glyph Spray"
	/call setINIVar runeReactive	 									string	Rune "Reactive Rune"
	/call setINIVar runePoly			 									string	Rune "Polyrefractive Rune"
	/call setINIVar runeUnity			 									string	Rune FALSE
	/call setINIVar runeWard			 									string	Rune "Ward of the Enticer"
	
	/call setINIVar runeAuspice		 									string	Rune ""
| 109 Tenebrous Auspice
| 104 Darkened Auspice
| 99 Umbral Auspice

	/call setINIVar runeAuspiceToon									string  Rune |FALSE



	
	/call setINIVar runeMind												string  Rune "Mind Over Matter"
	
	/call setINIVar runeEntAuspice									string	Rune "Enticer's Auspice"
	
	/call setINIVar runeLegion											string	Rune "Legion of Lucem"
	
	/call setINIVar runeShieldofFate								string	Rune "Shield of Consequence"


	| aa crystal items
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
	
	
	
	
	
	
	
	/call setINIVar ${Me.Class.ShortName}Point 			int 		Heal 80
	/call setINIVar switchModRod										bool	 	Buffs TRUE
















	| Nukes
	/call setINIVar DoNuke					 								int			Spells 0
	/for _count 1 to 6
		/call setINIVar Nuke${_count}									string	Spells FALSE
		/call setINIVar Nuke${_count}Condition				string	Spells FALSE
	/next _count	
	
	| DoTs
	/call setINIVar DoDoT								 						int			Spells 0
	/for _count 1 to 6
		/call setINIVar DoT${_count}									string	Spells FALSE
		/call setINIVar DoT${_count}chk								string	Spells FALSE
		/call setINIVar DoT${_count}Condition					string	Spells FALSE
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

	/call setDeclare PALspire1											string	outer "Fundament: First Spire of Holiness"
	/call setDeclare PALspire2											string	outer "Fundament: Second Spire of Holiness"
	/call setDeclare PALspire3											string	outer "Fundament: Third Spire of Holiness"
	/call setDeclare PALsynergyAA										string	outer "Knight's Synergy"
	/call setDeclare PALEpic1												string	outer "Fiery Defender"
	/call setDeclare PALEpic15											string	outer "Heartwood Blade"
	/call setDeclare PALEpic2												string	outer "Nightbane, Sword of the Valiant"
	/call setINIVar PALAlliance											string	Spells "Holy Alliance"
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
  /call setINIVar splashCure											string 	Heal "Splash of Depuration"
  /call setINIVar healPanic1											string 	Heal "Expiation"
  /call setINIVar healPanic2											string 	Heal "Grief"
  /call setINIVar healTouch												string 	Heal "Merciful Touch"
	/call setINIVar useGroupHeal 										bool		Heal TRUE
	/call setINIVar XTHealRadius										int			Heal 300
	/call setINIVar RampageTank											string	Heal FALSE
	/call setINIVar RandRezDelay										int 		Heal 5
	
	/call setINIVar doYaulp 												bool		Buffs TRUE
	/call setINIVar useYaulpPct											int			Buffs 98

	| yaulp attack buff
	/if (${Me.Level} >= 60)  {
		/call setINIVar spellYaulp			 							string	Buffs "Yaulp IV"
	} else /if (${Me.Level} >= 56)  {
		/call setINIVar spellYaulp			 							string	Buffs "Yaulp III"
	} else /if (${Me.Level} >= 38)  {
		/call setINIVar spellYaulp			 							string	Buffs "Yaulp II"
	} else /if (${Me.Level} >= 9)  {
		/call setINIVar spellYaulp			 							string	Buffs "Yaulp"
	} else {
		/call setINIVar spellYaulp					 					string	Buffs FALSE
	}		

	| rest disc
	/if (${Me.Level} >= 101)  {
		/call setINIVar DiscRest			 								string	Discs "Breather"
	} else /if (${Me.Level} >= 96)  {
		/call setINIVar DiscRest			 								string	Discs "Rest"
	} else /if (${Me.Level} >= 91)  {
		/call setINIVar DiscRest			 								string	Discs "Reprive"
	} else /if (${Me.Level} >= 86)  {
		/call setINIVar DiscRest			 								string	Discs "Respite"
	} else {
		/call setINIVar DiscRest			 								string	Discs FALSE
	}	
	
	| fast rune
	/if (${Me.Level} >= 108)  {
		/call setINIVar DiscFastRune			 						string	Discs "Spurn"
	} else /if (${Me.Level} >= 103)  {
		/call setINIVar DiscFastRune			 						string	Discs "Repel"
	} else /if (${Me.Level} >= 98)  {
		/call setINIVar DiscFastRune			 						string	Discs "Reprove"
	} else /if (${Me.Level} >= 93)  {
		/call setINIVar DiscFastRune			 						string	Discs "Renounce"
	} else {
		/call setINIVar DiscFastRune			 						string	Discs FALSE
	}	

	| armor disc
	/if (${Me.Level} >= 108)  {
		/call setINIVar DefensiveDisc1			 					string	Discs "Armor of Mercy"
	} else /if (${Me.Level} >= 103)  {
		/call setINIVar DefensiveDisc1			 					string	Discs "Armor of Ardency"
	} else /if (${Me.Level} >= 98)  {
		/call setINIVar DefensiveDisc1			 					string	Discs "Armor of Reverence"
	} else /if (${Me.Level} >= 93)  {
		/call setINIVar DefensiveDisc1			 					string	Discs "Armor of Zeal"
	} else {
		/call setINIVar DefensiveDisc1			 					string	Discs FALSE
	}	

	| mantle disc
	/if (${Me.Level} >= 107)  {
		/call setINIVar DefensiveDisc2			 					string	Discs "Kar`Zok Mantle"
	} else /if (${Me.Level} >= 103)  {
		/call setINIVar DefensiveDisc2			 					string	Discs "Skalber Mantle"
	} else /if (${Me.Level} >= 98)  {
		/call setINIVar DefensiveDisc2			 					string	Discs "Bright Wing Mantle"
	} else /if (${Me.Level} >= 93)  {
		/call setINIVar DefensiveDisc2			 					string	Discs "Prominent Mantle"
	} else {
		/call setINIVar DefensiveDisc2			 					string	Discs FALSE
	}		

	| guardian disc
	/if (${Me.Level} >= 107)  {
		/call setINIVar DefensiveDisc3			 					string	Discs "Blessed Guardian Discipline"
	} else /if (${Me.Level} >= 98)  {
		/call setINIVar DefensiveDisc3			 					string	Discs "Holy Guardian Discipline"
	} else {
		/call setINIVar DefensiveDisc3			 					string	Discs FALSE
	}			

	/call setINIVar defCount												int			Discs 2
                                          	
	/call setINIVar DiscUnflinching									string	Discs "Unflinching Affirmation"
	/call setINIVar DiscReflexive										string	Discs "Reflexive Reverence"

	/call setINIVar AAHatePet												string	Discs "Projection of Piety"
	/call setINIVar DiscTwincast										string	Discs "Hand of Tunare"
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
	                                         	
	| Nukes
	/call setINIVar DoNuke					 								int			Spells 0
	/for _count 1 to 6
		/call setINIVar Nuke${_count}									string	Spells FALSE
		/call setINIVar Nuke${_count}Condition				string	Spells FALSE
	/next _count                                           	

	| DoTs
	/call setINIVar DoDoT								 						int			Spells 0
	/for _count 1 to 6
		/call setINIVar DoT${_count}									string	Spells FALSE
		/call setINIVar DoT${_count}chk								string	Spells FALSE
		/call setINIVar DoT${_count}Condition					string	Spells FALSE
	/next _count
	
	/call setINIVar spellCripple 										string 	Spells FALSE
	
	| single target challenge spell
	/if (${Me.Level} >= 107)  {
		/call setINIVar ChallengeSpell			 					string	Spells "Refute for Honor"
	} else /if (${Me.Level} >= 102)  {
		/call setINIVar ChallengeSpell			 					string	Spells "Impose for Honor"
	} else /if (${Me.Level} >= 97)  {
		/call setINIVar ChallengeSpell			 					string	Spells "Demand for Honor"
	} else /if (${Me.Level} >= 87)  {
		/call setINIVar ChallengeSpell			 					string	Spells "Confrontation for Honor"
	} else /if (${Me.Level} >= 82)  {
		/call setINIVar ChallengeSpell			 					string	Spells "Charge for Honor"
	} else /if (${Me.Level} >= 77)  {
		/call setINIVar ChallengeSpell			 					string	Spells "Trial for Honor"
	} else /if (${Me.Level} >= 72)  {
		/call setINIVar ChallengeSpell			 					string	Spells "Challenge for Honor"
	} else {
		/call setINIVar ChallengeSpell			 					string	Spells FALSE
	}		

	| def proc spell
	/if (${Me.Level} >= 107)  {
		/call setINIVar ValiantDefense			 					string	Spells "Valiant Defense"
	} else /if (${Me.Level} >= 98)  {
		/call setINIVar ValiantDefense			 					string	Spells "Valiant Deflection"
	} else {
		/call setINIVar ValiantDefense			 					string	Spells FALSE
	}		

	/call setINIVar Twincast												string 	Spells "Glorious Exoneration"
	/call setINIVar switchModRod										bool	 	Buffs TRUE

	/call setINIVar switchHarmonious 								bool	 	Buffs TRUE
	/call setINIVar Harmonious 											string 	Buffs "Harmonious Blessing"

	| steel weapon heal proc
	/call setINIVar switchSteel			 								bool	 	Buffs TRUE
	/call setINIVar Steel			 											string 	Buffs ""
	/if (${Me.Level} >= 109)  {
		/call setINIVar Steel			 										string	Buffs "Revitalizating Steel"
	} else /if (${Me.Level} >= 99)  {
		/call setINIVar Steel			 										string	Buffs "Reinvigorating Steel"
	} else {
		/call setINIVar Steel			 										string	Buffs FALSE
	}		
	
	| stance temp HP buff
	/call setINIVar switchStance 										bool	 	Buffs FALSE
	/if (${Me.Level} >= 109)  {
		/call setINIVar Stance			 									string	Buffs "Defiant Stance"
	} else /if (${Me.Level} >= 104)  {
		/call setINIVar Stance			 									string	Buffs "Staunch Stance"
	} else /if (${Me.Level} >= 99)  {
		/call setINIVar Stance			 									string	Buffs "Steadfast Stance"
	} else /if (${Me.Level} >= 89)  {
		/call setINIVar Stance			 									string	Buffs "Stubborn Stance"
	} else /if (${Me.Level} >= 84)  {
		/call setINIVar Stance			 									string	Buffs "Steely Stance"
	} else {
		/call setINIVar Stance			 									string	Buffs FALSE
	}		
                                        		
	/call setINIVar useMG														bool		Buffs TRUE
	/call setINIVar useMGPct												int			Buffs 60
	/call setINIVar useShieldFlash									bool		Discs FALSE
	/call setINIVar useShieldFlashPct								int			Discs 20
	/call setINIVar useLoH													bool		Heal FALSE
	/call setINIVar useLoHPct												int			Heal 10
	/call setINIVar switchAttraction								bool		Discs FALSE

/return		                                        		




|***
 *	DES: Ranger base variables
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub pregen_RNG()
	/declare _count int local 0
	
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
	/call setINIVar RNGAlliance											string	Spells "Bosquestalker's Alliance"
	/call setDeclare RNGAAUnity											string	outer "Wildstalker's Unity (Azia)"
	/call setDeclare RNGAAFade											string	outer "Cover Tracks"
	
	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50

	/call setINIVar AuspiceTXT											string	Spells "AUSPICE Lit"
	/call setINIVar spellCripple 										string 	Spells FALSE

	/call setINIVar AADefensive1 										string	Spells "Outrider's Evasion"
	/call setINIVar AADefensive2 										string	Spells "Protection of the Spirit Wolf"
	/call setINIVar AADefensive3 										string	Spells "Bulwark of the Brownies"
	/call setINIVar switchModRod										bool	 	Buffs TRUE
	/call setINIVar ${Me.Class.ShortName}Point		 	int 		Heal 80

	| Nukes
	/call setINIVar DoNuke					 								int			Spells 0
	/for _count 1 to 6
		/call setINIVar Nuke${_count}									string	Spells FALSE
		/call setINIVar Nuke${_count}Condition				string	Spells FALSE
	/next _count
                                          		
	| Summer's line
	/call setINIVar switchNukeSummer		 						bool		Spells TRUE
	/call setTimer tNuke_Summer											string	Timer 3s timer_cast_nuke_summer
	/if (${Me.Level} >= 109)  {
		/call setINIVar NukeSummer		 								string	Spells "Summer's Tempest"
	} else /if (${Me.Level} >= 104)  {
		/call setINIVar NukeSummer		 								string	Spells "Summer's Cyclone"
	} else /if (${Me.Level} >= 99)  {
		/call setINIVar NukeSummer		 								string	Spells "Summer's Gale"
	} else /if (${Me.Level} >= 89)  {
		/call setINIVar NukeSummer		 								string	Spells "Summer's Storm"
	} else /if (${Me.Level} >= 84)  {
		/call setINIVar NukeSummer		 								string	Spells "Summer's Mist"
	} else /if (${Me.Level} >= 79)  {
		/call setINIVar NukeSummer		 								string	Spells "Summer's Viridity"
	} else /if (${Me.Level} >= 74)  {
		/call setINIVar NukeSummer		 								string	Spells "Summer's Dew"
	} else {
		/call setINIVar NukeSummer		 								string	Spells FALSE
	}
	/call setINIVar NukeSummerCondition							string	Spells FALSE

	| DoTs
	/call setINIVar DoDoT								 						int			Spells 0
	/for _count 1 to 6
		/call setINIVar DoT${_count}									string	Spells FALSE
		/call setINIVar DoT${_count}chk								string	Spells FALSE
		/call setINIVar DoT${_count}Condition					string	Spells FALSE
	/next _count

	| ToT DS DoT / debuff
	/if (${Me.Level} >= 108)  {
		/call setINIVar DoTSwarmDS		 								string	Spells "Swarm of Polybiads"
		/call setINIVar DoTSwarmDSdeBuff 							string	Spells "Polybiad Swarm"
	} else /if (${Me.Level} >= 103)  {
		/call setINIVar DoTSwarmDS		 								string	Spells "Swarm of Glistenwings"
		/call setINIVar DoTSwarmDSdeBuff 							string	Spells "Glistenwing Swarm"
	} else /if (${Me.Level} >= 98)  {
		/call setINIVar DoTSwarmDS		 								string	Spells "Swarm of Vespines"
		/call setINIVar DoTSwarmDSdeBuff 							string	Spells "Vespines Swarm"
	} else /if (${Me.Level} >= 93)  {
		/call setINIVar DoTSwarmDS		 								string	Spells "Swarm of Sand Wasps"
		/call setINIVar DoTSwarmDSdeBuff 							string	Spells "Sand Wasp Swarm"
	} else /if (${Me.Level} >= 88)  {
		/call setINIVar DoTSwarmDS		 								string	Spells "Swarm of Hornets"
		/call setINIVar DoTSwarmDSdeBuff 							string	Spells "Hornet Swarm"
	} else /if (${Me.Level} >= 83)  {
		/call setINIVar DoTSwarmDS		 								string	Spells "Swarm of Bees"
		/call setINIVar DoTSwarmDSdeBuff 							string	Spells "Bee Swarm"
	} else {
		/call setINIVar DoTSwarmDS		 								string	Spells FALSE
		/call setINIVar DoTSwarmDSdeBuff 							string	Spells FALSE
	}
	
	| slow heal
	/if (${Me.Level} >= 106)  {
	  /call setINIVar heal1	 												string  Heal "Wellspring"
	} else /if (${Me.Level} >= 101)  {
	  /call setINIVar heal1	 												string  Heal "Cloudfont"
	} else /if (${Me.Level} >= 96)  {
	  /call setINIVar heal1	 												string  Heal "Cloudburst"
	} else /if (${Me.Level} >= 91)  {
	  /call setINIVar heal1	 												string  Heal "Purespring"
	} else /if (${Me.Level} >= 86)  {
	  /call setINIVar heal1	 												string  Heal "Purefont"
	} else {
	  /call setINIVar heal1	 												string  Heal FALSE
	}

	| fast heal
	/if (${Me.Level} >= 109)  {
	  /call setINIVar heal2				 									string  Heal "Desperate Dewcloud"
	} else /if (${Me.Level} >= 104)  {
	  /call setINIVar heal2				 									string  Heal "Desperate Dousing"
	} else /if (${Me.Level} >= 99)  {
	  /call setINIVar heal2				 									string  Heal "Desperate Drenching"
	} else /if (${Me.Level} >= 94)  {
	  /call setINIVar heal2				 									string  Heal "Desperate Dounpour"
	} else /if (${Me.Level} >= 89)  {
	  /call setINIVar heal2				 									string  Heal "Desperate Deluge"
	} else {
	  /call setINIVar heal2				 									string  Heal FALSE
	}

	| bow disc
	/if (${Me.Level} >= 100)  {
		/call setINIVar DiscRange			 								string	Spells "Pureshot Discipline"
	} else /if (${Me.Level} >= 90)  {
		/call setINIVar DiscRange			 								string	Spells "Bullseye Discipline"
	} else /if (${Me.Level} >= 80)  {
		/call setINIVar DiscRange			 								string	Spells "Aimshot Discipline"
	} else /if (${Me.Level} >= 55)  {
		/call setINIVar DiscRange			 								string	Spells "Trueshot Discipline"
	} else {
		/call setINIVar DiscRange			 								string	Spells FALSE
	}

	| rest disc
	/if (${Me.Level} >= 101)  {
		/call setINIVar DiscRest			 								string	Discs "Breather"
	} else /if (${Me.Level} >= 96)  {
		/call setINIVar DiscRest			 								string	Discs "Rest"
	} else /if (${Me.Level} >= 91)  {
		/call setINIVar DiscRest			 								string	Discs "Reprive"
	} else /if (${Me.Level} >= 86)  {
		/call setINIVar DiscRest			 								string	Discs "Respite"
	} else {
		/call setINIVar DiscRest			 								string	Discs FALSE
	}
	
	| melee disc
	/if (${Me.Level} >= 110)  {
		/call setINIVar DiscMele			 								string	Spells "Windstalker's Discipline"
	} else /if (${Me.Level} >= 105)  {
		/call setINIVar DiscMele			 								string	Spells "Copsestalker's Discipline"
	} else /if (${Me.Level} >= 100)  {
		/call setINIVar DiscMele			 								string	Spells "Bosquestalker's Discipline"
	} else /if (${Me.Level} >= 69)  {
		/call setINIVar DiscMele			 								string	Spells "Warder's Wrath"
	} else {
		/call setINIVar DiscMele			 								string	Spells FALSE
	}	

	| agro/anti agro kicks
	/if (${Me.Level} >= 107)  {
		/call setINIVar DiscJolt			 								string	Spells "Jolting Wheel Kicks"
		/call setINIVar DiscEnrage		 								string	Spells "Enraging Wheel Kicks"
	} else /if (${Me.Level} >= 102)  {
		/call setINIVar DiscJolt			 								string	Spells "Jolting Cut Kicks"
		/call setINIVar DiscEnrage		 								string	Spells "Enraging Cut Kicks"
	} else /if (${Me.Level} >= 97)  {
		/call setINIVar DiscJolt			 								string	Spells "Jolting Heel Kicks"
		/call setINIVar DiscEnrage		 								string	Spells "Enraging Heel Kicks"
	} else /if (${Me.Level} >= 92)  {
		/call setINIVar DiscJolt			 								string	Spells "Jolting Crescent Kicks"
		/call setINIVar DiscEnrage		 								string	Spells "Enraging Crescent Kicks"
	} else /if (${Me.Level} >= 87)  {
		/call setINIVar DiscJolt			 								string	Spells "Jolting Hook Kicks"
		/call setINIVar DiscEnrage		 								string	Spells "Enraging Hook Kicks"
	} else /if (${Me.Level} >= 105)  {
		/call setINIVar DiscJolt			 								string	Spells "Jolting Frootkicks"
		/call setINIVar DiscEnrage		 								string	Spells FALSE
	} else /if (${Me.Level} >= 77)  {
		/call setINIVar DiscJolt			 								string	Spells "Jolting Snapkicks"
		/call setINIVar DiscEnrage		 								string	Spells FALSE
	} else /if (${Me.Level} >= 72)  {
		/call setINIVar DiscJolt			 								string	Spells "Jolting Kicks"
		/call setINIVar DiscEnrage		 								string	Spells FALSE
	} else {
		/call setINIVar DiscJolt			 								string	Spells FALSE
		/call setINIVar DiscEnrage		 								string	Spells FALSE
	}	

	| AE group heal disc
	/if (${Me.Level} >= 105)  {
		/call setINIVar DiscHeal			 								string	Spells "Reflexive Nettlespears"
	} else /if (${Me.Level} >= 100)  {
		/call setINIVar DiscHeal			 								string	Spells "Reflexive Bladespurs"
	} else {
		/call setINIVar DiscHeal		 									string	Spells FALSE
	}	

	| ae melee disc
	/if (${Me.Level} >= 106)  {
		/call setINIVar BladeStorm			 								string	Spells "Gale of Blades"
	} else /if (${Me.Level} >= 101)  {
		/call setINIVar BladeStorm			 								string	Spells "Squal of Blades"
	} else /if (${Me.Level} >= 96)  {
		/call setINIVar BladeStorm			 								string	Spells "Storm of Blades"
	} else {
		/call setINIVar BladeStorm		 									string	Spells FALSE
	}	

	| focused melee disc
	/if (${Me.Level} >= 103)  {
		/call setINIVar DiscSqual			 								string	Spells "Focused Squall of Blades"
	} else /if (${Me.Level} >= 98)  {
		/call setINIVar DiscSqual			 								string	Spells "Focused Storm of Blades"
	} else {
		/call setINIVar DiscSqual		 									string	Spells FALSE
	}	

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

	/call setDeclare SHDspire1											string	outer "Fundament: First Spire of the Reavers"
	/call setDeclare SHDspire2											string	outer "Fundament: Second Spire of the Reavers"
	/call setDeclare SHDspire3											string	outer "Fundament: Third Spire of the Reavers"
	/call setDeclare SHDsynergyAA										string	outer "Knight's Synergy"
	/call setDeclare SHDEpic1												string	outer "Innoruuk's Curse"
	/call setDeclare SHDEpic15											string	outer "Innoruuk's Voice"
	/call setDeclare SHDEpic2												string	outer "Innoruuk's Dark Blessing"
	/call setINIVar SHDAlliance											string	Spells "Bloodletting Alliance"
	/call setDeclare SHDAAUnity											string	outer "Dark Lord's Unity (Beza)"
	/call setDeclare SHDAAFade											string	outer "Death Peace"
	
	/call setINIVar switchAAFade										bool		Buffs FALSE
	/call setINIVar useAAFadePct										int			Buffs 50

	/call setTimer tcheck_Skin											string 	Timer 45s timer_check_Skin
	/call setTimer tcheck_Stance										string 	Timer 3m timer_check_Stance

	/call setDeclare mobAECount											int			outer 0

	| fast rune
	/if (${Me.Level} >= 108) {
		/call setINIVar Withstand			 								string	Discs "Spurn"
	} else /if (${Me.Level} >= 103) {
		/call setINIVar Withstand			 								string	Discs "Repel"
	} else /if (${Me.Level} >= 98) {
		/call setINIVar Withstand			 								string	Discs "Reprove"
	} else /if (${Me.Level} >= 93) {
		/call setINIVar Withstand			 								string	Discs "Renounce"
	} else /if (${Me.Level} >= 83) {
		/call setINIVar Withstand			 								string	Discs "Withstand"
	} else {
		/call setINIVar Withstand			 								string	Discs FALSE
	}	

	/call setINIVar BladeStrike 										string 	Discs "Lacerating Blade"

	| blade DPS
	/if (${Me.Level} >= 110) {
		/call setINIVar CrimsonBlade			 						string	Discs "Claret Blad"
	} else /if (${Me.Level} >= 105) {
		/call setINIVar CrimsonBlade			 						string	Discs "Carmine Blade"
	} else /if (${Me.Level} >= 95) {
		/call setINIVar CrimsonBlade			 						string	Discs "Crimson Blade"
	} else {
		/call setINIVar CrimsonBlade			 						string	Discs FALSE
	}	
	
	| rest disc
	/if (${Me.Level} >= 101)  {
		/call setINIVar DiscRest			 								string	Discs "Breather"
	} else /if (${Me.Level} >= 96)  {
		/call setINIVar DiscRest			 								string	Discs "Rest"
	} else /if (${Me.Level} >= 91)  {
		/call setINIVar DiscRest			 								string	Discs "Reprive"
	} else /if (${Me.Level} >= 86)  {
		/call setINIVar DiscRest			 								string	Discs "Respite"
	} else {
		/call setINIVar DiscRest			 								string	Discs FALSE
	}	

	| acromony
	/if (${Me.Level} >= 109) {
		/call setINIVar DiscFakeHate			 						string	Discs "Unyielding Acrimony"
	} else /if (${Me.Level} >= 104) {
		/call setINIVar DiscFakeHate			 						string	Discs "Unflinching Acrimony"
	} else /if (${Me.Level} >= 99) {
		/call setINIVar DiscFakeHate			 						string	Discs "Unbroken Acrimony"
	} else /if (${Me.Level} >= 94) {
		/call setINIVar DiscFakeHate			 						string	Discs "Undivided Acrimony"
	} else {
		/call setINIVar DiscFakeHate			 						string	Discs FALSE
	}	
	
	/call setINIVar DiscLeech												string	Discs	"Leechcurse Discipline"
	
	| rancor	
	/if (${Me.Level} >= 104) {
		/call setINIVar DiscRancor				 						string	Discs "Reflexive Revulsion"
	} else /if (${Me.Level} >= 100) {
		/call setINIVar DiscRancor				 						string	Discs "Reflexive Rancor"
	} else {
		/call setINIVar DiscRancor				 						string	Discs FALSE
	}	

	| defensive mantle
	/if (${Me.Level} >= 108) {
		/call setINIVar DefensiveDisc1			 					string	Discs "Krellnakor Mantle"
	} else /if (${Me.Level} >= 103) {
		/call setINIVar DefensiveDisc1			 					string	Discs "Doomscale Mantle"
	} else /if (${Me.Level} >= 98) {
		/call setINIVar DefensiveDisc1			 					string	Discs "Bonebrood Mantle"
	} else /if (${Me.Level} >= 93) {
		/call setINIVar DefensiveDisc1			 					string	Discs "Recondite Mantle"
	} else /if (${Me.Level} >= 83) {
		/call setINIVar DefensiveDisc1			 					string	Discs "Malarian Mantle"
	} else {
		/call setINIVar DefensiveDisc1			 					string	Discs FALSE
	}	

	| defensive carpace
	/if (${Me.Level} >= 108)  {
		/call setINIVar DefensiveDisc2			 					string	Discs "Tylix's Carapace"
	} else /if (${Me.Level} >= 103)  {
		/call setINIVar DefensiveDisc2								string	Discs "Vizat's Carpace"
	} else /if (${Me.Level} >= 98)  {
		/call setINIVar DefensiveDisc2								string	Discs "Grelleth's Carpace"
	} else /if (${Me.Level} >= 93)  {
		/call setINIVar DefensiveDisc2		 						string	Discs "Sholothian Carpace"
	} else {
		/call setINIVar DefensiveDisc2		 						string	Discs FALSE
	}	

	| defensive guardian
	/if (${Me.Level} >= 107) {
		/call setINIVar DefensiveDisc3			 					string	Discs "Cursed Guardian Discipline"
	} else /if (${Me.Level} >= 97) {
		/call setINIVar DefensiveDisc3			 					string	Discs "Unholy Guardian Discipline"
	} else {
		/call setINIVar DefensiveDisc3			 					string	Discs FALSE
	}		

	/call setINIVar defCount												int			Discs 2
	/call setINIVar AEMelee					 								bool	 	Discs FALSE

	| challenge	
	/if (${Me.Level} >= 107) {
		/call setINIVar ChallengeSpell			 					string	Spells "Refute for Power"
	} else /if (${Me.Level} >= 102) {
		/call setINIVar ChallengeSpell								string	Spells "Impose for Power"
	} else /if (${Me.Level} >= 97) {
		/call setINIVar ChallengeSpell								string	Spells "Demand for Power"
	} else /if (${Me.Level} >= 92) {
		/call setINIVar ChallengeSpell		 						string	Spells "Provocation for Power"
	} else /if (${Me.Level} >= 82) {
		/call setINIVar ChallengeSpell		 						string	Spells "Charge for Power"
	} else /if (${Me.Level} >= 77) {
		/call setINIVar ChallengeSpell		 						string	Spells "Trial for Power"
	} else /if (${Me.Level} >= 77) {
		/call setINIVar ChallengeSpell		 						string	Spells "Challenge for Power"
	} else {
		/call setINIVar ChallengeSpell		 						string	Spells FALSE
	}	
	
	/call setINIVar spellCripple										string 	Spells FALSE
	/call setINIVar switchModRod										bool	 	Buffs TRUE
	/call setINIVar switchHarmonious 								bool	 	Buffs TRUE
	/call setINIVar Harmonious 											string 	Buffs "Harmonious Disruption"

	| single hate
	/if (${Me.Level} >= 106)  {
		/call setINIVar Hate1 												string 	Spells "Terror of Kra`Du"
		/call setINIVar Hate2 												string 	Spells "Terror of Narus"
	} else /if (${Me.Level} >= 101) {
		/call setINIVar Hate1 												string 	Spells "Terror of Narus"
		/call setINIVar Hate2 												string 	Spells "Terror of Poira"
	} else /if (${Me.Level} >= 96) {
		/call setINIVar Hate1 												string 	Spells "Terror of Poira"
		/call setINIVar Hate2 												string 	Spells "Terror of Desalin"
	} else /if (${Me.Level} >= 91) {
		/call setINIVar Hate1 												string 	Spells "Terror of Desalin"
		/call setINIVar Hate2 												string 	Spells "Terror of Rerekalen"
	} else /if (${Me.Level} >= 86) {
		/call setINIVar Hate1 												string 	Spells "Terror of Rerekalen"
		/call setINIVar Hate2 												string 	Spells "Terror of Jelvalak"
	} else /if (${Me.Level} >= 81) {
		/call setINIVar Hate1 												string 	Spells "Terror of Jelvalak"
		/call setINIVar Hate2 												string 	Spells "Terror of the Soulbleeder"
	} else /if (${Me.Level} >= 76) {
		/call setINIVar Hate1 												string 	Spells "Terror of the Soulbleeder"
		/call setINIVar Hate2 												string 	Spells "Terror of Vergalid"
	} else /if (${Me.Level} >= 71) {
		/call setINIVar Hate1 												string 	Spells "Terror of Vergalid"
		/call setINIVar Hate2 												string 	Spells "Terror of Discord"
	} else /if (${Me.Level} >= 67) {
		/call setINIVar Hate1 												string 	Spells "Terror of Discord"
		/call setINIVar Hate2 												string 	Spells "Terror of Thule"
	} else /if (${Me.Level} >= 63) {
		/call setINIVar Hate1 												string 	Spells "Terror of Thule"
		/call setINIVar Hate2 												string 	Spells "Terrir of Terris"
	} else /if (${Me.Level} >= 59) {
		/call setINIVar Hate1 												string 	Spells "Terrir of Terris"
		/call setINIVar Hate2 												string 	Spells "Terror of Death"
	} else /if (${Me.Level} >= 53) {
		/call setINIVar Hate1 												string 	Spells "Terror of Death"
		/call setINIVar Hate2 												string 	Spells "Terror of Shadows"
	} else /if (${Me.Level} >= 42) {
		/call setINIVar Hate1 												string 	Spells "Terror of Shadows"
		/call setINIVar Hate2 												string 	Spells "Terror of Darkness"
	} else /if (${Me.Level} >= 33) {
		/call setINIVar Hate1 												string 	Spells "Terror of Darkness"
		/call setINIVar Hate2 												string 	Spells FALSE
	} else {
		/call setINIVar Hate1 												string 	Spells FALSE
		/call setINIVar Hate2 												string 	Spells FALSE
	}

	| AE Hate	
	/if (${Me.Level} >= 109) {
		/call setINIVar AEHate1 											string 	Spells "Revulsion"
		/call setINIVar AEHate2 											string 	Spells "Disgust"
	} else /if (${Me.Level} >= 104) {
		/call setINIVar AEHate1 											string 	Spells "Disgust"
		/call setINIVar AEHate2 											string 	Spells "Repugnance"
	} else /if (${Me.Level} >= 99) {
		/call setINIVar AEHate1 											string 	Spells "Repugnance"
		/call setINIVar AEHate2 											string 	Spells "Abhorrence"
	} else /if (${Me.Level} >= 94) {
		/call setINIVar AEHate1 											string 	Spells "Abhorrence"
		/call setINIVar AEHate2 											string 	Spells "Loathing"
	} else /if (${Me.Level} >= 89) {
		/call setINIVar AEHate1 											string 	Spells "Loathing"
		/call setINIVar AEHate2 											string 	Spells "Burst of Spite"
	} else /if (${Me.Level} >= 84) {
		/call setINIVar AEHate1 											string 	Spells "Burst of Spite"
		/call setINIVar AEHate2 											string 	Spells "Revile"
	} else /if (${Me.Level} >= 79) {
		/call setINIVar AEHate1 											string 	Spells "Revile"
		/call setINIVar AEHate2 											string 	Spells "Vilify"
	} else /if (${Me.Level} >= 74) {
		/call setINIVar AEHate1 											string 	Spells "Vilify"
		/call setINIVar AEHate2 											string 	Spells "Dread Gaze"
	} else /if (${Me.Level} >= 69) {
		/call setINIVar AEHate1 											string 	Spells "Dread Gaze"
		/call setINIVar AEHate2 											string 	Spells FALSE
	} else {
		/call setINIVar AEHate1 											string 	Spells FALSE
		/call setINIVar AEHate2 											string 	Spells FALSE
	}	

	/call setINIVar AELifeTap 											string 	Spells "Deceitful Deflection"
	/call setINIVar AAHatePet												string	Discs "Projection of Doom"

	| stance	
	/call setINIVar switchStance 										bool	 	Buffs TRUE
	/if (${Me.Level} >= 109) {
		/call setINIVar Stance			 									string	Buffs "Defiant Stance"
	} else /if (${Me.Level} >= 104) {
		/call setINIVar Stance												string	Buffs "Staunch Stance"
	} else /if (${Me.Level} >= 99) {
		/call setINIVar Stance												string	Buffs "Steadfast Stance"
	} else /if (${Me.Level} >= 94) {
		/call setINIVar Stance		 										string	Buffs "Stoic Stance"
	} else /if (${Me.Level} >= 89) {
		/call setINIVar Stance		 										string	Buffs "Stubborn Stance"
	} else /if (${Me.Level} >= 84) {
		/call setINIVar Stance		 										string	Buffs "Steely Stance"
	} else {
		/call setINIVar Stance		 										string	Buffs FALSE
	}	

	| skin rune buff	
	/call setINIVar switchSkin			 								bool	 	Buffs TRUE
	/if (${Me.Level} >= 110) {
		/call setINIVar Skin			 										string	Buffs "Tylix's Skin"
	} else /if (${Me.Level} >= 105) {
		/call setINIVar Skin													string	Buffs "Vizat's Skin"
	} else /if (${Me.Level} >= 100) {
		/call setINIVar Skin													string	Buffs "Grelleth's Skin"
	} else /if (${Me.Level} >= 95) {
		/call setINIVar Skin		 											string	Buffs "Sholothian Skin"
	} else /if (${Me.Level} >= 90) {
		/call setINIVar Skin		 											string	Buffs "Gorgon Skin"
	} else /if (${Me.Level} >= 85) {
		/call setINIVar Skin		 											string	Buffs "Malarian Skin"
	} else /if (${Me.Level} >= 80) {
		/call setINIVar Skin		 											string	Buffs "Umbrial Skin"
	} else /if (${Me.Level} >= 70) {
		/call setINIVar Skin		 											string	Buffs "Decrepit Skin"
	} else {
		/call setINIVar Skin		 											string	Buffs FALSE
	}	

	/call setINIVar switchVoT				 								bool	 	Buffs TRUE
	/call setINIVar ${Me.Class.ShortName}Point 			int 		Heal 80
	/call setINIVar useShieldFlash									bool		Discs FALSE
	/call setINIVar useShieldFlashPct								int			Discs 20
	/call setINIVar switchAttraction								bool		Discs TRUE

	| Nukes
	/call setINIVar DoNuke					 								int			Spells 0
	/for _count 1 to 6
		/call setINIVar Nuke${_count}									string	Spells FALSE
		/call setINIVar Nuke${_count}Condition				string	Spells FALSE
	/next _count	
	
	| DoTs
	/call setINIVar DoDoT								 						int			Spells 0
	/for _count 1 to 6
		/call setINIVar DoT${_count}									string	Spells FALSE
		/call setINIVar DoT${_count}chk								string	Spells FALSE
		/call setINIVar DoT${_count}Condition					string	Spells FALSE
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



