#include <GL/glut.h>

GLsizei winWidth = 600, winHeight = 500;

GLubyte label[36] = {'J', 'a', 'n', 'F', 'e', 'b', 'M', 'a', 'r',
                     'A', 'p', 'r', 'M', 'a', 'y', 'J', 'u', 'n',
                     'J', 'u', 'l', 'A', 'u', 'g', 'S', 'e', 'p',
                     'O', 'c', 't', 'N', 'o', 'v', 'D', 'e', 'c'};

GLint  dataValue[12] = {420, 342, 324, 310, 262, 185, 190, 196, 217, 240, 312, 438};

void initWindow(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("P105");
}

void initBackground() {
    glClearColor(1, 1, 1, 0);
    glMatrixMode(GL_PROJECTION);
    //glOrtho(0, winWidth, 0, winHeight, -1, 1);
    gluOrtho2D(0, winWidth, 0, winHeight);
}

void lineGraph() {
    GLint xRaster = 25, yRaster = 150;

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3d(0, 0, 1);

    glBegin(GL_LINE_STRIP);
    for (GLint k = 0; k < 12; k ++) {
        glVertex2i(xRaster + k * 50, dataValue[k]);
    }
    glEnd();

    glColor3d(1, 0, 0);
    for (GLint k = 0; k < 12; k ++) {
        glRasterPos2i(xRaster + k * 50, dataValue[k] - 4);
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '*');
    }

    glColor3d(0, 0, 0);
    xRaster = 20;
    for (GLint month = 0; month < 12; month ++) {
        glRasterPos2i(xRaster, yRaster);
        for (GLint k = 3 * month; k < 3 * month + 3; k ++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, label[k]);
        }
        xRaster += 50;
    }
    glFlush();
}

void barChart() {
    GLint xRaster = 20, yRaster = 150;

    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1, 0, 0);
    for (GLint k = 0; k < 12; k ++) {
        glRecti(20 + k * 50, 165, 40 + k * 50, dataValue[k]);
    }

    glColor3d(0, 0, 0);
    for (GLint month = 0; month < 12; month ++) {
        glRasterPos2i(xRaster, yRaster);
        for (GLint k = 3 * month; k < 3 * month + 3; k ++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, label[k]);
        }
        xRaster += 50;
    }
    glFlush();
}

void winReshapeFcn(GLint newWidth, GLint newHeight) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, (GLdouble)newWidth, 0, (GLdouble)newHeight, -1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char* argv[]) {
    initWindow(argc, argv);
    initBackground();
    //glutDisplayFunc(lineGraph);
    glutDisplayFunc(barChart);
    glutReshapeFunc(winReshapeFcn);
    glutMainLoop();
    return 0;
}
