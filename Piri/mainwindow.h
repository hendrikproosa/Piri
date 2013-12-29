#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QStandardItemModel>
#include <QDir>

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

class MainWindow : public QMainWindow
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
    QMenu *nodeMenu;
    QList<QMenu*> menuClasses;

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
    //void createToolbars();
    void createStatusBar();
    void createDockWindows();
    void loadPlugins();
    void populateMenus(QObject *plugin);
    void registerOp(QObject *plugin, const QString text,
                   const char *member);

    QDir pluginsDir;
    QStringList pluginFileNames;
    QGraphicsScene *scene2D;
    QGraphicsScene *sceneDAG;
    nodeGraph *mainGraph;
    QStandardItemModel *tableData;
    QWidget *propView;
    Viewer* myViewer;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QAction *aboutAct;
    QAction *quitAct;

};

#endif // MAINWINDOW_H
