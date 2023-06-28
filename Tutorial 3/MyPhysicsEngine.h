#pragma once

#include "BasicActors.h"
#include <iostream>
#include <iomanip>

namespace PhysicsEngine
{
	using namespace std;

	//a list of colours: Circus Palette
	static const PxVec3 color_palette[] = {PxVec3(46.f/255.f,9.f/255.f,39.f/255.f),PxVec3(217.f/255.f,0.f/255.f,0.f/255.f),
		PxVec3(255.f/255.f,45.f/255.f,0.f/255.f),PxVec3(255.f/255.f,140.f/255.f,54.f/255.f),PxVec3(4.f/255.f,117.f/255.f,111.f/255.f)};

	//pyramid vertices
	static PxVec3 pyramid_verts[] = {PxVec3(0,1,0), PxVec3(1,0,0), PxVec3(-1,0,0), PxVec3(0,0,1), PxVec3(0,0,-1)};

	//wedge vertices
	static PxVec3 wedge_verts[] = {PxVec3(0,1,2), PxVec3(-2,1,0), PxVec3(2,0,0), PxVec3(-2,0,0), PxVec3(0,0,2), PxVec3(0,0,-2)};

	//hexagon vertices
	//static PxVec3 hex_verts[] = { PxVec3(1,0,1.732f), PxVec3(2,0,0), PxVec3(1,0,-1.732f), PxVec3(-1,0,-1.732f), PxVec3(-2,0,0), PxVec3(-1,0,1.732f),
	//							PxVec3(1,1,1.732f), PxVec3(2,1,0), PxVec3(1,1,-1.732f), PxVec3(-1,1,-1.732f), PxVec3(-2,1,0), PxVec3(-1,1,1.732f) };
	static PxVec3 hex_verts[] = { PxVec3(.5f,0,.866f), PxVec3(1,0,0), PxVec3(.5f,0,-.866), PxVec3(-.5f,0,-.866f), PxVec3(-1,0,0), PxVec3(-.5f,0,.866f),
								PxVec3(.5f,.2f,.866f), PxVec3(1,.2f,0), PxVec3(.5f,.2f,-.866f), PxVec3(-.5f,.2f,-.866f), PxVec3(-1,.2f,0), PxVec3(-.5f,.2f,.866f) };

	//pyramid triangles: a list of three vertices for each triangle e.g. the first triangle consists of vertices 1, 4 and 0
	//vertices have to be specified in a counter-clockwise order to assure the correct shading in rendering
	static PxU32 pyramid_trigs[] = {1, 4, 0, 3, 1, 0, 2, 3, 0, 4, 2, 0, 3, 2, 1, 2, 4, 1};
	static PxU32 wedge_trigs[] = {3, 0, 1, 3, 4, 0, 3, 5, 1, 1, 0, 5, 2, 5, 0, 4, 2, 0, 4, 3, 2, 5, 2, 3};

	class Pyramid : public ConvexMesh
	{
	public:
		Pyramid(PxTransform pose=PxTransform(PxIdentity), PxReal density=1.f) :
			ConvexMesh(vector<PxVec3>(begin(pyramid_verts),end(pyramid_verts)), pose, density)
		{
		}
	};

	class PyramidStatic : public TriangleMesh
	{
	public:
		PyramidStatic(PxTransform pose=PxTransform(PxIdentity)) :
			TriangleMesh(vector<PxVec3>(begin(pyramid_verts),end(pyramid_verts)), vector<PxU32>(begin(pyramid_trigs),end(pyramid_trigs)), pose)
		{
		}
	};

	class Wedge : public ConvexMesh
	{
	public:
		Wedge(PxTransform pose = PxTransform(PxIdentity), PxReal density = 1.f) :
			ConvexMesh(vector<PxVec3>(begin(wedge_verts), end(wedge_verts)), pose, density)
		{
		}
	};

	class WedgeStatic : public TriangleMesh
	{
	public:
		WedgeStatic(PxTransform pose = PxTransform(PxIdentity)) :
			TriangleMesh(vector<PxVec3>(begin(wedge_verts), end(wedge_verts)), vector<PxU32>(begin(wedge_trigs), end(wedge_trigs)), pose)
		{
		}
	};

