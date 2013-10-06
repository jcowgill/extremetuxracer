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

#ifndef WINSYS_H
#define WINSYS_H

#include "bh.h"

#define NUM_RESOLUTIONS 10

namespace sf {
class Window;
class Event;
}
extern TVector2i cursor_pos;

struct TScreenRes {
	int width, height;
	TScreenRes(int w = 0, int h = 0) : width(w), height(h) {}
};

class CWinsys {
private:
	size_t numJoysticks;
	bool joystick_active;

	// sdl window
	TScreenRes resolutions[NUM_RESOLUTIONS];
	TScreenRes auto_resolution;
	sf::ContextSettings ctx;
	double CalcScreenScale () const;
public:
	sf::RenderWindow window;
	TScreenRes resolution;
	double scale;			// scale factor for screen, see 'use_quad_scale'

	CWinsys ();

	// sdl window
	const TScreenRes& GetResolution (size_t idx) const;
	string GetResName (size_t idx) const;
	void Init ();
	void SetupVideoMode (const TScreenRes& resolution);
	void SetupVideoMode (size_t idx);
	void SetupVideoMode (int width, int height);
	void KeyRepeat (bool repeat);
	void SetFonttype ();
	void PrintJoystickInfo () const;
	void ShowCursor(bool visible);
	void SwapBuffers();
	void Quit ();
	void Terminate ();
	void InitJoystick ();
	void CloseJoystick ();
	bool joystick_isActive() const { return joystick_active; }
	bool PollEvent(sf::Event& event);
};

extern CWinsys Winsys;

#endif
