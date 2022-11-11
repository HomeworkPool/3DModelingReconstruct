#include <GL/glut.h>
#include <cstdio>
#include <cmath>

typedef struct VERTEX_3D {
    float x;
    float y;
    float z;
} VERTEX3D;

typedef struct _TRIANGLE {
    int A;//Index of vertex 0
    int B;//Index of vertex 1
    int C;//Index of vertex 2
} TRIANGLE;

VERTEX3D *g_vet;
VERTEX3D *g_norm;
VERTEX3D *g_color;

float g_rotx = 20.0f, g_roty = 30.0f, g_rotz = 0.0f;
float g_modelPos[3] = {0, 0, -15.0f};
float g_scale = 10.0f;

const float g_lightPos[] = {1.0f, 1.0f, 1.0f, 0.0f};

char g_strModelName[512] = "plant.ply"; /*Problem happens when left the [512] being empty []*/
int g_nVerticesNumber;

void GetMinMax(VERTEX3D &vMin, VERTEX3D &vMax, VERTEX3D v) {
    if (v.x < vMin.x) vMin.x = v.x;
    if (v.y < vMin.y) vMin.y = v.y;
    if (v.z < vMin.z) vMin.z = v.z;
    if (v.x > vMax.x) vMax.x = v.x;
    if (v.y > vMax.y) vMax.y = v.y;
    if (v.z > vMax.z) vMax.z = v.z;
}

void FormatGeometry(VERTEX3D &v, VERTEX3D vMin, VERTEX3D vMax, float scale) {
    v.x = scale * (v.x - 0.5 * (vMin.x + vMax.x));
    v.y = scale * (v.y - 0.5 * (vMin.y + vMax.y));
    v.z = scale * (v.z - 0.5 * (vMin.z + vMax.z));
}

void LoadMeshPLY(char *FileName) {
    int i, j;
    VERTEX3D vMin, vMax;
    char strBuff[512];
    char type[512] = "";
    bool bStartPropertyCount = true;
    float alpha;

    vMin.x = vMin.y = vMin.z = 99999.0;
    vMax.x = vMax.y = vMax.z = -99999.0;

    FILE *fp = fopen(FileName, "r");

    if (fp == NULL) {
        printf("ERROR: unable to open model [%s]!\n", FileName);
        exit(0);
    }

    fgets(strBuff, 256, fp);
    fgets(strBuff, 256, fp);
    fgets(strBuff, 256, fp);
    fscanf(fp, "element vertex %d\n", &g_nVerticesNumber);
    printf("Number of Vertices: %d\n", g_nVerticesNumber);
    j = 0;
    while (strcmp(type, "element") != 0) {
        fgets(strBuff, 256, fp);
        i = 0;
        while (strBuff[i] != '\0' && strBuff[i] != ' ' && strBuff[i] != '\n') {
            type[i] = strBuff[i];
            i++;
        }
        if (bStartPropertyCount && strcmp(type, "property") == 0)
            j++;
        else
            bStartPropertyCount = false;
        type[i] = '\0';
    }
    fgets(strBuff, 256, fp);
    fgets(strBuff, 256, fp);
    fgets(strBuff, 256, fp);

    //Allocate the triangle array
    g_vet = new VERTEX3D[g_nVerticesNumber];
    g_norm = new VERTEX3D[g_nVerticesNumber];
    g_color = new VERTEX3D[g_nVerticesNumber];

    //Read in the triangles
    for (i = 0; i < g_nVerticesNumber; i++) {
        if (j == 3)
            fscanf(fp, "%f %f %f", &g_vet[i].x, &g_vet[i].y, &g_vet[i].z);
        else if (j == 6)
            fscanf(fp, "%f %f %f %f %f %f", &g_vet[i].x, &g_vet[i].y, &g_vet[i].z, &g_norm[i].x, &g_norm[i].y,
                   &g_norm[i].z);
        else if (j == 7)
            fscanf(fp, "%f %f %f %f %f %f %f", &g_vet[i].x, &g_vet[i].y, &g_vet[i].z, &g_color[i].x, &g_color[i].y,
                   &g_color[i].z, &alpha);
        else if (j == 10)
            fscanf(fp, "%f %f %f %f %f %f %f %f %f %f", &g_vet[i].x, &g_vet[i].y, &g_vet[i].z,
                   &g_norm[i].x, &g_norm[i].y, &g_norm[i].z,
                   &g_color[i].x, &g_color[i].y, &g_color[i].z, &alpha);
        else {
            printf("Warning: the viewer cann't read the colors of models\n");
            exit(1);
        }
        GetMinMax(vMin, vMax, g_vet[i]);
    }
/*
    for (i=0; i<g_nVerticesNumber; i++)
    {
        g_color[i].x = (g_vet[i].x - vMin.x)/(vMax.x - vMin.x);
        g_color[i].y = (g_vet[i].y - vMin.y)/(vMax.y - vMin.y);
        g_color[i].z = (g_vet[i].z- vMin.z)/(vMax.z - vMin.z);
    }*/

    if ((vMax.x - vMin.x) > (vMax.y - vMin.y)) {
        if ((vMax.x - vMin.x) > (vMax.z - vMin.z))
            g_scale = g_scale / (vMax.x - vMin.x);
        else
            g_scale = g_scale / (vMax.z - vMin.z);
    } else {
        if ((vMax.y - vMin.y) > (vMax.z - vMin.z))
            g_scale = g_scale / (vMax.y - vMin.y);
        else
            g_scale = g_scale / (vMax.z - vMin.z);
    }

    for (i = 0; i < g_nVerticesNumber; i++)
        FormatGeometry(g_vet[i], vMin, vMax, g_scale);

    fclose(fp);
}

void DrawVertices() {
    int i;

    glEnable(GL_LIGHTING);
    glPointSize(2.0);
    glBegin(GL_POINTS);
    for (i = 0; i < g_nVerticesNumber; i++) {
        //glColor3f(g_color[i].x, g_color[i].y, g_color[i].z);
        glNormal3f(g_norm[i].x, g_norm[i].y, g_norm[i].z);
        glVertex3f(g_vet[i].x, g_vet[i].y, g_vet[i].z);
    }
    glEnd();
}

static void resize(int width, int height) {
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, ar, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glTranslatef(g_modelPos[0], g_modelPos[1], g_modelPos[2]);
    glRotatef(g_rotx, 1.0, 0.0, 0.0);
    glRotatef(g_roty, 0.0, 1.0, 0.0);
    glRotatef(g_rotz, 0.0, 0.0, 1.0);

    DrawVertices();

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
    }

    glutPostRedisplay();
}

static void idle(void) {
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    printf("Tips: use 'w', 's', 'a', 'd', 'z', 'x', 'q', 'e' keys to control the model\n");
    if (argc < 2) {
        printf("Input the file name (without postfix e.g. .ply) of points:");
        scanf("%s", g_strModelName);
        strcat(g_strModelName, ".ply");
    } else {
        printf("%s\n", argv[1]);
        strcpy(g_strModelName, argv[1]);
    }

    glutCreateWindow("PLY Model Viewer");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(0.2, 0.55, 1.0, 1);
    glDisable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_POSITION, g_lightPos);

    glShadeModel(GL_SMOOTH);

    printf("Loading \"%s\"...\n", g_strModelName);
    LoadMeshPLY(g_strModelName);

    glutMainLoop();

    return EXIT_SUCCESS;
}
