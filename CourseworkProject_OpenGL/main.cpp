

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// CT4TOGA OpenGL without using GLUT - uses excerpts from here:
// http://bobobobo.wordpress.com/2008/02/11/opengl-in-a-proper-windows-app-no-glut/
// Feel free to adapt this for what you need, but please leave these comments in.

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


#pragma once

#include <windows.h>	// need this file if you want to create windows etc
#include <gl/gl.h>		// need this file to do graphics with opengl
#include <gl/glu.h>		// need this file to set up a perspective projection easily
#include <time.h>		// randomization 
#include <vector>
#include <stack>
#include "ClassManager.h"
#define NUMBER_OF_SLOTS 3
#include <iostream>
using namespace std;

// include the opengl and glu libraries
#pragma comment(lib, "opengl32.lib")	
#pragma comment(lib, "glu32.lib")

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


// function prototypes:
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow);


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


// In a C++ Windows app, the starting point is WinMain() rather than _tmain() or main().
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	// some basic numbers to hold the position and size of the window
	int windowWidth = 800;
	int windowHeight = 600;
	int windowTopLeftX = 50;
	int windowTopLeftY = 50;

	// some other variables we need for our game...
	MSG msg;								// this will be used to store messages from the operating system
	bool keepPlaying = true;				// whether or not we want to keep playing



											// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
											// this section contains all the window initialisation code, 
											// and should probably be collapsed for the time being to avoid confusion	
