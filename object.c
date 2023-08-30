#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct 
{
	double x,y,z;
}Vec3;


#define lenV(arr) sizeof(arr) / sizeof(Vec3)
#define lenI(arr) sizeof(arr) / sizeof(int)

typedef struct
{
	unsigned int size;
	Vec3* array;
}Vec3Array;

Vec3Array* constructVec3Array(void)
{
	Vec3Array* ret = malloc(sizeof(Vec3Array));
	ret->size = 0;
	ret->array = NULL;
	return ret;
}

void incSizeVec3Array(Vec3Array* ret)
{
	ret->size++;
	ret->array = realloc(ret->array, sizeof(Vec3)*ret->size);
	if (ret->array == NULL)
		{
			fprintf(stderr, "REALLOCATION VEC3ARRAY ECHOUEE\n");
			exit(1);
		}
}

void appendVec3Array(Vec3Array* ret, Vec3 vec)
{
	ret->size++;
	ret->array = realloc(ret->array, sizeof(Vec3)*ret->size);
	if (ret->array == NULL)
	{
		fprintf(stderr, "REALLOCATION VEC3ARRAY ECHOUEE\n");
		exit(1);
	}

	ret->array[ret->size-1] = vec;
}

void fillVec3Array(Vec3Array* ret1, Vec3Array* ret2, Vec3 vecs[], unsigned int size)
{
	for (int i = 0; i < size; ++i)
	{
		appendVec3Array(ret1, vecs[i]);
		incSizeVec3Array(ret2);
	}
}

typedef struct
{
	unsigned int size;
	unsigned int* array;
}intArray;

intArray* constructIntArray(void)
{
	intArray* ret = malloc(sizeof(int));
	ret->size = 0;
	ret->array = NULL;
	return ret;
}

void appendIntArray(intArray* ret, int x)
{
	ret->size++;
	ret->array = realloc(ret->array, sizeof(int)*ret->size);
	if (ret->array == NULL)
	{
		fprintf(stderr, "REALLOCATION INTARRAY ECHOUEE\n");
		exit(1);
	}

	ret->array[ret->size-1] = x;
}

void fillIntArray(intArray* ret, unsigned int ind[], unsigned int size)
{
	for (int i = 0; i < size; ++i)
	{
		appendIntArray(ret, ind[i]);
	}
}

typedef struct
{
	Vec3Array* vertices;
	Vec3Array* projected;
	intArray* indices;
	int w,h;
	Color color;
	unsigned int ID;
	Vec3 mean;
}Object;

void allocatePoints(Object* obj)
{
	obj->vertices = constructVec3Array();
	obj->projected = constructVec3Array();
	obj->indices = constructIntArray();
}

void fillVariables(Object* obj, Vec3 initVertices[], unsigned int indices[], int vertNum, int indNum)
{
	fillVec3Array(obj->vertices, obj->projected, initVertices, vertNum);
	fillIntArray(obj->indices, indices, indNum);
}


void freePoints(Object* obj)
{
	free(obj->projected->array);
	free(obj->vertices->array);
}

void computeMean(Object* obj)
{
	Vec3 mean = {.x = 0.0, .y = 0.0, .z = 0.0};
	for (int i = 0; i < obj->vertices->size; ++i)
	{
		mean.x += obj->vertices->array[i].x;
		mean.y += obj->vertices->array[i].y;
		mean.z += obj->vertices->array[i].z;
	}

	mean.x /= obj->vertices->size;
	mean.y /= obj->vertices->size;
	mean.z /= obj->vertices->size;

	obj->mean = mean;
	
}

void translate(Object* obj, double x, char* axis)
{
	for (int i = 0; i < obj->vertices->size; ++i)
	{
		if (strchr(axis, 'x') != NULL)
		{
			obj->vertices->array[i].x += x;
		}

		if (strchr(axis, 'y') != NULL)
		{
			obj->vertices->array[i].y += x;
		}
		
		if (strchr(axis, 'z') != NULL)
		{
			obj->vertices->array[i].z += x;
		}
	}
}

