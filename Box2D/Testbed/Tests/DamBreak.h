/*
* Copyright (c) 2013 Google, Inc.
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/
#ifndef DAM_BREAK_H
#define DAM_BREAK_H
#include <string>

class DamBreak : public Test
{
	private: 
		b2RevoluteJoint* joints[2];

public:

	DamBreak()
	{

		//grid
		{
			b2BodyDef bd;
			b2Body* ground = m_world->CreateBody(&bd);

			b2ChainShape shape;
			const b2Vec2 vertices[4] = {
				b2Vec2(-2, 0),
				b2Vec2(2, 0),
				b2Vec2(2, 4),
				b2Vec2(-2, 4)};
			shape.CreateLoop(vertices, 4);
			ground->CreateFixture(&shape, 0.0f);

		}

		m_particleSystem->SetRadius(0.025f);
		m_particleSystem->SetDamping(0.2f);

		//water
		{
			b2PolygonShape shape;
			shape.SetAsBox(2.0f, 1.0f, b2Vec2(0.0f, 1.1f), 0);
			b2ParticleGroupDef pd;
			pd.flags = TestMain::GetParticleParameterValue();
			pd.shape = &shape;
			b2ParticleGroup * const group = m_particleSystem->CreateParticleGroup(pd);
			if (pd.flags & b2_colorMixingParticle)
			{
				ColorParticleGroup(group, 0);
			}
		}


		//fishy
		{
			
		}

		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			b2Body* body = m_world->CreateBody(&bd);
			b2CircleShape headShape;
			headShape.m_p.Set(0, 3.0f);
			headShape.m_radius = 0.1f;
			body->CreateFixture(&headShape, 1.5f);
			m_particleSystem->DestroyParticlesInShape(headShape,
				body->GetTransform());

			b2PolygonShape shape;
			shape.SetAsBox(0.1, 0.01f);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1.0f;

			b2RevoluteJointDef jd;

			jd.enableMotor = true;
			jd.motorSpeed = 500.0f;
			jd.maxMotorTorque = 10000.0f;
			

			b2Body* prevBody = body;
			for (int32 i = 1; i <= 2; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(0.2f* i, 3.0f);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&fd);

				b2Vec2 anchor(0.2f* i + -0.1f, 3.0f);
				jd.Initialize(prevBody, body, anchor);
				joints[i - 1] = (b2RevoluteJoint*)(m_world->CreateJoint(&jd));

				prevBody = body;
			}
		}

	}

	float32 GetDefaultViewZoom() const
	{
		return 0.1f;
	}


	float32 time = 0.0f;

	void Step(Settings* settings)
	{
		Test::Step(settings);

		float32 timeStep = settings->hz > 0.0f ? 1.0f / settings->hz : float32(0.0f);
		time = time + timeStep;

		for (int32 i = 1; i <= 2; ++i) {
			b2RevoluteJoint* joint = joints[i - 1];
			float result = (cosf(6.0f * time)) *20;
			b2Log(std::to_string(time).c_str());
			b2Log(" ");
			joint->SetMotorSpeed(result);
		}
	}

	static Test* Create()
	{
		return new DamBreak;
	}
};

#endif
