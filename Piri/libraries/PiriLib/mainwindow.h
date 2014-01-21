#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QStandardItemModel>
#include <QDir>

#include "pirilib.h"

// Forward declarations of classes used
QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QTableView;
class Viewer;
QT_END_NAMESPACE

class nodeGraph;

class PIRILIBSHARED_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT   // Vajalik signal-slot mehhanismi jaoks
public:
    MainWindow();
    QStandardItemModel* getTableData();
    QWidget* getPropView();
    QLayout* getPropViewLayout();
    nodeGraph* getMainGraph();
    Viewer* getViewer();
    QGraphicsScene* getScene2D();

    QString errorMessage();
    QMenu *nodeMenu; /*!< Node graph context menu. */
    QList<QMenu*> menuClasses; /*!< List of node graph context menu items. */

public slots:
    //void showErrorMessage();

private slots:
    void about();
    void close();
    void addOp();

protected:
    void mouseMoveEvent(QMouseEvent *event);

private:
    void createScene();
    void createDAG();
    void createData();
    void createActions();
    void createMenus();
    void addNodeMenuItem(QString menuItemName);
    //void createToolbars();
    void createStatusBar();
    void createDockWindows();
    void loadPlugins();
    void populateMenus(QObject *plugin);
    void registerOp(QObject *plugin, const QString text,
                   const char *member);

    QDir pluginsDir; /*!< Plugin directory. */
    QStringList pluginFileNames; /*!< List of all plugin names. */
    QGraphicsScene *scene2D; /*!< 2D scene that holds graphical objects. */
    QGraphicsScene *sceneDAG; /*!< Nodegraph scene that holds graphical objects of graph. */
    nodeGraph *mainGraph; /*!< Nodegraph object that operates on DAG. */
    QStandardItemModel *tableData; /*!< Table data model. */
    QWidget *propView; /*!< Dockable properties subwindow. */
    Viewer* myViewer; /*!< Dockable viewer subwindow. */

    QMenu *fileMenu; /*!< File menu object. */
    QMenu *editMenu; /*!< Edit menu object. */
    QMenu *viewMenu; /*!< View menu object. */
    QMenu *helpMenu; /*!< Help menu object. */

    QAction *aboutAct; /*!< Shows about dialog. */
    QAction *quitAct; /*!< Closes application. */

};

#endif // MAINWINDOW_H
