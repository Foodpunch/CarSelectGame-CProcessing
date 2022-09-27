#include "utils.h"
#include "cprocessing.h"
#include "mainmenu.h"
#include "carlevel.h"
#include <stdio.h>

CP_POSITION_MODE menuRectPosition = CP_POSITION_CENTER;

Button rectButton;				//Starts game
Button circleButton;			//Not used
Button ellipseButton;			//Shakes camera
Button circleEllipseButton;		//exits game


char *tempText = "Bumper Car Select Game";

void Main_Menu_Init()
{
	CP_System_SetWindowSize(500, 500);
	CP_Settings_RectMode(menuRectPosition);
	rectButton = CreateButton(250, 200, 150, 50, SHAPE_RECTANGLE, "Start Game", LIGHT_BLUE, StartGame);
	//circleButton = CreateButton(250, 200, 40, 40, SHAPE_CIRCLE, "Circle Button", LIGHT_RED, StartGame);
	circleEllipseButton = CreateButton(250, 300, 80, 80, SHAPE_ELLIPSE, "Shake Screen", LIGHT_YELLOW, ShakeScreen);
	ellipseButton = CreateButton(250, 400, 150, 50, SHAPE_ELLIPSE, "Exit Game", LIGHT_RED, ExitGame);
}

void StartGame()
{
	CP_Engine_SetNextGameState(Car_Level_Init, Car_Level_Update, Car_Level_Exit);
}
void ShakeScreen()
{
	trauma += 0.2f;
}
void ExitGame()
{
	CP_Engine_Terminate();
}

void Main_Menu_Update()
{
	UpdateCameraShaker();
	UpdateGUI();

	CP_Settings_TextSize(50);
	CP_Font_DrawText(tempText, 250, 50);
	CP_Settings_TextSize(15);

	CP_Graphics_ClearBackground(LIGHT_GRAY);
	CP_Settings_NoStroke();
}

void Main_Menu_Exit()
{
	//Free whatever memory for stuff that was loaded in mainmenu here
	ClearButtonArray();		
	CP_Graphics_ClearBackground(LIGHT_GRAY);
	CP_Settings_NoStroke();
}