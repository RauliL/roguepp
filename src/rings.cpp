/*
 * Routines dealing specifically with rings
 *
 * @(#)rings.c	4.19 (Berkeley) 05/29/83
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 */

#include <cstdio>

#include <ncurses.h>

#include <roguepp/roguepp.hpp>

/*
 * ring_on:
 *	Put a ring on a hand
 */

void
ring_on()
{
    THING *obj;
    int ring;

    obj = get_item("put on", RING);
    /*
     * Make certain that it is somethings that we want to wear
     */
    if (obj == nullptr)
	return;
    if (obj->o_type != RING)
    {
	if (!terse)
	    msg("it would be difficult to wrap that around a finger");
	else
	    msg("not a ring");
	return;
    }

    /*
     * find out which hand to put it on
     */
    if (is_current(obj))
	return;

    if (cur_ring[LEFT] == nullptr && cur_ring[RIGHT] == nullptr)
    {
	if ((ring = gethand()) < 0)
	    return;
    }
    else if (cur_ring[LEFT] == nullptr)
	ring = LEFT;
    else if (cur_ring[RIGHT] == nullptr)
	ring = RIGHT;
    else
    {
	if (!terse)
	    msg("you already have a ring on each hand");
	else
	    msg("wearing two");
	return;
    }
    cur_ring[ring] = obj;

    /*
     * Calculate the effect it has on the poor guy.
     */
    switch (obj->o_which)
    {
	case R_ADDSTR:
	    chg_str(obj->o_arm);
	    break;
	case R_SEEINVIS:
	    invis_on();
	    break;
	case R_AGGR:
	    aggravate();
	    break;
    }

    if (!terse)
	addmsg("you are now wearing ");
    msg("%s (%c)", inv_name(obj, true), obj->o_packch);
}

/*
 * ring_off:
 *	take off a ring
 */

void
ring_off()
{
    int ring;
    THING *obj;

    if (cur_ring[LEFT] == nullptr && cur_ring[RIGHT] == nullptr)
    {
	if (terse)
	    msg("no rings");
	else
	    msg("you aren't wearing any rings");
	return;
    }
    else if (cur_ring[LEFT] == nullptr)
	ring = RIGHT;
    else if (cur_ring[RIGHT] == nullptr)
	ring = LEFT;
    else
	if ((ring = gethand()) < 0)
	    return;
    mpos = 0;
    obj = cur_ring[ring];
    if (obj == nullptr)
    {
	msg("not wearing such a ring");
	return;
    }
    if (dropcheck(obj))
	msg("was wearing %s(%c)", inv_name(obj, true), obj->o_packch);
}

/*
 * gethand:
 *	Which hand is the hero interested in?
 */
int
gethand()
{
    int c;

    for (;;)
    {
	if (terse)
	    msg("left or right ring? ");
	else
	    msg("left hand or right hand? ");
	if ((c = readchar()) == ESCAPE)
	    return -1;
	mpos = 0;
	if (c == 'l' || c == 'L')
	    return LEFT;
	else if (c == 'r' || c == 'R')
	    return RIGHT;
	if (terse)
	    msg("L or R");
	else
	    msg("please type L or R");
    }
}

/*
 * ring_eat:
 *	How much food does this ring use up?
 */
int
ring_eat(int hand)
{
    THING *ring;
    int eat;
    static int uses[] = {
	 1,	/* R_PROTECT */		 1,	/* R_ADDSTR */
	 1,	/* R_SUSTSTR */		-3,	/* R_SEARCH */
	-5,	/* R_SEEINVIS */	 0,	/* R_NOP */
	 0,	/* R_AGGR */		-3,	/* R_ADDHIT */
	-3,	/* R_ADDDAM */		 2,	/* R_REGEN */
	-2,	/* R_DIGEST */		 0,	/* R_TELEPORT */
	 1,	/* R_STEALTH */		 1	/* R_SUSTARM */
    };

    if ((ring = cur_ring[hand]) == nullptr)
	return 0;
    if ((eat = uses[ring->o_which]) < 0)
	eat = (rnd(-eat) == 0);
    if (ring->o_which == R_DIGEST)
	eat = -eat;
    return eat;
}

/*
 * ring_num:
 *	Print ring bonuses
 */
std::string
ring_num(const THING& obj)
{
    static char buffer[10];
    int length;

    if (!(obj.o_flags & ISKNOW))
    {
        return std::string();
    }
    switch (obj.o_which)
    {
        case R_PROTECT:
        case R_ADDSTR:
        case R_ADDDAM:
        case R_ADDHIT:
            length = std::snprintf(
                buffer,
                10,
                " [%s]",
                num(obj.o_arm, 0, RING)
            );
            break;

        default:
            return "";
    }

    return std::string(buffer, length);
}
