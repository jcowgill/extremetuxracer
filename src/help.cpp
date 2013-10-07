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

#include "help.h"
#include "particles.h"
#include "audio.h"
#include "ogl.h"
#include "font.h"
#include "gui.h"
#include "translation.h"
#include "game_type_select.h"
#include "winsys.h"

CHelp Help;

#define TEXT_LINES 13
sf::Text* headline;
sf::Text* texts[TEXT_LINES];
sf::Text* footnote;

void CHelp::Keyb(sf::Keyboard::Key key, bool special, bool release, int x, int y) {
	State::manager.RequestEnterState (GameTypeSelect);
}

void CHelp::Mouse(int button, int state, int x, int y) {
	if (state == 1) State::manager.RequestEnterState (GameTypeSelect);
}

void CHelp::Motion(int x, int y) {
	if (param.ui_snow) push_ui_snow (cursor_pos);
}

void CHelp::Enter() {
	Winsys.ShowCursor (false);
	Music.Play (param.credits_music, -1);

	int ytop = AutoYPosN(15);

	const int xleft1 = 40;

	FT.AutoSizeN(4);
	headline = new sf::Text(Trans.Text(57), FT.getCurrentFont(), FT.GetSize());
	headline->setPosition(xleft1, AutoYPosN(5));

	FT.AutoSizeN(3);
	int offs = FT.AutoDistanceN(2);
	for (int i = 0; i < TEXT_LINES; i++) {
		texts[i] = new sf::Text(Trans.Text(44 + i), FT.getCurrentFont(), FT.GetSize());
		texts[i]->setPosition(xleft1, ytop + offs*i);
	}

	footnote = new sf::Text(Trans.Text(65), FT.getCurrentFont(), FT.GetSize());
	footnote->setPosition((Winsys.resolution.width - footnote->getLocalBounds().width) / 2, AutoYPosN(90));
}

void CHelp::Loop(double timestep) {
	check_gl_error();
	ScopedRenderMode rm(GUI);
	Winsys.clear();

	if (param.ui_snow) {
		update_ui_snow (timestep);
		draw_ui_snow();
	}

	Winsys.draw(*headline);
	for (int i = 0; i < TEXT_LINES; i++)
		Winsys.draw(*texts[i]);
	Winsys.draw(*footnote);
	Winsys.SwapBuffers();
}

void CHelp::Exit() {
	delete headline;
	for (int i = 0; i < TEXT_LINES; i++)
		delete texts[i];
	delete footnote;
	Music.Halt();
}
