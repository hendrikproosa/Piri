#include "createpolygons.h"
#include "boost/geometry.hpp"
#include "boost/geometry/geometries/point_xy.hpp"
#include "boost/geometry/geometries/polygon.hpp"
#include "boost/geometry/io/wkt/wkt.hpp"
#include <boost/geometry/index/rtree.hpp>
#include "boost/geometry/geometries/point.hpp"
#include "boost/geometry/geometries/box.hpp"

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

void CreatePolygons::setup()
{
    _columnX = 2;
    _columnY = 3;
    _columnID = 4;
    inputCount = 1;
    _keepScene = true;
    _myName = "CreatePolygons";
    _myDesc = "Create polygons from table";
}

void CreatePolygons::description(QString name, QString desc)
{
    setup();
}


QString CreatePolygons::description()
{
    setup();
    qDebug() << "Description return" << inputCount;
    return QString("Create/CreatePolygons;Create polygons from points/1");
}

void CreatePolygons::knobs(Knob_Callback *f)
{
    ComboBox_knob(f, &_columnX, "X Coords: ", "col_x");
    ComboBox_knob(f, &_columnY, "Y Coords: ", "col_y");
    ComboBox_knob(f, &_columnID, "Polygon ID: ", "col_id");
    CheckBox_knob(f, &_useID, "Use ID values: ");
    CheckBox_knob(f, &_keepScene, "Keep prev. objects: ");
}


void CreatePolygons::engine()
{

    QGraphicsScene *scene = getScene2D();
    QStandardItemModel *model = getTableModel();

    qDebug() << "CreatePolygons engine: " << scene << model;

    if (myParent->edgesIn().count())
    {
        foreach (Edge* e, myParent->edgesIn())
        {
            e->sourceNode()->execute();
        }
    }

    qDebug() << "CreatePolygons engine!!!";

    // Boost test!

    typedef bg::model::point<float, 2, bg::cs::cartesian> point;
    typedef bg::model::box<point> box;
    typedef std::pair<box, unsigned> value;
    // Calculate the area of a cartesian polygon
    bg::model::polygon<bg::model::d2::point_xy<double> > poly;
    bg::read_wkt("POLYGON((0 0,0 7,4 2,2 0,0 0))", poly);
    double area = bg::area(poly);
    qDebug() << "BOOST Area: " << area;
    bgi::rtree< value, bgi::quadratic<16> > rtree;


    if (!_keepScene)
    {
        scene->clear();
    }

    if (model->columnCount() != _columnCount)
    {
        QString cols("-,");
        for (int c = 0; c < model->columnCount(); c++)
        {
            cols += model->horizontalHeaderItem(c)->text() + ",";
        }
        cols.chop(1);
        _columnCount = model->columnCount();

        UPDATE_VALUES(myCallback, getKnob("col_x"), cols);
        UPDATE_VALUES(myCallback, getKnob("col_y"), cols);
        UPDATE_VALUES(myCallback, getKnob("col_id"), cols);

    }

    _columnX = validateToCol(model, _columnX);
    _columnY = validateToCol(model, _columnY);
    _columnID = validateToCol(model, _columnID);

    qreal x, y;
    GeoLine *gL = new GeoLine(scene);
    for (int row = 0; row < model->rowCount(); row++)
    {
        QList<QStandardItem*> list = model->takeRow(row);
        x = list.at(_columnX-1)->text().toFloat();
        y = list.at(_columnY-1)->text().toFloat();
        QPen pen(Qt::black, 2);
        QBrush brush(Qt::red);
        gL->addPoint(x, y, 0);
        model->insertRow(row, list);
    }
    gL->setupPolygon(0);

    qDebug() << "CreatePolygons Engine finished sucessfully!!!";
}
