|***
 *	test.inc
 *	for testing purposes
 *
 *
 ***|
 
|**
sub test_function()
/return




| scribe.mac - Sym 9.4.2012
| Updated 10/04/2018 ~ Chatwiththisname
|	Replaced all instances of /e with /echo because /e is short for /easyfind now and was causing MQ2 window to spam easyfind help. 
| Updated 01/14/2018 ~ Chatwiththisname
|	Corrected multiple /next var in /for loops. 
|	Open all bags before entering for loop
|	Added 5ms delay after opening bag in for loop to allow it to populate. 
| Updated 12.22.2013
| Buys all available spells and tomes for specified level range (default is level 1 to current level) if a merchant window is open
| It will not buy lower rank spells than you already have. If you have a rk. ii spell it will not buy a rk. i of the same name, but would buy a rk. iii if it was available.
| when finished buying, or if no window open it scribes every available spell/tome found in inventory (regardless of level range given)
|
| It will now loop and keep buying/scribing if you run out of inventory space before you run out of spells to buy.
|
|
| Usage: /mac scribe minlevel maxlevel
|
| /mac scribe 10 20
| ^^ This will buy all spells/tomes from levels 10 to 20 from the current merchant
|
| /mac scribe
| ^^ This will buy all spells/tomes from level 1 to your current level from the current merchant
|
| /mac scribe 30
| ^^ This will buy all spells/tomes from level 30 to your current level from the current merchant
|

	#event FullInventory "#*#Your inventory appears full!#*#"
	#event NotGold "#*#you do not have at least a gold membership#*#"
	#event WrongDiety "#*#You do not worship#*#"


| --------------------------------------------------------------------------------------------
| SUB: Main
| --------------------------------------------------------------------------------------------
Sub Main
    /squelch /echo This line is required so this macro can be called by another macro (e.g. GoTrain.mac).
	/declare HasVoA int outer 0
	/declare InvScribeSlot int outer 1

	/declare MyDeity string outer ${Me.Deity}
	/declare MaxInvSlots int outer 10
	/declare MinLevel int outer 1
	/declare MaxLevel int outer ${Me.Level}
	/declare DoLoop int outer 0
	/declare mName string outer
	/if (${Param0}) /varset MinLevel ${Param0}
	/if (${Param1}) /varset MaxLevel ${Param1}
	/if (${Me.HaveExpansion[Veil of Alaris]}) /varset HasVoA 1

:Start
	/varset DoLoop 0
	/if (${Merchant.Open}) {
		/varset mName ${Merchant.Name}
		/echo Buying all ${Me.Class} spells/tomes for levels ${MinLevel} to ${MaxLevel}
		/if (!${Select[${Me.Class.ShortName},CLR,DRU,SHM,ENC,WIZ,MAG,NEC]}) /call BuyTomes
		/if (!${Select[${Me.Class.ShortName},WAR,BER,MNK,ROG]}) /call BuySpells
		/delay 2s
	}
	/if (${Merchant.Open}) {
		/notify MerchantWnd MW_Done_Button leftmouseup
		/delay 1s !${Merchant.Open}
	}
	/if (!${Select[${Me.Class.ShortName},CLR,DRU,SHM,ENC,WIZ,MAG,NEC]}) /call ScribeTomes
	/if (!${Select[${Me.Class.ShortName},WAR,BER,MNK,ROG]}) /call ScribeSpells
	/if (${DoLoop}) {
		/target ${mName}
		/delay 2s ${Target.ID}
		/click right target
		/delay 1s
		/goto :Start
	}
/return


