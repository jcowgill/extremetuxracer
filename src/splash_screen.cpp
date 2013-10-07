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

#include "splash_screen.h"
#include "ogl.h"
#include "textures.h"
#include "audio.h"
#include "gui.h"
#include "course.h"
#include "tux.h"
#include "env.h"
#include "particles.h"
#include "credits.h"
#include "font.h"
#include "game_ctrl.h"
#include "translation.h"
#include "score.h"
#include "regist.h"
#include "winsys.h"

CSplashScreen SplashScreen;

void CSplashScreen::Keyb(sf::Keyboard::Key key, bool special, bool release, int x, int y) {
	if (release) return;
	switch (key) {
		case sf::Keyboard::Escape:
			State::manager.RequestQuit();
			break;
		case sf::Keyboard::Return:
			State::manager.RequestEnterState (Regist);
			break;
	}
}


void CSplashScreen::Enter() {
	Winsys.ShowCursor (!param.ice_cursor);
	init_ui_snow ();
	Music.Play (param.menu_music, -1);
}

void CSplashScreen::Loop(double timestep) {
	check_gl_error();
	ScopedRenderMode rm(GUI);
	Winsys.clear();
	Trans.LoadLanguages ();
	Trans.LoadTranslations (param.language); // Before first texts are being displayed

	sf::Sprite logo(Tex.GetSFTexture(TEXLOGO));
	logo.setScale(Winsys.scale, Winsys.scale);
	logo.setPosition((Winsys.resolution.width - logo.getTextureRect().width) / 2, 60);

	FT.AutoSizeN(6);
	sf::Text t1(Trans.Text(67), FT.getCurrentFont(), FT.GetSize());
	int top = AutoYPosN(60);
	t1.setPosition((Winsys.resolution.width - t1.getLocalBounds().width) / 2, top);
	sf::Text t2(Trans.Text(68), FT.getCurrentFont(), FT.GetSize());
	int dist = FT.AutoDistanceN (3);
	t2.setPosition((Winsys.resolution.width - t2.getLocalBounds().width) / 2, top + dist);

	Winsys.draw(logo);
	Winsys.draw(t1);
	Winsys.draw(t2);
	Winsys.SwapBuffers();

	Course.MakeStandardPolyhedrons ();
	Sound.LoadSoundList ();
	Credits.LoadCreditList ();
	Char.LoadCharacterList ();
	Course.LoadObjectTypes ();
	Course.LoadTerrainTypes ();
	Env.LoadEnvironmentList ();
	Course.LoadCourseList ();
	Score.LoadHighScore (); // after LoadCourseList !!!
	Events.LoadEventList ();
	Players.LoadAvatars (); // before LoadPlayers !!!
	Players.LoadPlayers ();

	State::manager.RequestEnterState (Regist);
}
