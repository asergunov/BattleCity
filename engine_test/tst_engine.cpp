#include <QtTest>

#include "DummyTank.hpp"
#include "DummyBullet.hpp"

#include <Field.hpp>
#include <Scene.hpp>

#include <unordered_map>
#include <unordered_set>

Q_DECLARE_METATYPE(battlecity::engine::field_vector_type)
Q_DECLARE_METATYPE(battlecity::engine::direction_type)
Q_DECLARE_METATYPE(battlecity::engine::coordinate_vector_type)
Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE(std::unordered_set)

namespace std {
template<>
struct hash<battlecity::engine::Cell::Type> {
    size_t operator()(const battlecity::engine::Cell::Type& arg) const {
        return static_cast<size_t>(arg);
    }
};
}

namespace battlecity { namespace math {
template<class T>
char * toString(const Vector<T> &v) {
    using QTest::toString;
    return toString(QByteArray() + "Vector(" + toString(v.x) + ", " + toString(v.y) + ")");
}
}}

namespace std {
template<typename T, class Hash, class KeyEqual, class Allocator>
char * toString(const unordered_set<T, Hash, KeyEqual, Allocator>& container) {
    using QTest::toString;
    QByteArray ba;
    ba += "std::unordered_set(";
    for(const auto item : container) {
        ba += toString(item);
        ba += ", ";
    }
    ba += ")";

    return toString(ba);
}
}


namespace battlecity { namespace engine {

char * toString(const Cell &cell) {
    return QTest::toString(static_cast<uint>(cell.type()));
}

char * toString(const Field &field) {
    using QTest::toString;
    QByteArray ba;
    {
        QTextStream ds(&ba);
        ds << "Field (" << int(field.width()) << "x" << int(field.height()) << "\n";
        for(int y = 0; y < field.height(); ++y) {
            for(int x = 0; x < field.width(); ++x) {
                ds << static_cast<int>(field.at(x,y).type());
            }
            ds << '\n';
        }

    }
    return toString(ba);
}
}}

using namespace battlecity::engine;

namespace {
const auto allTypes = std::set<Cell::Type>{Cell::Type::Empty, Cell::Type::Brick, Cell::Type::Bush, Cell::Type::Ice, Cell::Type::Stone, Cell::Type::Water};
const auto emptyForTankTypes = std::set<Cell::Type>{Cell::Type::Empty, Cell::Type::Bush, Cell::Type::Ice};
const auto emptyForBulletTypes = std::set<Cell::Type>{Cell::Type::Empty, Cell::Type::Bush, Cell::Type::Ice, Cell::Type::Water};

const auto allHits = std::set<Cell::Hit>{Cell::Hit::Up, Cell::Hit::Down,  Cell::Hit::Left, Cell::Hit::Right};
}

class Engine : public QObject {
    Q_OBJECT
private slots:
    void cellSimple();
    void cellHitIgnore();
    void cellBrickBreak();
    void fieldResized();
    void fieldCreateEmpty();
    void fieldCreateByCellList();
    void fieldCreateFromClassicEncoding();
    void sceneCreate();

    void tankRespawn_data();
    void tankRespawn();

    void bulletProcess_data();
    void bulletProcess();

    void bulletCollistion_data();
    void bulletCollistion();
};

void Engine::cellSimple() {
    QVERIFY2(Cell() == Cell::Type::Empty, "Default constructor make it empty");
    for(const auto& type : allTypes) {
        QVERIFY(Cell(type) == type);

        if(emptyForBulletTypes.find(type) == emptyForBulletTypes.end())
            QVERIFY( Cell(type).isBuisyForBullet() );
        else
            QVERIFY( !Cell(type).isBuisyForBullet() );

        if(emptyForTankTypes.find(type) == emptyForTankTypes.end())
            QVERIFY( Cell(type).isBuisyForTank() );
        else
            QVERIFY( !Cell(type).isBuisyForTank() );
    }
}

void Engine::cellHitIgnore() {
    for(const auto type : allTypes) {
        for(const auto hit : allHits) {
            QCOMPARE(Cell(type).processHit(hit),
                     emptyForBulletTypes.find(type) == emptyForBulletTypes.end());
        }
    }
}