| --------------------------------------------------------------------------------------------
| SUB: BuySpells
| --------------------------------------------------------------------------------------------
Sub BuySpells
	/if (!${Merchant.Open}) /return
	/delay 2s
	/if (!${Merchant.Items}) /return

	/declare a int local
	/declare b int local
	/declare c int local
	/declare SpellName string local

	/for a 1 to ${Merchant.Items}
		/if (!${Me.FreeInventory}) /return
		/if (!${Merchant.Open}) /return
		/varset c 0
		/varset SpellName ${Merchant.Item[${a}].Spell.Name}
		/if (${SpellName.Find[ Rk. II ]}) /varset SpellName ${Merchant.Item[${a}].Spell.Name.Replace[ Rk. II ,]}
		/if (${SpellName.Find[ Rk. III ]}) /varset SpellName ${Merchant.Item[${a}].Spell.Name.Replace[ Rk. III ,]}

		/if (${Merchant.Item[${a}].Type.Equal[Scroll]} && ${Merchant.Item[${a}].Spell.Level} >= ${MinLevel} && ${Merchant.Item[${a}].Spell.Level} <= ${MaxLevel} && ${Math.Calc[${Merchant.Item[${a}].BuyPrice}\1000]} < ${Me.Platinum} && !${Me.Book[${Merchant.Item[${a}].Spell.Name}]} && !${FindItemCount[${Merchant.Item[${a}]}]}) {
			/if (${Me.CombatAbility[${SpellName}]}) {
				/echo:: I already know ${SpellName}
			/continue
			}

			/if (${FindItemCount[${Merchant.Item[${a}]}]}) {
				/echo:: I already have ${Merchant.Item[${a}]} in inventory
			/continue
			}

			/if (${Me.CombatAbility[${SpellName} Rk. II]} || ${FindItemCount[${Merchant.Item[${a}]} Rk. II]}) {
				/echo:: Skipping lower rank of ${SpellName}
			/continue
			}

			/if (${Me.CombatAbility[${SpellName} Rk. III]} || ${FindItemCount[${Merchant.Item[${a}]} Rk. III]}) {
				/echo:: Skipping lower rank of ${SpellName}
			/continue
			}

			/if (${Merchant.Item[${a}].Deities} && ${MyDeity.NotEqual[${Merchant.Item[${a}].Deity[1]}]}) {
				/echoUnable to use ${SpellName} because of deity
			/continue
			}
			/if (${Me.Book[${SpellName} Rk. II]} || ${FindItemCount[${SpellName} Rk. II]}) {
				/echoSkipping lower rank of ${SpellName}
			/continue
			}
			/if (${Me.Book[${SpellName} Rk. III]} || ${FindItemCount[${SpellName} Rk. III]}) {
				/echoSkipping lower rank of ${SpellName}
			/continue
			}

		| this loop is needed because the merchant item number doesn't match up to the interface line number
			/for b 1 to ${Window[MerchantWnd].Child[MW_ItemList].Items}
				/if (${Window[MerchantWnd].Child[MW_ItemList].List[${b},2].Equal[${Merchant.Item[${a}]}]}) /varset c ${b}
			/next b

		| merchant line number matches what we are looking to buy, buy 1 copy of it
			/if (${c}) {
				/echo Buying ${Window[MerchantWnd].Child[MW_ItemList].List[${c},2]}
				/notify MerchantWnd MW_ItemList listselect ${c}
				/nomodkey /ctrlkey /notify MerchantWnd MW_Buy_Button leftmouseup
				/delay 1s
				/doevents
			}
		}
	/next a

/return


