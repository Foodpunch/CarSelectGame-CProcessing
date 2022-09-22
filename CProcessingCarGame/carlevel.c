#include "cprocessing.h"
#include "utils.h"
#include "colortable.h"
#include "carlevel.h"
#include "mainmenu.h"
#include <stdio.h>


//Car CarA;
//Car CarB;
//Car CarC;
float gameTime;

//Car* selectedCar;

BCar BCar1;
BCar BCar2;
BCar BCar3;
BCar* selectedBCar;

//RectArea instructionsText;

void Car_Level_Init()
{
	CP_System_SetWindowSize(500, 500);
	//CarA = CreateCar(250, 150, 50, LIGHT_PURPLE,20,2.0f);
	//CarB = CreateCar(250, 250, 50, LIGHT_ORANGE, 15,1.5f);
	//CarC = CreateCar(250, 350, 50, LIGHT_PINK, 20,1.f);

	BCar1 = CreateBCar(250, 150, 25, LIGHT_RED, 20, 2.0f);
	BCar2 = CreateBCar(250, 250, 50, LIGHT_GREEN, 15, 1.5f);
	BCar3 = CreateBCar(250, 350, 50, LIGHT_BLUE, 20, 1.0f);

	selectedBCar = &BCar1;

	//selectedCar = &CarB;

	//QuitGamebutton = CreateButton(CP_System_GetWindowWidth() / 2.f, CP_System_GetWindowHeight() * 0.9f, 150, 50, "Quit Game", LIGHT_RED, ExitGame);

	//instructionsText = CreateRectArea(60, 20, 150, 50);

}

void Car_Level_Update()
{
	//UpdateCar(&CarA);
	//UpdateCar(&CarB);
	//UpdateCar(&CarC);
	//UpdateButton(QuitGamebutton,CP_POSITION_CENTER);

	CP_Settings_Fill(BLACK);
	//DisplayTextInRect(instructionsText, "WASD to move\n Space to brake \n Click to control");
	/*
	CP_Font_DrawText("WASD to move", 50, 50);
	CP_Font_DrawText("Space to brake", 50, 60);
	CP_Font_DrawText("Click on cars to control them", 50, 70);*/
	//DisplayBCar(&BCar1);
	UpdateBCar(&BCar1);
	MoveBCar(selectedBCar);
	//MoveCar(selectedCar);
	CP_Graphics_ClearBackground(LIGHT_GRAY);
	
}

void Car_Level_Exit()
{

}


BCar CreateBCar(float x, float y, float diameter, CP_Color color, float speed, float mass)
{
	BCar xnewCar;
	xnewCar.carShape = CreateShape(x, y, diameter, diameter, 0, SHAPE_CIRCLE);	//Circle and not circle ellipse because I think it's easier to calculate
	xnewCar.carCollider = CreateCollider(xnewCar.carShape, FALSE);
	xnewCar.carCollider.colliderLayer = 0;
	xnewCar.rigidbody = CreateRigidBody(xnewCar.carCollider, mass, 0);
	xnewCar.color = color;
	xnewCar.moveSpeed = speed;
	xnewCar.acceleration = 0;
	xnewCar.rigidbody.velocity = CP_Vector_Zero();
	xnewCar.direction = VECTOR_UP;
	return xnewCar;
}

void DisplayBCar(BCar* bcar)
{
	CP_Settings_Fill(bcar->color);
	DisplayShape(bcar->carShape);
	DrawTriangleAdvanced(GetPointInCircle(bcar->carShape, 50), GetPointInCircle(bcar->carShape, 130), GetPointInCircle(bcar->carShape, 270), bcar->carShape.transform.rotation);
}

