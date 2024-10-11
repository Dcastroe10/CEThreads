/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionsomething2;
    QAction *actionleft;
    QAction *actionright;
    QWidget *centralwidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *Waiting_Right;
    QLabel *waitingR0;
    QLabel *waitingR1;
    QLabel *waitingR2;
    QLabel *waitingR3;
    QLabel *waitingR4;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer;
    QPushButton *pruebaStructs;
    QPushButton *pruebaAddLabel;
    QVBoxLayout *Waiting_Left;
    QLabel *waitingL0;
    QLabel *waitingL1;
    QLabel *waitingL2;
    QLabel *waitingL3;
    QLabel *waitingL4;
    QHBoxLayout *El_canal;
    QSpacerItem *horizontalSpacer;
    QMenuBar *menubar;
    QMenu *menupiporin;
    QMenu *menucreate_boat;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1370, 683);
        actionsomething2 = new QAction(MainWindow);
        actionsomething2->setObjectName("actionsomething2");
        actionleft = new QAction(MainWindow);
        actionleft->setObjectName("actionleft");
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::ListAdd));
        actionleft->setIcon(icon);
        actionright = new QAction(MainWindow);
        actionright->setObjectName("actionright");
        actionright->setIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(10, 10, 1344, 618));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        Waiting_Right = new QVBoxLayout();
        Waiting_Right->setSpacing(10);
        Waiting_Right->setObjectName("Waiting_Right");
        Waiting_Right->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
        Waiting_Right->setContentsMargins(5, 5, 5, 5);
        waitingR0 = new QLabel(gridLayoutWidget);
        waitingR0->setObjectName("waitingR0");

        Waiting_Right->addWidget(waitingR0);

        waitingR1 = new QLabel(gridLayoutWidget);
        waitingR1->setObjectName("waitingR1");

        Waiting_Right->addWidget(waitingR1);

        waitingR2 = new QLabel(gridLayoutWidget);
        waitingR2->setObjectName("waitingR2");

        Waiting_Right->addWidget(waitingR2);

        waitingR3 = new QLabel(gridLayoutWidget);
        waitingR3->setObjectName("waitingR3");

        Waiting_Right->addWidget(waitingR3);

        waitingR4 = new QLabel(gridLayoutWidget);
        waitingR4->setObjectName("waitingR4");

        Waiting_Right->addWidget(waitingR4);


        gridLayout->addLayout(Waiting_Right, 0, 2, 5, 1);

        verticalSpacer_2 = new QSpacerItem(20, 200, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        gridLayout->addItem(verticalSpacer_2, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 200, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        gridLayout->addItem(verticalSpacer, 4, 1, 1, 1);

        pruebaStructs = new QPushButton(gridLayoutWidget);
        pruebaStructs->setObjectName("pruebaStructs");

        gridLayout->addWidget(pruebaStructs, 2, 1, 1, 1);

        pruebaAddLabel = new QPushButton(gridLayoutWidget);
        pruebaAddLabel->setObjectName("pruebaAddLabel");

        gridLayout->addWidget(pruebaAddLabel, 1, 1, 1, 1);

        Waiting_Left = new QVBoxLayout();
        Waiting_Left->setSpacing(10);
        Waiting_Left->setObjectName("Waiting_Left");
        Waiting_Left->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
        Waiting_Left->setContentsMargins(5, 5, 5, 5);
        waitingL0 = new QLabel(gridLayoutWidget);
        waitingL0->setObjectName("waitingL0");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(waitingL0->sizePolicy().hasHeightForWidth());
        waitingL0->setSizePolicy(sizePolicy);
        waitingL0->setMinimumSize(QSize(100, 100));

        Waiting_Left->addWidget(waitingL0);

        waitingL1 = new QLabel(gridLayoutWidget);
        waitingL1->setObjectName("waitingL1");
        sizePolicy.setHeightForWidth(waitingL1->sizePolicy().hasHeightForWidth());
        waitingL1->setSizePolicy(sizePolicy);
        waitingL1->setMinimumSize(QSize(100, 100));

        Waiting_Left->addWidget(waitingL1);

        waitingL2 = new QLabel(gridLayoutWidget);
        waitingL2->setObjectName("waitingL2");
        sizePolicy.setHeightForWidth(waitingL2->sizePolicy().hasHeightForWidth());
        waitingL2->setSizePolicy(sizePolicy);
        waitingL2->setMinimumSize(QSize(100, 100));

        Waiting_Left->addWidget(waitingL2);

        waitingL3 = new QLabel(gridLayoutWidget);
        waitingL3->setObjectName("waitingL3");
        sizePolicy.setHeightForWidth(waitingL3->sizePolicy().hasHeightForWidth());
        waitingL3->setSizePolicy(sizePolicy);
        waitingL3->setMinimumSize(QSize(100, 100));

        Waiting_Left->addWidget(waitingL3);

        waitingL4 = new QLabel(gridLayoutWidget);
        waitingL4->setObjectName("waitingL4");
        sizePolicy.setHeightForWidth(waitingL4->sizePolicy().hasHeightForWidth());
        waitingL4->setSizePolicy(sizePolicy);
        waitingL4->setMinimumSize(QSize(100, 100));

        Waiting_Left->addWidget(waitingL4);


        gridLayout->addLayout(Waiting_Left, 0, 0, 5, 1);

        El_canal = new QHBoxLayout();
        El_canal->setSpacing(10);
        El_canal->setObjectName("El_canal");
        El_canal->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
        El_canal->setContentsMargins(5, 5, 5, 5);
        horizontalSpacer = new QSpacerItem(900, 10, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        El_canal->addItem(horizontalSpacer);


        gridLayout->addLayout(El_canal, 3, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1370, 26));
        QSizePolicy sizePolicy1(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(menubar->sizePolicy().hasHeightForWidth());
        menubar->setSizePolicy(sizePolicy1);
        menubar->setMinimumSize(QSize(0, 2));
        menubar->setAutoFillBackground(false);
        menupiporin = new QMenu(menubar);
        menupiporin->setObjectName("menupiporin");
        menucreate_boat = new QMenu(menupiporin);
        menucreate_boat->setObjectName("menucreate_boat");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menupiporin->menuAction());
        menupiporin->addAction(menucreate_boat->menuAction());
        menupiporin->addSeparator();
        menupiporin->addSeparator();
        menupiporin->addAction(actionsomething2);
        menucreate_boat->addAction(actionleft);
        menucreate_boat->addAction(actionright);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "pipoWindow", nullptr));
        actionsomething2->setText(QCoreApplication::translate("MainWindow", "restart", nullptr));
        actionleft->setText(QCoreApplication::translate("MainWindow", "left", nullptr));
        actionright->setText(QCoreApplication::translate("MainWindow", "right", nullptr));
        waitingR0->setText(QCoreApplication::translate("MainWindow", "waitingR0", nullptr));
        waitingR1->setText(QCoreApplication::translate("MainWindow", "waitingR1", nullptr));
        waitingR2->setText(QCoreApplication::translate("MainWindow", "waitingR2", nullptr));
        waitingR3->setText(QCoreApplication::translate("MainWindow", "waitingR3", nullptr));
        waitingR4->setText(QCoreApplication::translate("MainWindow", "waitingR4", nullptr));
        pruebaStructs->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        pruebaAddLabel->setText(QCoreApplication::translate("MainWindow", "AddLabel", nullptr));
        waitingL0->setText(QCoreApplication::translate("MainWindow", "waitingL0", nullptr));
        waitingL1->setText(QCoreApplication::translate("MainWindow", "waitingL1", nullptr));
        waitingL2->setText(QCoreApplication::translate("MainWindow", "waitingL2", nullptr));
        waitingL3->setText(QCoreApplication::translate("MainWindow", "waitingL3", nullptr));
        waitingL4->setText(QCoreApplication::translate("MainWindow", "waitingL4", nullptr));
        menupiporin->setTitle(QCoreApplication::translate("MainWindow", "piporin", nullptr));
        menucreate_boat->setTitle(QCoreApplication::translate("MainWindow", "create boat", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