| --------------------------------------------------------------------------------------------
| SUB: ScribeSpells
| --------------------------------------------------------------------------------------------
Sub ScribeSpells
	/declare Bag int local
	/declare Slot int local
	/if (${Cursor.ID}) /autoinv
	| Opening your inventory for access bag slots
	/if (!${Window[InventoryWindow].Open}) {
		/windowstate InventoryWindow open
	}
	| Open all bags 
	/keypress OPEN_INV_BAGS

	
	| Check Top Level Inventory Slot for bag/no bag
	/for Bag 1 to ${MaxInvSlots}
		| If it's a bag do this
		/if (${InvSlot[pack${Bag}].Item.Container}) {
			/if (!${Window[Pack${Bag}].Open}) /itemnotify pack${Bag} rightmouseup
			/delay 1s ${Window[Pack${Bag}].Open}
			/delay 5

			/for Slot 1 to ${InvSlot[pack${Bag}].Item.Container}
				/if (${InvSlot[pack${Bag}].Item.Item[${Slot}].Type.Equal[Scroll]} && ${InvSlot[pack${Bag}].Item.Item[${Slot}].Spell.Level} <= ${Me.Level} && !${Me.Book[${InvSlot[pack${Bag}].Item.Item[${Slot}].Spell.Name}]}) {
					/nomodkey /ctrlkey /itemnotify in pack${Bag} ${Slot} rightmouseup
					/delay 1s ${Cursor.ID}
					/if (${Cursor.ID}) /call doScribe
				}
			/next Slot
			| Close the bag I just checked
			/if (${Window[Pack${Bag}].Open}) /itemnotify pack${Bag} rightmouseup
		} else {
			| If it's not a bag, do this
			| Debug message /echo TopLevel Inv Slot ~ IsAScroll: ${InvSlot[pack${Bag}].Item.Type.Equal[Scroll]} && Item Spell level: ${InvSlot[pack${Bag}].Item.Spell.Level} <= My Level: ${Me.Level} && Scribed Already: !${Me.Book[${InvSlot[pack${Bag}].Item.Spell.Name}]}
			/if (${InvSlot[pack${Bag}].Item.Type.Equal[Scroll]} && ${InvSlot[pack${Bag}].Item.Spell.Level} <= ${Me.Level} && !${Me.Book[${InvSlot[pack${Bag}].Item.Spell.Name}]}) {
				/echo Scribing: ${InvSlot[Pack${Bag}].Item.Name}
				/nomodkey /ctrlkey /itemnotify pack${Bag} rightmouseup
				/delay 1s ${Cursor.ID}
				/if (${Cursor.ID}) /call doScribe
			}
		}
	/next Bag

	/if (${Window[SpellBookWnd].Open}) /squelch /windowstate SpellBookWnd close
/return


| --------------------------------------------------------------------------------------------
| SUB: doScribe
| --------------------------------------------------------------------------------------------
Sub doScribe
	/declare tOut timer local 10s

:WaitFinished
	/if (${Window[ConfirmationDialogBox].Open} && ${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find[${Cursor.Spell.Name} will replace]}) /notify ConfirmationDialogBox Yes_Button leftmouseup
	/if (!${tOut}) /autoinv
	/delay 2
	/doevents
	/if (${Cursor.ID}) /goto :WaitFinished
/return


