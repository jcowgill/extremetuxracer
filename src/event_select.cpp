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

#include "event_select.h"
#include "gui.h"
#include "font.h"
#include "particles.h"
#include "audio.h"
#include "ogl.h"
#include "textures.h"
#include "game_ctrl.h"
#include "translation.h"
#include "event.h"
#include "game_type_select.h"
#include "winsys.h"

CEventSelect EventSelect;

static TEvent2 *EventList;
static TUpDown* event;
static TUpDown* cup;
static TWidget* textbuttons[2];
static TCup2 *CupList;

void EnterEvent () {
	g_game.game_type = CUPRACING;
	g_game.cup = EventList[event->GetValue()].cups[cup->GetValue()];
	g_game.race_id = 0;
	State::manager.RequestEnterState(Event);
}

void CEventSelect::Keyb (sf::Keyboard::Key key, bool special, bool release, int x, int y) {
	if (release) return;
	switch (key) {
		case sf::Keyboard::Escape:
			State::manager.RequestEnterState (GameTypeSelect);
			break;
		case sf::Keyboard::Q:
			State::manager.RequestQuit();
			break;
		case sf::Keyboard::Return:
			if (textbuttons[1]->focussed()) State::manager.RequestEnterState (GameTypeSelect);
			else if (Events.IsUnlocked (event->GetValue(), cup->GetValue())) EnterEvent();
			break;
		case sf::Keyboard::U:
			param.ui_snow = !param.ui_snow;
			break;
		default:
			KeyGUI(key, 0, release);
	}
}

void CEventSelect::Mouse (int button, int state, int x, int y) {
	if (state == 1) {
		TWidget* clicked = ClickGUI(x, y);
		if (textbuttons[0] == clicked) {
			if (Events.IsUnlocked (event->GetValue(), cup->GetValue()))
				EnterEvent();
		} else if (textbuttons[1] == clicked)
			State::manager.RequestEnterState (GameTypeSelect);
	}
}

void CEventSelect::Motion (int x, int y) {
	MouseMoveGUI(x, y);

	if (param.ui_snow) push_ui_snow (cursor_pos);
}

// --------------------------------------------------------------------
static TArea area;
static int framewidth, frameheight, frametop1, frametop2;
static sf::Text selectEvent;
static sf::Text selectCup;
static sf::Text selectedEvent;
static sf::Text selectedCup;
static sf::Text cupLocked;

void CEventSelect::Enter () {
	Winsys.ShowCursor (!param.ice_cursor);
	EventList = &Events.EventList[0];
	CupList = &Events.CupList[0];

	framewidth = 500 * Winsys.scale;
	frameheight = 50 * Winsys.scale;
	area = AutoAreaN (30, 80, framewidth);
	frametop1 = AutoYPosN (35);
	frametop2 = AutoYPosN (50);

	ResetGUI();
	event = AddUpDown(area.right+8, frametop1, 0, (int)Events.EventList.size() - 1, 0);
	cup = AddUpDown(area.right+8, frametop2, 0, (int)EventList[0].cups.size() - 1, 0);

	int siz = FT.AutoSizeN (5);

	double len = FT.GetTextWidth (Trans.Text(9));
	textbuttons[0] = AddTextButton (Trans.Text(9), area.right-len-50, AutoYPosN (70), siz);
	textbuttons[1] = AddTextButton (Trans.Text(8), area.left+50, AutoYPosN (70), siz);
	SetFocus(textbuttons[1]);

	FT.AutoSizeN(3);
	selectEvent.setString(Trans.Text(6));
	selectEvent.setCharacterSize(FT.GetSize());
	selectEvent.setFont(FT.getCurrentFont());
	selectEvent.setPosition(area.left, AutoYPosN(30));
	selectCup.setString(Trans.Text(7));
	selectCup.setCharacterSize(FT.GetSize());
	selectCup.setFont(FT.getCurrentFont());
	selectCup.setPosition(area.left, AutoYPosN(45));
	cupLocked.setString(Trans.Text(10));
	cupLocked.setCharacterSize(FT.GetSize());
	cupLocked.setFont(FT.getCurrentFont());
	cupLocked.setColor(colLGrey);
	cupLocked.setPosition((Winsys.resolution.width - cupLocked.getLocalBounds().width) / 2, AutoYPosN(58));

	FT.AutoSizeN(4);
	selectedEvent.setCharacterSize(FT.GetSize());
	selectedEvent.setFont(FT.getCurrentFont());
	selectedEvent.setPosition(area.left + 20, frametop1+2);
	selectedEvent.setColor(colDYell);
	selectedCup.setCharacterSize(FT.GetSize());
	selectedCup.setFont(FT.getCurrentFont());
	selectedCup.setPosition(area.left + 20, frametop2+2);

	Events.MakeUnlockList (Players.GetCurrUnlocked());
	Music.Play (param.menu_music, -1);
}

void CEventSelect::Loop (double timestep) {
	int ww = Winsys.resolution.width;
	int hh = Winsys.resolution.height;
	sf::Color col;

	check_gl_error();
	ScopedRenderMode rm(GUI);
	Winsys.clear();

	if (param.ui_snow) {
		update_ui_snow (timestep);
		draw_ui_snow ();
	}

	DrawGUIBackground(Winsys.scale);

	Winsys.draw(selectEvent);
	Winsys.draw(selectCup);
	if (Events.IsUnlocked(event->GetValue(), cup->GetValue()) == false)
		Winsys.draw(cupLocked);

	if (event->focussed()) col = colDYell;
	else col = colWhite;
	DrawFrameX(area.left, frametop1, framewidth, frameheight, 3, colMBackgr, col, 1.0);
	selectedEvent.setString(EventList[event->GetValue()].name);
	Winsys.draw(selectedEvent);

	if (cup->focussed()) col = colDYell;
	else col = colWhite;
	DrawFrameX (area.left, frametop2, framewidth, frameheight, 3, colMBackgr, col, 1.0);
	if (Events.IsUnlocked (event->GetValue(), cup->GetValue()))
		col = colDYell;
	else
		col = colLGrey;
	selectedCup.setColor(col);
	selectedCup.setString(Events.GetCupTrivialName(event->GetValue(), cup->GetValue()));
	Winsys.draw(selectedCup);

	textbuttons[0]->SetActive(Events.IsUnlocked (event->GetValue(), cup->GetValue()));
	DrawGUI();

	Winsys.SwapBuffers();
}
