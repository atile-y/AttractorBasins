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

        m_Thread = new QThread;
        m_Work = new Worker;
        m_Work->moveToThread(m_Thread);

        qRegisterMetaType< QVector<Graph*> >("QVector<Graph*>");
        connect(this, SIGNAL(newState(QVector<Graph*>)), m_Work, SLOT(findState(QVector<Graph*>)));
        connect(this, SIGNAL(evolve(State*,uint)), m_Work, SLOT(evolveState(State*,uint)));
        connect(m_Work, SIGNAL(check(State*)), this, SLOT(checkState(State*)));
        connect(m_Work, SIGNAL(add(State*)), this, SLOT(addState(State*)));
        connect(m_Work, SIGNAL(stateError(QString)), this, SLOT(handleError(QString)));

        m_Thread->start();

        State *s = new State(m_nSize);
        Graph *g = new Graph();
        g->addNode(s);
        m_vGraph.append(g);
        emit evolve(s, m_nRule);
    }
    else if( m_nCall == EVO )
        emit evolve(m_vGraph.last()->getLastNode(), m_nRule);
    else if( m_nCall == FIND )
        emit newState(m_vGraph);

    m_nState = PLAY;
    update();
}

void GLWidget::pause(){
    m_nState = PAUSE;
}

void GLWidget::reset(){
    if( m_Thread != NULL ){
        m_Thread->quit();
        m_Thread->wait();
        delete m_Thread;
        m_Thread = NULL;
        if( m_Work != NULL ){
            delete m_Work;
            m_Work = NULL;
        }
    }

    m_nRows = 1;

    for(Graph *g : m_vGraph)
        if( g != NULL )
            delete g;

    m_vGraph.clear();

    m_nState = STOP;
}

void GLWidget::Idle(){
    update();
}

void GLWidget::addState(State *state){
    Graph *g = new Graph();
    g->addNode(state);
    m_vGraph.append(g);
    if( m_nState == PLAY )
        emit evolve(state, m_nRule);
    else
        m_nCall = EVO;
}

void GLWidget::checkState(State *state){
    State *find = NULL;
    int idx = -1;

    for(int i=0;i<m_vGraph.size() && find == NULL;i++)
        for(State *s : m_vGraph[i]->getNodes()){
            if( s->equals(state) ){
                find = s;
                idx = i;
                break;
            }
        }

    if( find != NULL ){
        Graph *g;

        m_vGraph.last()->getLastNode()->setNext(find);
        if( idx + 1 < m_vGraph.size() ){
            g = m_vGraph.takeLast();
            for(State *s : g->getNodes())
                m_vGraph[idx]->addNode(s);
            g->freeNodes();
            delete g;
        }
        m_vGraph[idx]->layout();
        fitGraphs();

        if( m_nState == PLAY )
            emit newState(m_vGraph);
        else
            m_nCall = FIND;
    }
    else{
        m_vGraph.last()->getLastNode()->setNext(state);
        m_vGraph.last()->addNode(state);
        if( m_nState == PLAY )
            emit evolve(state, m_nRule);
        else
            m_nCall = EVO;
    }
}

void GLWidget::handleError(QString error){
    if( error == tr("Run out of tries.") ){
        if( m_nState == PLAY )
            emit newState(m_vGraph);
        else
            m_nCall = FIND;
    }
}

void GLWidget::initializeGL(){
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    gluLookAt(0, 0, 50, 0, 0, 0, 0, 1, 0);
}

void GLWidget::resizeGL(int width, int height){
    GLfloat w = width, h = height;

    m_dRatio = w / h;
    if( width < height ){
        m_dWidth = 100.0;
        m_dHeight = h*100.0/w;
    }
    else{
        m_dWidth = w*100.0/h;
        m_dHeight = 100.0;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-m_dWidth, m_dWidth, -m_dHeight, m_dHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    fitGraphs();
}

void GLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    double baseX = m_dWidth/m_nCols - m_dWidth;
    double baseY = m_dHeight - m_dHeight/m_nRows;
    double offsetX = 2 * (m_dWidth + baseX);
    double offsetY = 2 * (m_dHeight - baseY);

    for(int i=0;i<m_vGraph.size();i++){
        Graph *g = m_vGraph[i];

        glPushMatrix();
        glTranslated(baseX + (i%m_nCols)*offsetX, baseY - (i/m_nCols)*offsetY, 0.0);

        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2d(g->getTopLeft().x() - 4.0, g->getTopLeft().y() + 4.0);
            glVertex2d(g->getTopLeft().x() - 4.0, g->getBotRight().y() - 4.0);
            glVertex2d(g->getBotRight().x() + 4.0, g->getBotRight().y() - 4.0);
            glVertex2d(g->getBotRight().x() + 4.0, g->getTopLeft().y() + 4.0);
        glEnd();

        glColor3f(0.0f, 0.0f, 1.0f);
        for(State *s : g->getNodes()){
            State *nx = s->getNext();
            if( nx != NULL ){
                drawCircle(s->getPos(), 1.0);
                glBegin(GL_LINE_STRIP);
                    glVertex2d(s->getPos().x(), s->getPos().y());
                    glVertex2d(nx->getPos().x(), nx->getPos().y());
                glEnd();
            }
        }
        glPopMatrix();
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

void GLWidget::fitGraphs(){
    for(m_nRows=1;m_nRows < 1000;m_nRows++){
        m_nCols = qFloor(m_nRows * m_dRatio);
        if( m_nRows * m_nCols - m_vGraph.size() >= 0 )
            break;
    }

    for(Graph *g : m_vGraph)
        g->resize(m_dHeight / m_nRows);
}
