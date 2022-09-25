#pragma once
#include "cprocessing.h"
#include "utils.h"


//======================|| CAR FUNCTIONS || ========================

void MovePhysicsObject(PhysicsObject* obj);
//Init function for car.c
void Car_Level_Init(void);
//Update function for car.c, Updates car visuals and movement.
void Car_Level_Update(void);
//Exit function for car.c, shoud
void Car_Level_Exit(void);
void OnCarClick(PhysicsObject* obj);
//Function that creates a car with the specified parameters.
//Car CreateCar(float x, float y, float diameter, CP_Color color, float speed, float mass);
//Car CreateCar(CircleArea circle, float _moveSpeed); 
//Displays the car specified.
//Updates the car display and checks if the car has been clicked.
//Moves the specifed car with WASD mnovement.




