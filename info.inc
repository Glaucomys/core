|***
 *	info.inc
 *	Core Help /info Output
 *
 * 
 ***|

#event info "#*#Core Instructions:#*##1#"
sub Event_info(string line, string _section)
	/varset _section ${If[${Bool[${_section}]},${_section},NULL]}
	/call core_info ${_section}
/return

sub core_info(string _section)
	/mqclear
	/declare _toggle string local ${sepO}\ayTOGGLE\ax${sepC}
	/declare _numpct string local ${sepO}\ay##\ax%${sepC}
	/declare _num string local ${sepO}\ay##\ax${sepC}
	/declare _name string local ${sepO}\ayNAME\ax${sepC}
	/declare _on string local ${sepO}${cgood}ON\ax${sepC}
	/declare _off string local ${sepO}${cbad}OFF\ax${sepC}
	
	NOUT Core ${cinfo}${versionCore}\ax commands.
	NOUT ${cinfo}https://github.com/exspes007/core/wiki\ax

	
	/if (${Select[${_section},ALL,NULL]}) {	
		/echo 
		NOUT ${cgood}Use:\ax \aw/info ALL\ax or \aw/info HEAL\ax, etc..
		NOUT ${cinfo}ALL\ax, ${cinfo}ASSIST\ax, ${cinfo}BUFF\ax, ${cinfo}BUG\ax, ${cinfo}CORE\ax, ${cinfo}DEBUFF\ax, ${cinfo}HEAL\ax, ${cinfo}LIST\ax, ${cinfo}LOOT\ax, ${cinfo}MODES\ax, ${cinfo}MOVEMENT\ax, ${cinfo}ODD\ax, ${cinfo}PETS\ax, ${cinfo}TOAST\ax, ${cinfo}REZ\ax
		NOUT ${cinfo}Class Specific\ax ${sep} CLR, DRU, SHM, ROG, RNG, MON, BRD, BST, BER, PAL, SHD, WAR, MAG, WIZ, NEC, ENC
	}	
	
	
	| base core commands
	/if (${Select[${_section},ALL,CORE]}) {	
		/echo
		/echo ${sep}${sep}${sepO} ${cgood}Environment\ax ${sepC}
		NOUT ${cinfo}/on\ax ${sep} Turns Core ${_on} for all toons in BC except you.
		NOUT ${cinfo}/off\ax ${sep} Turns Core ${_off} for all toons in BC except you.
		NOUT ${cinfo}/core\ax ${sep} ${_toggle} Turn on or off the automatic actions of the bot.
		NOUT ${cinfo}/hudupdate\ax ${sep} Deleted the urrent [Elemets] HUD and implements the HUD for Core.
		NOUT ${cinfo}/verbose\ax ${sep} ${_toggle} Changes what is echoed in BC. More or less..
		NOUT ${cinfo}/checkcash\ax ${sep} Checks the in game cash. Will collect if there is any.
		NOUT ${cinfo}/hudtime\ax ${sep} ${_toggle} changes the HUD clock from Real Life time to In Game time.
		NOUT ${cinfo}/whitewash\ax ${sep} Clears all Core Aliases and will end mac.
		NOUT ${cinfo}/zonereset\ax ${sep} Re-Loads all INI settings relating to the zone.
		NOUT ${cinfo}/broadcast\ax ${sep} ${_toggle} Updates Hud for group/self to current /core status.
		NOUT ${cinfo}/echochannel\ax ${sep} Sets the verbage spew channel for several automated actions.
		
		/echo		
		
	}

	| Toon mode commands. most will interfeer with other uses
	/if (${Select[${_section},ALL,MODE]}) {	
		/echo	
		/echo ${sep}${sep}${sepO} ${cgood}Modes\ax ${sepC}
		NOUT ${cinfo}/combat\ax ${sep} [MELEE|RANGE|NUKE|HEAL|DUAL] sets combat modes for bots. NOT MANUAL MODE.

		/echo		
		/echo ${cbad}----${cgood}${cbad}When these are active, most other macro commands are ignored.
		NOUT ${cinfo}/drag\ax ${sep} ${_toggle} Drags ALL GUILD corpses at once. 
		NOUT ${cinfo}/fish\ax ${sep} ${_toggle} Fishing. Need a Fisherman's Companion and 1 open top slot 
		NOUT ${cinfo}/forage\ax ${sep} ${_toggle} Foraging. Keeps/destroy according to ${ForageIni}.
		NOUT ${cinfo}/petfarm\ax ${sep} ${_toggle}[MAG,NEC,BST] Pet Farming. Sends your pet out to kill shit.
		NOUT   ${owarning}: This will Ignore LOS / EngageAT.
		NOUT ${cinfo}/trainskill\ax ${sep} ${_toggle} *UNDER CONSTRUCTION*
		NOUT ${cinfo}/hunt\ax ${sep} ${_toggle} *UNDER CONSTRUCTION*
		
	} 
	
	| toast commands.. we like things burnt..
	/if (${Select[${_section},ALL,TOAST]}) {	
		/echo	
		/echo ${sep}${sep}${sepO} ${cgood}Toasting\ax ${sepC}
		NOUT ${cinfo}/toastengageat\ax ${sep} ${_numpct} Start automatic burn on Named Mobs
		NOUT ${cinfo}/toast\ax ${sep} ${_toggle} Always Burns. (${cbad}use caution\ax)
		NOUT ${cinfo}/toastauto\ax ${sep} ${_toggle} Burns when a Named is the target.
		NOUT ${cinfo}/toastmobcount\ax ${sep} ${_num} Mobs in agro range to auto burn on. Does not work in raids.
	}
	
	| loot control commands
	/if (${Select[${_section},ALL,LOOT]}) {	
		/echo	
		/echo ${sep}${sep}${sepO} ${cgood}Looting\ax ${sepC}
		NOUT ${cinfo}/lootdestroy\ax ${sep} Mark item on cursor as Destroy in loot.ini file
		NOUT ${cinfo}/lootleave\ax ${sep} Mark item on cursor as Leave in loot.ini file
		NOUT ${cinfo}/lootkeep\ax ${sep} Mark item on cursor as Keep in loot.ini file
		NOUT ${cinfo}/lootsell\ax ${sep} Mark item on cursor as Sell in loot.ini file
		NOUT ${cinfo}/lootwhat\ax ${sep} Tells you what the item on your cursor is set for in loot.ini file
		NOUT ${cinfo}/sell\ax ${sep} /sell 'ITEMNAME' .. 'loot' for ITEMNAME to sell all "Sell" in core.loot.ini
		NOUT ${cinfo}/buy\ax ${sep} /buy 'ITEMNAME' ##
		NOUT ${cinfo}/lootannounce\ax ${sep} Sets item on currsor to Announce in loot.ini. Toon will announce when they find this item..
		NOUT ${cinfo}/lootignore\ax ${sep} Loot controls will ignore this item completely.
	}

	| heal commands. NOT heal class specific
	/if (${Select[${_section},ALL,HEAL]}) {	
		/echo	
		/echo ${sep}${sep}${sepO} ${cgood}Healing\ax ${sepC}
		NOUT ${cinfo}/healoverride\ax ${sep} ${_numpct} Healing classes will stop healing at this point.
		NOUT   ${onotice}: Override all healing.  This includes class patch healing.
		NOUT   ${onotice}: This works on manna/mana regen markers as well.
		NOUT ${cinfo}/healself\ax ${sep} ${_toggle} Self only Patch Healing.
		NOUT ${cinfo}/healgroup\ax ${sep} ${_toggle} CLR,DRU,SHM Group Healing switch.
		NOUT ${cinfo}/healxtarget\ax ${sep} ${_toggle} CLR,DRU,SHM XTarget Healing switch.
		NOUT ${cinfo}/healpet\ax ${sep} ${_toggle} DRU,SHM,CLR,MAG,BST - pet healing. CLR,DRU,SHM heal others pets.
		NOUT ${cinfo}/healcure\ax ${sep} ${_toggle} DRU,SHM,CLR,PAL,RNG - use curing spells.
	}

	| Debuff routine commands
	/if (${Select[${_section},ALL,DEBUFF]}) {	
		/echo
		/echo ${sep}${sep}${sepO} ${cgood}deBuffing\ax ${sepC}
		NOUT ${cinfo}/debuffmezz\ax ${sep} ${_toggle} 
		NOUT ${cinfo}/debuffmalo\ax ${sep} ${_toggle} 
		NOUT ${cinfo}/debufftash\ax ${sep} ${_toggle} 
		NOUT ${cinfo}/debuffcripple\ax ${sep} ${_toggle} 
		NOUT ${cinfo}/debufferadicate\ax ${sep} ${_toggle} 
		NOUT ${cinfo}/debuffsnare\ax ${sep} ${_toggle} 
	}
	
	| Buff commands
	/if (${Select[${_section},ALL,BUFF]}) {	
		/echo	
		/echo ${sep}${sep}${sepO} ${cgood}Buffing\ax ${sepC}
		NOUT ${cinfo}/buffself\ax ${sep} ${_toggle} On enables self buffing.
		NOUT ${cinfo}/buffitem\ax ${sep} ${_toggle} On enables item buffing.
		NOUT ${cinfo}/buffcombat\ax ${sep} ${_toggle} Buff in combat or not. Will wait till current mob is dead.
		NOUT ${cinfo}/buffbc\ax ${sep} ${_toggle} On enables BC buffing. 
		NOUT ${cinfo}/buffraid\ax ${sep} ${_toggle} On enables Raid buffing.
		NOUT ${cinfo}/bufftell\ax ${sep} ${_toggle} On enables /t buffing.
		NOUT ${cinfo}/buffpet\ax ${sep} ${_toggle} On enables self pet buffing.
		NOUT ${cinfo}/autoshrink\ax ${sep} ${_toggle} Enable/Disable automatic shrinking.
		NOUT ${cinfo}/ib\ax ${sep} Check and cast missing item click buffs.
		NOUT ${cinfo}/sb\ax ${sep} Check and cast self buffs.
	}
	
	| Assist commnads
	/if (${Select[${_section},ALL,ASSIST]}) {
		/echo	
		/echo ${sep}${sep}${sepO} ${cgood}Assists and Ranges\ax ${sepC}
		NOUT ${cinfo}/agro\ax ${sep} ${_num} Radius for Auto Agro of Mobs and valid target range.
		NOUT ${cinfo}/smartengageat\ax ${sep} ${_numpct} Engage at when SmartMA is enabled.
		NOUT ${cinfo}/smartma\ax ${sep} ${_toggle} 4 way toggle. RaidAssist 1, RaidAssist 2, Group Assist, OFF. Adjusts XTarget to match.
		NOUT   Note: smartma will override MA1 and MA2 settings.
		NOUT   Note: Raid assist 1 and 2 are not persistant settings. They revert to off on Core start. Group assist does not.
		NOUT ${cinfo}/engageat\ax ${sep} ${_numpct} Engage the MA1/MA2 target at.
		NOUT ${cinfo}/ma1\ax ${sep} ${_name} Set Main Assist 1.
		NOUT ${cinfo}/ma2\ax ${sep} ${_name} Set Main Assist 2. Fall back if MA1 dead or not in zone.
		NOUT ${cinfo}/setpuller\ax ${sep} ${_toggle} Sets mob pulling on or off. Will ignore engage%/mezzes and pull if no mobs in agro.
		NOUT ${cinfo}/setagro\ax ${sep} ${_toggle} Sets toon to agro mode. Will fight everyone for attention of the mob.
		/echo
		NOUT ${sep} The next 4 commands are /bc commands that will adjust all toons to that assist mode.
		NOUT ${cinfo}sma1\ax ${sep} sets all in BC to assist the RAID smart assist 1.
		NOUT ${cinfo}sma2\ax ${sep} sets all in BC to assist the RAID smart assist 2.
		NOUT ${cinfo}smag\ax ${sep} sets all in BC to assist the Groupsmart assist.
		NOUT ${cinfo}smax\ax ${sep} turns off all assists for all in BC.
		/echo
		NOUT   Note on assists: Smart Assists will respond faster then target assists as the info is already present on the client.
	}

	| strange/ no home commands
	/if (${Select[${_section},ALL,ODD]}) {	
		/echo	
		/echo ${sep}${sep}${sepO} ${cgood}Oddly Specific Shit\ax ${sepC}
		NOUT ${cinfo}/autorest\ax ${sep} ${_numpct} Auto rest when manna or endurance is at point.
		NOUT ${cinfo}/clean\ax ${sep} /hidecorpse all then /hidecorpse looted.
		NOUT ${cinfo}/control\ax ${sep} Set the lead control toon for your crew.
		NOUT ${cinfo}/invis\ax ${sep} Casts Group Perfected Invisibility/vs Undead.
		NOUT   IVU - CLR,NEC,PAL,SHD
		NOUT   INVIS - ENC,MAG,NEC,WIZ
		NOUT ${cinfo}/glyph\ax ${sep} ${_name} Name of the glyph you want to use.
		NOUT ${cinfo}/invites\ax ${sep} ${_toggle} Accept Group or Raid invites.
		NOUT ${cinfo}/ri\ax ${sep} ${_toggle} Auto Raid invite request to for raid.
		NOUT ${cinfo}/banestrike\ax ${sep} ${_toggle} Use AA Banestrike.
		NOUT ${cinfo}/mirror\ax ${sep} Everyone in your BC will click the TBM port mirror.
		NOUT ${cinfo}/horn\ax ${sep} Activate Miniature Horn of Unity  
		NOUT ${cinfo}/crew\ax ${sep} invites missing croup members set in core.ini -> [Group] -> [NAME|OTHERNAME]  
		NOUT ${cinfo}/bcsay\ax ${sep} has everyone in bc say whatever words you need to your target.  
		NOUT ${cinfo}/echoaugments\ax ${sep} will dump all augments (worn and in invnetory) to core.augmentdump.ini  
		NOUT ${cinfo}/watchspawn\ax ${sep} [NAME|FALSE] will provide a HUDCOUNT with the number of spawns with that name (even partial) in the zone.
	}	
	
	| rezz commands
	/if (${Select[${_section},ALL,REZ]}) {	
		/echo	
		/echo ${sep}${sep}${sepO} ${cgood}Resurection\ax ${sepC}
		NOUT ${cinfo}/rezauto\ax ${sep} ${_toggle} Auto accept resses.
		NOUT ${cinfo}/rezcombat\ax ${sep} ${_toggle} Rez while in combat. (CLR|DRU|PAL|SHM)
		NOUT ${cinfo}/rezeveryone\ax ${sep} ${_toggle} Allows cleric to rez out side the guild. (CLR|DRU|PAL|SHM). This is not persistant.
		NOUT ${cinfo}/rezemergency\ax ${sep} ${_toggle} Allows the toon to use Tokens to rez people when the Cleric(s) AND Durid(s) are dead.
	}
	
	
	
	| all movement commands
	/if (${Select[${_section},ALL,MOVEMENT]}) {	
		/echo	
		/echo ${sep}${sep}${sepO} ${cgood}Movement\ax ${sepC}
		NOUT ${cinfo}/stop\ax ${sep} Stops and resets EVERYTHING; except /tie.
		NOUT ${cinfo}/halt\ax ${sep} Stops all movement; turns off tie, follow, makecamp/home, so on.
		NOUT ${cinfo}/home\ax ${sep} ${_toggle} On enables return to a specific (very close) point.
		NOUT ${cinfo}/homeradius\ax ${sep} ${_num} distance that is acceptable for the return too.
		NOUT ${cinfo}/homeleash\ax ${sep} ${_num} when exceeded will return  the toon to the home radius.
		NOUT ${cinfo}/combatdistance\ax ${sep} ${_num} to engate melee at. (do not advise setting this over 15)
		NOUT ${cinfo}/combatradius\ax ${sep} ${_num} to fake safe area. Mobs inside this will not be considered in combat for many functions.
		NOUT ${cinfo}/stickbehind\ax ${sep} ${_toggle} Toon will try and arrange itself behind the target.
		NOUT ${cinfo}/ignorelos\ax ${sep} ${_toggle} Will ignore LoS for just about everything. ${owarning} MQ2 giveaway.
		NOUT   ${cbad}USE WITH CAUTION.\ax Use in a safe place this will fuck shit up. 
		NOUT ${cinfo}/tie\ax ${sep} ${_toggle} On enables staying close to a specific toon.
		NOUT ${cinfo}/tietoon\ax ${sep} ${_name} of the toon you wish to get comfy with.
		NOUT ${cinfo}/tiedistance\ax ${sep} ${_num} to keep close to.
		NOUT ${cinfo}/tietime\ax ${sep} ${_num} how often you check for tie to be activate
		NOUT   e.g. 1s, 15 (1.5s), 1h, 5s ... default is 3s.
		NOUT ${cinfo}/take\ax ${sep} Accepts popup windows:  Guild Hall Banner, Translocate, AE TL, adventures, trade window.
		NOUT ${cinfo}/door\ax ${sep} Will click nearest door. If in range.
		NOUT ${cinfo}/facefast\ax ${sep} ${_toggle} Face the target instantly or not. ${owarning} MQ2 giveaway.
	}
	
	| pet stuffs.. cause we like pets.. right?
	/if (${Select[${_section},ALL,PETS]}) {	
		/echo	
		/echo ${sep}${sep}${sepO} ${cgood}Pets\ax ${sepC}
		NOUT ${cinfo}/companion\ax ${sep} ${_toggle} Pet Use.
		NOUT ${cinfo}/companionswarm\ax ${sep} ${_toggle} Swarm pet Use.
		NOUT ${cinfo}/pettank\ax ${sep} ${_toggle} Pet taunt on/off
		NOUT ${cinfo}/petengageat\ax ${sep} ${_numpct} Pets will engage the target at..
		NOUT ${cinfo}/petswarmengageat\ax ${sep} ${_numpct} Swarm pets will engage the target at..
		NOUT ${cinfo}/pb\ax ${sep} Check and cast missing pet buffs.
		NOUT ${cinfo}/buildpet\ax ${sep} Build a new pet.
	}	
	
	| INI adjustments for zone list
	/if (${Select[${_section},ALL,LIST]}) {	
		/echo	
		/echo ${sep}${sep}${sepO} ${cgood}INIs\ax ${sepC}
		NOUT ${cinfo}/listremove\ax ${sep} Will remove the current [TARGET] from all curent ZoneINI lists.
		NOUT ${cinfo}/listhunt\ax ${sep} Adds the current [TARGET] to the Hunter list.
		NOUT ${cinfo}/listexclude\ax ${sep} [TARGET] Ignore Spawn
		NOUT ${cinfo}/listnamed\ax ${sep} [TARGET] Named Spawn
		NOUT ${cinfo}/listnomezz\ax ${sep} [TARGET] Spawn NOT mezable
		NOUT ${cinfo}/listnocast\ax ${sep} [TARGET] NO Cast mob
		NOUT ${cinfo}/listnomelee\ax ${sep} [TARGET] NO Melee mob
		NOUT ${cinfo}/listrange\ax ${sep} [TARGET] Ranged combat only Mob
		NOUT ${cinfo}/listnofire\ax ${sep} [TARGET] NO Fire spell mob
		NOUT ${cinfo}/listnomagic\ax ${sep} [TARGET] NO Magic spell mob
		NOUT ${cinfo}/listnopoison\ax ${sep} [TARGET] NO Poison spell mob
		NOUT ${cinfo}/listnocorruption\ax ${sep} [TARGET] NO Corruption spell mob
		NOUT ${cinfo}/listnodisease\ax ${sep} [TARGET] NO Disease spell mob
		NOUT ${cinfo}/listnocold\ax ${sep} [TARGET] NO Cold spell Mob
	}

	| DEBUGGING Instructions
	/if (${Select[${_section},ALL,BUG]}) {	
		/echo	
		/echo ${sep}${sep}${sepO} ${cgood}BUG HUNTING\ax ${sepC}
		NOUT ${cinfo}/varset debug TRUE\ax ${sep} turn debugging ${cgood}ON\ax
		NOUT ${cinfo}/varset debug FALSE\ax ${sep} turn debugging ${cbad}OFF\ax
		NOUT ${owarning} ${sep} There are a lot of debug echos in the code. this will slow your shit Way down.
		NOUT ${onotice} ${sep} Markers are Time in Seconds and current Macro Line.
	}
	
	
	| trigger for class specific commands
	/if (${Select[${_section},CLR,DRU,ROG,RNG,MON,PAL,SHD,WAR,BRD,MAG,WIZ,NEC,ENC,BST,BER]}) {	
		/echo
		/echo ${sep}${sep}${_section}\ax
		/call ${_section}_info()
	}

	:exit
