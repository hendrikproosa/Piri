#include "knob_callback.h"
#include "node.h"
#include "nodegraph.h"
#include "mainwindow.h"
#include "dataop.h"

#include <QtDebug>
#include <QtWidgets>


Knob_Callback::Knob_Callback(QWidget *parent) : QWidget(parent)
{

    makeKnobs();
}


Knob_Callback::Knob_Callback(Node *parent)
{
    myParent = parent;
    setFocusPolicy(Qt::WheelFocus);

    makeKnobs();
}

Knob_Callback::~Knob_Callback()
{
    qDebug() << "Callback destructor";
}


void Knob_Callback::makeKnobs()
{
    qDebug() << "Making knobs...";

    // Header bar
    QLineEdit *nodeName = new QLineEdit(this->getParent()->name());
    QPushButton *buttonh = new QPushButton("?");
    QPushButton *buttonx = new QPushButton("✕");
    //buttonx->setStyleSheet("QPushButton { image: url(E:/hendrik/progemine/varia/icons/icon_close_x.png); }");
    buttonh->setFixedWidth(25);
    buttonh->setToolTip(getParent()->myDesc);
    buttonx->setFixedWidth(25);
    connect(buttonx, SIGNAL(clicked()), this, SLOT(hide()));
    connect(nodeName, SIGNAL(textChanged(QString)), this, SLOT(nodeNameChanged(QString)));

    QWidget *header = new QWidget();
    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->setMargin(0);
    header->setLayout(hLayout);
    //hLayout->addWidget(button);
    hLayout->addWidget(nodeName);
    hLayout->addWidget(buttonh);
    hLayout->addWidget(buttonx);

    // Widgetid, mis kirjeldavad erinevaid knobide tab'e
    QWidget *knobArea = new QWidget();
    QWidget *notesArea = new QWidget();
    QWidget *metadataArea = new QWidget();
    QWidget *userKnobArea = new QWidget();

    // Knobide ala layout - vormilayout, mis jaotab üksteise alla label-widget põhimõttel
    QFormLayout *layout = new QFormLayout;
    layout->setMargin(20);
    knobArea->setLayout(layout);
    myLayout = layout;

    // Tabwidget mis hoiab erinevaid knobide tab'e
    QTabWidget *propTab = new QTabWidget;
    //propTab->addTab(vGroupBox, "Knobs");
    propTab->addTab(knobArea, "Knobs");
    propTab->addTab(metadataArea, "Metadata");
    propTab->addTab(notesArea, "Notes");
    propTab->addTab(userKnobArea, "User");
    propTab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    // Kogu ühe sõlme andmete hoidjawidget ja selle layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(header);
    mainLayout->addWidget(propTab);
    //mainLayout->setSizePolicy(QSizePolicy::Expanding);
    this->setLayout(mainLayout);

}

void Knob_Callback::nodeNameChanged(QString name)
{
    this->getParent()->setName(name);
}


QFormLayout *Knob_Callback::getLayout()
{
    return myLayout;
}


Node* Knob_Callback::getParent()
{
    return myParent;
}

DataOp* Knob_Callback::getOp()
{
    return myParent->getOp();
}


void Knob_Callback::addKnob(QWidget* knob, QString label, QString valName)
{
    qDebug() << "Knob_Callback::addKnob...";
    //qDebug() << knob << value << *value << label << valName;
    KnobStruct *kn = new KnobStruct;
    kn->widget = knob;
    kn->label = label;
    kn->valueName = valName;
    knobs << kn;
    qDebug() << "Knob_Callback::addKnob: " << knobs;
}


KnobStruct* Knob_Callback::getKnob(QString valName)
{
    foreach (KnobStruct *kn, knobs) {
        if (kn->valueName == valName)
            return kn;
    }
    return 0;
}


void Knob_Callback::valueChanged()
{
    qDebug() << "Callback for value changed!";
    qDebug() << "Sender: " << QObject::sender();
    this->getParent()->getParent()->evaluate();
}

void Knob_Callback::updateKnobs()
{
    getParent()->getOp()->make_knobs(this);
}


void Knob_Callback::showError(QString msg)
{
    getParent()->getParent()->getParent()->statusBar()->showMessage(msg);
}
