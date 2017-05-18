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
    m_vValue.clear();
    m_pLocation = QPointF();
}

State::State(uint size){
    int mod = size%64;

    m_sNext = NULL;
    m_nSize = size;
    m_nVSize = size/64 + (mod==0?0:1);
    m_vValue.clear();

    uniform_int_distribution<uint64_t> dist;
    uint64_t *row = new uint64_t[m_nVSize];

    if( mod != 0 ){
        uniform_int_distribution<uint64_t> ldist(0, (1<<mod)-1);
        row[0] = ldist(State::generator);
        for(uint i=1;i<m_nVSize;i++)
            row[i] = dist(State::generator);
    }
    else
        for(uint i=0;i<m_nVSize;i++)
            row[i] = dist(State::generator);
    m_vValue.append(row);

    bool lap = false;
    lap = true; // Quitar si se quieren agregar todos la combinaciones
    uint64_t c;
    // Agregar todos los posibles corrimientos de bits
    for(int idx=0; !lap ;idx++){
        row = new uint64_t[m_nVSize];

        c = 0;
        for(uint i=0;i<m_nVSize;i++){
            row[i] = m_vValue[idx][i] / 2 + (c << 63);
            c = m_vValue[idx][i] % 2;
        }
        if( mod != 0 )
            row[0] += (c << (mod-1));
        else
            row[0] += (c << 63);

        lap = true;
        for(uint i=0;i<m_nVSize;i++)
            if( row[i] != m_vValue[0][i] ){
                lap = false;
                break;
            }

        if( !lap )
            m_vValue.append(row);
    }

    uniform_real_distribution<double> x(-90.0, -10.0);
    uniform_real_distribution<double> y(-9.0, 9.0);
    m_pLocation = QPointF(x(State::generator), y(State::generator)*10);
}

State::~State(){
    for(uint64_t *r : m_vValue)
        if( r != NULL )
            delete r;
}

void State::zeros(){
    if( m_nSize == 0 )
        return;

    for(uint i=0;i<m_nVSize;i++)
        m_vValue[0][i] = 0;
}

void State::next(){
    bool go = true;
    int mod = m_nSize%64;

    for(uint i=m_nVSize-1;i>0 && go;i--){
        m_vValue[0][i]++;
        go = m_vValue[0][i] == 0;
    }

    if( go ){
        m_vValue[0][0]++;
        if( m_vValue[0][0] == ((uint64_t)(1<<mod)) )
            m_vValue[0][0] = 0;
    }
}

