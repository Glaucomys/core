|***
 *	toast.inc
 *	Class synergy toast routines
 *
 *
 *
 *
 *
 * ex: to check for ranger spice: ${Bool[${Me.Song[${RNGauspice}]}]} returns TRUE/FALSE
 *
 ***|
 
 
 
|***
 *	DES: Group/AE toast effects by class
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: This is for class skills that touch group or raid.
 ***| 
sub synergy_core()

	| Cleric
	/call DeclareCoreIniVar CLRspire1							string	Synergy "Fundament: First Spire of Divinity"
	/call DeclareCoreIniVar CLRspire2							string	Synergy "Fundament: Second Spire of Divinity"
	/call DeclareCoreIniVar CLRspire3							string	Synergy "Fundament: Third Spire of Divinity"
	/call DeclareCoreIniVar CLRsynergyAA					string	Synergy "Templar's Synergy"

	| Druid
	/call DeclareCoreIniVar DRUspire1							string	Synergy "Fundament: First Spire of Nature"
	/call DeclareCoreIniVar DRUspire2							string	Synergy "Fundament: Second Spire of Nature"
	/call DeclareCoreIniVar DRUspire3							string	Synergy "Fundament: Third Spire of Nature"
	/call DeclareCoreIniVar DRUsynergyAA					string	Synergy "Preserver's Synergy"

	| Shaman
	/call DeclareCoreIniVar SHMspire1							string	Synergy "Fundament: First Spire of Ancestors"
	/call DeclareCoreIniVar SHMspire2							string	Synergy "Fundament: Second Spire of Ancestors"
	/call DeclareCoreIniVar SHMspire3							string	Synergy "Fundament: Third Spire of Ancestors"
	/call DeclareCoreIniVar SHMsynergyAA					string	Synergy "Luminary's Synergy"

	| Warrior
	/call DeclareCoreIniVar WARspire1							string	Synergy "Fundament: First Spire of the Warlord"
	/call DeclareCoreIniVar WARspire2							string	Synergy "Fundament: Second Spire of the Warlord"
	/call DeclareCoreIniVar WARspire3							string	Synergy "Fundament: Third Spire of the Warlord"
	/call DeclareCoreIniVar WARsynergyAA					string	Synergy "Myrmidon's Synergy"

	| Shadow Knight
	/call DeclareCoreIniVar SHDspire1							string	Synergy "Fundament: First Spire of the Reavers"
	/call DeclareCoreIniVar SHDspire2							string	Synergy "Fundament: Second Spire of the Reavers"
	/call DeclareCoreIniVar SHDspire3							string	Synergy "Fundament: Third Spire of the Reavers"
	/call DeclareCoreIniVar SHDsynergyAA					string	Synergy "Knight's Synergy"

	| Paladin
	/call DeclareCoreIniVar PALspire1							string	Synergy "Fundament: First Spire of Holiness"
	/call DeclareCoreIniVar PALspire2							string	Synergy "Fundament: Second Spire of Holiness"
	/call DeclareCoreIniVar PALspire3							string	Synergy "Fundament: Third Spire of Holiness"
	/call DeclareCoreIniVar PALsynergyAA					string	Synergy "Knight's Synergy"

	| Magician
	/call DeclareCoreIniVar MAGspire1							string	Synergy "Fundament: First Spire of the Elements"
	/call DeclareCoreIniVar MAGspire2							string	Synergy "Fundament: Second Spire of the Elements"
	/call DeclareCoreIniVar MAGspire3							string	Synergy "Fundament: Third Spire of the Elements"
	/call DeclareCoreIniVar MAGsynergyAA					string	Synergy "Conjurer's Synergy"

	| Enchanter	
	/call DeclareCoreIniVar ENCspire1							string	Synergy "Fundament: First Spire of Enchantment"
	/call DeclareCoreIniVar ENCspire2							string	Synergy "Fundament: Second Spire of Enchantment"
	/call DeclareCoreIniVar ENCspire3							string	Synergy "Fundament: Third Spire of Enchantment"
	/call DeclareCoreIniVar ENCIoG								string	Synergy "Illusions of Grandeur"
	/call DeclareCoreIniVar ENCMC									string	Synergy "Mental Contortion"
	/call DeclareCoreIniVar ENCsynergyAA					string	Synergy "Begulier's Synergy"
	
	| Wizard 
	/call DeclareCoreIniVar WIZspire1							string	Synergy "Fundament: First Spire of the Arcanum"
	/call DeclareCoreIniVar WIZspire2							string	Synergy "Fundament: Second Spire of the Arcanum"
	/call DeclareCoreIniVar WIZspire3							string	Synergy "Fundament: Third Spire of the Arcanum"
	/call DeclareCoreIniVar WIZmannaburn					string	Synergy "Manna Burn"
	/call DeclareCoreIniVar WIZsynergyAA					string	Synergy "Evoker's Synergy"

	| Necromancer
	/call DeclareCoreIniVar NECspire1							string	Synergy "Fundament: First Spire of Necromancy"
	/call DeclareCoreIniVar NECspire2							string	Synergy "Fundament: Second Spire of Necromancy"
	/call DeclareCoreIniVar NECspire3							string	Synergy "Fundament: Third Spire of Necromancy"
	/call DeclareCoreIniVar NECsynergyAA					string	Synergy "Defiler's Synergy"

	| Ranger
	/call DeclareCoreIniVar RNGspire1							string	Synergy "Fundament: First Spire of the Pathfinders"
	/call DeclareCoreIniVar RNGspire2							string	Synergy "Fundament: Second Spire of the Pathfinders"
	/call DeclareCoreIniVar RNGspire3							string	Synergy "Fundament: Third Spire of the Pathfinders"
	/call DeclareCoreIniVar RNGauspice						string	Synergy "Auspice of the Hunter"
	/call DeclareCoreIniVar RNGcheetah						string	Synergy "Scarlet Cheetah's Fang"
	/call DeclareCoreIniVar RNGgroupguardian			string	Synergy "Group Guardian of the Forest"
	/call DeclareCoreIniVar RNGsynergyAA					string	Synergy "Outrider's Synergy"

	| Bard
	/call DeclareCoreIniVar BRDspire1							string	Synergy "Fundament: First Spire of the Minstrel"
	/call DeclareCoreIniVar BRDspire2							string	Synergy "Fundament: Second Spire of the Minstrel"
	/call DeclareCoreIniVar BRDspire3							string	Synergy "Fundament: Third Spire of the Minstrel"
	/call DeclareCoreIniVar BRDsynergyAA					string	Synergy "Troubadour's Synergy"

	| Monk
	/call DeclareCoreIniVar MONspire1							string	Synergy "Fundament: First Spire of the Sensei"
	/call DeclareCoreIniVar MONspire2							string	Synergy "Fundament: Second Spire of the Sensei"
	/call DeclareCoreIniVar MONspire3							string	Synergy "Fundament: Third Spire of the Sensei"
	/call DeclareCoreIniVar MONsynergyAA					string	Synergy 

	| Beastlord
	/call DeclareCoreIniVar BSTspire1							string	Synergy "Fundament: First Spire of the Savage Lord"
	/call DeclareCoreIniVar BSTspire2							string	Synergy "Fundament: Second Spire of the Savage Lord"
	/call DeclareCoreIniVar BSTspire3							string	Synergy "Fundament: Third Spire of the Savage Lord"
	/call DeclareCoreIniVar BSTsynergyAA					string	Synergy "Primalist's Synergy"

	| Berzerker
	/call DeclareCoreIniVar BERspire1							string	Synergy "Fundament: First Spire of Savagery"
	/call DeclareCoreIniVar BERspire2							string	Synergy "Fundament: Second Spire of Savagery"
	/call DeclareCoreIniVar BERspire3							string	Synergy "Fundament: Third Spire of Savagery"
	/call DeclareCoreIniVar BERsynergyAA					string	Synergy "Fury's Synergy"

	| Rogue
	/call DeclareCoreIniVar ROGspire1							string	Synergy "Fundament: First Spire of the Rake"
	/call DeclareCoreIniVar ROGspire2							string	Synergy "Fundament: Second Spire of the Rake"
	/call DeclareCoreIniVar ROGspire3							string	Synergy "Fundament: Third Spire of the Rake"
	/call DeclareCoreIniVar ROGsynergyAA					string	Synergy "Blackguard's Synergy"


	:exit	
