// churromain.c
// Esqueleto de juegos de la churrera
// Copyleft 2010 The Mojon Twins

// Renombrar a tujuego.c o lo que sea para que quede m�s bello
// y to la pesca.

// Super mega hiper disclaimer: como ver�s, el c�digo es una 
// paranoia de la hostia. El tema est� en que se reaprovecha
// mucho para poder hacer distintos tipos de juego con dife-
// rentes caracter�sticas, pero no todo el c�digo es usable
// en todos los tipos de juego. Para ahorrar, se usan direc-
// tivas que incluyen s�lo el c�digo necesario, dejando fuera
// todo lo que no se usa... Por eso es tan zapati�stico.

// Y s�, es el mismo motor que Uwol o Phantomasa 2, pero
// mejorado, tuneado, modularizado y hipervitaminado.

#include <spritepack.h>

/*
#pragma output STACKPTR=61440
#define AD_FREE			60000
*/
// Tighten it even more... Gaining about 1.1 extra Kb 
#pragma output STACKPTR=61952
#define AD_FREE			60020

#include "config.h"

struct sp_UDK keys;
void *joyfunc;				// Puntero a la funci�n de manejo seleccionada.

void *my_malloc(uint bytes) {
   return sp_BlockAlloc(0);
}

void *u_malloc = my_malloc;
void *u_free = sp_FreeBlock;

// Globales muy globalizadas

unsigned char kempston_is_attached;
struct sp_SS *sp_player;
struct sp_SS *sp_moviles [3];
#ifdef PLAYER_CAN_FIRE
struct sp_SS *sp_bullets [MAX_BULLETS];
#endif
struct sp_Rect spritesClipValues;
struct sp_Rect *spritesClip;

unsigned char enoffs;


// Aux

extern char asm_number[1];
extern unsigned int asm_int [1];
extern unsigned int asm_int_2 [1];
extern unsigned int seed [1];
unsigned char half_life;

#asm
._asm_number 
	defb 0
._asm_int
	defw 0
._asm_int_2
	defw 0
._seed	
	defw 0
#endasm

// Codigo para joystick virtual Android compatible con Spectranet (comienzo)
#include <sys/socket.h>
#include <sockpoll.h>
#include <sys/compiler.h>
// Codigo para joystick virtual Android compatible con Spectranet (final)

// Cosas del juego:

#include "aplib.h"
#include "pantallas.h"
#include "mapa.h"
#include "tileset.h"
#include "sprites.h"
#if defined(PLAYER_KILLS_ENEMIES) || defined(PLAYER_CAN_FIRE)
#include "extrasprites.h"
#endif
#include "enems.h"
#include "beeper.h"
#include "engine.h"

// Y el main

