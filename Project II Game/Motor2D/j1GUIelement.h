#ifndef __j1GUIELEMENT_H__
#define __j1GUIELEMENT_H__

#include "j1Module.h"
#include "p2Point.h"
//#include "J1Animation.h"
#include "p2Log.h"
#include "SDL/include/SDL_render.h"

enum class GUItype
{
	GUI_ERROR,
	GUI_BUTTON,
	GUI_INPUTBOX,
	GUI_SCROLLBAR,
	GUI_LABEL,
	GUI_IMAGE
};

enum class SCROLL_TYPE
{
	SCROLL_NONE,
	SCROLL_MUSIC
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

	bool OnAbove();
	virtual void OnClick();
	virtual void OnRelease();
	virtual void Dragging();
	void Draw();

public:

	j1GUIelement* parent = nullptr;
	j1Module* listener = nullptr;
	GUItype type = GUItype::GUI_ERROR;
	iPoint Map_Position = { 0,0 };
	iPoint Inside_Position = { 0,0 };

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
