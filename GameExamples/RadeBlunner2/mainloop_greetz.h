// mainloop.h
// Churrera copyleft 2011 by The Mojon Twins.

void do_game (void) {
	unsigned char x;
	
	#asm
		di
	#endasm
	
	// splib2 initialization
	sp_Initialize (7, 0);
	sp_Border (BLACK);
	sp_AddMemory(0, 56, 14, AD_FREE);
	
	// Here the title screen
	sp_UpdateNow();
	blackout ();
	unpack ((unsigned int) (s_title), 16384);
	#asm
		; Music generated by beepola
		call musicstart
	#endasm
	
	// DAVID SEGUIR AQUI MANYANA ARREGLAR ESTO
	// PRIMERO MUSICA Y LUEGO GREETZ?
	// UNA NOTA Y VOLVER?
	while (1) {
		x = sp_GetKey ();
		
		switch (x) {
			case 'c':
				print_str (12, 21, 4, "AAA");
				break;
		}
		
		// Update to screen
		sp_UpdateNow();
	}
		
}