void UpdateBCar(BCar* bcar)
{
	if (IsMouseInShapeArea(bcar->carShape) && CP_Input_MouseClicked())
	{
		selectedBCar = bcar;
	}
	//Hacky collision detection
	_Bool CheckLeftBounds = (bcar->carShape.transform.position.x < bcar->carShape.transform.size.x);
	_Bool CheckRightBounds = (bcar->carShape.transform.position.x > ((float)CP_System_GetWindowWidth() - bcar->carShape.transform.size.x));
	_Bool CheckTopBounds = (bcar->carShape.transform.position.y < bcar->carShape.transform.size.x);
	_Bool CheckBottomBounds = (bcar->carShape.transform.position.y > ((float)CP_System_GetWindowHeight() - bcar->carShape.transform.size.x));

	if (CheckLeftBounds) bcar->carShape.transform.position.x = bcar->carShape.transform.size.x;
	if (CheckRightBounds) bcar->carShape.transform.position.x = (float)CP_System_GetWindowWidth() - (bcar->carShape.transform.size.x);
	if (CheckTopBounds) bcar->carShape.transform.position.y = bcar->carShape.transform.size.x;
	if (CheckBottomBounds) bcar->carShape.transform.position.y = (float)CP_System_GetWindowHeight() - (bcar->carShape.transform.size.x);
	//Hacky Car physics stuff, needs heavy clean up. 
	bcar->direction = RotateVectorByAngle(VECTOR_UP, bcar->carShape.transform.rotation);
	bcar->rigidbody.velocity = CP_Vector_Set(bcar->direction.x * bcar->acceleration, bcar->direction.y * bcar->acceleration);

	bcar->carShape.transform.position.x += (bcar->rigidbody.velocity.x * bcar->moveSpeed) * CP_System_GetDt();
	bcar->carShape.transform.position.y += (bcar->rigidbody.velocity.y * bcar->moveSpeed) * CP_System_GetDt();
	//Checks for acceleration here so you can accelerate and reverse properly
	if (bcar->acceleration > 0.0f) bcar->acceleration -= (1.f / bcar->rigidbody.mass) * CP_System_GetDt();
	else if (bcar->acceleration < 0.0f) bcar->acceleration += (1.f / bcar->rigidbody.mass) * CP_System_GetDt();

	if (CP_Vector_Length(bcar->rigidbody.velocity) < 0.01f) bcar->acceleration = 0.0f;			//clamps acceleration to zero
	if (bcar->acceleration >= bcar->moveSpeed) bcar->acceleration = bcar->moveSpeed;	//clamp movespeed
	if (bcar->acceleration <= -bcar->moveSpeed) bcar->acceleration = -bcar->moveSpeed;	//clamp movespeed
	DisplayBCar(bcar);
}

//
//void DisplayCar(Car *_car)
//{
//	DisplayCircle(_car->circleData);
//	DrawTriangleAdvanced(GetPointInCircle(_car->circleData, 50), GetPointInCircle(_car->circleData, 130), GetPointInCircle(_car->circleData, 270), _car->angleInDeg);
//}

