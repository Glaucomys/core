|***
 *	test.inc
 *	for testing purposes
 *
 *

 


| Countexp.inc Version 2.0
| Written by Dregs
| Based on exp-calculating code from the rogue helper macro
|------------------------------------------------------------------------------
| Usage: Include the following lines in your macro:
|       /call CreateCountExpVariables
|       /doevents
|------------------------------------------------------------------------------
#event GainedExp "#*#You gain #0#!!"
#event _rptexp "#*#[MQ2] Report exp#*#"

Sub CreateCountExpVariables
   /declare _mobsKilled int outer 0
   /declare _rptexpAA float outer 0
   /declare _rptexp float outer 0
   /declare _rptexpAATotal float outer 0
   /declare _rptexpTotal float outer 0
   /declare _expAA float outer ${Me.Pct_expAA} 
   /declare _exp float outer ${Me.PctExp}
   /declare _currAA int outer ${Me.AAPointsTotal}
   /declare _currLVL int outer ${Me.Level}
   /declare _strtexpAA float outer ${Me.Pct_expAA}
   /declare _strtexp float outer ${Me.PctExp}
/return

Sub Event_GainedExp(string WhatType)
   /varcalc _mobsKilled (${_mobsKilled} + 1)
   /if (${Me.Level} > ${_currLVL}) /varcalc _rptexp (100 + ${Me.PctExp} - ${_exp})
   /if (${Me.Level} == ${_currLVL}) /varcalc _rptexp (${Me.PctExp} - ${_exp})
   /if (${Me.AAPointsTotal} > ${_currAA}) /varcalc _rptexpAA (100 + ${Me.Pct_expAA} - ${_expAA})
   /if (${Me.AAPointsTotal} == ${_currAA}) /varcalc _rptexpAA (${Me.Pct_expAA} - ${_expAA})
   /if (${WhatType.Equal[experience]}) {
      /echo Kill number ${_mobsKilled} yielded Solo XP: REG (${_rptexp}%), AA (${_rptexpAA}%))
   }
   /if (${WhatType.Equal[party experience]}) {
      /echo Kill number ${_mobsKilled} yielded Group XP: REG (${_rptexp}%), AA (${_rptexpAA}%))
   }
   /varcalc _rptexpTotal (${_rptexp} + ${_rptexpTotal})
   /varcalc _rptexpAATotal (${_rptexpAA} + ${_rptexpAATotal})
   /varset _currAA ${Me.AAPointsTotal}
   /varset _currLVL ${Me.Level}
   /varset _expAA ${Me.Pct_expAA} 
   /varset _exp ${Me.PctExp}
/return

Sub Event__rptexp
   /echo You have gained ${_rptexpTotal}% level exp, ${_rptexpAATotal}% AA exp,  ${_mobsKilled} mobs.
   /if (${_mobsKilled} > 0) {
      /echo You are averaging ${Math.Calc[${_rptexpTotal} / ${_mobsKilled}]}% Level exp, ${Math.Calc[${_rptexpAATotal} / ${_mobsKilled}]}% AA exp, . 
   }
/return

 ***|