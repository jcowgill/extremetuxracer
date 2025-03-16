/* --------------------------------------------------------------------
EXTREME TUXRACER

Copyright (C) 1999-2001 Jasmin F. Patry (Tuxracer)
Copyright (C) 2010 Extreme Tux Racer Team

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

#include "states.h"
#include "ogl.h"
#include "winsys.h"

State::Manager State::manager(Winsys);

State::Manager::~Manager() {
	if (current)
		current->Exit();
}

void State::Manager::Run(State& entranceState) {
	current = &entranceState;
	current->Enter();
	while (!quit) {
		PollEvent();
		if (next)
			EnterNextState();
		CallLoopFunction();
	}
	current->Exit();
	previous = current;
	current = nullptr;
}

void State::Manager::EnterNextState() {
	current->Exit();
	previous = current;
	current = next;
	next = nullptr;
	current->Enter();
}

void State::Manager::PollEvent() {
	while (const auto event = Winsys.PollEvent()) {
		if (!next) {
			if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				current->Keyb(keyPressed->code, false, sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
			} else if (const auto *keyReleased = event->getIf<sf::Event::KeyReleased>()) {
				current->Keyb(keyReleased->code, true, sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
			} else if (const auto *textEntered = event->getIf<sf::Event::TextEntered>()) {
				current->TextEntered(static_cast<char>(textEntered->unicode));
			} else if (const auto *mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				current->Mouse(static_cast<int>(mousePressed->button), true, mousePressed->position.x, mousePressed->position.y);
			} else if (const auto *mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
				current->Mouse(static_cast<int>(mouseReleased->button), false, mouseReleased->position.x, mouseReleased->position.y);
			} else if (const auto *mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
				TVector2i old = cursor_pos;
				cursor_pos.x = mouseMoved->position.x;
				cursor_pos.y = mouseMoved->position.y;
				current->Motion(mouseMoved->position.x - old.x, mouseMoved->position.y - old.y);
			} else if (const auto *joystickMoved = event->getIf<sf::Event::JoystickMoved>()) {
				float val = joystickMoved->position / 100.f;
				current->Jaxis(joystickMoved->axis == sf::Joystick::Axis::X ? 0 : 1, val);
			} else if (const auto *joystickPressed = event->getIf<sf::Event::JoystickButtonPressed>()) {
				current->Jbutt(joystickPressed->button, true);
			} else if (const auto *joystickReleased = event->getIf<sf::Event::JoystickButtonReleased>()) {
				current->Jbutt(joystickReleased->button, false);
			} else if (const auto *resized = event->getIf<sf::Event::Resized>()) {
				if (Winsys.resolution.width != resized->size.x || Winsys.resolution.height != resized->size.y) {
					Winsys.resolution.width = resized->size.x;
					Winsys.resolution.height = resized->size.y;
					Winsys.SetupVideoMode(resized->size.x, resized->size.y);
				}
			} else if (event->is<sf::Event::Closed>()) {
				quit = true;
			}
		}
	}
}

void State::Manager::CallLoopFunction() {
	check_gl_error();

	g_game.time_step = std::max(0.0001f, timer.getElapsedTime().asSeconds());
	timer.restart();
	current->Loop(g_game.time_step);
}
