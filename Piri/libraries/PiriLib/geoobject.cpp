#include "geoobject.h"
#include <QPainter>

#include <QGraphicsScene>

GeoObject::GeoObject() : QGraphicsItem()
{
}

GeoObject::GeoObject(QGraphicsScene *scene, qreal x, qreal y, qreal z) :
    myScene(scene),
    position(x, y, z)
{
    setPos(position.x(), position.y());
}

GeoObject::~GeoObject()
{
}

QRectF GeoObject::boundingRect() const
{
}

void GeoObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}


QString GeoObject::hash()
{
    QString hashString = QString("%1_%2_%3").arg(position.x(), position.y(), position.z());
    return QString(QCryptographicHash::hash(hashString.toLatin1(), QCryptographicHash::Md5).toHex());
}


QVector3D GeoObject::toGlobal(QVector3D point)
{
    return point + position;
}

QVector3D GeoObject::toLocal(QVector3D point)
{
    return point - position;
}

/*
QPointF GeoObject::centerRect(QRect rect)
{
    return rect.center();
}
*/

//--------------------------------------------------------------------------------------------
//  GEOPOINT
//--------------------------------------------------------------------------------------------

GeoPoint::GeoPoint() : GeoObject()
{
    icon = 0;
}

GeoPoint::GeoPoint(QGraphicsScene *scene, qreal x, qreal y, qreal z) :  GeoObject(scene, x, y, z)
{
    //qDebug() << "GeoPoint...";
    icon = new QPixmap;
    //icon = new QPixmap("E:/hendrik/progemine/varia/icons/icon_close_x.png");
    iconScale = 1;
    mySize = 20;
    myScene = scene;
    position = QVector3D(x, y, z);
    myPen = QPen(Qt::black, 2);
    myBrush = QBrush(Qt::green);
    setPos(x, y);
    myScene->addItem(this);
    //qDebug() << "GeoPoint added!" << position;
}

GeoPoint::~GeoPoint()
{
}

QRectF GeoPoint::boundingRect() const
{
    if (icon->width())
    {
        return icon->rect();
    }
    return QRectF(QPointF(-mySize, -mySize), QPointF(mySize, mySize));
}

void GeoPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //qDebug() << "Painting GeoPoint";
    if (icon->width())
    {

        QPixmap temp(*icon);
        QSize s(icon->width()*iconScale, icon->height()*iconScale);
        temp = icon->scaled(s);
        painter->drawPixmap(-temp.rect().center(), temp);
        //painter->drawPixmap(-icon->rect().center(), *icon);
    } else {
        painter->setPen(myPen);
        painter->setBrush(myBrush);
        painter->drawEllipse(-mySize/2, -mySize/2, mySize, mySize);
    }
    //painter->drawText(QPointF(20.0, 0.0), hash());
    //qDebug() << "Painting complete...";
}


void GeoPoint::setIconScale(float scale)
{
    iconScale = scale;
}

void GeoPoint::setSize(float size)
{
    mySize = size;
}


float GeoPoint::getScale()
{
    return iconScale;
}


//--------------------------------------------------------------------------------------------
//  GEOLINE
//--------------------------------------------------------------------------------------------

GeoLine::GeoLine() : GeoObject()
{
}

GeoLine::GeoLine(QGraphicsScene *scene, qreal x, qreal y, qreal z) :  GeoObject(scene, x, y, z)
{
    myScene = scene;
    position = QVector3D(x, y, z);
    myPen = QPen(Qt::black, 2);
    myBrush = QBrush(Qt::green);
    setPos(x, y);
    myScene->addItem(this);
    /*
    vertices << QVector3D(0.0, 0.0, 0.0);
    vertices << QVector3D(30.0, 0.0, 0.0);
    vertices << QVector3D(15.0, 30.0, 0.0);
    vertices << QVector3D(-15.0, 15.0, 0.0);
    setupPolygon(0);
    */
}

GeoLine::~GeoLine()
{
}

void GeoLine::setupPolygon(int poly)
{

    QPolygonF pl;
    foreach (QVector3D p, vertices)
    {
        pl.append(QPointF(p.x(), p.y()));
    }
    if (polygons.count() >= poly && !polygons.isEmpty())
    {
        polygons.replace(poly, pl);
    } else {
        polygons.append(pl);
    }
    //qDebug() << polygons.first();


}


void GeoLine::addPoint(float x, float y, float z, int poly)
{
    vertices << QVector3D(x, y, z);
    //setupPolygon(poly);
}

void GeoLine::updatePoint(float x, float y, float z, int point)
{

    QList<QVector3D> temp;
    int v = 0;
    foreach (QVector3D p, vertices) {
        if (v == point)
        {
            p.setX(p.x() + x);
            p.setY(p.y() + y);
            p.setZ(p.z() + z);
        }
        temp.append(p);
        v++;
    }
    vertices = temp;

    /*
    QVector3D* p;
    p = vertices.at(point);
    p->setX(p->x() + x);
    p->setY(p->y() + y);
    p->setZ(p->z() + z);

    QVector3D p;
    p.setX(vertices.at(point).x() + x);
    p.setY(vertices.at(point).y() + y);
    p.setZ(vertices.at(point).z() + z);
    vertices.at(point).setX(p.x());
    vertices.at(point).setY(p.y());
    vertices.at(point).setZ(p.z());
    */
}

void GeoLine::flipDirection()
{
    QList<QVector3D> temp;
    foreach (QVector3D p, vertices) {
        temp.prepend(p);
    }
    vertices = temp;
}

QString GeoLine::hash()
{
    QString hashString = QString("%1_%2_%3").arg(position.x(), position.y(), position.z());
    foreach(QVector3D p, vertices)
    {
        hashString += QString("%1_%2_%3").arg(p.x(), p.y(), p.z());
    }
    return QString(QCryptographicHash::hash(hashString.toLatin1(), QCryptographicHash::Md5).toHex());
}

QRectF GeoLine::boundingRect() const
{
    float x0, y0, x1, y1;
    foreach (QVector3D p, vertices)
    {
        if (p.x() < x0) { x0 = p.x(); }
        if (p.x() > x1) { x1 = p.x(); }
        if (p.y() < y0) { y0 = p.y(); }
        if (p.y() > y1) { y1 = p.y(); }
    }

    return QRectF(QPointF(x0, y0), QPointF(x1, y1));
}

void GeoLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int i = vertices.length();
    QPointF points[i];
    for (int p = 0; p < i; p++)
    {
        points[p] = QPointF(vertices.at(p).x(), vertices.at(p).y());
    }
    painter->setPen(myPen);
    painter->setBrush(myBrush);
    painter->drawPolyline(points, i);
    //painter->drawPolygon(polygons.first());
    //painter->drawText(QPointF(30.0, 0.0), hash());
}
