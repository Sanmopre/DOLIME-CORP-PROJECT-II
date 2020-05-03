#include "ParticleSystem.h"
#include "j1ParticleManager.h"
#include "j1App.h"
#include "j1Textures.h"
#include "random.h"
#include "j1Render.h"
#include "p2Log.h"

ParticleSystem::ParticleSystem(PARTICLE_TYPES _type, p2Point<float> location, int index, float _timer)
{
	systemProps.Location = location;
	systemType = _type;
	timer = _timer;
	toDelete = false;

	awakeningCounter = 0;
	allParticlesAwake = false;

	if (_timer == 0)
		timeActive = false;
	else
		timeActive = true;

	loadSystem();
	activateSystem(index);
}

ParticleSystem::~ParticleSystem()
{
	deactivateAllParticles();
}

void ParticleSystem::loadSystem()
{
	systemProps.type = systemType;

	if (systemProps.type == PARTICLE_TYPES::CLOUD)
	{
		systemProps.Velocity = { -0.25, 0 };
		systemProps.lifetime = 60;
		systemProps.Acceleration = { 0, 0 };
		systemProps.rect = { 0, 0, 200, 200 };
		systemProps.lifetimeSubstraction = 0;
		systemProps.tex = App->pmanager->cloudTexture;
		numberOfParticles = 3;
	}
	else if (systemProps.type == PARTICLE_TYPES::SMOKE)
	{
		systemProps.lifetime = 255;
		systemProps.Acceleration = { 0, (-0.0015f) };
		systemProps.rect = { 0, 0, 7, 7 };
		systemProps.lifetimeSubstraction = 0.5;
		systemProps.tex = App->pmanager->smokeTexture;
		numberOfParticles = 6;
		systemProps.AwakeningDelay = 1;

	}
	else if (systemProps.type == PARTICLE_TYPES::FIRE)
	{
		systemProps.lifetime = 255;
		systemProps.Acceleration = { 0, (-0.001f) };
		systemProps.rect = { 0, 0, 7, 7 };
		systemProps.lifetimeSubstraction = 1.;
		systemProps.tex = App->pmanager->fireTexture;
		numberOfParticles = 6;
		systemProps.AwakeningDelay = 1;
	}
	else if (systemProps.type == PARTICLE_TYPES::EXPLOSION)
	{
		systemProps.lifetime = 255;
		systemProps.Acceleration = { 0, 0.01 };
		systemProps.rect = { 0, 0, 7, 7 };
		systemProps.lifetimeSubstraction = 3;
		systemProps.tex = App->pmanager->explosionTexture;
		numberOfParticles = 20;
	}
	else if (systemProps.type == PARTICLE_TYPES::DUST)
	{
		systemProps.lifetime = 255;
		systemProps.Acceleration = { 0, -0.0005 };
		systemProps.rect = { 0, 0, 25, 20 };
		systemProps.lifetimeSubstraction = 1;
		systemProps.tex = App->pmanager->dustTexture;
		numberOfParticles = 3;
	}
	else
	{
		systemProps.lifetime = 255;
		systemProps.Acceleration = { 0, 0.01 };
		systemProps.rect = { 0, 0, 7, 7 };
		systemProps.lifetimeSubstraction = 3;
	}
}

bool ParticleSystem::activateSystem(int index)
{
	Particle* pReference = nullptr;
	int counter = 0;
	int newIndex = index;

	while (counter < numberOfParticles)
	{
		if (App->pmanager->particlePool[newIndex].active == true || newIndex >= 1499)
		{
			if (App->pmanager->updateIndex())
				newIndex = App->pmanager->getIndex();
		}

		pReference = &(App->pmanager->particlePool[newIndex]);
		pReference->loadProperties(systemProps);
		pReference->switchParticleState();

		if (systemProps.AwakeningDelay == 0)
		{
			pReference->awake = true;
		}

		*(referencesArray + counter) = pReference; //asigno la referencia de la particula cargada al lugar que le corresponde de 

		newIndex++;
		counter++;
	}

	if (systemProps.AwakeningDelay == 0)
	{
		allParticlesAwake = true;
	}

	App->pmanager->updateIndex();

	return true;
}

void ParticleSystem::Update(float dt)
{
	if (timeActive == true)
	{
		timer -= dt;

		if (timer <= 0)
		{
			toDelete = true;
		}
	}

	if (allParticlesAwake != true && systemProps.AwakeningDelay != 0)
	{
		systemProps.AwakeningDelay -= dt;

		if (systemProps.AwakeningDelay <= 0)
		{
			if (awakeningCounter < numberOfParticles)
			{
				Particle* particle = *(referencesArray + awakeningCounter);
				particle->awake = true;
				LOG("particle active");

				awakeningCounter++;
			}
			else
				allParticlesAwake = true;

			systemProps.AwakeningDelay = 1;
		}
	}
}

void ParticleSystem::deactivateParticle(Particle* particle)
{
	particle->switchParticleState();
}

void ParticleSystem::deactivateAllParticles()
{
	Particle* particle = nullptr;
	for (int i = 0; i < numberOfParticles; i++)
	{
		particle = *(referencesArray + i);
		particle->switchParticleState();
	}
}

void ParticleSystem::changePosition(fPoint location)
{
	Particle* particle = nullptr;
	for (int i = 0; i < numberOfParticles; i++)
	{
		particle = *(referencesArray + i);
		particle->Props.Location = location;
	}
}