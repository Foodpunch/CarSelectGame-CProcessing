#pragma once
#include "cprocessing.h"
#include "utils.h"

//Struct containing circleArea, direction, movespeed and angleInDegrees.
typedef struct Car
{
	//Maybe I should rename this to something more generic so I can resuse as a player or something
	CircleArea circleData;
	CP_Vector direction;
	CP_Vector velocity;
	float mass;
	float acceleration;
	float moveSpeed;
	float angleInDeg;
} Car;


//======================|| CAR FUNCTIONS || ========================





void Car_Level_Init(void);

void Car_Level_Update(void);

void Car_Level_Exit(void);
//Function that creates a car with the specified parameters.
Car CreateCar(float x, float y, float diameter, CP_Color color, float speed, float mass);
//Car CreateCar(CircleArea circle, float _moveSpeed); 
//Displays the car specified.
void DisplayCar(Car *_car);
//Updates the car display and checks if the car has been clicked.
void UpdateCar(Car *car);
//Moves the specifed car with WASD mnovement.
void MoveCar(Car *car);




