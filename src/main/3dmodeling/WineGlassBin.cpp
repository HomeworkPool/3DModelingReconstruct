#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

float g_angleX = 0, g_angleY = 0;
float g_modelPos[] = {0, -6, -12.0};
float g_lightPos[] = {0, 30.0, 30.0, 0.0};
int g_nNumPts = 0;
double **g_ctrlPts = NULL;
double g_r = 0.1;
int g_nSelectedIdx = -1;
int g_winHeight = 768;
float g_originalClr[] = {0.1, 0.9, 0.1, 1.0};
float g_selectedClr[] = {0.9, 0.1, 0.1, 1.0};
char g_strFileName[] = "wineGlass.bmp";
bool g_bShowCtrlPoints = true;
int g_subdivMax = 10;

#pragma pack(1)
struct FILEHEADER {
    char dummy1[18];
    int width;+
    int height;
    char dummy2[28];
};

struct BGR {
    unsigned char b; // 蓝
    unsigned char g; // 绿
    unsigned char r; // 红
    BGR(unsigned char _b = 0, unsigned char _g = 0, unsigned char _r = 0) {
        b = _b;
        g = _g;
        r = _r;
    }

    bool operator==(BGR x) {
        if (b == x.b && g == x.g && r == x.r)
            return true;
        else
            return false;
    }
};

int bmpFileLoader(char *fileName) {
    FILEHEADER imgHead;
    BGR pixel;
    fstream imgFile;
    vector<double> vx;
    vector<double> vy;
    double ox;
    double oy;
    double ymax = 0;

    imgFile.open(fileName, ios::in | ios::binary);

    if (!imgFile) {
        cerr << "File opening or creating error!" << endl;
        return 0;
    }

    imgFile.read((char *) (&imgHead), sizeof(FILEHEADER));
    cout << imgHead.width << "," << imgHead.height << endl;
    if (imgHead.width % 4 != 0) return 0;

    for (int i = 0; i < (imgHead.width * imgHead.height); i++) {
        imgFile.read((char *) (&pixel), sizeof(BGR));
        if (pixel == BGR(0, 0, 255)) {
            vx.push_back(i % imgHead.width);
            vy.push_back(i / imgHead.width);
        }
        if (pixel == BGR(0, 255, 0)) {
            ox = i % imgHead.width;
            oy = i / imgHead.width;
        }
    }
    imgFile.close();

    for (unsigned int i = 0; i < vx.size(); i++) {
        vx[i] = vx[i] - ox;
        vy[i] = vy[i] - oy;
        if (vy[i] > ymax) ymax = vy[i];
    }

    g_nNumPts = vx.size();
    g_ctrlPts = new double *[g_nNumPts];
    for (int i = 0; i < g_nNumPts; i++)
        g_ctrlPts[i] = new double[3];
    for (int i = 0; i < g_nNumPts; i++) {
        vx[i] = 12 * vx[i] / ymax;
        vy[i] = 12 * vy[i] / ymax;
        g_ctrlPts[i][0] = vx[i];
        g_ctrlPts[i][1] = vy[i];
        g_ctrlPts[i][2] = 0;
        cout << vx[i] << '\t' << vy[i] << endl;
    }

    return 1;
}

static void resize(int w, int h) {
    g_winHeight = h;
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) w / (GLfloat) h, 0.2, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void getIntersectPos(int x, int y, double v[3]) {
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = x;
    winY = y;

    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &v[0], &v[1], &v[2]);
}

bool isInSphere(double v[3], double vc[3], double r) {
    double dis2, r2 = r * r;

    dis2 = pow((vc[0] - v[0]), 2.0) + pow((vc[1] - v[1]), 2.0) + pow((vc[2] - v[2]), 2.0);
    if (fabs(dis2 - r2) < 0.01)
        return true;
    else
        return false;
}

void drawSphere(double r, int slices, int stacks) {
    static int sphereList = glGenLists(1);
    static bool bFirstTime = true;
    if (bFirstTime) {
        glNewList(sphereList, GL_COMPILE);
        glutSolidSphere(r, slices, stacks);
        glEndList();
        bFirstTime = false;
    } else
        glCallList(sphereList);
}

void drawControlPoints() {
    int i;
    glEnable(GL_LIGHTING);
    for (i = 0; i < g_nNumPts; i++) {
        if (i == g_nSelectedIdx)
            glMaterialfv(GL_FRONT, GL_DIFFUSE, g_selectedClr);
        else
            glMaterialfv(GL_FRONT, GL_DIFFUSE, g_originalClr);

        glPushMatrix();
        glTranslatef(g_ctrlPts[i][0], g_ctrlPts[i][1], g_ctrlPts[i][2]);
        drawSphere(g_r, 16, 16);
        glPopMatrix();
    }
}

