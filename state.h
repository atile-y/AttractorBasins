#ifndef STATE_H
#define STATE_H

#include <QPointF>
#include <QString>
#include <QVector>
#include <QVector2D>

#include <random>

class State{
public:
    State();
    State(uint);
   ~State();

    State* getNext(){ return m_sNext; }
    uint getSize(){ return m_nSize; }
    uint getVSize(){ return m_nVSize; }
    uint64_t* getValues(){ return m_vValue[0]; }
    QPointF getPos(){ return m_pLocation; }

    void setNext(State *s){ m_sNext = s; }
    void setPos(QPointF p){ m_pLocation = p; }

    State* evolve(uint);
    State* clone();
    bool equals(State *);
    QString getStrTape();

    QVector2D repulsionForce(State *);
    QVector2D attractionForce(State *);

private:
    State *m_sNext;
    uint m_nSize;
    uint m_nVSize;
    QVector<uint64_t*> m_vValue;
    QPointF m_pLocation;

    static unsigned seed;
    static std::default_random_engine generator;
};

#endif // STATE_H