void render(Object* obj, int w, int h)
{
	if (obj->indices->size > 1)
	{
		computeMean(obj);
		for (int i = 0; i < obj->indices->size - 1; ++i)
		{
			Vec3 p1 = obj->projected->array[obj->indices->array[i]];
			Vec3 p2 = obj->projected->array[obj->indices->array[i+1]];
			
			if ((-.01 > p1.z && p1.z > -300) && (-.01 > p1.z  && p2.z> -300))
			{
				int x1 = round(p1.x) + round(w/2);
				int x2 = round(p2.x) + round(w/2);
				int y1 = round(p1.y) + round(h/2);
				int y2 = round(p2.y) + round(h/2);

				DrawLine(x1,y1,x2,y2, obj->color);
			}
		}
	}
}

void scale(Object* obj, double x)
{
	for (int i = 0; i < obj->vertices->size; ++i)
	{
		obj->vertices->array[i].x *= x;
		obj->vertices->array[i].y *= x;
		obj->vertices->array[i].z *= x;
	}
}


void rotate(Object* obj, double pitch, double roll, double yaw, bool self)
{
    computeMean(obj);

    if (self)
    {
        translate(obj, -1 * obj->mean.x, "x");
        translate(obj, -1 * obj->mean.y, "y");
        translate(obj, -1 * obj->mean.z, "z");
    }

    double cosa = cos(yaw);
    double sina = sin(yaw);
    double cosb = cos(pitch);
    double sinb = sin(pitch);
    double cosc = cos(roll);
    double sinc = sin(roll);

    double Axx = cosa * cosb;
    double Axy = cosa * sinb * sinc - sina * cosc;
    double Axz = cosa * sinb * cosc + sina * sinc;

    double Ayx = sina * cosb;
    double Ayy = sina * sinb * sinc + cosa * cosc;
    double Ayz = sina * sinb * cosc - cosa * sinc;

    double Azx = -sinb;
    double Azy = cosb * sinc;
    double Azz = cosb * cosc;

    for (int i = 0; i < obj->vertices->size; ++i)
    {
        double px = obj->vertices->array[i].x;
        double py = obj->vertices->array[i].y;
        double pz = obj->vertices->array[i].z;

        double tmppx = Axx * px + Axy * py + Axz * pz;
        double tmppy = Ayx * px + Ayy * py + Ayz * pz;
        double tmppz = Azx * px + Azy * py + Azz * pz;

        obj->vertices->array[i].x = tmppx;
        obj->vertices->array[i].y = tmppy;
        obj->vertices->array[i].z = tmppz;
    }

    if (self)
    {
        translate(obj, obj->mean.x, "x");
        translate(obj, obj->mean.y, "y");
        translate(obj, obj->mean.z, "z");
    }
}

Object* constructObject(Vec3 initVertices[], unsigned int indices[], unsigned int vertNum, unsigned int indNum,
unsigned int ID, int w, int h, Color color)
{	
	Object* obj = malloc(sizeof(Object));

	obj->vertices = constructVec3Array();
	obj->projected = constructVec3Array();
	obj->indices = constructIntArray();

	obj->vertices->size = vertNum;
	obj->indices->size = indNum;

	obj->color = color;

	obj->w = w; obj->h = h;

	allocatePoints(obj);
	fillVariables(obj, initVertices, indices, vertNum, indNum);	
	computeMean(obj);


	translate(obj, -1*obj->mean.x, "x");
	translate(obj, -1*obj->mean.y, "y");
	translate(obj, -1*obj->mean.z, "z");


	return obj;
}

void projection(Object* obj)
{
	for (int i = 0; i < obj->vertices->size; ++i)
	{
		double z = obj->vertices->array[i].z;
		if (z < 0)
		{
			obj->projected->array[i].x = (obj->vertices->array[i].x*100-50)/z*5;
			obj->projected->array[i].y = (obj->vertices->array[i].y*100-50)/z*5;
			obj->projected->array[i].z = z;

		} else {
			obj->projected->array[i].x = -10000;
			obj->projected->array[i].y = -10000;
			obj->projected->array[i].z = -10000;
		}
	}
}

void pipeline(Object* obj, int w, int h)
{
	projection(obj);
	render(obj, w, h);
}

void displayVertices(Object* obj)
{	
	for (int i = 0; i < obj->vertices->size; ++i)
	{
		printf("[x:%f, y:%f, z:%f]\n", obj->vertices->array[i].x,obj->vertices->array[i].y,obj->projected->array[i].z);
	}
}
