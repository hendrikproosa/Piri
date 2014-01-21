#include "readraster.h"
#include "top_append.h"
#include "tableop.h"
#include "node.h"
#include "edge.h"
#include "nodegraph.h"
#include "knob_callback.h"
#include "knobs.h"

#include <QtDebug>
#include <QStandardItemModel>
#include <QTableView>
#include <QList>
#include <QImage>
#include <QRect>

void ReadRaster::setup()
{
    _file = "";
    _delimiter = ",";
    _colorSpace = 0;
    _myName = "Read";
    _myDesc = "Read data table.";
    _headers = true;
}

void ReadRaster::description(QString name, QString desc)
{
    setup();
}


QString ReadRaster::description()
{
    setup();
    return QString("Input/ReadRaster;Read raster file./0");
}


void ReadRaster::knobs(Knob_Callback *f)
{
    FileDialogKnob* knob = FileDialog_knob(f, &_file, "Ava fail: ");
    knob->setToolTip("Siia pane faili nimed");
}


void ReadRaster::engine()
{
    QElapsedTimer timer;
    timer.start();

    QStandardItemModel *model = getTableModel();
    QGraphicsScene *scene = getScene2D();
    model->clear();
    scene->clear();

    QFile file(_file);
    QImage image;
    QPixmap pximage;

    if (image.load(_file)) {
        pximage.load(_file);
        qDebug() << "ReadRaster has image pixmap!" << pximage.size();
        int rows = image.height();
        int cols = image.width();


        for (int y = 0; y < rows; y++)
        {
            for (int x = 0; x < cols; x++)
            {
                QRgb color = image.pixel(x, y);
                QColor pcolor;
                pcolor.setRgba(color);
                QPen pen(Qt::NoPen);
                QBrush brush(pcolor, Qt::SolidPattern);
                scene->addRect(x-0.5, y-0.5, 1.0, 1.0, pen, brush)->setZValue(1);
            }
        }

        QImage renderImage(1000, 1000, QImage::Format_ARGB32);
        renderImage.fill(Qt::transparent);
        QPainter painter(&renderImage);
        QRectF targetRect, sourceRect;
        sourceRect = renderImage.rect();
        targetRect = renderImage.rect();
        scene->render(&painter, targetRect, sourceRect);
        pximage = pximage.fromImage(renderImage);
        qDebug() << "Pximage size: " << pximage.size();
        scene->clear();
        scene->addPixmap(pximage);


    }
    else
    {
        model->clear();
        scene->clear();
    }
    file.close();

    qDebug() << "Read Node took: " << timer.elapsed() << "milliseconds";

}