//done
//Your task is here:
//Modify the code below from linear interpolation to cubic interpolation
void cubicInterpolate(double p[4][3], double t, double pnew[3]) {
    int i;
    for (i = 0; i < 3; i++) {
        pnew[i] = p[1][i] + 0.5 * t * (p[2][i] - p[0][i] +
                                       t * (2.0 * p[0][i] - 5.0 * p[1][i] + 4.0 * p[2][i] - p[3][i] +
                                            t * (3.0 * (p[1][i] - p[2][i]) + p[3][i] - p[0][i])));
    }

}

void getCubicPos(int idx, double t, double pnew[3]) {
    int i;
    double p[4][3];

    if (idx < 0 || idx >= (g_nNumPts - 1) || t < 0 || t > 1.0) return;

    for (i = 0; i < 4; i++) {
        if ((idx + i - 1) < 0) {
            p[0][0] = g_ctrlPts[0][0];
            p[0][1] = g_ctrlPts[0][1];
            p[0][2] = g_ctrlPts[0][2];
        } else if ((idx + i - 1) > (g_nNumPts - 1)) {
            p[3][0] = g_ctrlPts[g_nNumPts - 1][0];
            p[3][1] = g_ctrlPts[g_nNumPts - 1][1];
            p[3][2] = g_ctrlPts[g_nNumPts - 1][2];
        } else {
            p[i][0] = g_ctrlPts[idx + i - 1][0];
            p[i][1] = g_ctrlPts[idx + i - 1][1];
            p[i][2] = g_ctrlPts[idx + i - 1][2];
        }
    }
    cubicInterpolate(p, t, pnew);
}

void init(void) {
    GLfloat model_specular[] = {1.0, 0.95, 0.9, 1.0};
    GLfloat model_shininess[] = {64};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, g_originalClr);
    glMaterialfv(GL_FRONT, GL_SPECULAR, model_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, model_shininess);

}

static void display(void) {
    int i, j;
    double pnew[3];

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glDisable(GL_LIGHTING);

    glTranslatef(g_modelPos[0], g_modelPos[1], g_modelPos[2]);
    glRotatef(g_angleX, 1, 0, 0);
    glRotatef(g_angleY, 0, 1, 0);

    glLineWidth(3.0);
    glBegin(GL_LINE_STRIP);
    for (i = 0; i < g_nNumPts; i++)
        for (j = 0; j < g_subdivMax; j++) {
            getCubicPos(i, j / (double) g_subdivMax, pnew);
            glVertex3dv(pnew);
        }
    glVertex3dv(g_ctrlPts[g_nNumPts - 1]);
    glEnd();
    glLineWidth(1.0);

    if (g_bShowCtrlPoints)
        drawControlPoints();
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y) {
    switch (key) {
        case 27 :
            exit(0);
            break;
        case ' ':
            g_bShowCtrlPoints = !g_bShowCtrlPoints;
            break;
        case 'w':
            g_angleX += 1.0;
            break;
        case 's':
            g_angleX -= 1.0;
            break;
        case 'a':
            g_angleY += 1.0;
            break;
        case 'd':
            g_angleY -= 1.0;
            break;
        case 'z':
            g_modelPos[2] += 0.1;
            break;
        case 'x':
            g_modelPos[2] -= 0.1;
            break;
        case 'u':
            if (g_nSelectedIdx != -1) {
                g_ctrlPts[g_nSelectedIdx][0] -= 0.1;
                glMap1d(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &g_ctrlPts[0][0]);
            }
            break;
        case 'i':
            if (g_nSelectedIdx != -1) {
                g_ctrlPts[g_nSelectedIdx][0] += 0.1;
                glMap1d(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &g_ctrlPts[0][0]);
            }
            break;
        case 'j':
            if (g_nSelectedIdx != -1) {
                g_ctrlPts[g_nSelectedIdx][1] += 0.1;
                glMap1d(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &g_ctrlPts[0][0]);
            }
            break;
        case 'k':
            if (g_nSelectedIdx != -1) {
                g_ctrlPts[g_nSelectedIdx][1] -= 0.1;
                glMap1d(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &g_ctrlPts[0][0]);
            }
            break;
        case 'n':
            if (g_nSelectedIdx != -1) {
                g_ctrlPts[g_nSelectedIdx][2] += 0.1;
                glMap1d(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &g_ctrlPts[0][0]);
            }
            break;
        case 'm':
            if (g_nSelectedIdx != -1) {
                g_ctrlPts[g_nSelectedIdx][2] -= 0.1;
                glMap1d(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &g_ctrlPts[0][0]);
            }
            break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    double pos[3];
    int i;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        y = g_winHeight - y;
        g_nSelectedIdx = -1;
        getIntersectPos(x, y, pos);
        for (i = 0; i < g_nNumPts; i++) {
            if (isInSphere(pos, g_ctrlPts[i], g_r))
                g_nSelectedIdx = i;
        }
    }
}

static void idle(void) {
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Cubic Spline Model");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);

    glClearColor(0.2, 0.4, 1.0, 1);
    glLightfv(GL_LIGHT0, GL_POSITION, g_lightPos);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);

    init();
    bmpFileLoader(g_strFileName);

    glutMainLoop();

    return 0;
}