void Engine::cellBrickBreak() {
    for(const auto hit : allHits) {
        Cell cell(Cell::Type::Brick);
        QVERIFY( cell.processHit(hit) );
        QVERIFY( cell == Cell::Type::Brick );
        QVERIFY( cell.processHit(hit) );
        QVERIFY( cell == Cell::Type::Empty );
    }

    {
        Cell cell(Cell::Type::Brick);
        QVERIFY( cell.processHit(Cell::Hit::Up) );
        QVERIFY( cell == Cell::Type::Brick );
        QVERIFY( cell.processHit(Cell::Hit::Down) );
        QVERIFY( cell == Cell::Type::Empty );
    }

    {
        Cell cell(Cell::Type::Brick);
        QVERIFY( cell.processHit(Cell::Hit::Left) );
        QVERIFY( cell == Cell::Type::Brick );
        QVERIFY( cell.processHit(Cell::Hit::Right) );
        QVERIFY( cell == Cell::Type::Empty );
    }

    for(const auto& extraHit : std::set<Cell::Hit>{Cell::Hit::Left, Cell::Hit::Right}) {
        Cell cell(Cell::Type::Brick);
        QVERIFY( cell.processHit(Cell::Hit::Up) );
        QVERIFY( cell == Cell::Type::Brick );
        QVERIFY( cell.processHit(extraHit) );
        QVERIFY( cell == Cell::Type::Brick );
        QVERIFY( cell.processHit(Cell::Hit::Down) );
        QVERIFY( cell == Cell::Type::Empty );
    }

    for(const auto& extraHit : std::set<Cell::Hit>{Cell::Hit::Up, Cell::Hit::Down}) {
        Cell cell(Cell::Type::Brick);
        QVERIFY( cell.processHit(Cell::Hit::Left) );
        QVERIFY( cell == Cell::Type::Brick );
        QVERIFY( cell.processHit(extraHit) );
        QVERIFY( cell == Cell::Type::Brick );
        QVERIFY( cell.processHit(Cell::Hit::Right) );
        QVERIFY( cell == Cell::Type::Empty );
    }
}

void Engine::fieldResized()
{
    QCOMPARE(Field(1, 1, Cell::Type::Ice).resized(2, 2, Cell::Type::Ice), Field(2, 2, Cell::Type::Ice));
    QCOMPARE(Field(2, 2, Cell::Type::Ice).resized(1, 1, Cell::Type::Ice), Field(1, 1, Cell::Type::Ice));

    QCOMPARE(Field(0, 1, Cell::Type::Brick).resized(2, 3, Cell::Type::Brick), Field(2, 3, Cell::Type::Brick));

    QCOMPARE(Field(1, 2).resized(2, 1), Field(2,1));

    QCOMPARE(Field(1,1, Cell::Type::Brick).resized(1, 2, Cell::Type::Bush).at(0,0), Cell(Cell::Type::Brick));
    QCOMPARE(Field(1,1, Cell::Type::Brick).resized(1, 2, Cell::Type::Bush).at(0,1), Cell(Cell::Type::Bush));
    QCOMPARE(Field(1,1, Cell::Type::Brick).resized(2, 1, Cell::Type::Bush).at(1,0), Cell(Cell::Type::Bush));
}

void Engine::fieldCreateEmpty()
{
    QCOMPARE(Field(0,0).width(), static_cast<Field::size_type>(0));
    QCOMPARE(Field(0,0).height(), static_cast<Field::size_type>(0));

    QCOMPARE(Field(1,0).width(), static_cast<Field::size_type>(1));
    QCOMPARE(Field(1,0).height(), static_cast<Field::size_type>(0));

    QCOMPARE(Field(0,1).width(), static_cast<Field::size_type>(0));
    QCOMPARE(Field(0,1).height(), static_cast<Field::size_type>(1));

    QCOMPARE(Field(1, 1).width(), static_cast<Field::size_type>(1));
    QCOMPARE(Field(1, 1).height(), static_cast<Field::size_type>(1));
    QCOMPARE(Field(1, 1).at(0,0), Cell());
}

void Engine::fieldCreateByCellList()
{
    const auto field = Field({{Cell::Type::Empty, Cell::Type::Brick},
           {Cell::Type::Water},
           {},
          });

    QCOMPARE(field.width(), static_cast<Field::size_type>(2));
    QCOMPARE(field.height(), static_cast<Field::size_type>(3));
    QCOMPARE(field.at(0,0), Cell(Cell::Type::Empty));
    QCOMPARE(field.at(1,0), Cell(Cell::Type::Brick));

    QCOMPARE(field.at(0,1), Cell(Cell::Type::Water));
    QCOMPARE(field.at(1,1), Cell(Cell::Type::Empty));

    QCOMPARE(field.at(0,2), Cell(Cell::Type::Empty));
    QCOMPARE(field.at(1,2), Cell(Cell::Type::Empty));
}

