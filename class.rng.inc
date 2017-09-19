|***
 *	class.rng.inc
 *	Core Ranger Include
 *
 *
 ***|
 



|***
 *	DES: Main Ranger Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_RNG()
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}()\ax
	/doevents
	AMIDEAD

	CHECKTIE

	/if (${Bool[${RangeItemSummon}]}) /call check_ammo

	:killtarget
	/call prep_combat
	/if (!${Macro.Return}) /return

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/call send_pet
		/call stick_to_target
		/call use_skill_melee
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke
		/if (${switchNukeSummer} && ${Bool[${NukeSummer}]} && !${Bool[${timer_cast_nuke_summer}]}) /call cast_nuke_summer ${Target2Kill}
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/call stick_to_target
		/call use_skill_melee
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
		/call set_combat RANGE
		/call send_pet
		CHECKTIE
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke
		/if (${switchNukeSummer} && ${Bool[${NukeSummer}]} && !${timer_cast_nuke_summer}) /call cast_nuke_summer ${Target2Kill}
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
		/call set_combat NUKE
		/call send_pet
		CHECKTIE
		/call disc_${Me.Class.ShortName}
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke
		/if (${switchNukeSummer} && ${Bool[${NukeSummer}]} && !${timer_cast_nuke_summer}) /call cast_nuke_summer ${Target2Kill}
		/call click_DPS_${Me.Class.ShortName}
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
		/call set_combat MELEE
		/call use_skill_melee
		/if (${switchNukeSummer} && ${Bool[${NukeSummer}]} && !${timer_cast_nuke_summer}) /call cast_nuke_summer ${Target2Kill}
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/call use_skill_melee
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}
	} 
	/if (${coreAuto}) {	
		/call use_mod_rod
		/if (${switchHealCure}) /call cast_cure
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
	}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} || !${coreAuto} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Macro.Return}) /goto :killtarget
	}
/return



|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: /call disc_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub disc_RNG()
	DEBUGCLASS	${cbug}disc_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	/call is_target_dead
	/if (${Macro.Return}) /return
	/if (${switchdeBuffSnare}) /call cast_AA_snare ${Target2Kill}

	/if (${switchAgro}) /call get_agro_${Me.Class.ShortName}
	/if (${Target.ID} && ${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	/if (${Me.SpellReady[${DoTSwarmDS}]} && !${Bool[${Spawn[${Target.Buff[${DoTSwarmDSdeBuff}].Caster}].ID}]}) /call core_cast "${DoTSwarmDS}" 0 ${Target2Kill}
	/if (${Target.ID} && ${switchdeBuffEradicate}) /call cast_eradicate_magic ${Target2Kill}
	/if (${Target.ID} && !${timer_Elemental_Arrow}) /call cast_AA_elemental_arrows ${Target2Kill}
	/call use_skill_melee
	/if (${Target.ID} && ${Bool[${DiscJolt}]} && !${switchAgro}) /call core_cast "${DiscJolt}" disc ${Target2Kill}
	/if (${Target.ID} && ${Bool[${BladeStorm}]}) /call core_cast "${BladeStorm}" disc ${Target2Kill}
	/if (${Target.ID} && ${Bool[${DiscSqual}]}) /call core_cast "${DiscSqual}" disc ${Target2Kill}
/return



|***
 *	DES: MGB ranger Auspice if both AAs are ready
 *	USE: 
 *	INI: 
 *  NOTE: Will not cast spice if the buff is present or if your name is Norbert.
 ***|
sub cast_AA_auspice()
	DEBUGCLASS ${cbug}cast_auspice()\ax ${If[${Me.AltAbilityReady[Mass Group Buff]},${cgood}MGB\ax,${cbad}MGB\ax]} ${If[${Me.AltAbilityReady[${RNGauspice}]},${cgood}Spice\ax,${cbad}Spice\ax]}
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[${RNGauspice}]} && !${Bool[${Me.Song[${RNGauspice}]}]}) {
		/call core_cast "Mass Group Buff" alt 0
		/call core_cast "${RNGauspice}" alt 0
		| /if (${Bool[ClassChannel}]}) /${ClassChannel} ${AuspiceTXT}
	 	| /if (${Bool[${Raid.Members}]}) /rs ${AuspiceTXT}
	} else /if (${Bool[${Me.Song[${RNGauspice}]}]}) {
		NOUT ${cbad}QUIT\ax trying to cast ${csp}Auspice of the Hunter\ax while its running.	
	}		
/return



