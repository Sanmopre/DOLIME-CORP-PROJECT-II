#include "j1App.h"
#include <vector>
#include "Particle.h"
#include "j1Render.h"
#include "random.h"
#include "p2Log.h"

using namespace std;

Particle::Particle()
{
	active = false;
}

void Particle::Update(float dt)
{
	pVelocity += pAcceleration;
	pLocation += pVelocity;

	pRect.x = pLocation.x;
	pRect.y = pLocation.y;

	remainingLifetime -= pLifetimeSubstraction;

	if (remainingLifetime < 0)
	{
		loadProperties(Props);
		remainingLifetime = lifespan;
	}

	Draw();
}

void Particle::loadProperties(ParticleProps properties)
{
	pLocation = properties.Location;
	pVelocity = properties.Velocity;
	pAcceleration = properties.Acceleration;
	lifespan = properties.lifetime;
	pRect = properties.rect;
	pType = properties.type;
	remainingLifetime = lifespan;
	pLifetimeSubstraction = properties.lifetimeSubstraction;
	Props = properties;

	if (pType == PARTICLE_TYPES::CLOUD)
	{
		pLocation = { (pLocation.x + (float)(50 * (Random::Randomize() - 0.5))), (pLocation.y + (float)(50 * (Random::Randomize() - 0.5))) };
		pRect.w = pRect.w + (170 * (Random::Randomize() - 0.5));
		pRect.h = pRect.w;
	}

	if (pType == PARTICLE_TYPES::EXPLOSION)
		pVelocity = { ((float)(Random::Randomize() - 0.5)), ((float)(Random::Randomize()) * (-1)) };

	if (pType == PARTICLE_TYPES::TEST)
		pVelocity = { (float)(Random::Randomize() - 0.5), (float)(Random::Randomize() - 0.5) };

}

void Particle::switchParticleState()
{
	if (active)
	{
		active = false;
	}
	else
	{
		active = true;
	}
}

bool Particle::Draw()
{
	if (pType == PARTICLE_TYPES::CLOUD)
	{
		App->render->AddBlitEvent(2, nullptr, pLocation.x, pLocation.y, pRect, false, 0.0f, 216, 237, 244, remainingLifetime);
	}
	else
		App->render->AddBlitEvent(2, nullptr, pLocation.x, pLocation.y, pRect, false, 0.0f, 255, 0, 0, remainingLifetime);

	return true;
}