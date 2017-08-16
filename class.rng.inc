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
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}(${cinfo}${version${Me.Class.ShortName}}\ax${cbug})\ax
	/doevents
	ISDEAD
	CHECKTIE

	/if (${Bool[${RangeItemSummon}]}) /call check_ammo

	:killtarget
	/call prep_combat
	/if (!${Bool[${Macro.Return}]}) /goto :exit

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
		/call set_combat MELEE
		/call send_pet
		/call stick_to_target
		/call use_skill_melee
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke
		/if (${Bool[${NukeSummer}]} && !${Bool[${timer_cast_nuke_summer}]}) /call cast_nuke_summer ${Target2Kill}
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
		/if (${Bool[${NukeSummer}]} && !${timer_cast_nuke_summer}) /call cast_nuke_summer ${Target2Kill}
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
		/call set_combat NUKE
		/call send_pet
		CHECKTIE
		/call disc_${Me.Class.ShortName}
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke
		/if (${Bool[${NukeSummer}]} && !${timer_cast_nuke_summer}) /call cast_nuke_summer ${Target2Kill}
		/call click_DPS_${Me.Class.ShortName}
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
		/call set_combat MELEE
		/call use_skill_melee
		/if (${Bool[${NukeSummer}]} && !${timer_cast_nuke_summer}) /call cast_nuke_summer ${Target2Kill}
		/if (${DoDoT}) /call cast_DOT
		/if (${DoNuke}) /call cast_nuke
		/call disc_${Me.Class.ShortName}
		/call click_BP
		/call use_skill_melee
		/if (${doclickItem}) /call click_item
		/call click_DPS_${Me.Class.ShortName}
	} 
	/call use_mod_rod
	/if (${coreAuto}) {	
		/if (${switchHealCure}) /call cast_cure
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
	}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} || !${coreAuto} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Bool[${Macro.Return}]}) /goto :killtarget
	}

	:exit
/return



|***
 *	DES: Combat skills, Disciplines and Alternate abilities.
 *	USE: /call disc_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub disc_RNG()
	DEBUGCLASS	${cbug}disc_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit TRUE
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/if (${switchdeBuffSnare}) /call cast_AA_snare ${Target2Kill}

	/if (${switchAgroMode}) /call get_agro_${Me.Class.ShortName}
	/if (${Target.ID} && ${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	/if (${Me.SpellReady[${DoTSwarmDS}]} && !${Bool[${Spawn[${Target.Buff[${DoTSwarmDSdeBuff}].Caster}].ID}]}) /call core_cast "${DoTSwarmDS}" 0 ${Target2Kill}
	/if (${Target.ID} && ${switchdeBuffEradicate}) /call cast_eradicate_magic ${Target2Kill}
	/if (${Target.ID} && !${timer_Elemental_Arrow}) /call cast_AA_elemental_arrows ${Target2Kill}
	/call use_skill_melee
	/if (${Target.ID} && ${Bool[${DiscJolt}]} && !${switchAgroMode}) /call core_cast "${DiscJolt}" disc ${Target2Kill}
	/if (${Target.ID} && ${Bool[${BladeStorm}]}) /call core_cast "${BladeStorm}" disc ${Target2Kill}
	/if (${Target.ID} && ${Bool[${DiscSqual}]}) /call core_cast "${DiscSqual}" disc ${Target2Kill}
	:exit
/return



|***
 *	DES: MGB ranger Auspice if both AAs are ready
 *	USE: /bc spice, /spice
 *	INI: 
 *  NOTE: Will not cast spice if the buff is present or if your name is Norbert.
 ***|
#bind cast_AA_auspice /spice
sub Bind_cast_AA_auspice()
	/if (${Select[${Me.Class.ShortName},RNG]}) {
		/delay 2s !${Me.Casting.ID}
		/call cast_AA_auspice
	}
/return

sub cast_AA_auspice()
	DEBUGCLASS ${cbug}cast_auspice()\ax ${If[${Me.AltAbilityReady[Mass Group Buff]},${cgood}MGB\ax,${cbad}MGB\ax]} ${If[${Me.AltAbilityReady[${RNGauspice}]},${cgood}Spice\ax,${cbad}Spice\ax]}
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/if (${Me.AltAbilityReady[Mass Group Buff]} && ${Me.AltAbilityReady[${RNGauspice}]} && !${Bool[${Me.Song[${RNGauspice}]}]}) {
		/call core_cast "Mass Group Buff" alt 0
		/call core_cast "${RNGauspice}" alt 0
		| /if (${Bool[ClassChannel}]}) /${ClassChannel} ${AuspiceTXT}
	 	| /if (${Bool[${Raid.Members}]}) /rs ${AuspiceTXT}
	} else /if (${Bool[${Me.Song[${RNGauspice}]}]}) {
		NOUT ${cbad}QUIT\ax trying to cast ${csp}Auspice of the Hunter\ax while its running.	
	}		
	:exit
/return



|***
 *	DES: emergency healing
 *	USE: /call patch_heal_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_RNG()
	DEBUGCLASS ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	/call check_exit
	/if (${Bool[${Macro.Return}]} || ${Me.PctHPs} > 81*.${tmpHealOverride}) /goto :exit
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride}) /call core_cast "Prayers of Life" item 0
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} || ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius ${Spell[${DiscHeal}].MyRange} playerstate 4 targetable]} >= 3) /if (${Select[${combat},MELEE]} && !${Bool[${Me.Song[Bladespur Reflection]}]}) /call core_cast "${DiscHeal}" disc ${Target.ID}
	/if (${Me.PctHPs} <= 70*.${tmpHealOverride}) /call core_cast "Small Manisi Branch" item 0
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride}) /call core_cast "Overflowing Urn of Life" item 0
	/if (${Me.PctHPs} <= 55*.${tmpHealOverride} && (${Bool[${NameList.Find[${Target.CleanName}]})]}	|| ${SpawnCount[npc ${If[${setIgnoreLOS},,los]} radius 50 playerstate 4 targetable]} >= 3)) /call core_cast "Bulwark of the Brownies" alt 0
	/if (${Me.PctHPs} <= 40*.${tmpHealOverride}) /call core_cast "Protection of the Spirit Wolf" alt 0
	/if (${Me.PctHPs} <= 30*.${tmpHealOverride}) /call core_cast "Outrider's Evasion" alt 0
	:exit
