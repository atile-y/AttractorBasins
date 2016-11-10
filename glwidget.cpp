#include "glwidget.h"

#include <QDebug>
#include <QtMath>
#include <QTimer>
#include <GL/glu.h>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent){
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

GLWidget::~GLWidget(){
    while( !children().isEmpty() )
        delete children()[0];
    for(Graph *g : m_vGraph)
        if( g != NULL )
            delete g;
    if( m_Thread != NULL ){
        m_Thread->quit();
        m_Thread->wait();
        delete m_Thread;
        if( m_Work != NULL )
            delete m_Work;
    }
}

void GLWidget::play(){
    if( m_nState == STOP ){
        reset();

        State *s = new State(m_nSize);
        Graph *g = new Graph();

        g->addNode(s);
        m_vGraph.append(g);

        m_Thread = new QThread;
        m_Work = new Worker;
        m_Work->moveToThread(m_Thread);

        connect(this, SIGNAL(operate(State*,uint)), m_Work, SLOT(doWork(State*,uint)));
        connect(m_Work, SIGNAL(stateError()), this, SLOT(handleError()));
        connect(m_Work, SIGNAL(stateReady(State*)), this, SLOT(handleState(State*)));

        m_Thread->start();
        emit operate(s, m_nRule);
    }
    else
        emit operate(m_vGraph.last()->getLastNode(), m_nRule);

    m_nState = PLAY;
    update();
}

void GLWidget::pause(){
    m_nState = PAUSE;
}

void GLWidget::reset(){
    if( m_Thread != NULL ){
        qInfo() << "quit";
        m_Thread->quit();
        qInfo() << "wait";
        m_Thread->wait();
        qInfo() << "delete";
        delete m_Thread;
        m_Thread = NULL;
        qInfo() << "work";
        if( m_Work != NULL ){
            delete m_Work;
            m_Work = NULL;
        }
    }

    for(Graph *g : m_vGraph)
        if( g != NULL )
            delete g;

    m_vGraph.clear();

    m_nState = STOP;
}

void GLWidget::Idle(){
    update();
}

void GLWidget::handleState(State *state){
    State *find = NULL;
    int idx = -1;
    int nodes = 0;

    for(int i=0;i<m_vGraph.size() && find == NULL;i++)
        for(State *s : m_vGraph[i]->getNodes()){
            nodes++;
            if( s->equals(state) ){
                find = s;
                idx = i;
                break;
            }
        }

    if( find != NULL ){
        Graph *g;

        if( idx + 1 == m_vGraph.size() )
            m_vGraph[idx]->getLastNode()->setNext(find);
        else{
            g = m_vGraph.takeLast();
            g->getLastNode()->setNext(find);
            for(State *s : g->getNodes())
                m_vGraph[idx]->addNode(s);
            g->freeNodes();
            delete g;
        }

        State *ns;
        int tries = 0;
        do{
            ns = new State(m_nSize);
            for(int i=0;i<m_vGraph.size() && ns != NULL;i++)
                for(State *s : m_vGraph[i]->getNodes())
                    if( s->equals(ns) ){
                        delete ns;
                        ns = NULL;
                        break;
                    }
            tries++;
        }
        while( ns == NULL && tries < 2050 );

        if( tries < 2050 ){
            g = new Graph();
            g->addNode(ns);
            m_vGraph.append(g);
            emit operate(ns, m_nRule);
        }
        else{
            m_Thread->quit();
            qInfo() << m_vGraph.size() << " Graphs";
            QString str;
            for(int i=0;i<m_vGraph.size();i++){
                int m = m_vGraph[i]->getNumNodes();
                str = QString("G[%1]: %2 Nodes").arg(i).arg(m);
                qInfo() << str;

                m_vGraph[i]->layout();

                qInfo() << "   [" << m_vGraph[i]->getTopLeft()
                        << "," << m_vGraph[i]->getCenter()
                        << "," << m_vGraph[i]->getBotRight() << "]";

                for(int j=0;j<m;j++){
                    State *s = m_vGraph[i]->getNode(j);
                    str = QString("N[%1]: %2 (%3, %4)  =>  ")
                            .arg(j)
                            .arg(s->getValues()[0])
                            .arg(s->getPos().x())
                            .arg(s->getPos().y());
                    s = s->getNext();
                    if( s == NULL )
                        str += "null";
                    else{
                        str += QString("%1 (%2, %3)")
                                .arg(s->getValues()[0])
                                .arg(s->getPos().x())
                                .arg(s->getPos().y());
                    }
                    qInfo() << str;
                }
            }
        }
    }
    else if( nodes > 1000 ){
        m_Thread->quit();

        qInfo() << "MORE THAN 1000 NODES";
        qInfo() << m_vGraph.size() << " Graphs";
        QString str;
        for(int i=0;i<m_vGraph.size();i++){
            int m = m_vGraph[i]->getNumNodes();
            str = QString("G[%1]: %2 Nodes").arg(i).arg(m);
            qInfo() << str;
            for(int j=0;j<m;j++){
                State *s = m_vGraph[i]->getNode(j);
                str = QString("N[%1]: %2 (%3, %4)  =>  ")
                        .arg(j)
                        .arg(s->getValues()[0])
                        .arg(s->getPos().x())
                        .arg(s->getPos().y());
                s = s->getNext();
                if( s == NULL )
                    str += "null";
                else{
                    str += QString("%1 (%2, %3)")
                            .arg(s->getValues()[0])
                            .arg(s->getPos().x())
                            .arg(s->getPos().y());
                }
                qInfo() << str;
            }
        }
    }
    else{
        m_vGraph.last()->getLastNode()->setNext(state);
        m_vGraph.last()->addNode(state);
        emit operate(state, m_nRule);
    }
}

void GLWidget::handleError(){
    qInfo() << "handleError()";
    //m_Work->start();
}

void GLWidget::initializeGL(){
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void GLWidget::resizeGL(int width, int height){
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

void GLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        for(int i=-90;i<0;i+=10){
            glVertex2i(i, -90);
            glVertex2i(i, 90);
            glVertex2i(-90, i);
            glVertex2i(90, i);
            glVertex2i(-i, -90);
            glVertex2i(-i, 90);
            glVertex2i(-90, -i);
            glVertex2i(90, -i);
        }
        glColor3f(0.7f, 0.1f, 0.1f);
        glVertex2i(0, -100);
        glVertex2i(0, 100);
        glVertex2i(-100, 0);
        glVertex2i(100, 0);
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);
    for(Graph *g : m_vGraph)
        for(State *s : g->getNodes()){
            State *nx = s->getNext();
            if( nx != NULL ){
                drawCircle(s->getPos(), 2);
                glLineWidth(2.0f);
                glBegin(GL_LINE_STRIP);
                    glVertex2d(s->getPos().x(), s->getPos().y());
                    glVertex2d(nx->getPos().x(), nx->getPos().y());
                glEnd();
                glLineWidth(1.0f);
            }
        }
}

void GLWidget::drawCircle(QPointF c, double r){
    glBegin(GL_TRIANGLE_FAN);
        glVertex2d(c.x(), c.y());
        for(double theta=0.0;theta <= 360.0;theta++)
            glVertex2d(c.x() + r * qCos(qDegreesToRadians(theta)),
                       c.y() + r * qSin(qDegreesToRadians(theta)));
    glEnd();
}
