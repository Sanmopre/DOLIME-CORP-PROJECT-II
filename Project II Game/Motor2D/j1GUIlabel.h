#ifndef __J1GUILABEL__
#define __J1GUILABEL__

#include "j1Module.h"
#include"j1GUIelement.h"
#include "j1GUI.h"

struct SDL_Texture;

class j1GUIlabel : public j1GUIelement
{
public:

	j1GUIlabel();
	~j1GUIlabel();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&) { return true; };
	bool Save(pugi::xml_node&) const  const { return true; };
private: 	
	int font_name = -1;
};

#endif // !__J1GUILABEL__

