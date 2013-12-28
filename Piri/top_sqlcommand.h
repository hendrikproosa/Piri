#ifndef TOP_SQLCOMMAND_H
#define TOP_SQLCOMMAND_H

#include "tableop.h"
#include <QString>
#include <QStandardItemModel>
#include <QSqlQuery>

class Node;

class tOp_sqlCommand : public TableOp
{
public:
    tOp_sqlCommand(Node* parent);
    virtual void engine();
    virtual void knobs(Knob_Callback* f);
    QString command;


private:
    //Node* myParent;
    QStringList getHeaderLabels(QStandardItemModel *model);
    QStringList getHeaderLabels(QSqlQuery query);
    QStringList getRowValues(QStandardItemModel *model, int row);
    void setRowValues(QStandardItemModel *model, int row, QStringList values);
    QStringList dbRowToStringList(QSqlQuery query);


};

#endif // TOP_SQLCOMMAND_H
