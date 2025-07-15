#include "raylib.h"
#include "rcamera.h"

#include <stdio.h>

#define CUBE 100
#define SPHERE 200
#define CYLINDER 300
#define CAPSULE 400
#define CUSTOM 500

struct GameObject
{
    char name[50];
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



int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    // Create the Window
    int screenWidth = 1280;
    int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "3D Engine");

    SetExitKey(KEY_NULL);


    Camera camera = {0};

    camera.position = (Vector3){ 0.0f, 3.0f, 0.0f };
    camera.target = (Vector3){ -16.0f, 3.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 70.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Create some test objects
    // struct GameObject* wall = new GameObject("wall", CUBE);
    struct GameObject wall;
    wall.transform.translation = (Vector3){ -16.0f, 2.5f, 0.0f };
    wall.transform.scale = (Vector3){ 1.0f, 5.0f, 32.0f };
    // GlobalObjects.push_back(wall);

    // GameObject* circle = new GameObject("circle", SPHERE);
    struct GameObject circle;
    circle.transform.translation = (Vector3){5.0f, 4.0f, 5.0f};
    circle.transform.scale = (Vector3){1, 1, 1};
    // GlobalObjects.push_back(circle);

    SetTargetFPS(60);

    DisableCursor();

    short paused = 0;
    short running = 1;
    while (running == 1 && WindowShouldClose() == false)
    {
        // Update
        // Closes the game
        if (IsKeyPressed(KEY_BACKSLASH))
            running = 0;
        
        // Sets whether the cursor is active
        if (IsKeyPressed(KEY_ESCAPE))
        {
            paused = 1 - paused;
            if (paused == 0)
                DisableCursor();
            else if (paused == 1)
                EnableCursor();
        }

        if (paused == 0)
        {
            UpdateCam(&camera);
        }

        if (GetScreenWidth() != screenWidth || GetScreenHeight() != screenHeight)
        {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();
        }
        

        // Drawing section
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        BeginMode3D(camera);

        DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, BLUE);

        DrawCube(wall.transform.translation, wall.transform.scale.x, wall.transform.scale.y, wall.transform.scale.z, GREEN);
        DrawSphere(circle.transform.translation, circle.transform.scale.x, PURPLE);


        EndMode3D();

        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();


    return 0;
}

/*
gcc ./src/main.c -o rayengine -I include -L lib -lraylib -lgdi32 -lwinmm
gcc main.c -I include -L lib -lraylib -lgdi32 -lwinmm
*/