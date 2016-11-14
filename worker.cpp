#include "worker.h"

#include <QDebug>
#include <QThread>
#include <QTime>

Worker::Worker(){
    m_nErrors = 0;
}

void Worker::findState(QVector<Graph*> Gs){
    QTime ini;
    ini.start();

    State *ns;
    int size = Gs[0]->getNode(0)->getSize();
    int tries = 0;
    do{
        ns = new State(size);
        for(int i=0;i<Gs.size() && ns != NULL;i++)
            for(State *s : Gs[i]->getNodes())
                if( s->equals(ns) ){
                    delete ns;
                    ns = NULL;
                    break;
                }
        tries++;
    }
    while( ns == NULL && tries < 10000 );

    if( ns != NULL ){
        m_nErrors = 0;
        emit add(ns);
    }
    else{
        if( m_nErrors < 100 )
            m_nErrors++;
        QThread::msleep(m_nErrors * 10);
        emit stateError(tr("Run out of tries."));
    }
    if( ini.elapsed() > 500 )
        qInfo() << "findState end [" << ini.elapsed() << "ms]";
}

void Worker::evolveState(State *s, uint r){
    State *ns = s->evolve(r);

    emit check(ns);
}
