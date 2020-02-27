#include "j1GUIimage.h"
#include "j1App.h"
#include "j1Render.h"

j1GUIimage::j1GUIimage() {

	this->type = GUItype::GUI_IMAGE;
	texture = App->gui->GetAtlasTexture();
}

j1GUIimage::~j1GUIimage() {

}


bool j1GUIimage::PostUpdate()
{	
	 if(enabled)
		Draw();

	return true;
}

bool j1GUIimage::CleanUp()
{
	return true;
}