	class Hexagon : public ConvexMesh
	{
	public:
		Hexagon(PxTransform pose = PxTransform(PxIdentity), PxReal density = 1.f) :
			ConvexMesh(vector<PxVec3>(begin(hex_verts), end(hex_verts)), pose, density)
		{
		}
	};

	struct FilterGroup
	{
		enum Enum
		{
			ACTOR0		= (1 << 0),
			ACTOR1		= (1 << 1),
			ACTOR2		= (1 << 2)
			//add more if you need
		};
	};

	///An example class showing the use of springs (distance joints).
	class Trampoline
	{
		vector<DistanceJoint*> springs;
		Hexagon* bottom, * top;
		PxMaterial* iron_on_iron = CreateMaterial(1.1f, .15f, .3f);
		PxMaterial* rubber_on_rubber = CreateMaterial(1.16f, .15f, 0.828f);

	public:
		Trampoline(const PxVec3& dimensions = PxVec3(1.f, 1.f, 1.f), PxReal stiffness = 15.f, PxReal damping = .5f)
		{
			PxReal thickness = .2f;
			bottom = new Hexagon(PxTransform(PxVec3(-3.f, 0.75, -4.f)), 7860.f);
			bottom->Color(PxVec3(192.f / 255.f, 192.f / 255.f, 192.f / 255.f));
			bottom->Material(iron_on_iron);

			top = new Hexagon(PxTransform(PxVec3(-3.f, 0.75 + dimensions.y + thickness, -4.f)));
			top->Color(PxVec3(.0f,.0f,.0f));
			bottom->Material(rubber_on_rubber);

			//bottom = new Box(PxTransform(PxVec3(-3.f, 0.75, -4.f)), PxVec3(dimensions.x, thickness, dimensions.z));
			//top = new Box(PxTransform(PxVec3(-3.f, 0.75 + dimensions.y + thickness, -4.f)), PxVec3(dimensions.x, thickness, dimensions.z));
			springs.resize(4);
			springs[0] = new DistanceJoint(bottom, PxTransform(PxVec3(dimensions.x, thickness, dimensions.z)), top, PxTransform(PxVec3(dimensions.x, -dimensions.y, dimensions.z)));
			springs[1] = new DistanceJoint(bottom, PxTransform(PxVec3(dimensions.x, thickness, -dimensions.z)), top, PxTransform(PxVec3(dimensions.x, -dimensions.y, -dimensions.z)));
			springs[2] = new DistanceJoint(bottom, PxTransform(PxVec3(-dimensions.x, thickness, dimensions.z)), top, PxTransform(PxVec3(-dimensions.x, -dimensions.y, dimensions.z)));
			springs[3] = new DistanceJoint(bottom, PxTransform(PxVec3(-dimensions.x, thickness, -dimensions.z)), top, PxTransform(PxVec3(-dimensions.x, -dimensions.y, -dimensions.z)));

			for (unsigned int i = 0; i < springs.size(); i++)
			{
				springs[i]->Stiffness(stiffness);
				springs[i]->Damping(damping);
			}
		}

		void AddToScene(Scene* scene)
		{
			scene->Add(bottom);
			scene->Add(top);
		}

		~Trampoline()
		{
			for (unsigned int i = 0; i < springs.size(); i++)
				delete springs[i];
		}
	};

	///A customised collision class, implemneting various callbacks
	class MySimulationEventCallback : public PxSimulationEventCallback
	{
	public:
		//an example variable that will be checked in the main simulation loop
		bool trigger;

		MySimulationEventCallback() : trigger(false) {}

