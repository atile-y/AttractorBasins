#ifndef GRAPH_H
#define GRAPH_H

#include <QOpenGLWidget>

class Graph : public QOpenGLWidget {
    Q_OBJECT
public:
    explicit Graph(QWidget *parent = 0);
    ~Graph();

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
};

#endif // GRAPH_H
