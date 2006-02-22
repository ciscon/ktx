/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// commands.c

#include "g_local.h"

extern char *Enables( float f );
extern char *Allows( float f );

void StuffMainMaps();

void SendMessage(char *name);
float CountRPlayers();
float CountPlayers();
float CountRTeams();
float CountTeams();
void PlayerReady ();
void PlayerBreak ();
void ReqAdmin ();
void AdminForceStart ();
void AdminForceBreak ();
void AdminForcePause ();
void TogglePreWar ();
void ToggleMapLock ();
void ToggleMaster ();
void AdminKick ();
void YesKick ();
void DontKick ();
void VoteAdmin();
void VoteYes();
void VoteNo();
void BecomeCaptain ();
void Deathmsg ();
void RandomPickup();
void ShowDMM();
void ChangeDM(float dmm);
void ChangeLock();
void ChangeOvertime();
void ChangeOvertimeUp();
void ChangeTP();
void ToggleFallBunny ();
void FragsDown();
void FragsUp();
void ListWhoNot();
void ModStatus();
void ModStatus2();
void ModStatusVote();
void PlayerStats();
void PlayerStatus();
void PlayerStatusN();
void PlayerStatusS();
void PrintScores();
void ResetOptions();
void ReportMe();
void SendKillerMsg();
void SendNewcomerMsg();
void SendVictimMsg();
void ShowNick();
void ShowCmds();
void ShowMaps();
void ShowMessages();
void ShowOpts();
void ShowQizmo();
void ShowRules();
void ShowVersion();
void ToggleBerzerk();
void ToggleDischarge();
void ToggleDropPack();
void ToggleDropQuad();
void ToggleDropRing();
void ToggleFairPacks();
void ToggleFreeze();
void TogglePowerups();
void ToggleQEnemy();
void ToggleQLag();
void ToggleQPoint();
void ToggleRespawn666();
void ToggleRespawns();
void ToggleSpecTalk();
void ToggleSpeed();
void VotePickup();
void VoteUnpause();
void UserMode(float umode);
void Wp_Reset ();
void Wp_Stats(float on);
void Sc_Stats(float on);
void t_jump (float j_type);
void klist ();
void hdptoggle ();
void handicap ();
void noweapon ();
void tracklist ();
void fpslist ();

void favx_add ( float fav_num );
void xfav_go ( float fav_num );
void fav_add ();
void fav_del ();
void fav_all_del ();
void fav_next ();
void fav_show ();

// VVD pos_save/pos_move commands {
void Pos_Show ();
void Pos_Save ();
void Pos_Move ();
// VVD }

void TogglePractice();

// spec
void ShowCamHelp();

void TeamSay(float fsndname);
void TimeDown(float t);
void TimeUp(float t);
void TimeSet(float t);

cmd_t cmds[] = {

	{ "commands",    ShowCmds,			        0    , CF_BOTH | CF_MATCHLESS },
	{ "scores",      PrintScores,		        0    , CF_BOTH | CF_MATCHLESS },
	{ "stats",       PlayerStats,               0    , CF_BOTH | CF_MATCHLESS },
	{ "options",     ShowOpts,                  0    , CF_PLAYER      },
	{ "ready",       PlayerReady,               0    , CF_PLAYER },
	{ "break",       PlayerBreak,               0    , CF_PLAYER | CF_MATCHLESS },
	{ "status",      ModStatus,                 0    , CF_BOTH | CF_MATCHLESS },
	{ "status2",     ModStatus2,                0    , CF_BOTH | CF_MATCHLESS },
	{ "who",         PlayerStatus,              0    , CF_BOTH        },
	{ "whoskin",     PlayerStatusS,             0    , CF_BOTH | CF_MATCHLESS },
	{ "whonot",      PlayerStatusN,             0    , CF_BOTH        },
	{ "whovote",     ModStatusVote,             0    , CF_BOTH | CF_MATCHLESS },
	{ "spawn",       ToggleRespawns,            0    , CF_PLAYER | CF_SPC_ADMIN },
	{ "powerups",    TogglePowerups,            0    , CF_PLAYER | CF_SPC_ADMIN },
	{ "discharge",   ToggleDischarge,           0    , CF_PLAYER      },
	{ "dm",          ShowDMM,                   0    , CF_PLAYER      },
	{ "dmm1",        ChangeDM,                  1    , CF_PLAYER | CF_SPC_ADMIN },
	{ "dmm2",        ChangeDM,                  2    , CF_PLAYER | CF_SPC_ADMIN },
	{ "dmm3",        ChangeDM,                  3    , CF_PLAYER | CF_SPC_ADMIN },
	{ "dmm4",        ChangeDM,                  4    , CF_PLAYER | CF_SPC_ADMIN },
	{ "dmm5",        ChangeDM,                  5    , CF_PLAYER | CF_SPC_ADMIN },
	{ "tp",          ChangeTP,                  0    , CF_PLAYER | CF_SPC_ADMIN },
	{ "timedown1",   TimeDown,				  1.0f   , CF_PLAYER | CF_SPC_ADMIN },
	{ "timeup1",     TimeUp,				  1.0f   , CF_PLAYER | CF_SPC_ADMIN },
	{ "timedown",    TimeDown,				  5.0f   , CF_PLAYER | CF_SPC_ADMIN },
	{ "timeup",      TimeUp,				  5.0f   , CF_PLAYER | CF_SPC_ADMIN },
	{ "fallbunny",   ToggleFallBunny,           0    , CF_BOTH_ADMIN  },
	{ "fragsdown",   FragsDown,                 0    , CF_PLAYER      },
	{ "fragsup",     FragsUp,                   0    , CF_PLAYER      },
	{ "dropquad",    ToggleDropQuad,            0    , CF_PLAYER      },
	{ "dropring",    ToggleDropRing,            0    , CF_PLAYER      },
	{ "droppack",    ToggleDropPack,            0    , CF_PLAYER      },
	                                             
    { "silence",     ToggleSpecTalk,            0    , CF_PLAYER | CF_SPC_ADMIN },
	{ "reset",       ResetOptions,              0    , CF_PLAYER | CF_SPC_ADMIN },
	{ "report",      ReportMe,                  0    , CF_PLAYER      },
	{ "rules",       ShowRules,                 0    , CF_PLAYER | CF_MATCHLESS },
	{ "lock",        ChangeLock,                0    , CF_PLAYER      },
	{ "maps",        ShowMaps,                  0    , CF_PLAYER | CF_SPC_ADMIN | CF_MATCHLESS},
	{ "spawn666",    ToggleRespawn666,          0    , CF_PLAYER      },
	{ "admin",       ReqAdmin,                  0    , CF_BOTH        },
	{ "forcestart",  AdminForceStart,           0    , CF_BOTH_ADMIN  },
	{ "forcebreak",  AdminForceBreak,           0    , CF_BOTH_ADMIN  },
	{ "forcepause",  AdminForcePause,           0    , CF_BOTH_ADMIN  },
	{ "pickup",      VotePickup,                0    , CF_PLAYER      },
	{ "prewar",      TogglePreWar,              0    , CF_BOTH_ADMIN  },
	{ "lockmap",     ToggleMapLock,             0    , CF_BOTH_ADMIN  },
	{ "master",      ToggleMaster,              0    , CF_BOTH_ADMIN  },
	{ "speed",       ToggleSpeed,               0    , CF_PLAYER      },
	{ "fairpacks",   ToggleFairPacks,           0    , CF_PLAYER      },
	{ "about",       ShowVersion,               0    , CF_BOTH | CF_MATCHLESS },
	{ "shownick",    ShowNick,                  0    , CF_PLAYER      },
	{ "time5",       TimeSet,		  	 	  5.0f   , CF_PLAYER      },
	{ "time10",      TimeSet,		  	     10.0f   , CF_PLAYER      },
	{ "time15",      TimeSet,		  	     15.0f   , CF_PLAYER      },
	{ "time20",      TimeSet,                20.0f   , CF_PLAYER      },
	{ "time25",      TimeSet,                25.0f   , CF_PLAYER      },
	{ "time30",      TimeSet,                30.0f   , CF_PLAYER      },
	{ "berzerk",     ToggleBerzerk,             0    , CF_PLAYER      },
	                                             
	{ "ksound1",     TeamSay,   			    1    , CF_PLAYER      },
	{ "ksound2",     TeamSay,   			    2    , CF_PLAYER      },
	{ "ksound3",     TeamSay,   			    3    , CF_PLAYER      },
	{ "ksound4",     TeamSay,   			    4    , CF_PLAYER      },
	{ "ksound5",     TeamSay,   			    5    , CF_PLAYER      },
	{ "ksound6",     TeamSay,   			    6    , CF_PLAYER      },
	                                           
	{ "qizmo",       ShowQizmo,                 0    , CF_PLAYER      },
	                                             
	{ "messages",    ShowMessages,              0    , CF_PLAYER | CF_MATCHLESS },
	{ "killer",      SendKillerMsg,             0    , CF_PLAYER | CF_MATCHLESS },
	{ "victim",      SendVictimMsg,             0    , CF_PLAYER | CF_MATCHLESS },
	{ "newcomer",    SendNewcomerMsg,           0    , CF_PLAYER | CF_MATCHLESS },
	                                             
	{ "qlag",        ToggleQLag,                0    , CF_PLAYER | CF_SPC_ADMIN },
	{ "qenemy",      ToggleQEnemy,              0    , CF_PLAYER | CF_SPC_ADMIN },
	{ "qpoint",      ToggleQPoint,              0    , CF_PLAYER | CF_SPC_ADMIN },
	                                          
    { "kick",        AdminKick,                 0    , CF_BOTH_ADMIN  },
    { "y",           YesKick,                   0    , CF_BOTH_ADMIN  },
    { "n",           DontKick,                  0    , CF_BOTH_ADMIN  },
    { "list",        ListWhoNot,                0    , CF_PLAYER      },
    { "overtime",    ChangeOvertime,            0    , CF_PLAYER | CF_SPC_ADMIN },
    { "overtimeup",  ChangeOvertimeUp,          0    , CF_PLAYER | CF_SPC_ADMIN },
    { "elect",       VoteAdmin,                 0    , CF_BOTH        },
    { "yes",         VoteYes,                   0    , CF_PLAYER      },
    { "no",          VoteNo,                    0    , CF_PLAYER      },
    { "captain",     BecomeCaptain,             0    , CF_PLAYER      },
    { "freeze",      ToggleFreeze,              0    , CF_PLAYER      },
	{ "deathmsg",    Deathmsg,                  0    , CF_BOTH_ADMIN  },
    { "rpickup",     RandomPickup,              0    , CF_PLAYER | CF_SPC_ADMIN  },
    
    { "1on1",        UserMode,                  1	 , CF_PLAYER | CF_SPC_ADMIN },
    { "2on2",        UserMode,                  2	 , CF_PLAYER | CF_SPC_ADMIN },
    { "3on3",        UserMode,                  3	 , CF_PLAYER | CF_SPC_ADMIN },
    { "4on4",        UserMode,                  4	 , CF_PLAYER | CF_SPC_ADMIN },
    { "10on10",      UserMode,                  5	 , CF_PLAYER | CF_SPC_ADMIN },
    { "ffa",         UserMode,                  6	 , CF_PLAYER | CF_SPC_ADMIN },
    
    { "unpause",     VoteUnpause,               0    , CF_PLAYER      },
    { "practice",    TogglePractice,            0    , CF_PLAYER | CF_SPC_ADMIN },
    { "wp_reset",    Wp_Reset,                  0    , CF_PLAYER      },
    { "+wp_stats",   Wp_Stats,                  2    , CF_BOTH | CF_MATCHLESS },
    { "-wp_stats",   Wp_Stats,                  1    , CF_BOTH | CF_MATCHLESS },
    { "tkfjump",     t_jump,                    1    , CF_PLAYER | CF_SPC_ADMIN },
    { "tkrjump",     t_jump,                    2    , CF_PLAYER | CF_SPC_ADMIN },
    { "klist",       klist,                     0    , CF_BOTH | CF_MATCHLESS },
    { "hdptoggle",   hdptoggle,                 0    , CF_BOTH_ADMIN },
    { "handicap",    handicap,                  0    , CF_PLAYER | CF_PARAMS | CF_MATCHLESS },
    { "noweapon",    noweapon,                  0    , CF_PLAYER | CF_PARAMS | CF_SPC_ADMIN },

    { "cam",         ShowCamHelp,               0    , CF_SPECTATOR | CF_MATCHLESS },

    { "tracklist",   tracklist,                 0    , CF_BOTH | CF_MATCHLESS  },
    { "fpslist",     fpslist,                   0    , CF_BOTH | CF_MATCHLESS  },

    { "fav1_add",    favx_add,                  1    , CF_SPECTATOR },
    { "fav2_add",    favx_add,                  2    , CF_SPECTATOR },
    { "fav3_add",    favx_add,                  3    , CF_SPECTATOR },
    { "fav4_add",    favx_add,                  4    , CF_SPECTATOR },
    { "fav5_add",    favx_add,                  5    , CF_SPECTATOR },
    { "fav6_add",    favx_add,                  6    , CF_SPECTATOR },
    { "fav7_add",    favx_add,                  7    , CF_SPECTATOR },
    { "fav8_add",    favx_add,                  8    , CF_SPECTATOR },
    { "fav9_add",    favx_add,                  9    , CF_SPECTATOR },
    { "fav10_add",   favx_add,                 10    , CF_SPECTATOR },
    { "fav11_add",   favx_add,                 11    , CF_SPECTATOR },
    { "fav12_add",   favx_add,                 12    , CF_SPECTATOR },
    { "fav13_add",   favx_add,                 13    , CF_SPECTATOR },
    { "fav14_add",   favx_add,                 14    , CF_SPECTATOR },
    { "fav15_add",   favx_add,                 15    , CF_SPECTATOR },
    { "fav16_add",   favx_add,                 16    , CF_SPECTATOR },
    { "fav17_add",   favx_add,                 17    , CF_SPECTATOR },
    { "fav18_add",   favx_add,                 18    , CF_SPECTATOR },
    { "fav19_add",   favx_add,                 19    , CF_SPECTATOR },
    { "fav20_add",   favx_add,                 20    , CF_SPECTATOR },
    { "1fav_go",     xfav_go,                   1    , CF_SPECTATOR },
    { "2fav_go",     xfav_go,                   2    , CF_SPECTATOR },
    { "3fav_go",     xfav_go,                   3    , CF_SPECTATOR },
    { "4fav_go",     xfav_go,                   4    , CF_SPECTATOR },
    { "5fav_go",     xfav_go,                   5    , CF_SPECTATOR },
    { "6fav_go",     xfav_go,                   6    , CF_SPECTATOR },
    { "7fav_go",     xfav_go,                   7    , CF_SPECTATOR },
    { "8fav_go",     xfav_go,                   8    , CF_SPECTATOR },
    { "9fav_go",     xfav_go,                   9    , CF_SPECTATOR },
    { "10fav_go",    xfav_go,                  10    , CF_SPECTATOR },
    { "11fav_go",    xfav_go,                  11    , CF_SPECTATOR },
    { "12fav_go",    xfav_go,                  12    , CF_SPECTATOR },
    { "13fav_go",    xfav_go,                  13    , CF_SPECTATOR },
    { "14fav_go",    xfav_go,                  14    , CF_SPECTATOR },
    { "15fav_go",    xfav_go,                  15    , CF_SPECTATOR },
    { "16fav_go",    xfav_go,                  16    , CF_SPECTATOR },
    { "17fav_go",    xfav_go,                  17    , CF_SPECTATOR },
    { "18fav_go",    xfav_go,                  18    , CF_SPECTATOR },
    { "19fav_go",    xfav_go,                  19    , CF_SPECTATOR },
    { "20fav_go",    xfav_go,                  20    , CF_SPECTATOR },
    { "fav_add",      fav_add,                  0    , CF_SPECTATOR | CF_MATCHLESS },
    { "fav_del",      fav_del,                  0    , CF_SPECTATOR | CF_MATCHLESS },
    { "fav_all_del",  fav_all_del,              0    , CF_SPECTATOR | CF_MATCHLESS },
    { "fav_next",     fav_next,                 0    , CF_SPECTATOR | CF_MATCHLESS },
    { "fav_show",     fav_show,                 0    , CF_SPECTATOR | CF_MATCHLESS },
    { "+scores",     Sc_Stats,                  2    , CF_BOTH | CF_MATCHLESS },
    { "-scores",     Sc_Stats,                  1    , CF_BOTH | CF_MATCHLESS },
// VVD pos_save/pos_move commands {
    { "pos_show",     Pos_Show,                 0    , CF_BOTH | CF_MATCHLESS },
    { "pos_save",     Pos_Save,                 0    , CF_BOTH | CF_MATCHLESS },
    { "pos_move",     Pos_Move,                 0    , CF_BOTH | CF_MATCHLESS }
// VVD }
};

