#include "ui_local.h"

typedef struct
{
	menuframework_s	menu;
	menubitmap_s	borderleft;
	menubitmap_s	borderright;
	menubitmap_s	player;
	playerInfo_t	playerinfo;
	char		playerModel[MAX_QPATH];
	weapon_t	weaponnum;
	int 	mode;
} playershot_t;

static playershot_t s_playershot;

/*
=================
PlayerShot_DrawBorder
=================
*/
static void PlayerShot_DrawBorder( void *self )
{
	menubitmap_s	*b;
	vec4_t color      = {1.00f, 1.00f, 1.00f, 1.00f};
	int num;
	char		text[1024];

	b = (menubitmap_s*) self;

	sscanf( ui_playershotbackgroundcolor.string, "%f %f %f %f", &color[0], &color[1], &color[2], &color[3] );

	UI_FillRect( b->generic.x, b->generic.y, b->width, b->height, color );
}

/*
=================
PlayerShot_DrawPlayer
=================
*/
static void PlayerShot_DrawPlayer( void *self )
{
	vec3_t	viewangles;
	menubitmap_s	*b;
	char			buf[MAX_QPATH];
	weapon_t weaponnum;

	b = (menubitmap_s*) self;

	if( ui_playershotmode.value == 0 ) {
		static int i = 0;
		i++;
		i = i%360;
		viewangles[YAW]   = i;
		viewangles[PITCH] = 0;
		viewangles[ROLL]  = 0;
	} else if( ui_playershotmode.value == 1 ) {
		viewangles[YAW]   = 180 - 30;
		viewangles[PITCH] = 0;
		viewangles[ROLL]  = 0;
	} else if( ui_playershotmode.value == 2 ) {
		viewangles[YAW]   = 180;
		viewangles[PITCH] = 0;
		viewangles[ROLL]  = 0;
	} else if( ui_playershotmode.value == 3 ) {
		viewangles[YAW]   = 270;
		viewangles[PITCH] = 0;
		viewangles[ROLL]  = 0;
	} else if( ui_playershotmode.value == 4 ) {
		static int i = 0;
		i++;
		i = i%360;
		viewangles[YAW]   = i;
		viewangles[PITCH] = 0;
		viewangles[ROLL]  = 0;
	}

	trap_Cvar_VariableStringBuffer( "model", buf, sizeof( buf ) );
	weaponnum = ui_playershotweapon.value;
	if ( strcmp( buf, s_playershot.playerModel ) != 0 || s_playershot.mode != ui_playershotmode.value ) {
		UI_PlayerInfo_SetModel( &s_playershot.playerinfo, buf );
		strcpy( s_playershot.playerModel, buf );
		s_playershot.mode = ui_playershotmode.value;
		s_playershot.weaponnum = weaponnum;
		UI_PlayerInfo_SetInfo( &s_playershot.playerinfo, LEGS_IDLE, TORSO_STAND, viewangles, vec3_origin, weaponnum, qfalse );
		s_playershot.playerinfo.pendingWeapon = weaponnum;
	}
	if ( s_playershot.weaponnum != weaponnum ) {
		s_playershot.weaponnum = weaponnum;
		UI_PlayerInfo_SetInfo( &s_playershot.playerinfo, LEGS_IDLE, TORSO_STAND, viewangles, vec3_origin, weaponnum, qfalse );
	}

	PlayerShot_DrawBorder( b );

	if( ui_playershotmode.value == 0 ) {
		VectorCopy( viewangles, s_playershot.playerinfo.viewAngles );

		UI_DrawPlayer( b->generic.x, b->generic.y, b->width, b->height, &s_playershot.playerinfo, uis.realtime/2 );
	} else if( ui_playershotmode.value == 1 ) {
		VectorCopy( viewangles, s_playershot.playerinfo.viewAngles );

		shotmod_UI_DrawPlayerBody( b->generic.x, b->generic.y, b->width, b->height, &s_playershot.playerinfo, uis.realtime/2 );
	} else if( ui_playershotmode.value == 2 ) {
		VectorCopy( viewangles, s_playershot.playerinfo.viewAngles );

		shotmod_UI_DrawPlayerFace( b->generic.x, b->generic.y, b->width, b->height, &s_playershot.playerinfo, uis.realtime/2 );
	} else if( ui_playershotmode.value == 3 ) {
		VectorCopy( viewangles, s_playershot.playerinfo.viewAngles );

		shotmod_UI_DrawPlayerFace( b->generic.x, b->generic.y, b->width, b->height, &s_playershot.playerinfo, uis.realtime/2 );
	} else if( ui_playershotmode.value == 4 ) {
		VectorCopy( viewangles, s_playershot.playerinfo.viewAngles );

		shotmod_UI_DrawPlayerFace( b->generic.x, b->generic.y, b->width, b->height, &s_playershot.playerinfo, uis.realtime/2 );
	}
}

