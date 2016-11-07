#include "thread.h"

Thread::Thread(State st, uint r, QObject *parent) : QThread(parent){
    s = st;
    rule = r;
}

void Thread::run(){
    State *ns = s.evolve(rule);

    if( ns != NULL ){
        s = *ns;
        emit stateReady(s);
    }
    else
        emit stateError();
}
