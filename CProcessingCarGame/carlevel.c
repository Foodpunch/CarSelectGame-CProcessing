#include "cprocessing.h"
#include "utils.h"
#include "colortable.h"
#include "carlevel.h"
#include "mainmenu.h"
#include <stdio.h>


Car CarA;
Car CarB;
Car CarC;
float gameTime;

Car* selectedCar;

Button QuitGamebutton;

RectArea instructionsText;

void Car_Level_Init()
{
	CP_System_SetWindowSize(500, 500);
	CarA = CreateCar(250, 150, 50, LIGHT_PURPLE,20,2.0f);
	CarB = CreateCar(250, 250, 50, LIGHT_ORANGE, 15,1.5f);
	CarC = CreateCar(250, 350, 50, LIGHT_PINK, 20,1.f);
	selectedCar = &CarA;

	//QuitGamebutton = CreateButton(CP_System_GetWindowWidth() / 2.f, CP_System_GetWindowHeight() * 0.9f, 150, 50, "Quit Game", LIGHT_RED, ExitGame);

	//instructionsText = CreateRectArea(60, 20, 150, 50);

}

void Car_Level_Update()
{
	UpdateCar(&CarA);
	UpdateCar(&CarB);
	UpdateCar(&CarC);
	//UpdateButton(QuitGamebutton,CP_POSITION_CENTER);

	CP_Settings_Fill(BLACK);
	//DisplayTextInRect(instructionsText, "WASD to move\n Space to brake \n Click to control");
	/*
	CP_Font_DrawText("WASD to move", 50, 50);
	CP_Font_DrawText("Space to brake", 50, 60);
	CP_Font_DrawText("Click on cars to control them", 50, 70);*/


	MoveCar(selectedCar);
	CP_Graphics_ClearBackground(LIGHT_GRAY);
	
}

void Car_Level_Exit()
{

}

Car CreateCar(float x, float y, float diameter, CP_Color color, float speed, float mass)
{
	CircleArea _circle = CreateCircleArea(x, y, diameter, color);
	Car newCar;
	newCar.circleData = _circle;
	newCar.moveSpeed = speed;
	newCar.mass = mass;
	newCar.acceleration = 0;
	newCar.velocity = CP_Vector_Zero();
	newCar.direction = VECTOR_UP;
	newCar.angleInDeg = 0.f;
	return newCar;
}


void DisplayCar(Car *_car)
{
	DisplayCircle(_car->circleData);
	DrawTriangleAdvanced(GetPointInCircle(_car->circleData, 50), GetPointInCircle(_car->circleData, 130), GetPointInCircle(_car->circleData, 270), _car->angleInDeg);
}

//Updates the car display and also checks if the car has been clicked. 
void UpdateCar(Car *car)
{
	//Feels messy but it's late and I'm tired with dealing with pointers
	if (IsCircleClicked(car->circleData))
	{
		selectedCar = car;
	}

	//Hacky text thing to display car stats
	int j = 0;
	char carText[64];
	j = sprintf_s(carText, 64, "Speed : %.2f ", CP_Vector_Length(car->velocity));
	j += sprintf_s(carText + j, 64 - j, "Mass : %.0f ", car->mass);
	j += sprintf_s(carText + j, 64 - j, "Max speed : %.2f", car->moveSpeed);
	CP_Settings_Fill(BLACK);
	CP_Font_DrawText(carText, car->circleData.x, car->circleData.y - 50);

	//Hacky collision detection
	_Bool CheckLeftBounds = (car->circleData.x < car->circleData.diameter / 2.f);
	_Bool CheckRightBounds = (car->circleData.x > ((float)CP_System_GetWindowWidth() - car->circleData.diameter / 2.f));
	_Bool CheckTopBounds = (car->circleData.y < car->circleData.diameter / 2.f);
	_Bool CheckBottomBounds = (car->circleData.y > ((float)CP_System_GetWindowHeight() - car->circleData.diameter / 2.f));

	if (CheckLeftBounds) car->circleData.x = car->circleData.diameter / 2.f;
	if (CheckRightBounds) car->circleData.x = (float)CP_System_GetWindowWidth() - (car->circleData.diameter / 2.f);
	if (CheckTopBounds) car->circleData.y = car->circleData.diameter / 2.f;
	if (CheckBottomBounds) car->circleData.y = (float)CP_System_GetWindowHeight() - (car->circleData.diameter / 2.f);



	//Hacky Car physics stuff, needs heavy clean up. 
	car->direction = RotateVectorByAngle(VECTOR_UP, car->angleInDeg);
	car->velocity = CP_Vector_Set(car->direction.x*car->acceleration, car->direction.y* car->acceleration);

	car->circleData.x += (car->velocity.x * car->moveSpeed) * CP_System_GetDt();
	car->circleData.y += (car->velocity.y * car->moveSpeed) * CP_System_GetDt();
	//Checks for acceleration here so you can accelerate and reverse properly
	if (car->acceleration > 0.0f) car->acceleration -= (1.f / car->mass) * CP_System_GetDt();
	else if (car->acceleration < 0.0f) car->acceleration += (1.f / car->mass) * CP_System_GetDt();

	if (CP_Vector_Length(car->velocity) < 0.01f) car->acceleration = 0.0f;			//clamps acceleration to zero
	if (car->acceleration >= car->moveSpeed) car->acceleration = car->moveSpeed;	//clamp movespeed
	if (car->acceleration <= -car->moveSpeed) car->acceleration = -car->moveSpeed;	//clamp movespeed
	DisplayCar(car);
}

//Moves the specified car with WASD movement keys. Arrow key implementation not added 
void MoveCar(Car *car)
{
	//Note: The fancy pointer stuff here is because structs are NOT the same as classes. You need to use pointers if you
	//wanna modify the values, if not you're just copying and applying changes to the copy.
	if (CP_Input_KeyDown(KEY_W)||(CP_Input_KeyDown(KEY_UP)))
	{
		car->acceleration += (1.f / car->mass);
	}
	if (CP_Input_KeyDown(KEY_S)|| (CP_Input_KeyDown(KEY_DOWN)))
	{
		car->acceleration -= (1.f / car->mass);
	}
	if (CP_Input_KeyDown(KEY_D)|| (CP_Input_KeyDown(KEY_RIGHT)))
	{
		car->angleInDeg += 10.f;
	}
	if (CP_Input_KeyDown(KEY_A)|| (CP_Input_KeyDown(KEY_LEFT)))
	{
		car->angleInDeg -= 10.f;
	}
	if (CP_Input_KeyDown(KEY_SPACE))
	{
		car->acceleration /= 1.1f;
	}
	CP_Settings_Stroke(BLACK);
	DisplayCar(car);
	CP_Settings_NoStroke();
}