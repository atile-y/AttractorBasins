#include "worker.h"

#include <QDebug>

Worker::Worker(State *s, uint r){
    m_State = s;
    m_nRule = r;
}

Worker::~Worker(){
    if( m_State != NULL )
        delete m_State;
}

void Worker::doWork(){
    qInfo() << "doWork";

    State *aux = m_State->clone();
    State *newState = aux->evolve(m_nRule);

    if( newState != NULL ){
        m_State = newState;
        emit stateReady(m_State);
    }
    else
        emit stateError();
}
