/*
 *  MetaRoom.cpp
 *  openc2e
 *
 *  Created by Alyssa Milburn on Tue May 25 2004.
 *  Copyright (c) 2004 Alyssa Milburn. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 */

#include "MetaRoom.h"
#include "Room.h"
#include "World.h"
#include "Engine.h"
#include "images/blkImage.h"
#include <assert.h>
#include "Backend.h"

MetaRoom::MetaRoom(int _x, int _y, int _width, int _height, const std::string &back, std::shared_ptr<creaturesImage> spr, bool wrap) {
	xloc = _x; yloc = _y; wid = _width; hei = _height; wraps = wrap;

	// if we were provided with a background, add it
	if (!back.empty()) {
		if (spr) {
			addBackground(back, spr);
		} else {
			addBackground(back);
		}
	}
}

void MetaRoom::addBackground(std::string back, std::shared_ptr<creaturesImage> spr) {
	std::shared_ptr<creaturesImage> backsprite;
	unsigned int totalwidth, totalheight;

	caos_assert(!back.empty());
	// TODO: cadv adds backgrounds which have already been added as the default, look into this,
	// should we preserve the default once extra backgrounds have been added and change this to
	// a caos_assert?
	if (backgrounds.find(back) != backgrounds.end()) return;
	
	if (!spr) {
		// we weren't passed a sprite, so we need to load one
		backsprite = world.gallery.getImage(back, true);
		blkImage *background = dynamic_cast<blkImage *>(backsprite.get());
		if (!background && engine.bmprenderer) {
			totalwidth = backsprite->width(0);
			totalheight = backsprite->height(0);
		} else {
			caos_assert(background);

			totalwidth = background->totalwidth;
			totalheight = background->totalheight;
		}
	} else {
		// we were provided with a sprite, so use it
		backsprite = spr;
		totalwidth = wid;
		totalheight = hei;
	}

	// store the background
	backgrounds[back] = backsprite;
	if (!firstback) {
		// set the first background
		firstback = backsprite;
		fullwid = totalwidth;
		fullhei = totalheight;
	} else {
		// make sure other backgrounds are the same size
		if (world.gametype == "sm") return; // TODO: seamonkeys fails the background size checks :/
		assert(totalwidth == fullwid);
		assert(totalheight == fullhei);
	}
}

std::vector<std::string> MetaRoom::backgroundList() {
	// construct a temporary vector from our std::map

	std::vector<std::string> b;
	for (std::map<std::string, std::shared_ptr<creaturesImage> >::iterator i = backgrounds.begin(); i != backgrounds.end(); i++)
		b.push_back(i->first);
	return b;
}

std::shared_ptr<creaturesImage> MetaRoom::getBackground(std::string back) {
	// return the first background by default
	if (back.empty()) {
		return firstback;
	}
	
	// if this background name isn't found, return null
	if (backgrounds.find(back) != backgrounds.end()) return std::shared_ptr<creaturesImage>();
	
	// otherwise, return the relevant background
	return backgrounds[back];
}
	
MetaRoom::~MetaRoom() {
	// we hold the only strong reference to our contained rooms, so they'll be auto-deleted
}

std::shared_ptr<Room> MetaRoom::nextFloorFromPoint(float x, float y) {
	std::shared_ptr<Room> closest_up, closest_down;
	float dist_down = -1, dist_up = -1;
	for (std::vector<std::shared_ptr<Room> >::iterator r = rooms.begin(); r != rooms.end(); r++) {
		if (!(*r)->bot.containsX(x)) continue;
		float dist = (*r)->bot.pointAtX(x).y - y; // down is positive
		float absdist = fabs(dist);
		if (dist >= 0 && (absdist < dist_down || dist_down < 0)) {
			dist_down = absdist;
			closest_down = *r;
		} else if (dist < 0 && (absdist < dist_up || dist_up < 0)) {
			dist_up = absdist;
			closest_up = *r;
		}
	}
	if (closest_down) return closest_down;
	if (closest_up) return closest_up;
	return std::shared_ptr<Room>();
}

unsigned int MetaRoom::addRoom(std::shared_ptr<Room> r) {
	// add to both our local list and the global list
	rooms.push_back(r);
	world.map.rooms.push_back(r);

	// set the id and return
	r->id = world.map.room_base++;
	return r->id;
}

std::shared_ptr<Room> MetaRoom::roomAt(float _x, float _y) {
	if (wraps) {
		if (_x > (int)xloc + (int)wid) _x -= wid;
		else if (_x < (int)xloc) _x += wid;
	}

	for (std::vector<std::shared_ptr<Room> >::iterator i = rooms.begin(); i != rooms.end(); i++) {
		std::shared_ptr<Room> r = *i;
		if (r->containsPoint(_x, _y)) return r;
	}

	return std::shared_ptr<Room>();
}

std::vector<std::shared_ptr<Room> > MetaRoom::roomsAt(float _x, float _y) {
	if (wraps) {
		if (_x > (int)xloc + (int)wid) _x -= wid;
		else if (_x < (int)xloc) _x += wid;
	}

	std::vector<std::shared_ptr<Room> > ourlist;

	for (std::vector<std::shared_ptr<Room> >::iterator i = rooms.begin(); i != rooms.end(); i++) {
		std::shared_ptr<Room> r = *i;
		if (r->containsPoint(_x, _y)) ourlist.push_back(r);
	}

	return ourlist;
}

/* vim: set noet: */
