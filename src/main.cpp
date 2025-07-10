#include "raylib.h"
#include "rcamera.h"
//#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
//#include <memory>


#define CUBE 100
#define SPHERE 200
#define CYLINDER 300
#define CAPSULE 400
#define CUSTOM 500


// Player Struct, which has a camera and a specified height
struct MainCamera
{
    Camera camera;

    float movementSpeed = 50;
    float fastMovementSpeed = 100;

    void Update()
    {
        float speed = movementSpeed * GetFrameTime();
        
        if (IsKeyDown(KEY_LEFT_SHIFT))
            speed = fastMovementSpeed * GetFrameTime();

        if (IsKeyDown(KEY_W))
            CameraMoveForward(&camera, speed, false);
        if (IsKeyDown(KEY_A))
            CameraMoveRight(&camera, -speed, false);
        if (IsKeyDown(KEY_S))
            CameraMoveForward(&camera, -speed, false);
        if (IsKeyDown(KEY_D))
            CameraMoveRight(&camera, speed, false);

        CameraYaw(&camera, GetMouseDelta().x * -0.005f, false);
        CameraPitch(&camera, GetMouseDelta().y * -0.005f, false, false, false);
    }
};


// GameObject Struct. Includes a name, a transform, and a meshtype for rendering
struct GameObject
{
    std::string name;
    Transform transform;
    int MeshType;


    Color color;
    
    // Cube properties
    float length = 1.0;
    float width = 1.0;
    float height = 1.0;
    // Sphere properties
    float radius = 1.0;
    // Cylinder properties
    float radiusTop = 1.0;
    float radiusBottom = 1.0;
    int slices = 1;
    // Model properties
    Model model;



    // A vector would be used for components in the future
    //std::vector<std::unique_ptr<Component>> components;
    //std::vector<void*> components;

    GameObject(std::string str, int type)
    {
        name = str;

        transform.translation = { 0, 0, 0 };
        transform.rotation = { 0, 0, 0, 0 };
        transform.scale = { 1, 1, 1 };

        MeshType = type;

        length, width, height, radius, radiusTop, radiusBottom = 1.0;
        slices = 1;
        color = WHITE;
    }
};


struct Button
{
    std::string text;
    Vector2 location;
    int width;
    int height;
    Color color;

    int buttonState;

    void Draw()
    {
        DrawRectangle(location.x, location.y, width, height, color);
    }

    bool CheckPress(int x, int y)
    {
        if (x >= location.x &&
            x <= location.x + width &&
            y >= location.y &&
            y <= location.y + height)
        {
            return true;
        }
        else
            return false;
    }
};

struct Textfield
{
    Vector2 location;
    Rectangle box;
    Color color;

    bool active;

    std::string str;

    Textfield(Vector2 spot)
    {
        location = spot;
        box = { location.x, location.y, 100, 50 };
        color = LIGHTGRAY;
        active = false;
        str = "";
    }


    bool CheckPress(Vector2 spot)
    {
        if (spot.x >= box.x && spot.x <= box.x + box.width &&
            spot.y >= box.y && spot.y <= box.y + box.height)
        {
            return true;
        }
        else
            return false;
    }

    void Update()
    {
        if (CheckCollisionPointRec(GetMousePosition(), box))
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
        }
        else
        {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        if (active)
        {
            int key = GetKeyPressed();

            if (key >= 48 && key <= 57)
            {
                str = str + (char)key;
            }

            while (key > 0)
            {
                if ((key >= 48) && (key <= 57))
                {
                    str = str + (char)key;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                if (str != "")
                    str.erase(str.size() - 1);
            }
        }
    }
};



// Function creates object with given name and mesh type
// Transform is set to default from constructor.
GameObject* createObject(std::string name, int type)
{
    GameObject* obj = new GameObject(name, type);
    return obj;
}

// Prints out all GameObjects in a list
void PrintGlobalObjects(std::vector<GameObject*> list)
{
    for (const GameObject* obj : list)
    {
        //printf("%s\n", obj.name);
        std::cout << obj->name << std::endl;
    }
    std::cout << std::endl;
}



void DrawOptionsMenu(std::vector<Button> menu, /*std::vector<Textfield> fields*/ Textfield field)
{
    
    for (const Button &button : menu)
    {
        DrawRectangle(button.location.x, button.location.y, button.width, button.height, button.color);
        DrawText(button.text.c_str(),
                 button.location.x + (button.width / 2) - (MeasureTextEx(GetFontDefault(), button.text.c_str(), 16, 1).x / 2),
                 button.location.y + (button.height / 2) - (MeasureTextEx(GetFontDefault(), button.text.c_str(), 16, 1).y / 2),
                 16,
                 SKYBLUE);
    }

    //for (Textfield &field : fields)
    //{
    //    field.Update();
    //    DrawRectangle(field.location.x, field.location.y, field.box.width, field.box.height, field.color);
    //    DrawRectangleLines(field.location.x, field.location.y, field.box.width, field.box.height, DARKGRAY);
    //    DrawText(field.str.c_str(), field.location.x + 2, field.location.y + 2, 16, field.color);
    //}

    //field.Update();
    if (field.active)
        DrawRectangle(field.location.x, field.location.y, field.box.width, field.box.height, WHITE);
    else
        DrawRectangle(field.location.x, field.location.y, field.box.width, field.box.height, field.color);

    DrawRectangleLines(field.location.x, field.location.y, field.box.width, field.box.height, DARKGRAY);
    DrawText(field.str.c_str(), field.location.x + 2, field.location.y + 2, 16, field.color);


    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mouseClick = GetMousePosition();

        for (Button &button : menu)
        {
            if (button.CheckPress(mouseClick.x, mouseClick.y))
                std::cout << "Button pressed" << std::endl;
        }

        if (field.CheckPress(mouseClick))
        {
            std::cout << "Field clicked" << std::endl;
            field.active = true;
        }
        else
            field.active = false;
    }
}




