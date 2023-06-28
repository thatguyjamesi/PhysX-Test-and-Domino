#include <iostream> //cout, cerr
#include <iomanip> //stream formatting
#include <windows.h> //delay function
#include <PxPhysicsAPI.h> //PhysX

using namespace std;
using namespace physx;

//PhysX objects
PxPhysics* physics = 0;
PxFoundation* foundation = 0;
#if PX_PHYSICS_VERSION < 0x304000 // SDK 3.3
debugger::comm::PvdConnection* pvd = 0;
#else
PxPvd*  pvd = 0;
#endif

//simulation objects
PxScene* scene;
PxRigidDynamic* box;
PxRigidDynamic* box2;
PxRigidStatic* box3;
PxRigidDynamic* box4;
PxRigidStatic* plane;

void CreateBox(PxVec3 origin, PxVec3 shape, PxReal density)
{
	PxRigidDynamic* box;
	PxMaterial* box_material = physics->createMaterial(0.f, .07f, .0f);
	box = physics->createRigidDynamic(PxTransform(origin));
	box->createShape(PxBoxGeometry(shape), *box_material);
	PxRigidBodyExt::updateMassAndInertia(*box, density);
	scene->addActor(*box);
}

void CreateSphere(PxVec3 origin, float radius, PxReal density)
{
	PxRigidDynamic* sphere;
	PxMaterial* sphere_material = physics->createMaterial(0.f, .07f, .0f);
	sphere = physics->createRigidDynamic(PxTransform(origin));
	sphere->createShape(PxSphereGeometry(radius), *sphere_material);
	PxRigidBodyExt::updateMassAndInertia(*sphere, density);
	scene->addActor(*sphere);
}

void CreateCapsule(PxVec3 origin, float radius, float half_height, PxReal density)
{
	PxRigidDynamic* capsule;
	PxMaterial* capsule_material = physics->createMaterial(0.f, .07f, .0f);
	capsule = physics->createRigidDynamic(PxTransform(origin));
	capsule->createShape(PxCapsuleGeometry(radius, half_height), *capsule_material);
	PxRigidBodyExt::updateMassAndInertia(*capsule, density);
	scene->addActor(*capsule);
}

///Initialise PhysX objects
bool PxInit()
{
	//default error and allocator callbacks
	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;

	//Init PhysX
	//foundation
#if PX_PHYSICS_VERSION < 0x304000 // SDK 3.3
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
#else
	foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
#endif

	if(!foundation)
		return false;

	//connect to an external visual debugger (if exists)
#if PX_PHYSICS_VERSION < 0x304000 // SDK 3.3
	pvd = PxVisualDebuggerExt::createConnection(physics->getPvdConnectionManager(), "localhost", 5425, 100,
		PxVisualDebuggerExt::getAllConnectionFlags());
#else
	pvd = PxCreatePvd(*foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
	pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
#endif

	//physics
#if PX_PHYSICS_VERSION < 0x304000 // SDK 3.3
	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale());
#else
	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), true, pvd);
#endif

	if(!physics)
		return false;

	//create a default scene
	PxSceneDesc sceneDesc(physics->getTolerancesScale());

	if(!sceneDesc.cpuDispatcher)
	{
		PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		sceneDesc.cpuDispatcher = mCpuDispatcher;
	}

	if(!sceneDesc.filterShader)
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	scene = physics->createScene(sceneDesc);

	if (!scene)
		return false;

	return true;
}

/// Release all allocated resources
void PxRelease()
{
	if (scene)
		scene->release();
	if (physics)
		physics->release();
	if (pvd)
		pvd->release();
	if (foundation)
		foundation->release();
}

