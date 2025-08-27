// Zamboni Zone


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Shaders.h"
#include "Global.cpp"
#include "FileHandling.cpp"
#include "Render.cpp"









    

// Runs everything
void RunEngine();

// Takes user input
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Updates physics, called every frame
void UpdatePhysics(float delta);



// The actions performed when starting and running the engine
void RunEngine()
{
    // Initialize the library
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

    // Load the meshes
    LoadAssets();

    

    // Create a windowed mode window and its OpenGL context
    float windowWidth = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
    float windowHeight = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;

    
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "", glfwGetPrimaryMonitor(), nullptr);

    // Update the screen data to the screen size
    mainCam.depthBuffer = new float[screenWidth * screenHeight];
    mainCam.screenColorData = new RGBColor[screenWidth * screenHeight];

    windowRatio *= float(windowWidth) / float(windowHeight);
    screenRatio = 1.0f / windowRatio;




    // Create the window
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, windowWidth, windowHeight);
    

   //std::cout << "a";

    ////////////////////////////////////////////////////////////////////////////// Set up the sprite mesh

    float screenVertices[] = {
        -1.0, -1.0, 0.0, 0.0,
        1.0, -1.0, 1.0, 0.0,
        -1.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0,
        1.0, -1.0, 1.0, 0.0,
        -1.0, 1.0f, 0.0, 1.0,
    };
    ////////////////////////////////////////////////////////////////////////////// Set up the sprite VAO

    unsigned int sVBO;
    unsigned int sVAO;

    glGenBuffers(1, &sVBO);
    glGenVertexArrays(1, &sVAO);

    // 1. bind Vertex Array Object
    glBindVertexArray(sVAO);
    // Bind the buffer to the VBO
    glBindBuffer(GL_ARRAY_BUFFER, sVBO);

    // Use the buffer to fill the VBO with the vertices
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), screenVertices, GL_STATIC_DRAW);
    // Assign how vertex attributes should be interpreted.

    // Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // UV coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    

    ////////////////////////////////////////////////////////////////////////////// Create and compile the vertex shaders
    unsigned int uiVertexShader;
    uiVertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Compile the shader
    glShaderSource(uiVertexShader, 1, &uiVertexShaderSource, NULL);
    glCompileShader(uiVertexShader);
    ////////////////////////////////////////////////////////////////////////////// Create and compile the fragment shaders
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Create the shader programs
    unsigned int uiShaderProgram;
    uiShaderProgram = glCreateProgram();

    glAttachShader(uiShaderProgram, uiVertexShader);
    glAttachShader(uiShaderProgram, fragmentShader);
    glLinkProgram(uiShaderProgram);
    // Use the screen shader.
    glUseProgram(uiShaderProgram);



    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glfwSetCursorPos(window, 0, 0);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Create Screen Texture
    unsigned int screenTex;
    glGenTextures(1, &screenTex);
    glBindTexture(GL_TEXTURE_2D, screenTex);
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //players[0].cameraRotation.y = 180;
    
    

    // Set the mouse position for rotation.
    glfwSetCursorPos(window, 400, 60);
    
    
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Start the delta timer
        std::chrono::high_resolution_clock time;
        auto start = time.now();

        // Update game physics
        UpdatePhysics(deltaT);




        // Draw the screen
        for (int i = 0; i < (screenWidth * screenHeight); i++)
        {
            mainCam.depthBuffer[i] = 400; // Here, 400 is the farthest away from the camera an object can render
            mainCam.screenColorData[i] = { 0, 0, 0 };
        }

        // Draw the triangles for each loaded mesh
        for (int i = 0; i < loadedMeshInstances.size(); i++)
        {
            for (int j = 0; j < loadedMeshes[loadedMeshInstances.at(i).instanceMesh].tris.size(); j++)
            {
                Triangle worldPoint = loadedMeshes[loadedMeshInstances.at(i).instanceMesh].tris[j];


                // Apply object transformations and rotations
                for (int k = 0; k < 3; k++)
                {
                    worldPoint.p[k].coord = RotateX(worldPoint.p[k].coord, loadedMeshInstances.at(i).rotation.x);
                    worldPoint.p[k].coord = RotateZ(worldPoint.p[k].coord, loadedMeshInstances.at(i).rotation.z);
                    worldPoint.p[k].coord = RotateY(worldPoint.p[k].coord, loadedMeshInstances.at(i).rotation.y);
                    worldPoint.p[k].coord = Translate(worldPoint.p[k].coord, loadedMeshInstances.at(i).position);
                    worldPoint.p[k].coord = Translate(worldPoint.p[k].coord, { -mainCam.cameraPosition.x, -mainCam.cameraPosition.y, -mainCam.cameraPosition.z });
                }



                // Find the normal of the triangle
                float normal = CalculateNormal(worldPoint);


                // Draw the projected triangle.
                if (normal < 0) // Back face culling
                {
                    // Rotate each triangle to match camera space, and then project it.
                    for (int k = 0; k < 3; k++)
                    {
                        worldPoint.p[k].coord = RotateY(worldPoint.p[k].coord, mainCam.cameraRotation.y);
                        worldPoint.p[k].coord = RotateX(worldPoint.p[k].coord, mainCam.cameraRotation.x);
                    }

                    if (loadedMeshInstances.at(i).vertexColorOverride.r < 1 || loadedMeshInstances.at(i).vertexColorOverride.g < 1 || loadedMeshInstances.at(i).vertexColorOverride.b < 1)
                    {
                        worldPoint.p[0].vertCol = loadedMeshInstances.at(i).vertexColorOverride;
                        worldPoint.p[1].vertCol = loadedMeshInstances.at(i).vertexColorOverride;
                        worldPoint.p[2].vertCol = loadedMeshInstances.at(i).vertexColorOverride;
                    }

                    Clip(worldPoint, false, loadedMeshes[loadedMeshInstances.at(i).instanceMesh].texture, 0);
                }
            }
        }






        // Draw framerate counter
        if (displayFPS)
            DrawText("FPS " + std::to_string(1000 / deltaT), 0, 0); // Draw FPS counter
        
        
        ///////////////////////////////////////////////////////////////////////////

        // Create window quad

        //glBindTexture(GL_TEXTURE_2D, players[0].screenColorData);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, uiSpriteAtlasBuffer);
        

        //glBindTexture(GL_TEXTURE_2D, screenTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mainCam.screenColorData);
        
        //glBindTexture(GL_TEXTURE_2D, screenTex);
        //for (int i = 0; i < screenWidth; i++)
        //    std::cout << players.screenColorData[i].r;

        glBindVertexArray(sVAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1); // Draw the sprites.
        
        
        
        //glBindVertexArray(VAO);
        //glBindTexture(GL_TEXTURE_2D, crtTexture);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();


        // Process player input
        processInput(window);

        

        

        // Find the frame time
        auto end = time.now();
        using ms = std::chrono::duration<float, std::milli>;
        deltaT = std::chrono::duration_cast<ms>(end - start).count();
    }

    glfwTerminate();
}



