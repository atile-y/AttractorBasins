#include <QtTest/QtTest>
#include "../state.h"

class TestState : public QObject
{
    Q_OBJECT
private slots:
    void testZeros();
    void testNext();
};

void TestState::testZeros()
{
    State s(4);
    s.zeros();
    QVERIFY(s.isZero());
    uint64_t val = s.getValues()[0];
    QCOMPARE(val, static_cast<uint64_t>(0));
}

void TestState::testNext()
{
    State s(4);
    s.zeros();
    s.next();
    uint64_t val = s.getValues()[0] & 0xF; // only 4 bits
    QCOMPARE(val, static_cast<uint64_t>(1));
    s.next();
    val = s.getValues()[0] & 0xF;
    QCOMPARE(val, static_cast<uint64_t>(2));
}

QTEST_MAIN(TestState)
#include "test_state.moc"
