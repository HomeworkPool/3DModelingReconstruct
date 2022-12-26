#include <GL/glut.h>
#include <cstdio>
#include <cmath>

//声明结构体用于处理三维点云数据
typedef struct VERTEX_3D {
    float x;
    float y;
    float z;
} VERTEX3D;

//三角
typedef struct _TRIANGLE {
    int A;//Index of vertex 0
    int B;//Index of vertex 1
    int C;//Index of vertex 2
} TRIANGLE;

VERTEX3D *g_vet;
VERTEX3D *g_norm;
VERTEX3D *g_color;


TRIANGLE *triPatch;


GLfloat light_ambient[] = {.6, .6, .6, 1.0};

float g_rotx = 20.0f, g_roty = 30.0f, g_rotz = 0.0f;
float g_modelPos[3] = {0, 0, -15.0f};
float g_scale = 10.0f;

const float g_lightPos[] = {1.0f, 1.0f, 1.0f, 1.0f};

char g_strModelName[512] = "./buddha/poisson-mesh-5-10-0.05.ply"; /*Problem happens when left the [512] being empty []*/
int g_nVerticesNumber;//顶点数
int g_nFacesNumber;//面数

//归一化
void Normalize(VERTEX3D &v) {
    double length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

    if (length != 0) {
        v.x = v.x / length;
        v.y = v.y / length;
        v.z = v.z / length;
    }
}

//获取最小最大值
void GetMinMax(VERTEX3D &vMin, VERTEX3D &vMax, VERTEX3D v) {
    if (v.x < vMin.x) vMin.x = v.x;
    if (v.y < vMin.y) vMin.y = v.y;
    if (v.z < vMin.z) vMin.z = v.z;
    if (v.x > vMax.x) vMax.x = v.x;
    if (v.y > vMax.y) vMax.y = v.y;
    if (v.z > vMax.z) vMax.z = v.z;
}

//格式化几何图形
void FormatGeometry(VERTEX3D &v, VERTEX3D vMin, VERTEX3D vMax, float scale) {
    v.x = scale * (v.x - 0.5 * (vMin.x + vMax.x));
    v.y = scale * (v.y - 0.5 * (vMin.y + vMax.y));
    v.z = scale * (v.z - 0.5 * (vMin.z + vMax.z));
}

//加载网格层
void LoadMeshPLY(char *FileName) {
    int i, j;
    VERTEX3D vMin, vMax;
    char strBuff[512];
    char type[512] = "";
    bool bStartPropertyCount = true;//开始属性计数
    float alpha;

    vMin.x = vMin.y = vMin.z = 99999.0;
    vMax.x = vMax.y = vMax.z = -99999.0;

    FILE *fp = fopen(FileName, "r");

    if (fp == NULL)//文件不存在
    {
        printf("ERROR: unable to open model [%s]!\n", FileName);
        exit(0);
    }
    //读文件
    fgets(strBuff, 256, fp);
    fgets(strBuff, 256, fp);
    fgets(strBuff, 256, fp);
    fscanf(fp, "element vertex %d\n", &g_nVerticesNumber);
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
    sscanf(strBuff, "element face %d\n", &g_nFacesNumber);
    printf("Number of Triangles: %d\n", g_nFacesNumber);

    fgets(strBuff, 256, fp);
    fgets(strBuff, 256, fp);
    //fgets(strBuff, 256, fp);多读的一点

    //Allocate the triangle array
    g_vet = new VERTEX3D[g_nVerticesNumber];
    g_norm = new VERTEX3D[g_nVerticesNumber];
    g_color = new VERTEX3D[g_nVerticesNumber];

    triPatch = new TRIANGLE[g_nFacesNumber];

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
            printf("Warning: the viewer can't read the colors of models\n");
            exit(1);
        }
        GetMinMax(vMin, vMax, g_vet[i]);
        Normalize(g_norm[i]);
    }

    int tmp;
    for (i = 0; i < g_nFacesNumber; i++) {
        fscanf(fp, "%d %d %d %d", &tmp, &triPatch[i].A, &triPatch[i].B, &triPatch[i].C);
    }

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

