#include "cprocessing.h"
#include "utils.h"
#include "colortable.h"
#include "carlevel.h"


Car CarA;
Car CarB;
float gameTime;
void Car_Level_Init()
{
	CP_System_SetWindowSize(1000, 1000);
	CarA = CreateCar(CreateCircleArea(250, 150, 50, LIGHT_PURPLE),10);
	CarB = CreateCar(CreateCircleArea(250, 250, 50, LIGHT_ORANGE), 10);
}

void Car_Level_Update()
{
	gameTime += CP_System_GetDt();
	//DisplayCircle(CarA);
	UpdateCar(&CarA);
}

void Car_Level_Exit()
{

}


Car CreateCar(CircleArea circle, float _moveSpeed)
{
	Car newCar;
	newCar.circleData = circle;
	newCar.moveSpeed = _moveSpeed;
	newCar.direction = CP_Vector_Set(0, -1);
	newCar.angleInDeg = 0.f;
	return newCar;
}

void DisplayCar(Car _car)
{
	DisplayCircle(_car.circleData);
	DrawTriangleAdvanced(GetPointInCircle(_car.circleData, 30), GetPointInCircle(_car.circleData, 150), GetPointInCircle(_car.circleData, 270), _car.angleInDeg);
	CP_Graphics_ClearBackground(LIGHT_GRAY);
}

void UpdateCar(Car *car)
{
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
	car->direction = RotateVectorByAngle(CP_Vector_Set(0, -1), car->angleInDeg);
	DisplayCar(*car);
}