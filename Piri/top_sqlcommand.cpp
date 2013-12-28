#include "top_sqlcommand.h"
#include "tableop.h"
#include "node.h"
#include "edge.h"
#include "nodegraph.h"
#include "knobs.h"
#include "knob_callback.h"
#include "mainwindow.h"

#include <QtDebug>
#include <QStandardItemModel>
#include <QTableView>
#include <QMessageBox>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSqlResult>

tOp_sqlCommand::tOp_sqlCommand(Node* parent) :
    command("select * from input0")
{
    myParent = parent;
    description("Select/SQL", "SQL Command enables sql syntax parsing. Use input0 for table name.");
}


void tOp_sqlCommand::knobs(Knob_Callback *f)
{
    qDebug() << "Got KNOBS: sort" << f;
    String_knob(f, &command, "SQL command: ");

}


void tOp_sqlCommand::engine()
{
    QStandardItemModel *model = myParent->getParent()->getTableData();
    foreach (Edge* e, myParent->edgesIn())
    {
        qDebug() << "Start exec: " << e;
        e->sourceNode()->execute();
    }

    qDebug() << "Start connection...";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(":memory:");
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "This example needs SQLite support. Please read "
                     "the Qt SQL driver documentation for information how "
                     "to build it.\n\n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
        showError("Database error!");
        return;
    }

    qDebug() << "QSQLITE Database connection";

    QSqlQuery query;

    QString com;
    QStringList headerLabels = getHeaderLabels(model);

    // Loome uue tabeli ja lisame sinna veerud mudelist
    com = "create table input0 (";
    foreach (QString s, getHeaderLabels(model))
    {
        com += s + " varchar(50),";
    }
    com.chop(1);
    com += ")";
    query.exec(com);

    com = "insert into input0 values(";
    for (int r = 0; r < model->rowCount(); r++)
    {
        foreach (QString s, getRowValues(model, r))
        {
            com += "\'" + s + "\',";
        }
        com.chop(1);
        com += ")";
        query.exec(com);
        com = "insert into input0 values(";
    }
    showError("Database OK!");

    model->clear();
    //query.exec("insert into input0 values('99','99','99','99','99','99','99')");
    query.exec(command);
    //qDebug() << command;
    model->setHorizontalHeaderLabels(getHeaderLabels(query));


    query.first();
    setRowValues(model, 0, dbRowToStringList(query));
    while (query.next())
    {
        setRowValues(model, 0, dbRowToStringList(query));
    }

    db.close();
    //showError("SQL query!");
}

QStringList tOp_sqlCommand::getHeaderLabels(QStandardItemModel *model)
{
    QStringList headerList;
    for (int c = 0; c < model->columnCount(); c++)
    {
        QStandardItem *i = model->horizontalHeaderItem(c);
        headerList << i->text();
    }
    return headerList;
}

QStringList tOp_sqlCommand::getHeaderLabels(QSqlQuery query)
{
    QStringList headerList;
    query.first();
    QSqlRecord rec = query.record();
    for (int i = 0; i < rec.count(); i++)
        headerList.append(rec.fieldName(i));
    return headerList;
}

QStringList tOp_sqlCommand::getRowValues(QStandardItemModel *model, int row)
{
    QStringList valuesList;
    for (int c = 0; c < model->columnCount(); c++)
    {
        QStandardItem *i = model->item(row, c);
        valuesList << i->text();
    }
    return valuesList;
}

void tOp_sqlCommand::setRowValues(QStandardItemModel *model, int row, QStringList values)
{
    QList<QStandardItem*> itemList;
    foreach(QString s, values)
    {
        itemList << new QStandardItem(s);
    }

    model->appendRow(itemList);
}

QStringList tOp_sqlCommand::dbRowToStringList(QSqlQuery query)
{
    QStringList valuesList;
    int c = 0;
    while (query.value(c).isValid())
    {
        valuesList << query.value(c).toString();
        c++;
    }
    return valuesList;
}