		///Method called when the contact with the trigger object is detected.
		virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) 
		{
			//you can read the trigger information here
			for (PxU32 i = 0; i < count; i++)
			{
				//filter out contact with the planes
				if (pairs[i].otherShape->getGeometryType() != PxGeometryType::ePLANE)
				{
					//check if eNOTIFY_TOUCH_FOUND trigger
					if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
					{
						cerr << "onTrigger::eNOTIFY_TOUCH_FOUND" << endl;
						trigger = true;
					}
					//check if eNOTIFY_TOUCH_LOST trigger
					if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST)
					{
						cerr << "onTrigger::eNOTIFY_TOUCH_LOST" << endl;
						trigger = false;
					}
				}
			}
		}

		///Method called when the contact by the filter shader is detected.
		virtual void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs) 
		{
			cerr << "Contact found between " << pairHeader.actors[0]->getName() << " " << pairHeader.actors[1]->getName() << endl;

			//check all pairs
			for (PxU32 i = 0; i < nbPairs; i++)
			{
				//check eNOTIFY_TOUCH_FOUND
				if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				{
					cerr << "onContact::eNOTIFY_TOUCH_FOUND" << endl;
				}
				//check eNOTIFY_TOUCH_LOST
				if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_LOST)
				{
					cerr << "onContact::eNOTIFY_TOUCH_LOST" << endl;
				}
			}
		}

		virtual void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) {}
		virtual void onWake(PxActor **actors, PxU32 count) {}
		virtual void onSleep(PxActor **actors, PxU32 count) {}
#if PX_PHYSICS_VERSION >= 0x304000
		virtual void onAdvance(const PxRigidBody *const *bodyBuffer, const PxTransform *poseBuffer, const PxU32 count) {}
#endif
	};

	//A simple filter shader based on PxDefaultSimulationFilterShader - without group filtering
	static PxFilterFlags CustomFilterShader( PxFilterObjectAttributes attributes0,	PxFilterData filterData0,
		PxFilterObjectAttributes attributes1,	PxFilterData filterData1,
		PxPairFlags& pairFlags,	const void* constantBlock,	PxU32 constantBlockSize)
	{
		// let triggers through
		if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlags();
		}

		pairFlags = PxPairFlag::eCONTACT_DEFAULT;
		//enable continous collision detection