#pragma region  <-- click the plus/minus sign to collapse/expand!

											// this bit creates a window class, basically a template for the window we will make later, so we can do more windows the same.
	WNDCLASS myWindowClass;
	myWindowClass.cbClsExtra = 0;											// no idea
	myWindowClass.cbWndExtra = 0;											// no idea
	myWindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// background fill black
	myWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);					// arrow cursor       
	myWindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// type of icon to use (default app icon)
	myWindowClass.hInstance = hInstance;									// window instance name (given by the OS when the window is created)   
	myWindowClass.lpfnWndProc = WndProc;									// window callback function - this is our function below that is called whenever something happens
	myWindowClass.lpszClassName = TEXT("my window class name");				// our new window class name
	myWindowClass.lpszMenuName = 0;											// window menu name (0 = default menu?) 
	myWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;				// redraw if the window is resized horizontally or vertically, allow different context for each window instance

																			// Register that class with the Windows OS..
	RegisterClass(&myWindowClass);

	// create a rect structure to hold the dimensions of our window
	RECT rect;
	SetRect(&rect, windowTopLeftX,				// the top-left corner x-coordinate
		windowTopLeftY,				// the top-left corner y-coordinate
		windowTopLeftX + windowWidth,		// far right
		windowTopLeftY + windowHeight);	// far left

										// adjust the window, no idea why.
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// call CreateWindow to create the window
	HWND myWindow = CreateWindow(TEXT("my window class name"),		// window class to use - in this case the one we created a minute ago
		TEXT("Slot Machine - CT4TOGA Coursework. UP843814"),		// window title
		WS_OVERLAPPEDWINDOW,						// ??
		windowTopLeftX, windowTopLeftY,			// x, y
		windowWidth, windowHeight,				// width and height
		NULL, NULL,								// ??
		hInstance, NULL);							// ??


													// check to see that the window created okay
	if (myWindow == NULL)
	{
		FatalAppExit(NULL, TEXT("CreateWindow() failed!")); // ch16
	}

	// if so, show it
	ShowWindow(myWindow, iCmdShow);


	// get a device context from the window
	HDC myDeviceContext = GetDC(myWindow);


	// we create a pixel format descriptor, to describe our desired pixel format. 
	// we set all of the fields to 0 before we do anything else
	// this is because PIXELFORMATDESCRIPTOR has loads of fields that we won't use
	PIXELFORMATDESCRIPTOR myPfd = { 0 };


	// now set only the fields of the pfd we care about:
	myPfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);		// size of the pfd in memory
	myPfd.nVersion = 1;									// always 1

	myPfd.dwFlags = PFD_SUPPORT_OPENGL |				// OpenGL support - not DirectDraw
		PFD_DOUBLEBUFFER |				// double buffering support
		PFD_DRAW_TO_WINDOW;					// draw to the app window, not to a bitmap image

	myPfd.iPixelType = PFD_TYPE_RGBA;					// red, green, blue, alpha for each pixel
	myPfd.cColorBits = 24;								// 24 bit == 8 bits for red, 8 for green, 8 for blue.
														// This count of color bits EXCLUDES alpha.

	myPfd.cDepthBits = 32;								// 32 bits to measure pixel depth.


														// now we need to choose the closest pixel format to the one we wanted...	
	int chosenPixelFormat = ChoosePixelFormat(myDeviceContext, &myPfd);

	// if windows didnt have a suitable format, 0 would have been returned...
	if (chosenPixelFormat == 0)
	{
		FatalAppExit(NULL, TEXT("ChoosePixelFormat() failed!"));
	}

	// if we get this far it means we've got a valid pixel format
	// so now we need to set the device context up with that format...
	int result = SetPixelFormat(myDeviceContext, chosenPixelFormat, &myPfd);

	// if it failed...
	if (result == NULL)
	{
		FatalAppExit(NULL, TEXT("SetPixelFormat() failed!"));
	}

	// we now need to set up a render context (for opengl) that is compatible with the device context (from windows)...
	HGLRC myRenderContext = wglCreateContext(myDeviceContext);

	// then we connect the two together
	wglMakeCurrent(myDeviceContext, myRenderContext);



	// opengl display setup
	glMatrixMode(GL_PROJECTION);	// select the projection matrix, i.e. the one that controls the "camera"
	glLoadIdentity();				// reset it
	gluPerspective(45.0, (float)windowWidth / (float)windowHeight, 0.1, 1000);	// set up fov, and near / far clipping planes
	glViewport(0, 0, windowWidth, windowHeight);								// make the viewport cover the whole window
	glClearColor(0.5, 0, 0, 1.0);												// set the colour used for clearing the screen
	glEnableClientState(GL_VERTEX_ARRAY);										//	turn on the ability to do vertex arrays
	glMatrixMode(GL_MODELVIEW);													// select the modelview matrix, i.e. the one that controls how local space is mapped to world space


																				// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#pragma endregion



	//float matrix1[16];
	//float matrix2[16];
	//float matrix3[16];

	slotMachine machine;
	bool isSpinning = false;

	float *ptrVertex = new float;

	// main game loop starts here!



	// keep doing this as long as the player hasnt exited the app: 
	while (keepPlaying == true)
	{
		

		// we need to listen out for OS messages.
		// if there is a windows message to process...
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// and if the message is a "quit" message...
			if (msg.message == WM_QUIT)
			{
				keepPlaying = false;	// we want to quit asap
			}

			if (msg.wParam == VK_ESCAPE)
			{
				keepPlaying = false; //close the window when user presses the 'ESC' key
			}

			else if (msg.wParam == VK_RETURN)
			{
				int credits = 40;
				//credits =- 10;

				if (credits > 0)
				{
					
				// @@@ Game Loop to go here @@@
				for (int i = 0; i < NUMBER_OF_SLOTS; i++)
				{
					credits -= 10;
					machine.slots.erase(machine.slots.begin() + i);

					int r = rand() % 3;

					switch (r)
						{
						case 0:
							machine.slots.insert(machine.slots.begin() + i, new triangle());
							break;

						case 1:
							machine.slots.insert(machine.slots.begin() + i, new square());
							break;

						case 2:
							machine.slots.insert(machine.slots.begin() + i, new diamond());
							break;
						}
					}
				if (credits <= 0)
				{
					keepPlaying = false;
				}
				}

			}
			// or if it was any other type of message (i.e. one we don't care about), process it as normal...
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// clear screen

		machine.moveReels();
		machine.draw();

			SwapBuffers(myDeviceContext);							// update graphics

	}


	// the next bit will therefore happen when the player quits the app,
	// because they are trapped in the previous section as long as (keepPlaying == true).
	ptrVertex = NULL;
	delete ptrVertex;

	// UNmake our rendering context (make it 'uncurrent')
	wglMakeCurrent(NULL, NULL);

	// delete the rendering context, we no longer need it.
	wglDeleteContext(myRenderContext);

	// release our window's DC from the window
	ReleaseDC(myWindow, myDeviceContext);

	// end the program
	return msg.wParam;
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// this part contains some code that should be collapsed for now too...
#pragma region keep_this_bit_collapsed_too!

// this function is called when any events happen to our window
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{

	switch (message)
	{
		// if they exited the window...	
	case WM_DESTROY:
		// post a message "quit" message to the main windows loop
		PostQuitMessage(0);
		return 0;
		break;
	}

	// must do this as a default case (i.e. if no other event was handled)...
	return DefWindowProc(hwnd, message, wparam, lparam);

}

#pragma endregion
