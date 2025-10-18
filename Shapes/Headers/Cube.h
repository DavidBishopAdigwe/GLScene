#pragma once
#include <GL/gl.h>

namespace Cube
{

    constexpr static int indexCount{ 36 };

    constexpr static GLfloat vertices[192]
    {
        // Front face
    -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // Top-left
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // Top-right
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // Bottom-left
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // Bottom-right

     // Back face
       0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f, 1.0f,  // Top-left 
      -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,   1.0f, 1.0f,  // Top-right
       0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,   0.0f, 0.0f,  // Bottom-left
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,   1.0f, 0.0f,  // Bottom-right

      // Left face
       -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Top-left
       -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // Top-right
       -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,  // Bottom-left
       -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,  // Bottom-right

       // Right face
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Top-left
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // Top-right
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,   0.0f, 0.0f,  // Bottom-left
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,   1.0f, 0.0f,  // Bottom-right

         // Top face
          -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // Top-left
           0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // Top-right
          -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,   0.0f, 0.0f,  // Bottom-left
           0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // Bottom-right

           // Bottom face
           -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Top-left
            0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // Top-right
           -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   0.0f, 0.0f,  // Bottom-left
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   1.0f, 0.0f   // Bottom-right
    };

    constexpr static GLuint indices[36]
    {
        // Front face
         0, 1, 2,
         1, 3, 2,

         // Back face
         4, 5, 6,
         5, 7, 6,

         // Left face
         8, 9, 10,
         9, 11, 10,

         // Right face
         12, 13, 14,
         13, 15, 14,

         // Top face
         16, 17, 18,
         17, 19, 18,

         // Bottom face
         20, 21, 22,
         21, 23, 22
    };

}
