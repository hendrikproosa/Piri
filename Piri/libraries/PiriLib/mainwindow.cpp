#include "mainwindow.h"
#include "nodegraph.h"
#include "viewer.h"
#include "interfaces.h"

#include <QtWidgets>
#include <QStandardItemModel>
#include <QtDebug>
#include <QScrollArea>


/*!
 * \brief MainWindow initializer.
 *
 * Calls methots to create scene, DAG, table datamodel, actions, menus,
 * statusbar and docked subwindows. Also calls loadPlugins() that loads
 * all DLL plugins.
 * @see createScene()
 * @see createDAG()
 * @see createData()
 * @see createActions()
 * @see createMenus()
 * @see createStatusBar()
 * @see loadPlugins()
 * @see createDockWindows()
 */
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

/*!
 * \brief MainWindow about action.
 *
 * Action that gets called when user selects menu item Help->About...
 * @see createActions()
 * @see createMenus()
 */
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Piri"),
             tr("<b>Piri</b> is a node-based GIS application "
                "designed to be fast, intuitive and highly adaptable. <p>"
                "If how it's done is more important than how it looks,"
                "then <b>Piri</b> is made for you!"));
}

/*!
 * \brief MainWindow close action.
 *
 * Action that gets called when user selects menu item File->Close
 * @see createActions()
 * @see createMenus()
 */
void MainWindow::close()
{
    qApp->exit();
}

/*!
 * \brief Adds new Op to nodegraph.
 * Adds new Op to main nodegraph. At first it casts the sender object to QAction,
 * then casts QAction parent to OpInterface and calls mainGraph->addOp(Op).
 * @see nodeGraph::addOp()
 */
void MainWindow::addOp()
{
    QAction *action = qobject_cast<QAction *>(sender());
    OpInterface *Op = qobject_cast<OpInterface *>(action->parent());
    mainGraph->addOp(Op);
}

/*!
 * \brief Creates the main scene for 2D data.
 *
 * This function creates the main QGraphicsScene 'scene2D' where all 2D graphical objects will be added.
 * Scene span is -10 000 000 to 10 000 000 units.
 */
void MainWindow::createScene()
{
    QBrush bgBrush(Qt::white);
    scene2D = new QGraphicsScene(this);
    scene2D->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene2D->setSceneRect(-10000000, -10000000, 20000000, 20000000);
    scene2D->setBackgroundBrush(bgBrush);
}

/*!
 * \brief Creates the nodegraph DAG scene.
 *
 * Nodegraph DAG scene creates a QGrapicsScene 'sceneDAG' that spans
 * -10 000 to 10 000 units and holds all node graph items.
 */
void MainWindow::createDAG()
{
    sceneDAG = new QGraphicsScene(this);
    sceneDAG->setItemIndexMethod(QGraphicsScene::NoIndex);
    sceneDAG->setSceneRect(-10000, -10000, 20000, 20000);
}

/*!
 * \brief Creates the table datamodel.
 *
 * Creates table data model 'tableData' based on QStandardItemModel.
 * @see getTableData()
 */
void MainWindow::createData()
{
    tableData = new QStandardItemModel(this);
}

/*!
 * \brief Get main tabledata model.
 * @see createData()
 * \return main table datamodel 'tableData'
 */
QStandardItemModel* MainWindow::getTableData()
{
    if (tableData)
        return tableData;
    return 0;
}

/*!
 * \brief Get main properties view.
 * @see createDockWindows()
 * \return Main properties view 'propView'
 */
QWidget* MainWindow::getPropView()
{
    if (propView)
        return propView;
    return 0;
}

/*!
 * \brief Get main properties view layout.
 * @see createDockWindows()
 * @see getPropView()
 * \return Main properties view layout.
 */
QLayout* MainWindow::getPropViewLayout()
{
    if (propView)
    {
        if (propView->layout())
            return propView->layout();
    }
    return 0;
}

/*!
 * \brief Get main nodegraph.
 * Get main nodegraph that holds nodes and edges.
 * @see createDAG()
 * @see createDockWindows()
 * \return Nodegraph 'mainGraph'
 */
nodeGraph* MainWindow::getMainGraph()
{
    if (mainGraph)
        return mainGraph;
    return 0;
}

/*!
 * \brief Get main viewer area.
 * Returns main viewer area, that contains tableview and graphical views.
 * @see createDockWindows()
 * \return Main viewer area 'myViewer'
 */
Viewer* MainWindow::getViewer()
{
    if (myViewer)
        return myViewer;
    return 0;
}

/*!
 * \brief Get main 2D scene.
 * Returns main 2D QGraphicsScene scene called 'scene2D'.
 * \return Main 2D scene 'scene2D'
 */
QGraphicsScene* MainWindow::getScene2D()
{
    if (scene2D)
        return scene2D;
    return 0;
}

/*!
 * \brief Creates main application actions.
 *
 * Creates main application actions.
 * @see createMenus()
 * @see close()
 * @see about()
 */
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

/*!
 * \brief Creates main menubar and nodegraph popup menu.
 *
 * Creates main UI menubar items and nodegraph popup menu items.
 * @see addNodeMenuItem()
 */
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(quitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);

    // Nodegraph popup menu.
    nodeMenu = new QMenu;

    QStringList menulist;
    menulist << "File"
             << "Edit"
             << "Input"
             << "Output"
             << "Create"
             << "Transform"
             << "Query"
             << "Merge"
             << "MetaData"
             << "Viewer"
             << "Other";

    foreach(QString s, menulist)
    {
        addNodeMenuItem(s);
    }

}


/*!
 * \brief Adds new class to node menu. Not used?
 * \param s Menu item name.
 * @see createMenus()
 */
void MainWindow::addNodeMenuItem(QString menuItemName)
{
    QMenu *menu = new QMenu(menuItemName);
    nodeMenu->addMenu(menu);
    menuClasses.append(menu);
}


/*!
 * \brief Creates main UI statusbar.
 *
 * Main UI statusbar in the bottom left corner.
 * @see Knob_Callback::showError()
 */
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}


/*!
 * \brief Create UI docking windows.
 *
 * Creates main UI docking windows: viewer area, nodegraph, properties view.
 * @see createScene()
 * @see createData()
 * @see createDAG()
 */
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

/*!
 * \brief Loads DLL plugins.
 *
 * Loads all DLL plugins from /plugin directory that are compatible with OpInterface.
 * @see OpInterface
 * @see registerOp()
 * @see populateMenus()
 */
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

/*!
 * \brief Casts plugins to OpInterface and calls registerOp()
 *
 * Casts loaded plugins to OpInterface and if successful, sends them to registerOp() for loading.
 * \param plugin Plugin to be cast.
 * @see loadPlugins()
 * @see registerOp()
 * @see OpInterface
 */
void MainWindow::populateMenus(QObject *plugin)
{
    OpInterface *Op = qobject_cast<OpInterface *>(plugin);
    if (Op)
    {
        qDebug() << "Plugin OK!";
        registerOp(plugin, Op->description(), SLOT(addOp()));
    }
}

/*!
 * \brief Registers loaded plugin to system.
 *
 * Splits description of loaded plugin into class, name and description.
 * Creates new QAction and ties it with menu entry and addOp() function
 * for adding new node to graph.
 * Adds new menu class if necessary and new entry for loaded operation.
 * \param plugin Plugin to be registered.
 * \param text Plugin desctiption from Op->description().
 * \param member addOp() function to be tied into signal-slot mechanism.
 * @see loadPlugins()
 * @see populateMenus()
 */
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
