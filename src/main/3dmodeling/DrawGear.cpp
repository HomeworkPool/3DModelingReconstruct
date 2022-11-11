#include <GL/freeglut.h>
#include <stdlib.h>
#include <math.h>

const double PI = 3.14159265;

GLfloat g_rotx = 20.0, g_roty = 30.0, g_rotz = 0.0, g_angle = 0.0;
GLfloat g_modelPos[3] = {0, 0, -20};
GLfloat g_lightPos[4] = {15.0, 15.0, 10.0, 0.0};
GLfloat g_color1[4] = {0.8, 0.1, 0.0, 1.0};
GLfloat g_color2[4] = {0.0, 0.8, 0.2, 1.0};
GLfloat g_color3[4] = {0.2, 0.2, 1.0, 1.0};

bool g_bStartAnim = true;

static void resize(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat) w / (GLfloat) h, 0.2, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init(void) {
    glLightfv(GL_LIGHT0, GL_POSITION, g_lightPos);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}


/*Input:  inner_radius - radius of hole at center
          outer_radius - radius at center of teeth
          width - width of gear
          teeth - number of teeth
          tooth_depth_offset - depth of tooth */
void DrawGear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width, GLint teeth, GLfloat tooth_depth_offset,
              GLfloat tooth_depth = 10.0, GLfloat offset_z = 0.0) {
    GLint i;
    GLfloat r0, r1, r2;
    GLfloat angle, da;
    GLfloat u, v, len;

    r0 = inner_radius;
    r1 = outer_radius - tooth_depth_offset / 2.0;
    r2 = outer_radius + tooth_depth_offset / 2.0;

    da = 2.0 * PI / teeth / 4.0;

    glShadeModel(GL_FLAT);
    glNormal3f(0.0, 0.0, 1.0);
    //Draw front face
    glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= teeth; i++) {
        angle = i * 2.0 * PI / teeth;
        glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5 + offset_z);
        glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5 + offset_z);
        glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5 + offset_z);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5 + offset_z);
    }
    glEnd();

    //Draw front sides of teeth
    glBegin(GL_QUADS);
    da = 2.0 * PI / teeth / 4.0;
    for (i = 0; i < teeth; i++) {
        angle = i * 2.0 * PI / teeth;
        glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
        glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
        glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
    }
    glEnd();

    //Draw back face
    glBegin(GL_TRIANGLE_STRIP);
    for (i = 0; i <= teeth; i++) {
        angle = i * 2.0 * PI / teeth;

        glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0);
        glNormal3f(0, 0, -1);
        glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0);
        glNormal3f(0, 0, -1);
        glVertex3f(r0 * cos(angle + 3 * da), r0 * sin(angle + 3 * da), width * 0);
        glNormal3f(0, 0, -1);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0);
        glNormal3f(0, 0, -1);
    }
    glEnd();

    //Draw back sides of teeth
    glBegin(GL_QUADS);
    da = 2.0 * PI / teeth / 4.0;
    for (i = 0; i < teeth; i++) {
        angle = i * 2.0 * PI / teeth;
        glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0);
        glNormal3f(0, 0, -1);
        glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0);
        glNormal3f(0, 0, -1);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0);
        glNormal3f(0, 0, -1);
        glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0);
        glNormal3f(0, 0, -1);
    }
    glEnd();


    glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= teeth; i++) {
        angle = i * 2.0 * PI / teeth;
        glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0);

        glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);

        glVertex3f(r0 * cos(angle + 3 * da), r0 * sin(angle + 3 * da), width * 0);

        glVertex3f(r0 * cos(angle + 3 * da), r0 * sin(angle + 3 * da), width * 0.5);

    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    for (i = 0; i <= teeth; i++) {
        angle = i * 2.0 * PI / teeth;
        glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
        glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0);

    }
    glEnd();

    glBegin(GL_QUADS);
    for (i = 0; i < teeth; i++) {
        angle = i * 2.0 * PI / teeth;
        glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
        glNormal3f(1, 0, 0);
        glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
        glNormal3f(1, 0, 0);
        glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0);
        glNormal3f(1, 0, 0);
        glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0);
        glNormal3f(1, 0, 0);
    }
    glEnd();

    glBegin(GL_QUADS);
    for (i = 0; i < teeth; i++) {
        angle = i * 2.0 * PI / teeth;
        glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
        glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
    }
    glEnd();

    glBegin(GL_QUADS);
    for (i = 0; i < teeth; i++) {
        angle = i * 2.0 * PI / teeth;

        glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
        glVertex3f(r2 * cos(angle + 1 * da), r2 * sin(angle + 1 * da), width * 0.5);
        glVertex3f(r2 * cos(angle + 1 * da), r2 * sin(angle + 1 * da), width * 0);
        glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0);
    }
    glEnd();
}

static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (g_bStartAnim)
        g_angle = 30 * glutGet(GLUT_ELAPSED_TIME) / 1000.0;

    glLoadIdentity();
    glTranslatef(g_modelPos[0], g_modelPos[1], g_modelPos[2]);

    glPushMatrix();
    glRotatef(g_rotx, 1.0, 0.0, 0.0);
    glRotatef(g_roty, 0.0, 1.0, 0.0);
    glRotatef(g_rotz, 0.0, 0.0, 1.0);

    glPushMatrix();
    glTranslatef(-3.0, -2.0, 0.0);
    glRotatef(g_angle, 0.0, 0.0, 1.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, g_color1);
    DrawGear(1.0, 4.0, 1.0, 20, 0.7);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 3.5, 0.0);
    glRotatef(g_angle, 0.0, 0.0, 3.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, g_color2);
    DrawGear(0.2, 2.0, 0.5, 20, 0.35);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-5.0, 3.9, 0.5);
    glRotatef(g_angle, 0.0, 0.0, -2.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, g_color3);
    DrawGear(1.65, 3.0, 0.75, 20, 0.6);
    glPopMatrix();

    glPopMatrix();
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y) {
    switch (key) {
        case 27 :
            exit(0);
            break;
        case 'w':
            g_rotx += 5;
            break;
        case 's':
            g_rotx -= 5;
            break;
        case 'a':
            g_roty += 5;
            break;
        case 'd':
            g_roty -= 5;
            break;
        case 'q':
            g_rotz += 5;
            break;
        case 'e':
            g_rotz -= 5;
            break;
        case 'z':
            g_modelPos[2] += 1;
            break;
        case 'x':
            g_modelPos[2] -= 1;
            break;
        case ' ':
            g_bStartAnim = !g_bStartAnim;
            break;
    }
    glutPostRedisplay();
}

static void idle(void) {
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1, 1, 1, 1);

    init();

    glutMainLoop();

    return 0;
}
