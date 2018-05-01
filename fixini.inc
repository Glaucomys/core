|***
 *	fixini.inc
 *	Core ild INI removal utility
 *
 *
 ***|
 
 
 
 
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
 
 
 
|***
 *	DES: Used to remove stale entries from the toons INI. 'cause I gotta keep changing shits.
 *	USE: /call fix_INI_declares
 *	INI: 
 *	NOTE: may look a little strange, but I leave the old variables here for history. Just in case.
 ***|
sub fix_INI_declares(bool _forceold)

	VOUT INI Housekeeping ${sep} ${cinfo}current\ax
	| core.ini
		
		| 20180429
		/ini "${INICore}" "General" "switchAlwaysFaceTarget" NULL
		/ini "${INICore}" "Here" "combatWAR" NULL
		/ini "${INICore}" "Here" "combatPAL" NULL
		/ini "${INICore}" "Here" "combatSHD" NULL
		/ini "${INICore}" "Here" "combatCLR" NULL
		/ini "${INICore}" "Here" "combatSHM" NULL
		/ini "${INICore}" "Here" "combatDRU" NULL
		/ini "${INICore}" "Here" "combatMAG" NULL
		/ini "${INICore}" "Here" "combatENC" NULL
		/ini "${INICore}" "Here" "combatWIZ" NULL
		/ini "${INICore}" "Here" "combatNEC" NULL
		/ini "${INICore}" "Here" "combatROG" NULL
		/ini "${INICore}" "Here" "combatBER" NULL
		/ini "${INICore}" "Here" "combatBST" NULL
		/ini "${INICore}" "Here" "combatRNG" NULL
		/ini "${INICore}" "Here" "combatBRD" NULL
		/ini "${INICore}" "Here" "combatMNK" NULL
	
	
	
		
		
		
	| toon INI non specific class
		| fix the range summon INI entries
		/if (!${RangeItem.Count[|]}) /call writeINI RangeItem Common |"${RangeItem}"
		/if (!${RangeItemSummon.Count[|]}) /call writeINI RangeItemSummon Common |"${RangeItemSummon}"		
	
		| 20180429
		/ini "${INIClass}" "Common" "combat" NULL	
		


	| class Specific
	
		| BERZERKER
		/if (${Select[${Me.Class.ShortName},BER]}) {
			
		| BARD
		} else /if (${Select[${Me.Class.ShortName},BRD]}) {
			
		| BEASTLORD
		} else /if (${Select[${Me.Class.ShortName},BST]}) {
			
		| CLERIC
		} else /if (${Select[${Me.Class.ShortName},CLR]}) {
			| remove cleric QM from 20 sep 2017 patch
			/ini "${INIClass}" "Buffs" "useQM" NULL
			/ini "${INIClass}" "Buffs" "useQMPct" NULL
			/ini "${INIClass}" "Timer" "tcheck_QM" NULL

		| DRUID
		} else /if (${Select[${Me.Class.ShortName},DRU]}) {
			
		| ENCHANTER
		} else /if (${Select[${Me.Class.ShortName},ENC]}) {
			
		| MAGICIAN
		} else /if (${Select[${Me.Class.ShortName},MAG]}) {
			
		| MONK
		} else /if (${Select[${Me.Class.ShortName},MNK]}) {
		
		| NECROMANCER
		} else /if (${Select[${Me.Class.ShortName},NEC]}) {
			
		| PALADIN
		} else /if (${Select[${Me.Class.ShortName},PAL]}) {

		| RANGER
		} else /if (${Select[${Me.Class.ShortName},RNG]}) {
			| 20180429
			/ini "${INIClass}" "Spells" "ElementalArrow" NULL
			/ini "${INIClass}" "Spells" "GlacialArow" NULL
			/ini "${INIClass}" "Spells" "VolatileArrow" NULL
			/ini "${INIClass}" "Spells" "ArrowPoison" NULL
			/ini "${INIClass}" "Spells" "ArrowFire" NULL
			/ini "${INIClass}" "Spells" "ArrowIce" NULL

		} else /if (${Select[${Me.Class.ShortName},ROG]}) {

		| SHADOWKNIGHT
		} else /if (${Select[${Me.Class.ShortName},SHD]}) {

		| SHAMAN
		} else /if (${Select[${Me.Class.ShortName},SHM]}) {
			| 20180501
			/ini "${INIClass}" "Spells" "CaniPercent" NULL

		| WARRIOR
		} else /if (${Select[${Me.Class.ShortName},WAR]}) {
			
			| 20180501
			/ini "${INIClass}" "Discs" "AAVhementRage" NULL

		| WIZARD
		} else /if (${Select[${Me.Class.ShortName},WIZ]}) {
			
			
		}




	| everything forom this point on is old archived entries. stale and not used in quite a while
	| left them here just in case
	
	/if (${_forceold}) {
		
		VOUT INI Housekeeping ${sep} ${cinfo}old\ax

		/ini "${INIClass}" "Common" "SetModeInvis" NULL
		/ini "${INIClass}" "Common" "SetScorchedEarthMode" NULL
		/ini "${INIClass}" "Common" "SetSkillsMode" NULL
		/ini "${INIClass}" "Common" "SetFishMode" NULL
		/ini "${INIClass}" "Common" "SetForageMode" NULL
		/ini "${INIClass}" "Common" "SetDragMode" NULL
		/ini "${INIClass}" "Common" "Looter" NULL
		/ini "${INIClass}" "Common" "ForageFishInvis" NULL
		/ini "${INIClass}" "Common" "SetHunterMode" NULL
		/ini "${INIClass}" "Common" "SetHunterAgro" NULL
		/ini "${INIClass}" "Common" "SetPetFarmMode" NULL
		/ini "${INIClass}" "Movement" "SetIgnoreLOS" NULL
		/ini "${INICore}" "General" "QPath" NULL
		/ini "${INICore}" "Timer" "tPetFarm_delay" NULL
		/ini "${INIClass}" "Spells" "HuntNuke" NULL
		/ini "${INIClass}" "Common" "setRaidInvite" NULL
		/ini "${INIClass}" "Common" "setAgroMode" NULL
		/ini "${INIClass}" "Common" "setPullMode" NULL
		/ini "${INIClass}" "Common" "setToastMode" NULL
		/ini "${INIClass}" "Common" "switchdeBuffMezz" NULL
		/ini "${INIClass}" "Common" "switchdeBuffMalo" NULL
		/ini "${INIClass}" "Common" "switchdeBuffTash" NULL
		/ini "${INIClass}" "Common" "switchdeBuffSlow" NULL
		/ini "${INIClass}" "Common" "switchdeBuffEradicate" NULL
		/ini "${INIClass}" "Common" "switchdeBuffSnare" NULL
		/ini "${INIClass}" "Common" "switchdeBuffCripple" NULL
		/ini "${INIClass}" "Common" "setToastMode" NULL
		/ini "${INIClass}" "Common" "BuffCombat" NULL
		/ini "${INIClass}" "Common" "BuffSelf" NULL
		/ini "${INIClass}" "Common" "BuffBC" NULL
		/ini "${INIClass}" "Common" "BuffRaid" NULL
		/ini "${INIClass}" "Common" "BuffTell" NULL
		/ini "${INIClass}" "Common" "BuffPet" NULL
		/ini "${INIClass}" "Common" "BuffItem" NULL
		/ini "${INIClass}" "Common" "BuffAuto" NULL	
		/ini "${INIClass}" "Buffs" "BuffCombat" NULL
		/ini "${INIClass}" "Buffs" "BuffSelf" NULL
		/ini "${INIClass}" "Buffs" "BuffBC" NULL
		/ini "${INIClass}" "Buffs" "BuffRaid" NULL
		/ini "${INIClass}" "Buffs" "BuffTell" NULL
		/ini "${INIClass}" "Buffs" "BuffPet" NULL
		/ini "${INIClass}" "Buffs" "BuffItem" NULL	
		/ini "${INIClass}" "Buffs" "BuffAuto" NULL	
		/ini "${INIClass}" "Spells" "useBuffKill" NULL
		/ini "${INIClass}" "Buffs" "BuffCount" NULL
		/ini "${INIClass}" "Spells" "SpellSet" NULL
		/ini "${INIClass}" "Spells" "Spire" NULL
		/ini "${INIClass}" "Common" "autoCursor" NULL	
		/ini "${INIClass}" "Common" "switchPullMode" NULL	
		/ini "${INIClass}" "Common" "switchAgroMode" NULL	
		/ini "${INIClass}" "Heal" "rezAuto" NULL	
		/ini "${INIClass}" "Heal" "rezEmergency" NULL	
		/ini "${INIClass}" "Heal" "rezEveryone" NULL	
		/ini "${INIClass}" "Heal" "rezCombat" NULL	
		/ini "${INIClass}" "Heal" "rezEnable" NULL	
		/ini "${INIClass}" "Heal" "switchRezAtuo" NULL	
		/ini "${INIClass}" "Heal" "switchRezEnable" NULL	
		/ini "${INIClass}" "Buffs" "switchRezEmergency" NULL	
		/ini "${INIClass}" "Buffs" "switchRezAuto" NULL	
		/ini "${INIClass}" "Buffs" "switchRezCombat" NULL	
		/ini "${INIClass}" "Buffs" "switchRezEveryone" NULL	
		/ini "${INICore}" "General" "RezTokenList" NULL	
		/ini "${INIClass}" "Pet" "PetTank" NULL
		/ini "${INIClass}" "Pet" "sendPetPerc" NULL
		/ini "${INIClass}" "Pet" "usePet" NULL
		/ini "${INIClass}" "Pet" "sendPetSwarmPerc" NULL
		/ini "${INIClass}" "Pet" "usePetSwarm" NULL
		/ini "${INIClass}" "Movement" "SetTie" NULL
		/ini "${INIClass}" "Common" "UseBanestrike" NULL
		/ini "${INIClass}" "Common" "ModeNuke" NULL
		/ini "${INIClass}" "Common" "AutoDeleteItem" NULL
		/ini "${INIClass}" "Common" "CombatBuff" NULL
		/ini "${INIClass}" "Common" "BalanceHP" NULL
		/ini "${INIClass}" "Common" "DoDebuff" NULL
		/ini "${INIClass}" "Common" "Mount" NULL
		/ini "${INIClass}" "Common" "HealOverride" NULL
		/ini "${INIClass}" "Spells" "RuneSpell" NULL
		/ini "${INIClass}" "Spells" "RuneType" NULL
		/ini "${INIClass}" "Spells" "CauldronDestroy" NULL
		/ini "${INIClass}" "Common" "AutoSit" NULL
		/ini "${INIClass}" "Common" "AutoBuff" NULL
		/ini "${INIClass}" "Common" "UseCharm" NULL
		/ini "${INIClass}" "Common" "SetTankMode" NULL
		/ini "${INIClass}" "Common" "AutoBot" NULL
		/ini "${INIClass}" "Common" "forcesafelist" NULL
		/ini "${INIClass}" "SelfBuff" NULL NULL
		/ini "${INIClass}" "Transport" NULL NULL
		/ini "${INICore}" "bitch" NULL NULL
		/ini "${INICore}" "Synergy" NULL NULL
		/ini "${INICore}" "Output" NULL NULL
		/ini "${INIClass}" "Common" "setAutoShrink" NULL
		/ini "${INIClass}" "Common" "HornofUnity" NULL
		/ini "${INIClass}" "Heal" "MercWarPoint" NULL
		/ini "${INIClass}" "Heal" "MercCLRPoint" NULL
		/ini "${INIClass}" "Heal" "MercDPSpoint" NULL
		/ini "${INIClass}" "Heal" "healTouch" NULL
		/ini "${INIClass}" "Common" "ToastMobCount" NULL
		/ini "${INIClass}" "Common" "WhentoToast" NULL
		/ini "${INIClass}" "Common" "AutoToast" NULL
		/ini "${INIClass}" "Common" "MyDeathCount" NULL
		/ini "${INIClass}" "Common" "BCMA" NULL
		/ini "${INIClass}" "Timer" "AutoInvTime" NULL
		/ini "${INIClass}" "Common" "BuffAuto" NULL
		/ini "${INIClass}" "Common" "BuffCombat" NULL
		/ini "${INIClass}" "Common" "BuffSelf" NULL
		/ini "${INIClass}" "Timer" "SummerTime" NULL
		/ini "${INIClass}" "Timer" "CheckAmmo" NULL
		/ini "${INIClass}" "Timer" "CheckTieTime" NULL
		/ini "${INIClass}" "Timer" "CheckFamiliar" NULL
		/ini "${INIClass}" "Common" "EQBCServer" NULL
		/ini "${INIClass}" "Common" "EQBCPort" NULL
		/ini "${INIClass}" "Common" "EQBCChannel" NULL
		/ini "${INIClass}" "Timer" "ClearCursorTime" NULL
		/ini "${INIClass}" "Timer" "tCheckTieTime" NULL
		/ini "${INIClass}" "Timer" "SelfBuffTime" NULL
		/ini "${INIClass}" "Timer" "ItemBuffTime" NULL
		/ini "${INIClass}" "Timer" "CheckEXPTime" NULL
		/ini "${INIClass}" "Timer" "TellBuffTime" NULL
		/ini "${INIClass}" "Timer" "BCBuffTime" NULL
		/ini "${INIClass}" "Timer" "RaidBuffTime" NULL
		/ini "${INIClass}" "Timer" "CheckEQCBTime" NULL
		/ini "${INIClass}" "Timer" "AuraTime" NULL
		/ini "${INIClass}" "Timer" "PetBuffTime" NULL
		/ini "${INIClass}" "Timer" "RezRetryTime" NULL
		/ini "${INIClass}" "Timer" "BitchTime" NULL
		/ini "${INIClass}" "Timer" "BuffKillTime" NULL
		/ini "${INIClass}" "Timer" "XHealTime" NULL
		/ini "${INIClass}" "Common" "setPetFarmMode" NULL
		/ini "${INIClass}" "Common" "autoRez" NULL
		/ini "${INIClass}" "Heal" "CombatCall" NULL
		/ini "${INIClass}" "Common" "echoChannel" NULL
		/ini "${INIClass}" "Common" "relayTells" NULL
		/ini "${INIClass}" "Common" "verbose" NULL
		/ini "${INIClass}" "Items" "doclickItem" NULL
		/ini "${INIClass}" "Items" "doclickItemBurn" NULL
		/ini "${INIClass}" "Odds" "doclickOdds" NULL
		/ini "${INIClass}" "Discs" "DefensiveDisc" NULL
		/ini "${INIClass}" "Discs" "DiscArmor" NULL
		/ini "${INIClass}" "Discs" "DiscGuard" NULL
		/ini "${INIClass}" "Discs" "DiscHoly" NULL
		/ini "${INIClass}" "Common" "useSmartAssist1" NULL
		/ini "${INIClass}" "Common" "useSmartAssist2" NULL
		/ini "${INIClass}" "Common" "switchInvites" NULL
		/ini "${INIClass}" "Common" "switchRaidInvite" NULL
			
		/if (${Me.Class.ShortName.Equal[BRD]}) {
			/ini "${INIClass}" "meleedisc" "seloskick" NULL
		}

		/if (${Me.Class.ShortName.Equal[WIZ]}) {
			/ini "${INIClass}" "Spells" "DoNuke" NULL
			/ini "${INIClass}" "Spells" "Nuke1" NULL
			/ini "${INIClass}" "Spells" "Nuke2" NULL
			/ini "${INIClass}" "Spells" "Nuke3" NULL
			/ini "${INIClass}" "Spells" "Nuke4" NULL
			/ini "${INIClass}" "Spells" "Nuke5" NULL
			/ini "${INIClass}" "Spells" "Nuke6" NULL
			/ini "${INIClass}" "Spells" "Nuke7" NULL
			/ini "${INIClass}" "Spells" "Nuke8" NULL
		}
		
	}
}

	
	



	VOUT INI Housekeeping ${sep} ${cgood}DONE\ax
/return


 
 
 
 
