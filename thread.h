#ifndef THREAD_H
#define THREAD_H

#include "state.h"

#include <QThread>

class Thread : public QThread{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE;

public:
    Thread(State, uint, QObject *parent = Q_NULLPTR);

signals:
    void stateReady(State);
    void stateError();

private:
    State s;
    uint rule;
};

#endif // THREAD_H
