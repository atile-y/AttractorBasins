#include "graph.h"
#include "state.h"

#include <QDebug>
#include <QTimer>
#include <GL/glu.h>

Graph::Graph(QWidget *parent) : QOpenGLWidget(parent){
    setWindowFlags(Qt::Window);
    setWindowTitle(tr("Graph"));
    setMinimumWidth(400);
    setMinimumHeight(400);
    resize(800, 600);

    m_nState = STOP;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Idle()));
    timer->start(20);
}

Graph::~Graph(){
    while( !children().isEmpty() )
        delete children()[0];
}

void Graph::play(){
    if( m_nState == STOP )
        reset();

    State *s = new State(m_nSize);
    /*qInfo() << s->getSize();
    QString str = "";
    for(int i=0;i<s->getSize();i++)
        str += (s->getTape()[i]?"1":"0");
    qInfo() << str;
    qInfo() << s->getVSize();
    for(int i=0;i<s->getVSize();i++)
        qInfo() << s->getValues()[i];*/

    delete s;

    m_nState = PLAY;
    paintGL();
}

void Graph::pause(){
    paintGL();
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
    GLfloat w = width, h = height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if( width < height )
        gluOrtho2D(-100, 100, -h*100/w, h*100/w);
    else
        gluOrtho2D(-w*100/h, w*100/h, -100, 100);
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
