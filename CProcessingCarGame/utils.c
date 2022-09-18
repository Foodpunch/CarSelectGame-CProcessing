#include "cprocessing.h"
#include "utils.h"
#include "colortable.h"


//CProcessing's origin 0,0 starts at the top left.
//Basically an over engineered utils.c file. 
//At least making buttons in the future might be easier (?) lol


//Checks if button has been clicked, and changes it's Luminosity onClick
//Note: When clicked, it sets the Saturation and Luminosity to half it's current amount
//On release, it sets it back to the original color. 
//DisplayButton is called to set the color.
_Bool IsButtonClicked(Button _button, CP_Vector mousePos, CP_POSITION_MODE mode)
{
	
	if (IsAreaClicked(_button.rectData, mousePos, mode))
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
void UpdateButton(Button _button, CP_Vector mousePos, CP_POSITION_MODE mode)
{
	//Hacky way to change the color of the button when pressed imo
	CP_ColorHSL cachedColor = CP_ColorHSL_FromColor(_button.rectData.color);
	if (IsButtonClicked(_button, mousePos, mode))
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
}

//Checks if the mouse has clicked within the rect area
_Bool IsAreaClicked(RectArea rect, CP_Vector mousePos, CP_POSITION_MODE mode)
{
	//Might not be correct to do this here but I'm testing
	if (mode == CP_POSITION_CORNER) //By default we use center ba..
	{	//simple checking of bounds
		_Bool IsWithinX_CORNERMODE = (mousePos.x >= rect.x) && (mousePos.x <= (rect.x + rect.sizeX));
		_Bool IsWithinY_CORNERMODE = (mousePos.y >= rect.y) && (mousePos.y <= (rect.y + rect.sizeY));
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
		_Bool IsWithinX_CENTERMODE = (mousePos.x >= rect.x - (rect.sizeX / 2.f)) && ((mousePos.x <= rect.x + (rect.sizeX / 2.f)));
		_Bool IsWithinY_CENTERMODE = (mousePos.y >= rect.y - (rect.sizeY / 2.f)) && ((mousePos.y <= rect.y + (rect.sizeY / 2.f)));
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
	RectArea newRect = CreateRectArea(_x, _y, _sizeX, _sizeY);
	newRect.color = _color;
	return newRect;
}

//Apparently you can't overload functions :c

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

_Bool IsCircleAreaClicked(CircleArea circleArea, CP_Vector mousePos)
{
	CP_Vector CirclePos = CP_Vector_Set(circleArea.x, circleArea.y);
	float distMouseCircle = CP_Vector_Distance(mousePos, CirclePos);

	return (distMouseCircle <= (circleArea.diameter/2))? TRUE:FALSE;
}

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y)
{
	// TODO
	return 0;
}

CP_Vector AngleToVector(float radian_angle)
{
    // TODO 
    CP_Vector ret = CP_Vector_Zero();
    return ret;
}

