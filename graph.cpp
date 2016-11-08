#include "graph.h"

#include <QDebug>
#include <QTimer>
#include <GL/glu.h>

Graph::Graph(QWidget *parent) : QOpenGLWidget(parent){
    setWindowFlags(Qt::Window);
    setWindowTitle(tr("Graph"));
    setMinimumWidth(400);
    setMinimumHeight(400);
    resize(500, 500);

    m_nState = STOP;
    m_Work = NULL;
    m_Thread = NULL;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Idle()));
    timer->start(20);
}

Graph::~Graph(){
    while( !children().isEmpty() )
        delete children()[0];
    qInfo() << "DESTRUCTOR";
    qInfo() << "Lenght: " << m_vGraph.size();
    for(State *s : m_vGraph){
        qInfo() << s;
        if( s != NULL )
            delete s;
    }
    if( m_Work != NULL )
        delete m_Work;
    if( m_Thread != NULL )
        delete m_Thread;
}

void Graph::play(){
    if( m_nState == STOP ){
        reset();

        State *s = new State(m_nSize);
        m_vGraph.append(s);
        qInfo() << "S_0(" << s << "): " << s->getStrTape();

        m_Thread = new QThread();
        m_Work = new Worker(s, m_nRule);
        m_Work->moveToThread(m_Thread);

        connect(m_Work, SIGNAL(stateError()), this, SLOT(handleError()));
        connect(m_Thread, SIGNAL(started()), m_Work, SLOT(doWork()));
        connect(m_Work, SIGNAL(stateReady(State*)), this, SLOT(handleState(State*)));

        m_Thread->start();
    }

    m_nState = PLAY;
    paintGL();
}

void Graph::pause(){
    m_nState = PAUSE;
    paintGL();
}

void Graph::reset(){
    if( m_Thread != NULL ){
        m_Thread->quit();
        m_Thread->deleteLater();
    }
    if( m_Work != NULL )
        m_Work->deleteLater();

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

void Graph::handleState(State *state){
    State *find = NULL;

    qInfo() << "S_1(" << state << "): " << state->getStrTape();

    for(State *s : m_vGraph)
        if( s->equals(state) )
            find = s;

    qInfo() << "find(" << find << ")";

    if( find != NULL ){
        m_vGraph.takeLast()->setNext(find);

        State *ns;
        do{
            find = NULL;
            ns = new State(m_nSize);
            for(State *s : m_vGraph)
                if( s->equals(ns) )
                    find = s;
        }
        while( find != NULL );

        m_vGraph.append(ns);
        m_Thread->quit();
        m_Thread->deleteLater();
        m_Work->deleteLater();
    }
    else{
        m_vGraph.back()->setNext(state);
        qInfo() << "   next(" << m_vGraph.back()->getNext() << ")";
        m_vGraph.append(state);
        m_Thread->run();
    }

    qInfo() << "Length: " << m_vGraph.size();
    qInfo() << "Last(" << m_vGraph.back() << "): " << m_vGraph.back()->getStrTape();
}

void Graph::handleError(){
    qInfo() << "handleError()";
    //m_Work->start();
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
    glBegin(GL_LINES);
        for(int i=0;i<m_vGraph.size();i++){
            glVertex2i(i, -100);
            glVertex2i(i, 100);
        }
    glEnd();
}

bool Graph::Inicializa(){
    return true;
}
