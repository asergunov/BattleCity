#include <QtTest>

#include <Vector.hpp>
#include <Direction.hpp>
#include <FixedPoint.hpp>

using namespace battlecity::math;

namespace QTest {
template<>
char * toString(const Direction &dir) {
    switch (dir.type()) {
    case Direction::Up:
        return qstrdup("Up");
    case Direction::Down:
        return qstrdup("Down");
    case Direction::Left:
        return qstrdup("Left");
    case Direction::Right:
        return qstrdup("Right");
    }
    return nullptr;
}

template<class T>
char * toString(const Vector<T> &vector) {
    QByteArray ba;
    {
        QTextStream ds(&ba);
        ds << "Vector<" << typeid(T).name() << ">(" << toString(vector.x) << ", " << toString(vector.y) << ")";
    }
    return qstrdup(ba.data());
}


}

class Math : public QObject {
    Q_OBJECT
private slots:

};

QTEST_APPLESS_MAIN(Math)
#include "tst_math.moc"