|***
 *	DES: emergency healing
 *	USE: /call patch_heal_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_RNG()
	DEBUGCLASS ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	/if (${Me.PctHPs} > 81*.${tmpHealOverride}) /return
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride}) /call core_cast "Prayers of Life" item 0
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} || ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${Spell[${DiscHeal}].MyRange} playerstate 4 targetable]} >= 3) /if (${Select[${combat},MELEE]} && !${Bool[${Me.Song[Bladespur Reflection]}]}) /call core_cast "${DiscHeal}" disc ${Target.ID}
	/if (${Me.PctHPs} <= 70*.${tmpHealOverride}) /call core_cast "Small Manisi Branch" item 0
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride}) /call core_cast "Overflowing Urn of Life" item 0
	/if (${Me.PctHPs} <= 55*.${tmpHealOverride} && (${Bool[${NameList.Find[${Target.CleanName}]})]}	|| ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius 50 playerstate 4 targetable]} >= 3)) /call core_cast "Bulwark of the Brownies" alt 0
	/if (${Me.PctHPs} <= 40*.${tmpHealOverride}) /call core_cast "Protection of the Spirit Wolf" alt 0
	/if (${Me.PctHPs} <= 30*.${tmpHealOverride}) /call core_cast "Outrider's Evasion" alt 0
/return



|***
 *	DES: Agro Routines
 *	USE: /call get_agro_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: manual/Named/raid. all sorts...
 ***|
sub get_agro_RNG()
	DEBUGCLASS ${cbug}get_agro_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	/if (${switchAgro}) {
		/if (${Me.AltAbilityReady[Ferocious Kick Subtlety: Enabled]}) /call core_cast "Ferocious Kick Subtlety: Enabled" alt 0
		/if (${Bool[${DiscEnrage}]} && ${Select[${combat},MELEE]}) /call core_cast "${DiscEnrage}" disc ${Target2Kill}
	}
	/if (!${switchAgro}) {
			/if (${Me.AltAbilityReady[Ferocious Kick Subtlety: Disabled]}) /call core_cast "Ferocious Kick Subtlety: Disabled" alt 0
	}
	/call use_skill_melee
/return



|***
 *	DES: Ranger Elemental Arrows 
 *	USE: /call cast_AA_elemental_arrows
 *	INI: 
 *  NOTE: Rotates the three ranger elemental arrows. Prevents overlapping (inc other rangers). It will attempt to keep both ice and fire debufs on the mob at the same time.
 ***|
sub cast_AA_elemental_arrows(int _tmpID)
	DEBUGCLASS ${cbug}cast_AA_elemental_arrows()\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	/call use_skill_melee
	/if (${Bool[${ElementalArrow}]}) {
		/if (!${Bool[${Target.Buff["Elemental Arrow: Ice"]}]} && !${Bool[${Target.Buff["Elemental Arrow: Fire"]}]} && !${Bool[${Target.Buff[${GlacialArow}]}]} && !${Bool[${Target.Buff[${VolatileArrow}]}]}) {
			/call core_cast "${ElementalArrow}" alt ${_tmpID}
			/call create_timer timer_Elemental_Arrow 25s
			/return
		} else {
			/call create_timer timer_Elemental_Arrow 10s
		}
	}
	/if (${Bool[${GlacialArow}]}) {
		/if (!${Bool[${Target.Buff["Elemental Arrow: Ice"]}]} && !${Bool[${Target.Buff[${GlacialArow}]}]}) {
			/call core_cast "${GlacialArow}" alt ${_tmpID}
			/call create_timer timer_Elemental_Arrow 25s
			/return
		} else {
			/call create_timer timer_Elemental_Arrow 10s
		}
	}	
	/if (${Bool[${VolatileArrow}]}) {
		/if (!${Bool[${Target.Buff["Elemental Arrow: Fire"]}]} && !${Bool[${Target.Buff[${VolatileArrow}]}]}) {
			/call core_cast "${VolatileArrow}" alt ${_tmpID}
			/call create_timer timer_Elemental_Arrow 25s
			/return
		}	else {
			/call create_timer timer_Elemental_Arrow 10s
		}
	}
/return



|***
 *	DES: DPS Clickies
 *	USE: /call click_DPS_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_RNG()
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	AMIDEAD
	CHECKEXIT
	/doevents
	/call use_skill_melee
	/call click_horn_of_unity 	
/return



|***
 *	DES: Control for Spam Agro Nuke
 *	USE: /call cast_nuke_summer
 *	INI: 
 *  NOTE: When burn is engaged, the timer for this is ignored.
 ***|
sub cast_nuke_summer(int _tmpID)
	DEBUGCLASS ${cbug}cast_nuke_summer()\ax 
	/if (!${switchNukeSummer}) /return	
	AMIDEAD
	CHECKEXIT
	/doevents
	/call create_timer timer_cast_nuke_summer ${tNuke_Summer}
	/call is_target_dead
	/if (${Macro.Return}) /return		
	/call core_cast "${NukeSummer}" 0 ${_tmpID}
