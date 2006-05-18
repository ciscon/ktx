/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 *  $Id: motd.c,v 1.17 2006/05/18 18:45:27 oldmanuk Exp $
 */

// motd.c

#include "g_local.h"

void StuffModCommands();

void PMOTDThink()
{
	int i;
	char buf[2048] = {0};
	char *s;

	// remove MOTD in some cases
	if( self->attack_finished < g_globalvars.time // expired
    	|| ( !k_matchLess && match_in_progress )  // non matchless and (match has began or countdown)
	   	|| ( k_matchLess && match_in_progress == 1 ) // matchless and countdown
	  ) {
		if ( self->attack_finished < g_globalvars.time )
			G_centerprint ( PROG_TO_EDICT( self->s.v.owner ), "");

		ent_remove( self );
		return;
	}

	for( i = 1; i <= MOTD_LINES; i++) {
		if ( strnull( s = cvar_string(va("k_motd%d", i)) ) )
			continue;
		
		strlcat(buf, s, sizeof(buf));
		strlcat(buf, "\n", sizeof(buf));
	}

 	// no "welcome" - if k_motd keys is present - because admin may wanna customize this
	if ( strnull( buf ) )
		strlcat(buf, "Welcome\n\n", sizeof(buf));

	strlcat(buf, "\n�������\n\n", sizeof(buf));
	strlcat(buf, va("Running %s v.%s by %s\n\n", redtext(MOD_NAME),
					dig3s(MOD_VERSION), redtext("KTX dev. team")), sizeof(buf));
	strlcat(buf, va("Based on %s\n", redtext("Kombat teams 2.21")), sizeof(buf));
// qqshka - this info can be found in /about command
//	strlcat(buf, "by kemiKal, Cenobite, Sturm and Fang\n\n", sizeof(buf));
	strlcat(buf, va("Type \"%s\" for help", redtext("commands")), sizeof(buf));

	G_centerprint ( PROG_TO_EDICT( self->s.v.owner ), buf);

	self->s.v.nextthink = g_globalvars.time + 0.7;
}

void SMOTDThink()
{
	PMOTDThink(); // equal motd for player and spectator now
}

void MOTDThinkX()
{
// check if we are need to stuff aliases, or already done this
	if( !(PROG_TO_EDICT( self->s.v.owner )->k_stuff) )
	{
		gedict_t *p = spawn();

		p->s.v.classname = "motdX";
		p->s.v.owner = self->s.v.owner;
		p->cnt = -1;
    	p->s.v.think = ( func_t ) StuffModCommands;
    	p->s.v.nextthink = g_globalvars.time + 0.1;
	}

	self->s.v.think = // select MOTD for spectator or player
		  ( func_t ) ( PROG_TO_EDICT( self->s.v.owner )->k_spectator ? SMOTDThink : PMOTDThink );
	self->s.v.nextthink = g_globalvars.time + 0.3;

	 // remove motd if player already stuffed, because them probably sow motd already one time
	if( k_matchLess && PROG_TO_EDICT( self->s.v.owner )->k_stuff )
		ent_remove( self );
}

