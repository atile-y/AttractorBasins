#ifndef WORKER_H
#define WORKER_H

#include "graph.h"

#include <QObject>

class Worker : public QObject{
    Q_OBJECT

public:
    Worker(uint);
    ~Worker();

public slots:
    void findState(QVector<Graph*>);
    void evolveState(State *, uint);

signals:
    void check(State *);
    void add(State *);
    void stateError(QString);

private:
    uint m_nErrors;
    State *m_state;
    bool *m_visitado;
};

#endif // WORKER_H
