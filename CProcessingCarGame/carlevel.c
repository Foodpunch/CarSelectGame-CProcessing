#include "cprocessing.h"
#include "utils.h"
#include "colortable.h"
#include "carlevel.h"


Car CarA;
Car CarB;
Car CarC;
float gameTime;


Car* selectedCar;

void Car_Level_Init()
{
	CP_System_SetWindowSize(1000, 1000);
	CarA = CreateCar(250, 150, 50, LIGHT_PURPLE,10);
	CarB = CreateCar(250, 250, 50, LIGHT_ORANGE, 10);
	CarC = CreateCar(250, 350, 50, LIGHT_PINK, 10);

	selectedCar = &CarA;
}

void Car_Level_Update()
{
	UpdateCar(&CarA);
	UpdateCar(&CarB);
	UpdateCar(&CarC);

	MoveCar(selectedCar);
	
	CP_Graphics_ClearBackground(LIGHT_GRAY);
	
}

void Car_Level_Exit()
{

}

Car CreateCar(float x, float y, float diameter, CP_Color color, float speed)
{
	CircleArea _circle = CreateCircleArea(x, y, diameter, color);
	Car newCar;
	newCar.circleData = _circle;
	newCar.moveSpeed = speed;
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
	DisplayCar(car);
}

//Moves the specified car with WASD movement keys. Arrow key implementation not added 
void MoveCar(Car *car)
{
	//Note: The fancy pointer stuff here is because structs are NOT the same as classes. You need to use pointers if you
	//wanna modify the values, if not you're just copying and applying changes to the copy.
	if (CP_Input_KeyDown(KEY_W))
	{
		car->circleData.x += (car->direction.x * car->moveSpeed);
		car->circleData.y += (car->direction.y * car->moveSpeed);
	}
	if (CP_Input_KeyDown(KEY_S))
	{
		car->circleData.x -= (car->direction.x * car->moveSpeed);
		car->circleData.y -= (car->direction.y * car->moveSpeed);
	}
	if (CP_Input_KeyDown(KEY_D))
	{
		car->angleInDeg += 10.f;
	}
	if (CP_Input_KeyDown(KEY_A))
	{
		car->angleInDeg -= 10.f;
	}
	car->direction = RotateVectorByAngle(VECTOR_UP, car->angleInDeg);
}