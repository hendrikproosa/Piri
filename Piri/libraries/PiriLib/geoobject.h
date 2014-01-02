#ifndef GEOOBJECT_H
#define GEOOBJECT_H

#include "pirilib.h"

#include <QtWidgets>
#include <QList>
#include <QtGui/qvector3d.h>
#include <QtDebug>

class PIRILIBSHARED_EXPORT GeoObject : public QGraphicsItem
{
public:
    GeoObject();
    enum { Type = UserType + 1 };
    int type() const { return Type; }
    GeoObject(QGraphicsScene *scene, qreal x, qreal y, qreal z = 0);
    virtual ~GeoObject();
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QString hash();

    virtual QGraphicsScene* getScene() { return myScene; }
    virtual void setPen(QPen pen) { myPen = pen; }
    virtual void setBrush(QBrush brush) { myBrush = brush; }
    virtual QPen getPen() { return myPen; }
    virtual QBrush getBrush() { return myBrush; }
    virtual QList<QVector3D> getVertices() { return vertices; }

protected:

    QVector3D toGlobal(QVector3D point);
    QVector3D toLocal(QVector3D point);

    QVector3D position;
    QList<QVector3D> vertices;
    QGraphicsScene *myScene;
    QPen myPen;
    QBrush myBrush;
};

//----------------------------------------------------------------------------------------------------

class PIRILIBSHARED_EXPORT GeoPoint : public GeoObject
{
public:
    GeoPoint();
    GeoPoint(QGraphicsScene *scene, qreal x, qreal y, qreal z = 0);
    virtual ~GeoPoint();
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setIconScale(float scale);
    void setSize(float size);
    float getScale();

private:
    QPixmap *icon;
    float iconScale;
    float mySize;

};

//----------------------------------------------------------------------------------------------------

class PIRILIBSHARED_EXPORT GeoLine : public GeoObject
{
public:
    GeoLine();
    GeoLine(QGraphicsScene *scene, qreal x = 0, qreal y = 0, qreal z = 0);
    virtual ~GeoLine();
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void flipDirection();
    virtual QString hash();
    virtual void setupPolygon(int poly);
    virtual void addPoint(float x, float y, float z = 0, int poly = 0);


private:
    //QList<QVector3D> vertices;
    QList<QPolygonF> polygons;
    bool direction;             // Joone suund - false: 0 verteks on esimene; true: viimane verteks on esimene
};

//----------------------------------------------------------------------------------------------------

class PIRILIBSHARED_EXPORT GeoPolygon : public GeoObject
{
public:
    GeoPolygon();
    GeoPolygon(QGraphicsScene *scene, qreal x, qreal y, qreal z = 0);
    virtual ~GeoPolygon();
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void flipDirection();
    virtual void setupPolygon();

private:
    //QList<QVector3D> vertices;
    QList<QPolygonF> polygons;
    bool direction;             // Joone suund - false: 0 verteks on esimene; true: viimane verteks on esimene

};

#endif // GEOOBJECT_H
