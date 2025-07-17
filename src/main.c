#include "raylib.h"
#include "rcamera.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CUBE 100
#define SPHERE 200
#define CYLINDER 300
#define CAPSULE 400
#define CUSTOM 500


struct GameObject
{
    char *name;
    Transform transform;
    int MeshType;

    Color color;
    
    // Cube properties
    float length;
    float width;
    float height;
    // Sphere properties
    float radius;
    // Cylinder properties
    float radiusTop;
    float radiusBottom;
    int slices;
    // Model properties
    Model model;
};




struct Button
{
    char *name;
    Rectangle rect;
    Color color;
    char *text;
    Vector2 textPos;
};





struct GameObject* CreateObject()
{
    printf("Creating Object\n");

    struct GameObject* obj = malloc(sizeof(struct GameObject));

    if (obj == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);  // Or handle the error more gracefully
    }

    obj->name = "New Object";
    obj->transform.translation = (Vector3){0, 0, 0};
    obj->transform.scale = (Vector3){1, 1, 1};
    obj->transform.rotation = (Quaternion){1, 0, 0, 0};
    obj->MeshType = CUBE;
    obj->length = 1.0;
    obj->width = 1.0;
    obj->height = 1.0;
    obj->radius = 1.0;
    obj->radiusTop = 1.0;
    obj->radiusBottom = 1.0;
    obj->slices = 1;
    obj->color = RAYWHITE;

    printf("Pointer made, returning\n");
    // struct GameObject* ptr = &newObj;
    return obj;
}





void UpdateCam(Camera* camera)
{
    float speed = 50 * GetFrameTime();
    
    if (IsKeyDown(KEY_LEFT_SHIFT))
        speed = 100 * GetFrameTime();

    if (IsKeyDown(KEY_W))
        CameraMoveForward(camera, speed, false);
    if (IsKeyDown(KEY_A))
        CameraMoveRight(camera, -speed, false);
    if (IsKeyDown(KEY_S))
        CameraMoveForward(camera, -speed, false);
    if (IsKeyDown(KEY_D))
        CameraMoveRight(camera, speed, false);

    CameraYaw(camera, GetMouseDelta().x * -0.005f, false);
    CameraPitch(camera, GetMouseDelta().y * -0.005f, false, false, false);
}





void PrintGlobalObjects(struct GameObject* list[], int num)
{
    printf("Global Objects:\n");
    for (int i = 0; i < num; i++)
    {
        printf("\n");
        printf("%s\n", list[i]->name);
        printf("%d, %d, %d\n", list[i]->transform.translation.x, list[i]->transform.translation.y, list[i]->transform.translation.z);
        printf("MesType: %d\n",list[i]->MeshType);
        printf("Color: %d\n",list[i]->color);
        printf("\n");
    }
}





