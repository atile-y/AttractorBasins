#ifndef GRAPH_H
#define GRAPH_H

#define STOP    1
#define PAUSE   2
#define PLAY    3

#include <QOpenGLWidget>
#include <QThread>

#include "state.h"
#include "worker.h"

class Graph : public QOpenGLWidget {
    Q_OBJECT
public:
    explicit Graph(QWidget *parent = 0);
    ~Graph();

    uint getSize(){ return m_nSize; }
    uint getRule(){ return m_nRule; }

    void setSize(uint s){ m_nSize = s; }
    void setRule(uint r){ m_nRule = r; }

    void play();
    void pause();
    void reset();

public slots:
    void Idle();
    void handleState(State*);
    void handleError();

signals:
    void operate(State*, uint);

protected:
    void initializeGL();
    void resizeGL(int, int);
    void paintGL();

private:
    bool Inicializa();

    uint m_nSize;
    uint m_nRule;
    uint m_nState;
    QVector<State*> m_vGraph;
    Worker *m_Work;
    QThread *m_Thread;
};

#endif // GRAPH_H