void Engine::fieldCreateFromClassicEncoding()
{
    const auto field = Field::fromClassicCoding({ 0x08FFFFFFFFFFF,
                                                  0x19FFFFFFFFFFF,
                                                  0x2aFFFFFFFFFFF,
                                                  0x3bFFFFFFFFFFF,
                                                  0x4cFFFFFFFFFFF,
                                                  0x5dFFFFFFFFFFF,
                                                  0x6eFFFFFFFFFFF,
                                                  0x7fFFFFFFFFFFF,
                                                  0xFFFFFFFFFFFFF,
                                                  0xFFFFFFFFFFFFF,
                                                  0xFFFFFFFFFFFFF,
                                                  0xFFFFFFFFFFFFF,
                                                  0xFFFFFFFFFFFFF});

    const auto expectedField = Field({
        // 08
         {Cell::Type::Empty, Cell::Type::Brick, Cell::Type::Stone, Cell::Type::Stone},
         {Cell::Type::Empty, Cell::Type::Brick, Cell::Type::Empty, Cell::Type::Empty},
        // 19
         {Cell::Type::Empty, Cell::Type::Empty, Cell::Type::Stone, Cell::Type::Stone},
         {Cell::Type::Brick, Cell::Type::Brick, Cell::Type::Stone, Cell::Type::Stone},
        // 2a
         {Cell::Type::Brick, Cell::Type::Empty, Cell::Type::Water, Cell::Type::Water},
         {Cell::Type::Brick, Cell::Type::Empty, Cell::Type::Water, Cell::Type::Water},
        // 3b
         {Cell::Type::Brick, Cell::Type::Brick, Cell::Type::Bush, Cell::Type::Bush},
         {Cell::Type::Empty, Cell::Type::Empty, Cell::Type::Bush, Cell::Type::Bush},
        // 4c
         {Cell::Type::Brick, Cell::Type::Brick, Cell::Type::Ice, Cell::Type::Ice},
         {Cell::Type::Brick, Cell::Type::Brick, Cell::Type::Ice, Cell::Type::Ice},
        // 5d
         {Cell::Type::Empty, Cell::Type::Stone, Cell::Type::Empty, Cell::Type::Empty},
         {Cell::Type::Empty, Cell::Type::Stone, Cell::Type::Empty, Cell::Type::Empty},
        // 6e
         {Cell::Type::Empty, Cell::Type::Empty, Cell::Type::Empty, Cell::Type::Empty},
         {Cell::Type::Stone, Cell::Type::Stone, Cell::Type::Empty, Cell::Type::Empty},
        // 7f
         {Cell::Type::Stone, Cell::Type::Empty, Cell::Type::Empty, Cell::Type::Empty},
         {Cell::Type::Stone, Cell::Type::Empty, Cell::Type::Empty, Cell::Type::Empty},
     });

    // by square
    for(int i = 0; i<2; ++i) for(int j=0; j<8; ++j)
        QCOMPARE(expectedField.part(i*2,j*2,2,2), field.part(i*2,j*2,2,2));

    // the base
    QCOMPARE(field.part(11, field.height()-3, 4,3), Field(2,2,Cell::Type::Empty).part(-1,-1, 4, 3, Cell::Type::Brick));

    // empty space at left
    QCOMPARE(Field(field.width()-4, field.height()-3, Cell::Type::Empty),
             field.part(4, 0, field.width()-4, field.height()-3));

    // empty space at bottom except the base
    QCOMPARE(Field(field.width(), field.height()-16-3, Cell::Type::Empty),
             field.part(0, 16, field.width(), field.height()-16-3));

    // bottom 3 lines with base
    QCOMPARE(field.part(0, field.height()-3, field.width(), 3),
             Field(2,2,Cell::Type::Empty).part(-1,-1, 4, 3, Cell::Type::Brick).part(-11, 0, field.width(), 3));

    // filled up area
    QCOMPARE(expectedField, field.resized(expectedField.width(), expectedField.height()));


}

struct SceneListener : public Scene::Listener {


    // Listener interface
public:
    void tankExploded(Tank *) override {}
    void bulletExploded(Bullet *) override {}
    void scoreForEnemy(Player *, Enemy *) override {}
    void enemySpawned(Enemy *) override {}
    void bulletSpawned(Bullet *) override {}
    void enemyErased(Enemy *) override {}
    void scoreForBonus(Player *, Bonus *) override {}
    void bonusCreated(const Bonus *) override {}
    void bonusRemoved(const Bonus *) override {}
    void bonusTaken(const Player *, const Bonus *) override {}
    void gameOver() override {}
    void levelCompleted() override {}
};

void Engine::sceneCreate()
{
    SceneListener listener;
    Scene scene(&listener);
}

void Engine::tankRespawn_data()
{
    QTest::addColumn<field_vector_type>("cell");
    QTest::addColumn<direction_type>("direction");

    field_vector_type center_cell(2,2);
    QTest::newRow("center-up") << center_cell << direction_type(direction_type::Up);
    QTest::newRow("center-left") << center_cell << direction_type(direction_type::Left);
    QTest::newRow("center-right") << center_cell << direction_type(direction_type::Right);
    QTest::newRow("center-down") << center_cell << direction_type(direction_type::Down);
}

