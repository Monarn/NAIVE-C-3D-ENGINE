#include "renderer.h"
#include <stdlib.h>
#include <time.h>

Vec3* deepCopy(Vec3 original[], int size) {
    Vec3* new_list = malloc(size * sizeof(Vec3));
    if (new_list == NULL) {
        fprintf(stderr, "Allocation error\n");
        exit(1);
    }

    for (int i = 0; i < size; i++) {
        new_list[i] = original[i];
    }

    return new_list;
}

float i = 0;

void run(Renderer* app)
{
	draw(app);
	controls(app);
	printf("FPS : %d\n", GetFPS());
	i++;
}

void init(Renderer* app)
{
	
	Vec3 vertices[] = {
	    {0.0f, 0.0f, 0.0f}, // Sommet 0
	    {1.0f, 0.0f, 0.0f}, // Sommet 1
	    {1.0f, 1.0f, 0.0f}, // Sommet 2
	    {0.0f, 1.0f, 0.0f}, // Sommet 3
	    {0.0f, 0.0f, 1.0f}, // Sommet 4
	    {1.0f, 0.0f, 1.0f}, // Sommet 5
	    {1.0f, 1.0f, 1.0f}, // Sommet 6
	    {0.0f, 1.0f, 1.0f}  // Sommet 7
	};

	unsigned int indices[] = {
	    0, 1, // Arête avant bas
	    1, 2, // Arête avant droite
	    2, 3, // Arête avant haut
	    3, 0, // Arête avant gauche
	    4, 5, // Arête arrière bas
	    5, 6, // Arête arrière droite
	    6, 7, // Arête arrière haut
	    7, 4, // Arête arrière gauche
	    0, 4, // Arête gauche bas
	    1, 5, // Arête droite bas
	    2, 6, // Arête droite haut
	    3, 7  // Arête gauche haut
	};

	SetMousePosition(app->w/2, app->h/2);
	
	Color objCol = {255,0,0,255};

	addObj(app, vertices, indices, lenV(vertices), lenI(indices), objCol);

	for (int i = 0; i < 9000; ++i)
	{
		Vec3* newVert = deepCopy(vertices, lenV(vertices));
		addObj(app, (Vec3*)newVert, indices, lenV(vertices), lenI(indices), objCol);
	}


	for (int i = 0; i < app->objl->num; ++i)
	{
		translate(app->objl->lObjects[i], 7*cos(i)*5*sin(i), "z");
		translate(app->objl->lObjects[i], 13*cos(i) - 5*cos(2*i) - cos(4*i), "y");
		translate(app->objl->lObjects[i], 16 * pow(sin(i), 3), "x");
	}

}

int main()
{
	int w = 800; int h = 800;
	Color rendcol = {0,0,0,255};

	Renderer* app =  constructRenderer(60, w, h, "Moteur 3D", rendcol, 0.1, 0.01);

	start(app);

	return 0;
}
