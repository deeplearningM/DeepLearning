#include "Physics.hpp"


class CarRayCallback : public b2RayCastCallback
{
public:
	CarRayCallback()
	{
		m_hit = false;
	}

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
	{
		m_hit = true;
		m_point = point;
		fixtureA = fixture;
		normalA = normal;
		return fraction;
	}

	bool m_hit;
	b2Vec2 m_point;
	b2Fixture* fixtureA;
	b2Vec2 normalA;
};




Physics::Physics(b2World* world) : world(world)
{
	
}


std::vector<float> Physics::updateRays(b2Body& carBody, int size, int degrees) {

	std::vector<float> distances;
	// Rays for checking the distance
	// Initialized with starting and ending points
	
	float rayLenght = 200;

	for (int i = -(size - 1) / 2; i <= (size - 1) / 2; i++) {

		CarRayCallback callback;

		b2Vec2 rayStart = carBody.GetWorldPoint(b2Vec2(0, 0));

		float y = rayLenght*cos((90 + i*degrees / (size - 1))*DEGTORAD);
		float x = rayLenght*sin((90 + i*degrees / (size - 1))*DEGTORAD);

		b2Vec2 rayEnd = carBody.GetWorldPoint(b2Vec2(x, y));

		world->RayCast(&callback, rayStart, rayEnd);

		if (callback.m_hit) {
			distances.push_back((rayStart - callback.m_point).Length()-20);
		}
		else {
			distances.push_back(rayLenght);
		}
	}


	return distances;
}

void Physics::updateFriction(b2Body* carBody) {
	//lateral linear velocity
	b2Vec2 impulse = carBody->GetMass() * -getLateralVelocity(carBody);
	if (impulse.Length() > maxLateralImpulse){
		impulse *= maxLateralImpulse / impulse.Length();
	}
	carBody->ApplyLinearImpulse(impulse, carBody->GetWorldCenter(), true);

	//angular velocity
	carBody->ApplyAngularImpulse(1.0f * carBody->GetInertia() * -carBody->GetAngularVelocity(), true);

	//forward linear velocity
	b2Vec2 currentForwardNormal = getForwardVelocity(carBody);
	float dragForceMagnitude = -0.1 * 120;
	carBody->ApplyForce(dragForceMagnitude * currentForwardNormal, carBody->GetWorldCenter(), true);
}

b2Vec2 Physics::getForwardVelocity(b2Body* carBody) const {
	b2Vec2 currentForwardNormal = carBody->GetWorldVector(b2Vec2(1, 0));
	return b2Dot(currentForwardNormal, carBody->GetLinearVelocity()) * currentForwardNormal;
}

b2Vec2 Physics::getLateralVelocity(b2Body* carBody) const {
	b2Vec2 currentRightNormal = carBody->GetWorldVector(b2Vec2(0, 1));
	return b2Dot(currentRightNormal, carBody->GetLinearVelocity()) * currentRightNormal;
}
