#pragma once

#include "Global.cpp"


// Move a point in 3d space
Vector3 Translate(Vector3 vect, Vector3 vect2);

// Find the normal of a triangle
float CalculateNormal(Triangle tri);

// Functions for rotating vectors around axes in radians
Vector3 RotateX(Vector3 vec, float angle);
Vector3 RotateY(Vector3 vec, float angle);
Vector3 RotateZ(Vector3 vec, float angle);






Vector3 Translate(Vector3 vect, Vector3 vect2)
{
    return { vect.x + vect2.x, vect.y + vect2.y, vect.z + vect2.z };
}



float CalculateNormal(Triangle tri)
{
    // Use Cross Product formula to find the normal of the triangle. Only draw triangles with a normal facing the camera
    Vector3 normal;
    normal.x = ((tri.p[1].coord.y - tri.p[0].coord.y) * (tri.p[2].coord.z - tri.p[0].coord.z)) - ((tri.p[1].coord.z - tri.p[0].coord.z) * (tri.p[2].coord.y - tri.p[0].coord.y));
    normal.y = ((tri.p[1].coord.z - tri.p[0].coord.z) * (tri.p[2].coord.x - tri.p[0].coord.x)) - ((tri.p[1].coord.x - tri.p[0].coord.x) * (tri.p[2].coord.z - tri.p[0].coord.z));
    normal.z = ((tri.p[1].coord.x - tri.p[0].coord.x) * (tri.p[2].coord.y - tri.p[0].coord.y)) - ((tri.p[1].coord.y - tri.p[0].coord.y) * (tri.p[2].coord.x - tri.p[0].coord.x));

    // Normalize the face's normal vector
    float vecLength = 1 / sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    normal.x *= vecLength;
    normal.y *= vecLength;
    normal.z *= vecLength;

    Vector3 vec3;
    float vecLength2 = 1 / sqrt(tri.p[0].coord.x * tri.p[0].coord.x + tri.p[0].coord.y * tri.p[0].coord.y + tri.p[0].coord.z * tri.p[0].coord.z);
    vec3.x = tri.p[0].coord.x * vecLength2;
    vec3.y = tri.p[0].coord.y * vecLength2;
    vec3.z = tri.p[0].coord.z * vecLength2;

    // Return the dot product
    return (normal.x * vec3.x) + (normal.y * vec3.y) + (normal.z * vec3.z);
}



// Functions for transforming Vectors
Vector3 RotateX(Vector3 vec, float angle)
{
    Vector3 retVec;

    // Cinvert degrees to radians
    angle *= 3.1415 / 180;


    retVec.x = vec.x;
    retVec.y = vec.y * cos(angle) + vec.z * sin(angle);
    retVec.z = vec.y * -sin(angle) + vec.z * cos(angle);

    return retVec;
}


Vector3 RotateY(Vector3 vec, float angle)
{
    Vector3 retVec;

    // Cinvert degrees to radians
    angle *= 3.1415 / 180;

    retVec.x = vec.x * cos(angle) + vec.z * -sin(angle);
    retVec.y = vec.y;
    retVec.z = vec.x * sin(angle) + vec.z * cos(angle);

    return retVec;
}


Vector3 RotateZ(Vector3 vec, float angle)
{
    Vector3 retVec;

    // Cinvert degrees to radians
    angle *= 3.1415 / 180;

    retVec.x = vec.x * cos(angle) + vec.y * sin(angle);
    retVec.y = vec.x * sin(angle) + vec.y * cos(angle);
    retVec.z = vec.z;

    return retVec;
}