//绘图顶点
void DrawVertices() {
    int i;

    glEnable(GL_LIGHTING);
    glPointSize(2.0);
    glBegin(GL_TRIANGLES);
    for (i = 0; i < g_nFacesNumber; i++) {
        glBegin(GL_TRIANGLES);
//glBegin();创建元素的类型，比如：点，线等。
//GL_POINTS：把每个顶点作为一个点进行处理，顶点n定义了点n，绘制N个点。
//GL_LINES：   把每个顶点作为一个独立的线段，顶点2n-1和2n之间定义了n条线段，绘制N/2条线段
//GL_LINE_STRIP：绘制从第一个顶点到最后一个顶点依次相连的一组线段，第n和n+1个顶点定义了线段n，绘制n-1条线段。
//GL_LINE_LOOP： 绘制从第一个顶点到最后一个顶点依次相连的一组线段，然后最后一个顶点和第一个顶点相连，第n和n+1个顶点定义了线段n，绘制n条线段。
//GL_TRIANGLES： 把每个顶点作为一个独立的三角形，顶点3n-2，3n-1和3n定义了第n个三角形，绘制了N/3个三角形。
//GL_TRIANGLE_STPIP：绘制一组相连的三角形，对于奇数n，顶点n，n+1，和n+2定义了第n个三角形；对于偶数n，顶点n+1，n和n+2定义了第n个三角形，绘制N-2个三角 形。
//GL_QUADS: 绘制由四个顶点组成的一组单独的四边形。顶点4n-3，4n-2，4n-1和4n定义了第n个四边形。绘制了N/4个四边形。
//GL_QUAD_STRIP：绘制一组相连的四边形。每个四边形是由一对顶点及其后给定的一对顶点共同确定的。顶点2n-1，2n，2n+2和2n+1定义了第n个四边形，绘制了N/2-1个   四边形。
//GL_POLYGON：      绘制了一个凸多边形。顶点1到n定义了这个多边形。



        glNormal3f(g_norm[triPatch[i].A].x, g_norm[triPatch[i].A].y, g_norm[triPatch[i].A].z);
        glNormal3f(g_norm[triPatch[i].B].x, g_norm[triPatch[i].B].y, g_norm[triPatch[i].B].z);
        glNormal3f(g_norm[triPatch[i].C].x, g_norm[triPatch[i].C].y, g_norm[triPatch[i].C].z);

        glColor3f(g_color[triPatch[i].A].x, g_color[triPatch[i].A].y, g_color[triPatch[i].A].z);
        glColor3f(g_color[triPatch[i].B].x, g_color[triPatch[i].B].y, g_color[triPatch[i].B].z);
        glColor3f(g_color[triPatch[i].C].x, g_color[triPatch[i].C].y, g_color[triPatch[i].C].z);
        glVertex3f(g_vet[triPatch[i].A].x, g_vet[triPatch[i].A].y, g_vet[triPatch[i].A].z);
        glVertex3f(g_vet[triPatch[i].B].x, g_vet[triPatch[i].B].y, g_vet[triPatch[i].B].z);
        glVertex3f(g_vet[triPatch[i].C].x, g_vet[triPatch[i].C].y, g_vet[triPatch[i].C].z);


        glEnd();
    }
    //glEnd();
}

static void resize(int width, int height) {
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);//0,0指定了窗口的左下角位置。
    //width,height表示视口矩形的宽度和高度，根据窗口的实时变化重绘窗口。
    glMatrixMode(GL_PROJECTION);//将当前矩阵指定为投影矩阵
    glLoadIdentity();//把矩阵设为单位矩阵
    gluPerspective(45, ar, 1.0, 100.0);//指定要观察的视景体在世界坐标系中的具体大小
    glMatrixMode(GL_MODELVIEW);//指定当前要操作的矩阵， GL_MODELVIEW模型视图
    glLoadIdentity();
}

static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glTranslatef(g_modelPos[0], g_modelPos[1], g_modelPos[2]);
    //定义一个平移矩阵，该矩阵与当前矩阵相乘，使后续的图形相对与当前所在的屏幕位置。进行平移变换。

    glRotatef(g_rotx, 1.0, 0.0, 0.0);
    glRotatef(g_roty, 0.0, 1.0, 0.0);
    glRotatef(g_rotz, 0.0, 0.0, 1.0);

    DrawVertices();

    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y) {
    switch (key) {
        case 27 ://ASCLL表'ESC'
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

    glutPostRedisplay();//重绘图像，否则图像只有响应鼠标或键盘消息时才会更新图像。
}

static void idle(void) {
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);//初始化GLUT
    glutInitWindowSize(640, 480);//定义窗口大小
    glutInitWindowPosition(10, 10);//定义窗口位置
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    printf("Tips: use 'w', 's', 'a', 'd', 'z', 'x', 'q', 'e' keys to control the model\n");

    glutCreateWindow("3D Model Viewer");//创建窗口("窗口标题")

    glutReshapeFunc(resize);//自适应屏幕窗口大小的改变
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(0.2, 0.55, 1.0, 1);//设置好清除颜色
    glDisable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_POSITION, g_lightPos);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    glShadeModel(GL_SMOOTH);

    printf("Loading \"%s\"...\n", g_strModelName);
    LoadMeshPLY(g_strModelName);

    glutMainLoop();//使程序进入事件处理循环。该函数必须是main主函数的最后一条语句。

    return EXIT_SUCCESS;
}
