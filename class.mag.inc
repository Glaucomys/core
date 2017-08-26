|***
 *	class.mag.inc
 *	Core Magician Include
 *
 * 
 ***| 



|***
 *	DES: Main Magician Routine
 *	USE: 
 *	INI: 
 *  NOTE: 
 ***|
sub core_MAG()
	DEBUGCLASS Class Start: ${cbug}core_${Me.Class.ShortName}(${cinfo}${version${Me.Class.ShortName}}\ax${cbug})\ax
	/doevents
	ISDEAD
	CHECKTIE

	/if (${coreAuto} && !${Me.Invis}) {
		/call check_engaged
		/if (!${Bool[${Macro.Return}]}) {
			/if (${switchPcPet}) /call check_pet
			CHECKEXIT
			/if (!${timer_caldron_use}) /call use_cauldron
		}
	}

	:killtarget
	/call prep_combat
	/if (!${Bool[${Macro.Return}]}) /goto :exit

	/if (${Select[${combat},MELEE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},RANGE]} && ${isValidTarget}) {
	} else /if (${Select[${combat},NUKE]} && ${isValidTarget}) {
		/call set_combat NUKE
		/if (${switchPcPet} && !${Pet.ID} && ${coreAuto}) /call check_pet
		/call send_pet
		/if (${switchdeBuffMalo}) /call cast_malo_tash ${Target2Kill}
		/call click_DPS_${Me.Class.ShortName}
		/if (${switchdeBuffEradicate}) /call cast_eradicate_magic ${Target2Kill}
		/if (${switchdHealPet}) /call pet_heal_${Me.Class.ShortName}
		/if (${switchPcSwarm}) /call cast_nuke_servant
		/if (${useRain}) /call cast_nuke_rain		
		/if (${doclickItem}) /call click_item
		/call disc_${Me.Class.ShortName}
		/if (${DoNuke}) /call cast_nuke
		/if (${switchdHealPet}) /call pet_heal_${Me.Class.ShortName}
	} else /if (${Select[${combat},HEAL]}) {
	} else /if (${Select[${combat},DUAL]}) {
	}
	/if (${coreAuto}) {	
		/if (${switchHealCure}) /call cast_cure
		/if (${switchHealSelf}) /call patch_heal_${Me.Class.ShortName}
		/if (${switchdHealPet}) /call pet_heal_${Me.Class.ShortName}
	}
	/if (${coreAuto} && !${Spawn[${Target2Kill}].ID} || !${coreAuto} && ${Target.ID} && ${Bool[${combat}]}) {
		/call is_target_dead
		/if (!${Macro.Return}) /goto :killtarget
	}
	:exit
/return



|***
 *	DES: Discs
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub disc_MAG()
	DEBUGCLASS ${cbug}disc_${Me.Class.ShortName}()\ax 
	/doevents
	/call check_exit TRUE
	/if (${Bool[${Macro.Return}]}) /goto :exit
	ISDEAD
	CHECKTIE
	/call is_target_dead
	/if (!${Bool[${Macro.Return}]}) /goto :exit
	/if (${switchBanestrike}) /call cast_aa_banestrike ${Target2Kill}
	/if (${Me.AltAbilityReady[Force of Elements]} && ${Spawn[${Target2Kill}].Distance} <= ${Spell[Force of Elements].MyRange}) /call core_cast "Force of Elements" alt ${Target2Kill}

	:exit
/return



|***
 *	DES: summon cauldron item
 *	USE: auto on timer
 *	INI: 
 *  NOTE: 
 ***|
sub use_cauldron()
	DEBUGCLASS ${cbug}use_cauldron()\ax 
	/doevents
	ISDEAD
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	CHECKTIE
	/if (${Bool[${Cauldron}]} && ${Me.ItemReady[${Cauldron}]}) {
		/call core_cast "${Cauldron}" item
		/call sort_cursor TRUE
	}
	:exit
	/call create_timer timer_caldron_use ${tCauldron_use}
/return



