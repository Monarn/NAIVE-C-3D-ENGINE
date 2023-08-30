#include "object.h"

typedef struct {
    int num;
    Object** lObjects;
}ObjList;

ObjList* createObjList(void)
{
	ObjList* ret = malloc(sizeof(ObjList));
	printf("POINTEUR LISTE OBJET : %p\n", ret);
	ret->num = 0;
	ret->lObjects = NULL;
	return ret;
}

void appendObjList(ObjList* objl, Object* obj)
{
	Object** newList = malloc((objl->num+1) * sizeof(Object*));

	if (newList == NULL)
	{
		fprintf(stderr, "ALLOCATION ECHOUEE\n");	
	}

	for (int i = 0; i < objl->num; ++i)
	{
		newList[i] = objl->lObjects[i];
	}

	newList[objl->num] = obj;

	free(objl->lObjects);

	objl->lObjects = newList;
	objl->num++;
}

void popObjList(ObjList* objl)
{
	
	Object** newList = malloc((objl->num-1)*sizeof(Object*));

	for (int i = 0; i  < objl->num-1; ++i)
	{
		newList[i] = objl->lObjects[i];
		free(objl->lObjects[i]);
	}

	free(objl->lObjects);

	objl->num--;
}

typedef struct {
    unsigned int FPS;
    unsigned int w, h;
    char* name;
	double pitch, yaw, roll;
    Color bgcol;
    ObjList* objl;
    double speed;
    double rotSpeed;
} Renderer;


void addObj(Renderer* app, Vec3 vertices[], unsigned int indices[] ,unsigned int vertNum, unsigned int indNum, Color color)
{
	unsigned int ID = app->objl->num;
	Object* newObj = malloc(sizeof(Object));
	newObj = constructObject(vertices, indices, vertNum, indNum, ID, app->w, app->h, color);
	appendObjList(app->objl, newObj);
}

Object* fetchObject(Renderer* app, unsigned int ID)
{
	for (int i = 0; i < app->objl->num; ++i)
	{
		if (app->objl->lObjects[i]->ID == ID)
		{
			return app->objl->lObjects[i]; 
		}
	}

	return NULL;
}

void draw(Renderer* app)
{
	for (int i = 0; i < app->objl->num; ++i)
	{
		pipeline(app->objl->lObjects[i], app->w, app->h);
	}
}

void look(Renderer* app, double pitch, double roll, double yaw)
{
	for (int i = 0; i < app->objl->num; ++i)
	{
		app->pitch += pitch;
		app->roll += roll;
		app->yaw += yaw;
		rotate(app->objl->lObjects[i], pitch, roll, yaw, false);
	}
}

void move(Renderer* app, double x, char* axis) {
    for (int i = 0; i < app->objl->num; ++i) {
        if (strchr(axis, 'x') != NULL) {
            translate(app->objl->lObjects[i], -1 * x * app->speed, "x");
        }

        if (strchr(axis, 'y') != NULL) {
            translate(app->objl->lObjects[i], -1 * x * app->speed, "y");
        }

        if (strchr(axis, 'z') != NULL) {
            translate(app->objl->lObjects[i], -1 * x * app->speed, "z");
        }
    }
}

void controls(Renderer* app)
{
	if (IsKeyDown(KEY_W))
	{
		move(app, -1*app->speed, "z");
	}
	
	if (IsKeyDown(KEY_S))
	{
		move(app, app->speed, "z");
	}

	if (IsKeyDown(KEY_A))
	{
		move(app, app->speed, "x");
	}

	if (IsKeyDown(KEY_D))
	{
		move(app, -1*app->speed, "x");
	}

	if (IsKeyDown(KEY_Q))
	{
		move(app, -1*app->speed, "y");
	}

	if (IsKeyDown(KEY_E))
	{
		move(app, app->speed, "y");
	}

	if (IsKeyDown(KEY_LEFT_SHIFT))
	{
		app->speed = 0.5;
	} else if (IsKeyUp(KEY_LEFT_SHIFT)) {
		app->speed = 0.1;
	}

	if (IsKeyDown(KEY_UP))
	{
		look(app, 0, -1*app->rotSpeed, 0);
	}
	
	if (IsKeyDown(KEY_DOWN))
	{
		look(app, 0, app->rotSpeed, 0);
	}

	if (IsKeyDown(KEY_LEFT))
	{
		look(app, app->rotSpeed, 0,0);
	}
	
	if (IsKeyDown(KEY_RIGHT))
	{
		look(app, -1*app->rotSpeed, 0, 0);
	}

	int mouseX = GetMouseX() - app->w/2;
	int mouseY = GetMouseY() - app->h/2;

	HideCursor();

	look(app, -app->rotSpeed*mouseX,app->rotSpeed*mouseY , 0);

	SetMousePosition(app->w/2,app->h/2);

}

void run(Renderer* app);

void init(Renderer* app);

Renderer* constructRenderer(unsigned int FPS, unsigned int w, unsigned int h, char* name, Color bgcol, double speed, double rotSpeed)
{

	Renderer* ret = malloc(sizeof(Renderer));
	ret->FPS = FPS;
	ret->w = w; ret->h = h;
	ret->objl = createObjList();
	printf("POINTEUR LISTE OBJET : %p\n", ret->objl);
	ret->name = name;
	ret->pitch = 0; ret->yaw = 0; ret->roll = 0;
	ret->bgcol = bgcol;
	ret->speed = speed;
	ret->rotSpeed = rotSpeed;

	return ret;
}

void start(Renderer* app)
{
	InitWindow(app->w, app->h, app->name);

	init(app);

	SetTargetFPS(app->FPS);

	while(!WindowShouldClose())
	{
		app->w = GetScreenWidth();
		app->h = GetScreenHeight();
		BeginDrawing();
			ClearBackground(app->bgcol);
			run(app);
		EndDrawing();
	}

	CloseWindow();
}

