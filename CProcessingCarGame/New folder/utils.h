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
void ClearButtonArray(void);


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



//==============|| COLOR TABLE ||================================
//@version 0.3 last updated 27/09/2022
//@author Amadeus Chia Jinhan (don't remove my credit pls :c)

// LGBT Table
// This many defines feels BAD, Function Macros feel BAD
// but I'm lazy



//===================RED=====================//

//Red that is on the darker side
#define DARK_RED CP_Color_Create(102,0,0,255)
//RED, like R = 255 G =0 B = 0 kind of eye blinding red.
#define RED CP_Color_Create(255,0,0,255)
//Light red, or lite red or pink or whatever. 
#define LIGHT_RED CP_Color_Create(255,153,153,255)


//===================ORANGE=====================//

//Dark orange. The color of oranges about to go bad.
#define DARK_ORANGE CP_Color_Create(102,51,0,255)
//Orange, full red, half green somehow makes orange.
#define ORANGE CP_Color_Create(255,128,0,255)
//Light orange, almost like beige.
#define LIGHT_ORANGE CP_Color_Create(255,204,153,255)


//===================YELLOW=====================//

//Dark yellow, almost bile-like in color.
#define DARK_YELLOW CP_Color_Create(153,153,0,255)
//Yellow, full red and rull green.
#define YELLOW CP_Color_Create(255,255,0,255)
//Light Yellow, somehow adding 153 blue to yellow makes it... lighter
#define LIGHT_YELLOW CP_Color_Create(255,255,153,255)


//===================GREEN=====================//

//Dark green, the color of leaves.
#define DARK_GREEN CP_Color_Create(0,153,0,255)
//GREEN, like R =0, G = 255, B = 0, eyeblinding green.
#define GREEN CP_Color_Create(0,255,0,255)
//Light green, almost the color of mint ice cream
#define LIGHT_GREEN CP_Color_Create(153,255,153,255)


//===================BLUE=====================//

//Dark blue, almost like blue pen ink blue
#define DARK_BLUE CP_Color_Create(0,0,153,255)
//BLUE, like R = 0, G = 0 B = 255 kind of blue. Not very close to the other blues here.
#define BLUE CP_Color_Create(0,0,255,255)
//Light blue, a bit lighter than say.. the telegram logo blue
#define LIGHT_BLUE CP_Color_Create(153,204,255,255)


//===================VIOLET(Or purple for plebs)====//

//Dark purple, the color of an eggplant
#define DARK_PURPLE CP_Color_Create(51,0,102,255)
//Purple, half red and full blue
#define PURPLE CP_Color_Create(102,0,204,255)
//Light purple, almost like visual studios logo light purple
#define LIGHT_PURPLE CP_Color_Create(204,153,255,255)


//===================INDIGO(pink for plebs)====//

//Dark pink. Not actually red but more like the color of grapes.
#define DARK_PINK CP_Color_Create(102,0,102,255)
//Pink, full red and full blue. Almost hot pink. Also known as MAGENTA
#define PINK CP_Color_Create(255,0,255,255)
//Magenta, full red and full blue. Almost hot pink. Also known as PINK
#define MAGENTA CP_Color_Create(255,0,255,255)
//Light pink, bubble gum or light magenta
#define LIGHT_PINK CP_Color_Create(255,153,255,255)

//==================50 Shades of Gray===========//

//Dark Gray.
#define DARK_GRAY CP_Color_Create(96,96,96,255)
//Gray
#define GRAY CP_Color_Create(128,128,128,255)
//Light gray
#define LIGHT_GRAY CP_Color_Create(224,224,224,255)
//Black. Note, it's not a full black.
#define BLACK CP_Color_Create(32,32,32,255)


//================PASTEL COLORS=================//
//Some colors have bad names, especially from green onwards, the names aren't very clear.

//Pastel Red
#define PASTEL_RED CP_Color_Create(255,204,204,255)
//Pastel Orange
#define PASTEL_ORANGE CP_Color_Create(255,229,204,255)
//Pastel Yellow
#define PASTEL_YELLOW CP_Color_Create(255,229,204,255)
//Pastel Tea Green
#define PASTEL_GREEN CP_Color_Create(224,255,204,255)
//Pastel Mint Green
#define PASTEL_GREEN2 CP_Color_Create(204,255,204,255)
//Pastel Turquioise
#define PASTEL_GREEN3 CP_Color_Create(204,255,229,255)
//Pastel Light Blue
#define PASTEL_LITEBLUE CP_Color_Create(204,255,255,255)
//Pastel Blue
#define PASTEL_BLUE CP_Color_Create(204,229,255,255)
//Pastel Indigo
#define PASTEL_INDIGO CP_Color_Create(204,204,255,255)
//Pastel Violet
#define PASTEL_VIOLET CP_Color_Create(229,204,255,255)
//Pastel Pink
#define PASTEL_PINK CP_Color_Create(255,204,255,255)
//Pastel Rose
#define PASTEL_PINK2 CP_Color_Create(255,204,229,255)


//Full White
#define FULL_WHITE CP_Color_Create(255,255,255,255)
//Full Black
#define FULL_BLACK CP_Color_Create(0,0,0,255)