|***
 *	DES: Check and resummon rods when missing
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub check_rods()
	DEBUGCLASS ${cbug}check_rods()\ax 
	/doevents
	ISDEAD
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	CHECKTIE
	| self only mod rod
	/if (${Bool[${SelfRodName}]} && !${FindItem[${SelfRodName}].ID} && !${Cursor.Name.Equal[${SelfRodName}]} && ${Me.CurrentMana} >= ${Spell[${SelfRodSpell}].Mana}) {
		/call core_cast "${SelfRodSpell}" ${buffGem} 0
		/delay 20s !${Me.Casting.ID}
		/delay 2s ${Cursor.Name.Equal[${SelfRodName}]}
		/call sort_cursor TRUE
	}
	
	| AE mod rod
	/if (${Bool[${ModRodName}]} && !${FindItem[${ModRodName}].ID} && !${Cursor.Name.Equal[${ModRodName}]}) {
		/target ID ${Me.ID}
		/call core_cast "${ModRodSpell}" alt 0
		/delay 20s !${Me.Casting.ID}
		/delay 2s ${Cursor.Name.Equal[${ModRodName}]}
		/call sort_cursor TRUE
	} else /if (${Bool[${ParadoxRodName}]} && !${Bool[${FindItem[${ParadoxRodName}]}]} && !${Bool[${Cursor.Name.Equal[${ParadoxRodName}]}]} && ${Me.CurrentMana} >= ${Spell[${ParadoxSpell}].Mana}) {
		/call core_cast  "${ParadoxSpell}" ${buffGem} ${Me.ID}
		/delay 20s !${Me.Casting.ID}
		/delay 2s ${Bool[${Cursor.Name.Equal[${ParadoxRodName}]}]}
		/call sort_cursor TRUE
	} else /if (${Bool[${OrbName}]} && !${Bool[${FindItem[${OrbName}]}]} && !${Bool[${Cursor.Name.Equal[${OrbName}]}]} && ${Me.CurrentMana} >= ${Spell[${OrbSpell}].Mana}) {
		/call core_cast "${OrbSpell}" ${buffGem} 0
		/delay 20s !${Me.Casting.ID}
		/delay 2s ${Bool[${Cursor.Name.Equal[${OrbName}]}]}
		/call sort_cursor TRUE
	} else /if (${Bool[${ElementRodName}]} && !${Bool[${FindItem[${ElementRodName}]}]} && !${Bool[${Cursor.Name.Equal[${ElementRodName}]}]} && ${Me.CurrentMana} >= ${Spell[${ElementRodSpell}].Mana}) {
		/call core_cast "${ElementRodSpell}" ${buffGem} 0
		/delay 20s !${Me.Casting.ID}
		/delay 2s ${Bool[${Cursor.Name.Equal[${ElementRodName}]}]}
		/call sort_cursor TRUE
	}
	CHECKTIE

	/if (${Bool[${SelfRodName}]} && ${Me.ItemReady[${SelfRodName}]} && ${Me.PctHPs} > 50*.${tmpHealOverride} && ${Me.PctMana} < 80*.${tmpHealOverride}) /call core_cast "${SelfRodName}" item 0
	/if (${Bool[${ModRodName}]} && ${Me.ItemReady[${ModRodName}]} && ${Me.PctHPs} > 50*.${tmpHealOverride} && ${Me.PctMana} < 90*.${tmpHealOverride}) /call core_cast "${ModRodName}" item 0
	/if (${Me.AltAbilityReady[Mana Reserve]} && ${Me.PctMana} <= 19*.${tmpHealOverride}) /call core_cast "Mana Reserve" alt 0
	
	:checkgather
  /if (${Bool[${GatherSpell}]} && ${Me.SpellReady[${GatherSpell}]} && ${Me.PctMana} < ${autoRest}*.${tmpHealOverride}) /call core_cast "${GatherSpell}" 0 0
  
	/if (${Me.AltAbilityReady[Forceful Rejuvenation]} && ${Me.PctMana} < 50*.${tmpHealOverride} && !${Me.SpellReady[${GatherSpell}]}) {
		/call core_cast "Forceful Rejuvenation" alt
		/delay 5
		/goto :checkgather
	}
	:exit
/return



|***
 *	DES: click dps thingies on bad guys
 *	USE: 
 *	INI: 
 *  NOTE: this is filler DPS. cast on thenb move on to something else to compensate for global cooldown
 ***|