State* State::evolve(uint rule){
    if( m_vValue.isEmpty() )
        return NULL;

    State *res = new State();
    res->m_nSize = m_nSize;
    res->m_nVSize = m_nVSize;
    res->m_pLocation = m_pLocation + QPointF(2.0, 0.0);

    uint64_t *row = new uint64_t[res->m_nVSize];
    uint64_t *V = m_vValue[0];
    int mod = res->m_nSize%64;
    int bit;
    uint m = res->m_nVSize;

    switch(mod){
        case 0:
            for(uint i=0;i<m;i++){
                bit = 4*(V[(i+m-1)%m]%2) + 2*(V[i]>>63) + (V[i]>>62)%2;
                row[i] = ((1<<bit) & rule) != 0;
                for(int j=63;j>=2;j--){
                    bit = 4*((V[i]>>j)%2) + 2*((V[i]>>(j-1))%2) + (V[i]>>(j-2))%2;
                    row[i] = 2*row[i] + (((1<<bit) & rule) != 0);
                }
                bit = 4*((V[i]>>1)%2) + 2*(V[i]%2) + (V[(i+1)%m]>>63);
                row[i] = 2*row[i] + (((1<<bit) & rule) != 0);
            }
            break;

        case 1:
            bit = 4*(V[m-1]%2) + 2*V[0] + (V[1]>>63);
            row[0] = ((1<<bit) & rule) != 0;
            for(uint i=1;i<m;i++){
                bit = 4*(V[(i+m-1)%m]%2) + 2*(V[i]>>63) + (V[i]>>62)%2;
                row[i] = ((1<<bit) & rule) != 0;
                for(int j=63;j>=2;j--){
                    bit = 4*((V[i]>>j)%2) + 2*((V[i]>>(j-1))%2) + (V[i]>>(j-2))%2;
                    row[i] = 2*row[i] + (((1<<bit) & rule) != 0);
                }
                if( i+1 < m )
                    bit = 4*((V[i]>>1)%2) + 2*(V[i]%2) + (V[i+1]>>63);
                else
                    bit = 4*((V[i]>>1)%2) + 2*(V[i]%2) + V[0];
                row[i] = 2*row[i] + (((1<<bit) & rule) != 0);
            }
            break;

        default:
            bit = 4*(V[m-1]%2) + 2*(V[0]>>(mod-1)) + ((V[0]>>(mod-2))%2);
            row[0] = ((1<<bit) & rule) != 0;
            for(int j=mod-1;j>=2;j--){
                bit = 4*((V[0]>>j)%2) + 2*((V[0]>>(j-1))%2) + (V[0]>>(j-2))%2;
                row[0] = 2*row[0] + (((1<<bit) & rule) != 0);
            }
            if( m > 1 )
                bit = 4*((V[0]>>1)%2) + 2*(V[0]%2) + (V[1]>>63);
            else
                bit = 4*((V[0]>>1)%2) + 2*(V[0]%2) + (V[0]>>(mod-1));
            row[0] = 2*row[0] + (((1<<bit) & rule) != 0);
            for(uint i=1;i<m;i++){
                bit = 4*(V[(i+m-1)%m]%2) + 2*(V[i]>>63) + (V[i]>>62)%2;
                row[i] = ((1<<bit) & rule) != 0;
                for(int j=63;j>=2;j--){
                    bit = 4*((V[i]>>j)%2) + 2*((V[i]>>(j-1))%2) + (V[i]>>(j-2))%2;
                    row[i] = 2*row[i] + (((1<<bit) & rule) != 0);
                }
                if( i+1 < m )
                    bit = 4*((V[i]>>1)%2) + 2*(V[i]%2) + (V[i+1]>>63);
                else
                    bit = 4*((V[i]>>1)%2) + 2*(V[i]%2) + (V[0]>>(mod-1));
                row[i] = 2*row[i] + (((1<<bit) & rule) != 0);
            }
            break;
    }

    res->m_vValue.append(row);

    bool lap = false;
    lap = true;
    uint64_t c;
    for(int idx=0; !lap ;idx++){
        row = new uint64_t[res->m_nVSize];

        c = 0;
        for(uint i=0;i<res->m_nVSize;i++){
            row[i] = res->m_vValue[idx][i] / 2 + (c << 63);
            c = res->m_vValue[idx][i] % 2;
        }
        if( mod != 0 )
            row[0] += (c << (mod-1));
        else
            row[0] += (c << 63);

        lap = true;
        for(uint i=0;i<res->m_nVSize;i++)
            if( row[i] != res->m_vValue[0][i] ){
                lap = false;
                break;
            }

        if( !lap )
            res->m_vValue.append(row);
    }

    return res;
}

State* State::clone(){
    State *s = new State();

    s->m_sNext = this->m_sNext;
    s->m_nSize = this->m_nSize;
    s->m_nVSize = this->m_nVSize;
    s->m_vValue.clear();
    s->m_pLocation = this->m_pLocation;

    for(int i=0;i<this->m_vValue.size();i++){
        s->m_vValue.append(new uint64_t[s->m_nVSize]);
        memcpy(s->m_vValue[i], this->m_vValue[i], s->m_nVSize*sizeof(s->m_vValue[0][0]));
    }

    return s;
}

bool State::equals(State *s){
    if( s->m_nSize != this->m_nSize || s->m_nVSize != this->m_nVSize )
        return false;

    for(int i=0;i<s->m_vValue.size();i++){
        bool equals = true;
        for(uint j=0;j<s->m_nVSize;j++)
            if( s->m_vValue[i][j] != this->m_vValue[0][j] ){
                equals = false;
                break;
            }
        if( equals )
            return true;
    }

    return false;
}

bool State::isZero(){
    for(uint i=0;i<m_nVSize;i++)
        if( m_vValue[0][i] != 0 )
            return false;
    return true;
}

QString State::getStrTape(){
    QString str = "";
    for(uint i=0;i<m_nVSize;i++)
        str += QString::number(m_vValue[0][i], 2);
    return str;
}

QVector2D State::repulsionForce(State *s){
    QVector2D dif(m_pLocation - s->m_pLocation);

    double mag = 200 / dif.lengthSquared();

    return mag * dif;
}

QVector2D State::attractionForce(State *s){
    QVector2D dif(s->m_pLocation - m_pLocation);

    return 0.1 * dif;
}
