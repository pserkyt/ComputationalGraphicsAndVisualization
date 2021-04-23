// Name: geometry.cpp
// Author: Cameron Winningham
// Date: 4/9/21
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Geometry and other math fun!

#include "geometry.h"

/**
 * Cross product.
 * @param vector1 First vector.
 * @param vector2 Second vector.
 * @return Cross product of two vectors.
 */
glm::vec3 crossProduct(const glm::vec3 &v1, const glm::vec3 &v2) {
    glm::vec3 temp;
    temp.x = v1.y * v2.z - v1.z * v2.y;
    temp.y = v1.z * v2.x - v1.x * v2.z;
    temp.z = v1.x * v2.y - v1.y * v2.x;

    return temp;
}

// Returns the normal of a vector:
glm::vec3 normalize(const glm::vec3 &v) {
    glm::vec3 temp;

    float mag = glm::vec3(v.x, v.y, v.z).length(); // get the magnitude

    temp.x = (v.x / mag); // divide the vector's X by the magnitude
    temp.y = (v.y / mag); // divide the vector's Y by the magnitude
    temp.z = (v.z / mag); // divide the vector's Z by the magnitude

    return temp;
}

// Returns the normal of a polygon (the direction the polygon is facing)
glm::vec3 polygonNormal(glm::vec3 vPolygon[]) {
    // Get 2 vectors from the polygon (2 sides) ORDER MATTERS!
    glm::vec3 vector1 = vPolygon[1] - vPolygon[0];
    glm::vec3 vector2 = vPolygon[2] - vPolygon[0];

    // Take the cross product of the 2 vectors to get a perpendicular vector:
    glm::vec3 normal = crossProduct(vector1, vector2);

    normal = normalize(normal); // Normalize the normal (makes it a length of one)

    return normal;
}