sub click_DPS_MAG()
	DEBUGCLASS ${cbug}click_DPS_${Me.Class.ShortName}()\ax 
	/doevents
	ISDEAD
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	CHECKTIE
	/call is_target_dead
	/if (!${Bool[${Macro.Return}]}) /goto :exit
	/call click_horn_of_unity 	
	
	| click summon rods. DPS fillers....
	/if (${Me.ItemReady[${ParadoxRodName}]}) {
		/call MQ2Cast "${ParadoxRodName}" item -targetid|${Target2Kill}
		/goto :exit
	}
	/if (${Me.ItemReady[${OrbName}]}) {
		/call MQ2Cast "${OrbName}" item -targetid|${Target2Kill}
		/goto :exit
	}
	/if (${switchPcPet} && ${Me.ItemReady[${ElementRodName}]} && ${Target.PctHPs} > 20) {
		/call MQ2Cast "${ElementRodName}" item -targetid|${Target2Kill}
		/goto :exit
	}

	:exit
/return



|***
 *	DES: call of hero requesting toon in group.
 *	USE: /g coh
 *	INI: 
 *  NOTE: 
 ***|
#event CoH 			"#1# tells the group, 'coh'"
sub Event_CoH(string line, string _sender)
	/if (${Select[${Me.Class.ShortName},MAG]}) {
		CHECKTIE
		CHECKSAFELIST
	  /delay 10s ${Me.AltAbilityReady[Call of the Hero]}
	  /if (${Me.Invis}) /makemevisible
	  /target ID ${Spawn[pc ${_sender}].ID}
	  /delay 3s ${Target.ID} == ${Spawn[pc ${_sender}].ID}
	  ECHOCHANNEL Calling ... ${_sender}
	 	/call core_cast "Call of the Hero" alt ${Spawn[pc ${_sender}].ID}
	}
	:exit
/return



|***
 *	DES: Pet nuke
 *	USE: auto when 3+ pets in range
 *	INI: 
 *  NOTE: 
 ***|
sub cast_nuke_salvo()
	DEBUGCLASS ${cbug}cast_nuke_salvo()\ax
	ISDEAD
	/doevents
	CHECKTIE
	CHECKEXIT
	/call is_target_dead
	/if (${Bool[${Macro.Return}]}) /goto :exit
	/if (${switchPcPet} && ${Spawn[${Target2Kill}].LineOfSight} && ${Me.SpellReady[${NukeSalvo}]} && ${Me.CurrentMana} >= ${Spell[${NukeSalvo}].Mana} && ${Spawn[${Target.ID}].NearestSpawn[3,pet radius 30]}) {
		/call core_cast "${NukeSalvo}" 0 ${Target2Kill}
	}
	:exit
/return



|***
 *	DES: Pet heal routines
 *	USE: /call petHeal_${Me.Class.ShortName}
 *	INI: 
 *  NOTE: 
 ***|
sub pet_heal__MAG()
	DEBUGCLASS ${cbug}pet_heal_${Me.Class.ShortName}()${citem}
	ISDEAD
	/doevents
	/call check_exit TRUE
	/if (${Bool[${Macro.Return}]} || !${switchPcPet}) /goto :exit
	CHECKTIE
	/if (${Pet.PctHPs} < 40 && ${Me.AltAbilityReady[${PetAAHeal}]}) /call core_cast "${PetAAHeal}" alt ${Me.Pet.ID}
	/if (${Pet.PctHPs} < 60 && ${Me.SpellReady[${PetHealSpell}]}) /call core_cast "${PetHealSpell}" ${PetHealType} ${Me.Pet.ID}
	/if (${Pet.PctHPs} < 80 && ${Me.SpellReady[${PetRenewalSpell}]}) /call core_cast "${PetRenewalSpell}" ${PetRenewalType} ${Me.Pet.ID}
	:exit
/return



|***
 *	DES: forces recasting of rods not at max charge or missing
 *	USE: /rods
 *	INI: 
 *  NOTE: 
 ***|
