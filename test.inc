|***
 *	test.inc
 *	for testing purposes
 *
 *
 ***|
 

sub test_function()
/return



|***
 *	DES: 
 *	USE: /port 
 *	INI: 
 *  NOTE:
 ***| 
#bind port_list /port
sub Bind_port_list(string _class, string _location)
	/declare _listout 				string 	local
	/declare _listouttype			string 	local
	/declare _sep 						bool 		local FALSE
	/declare _country 				int 		local 0
	/declare _count 					int 		local 0
	/declare _state 					int 		local 0
	/declare _portclass 			string 	local |druid|wizard
	/declare _porttype	 			string 	local |tl|group
	/declare _continent 			string 	local |alaris|antonica|discord|faydwer|kunark|luclin|odus|serpentsspine|taelosia|planes|other
	/declare _tell						bool		local FALSE

	| druid port list
	/declare _portDruidAlaris 											string local |beast|pillars|shardslanding
	/declare _portDruidAntonica  										string local |commonlands|feerrott|karana|lavastorm|misty|ro|surefall|undershore|westkarna
	/declare _portDruidDiscord 											string local |bloodfields|wallofslaughter
	/declare _portDruidFaydwer 											string local |butcherblock|looping|steamfront
	/declare _portDruidKunark 											string local |dreadlands|lceanium
	/declare _portDruidLuclin 											string local |dawnshroud|grimling|nexus|twilight
	/declare _portDruidOdus 												string local |buriedsea|stonebrunt|tempesttemple|tox
	/declare _portDruidSerpentsspine							 	string local |blightfire|direwind|steppes
	/declare _portDruidTaelosia 										string local |barindu|natimbi
	/declare _portDruidPlanes 											string local |grounds|potime|brells|arcstone|knowledge
	/declare _portDruidVelious 											string local |cobaltscar|wakening|greatdivide|iceclad
	/declare _portDruidOther 												string local |primarypush|secondarypush|primary|secondary

	| wizard port list
	/declare _portWizardAlaris 											string local |pillars|sarith 
	/declare _portWizardAntonica 										string local |northkarana|commonlands|nektulos|qeyenos|northro|cazic|undershore|westkarana
	/declare _portWizardDiscord 										string local |wallofslaughter|bloodfields
	/declare _portWizardFaydwer 										string local |fay|dragonscale
	/declare _portWizardKunark 											string local |lceanium|dreadlands|skyfire
	/declare _portWizardLuclin 											string local |dawnshroud|twilight|grimling
	/declare _portWizardOdus 												string local |tempesttemple|katta
	/declare _portWizardSerpentsspine							 	string local |tox|stonebrunt|icefall|sunderock|blightfire
	/declare _portWizardTaelosia 										string local |barindu|natimbi
	/declare _portWizardPlanes 											string local |grounds|potime|brells|arcstone|pok
	/declare _portWizardVelious 										string local |cobaltscar|wakening|greatdivide|iceclad
	/declare _portWizardOther 											string local |bind|nexus|primary|secondary|primarypush|secondarypush
			
	| sort the class
	/if (${Bool[${_class.Find[druid]}]}) {
		/varset _class Druid
	} else /if (${Bool[${_class.Find[wizard]}]}) {
		/varset _class Wizard
	} else {
		| build class types
		/for _count 1 to ${_portclass.Count[|]}
			/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_portclass.Arg[${_count},|]}\ax
			/varset _sep TRUE
		/next _count
		/varset _sep FALSE
		
		| build port types
		/for _count 1 to ${_porttype.Count[|]}
			/varset _listouttype ${_listouttype} ${If[${_sep},${sep},]} ${cinfo}${_porttype.Arg[${_count},|]}\ax
			/varset _sep TRUE
		/next _count

		VOUT /port [${_listout}] [${cinfo}location\ax] [${_listouttype}]
		/return
	}
	
  | echo locations if class correct
	/if (${Bool[${_class}]} && !${Bool[${_location}]}) {
		/for _country 1 to ${_continent.Count[|]}
			/for _state 1 to ${_port${_class}${_continent.Arg[${_country},|].Left[1].Upper}${_continent.Arg[${_country},|].Right[-1].Lower}.Count[|]}
				/varset _listout ${_listout} ${If[${_sep},${sep},]} ${cinfo}${_port${_class}${_continent.Arg[${_country},|].Left[1].Upper}${_continent.Arg[${_country},|].Right[-1].Lower}.Arg[${_state},|]}\ax
				/varset _sep TRUE
			/next _state
			VOUT ${_continent.Arg[${_country},|].Left[1].Upper}${_continent.Arg[${_country},|].Right[-1].Lower} [${_listout}] 
			/varset _listout
			/varset _sep
		/next _country

	| request port when class and location are present
	} else /if (${Bool[${_class}]} && ${Bool[${_location}]}) {
		/for _count 1 to ${Resident${_class}.Count[|]}
			/if (${Spawn[pc ${Resident${_class}.Arg[${_count},|]} zradius 80 radius 100].ID}) {
				/t ${Resident${_class}.Arg[${_count},|]} send ${_location}
				/varset _tell TRUE
			}		
		/next _count
		/if (!${_tell}) /call echos failboat 0 0 0 0
	}