void UpdatePhysics(float delta)
{
    
    Vector3 rotatedVelocity = RotateY(mainCam.cameraVelocity, -mainCam.cameraRotation.y);
    rotatedVelocity.x *= 0.03 * delta;
    rotatedVelocity.y *= 0.03 * delta;
    rotatedVelocity.z *= 0.03 * delta;
    mainCam.cameraPosition = Translate(mainCam.cameraPosition, rotatedVelocity);
}



void processInput(GLFWwindow* window)
{
    //if (editing)
    //{
    mainCam.cameraVelocity.x = (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) - (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);
    mainCam.cameraVelocity.y = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) - (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
    mainCam.cameraVelocity.z = (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) - (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS);
    
    //}
    //else
    //{
    //    cameraVelocity.x = (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) - (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);
    //    cameraVelocity.z = (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) - (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS);
    //}
    


    glfwSetKeyCallback(window, key_callback);
    

    glfwGetCursorPos(window, &mousePosX, &mousePosY);


    // Set rotation to wrap

    float mouseS = 1 / (mouseSensitivity);

    if (mousePosY > 90 * mouseS)
    {
        mousePosY = 90 * mouseS;
    }
    else if (mousePosY < -90 * mouseS)
    {
        mousePosY = -90 * mouseS;
    }
    if (mousePosX > 360 * mouseS)
    {
        mousePosX -= 360 * mouseS;
    }
    else if (mousePosX < 0 * mouseS)
    {
        mousePosX += 360 * mouseS;
    }
    


    mainCam.cameraRotation.y = -mousePosX * mouseSensitivity;
    mainCam.cameraRotation.x = mousePosY * mouseSensitivity;


    glfwSetCursorPos(window, mousePosX, mousePosY);
}


// Checks for input the moment a key is pressed
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {

        if (key == GLFW_KEY_ESCAPE)
        {
           glfwTerminate();
        }


        if (key == GLFW_KEY_2)
        {
            displayFPS = !displayFPS;
        }


        /*
        if (key == GLFW_KEY_3)
        {
            editMode++;

            if (editMode > 1)
                editMode = 0;
        }*/

        if (key == GLFW_KEY_EQUAL)
        {
            fov -= 0.125;
        }

        if (key == GLFW_KEY_MINUS)
        {
            fov += 0.125;
        }
    }
}


int main(void)
{
    RunEngine();

    return 0;
}