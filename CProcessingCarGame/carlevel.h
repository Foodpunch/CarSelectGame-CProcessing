#pragma once
#include "cprocessing.h"
#include "utils.h"


typedef struct Car
{
	CircleArea circleData;
	CP_Vector direction;
	float moveSpeed;
	float angleInDeg;
}Car;


//======================|| CAR FUNCTIONS || ========================





void Car_Level_Init(void);

void Car_Level_Update(void);

void Car_Level_Exit(void);

Car CreateCar(CircleArea circle, float _moveSpeed); 
void DisplayCar(Car _car);
void UpdateCar(Car *car);