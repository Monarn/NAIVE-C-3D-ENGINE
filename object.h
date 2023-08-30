/*
=============================================================================================================================================
REPRESENTATION AND OPERATIONS OF AN OBJECT. USED IN RENDERER.H TO HANDLE 3D GRAPHICS.
ALSO VERY NAIVE. IN THEORY, YOU DONT REALLY NEED TO USE THESE FUNCTIONS, EXCEPT FOR THE MACROS.
=============================================================================================================================================
*/

#ifndef OBJECT_H_
#define OBJECT_H_

#define MAPI

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include <stdio.h>
#include <stdbool.h>


#define lenV(arr) sizeof(arr) / sizeof(Vec3)
#define lenI(arr) sizeof(arr) / sizeof(int)

// Structure for a 3D vertice (or vector)
typedef struct{
	double x,y,z;
}Vec3;

// Structure for an array of 3d-vectors
typedef struct
{
	unsigned int size;
	Vec3* array;
}Vec3Array;

// Structure for an array of uint.
typedef struct
{
	unsigned int size;
	unsigned int* array;
}intArray;

// Structure for an Object representation.
typedef struct
{
	Vec3Array* vertices; 				// Pointer to an array of 3D vertices.
	Vec3Array* projected;				// Pointer to an array of 3D projected-to-screen coordinates with the 3rd coordinate the depth 
	intArray* indices;					// Pointer to an array of uint. Used to trace edges of the object in the right order
	int w,h;							// Dimension of the window the object is traced in
	Color color;						// Color of the object (Unique color, no textures)
	unsigned int ID;					// ID of the object (Perhaps useful, I guess)
	Vec3 mean;							// The center point of the object (used to make right rotations and translations)
}Object;

// ============================================ FUNCTIONS FOR VEC3ARRAY STRUCT ===============================================================

// Returns a pointer to a Vec3Array (Returns NULL because the size is initialized to 0)
MAPI Vec3Array* constructVec3Array(void);

// Reallocates the array of vertices with one more space (But it does not add any vertice)
MAPI void incSizeVec3Array(Vec3Array* ret);

//Reallocates the array of vertices with one more space (Adds one vertice)
MAPI void appendVec3Array(Vec3Array* ret1, Vec3 vec);

// Fills ret1 with all the Vec3 in vecs. Adds space for ret2 but does not fill anything
MAPI void fillVec3Array(Vec3Array* ret1, Vec3Array* ret2, Vec3 vecs[], unsigned int size);

// ============================================ FUNCTIONS FOR INTARRAY STRUCT ===============================================================

// Returns a pointer to an intArray (Returns NULL because the size of the array is 0)
MAPI intArray* constructIntArray(void);

// Adds an element in the intArray. Realloc the size and increments the size by one
MAPI void appendIntArray(intArray* ret, int x);

// Fills ret with ind
MAPI void fillIntArray(intArray* ret, unsigned int ind[], unsigned int size);

// =========================================== FUNCTIONS FOR VEC3 STRUCTURE ================================================================

// Returns a Vec3 made up of x,y and z coordinates
MAPI Vec3 cVec3(double x, double y, double z);

// =========================================== FUNCTIONS FOR OBJECT STRUCT ===============================================================

// Constructor for object structure. Returns a pointer to this object
MAPI Object* constructObject(Vec3 initVertices[], unsigned int indices[], unsigned int vertNum, unsigned int indNum, unsigned int ID, int w, int h, Color color);

//							****************** MEMORY MANAGEMENT FUNCTIONS ************************

// Allocates vertices, projected and indices in the object structure
MAPI void allocatePoints(Object* obj);

// Fills the object's vertices and indices with the second and third args
MAPI void fillVariables(Object* obj, Vec3* initVertices[], unsigned int* indices[]);

// Frees the allocated memory of the obj
MAPI void freePoints(Object* obj);

//						   ******************* TRANSFORMATION FUNCTIONS *****************************

// Computes the mid-point of the object, and stores it to obj->mean
MAPI void computeMean(Object* obj); 

// Translates the object's vertices by x units in the axis we want 
// EX : we can put "xy", "yz", "zy" or "xyz", and the displacement will be made considering these axis.
// /!\ "foo" will not throw an error, but nothing will be done
MAPI void translate(Object* obj, double x, char* axis);

// Rotates the object with euler angles. Self argument describes if the rotation is done considering the center of the
// object, or the origin of the space
MAPI void rotate(Object* obj, double pitch, double roll, double yaw, bool self);

// Scales the object by a factor x
MAPI void scale(Object* obj, double x);

//	Projects the vertices of the object and stores it in obj->projected
MAPI void projection(Object* obj);

// Draws the object's edges in a window of size w, h
MAPI void render(Object* obj, int w, int h);

// Encapsulates projection and render
MAPI void pipeline(Object* obj, int w, int h);

//						   *********************** MISC FUNCTIONS *******************************

// Prints all the vertices of the object. Used for debbuging purposes.
MAPI void displayVertices(Object* obj);

#endif
