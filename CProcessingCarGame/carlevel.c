//#include "cprocessing.h"
#include "utils.h"
#include "colortable.h"
#include "carlevel.h"
#include "mainmenu.h"
#include <stdio.h>



float gameTime;


PhysicsObject CarA;
PhysicsObject CarB;
PhysicsObject CarC;
PhysicsObject* selectedObject;
Shape instructionsText;
char carText[64];
void Car_Level_Init()
{
	CP_System_SetWindowSize(500, 500);
	
	CarA = CreatePhysicsObject(250, 450, 25, LIGHT_PURPLE, 10, 5.0f,CONSTRAINTS_LEVEL);
	CarB = CreatePhysicsObject(250, 350, 15, LIGHT_PINK, 8, 2.0f,CONSTRAINTS_LEVEL);
	CarC = CreatePhysicsObject(250, 250, 35, LIGHT_BLUE, 8, 10.0f, CONSTRAINTS_LEVEL);

	selectedObject = &CarA;

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
	CP_Settings_Stroke(BLACK);
	CP_Settings_Fill(PhysicsObjectArray[selectedObject->id].color);
	DisplayShape(PhysicsObjectArray[selectedObject->id].rigidBody.collider.shape);
	CP_Settings_NoStroke();

	UpdateCars(&CarA);
	UpdateCars(&CarB);
	UpdateCars(&CarC);
	
	CP_Graphics_ClearBackground(LIGHT_GRAY);
}

void Car_Level_Exit()
{

}
void UpdateCars(PhysicsObject* obj)
{
	//By right this should be split but I can't seem to reference my physics objs without referencing the array. :T
	DrawTriangleAdvanced(GetPointInCircle(PhysicsObjectArray[obj->id].rigidBody.collider.shape, 50), GetPointInCircle(PhysicsObjectArray[obj->id].rigidBody.collider.shape, 130), GetPointInCircle(PhysicsObjectArray[obj->id].rigidBody.collider.shape, 270), PhysicsObjectArray[obj->id].rigidBody.collider.shape.transform.rotation);
	
	if (IsMouseClickedShapeArea(PhysicsObjectArray[obj->id].rigidBody.collider.shape))
	{
		selectedObject = obj;
	}
	int j = 0;
	j = sprintf_s(carText, 64, "Speed: % .2f ", CP_Vector_Length(PhysicsObjectArray[obj->id].rigidBody.velocity));
	j += sprintf_s(carText+j, 64-j, "Mass: % .0f ", PhysicsObjectArray[obj->id].rigidBody.mass);
	j += sprintf_s(carText+j, 64-j, "Max Speed: % .2f ", PhysicsObjectArray[obj->id].speed);
	CP_Settings_Fill(BLACK);
	CP_Font_DrawText(carText, PhysicsObjectArray[obj->id].rigidBody.collider.shape.transform.position.x, PhysicsObjectArray[obj->id].rigidBody.collider.shape.transform.position.y - 50);
	
}

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
