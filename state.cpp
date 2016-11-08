#include "state.h"

#include <QDebug>

#include <chrono>
using namespace std;

unsigned State::seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine State::generator = default_random_engine(seed);

State::State(){
    m_sNext = NULL;
    m_nSize = 0;
    m_nVSize = 0;
    m_vTape = NULL;
    m_vValue = NULL;
}

State::State(uint size){
    int mod = size%64;

    m_sNext = NULL;
    m_nSize = size;
    m_nVSize = size/64 + (mod==0?0:1);
    m_vTape = new bool[size];
    m_vValue = new uint64_t[m_nVSize];

    uniform_int_distribution<uint64_t> dist;
    uniform_int_distribution<uint64_t> ldist(0, (1<<mod)-1);

    if( mod == 0 ){
        for(uint i=0;i<m_nVSize;i++){
            m_vValue[i] = dist(State::generator);
            for(int j=0;j<64;j++)
                m_vTape[i*64+j] = (m_vValue[i] & (1ll<<(63-j))) != 0;
        }
    }
    else{
        m_vValue[0] = ldist(State::generator);
        for(int j=mod-1;j>=0;j--)
            m_vTape[mod-1-j] = (m_vValue[0] & (1ll<<j)) != 0;
        for(uint i=1;i<m_nVSize;i++){
            m_vValue[i] = dist(State::generator);
            for(int j=0;j<64;j++)
                m_vTape[(i-1)*64+mod+j] = (m_vValue[i] & (1ll<<(63-j))) != 0;
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

State* State::evolve(uint rule){
    if( m_vTape == NULL || m_vValue == NULL )
        return NULL;

    State *res = new State();
    res->m_nSize = m_nSize;
    res->m_nVSize = m_nVSize;
    res->m_vTape = new bool[m_nSize];
    res->m_vValue = new uint64_t[m_nVSize];

    int bit = 4*m_vTape[m_nSize-1] + 2*m_vTape[0] + m_vTape[1];
    res->m_vTape[0] = ((1<<bit) & rule) != 0;

    for(uint i=1;i<m_nSize-1;i++){
        bit = 4*m_vTape[i-1] + 2*m_vTape[i] + m_vTape[i+1];
        res->m_vTape[i] = ((1<<bit) & rule) != 0;
    }

    bit = 4*m_vTape[m_nSize-2] + 2*m_vTape[m_nSize-1] + m_vTape[0];
    res->m_vTape[m_nSize-1] = ((1<<bit) & rule) != 0;

    int mod = m_nSize%64;
    if( mod == 0 ){
        for(uint i=0;i<m_nVSize;i++){
            m_vValue[i] = 0;
            for(int j=0;j<64;j++)
                m_vValue[i] = m_vValue[i]*2 + m_vTape[i*64+j];
        }
    }
    else{
        m_vValue[0] = 0;
        for(int j=0;j<mod;j++)
            m_vValue[0] = m_vValue[0]*2 + m_vTape[j];
        for(uint i=1;i<m_nVSize;i++){
            m_vValue[i] = 0;
            for(int j=0;j<64;j++)
                m_vValue[i] = m_vValue[i]*2 + m_vTape[(i-1)*64+mod+j];
        }
    }

    return res;
}

State* State::clone(){
    State *s = new State();

    s->m_sNext = this->m_sNext;
    s->m_nSize = this->m_nSize;
    s->m_nVSize = this->m_nVSize;
    s->m_vTape = new bool[s->m_nSize];
    memcpy(s->m_vTape, this->m_vTape, s->m_nSize*sizeof(s->m_vTape[0]));
    s->m_vValue = new uint64_t[s->m_nVSize];
    memcpy(s->m_vValue, this->m_vValue, s->m_nVSize*sizeof(s->m_vValue[0]));

    return s;
}

bool State::equals(State *s){
    if( s->m_sNext != this->m_sNext || s->m_nSize != this->m_nSize
     || s->m_nVSize != this->m_nVSize ){
        return false;
    }

    for(uint i=0;i<s->m_nSize;i++)
        if( s->m_vTape[i] != this->m_vTape[i] )
            return false;

    for(uint i=0;i<s->m_nVSize;i++)
        if( s->m_vValue[i] != this->m_vValue[i] )
            return false;

    return true;
}

QString State::getStrTape(){
    QString str = "";
    for(uint i=0;i<m_nSize;i++)
        str += (m_vTape[i]?"1":"0");
    return str;
}