/return
	
 
 
 |***
 *	DES: Circle of [Power|Life|Mana]
 *	USE: keep Circle of [Power|Life|Mana] going
 *	INI: TOON - [General] -> Circleof[Power|Life|Mana]=
 *	NOTE: 
 ***|  
sub synergy_click_Circle()
	DEBUG ${cbug}synergy_click_Circle()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit	
	
	| make sure we have an open aura slot
	| /if ((${Select[${Me.Class.ShortName},RNG,WAR,WIZ,SHM,SHD,ROG,BST,NEC,BER,DRU,MAG,PAL,BRD,MON]} && ${Bool[${Me.Aura[1]}]}) || (${Select[${Me.Class.ShortName},CLR,ENC]} && ${Bool[${Me.Aura[1]}]} && ${Bool[${Me.Aura[2]}]})) /goto :exit

	| circle of power
	/if (${Me.ItemReady[${CircleofPower}]} && ${Spell[${FindItem[${CircleofPower}].Spell}].Stacks}) {
		/if (${Bool[${Me.Song[Circle of Power]}]} || ${Bool[${Me.Song[${RNGauspice}]}]}) /goto :exit
		/call core_cast "${CircleofPower}" item 0
		/goto :exit
	}

	| circle of life
	/if (${Me.ItemReady[${CircleofLife}]} && ${Spell[${FindItem[${CircleofLife}].Spell}].Stacks}) {
		/if (${Bool[${Me.Song[Circle of Life]}]}) /goto :exit
		/call core_cast "${CircleofLife}" item 0
		/goto :exit
	}	
	
	| circle of Mana
	/if (${Me.ItemReady[${CircleofMana}]} && ${Spell[${FindItem[${CircleofMana}].Spell}].Stacks}) {
		/if (${Bool[${Me.Song[Circle of Mana]}]}) /goto :exit
		/call core_cast "${CircleofMana}" item 0
		/goto :exit
	}	
		
 	:exit