/*
=================
PlayerShot_MenuKey
=================
*/
static sfxHandle_t PlayerShot_MenuKey( int key )
{
//	menucommon_s*	m;

	switch (key)
	{
		case K_KP_LEFTARROW:
		case K_LEFTARROW:
			break;

		case K_KP_RIGHTARROW:
		case K_RIGHTARROW:
			break;
			
		case K_MOUSE2:
		case K_ESCAPE:
			break;
	}

	return ( Menu_DefaultKey( &s_playershot.menu, key ) );
}

/*
=================
PlayerShot_SetMenuItems
=================
*/
static void PlayerShot_SetMenuItems( void ) {
	vec3_t	viewangles;

	// model/skin
	memset( &s_playershot.playerinfo, 0, sizeof(playerInfo_t) );
	
	UI_PlayerInfo_SetModel( &s_playershot.playerinfo, UI_Cvar_VariableString( "model" ) );
	UI_PlayerInfo_SetInfo( &s_playershot.playerinfo, LEGS_IDLE, TORSO_STAND, viewangles, vec3_origin, ui_playershotweapon.value, qfalse );
}

/*
=================
PlayerShot_MenuInit
=================
*/
static void PlayerShot_MenuInit( void )
{
	int height;

	// zero set all our globals
	memset( &s_playershot, 0, sizeof(playershot_t) );

	PlayerModel_Cache();

	s_playershot.menu.key        = PlayerShot_MenuKey;
	s_playershot.menu.wrapAround = qtrue;
	s_playershot.menu.fullscreen = qtrue;

	height = 480 + 0;
	s_playershot.player.generic.type      = MTYPE_BITMAP;
	s_playershot.player.generic.flags     = QMF_INACTIVE;
	s_playershot.player.generic.ownerdraw = PlayerShot_DrawPlayer;
	s_playershot.player.generic.x	       = 0;
	s_playershot.player.generic.y	       = 0;
	s_playershot.player.width	           = 640;
	s_playershot.player.height            = 640;
/*	s_playershot.player.generic.x	       = 640/2-height/2 * 32/56;
	s_playershot.player.generic.y	       = -0/2;
	s_playershot.player.width	           = height * 32/56;
	s_playershot.player.height            = height;
*/
	s_playershot.borderleft.generic.type      = MTYPE_BITMAP;
	s_playershot.borderleft.generic.flags     = QMF_INACTIVE;
	s_playershot.borderleft.generic.ownerdraw = PlayerShot_DrawBorder;
	s_playershot.borderleft.generic.x	       = 0;
	s_playershot.borderleft.generic.y	       = 0;
	s_playershot.borderleft.width	           = 640/2-480 * 32/56 /2;
	s_playershot.borderleft.height            = 480;

	s_playershot.borderright.generic.type      = MTYPE_BITMAP;
	s_playershot.borderright.generic.flags     = QMF_INACTIVE;
	s_playershot.borderright.generic.ownerdraw = PlayerShot_DrawBorder;
	s_playershot.borderright.generic.x	       = 640/2+480 * 32/56 /2;
	s_playershot.borderright.generic.y	       = 0;
	s_playershot.borderright.width	           = 640/2-480 * 32/56 /2;
	s_playershot.borderright.height            = 480;


//	Menu_AddItem( &s_playershot.menu,	&s_playershot.borderleft );
//	Menu_AddItem( &s_playershot.menu,	&s_playershot.borderright );
	Menu_AddItem( &s_playershot.menu,	&s_playershot.player );

	PlayerShot_SetMenuItems();
}

/*
=================
UI_PlayerShotMenu
=================
*/
void UI_PlayerShotMenu(void)
{
	PlayerShot_MenuInit();

	UI_PushMenu( &s_playershot.menu );
}