#bind Bind_cast_rods /rods
sub Bind_cast_rods()
	DEBUGCLASS ${cbug}Bind_cast_rods()${citem}
	ISDEAD
	/doevents
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	CHECKTIE
	/if (${Bool[${FindItem[${ParadoxRodName}]}]} && ${FindItem[${ParadoxRodName}].Charges} < 5) {
		VOUT ${citem}${ParadoxRodName}\ax has ${FindItem[${ParadoxRodName}].Charges} ${If[${FindItem[${ParadoxRodName}].Charges}==1,charge,charges]} ${sep} ${odestroy}
		/if (${Bool[${FindItem[${ParadoxRodName}]}]}) /nomodkey /itemnotify "${ParadoxRodName}" leftmouseup
		/delay 5s ${Cursor.Name.Equal[${ParadoxRodName}]}
		/if (${Cursor.Name.Equal[${ParadoxRodName}]}) /destroy
	}
	/if (${Bool[${FindItem[${OrbName}]}]} && ${FindItem[${OrbName}].Charges} < 10) {
		VOUT ${citem}${OrbName}\ax has ${FindItem[${OrbName}].Charges} ${If[${FindItem[${OrbName}].Charges}==1,charge,charges]} ${sep} ${odestroy}
		/if (${Bool[${FindItem[${OrbName}]}]}) /nomodkey /itemnotify "${OrbName}" leftmouseup
		/delay 2s ${Cursor.Name.Equal[${OrbName}]}
		/if (${Cursor.Name.Equal[${OrbName}]}) /destroy
	}
	/if (${Bool[${FindItem[${SelfRodName}]}]} && ${FindItem[${SelfRodName}].Charges} < 3) {
		VOUT ${citem}${SelfRodName}\ax has ${FindItem[${SelfRodName}].Charges} ${If[${FindItem[${SelfRodName}].Charges}==1,charge,charges]} ${sep} ${odestroy}
		/if (${Bool[${FindItem[${SelfRodName}]}]}) /nomodkey /itemnotify "${SelfRodName}" leftmouseup
		/delay 2s ${Cursor.Name.Equal[${SelfRodName}]}
		/if (${Cursor.Name.Equal[${SelfRodName}]}) /destroy
	}
	/if (${Bool[${FindItem[${ModRodName}]}]} && ${FindItem[${ModRodName}].Charges} < 3) {
		VOUT ${citem}${ModRodName}\ax has ${FindItem[${ModRodName}].Charges} ${If[${FindItem[${ModRodName}].Charges}==1,charge,charges]} ${sep} ${odestroy}
		/if (${Bool[${FindItem[${ModRodName}]}]}) /nomodkey /itemnotify "${ModRodName}" leftmouseup
		/delay 2s ${Cursor.Name.Equal[${ModRodName}]}
		/if (${Cursor.Name.Equal[${ModRodName}]}) /destroy
	}
	/delay 5
	/if (${switchBuffItem}) /call check_rods
	:exit
/return



|***
 *	DES: emergency healing
 *	USE: auto
 *	INI: 
 *  NOTE: 
 ***|
sub patch_heal_MAG()
	DEBUGCLASS ${cbug}patch_heal_${Me.Class.ShortName}()\ax
	/call check_exit
	/if (${Bool[${Macro.Return}]}) /goto :exit
	CHECKTIE
	/if (${Me.PctHPs} <= 80*.${tmpHealOverride} && ${Me.ItemReady[Prayers of Life]}) /call core_cast "Prayers of Life" item 0
	/if (${Me.PctHPs} <= 60*.${tmpHealOverride} && ${Me.ItemReady[Overflowing Urn of Life]}) /call core_cast "Overflowing Urn of Life" item 0
	:exit
/return



|***
 *	DES: Class controls
 *	USE: /mag []
 *	INI: 
 *  NOTE:
 ***|
#bind setMAG /mag
sub Bind_setMAG(string _classType, string _classVerbage)
	DEBUG ${cbug}Bind_setMAG(type:${_classType}, verbage:${_classVerbage})\ax
	/if (!${Select[${Me.Class.ShortName},MAG]}) {
		/call echo_bitch EQBC
		/goto :exit
	}
	
	| enable/disable summer line of spells
	/if (${Bool[${_classType.Find[TEST]}]}) {
		| /call Event_SetVar NA switchNukeSummer Spells ${If[${switchNukeSummer},FALSE,TRUE]}


	} else {
		/call echos listtype ${Me.Class.ShortName.Lower} ${Me.Class.ShortName.Lower}
	} 
	:exit
/return