/return

 
  
|***
 *	DES: Clicks usefull 1 off items during a Burn
 *	USE: 
 *	INI: set [doclickItemToast] to 0 to disable
 *  NOTE: 
 ***|
sub click_item_toast()
	DEBUG ${cbug}click_item_toast()\ax
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	/declare _count int local
	/for _count 1 to ${doclickItemToast}
		/doevents
		ISDEAD
		/call is_target_dead
		/if (!${Macro.Return}) /goto :exit
		/if (${Me.ItemReady[${clickItemToast${_count}}]}) /call core_cast "${clickItemToast${_count}}" item 0
	/next _count
	:exit
/return  
  

 
 |***
 *	DES: Cleric toast routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_CLR()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit


	/call use_glyph
	/call synergy_click_Circle
	/if (${doclickItemToast}) /call click_item_toast
 	:exit	
/return
	
 
 
 |***
 *	DES: Druid toast routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_DRU()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	
	| swarm pets
	/if (${Me.AltAbilityReady[Natures Guardian]}) /call core_cast "Natures Guardian" alt ${Target2Kill}
	| /if (${Me.AltAbilityReady[Spirits of Nature]}) /call core_cast "Spirits of Nature" alt ${Target2Kill}
	
	| defense
	/if (${Me.AltAbilityReady[Protection of Direwood]}) /call core_cast "Protection of Direwood" alt 0
	/if (${Me.AltAbilityReady[Silent Casting]}) /call core_cast "Silent Casting" alt 0

	| damage boost
	/if (${Me.AltAbilityReady[Distructive Vortex]}) /call core_cast "Distructive Vortex" alt 0
	/if (${Me.AltAbilityReady[Natures Fury]}) /call core_cast "Natures Fury" alt 0
	/if (${Me.AltAbilityReady[Group Spirit of the Black Wolf]}) /call core_cast "Group Spirit of the Black Wolf" alt 0
	/if (${Me.AltAbilityReady[Seasons Wrath]}) /call core_cast "Seasons Wrath" alt ${Target2Kill}
	
	| heal boost
	/if (${Me.AltAbilityReady[Nature's Blessing]}) /call core_cast "Nature's Blessing" alt 0

	| spire
	/if (${Group.Members} && ${Me.AltAbilityReady[${DRUspire3}]}) {
			/call core_cast "${DRUspire3}" alt 0
	} else /if (!${Group.Members} && ${Me.AltAbilityReady[${DRUspire1}]}) {
		/call core_cast "${DRUspire1}" alt 0
	}
	
	| AA AE heal
	/if (${useGroupHeal} && ${Me.AltAbilityReady[Nature's Boon]}) /call core_cast "Nature's Boon" alt 0

	/call synergy_click_Circle
	/if (${doclickItemToast}) /call click_item_toast
	:exit	
/return
	
 
 
 |***
 *	DES: Shaman toast routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_SHM()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	

	/call use_glyph
	/call synergy_click_Circle
	/if (${doclickItemToast}) /call click_item_toast
	:exit	
/return
	
 
 
 |***
 *	DES: Warrior toast routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_WAR()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	
	
	/if (${Me.CombatAbilityReady[${DiscAttention}]} && ${Me.CurrentEndurance} > ${Spell[${DiscAttention}].EnduranceCost}) /call core_cast "${DiscAttention}" disc ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscPhantom}]} && ${Me.CurrentEndurance} > ${Spell[${DiscPhantom}].EnduranceCost} && ${Me.TargetOfTarget.Name.NotEqual[${Me.DisplayName}]}) /call core_cast "${DiscPhantom}" disc ${Target2Kill}

	/if (${Me.AltAbilityReady[${AAResplendentGlory}]} && !${Me.Song[${AAHoldtheLine}].ID}) {
		/call core_cast "${AAResplendentGlory}" alt 0
	} else /if (${Me.AltAbilityReady[${AAHoldtheLine}]} && !${Me.Song[${AAResplendentGlory}].ID}) {
		/call core_cast "${AAHoldtheLine}" alt 0
	}
	/if (${Me.AltAbilityReady[${AAWarlordsTenacity}]}) /call core_cast "${AAWarlordsTenacity}" alt 0
	/if (${Me.AltAbilityReady[${AAWarlordsBravery}]}) /call core_cast "${AAWarlordsBravery}" alt 0
	
	/if (${Me.CombatAbilityReady[${DiscFlash}]} && ${Me.CurrentEndurance} > ${Spell[${DiscFlash}].EnduranceCost}) /call core_cast "${DiscFlash}" disc 0


	/call use_glyph
	/call synergy_click_Circle
	/if (${doclickItemToast}) /call click_item_toast
	:exit	
/return
	
 
 
 |***
 *	DES: Shadowknight toast routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_SHD()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit

	| tap disc
	/if (${Me.PctHPs} < 50*.${tmpHealOverride} && ${Me.CombatAbilityReady[${DiscRancor}]}) /call core_cast "${DiscRancor}" disc ${Target2Kill}

	| defensive	
	/if (${Me.CombatAbilityReady[${DefensiveDisc}]} && ${Me.CurrentEndurance} > ${Spell[${DefensiveDisc}].EnduranceCost}) {
		/call core_cast "${DefensiveDisc}" disc 0
	} else /if (!${Me.CombatAbilityReady[${DefensiveDisc}]} && ${Me.CurrentEndurance} > ${Spell[${DefensiveDisc2}].EnduranceCost} && !${Bool[${Me.ActiveDisc}]}) {
		/call core_cast "${DefensiveDisc2}" disc 0
	} else /if (!${Me.CombatAbilityReady[${DefensiveDisc2}]} && ${Me.CurrentEndurance} > ${Spell[${DefensiveDisc3}].EnduranceCost} && !${Bool[${Me.ActiveDisc}]}) {
		/call core_cast "${DefensiveDisc3}" disc 0
	}
	
	/if (${Me.AltAbilityReady[T`Vyl's Resolve]} && ${Target.Distance} <= ${Spell[T`Vyl's Resolve].MyRange}) /call core_cast "T`Vyl's Resolve" alt ${Target2Kill}
	/if (${Me.CombatAbilityReady[${CrimsonBlade}]} && ${Me.CurrentEndurance} > ${Spell[${CrimesonBlade}].EnduranceCost}) /call core_cast "${CrimsonBlade}" disc 0
	/if (${Me.AltAbilityReady[${SHDspire3}]}) /call core_cast "${SHDspire3}" alt 0
	/if (${Me.AltAbilityReady[Veil of Darkness]}) /call core_cast "Veil of Darkness" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[Harm Touch]}) /call core_cast "Harm Touch" alt ${Target2Kill}
	/if (${Me.AltAbilityReady[Scourge Skin]}) /call core_cast "Scourge Skin" alt 0

	| AA Visage of Decay/Death stacking issues
	/if (${Me.AltAbilityReady[Visage of Death]} && !${Me.Buff[Visage of Decay].ID}) {
		/call core_cast "Visage of Death" alt 0
	} else /if (${Me.AltAbilityReady[Visage of Decay]} && !${Me.AltAbilityReady[Visage of Death]} && !${Me.Buff[Visage of Death].ID}) {
		/call core_cast "Visage of Decay" alt 0
	}

	| swarm pets
	/if (${usePetSwarm} && ${Me.AltAbilityReady[Chattering Bones]}) /call core_cast "Chattering Bones" alt ${Target2Kill}
	
	| epic 2.0
	/if (${Me.ItemReady[${Epic}]} && ${Spell[${FindItem[${Epic}].Spell}].Stacks} && !${Me.Buff[${FindItem[${Epic}].Spell}].ID} && !${Me.Song[${FindItem[${Epic}].Spell}].ID}) /call core_cast "${Epic}" item 0


	/call use_glyph	
	/call synergy_click_Circle
	/if (${doclickItemToast}) /call click_item_toast
	:exit	
/return


 
 |***
 *	DES: Paladin toast routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_PAL()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit

	| defensive	
	/if (${Me.CombatAbilityReady[${DiscArmor}]} && ${Me.CurrentEndurance} > ${Spell[${DiscArmor}].EnduranceCost}) {
		/call core_cast "${DiscArmor}" disc 0
	} else /if (!${Me.CombatAbilityReady[${DiscGuard}]} && ${Me.CurrentEndurance} > ${Spell[${DiscGuard}].EnduranceCost} && !${Bool[${Me.ActiveDisc}]}) {
		/call core_cast "${DiscGuard}" disc 0
	} 

	/if (${Me.AltAbilityReady[${AAJudgement}]}) /call core_cast "${AAJudgement}" alt 0
	/if (${Me.AltAbilityReady[${AAValorousRage}]}) /call core_cast "${AAValorousRage}" alt 0
	/if (${Me.AltAbilityReady[${AAShield}]}) /call core_cast "${AAShield}" alt 0
	
	| epic 2.0
	/if (${Me.ItemReady[${Epic}]} && ${Spell[${FindItem[${Epic}].Spell}].Stacks} && !${Me.Buff[${FindItem[${Epic}].Spell}].ID} && !${Me.Song[${FindItem[${Epic}].Spell}].ID}) /call core_cast "${Epic}" item 0
	
	| dpsheal
	/if (${Me.CombatAbilityReady[${DiscReflexive}]}) /call core_cast "${DiscReflexive}" disc ${Target2Kill}
	
	/call use_glyph
	/call synergy_click_Circle
	/if (${doclickItemToast}) /call click_item_toast
	:exit	
/return
	

 
 |***
 *	DES: Magician toast routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_MAG()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit

	/if (${Me.AltAbilityReady[Elemental Ward]}) /call core_cast "Elemental Ward" alt 0
	
	| twincast
	/if (${Me.AltAbilityReady[Improved Twincast]} && !${Me.Buff[Twincast].ID}) {
		/call core_cast "Improved Twincast" alt 0
	} else /if (${Me.SpellReady[Twincast]} && !${Me.Buff[Improved Twincast].ID}) {
		/call core_cast "Twincast" 0 0
	}
	
	/if (${Me.AltAbilityReady[Focus of Arcanum]} && !${Bool[${Me.Buff[Focus of Arcanum]}]} && ${Target.PctHPs} > 20) /call core_cast "Focus of Arcanum" alt 0
	
	| direct damage bonus
	/if (${Target.PctHPs} > 10) {
		/if (${Me.AltAbilityReady[Heart of Flames]} && !${Bool[${Me.Buff[Elemental Union]}]}) {
			/call core_cast "Heart of Flames" alt 0
		} else /if (${Me.AltAbilityReady[Elemental Union]} && !${Bool[${Me.Buff[Heart of Flames]}]} && !${Me.AltAbilityReady[Heart of Flames]}) {
			/call core_cast "Elemental Union" alt 0
		} else /if (!${Me.AltAbilityReady[Heart of Flames]} && !${Bool[${Me.Buff[Heart of Flames]}]} && !${Me.AltAbilityReady[Elemental Union]} && !${Bool[${Me.Buff[Elemental Union]}]}) {
			/call use_glyph
		}
	}

	/if (${Me.AltAbilityReady[${MAGspire1}]}) /call core_cast "${MAGspire1}" alt 0
	/if (${Me.AltAbilityReady[${SilentCast}]}) /call core_cast "${SilentCast}" alt 0
	/if (${Me.AltAbilityReady[Heart of Stone]}) /call core_cast "Heart of Stone" alt 0

	| pet AAs
	/if (${usePet}) {
		/if (${Me.AltAbilityReady[Servant of Ro]} && ${Spawn[${Target2Kill}].Distance} <= ${Spell[Servant of Ro].MyRange}) /call core_cast "Servant of Ro" alt ${Target2Kill} 
		/if (${Me.AltAbilityReady[Frenzied Burnout]}) /call core_cast "Frenzied Burnout" alt ${Me.Pet.ID}
	}
	/if (${usePetSwarm}) /if (${Me.AltAbilityReady[Host of the Elements]} && ${Spawn[${Target2Kill}].Distance} <= ${Spell[Host of the Elements].MyRange}) /call core_cast "Host of the Elements" alt ${Target2Kill}

	/call synergy_click_Circle
	/if (${doclickItemToast}) /call click_item_toast
	:exit	
/return
	
 
 
 |***
 *	DES: Enchanter toast routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_ENC()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	
	/if (${Me.AltAbilityReady[${ENCIoG}]}) /call core_cast "${ENCIoG}" alt 0
	/if (${Me.AltAbilityReady[${ENCspire3}]} && ${Spell[${ENCspire3}].Stacks}) /call core_cast "${ENCspire3}" alt 0
	/if (${Me.AltAbilityReady[Dimensional Shield]} && ${Spell[Dimensional Shield].Stacks}) /call core_cast "Dimensional Shield" alt 0
	/if (${Me.AltAbilityReady[Calculated Insanity]} && ${Spell[Calculated Insanity].Stacks}) /call core_cast "Calculated Insanity" alt 0
	
	/call use_glyph
	/call synergy_click_Circle
	/if (${doclickItemToast}) /call click_item_toast
	:exit	
/return
	

 
 |***
 *	DES: Wizard toast routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_WIZ()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	
	| shhhhh..
	/if (${Me.AltAbilityReady[Silent Casting]}) /call core_cast "Silent Casting" alt 0

	| next 45 spells +51% crit rate
	/if (${Me.AltAbilityReady[Frenzied Devastation]}) /call core_cast "Frenzied Devastation" alt 0
	| increase chance to land
	/if (${Me.AltAbilityReady[Focus of Arcanum]}) /call core_cast "Focus of Arcanum" alt 0


	| alternate twincast
	/if (${Me.AltAbilityReady[Improved Twincast]} && !${Me.Buff[${TwincastSpell}].ID}) {
		/call core_cast "Improved Twincast" alt ${Me.ID}
	} else /if (${Me.SpellReady[${TwincastSpell}]} && !${Me.Buff[Improved Twincast].ID}) {
		/call core_cast "${TwincastSpell}" 0 ${Me.ID}
	}
	
	/if (${Me.AltAbilityReady[${WIZspire2}]}) /call core_cast "${WIZspire2}" alt 0

	| boost spell type damage
	/if (${Me.AltAbilityReady[Fury of Kerafyrm]} && !${Bool[${Me.Buff[Fury of Ro]}]}) /call core_cast "Fury of Kerafyrm" alt 0
	/if (${Me.AltAbilityReady[Fury of Ro]} && !${Bool[${Me.Buff[Fury of Kerafyrm]}]}) /call core_cast "Fury of Ro" alt 0

	/if (${Me.AltAbilityReady[Arcane Destruction]}) /call core_cast "Arcane Destruction" alt 0
	
	| increase damage
	/if (${Me.AltAbilityReady[Arcane Fury]}) /call core_cast "Arcane Fury" alt 0
	
	| aww baby
	/if (${Me.AltAbilityReady[Mana Burn]}) /call core_cast "Mana Burn" alt 0

	| swarm pet
	/if (${usePetSwarm} && ${Me.AltAbilityReady[Call of Xuzl]}) /call core_cast "Call of Xuzl" alt ${Target2Kill}    

	| ditch agrgro on named
	/if (${Me.PctAggro} > 80 && ${NameList.Find[${Target.DisplayName}]} && ${Me.AltAbilityReady[Mind Crash]}) /call core_cast "Mind Crash" alt ${Target2Kill}
	
	| epic 2.0
	/if (${Me.ItemReady[${Epic}]} && ${Spell[${FindItem[${Epic}].Spell}].Stacks} && !${Me.Buff[${FindItem[${Epic}].Spell}].ID} && !${Me.Song[${FindItem[${Epic}].Spell}].ID}) /call core_cast "${Epic}" item 0


	/call use_glyph	
	/call synergy_click_Circle
	/if (${doclickItemToast}) /call click_item_toast
	:exit	
/return
	
 
 
 |***
 *	DES: Necromancer toast routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_NEC()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	

	/call use_glyph
	/call synergy_click_Circle
	/if (${doclickItemToast}) /call click_item_toast
	:exit	
/return
 
  
 
 |***
 *	DES: Ranger toast routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_RNG()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	
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
	
	| proc disc / spire
	/if (${Raid.Members} && ${Me.AltAbilityReady[${RNGspire3}]} && !${Bool[${Me.Buff[${RNGspire3}]}]}) {
			/call core_cast "${RNGspire3}" alt 0
	} else /if (!${Raid.Members} && ${Me.AltAbilityReady[${RNGspire1}]} && !${Me.AltAbilityReady[Imbued Ferocity]} && !${Bool[${Me.Buff[Imbued Ferocity]}]}) {
		/call core_cast "${RNGspire1}" alt 0
	}
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
	/if (${usePetSwarm} && ${Me.AltAbilityReady[Pack Hunt]}) /call core_cast "Pack Hunt" alt ${Target2Kill}    
	
	| ae regen 
	/if (!${Bool[${DiscHeal}]} && !${Bool[${Me.Song[Bladespur Reflection]}]} && ${Me.CombatAbilityReady[${DiscHeal}]}) /call core_cast "${DiscHeal}" disc ${Target2Kill}
	
		
	/call synergy_click_Circle
	/if (${doclickItemToast}) /call click_item_toast
 	:exit
 /return
 	
 
 
 |***
 *	DES: Bard toast routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_BRD()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	
 	/if (${Me.AltAbilityReady[Bladed Song]}) /call core_cast "Bladed Song" alt 0
 	/if (${Me.CombatAbilityReady[Thousand Blades]}) /call core_cast "Thousand Blades" disc 0	
	/if (${Me.AltAbilityReady[Fierce Eye]}) /call core_cast "Fierce Eye" alt 0
	/if (${Me.AltAbilityReady[${BRDspire3}]}) /call core_cast "${BRDspire3}" alt 0
	/if (${Me.AltAbilityReady[Funeral Dirge]}) /call core_cast "Funeral Dirge" alt 0
	/if (${Me.AltAbilityReady[Quick Time]} && !${Me.Song[Ruaabri's Fury].ID}) /call core_cast "Quick Time" alt 0

	/call use_glyph
	/call synergy_click_Circle
	/if (${doclickItemToast}) /call click_item_toast	
 	:exit	
/return
	
 
 
 |***
 *	DES: Monk toast routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_MNK()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	
	
	| AA Zan Fi's Whistle - DPS increase
	| aa Focused Destructive Force
	| AA Silent Strikes
	| aa five point palm
	
	

	/call use_glyph
	/call synergy_click_Circle
	/if (${doclickItemToast}) /call click_item_toast
	:exit	
/return
	
 
 
 |***
 *	DES: Beast toast routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_BST()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	| /if (${Me.AltAbilityReady[Bloodlust]}) /call core_cast "Bloodlust" alt 0
	/if (${Me.AltAbilityReady[Bestial Alignment]}) /call core_cast "Bestial Alignment" alt 0
	/if (${Me.AltAbilityReady[Frenzied Swipes]}) /call core_cast "Frenzied Swipes" alt 0
	/if (${Me.Pet.ID} && ${Me.AltAbilityReady[Bestial BloodRage]}) /call core_cast "Bestial BloodRage" alt 0
	/call use_glyph
	/if (${usePet}) /call core_cast "Attack of the Warders" alt ${Target2Kill}
	/if (${Me.CombatAbilityReady[${DiscRuaabri}]} && !${Me.Song[Frenzy of Spirit].ID} && !${Me.Buff[Dichotomic Fury].ID}) {
		/call core_cast "${DiscRuaabri}" disc 0
	} else {
		/if (${Me.Song[${DiscRuaabri}].ID}) /goto :exit
		/if (${Me.AltAbilityReady[Frenzy of Spirit]}) /call core_cast "Frenzy of Spirit" alt 0
		/if (${Me.AltAbilityReady[BloodLust]}) /call core_cast "BloodLust" alt 0
		/if (${Me.AltAbilityReady[${BSTspire2}]}) /call core_cast "${BSTspire2}" alt 0
		/if (!${Me.Song[Bestial Alignment].ID} && ${Me.CombatAbilityReady[${DiscSavage}]}) /call core_cast "${DiscSavage}" disc 0
		/if (!${Me.Song[${DiscSavage}].ID} && ${Me.AltAbilityReady[Ferociousness]}) /call core_cast "Ferociousness" alt 0
		/if (${Me.AltAbilityReady[Group Bestial Alignment]} && !${Me.Buff[Ferociousness].ID}) /call core_cast "Group Bestial Alignment" alt 0
	}

	/call use_glyph
	/call synergy_click_Circle
	/if (${doclickItemToast}) /call click_item_toast	
	:exit	
/return
	
 
 
 |***
 *	DES: Berzerker toast routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_BER()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit
	
	/if (${Me.AltAbilityReady[desperation]} && ${Me.PctHPs} < 90) /call core_cast "Desperation" alt 0
	/if (${Me.CombatAbilityReady[${DiscScream}]} && ${Me.CurrentEndurance} > ${Spell[${DiscScream}].EnduranceCost}) /disc ${DiscScream}
	/if (${Me.CombatAbilityReady[${CryOfChaos}]} && !${Me.Song[${CryOfChaos}].ID}) {
		/delay ${Math.Rand[8]}s ${Me.Song[${CryOfChaos}].ID}
		/if (!${Me.Song[${CryOfChaos}].ID}) {
			/if (${Me.AltAbilityReady[Cry of Battle]} && ${Raid.Members}) /call core_cast "Cry of Battle" alt 0
			/if (${Me.CurrentEndurance} > ${Spell[${CryOfChaos}].EnduranceCost}) /disc ${CryOfChaos}
		}
	} 
	/if (${Me.AltAbilityReady[Savage Spirit]} && ${Window[CombatAbilityWnd].Child[CAW_CombatEffectLabel].Text.Equal["No Effect"]}) {
		/if (${Me.CombatAbilityReady[${DiscOpenWound}]} && ${Me.PctHPs} > 90 && !${Me.Song[${OpenWoundEffect}].ID} && ${Me.CurrentEndurance} > ${Spell[${DiscOpenWound}].EnduranceCost})  /call core_cast "${DiscOpenWound}" disc 0
		/if (${Me.CurrentEndurance} > ${Spell[${DiscFrenzyResolve}].EnduranceCost}) /call core_cast "${DiscFrenzyResolve}" disc 0
		/delay 3
		/call core_cast "Savage Spirit" alt 0
		/if (${Me.AltAbilityReady[Silent Strikes]}) /call core_cast "Silent Strikes" alt 0
		/if (${Me.AltAbilityReady[${UntamedgRage}]}) /call core_cast "${UntamedgRage}" alt 0
		/if (${Me.AltAbilityReady[${RecklessAbandon}]}) /call core_cast "${RecklessAbandon}" alt 0
		/if (${Me.AltAbilityReady[${BERspire2}]}) /call core_cast "${BERspire2}" alt 0
		/if (${Me.AltAbilityReady[Blood Pact]}) /call core_cast "Blood Pact" alt 0
	} else /if (${Window[CombatAbilityWnd].Child[CAW_CombatEffectLabel].Text.Equal["No Effect"]} && ${Me.CombatAbilityReady[${DiscBerserking}]} && ${Me.CurrentEndurance} > ${Spell[${DiscBerserking}].EnduranceCost}) {
		/if (${Me.AltAbilityReady[Blinding Fury]} && !${Me.Song[Strike of Savagery].ID}) /call core_cast "Blinding Fury" alt 0
		/if (${Me.Buff[Blinded by Fury].ID}) /call core_cast "${DiscBerserking}" disc 0
		/if (${Me.AltAbilityReady[Juggernaut Surge]}) /call core_cast "Juggernaut Surge" alt 0
		/if (!${Bool[${Me.ActiveDisc[${DiscMele}]}]} || !${Bool[${Me.Song[Empowered Blades]}]}) /call use_glyph
		/if (${Me.AltAbilityReady[Intensity of the Resolute]} && !${Me.Song[Reckless Abandon].ID} && ${Raid.Members}) /call core_cast "Intensity of the Resolute" alt 0
	} else /if (${Me.CombatAbilityReady[${DiscCleavingAnger}]} && ${Window[CombatAbilityWnd].Child[CAW_CombatEffectLabel].Text.Equal["No Effect"]} && ${Me.CurrentEndurance} > ${Spell[${DiscCleavingAnger}].EnduranceCost}) {
		/disc ${DiscCleavingAnger}
		/if (${Me.AltAbilityReady[Vehement Rage]} && !${Me.Song[Reckless Abandon].ID}) /call core_cast "Vehement Rage" alt 0
	} else /if (${Window[CombatAbilityWnd].Child[CAW_CombatEffectLabel].Text.Equal["No Effect"]} && ${Me.CombatAbilityReady[${DiscVengeful}]} && ${Me.CurrentEndurance} > ${Spell[${DiscVengeful}].EnduranceCost}) {
		/disc ${DiscVengeful}
		/if (${Me.AltAbilityReady[Blood Pact]}) /call core_cast "Blood Pact" alt 0
	}	
	
	
	/call synergy_click_Circle
	/if (${doclickItemToast}) /call click_item_toast	
	:exit	
/return
	
 
 
 |***
 *	DES: Rogue toast routines
 *	USE: establish all comp/trigger skills
 *	INI: 
 *	NOTE: 
 ***| 
sub synergy_ROG()
	DEBUG ${cbug}synergy_${Me.Class.ShortName}()\ax
	ISDEAD
	/doevents
	/call check_exit
	/if (${Macro.Return}) /goto :exit

	/if (${Me.ActiveDisc.Name.Equal[${DiscPlay}]} && ${Me.CombatAbilityReady[${DiscFrenzy}]}) {
		:loopmakestop
		/delay 5
		/if (${Me.ActiveDisc.Name.Equal[${DiscPlay}]} && ${Me.CombatAbilityReady[${DiscFrenzy}]}) /stopdisc
		/delay 5
		/if (${Me.ActiveDisc.Name.Equal[${DiscPlay}]} && ${Me.CombatAbilityReady[${DiscFrenzy}]}) /goto :loopmakestop
	}
		
	/if (${Me.CombatAbilityReady[Dichotomic Weapons]}) /call core_cast "Dichotomic Weapons" disc 0
	/call use_skill_melee
	/if (${Me.AltAbilityReady[Focused Rake's Rampage]}) /call core_cast "Focused Rake's Rampage" alt 0
	/if (${Me.CombatAbilityReady[${DiscPinPoint}]}) /call core_cast "${DiscPinPoint}" disc ${Target2Kill}
	/if (${Me.AltAbilityReady[Envenomed Blades]}) /call core_cast "Envenomed Blades" alt 0
	/if (${Bool[${Target2Kill}]} && ${Me.CombatAbilityReady[${DiscAggro}]} && ${Bool[${DiscAggro}]}) /call core_cast "${DiscAggro}" disc ${Target2Kill}
	/if (${Me.AltAbilityReady[Rogue's Fury]}) /call core_cast "Rogue's Fury" alt 0
	/if (${Me.CombatAbilityReady[${DiscMark}]} && ${Me.PctEndurance} > 15) /call core_cast "${DiscMark}" disc ${Target2Kill}
	/if (${Me.AltAbilityReady[${ROGspire3}]}) /call core_cast "${ROGspire3}" alt 0
	| /call use_glyph
	/if (${Me.AltAbilityReady[Intensity of the Resolute]} && ${Raid.Members}) /call core_cast "Intensity of the Resolute" alt 0
  /if (${Me.CombatAbilityReady[${DiscFrenzy}]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscFrenzy}" disc 0
  /if (${Me.CombatAbilityReady[${DiscTwistedChance}]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscTwistedChance}" disc 0
  /if (${Me.CombatAbilityReady[${DiscEradicator}]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscEradicator}" disc 0
  /if (${Me.CombatAbilityReady[${DiscRazor}]} && !${Bool[${Me.ActiveDisc}]}) /call core_cast "${DiscRazor}" disc 0

	/call synergy_click_Circle
	/if (${doclickItemToast}) /call click_item_toast
	:exit	
/return

