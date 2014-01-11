#include <QLayout>
#include <QGraphicsView>

#include "viewer.h"
#include "nodegraph.h"
#include "node.h"

Viewer::Viewer(QWidget *parent) :
    QWidget(parent)
{
}

Viewer::Viewer(MainWindow *mainW) :
    myMainWindow(mainW),
    viewerContext(table)
{
    createTableView();
    createSceneView();
    createTabs();
}



MainWindow* Viewer::getMainWindow()
{
    if (myMainWindow)
        return myMainWindow;
    return 0;
}

QTableView* Viewer::getTableView()
{
    if (myTableView)
        return myTableView;
    return 0;
}


QGraphicsView* Viewer::get2dView()
{
    if (my2dView)
        return my2dView;
    return 0;
}

context Viewer::getContext()
{
    if (viewerContext)
        return viewerContext;
    return table;
}


void Viewer::addWidget2d(QWidget *widget)
{
    widget->setParent(my2dView);
}

void Viewer::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Viewer key pressed";
    switch (event->key()) {
    case Qt::Key_1:
        qDebug() << "Active viewer:" << myMainWindow->getMainGraph()->getActiveViewer()->name();
        break;
    case Qt::Key_2:
        qDebug() << "Active viewer:" << myMainWindow->getMainGraph()->getActiveViewer()->name();
        break;
    case Qt::Key_3:
        qDebug() << "Active viewer:" << myMainWindow->getMainGraph()->getActiveViewer()->name();
        break;
    case Qt::Key_Q:
        switch (viewerContext)
        {
        case table:
            viewerContext = view2d;
            myMainWindow->statusBar()->showMessage("Context view2d");
            emit contextChanged(1);
            break;
        case view2d:
            viewerContext = view3d;
            myMainWindow->statusBar()->showMessage("Context view3d");
            emit contextChanged(2);
            break;
        case view3d:
            viewerContext = table;
            myMainWindow->statusBar()->showMessage("Context table");
            emit contextChanged(0);
            break;
        default:
            viewerContext = table;
            myMainWindow->statusBar()->showMessage("Context table");
            emit contextChanged(0);
        }
        qDebug() << "View context: " << viewerContext;
        break;
    default:
        break;
    }

    //QWidget::keyPressEvent(event);
}


void Viewer::enterEvent(QEvent *event)
{
    setFocus();
}

void Viewer::leaveEvent(QEvent *event)
{
    clearFocus();
}


void Viewer::createTableView()
{
    myTableView = new QTableView;
    if (myMainWindow->getTableData())
    {
        myTableView->setModel(myMainWindow->getTableData());
    } else {
        QMessageBox::critical(this, tr("Viewer::createTableView"),
                          tr("myMainWindow->getTableData() returned 0!"));
    }
    myTableView->setAlternatingRowColors(true);
    //qDebug() << "---------------------- Table rows: " << QString(myMainWindow->getTableData()->rowCount()).length();
    myTableView->verticalHeader()->setFixedWidth(30);
    //myTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}


void Viewer::createSceneView()
{
    if (myMainWindow->getScene2D())
    {
        my2dView = new Viewer2dView(myMainWindow->getScene2D(), this);
        my3dView = new Viewer3dView(myMainWindow->getScene2D(), this);
    } else {
        QMessageBox::critical(this, tr("Viewer::createSceneView"),
                                    tr("myMainWindow->getScene2D() returned 0!"));
    }
}


void Viewer::createTabs()
{
    // Widgetid, mis kirjeldavad erinevaid vieweri tab'e
    QWidget *viewerArea = new QWidget;

    QStackedLayout *viewerAreaLayout = new QStackedLayout;
    viewerAreaLayout->addWidget(myTableView);
    viewerAreaLayout->addWidget(my2dView);
    viewerAreaLayout->addWidget(my3dView);
    viewerAreaLayout->setMargin(0);
    viewerAreaLayout->setSpacing(0);
    viewerArea->setLayout(viewerAreaLayout);

    connect(this, SIGNAL(contextChanged(int)), viewerAreaLayout, SLOT(setCurrentIndex(int)));

    // Tabwidget mis hoiab vieweri eri tab'e. Miks neid vaja on, ei tea...
    QTabWidget *viewerTabWidget = new QTabWidget;

    //viewerTabWidget->addTab(viewerArea, "Viewer 1");

    // Sõlmede andmete hoidjawidget ja selle layout. Iga uus Knob_Callback paigutatakse teiste alla
    QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(viewerTabWidget);
    QLabel *settingBox = new QLabel(QString("Selection %1").arg(viewerSettings.sortEnabled));
    settingBox->setMaximumSize(120, 18);
    settingBox->setStyleSheet("QLabel { border: 1px inset rgb(100, 100, 100) }");
    QLabel *coordsBoxX = new QLabel("6540360");
    coordsBoxX->setMaximumSize(80, 18);
    coordsBoxX->move(120, 0);
    QLabel *coordsBoxY = new QLabel("450300");
    coordsBoxX->setMaximumSize(80, 18);
    coordsBoxY->move(200, 0);
    //mainLayout->addWidget(settingBox);
    //mainLayout->addWidget(coordsBoxX);
    //mainLayout->addWidget(coordsBoxY);
    mainLayout->addWidget(viewerArea);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);
}



