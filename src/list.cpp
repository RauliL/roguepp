/*
 * Functions for dealing with linked lists of goodies
 *
 * @(#)list.c	4.12 (Berkeley) 02/05/99
 *
 * Rogue: Exploring the Dungeons of Doom
 * Copyright (C) 1980-1983, 1985, 1999 Michael Toy, Ken Arnold and Glenn Wichman
 * All rights reserved.
 *
 * See the file LICENSE.TXT for full copyright and licensing information.
 */

#include <cstdlib>

#include <ncurses.h>

#include <roguepp/roguepp.hpp>

#ifdef MASTER
int total = 0;			/* total dynamic memory bytes */
#endif

/*
 * detach:
 *	takes an item out of whatever linked list it might be in
 */

void
_detach(THING **list, THING *item)
{
    if (*list == item)
	*list = next(item);
    if (prev(item) != nullptr)
	item->l_prev->l_next = next(item);
    if (next(item) != nullptr)
	item->l_next->l_prev = prev(item);
    item->l_next = nullptr;
    item->l_prev = nullptr;
}

/*
 * _attach:
 *	add an item to the head of a list
 */

void
_attach(THING **list, THING *item)
{
    if (*list != nullptr)
    {
	item->l_next = *list;
	(*list)->l_prev = item;
	item->l_prev = nullptr;
    }
    else
    {
	item->l_next = nullptr;
	item->l_prev = nullptr;
    }
    *list = item;
}

/*
 * _free_list:
 *	Throw the whole blamed thing away
 */

void
_free_list(THING **ptr)
{
    THING *item;

    while (*ptr != nullptr)
    {
	item = *ptr;
	*ptr = next(item);
	discard(item);
    }
}

/*
 * discard:
 *	Free up an item
 */

void
discard(THING *item)
{
#ifdef MASTER
    total--;
#endif
    free((char *) item);
}

/*
 * new_item
 *	Get a new item with a specified size
 */
THING*
new_item()
{
    THING* item = static_cast<THING*>(std::calloc(1, sizeof(THING)));

#if defined(MASTER)
    if (!item)
    {
        msg("ran out of memory after %d items", total);
    } else {
        ++total;
    }
#endif
    item->l_next = nullptr;
    item->l_prev = nullptr;

    return item;
}
