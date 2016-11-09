#ifndef WORKER_H
#define WORKER_H

#include "state.h"

#include <QObject>

class Worker : public QObject{
    Q_OBJECT

public slots:
    void doWork(State *, uint);

signals:
    void stateReady(State *);
    void stateError();
};

#endif // WORKER_H