/return



 |***
 *	DES: Ranger burn routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_RNG()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	AMIDEAD
	CHECKEXIT
	/doevents
	
	| mele disc
	/if (${Me.AltAbilityReady[Empowered Blades]} && !${Bool[${Me.ActiveDisc}]} && !${Bool[${Me.Buff[${useGlyph}]}]}) {
		/call core_cast "Empowered Blades" alt 0 
	} else /if (${Me.CombatAbilityReady[${DiscMele}]} && !${Bool[${Me.ActiveDisc}]} && !${Bool[${Me.Song[Empowered Blades]}]} && !${Bool[${Me.Buff[${useGlyph}]}]}) {
		/call core_cast "${DiscMele}" disc 0
	} else /if (!${Bool[${Me.ActiveDisc}]} && !${Me.CombatAbilityReady[${DiscMele}]} && !${Me.AltAbilityReady[Empowered Blades]} && !${Bool[${Me.Song[Empowered Blades]}]}) {
		/call use_glyph
	} else /if (!${Bool[${Me.ActiveDisc}]} && !${Me.CombatAbilityReady[${DiscMele}]} && !${Me.AltAbilityReady[Empowered Blades]} && !${Bool[${Me.Song[Empowered Blades]}]} && !${Bool[${Me.Buff[${useGlyph}]}]}) {
		/call core_cast "${Epic}" item 0
	}
	
	/call cast_AA_spire
	
	| proc
	/if (${Me.AltAbilityReady[Imbued Ferocity]} && !${Bool[${Me.Buff[${RNGspire1}]}]}) {
		/call core_cast "Imbued Ferocity" alt 0
	}

	| cheeta
	/if (${Me.AltAbilityReady[${RNGcheetah}]}) /call core_cast "${RNGcheetah}" alt 0 	
	
	| att buffs
	/if (${Me.AltAbilityReady[Outrider's Accuracy]} && !${Bool[${Me.Buff[Outrider's Attack]}]}) {
		/call core_cast "Outrider's Accuracy" alt 0
	} else /if (${Me.AltAbilityReady[Outrider's Attack]}  && !${Bool[${Me.Song[Outrider's Accuracy]}]} && !${Me.AltAbilityReady[Outrider's Accuracy]} ) {
		/call core_cast "Outrider's Attack" alt 0
	}
	
	| guardian buff
	/if (${Me.AltAbilityReady[Guardian of the Forest]} && !${Bool[${Me.Song[${RNGgroupguardian}]}]}) {  
		/call core_cast "Guardian of the Forest" alt 0
	} else /if (${Me.AltAbilityReady[${RNGgroupguardian}]} && !${Bool[${Me.Song[Guardian of the Forest]}]} && !${Me.AltAbilityReady[Guardian of the Forest]}) {
		/call core_cast "${RNGgroupguardian}" alt 0
	}
		
	| swarm
	/if (${switchPcSwarm} && ${Me.AltAbilityReady[Pack Hunt]}) /call core_cast "Pack Hunt" alt ${Target2Kill}    
	
	| ae regen 
	/if (!${Bool[${DiscHeal}]} && !${Bool[${Me.Song[Bladespur Reflection]}]} && ${Me.CombatAbilityReady[${DiscHeal}]}) /call core_cast "${DiscHeal}" disc ${Target2Kill}
	
		
	/call synergy_click_Circle
	/if (${doclickItemBurn}) /call click_item_burn
 /return
 	


|***
 *	DES: Class controls
 *	USE: /RNG []
 *	INI: 
 *  NOTE:
 ***|
#bind setRNG /rng
sub Bind_setRNG(string _classType, string _classVerbage)
	DEBUG ${cbug}Bind_setRNG(type:${_classType}, verbage:${_classVerbage})\ax
	/if (!${Select[${Me.Class.ShortName},RNG]}) {
		/call echo_bitch EQBC
		/return
	}
	/declare _count	int local 0
	
	| enable/disable summer line of spells
	/if (${Bool[${_classType.Find[summer]}]}) {
		/call writeINI switchNukeSummer Spells ${If[${switchNukeSummer},FALSE,TRUE]}
		/call echos switch "Summer ${sep} ${csp}${switchNukeSummer}\ax" switchNukeSummer

	| spire control
	} else /if (${Bool[${_classType.Equal[spiregroup]}]} || ${Bool[${_classType.Equal[spireraid]}]}) {
		/declare _spiretype string local ${_classType.Left[5].Lower}${_classType.Mid[6,1].Upper}${_classType.Right[-6].Lower}
		/if (${Range.Between[0,3:${Int[${_classVerbage}]}]}) {
			/call writeINI ${_spiretype} Buffs ${_classVerbage}
		} 
		/declare _listout 	string 	local
		/declare _sep 			bool 		local
		/for _count 1 to ${typeSpire.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${If[${${_spiretype}} == ${typeSpire.Arg[${_count},|]},${cgood},${cinfo}]}${typeSpire.Arg[${_count},|]}\ax   
			/varset _sep TRUE
		/next _count
		VOUT /${Me.Class.ShortName.Lower} ${_classType.Lower} [${_listout}]	

	| mgb spice
	} else /if (${Bool[${_classType.Find[spice]}]}) {
		/delay 2s !${Me.Casting.ID}
		/call cast_AA_auspice

	} else {
		/call echos listtype ${Me.Class.ShortName.Lower} ${Me.Class.ShortName.Lower}
	} 
/return



