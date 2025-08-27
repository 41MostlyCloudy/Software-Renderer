#pragma once



#include "Objects.cpp"

// Important variables
float deltaT;// Multiply to get frame-independent speed.
float cameraNear = 0.5; // The near distance from the camera that triangles are clipped
float cameraFar = 400; // The furthest away an object is visible.

// Screen configuration variables
float windowRatio = 1; // The aspect ratio of your computer screen
int screenWidth = 512;
int screenHeight = 384;
float screenRatio = 1; // The aspect ratio of the screen the game is draw to
float fov = 1; // Field of view



double mousePosX, mousePosY = 0;
float mouseSensitivity = 0.2;

//float playerSize = 1;
//bool playerOnFloor = false;

// Resources
std::vector <Texture> loadedTextures;
std::vector <Mesh> loadedMeshes;
std::vector <MeshInstance> loadedMeshInstances;



// Level properties
Vector3 playerStartPosition = { 0, 0, 0 };
Vector3 playerStartRotation = { 0, 0, 0 };


// Editing tool
bool displayFPS = true;



Camera mainCam;