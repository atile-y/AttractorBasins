#include "graph.h"

#include <QDebug>
#include <QVector2D>

#include <chrono>
#include <random>
using namespace std;

Graph::Graph(){
}

Graph::~Graph(){
    for(State *s : m_vNodes)
        if( s != NULL )
            delete s;
}

State* Graph::getNode(int i){
    if( i < 0 || i >= m_vNodes.size() )
        return NULL;
    return m_vNodes[i];
}

int Graph::getNumNodes(){
    return m_vNodes.size();
}

State* Graph::getLastNode(){
    if( m_vNodes.isEmpty() )
        return NULL;

    return m_vNodes.back();
}

void Graph::addNode(State *s){
    m_vNodes.append(s);
}

void Graph::freeNodes(){
    m_vNodes.clear();
}

void Graph::layout(){
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine gen(seed);
    uniform_real_distribution<double> coor(-90.0, 90.0);

    for(State *s : m_vNodes)
        s->setPos(QPointF(coor(gen), coor(gen)));

    double threshold = 0.001;
    double totalMovement = 2*threshold;
    QVector2D tmp;
    QVector2D Vel[m_vNodes.size()];

    for(int i=0;i<500 && totalMovement > threshold;i++){
        totalMovement = 0.0;
        for(int j=0;j<m_vNodes.size();j++){
            tmp = Vel[j];
            for(int k=0;k<m_vNodes.size();k++)
                if( j != k ){
                    Vel[j] += m_vNodes[j]->repulsionForce(m_vNodes[k]);
                    if( m_vNodes[j] == m_vNodes[k]->getNext() )
                        Vel[j] += m_vNodes[j]->attractionForce(m_vNodes[k]);
                }
            if( m_vNodes[j] != m_vNodes[j]->getNext() )
                Vel[j] += m_vNodes[j]->attractionForce(m_vNodes[j]->getNext());

            Vel[j] *= 0.85;
            totalMovement += Vel[j].distanceToPoint(tmp);
        }
        for(int j=0;j<m_vNodes.size();j++)
            m_vNodes[j]->setPos(m_vNodes[j]->getPos() + Vel[j].toPointF());
    }

    qInfo() << "LAYOUT ENDS WITH" << totalMovement << "MOVEMENT";

    m_pCenter = QPointF();
    m_pBoundary[0] = m_vNodes[0]->getPos();
    m_pBoundary[1] = m_vNodes[0]->getPos();

    for(State *s : m_vNodes){
        m_pCenter += s->getPos();
        if( s->getPos().x() < m_pBoundary[0].x() )
            m_pBoundary[0].setX(s->getPos().x());
        if( s->getPos().y() > m_pBoundary[0].y() )
            m_pBoundary[0].setY(s->getPos().y());
        if( s->getPos().x() > m_pBoundary[1].x() )
            m_pBoundary[1].setX(s->getPos().x());
        if( s->getPos().y() < m_pBoundary[1].y() )
            m_pBoundary[1].setY(s->getPos().y());
    }
    m_pCenter /= m_vNodes.size();

    for(State *s : m_vNodes)
        s->setPos(s->getPos() - m_pCenter);
    m_pBoundary[0] -= m_pCenter;
    m_pBoundary[1] -= m_pCenter;

    m_pCenter -= m_pCenter;
}
