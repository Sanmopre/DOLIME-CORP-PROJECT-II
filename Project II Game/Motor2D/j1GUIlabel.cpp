#include "j1GUIlabel.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"

j1GUIlabel::j1GUIlabel() 
{
	this->type = GUItype::GUI_LABEL;	 
}

j1GUIlabel::~j1GUIlabel() {}

bool j1GUIlabel::Start()
{		
	if(text && strlen(text)>2)
		texture = App->fonts->Print(text, color);

	return true;
}

bool j1GUIlabel::PostUpdate()
{		
	if (enabled && texture!=nullptr) 			
	//	App->render->Blit(texture, globalPosition.x + localPosition.x, globalPosition.y + localPosition.y, nullptr, 0.0f);			

	return true;
}

bool j1GUIlabel::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}
