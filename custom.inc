|***
 *	custom.inc
 *	this is a blank structure file
 *	feel free to add/create your own commands whatever
 *	use the other files as a templates if you need examples
 *  the set_custom_declares() will be loded on mac start if you need to add your own variables
 *
 ***|





|***
 *	DES: custom declares
 *	USE: /call set_declares_custom
 *	INI: 
 *  NOTE: This function is initialized for you in the macs base prep routine.
 ***|
sub set_declares_custom()

/return

/declare x int local 0
/declare y int local 0
/declare arraytest[3,3] string outer FALSE



/varset arraytest[1,3] elem 1,3

/varset arraytest[3,2] elem 3,2







| first dimension
/for x 1 to 3
	
	| second dimension
	/for y 1 to 3

		/echo [${x},${y}]: ${arraytest[${x},${y}]}
		
		/ini "testarray.ini" test "arraytest[${x},${y}]" "${arraytest[${x},${y}]}"
	
	/next y
	
/next x


/return

