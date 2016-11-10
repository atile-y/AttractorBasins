#ifndef GRAPH_H
#define GRAPH_H

#include "state.h"

class Graph{
public:
    Graph();
   ~Graph();

    QVector<State*> getNodes(){ return m_vNodes; }
    State* getNode(int);
    QPointF getCenter(){ return m_pCenter; }
    QPointF getTopLeft(){ return m_pBoundary[0]; }
    QPointF getBotRight(){ return m_pBoundary[1]; }

    int getNumNodes();
    State* getLastNode();
    void addNode(State *);
    void freeNodes();

    void layout();

private:
    QVector<State*> m_vNodes;
    QPointF m_pCenter;
    QPointF m_pBoundary[2];
};

#endif // GRAPH_H
