#include "utils.h"
#include "cprocessing.h"
#include "mainmenu.h"
#include "colortable.h"
#include "carlevel.h"
#include <stdio.h>

CP_POSITION_MODE menuRectPosition = CP_POSITION_CENTER;

//Shape testRect;
//Shape testCircle;
//Shape testEllipse;
//Shape testCircleEllipse;

Button rectButton;
Button circleButton;
Button ellipseButton;
Button circleEllipseButton;

char *tempText = "Car Select Game";
FILE* testFile;
char buffer2[64];
void Main_Menu_Init()
{
	CP_System_SetWindowSize(500, 500);
	CP_Settings_RectMode(menuRectPosition);


	rectButton = CreateButton(250, 100, 150, 50, SHAPE_RECTANGLE, "Rect Button", LIGHT_BLUE, StartGame);
	rectButton = CreateButton(250, 200, 40, 40, SHAPE_CIRCLE, "Circle Button", LIGHT_RED, StartGame);
	rectButton = CreateButton(250, 300, 80, 80, SHAPE_ELLIPSE, "circle ellipse Button", LIGHT_GREEN, StartGame);
	rectButton = CreateButton(250, 400, 150, 50, SHAPE_ELLIPSE, "Ellipse Button", LIGHT_YELLOW, RandomNoise);

}

void StartGame()
{
	CP_Engine_SetNextGameState(Car_Level_Init, Car_Level_Update, Car_Level_Exit);
}
void RandomNoise()
{
	Shake();
	//snprintf(buffer2, sizeof buffer2, "%f", PerlinNoise(CP_Random_RangeInt(1,10), 10));
	
}
void ExitGame()
{
	CP_Engine_Terminate();
}

void TestFunction()
{
	printf("\a");
	printf("test");
	errno_t err;
	err = fopen_s(&testFile, "./Assets/testfile.txt", "w");
	if (testFile)
	{
		fprintf(testFile, "12345");
		fclose(testFile);
	}
}
void ReadFileFunction()
{
	printf("test");
	errno_t err1;
	err1 = fopen_s(&testFile, "./Assets/testfile.txt", "r");
	// reading line by line, max 256 bytes
	
	if (testFile)
	{
		char buffer[6];
		if (fgets(buffer, 6, testFile))
		{
			tempText = buffer;
		}

		fclose(testFile);
	}

}


void Main_Menu_Update()
{



	UpdateGUI();
	
	CP_Font_DrawText(buffer2, 250, 50);
	CP_Graphics_ClearBackground(LIGHT_GRAY);
	CP_Settings_NoStroke();
}

void Main_Menu_Exit()
{
	//Free whatever memory for stuff that was loaded in mainmenu here
	CP_Graphics_ClearBackground(LIGHT_GRAY);
	CP_Settings_NoStroke();
}