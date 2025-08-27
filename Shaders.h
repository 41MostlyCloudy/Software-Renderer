#pragma once


const char* uiVertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"

"out vec2 TexCoord;\n"

"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y * -1.0f, aPos.z, 1.0);\n"
"	TexCoord = aTexCoord;\n"

"}\0";



// Fragment shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n" // we set this variable in the OpenGL code.

"uniform sampler2D ourTexture;\n"

"void main()\n"
"{\n"
"   FragColor = texture(ourTexture, TexCoord); \n"
"}\n";