#ifndef __j1GUIELEMENT_H__
#define __j1GUIELEMENT_H__

#include "j1Module.h"
#include "p2Point.h"
//#include "p2Animation.h"
#include "p2Log.h"
#include "SDL/include/SDL_render.h"

enum class GUItype
{
	GUI_ERROR,
	GUI_BUTTON,
	GUI_INPUTBOX,
	GUI_SCROLLBAR,
	GUI_LABEL,
	GUI_IMAGE,
	GUI_MAX
};

enum class SCROLL_TYPE
{
	SCROLL_NONE,
	SCROLL_MUSIC,
	SCROLL_FX,
	SCROLL_BASIC,
	SCROLL_MAX
};


class j1GUIelement
{

public:

	j1GUIelement() { };
	~j1GUIelement();

	virtual bool Awake(pugi::xml_node& config) { return true; };
	virtual bool Start();
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };

	virtual bool CleanUp() { return true; };
	virtual bool Save(pugi::xml_node& file) const { return true; };
	virtual bool Load(pugi::xml_node& file) { return true; };

	virtual void OnClick();
	virtual void OnRelease();
	virtual void Dragging();
	virtual void PrintText(const char* txt);

	void Draw();
	bool OnAbove();

public:

	j1GUIelement* parent = nullptr;
	j1Module* listener = nullptr;
	GUItype type = GUItype::GUI_ERROR;

	iPoint globalPosition = { 0,0 };
	iPoint localPosition = { 0,0 };

	SDL_Color color = { 255,255,255 };
	char* text = nullptr;

	bool enabled = false;
	bool interactable = false;
	bool X_drag = false;
	bool Y_drag = false;
	bool above = false;
	bool focus = false;
	bool decorative = false;

	SDL_Rect rect = { 0,0,0,0 };
	SDL_Texture* texture = nullptr;

};

#endif // __j1GUIELEMENT_H__