/return



sub RNG_info()
	NOUT ${cinfo}/spice\ax ${sep} Mass Group Buff and Auspice of the Hunter if both AAs are ready. 
/return

sub MAG_info()
	NOUT ${cinfo}/g coh\ax ${sep} Will have mage cast Call of the Hero on person doing request.
	|userain

/return

sub WIZ_info()
	NOUT ${cinfo}/gather\ax ${sep} #% of manna the wizard will use gather Spells and AA.

/return

sub ENC_info()
/return

sub NEC_info()
/return

sub SHD_info()
/return

sub PAL_info()
	NOUT ${cinfo}/doyaulp\ax ${sep} ${_toggle} Use AA Yaulp.
/return

sub WAR_info()
/return

sub CLR_info()
	NOUT ${cinfo}/usegrouphot\ax ${sep} ${_toggle} Use group Heal Over Time.
	NOUT ${cinfo}/usegroupheal\ax ${sep} ${_toggle} Use group Heal.
	NOUT ${cinfo}/doyaulp\ax ${sep} ${_toggle} Use AA Yaulp.
	NOUT ${cinfo}/qm\ax ${sep} ${_toggle} Use AA Quiet Miracle on group, or not.
	NOUT ${cinfo}/issuance\ax ${sep} AA shield spam.. DISABLED ATM..
	NOUT ${cinfo}/cr\ax ${sep} Mass Group Buff Celestioal Regeneration
/return

sub ROG_info()
/return

sub BST_info()
	NOUT ${cinfo}/paragon\ax ${sep} Mass Group Buff and AParagon of Spirit if both AAs are ready.
/return

sub BER_info()
/return

sub DRU_info()
	NOUT ${cinfo}/usegroupheal\ax ${sep} ${_toggle} Use group Heal.
/return

sub SHM_info()
	NOUT ${cinfo}/aid\ax ${sep}
	NOUT ${cinfo}/cani\ax ${sep} #% of manna the shaman will use cani Spells and AA.
	NOUT ${cinfo}/usegroupheal\ax ${sep} ${_toggle} Use group Heal.
/return

sub BRD_info()
	NOUT ${cinfo}/song\ax ${sep} [melee|caster|tank|mix] mem song set set to the specific mode.
/return