int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    // Create the Window
    int screenWidth = 1280;
    int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "3D Engine");

    SetExitKey(KEY_NULL);


    struct GameObject* GlobalObjects[100];
    int numObjects = 0;


    struct Button* Menu[20];
    int numButtons = 0;


    // Initialize camera
    Camera camera = {0};

    camera.position = (Vector3){ 0.0f, 3.0f, 0.0f };
    camera.target = (Vector3){ -16.0f, 3.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 70.0f;
    camera.projection = CAMERA_PERSPECTIVE;



    // Create some test objects
    // struct GameObject* wall = new GameObject("wall", CUBE);
    struct GameObject* wall = malloc(sizeof(struct GameObject));
    wall->name = "Wall";
    wall->transform.translation = (Vector3){ -16.0f, 2.5f, 0.0f };
    wall->transform.scale = (Vector3){ 1.0f, 5.0f, 32.0f };
    wall->length = 1.0f;
    wall->width = 1.0f;
    wall->height = 1.0f;
    wall->color = GREEN;
    wall->MeshType = CUBE;
    // struct GameObject* ptr = &wall;
    GlobalObjects[0] = wall;
    numObjects++;
    // GlobalObjects.push_back(wall);

    // GameObject* circle = new GameObject("circle", SPHERE);
    struct GameObject* circle = malloc(sizeof(struct GameObject));
    circle->name = "Sphere";
    circle->transform.translation = (Vector3){5.0f, 4.0f, 5.0f};
    circle->transform.scale = (Vector3){1, 1, 1};
    circle->radius = 1;
    circle->color = PURPLE;
    circle->MeshType = SPHERE;
    // struct GameObject* ptr2 = &circle;
    GlobalObjects[1] = circle;
    numObjects++;
    // GlobalObjects.push_back(circle);



    struct Button* Quit = malloc(sizeof(struct Button));
    Quit->name = "Quit";
    Quit->rect.width = 100;
    Quit->rect.height = 50;
    Quit->rect.x = 10;
    Quit->rect.y = screenHeight - 10 - Quit->rect.height;
    Quit->color = RAYWHITE;
    Quit->text = "Quit";
    Quit->textPos = (Vector2){ Quit->rect.x + (Quit->rect.width / 2) - (MeasureTextEx(GetFontDefault(), Quit->text, 16, 0).x / 2),
                                Quit->rect.y + (Quit->rect.height / 2) - (MeasureTextEx(GetFontDefault(), Quit->text, 16, 0).y / 2) };

    Menu[numButtons] = Quit;
    numButtons++;



    // Set some variables
    SetTargetFPS(60);

    DisableCursor();

    bool paused = false;
    bool running = true;
    while (running == true && WindowShouldClose() == false)
    {
        // Update
        // Closes the game
        if (IsKeyPressed(KEY_BACKSLASH))
            running = false;
        
        // Sets whether the cursor is active
        if (IsKeyPressed(KEY_ESCAPE))
        {
            paused = !paused;
            if (paused == false)
                DisableCursor();
            else if (paused == true)
                EnableCursor();
        }

        if (IsKeyPressed(KEY_O))
        {
            struct GameObject* obj = CreateObject();
            GlobalObjects[numObjects] = obj;
            numObjects++;
            printf("Object made\n");
            printf("numObjects is currently: %d\n", numObjects);
        }

        if (IsKeyPressed(KEY_P))
        {
            PrintGlobalObjects(GlobalObjects, numObjects);
        }


        if (paused == false)
        {
            UpdateCam(&camera);
        }

        // Get the screen width and height every frame in case in changes
        if (GetScreenWidth() != screenWidth || GetScreenHeight() != screenHeight)
        {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();
            // UpdateUI();
        }
        

        // Drawing section
        BeginDrawing();

        ClearBackground(LIGHTGRAY);
        
        BeginMode3D(camera);

        DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, BLUE);

        // DrawCube(wall.transform.translation, wall.transform.scale.x, wall.transform.scale.y, wall.transform.scale.z, GREEN);
        // DrawSphere(circle.transform.translation, circle.transform.scale.x, PURPLE);

        for (int i = 0; i < numObjects; i++)
        {
            struct GameObject* currObj = GlobalObjects[i];

            if (currObj->MeshType == CUBE)
            {
                DrawCube(currObj->transform.translation,
                        currObj->width * currObj->transform.scale.x,
                        currObj->height * currObj->transform.scale.y,
                        currObj->length * currObj->transform.scale.z,
                        currObj->color);
                DrawCubeWires(currObj->transform.translation,
                            currObj->width * currObj->transform.scale.x,
                            currObj->height * currObj->transform.scale.y,
                            currObj->length * currObj->transform.scale.z,
                            DARKGREEN);
            }
            else if (currObj->MeshType == SPHERE)
            {
                DrawSphere(currObj->transform.translation, currObj->radius, currObj->color);
            }
            else if (currObj->MeshType == CYLINDER)
            {
                DrawCylinder(currObj->transform.translation,
                            currObj->radiusTop, currObj->radiusBottom,
                            currObj->height, currObj->slices,
                            currObj->color);
            }
            else if (currObj->MeshType == CAPSULE)
            {
                continue;
            }
        }


        EndMode3D();

        DrawFPS(10, 10);

        if (paused == true)
        {
            DrawRectangle(Quit->rect.x, Quit->rect.y, Quit->rect.width, Quit->rect.height, Quit->color);
            DrawText(Quit->text, Quit->textPos.x, Quit->textPos.y, 16, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();


    return 0;
}

/*
gcc ./src/main.c -o rayengine -I include -L lib -lraylib -lgdi32 -lwinmm
gcc main.c -I include -L lib -lraylib -lgdi32 -lwinmm
*/