#include "utils.h"
#include "cprocessing.h"
#include "mainmenu.h"
#include "colortable.h"

CP_POSITION_MODE menuRectPosition = CP_POSITION_CENTER;

Button StartGameButton;
Button ExitGameButton;

CircleArea _testCircle;

void Main_Menu_Init()
{
	CP_System_SetWindowSize(500, 500);
	CP_Settings_RectMode(menuRectPosition);

	StartGameButton = CreateButton(250, 150, 150, 50, "Start Game", LIGHT_BLUE);
	ExitGameButton = CreateButton(250, 250, 150, 50, "Exit Game", LIGHT_BLUE);

	_testCircle = CreateCircleArea(250, 400, 50, DARK_RED);
}

void Main_Menu_Update()
{
	DisplayButton(StartGameButton);
	DisplayButton(ExitGameButton);
	DisplayCircle(_testCircle);


	CP_Vector mousePosition = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());





	if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT))
	{
		if (IsButtonClicked(StartGameButton, mousePosition, menuRectPosition))
		{
			
			//TODO
			//Clicking here will go to car level
		}
		if (IsButtonClicked(ExitGameButton, mousePosition, menuRectPosition))
		{
			CP_Engine_Terminate();
		}
		if (IsCircleAreaClicked(_testCircle, mousePosition))
		{
			CP_Settings_Stroke(LIGHT_RED);
		}
		else CP_Settings_NoStroke();
	}
	CP_Graphics_ClearBackground(LIGHT_GRAY);
}

void Main_Menu_Exit()
{

}