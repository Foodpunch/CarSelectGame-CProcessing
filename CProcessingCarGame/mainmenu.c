#include "utils.h"
#include "cprocessing.h"
#include "mainmenu.h"
#include "colortable.h"
#include "carlevel.h"
#include <stdio.h>

CP_POSITION_MODE menuRectPosition = CP_POSITION_CENTER;

Button StartGameButton;
Button ExitGameButton;
Button TestButton;

CircleArea _testCircle;

void Main_Menu_Init()
{
	CP_System_SetWindowSize(500, 500);
	CP_Settings_RectMode(menuRectPosition);

	StartGameButton = CreateButton(250, 150, 150, 50, "Start Game", LIGHT_BLUE,StartGame);
	ExitGameButton = CreateButton(250, 250, 150, 50, "Exit Game", LIGHT_BLUE,ExitGame);
	TestButton = CreateButton(250, 350, 150, 50, "Test Button", PASTEL_GREEN2, TestFunction);

}

void StartGame()
{
	CP_Engine_SetNextGameState(Car_Level_Init, Car_Level_Update, Car_Level_Exit);
}

void ExitGame()
{
	CP_Engine_Terminate();
}

void TestFunction()
{
	printf("\a");
}


void Main_Menu_Update()
{

	UpdateButton(StartGameButton, menuRectPosition);
	UpdateButton(ExitGameButton, menuRectPosition);
	UpdateButton(TestButton, menuRectPosition);

	CP_Graphics_ClearBackground(LIGHT_GRAY);
	CP_Settings_NoStroke();
}

void Main_Menu_Exit()
{
	//Free whatever memory for stuff that was loaded in mainmenu here
	CP_Graphics_ClearBackground(LIGHT_GRAY);
	CP_Settings_NoStroke();
}