////Updates the car display and also checks if the car has been clicked. 
//void UpdateCar(Car *car)
//{
//	//Feels messy but it's late and I'm tired with dealing with pointers
//	if (IsCircleClicked(car->circleData))
//	{
//		selectedCar = car;
//	}
//
//	//Hacky text thing to display car stats
//	int j = 0;
//	char carText[64];
//	j = sprintf_s(carText, 64, "Speed : %.2f ", CP_Vector_Length(car->velocity));
//	j += sprintf_s(carText + j, 64 - j, "Mass : %.0f ", car->mass);
//	j += sprintf_s(carText + j, 64 - j, "Max speed : %.2f", car->moveSpeed);
//	CP_Settings_Fill(BLACK);
//	CP_Font_DrawText(carText, car->circleData.x, car->circleData.y - 50);
//
//	//Hacky collision detection
//	_Bool CheckLeftBounds = (car->circleData.x < car->circleData.diameter / 2.f);
//	_Bool CheckRightBounds = (car->circleData.x > ((float)CP_System_GetWindowWidth() - car->circleData.diameter / 2.f));
//	_Bool CheckTopBounds = (car->circleData.y < car->circleData.diameter / 2.f);
//	_Bool CheckBottomBounds = (car->circleData.y > ((float)CP_System_GetWindowHeight() - car->circleData.diameter / 2.f));
//
//	if (CheckLeftBounds) car->circleData.x = car->circleData.diameter / 2.f;
//	if (CheckRightBounds) car->circleData.x = (float)CP_System_GetWindowWidth() - (car->circleData.diameter / 2.f);
//	if (CheckTopBounds) car->circleData.y = car->circleData.diameter / 2.f;
//	if (CheckBottomBounds) car->circleData.y = (float)CP_System_GetWindowHeight() - (car->circleData.diameter / 2.f);
//
//
//
//	//Hacky Car physics stuff, needs heavy clean up. 
//	car->direction = RotateVectorByAngle(VECTOR_UP, car->angleInDeg);
//	car->velocity = CP_Vector_Set(car->direction.x*car->acceleration, car->direction.y* car->acceleration);
//
//	car->circleData.x += (car->velocity.x * car->moveSpeed) * CP_System_GetDt();
//	car->circleData.y += (car->velocity.y * car->moveSpeed) * CP_System_GetDt();
//	//Checks for acceleration here so you can accelerate and reverse properly
//	if (car->acceleration > 0.0f) car->acceleration -= (1.f / car->mass) * CP_System_GetDt();
//	else if (car->acceleration < 0.0f) car->acceleration += (1.f / car->mass) * CP_System_GetDt();
//
//	if (CP_Vector_Length(car->velocity) < 0.01f) car->acceleration = 0.0f;			//clamps acceleration to zero
//	if (car->acceleration >= car->moveSpeed) car->acceleration = car->moveSpeed;	//clamp movespeed
//	if (car->acceleration <= -car->moveSpeed) car->acceleration = -car->moveSpeed;	//clamp movespeed
//	DisplayCar(car);
//}

//Moves the specified car with WASD movement keys. Arrow key implementation not added 
void MoveBCar(BCar* car)
{
	//Note: The fancy pointer stuff here is because structs are NOT the same as classes. You need to use pointers if you
	//wanna modify the values, if not you're just copying and applying changes to the copy.
	if (CP_Input_KeyDown(KEY_W) || (CP_Input_KeyDown(KEY_UP)))
	{
		car->acceleration += (1.f / car->rigidbody.mass);
	}
	if (CP_Input_KeyDown(KEY_S) || (CP_Input_KeyDown(KEY_DOWN)))
	{
		car->acceleration -= (1.f / car->rigidbody.mass);
	}
	if (CP_Input_KeyDown(KEY_D) || (CP_Input_KeyDown(KEY_RIGHT)))
	{
		car->carShape.transform.rotation += 10.f;
	}
	if (CP_Input_KeyDown(KEY_A) || (CP_Input_KeyDown(KEY_LEFT)))
	{
		car->carShape.transform.rotation -= 10.f;
	}
	if (CP_Input_KeyDown(KEY_SPACE))
	{
		car->acceleration /= 1.1f;
	}
	CP_Settings_Stroke(BLACK);
	DisplayBCar(car);
	CP_Settings_NoStroke();
}

////Moves the specified car with WASD movement keys. Arrow key implementation not added 
//void MoveCar(Car *car)
//{
//	//Note: The fancy pointer stuff here is because structs are NOT the same as classes. You need to use pointers if you
//	//wanna modify the values, if not you're just copying and applying changes to the copy.
//	if (CP_Input_KeyDown(KEY_W)||(CP_Input_KeyDown(KEY_UP)))
//	{
//		car->acceleration += (1.f / car->mass);
//	}
//	if (CP_Input_KeyDown(KEY_S)|| (CP_Input_KeyDown(KEY_DOWN)))
//	{
//		car->acceleration -= (1.f / car->mass);
//	}
//	if (CP_Input_KeyDown(KEY_D)|| (CP_Input_KeyDown(KEY_RIGHT)))
//	{
//		car->angleInDeg += 10.f;
//	}
//	if (CP_Input_KeyDown(KEY_A)|| (CP_Input_KeyDown(KEY_LEFT)))
//	{
//		car->angleInDeg -= 10.f;
//	}
//	if (CP_Input_KeyDown(KEY_SPACE))
//	{
//		car->acceleration /= 1.1f;
//	}
//	CP_Settings_Stroke(BLACK);
//	DisplayCar(car);
//	CP_Settings_NoStroke();
//}