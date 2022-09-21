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
_Bool IsShapeButtonHovered(ShapeButton *_button)
{
	return IsMouseInShapeArea(_button->shape);
}

_Bool IsShapeButtonClicked(ShapeButton *_button)
{
	return(CP_Input_MouseDown(MOUSE_BUTTON_LEFT) && IsShapeButtonHovered(_button));
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
//void UpdateButton(Button _button, CP_POSITION_MODE mode)
//{	
//	//Maybe next time add buttons to an array when created, then updatebutton just updates all buttons in the button array
//	//Requires dynamic arrays I think? 
//	DisplayButton(_button);
//
//	//Hacky way to change the color of the button when pressed imo. 
//	CP_ColorHSL cachedColor = CP_ColorHSL_FromColor(_button.rectData.color);
//	
//	//Moving the button up when its hovered is a nice way to show it's being hovered without color.
//	if (IsButtonHovered(_button, mode))
//	{
//		//TODO: fix the bug where you can actually see the previously rendered button when you hover and the button moves up.a
//		//Edit: fixing it causes it to constantly -2 and color change. Might need a coroutine or a state to check
//		_button.rectData.color = CP_Color_FromColorHSL(CP_ColorHSL_Create(cachedColor.h, (int)(cachedColor.s/1.1f), (int)(cachedColor.l/1.1f), 255));
//		_button.rectData.y -= 2;		//if only you could grab the stroke weight.. anyway, 2px also to hide the bug
//		CP_Settings_Stroke(GRAY);
//		DisplayButton(_button);
//	}
//	else
//	{
//		_button.rectData.color = CP_Color_FromColorHSL(cachedColor);
//	}
//	if (IsButtonClicked(_button, mode))
//	{
//		_button.buttEvent();
//		_button.rectData.color = CP_Color_FromColorHSL(CP_ColorHSL_Create(cachedColor.h, cachedColor.s / 2, cachedColor.l / 2, 255));
//		CP_Settings_Stroke(BLACK);
//		DisplayButton(_button);
//	}
//	else
//	{
//		_button.rectData.color = CP_Color_FromColorHSL(cachedColor);
//	}
//	//DisplayButton(_button);
//}

void UpdateShapebutton(ShapeButton *_button)
{
	//Maybe next time add buttons to an array when created, then updatebutton just updates all buttons in the button array
	//Requires dynamic arrays I think? 
	DisplayButton(_button);
	//Hacky way to change the color of the button when pressed imo. 
	CP_ColorHSL cachedColor = CP_ColorHSL_FromColor(_button->cachedColor);

	//Moving the button up when its hovered is a nice way to show it's being hovered without color.
	if (IsShapeButtonHovered(_button))
	{
		//TODO: Fix the bug where the button spazzes out if you hover from below. 
		//I think you probably need to check from the cached position.
		_button->color = CP_Color_FromColorHSL(CP_ColorHSL_Create(cachedColor.h, (int)(cachedColor.s * 1.1f), (int)(cachedColor.l / 1.1f), 255));
		_button->shape.transform.position.y =_button->cachedTransform.position.y - 3;		//if only you could grab the stroke weight.. anyway, 2px also to hide the bug
		CP_Settings_Stroke(GRAY);
		DisplayButton(_button);
		//_button->shape.transform = _button->cachedTransform;
	}
	else
	{
		_button->shape.transform = _button->cachedTransform;
		_button->color = CP_Color_FromColorHSL(cachedColor);
	}
	if (IsShapeButtonClicked(_button))
	{
		_button->buttEvent();
		_button->color = CP_Color_FromColorHSL(CP_ColorHSL_Create(cachedColor.h, cachedColor.s / 2, cachedColor.l / 2, 255));
		CP_Settings_Stroke(BLACK);
		DisplayButton(_button);
	}
	else
	{
		_button->color = CP_Color_FromColorHSL(cachedColor);
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
//I think I need to have an updatebutton function somewhere in here to constantly update all buttons.
//Maybe store them in some array and have it just call one UpdateGUI(); much like in unity.
//The settings in the UI can also be split and refined more.


//Creates a button at the positions specified, and sets its size, text and color.
//Button CreateButton(float _x, float _y, float _sizeX, float _sizeY,const char *text, CP_Color _color, ButtonEvent buttonFunction)
//{
//	Button newButton;
//	newButton.rectData = CreateRectArea(_x, _y, _sizeX, _sizeY);
//	newButton.rectData.color = _color;
//	newButton.text = text;
//	newButton.buttEvent = buttonFunction;
//	return newButton;
//}

ShapeButton CreateShapeButton(Shape shape, const char* buttonText,CP_Color color, ButtonEvent buttEvent) 
{
	ShapeButton newButton;
	//for now
	newButton.color = color;
	newButton.shape = shape;
	newButton.text = buttonText;
	newButton.buttEvent = buttEvent;
	newButton.cachedTransform = shape.transform;
	newButton.cachedColor = color;
	return newButton;
}


//Displays Text in the Rect Area specified. Default alignment is in the center
//Note: Font size is not set in this function
//void DisplayTextInRect(RectArea rect,const char *text)
//{
//	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
//	//Textbox pivots start from right to left
//	CP_Font_DrawTextBox(text, rect.x - (float)(rect.sizeX / 2.f), rect.y, rect.sizeX);
//}
void DisplayTextInShape(Shape shape, const char* text)
{
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	if(shape.shape == SHAPE_ELLIPSE || shape.shape == SHAPE_RECTANGLE)
	//Textbox pivots start from right to left
	CP_Font_DrawTextBox(text, shape.transform.position.x - (float)(shape.transform.size.x / 2.f), shape.transform.position.y, shape.transform.size.x);
	if(shape.shape == SHAPE_CIRCLE)
	CP_Font_DrawTextBox(text, shape.transform.position.x - (float)(shape.transform.size.x / 2.f), shape.transform.position.y- (float)(shape.transform.size.x / 2.f), shape.transform.size.x);

}
//Displays the button text, setting the color to the one specified.
//Note: Default color is BLACK. (If BLACK doesn't exist, colortable.h is missing)
void DisplayButtonText(ShapeButton *_button,CP_Color _color)
{
	CP_Settings_Fill(_color); //Color for the font, not button
	DisplayTextInShape(_button->shape, _button->text);
}
void DisplayButton(ShapeButton *button)
{
	CP_Settings_Fill(button->color);
	DisplayShape(button->shape);
	DisplayButtonText(button, BLACK);
	CP_Settings_NoStroke();
}

//Displays the Rect Area by using CProcessing's DrawRect, using the color specified.
//Note: This function calls the CP_Settings_Fill function first before drawing the rect area
//void DisplayRect(RectArea rect)
//{
//	CP_Settings_Fill(rect.color);
//	CP_Graphics_DrawRect(rect.x, rect.y, rect.sizeX, rect.sizeY);
//}

//Displays the Button and its text by calling DisplayRect and DisplayButtonText
//void DisplayButton(Button _button)
//{	//Default font color is black I guess
//	DisplayRect(_button.rectData);
//	DisplayButtonText(_button, BLACK);
//	CP_Settings_NoStroke();
//}

////Creates a new rect area at the specified position and sizes
////Note: Default color is LIGHT_GRAY.  (If LIGHT_GRAY doesn't exist, colortable.h is missing)
//RectArea CreateRectArea(float _x, float _y, float _sizeX, float _sizeY)
//{
//	//Maybe use vectors next time??
//	RectArea newRect;
//	newRect.x = _x;
//	newRect.y = _y;
//	newRect.sizeX = _sizeX;
//	newRect.sizeY = _sizeY;
//	newRect.color = LIGHT_GRAY;
//	return newRect;
//}
//
////Creates a rect area with specified position, size and color.
//RectArea CreateRectAreaWithColor(float _x, float _y, float _sizeX, float _sizeY,CP_Color _color)
//{
//	//Probably a redundant function. I just wanted to see if I could do function overloading.
//	RectArea newRect = CreateRectArea(_x, _y, _sizeX, _sizeY);
//	newRect.color = _color;
//	return newRect;
//}

//which apparently you can't overload functions :c


//Creates a circle area with specified position, diameter and color
CircleArea CreateCircleArea(float _x, float _y, float _diameter, CP_Color _color)
{
	CircleArea newCircle;
	newCircle.x = _x;
	newCircle.y = _y;
	newCircle.diameter = _diameter;
	newCircle.color = _color;
	return newCircle;
}

//Displays the circle with the given circle area data.
void DisplayCircle(CircleArea _circle)
{
	CP_Settings_Fill(_circle.color);
	CP_Graphics_DrawCircle(_circle.x, _circle.y, _circle.diameter);
}

//Checks if the mouse is within the circle area specied
//Note: this uses distance squared.
_Bool IsMouseInCircleArea(CircleArea circleArea)
{
	CP_Vector CirclePos = CP_Vector_Set(circleArea.x, circleArea.y);
	float distMouseCircle = Vector_Distance_Squared(CirclePos,GetMousePosition());
	float circleRadiusSquared = CP_Math_Square(circleArea.diameter / 2);

	return (distMouseCircle <= circleRadiusSquared) ? TRUE:FALSE;
}

_Bool IsMouseInShapeArea(Shape shape)
{
	//Assuming the rect mode is set to center
	float disMouseCircle;

	_Bool IsWithinX_CENTERMODE;
	_Bool IsWithinY_CENTERMODE;
	switch (shape.shape)
	{
		case SHAPE_CIRCLE:

			disMouseCircle = Vector_Distance_Squared(shape.transform.position, GetMousePosition());

			return (disMouseCircle <= CP_Math_Square(shape.transform.size.x));

			break;
		case SHAPE_RECTANGLE:
		{	//since x is in the center of the button now, you need to add the size of X/2 to check right, and -size of X/2 to check left. 
	//same for y axis (just in case anyone actually reads this lmao)
			IsWithinX_CENTERMODE = (GetMousePosition().x >= shape.transform.position.x - (shape.transform.size.x / 2.f)) && ((GetMousePosition().x <= shape.transform.position.x + (shape.transform.size.x / 2.f)));
			IsWithinY_CENTERMODE = (GetMousePosition().y >= shape.transform.position.y - (shape.transform.size.y / 2.f)) && ((GetMousePosition().y <= shape.transform.position.y + (shape.transform.size.y / 2.f)));
			if (IsWithinX_CENTERMODE && IsWithinY_CENTERMODE)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		case SHAPE_ELLIPSE:
			//Formula is ((x-h)^2/(rx^2)+(y-k)^2/(ry^2)) Where:
			//x,y = Mouse Position
			//h,k = Ellipse Position
			//rx,ry = ellipse Size
			disMouseCircle = (CP_Math_Square(GetMousePosition().x - shape.transform.position.x) / (CP_Math_Square(shape.transform.size.x/2))) + (CP_Math_Square(GetMousePosition().y - shape.transform.position.y)/(CP_Math_Square(shape.transform.size.y/2)));
			return (disMouseCircle < 1.f);	//if the dist < 1, true else > 1 false because out of the circle.
			break;
			
		default:
			return FALSE;

	}
}


Shape CreateShape(float x, float y, float sizeX, float sizeY,float rotation, ShapeType shapetype)
{
	Shape newShape;
	newShape.transform.position.x = x;
	newShape.transform.position.y = y;
	newShape.transform.size.x = sizeX;
	newShape.transform.size.y = sizeY;
	newShape.transform.rotation = rotation;
	newShape.shape = shapetype;
	return newShape;
}

void DisplayShape(Shape _shape)
{
	switch (_shape.shape)
	{
	case SHAPE_CIRCLE:
		CP_Graphics_DrawCircle(_shape.transform.position.x, _shape.transform.position.y, _shape.transform.size.x * 2);
		break;
	case SHAPE_RECTANGLE:
		CP_Graphics_DrawRectAdvanced(_shape.transform.position.x, _shape.transform.position.y, _shape.transform.size.x, _shape.transform.size.y, _shape.transform.rotation, 0);
		break;
	case SHAPE_ELLIPSE:
		CP_Graphics_DrawEllipseAdvanced(_shape.transform.position.x, _shape.transform.position.y, _shape.transform.size.x, _shape.transform.size.y, _shape.transform.rotation);
		break;
	default:
		break;
	}
}

//Returns the distance squared between 2 vectors (value is abs)
float Vector_Distance_Squared(CP_Vector a, CP_Vector b)
{
	float distSquared = (CP_Math_Square((b.x - a.x)) + CP_Math_Square((b.y - a.y)));
	return fAbs(distSquared);
}

//Checks if the mouse left click down on circle area specified
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

//Function that returns absolute float values
float fAbs(float f)
{
	if (f < 0)
	{
		return (f *= -1);
	}
	else return f;
}

//Returns a point in a circle area at angleInDeg away from VECTOR_RIGHT of the circle.
//Note: this function is not used anymore, it's outdated
CP_Vector PointInCircle(CircleArea circle, float angleInDeg)
{	//Assuming (1,0) in the circle is 0deg, rotates clockwise (note, origin is top left so it is clockwise)
	CP_Vector startV = VECTOR_RIGHT;
	
	//Thank you khan academy for this LMAOOOO
	float angleInRadians = CP_Math_Radians(angleInDeg);
	float x2 = (startV.x * cosf(angleInRadians)) - ((startV.y) * (sinf(angleInRadians)));
	float y2 = (startV.x * sinf(angleInRadians))+((startV.y)*(cosf(angleInRadians)));
	CP_Vector resultingVector = CP_Vector_Set((x2 * (circle.diameter / 2.f)+circle.x), (y2 * (circle.diameter/2.f)+circle.y));
	return resultingVector;
}

//Gets a point in the circle that is angleInDeg away from VECTOR_RIGHT
//Note: Increasing the angle returns a point in the clockwise direction from VECTOR_RIGHT
CP_Vector GetPointInCircle(CircleArea circle, float angleInDeg)
{
	CP_Vector circlePos = CP_Vector_Set(circle.x, circle.y);
	CP_Vector startV = CP_Vector_Set(circle.diameter/2, 0);
	CP_Vector result = RotateVectorByAngle(startV, angleInDeg);
	return CP_Vector_Add(circlePos,result);
}

//Rotates a vector by angleInDeg.
//Note: Increasing the angle returns a vector in the clockwise direction from VECTOR_RIGHT
CP_Vector RotateVectorByAngle(CP_Vector vector, float angleInDeg)
{
	float vectorMagnitude = CP_Vector_Length(vector);
	CP_Vector normalisedV = CP_Vector_Normalize(vector);
	float angleInRadians = CP_Math_Radians(angleInDeg);
	//Thank you khan academy for this LMAOOOO
	float x2 = (normalisedV.x * cosf(angleInRadians) - normalisedV.y * sinf(angleInRadians));
	float y2 = (normalisedV.x * sinf(angleInRadians) + normalisedV.y * (cosf(angleInRadians)));
	CP_Vector resultVector = CP_Vector_Set(x2 * vectorMagnitude, y2 * vectorMagnitude);
	return resultVector;
}

//Draws a triangle with the 3 points and rotation in degrees for the triangle
void DrawTriangleAdvanced(CP_Vector a, CP_Vector b, CP_Vector c, float deg)
{
	CP_Settings_Fill(FULL_WHITE);
	CP_Graphics_DrawTriangleAdvanced(a.x, a.y, b.x, b.y, c.x, c.y, deg);
}

CP_Vector Reflect(CP_Vector direction, CP_Vector normal)
{
	//r = d-2(d*n)n where n is normalised, d*n is dot product
	CP_Vector reflectedVector;
	normal = CP_Vector_Normalize(normal);
	float dotProduct = CP_Vector_DotProduct(direction, normal);
	reflectedVector = CP_Vector_Subtract(direction, (CP_Vector_Scale(normal, 2.f * (dotProduct))));
	return reflectedVector;
}