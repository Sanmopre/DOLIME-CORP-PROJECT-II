#include "j1Entities.h"

#include "j1Render.h"
#include "j1Input.h"
#include "j1EntityManager.h"
#include "j1ParticleManager.h"
#include "j1InGameUI.h"
#include "j1Window.h"

#include "j1Player.h"
#include <vector>

j1Harvester::j1Harvester(float x, float y, int level, int team)
{
	type = EntityType::HARVESTER;
	terrain = NodeType::WATER;//GROUND;

	state = NOT_BUILDING;
	position.x = x;
	position.y = y;
	destination = position;
	this->level = level;
	trading_range = 50;
	this->team = team;
	speed = 30;
	range = 50;
	power = 30;
	harvestrate = { 2 };
	transferrate = { 0.5 };
	max_health = 50;
	health = max_health;
	load = { 0, 0, 0, 500 };
	target = nullptr;

	automatic = false;
	automating = false;

	for (std::vector<Animation>::iterator i = App->entitymanager->allAnimations.begin(); i != App->entitymanager->allAnimations.end(); i++)
	{
		if (strcmp("entity_eight_north", i->name) == 0)
			north = i->GetAnimation();
		if (strcmp("entity_eight_northeast", i->name) == 0)
			north_east = i->GetAnimation();
		if (strcmp("entity_eight_east", i->name) == 0)
			east = i->GetAnimation();
		if (strcmp("entity_eight_southeast", i->name) == 0)
			south_east = i->GetAnimation();
		if (strcmp("entity_eight_south", i->name) == 0)
			south = i->GetAnimation();
		if (strcmp("entity_eight_southwest", i->name) == 0)
			south_west = i->GetAnimation();
		if (strcmp("entity_eight_west", i->name) == 0)
			west = i->GetAnimation();
		if (strcmp("entity_eight_northwest", i->name) == 0)
			north_west = i->GetAnimation();
	}

	for (std::vector<TextureInfo>::iterator e = App->entitymanager->allTextures.begin(); e != App->entitymanager->allTextures.end(); e++)
		if (type == e->type && level == e->level)
		{
			texture = e->texture;
			break;
		}

	rect = north.GetCurrentFrame();
	App->audio->PlayFx(App->audio->harvester_spawn);
}

j1Harvester::~j1Harvester()
{

}

void j1Harvester::Update(float dt)
{
	if (dt != 0.0f)
	{
		if (selected)
		{
			if (team == 1)
			{
				if (!automating && state != TO_BUILD)
					if (App->input->GetMouseButtonDown(3) == KEY_DOWN)
					{
						if (App->entitymanager->selected_n == 1)
							SetDestination(NodeType::ALL);
						automatic = false;
					}

				if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
				{
					SetAutomatic();
				}
				if (App->godmode)
				{
					if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN && state == NOT_BUILDING)
					{
						BuildStructure(EntityType::BOATHOUSE);
					}
					if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN && state == NOT_BUILDING)
					{
						BuildStructure(EntityType::STORAGE);
					}
				}
			}
		}

		float distance = 0.0f;
		Color c = {};
		SDL_Rect r = {};
		if (building != nullptr)
		{

			float x = building->position.x - position.x;
			float y = building->position.y - position.y;
			distance = sqrtf(x * x + y * y);
		}

		switch (state)
		{
		case TO_BUILD:
			building->Primitive_Update(dt);
			building->Update(dt);
			c.Red();
			App->player->disable_click = true;
			if (distance < range)
			{
				if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
				{
					state = BUILDING;
					float x = building->position.x;
					float y = building->position.y;
					EntityType ty = building->type;
					int l = building->level;
					int t = building->team;

					delete building;

					building = App->entitymanager->AddEntity(x, y, ty, l, t);
					building->SetBuiltState(BUILDING);
				}
				c.Green();
			}
			if (App->input->GetMouseButtonDown(3) == KEY_DOWN)
			{
				state = NOT_BUILDING;
				delete building;
				building = nullptr;
			}
			r = { building->GetRenderPositionX(), building->GetRenderPositionY(), building->rect.w, building->rect.h };
			App->render->AddBlitEvent(3, nullptr, 0, 0, r, false, false, c.r, c.g, c.b, 50);
			break;
		case BUILDING:
			if (distance < range)
				if (building->health < building->max_health)
					building->health += dt * 100 * level;
				else
				{
					building->health = building->max_health;
					building->SetBuiltState(NOT_BUILDING);
					state = NOT_BUILDING;
					building = nullptr;
				}
		break;
		}

		if (state != NOT_BUILDING)
		{
			automatic = false;
			automating = false;
			harvest_destination = {};
			deposit_destination = {};
		}

		if (automating)
			if (App->input->GetMouseButtonDown(3) == KEY_DOWN)
				if (harvest_destination == position)
				{
					iPoint m;
					App->input->GetMousePosition(m.x, m.y);
					m.x -= App->render->camera.x / App->win->GetScale();
					m.y -= App->render->camera.y / App->win->GetScale();
					if (FindTarget((float)m.x, (float)m.y, range, EntityType::STORAGE, team) != nullptr)
					{
						deposit_destination = { (float)m.x, (float)m.y };
						automating = false;
						automatic = true;
					}
					else
					{
						harvest_destination = {};
						automating = false;
					}
				}
				else if (deposit_destination == position)
				{
					iPoint m;
					App->input->GetMousePosition(m.x, m.y);
					m.x -= App->render->camera.x / App->win->GetScale();
					m.y -= App->render->camera.y / App->win->GetScale();
					if (SearchResources((float)m.x, (float)m.y) != nullptr)
					{
						harvest_destination = { (float)m.x, (float)m.y };
						automating = false;
						automatic = true;
					}
					else
					{
						deposit_destination = {};
						automating = false;
					}
				}

		if (automatic)
		{
			if (harvest_destination == position)
			{
				target = SearchResources(position.x, position.y);

				if (target != nullptr)
					if (load.Weight() == load.maxweight || target->load.Total() == 0)
					{
						GoTo(deposit_destination, NodeType::ALL);
						if (target->load.Total() == 0)
							automatic = false;
					}
			}
			else if (deposit_destination == position)
			{
				target = FindTarget(position.x, position.y, range, EntityType::STORAGE, team);

				if (target != nullptr)
					if (load.Total() == 0 || target->load.Total() == target->load.maxweight)
					{
						GoTo(harvest_destination, NodeType::ALL);
						if (target->load.Total() == target->load.maxweight)
							automatic = false;
					}
			}
		}

		if (destination != position)
			Move(dt);
		else
		{
			NextStep();

			if (!automatic)
			{
				if (load.Total() != load.maxweight)
					target = SearchResources(position.x, position.y);
				if (target == nullptr)
				{
					target = FindTarget(position.x, position.y, range, EntityType::STORAGE, team);
				}
			}

			if (target != nullptr)
			{
				if (target->main_type == EntityType::RESOURCE)
				{
					harvestrate.counter += dt;
					if (harvestrate.counter >= harvestrate.iterations)
					{
						Harvest(power, target);
						harvestrate.counter = 0;
					}
				}
				else if (target->type == EntityType::STORAGE)
				{
					transferrate.counter += dt;
					if (transferrate.counter >= transferrate.iterations)
					{
						if (load.wood != 0)
							target->load.Transfer(WOOD, &load.wood, 20);
						else if (load.cotton != 0)
							target->load.Transfer(COTTON, &load.cotton, 20);
						else if (load.metal != 0)
							target->load.Transfer(METAL, &load.metal, 20);
						transferrate.counter = 0;
					}
				}
			}
		}
	}

	App->render->AddBlitEvent(1, texture, GetRenderPositionX(), GetRenderPositionY(), rect);
	
	if (health == 0) {
		CleanUp();
		App->audio->PlayFx(App->audio->harvester_destroy);
	}
}

