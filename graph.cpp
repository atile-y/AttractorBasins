#include "graph.h"

#include <QDebug>
//#include <QMouseEvent>
#include <QTimer>
#include <GL/glu.h>

Graph::Graph(QWidget *parent) : QOpenGLWidget(parent){
    //setMouseTracking(true);
    setWindowFlags(Qt::Window);
    setWindowTitle(tr("Graph"));
    setMinimumWidth(400);
    setMinimumHeight(300);
    resize(800, 600);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Idle()));
    timer->start(20);
}

Graph::~Graph(){
    while( !children().isEmpty() )
        delete children()[0];
}

void Graph::reset(){
    paintGL();
}

void Graph::Idle(){
    paintGL();
}

void Graph::initializeGL(){
    if( !Inicializa() )
        qCritical() << "Error inicializando\n";

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void Graph::resizeGL(int width, int height){
    GLfloat ratio;

    if( height == 0 )
        height = 1;

    ratio = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-100, 100, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 50, 0, 0, 0, 0, 1, 0);
}

void Graph::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glColor3f(0.5, 0.5, 0.8);
    glBegin(GL_TRIANGLES);
        glVertex2i(-100, -100);
        glVertex2i(0, 100);
        glVertex2i(100, -100);
    glEnd();
}

bool Graph::Inicializa(){
    return true;
}
