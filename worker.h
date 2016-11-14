#ifndef WORKER_H
#define WORKER_H

#include "graph.h"

#include <QObject>

class Worker : public QObject{
    Q_OBJECT

public:
    Worker();

public slots:
    void findState(QVector<Graph*>);
    void evolveState(State *, uint);

signals:
    void check(State *);
    void add(State *);
    void stateError(QString);

private:
    uint m_nErrors;
};

#endif // WORKER_H
