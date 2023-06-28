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
	static PxVec3 wedge_verts[] = {PxVec3(0,1,1), PxVec3(-1,1,0), PxVec3(1,0,0), PxVec3(-1,0,0), PxVec3(0,0,1), PxVec3(0,0,-1)};
	
	//hexagon vertices
	static PxVec3 hex_verts[] = {PxVec3(.5f,0,.866f), PxVec3(1,0,0), PxVec3(.5f,0,-.866), PxVec3(-.5f,0,-.866f), PxVec3(-1,0,0), PxVec3(-.5f,0,.866f),
								PxVec3(.5f,1,.866f), PxVec3(1,1,0), PxVec3(.5f,1,-.866f), PxVec3(-.5f,1,-.866f), PxVec3(-1,1,0), PxVec3(-.5f,1,.866f)};

	//pyramid triangles: a list of three vertices for each triangle e.g. the first triangle consists of vertices 1, 4 and 0
	//vertices have to be specified in a counter-clockwise order to assure the correct shading in rendering
	static PxU32 pyramid_trigs[] = {1, 4, 0, 3, 1, 0, 2, 3, 0, 4, 2, 0, 3, 2, 1, 2, 4, 1};

	class Pyramid : public ConvexMesh
	{
	public:
		Pyramid(PxTransform pose=PxTransform(PxIdentity), PxReal density=1.f) :
			ConvexMesh(vector<PxVec3>(begin(pyramid_verts),end(pyramid_verts)), pose, density)
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

	class Hexagon : public ConvexMesh
	{
	public:
		Hexagon(PxTransform pose = PxTransform(PxIdentity), float height = 1, float side_length = 1, PxReal density = 1.f) :
			ConvexMesh(vector<PxVec3>(begin(hex_verts), end(hex_verts)), pose, density)
		{
			float tri_height = PxSqrt(((side_length / 2.f) * (side_length / 2.f)) + (side_length * side_length));
			static PxVec3 hex_verts[] = { PxVec3(side_length/2.f,0,tri_height), PxVec3(side_length,0,0), PxVec3(side_length / 2.f,0,-tri_height), PxVec3(-(side_length / 2.f),0,-tri_height), PxVec3(-side_length,0,0), PxVec3(-(side_length / 2.f),0,tri_height),
										PxVec3(side_length / 2.f,height,tri_height), PxVec3(side_length,height,0), PxVec3(side_length / 2.f,height,-tri_height), PxVec3(-(side_length / 2.f),height,-tri_height), PxVec3(-side_length,height,0), PxVec3(-(side_length / 2.f),height,tri_height) };
	//		ConvexMesh(vector<PxVec3>(begin(hex_verts), end(hex_verts)), pose, density);
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

	///Custom scene class
	class MyScene : public Scene
	{
		Plane* plane;
		Box* box;
		Box* boxSpin;
		Sphere* sphere;
		CompoundObject* compoundObject;
		Enclosure* enclosure;
		Pyramid* pyramid;
		Wedge* wedge;
		Hexagon* hexagon;
		PyramidStatic* pyramidStatic;
		float time_counter = 0;

	public:
		///A custom scene class
		void SetVisualisation()
		{
			px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eBODY_LIN_VELOCITY, 1.0f);
		}

		//Custom scene initialisation
		virtual void CustomInit() 
		{
			SetVisualisation();			

			GetMaterial()->setDynamicFriction(.2f);

			PxMaterial* wood_on_wood = CreateMaterial(.3f, .2f, .603f); //static friction, dynamic friction, restitution
			PxMaterial* teflon_on_teflon = CreateMaterial(.04f, .04f, .8f);
			PxMaterial* iron_on_iron = CreateMaterial(1.1f, .15f, .3f);

			plane = new Plane();
			plane->Color(PxVec3(210.f/255.f,210.f/255.f,210.f/255.f));
			Add(plane);

			//enclosure = new Enclosure(PxTransform(PxVec3(10.0f, 1.f, 2.f)));
			//enclosure->Color(PxVec3(0.f, 0.f, 0.f));
			//Add(enclosure);

			compoundObject = new CompoundObject(PxTransform(PxVec3(-10.0f, 1.f, .0f), PxQuat(PxPi/2.f,PxVec3(1.f,0.f,0.f))));
			compoundObject->Color(PxVec3(0.f, 0.f, 0.f));
			Add(compoundObject);

			box = new Box(PxTransform(PxVec3(10.f, 10.f, .0f), PxQuat(PxPi / 4.f, PxVec3(0.f, 1.f, 0.f))));
			PxTransform pose = ((PxRigidBody*)box->Get())->getGlobalPose();
			PxQuat quat = (PxQuat(PxPi/4.f,PxVec3(0.f, 0.f, 1.f)));
			PxTransform turn = PxTransform(pose.p, quat * pose.q);
			((PxRigidBody*)box->Get())->setGlobalPose(turn);
			//box->GetShape(0)->setLocalPose(turn);
			//box->GetShape(0)->setLocalPose(PxTransform(PxQuat(PxPi / 4.f, PxVec3(1.f, 0.f, 0.f))));
			box->Color(color_palette[2]);
			Add(box);

			//for (float i = 0.f; i < 20.f; i++)
			//{
			//	box = new Box(PxTransform(PxVec3((.0f + (i * 5)), 1.f, 2.f), PxQuat((PxPi / 4.f) + (i * 2), PxVec3(0.f, 1.f, 0.f))));
			//	box->GetShape(0)->setLocalPose(PxTransform(PxQuat((PxPi / 4.f), PxVec3(0.f, 0.f, 1.f))));
			//	box->Color(PxVec3(0.f, 0.f, 0.f));
			//	Add(box);
			//}
			//box = new Box(PxTransform(PxVec3(.0f, 1.f, 4.f)), PxVec3(.2f, 2.f, .5f));
			//box->GetShape(0)->setLocalPose(PxTransform(PxQuat((PxPi / 4.f), PxVec3(0.f, 0.f, 1.f))));
			//box->Color(PxVec3(0.f, 0.f, 0.f));
			//Add(box);

			for (float i = 0.f; i < 20.f; i++)
			{
				box = new Box(PxTransform(PxVec3((.0f + (i * 2)), 1.f, -4.f)), PxVec3(.2f, 2.f, .5f));
				//box->GetShape(0)->setLocalPose(PxTransform(PxQuat((PxPi / 4.f), PxVec3(0.f, 0.f, 1.f))));
				box->Color(PxVec3(0.f, 0.f, 0.f));
				Add(box);
			}

			boxSpin = new Box(PxTransform(PxQuat(PxPi / 4.f, PxVec3(.0f, 1.f, .0f))));
			boxSpin->Color(PxVec3(0.f, 0.f, 0.f));
			Add(boxSpin);
			//box->GetShape(0)->setLocalPose(PxTransform(PxQuat((PxPi / 180.f) * time_counter, PxVec3(0.f, 1.f, 0.f))));

			sphere = new Sphere(PxTransform(PxVec3(5.f, 10.f, .0f)));
			sphere->Color(color_palette[2]);
			Add(sphere);
			sphere->Material(wood_on_wood);

			sphere = new Sphere(PxTransform(PxVec3(5.f, 10.f, .0f)));
			sphere->Color(color_palette[2]);
			Add(sphere);
			sphere->Material(teflon_on_teflon);

			pyramid = new Pyramid(PxTransform(PxVec3(15.f, 5.f, .0f)));
			pyramid->Color(color_palette[3]);
			Add(pyramid);

			wedge = new Wedge(PxTransform(PxVec3(20.f, 5.f, .0f)));
			wedge->Color(color_palette[3]);
			Add(wedge);

			hexagon = new Hexagon(PxTransform(PxVec3(0.f, 5.f, 2.f)), 3.f);
			hexagon->Color(color_palette[0]);
			Add(hexagon);


		}

		//Custom udpate function
		virtual void CustomUpdate() 
		{
			time_counter++;
			boxSpin->GetShape(0)->setLocalPose(PxTransform(PxQuat((PxPi / 180.f) * time_counter, PxVec3(0.f, 1.f, 0.f))));
			PxTransform pose = ((PxRigidBody*)compoundObject->Get())->getGlobalPose();
			PxQuat quat = (PxQuat((PxPi / 30.f) * time_counter, PxVec3(0.f, 1.f, 0.f)));
			PxTransform turn = PxTransform(pose.p, quat * pose.q);

			((PxRigidBody*)compoundObject->Get())->setGlobalPose(turn);
		}
	};
}