///Initialise the scene
void InitScene()
{
	//default gravity
	scene->setGravity(PxVec3(0.f, -9.81f, 0.f));

	//materials
	PxMaterial* default_material = physics->createMaterial(0.f, .07f, .0f);   //static friction, dynamic friction, restitution

	//PxRigidDynamic* CreateBox(PxVec3(10.f, .5f, 0.f), PxVec3(.5f, .5f, .5f), PxReal(1.f));

	//create a static plane (XZ)
	plane = PxCreatePlane(*physics, PxPlane(PxVec3(0.f, 1.f, 0.f), 0.f), *default_material);
	scene->addActor(*plane);

	//create a dynamic actor and place it 10 m above the ground
	//box = physics->createRigidDynamic(PxTransform(PxVec3(0.f, 0.5f, 0.f)));
	//box2 = physics->createRigidDynamic(PxTransform(PxVec3(0.f, 100.f, 0.f)));
	//box3 = physics->createRigidStatic(PxTransform(PxVec3(5.f, .5f, 0.f)));
	//box4 = physics->createRigidDynamic(PxTransform(PxVec3(10.f, .5f, 0.f)));
	//create a box shape of 1m x 1m x 1m size (values are provided in halves)
	//box->createShape(PxBoxGeometry(.5f, .5f, .5f), *default_material);
	//box2->createShape(PxBoxGeometry(.5f, .5f, .5f), *default_material);
	//box3->createShape(PxBoxGeometry(.5f, .5f, .5f), *default_material);
	//box4->createShape(PxBoxGeometry(.5f, .5f, .5f), *default_material);
	//update the mass of the box
	//PxRigidBodyExt::updateMassAndInertia(*box2, 1.f); //density of 1kg/m^3
	//scene->addActor(*box);
	//scene->addActor(*box2);
	//scene->addActor(*box3);
	//scene->addActor(*box4);
	CreateBox(PxVec3(10.f, .5f, 0.f), PxVec3(.5f, .5f, .5f), PxReal(1.f));
	CreateSphere(PxVec3(0.f, .5f, 0.f), .5f, PxReal(1.f));
	CreateCapsule(PxVec3(-10.f, .5f, 0.f), .5f, 1.f, PxReal(1.f));
}

/// Perform a single simulation step
void Update(PxReal delta_time)
{
	scene->simulate(delta_time);
	scene->fetchResults(true);
}

/// The main function
int main()
{
	//initialise PhysX	
	if (!PxInit())
	{
		cerr << "Could not initialise PhysX." << endl;
		return 0;
	}

	//initialise the scene
	InitScene();


	//box->addForce(PxVec3(100.f, 0.f, 0.f),PxForceMode::eFORCE);
	//box->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	//cout << box->getMass() << endl;

	//set the simulation step to 1/60th of a second
	PxReal delta_time = 1.f/60.f;
	float time_counter = 0;
	int incr = 0;
	//simulate until the 'Esc' is pressed
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		incr = incr + 10;
		//box->setKinematicTarget(PxTransform(PxVec3(incr, 0.f, 0.f)));
		//'visualise' position and velocity of the box
		//PxVec3 position = box->getGlobalPose().p;
		//PxVec3 velocity = box->getLinearVelocity();

		//cout << setiosflags(ios::fixed) << setprecision(2) << "x=" << position.x << 
		//	", y=" << position.y << ", z=" << position.z << ",  ";
		//cout << setiosflags(ios::fixed) << setprecision(2) << "vx=" << velocity.x << 
		//	", vy=" << velocity.y << ", vz=" << velocity.z << ",  ";
		//cout << "Time =" << (time_counter/60) << ",  ";
		//cout << "Sleeping =" << box2->isSleeping() << endl;

		time_counter++;


		//if (position.x >= 10.f) {
		//	box->setLinearVelocity(PxVec3(0.f,0.f,0.f));
		//}
		//perform a single simulation step

		Update(delta_time);
		//if (time_counter == 10) {
		//	box->setGlobalPose(PxTransform(PxVec3(10.f, 0.f, 0.f)));
		//}
		//introduce 100ms delay for easier visual analysis of the results
		//Sleep(100);
	}

	//Release all resources
	PxRelease();

	return 0;
}
