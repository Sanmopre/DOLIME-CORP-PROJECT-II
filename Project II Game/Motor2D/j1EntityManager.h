#ifndef __j1EntityManager_H__
#define __j1EntityManager_H__

#include "j1Module.h"
#include "j1Entities.h"
#include <vector>

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	j1Entity* AddEntity(float x = 0, float y = 0, EntityType = EntityType::NONE, int level = 1, int team = 0);

	void DeleteEntity(j1Entity*);
	void DeleteAll();

	std::vector<j1Entity*> entities;
	std::vector<j1Entity*> selected_units;

	int selected_n;

private:

	std::vector<j1Entity*> buffer;
	void QuickDeleteEntity(std::vector<j1Entity*>::iterator itr);

};

#endif // __j1EntityManager_H__
