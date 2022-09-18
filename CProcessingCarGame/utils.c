#include "cprocessing.h"
#include "utils.h"
#include "colortable.h"


//CProcessing's origin 0,0 starts at the top left.
//Basically an over engineered utils.c file. 
//At least making buttons in the future might be easier (?) lol

//Gets the mouse input X and Y from CProcessing and puts them into a vector
CP_Vector GetMousePosition()
{
	return CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());
}

//Checks if button has been clicked, uses CProcessing's mouse inputs
_Bool IsButtonClicked(Button _button, CP_POSITION_MODE mode)
{
	if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT) && IsButtonHovered(_button,mode))
	{
		return TRUE;
	}
	else return FALSE;
}

//Checks if button is being hovered over by the mouse using CProcessing's mouse inputs
_Bool IsButtonHovered(Button _button, CP_POSITION_MODE mode)
{
	if (MouseInRectArea(_button.rectData, mode))
	{

		return TRUE;
	}
	else
	{

		return FALSE;
	}
}

//Updates the button if it has been clicked, changing it's color by halving saturation and luminosity and adding a BLACK stroke
//Also calls the function in the button
void UpdateButton(Button _button, CP_POSITION_MODE mode)
{	
	//Maybe next time add buttons to an array when created, then updatebutton just updates all buttons in the button array
	//Requires dynamic arrays I think? 
	DisplayButton(_button);

	//Hacky way to change the color of the button when pressed imo. 
	CP_ColorHSL cachedColor = CP_ColorHSL_FromColor(_button.rectData.color);
	
	//Moving the button up when its hovered is a nice way to show it's being hovered without color.
	if (IsButtonHovered(_button, mode))
	{
		//TODO: fix the bug where you can actually see the previously rendered button when you hover and the button moves up.a
		_button.rectData.color = CP_Color_FromColorHSL(CP_ColorHSL_Create(cachedColor.h, (int)(cachedColor.s/1.1f), (int)(cachedColor.l/1.1f), 255));
		_button.rectData.y -= 2;		//if only you could grab the stroke weight.. anyway, 2px also to hide the bug
		CP_Settings_Stroke(GRAY);
		DisplayButton(_button);
	}
	else
	{
		_button.rectData.color = CP_Color_FromColorHSL(cachedColor);
	}
	if (IsButtonClicked(_button, mode))
	{
		_button.buttEvent();
		_button.rectData.color = CP_Color_FromColorHSL(CP_ColorHSL_Create(cachedColor.h, cachedColor.s / 2, cachedColor.l / 2, 255));
		CP_Settings_Stroke(BLACK);
		DisplayButton(_button);
	}
	else
	{
		_button.rectData.color = CP_Color_FromColorHSL(cachedColor);
	}
	//DisplayButton(_button);
}

