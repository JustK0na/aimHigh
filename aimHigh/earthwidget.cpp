#include "earthwidget.h"
#include <cmath>

earthWidget::earthWidget(QWidget *parent) : QOpenGLWidget(parent){}

void earthWidget::drawSphere(int lats, int longs)
{
    float radius = 1.0f;

    GLfloat mat_ambient[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    GLfloat mat_diffuse[] = { 0.2f, 0.6f, 1.0f, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    for (int i = 0; i <= lats; i++) {
        float lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
        float z0 = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = M_PI * (-0.5 + (double)i / lats);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= longs; j++) {
            float lng = 2 * M_PI * (double)(j - 1) / longs;
            float x = cos(lng);
            float y = sin(lng);

            glColor3f(0.2f, 0.6f, 1.0f); // Earth blue
            glVertex3f(x * zr0 * radius, y * zr0 * radius, z0 * radius);
            glVertex3f(x * zr1 * radius, y * zr1 * radius, z1 * radius);
        }
        glEnd();
    }
}

void earthWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);


    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
}

void earthWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = static_cast<float>(w) / static_cast<float>(h ? h : 1);
    glFrustum(-aspect, aspect, -1, 1, 1.5, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

void earthWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);

    drawSphere(40, 40); // Lats and longs for smoother sphere
}
