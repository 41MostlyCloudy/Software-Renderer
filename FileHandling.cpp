#pragma once

#include "Global.cpp"
#include "UsefulFunctions.cpp"
#include <sstream>
#include <iomanip>

// For searching file directories. New as of c++ 17.
#include <filesystem>




#include <cstdlib>



#define STB_IMAGE_IMPLEMENTATION // Image loading library made by Sean Barrett. Loads PNGs Modified to remove most parts having to do with other file formats.
#include "stb_image.h"



//using std::filesystem::directory_iterator;


// Loads objects and textures
void LoadAssets();

// Load the texture with the given name
void LoadTexture(std::string filePath);

// Load the model with the given name
void LoadModel(std::string filePath);

// Return the index of the mesh with the given name
int GetMesh(std::string meshName);

// Return the index of the mesh with the given name
int GetTexture(std::string textureName);




int GetMesh(std::string meshName)
{
    
    for (int i = 0; i < loadedMeshes.size(); i++)
    {
        if (loadedMeshes[i].meshName == meshName)
            return i;
    }

    std::cout << " Couldn't find mesh " << meshName << ". ";
    return 0;
}



int GetTexture(std::string textureName)
{

    for (int i = 0; i < loadedTextures.size(); i++)
    {
        if (loadedTextures[i].textureName == textureName)
            return i;
    }

    std::cout << " Couldn't find texture " << textureName << ". ";
    return 0;
}



void LoadModel(std::string filePath)
{
    Mesh newMesh;

    std::ifstream objectData;
    objectData.open(filePath);

    std::string fileName = "";

    for (int i = 7; i < int(filePath.length() - 4); i++)
        fileName += filePath[i];

    newMesh.meshName = fileName;

    std::string line;

    std::vector <Vector3> vertexData;
    std::vector <RGBFloat> colorData;
    std::vector <UV> uvData;

    for (int i = 0; i < 5; i++) getline(objectData, line); // The first four lines are not used

    while (line[0] == 'v' && line[1] != 't') // Read in the vertex data
    {
        Vector3 pos;
        int index = 2;
        std::string num = "";

        while (line[index] != ' ') { num += line[index]; index++; }
        pos.x = stof(num);
        num = "";
        index++;
        while (line[index] != ' ') { num += line[index]; index++; }
        pos.y = stof(num);
        num = "";
        index++;
        while (index < line.length() && line[index] != ' ') { num += line[index]; index++; }
        pos.z = stof(num);
        num = "";
        index++;


        vertexData.emplace_back(pos);

        RGBFloat col = { 1, 1, 1 };

        if (index < line.length()) // Load vertex color sif there are any.
        {
            while (line[index] != ' ') { num += line[index]; index++; }
            col.r = stof(num);
            num = "";
            index++;
            while (line[index] != ' ') { num += line[index]; index++; }
            col.g = stof(num);
            num = "";
            index++;
            while (index < line.length()) { num += line[index]; index++; }
            col.b = stof(num);
            num = "";
            index++;
        }

        colorData.emplace_back(col);

        getline(objectData, line);
    }
    while (line[0] == 'v') // Read in the uv data
    {
        UV uv;
        int index = 3;
        std::string num = "";

        while (line[index] != ' ') { num += line[index]; index++; }
        uv.u = stof(num);
        num = "";
        index++;
        while (index < line.length()) { num += line[index]; index++; }
        uv.v = stof(num);

        uvData.emplace_back(uv);

        getline(objectData, line);
    }


    while (getline(objectData, line) && (line[0] == 's' || line[0] == 'f' || line[0] == 'u')) // Read in the triangles
    {
        if (line[0] == 'u')
        {
            int index = 7;
            std::string texName = "";

            while (index < line.length()) { texName += line[index]; index++; }

            newMesh.texture = GetTexture(texName);

        }
        else if (line[0] != 's')
        {
            Triangle newTri;
            int index = 2;
            std::string num = "";
            int pos;

            while (line[index] != '/') { num += line[index]; index++; }
            pos = stoi(num) - 1;
            num = "";
            index++;
            newTri.p[0].coord = vertexData[pos];
            newTri.p[0].vertCol = colorData[pos];
            while (line[index] != ' ') { num += line[index]; index++; }
            pos = stoi(num) - 1;
            num = "";
            index++;
            newTri.p[0].uv = uvData[pos];

            while (line[index] != '/') { num += line[index]; index++; }
            pos = stoi(num) - 1;
            num = "";
            index++;
            newTri.p[1].coord = vertexData[pos];
            newTri.p[1].vertCol = colorData[pos];
            while (line[index] != ' ') { num += line[index]; index++; }
            pos = stoi(num) - 1;
            num = "";
            index++;
            newTri.p[1].uv = uvData[pos];

            while (line[index] != '/') { num += line[index]; index++; }
            pos = stoi(num) - 1;
            num = "";
            index++;
            newTri.p[2].coord = vertexData[pos];
            newTri.p[2].vertCol = colorData[pos];
            while (index < line.length()) { num += line[index]; index++; }
            pos = stoi(num) - 1;
            num = "";
            index++;
            newTri.p[2].uv = uvData[pos];


            newMesh.tris.emplace_back(newTri);
        }
    }

    loadedMeshes.emplace_back(newMesh);

    //cout << "Model " << fileName << " loaded.";

    objectData.close();
}



void LoadTexture(std::string filePath)
{
    Texture loadingTexture;

    std::string fileName = "";

    for (int i = 9; i < int(filePath.length() - 4); i++)
        fileName += filePath[i];

    loadingTexture.textureName = fileName;



    char const* fileName1 = filePath.c_str();
    int x, y, comps;
    unsigned char* texData = stbi_load(fileName1, &x, &y, &comps, 3);


    loadingTexture.px.resize(x * y);

    loadingTexture.textureWidth = x;
    loadingTexture.textureHeight = y;


    for (int i = 0; i < y; i++)
    {
        for (int j = 0; j < x; j++)
        {
            unsigned char* pixelOffset = texData + (j + i * x) * 3;

            RGBColor pixel = { pixelOffset[0], pixelOffset[1], pixelOffset[2] };

            loadingTexture.px[x * (y - i - 1) + j] = pixel;
        }
    }

    loadedTextures.emplace_back(loadingTexture);

    stbi_image_free(texData);
}
                                                                       



void LoadAssets()
{
    // Load textures as the models use them.
    // Load textures
    bool fileExists = true;
    int m = 0;
    std::string fileName;

    // Read .png files
    for (auto const& dir_entry : std::filesystem::directory_iterator("Textures/"))
    {
        std::string filePath = dir_entry.path().generic_string();
        LoadTexture(filePath);
    }

    // Read .obj files
    for (auto const& dir_entry : std::filesystem::directory_iterator("Models/"))
    {
        std::string filePath = dir_entry.path().generic_string();
        LoadModel(filePath);
    }

    // Create mesh instances
    for (int i = 0; i < loadedMeshes.size(); i++)
    {
        MeshInstance newInstance;
        newInstance.instanceMesh = GetMesh(loadedMeshes[i].meshName);
        newInstance.position = { 0, 0, 0 };
        newInstance.rotation = { 0, 0, 0 };
        loadedMeshInstances.emplace_back(newInstance);
    }
}