void j1Harvester::CleanUp()
{
	path.erase(path.begin(), path.end());
	path.shrink_to_fit();
	to_delete = true;
}

void j1Harvester::SetAutomatic()
{
	if (!automatic)
	{
		automating = true;
		if (SearchResources(position.x, position.y) != nullptr)
			harvest_destination = position;
		else if (FindTarget(position.x, position.y, range, EntityType::STORAGE, team) != nullptr)
			deposit_destination = position;
		else
			automating = false;
	}
	else
		automatic = false;
}

void j1Harvester::BuildStructure(EntityType type)
{
	switch (type)
	{
	case EntityType::BOATHOUSE:
		building = new j1BoatHouse(0, 0, team);
		break;
	case EntityType::STORAGE:
		building = new j1Storage(0, 0, team);
		break;
	}
	
	if (building != nullptr)
	{
		building->SetBuiltState(TO_BUILD);
		building->ToPlace(false);
		state = TO_BUILD;
	}
}

void j1Harvester::Harvest(int power, j1Entity* target)
{
	if (target->load.wood != 0)
	{
		load.Transfer(WOOD, &target->load.wood, power);
	}
	else if (target->load.cotton != 0)
	{
		load.Transfer(COTTON, &target->load.cotton, power);
	}
	else if (target->load.metal != 0)
	{
		load.Transfer(METAL, &target->load.metal, power);
	}
	App->audio->PlayFx(App->audio->harvester_work);
}

j1Entity* j1Harvester::SearchResources(float x, float y)
{
	j1Entity* ret = nullptr;

	float targetdistance = range;
	float distance = 0.0f;

	for (std::vector<j1Entity*>::iterator e = App->entitymanager->entities.begin(); e != App->entitymanager->entities.end(); e++)
		if (*e != this && (*e)->main_type == EntityType::RESOURCE && (*e)->load.Total() > 0)
				if (x + range > (*e)->position.x &&
					x - range < (*e)->position.x &&
					y + range >(*e)->position.y &&
					y - range < (*e)->position.y)
				{
					distance = sqrtf((x - (*e)->position.x) * (x - (*e)->position.x) + (y - (*e)->position.y) * (y - (*e)->position.y));

					if (distance < range && distance < targetdistance)
					{
						ret = *e;
						targetdistance = distance;
					}

					ShowHPbar(10, 5);
				}

	if (distance == 0.0f)
		ret = nullptr;

	return ret;
}