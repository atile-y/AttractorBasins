#include "state.h"

#include <QDebug>

#include <chrono>
using namespace std;

unsigned State::seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine State::generator = default_random_engine(seed);

State::State(uint size){
    uint mod = size%64;

    m_sNext = NULL;
    m_nSize = size;
    m_nVSize = size/64 + (mod==0?0:1);
    m_vTape = new bool[size];
    m_vValue = new uint64_t[m_nVSize];

    uniform_int_distribution<uint64_t> dist;
    uniform_int_distribution<uint64_t> ldist(0, (1<<mod)-1);
    qInfo() << "mod: " << mod;
    qInfo() << "Size: " << m_nSize;
    qInfo() << "VSize: " << m_nVSize;
    qInfo() << "dist: " << dist.min() << " -> " << dist.max();
    qInfo() << "ldist: " << ldist.min() << " -> " << ldist.max();

    if( mod == 0 ){
        for(uint i=0;i<m_nVSize;i++){
            m_vValue[i] = dist(State::generator);
            for(int j=0;j<64;j++)
                m_vTape[i*64+j] = (m_vValue[i] & (1<<(63-j))) != 0;
        }
    }
    else{
        m_vValue[0] = ldist(State::generator);
        qInfo() << "m_vValue[0]: " << m_vValue[0];
        QString str = "";
        for(int j=mod-1;j>=0;j--){
            m_vTape[mod-1-j] = (m_vValue[0] & (1<<j)) != 0;
            str += m_vTape[mod-1-j]?"1":"0";
        }
        qInfo() << "   : " << str;
        for(uint i=1;i<m_nVSize;i++){
            m_vValue[i] = dist(State::generator);
            qInfo() << "m_vValue[" << i << "]: " << m_vValue[i];
            str = "";
            for(int j=0;j<64;j++){
                m_vTape[(i-1)*64+mod+j] = (m_vValue[i] & (1<<(63-j))) != 0;
                str += m_vTape[(i-1)*64+mod+j]?"1":"0";
            }
            qInfo() << "   : " << str;
        }
    }
}

State::~State(){
    if( m_sNext != NULL )
        delete m_sNext;
    if( m_vTape != NULL )
        delete m_vTape;
    if( m_vValue != NULL )
        delete m_vValue;
}