/return



|***
 *	DES: Agro Routines
 *	USE: /call get_agro_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: manual/Named/raid. all sorts...
 ***|
sub get_agro_RNG()
	DEBUGCLASS ${cbug}get_agro_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/if (${switchAgroMode}) {
		/if (${Me.AltAbilityReady[Ferocious Kick Subtlety: Enabled]}) /call core_cast "Ferocious Kick Subtlety: Enabled" alt 0
		/if (${Bool[${DiscEnrage}]} && ${Select[${combat},MELEE]}) /call core_cast "${DiscEnrage}" disc ${Target2Kill}
	}
	/if (!${switchAgroMode}) {
			/if (${Me.AltAbilityReady[Ferocious Kick Subtlety: Disabled]}) /call core_cast "Ferocious Kick Subtlety: Disabled" alt 0
	}
	/call use_skill_melee
	:exit
/return



|***
 *	DES: Ranger Elemental Arrows 
 *	USE: /call cast_AA_elemental_arrows
 *	INI: 
 *  NOTE: Rotates the three ranger elemental arrows. Prevents overlapping (inc other rangers). It will attempt to keep both ice and fire debufs on the mob at the same time.
 ***|
sub cast_AA_elemental_arrows(int _tmpID)
	DEBUGCLASS ${cbug}cast_AA_elemental_arrows()\ax
	ISDEAD
	/doevents
	/call check_exit TRUE
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/call use_skill_melee
	/if (${Bool[${ElementalArrow}]}) {
		/if (!${Bool[${Target.Buff["Elemental Arrow: Ice"]}]} && !${Bool[${Target.Buff["Elemental Arrow: Fire"]}]} && !${Bool[${Target.Buff[${GlacialArow}]}]} && !${Bool[${Target.Buff[${VolatileArrow}]}]}) {
			/call core_cast "${ElementalArrow}" alt ${_tmpID}
			/call create_timer timer_Elemental_Arrow 25s
			/goto :exit
		} else {
			/call create_timer timer_Elemental_Arrow 10s
		}
	}
	/if (${Bool[${GlacialArow}]}) {
		/if (!${Bool[${Target.Buff["Elemental Arrow: Ice"]}]} && !${Bool[${Target.Buff[${GlacialArow}]}]}) {
			/call core_cast "${GlacialArow}" alt ${_tmpID}
			/call create_timer timer_Elemental_Arrow 25s
			/goto :exit
		} else {
			/call create_timer timer_Elemental_Arrow 10s
		}
	}	
	/if (${Bool[${VolatileArrow}]}) {
		/if (!${Bool[${Target.Buff["Elemental Arrow: Fire"]}]} && !${Bool[${Target.Buff[${VolatileArrow}]}]}) {
			/call core_cast "${VolatileArrow}" alt ${_tmpID}
			/call create_timer timer_Elemental_Arrow 25s
			/goto :exit
		}	else {
			/call create_timer timer_Elemental_Arrow 10s
		}
	}
	:exit
/return



|***
 *	DES: DPS Clickies
 *	USE: /call click_DPS_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub click_DPS_RNG()
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/call use_skill_melee
	/call click_horn_of_unity 	
	:exit
/return



|***
 *	DES: Control for Spam Agro Nuke
 *	USE: /call cast_nuke_summer
 *	INI: 
 *  NOTE: When burn is engaged, the timer for this is ignored.
 ***|
sub cast_nuke_summer(int _tmpID)
	DEBUGCLASS ${cbug}cast_nuke_summer()\ax 
	ISDEAD
	/doevents
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /goto :exit		
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit	
	/call core_cast "${NukeSummer}" 0 ${_tmpID}
	/call create_timer timer_cast_nuke_summer ${tNuke_Summer}
	:exit
/return

