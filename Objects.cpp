#pragma once



#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono> // Deals with time
#include <stdlib.h> // For rand()



// Color structure
struct RGBColor { uint8_t r, g, b; };
struct RGBFloat { float r, g, b; };



// texture of any size (defaulting to 64x) using colors from the lookup table
struct Texture
{
    std::string textureName = "";
    std::vector <RGBColor> px; // Array of texture pixels
    int textureWidth = 64;
    int textureHeight = 64;
};


// 2D structure
struct UV { float u = 0, v = 0; };


// 3D structure
struct Vector3 { float x = 0, y = 0, z = 0; };


// A point on a mesh with usual point data
struct Point
{
    Vector3 coord;
    RGBFloat vertCol;
    UV uv;
};



// A triangle structure
struct Triangle
{
    Point p[3]; // The position of each vertex
};



// A 3d object structure
struct Mesh
{
    std::string meshName = "";
    int texture = 0; // The texture to be drawn on the object
    std::vector<Triangle> tris;
};



// A 3d object instance
struct MeshInstance
{
    int instanceMesh;
    Vector3 position;
    Vector3 rotation; // Rotation is measured in degrees
    RGBFloat vertexColorOverride = { 1, 1, 1 }; // A color multiplied with the model's surface, overriding vertex colors. Ignored when white.
};



// A player
struct Camera
{
    RGBColor* screenColorData; // Screen data
    float* depthBuffer;

    // Movement
    Vector3 cameraPosition = { 14.0f, 4.0f, 4.0f };
    Vector3 cameraRotation;
    Vector3 cameraVelocity;
    Vector3 CameraVelocity; // How fast the player is moving
};