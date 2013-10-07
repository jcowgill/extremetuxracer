/* --------------------------------------------------------------------
EXTREME TUXRACER

Copyright (C) 1999-2001 Jasmin F. Patry (Tuxracer)
Copyright (C) 2010 Extreme Tuxracer Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
---------------------------------------------------------------------*/

#ifdef HAVE_CONFIG_H
#include <etr_config.h>
#endif

#include "winsys.h"
#include "ogl.h"
#include "audio.h"
#include "game_ctrl.h"
#include "font.h"
#include "score.h"
#include "textures.h"
#include "spx.h"
#include "course.h"
#include <SFML/Window.hpp>
#include <iostream>

#define USE_JOYSTICK true

TVector2i cursor_pos(0, 0);

CWinsys Winsys;

CWinsys::CWinsys ()
	: auto_resolution(800, 600)
	, sfmlRenders(false) {
	for (int i = 0; i < 8; i++) {
		if (sf::Joystick::isConnected(i))
			numJoysticks++;
		else
			break;
	}
	joystick_active = false;

	resolutions[0] = TScreenRes(0, 0);
	resolutions[1] = TScreenRes(800, 600);
	resolutions[2] = TScreenRes(1024, 768);
	resolutions[3] = TScreenRes(1152, 864);
	resolutions[4] = TScreenRes(1280, 960);
	resolutions[5] = TScreenRes(1280, 1024);
	resolutions[6] = TScreenRes(1360, 768);
	resolutions[7] = TScreenRes(1400, 1050);
	resolutions[8] = TScreenRes(1440, 900);
	resolutions[9] = TScreenRes(1680, 1050);
}

const TScreenRes& CWinsys::GetResolution (size_t idx) const {
	if (idx >= NUM_RESOLUTIONS || (idx == 0 && !param.fullscreen)) return auto_resolution;
	return resolutions[idx];
}

string CWinsys::GetResName (size_t idx) const {
	if (idx >= NUM_RESOLUTIONS) return "800 x 600";
	if (idx == 0) return ("auto");
	string line = Int_StrN (resolutions[idx].width);
	line += " x " + Int_StrN (resolutions[idx].height);
	return line;
}

double CWinsys::CalcScreenScale () const {
	if (resolution.height < 768) return 0.78;
	else if (resolution.height == 768) return 1.0;
	else return (resolution.height / 768);
}

void CWinsys::SetupVideoMode (const TScreenRes& resolution_) {
	int bpp = 32;
	switch (param.bpp_mode) {
		case 0:
			bpp = 32;
			break;
		case 1:
			bpp = 16;
			break;
		case 2:
			bpp = 32;
			break;
		default:
			param.bpp_mode = 0;
			bpp = 32;
	}
	sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar;
	if (param.fullscreen)
		style |= sf::Style::Fullscreen;

	resolution = resolution_;

#ifdef USE_STENCIL_BUFFER
	sf::ContextSettings ctx(bpp, 8, 0, 1, 2);
#else
	sf::ContextSettings ctx(bpp, 0, 0, 1, 2);
#endif
	window.create(sf::VideoMode(resolution.width, resolution.height, bpp), WINDOW_TITLE, style, ctx);
	//if (param.framerate)
	//window.setFramerateLimit(param.framerate);

	scale = CalcScreenScale ();
	if (param.use_quad_scale) scale = sqrt (scale);
}

void CWinsys::SetupVideoMode (size_t idx) {
	SetupVideoMode (GetResolution(idx));
}

void CWinsys::SetupVideoMode (int width, int height) {
	SetupVideoMode (TScreenRes(width, height));
}

void CWinsys::Init () {
	SetupVideoMode(GetResolution(param.res_type));

	KeyRepeat (false);
}

void CWinsys::KeyRepeat (bool repeat) {
	window.setKeyRepeatEnabled(repeat);
}

void CWinsys::SetFonttype () {
	if (param.use_papercut_font > 0) {
		FT.SetFont ("pc20");
	} else {
		FT.SetFont ("bold");
	}
}

void CWinsys::Quit () {
	Score.SaveHighScore ();
	SaveMessages ();
	FT.Clear ();
	if (g_game.argument < 1) Players.SavePlayers ();
	window.close();
}

void CWinsys::Terminate () {
	Quit();
	exit(0);
}

void CWinsys::PrintJoystickInfo () const {
	if (numJoysticks == 0) {
		cout << "No joystick found\n";
		return;
	}
	cout << '\n';
	for (int i = 0; i < numJoysticks; i++) {
		cout << "Joystick " << i << '\n';
		int buttons = sf::Joystick::getButtonCount(i);
		cout << "Joystick has " << buttons << " button" << (buttons == 1 ? "" : "s") << '\n';
		cout << "Axes: ";
		if (sf::Joystick::hasAxis(i, sf::Joystick::R)) cout << "R ";
		if (sf::Joystick::hasAxis(i, sf::Joystick::U)) cout << "U ";
		if (sf::Joystick::hasAxis(i, sf::Joystick::V)) cout << "V ";
		if (sf::Joystick::hasAxis(i, sf::Joystick::X)) cout << "X ";
		if (sf::Joystick::hasAxis(i, sf::Joystick::Y)) cout << "Y ";
		if (sf::Joystick::hasAxis(i, sf::Joystick::Z)) cout << "Z ";
		cout << '\n';
	}
}