| --------------------------------------------------------------------------------------------
| SUB: BuyTomes
| --------------------------------------------------------------------------------------------
Sub BuyTomes
	/if (!${Merchant.Open}) /return
	/delay 2s
	/if (!${Merchant.Items}) /return

	/declare a int local
	/declare b int local
	/declare c int local
	/declare SpellName string local

	/for a 1 to ${Merchant.Items}
		/if (!${Me.FreeInventory}) /return
		/varset c 0
		/varset SpellName ${Merchant.Item[${a}].Spell.Name}
		/if (${SpellName.Find[ Rk. II ]}) /varset SpellName ${Merchant.Item[${a}].Spell.Name.Replace[ Rk. II ,]}
		/if (${SpellName.Find[ Rk. III ]}) /varset SpellName ${Merchant.Item[${a}].Spell.Name.Replace[ Rk. III ,]}
	|/echoItem ${a} is ${Merchant.Item[${a}].Spell.Name}

		/if (${Merchant.Item[${a}].Type.Equal[Scroll]} && ${Merchant.Item[${a}].Spell.Level} >= ${MinLevel} && ${Merchant.Item[${a}].Spell.Level} <= ${MaxLevel} && ${Math.Calc[${Merchant.Item[${a}].BuyPrice}\1000]} < ${Me.Platinum}) {

			/if (${Me.CombatAbility[${SpellName}]}) {
				/echo:: I already know ${SpellName}
			/continue
			}

			/if (${FindItemCount[${Merchant.Item[${a}]}]}) {
				/echo:: I already have ${Merchant.Item[${a}]} in inventory
			/continue
			}

			/if (${Me.CombatAbility[${SpellName} Rk. II]} || ${FindItemCount[${Merchant.Item[${a}]} Rk. II]}) {
				/echo:: Skipping lower rank of ${SpellName}
			/continue
			}

			/if (${Me.CombatAbility[${SpellName} Rk. III]} || ${FindItemCount[${Merchant.Item[${a}]} Rk. III]}) {
				/echo:: I already have the highest rank of ${SpellName}
			/continue
			}

		| this loop is needed because the merchant item number doesn't match up to the interface line number
			/for b 1 to ${Window[MerchantWnd].Child[MW_ItemList].Items}
				/if (${Window[MerchantWnd].Child[MW_ItemList].List[${b},2].Equal[${Merchant.Item[${a}]}]}) /varset c ${b}
			/next b

		| merchant line number matches what we are looking to buy, buy 1 copy of it
			/if (${c}) {
				/echo Buying ${Window[MerchantWnd].Child[MW_ItemList].List[${c},2]}
				/notify MerchantWnd MW_ItemList listselect ${c}
				/nomodkey /ctrlkey /notify MerchantWnd MW_Buy_Button leftmouseup
				/delay 1s
				/doevents
			}
		}
	/next a
/return


| --------------------------------------------------------------------------------------------
| SUB: ScribeTomes
| --------------------------------------------------------------------------------------------
Sub ScribeTomes
	/declare Bag int local
	/declare Slot int local

	/if (${Cursor.ID}) /autoinv

	/if (!${HasVoA} && ${InvSlot[pack${InvScribeSlot}].Item.Type.Equal[Scroll]} && ${InvSlot[pack${InvScribeSlot}].Item.Spell.Level} <= ${Me.Level} && !${Me.Book[${InvSlot[pack${InvScribeSlot}].Item.Spell.Name}]}) /nomodkey /ctrlkey /itemnotify ${InvSlot[pack${InvScribeSlot}]} rightmouseup

	/for Bag 1 to ${MaxInvSlots}
		/if (${InvSlot[pack${Bag}].Item.Container}) {
			/if (!${Window[Pack${Bag}].Open}) /itemnotify pack${Bag} rightmouseup
			/delay 1s ${Window[Pack${Bag}].Open}
			/for Slot 1 to ${InvSlot[pack${Bag}].Item.Container}
				/if (${InvSlot[pack${Bag}].Item.Item[${Slot}].Name.Find[Tome]} && ${InvSlot[pack${Bag}].Item.Item[${Slot}].Type.Equal[Scroll]} && ${InvSlot[pack${Bag}].Item.Item[${Slot}].Spell.Level} <= ${Me.Level} && !${Me.Book[${InvSlot[pack${Bag}].Item.Item[${Slot}].Spell.Name}]}) {
					/if (${HasVoA}) {
						/nomodkey /ctrlkey /itemnotify in pack${Bag} ${Slot} rightmouseup
						/delay 1s !${InvSlot[pack${Bag}].Item.Item[${Slot}].ID}
					} else {
						/nomodkey /ctrlkey /itemnotify in pack${Bag} ${Slot} leftmouseup
						/delay 1s ${Cursor.ID}
						/nomodkey /ctrlkey /itemnotify ${InvSlot[pack${InvScribeSlot}]} leftmouseup
						/delay 1s !${Cursor.ID}
						/nomodkey /ctrlkey /itemnotify ${InvSlot[pack${InvScribeSlot}]} rightmouseup
						/delay 1s !${InvSlot[pack${InvScribeSlot}].ID}
					}
				}
			/next Slot

		}
		/if (${Window[Pack${Bag}].Open}) /itemnotify pack${Bag} rightmouseup
	/next Bag
	/if (${Cursor.ID}) /autoinv