int main(void)
{

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    // Create the Window
    int screenWidth = 1280;
    int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "3D Engine");

    SetExitKey(KEY_NULL);

    // Initialize the GlobalObject vector (stores all objects)
    std::vector<GameObject*> GlobalObjects;

    // Create the main camera
    struct MainCamera maincam = { {0}, 3 };
    maincam.camera.position = { 0.0f, 3.0f, 0.0f };
    maincam.camera.target = { -16.0f, 3.0f, 0.0f };
    maincam.camera.up = { 0.0f, 1.0f, 0.0f };
    maincam.camera.fovy = 70.0f;
    maincam.camera.projection = CAMERA_PERSPECTIVE;

    // Setting the main camera movement speed
    maincam.movementSpeed = 20;
    maincam.fastMovementSpeed = 60;


    // Initialize the buttons and fields for the paused menu
    std::vector<Button> menu;

    Button quit;
    quit.width = 150;
    quit.height = 50;
    quit.location = { 10, (float)GetScreenHeight() - 10 - quit.height };
    quit.color = GRAY;
    quit.text = "Quit";
    menu.push_back(quit);

    Button SetFPSButton;
    SetFPSButton.width = 100;
    SetFPSButton.height = 50;
    SetFPSButton.location = { quit.location.x, quit.location.y - quit.height - 20 };
    SetFPSButton.color = GRAY;
    SetFPSButton.text = "Set FPS";
    menu.push_back(SetFPSButton);


    std::vector<Textfield> fields;
    Textfield FPSfield = { {SetFPSButton.location.x + SetFPSButton.width + 20, SetFPSButton.location.y} };
    fields.push_back(FPSfield);




    // Create some test objects
    GameObject* wall = new GameObject("wall", CUBE);
    wall->transform.translation = { -16.0f, 2.5f, 0.0f };
    wall->transform.scale = { 1.0f, 5.0f, 32.0f };
    GlobalObjects.push_back(wall);

    GameObject* circle = new GameObject("circle", SPHERE);
    circle->transform.translation = {5.0f, 4.0f, 5.0f};
    GlobalObjects.push_back(circle);


    // Set the target FPS
    SetTargetFPS(60);

    // Disable cursor
    DisableCursor();
    

    // Set variables and start the main loop
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

        if (IsKeyPressed(KEY_LEFT_BRACKET))
            PrintGlobalObjects(GlobalObjects);

        
        // Creates a object (TODO: add customization with this
        if (IsKeyPressed(KEY_C))
        {
            GameObject* obj = createObject("Sample Name", CUBE);
            GlobalObjects.push_back(obj);
            std::cout << "object created" << std::endl;
        }


        // If the cursor isn't active, update the cameras movement and rotation
        if (paused == 0)
        {
            maincam.Update();
        }


        if (GetScreenWidth() != screenWidth || GetScreenHeight() != screenHeight)
        {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();

            quit.location = { 10, (float)GetScreenHeight() - 10 - quit.height };
            SetFPSButton.location = { quit.location.x, quit.location.y - quit.height - 20 };
            FPSfield.location = { SetFPSButton.location.x + SetFPSButton.width + 20, SetFPSButton.location.y };
        }
        

        // Drawing section
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(maincam.camera);

        // Draws floor
        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 32.0f, 32.0f }, BLUE);


        //DrawCube({player.camera.position.x, player.camera.position.y - (player.height/2), player.camera.position.z + 2 }, 1.0f, player.height, 1.0f, YELLOW);

        // Loops through the GlobalObjects and draws each object based on what it is.
        for (const GameObject* obj : GlobalObjects)
        {
            if (obj->MeshType == CUBE)
            {
                DrawCube(obj->transform.translation, obj->transform.scale.x, obj->transform.scale.y, obj->transform.scale.z, GREEN);
                DrawCubeWires(obj->transform.translation, obj->transform.scale.x, obj->transform.scale.y, obj->transform.scale.z, DARKGREEN);
            }
            
            if (obj->MeshType == SPHERE)
            {
                DrawSphere(obj->transform.translation, obj->transform.scale.x, PURPLE);
                DrawSphereWires(obj->transform.translation, obj->transform.scale.x, 4, 4, DARKPURPLE);
            }

            if (obj->MeshType == CYLINDER)
            {
                DrawCylinder(obj->transform.translation, obj->radiusTop, obj->radiusBottom, obj->height, obj->slices, obj->color);
                DrawCylinderWires(obj->transform.translation, obj->radiusTop, obj->radiusBottom, obj->height, obj->slices, obj->color);
            }

            if (obj->MeshType == CAPSULE)
                continue;

            if (obj->MeshType == CUSTOM)
                continue;
        }

        EndMode3D();

        DrawFPS(10, 10);

        if (paused == 1)
            DrawOptionsMenu(menu, FPSfield);


        EndDrawing();
    }

    CloseWindow();

    return 0;
}