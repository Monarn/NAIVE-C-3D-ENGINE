/*
=============================================================================================================================================
3D ENGINE IN C. WRAPPER FOR RAYLIB 2D LIBRARY (BECAUSE WHY NOT). NO MATRICES NOR CAMERA (ONLY PSEUDO-CAMERA).
PROJECTION ON CPU AND RENDER ON GPU. NO ADDITIONAL SHADER.

THIS 3D ENGINE WAS MADE ONLY FOR EDUCATIONAL PURPOSES. DO NOT USE IT FOR YOUR PROJECTS.
THIS IS NOT FINISHED. THERE ARE PROBLEMS SUCH AS MEMORY LEAK OR UNINTEDED CAMERA ROLL. PLEASE CONSIDER THIS.

THIS 3D ENGINE HAS NO Z-BUFFER (I WANTED TO USE THE LEAST OF RAYLIB I COULD). THIS ENGINE SHOWS ONLY THE EDGES OF THE OBJECTS YOU WANT TO 
DRAW. YOU CANT IMPORT MESH NEITHER.

THIS IS A NAIVE WAY TO DO A 3D ENGINE. COMMERCIALS ONES USE MORE COMPLEX MATHS AND NOTIONS.
=============================================================================================================================================
*/

#ifndef RENDERER_H_
#define RENDERER_H_

#define MAPI

#include <raylib.h>
#include "object.h"

// Structure to handle multi-objects processing
typedef struct
{
	int num;
	Object** lObjects;
}ObjList;

// Structure for the renderer
typedef struct {
    unsigned int FPS;                       // The number of frame the renderer will draw each second
    unsigned int w, h;                      // The width and height of the window the renderer will create
    char* name;                             // The name of the window the renderer will create
	double pitch, yaw, roll;                // The rotation of the pseudo-camera (non-used)
    Color bgcol;                            // Background color of the renderer
    ObjList* objl;                          // A pointer to the list of objects
    double speed;                           // The movement speed of the camera
    double rotSpeed;                        // The rotation speed of the camera
} Renderer;

// ============================================= FUNCTIONS TO HANDLE OBJLIST ================================================================

// Returns a pointer to an object list. This list is initialized with a size of 0 (basically returns NULL)
MAPI ObjList* createObjList();

// Adds an object to the object list. The size is incremented by one.
MAPI void appendObjList(ObjList* objl, Object* obj);

// Pops the last object of the list. The size is decremented by one.
MAPI void popObjList(ObjList* objl);

// Adds an object to the renderer so it can manipulate it
MAPI void addObj(Renderer* app, Vec3* vertices, unsigned int* indices ,unsigned int vertNum, unsigned int indNum, Color color);

// Returns a pointer to an object based on his ID (Returns NULL if none)
MAPI Object* fetchObject(Renderer* app, unsigned int ID);

// ============================================= FUNCTIONS TO HANDLE RENDERER ===============================================================

// Constructor for the renderer (generates the ObjList).
MAPI Renderer* constructRenderer(unsigned int FPS, unsigned int w, unsigned int h, char* name, Color bgcol, double speed, double rotSpeed);

//                  **************************** FUNCTIONS FOR RENDERER LOGIC ******************************************

// Draws every object to the screen
MAPI void draw(Renderer* app);

// Empty function. You have to modify it in your main file in order to script the renderer (this function is called every frame)
MAPI void run(Renderer* app);

// Empty function. You have to modify it in your main file in order to script the renderer (the function is called once at the begining)
MAPI void init(Renderer* app);

// This function starts the renderer. Call it once you have written run and init
MAPI void start(Renderer* app);

//              **************************** FUNCTIONS FOR PSEUDO-CAMERA MANIPULATIONS **************************************

// Manipulate the rotation of the pseudo-camera
MAPI void look(Renderer* app, double pitch, double yaw, double roll);

// This function moves the camera of x units based on the axis you want to move
// The axis is a string. if 'x' in axis, the camera will be translated left or right
MAPI void move(Renderer* app, double x, char* axis);

// Function that handles user inputs. you have to call it in your run function if you want to handle user input
MAPI void controls(Renderer* app);

#endif