void MOTDStuff()
{
	gedict_t *p, *so;
	char *t, *t2, *tmp, *s1;
	float kick, f1, f2, f3;

	so = PROG_TO_EDICT( self->s.v.owner );

	if ( !so )
		G_Error ("MOTDStuff null");

	so->k_teamnum = 0;
	so->k_msgcount = g_globalvars.time;
	so->k_lastspawn = world;

	if( so->k_accepted != 2 )
			so->k_accepted = 1;

	// If the game is running already then . . .
	// guess is player can enter/re-enter the game if server locked or not
	if( match_in_progress )
	{
		kick         = 0;
		so->ready    = 1;
		so->k_666    = 0;
		so->deaths   = 0;
		so->friendly = 0;

		if( lock == 2 ) { // kick anyway
			kick = 1;

			G_sprint(so, 2, "Match in progress, server locked\n");
		}
		else if( lock == 1 ) // kick if team is not set properly
		{
			int from1;

			kick = 1;

			t = getteam( so );

			for( from1 = 0, p = world; p = find_plrghst( p, &from1 ); )
				if ( p != so && streq( getteam( p ), t ) ) {
					kick = 0;  // don't kick, find "player" or "ghost" with equal team
					break;
				}
			
			if ( kick )
				G_sprint(so, 2, "Set your team before connecting\n");
		}

		if( !k_matchLess ) // ignore in matchLess mode
		if( !kick ) // kick is exclusive 
		{
			f2 = CountPlayers();
			if( f2 >= k_attendees && cvar("k_exclusive") ) 
			{
				G_sprint(so, 2, "Sorry, all teams are full\n");
				kick = 1;
			}
		}

		if( kick ) 
		{
			so->k_accepted = 0;
			so->s.v.classname = "";

            stuffcmd(so, "disconnect\n"); // FIXME: stupid way

			ent_remove( self );

			return;
		}
	}

	if( k_matchLess ) { // no ghost and team check in matchLess mode
		so->k_teamnum = 0;
		G_bprint(2, "%s entered the game\n", so->s.v.netname);
	}
	else if( match_in_progress == 2 )
	{
		f2 = 1;
		f3 = 0;


		while( f2 < k_userid && !f3 ) // search for ghost for this player (localinfo)
		{
			t = ezinfokey(world, va("%d", (int) f2));
			if( streq( t, so->s.v.netname ))
				f3 = 1;
			else
				f2++;
		}

		if( f2 == k_userid ) // ghost not found (localinfo)
		{
			G_bprint(2, "%s arrives late �%s�\n", so->s.v.netname, getteam(so));
		} 
		else // ghost probably found (localinfo)
		{
			p = find(world, FOFCLSN, "ghost");
			while( p && f3 ) // search ghost entity
			{
				if( p->cnt2 == f2 )
					f3 = 0;
				else
					p = find(p, FOFCLSN, "ghost");
			}

			if( p ) // found ghost entity
			{
				t2 = getteam( p );
				t  = getteam( so );
				if( strneq( t, t2 ) ) 
				{
					so->k_accepted = 0;
					G_sprint(so, 2, "Please join your old team and reconnect\n");
					so->s.v.classname = "";
                    stuffcmd(so, "disconnect\n"); // FIXME: stupid way

					ent_remove( self );

					return;
				}

				ghostClearScores( p );
				G_bprint(2, "%s rejoins the game �%s�\n", so->s.v.netname, getteam( so ));
				localcmd("localinfo %d \"\"\n", (int)f2);

				so->fraggie   = p->s.v.frags;
				so->deaths    = p->deaths;
				so->friendly  = p->friendly;
				so->k_teamnum = p->k_teamnum;

				so->ps        = p->ps; // restore player stats


				
				ent_remove( p );
			}
			else // ghost entity not found
			{
				localcmd("localinfo %d \"\"\n", (int)f2);
				G_bprint(2, "%s reenters the game without stats\n", so->s.v.netname);
			}
		}

		if( !strnull ( getteam( so ) ) ) 
		{
			f1 = 665;
			while( k_teamid > f1 && !so->k_teamnum ) 
			{
				f1++;
				s1  = ezinfokey(world, va("%d", (int)f1));
				tmp = getteam( so );
				if( streq( tmp, s1 ) )
					so->k_teamnum = f1;
			}

			if( !(so->k_teamnum) ) 
			{
				f1++;

				localcmd("localinfo %d \"%s\"\n", (int)f1, getteam( so ));
				k_teamid     = f1;
				so->k_teamnum = f1;
			}
		} else
			so->k_teamnum = 666;
	} 
	else 
	{
		G_bprint(2, "%s entered the game\n", so->s.v.netname);
	}

	// terminate elect and kick modes

//	so->k_kicking = 0;
//	so->k_captain = 0;

	so->s.v.frags = so->fraggie; // restore frags from ghost or in case of captains set player number via his frags
	newcomer = so->s.v.netname;
	self->s.v.think = ( func_t ) MOTDThinkX;
	self->s.v.nextthink = g_globalvars.time + 0.1;
}

void MakeMOTD()
{
	gedict_t *motd;
	int i = bound(0, cvar("k_motd_time"), 30);

	motd = spawn();
	motd->s.v.classname = "motd";
	motd->s.v.owner = EDICT_TO_PROG( self );
	motd->s.v.think = ( func_t ) MOTDStuff;
	motd->s.v.nextthink = g_globalvars.time + 0.1;
	motd->attack_finished = g_globalvars.time + (i ? i : ( k_matchLess ? 3 : 7 ));
}

void SMakeMOTD()
{
	gedict_t *motd;
	int i = bound(0, cvar("k_motd_time"), 30);

	motd = spawn();
	motd->s.v.classname = "motd";
	motd->s.v.owner = EDICT_TO_PROG( self );
	motd->s.v.think = ( func_t ) MOTDThinkX;
	motd->s.v.nextthink = g_globalvars.time + 0.1;
	motd->attack_finished = g_globalvars.time + (i ? i : 7 );
}

