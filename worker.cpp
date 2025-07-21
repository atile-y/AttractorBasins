#include "worker.h"

#include <QDebug>
#include <QThread>

Worker::Worker(uint s){
    m_nErrors = 0;
    m_state = new State(s);
    m_state->zeros();

    if( s <= 20 ){
        m_visitado = new bool[1<<s];
        memset(m_visitado, 0, (1<<s)*sizeof(m_visitado[0]));
    }
    else
        m_visitado = NULL;
}

Worker::~Worker(){
    delete m_state;
    if( m_visitado != NULL )
        delete[] m_visitado;
}

void Worker::findState(QVector<Graph*> Gs){
    if( m_state->getSize() != Gs[0]->getNode(0)->getSize() ){
        emit stateError(tr("Different state's size."));
        return;
    }

    bool find = true;
    while( find ){
        find = false;

        if( m_visitado == NULL ){
            for(int i=0;i<Gs.size() && !find;i++)
                for(State *s : Gs[i]->getNodes())
                    if( s->equals(m_state) ){
                        find = true;
                        break;
                    }
        }
        else
            find = m_visitado[m_state->getValues()[0]];


        if( find ){
            m_state->next();
            if( m_state->isZero() ){
                emit add(NULL);
                return;
            }
        }
    }

    if( m_visitado != NULL )
        m_visitado[m_state->getValues()[0]] = true;
    emit add(m_state->clone());
}

void Worker::evolveState(State *s, uint r){
    State *ns = s->evolve(r);

    if( m_visitado != NULL )
        m_visitado[ns->getValues()[0]] = true;

    emit check(ns);
}
