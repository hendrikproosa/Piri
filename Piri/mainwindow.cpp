#include "mainwindow.h"
#include "nodegraph.h"
#include "viewer.h"
#include "interfaces.h"

#include <QtWidgets>
#include <QStandardItemModel>
#include <QtDebug>
#include <QScrollArea>


MainWindow::MainWindow()
{

    qDebug() << "MainWindow init start...";

    qDebug() << "MainWindow scene...";
    createScene();

    qDebug() << "MainWindow DAG...";
    createDAG();

    qDebug() << "MainWindow data...";
    createData();

    qDebug() << "MainWindow actions...";
    createActions();

    qDebug() << "MainWindow menus...";
    createMenus();

    qDebug() << "MainWindow statusbar...";
    createStatusBar();

    qDebug() << "MainWindow load plugins...";
    loadPlugins();

    createDockWindows();

    setWindowTitle(tr("Piri v.01"));
    qDebug() << "MainWindow initialized!";

}


void MainWindow::about()
{
    QMessageBox::about(this, tr("About Piri"),
             tr("<b>Piri</b> is a node-based GIS application "
                "designed to be fast, intuitive and highly adaptable. <p>"
                "If how it's done is more important than how it looks,"
                "then <b>Piri</b> is made for you!"));
}


void MainWindow::close()
{
    qApp->exit();
}


void MainWindow::addOp()
{
    QAction *action = qobject_cast<QAction *>(sender());
    OpInterface *Op = qobject_cast<OpInterface *>(action->parent());
    mainGraph->addOp(Op);
}


void MainWindow::createScene()
{
    QBrush bgBrush(Qt::white);
    scene2D = new QGraphicsScene(this);
    scene2D->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene2D->setSceneRect(-100000, -100000, 200000, 200000);
    scene2D->setBackgroundBrush(bgBrush);
}


void MainWindow::createDAG()
{
    sceneDAG = new QGraphicsScene(this);
    sceneDAG->setItemIndexMethod(QGraphicsScene::NoIndex);
    sceneDAG->setSceneRect(-10000, -10000, 20000, 20000);
}

void MainWindow::createData()
{
    tableData = new QStandardItemModel(this);
}


QStandardItemModel* MainWindow::getTableData()
{
    qDebug() << "MainWindow::getTableData()";
    if (tableData)
        return tableData;
    return 0;
}

QWidget* MainWindow::getPropView()
{
    if (propView)
        return propView;
    return 0;
}

QLayout* MainWindow::getPropViewLayout()
{
    if (propView)
    {
        if (propView->layout())
            return propView->layout();
    }
    return 0;
}


nodeGraph* MainWindow::getMainGraph()
{
    if (mainGraph)
        return mainGraph;
    return 0;
}


Viewer* MainWindow::getViewer()
{
    if (myViewer)
        return myViewer;
    return 0;
}


QGraphicsScene* MainWindow::getScene2D()
{
    if (scene2D)
        return scene2D;
    return 0;
}


void MainWindow::createActions()
{
    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setShortcuts(QKeySequence::HelpContents);
    aboutAct->setStatusTip(tr("Show the About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}


void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    //fileMenu->addAction(newLetterAct);
    //fileMenu->addAction(saveAct);
    //fileMenu->addAction(printAct);
    //fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    //editMenu->addAction(undoAct);

    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    nodeMenu = new QMenu;
}


void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}


void MainWindow::createDockWindows()
{
    QDockWidget* dock;

    // Viewer area
    dock = new QDockWidget(tr("Viewer"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    dock->setMinimumWidth(600);

    Viewer* viewer = new Viewer(this);
    myViewer = viewer;
    dock->setWidget(viewer);

    addDockWidget(Qt::LeftDockWidgetArea, dock);

    //Properties view
    dock = new QDockWidget(tr("Properties"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    dock->setMinimumWidth(400);
    dock->setMinimumHeight(200);
    dock->setMaximumWidth(600);
    propView = new QWidget;

    QVBoxLayout *myLayout = new QVBoxLayout;
    myLayout->setAlignment(Qt::AlignTop);
    myLayout->setMargin(0);
    myLayout->setSpacing(0);
    myLayout->setSizeConstraint(QLayout::SetMinimumSize);
    myLayout->setDirection(QBoxLayout::BottomToTop);
    propView->setLayout(myLayout);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(propView);

    dock->setWidget(scrollArea);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    // Nodegraph view
    dock = new QDockWidget(tr("Nodegraph"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    dock->setMinimumWidth(300);
    dock->setMinimumHeight(200);
    mainGraph = new nodeGraph(sceneDAG, this);
    dock->setWidget(mainGraph);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    this->setDockNestingEnabled(true);

}


void MainWindow::loadPlugins()
{
    pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");
    //pluginsDir = QDir("C:/Qt/Qt5.1.0/Tools/QtCreator/bin/nodeTest/plugins/");
    qDebug() << "Plugins dir: " << pluginsDir;

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        qDebug() << fileName;
        if (plugin) {
            populateMenus(plugin);
            pluginFileNames += fileName;
        }
    }
    qDebug() << "Loaded plugins: " << pluginFileNames;
}


void MainWindow::populateMenus(QObject *plugin)
{
    OpInterface *Op = qobject_cast<OpInterface *>(plugin);
    if (Op)
    {
        qDebug() << "Plugin OK!";
        registerOp(plugin, Op->description(), SLOT(addOp()));
    }
}


void MainWindow::registerOp(QObject *plugin, const QString text, const char *member)
{
    qDebug() << "Register plugin...";
    QString opName, menuName;
    opName = text.split(";").first();
    opName = opName.split("/").last();
    menuName = text.split(";").first();
    menuName = menuName.split("/").first();

    qDebug() << opName << menuName;
    QAction *action = new QAction(opName, plugin);
    qDebug() << "Action OK!";
    connect(action, SIGNAL(triggered()), this, member);
    qDebug() << "Action Connected!";

    // Otsime, kas vastav menüü on olemas ja kui pole, siis loome selle
    int exists = 0;
    foreach (QMenu *m, menuClasses)
    {
        if (m->title() == menuName)
            exists = 1;
    }
    if (!exists)
    {
        QMenu *menu = new QMenu(menuName);
        nodeMenu->addMenu(menu);
        menuClasses.append(menu);
    }
    foreach (QMenu *m, menuClasses)
    {
        if (m->title() == menuName)
            m->addAction(action);
    }
    qDebug() << "Action in Menu!";
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QMainWindow::mouseMoveEvent(event);
}