int cmds_cnt = sizeof( cmds ) / sizeof( cmds[0] );

//
// DoCommand
//
// return -1 if command is out of range in 'cmds' array
// return -2 if wrong class
// return -3 if access denied
// return -4 if function is wrong
// return -5 if cmd does't allowed in matchLess mode
//

int DoCommand(int icmd)
{
	int spc = self->k_spectator;
	int adm = (int) self->k_admin;

	if ( !( icmd >= 0 && icmd < cmds_cnt ) )
		return -1;

	if ( k_matchLess && !(cmds[icmd].cf_flags & CF_MATCHLESS) )
		return -5; // cmd does't allowed in matchLess mode

	if ( spc ) { // spec
		if ( !(cmds[icmd].cf_flags & CF_SPECTATOR) )
			return -2; // cmd not for spectator
		if ( (cmds[icmd].cf_flags & CF_SPC_ADMIN) && adm != 2 )
			return -3; // admin rights required
	}
	else { // player
		if ( !(cmds[icmd].cf_flags & CF_PLAYER) )
			return -2; // cmd not for player
		if ( (cmds[icmd].cf_flags & CF_PLR_ADMIN) && adm != 2 )
			return -3; // admin rights required
	}

	if ( strnull( cmds[icmd].name ) || !( cmds[icmd].f ) )
		return -4;

	if (cmds[icmd].arg)
		( ( void ( * )(float) ) ( cmds[icmd].f ) ) ( cmds[icmd].arg );
	else
		( cmds[icmd].f )  ();

	return icmd;
}

void StuffAliases()
{
// stuffing for numbers, hope no flooding out
	int i;

	for ( i = 1; i <= 16; i++ )
		stuffcmd(PROG_TO_EDICT( self->s.v.owner ), "alias %d impulse %d\n", i, i);

	if ( PROG_TO_EDICT( self->s.v.owner )->k_spectator ) {
		stuffcmd(PROG_TO_EDICT( self->s.v.owner ), "alias next_fav fav_next\n");
	}
	else {
		stuffcmd(PROG_TO_EDICT( self->s.v.owner ), "alias notready break\n");
		stuffcmd(PROG_TO_EDICT( self->s.v.owner ), "alias kfjump \"impulse 156;+jump;wait;-jump\"\n");
		stuffcmd(PROG_TO_EDICT( self->s.v.owner ), "alias krjump \"impulse 164;+jump;wait;-jump\"\n");
		stuffcmd(PROG_TO_EDICT( self->s.v.owner ), "alias kinfo cmd info %%1 %%2\n");
		stuffcmd(PROG_TO_EDICT( self->s.v.owner ), "alias kuinfo cmd uinfo %%1 %%2\n");
	}
}

float StuffDeltaTime(int iDelta)
{
	iDelta = bound ( 1, ( iDelta ? iDelta : 4 ), 5); // i.e. default is 4
	return 0.01f * (float)iDelta;
}

// just check is this cmd valid for class of this player
// admin rights skipped here
qboolean isValidCmdForClass( int icmd, qboolean isSpec )
{
	if ( icmd < 0 || icmd >= cmds_cnt )
		return false;

	if ( k_matchLess && !(cmds[icmd].cf_flags & CF_MATCHLESS) )
		return false; // cmd does't allowed in matchLess mode

	// split class
	if ( isSpec ) { // spec
		if ( !(cmds[icmd].cf_flags & CF_SPECTATOR) )
			return false; // cmd not for spec
	}
	else { // player
		if ( !(cmds[icmd].cf_flags & CF_PLAYER) )
			return false; // cmd not for player
	}

	return true;
}

void StuffModCommands()
{
	int i, limit;
	qboolean spc = PROG_TO_EDICT( self->s.v.owner )->k_spectator;
	char *name, *params;
	float dt = StuffDeltaTime( atoi ( ezinfokey( PROG_TO_EDICT( self->s.v.owner ), "ss" ) ) );

	if(self->cnt == -1)	{
		StuffAliases(); // stuff impulses based aliases, or just aliases

		self->cnt = 0;
		self->s.v.nextthink = g_globalvars.time + 0.1;
		return;
	}

	i = self->cnt;
	limit = i + STUFFCMDS_PER_PORTION; // # stuffcmd's per portion

	for( ; i <= limit && ( i >= 0 && i < cmds_cnt ); i++ ) {

		name = cmds[i].name;

		if ( !isValidCmdForClass( i, spc ) ) {
			limit++;
			continue; // cmd does't valid for this class of player or matchless mode does't have this command
		}

		params = (cmds[i].cf_flags & CF_PARAMS) ? " %1 %2 %3 %4 %5" : "";

		stuffcmd(PROG_TO_EDICT( self->s.v.owner ), "alias %s cmd cc %d%s\n", name, (int)i, params);
	}

	if( i <= limit /* all commands is stuffed */ )
	{
		stuffcmd(PROG_TO_EDICT( self->s.v.owner ), "echo Commands downloaded\n");

		// no more commands, so setup for stuffing main maps.
		self->cnt = -1;
		self->s.v.think = ( func_t ) StuffMainMaps;
		self->s.v.nextthink = g_globalvars.time + dt;

		return;
	}

	// next time around we'll be starting from the i variable which is the next available command.
	self->cnt = i;
	
	// 'dt' sec delay before next stuffing.
	self->s.v.nextthink = g_globalvars.time + dt;
	
	return;
}

void Init_cmds(void)
{
	int i = 0;

	for( ; i < cmds_cnt; i++ ) {
		if ( strnull( cmds[i].name ) || !( cmds[i].f ) )
			G_Error("Init_cmds: null");

		if ( cmds[i].cf_flags & CF_PLR_ADMIN )
			cmds[i].cf_flags |= CF_PLAYER;    // this let simplify cmds[] table

		if ( cmds[i].cf_flags & CF_SPC_ADMIN )
			cmds[i].cf_flags |= CF_SPECTATOR; // this let simplify cmds[] table
	}
}   
    
void PShowCmds()
{   
	if( self->k_admin == 2 ) {
		G_sprint(self, 2,
		"�����....... give up admin rights\n"
		"������...... playerfire before game\n"
		"�������..... (un)lock current map\n"
		"����������.. force match to start\n"
		"����������.. force match to end\n"
		"����������.. toggle pausemode\n"
		"������...... toggle mastermode\n"
		"����........ toggle kick mode\n"
		"��������.... toggle new messages\n"
		"�������..... random team pickup\n"
		       "%s... toggle fallbunny\n", redtext("fallbanny"));
	} else {
		G_sprint(self, 2,
		"�����...... kombat teams info\n"
		"�����...... show game rules\n"
		"�����...... when you feel ready\n"
		"�����...... unready / vote matchend\n"
		"�������.... match control commands\n"
		"�����...... qizmo related commands\n"
		"��������... fun message commands\n"
		"������..... show server settings\n"
		"������.... more server settings\n"
		"�����...... set defaults\n"
		"������..... (un)freeze the map\n"
		"����....... list custom maps\n"
		"���........ player teamlist\n"
		"������..... players not ready\n"
		"����....... whonot to everyone\n"
		"�������.... player skinlist\n"
		"�������.... info on received votes\n"
		"������..... vote for pickup game\n"
		"�����...... show playerstats\n"
		"��������... toggle overtime mode\n"
		"����������. change overtime length\n"
		"�������.... toggle captain election\n");
		
		if( atoi( ezinfokey( world, "k_admins" ) ) ) {
			G_sprint(self, 2, "�����...... toggle admin-mode\n");
			G_sprint(self, 2, "�����...... toggle admin election\n");
		}
	}
}

void SShowCmds()
{
	if( self->k_admin == 2 ) {
		G_sprint(self, 2, 
			"�����....... give up admin rights\n"
			"������...... playerfire before game\n"
			"�������..... (un)lock current map\n"
			"����������.. force match to start\n"
			"����������.. force match to end\n"
			"����������.. toggle pausemode\n"
			"������...... toggle mastermode\n"
			"����........ toggle kick mode\n"
			"��������.... toggle new messages\n"
			"�������..... random team pickup\n"
		           "%s... toggle fallbunny\n", redtext("fallbanny"));
		G_sprint(self, 2,
			"��������.... toggle overtime mode\n"
			"����������.. change overtime length\n"
			"you may also use\n"
			"��  ��  ��������  �������\n"
			"������  ��������  �����\n"
			"����    ������    ������\n"
			"�����   ����\n");
	}
	else {
		G_sprint(self, 2, 
			"������..... show match settings\n"
			"������.... more match settings\n"
			"���........ camera help text\n"
			"���........ player teamlist\n"
			"������..... players not ready\n"
			"�������.... player skinlist\n"
			"�������.... info on received votes\n"
			"����....... joins game\n"				// FIXME: intersect with client command
			"�����...... show playerstats\n");

		if( atoi( ezinfokey( world, "k_admins" ) ) ) {
			G_sprint(self, 2, "�����...... toggle admin-mode\n");
			G_sprint(self, 2, "�����...... toggle admin election\n");
		}
	}
}

void NewShowCmds ()
{
	int i;
	char *name;

	G_sprint(self, 2, "Valid commands for %s is:\n", 
				( self->k_spectator ? redtext("spectator") : redtext("player") ));

	for( i = 0; i >= 0 && i < cmds_cnt; i++ ) {

		name = cmds[i].name;

		if ( !isValidCmdForClass( i, self->k_spectator ) )
			continue; // cmd does't valid for this class of player or matchless mode does't have this command

		G_sprint(self, 2, "%s\n", redtext(name));
		
	}
}

void ShowCmds()
{
	// FIXME: complete this some day
	if ( k_matchLess )
	{
		NewShowCmds ();
		return;
	}

	if ( self->k_spectator )
		SShowCmds();
	else 
		PShowCmds();
}

void ShowOpts()
{
	G_sprint(self, 2,
			"��������.. -1 mins match time\n"
			"������.... +1 mins match time\n"
			"��������... -5 mins match time\n"
			"������..... +5 mins match time\n"
			"���������.. -10 fraglimit\n"
			"�������.... +10 fraglimit\n"
			"��......... change deathmatch mode\n"
			"��......... change teamplay mode\n"
			"��������... drop quad when killed\n"
			"��������... drop ring when killed\n"
			"��������... drop pack when killed\n"
			"����....... change locking mode\n"
			"�����...... change spawntype\n"
			"�����...... toggle sv_maxspeed\n"
			"��������... quad, ���, ring & suit\n"
			"�����... 2 secs of ��� on respawn\n"
			"���������.. best/last weapon dropped\n"
			"���������.. underwater discharges\n"
			"�������.... toggle spectator talk\n"
			"�������.... toggle berzerk mode\n");
}

void ShowQizmo()
{
	G_sprint(self, 2,
			"����....... lagsettings\n"
			"������..... enemy vicinity reporting\n"
			"������..... point function\n");
}

// ShowMessages and SendMessage command implementations added
void ShowMessages()
{
	G_sprint(self, 2,
			"������..... who killed you last\n"
			"������..... who you last killed\n"
			"��������... last player joined\n");
}

void ShowVersion()
{
	char buf[2048] = {0};

	strlcat(buf, va("Running %s v.%s build:%s by qqshka\n\n", redtext(MOD_NAME),
				 dig3s(MOD_VERSION), dig3s("%05d", build_number())), sizeof(buf));
	strlcat(buf, va("Based on %s\n", redtext("Kombat teams 2.21")), sizeof(buf));
	strlcat(buf, "by kemiKal, Cenobite, Sturm and Fang\n\n", sizeof(buf));
	strlcat(buf, va("Source at:\n%s", MOD_URL), sizeof(buf));

	G_sprint(self, 2, "%s\n", buf);
}

