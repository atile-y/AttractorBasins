#ifndef WORKER_H
#define WORKER_H

#include "state.h"

#include <QObject>

class Worker : public QObject{
    Q_OBJECT

public:
    Worker(State *, uint);
   ~Worker();

public slots:
    void doWork();

signals:
    void stateReady(State *);
    void stateError();

private:
    State *m_State;
    uint m_nRule;
};

#endif // WORKER_H
