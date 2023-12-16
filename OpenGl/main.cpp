#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <cmath>
float vertPol[] = { 1, 1, 0,   1, -1, 0,   -1, -1, 0,    -1, 1, 0 };
float xAlpha = 100;
float zAlpha = 0;
bool isJumping = false; 
float jumpHeight = 2.0f; 
POINTFLOAT pos = { 0, 0 }; //положение игрока на карте 

void ShowWorld() {
    glEnableClientState(GL_VERTEX_ARRAY); //разрешаем рисовать из массива вершин 
    glVertexPointer(3, GL_FLOAT, 0, &vertPol); //указ OpenGl на массив 
    for (int i = -50; i <= 50; i++)
        for (int j = -50; j <= 50; j++) {
            glPushMatrix();
            if ((i + j) % 2 == 0) {
                glColor3f(0.0f, 0.0f, 1.0f); //текущий свет ячек пола 
            }
            else
            {
                glColor3f(0.0f, 0.0f, 0.5f); //текущий свет ячек пола 
            }
            glTranslatef(i * 2, j * 2, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4); //рисовка 
            glPopMatrix();
        }
    glDisableClientState(GL_VERTEX_ARRAY);
}

void MoveCamera() {
     if ((GetKeyState(VK_UP) < 0) && (xAlpha < 180)) xAlpha += 2;
     if ((GetKeyState(VK_DOWN) < 0) && (xAlpha > 0)) xAlpha -= 2;
     if (GetKeyState(VK_LEFT) < 0) zAlpha += 2;
     if (GetKeyState(VK_RIGHT) < 0) zAlpha -= 2;
     float ugol = -zAlpha / 180 * M_PI;
     float speed = 0;
     if (GetKeyState('W') < 0) speed = 0.2;
     if (GetKeyState('S') < 0) speed = -0.2;
     if (GetKeyState('A') < 0) {
         speed = 0.2; 
         ugol -= M_PI * 0.5;
     };
     if (GetKeyState('D') < 0) { 
         speed = 0.2; 
         ugol += M_PI * 0.5; 
     };
     if (speed != 0) {
        pos.x += sin(ugol) * speed;
        pos.y += cos(ugol) * speed;
     }
     glRotatef(-xAlpha, 1.0f, 0.0f, 0.0f);
     glRotatef(-zAlpha, 0.0f, 0.0f, 1.0f);
     glTranslatef(-pos.x, -pos.y, -2.0f); // отодвигаение камеры на -3 по оси z 
}
void position(float x, float y, float z, float rx, float ry, float rz, float sx,float sy, float sz) {
    glTranslatef(x, y, z);
    glRotatef(rx, 1.0f, 0.0f, 0.0f);
    glRotatef(ry, 0.0f, 1.0f, 0.0f);
    glRotatef(rz, 0.0f, 0.0f, 1.0f);
    glScalef(sx, sy, sz);
}
void draw_square(float x, float y, float z, float rx, float ry, float rz, float sx,float sy, float r, float g, float b) {

    float points[] = {
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    };
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &points);
    glPushMatrix();
    position(x, y, z, rx, ry, rz, sx, sy, 1.0f);
    glColor3f(r, g, b);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
}
void draw_cube(float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz, float r, float g, float b) {
    glPushMatrix();
    position(x, y, z, rx, ry, rz, sx, sy, sz);
    draw_square(0.0f, 0.0f, -0.5f, 0, 0, 0, 1.0f, 1.0f, r, g, b);
    draw_square(0.0f, 0.0f, 0.5f, 0, 0, 0, 1.0f, 1.0f, r, g, b);
    draw_square(0.0f, -0.5f, 0.0f, 90, 0, 0, 1.0f, 1.0f, r, g, b);
    draw_square(0.0f, 0.5f, 0.0f, 90, 0, 0, 1.0f, 1.0f, r, g, b);
    draw_square(-0.5f, 0.0f, 0.0f, 0, 90, 0, 1.0f, 1.0f, r, g, b);
    draw_square(0.5f, 0.0f, 0.0f, 0, 90, 0, 1.0f, 1.0f, r, g, b);
    glPopMatrix();
}
void draw_column(float x, float y, float z, float rx, float ry, float rz, float sx,float sz, int N, float r, float g, float b) {
    glPushMatrix();
    position(x, y, z, rx, ry, rz, sx, sx, sz);
    float a = 2 * tan(M_PI / N);
    for (int i = 0; i < N; i++) {
        float grad = 360.0f / N * i;
        float rad = grad * M_PI / 180.0f;
        float xx = -sin(rad);
        float yy = cos(rad);
        draw_square(xx, yy, 0.0f, 90.0f, grad, 0.0f, a, 1.0f, r, g, b);
    }
    glPopMatrix();
}
void draw_Parthenon(float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz) {
    /*glPushMatrix();*/
    position(x, y, z, rx, ry, rz, sx, sy, sz);
    draw_cube(0.0f, 0.0f, 0.26f, 0.0f, 0.0f, 0.0f, 40.95f, 25.2f, 0.5f, 0.98f,0.92f, 0.84f);
    draw_cube(0.0f, 0.0f, 0.5825f, 0.0f, 0.0f, 0.0f, 28.35f, 12.6f, 0.125f, 0.63f,0.15f, 0.11f);
    for (int i = 0; i < 12; i++) {
        draw_column(-17.325f + i * 3.15, -9.45f, 8.2f, 0.0f, 0.0f, 0.0f, 0.7f,15.0f, 32, 0.98f, 0.92f, 0.84f);
    }
    for (int i = 0; i < 7; i++) {
        draw_column(-17.325f, -9.45f + i * 3.15, 8.2f, 0.0f, 0.0f, 0.0f, 0.7f,15.0f, 32, 0.98f, 0.92f, 0.84f);
    }
    for (int i = 0; i < 12; i++) {
        draw_column(17.325f - i * 3.15, 9.45f, 8.2f, 0.0f, 0.0f, 0.0f, 0.7f, 15.0f,32, 0.98f, 0.92f, 0.84f);
    }
    for (int i = 0; i < 7; i++) {
        if (i != 3)
            draw_column(17.325f, 9.45f - i * 3.15, 8.2f, 0.0f, 0.0f, 0.0f, 0.7f,15.0f, 32, 0.98f, 0.92f, 0.84f);
    }
    draw_cube(0.0f, 0.0f, 16.05f, 0.0f, 0.0f, 0.0f, 40.95f, 25.2f, 0.5f, 0.98f,0.92f, 0.84f);
    draw_cube(0.0f, -6.3f, 16.05f + 6.3f / sqrt(3.0f), 30.0f, 0.0f, 0.0f, 40.95f,25.2f / sqrt(3.0f), 0.5f, 0.98f, 0.92f, 0.84f);
    draw_cube(0.0f, 6.3f, 16.05f + 6.3f / sqrt(3.0f), -30.0f, 0.0f, 0.0f, 40.95f,25.2f / sqrt(3.0f), 0.5f, 0.98f, 0.92f, 0.84f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
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
        draw_Parthenon(0.0f, 25.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.7f, 0.7f);
        glPopMatrix();
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glfwTerminate();
    return 0;
}