/return


| --------------------------------------------------------------------------------------------
| SUB: Event_NotGold
| --------------------------------------------------------------------------------------------
Sub Event_NotGold
	/autoinv
/return


| --------------------------------------------------------------------------------------------
| SUB: Event_WrongDiety
| --------------------------------------------------------------------------------------------
Sub Event_WrongDiety
	/autoinv
/return


| --------------------------------------------------------------------------------------------
| SUB: Event_FullInventory
| --------------------------------------------------------------------------------------------
Sub Event_FullInventory
	/if (${Merchant.Open}) {
		/notify MerchantWnd MW_Done_Button leftmouseup
		/delay 1s !${Merchant.Open}
	}
	/varset DoLoop 1

/return



























| gotrain.mac by Rogue601  v1.6
| 
| Simple macro to send your team to the trainers
| Requires MQ2Nav and Mesh
|    

| TODO - Add stuck logic. Nav gets stuck sometimes. 


| -------------------------------------------------------------------------------------
| Sub Main
| -------------------------------------------------------------------------------------
Sub Main
    /declare thislineisignored
    | COMMENT : Check for POK zone or exit.
    /if (!${Zone.ShortName.Equal[PoKnowledge]}) {
        /echo "This macro only works in the Plane of Knowledge"
        /endmac    
    }

    | ----------------------------------------------------------------------------
    | Customizable Variables
    | ----------------------------------------------------------------------------    
    /declare randomDelayedStart int outer 0
    /declare debugOn bool outer False
    /declare buySpells bool outer TRUE

    | COMMENT : Declare Variables
    /declare myLevel int outer ${Param0}
    /declare myClass string outer ${Param1.Lower}
    /declare myTrainerID int outer 0
    /declare myTrainerName string outer 0
    /declare myTrainerX float outer 0
    /declare myTrainerY float outer 0
    /declare myTrainerZ float outer 0


    | ----------------------------------------------------------------------------
    | Require Parameters - Param1 = Level, Param2 = Class
    | ----------------------------------------------------------------------------    
    /if (${Macro.Params}==0) {
        
        /varset myLevel ${Me.Level}
        /varset myClass ${Me.Class}
        /varset myClass ${myClass.Lower}
        /echo Optional Usage: /mac gotrain level    
        /echo Optional Usage: /mac gotrain level class        
        /echo Example : /mac gotrain 10 cleric
        /echo If no Parameters are given it will use yours
        /echo
        /echo You are a Level: ${myLevel} ${myClass}
        
    } else /if (${Macro.Params}==1) {
        
        /varset myClass ${Me.Class}
        /echo Overriding your level and using ${myLevel} instead!
            
    
    } else /if (${Macro.Params}==2) {
    
        /echo Overriding your level and class and using Level: ${myLevel} Class: ${myClass}
        
    } else {
        
        /echo GOTRAIN ERROR!
        /echo Too many parameters given?
        /echo Example : /mac gotrain 10
        /echo Example : /mac gotrain 10 cleric
        /endmac
        
    }
    
    
    /if (${debugOn}) /echo DEBUG: Main: Starting Mainloop    
    /nav reload
    
    | ----------------------------------------------------------------------------
    | RandonDelayStart - Lets wait here a random time to prevent a swarm
    | ----------------------------------------------------------------------------    
    /if (${randomDelayedStart} > 0) /echo randomStartDelay is active. Waiting ${randomDelayedStart} 
    /delay ${randomDelayedStart}
    
    :mainloop

    
    | ----------------------------------------------------------------------------
    | SetTrainer - If I don't have a trainer set, then find and set it.
    | ----------------------------------------------------------------------------    
    /if (${myTrainerID}==0) {
        /if (${debugOn}) /echo DEBUG: Mainloop: Call SetTrainer
        /call SetTrainer


    | ----------------------------------------------------------------------------
    | navToTrainer - I have a trainer now go there
    | ----------------------------------------------------------------------------    
    } else /if (${Spawn[${myTrainerID}].Distance} > 30) {
        /if (${debugOn}) /echo DEBUG: Mainloop: Call NavToTrainer
        /call NavToTrainer
        
        
    | ----------------------------------------------------------------------------
    | BeginTraining - We have a trainer and we are close to them. What now?
    | ----------------------------------------------------------------------------            
    } else {
        /if (${debugOn}) /echo DEBUG: Mainloop: Call BeginTraining
        /call BeginTraining
    }



    
     
    /delay 10
    /goto :mainloop
    /if (${debugOn}) /echo DEBUG: Main: Ending Mainloop
    
    :mainend
    
