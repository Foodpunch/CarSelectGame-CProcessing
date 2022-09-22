#pragma once
#include "cprocessing.h"
#include "utils.h"

//Struct containing circleArea, direction, movespeed and angleInDegrees.
typedef struct Car
{
	//Maybe I should rename this to something more generic so I can resuse as a player or something
	CircleArea circleData;		//collider?
	CP_Vector direction;		//car
	CP_Vector velocity;			//rigidbody
	float mass;					//rigidbody 
	float acceleration;			//car
	float moveSpeed;			//car
	float angleInDeg;			//??? maybe it's the rigidbody???
} Car;


typedef struct BCar
{
	Shape carShape;
	CP_Color color;
	Collider carCollider;
	RigidBody2D rigidbody;
	CP_Vector direction;
	float acceleration;
	float moveSpeed;

} BCar;

//======================|| CAR FUNCTIONS || ========================
BCar CreateBCar(float x, float y, float diameter, CP_Color color, float speed, float mass);

void DisplayBCar(BCar* bcar);
void UpdateBCar(BCar* bcar);
void MoveBCar(BCar* bcar);


//Init function for car.c
void Car_Level_Init(void);
//Update function for car.c, Updates car visuals and movement.
void Car_Level_Update(void);
//Exit function for car.c, shoud
void Car_Level_Exit(void);
//Function that creates a car with the specified parameters.
Car CreateCar(float x, float y, float diameter, CP_Color color, float speed, float mass);
//Car CreateCar(CircleArea circle, float _moveSpeed); 
//Displays the car specified.
//Updates the car display and checks if the car has been clicked.
//Moves the specifed car with WASD mnovement.




