#pragma once
//#include "cprocessing.h"
#include "utils.h"


//======================|| CAR FUNCTIONS || ========================

void MovePhysicsObject(PhysicsObject* obj);
//Init function for car.c
void Car_Level_Init(void);
//Update function for car.c, Updates car visuals and movement.
void Car_Level_Update(void);
//Exit function for car.c, should clear the array 
void Car_Level_Exit(void);
//Updates specific physics object (checks if they have been clicked and selects them)
void UpdateCars(PhysicsObject* obj);





