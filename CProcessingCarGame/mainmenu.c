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
Button ReadFileButton;

Shape testRect;
Shape testCircle;

char *tempText = "Car Select Game";
FILE* testFile;
void Main_Menu_Init()
{
	CP_System_SetWindowSize(500, 500);
	CP_Settings_RectMode(menuRectPosition);

	StartGameButton = CreateButton(250, 150, 150, 50, "Start Game", LIGHT_BLUE,StartGame);
	ExitGameButton = CreateButton(250, 250, 150, 50, "Exit Game", LIGHT_RED ,ExitGame);
	TestButton = CreateButton(250, 350, 150, 50, "Test Button", PASTEL_GREEN2, TestFunction);
	ReadFileButton = CreateButton(250, 450,150, 50, "Read text file", PASTEL_PINK, ReadFileFunction);

	testRect = CreateShape(100, 100, 100, 100, 0, SHAPE_RECTANGLE);
	testCircle = CreateShape(100, 100, 15, 15, 0, SHAPE_CIRCLE);
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
	
		
	// close the file
	

}


void Main_Menu_Update()
{

	UpdateButton(StartGameButton, menuRectPosition);
	UpdateButton(ExitGameButton, menuRectPosition);
	//UpdateButton(TestButton, menuRectPosition);
	//UpdateButton(ReadFileButton, menuRectPosition);

	CP_Settings_Fill(GREEN);
	DrawShape(testRect);
	CP_Settings_Fill(RED);
	DrawShape(testCircle);


	CP_Font_DrawText(tempText, 250, 50);

	CP_Graphics_ClearBackground(LIGHT_GRAY);
	CP_Settings_NoStroke();
}

void Main_Menu_Exit()
{
	//Free whatever memory for stuff that was loaded in mainmenu here
	CP_Graphics_ClearBackground(LIGHT_GRAY);
	CP_Settings_NoStroke();
}