/return 


        
| -------------------------------------------------------------------------------------
| Sub SetTrainer - Define myTrainer based on class and level
| -------------------------------------------------------------------------------------
Sub SetTrainer
    
    /if (${myClass.Equal["bard"]}) {
        /if (${myLevel} > 80) {
            /varset myTrainerName "Minstrel Franchot"
        } else /if (${myLevel} > 80) {
            /varset myTrainerName "Minstrel Franchot"
        } else /if (${myLevel} > 70) {
            /varset myTrainerName "Minstrel Cirsaelle"
        } else /if (${myLevel} > 60) {
            /varset myTrainerName "Minstrel Silnon"
        } else /if (${myLevel} > 50) {
            /varset myTrainerName "Minstrel Gwiar"
        } else /if (${myLevel} > 25) {
            /varset myTrainerName "Minstrel Joet"
        } else /if (${myLevel} > 0) {
            /varset myTrainerName "Minstrel Eoweril"
        }
    } else /if (${myClass.Equal["beastlord"]}) {
        /if (${myLevel} > 80) {
            /varset myTrainerName "Primalist Beal"
        } else /if (${myLevel} > 70) {
            /varset myTrainerName "Primalist Uliag"
        } else /if (${myLevel} > 60) {
            /varset myTrainerName "Primalist Loerith"
        } else /if (${myLevel} > 50) {
            /varset myTrainerName "Primalist Nydalith"
        } else /if (${myLevel} > 25) {
            /varset myTrainerName "Primalist Worenon"
        } else /if (${myLevel} > 0) {
            /varset myTrainerName "Primalist Saosith"
        }
    } else /if (${myClass.Equal["berserker"]}) {
        /varset myTrainerName "Kurlond Axebringer"
    } else /if (${myClass.Equal["cleric"]}) {    
        /if (${myLevel} > 80) {
            /varset myTrainerName "Vicar Feyl"
        } else /if (${myLevel} > 70) {
            /varset myTrainerName "Vicar Glaunn"
        } else /if (${myLevel} > 60) {
            /varset myTrainerName "Vicar Diarin"
        } else /if (${myLevel} > 50) {
            /varset myTrainerName "Vicar Delin"
        } else /if (${myLevel} > 25) {
            /varset myTrainerName "Vicar Thiran"
        } else /if (${myLevel} > 0) {
            /varset myTrainerName "Vicar Ceraen"
        }
    } else /if (${myClass.Equal["druid"]}) {
        /if (${myLevel} > 80) {
            /varset myTrainerName "Wanderer Abel"
        } else /if (${myLevel} > 70) {
            /varset myTrainerName "Wanderer Thermon"
        } else /if (${myLevel} > 60) {
            /varset myTrainerName "Wanderer Kedrisan"
        } else /if (${myLevel} > 50) {
            /varset myTrainerName "Wanderer Frardok"
        } else /if (${myLevel} > 25) {
            /varset myTrainerName "Wanderer Qenda"
        } else /if (${myLevel} > 0) {
            /varset myTrainerName "Wanderer Astobin"
        }
    } else /if (${myClass.Equal["enchanter"]}) {
        /if (${myLevel} > 80) {
            /varset myTrainerName "Illusionist Phili"
        } else /if (${myLevel} > 70) {
            /varset myTrainerName "Illusionist Warek"
        } else /if (${myLevel} > 60) {
            /varset myTrainerName "Illusionist Acored"
        } else /if (${myLevel} > 50) {
            /varset myTrainerName "Illusionist Lobaen"
        } else /if (${myLevel} > 25) {
            /varset myTrainerName "Illusionist Sevat"
        } else /if (${myLevel} > 0) {
            /varset myTrainerName "Illusionist Jerup"
        }
    } else /if (${myClass.Equal["magician"]}) {
        /if (${myLevel} > 80) {
            /varset myTrainerName "Elementalist Bartbur"
        } else /if (${myLevel} > 70) {
            /varset myTrainerName "Elementalist Onaec"
        } else /if (${myLevel} > 60) {
            /varset myTrainerName "Elementalist Siewth"
        } else /if (${myLevel} > 50) {
            /varset myTrainerName "Elementalist Padan"
        } else /if (${myLevel} > 25) {
            /varset myTrainerName "Elementalist Kaeob"
        } else /if (${myLevel} > 0) {
            /varset myTrainerName "Elementalist Somat"
        }
    } else /if (${myClass.Lower.Equal["monk"]}) {
         /varset myTrainerName "Beorobin Amondson"
    } else /if (${myClass.Equal["necromancer"]}) {
        /if (${myLevel} > 80) {
            /varset myTrainerName "Heretic Lartman"
        } else /if (${myLevel} > 70) {
            /varset myTrainerName "Heretic Niraf"
        } else /if (${myLevel} > 60) {
            /varset myTrainerName "Heretic Ceikon"
        } else /if (${myLevel} > 50) {
            /varset myTrainerName "Heretic Edalith"
        } else /if (${myLevel} > 25) {
            /varset myTrainerName "Heretic Elirev"
        } else /if (${myLevel} > 0) {
            /varset myTrainerName "Heretic Drahur"
        }
    } else /if (${myClass.Equal["warrior"]}) {
         /varset myTrainerName "Heldin Swordbreaker"
    } else /if (${myClass.Equal["paladin"]}) {
        /if (${myLevel} > 80) {
            /varset myTrainerName "Cavalier Tummings"
        } else /if (${myLevel} > 70) {
            /varset myTrainerName "Cavalier Ethigom"
        } else /if (${myLevel} > 60) {
            /varset myTrainerName "Cavalier Cerakor"
        } else /if (${myLevel} > 50) {
            /varset myTrainerName "Cavalier Preradus"
        } else /if (${myLevel} > 25) {
            /varset myTrainerName "Cavalier Aodus"
        } else /if (${myLevel} > 0) {
            /varset myTrainerName "Cavalier Waut"
        }
    } else /if (${myClass.Equal["ranger"]}) {
        /if (${myLevel} > 80) {
            /varset myTrainerName "Pathfinder Sweeneph"
        } else /if (${myLevel} > 70) {
            /varset myTrainerName "Pathfinder Arelat"
        } else /if (${myLevel} > 60) {
            /varset myTrainerName "Pathfinder Naend"
        } else /if (${myLevel} > 50) {
            /varset myTrainerName "Pathfinder Thoajin"
        } else /if (${myLevel} > 25) {
            /varset myTrainerName "Pathfinder Vaered"
        } else /if (${myLevel} > 0) {
            /varset myTrainerName "Pathfinder Viliken"
        }
    } else /if (${myClass.Equal["rogue"]}) {        
         /echo Sorry rogue isn't setup yet : Exiting 
        /endmac
    } else /if (${myClass.Equal["shadow knight"]}) {
        /if (${myLevel} > 80) {
            /varset myTrainerName "Reaver Enold"
        } else /if (${myLevel} > 70) {
            /varset myTrainerName "Reaver Praden"
        } else /if (${myLevel} > 60) {
            /varset myTrainerName "Reaver Muron"
        } else /if (${myLevel} > 50) {
            /varset myTrainerName "Reaver Thirlan"
        } else /if (${myLevel} > 25) {
            /varset myTrainerName "Reaver Uledrith"
        } else /if (${myLevel} > 0) {
            /varset myTrainerName "Reaver Nydlil"
        }
    } else /if (${myClass.Equal["shaman"]}) {
        /if (${myLevel} > 80) {
            /varset myTrainerName "Mystic Voskovek"
        } else /if (${myLevel} > 70) {
            /varset myTrainerName "Mystic Somas"
        } else /if (${myLevel} > 60) {
            /varset myTrainerName "Mystic Pikor"
        } else /if (${myLevel} > 50) {
            /varset myTrainerName "Mystic Ryrin"
        } else /if (${myLevel} > 25) {
            /varset myTrainerName "Mystic Goharkor"
        } else /if (${myLevel} > 0) {
            /varset myTrainerName "Mystic Abomin"
        }
    } else /if (${myClass.Equal["wizard"]}) {
        /if (${myLevel} > 80) {
            /varset myTrainerName "Channeler Larkord"
        } else /if (${myLevel} > 70) {
            /varset myTrainerName "Channeler Keatrem"
        } else /if (${myLevel} > 60) {
            /varset myTrainerName "Channeler Alyrianne"
        } else /if (${myLevel} > 50) {
            /varset myTrainerName "Channeler Cerakoth"
        } else /if (${myLevel} > 25) {
            /varset myTrainerName "Channeler Lariland"
        } else /if (${myLevel} > 0) {
            /varset myTrainerName "Channeler Olaemos"
        }
    } else {
        /echo Unrecognized Class Name: ${myClass} : Exiting 
        /endmac
    }
     
     /varset myTrainerID ${NearestSpawn[npc targetable radius 10000 "${myTrainerName}"].ID}
    /if (${debugOn}) /echo DEBUG: SetTrainer: ${myTrainerName} ID:${myTrainerID}
    /echo ${myLevel} ${myClass} Trainer is ${myTrainerName} ID:${myTrainerID}
    /varset myTrainerX ${Math.Calc[${Spawn[${myTrainerID}].X}+${Math.Rand[5]}]}
    /varset myTrainerY ${Math.Calc[${Spawn[${myTrainerID}].Y}+${Math.Rand[5]}]}
    /varset myTrainerZ ${Spawn[${myTrainerID}].Z}
     