/return







|***
 *	DES: zephyr/TL requesting toon.
 *	USE: /t [toon] send [location]
 *	INI: 
 *  NOTE: 
 ***|
#event port_request "#1# tells you, 'send #2#'"
sub Event_port_request(string line, string _sender, string _location)
	SDEBUG ${cbug}Event_port_request(sender:${_sender}, location:${_location})\ax
	/return
	/if (!${Select[${Me.Class.ShortName},DRU,WIZ]}) /return
	/if (!${Defined[${_location}]}) /return
	CHECKSAFELIST
	/if (${Spawn[pc ${_sender}].Distance} > 100) /return
	
	
	/declare _tmpID int local ${Spawn[pc ${_sender}].ID}
	
	| send 'em on their way with a valid location request
	SDEBUG location: ${_location} - ${${_location}} @ ${_tmpID}
	
	/memspell ${buffNum} "${${_location}}"
	

	/t ${_sender} Please enjoy your flight. ${${_location}} on its way.
	/delay 2s
	
	/call MQ2cast "${${_location}}" ${buffNum} -targetid|${_tmpID}

/return














|***
 *	DES: group port requesting toon.
 *	USE: /t [toon] send [location]
 *	INI: 
 *  NOTE: 

#event sendgroup "#1# tells you, 'sendgroup #2#'"
sub Event_sendgroup(string line, string _sender, string _location)
	SDEBUG sender:${_sender}, location:${_location}
	/if (!${Select[${Me.Class.ShortName},DRU,WIZ]}) /return
	CHECKSAFELIST
	/if (${Spawn[pc ${_sender}].Distance} > 100) /return
	/varset _location ${_location}_group
	
	| send 'em on their way with a valid location request
	/if (${Defined[${_location}]}) {
		DEBUG location: ${_location} - ${${_location}} group:${_group}
		/t ${_sender} Sending group to ${${_location}}.
		/call mem_spell TRUE "${${_location}}" ${buffGem} TRUE
		/call core_cast "${${_location}}" 0 ${Spawn[pc ${_sender}].ID} FALSE
	} else /if (!${Defined[${_location}]}) {
		VOUT ${owarning} ${sep} Bad location.
	}
/return
 ***|




#event portlist "#1# tells you, 'portlist'"
sub Event_portlist(string line, string _sender)
	SDEBUG sender:${_sender}
	/if (!${Select[${Me.Class.ShortName},DRU,WIZ]}) /return
	CHECKSAFELIST
	/declare _portList[13]		string local
	/declare _count 					int local 0


	/if (${Select[${Me.Class.ShortName},DRU]}) {
		/varset _portList[1] Alaris: shardslanding, pillars, beast
		/varset _portList[2] Antonica: westkarna, lavastorm, undershore, misty, ro, feerrott, commonlands, surefall, karana
		/varset _portList[3] Discord: wallofslaughter, bloodfields
		/varset _portList[4] Faydwer: looping, steamfront, butcherblock
		/varset _portList[5] Kunark: lceanium, dreadlands
		/varset _portList[6] Luclin: dawnshroud, twilight, grimling, nexus
		/varset _portList[7] Misc: primarypush, secondarypush, primary, secondary
		/varset _portList[8] Odus: tempesttemple, buriedsea, stonebrunt, tox
		/varset _portList[9] Serpent's Spine: direwind, steppes, blightfire
		/varset _portList[10] Taelosia: barindu, natimbi
		/varset _portList[11] The Planes: grounds, potime, brells, arcstone, knowledge
		/varset _portList[12] Velious: cobaltscar, wakening, greatdivide, iceclad
		/varset _portList[13]
		/varset _portList[14] -- end port list
		
	} else /if (${Select[${Me.Class.ShortName},WIZ]}) {
		/varset _portList[1] Alaris: pillars, sarith 
		/varset _portList[2] Antonica: northkarana, commonlands, nektulos, qeyenos, northro, cazic, undershore, westkarana
		/varset _portList[3] Discord: wallofslaughter, bloodfields
		/varset _portList[4] Faydwer: fay, dragonscale
		/varset _portList[5] Kunark: lceanium, dreadlands, skyfire
		/varset _portList[6] Luclin: dawnshroud, twilight, grimling
		/varset _portList[7] Misc: bind, nexus, primary, secondary, primarypush, secondarypush
		/varset _portList[8] Odus: tempesttemple, katta
		/varset _portList[9] Serpent's Spine: tox, stonebrunt, icefall, sunderock, blightfire
		/varset _portList[10] Taelosia: barindu, natimbi
		/varset _portList[11] The Planes: grounds, potime, brells, arcstone, pok
		/varset _portList[12] Velious: cobaltscar, wakening, greatdivide, iceclad
		/varset _portList[13]
		/varset _portList[14] -- end port list
	}
	
	| feed the response back to sender		
	/for _count 1 to ${_portList.Size}
		/t ${_sender} ${_portList[${_count}]}
		| /delay 5
	/next _count

/return