void ChangeOvertime()
{
	float f1, f2;
//    char *tmp;

	if ( match_in_progress )
		return;

	if( atoi( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

    f1 = bound(0, atoi( ezinfokey( world, "k_overtime" ) ), 2);
    f2 = bound(0, atoi( ezinfokey( world, "k_exttime" ) ), 999);

    if( !f1 )
    {
		if( !f2 ) 
		{
			localcmd("localinfo k_exttime 1\n");
			f2 = 1;
		}

		G_bprint(2, "��������: time based\n");
		G_bprint(2, "�������� ������ �� %d ������%s\n", (int)f2, ( f1== 1 ? "": "�" ));

		localcmd("localinfo k_overtime 1\n");
    }
    else if( f1 == 1 )
	{
		G_bprint(2, "��������: sudden death\n");
		localcmd("localinfo k_overtime 2\n");
	}
    else if( f1 == 2 )
    {
		G_bprint(2, "��������: off\n");
		localcmd("localinfo k_overtime 0\n");
    }
}

void ChangeOvertimeUp ()
{
	float f1;
//	char *tmp;

	if( match_in_progress )
		return;

	if( atoi( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 )
	{
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	f1 = atoi( ezinfokey( world, "k_exttime" ) );

	if ( f1 == 10 )
		f1 = 1;
    else
		f1++;

	G_bprint(2, "�������� ������ ��� �� %d ������%s\n", (int)f1, ( f1==1 ? "" : "�" ) );

	localcmd("localinfo k_exttime %d\n", (int)f1);
}

void ListWhoNot()
{
	gedict_t *p;
//	char *tmp;
	float f1, f2;

	f1 = CountRPlayers();
	f2 = CountPlayers();
	if( f1 == f2 ) {
		G_sprint(self, 2, "All players ready\n");
		return;
	}

	if( !match_in_progress )
    {
		if( streq(self->s.v.classname, "player") && !self->ready ) 
		{
			G_sprint(self, 2, "Ready yourself first\n");
			return;
		}	

		if( g_globalvars.time < k_whonottime + 10 )
		{
            G_sprint(self, 2, "Only one ���� in 10 seconds\n");
			return;
		}
		k_whonottime = g_globalvars.time;
		G_bprint(2, "Players ��� ready:\n");
		G_bprint(3, "������������������\n");

		p = find( world, FOFCLSN, "player" );
		while( p ) {
			if(!strnull( p->s.v.netname ) && !p->ready) {
				G_bprint(2, "%s�%s� %s is not ready\n",
					(p->k_admin == 2 ? "� ": ""), ezinfokey(p, "team"), p->s.v.netname);
			}

			p = find( p, FOFCLSN, "player" );
		}
    }
	else
		G_sprint(self, 2, "Game in progress\n");
}


void SendKillerMsg()
{
	SendMessage(self->killer);
}

void SendVictimMsg()
{
	SendMessage(self->victim);
}

void SendNewcomerMsg()
{
	SendMessage(newcomer);
}

void SendMessage(char *name)
{
	gedict_t *p;

	if ( !strnull( name ) ) {

		p = find( world, FOFCLSN, "player" );
		while( ( p && strneq( p->s.v.netname, name ) ) || p == self )
			p = find( p, FOFCLSN, "player" );
    
		if( p ) {
			G_bprint(3, "%s: %s", self->s.v.netname, ezinfokey(self, "premsg"));
			G_bprint(3, name);
			G_bprint(3,"%s\n", ezinfokey(self, "postmsg"));

			return;
		}
	}

	G_sprint(self, 2, "No name to display\n");
}

void ShowMaps()
{
	float f1;

	G_sprint(self, 2, "Vote for maps by typing the mapname,\nfor example \"���\".\n");

	if ( strnull ( ezinfokey( world, va("%d", (int)LOCALINFO_MAPS_LIST_START) ) ) )
		return;

	G_sprint(self, 2, "\n---���� �� ������ ����\n");

	f1 = LOCALINFO_MAPS_LIST_START;

	for( ; !strnull(ezinfokey(world, va("%d", (int)f1))) && f1 <= LOCALINFO_MAPS_LIST_END; f1++ )
	{
		G_sprint(self, 2, "%s%s",
				ezinfokey( world, va("%d", (int)f1) ), (((int)f1 & 1) ? "\n" : "   "));
	}

	if( (int)f1 & 1 )
		G_sprint(self, 2, "\n");

	G_sprint( self, 2, "---��� �� ����\n" );
}

void PrintToggle1( char *tog, char *key )
{
	int i;

	if ( strnull(tog) || strnull(key) )
		G_Error("PrintToggle1 null");

	G_sprint(self, 2, tog);

	i = streq(key, "k_pow") ? Get_Powerups() : bound(0, iKey( world, key ), 1);

	if( !i )
		G_sprint(self, 2, "Off ");
	else if ( i == 1 )
		G_sprint(self, 2, "On  ");
	else
		G_sprint(self, 2, "Jam ");
}

void PrintToggle2( char *tog, char *key )
{
	if ( strnull(tog) || strnull(key) )
		G_Error("PrintToggle2 null");

	G_sprint(self, 2, tog);

	if( atoi( ezinfokey( world, key ) ) )
		G_sprint(self, 2, "On\n");
	else
		G_sprint(self, 2, "Off\n");
}

//Ceno
void PrintToggle3(char *tog, char *key)
{
	float f1;

	if ( strnull(tog) || strnull(key) )
		G_Error("PrintToggle3 null");

	G_sprint(self, 2, tog);

	f1 = atoi( ezinfokey( world, key ) );

	if( !f1 )
		G_sprint(self, 2, "Off ");
	else if( f1 == 1 )
		G_sprint(self, 2, "On  ");
	else
		G_sprint(self, 2, "Lst ");
}

void ModStatus ()
{
	int votes;
	gedict_t *p;

	G_sprint(self, 2, "%-14s %-3d\n", redtext("Maxspeed"), (int)k_maxspeed);
	G_sprint(self, 2, "%-14s %-3d ",  redtext("Deathmatch"), (int)deathmatch);
	G_sprint(self, 2, "%-14s %-3d\n", redtext("Teamplay"), (int)teamplay);
	G_sprint(self, 2, "%-14s %-3d ",  redtext("Timelimit"), (int)timelimit);
	G_sprint(self, 2, "%-14s %-3d\n", redtext("Fraglimit"), (int)fraglimit);
	PrintToggle1(redtext("Powerups       "), "k_pow");
	PrintToggle2(va("%s \x98\x98\x98    ", redtext("Respawn")), "k_666");
	PrintToggle1(redtext("Drop Quad      "), "dq");
	PrintToggle2(redtext("Drop Ring      "), "dr");
	PrintToggle3(redtext("Fair Backpacks "), "k_frp");
	PrintToggle2(redtext("Drop Backpacks "), "dp");
	PrintToggle1(redtext("Discharge      "), "k_dis");
	PrintToggle2(redtext("Berzerk mode   "), "k_bzk");

	if( match_in_progress == 1 ) {
		p = find(world, FOFCLSN, "timer" );
		G_sprint(self, 2, "The match will start in %d second%s\n",
						 (int)p->cnt2, count_s(p->cnt2));
		return;
	}

	if( votes = get_votes( OV_ELECT ) )
		G_sprint(self, 2, "%s election in progress:\x90%d/%d\x91 vote%s\n",
							 redtext(get_elect_type_str()), votes,
							 get_votes_req( OV_ELECT, false ), count_s(votes));

	if( k_captains == 2 )
		G_sprint(self, 2, "%s in progress\n", redtext("Team picking"));

	if( floor( k_captains ) == 1 ) 
		G_sprint(self, 2, "\x90\x31\x91 %s present\n", redtext("captain"));

	if( match_in_progress == 2 ) {
		if( k_sudden_death )                
			G_sprint(self, 2, "%s overtime in progress\n", redtext("Sudden death"));
		else {
			p = find(world, FOFCLSN, "timer");
			if ( p )
				G_sprint(self, 2, "Match in progress\n"
								  "\x90%s\x91 full minute%s left\n",
									dig3(p->cnt - 1), count_s(p->cnt));
		}
	}
}

void ModStatus2()
{
	float f1, f2;
	char *ot = "";

	f1 = atoi( ezinfokey( world, "k_spw" ) );
	if( f1 == 2 )
		G_sprint(self, 2, redtext("Kombat Teams Respawns\n"));
	else if ( f1 == 1 )
		G_sprint(self, 2, redtext("KT SpawnSafety\n"));
	else
		G_sprint(self, 2, redtext("Normal QW Respawns\n"));

	if( isDuel() )
		G_sprint(self, 2, "%s: duel\n", redtext("Server mode"));
	else if ( isFFA() )
		G_sprint(self, 2, "%s:  FFA\n", redtext("Server mode"));
	else if ( isTeam() ) {
		G_sprint(self, 2, "%s: team\n", redtext("Server mode"));
		G_sprint(self, 2, "%s: %s\n", redtext("Server locking"),
					(!lock ? "off" : (lock == 2 ? "all" : (lock == 1 ? "team" : "unknown"))));
	}
	else
		G_sprint(self, 2, "%s: unknown\n", redtext("Server mode"));

	if( !match_in_progress ) {
		G_sprint(self, 2, "�������� (���: %d", (int)CountRTeams());
		G_sprint(self, 2, " ���: %s",    ezinfokey(world, "k_lockmin"));
		G_sprint(self, 2, " ���: %s)\n", ezinfokey(world, "k_lockmax"));
	}

	G_sprint(self, 2, "��������: %s\n", ( atoi(ezinfokey(world, "k_spectalk")) ? "on" : "off" ));

	f1 = atoi( ezinfokey( world, "k_overtime" ) );
	f2 = atoi( ezinfokey( world, "k_exttime" ) );
	
	switch ( (int)f1) {
		case 0:  ot = "off"; break;
		case 1:  ot = va("%d minute%s", (int)f2, (f2 != 1 ? "s" : "")); break;
		case 2:  ot = "sudden death"; break;
		default: ot	= "unknown"; break;
	}
	G_sprint(self, 2, "%s: %s\n", redtext("Overtime"), ot);

	f1 = atoi( ezinfokey( world, "fpd" ) );

	G_sprint(self, 2, "����� ���: %s\n",             ( (int)f1 &   8 ? "off" : "on" ));
	G_sprint(self, 2, "����� ������: %s\n",          ( (int)f1 &   2 ? "off" : "on" ));
	G_sprint(self, 2, "����� ����� ���������: %s\n", ( (int)f1 &  32 ? "off" : "on" ));
	G_sprint(self, 2, "����� ��������: %s\n",        ( (int)f1 & 128 ? "off" : "on" ));

	f1 = atoi( ezinfokey( world, "k_deathmsg" ) );
	G_sprint(self, 2, "������ �������������: %s\n", ( f1 ? "on" : "off" ));

	f1 = atoi(ezinfokey(world, "k_allowvoteadmin"));
	G_sprint(self, 2, "����� ��������: %s\n", ( f1 ? "allowed" : "disallowed" ));

	G_sprint(self, 2, "����� ����������: %s\n", ( framechecks ? "enabled" : "disabled" ));
}

void ModStatusVote()
{
	qboolean voted = false;
	int votes, i;
	int from;
	gedict_t *p;

	if ( k_matchLess || !match_in_progress )
	if ( vote_get_maps () >= 0 ) {
		voted = true;

		G_sprint(self, 2, "%s:\n", redtext("Map voting"));
		
		for( i = 0; i < MAX_CLIENTS; i++) {
			if (!maps_voted[i].map_id)
				break;

			G_sprint(self, 2, "\x90%s\x91 %2d vote%s\n", GetMapName( maps_voted[i].map_id ),
								maps_voted[i].map_votes, count_s(maps_voted[i].map_votes) );

			for( from = 0, p = world; p = find_plrspc(p, &from); )
				if ( p->v.map == maps_voted[i].map_id )
					G_sprint(self, 2, " %s\n", p->s.v.netname);
		}
	}

	if( !(get_elect_type() == etCaptain && match_in_progress) ) // does't show captain ellection in game
	if( votes = get_votes( OV_ELECT ) ) {
		voted = true;

		G_sprint(self, 2, "\x90%d/%d\x91 vote%s for %s election:\n", votes, 
			get_votes_req( OV_ELECT, false ), count_s(votes), redtext(get_elect_type_str()) );

		for( from = 0, p = world; p = find_plrspc(p, &from); )
			if ( p->v.elect )
				G_sprint(self, 2, "%s%s\n", 
				((p->k_admin == 1.5 || p->k_captain > 10) ? "\x87" : " " ), p->s.v.netname);
	}

	if ( !match_in_progress )
	if ( votes = get_votes( OV_PICKUP )) {
		voted = true;

		G_sprint(self, 2, "\x90%d/%d\x91 vote%s for a %s game:\n", votes,
			 get_votes_req( OV_PICKUP, false ), count_s(votes), redtext("pickup"));

		for( from = 0, p = world; p = find_plrspc(p, &from); )
			if ( p->v.pickup )
				G_sprint(self, 2, " %s\n", p->s.v.netname);
	}

	if ( !match_in_progress )
	if ( votes = get_votes( OV_RPICKUP )) {
		voted = true;

		G_sprint(self, 2, "\x90%d/%d\x91 vote%s for a %s game:\n", votes,
			 get_votes_req( OV_RPICKUP, false ), count_s(votes), redtext("rpickup"));

		for( from = 0, p = world; p = find_plrspc(p, &from); )
			if ( p->v.rpickup )
				G_sprint(self, 2, " %s\n", p->s.v.netname);
	}

	if( k_matchLess || match_in_progress == 2 )
	if ( votes = get_votes( OV_BREAK ) ) {
		voted = true;

		G_sprint(self, 2, "\x90%d/%d\x91 vote%s for stopping:\n", votes,
			get_votes_req( OV_BREAK, false ), count_s(votes));

		for( from = 0, p = world; p = find_plrspc(p, &from); )
			if ( p->v.brk )
				G_sprint(self, 2, " %s\n", p->s.v.netname);
	}

	if ( voted )
		G_sprint(self, 2, "%s\n", redtext("--------------"));
	else
		G_sprint(self, 2, "%s\n", redtext("No election going on"));
}

void PlayerStatus()
{
	gedict_t *p;
	char *tmp;

	if( !match_in_progress ) {
		p = find( world, FOFCLSN, "player" );
		while( p ) {
			if( !strnull( p->s.v.netname ) ) {
				if( p->k_admin == 2 )
					G_sprint(self, 2, "* ");

				G_sprint(self, 2, p->s.v.netname);

				if(p->ready) {
					tmp = ezinfokey( p, "team" );
					if( strnull( tmp ) )
						G_sprint(self, 2, " is ready\n");
					else
						G_sprint(self, 2, " is in team \x90%s\x91\n", tmp);
				}
				else
					G_sprint(self, 2, " is not ready\n");
			}

			p = find( p, FOFCLSN, "player" );
		}
		G_sprint(self, 2, "--------------\n");
	} else
		G_sprint(self, 2, "Game in progress\n");
}

void PlayerStatusS()
{
	gedict_t *p;
//	char *tmp;

	p = find( world, FOFCLSN, "player" );
	while( p ) {
		if( !strnull( p->s.v.netname ) )
			G_sprint(self, 2, "�%s� %s\n", ezinfokey(p, "skin"), p->s.v.netname);

		p = find( p, FOFCLSN, "player" );
	}
	G_sprint(self, 2, "--------------\n");
}

void PlayerStatusN()
{
	gedict_t *p;
	float f1, f2;

	f1 = CountRPlayers();
	f2 = CountPlayers();
	if( f1 == f2 ) {
		G_sprint(self, 2, "All players ready\n");
		return;
	}

	if( match_in_progress )
		G_sprint(self, 2, "Game in progress\n");
	else {
		G_sprint(self, 2, "Players ��� ready:\n������������������\n");
		p = find( world, FOFCLSN, "player" );
		while( p ) {
			if( !strnull( p->s.v.netname ) && !p->ready ) {
				if( p->k_admin == 2 )
					G_sprint(self, 2, "� ");
				G_sprint(self, 2, "%s is not ready\n", p->s.v.netname);
			}

			p = find( p, FOFCLSN, "player" );
		}
	}
}

void ResetOptions()
{
	char *s1;

	if( match_in_progress )
		return;

	if(atoi( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	s1 = ezinfokey(self, "team");

#if 1 // TODO: make commented code safe or remove it
		localcmd("exec configs/reset.cfg\n");
#else
/*
	if( self->k_admin != 2 || strnull( s1 ) ) {
		localcmd("exec configs/reset.cfg\n");
	} else {
		localcmd("exec configs/%s.cfg\n", s1); // FIXME: UNSAFE: player can set some dangerous team
		G_bprint(2, "*** \"%s\" server setup by %s\n", s1, self->s.v.netname);
	}
*/
#endif
}

void VotePickup()
{
	int votes;

	if( match_in_progress )
		return;

	if( k_captains ) {
		G_sprint(self, 2, "No pickup when captain stuffing\n");
		return;
	}

	if( atoi( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 2, "console: command is locked\n");
		return;
	}

	self->v.pickup = !self->v.pickup;

	G_bprint(2, "%s %s %s%s\n", self->s.v.netname, 
					redtext("says"), (self->v.pickup ? "pickup!" : "no pickup"),
					((votes = get_votes_req( OV_PICKUP, true )) ? va(" (%d)", votes) : ""));

	vote_check_pickup ();
}

void ReportMe()
{
	gedict_t *p;
	char *t1, *t2 , *wt, *pa1, *pa2;
	float f1, flag = 0;

	if( !strnull( ezinfokey(self, "k_nick") ) || !strnull( ezinfokey(self, "k") ) )
		flag = 1;

	pa1 = "";
	pa2 = ": ";

	wt = "axe:";
	f1 = 0;

	if( (int)self->s.v.items & 1 ) {
		wt = "sg:";
		f1 = self->s.v.ammo_shells;
	}
	if( (int)self->s.v.items & 4) {
		wt = "ng:";
		f1 = self->s.v.ammo_nails;
	}
	if( (int)self->s.v.items & 2) {
		wt = "ssg:";
		f1 = self->s.v.ammo_shells;
	}
	if( (int)self->s.v.items & 8) {
		wt = "sng:";
		f1 = self->s.v.ammo_nails;
	}
	if( (int)self->s.v.items & 16) {
		wt = "gl:";
		f1 = self->s.v.ammo_rockets;
	}
	if( (int)self->s.v.items & 64) {
		wt = "lg:";
		f1 = self->s.v.ammo_cells;
	}
	if( (int)self->s.v.items & 32) {
		wt = "rl:";
		f1 = self->s.v.ammo_rockets;
	}

    p = find( world, FOFCLSN, "player" );
	while( p ) {
		if( !strnull( p->s.v.netname ) ) {
			t1 = ezinfokey(self, "team");
			t2 = ezinfokey(p, "team");

			if( streq( t1, t2 ) ) {
				if( flag ) {
					t1 = ezinfokey(self, "k_nick");

					if ( strnull( t1 ) )
						t1 = ezinfokey(self, "k");

					G_sprint(p, 3, "%s: ", t1);
				}
				else
					G_sprint(p, 3, "%s%s%s", pa1, self->s.v.netname, pa2);

				if( self->s.v.armorvalue )
					G_sprint(p, 3, "%s:%d", armor_type((int)self->s.v.items),
									 (int)self->s.v.armorvalue);
				else
					G_sprint(p, 3, "a:0");

				G_sprint(p, 3, "  h:%d  %s%d", (int)self->s.v.health, wt, (int)f1);

				if( (int)self->s.v.items & 524288)
					G_sprint(p, 3, "  ������");
				if( (int)self->s.v.items & 1048576)
					G_sprint(p, 3, "  �������");
				if( (int)self->s.v.items & 4194304)
					G_sprint(p, 3, "  �����䜑");

				G_sprint(p, 3, "\n");
			}
		}

		p = find( p, FOFCLSN, "player" );
	}
}

void ToggleRespawns()
{
	float tmp;

	if ( match_in_progress )
		return;

	if( atoi( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	tmp = atoi( ezinfokey( world, "k_spw" ) );

	if( tmp == 2 ) {
		G_bprint(2, "Normal QW respawns (avoid spawnfrags)\n");
		localcmd("localinfo k_spw 0\n");
	} else if( tmp == 0 ) {
		G_bprint(2, "KT SpawnSafety\n");
		localcmd("localinfo k_spw 1\n");
	} else {
		G_bprint(2, "Kombat Teams respawns\n");
		localcmd("localinfo k_spw 2\n");
	}
}

void ToggleRespawn666()
{
	float tmp;

	if( match_in_progress )
		return;

	if( atoi( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	tmp = atoi( ezinfokey( world, "k_666" ) );

	G_bprint(2, "������� ��� ");

	if( tmp != 0 ) {
		G_bprint(2, "disabled\n");
		localcmd("localinfo k_666 0\n");
	} else {
		G_bprint(2, "enabled\n");
		localcmd("localinfo k_666 1\n");
	}
}

void TogglePowerups()
{
	float tmp;

	if ( match_in_progress )
		return;

	if ( atoi( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	tmp = iKey( world, "k_pow" ); // here we are not using Get_Powerups

	G_bprint(2, "�������� ");

	if( tmp == 2 ) {
		G_bprint(2, "disabled\n");
		localcmd("localinfo k_pow 0\n");
	} else if( !tmp ) {
		G_bprint(2, "enabled\n");
		localcmd("localinfo k_pow 1\n");
	} else {
		G_bprint(2, "enabled (timer jammer)\n");
		localcmd("localinfo k_pow 2\n");
	}
}

void ToggleDischarge()
{
	float tmp;

	if( match_in_progress )
		return;
	if( atoi( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	tmp = atoi( ezinfokey( world, "k_dis" ) );

	G_bprint(2, "���������� ");

	if( tmp != 0 ) {
		G_bprint(2, "disabled\n");
		localcmd("localinfo k_dis 0\n");
	} else {
		G_bprint(2, "enabled\n");
		localcmd("localinfo k_dis 1\n");
	}
}

#define DMM_NUM		((char)(146) + (char)(deathmatch))

void ShowDMM()
{
	G_sprint(self, 2, "Deathmatch %c\n", DMM_NUM);
}

void ChangeDM(float dmm)
{
	if ( match_in_progress )
		return;

	if ( atoi ( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	if ( deathmatch == (int)dmm ) {
		G_sprint(self, 2, "%s%c already set\n", redtext("dmm"), DMM_NUM);
		return;
	}

	deathmatch = bound(1, (int)dmm, 5);

	cvar_set("deathmatch", va("%d", (int)deathmatch));

	G_bprint(2, "Deathmatch %c\n", DMM_NUM);
}

void ChangeTP()
{
	char *tmp;

	if ( match_in_progress )
		return;

	if ( atoi ( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	if( !isTeam() ) {
		G_sprint(self, 3, "console: non team mode disallows you to change teamplay setting\n");
		return;
	}

	teamplay = bound (1, teamplay, 3);

	teamplay++;

	if ( teamplay == 4 )
		teamplay = 1;

	cvar_set("teamplay", va("%d", (int)teamplay));
	if( teamplay == 1 ) tmp = "�";
	else if( teamplay == 2 ) tmp = "�";
	else tmp = "�";

	G_bprint(2, "Teamplay %s\n", tmp);
}

void TimeDown(float t)
{
//	char *tmp;

	if ( match_in_progress )
		return;
	if ( iKey( world, "k_master" ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	timelimit = timelimit - t;

	if ( timelimit < 3 )
		timelimit = 3;

	cvar_set("timelimit", va("%d", (int)timelimit));

	G_bprint(2, "����� ������ ��� �� %d �������\n", (int)timelimit);
}

void TimeUp(float t)
{
	float top=0.0;

	if ( match_in_progress )
		return;
	if ( iKey( world, "k_master" ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	timelimit = timelimit + t;
	top = iKey( world, "k_timetop" );

	if( timelimit > top )
		timelimit = top;

	cvar_set("timelimit", va("%d", (int)timelimit));

	G_bprint(2, "����� ������ ��� �� %d �������\n", (int)timelimit);
}

void TimeSet(float t)
{
	float top;

	if ( match_in_progress )
		return;
	if ( atoi ( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	timelimit = t;

	top = iKey( world, "k_timetop" );
	if( timelimit > top )
		timelimit = top;

	cvar_set("timelimit", va("%d", (int)timelimit));

	G_bprint(2, "����� ������ ��� �� %d �������\n", (int)timelimit);
}

void FragsDown()
{
	if ( match_in_progress )
		return;
	if ( atoi ( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	fraglimit -= 10;

	if( fraglimit < 0 )
		fraglimit = 0;

	cvar_set("fraglimit", va("%d", (int)(fraglimit)));
	G_bprint(2, "��������� ��� �� %d\n", (int)(fraglimit));
}

void FragsUp()
{
//	char *tmp;

	if ( match_in_progress )
		return;
	if ( atoi ( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	fraglimit += 10;

	if(fraglimit > 100)
		fraglimit = 100;

	cvar_set("fraglimit", va("%d", (int)(fraglimit)));
	G_bprint(2, "��������� ��� �� %d\n", (int)(fraglimit));
}

void ToggleDropQuad()
{
	float dq;

	if ( match_in_progress )
		return;
	if ( atoi ( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	dq = atoi( ezinfokey( world, "dq" ) );

	if(dq != 0) {
		localcmd("localinfo dq 0\n");
		G_bprint(2, "�������� off\n");
		return;
	}
	localcmd("localinfo dq 1\n");
	G_bprint(2, "�������� on\n");
}

void ToggleDropRing()
{
	float dr;

	if ( match_in_progress )
		return;
	if ( atoi ( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	dr = atoi( ezinfokey( world, "dr" ) );

	if( dr ) {
		localcmd("localinfo dr 0\n");
		G_bprint(2, "�������� off\n");
		return;
	}
	localcmd("localinfo dr 1\n");
	G_bprint(2, "�������� on\n");
}

void ToggleDropPack()
{
	float dp;

	if ( match_in_progress )
		return;
	if ( atoi ( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	dp = atoi( ezinfokey( world, "dp" ) );

	if(dp != 0) {
		localcmd("localinfo dp 0\n");
		G_bprint(2, "��������� off\n");
		return;
	}
	localcmd("localinfo dp 1\n");
	G_bprint(2, "��������� on\n");
}

void ToggleFairPacks()
{
	float f1;

    if ( match_in_progress )
		return;
	if ( atoi ( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	f1 = atoi( ezinfokey( world, "k_frp" ) );

	if( !f1)
	{
		localcmd("localinfo k_frp 1\n");
		G_bprint(2, "��������� enabled - drop best weapon.\n");
		return;
	}
	else if( f1==1 )
	{
		localcmd("localinfo k_frp 2\n");
		G_bprint(2, "��������� enabled - drop last fired weapon.\n");
		return;
	}
	else if( f1==2 )
	{
		localcmd("localinfo k_frp 0\n");
		G_bprint(2, "��������� disabled\n");
		return;
	}
}

void ToggleSpeed()
{
//	char *s1;
	gedict_t *p;

	if ( match_in_progress )
		return;
	if ( atoi ( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	if( k_maxspeed != 320 )
		k_maxspeed = 320;
	else
		k_maxspeed = atoi( ezinfokey( world, "k_highspeed" ) );

	G_bprint(2, "�������� ��� �� %d\n", (int)k_maxspeed);
	cvar_set("sv_maxspeed", va("%d", (int)k_maxspeed));

	p = find( world, FOFCLSN, "player" );
	while( p ) {
		p->maxspeed = k_maxspeed;
		p = find( p, FOFCLSN, "player" );
	}
}

void ToggleBerzerk()
{
	float tmp;

	if ( match_in_progress )
		return;
	if ( atoi ( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	tmp = atoi( ezinfokey( world, "k_bzk" ) );

	if( tmp != 0 ) {
		localcmd("localinfo k_bzk 0\n");
		G_bprint(2, "������� ���� off\n");
		return;
	}
	localcmd("localinfo k_bzk 1\n");
	G_bprint(2, "������� ���� on\n");
}


void ToggleSpecTalk()
{
	float tmp, tmp2;
//	char *s1;

	if ( match_in_progress && self->k_admin < 2 )
		return;

	tmp2 = atoi( ezinfokey( world, "fpd" ) );

	tmp2 = tmp2 - ((int)tmp2 & 64);

	if( match_in_progress == 2 ) {
		tmp = atoi( ezinfokey( world, "k_spectalk" ) );
		G_bprint(2, "Spectalk ");

		if(tmp != 0) {
			tmp2 = tmp2 + 64;

			localcmd("sv_spectalk 0\n");
			localcmd("serverinfo fpd %d\n", (int)tmp2);
			localcmd("localinfo k_spectalk 0\n");
			G_bprint(2, "off: ������� ��� �� ������ ���� ����������\n");
			return;
		}
		localcmd("sv_spectalk 1\n");
		localcmd("serverinfo fpd %d\n", (int)tmp2);
		localcmd("localinfo k_spectalk 1\n");
		G_bprint(2, "on: ������� ��� ��� ���� ����������\n");
		return;
	} else {
		if ( atoi ( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
			G_sprint(self, 3, "console: command is locked\n");
			return;
		}

		tmp = atoi( ezinfokey( world, "k_spectalk" ) );
		G_bprint(2, "Spectalk ");

		if(tmp != 0) {
			localcmd("localinfo k_spectalk 0\n");
			G_bprint(2, "off: ������� ������ ���� ���������� ������ ����\n");
			return;
		}
		localcmd("localinfo k_spectalk 1\n");
		G_bprint(2, "on: ������� ��� ���� ���������� ������ ����\n");
	}
}

void ShowRules()
{
	if( isDuel() )
		G_sprint(self, 2, "Server is in duel mode.\n");
	else if ( isFFA() )
		G_sprint(self, 2, "Server is in FFA mode.\n");
	else if ( isTeam() )
		G_sprint(self, 2,
			"Bind ������ to a key.\n"
			"Pressing that key will send\n"
			"your status to your teammates.\n"
			"Typing ������ during game\n"
			"will print time left and teamscores.\n"
			"Also available during game\n"
			"are ����� and ����������.\n\n"
			"Remember that telefragging a teammate\n"
			"does not result in frags.\n");
	else
		G_sprint(self, 2, "Server is in unknown mode.\n");

	if( atoi( ezinfokey( world, "k_bzk" ) ) != 0 )
		G_sprint(self, 2,
			"\nBERZERK mode is activated!\n"
			"This means that when only %d seconds\n"
			"remains of the game, all players\n"
			"gets QUAD/OCTA powered.\n", atoi( ezinfokey(world, "k_btime") ) );

	G_sprint(self, 2, "\n");
}

void ChangeLock()
{
	if ( match_in_progress )
		return;
	if ( atoi ( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	lock++;

    if( lock > 2 )
		lock = 0;
	if( lock == 0 )
		G_bprint(2, "������ ������� off\n");
	else if( lock == 2 )
		G_bprint(2, "������ ������ - players cannot connect during game\n");
	else if( lock == 1 )
		G_bprint(2, "�������� �� - only players in existing teams can connect during game\n");
}

void TeamSay(float fsndname)
{
	gedict_t *p;
	char *sndname = va("ktsound%d.wav", (int)fsndname);

    p = find( world, FOFCLSN, "player" );
	while( p ) {
		if( p != self && teamplay && !strnull( p->s.v.netname ) &&
			 ( atoi( ezinfokey( p, "kf" ) ) & 1 ) ) {
			if( streq( ezinfokey(self, "team"), ezinfokey(p, "team") ) ) {
				char *t1 = ezinfokey(p, "k_sdir");
				stuffcmd(p, "play %s%s\n", (strnull( t1 ) ? "" : va("%s/", t1)), sndname);
			}
		}

		p = find( p, FOFCLSN, "player" );
	}
}

void PrintScores()
{
	int minutes, seconds;
	gedict_t *p;

	if( match_in_progress != 2 ) {
		G_sprint(self, 2, "no game - no scores.\n");
		return;
	}

	if( k_sudden_death )                
		G_sprint(self, 2, "Sudden death %s\n", redtext("overtime in progress"));

	if( p = find(world, FOFCLSN, "timer") ) {
		minutes = p->cnt;
		seconds = p->cnt2;
		if( seconds == 60 )
			seconds = 0;
		else
			minutes--;

		// we can't use dig3 here because of zero padding, so using dig3s
		G_sprint(self, 2, "\x90%s:%s\x91 remaining\n", 
							dig3s("%02d", minutes), dig3s("%02d", seconds));
	}

	if( k_showscores ) {
		int s1 = get_scores1();
		int s2 = get_scores2();
		char *t1 = ezinfokey(world, "k_team1");
		char *t2 = ezinfokey(world, "k_team2");

		G_sprint(self, 2, "%s \x90%s\x91 = %s\n", redtext("Team"),
								 (s1 > s2 ? t1 : t2), dig3(s1 > s2 ? s1 : s2));
		G_sprint(self, 2, "%s \x90%s\x91 = %s\n", redtext("Team"),
								 (s1 > s2 ? t2 : t1), dig3(s1 > s2 ? s2 : s1));
	}
}


void PlayerStats()
{
	float f1;
	gedict_t *p, *p2;
	char *tmp, *tmp2;

	if( match_in_progress != 2 ) {
		G_sprint(self, 2, "no game - no statistics.\n");
		return;
	}

	f1 = CountTeams();
	G_sprint(self, 2, "������ ����������:\n"
					  "����� (����) %s� ����������\n", (isTeam() ? "����������� " : ""));

	G_sprint(self, 2, "������������������������%s�\n", (isTeam() ? "������������" : ""));

	p = find( world, FOFCLSN, "player" );
	while( p ) {
		if( !strnull( p->s.v.netname ) && p->k_accepted == 2 && p->ready ) {
			p2 = p;

			while ( p2 ) {
				tmp  = ezinfokey(p, "team");
				tmp2 = ezinfokey(p2, "team");

				if( streq( tmp, tmp2 ) ) {
					G_sprint(self, 2, "�%s� %s:  %d(%d) ", tmp2, p2->s.v.netname,
						(int)p2->s.v.frags, (int)(p2->s.v.frags - p2->deaths));

					if( isTeam() )
						G_sprint(self, 2, "%d ", (int)p2->friendly);

					if(p2->s.v.frags < 1)
						p2->efficiency = 0;
					else
						p2->efficiency = p2->s.v.frags / (p2->s.v.frags + p2->deaths) * 100;

					if( floor( p2->efficiency ) == p2->efficiency)
							G_sprint(self, 2, "� ");
					else
						G_sprint(self, 2, "�");
					G_sprint(self, 2, "%3.1f%%\n", p2->efficiency);

					p2->ready = 0;
				}

				p2 = find(p2, FOFCLSN, "player");
			}
		}

		p = find( p, FOFCLSN, "player" );
	}

	p = find( world, FOFCLSN, "player" );
	while( p ) {
		p->ready = 1;

		p = find( p, FOFCLSN, "player" );
	}
}

void ToggleQLag()
{
	float f1, f2;
//	char *s1;

	if ( match_in_progress )
		return;
	if ( atoi ( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	f1 = atoi( ezinfokey( world, "fpd" ) );

	f2 = f1 - ((int)f1 & 8);

	if((int)f1 & 8) {
		localcmd("serverinfo fpd %d\n", (int)f2);
		G_bprint(2, "����� ��� �������� in effect\n");

		return;
	} else {
		f2 = (int)f2 | 8;
		localcmd("serverinfo fpd %d\n", (int)f2);
		G_bprint(2, "����� ��� �������� not in effect\n");
	}
}

void ToggleQEnemy()
{
	float f1, f2;
//	char *s1;

	if ( match_in_progress )
		return;
	if ( atoi ( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	f1 = atoi( ezinfokey( world, "fpd" ) );

	f2 = f1 - ((int)f1 & 32);

	if( (int)f1 & 32) {
		localcmd("serverinfo fpd %d\n", (int)f2);
		G_bprint(2, "����� ����� ��������� allowed\n");

		return;
	} else {
		f2 = (int)f2 | 32;
		localcmd("serverinfo fpd %d\n", (int)f2);
		G_bprint(2, "����� ����� ��������� disallowed\n");
	}
}

void ToggleQPoint()
{
	float f1, f2;
//	char *s1;

	if ( match_in_progress )
		return;
	if ( atoi ( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	f1 = atoi( ezinfokey( world, "fpd" ) );

	f2 = f1 - ((int)f1 & 128);

	if( (int)f1 & 128 ) {
		localcmd("serverinfo fpd %d\n", (int)f2);
		G_bprint(2, "����� �������� enabled\n");

		return;
	} else {
		f2 =  (int)f2 | 128;
		localcmd("serverinfo fpd %d\n", (int)f2);
		G_bprint(2, "����� �������� disabled\n");
	}
}

void ToggleFreeze()
{
	float f1;

	if ( match_in_progress )
		return;
	if( atoi( ezinfokey( world, "k_master" ) ) && self->k_admin != 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	f1 = atoi( ezinfokey( world, "k_freeze" ) );

	if( f1 ) {
		localcmd("localinfo k_freeze 0\n");
		G_bprint(2, "%s unfreezes map\n", self->s.v.netname);
	} else {
		localcmd("localinfo k_freeze 1\n");
		G_bprint(2, "%s freezes map\n", self->s.v.netname);
	}
}

// qqshka: pointing code stolen from Zquake

void ShowNick()
{
	gedict_t	*p, *bp = NULL;
	char		*s1, *s2, *pups, *kn, buf[256] = {0};
	float		best;
	vec3_t		forward, right, up;
	vec3_t		ang;
	vec3_t		vieworg, entorg;
	int			itms, i;

	if ( !teamplay )
		return;

	ang[0] = self->s.v.v_angle[0];
	ang[1] = self->s.v.v_angle[1];
	ang[2] = 0;

	AngleVectors (ang, forward, right, up);

	VectorCopy (self->s.v.origin, vieworg);
	vieworg[2] += 16 /* was 22 */;	// adjust for view height

	best = -1;

	s1 = ezinfokey ( self, "team" );

	p = find(world, FOFCLSN, "player");

	for ( ; p ; p = find(p, FOFCLSN, "player") )
	{
		vec3_t	v, v2, v3;
		float dist, miss, rank;

		if ( ISDEAD(p) )
			continue; // ignore dead

		if ( p == self )
			continue; // ignore self

		if ( strnull( p->s.v.netname ) || p->k_accepted != 2 )
			continue; // ignore not really players

		if ( p->s.v.modelindex != modelindex_player )
			continue; // ignore non player mdl index (EYES)

		s2 = ezinfokey ( p, "team" );

		if ( strneq( s1, s2 ) )
			continue; // ignore non teammaters


		VectorCopy (p->s.v.origin, entorg);
		entorg[2] += 30;
		VectorSubtract (entorg, vieworg, v);

		dist = DotProduct (v, forward);
		if ( dist < 10 )
			continue;

		VectorScale (forward, dist, v2);
		VectorSubtract (v2, v, v3);
		miss = VectorLength (v3);
		if (miss > 300)
			continue;
		if (miss > dist*1.7)
			continue;		// over 60 degrees off
		if (dist < 3000.0/8.0)
			rank = miss * (dist*8.0*0.0002f + 0.3f);
		else
			rank = miss;
		
		if (rank < best || best < 0) {
			// check if we can actually see the object
			vec3_t	end;
			float	radius;

			radius = 27;
			if ((int)p->s.v.effects & (EF_BLUE|EF_RED|EF_DIMLIGHT|EF_BRIGHTLIGHT))
				radius = 200;

			if (dist <= radius)
				goto ok;

			VectorSubtract (vieworg, entorg, v);
			VectorNormalize (v);
			VectorMA (entorg, radius, v, end);
			traceline( PASSVEC3( vieworg ), PASSVEC3( end ), true, self );
			if ( g_globalvars.trace_fraction == 1 )
				goto ok;

			VectorMA (entorg, radius, right, end);
			VectorSubtract (vieworg, end, v);
			VectorNormalize (v);
			VectorMA (end, radius, v, end);
			traceline( PASSVEC3( vieworg ), PASSVEC3( end ), true, self );
			if ( g_globalvars.trace_fraction == 1 )
				goto ok;

			VectorMA (entorg, -radius, right, end);
			VectorSubtract (vieworg, end, v);
			VectorNormalize (v);
			VectorMA (end, radius, v, end);
			traceline( PASSVEC3( vieworg ), PASSVEC3( end ), true, self );
			if ( g_globalvars.trace_fraction == 1 )
				goto ok;

			VectorMA (entorg, radius, up, end);
			VectorSubtract (vieworg, end, v);
			VectorNormalize (v);
			VectorMA (end, radius, v, end);
			traceline( PASSVEC3( vieworg ), PASSVEC3( end ), true, self );
			if ( g_globalvars.trace_fraction == 1 )
				goto ok;

			// use half the radius, otherwise it's possible to see
			// through floor in some places
			VectorMA (entorg, -radius/2, up, end);
			VectorSubtract (vieworg, end, v);
			VectorNormalize (v);
			VectorMA (end, radius, v, end);
			traceline( PASSVEC3( vieworg ), PASSVEC3( end ), true, self );
			if ( g_globalvars.trace_fraction == 1 )
				goto ok;


			continue;	// not visible
ok:
			best = rank;
			bp = p;
		}
	}

	if ( best < 0 || !bp )
		return;

	itms = (int)bp->s.v.items;

	pups = "";

	if ( bp->invincible_finished >= g_globalvars.time )
		pups = va("%s\x90%s\x91", ( *pups ? va("%s ", pups) : ""), redtext("Pent"));
	if ( bp->super_damage_finished > g_globalvars.time )
		pups = va("%s\x90%s\x91", ( *pups ? va("%s ", pups) : ""), redtext("Quad"));
	if ( bp->radsuit_finished > g_globalvars.time )
		pups = va("%s\x90%s\x91", ( *pups ? va("%s ", pups) : ""), redtext("Suit"));

	if ( *pups )
		pups = va("%s\n", pups);

	if ((int)bp->s.v.armorvalue)
		s1 = va("%s:%d ", redtext( armor_type( itms ) ), (int)bp->s.v.armorvalue);
	else
		s1 = "";

	if      ( itms & IT_ROCKET_LAUNCHER )  s2 = va(" %s:%d", redtext("rl"), (int)bp->s.v.ammo_rockets);
	else if ( itms & IT_LIGHTNING )        s2 = va(" %s:%d", redtext("lg"), (int)bp->s.v.ammo_cells);
	else if ( itms & IT_GRENADE_LAUNCHER ) s2 = va(" %s:%d", redtext("gl"), (int)bp->s.v.ammo_rockets);
	else if ( itms & IT_SUPER_NAILGUN )    s2 = va(" %s:%d", redtext("sng"),(int)bp->s.v.ammo_nails);
	else if ( itms & IT_SUPER_SHOTGUN )    s2 = va(" %s:%d", redtext("ssg"),(int)bp->s.v.ammo_shells);
	else if ( itms & IT_NAILGUN )          s2 = va(" %s:%d", redtext("ng"), (int)bp->s.v.ammo_nails);
	else if ( itms & IT_SHOTGUN )          s2 = va(" %s:%d", redtext("sg"), (int)bp->s.v.ammo_shells);
	else if ( itms & IT_AXE )              s2 = redtext(" axe!"); // just in case :]
	else
		s2 = "";

	kn = ezinfokey( bp, "k_nick" );
	if ( strnull( kn ) )
		kn = ezinfokey( bp, "k" );

	if ( (i = iKey( self, "ln" )) > 0 ) {
		i = bound(0, i, sizeof(buf)-1 );
		memset( (void*)buf, (int)'\n', i);
		buf[i] = 0;
	}

	strlcat(buf, va(	"%s" // if powerups present \n is too
					"%s%s:%d%s\n"
						"%s\n" , pups,
						 s1, redtext("h"), (int)bp->s.v.health, s2,
								( strnull( kn ) ? bp->s.v.netname : kn )), sizeof(buf));

	if ( (i = iKey( self, "ln" )) < 0 ) {
		int offset = strlen(buf);
		i = bound(0, -i, (int)sizeof(buf) - offset - 1);
		memset( (void*)(buf + offset), (int)'\n', i);
		buf[i+offset] = 0;
	}

	G_centerprint(self, "%s", buf);

	self->need_clearCP  = 1;
	self->shownick_time = g_globalvars.time + 0.8; // clear centerprint at this time
}

// qqshka

// below predefined settings for usermodes
// I ripped this from ktpro

// common settings for all user modes
const char common_um_init[] =
	"k_disallow_weapons 16\n"			// disallow gl in dmm4 by default

	"localinfo k_new_mode 0\n" 			// UNKNOWN ktpro
	"localinfo k_fast_mode 0\n"			// UNKNOWN ktpro
	"localinfo matrix 0\n"              // UNKNOWN ktpro
	"localinfo k_safe_rj 0\n"           // UNKNOWN ktpro

	"localinfo spec_info 1\n"			// TODO not implemented yet
										// allow spectators receive took info during game
                                   		// (they have to use "moreinfo" command to set proper level)
	"localinfo spec_info_notlock 1\n"	// allow all spectators receive it (0 = only admins)
	"localinfo k_midair 0\n"			// not implemented
	"localinfo k_instagib 0\n"			// not implemented

	"fraglimit 0\n"						// fraglimit %)
	"localinfo k_666 0\n"				// respawn 666
	"localinfo dp 1\n"					// drop pack
	"localinfo dq 0\n"					// drop quad
	"localinfo dr 0\n"					// drop ring
	"localinfo k_frp 0\n"				// fairpacks
	"localinfo k_spectalk 0\n"			// silence
	"localinfo k_dis 1\n"				// discharge on
	"localinfo k_lockmin 2\n"			// minimum number of teams in game
	"localinfo k_bzk 0\n"				// berzerk
	"localinfo k_spw 0\n"				// affect spawn type
	"localinfo k_new_spw 0\n"			// ktpro feature

	"localinfo k_membercount 0\n"		// some unlimited values
	"localinfo k_lockmin 0\n"			// some unlimited values
	"localinfo k_lockmax 64\n";         // some unlimited values


const char _1on1_um_init[] =
	"timelimit  10\n"					//
	"teamplay   0\n"					//
	"deathmatch 3\n"					//
	"localinfo k_deathmatch 3\n"		// TODO not implemented
	"localinfo k_overtime 2\n"			// overtime type
	"localinfo k_pow 0\n"				// powerups
	"localinfo k_membercount 0\n"		// no efect in duel
	"localinfo k_lockmin 0\n"			// no efect in duel
	"localinfo k_lockmax 0\n"           // no efect in duel
	"k_mode 1\n";

const char _2on2_um_init[] =
	"floodprot 9 1 1\n"					//
	"localinfo k_fp 1\n"				// TODO not implemented
	"timelimit  10\n"					//
	"teamplay   2\n"					//
	"deathmatch 3\n"					//
	"localinfo k_deathmatch 3\n"		//
	"localinfo k_overtime 1\n"			// overtime type
	"localinfo k_exttime 2\n"			// extende time for overtime
	"localinfo k_pow 1\n"				//
	"localinfo k_membercount 1\n"		// minimum number of players in each team
	"localinfo k_lockmin 1\n"			//
	"localinfo k_lockmax 2\n"           //
	"k_mode 2\n";

const char _3on3_um_init[] =
	"floodprot 9 1 1\n"
	"localinfo k_fp 1\n"
	"timelimit  20\n"
	"teamplay   2\n"
	"deathmatch 1\n"
	"localinfo k_deathmatch 1\n"
	"localinfo k_pow 1\n"
	"localinfo k_membercount 2\n"		// minimum number of players in each team
	"localinfo k_lockmin 1\n"			//
	"localinfo k_lockmax 2\n"           //
	"localinfo k_overtime 1\n"
	"localinfo k_exttime 5\n"
	"k_mode 2\n";

const char _4on4_um_init[] =
	"floodprot 9 1 1\n"
	"localinfo k_fp 1\n"
	"timelimit  20\n"
	"teamplay   2\n"
	"deathmatch 1\n"
	"localinfo k_deathmatch 1\n"
	"localinfo k_pow 1\n"
	"localinfo k_membercount 3\n"		// minimum number of players in each team
	"localinfo k_lockmin 1\n"			//
	"localinfo k_lockmax 2\n"           //
	"localinfo k_overtime 1\n"
	"localinfo k_exttime 5\n"
	"k_mode 2\n";

const char _10on10_um_init[] =
	"floodprot 9 1 1\n"
	"localinfo k_fp 1\n"
	"timelimit  30\n"
	"teamplay   2\n"
	"deathmatch 1\n"
	"localinfo k_deathmatch 1\n"
	"localinfo k_pow 1\n"
	"localinfo k_membercount 5\n"		// minimum number of players in each team
	"localinfo k_lockmin 1\n"			//
	"localinfo k_lockmax 2\n"           //
	"localinfo k_overtime 1\n"
	"localinfo k_exttime 5\n"
	"k_mode 2\n";

const char ffa_um_init[] =
	"timelimit  20\n"
	"teamplay   0\n"
	"deathmatch 3\n"
	"localinfo k_deathmatch 3\n"
	"localinfo dq 1\n"
	"localinfo dr 1\n"
	"localinfo k_pow 1\n"
	"localinfo k_membercount 0\n"		// no effect in ffa
	"localinfo k_lockmin 0\n"			// no effect in ffa
	"localinfo k_lockmax 0\n"           // no effect in ffa
	"localinfo k_dis 1\n"
	"k_mode 3\n";


#define UM_1ON1		( 1<<0  )
#define UM_2ON2		( 1<<1  )
#define UM_3ON3		( 1<<2  )
#define UM_4ON4		( 1<<3  )
#define UM_10ON10	( 1<<4  )
#define UM_FFA		( 1<<5  )

typedef struct usermode_s {
	const char 	  *name;
	const char 	  *displayname;
	const char    *initstring;
	int		um_flags;
} usermode;

usermode um_list[] =
{
	{ "1on1", 	"1 on 1",	_1on1_um_init,		UM_1ON1},
	{ "2on2",	"2 on 2",	_2on2_um_init,		UM_2ON2},
	{ "3on3",	"3 on 3",	_3on3_um_init,		UM_3ON3},
	{ "4on4",	"4 on 4",	_4on4_um_init,		UM_4ON4},
	{ "10on10",	"10 on 10",	_10on10_um_init,	UM_10ON10},
	{ "ffa",	"ffa",		ffa_um_init,		UM_FFA}
};

int um_cnt = sizeof (um_list) / sizeof (um_list[0]);

void UserMode(float umode)
{
	char buf[1024*4];

	char *um=NULL;
	int k_free_mode = atoi( ezinfokey( world, "k_free_mode" ) );
	int k_allowed_free_modes = atoi( ezinfokey( world, "k_allowed_free_modes" ) );
	int i;

	if ( match_in_progress )
		return;

	if( atoi( ezinfokey( world, "k_master" ) ) && self->k_admin != 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	switch ((int)umode) {
		case 1: um = "1on1";   break;
		case 2: um = "2on2";   break;
		case 3: um = "3on3";   break;
		case 4: um = "4on4";   break;
		case 5: um = "10on10"; break;
		case 6: um = "ffa";    break;
		default: G_Error ("UserMode: unknown mode");
	}

//for 1on1 / 2on2 / 4on4 and ffa commands manipulation
//0 - noone, 1 - admins, 2 elected admins too
//3 - only real real judges, 4 - elected judges too
//5 - all players

// hmm, I didn't understand how k_free_mode affect this command,
// so implement how i think this must be, it is like some sort of access control

	switch ( k_free_mode ) {
		case 0:	G_sprint(self, 2, "%s can use this command\n", redtext("noone"));
				return;
		case 1:
		case 2:	if ( self->k_admin != 2 ) {
					G_sprint(self, 2, "you must be an %s\n", redtext("admin"));
					return;
				}
				break;
		case 3:
		case 4:	if ( self->k_admin != 2 ) {
					G_sprint(self, 2, "%s is not implemented in this mode\n", redtext("judges"));
					G_sprint(self, 2, "you must be an %s\n", redtext("admin"));
					return;
				}
				break;
		case 5:
				break;
		default:
				G_sprint(self, 2, "server is misconfigured, command %s\n", redtext("skipped"));
				return;
	}

// ok u have access, but is this command really allowed at all?

	for ( i = 0; i < um_cnt; i++ )
		if ( streq(um, um_list[i].name) && (um_list[i].um_flags & k_allowed_free_modes))
			break;

	if ( i >= um_cnt ) {
		G_sprint(self, 2, "server configuration %s this command\n", redtext("lock"));
		return;
	}

	G_bprint(2, "%s %s\n", redtext(um_list[i].displayname), redtext("settings enabled"));

	trap_readcmd( common_um_init, buf, sizeof(buf) );
	G_cprint("%s", buf);

	if ( self->k_admin == 2 ) // some admin features, may be overwriten by um_list[i].initstring
	{
		// introduce 'k_umfallbunny', which is just control which value
		// must be set to 'k_fallbunny' after XonX
		int k_umfallbunny = bound ( 0, atoi( ezinfokey( world, "k_umfallbunny" ) ), 1);
		trap_readcmd(va("localinfo k_fallbunny %d\n", k_umfallbunny), buf, sizeof(buf) );
		G_cprint("%s", buf);
	}

	trap_readcmd( um_list[i].initstring, buf, sizeof(buf) );
	G_cprint("%s\n", buf);

	// TODO: IMO possible check existence of each file, so don't spam in logs like "can't find etc..."

	trap_readcmd(va("exec configs/usermodes/default.cfg\n"), buf, sizeof(buf) );
	G_cprint("%s", buf);
	trap_readcmd(va("exec configs/usermodes/%s.cfg\n", g_globalvars.mapname), buf, sizeof(buf) );
	G_cprint("%s", buf);
	trap_readcmd(va("exec configs/usermodes/%s/default.cfg\n", um), buf, sizeof(buf) );
	G_cprint("%s", buf);
	trap_readcmd(va("exec configs/usermodes/%s/%s.cfg\n", um, g_globalvars.mapname), buf, sizeof(buf) );
	G_cprint("%s", buf);
}

#define UNPAUSEGUARD ( "unpauseGuard" )


void VoteUnpauseClean()
{
	gedict_t *p = find(world, FOFCLSN, "player");

	while( p ) {
		p->k_voteUnpause = 0; // just for sanity
		p = find(p, FOFCLSN, "player");
	}

	ent_remove( self );
}

void VoteUnpauseThink()
{
	gedict_t *p;
	float f1 = 0, f2 = floor( (float)CountPlayers() / 2 ) + 1;

	if ( !k_pause ) { // admin unpaused?
		VoteUnpauseClean();
		return;
	}

	p = find(world, FOFCLSN, "player");
	while( p ) {
		if( p->k_voteUnpause )
			f1++;

		p = find(p, FOFCLSN, "player");
	}

	if ( f1 >= f2 ) {
		G_bprint(2, "Server unpaused the game\n");
		ModPause( 0 );
		VoteUnpauseClean();
		return;
	}

	if ( self->cnt <= 0 ) {
		G_bprint(2, "The unpause voting has timed out, aborting\n");
		VoteUnpauseClean();
		return;
	}

	self->s.v.nextthink = g_globalvars.time + 0.5;
	self->cnt -= 0.5;
}

void VoteUnpause ()
{
	gedict_t *unpauseGuard, *p;
	float f1 = 0, f2 = floor( (float)CountPlayers() / 2 ) + 1;

	if ( k_pause != 2 )
		return;

	if( self->k_voteUnpause ) {
		G_sprint(self, 2, "You are already voted\n"); // FIXME: voted or voting, spell check plzzz
		return;
	}

// one guard per server
	unpauseGuard = find(world, FOFCLSN, UNPAUSEGUARD );
	if( !unpauseGuard ) {
		unpauseGuard = spawn();
		unpauseGuard->s.v.owner = EDICT_TO_PROG( world );
		unpauseGuard->s.v.classname = UNPAUSEGUARD;
		unpauseGuard->s.v.think = ( func_t ) VoteUnpauseThink;
		unpauseGuard->s.v.nextthink = g_globalvars.time + 0.5;
		unpauseGuard->cnt = 60; // Check the 1 minute timeout for vote

		p = find(world, FOFCLSN, "player");
		while( p ) {
			p->k_voteUnpause = 0; // reset players
			p = find(p, FOFCLSN, "player");
		}
	}

	self->k_voteUnpause = 1;

	p = find(world, FOFCLSN, "player");
	while( p ) {
		if( p->k_voteUnpause )
			f1++;

		p = find(p, FOFCLSN, "player");
	}

	G_bprint(2, "%s %s\n", self->s.v.netname, redtext("votes for unpause!"));
    if ( f1 < f2 )
		G_bprint(2, "%d more vote%s needed\n", (int)(f2 - f1),  ( (int)(f2 - f1) == 1 ? "" : "s") );
}

// ok, a bit complicated
// this routine may change map if srv_practice_mode == 0 and mapname is not NULL
void SetPractice(int srv_practice_mode, const char *mapname)
{
	if ( match_in_progress )
		G_Error ("SetPractice: match_in_progress");

	k_practice = srv_practice_mode;
	localcmd("localinfo srv_practice_mode %d\n", srv_practice_mode);

	if ( k_practice )
		G_bprint(2, "%s\n", redtext("Server in practice mode"));
	else {
		G_bprint(2, "%s\n", redtext("Server in normal mode"));
		if ( mapname ) // mapname may be "" i.e empty, reload current map in this case
			changelevel( ( strnull( mapname ) ? g_globalvars.mapname : mapname ) );
	}
}

void TogglePractice()
{
	int lock_practice         = atoi( ezinfokey( world, "lock_practice" ) );
	int allow_toggle_practice = atoi( ezinfokey( world, "allow_toggle_practice" ) );

	if ( match_in_progress )
		return;

	if( (atoi( ezinfokey( world, "k_master" ) ) && self->k_admin != 2)
		|| lock_practice == 2 /* server locked in current practice mode */
		|| (lock_practice != 0 && lock_practice != 1) /* unknown lock type, ignore command */
	  ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	if ( k_force || find ( world, FOFCLSN, "idlebot" ) )
		return;  // cmon, no practice if forcestart or idlebot active

//0 - noone, 1 - admins, 2 elected admins too
//3 - only real real judges, 4 - elected judges too
//5 - all players

// implement how i think this must be, it is like some sort of access control

	switch ( allow_toggle_practice ) {
		case 0:	G_sprint(self, 2, "%s can use this command\n", redtext("noone"));
				return;
		case 1:
		case 2:	if ( self->k_admin != 2 ) {
					G_sprint(self, 2, "you must be an %s\n", redtext("admin"));
					return;
				}
				break;
		case 3:
		case 4:	if ( self->k_admin != 2 ) {
					G_sprint(self, 2, "%s is not implemented in this mode\n", redtext("judges"));
					G_sprint(self, 2, "you must be an %s\n", redtext("admin"));
					return;
				}
				break;
		case 5:
				break;
		default:
				G_sprint(self, 2, "server is misconfigured, command %s\n", redtext("skipped"));
				return;
	}

// ok u have access
	SetPractice( !k_practice, "" ); // reload current map if needed
}

// allow reset weapon stats in prewar
void Wp_Reset ( )
{
	if ( match_in_progress )
		return;

	self->ps.h_axe = self->ps.a_axe = self->ps.h_sg = self->ps.a_sg  = 0;
	self->ps.h_ssg = self->ps.a_ssg = 0;
	self->ps.h_ng  = self->ps.a_ng  = self->ps.h_sng= self->ps.a_sng = 0;
	self->ps.h_gl  = self->ps.a_gl  = 0;
	self->ps.h_rl  = self->ps.a_rl  = self->ps.h_lg = self->ps.a_lg  = 0;
}

void Wp_Stats(float on)
{
	on--;

	self->wp_stats = (int)on;
	self->wp_stats_time = g_globalvars.time; // force show/hide
}

void Sc_Stats(float on)
{
	on--;

	self->sc_stats = (int)on;
	self->sc_stats_time = g_globalvars.time; // force show/hide
}

void W_WeaponFrame();

void kfjump ()
{
	int button0 = self->s.v.button0;

	if ( cvar( "k_disallow_kfjump" ) ) {
		G_sprint(self, 2, "%s is disabled\n", redtext("kfjump"));
		return;
	}

	self->s.v.impulse = 7;		 // select switch to rl
	self->s.v.button0 = 1;		 // force attack button
	self->s.v.v_angle[1] += 180; // turn 180
	W_WeaponFrame ();			 // switch to rl and fire
// angle don't changed in server - only in mod //VVD
//	self->s.v.v_angle[1] -= 180; // turn back
	self->s.v.button0 = button0; // restore button state
}

void krjump ()
{
	int button0 = self->s.v.button0;
// angle don't changed in server - only in mod //VVD
//	float va_x = self->s.v.v_angle[0];

	if ( cvar( "k_disallow_krjump" ) ) {
		G_sprint(self, 2, "%s is disabled\n", redtext("krjump"));
		return;
	}

	self->s.v.impulse = 7;		 // select switch to rl
	self->s.v.button0 = 1;		 // force attack button
	self->s.v.v_angle[0] = 80;   // look down much as possible, qw block this at 80
	W_WeaponFrame ();			 // switch to rl and fire
// angle don't changed in server - only in mod //VVD
//	self->s.v.v_angle[0] = va_x; // restore
	self->s.v.button0 = button0; // restore button state
}

void t_jump (float j_type)
{
	char *jt, *cv_jt, cjt = j_type == 1 ? 'f' : 'r';

	if ( match_in_progress )
		return;

	if ( iKey( world, "k_master" ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	jt    = va("k%cjump", cjt);
	cv_jt = va("k_disallow_k%cjump", cjt);

	trap_cvar_set_float( cv_jt, !cvar( cv_jt ) );
	G_bprint(2, "%s %s %s\n", self->s.v.netname, redtext( Enables( !cvar( cv_jt ) ) ),
							  redtext( jt ) );
}

void klist ( )
{
	int i, hdc;
	gedict_t *p = world;
	char *track;

	for( i = 0, p = world; p = find(p, FOFCLSN, "player"); i++ ) {
		if ( !i ) {
			G_sprint(self, 2, "Clients list: %s\n", redtext( "players" ) );
			G_sprint(self, 2, "%s %s %s %s %s %s\n",
 						redtext( "id" ), redtext( "ad" ), redtext( "vip" ),
						redtext( "hdp" ), redtext( "team" ), redtext( "name" ) );
		}

		hdc = GetHandicap(p);

		G_sprint(self, 2, "%2d|%2s|%3d|%3s|%4.4s|%s\n", GetUserID( p ),
						(p->k_admin == 2 ? redtext("A") : ""), p->vip,
						(hdc == 100 ? "off" : va("%d%%", hdc)), getteam( p ), getname( p ));
	}

	if (i)
		G_sprint(self, 2, "%s %2d found %s\n", redtext("--"), i, redtext("-------------") );

	for( i = 0, p = world; p = find(p, FOFCLSN, "spectator"); i++ ) {
		if ( !i ) {
			G_sprint(self, 2, "Clients list: %s\n", redtext( "spectators" ) );
			G_sprint(self, 2, "%s %s %s %s\n",
 						redtext( "id" ), redtext( "ad" ), redtext( "vip" ),
						redtext( "name" ) );
		}

		track = TrackWhom( p );

		G_sprint(self, 2, "%2d|%2s|%3d|%s%s\n", GetUserID( p ),
						(p->k_admin == 2 ? redtext("A") : ""), p->vip, getname( p ),
						(strnull(track) ? "" : va(" \x8D %s", track)) );
	}

	if (i)
		G_sprint(self, 2, "%s %2d found %s\n", redtext("--"), i, redtext("-------------") );

}

void hdptoggle ()
{
	if ( match_in_progress )
		return;

	if ( iKey( world, "k_master" ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	trap_cvar_set_float( "k_lock_hdp", !cvar( "k_lock_hdp" ) );
	G_bprint(2, "%s %s %s\n", self->s.v.netname,
				redtext( Allows( !cvar( "k_lock_hdp" ) ) ), redtext( "handicap" ) );
}

void handicap ()
{
	char arg_3[1024];
	int hdc = GetHandicap(self);

	if (trap_CmdArgc() == 3) {
		trap_CmdArgv( 2, arg_3, sizeof( arg_3 ) );
		hdc = atoi(arg_3);
	}
	else if ( hdc > 85 )
		hdc = 85;
	else if ( hdc > 70 )
		hdc = 70;
	else if ( hdc > 55 )
		hdc = 55;
	else if ( hdc > 40 )
		hdc = 40;
	else
		hdc = 100;

	SetHandicap(self, hdc);
}

void show_disallowed_weapons( int k_disallow_weapons )
{
	char dwp[128] = {0};

	if (k_disallow_weapons & IT_AXE)
		strlcat(dwp, " axe", sizeof(dwp));
	if (k_disallow_weapons & IT_SHOTGUN)
		strlcat(dwp, " sg", sizeof(dwp));
	if (k_disallow_weapons & IT_SUPER_SHOTGUN)
		strlcat(dwp, " ssg", sizeof(dwp));
	if (k_disallow_weapons & IT_NAILGUN)
		strlcat(dwp, " ng", sizeof(dwp));
	if (k_disallow_weapons & IT_SUPER_NAILGUN)
		strlcat(dwp, " sng", sizeof(dwp));
	if (k_disallow_weapons & IT_GRENADE_LAUNCHER)
		strlcat(dwp, " gl", sizeof(dwp));
	if (k_disallow_weapons & IT_ROCKET_LAUNCHER)
		strlcat(dwp, " rl", sizeof(dwp));
	if (k_disallow_weapons & IT_LIGHTNING)
		strlcat(dwp, " lg", sizeof(dwp));

	G_sprint(self, 2, "weapons disallowed:%s\n", 
				( strnull( dwp ) ? redtext( " none" ) : redtext( dwp ) ) );
}

void noweapon ()
{
	char arg_3[1024];
	int	k_disallow_weapons = (int)cvar("k_disallow_weapons") & DA_WPNS;

	if ( match_in_progress ) {
		if ( deathmatch == 4 ) // match started, show info and return
			show_disallowed_weapons( k_disallow_weapons );
		return;
	}

	if ( iKey( world, "k_master" ) && self->k_admin < 2 ) {
		G_sprint(self, 3, "console: command is locked\n");
		return;
	}

	if ( deathmatch != 4 ) {
		G_sprint(self, 2, "command allowed in %s only\n", redtext("dmm4"));
		return;
	}

	if ( trap_CmdArgc() == 2 ) { // no arguments, show info and return
		show_disallowed_weapons( k_disallow_weapons );
		return;
	}

	// one argument
	if (trap_CmdArgc() == 3) {
		char *nwp = NULL;
		int bit = 0;

		trap_CmdArgv( 2, arg_3, sizeof( arg_3 ) );

		if ( streq( nwp = "axe", arg_3 ) )
			k_disallow_weapons ^= bit = IT_AXE;
		else if ( streq( nwp = "sg", arg_3 ) )
			k_disallow_weapons ^= bit = IT_SHOTGUN;
		else if ( streq( nwp = "ssg", arg_3 ) )
			k_disallow_weapons ^= bit = IT_SUPER_SHOTGUN;
		else if ( streq( nwp = "ng", arg_3 ) )
			k_disallow_weapons ^= bit = IT_NAILGUN;
		else if ( streq( nwp = "sng", arg_3 ) )
			k_disallow_weapons ^= bit = IT_SUPER_NAILGUN;
		else if ( streq( nwp = "gl", arg_3 ) )
			k_disallow_weapons ^= bit = IT_GRENADE_LAUNCHER;
		else if ( streq( nwp = "rl", arg_3 ) )
			k_disallow_weapons ^= bit = IT_ROCKET_LAUNCHER;
		else if ( streq( nwp = "lg", arg_3 ) )
			k_disallow_weapons ^= bit = IT_LIGHTNING;

		if ( bit ) {
			G_bprint(2, "%s %s %s\n", self->s.v.netname,
				redtext( Allows( !(k_disallow_weapons & bit) ) ), redtext( nwp ) );
			trap_cvar_set_float( "k_disallow_weapons", k_disallow_weapons );
		}
		else {
			G_sprint(self, 2, "unknown weapon name %s\n", redtext(arg_3));
		}
		return;
	}
}

void tracklist ( )
{
	int i;
	gedict_t *p;
	char *track;
	char *nt = redtext(" not tracking");

	for( i = 0, p = world; p = find(p, FOFCLSN, "spectator"); i++ ) {
		if ( !i )
			G_sprint(self, 2, "%s:\n", redtext( "Trackers list" ) );

		track = TrackWhom( p );

		G_sprint(self, 2, "%15s%s\n", getname( p ),
							(strnull(track) ? nt : va(" \x8D %s", track)) );
	}

	if ( !i )
		G_sprint(self, 2, "No spectators present\n" );
}

void fpslist ( )
{
	int i;
	gedict_t *p;
	float cur, max, min, avg;

	for( i = 0, p = world; p = find(p, FOFCLSN, "player"); i++ ) {
		if ( !i ) {
			G_sprint(self, 2, "Players %s list:\n", redtext("FPS") );
			G_sprint(self, 2, "         name:(cur \x8f max \x8f min \x8f avg)\n");
			G_sprint(self, 2, "�������������������������������������\n");
		}

		cur = p->fCurrentFrameTime ? ( 1.0f / p->fCurrentFrameTime ) : 0;
		max = p->fLowestFrameTime ? ( 1.0f / p->fLowestFrameTime ) : 0;
		min = p->fHighestFrameTime ? ( 1.0f / p->fHighestFrameTime ) : 0;

		avg = p->fFrameCount ? ( p->fAverageFrameTime / p->fFrameCount ) : 0;
		avg = avg ? (1.0f / avg) : 0;

		G_sprint(self, 2, "%13s: %3d \x8f %3d \x8f %3d \x8f%5.1f\n", getname( p ),
				Q_rint(cur), Q_rint(max), Q_rint(min), avg);
	}

	if ( !i )
		G_sprint(self, 2, "No players present\n" );
}

// This is designed for pickup games and creates totally random teams(ish)
// It creates teams thus :
// Team red  color  4 skin ""
// team blue color 13 skin ""
void RandomPickup ()
{
    int votes;
	
	if( match_in_progress )
        return;

	if( k_captains ) {
		G_sprint(self, 2, "No random pickup when captain stuffing\n");
		return;
	}

	if( atoi( ezinfokey( world, "k_master" ) ) && self->k_admin < 2 ) {
		G_sprint(self, 2, "console: command is locked\n");
		return;
	}

    // Dont need to bother if less than 4 players
    if( CountPlayers() < 4 )
    {
        G_sprint(self, 2, "You need at least 4 players to do this.\n");
        return;
    }

	self->v.rpickup = !self->v.rpickup;

	G_bprint(2, "%s %s!%s\n", self->s.v.netname, 
			(self->v.rpickup ? redtext("votes for rpickup") :
							   redtext("withdraws %s rpickup vote", SexStr(self))),
			((votes = get_votes_req( OV_RPICKUP, true )) ? va(" (%d)", votes) : ""));

	vote_check_rpickup ();
}

// { spec tracking stuff 

qboolean fav_del_do(gedict_t *s, gedict_t *p, char *prefix);
qboolean favx_del_do(gedict_t *s, gedict_t *p, char *prefix);

// this is called from ClientDisconnect - so disconnected players
// removed from spectators favourites
void del_from_specs_favourites(gedict_t *rm)
{
	int from;
	gedict_t *p;

	for( from = 1, p = world; p = find_plrspc(p, &from); ) {
		fav_del_do(p, rm, "auto: ");
		favx_del_do(p, rm, "auto: ");
	}
}

void fav_add( )
{
	int fav_num, free_num;
	gedict_t *goal = PROG_TO_EDICT(self->s.v.goalentity);
	int diff = (int)(goal - world);

	if ( !goal->k_player || diff < 1 || diff > MAX_CLIENTS ) {
		G_sprint(self, 2, "fav_add: you are %s player!\n", redtext("not tracking"));
		return;
	}

	for ( free_num = -1, fav_num = 0; fav_num < MAX_CLIENTS; fav_num++ )
		if ( self->fav[fav_num] == diff ) {
			G_sprint(self, 2, "fav_add: %s %s added to favourites\n", goal->s.v.netname,
																	redtext("already"));
			return;
		}
		else if ( free_num < 0 && !self->fav[fav_num] ) { // ok - found free slot
			free_num = fav_num;
		}

	fav_num = free_num + 1;

	if ( fav_num < 1 || fav_num > MAX_CLIENTS ) { // must not happen
		G_sprint(self, 2, "fav_add: oops, all slots busy? Can't add.\n");
		return;
	}
	
	G_sprint(self, 2, "fav_add: %s added to favourites\n", goal->s.v.netname);

	self->fav[(int)fav_num - 1] = diff;
}


// s - for whom remove
// p - who removed
qboolean fav_del_do(gedict_t *s, gedict_t *p, char *prefix)
{
	qboolean removed = false;
	int fav_num;

	if ( !s || !p )
		return false;

	for ( fav_num = 0; fav_num < MAX_CLIENTS; fav_num++ )
		if ( s->fav[fav_num] && (world + s->fav[fav_num]) == p ) {
			if ( removed == false ) // show info one time
				G_sprint(s, 2, "%s%s removed from favourites\n", 
							prefix, (strnull(p->s.v.netname) ? "-someone-" : p->s.v.netname));

			s->fav[fav_num] = 0;
			removed = true; // does't break, so if this player multiple times in favourites
							// he will removed anyway, must not happend really
		}

	return removed;
}

// s - for whom remove
// p - who removed
qboolean favx_del_do(gedict_t *s, gedict_t *p, char *prefix)
{
	qboolean removed = false;
	int fav_num;

	if ( !s || !p )
		return false;

	for ( fav_num = 0; fav_num < MAX_CLIENTS; fav_num++ )
		if ( s->favx[fav_num] && (world + s->favx[fav_num]) == p ) {
			G_sprint(s, 2, "%s%s removed from \x90slot %2d\x91\n", 
				prefix, (strnull(p->s.v.netname) ? "-someone-" : p->s.v.netname), fav_num + 1);

			s->favx[fav_num] = 0;
			removed = true; // does't break, so if this player multiple times in favourites
							// he will removed anyway
		}

	return removed;
}

void fav_del( )
{
	gedict_t *goal = PROG_TO_EDICT(self->s.v.goalentity);
	int diff = (int)(goal - world);

	if ( !goal->k_player || diff < 1 || diff > MAX_CLIENTS ) {
		G_sprint(self, 2, "fav_del: you are %s player!\n", redtext("not tracking"));
		return;
	}

	if ( fav_del_do(self, goal, "fav_del: ") )
		return;

	G_sprint(self, 2, "fav_del: %s is %s favourites\n", goal->s.v.netname, redtext("not in"));
}

void fav_all_del( )
{
	qboolean deleted = false;
	int fav_num;

	for ( fav_num = 0; fav_num < MAX_CLIENTS; fav_num++ )
		if ( self->fav[fav_num] ) {
			self->fav[fav_num] = 0;
			deleted = true;
		}

	G_sprint(self, 2, "Favourites list %sdeleted\n", (deleted ? "" : redtext("already ")));
}

void favx_add( float fav_num )
{
	gedict_t *goal = PROG_TO_EDICT(self->s.v.goalentity);
	int diff = (int)(goal - world);

	if ( fav_num < 1 || fav_num > MAX_CLIENTS )
		return;

	if ( !goal->k_player || diff < 1 || diff > MAX_CLIENTS ) {
		G_sprint(self, 2, "fav add: you are %s player!\n", redtext("not tracking"));
		return;
	}
	
	G_sprint(self, 2, "fav add: %s added to \x90slot %d\x91\n", goal->s.v.netname, (int)fav_num);

	self->favx[(int)fav_num - 1] = diff;
}

void fav_next( )
{
	int pl_num, fav_num, first_fav, desired_fav;
	gedict_t *goal = PROG_TO_EDICT(self->s.v.goalentity), *p;
	int diff = (int)(goal - world);

	for ( fav_num = 0; fav_num < MAX_CLIENTS; fav_num++ )
		if ( self->fav[fav_num] )
			break;

	if ( fav_num >= MAX_CLIENTS ) {
		G_sprint(self, 2, "fav_next: favourites list is %s\n", redtext("empty"));
		return;
	}

	desired_fav = -2;
	first_fav = fav_num; // remember

	if ( !( !goal->k_player || diff < 1 || diff > MAX_CLIENTS ) ) {
 		// ok - tracking player, so if goal in favourites switch to the next favourite,
 		// if goal not in favourites switch to the first favourite
		for ( fav_num = first_fav; fav_num < MAX_CLIENTS; fav_num++ )
			if ( desired_fav == -2 && self->fav[fav_num] == diff ) {
				desired_fav = -1; // found goal in favourites, search now next fav
			}
			else if ( desired_fav == -1 && self->fav[fav_num] ) {
				desired_fav = fav_num; // found next fav in favourites
				break;
			}
	}

	if ( desired_fav >= 0 )
		fav_num = desired_fav + 1;
	else
		fav_num = first_fav + 1;

	if ( fav_num < 1 || fav_num > MAX_CLIENTS ) {
		G_sprint(self, 2, "fav_next: internal error, fav_num %d\n", fav_num);
		return; // sanity - must not happen
	}

	pl_num = self->fav[fav_num - 1];

	if ( pl_num < 1 || pl_num > MAX_CLIENTS ) {
		G_sprint(self, 2, "fav_next: internal error, slot %d\n", pl_num);
		return;
	}

	p = world + pl_num;

	if ( !p->k_player ) {
		G_sprint(self, 2, "fav_next: can't find player\n");
		return;
	}

	if ( PROG_TO_EDICT( self->s.v.goalentity ) == p ) {
		G_sprint(self, 2, "fav_next: already observing...\n");
		return;
	}
	
	stuffcmd( self, "track %d\n", GetUserID( p ) );
}

void xfav_go( float fav_num )
{
	gedict_t *p;
	int pl_num;

	if ( fav_num < 1 || fav_num > MAX_CLIENTS )
		return;

	pl_num = self->favx[(int)fav_num - 1];

	if ( pl_num < 1 || pl_num > MAX_CLIENTS ) {
		G_sprint(self, 2, "fav go: \x90slot %d\x91 is not defined\n", (int)fav_num);
		return;
	}

	p = world + pl_num;

	if ( !p->k_player ) {
		G_sprint(self, 2, "fav go: \x90slot %d\x91 can't find player\n", (int)fav_num);
		return;
	}

	if ( PROG_TO_EDICT( self->s.v.goalentity ) == p ) {
		G_sprint(self, 2, "fav go: already observing...\n");
		return;
	}
	
	stuffcmd( self, "track %d\n", GetUserID( p ) );
}

void fav_show( )
{
	gedict_t *p;
	qboolean first, showed = false;
	int fav_num, diff;

	for ( first = true, fav_num = 0; fav_num < MAX_CLIENTS; fav_num++ )
		if ( diff = self->favx[fav_num] ) {
		    p = world + diff;
			if ( !p->k_player || strnull( p->s.v.netname ) )
				continue;

			if ( first ) {
				G_sprint(self, 2, "%s \x90%s\x91 %s:\n", redtext("Favourites"),
							redtext("slots based"), redtext("list"));
				first = false;
			}

			G_sprint(self, 2, " \x90slot %2d\x91 \x8D %s\n", fav_num + 1, p->s.v.netname);
			showed = true;
		}

	if ( showed )
		G_sprint(self, 2, "\n");

	for ( first = true, fav_num = 0; fav_num < MAX_CLIENTS; fav_num++ )
		if ( diff = self->fav[fav_num] ) {
		    p = world + diff;
			if ( !p->k_player || strnull( p->s.v.netname ) )
				continue;

			if ( first ) {
				G_sprint(self, 2, "%s:\n", redtext("Favourites list"));
				first = false;
			}

			G_sprint(self, 2, " %s\n", p->s.v.netname);
			showed = true;
		}

	if ( !showed )
		G_sprint(self, 2, "Favourites list %s or nothing to show\n", redtext("empty"));
}

// }  spec tracking stuff 

// VVD pos_save/pos_move commands {
vec3_t velocity, avelocity, origin, angles, v_angle;

void Pos_Show ()
{
	G_sprint(self, 2, "velocity: %.2f %.2f %.2f\n", self->s.v.velocity[0], self->s.v.velocity[1], self->s.v.velocity[2]);
	G_sprint(self, 2, "avelocity: %.2f %.2f %.2f\n", self->s.v.avelocity[0], self->s.v.avelocity[1], self->s.v.avelocity[2]);
	G_sprint(self, 2, "origin: %.2f %.2f %.2f\n", self->s.v.origin[0], self->s.v.origin[1], self->s.v.origin[2]);
	G_sprint(self, 2, "angles: %.2f %.2f %.2f\n", self->s.v.angles[0], self->s.v.angles[1], self->s.v.angles[2]);
	G_sprint(self, 2, "v_angle: %.2f %.2f %.2f\n\n", self->s.v.v_angle[0], self->s.v.v_angle[1], self->s.v.v_angle[2]);

	G_sprint(self, 2, "velocity: %.2f %.2f %.2f\n", velocity[0], velocity[1], velocity[2]);
	G_sprint(self, 2, "avelocity: %.2f %.2f %.2f\n", avelocity[0], avelocity[1], avelocity[2]);
	G_sprint(self, 2, "origin: %.2f %.2f %.2f\n", origin[0], origin[1], origin[2]);
	G_sprint(self, 2, "angles: %.2f %.2f %.2f\n", angles[0], angles[1], angles[2]);
	G_sprint(self, 2, "v_angle: %.2f %.2f %.2f\n",v_angle[0], v_angle[1], v_angle[2]);
}

void Pos_Save ()
{
	velocity[0] = self->s.v.velocity[0];
	velocity[1] = self->s.v.velocity[1];
	velocity[2] = self->s.v.velocity[2];
	avelocity[0] = self->s.v.avelocity[0];
	avelocity[1] = self->s.v.avelocity[1];
	avelocity[2] = self->s.v.avelocity[2];
	origin[0] = self->s.v.origin[0];
	origin[1] = self->s.v.origin[1];
	origin[2] = self->s.v.origin[2];
	angles[0] = self->s.v.angles[0];
	angles[1] = self->s.v.angles[1];
	angles[2] = self->s.v.angles[2];
	v_angle[0] = self->s.v.v_angle[0];
	v_angle[1] = self->s.v.v_angle[1];
	v_angle[2] = self->s.v.v_angle[2];
	G_sprint(self, 2, "Position was saved\n");
}

// Don't work setting angle. //VVD
void Pos_Move ()
{
	self->s.v.velocity[0] = velocity[0] = 0;
	self->s.v.velocity[1] = velocity[1] = 0;
	self->s.v.velocity[2] = velocity[2] = 0;
	self->s.v.avelocity[0] = avelocity[0] = 0;
	self->s.v.avelocity[1] = avelocity[1] = 0;
	self->s.v.avelocity[2] = avelocity[2] = 0;
	self->s.v.origin[0] = origin[0];
	self->s.v.origin[1] = origin[1];
	self->s.v.origin[2] = origin[2];
/*	self->s.v.angles[0] = angles[0];
	self->s.v.angles[1] = angles[1];
	self->s.v.angles[2] = angles[2];*/
	self->s.v.v_angle[0] = v_angle[0];
	self->s.v.v_angle[1] = v_angle[1];
	self->s.v.v_angle[2] = v_angle[2] = 0;
	G_sprint(self, 2, "Position was restored\n");
}
// VVD }
