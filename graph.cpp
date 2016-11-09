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
    qInfo() << "===== DESTRUCTOR ========";
    qInfo() << "Lenght: " << m_vGraph.size();
    for(State *s : m_vGraph){
        qInfo() << s;
        if( s != NULL )
            delete s;
    }
    if( m_Thread != NULL ){
        m_Thread->quit();
        m_Thread->wait();
        delete m_Thread;
        if( m_Work != NULL )
            delete m_Work;
    }
}

void Graph::play(){
    if( m_nState == STOP ){
        reset();

        State *s = new State(m_nSize);
        m_vGraph.append(s);

        m_Thread = new QThread;
        m_Work = new Worker;
        m_Work->moveToThread(m_Thread);

        connect(this, SIGNAL(operate(State*,uint)), m_Work, SLOT(doWork(State*,uint)));
        connect(m_Work, SIGNAL(stateError()), this, SLOT(handleError()));
        connect(m_Work, SIGNAL(stateReady(State*)), this, SLOT(handleState(State*)));

        m_Thread->start();
        emit operate(s, m_nRule);
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
        m_Thread->wait();
        delete m_Thread;
        if( m_Work != NULL )
            delete m_Work;
    }

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

    for(State *s : m_vGraph)
        if( s->equals(state) ){
            find = s;
            break;
        }

    if( find != NULL ){
        m_vGraph.back()->setNext(find);

        State *ns;
        int tries = 0;
        do{
            find = NULL;
            ns = new State(m_nSize);
            for(State *s : m_vGraph)
                if( s->equals(ns) ){
                    find = s;
                    break;
                }
            tries++;
        }
        while( find != NULL && tries < 520 );

        qInfo() << tries << " Intentos";
        m_vGraph.append(ns);
        if( tries < 520 )
            emit operate(ns, m_nRule);
        else{
            m_Thread->quit();
            qInfo() << "Size: " << m_vGraph.size();
            for(int i=0;i<m_vGraph.size();i++){
                if( m_vGraph[i]->getNext() == NULL )
                    qInfo() << "G[" << i << "]: " << m_vGraph[i]->getValues()[0] << "  =>  null";
                else
                    qInfo() << "G[" << i << "]: " << m_vGraph[i]->getValues()[0] << "  =>  " << (m_vGraph[i]->getNext()->getValues()[0]);
            }
        }
    }
    else if( m_vGraph.size() > 100 ){
        m_Thread->quit();

        qInfo() << "Size: " << m_vGraph.size();
        for(int i=0;i<m_vGraph.size();i++){
            qInfo() << "G[" << i << "]: State(" << m_vGraph[i] << "): "
                    << m_vGraph[i]->getStrTape() << "  =>  State("
                    << m_vGraph[i]->getNext() << ")";
        }
    }
    else{
        m_vGraph.back()->setNext(state);
        m_vGraph.append(state);
        emit operate(state, m_nRule);
    }
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
