#pragma once
#include "cprocessing.h"

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

//Struct containing Rect Area and Text.
typedef struct Button
{
	RectArea rectData;
	char *text;		//Note: lol don't forget the pointer...
	//imagine a world where I made enum states for the button... ENABLED,DISABLED,ACTIVE
} Button;

//===============|| RECT AREA FUNCTIONS || =============================

//Creates a rectangular area at the position, size and color specified.
RectArea CreateRectAreaWithColor(float _x, float _y, float _sizeX, float _sizeY, CP_Color _color);
//Creates a rectangular area at the position and size specified.
RectArea CreateRectArea(float x, float y, float sizeX, float sizeY);
//Displays text inside the rectangular area given.
void DisplayTextInRect(RectArea rect,const char *text);
//Displays the rectangular area given
void DisplayRect(RectArea rect);

//===============|| CIRCLE AREA FUNCTIONS || =============================

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);
_Bool IsCircleAreaClicked(CircleArea circleArea, CP_Vector mousePos);
void DisplayCircle(CircleArea _circle);
CircleArea CreateCircleArea(float _x, float _y, float _diameter, CP_Color _color);



//===============|| BUTTON FUNCTIONS || =============================

//Creates a button at the position, size and color specified, with the string as its name.
Button CreateButton(float _x, float _y, float _sizeX, float _sizeY,const char *text, CP_Color color);
//Displays text inside the button at the font color specified.
void DisplayButtonText(Button butt, CP_Color color);
//Displays the button specified.
void DisplayButton(Button butt);
//Checks if the button specified has been clicked and changes the color of the button if it has.
_Bool IsButtonClicked(Button _button, CP_Vector mousePos, CP_POSITION_MODE mode);

//===============|| UTILITY FUNCTIONS || =============================

//Checks if an area specified has been clicked.
_Bool IsAreaClicked(RectArea rect, CP_Vector mousePos, CP_POSITION_MODE mode);
//int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

union CP_Vector AngleToVector(float radian_angle);