//		pairFlags |= PxPairFlag::eCCD_LINEAR;
		
		
		//customise collision filtering here
		//e.g.

		// trigger the contact callback for pairs (A,B) where 
		// the filtermask of A contains the ID of B and vice versa.
		if((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		{
			//trigger onContact callback for this pair of objects
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
//			pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;
		}

		return PxFilterFlags();
	};

	///Custom scene class
	class MyScene : public Scene
	{

		Plane* plane;
		StaticBox* staticBox;
		Box* box,* box2,* box3, * domino;
		Sphere* sphere,* sphere2;
		Trampoline* trampoline;
		WedgeStatic* wedgeStatic;
		//Wedge* wedge;
		MySimulationEventCallback* my_callback;
		Hexagon* hexagon,* hexagon2,* hexagon3;
		DivingBoard* divingBoard;
		CompoundObject* compoundObject;
		RevoluteJoint* revoluteJoint,* revoluteJoint2;
		//PhysicsEngine::Scene* scene;
		
	public:
		//specify your custom filter shader here
		//PxDefaultSimulationFilterShader by default
		MyScene() : Scene() {};

		///A custom scene class
		void SetVisualisation()
		{
			px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eBODY_LIN_VELOCITY, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
		}

		//Custom scene initialisation
		virtual void CustomInit() 
		{
			SetVisualisation();			

			GetMaterial()->setDynamicFriction(.2f);

			//list of materials
			PxMaterial* wood_on_wood = CreateMaterial(.3f, .2f, .603f); //static friction, dynamic friction, restitution
			PxMaterial* iron_on_iron = CreateMaterial(1.1f, .15f, .3f);
			PxMaterial* rubber_on_rubber = CreateMaterial(1.16f, .5f, 0.828f);
			PxMaterial* plastic_on_plastic = CreateMaterial(.3f, .2f, 0.688);
			PxMaterial * wood_on_concrete = CreateMaterial(.62f, .2f, 0.85);
			PxMaterial* iron_on_concrete = CreateMaterial(1.05f, .2f, .3f);

			///Initialise and set the customised event callback
			my_callback = new MySimulationEventCallback();
			px_scene->setSimulationEventCallback(my_callback);

			plane = new Plane();
			plane->Color(PxVec3(210.f/255.f,210.f/255.f,210.f/255.f));
			Add(plane);
			plane->Material(wood_on_concrete);

			sphere2 = new Sphere(PxTransform(PxVec3(-6.f, 11.f, -4.f)),.5f);
			sphere2->Color(PxVec3(1.f, 0.f, 0.f));
			Add(sphere2);
			sphere2->Material(rubber_on_rubber);
			//sphere->Get()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

			divingBoard = new DivingBoard(PxTransform(PxVec3(-10.f, 5.f, -4.f)));
			divingBoard->Color(PxVec3(1.f, 1.f, 1.f));
			Add(divingBoard);
			divingBoard->Material(iron_on_iron);

			wedgeStatic = new WedgeStatic(PxTransform(PxVec3(-3.f, 0.f, -4.f), PxQuat(PxPi / 4, PxVec3(0.f, -1.f, 0.f))));
			wedgeStatic->Color(PxVec3(1.f, 1.f, 1.f));
			Add(wedgeStatic);
			sphere2->Material(iron_on_concrete);

			//this sphere and the two boxes beneath are rotated so the resolute joint moves in the right direction when DriveVelocity() is on
			sphere = new Sphere(PxTransform(PxVec3(-8.f,13.f,-4.f), PxQuat(PxPi / 2, PxVec3(0.f, 1.f, 0.f))),1.5f);
			sphere->Color(PxVec3(0.f, 0.f, 1.f));

			box2 = new Box(PxTransform(PxVec3(-10.f,13.f, -4.f), PxQuat(PxPi / 2, PxVec3(0.f, 1.f, 0.f))));
			//box2 = new Box(PxTransform(PxVec3(-9.f, 13.f, -4.f)));
			box2->Color(PxVec3(105.f / 255.f, 105.f / 255.f, 105.f / 255.f));

			box3 = new Box(PxTransform(PxVec3(-8.f, 13.f, -4.f), PxQuat(PxPi / 2, PxVec3(0.f, 1.f, 0.f))));
			box3->Color(PxVec3(105.f / 255.f, 105.f / 255.f, 105.f / 255.f));

			//set collision filter flags
			//box->SetupFiltering(FilterGroup::ACTOR0, FilterGroup::ACTOR1);
			sphere->SetupFiltering(FilterGroup::ACTOR0, FilterGroup::ACTOR1 | FilterGroup::ACTOR2);
			box2->SetupFiltering(FilterGroup::ACTOR2, FilterGroup::ACTOR0 | FilterGroup::ACTOR1);
			box3->SetupFiltering(FilterGroup::ACTOR1, FilterGroup::ACTOR0 | FilterGroup::ACTOR2);

			Add(sphere);
			Add(box2);
			Add(box3);
			sphere->Material(plastic_on_plastic);
			box2->Material(plastic_on_plastic);
			box3->Material(plastic_on_plastic);

			//staticBox = new StaticBox(PxTransform(PxVec3(-9.f, 10.f, -4.f)), PxVec3(2.f, .2f, 2.f));
			//staticBox->Color(color_palette[1]);
			//Add(staticBox);

			//joint two boxes together
			//the joint is fixed to the centre of the boxes the sphere is attached 2 metres away from the sphere one on the y axis, the other on the x
			//and has the second object attached 5 meters away along the Y axis from the first object.
			revoluteJoint = new RevoluteJoint (sphere, PxTransform(PxVec3(0.f,-2.f,0.f)), box3, PxTransform(PxVec3(0.f,0.f,0.f)));
			revoluteJoint2 = new RevoluteJoint (sphere, PxTransform(PxVec3(0.f, 0.f, -2.f)), box2, PxTransform(PxVec3(0.f, 0.f, 0.f)));

			//testing
			//revoluteJoint->DriveVelocity(1.0f);
			//revoluteJoint2->DriveVelocity(1.0f);
			//box->Get()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			//box2->Get()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			//RevoluteJoint joint(nullptr, PxTransform(PxVec3(-6.f, 10.f, -4.f), PxQuat(PxPi / 2, PxVec3(0.f, 1.f, 0.f))), box2, PxTransform(PxVec3(0.f, 0.f, -2.f)));
			//RevoluteJoint joint2(nullptr, PxTransform(PxVec3(-2.f, 10.f, -4.f), PxQuat(PxPi / 2, PxVec3(0.f, 1.f, 0.f))), box2, PxTransform(PxVec3(0.f, 0.f, 2.f)));

			for (float i = 0.f; i < 51.f; i++)
			{
				domino = new Box(PxTransform(PxVec3((0.f + (i * 2)), 1.f, -4.f)), PxVec3(.2f, 2.f, .5f));
				domino->Color(PxVec3((255.f - (i * 5)) / 255.f, 0.f, (i * 5) / 255.f));
				Add(domino);
				domino->Material(wood_on_wood);
			}

			domino = new Box(PxTransform(PxVec3(102.f, 1.f, -4.5f), PxQuat(PxPi / 4.f, PxVec3(0.f, 1.f, 0.f))), PxVec3(.2f, 2.f, .5f));
			domino->Color(PxVec3(0.f, 0.f, 1.f));
			Add(domino);
			domino->Material(wood_on_wood);

			domino = new Box(PxTransform(PxVec3(104.f, 1.f, -7.f), PxQuat(PxPi / 2.f, PxVec3(0.f, 1.f, 0.f))), PxVec3(.2f, 2.f, .5f));
			domino->Color(PxVec3(0.f, 0.f, 1.f));
			Add(domino);
			domino->Material(wood_on_wood);

			domino = new Box(PxTransform(PxVec3(104.f, 1.f, -9.5f), PxQuat(PxPi / 4.f, PxVec3(0.f, -1.f, 0.f))), PxVec3(.2f, 2.f, .5f));
			domino->Color(PxVec3(0.f, 0.f, 1.f));
			Add(domino);
			domino->Material(wood_on_wood);

			domino = new Box(PxTransform(PxVec3(102.5f, 1.f, -11.5f)), PxVec3(.2f, 2.f, .5f));
			domino->Color(PxVec3(0.f, 0.f, 1.f));
			Add(domino);
			domino->Material(wood_on_wood);


			for (float i = 0.f; i < 51.f; i++)
			{
				domino = new Box(PxTransform(PxVec3((100.f - (i * 2)), 1.f, -12.f)), PxVec3(.2f, 2.f, .5f));
				domino->Color(PxVec3(0.f, (i * 5) / 255.f, (255.f - (i * 5)) / 255.f));
				Add(domino);
				domino->Material(wood_on_wood);
			}
			
			//experimenting
			//hexagon = new Hexagon(PxTransform(PxVec3(-3.f, 6.f, 6.f), PxQuat(PxPi / 2, PxVec3(1.f, 0.f, 0.f))));
			//hexagon->Color(color_palette[0]);
			//Add(hexagon);

			//box = new Box(PxTransform(PxVec3(-5.f, 4.f, 4.f)), PxVec3(2.f, .2f, 1.f));
			//box->Color(color_palette[0]);
			//Add(box);

			//hexagon->SetupFiltering(FilterGroup::ACTOR0, FilterGroup::ACTOR1);
			//box->SetupFiltering(FilterGroup::ACTOR1, FilterGroup::ACTOR0);

			//revoluteJoint = new RevoluteJoint(hexagon, PxTransform(PxVec3(0.f, 0.f, 0.f)), box, PxTransform(PxVec3(-2.f, -2.f, -2.f)));
			//box->Get()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			//hexagon->Get()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			//revoluteJoint->DriveVelocity(1.0f);
		}

		//Custom udpate function
		virtual void CustomUpdate() 
		{
		}

		/// An example use of key release handling
		void ExampleKeyReleaseHandler()
		{
			cerr << "I am realeased!" << endl;
			revoluteJoint->DriveVelocity(0.0f);
			revoluteJoint2->DriveVelocity(0.0f);
		}

		/// An example use of key presse handling
		void ExampleKeyPressHandler()
		{
			cerr << "I am pressed!" << endl;
			revoluteJoint->DriveVelocity(1.0f);
			revoluteJoint2->DriveVelocity(1.0f);
		}
	};
}