void main (void) {
	int j;
	unsigned char *allpurposepuntero;
	unsigned char i;
	unsigned char playing;
	unsigned char n_pant;
	unsigned char maincounter;
	unsigned int orgx, orgy;
#ifdef RANDOM_RESPAWN
	int x, y;
#else
	unsigned char x, y;
#endif
	unsigned char success;
	
	unsigned int key_r;
	
	// Para uso exclusivo con Spectranet, configura puerto 8081 TCP/IP con Joystick virtual
	// El juego no cargara hasta que detecte conexion de joystick o se pulse una tecla
	#asm
		CALL 0DAFh;
		
		; Data
		line: 
			defb 'H','o','r','a','c','e',' ','G','o','e','s',' ','T','o',' ','T','h','e',' ','T','o','w','e','r','.',' ','A','n','d','r','o','i','d',' ','V','i','r','t','u','a','l',' ','J','o','y','s','t','i','c','k',' ','c','o','m','p','a','t','i','b','l','e',' ','v','e','r','s','i','o','n','.','W','a','i','t','i','n','g',' ','f','o','r',' ','a','n',' ','A','n','d','r','o','i','d',' ','J','o','y','s','t','i','c','k','.',13,'$'
			
		start:
			ld      a, 2                  ; channel 2 = "S" for screen
			call    $1601                 ; Select print channel using ROM

			ld hl,line                    ; Print line
			call printline
			ret                        

		printline:                     	  ; Routine to print out a line
			ld a,(hl)                     ; Get character to print
			cp '$'                        ; See if it '$' terminator
			jp z,printend                 ; 
			rst 16                        ; Spectrum: Print the character in 'A'
			inc hl                        ; Move onto the next character
			jp printline                  ; Loop round
		
		printend:
		
	#endasm
	init_tcpip ();
	
	// Primero detecto el kempston
	
	#asm
		halt
		in	a, (31)
		inc a
		ld	(_kempston_is_attached), a
		di
	#endasm
	
	// Inicializo splib2
	
	sp_Initialize (7, 0);
	sp_Border (BLACK);
	sp_AddMemory(0, 56, 14, AD_FREE);
	
	// Defino las teclas y el control por defecto
	
	keys.up    = sp_LookupKey('q');
	keys.down  = sp_LookupKey('a');
	keys.left  = sp_LookupKey('o');
	keys.right = sp_LookupKey('p');
	keys.fire  = sp_LookupKey(' ');
	key_r = sp_LookupKey ('r');
	
	joyfunc = sp_JoyKeyboard;

	// Cargo el tileset
		
	allpurposepuntero = tileset;
	for (j = 0; j < 255; j++) {
		sp_TileArray (j, allpurposepuntero);
		allpurposepuntero += 8;
	}

	// Rect�ngulo de clipping

	spritesClipValues.row_coord = VIEWPORT_Y;
	spritesClipValues.col_coord = VIEWPORT_X;
	spritesClipValues.height = 20;
	spritesClipValues.width = 30;
	spritesClip = &spritesClipValues;
	
	// Creamos los sprites
	
#ifdef NO_MASKS
	sp_player = sp_CreateSpr (sp_OR_SPRITE, 3, sprite_2_a, 1, TRANSPARENT);
	sp_AddColSpr (sp_player, sprite_2_b, TRANSPARENT);
	sp_AddColSpr (sp_player, sprite_2_c, TRANSPARENT);
	player.current_frame = player.next_frame = sprite_2_a;
	
	for (i = 0; i < 3; i ++) {
		sp_moviles [i] = sp_CreateSpr(sp_OR_SPRITE, 3, sprite_9_a, 1, TRANSPARENT);
		sp_AddColSpr (sp_moviles [i], sprite_9_b, TRANSPARENT);
		sp_AddColSpr (sp_moviles [i], sprite_9_c, TRANSPARENT);	
		en_an [i].current_frame = sprite_9_a;
	}
#else
	sp_player = sp_CreateSpr (sp_MASK_SPRITE, 3, sprite_2_a, 1, TRANSPARENT);
	sp_AddColSpr (sp_player, sprite_2_b, TRANSPARENT);
	sp_AddColSpr (sp_player, sprite_2_c, TRANSPARENT);
	player.current_frame = player.next_frame = sprite_2_a;
	
	for (i = 0; i < 3; i ++) {
		sp_moviles [i] = sp_CreateSpr(sp_MASK_SPRITE, 3, sprite_9_a, 2, TRANSPARENT);
		sp_AddColSpr (sp_moviles [i], sprite_9_b, TRANSPARENT);
		sp_AddColSpr (sp_moviles [i], sprite_9_c, TRANSPARENT);	
		en_an [i].current_frame = sprite_9_a;
	}
#endif

#ifdef PLAYER_CAN_FIRE
	for (i = 0; i < MAX_BULLETS; i ++) {
		sp_bullets [i] = sp_CreateSpr (sp_OR_SPRITE, 2, sprite_19_a, 1, TRANSPARENT);
		sp_AddColSpr (sp_bullets [i], sprite_19_b, TRANSPARENT);
	}
#endif
	
	while (1) {
		// Aqu� la pantalla de t�tulo
		sp_UpdateNow();
		unpack ((unsigned int) (s_title));
		select_joyfunc ();
		
#ifndef DIRECT_TO_PLAY
		// Si no est� definido DIRECT_TO_PLAY, entonces borramos la pantalla de
		// t�tulo y colocamos el marco
		cortina ();
		sp_UpdateNow();
		unpack ((unsigned int) (s_marco));
#endif

		// Empezamos.

		playing = 1;
		init_player ();
		init_hotspots ();
#ifndef DEACTIVATE_KEYS
		init_cerrojos ();
#endif
#if defined(PLAYER_KILLS_ENEMIES) || defined (PLAYER_CAN_FIRE)
		init_malotes ();
#endif
#ifdef PLAYER_CAN_FIRE
		init_bullets ();
#endif	
			
		n_pant = SCR_INICIO;
		maincounter = 0;
		
		draw_scr (n_pant);
		draw_life ();
#ifndef	DEACTIVATE_OBJECTS
		draw_objs ();
#endif
#ifndef DEACTIVATE_KEYS
		draw_keys ();
#endif

#if defined(PLAYER_KILLS_ENEMIES) || defined(PLAYER_CAN_FIRE)
		draw_killed ();
#endif
#ifdef PLAYER_KILLS_ENEMIES		
		sp_PrintAtInv (KILLED_Y, 2 + KILLED_X, 71, 15);// Poner total
		sp_PrintAtInv (KILLED_Y, 3 + KILLED_X, 71, 16 + BADDIES_COUNT / 10);
		sp_PrintAtInv (KILLED_Y, 4 + KILLED_X, 71, 16 + BADDIES_COUNT % 10);
#endif
		
		half_life = 0;
		
		orgx = player.x;
		orgy = player.y;
		
		while (playing) {
			maincounter ++;
			half_life = !half_life;
			
			// Mover player
			
			if ( !(player.estado & EST_MUR) )
				move (n_pant);
			else {
				// ?
			}
			
			// Mover enemigos
			
			mueve_bicharracos (n_pant);

#ifdef PLAYER_CAN_FIRE
			// Mover balas
				
			mueve_bullets ();
#endif

			// Render			

			for (i = 0; i < 3; i ++) {
				if (malotes [enoffs + i].t != 0) {		// Esto s�lo es necesario si hay habitaciones con menos de 3.
#ifdef RANDOM_RESPAWN
					if (en_an [i].fanty_activo) {
						x = en_an [i].x >> 6;
						y = en_an [i].y >> 6;
					} else {
#endif
						x = malotes [enoffs + i].x;
						y = malotes [enoffs + i].y;
#ifdef RANDOM_RESPAWN
					}
#endif
					sp_MoveSprAbs (sp_moviles [i], spritesClip, en_an [i].next_frame - en_an [i].current_frame, VIEWPORT_Y + (y >> 3), VIEWPORT_X + (x >> 3),x & 7, y & 7);
					en_an [i].current_frame = en_an [i].next_frame;
				}
			}

			// Precalc para c�lculos (eh?)
			
			x = player.x >> 6;
			y = player.y >> 6;
			
			if ( !(player.estado & EST_PARP) || !(half_life) )
				sp_MoveSprAbs (sp_player, spritesClip, player.next_frame - player.current_frame, VIEWPORT_Y + (y >> 3), VIEWPORT_X + (x >> 3), x & 7, y & 7);
			else
				sp_MoveSprAbs (sp_player, spritesClip, player.next_frame - player.current_frame, -2, -2, 0, 0);
			
			player.current_frame = player.next_frame;
			
			
#ifdef PLAYER_CAN_FIRE
			for (i = 0; i < MAX_BULLETS; i ++) {
				if (bullets [i].estado == 1) {
					sp_MoveSprAbs (sp_bullets [i], spritesClip, 0, VIEWPORT_Y + (bullets [i].y >> 3), VIEWPORT_X + (bullets [i].x >> 3), bullets [i].x & 7, bullets [i].y & 7);
				} else {
					sp_MoveSprAbs (sp_bullets [i], spritesClip, 0, -2, -2, 0, 0);
				}
			}
#endif			
			
			// Update to screen
			
			sp_UpdateNow();
			
#ifdef PLAYER_CAN_FIRE
			for (i = 0; i < 3; i ++)
				if (en_an [i].morido == 1) {
					peta_el_beeper (1);
					en_an [i].morido = 0;
				} 	
#endif

#ifdef PLAYER_FLICKERS
			// parpadeo
			if (player.estado == EST_PARP) {
				player.ct_estado --;
				if (player.ct_estado == 0)
					player.estado = EST_NORMAL;	
			}
#endif			
			
			// Coger objeto
			
			if (x >= hotspot_x - 15 && x <= hotspot_x + 15 && y >= hotspot_y - 15 && y <= hotspot_y + 15) {
				// restauramos el tile de fondo
				draw_coloured_tile (VIEWPORT_X + (hotspot_x >> 3), VIEWPORT_Y + (hotspot_y >> 3), orig_tile);
				// Desactivamos el hotspot:
				hotspot_x = hotspot_y = 240;
				// �Sumamos un objeto, una llave, o vida?
				if (hotspots [n_pant].act == 0) {
					player.life += PLAYER_REFILL;
					if (player.life > PLAYER_LIFE)
						player.life = PLAYER_LIFE;
					draw_life ();
					hotspots [n_pant].act = 2;
#ifndef DEACTIVATE_OBJECTS
				} else if (hotspots [n_pant].tipo == 1) {
					player.objs ++;
					draw_objs ();	
					hotspots [n_pant].act = 0;
#endif
#ifndef DEACTIVATE_KEYS
				} else if (hotspots [n_pant].tipo == 2) {
					player.keys ++;
					draw_keys ();
					hotspots [n_pant].act = 0;
#endif
				} 
				// PLOP!!
				peta_el_beeper (0);			
			}
			
			// Comprobaciones
			
			i = (joyfunc) (&keys);
#ifdef PLAYER_AUTO_CHANGE_SCREEN
			if (player.x == 0 && player.vx < 0) {
				n_pant --;
				draw_scr (n_pant);
				player.x = 14336;
				orgx = player.x;
				orgy = player.y;
			}
			if (player.x == 14336 && player.vx > 0) {
				n_pant ++;
				draw_scr (n_pant);
				player.x = 0;
				orgx = player.x;
				orgy = player.y;
			}
#else
			if (player.x == 0 && ((i & sp_LEFT) == 0)) {
				n_pant --;
				draw_scr (n_pant);	
				player.x = 14336;
				orgx = player.x;
				orgy = player.y;
			}
			if (player.x == 14336 && ((i & sp_RIGHT) == 0)) {		// 14336 = 224 * 64
				n_pant ++;
				draw_scr (n_pant);
				player.x = 0;
				orgx = player.x;
				orgy = player.y;
			}
#endif
			if (player.y == 0 && player.vy < 0 && n_pant >= MAP_W) {
				n_pant -= MAP_W;
				draw_scr (n_pant);
				player.y = 9216;
				orgx = player.x;
				orgy = player.y;	
			}
			if (player.y == 9216 && player.vy > 0) {				// 9216 = 144 * 64
				if (n_pant < MAP_W * MAP_H - MAP_W) {
					n_pant += MAP_W;
					draw_scr (n_pant);
					player.y = 0;
					orgx = player.x;
					orgy = player.y;
				} else {
					player.vy = -PLAYER_MAX_VY_CAYENDO;	
					if (player.life > 0) {
						peta_el_beeper (4);
						player.life --;	
						draw_life ();
					}
				}
			}
			
			// Restart screen
			// Codigo para joystick virtual Android compatible con Spectranet (simulate_key_r)
			if (sp_KeyPressed (key_r)||simulate_key_r==1) {
				
				simulate_key_r=0;
				
				peta_el_beeper (3);	
				peta_el_beeper (3);
				player.x = orgx;
				player.y = orgy;
				draw_scr (n_pant);
			}
			
			// llegarse al final
			
			if (player.objs == PLAYER_NUM_OBJETOS) {
				success = 0;
				if (n_pant == pant_final) {
					if ((player.x >> 10) == PLAYER_FIN_X && (player.y >> 10) == PLAYER_FIN_Y) 
						success = 1;
				} else if (pant_final == 99) {
					success = 1;
				}
				if (success) {
					cortina ();
					game_ending ();
					playing = 0;
					cortina ();
				}
			}
			
			// game over
			
			if (player.life == 0) {
				
				// �Saca a todo el mundo de aqu�!
				sp_MoveSprAbs (sp_player, spritesClip, 0, VIEWPORT_Y + 30, VIEWPORT_X + 20, 0, 0);				
				for (i = 0; i < 3; i ++) {
					if (malotes [enoffs + i].t != 0)
						sp_MoveSprAbs (sp_moviles [i], spritesClip, 0, VIEWPORT_Y + 30, VIEWPORT_X + 20, 0, 0);
				}
				
				game_over ();
				playing = 0;
				cortina ();
			}
		}	
	}
}
