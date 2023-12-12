#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <cmath>

float vert[] = { 1, 1,0,  1, -1, 0,  -1, -1, 0,  -1, 1, 0 };

float xAlpha = 100;
float zAlpha = 0;
POINTFLOAT pos = { 0, 0 };

void ShowWorld() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vert);
    for (int i = -50; i <= 50; i++)
        for (int j = -50; j <= 50; j++) {
            glPushMatrix();
            if ((i + j) % 2 == 0) {
                glColor3f(0.0f, 0.0f, 1.0f);
            }
            else
            {
                glColor3f(0.0f, 0.0f, 0.5f);
            }
            glTranslatef(i * 2, j * 2, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glPopMatrix();
        }
    glDisableClientState(GL_VERTEX_ARRAY);
}


void MoveCamera() {
     if ((GetKeyState(VK_UP) < 0) && (xAlpha < 180)) xAlpha += 2;
     if ((GetKeyState(VK_DOWN) < 0) && (xAlpha > 0)) xAlpha -= 2;
     if (GetKeyState(VK_LEFT) < 0) zAlpha += 2;
     if (GetKeyState(VK_RIGHT) < 0) zAlpha -= 2;
     float angle = -zAlpha / 180 * M_PI;
     float speed = 0;
     if (GetKeyState('W') < 0) speed = 0.3;
     if (GetKeyState('S') < 0) speed = -0.3;
     if (GetKeyState('A') < 0) { speed = 0.3; angle -= M_PI * 0.5; };
     if (GetKeyState('D') < 0) { speed = 0.3; angle += M_PI * 0.5; };
     if (speed != 0) {
        pos.x += sin(angle) * speed;
        pos.y += cos(angle) * speed;
     }
     glRotatef(-xAlpha, 1.0f, 0.0f, 0.0f);
     glRotatef(-zAlpha, 0.0f, 0.0f, 1.0f);
     glTranslatef(-pos.x, -pos.y, -3.0f);
}
    
int main(void)
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(1920, 1080, "Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        return -1;
    glFrustum(-1, 1, -1, 1, 2, 100);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.75f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPushMatrix();
        MoveCamera();
        ShowWorld();
        glPopMatrix();
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glfwTerminate();
    return 0;
}
