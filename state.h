#ifndef STATE_H
#define STATE_H

#include <QString>

#include <random>

class State{
public:
    State();
    State(uint);
   ~State();

    State* getNext(){ return m_sNext; }
    uint getSize(){ return m_nSize; }
    uint getVSize(){ return m_nVSize; }
    bool* getTape(){ return m_vTape; }
    uint64_t* getValues(){ return m_vValue; }

    void setNext(State *s){ m_sNext = s; }

    State* evolve(uint);
    State* clone();
    bool equals(State *);
    QString getStrTape();

private:
    State *m_sNext;
    uint m_nSize;
    uint m_nVSize;
    bool *m_vTape;
    uint64_t *m_vValue;

    static unsigned seed;
    static std::default_random_engine generator;
};

#endif // STATE_H