//Checks if the mouse position is within the rect area
_Bool MouseInRectArea(RectArea rect, CP_POSITION_MODE mode)
{
	//Might not be correct to do this here but I'm testing
	if (mode == CP_POSITION_CORNER) //By default we use center ba..
	{	//simple checking of bounds
		_Bool IsWithinX_CORNERMODE = (GetMousePosition().x >= rect.x) && (GetMousePosition().x <= (rect.x + rect.sizeX));
		_Bool IsWithinY_CORNERMODE = (GetMousePosition().y >= rect.y) && (GetMousePosition().y <= (rect.y + rect.sizeY));
		if (IsWithinX_CORNERMODE && IsWithinY_CORNERMODE)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else if (mode == CP_POSITION_CENTER)
	{	//since x is in the center of the button now, you need to add the size of X/2 to check right, and -size of X/2 to check left. 
		//same for y axis (just in case anyone actually reads this lmao)
		_Bool IsWithinX_CENTERMODE = (GetMousePosition().x >= rect.x - (rect.sizeX / 2.f)) && ((GetMousePosition().x <= rect.x + (rect.sizeX / 2.f)));
		_Bool IsWithinY_CENTERMODE = (GetMousePosition().y >= rect.y - (rect.sizeY / 2.f)) && ((GetMousePosition().y <= rect.y + (rect.sizeY / 2.f)));
		if (IsWithinX_CENTERMODE && IsWithinY_CENTERMODE)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	//probably should do a debug.log error here? idk
	return FALSE;
}





//Wrapper for some reason. I feel like this is correct? but at the same time it feels redundant

//Creates a button at the positions specified, and sets its size, text and color.
Button CreateButton(float _x, float _y, float _sizeX, float _sizeY,const char *text, CP_Color _color, ButtonEvent buttonFunction)
{
	Button newButton;
	newButton.rectData = CreateRectArea(_x, _y, _sizeX, _sizeY);
	newButton.rectData.color = _color;
	newButton.text = text;
	newButton.buttEvent = buttonFunction;
	return newButton;
}

//Displays Text in the Rect Area specified. Default alignment is in the center
//Note: Font size is not set in this function
void DisplayTextInRect(RectArea rect,const char *text)
{
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	//Textbox pivots start from right to left
	CP_Font_DrawTextBox(text, rect.x - (float)(rect.sizeX / 2.f), rect.y, rect.sizeX);
}

//Displays the button text, setting the color to the one specified.
//Note: Default color is BLACK. (If BLACK doesn't exist, colortable.h is missing)
void DisplayButtonText(Button _button,CP_Color _color)
{
	CP_Settings_Fill(_color); //Color for the font, not button
	DisplayTextInRect(_button.rectData, _button.text);
}

//Displays the Rect Area by using CProcessing's DrawRect, using the color specified.
//Note: This function calls the CP_Settings_Fill function first before drawing the rect area
void DisplayRect(RectArea rect)
{
	CP_Settings_Fill(rect.color);
	CP_Graphics_DrawRect(rect.x, rect.y, rect.sizeX, rect.sizeY);
}

//Displays the Button and its text by calling DisplayRect and DisplayButtonText
void DisplayButton(Button _button)
{	//Default font color is black I guess
	DisplayRect(_button.rectData);
	DisplayButtonText(_button, BLACK);
	CP_Settings_NoStroke();
}

//Creates a new rect area at the specified position and sizes
//Note: Default color is LIGHT_GRAY.  (If LIGHT_GRAY doesn't exist, colortable.h is missing)
RectArea CreateRectArea(float _x, float _y, float _sizeX, float _sizeY)
{
	//Maybe use vectors next time??
	RectArea newRect;
	newRect.x = _x;
	newRect.y = _y;
	newRect.sizeX = _sizeX;
	newRect.sizeY = _sizeY;
	newRect.color = LIGHT_GRAY;
	return newRect;
}

//Creates a rect area with specified position, size and color.
RectArea CreateRectAreaWithColor(float _x, float _y, float _sizeX, float _sizeY,CP_Color _color)
{
	//Probably a redundant function. I just wanted to see if I could do function overloading.
	RectArea newRect = CreateRectArea(_x, _y, _sizeX, _sizeY);
	newRect.color = _color;
	return newRect;
}

//which apparently you can't overload functions :c

CircleArea CreateCircleArea(float _x, float _y, float _diameter, CP_Color _color)
{
	CircleArea newCircle;
	newCircle.x = _x;
	newCircle.y = _y;
	newCircle.diameter = _diameter;
	newCircle.color = _color;
	return newCircle;
}

void DisplayCircle(CircleArea _circle)
{
	CP_Settings_Fill(_circle.color);
	CP_Graphics_DrawCircle(_circle.x, _circle.y, _circle.diameter);
}


_Bool IsMouseInCircleArea(CircleArea circleArea)
{
	CP_Vector CirclePos = CP_Vector_Set(circleArea.x, circleArea.y);
	float distMouseCircle = Vector_Distance_Squared(CirclePos,GetMousePosition());
	float circleRadiusSquared = CP_Math_Square(circleArea.diameter / 2);

	return (distMouseCircle <= circleRadiusSquared) ? TRUE:FALSE;
}

float Vector_Distance_Squared(CP_Vector a, CP_Vector b)
{
	float distSquared = (CP_Math_Square((b.x - a.x)) + CP_Math_Square((b.y - a.y)));
	return fAbs(distSquared);
}

_Bool IsCircleClicked(CircleArea _circle)
{
	if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT) && IsMouseInCircleArea(_circle))
	{
		return TRUE;
	}
	else return FALSE;
}

CP_Vector AngleToVector(float radian_angle)
{
    // TODO 
    CP_Vector ret = CP_Vector_Zero();
    return ret;
}

float fAbs(float f)
{
	if (f < 0)
	{
		return (f *= -1);
	}
	else return f;
}

CP_Vector PointInCircle(CircleArea circle, float angleInDeg)
{	//Assuming (1,0) in the circle is 0deg, rotates clockwise (note, origin is top left so it is clockwise)
	CP_Vector startV = CP_Vector_Set(1, 0);
	
	//Thank you khan academy for this LMAOOOO
	float angleInRadians = CP_Math_Radians(angleInDeg);
	float x2 = (startV.x * cosf(angleInRadians)) - ((startV.y) * (sinf(angleInRadians)));
	float y2 = (startV.x * sinf(angleInRadians))+((startV.y)*(cosf(angleInRadians)));
	CP_Vector resultingVector = CP_Vector_Set((x2 * (circle.diameter / 2.f)+circle.x), (y2 * (circle.diameter/2.f)+circle.y));
	return resultingVector;
}


CP_Vector GetPointInCircle(CircleArea circle, float angleInDeg)
{
	CP_Vector circlePos = CP_Vector_Set(circle.x, circle.y);
	CP_Vector startV = CP_Vector_Set(circle.diameter/2, 0);
	CP_Vector result = RotateVectorByAngle(startV, angleInDeg);
	return CP_Vector_Add(circlePos,result);
}


CP_Vector RotateVectorByAngle(CP_Vector vector, float angleInDeg)
{
	float vectorMagnitude = CP_Vector_Length(vector);
	CP_Vector normalisedV = CP_Vector_Normalize(vector);
	float angleInRadians = CP_Math_Radians(angleInDeg);
	float x2 = (normalisedV.x * cosf(angleInRadians) - normalisedV.y * sinf(angleInRadians));
	float y2 = (normalisedV.x * sinf(angleInRadians) + normalisedV.y * (cosf(angleInRadians)));
	CP_Vector resultVector = CP_Vector_Set(x2 * vectorMagnitude, y2 * vectorMagnitude);
	return resultVector;
}

void DrawTriangleAdvanced(CP_Vector a, CP_Vector b, CP_Vector c, float deg)
{
	CP_Settings_Fill(FULL_WHITE);
	CP_Graphics_DrawTriangleAdvanced(a.x, a.y, b.x, b.y, c.x, c.y, deg);
}