#include "graph.h"

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
    for(State *s : m_vGraph)
        if( s != NULL )
            delete s;
}

void Graph::play(){
    if( m_nState == STOP )
        reset();

    State *s = new State(m_nSize);
    m_vGraph.append(s);

    th = new Thread(*s, m_nRule);
    connect(th, SIGNAL(stateReady(State)), this, SLOT(handleState(State)));
    connect(th, SIGNAL(stateError()), this, SLOT(handleError()));
    th->start();

    m_nState = PLAY;
    paintGL();
}

void Graph::pause(){
    m_nState = PAUSE;
    paintGL();
}

void Graph::reset(){
    for(State *s : m_vGraph)
        if( s != NULL )
            delete s;

    m_vGraph.clear();

    m_nState = STOP;
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

void Graph::handleState(State s){

}

void Graph::handleError(){

}

bool Graph::Inicializa(){
    return true;
}
