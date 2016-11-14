#ifndef GLWIDGET_H
#define GLWIDGET_H

#define STOP    1
#define PAUSE   2
#define PLAY    3

#define EVO     4
#define FIND    5

#include <QOpenGLWidget>
#include <QThread>

#include "graph.h"
#include "worker.h"

class GLWidget : public QOpenGLWidget {
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

    uint getSize(){ return m_nSize; }
    uint getRule(){ return m_nRule; }

    void setSize(uint s){ m_nSize = s; }
    void setRule(uint r){ m_nRule = r; }

    void play();
    void pause();
    void reset();

public slots:
    void Idle();
    void addState(State *);
    void checkState(State *);
    void handleError(QString);

signals:
    void newState(QVector<Graph*>);
    void evolve(State *, uint);

protected:
    void initializeGL();
    void resizeGL(int, int);
    void paintGL();

private:
    void drawCircle(QPointF, double);
    void fitGraphs();

    uint m_nSize;
    uint m_nRule;
    uint m_nState;
    uint m_nCall;
    int m_nRows;
    int m_nCols;
    double m_dRatio;
    double m_dWidth;
    double m_dHeight;
    QVector<Graph*> m_vGraph;
    Worker *m_Work;
    QThread *m_Thread;
};

#endif // GLWIDGET_H
