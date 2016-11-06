#ifndef GRAPH_H
#define GRAPH_H

#include <QOpenGLWidget>

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

protected:
    void initializeGL();
    void resizeGL(int, int);
    void paintGL();
    //void mousePressEvent(QMouseEvent *);
    //void mouseMoveEvent(QMouseEvent *);
    //void keyPressEvent(QKeyEvent *);

private:
    bool Inicializa();

    uint m_nSize;
    uint m_nRule;
};

#endif // GRAPH_H
