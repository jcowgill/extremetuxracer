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

#ifndef GUI_H
#define GUI_H

#include "bh.h"


#define CENTER -1
#define CRSR_PERIODE 0.4


struct TRect {
	int left;
	int top;
	int width;
	int height;
};

struct TArea {
	int left;
	int right;
	int top;
	int bottom;
};


class TTexture;

class TWidget {
protected:
	TRect mouseRect;
	TVector2i position;
	bool active;
	bool visible;
public:
	bool focus;

	TWidget(int x, int y, int width, int height);
	virtual void Draw() const = 0;
	virtual bool Click(int x, int y);
	virtual void TextEnter(char text) {}
	virtual void Key(sf::Keyboard::Key key, unsigned int mod, bool released) {}
	virtual void MouseMove(int x, int y);
	virtual void Focussed() {}
	bool focussed() const { return focus; }
	void SetActive(bool a) { active = a; if (!a) focus = false; }
	void SetVisible(bool v) { visible = v; if (!v) focus = false; }
	bool GetActive() const { return active; }
	bool GetVisible() const { return visible; }
};

class TTextButton : public TWidget {
	sf::Text text;
public:
	TTextButton(int x, int y, const sf::String& text_, float ftsize);
	void Focussed();
	void Draw() const;
};
TTextButton* AddTextButton(const sf::String& text, int x, int y, float ftsize);
TTextButton* AddTextButtonN(const sf::String& text, int x, int y, int rel_ftsize);

class TTextField : public TWidget {
	sf::String text;
	size_t cursorPos;
	size_t maxLng;
	double time;
	bool cursor;
public:
	TTextField(int x, int y, int width, int height, const sf::String& text_);
	void Draw() const;
	void TextEnter(char text);
	void Key(sf::Keyboard::Key key, unsigned int mod, bool released);
	void UpdateCursor(double timestep);
	const sf::String& Text() const { return text; }
};
TTextField* AddTextField(const sf::String& text, int x, int y, int width, int height);

class TCheckbox : public TWidget {
	sf::Text text;
	sf::Sprite back, checkmark;
public:
	bool checked;

	TCheckbox(int x, int y, int width_, const sf::String& tag_);
	void Draw() const;
	void Focussed();
	bool Click(int x, int y);
	void Key(sf::Keyboard::Key key, unsigned int mod, bool released);
};
TCheckbox* AddCheckbox(int x, int y, int width, const sf::String& tag);

class TIconButton : public TWidget {
	double size;
	sf::Sprite sprite;
	int maximum;
	int value;
public:
	TIconButton(int x, int y, const sf::Texture& texture, double size_, int max_, int value_);
	int GetValue() const { return value; }
	void SetValue(int _value);
	void Draw() const;
	bool Click(int x, int y);
	void Key(sf::Keyboard::Key key, unsigned int mod, bool released);
};
TIconButton* AddIconButton(int x, int y, const sf::Texture& texture, double size, int maximum, int value);

class TArrow : public TWidget {
	sf::Sprite sprite;
	void SetTexture();
public:
	TArrow(int x, int y, bool down_);
	bool down;
	void Focussed();
	void Draw() const;
};
TArrow* AddArrow(int x, int y, bool down);

class TUpDown : public TWidget {
	TArrow up;
	TArrow down;
	int value;
	int minimum;
	int maximum;
public:
	TUpDown(int x, int y, int min_, int max_, int value_, int distance);
	int GetValue() const { return value; }
	void SetValue(int value_);
	void SetMinimum(int min_);
	void SetMaximum(int max_);
	void Draw() const;
	bool Click(int x, int y);
	void Key(sf::Keyboard::Key key, unsigned int mod, bool released);
	void MouseMove(int x, int y);
};
TUpDown* AddUpDown(int x, int y, int minimum, int maximum, int value, int distance = 2);

// --------------------------------------------------------------------

void DrawGUI();
TWidget* ClickGUI(int x, int y);
TWidget* MouseMoveGUI(int x, int y);
TWidget* KeyGUI(sf::Keyboard::Key key, unsigned int mod, bool released);
TWidget* TextEnterGUI(char text);
void SetFocus(TWidget* widget);
void IncreaseFocus();
void DecreaseFocus();
void ResetGUI();

// --------------------------------------------------------------------

void DrawFrameX (int x, int y, int w, int h, int line,
                 const TColor& backcol, const TColor& framecol, double transp);
void DrawBonusExt (int y, size_t numraces, size_t num);
void DrawGUIBackground(float scale);
void DrawGUIFrame();
void DrawCursor();

// --------------------------------------------------------------------

int AutoYPosN (double percent);
TArea AutoAreaN (double top_perc, double bott_perc, int w);

#endif