/return

| -------------------------------------------------------------------------------------
| Sub NavToTrainer - Navigate using MQ2Nav 
| -------------------------------------------------------------------------------------
Sub NavToTrainer
    /if (${debugOn}) /echo DEBUG: NavToTrainer: Tracking ${myTrainerName}  ID: ${myTrainerID} Distance: ${Spawn[${myTrainerID}].Distance}
    /setchattitle Heading towards ${myTrainerName}
    /squelch /nav locxyz ${myTrainerX} ${myTrainerY} ${myTrainerZ}
    
/return    


| -------------------------------------------------------------------------------------
| Sub BeginTraining - At the Trainer setup position and open merchant
| -------------------------------------------------------------------------------------
Sub BeginTraining

    /if (${debugOn}) /echo DEBUG: BeginTraining: Start
    /setchattitle MQ2
    /target id ${myTrainerID}
    /if (${Target.ID} && ${Target.Type.Equal[NPC]}) {
        /delay 10
        /stick id ${myTrainerID} 15 moveback 
        /delay 10
        /stick off
        /delay 10
        /nomodkey /click right target
        
        /if (${buySpells}) {
            /if (${debugOn}) /echo DEBUG: BeginTraining: Buying Spells with Scribe.mac
            /delay 30
            /mac scribe
        }
        /endmac
    }
    /if (${debugOn}) /echo DEBUG: BeginTraining: END

    
/return    


| -------------------------------------------------------------------------------------
| OnExit - Things it does when you type /endmac
| -------------------------------------------------------------------------------------
:OnExit
    /setchattitle MQ2
    /squelch /nav stop
    /end











**|























