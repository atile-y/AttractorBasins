#include "worker.h"

#include <QDebug>

void Worker::doWork(State *state, uint rule){
    State *newState = state->evolve(rule);

    if( newState != NULL )
        emit stateReady(newState);
    else
        emit stateError();
}
