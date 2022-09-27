#pragma once
#include "cprocessing.h"
#include "cprocessing_common.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//Can probably rename this to gamecore or coreutils or something


extern float trauma;

typedef struct Transform
{
	CP_Vector position;
	CP_Vector size;
	float rotation;
	//maybe include a pointer for another transform here for parent child stuff
} Transform;

typedef enum ShapeType
{
	SHAPE_RECTANGLE,
	//ROUNDED RECTANGLE HERE NEXT TIME
	SHAPE_CIRCLE,
	SHAPE_TRIANGLE,
	SHAPE_ELLIPSE
} ShapeType;

typedef struct Shape
{
	Transform transform;
	ShapeType shape;
}Shape;

typedef void(*ColliderEvent)(void);

typedef struct Collider
{
	Shape shape;
	_Bool isTrigger;
	int colliderLayer;
	ColliderEvent colliderEvent;
}Collider;
typedef enum Constraints
{
	CONSTRAINTS_NONE,
	CONSTRAINTS_LEVEL
}Constraints;
typedef struct RigidBody2D
{
	Collider collider;
	CP_Vector velocity;
	CP_Vector force;
	Constraints constraints;
	float mass;
	float gravityScale;
	//maybe need to do the constraints here too?
}RigidBody2D;



typedef struct PhysicsObject
{
	CP_Color color;
	RigidBody2D rigidBody;
	int id;
	CP_Vector direction;
	float speed;

}PhysicsObject;

extern PhysicsObject PhysicsObjectArray[100];

typedef struct CollisionData
{
	PhysicsObject* ObjA;
	PhysicsObject* ObjB;
	int id;
} CollisionData;

extern CollisionData CollisionObjectsArray[100];

typedef enum ForceMode				//Plucked from UNITY
{
	FORCEMODE_IMPULSE,				//Instant, for maybe like explosions
	FORCEMODE_FORCE,				//Force over time, using mass
	//FOREMODE_ACCELERATION			//Continuous Acceleration over time ignoring mass
	//FORCEMODE_VELOCITYCHANGE		//INstant velocity change ignoring mass
}ForceMode;


typedef void(*ButtonEvent)(void);

typedef struct Button
{
	Shape shape;
	int id;
	CP_Color color;
	char* text;
	ButtonEvent buttEvent;
	Transform cachedTransform;
	CP_Color cachedColor;		//feels so dirty to do this hmm...
	//ShapeButton cachedButton; //I think this is not allowed because the size isn't defined yet? 
} Button;
extern Button ButtonArray[100];
//===============|| VECTOR DEFINES || =================================

//VECTOR UP: (0,-1)
#define VECTOR_UP CP_Vector_Set(0,-1)
//VECTOR DOWN: (0,1)
#define VECTOR_DOWN CP_Vector_Set(0,1)
//VECTOR RIGHT: (1,0)
#define VECTOR_RIGHT CP_Vector_Set(1,0)
//VECTOR LEFT: (-1,0)
#define VECTOR_LEFT CP_Vector_Set(-1,0)

//============|| PHYSICS STUFF ||========================================
RigidBody2D CreateRigidBody(Collider coll, float mass, Constraints constraints);
Collider CreateCollider(Shape shape, _Bool isTrigger);
void OnCollisionEnter(void);
void UpdatePhysics(void);
void AddForce(PhysicsObject rb, CP_Vector force, ForceMode forcemode);

PhysicsObject CreatePhysicsObject(float x, float y, float diameter, CP_Color color, float speed, float mass,Constraints constraints);
void UpdatePhysicsObjects(PhysicsObject* obj);
PhysicsObject GetPhysicsObject(PhysicsObject* obj);
void CreateCollisionData(PhysicsObject* ObjA, PhysicsObject* ObjB);

_Bool IsCircleIntersecting(Shape shapeA, Shape shapeB);
void CheckCollisionsBruteForce(void); //Next time do sweep and prune or something

//=============|| GUI && BUTTON STUFF ||=================================

Shape CreateShape(float x, float y, float sizeX, float sizeY,float rotation, ShapeType shapetype);
Button CreateButton(float x, float y, float sizeX, float sizeY, ShapeType shapeType, const char* text, CP_Color color, ButtonEvent buttEvent);
Button CreateShapeButton(Shape shape, const char* buttonText, CP_Color color, ButtonEvent buttEvent);
void DisplayShape(Shape shape);
_Bool IsMouseInShapeArea(Shape shape);
_Bool IsMouseClickedShapeArea(Shape shape);
_Bool IsShapeButtonClicked(Button *_button);
_Bool IsShapeButtonHovered(Button *_button);
void UpdateShapebutton(Button *_button);
void DisplayTextInShape(Shape shape, const char* text);
void DisplayButtonText(Button *_button, CP_Color _color);
void DisplayButton(Button *_button);
void UpdateGUI(void);
void RemoveButton(Button button);


//===============|| UTILITY FUNCTIONS || =============================

//Gets Mouse Position
CP_Vector GetMousePosition();
//Returns the absolute value of a float
float fAbs(float f);
//Returns the squared distance between two vectors
float Vector_Distance_Squared(CP_Vector a, CP_Vector b);
//[DEPRECATED] Returns a point in the circumference of a circle. 
//Note: 0 degrees is center of circle + radius. Increasing the angle returns a point in the clockwise direction.
CP_Vector PointInCircle(Shape circle, float angleInDeg);
//Returns the angle to vector. [NOT IMPLEMENTED YET]
union CP_Vector AngleToVector(float radian_angle);
//Draws a triangle with the 3 points specified. The triangle is then rotated by the degrees specified.
void DrawTriangleAdvanced(CP_Vector a, CP_Vector b, CP_Vector c, float deg);
//Rotates a given vector by angleInDeg. Rotation is applied in the clockwise direction.
CP_Vector RotateVectorByAngle(CP_Vector vector, float angleInDeg);
//Returns a point in the circumference of a circle. 
//Note: 0 degrees is center of circle + radius. Increasing the angle returns a point in the clockwise direction.
CP_Vector GetPointInCircle(Shape shape, float angleInDeg);
//returns the reflected vector normalised
CP_Vector Reflect(CP_Vector direction, CP_Vector normal);
float PerlinNoise(int x, int y);	
CP_Vector GetRandomVector(void);

float fMax(float a, float b);
float fMin(float a, float b);


float DistanceBetweenPhysicsObject(PhysicsObject ObjA, PhysicsObject ObjB);

//===============|| CAMERA FUNCTIONS || =============================
//Function that Updates the camera shaker. Must be called in update loop for the camera shaker to work.
void UpdateCameraShaker(void);
//Example function to shake the camera. 
//Note: Trauma value is clamped between 0-1. Anything above 0 will produce a shake.
//Edit the strength of the shake by editing the trauma numbers.
void Shake(void);