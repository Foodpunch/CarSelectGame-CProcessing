#pragma once
#include "cprocessing.h"
#include "cprocessing_common.h"
#include <math.h>

//Can probably rename this to gamecore

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

typedef struct RigidBody2D
{
	CP_Vector velocity;
	float mass;
	float gravityScale;
	//maybe need to do the constraints here too?
}RigidBody2D;

//Struct containing the 4 floats, corresponding to position x, position y, size x and size y and color 
//to define a Rect Area. 
//Can be used to create multiple Rect Areas of different colors and sizes
typedef struct RectArea	
{
	float x, y;
	float sizeX, sizeY;
	CP_Color color;
} RectArea; //feels like a bad name to describe stuff

typedef struct CircleArea
{
	float x, y;
	float diameter;
	CP_Color color;
} CircleArea; //maybe should have done a vector position

typedef void(*ButtonEvent)(void);


////Struct containing Rect Area and Text.
//typedef struct Button
//{
//	RectArea rectData;
//	char *text;		//Note: lol don't forget the pointer...
//	ButtonEvent buttEvent;
//
//	//imagine a world where I made enum states for the button... ENABLED,DISABLED,ACTIVE
//	//also imagine a world where buttons can be circles too and you can just check for button..
//	//Edit: button states would be really good...
//} Button;

typedef struct Button
{
	Shape shape;
	CP_Color color;
	char* text;
	ButtonEvent buttEvent;
	Transform cachedTransform;
	CP_Color cachedColor;		//feels so dirty to do this hmm...
	//ShapeButton cachedButton; //I think this is not allowed because the size isn't defined yet? 
} Button;

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
RigidBody2D CreateRigidBody(Collider collider, float mass, float gravityScale);
Collider CreateCollider(Shape shape, _Bool isTrigger);
void OnCollisionEnter(void);




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
void UpdateGUI(void);
//===============|| RECT AREA FUNCTIONS || =============================

//Creates a rectangular area at the position, size and color specified.
//RectArea CreateRectAreaWithColor(float _x, float _y, float _sizeX, float _sizeY, CP_Color _color);
//Creates a rectangular area at the position and size specified.
//RectArea CreateRectArea(float x, float y, float sizeX, float sizeY);
//Displays text inside the rectangular area given. D
//Note: alignment for text is defaulted to center v center h.
//void DisplayTextInRect(RectArea rect,const char *text);
//Displays the rectangular area given
//void DisplayRect(RectArea rect);


//===============|| BUTTON FUNCTIONS || =============================

//Creates a button at the position, size and color specified, with the string as its name.
//Button CreateButton(float _x, float _y, float _sizeX, float _sizeY,const char *text, CP_Color color, ButtonEvent buttEvent);
//Displays text inside the button at the font color specified.
//void DisplayButtonText(Button butt, CP_Color color);
//Displays the button specified.
//void DisplayButton(Button butt);
//Checks if the button specified has been clicked.
//_Bool IsButtonClicked(Button _button, CP_POSITION_MODE mode);
//Checks if mouse is hovering over button specified.
//_Bool IsButtonHovered(Button _button, CP_POSITION_MODE mode);
//Updates the button's graphic depending on if hovered or clicked. If clicked, it calls the function assigned to the button.
//void UpdateButton(Button _button, CP_POSITION_MODE mode);

//===============|| UTILITY FUNCTIONS || =============================

//Gets Mouse Position
CP_Vector GetMousePosition();

//Checks if an area specified has been clicked.
//_Bool MouseInRectArea(RectArea rect, CP_POSITION_MODE mode);
//int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

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

//===============|| CAMERA FUNCTIONS || =============================
//Function that Updates the camera shaker. Must be called in update loop for the camera shaker to work.
void UpdateCameraShaker(void);
//Example function to shake the camera. 
//Note: Trauma value is clamped between 0-1. Anything above 0 will produce a shake.
//Edit the strength of the shake by editing the trauma numbers.
void Shake(void);