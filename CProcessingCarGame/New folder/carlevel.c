//#include "cprocessing.h"
#include "utils.h"
#include "carlevel.h"
#include "mainmenu.h"



float gameTime;


PhysicsObject CarA;
PhysicsObject CarB;
PhysicsObject CarC;
PhysicsObject* selectedObject;

Shape instructionsText;

Button QuitGameButton;

char carText[64];
void Car_Level_Init()
{
	//Quit game button just to test the whole button thing
	QuitGameButton = CreateButton((float)(CP_System_GetWindowWidth()-25), 25, 25, 25, SHAPE_RECTANGLE, "X", LIGHT_RED, ExitGame);
	
	CP_System_SetWindowSize(500, 500);
	
	CarA = CreatePhysicsObject(250, 150, 25, LIGHT_PURPLE, 10, 5.0f,CONSTRAINTS_LEVEL);
	CarB = CreatePhysicsObject(250, 350, 15, LIGHT_PINK, 8, 2.0f,CONSTRAINTS_LEVEL);
	CarC = CreatePhysicsObject(250, 250, 35, LIGHT_BLUE, 8, 10.0f, CONSTRAINTS_LEVEL);

	selectedObject = &CarC;

}

void Car_Level_Update()
{
	UpdateCameraShaker();
	CP_Settings_Fill(BLACK);
	
	CP_Font_DrawText("WASD to move", 50, 50);
	CP_Font_DrawText("Space to brake", 50, 65);
	CP_Font_DrawText("Click on cars to control them", 95, 80);


	

	MovePhysicsObject(selectedObject);
	
	UpdatePhysics();

	//Draws stroke over selected object
	CP_Settings_Stroke(BLACK);
	CP_Settings_NoFill();
	DisplayShape(PhysicsObjectArray[selectedObject->id].rigidBody.collider.shape);
	CP_Settings_NoStroke();

	//Checks for if the car has been clicked. Also displays car stats
	UpdateCars(&CarA);
	UpdateCars(&CarB);
	UpdateCars(&CarC);
	UpdateGUI();
	CP_Graphics_ClearBackground(LIGHT_GRAY);
}

void Car_Level_Exit()
{

}
void UpdateCars(PhysicsObject* obj)
{
	
	if (IsMouseClickedShapeArea(PhysicsObjectArray[obj->id].rigidBody.collider.shape))
	{
		selectedObject = obj;
	}

	//Hacky car debug text
	int j = 0;
	j = sprintf_s(carText, 64, "Speed: % .2f ", CP_Vector_Length(PhysicsObjectArray[obj->id].rigidBody.velocity));
	j += sprintf_s(carText+j, 64-j, "Mass: % .0f ", PhysicsObjectArray[obj->id].rigidBody.mass);
	j += sprintf_s(carText+j, 64-j, "Max Speed: % .2f ", PhysicsObjectArray[obj->id].speed);
	CP_Settings_Fill(BLACK);
	CP_Font_DrawText(carText, PhysicsObjectArray[obj->id].rigidBody.collider.shape.transform.position.x, PhysicsObjectArray[obj->id].rigidBody.collider.shape.transform.position.y - 50);
	
}

//Adds forces to the physics object
void MovePhysicsObject(PhysicsObject* obj)
{
	if (CP_Input_KeyDown(KEY_W) || (CP_Input_KeyDown(KEY_UP)))
	{
		PhysicsObjectArray[obj->id].direction = RotateVectorByAngle(VECTOR_UP, PhysicsObjectArray[obj->id].rigidBody.collider.shape.transform.rotation);
		CP_Vector force = CP_Vector_Scale(CP_Vector_Normalize(PhysicsObjectArray[obj->id].direction), obj->speed);
		AddForce(*obj, force, FORCEMODE_FORCE);
	}
	if (CP_Input_KeyDown(KEY_S) || (CP_Input_KeyDown(KEY_DOWN)))
	{
		PhysicsObjectArray[obj->id].direction = RotateVectorByAngle(VECTOR_UP, PhysicsObjectArray[obj->id].rigidBody.collider.shape.transform.rotation);
		CP_Vector force = CP_Vector_Scale(CP_Vector_Normalize(PhysicsObjectArray[obj->id].direction), -obj->speed);
		AddForce(*obj, force, FORCEMODE_FORCE);
	}
	if (CP_Input_KeyDown(KEY_D) || (CP_Input_KeyDown(KEY_RIGHT)))
	{
		PhysicsObjectArray[obj->id].rigidBody.collider.shape.transform.rotation += 10.f;
	}
	if (CP_Input_KeyDown(KEY_A) || (CP_Input_KeyDown(KEY_LEFT)))
	{
		PhysicsObjectArray[obj->id].rigidBody.collider.shape.transform.rotation -= 10.f;
	}
	if (CP_Input_KeyDown(KEY_SPACE))
	{
		//obj->direction = RotateVectorByAngle(VECTOR_UP, obj->rigidBody.collider.shape.transform.rotation);
		CP_Vector force = CP_Vector_Scale(CP_Vector_Normalize(PhysicsObjectArray[obj->id].rigidBody.velocity), CP_Vector_Length(PhysicsObjectArray[obj->id].rigidBody.velocity) * -1.1f);
		AddForce(*obj, force, FORCEMODE_FORCE);
	}
}