void Engine::tankRespawn()
{
    SceneListener listener;
    Scene scene(&listener);

    DummyTank tank(&scene);

    QFETCH(field_vector_type, cell);
    QFETCH(direction_type, direction);
    tank.respawn(cell, direction);

    QCOMPARE(tank.direction(), direction);
    QCOMPARE(tank.cell(), cell);
    QCOMPARE(tank.position(), scene.mapFromField(cell));
}

void Engine::bulletProcess_data()
{
    QTest::addColumn<field_vector_type>("cell");
    QTest::addColumn<direction_type>("direction");
    QTest::addColumn<field_vector_type>("expectedCell");
    QTest::addColumn<std::unordered_set<field_vector_type>>("expectedHit");
    QTest::addColumn<coordinate_vector_type>("expectedPosition");

    field_vector_type center_cell(2,2);
    QTest::newRow("center-up") << center_cell << direction_type(direction_type::Up)
                               << field_vector_type(2,2)
                               << std::unordered_set<field_vector_type>({{2,2}, {3,2}})
                               << coordinate_vector_type(20,14);
    QTest::newRow("center-left") << center_cell << direction_type(direction_type::Left)
                                 << field_vector_type(2,3)
                                 << std::unordered_set<field_vector_type>({{2,2}, {2,3}})
                                 << coordinate_vector_type(14,20);
    QTest::newRow("center-down") << center_cell << direction_type(direction_type::Down)
                                 << field_vector_type(3,3)
                                 << std::unordered_set<field_vector_type>({{2,3}, {3,3}})
                                 << coordinate_vector_type(20,26);
    QTest::newRow("center-right") << center_cell << direction_type(direction_type::Right)
                                  << field_vector_type(3,2)
                                  << std::unordered_set<field_vector_type>({{3,2}, {3,3}})
                                  << coordinate_vector_type(26,20);
}

void Engine::bulletProcess()
{
    SceneListener listener;
    Scene scene(&listener);

    DummyTank tank(&scene);

    QFETCH(field_vector_type, cell);
    QFETCH(direction_type, direction);
    QFETCH(field_vector_type, expectedCell);
    QFETCH(std::unordered_set<field_vector_type>, expectedHit);
    QFETCH(coordinate_vector_type, expectedPosition);

    tank.respawn(cell, direction);
    tank.fire();

    const auto bullet = tank.bullet();
    QCOMPARE(bullet->direction(), tank.direction());
    QCOMPARE(bullet->cell(), expectedCell);
    QCOMPARE(bullet->cellsToHit(), expectedHit);
    QCOMPARE(bullet->position(), expectedPosition);


    //bullet process
    bullet->process(8);

    std::unordered_set<field_vector_type> moved;
    for(const auto& item : expectedHit) {
        moved.insert(item + direction.toVector<field_coordinate_type>());
    }
    QCOMPARE(bullet->cellsToHit(), moved);
    QCOMPARE(bullet->position(), expectedPosition+direction.toVector<coordinate_type>()*8);
}

void Engine::bulletCollistion_data()
{
    QTest::addColumn<field_vector_type>("cellA");
    QTest::addColumn<direction_type>("directionA");

    QTest::addColumn<field_vector_type>("cellB");
    QTest::addColumn<direction_type>("directionB");

    QTest::newRow("verical")
            << field_vector_type(0,0) << direction_type(direction_type::Right)
            << field_vector_type(3,0) << direction_type(direction_type::Left);
    QTest::newRow("horizontal")
            << field_vector_type(0,0) << direction_type(direction_type::Down)
            << field_vector_type(0,3) << direction_type(direction_type::Up);
}

void Engine::bulletCollistion()
{
    SceneListener listener;
    Scene scene(&listener);

    DummyTank a(&scene), b(&scene);

    QFETCH(field_vector_type, cellA);
    QFETCH(direction_type, directionA);
    QFETCH(field_vector_type, cellB);
    QFETCH(direction_type, directionB);

    a.respawn(cellA, directionA);
    b.respawn(cellB, directionB);

    a.fire();
    b.fire();

    QCOMPARE(scene.bullets().size(), Scene::bullets_container::size_type(2));

    auto bulletA = *scene.bullets().begin();
    auto bulletB = *std::next(scene.bullets().begin());

    QVERIFY(!bulletA->collides(bulletB.get()));
    QVERIFY(!bulletB->collides(bulletA.get()));

    QVERIFY(!bulletA->collides(bulletA.get()));
    QVERIFY(!bulletB->collides(bulletB.get()));

    bulletA->process(8);
    bulletB->process(8);

    QVERIFY(bulletA->collides(bulletB.get()));
    QVERIFY(bulletB->collides(bulletA.get()));

    QVERIFY(!bulletA->collides(bulletA.get()));
    QVERIFY(!bulletB->collides(bulletB.get()));
}

QTEST_APPLESS_MAIN(Engine)
#include "tst_engine.moc"
