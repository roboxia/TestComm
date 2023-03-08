/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QHBoxLayout *horizontalLayout_7;
    QSplitter *splitter_4;
    QSplitter *splitter;
    QTextBrowser *rxBrowser;
    QTextBrowser *logBrowser;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_7;
    QSplitter *splitter_3;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_5;
    QTableView *rxItemTable;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_4;
    QTableView *itemTable;
    QHBoxLayout *horizontalLayout_6;
    QHBoxLayout *horizontalLayout_2;
    QLabel *rxLab;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QPushButton *addRx;
    QPushButton *rmRx;
    QCheckBox *checkBox;
    QSpacerItem *horizontalSpacer;
    QLabel *txLab;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *saveFram;
    QPushButton *rmFram;
    QPushButton *addTx;
    QPushButton *rmTx;
    QCheckBox *checkBox_2;
    QSplitter *splitter_2;
    QTableView *rxFramTable;
    QTableView *framTable;
    QHBoxLayout *horizontalLayout_3;
    QFrame *line_8;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout;
    QComboBox *choicePort;
    QComboBox *baudNum;
    QCheckBox *openPort;
    QPushButton *clearRx;
    QFrame *line_6;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_2;
    QFrame *line_5;
    QCheckBox *timeShow;
    QLabel *label;
    QSpinBox *spinBox;
    QFrame *line;
    QFrame *line_2;
    QCheckBox *crcCheck;
    QLabel *labelStart;
    QSpinBox *crcStart;
    QLabel *labelEnd;
    QSpinBox *crcEnd;
    QFrame *line_4;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout_3;
    QFrame *line_7;
    QFrame *line_3;
    QCheckBox *hexTx;
    QPushButton *tx;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(1576, 1030);
        horizontalLayout_7 = new QHBoxLayout(Widget);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        splitter_4 = new QSplitter(Widget);
        splitter_4->setObjectName(QStringLiteral("splitter_4"));
        splitter_4->setOrientation(Qt::Vertical);
        splitter = new QSplitter(splitter_4);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        rxBrowser = new QTextBrowser(splitter);
        rxBrowser->setObjectName(QStringLiteral("rxBrowser"));
        splitter->addWidget(rxBrowser);
        logBrowser = new QTextBrowser(splitter);
        logBrowser->setObjectName(QStringLiteral("logBrowser"));
        splitter->addWidget(logBrowser);
        splitter_4->addWidget(splitter);
        layoutWidget = new QWidget(splitter_4);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        verticalLayout_7 = new QVBoxLayout(layoutWidget);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        splitter_3 = new QSplitter(layoutWidget);
        splitter_3->setObjectName(QStringLiteral("splitter_3"));
        splitter_3->setOrientation(Qt::Horizontal);
        layoutWidget1 = new QWidget(splitter_3);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        verticalLayout_5 = new QVBoxLayout(layoutWidget1);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        rxItemTable = new QTableView(layoutWidget1);
        rxItemTable->setObjectName(QStringLiteral("rxItemTable"));

        verticalLayout_5->addWidget(rxItemTable);

        splitter_3->addWidget(layoutWidget1);
        layoutWidget2 = new QWidget(splitter_3);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        verticalLayout_4 = new QVBoxLayout(layoutWidget2);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        itemTable = new QTableView(layoutWidget2);
        itemTable->setObjectName(QStringLiteral("itemTable"));

        verticalLayout_4->addWidget(itemTable);

        splitter_3->addWidget(layoutWidget2);

        verticalLayout_7->addWidget(splitter_3);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        rxLab = new QLabel(layoutWidget);
        rxLab->setObjectName(QStringLiteral("rxLab"));

        horizontalLayout_2->addWidget(rxLab);

        pushButton_2 = new QPushButton(layoutWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_2->addWidget(pushButton_2);

        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        addRx = new QPushButton(layoutWidget);
        addRx->setObjectName(QStringLiteral("addRx"));

        horizontalLayout_2->addWidget(addRx);

        rmRx = new QPushButton(layoutWidget);
        rmRx->setObjectName(QStringLiteral("rmRx"));

        horizontalLayout_2->addWidget(rmRx);

        checkBox = new QCheckBox(layoutWidget);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        horizontalLayout_2->addWidget(checkBox);


        horizontalLayout_6->addLayout(horizontalLayout_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        txLab = new QLabel(layoutWidget);
        txLab->setObjectName(QStringLiteral("txLab"));

        horizontalLayout_6->addWidget(txLab);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        saveFram = new QPushButton(layoutWidget);
        saveFram->setObjectName(QStringLiteral("saveFram"));

        horizontalLayout_5->addWidget(saveFram);

        rmFram = new QPushButton(layoutWidget);
        rmFram->setObjectName(QStringLiteral("rmFram"));

        horizontalLayout_5->addWidget(rmFram);

        addTx = new QPushButton(layoutWidget);
        addTx->setObjectName(QStringLiteral("addTx"));

        horizontalLayout_5->addWidget(addTx);

        rmTx = new QPushButton(layoutWidget);
        rmTx->setObjectName(QStringLiteral("rmTx"));

        horizontalLayout_5->addWidget(rmTx);

        checkBox_2 = new QCheckBox(layoutWidget);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        horizontalLayout_5->addWidget(checkBox_2);


        horizontalLayout_6->addLayout(horizontalLayout_5);


        verticalLayout_7->addLayout(horizontalLayout_6);

        splitter_4->addWidget(layoutWidget);
        splitter_2 = new QSplitter(splitter_4);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        rxFramTable = new QTableView(splitter_2);
        rxFramTable->setObjectName(QStringLiteral("rxFramTable"));
        splitter_2->addWidget(rxFramTable);
        framTable = new QTableView(splitter_2);
        framTable->setObjectName(QStringLiteral("framTable"));
        splitter_2->addWidget(framTable);
        splitter_4->addWidget(splitter_2);

        horizontalLayout_7->addWidget(splitter_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));

        horizontalLayout_7->addLayout(horizontalLayout_3);

        line_8 = new QFrame(Widget);
        line_8->setObjectName(QStringLiteral("line_8"));
        line_8->setFrameShape(QFrame::VLine);
        line_8->setFrameShadow(QFrame::Sunken);

        horizontalLayout_7->addWidget(line_8);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        choicePort = new QComboBox(Widget);
        choicePort->setObjectName(QStringLiteral("choicePort"));

        verticalLayout->addWidget(choicePort);

        baudNum = new QComboBox(Widget);
        baudNum->setObjectName(QStringLiteral("baudNum"));

        verticalLayout->addWidget(baudNum);

        openPort = new QCheckBox(Widget);
        openPort->setObjectName(QStringLiteral("openPort"));

        verticalLayout->addWidget(openPort);

        clearRx = new QPushButton(Widget);
        clearRx->setObjectName(QStringLiteral("clearRx"));

        verticalLayout->addWidget(clearRx);


        verticalLayout_6->addLayout(verticalLayout);

        line_6 = new QFrame(Widget);
        line_6->setObjectName(QStringLiteral("line_6"));
        line_6->setFrameShape(QFrame::HLine);
        line_6->setFrameShadow(QFrame::Sunken);

        verticalLayout_6->addWidget(line_6);

        verticalSpacer = new QSpacerItem(20, 166, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        line_5 = new QFrame(Widget);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line_5);

        timeShow = new QCheckBox(Widget);
        timeShow->setObjectName(QStringLiteral("timeShow"));

        verticalLayout_2->addWidget(timeShow);

        label = new QLabel(Widget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_2->addWidget(label);

        spinBox = new QSpinBox(Widget);
        spinBox->setObjectName(QStringLiteral("spinBox"));

        verticalLayout_2->addWidget(spinBox);

        line = new QFrame(Widget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line);

        line_2 = new QFrame(Widget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line_2);

        crcCheck = new QCheckBox(Widget);
        crcCheck->setObjectName(QStringLiteral("crcCheck"));

        verticalLayout_2->addWidget(crcCheck);

        labelStart = new QLabel(Widget);
        labelStart->setObjectName(QStringLiteral("labelStart"));

        verticalLayout_2->addWidget(labelStart);

        crcStart = new QSpinBox(Widget);
        crcStart->setObjectName(QStringLiteral("crcStart"));

        verticalLayout_2->addWidget(crcStart);

        labelEnd = new QLabel(Widget);
        labelEnd->setObjectName(QStringLiteral("labelEnd"));

        verticalLayout_2->addWidget(labelEnd);

        crcEnd = new QSpinBox(Widget);
        crcEnd->setObjectName(QStringLiteral("crcEnd"));

        verticalLayout_2->addWidget(crcEnd);


        verticalLayout_6->addLayout(verticalLayout_2);

        line_4 = new QFrame(Widget);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        verticalLayout_6->addWidget(line_4);

        verticalSpacer_2 = new QSpacerItem(20, 165, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        line_7 = new QFrame(Widget);
        line_7->setObjectName(QStringLiteral("line_7"));
        line_7->setFrameShape(QFrame::HLine);
        line_7->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line_7);

        line_3 = new QFrame(Widget);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line_3);

        hexTx = new QCheckBox(Widget);
        hexTx->setObjectName(QStringLiteral("hexTx"));
        hexTx->setCheckable(true);
        hexTx->setChecked(true);

        verticalLayout_3->addWidget(hexTx);

        tx = new QPushButton(Widget);
        tx->setObjectName(QStringLiteral("tx"));
        tx->setMinimumSize(QSize(0, 200));

        verticalLayout_3->addWidget(tx);


        verticalLayout_6->addLayout(verticalLayout_3);


        horizontalLayout_7->addLayout(verticalLayout_6);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        rxLab->setText(QApplication::translate("Widget", "\346\216\245\346\224\266\345\270\247\347\274\226\350\276\221", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("Widget", "\346\267\273\345\212\240\345\270\247", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Widget", "\345\210\240\351\231\244\345\270\247", Q_NULLPTR));
        addRx->setText(QApplication::translate("Widget", "add rx item", Q_NULLPTR));
        rmRx->setText(QApplication::translate("Widget", "rm rx item", Q_NULLPTR));
        checkBox->setText(QApplication::translate("Widget", "HEX\346\230\276\347\244\272", Q_NULLPTR));
        txLab->setText(QApplication::translate("Widget", "\345\217\221\351\200\201\345\270\247\347\274\226\350\276\221", Q_NULLPTR));
        saveFram->setText(QApplication::translate("Widget", "\346\267\273\345\212\240\345\270\247", Q_NULLPTR));
        rmFram->setText(QApplication::translate("Widget", "\345\210\240\351\231\244\345\270\247", Q_NULLPTR));
        addTx->setText(QApplication::translate("Widget", "add tx item", Q_NULLPTR));
        rmTx->setText(QApplication::translate("Widget", "rm tx item", Q_NULLPTR));
        checkBox_2->setText(QApplication::translate("Widget", "HEX\346\230\276\347\244\272", Q_NULLPTR));
        baudNum->clear();
        baudNum->insertItems(0, QStringList()
         << QApplication::translate("Widget", "115200", Q_NULLPTR)
        );
        openPort->setText(QApplication::translate("Widget", "\346\211\223\345\274\200\344\270\262\345\217\243", Q_NULLPTR));
        clearRx->setText(QApplication::translate("Widget", "\346\270\205\351\231\244\346\216\245\346\224\266", Q_NULLPTR));
        timeShow->setText(QApplication::translate("Widget", "\346\227\266\351\227\264\346\210\263", Q_NULLPTR));
        label->setText(QApplication::translate("Widget", "\351\227\264\351\232\224", Q_NULLPTR));
        crcCheck->setText(QApplication::translate("Widget", "MB CRC16", Q_NULLPTR));
        labelStart->setText(QApplication::translate("Widget", "\350\265\267\345\247\213\345\255\227\350\212\202", Q_NULLPTR));
        labelEnd->setText(QApplication::translate("Widget", "\347\273\223\346\235\237\345\255\227\350\212\202", Q_NULLPTR));
        hexTx->setText(QApplication::translate("Widget", "HEX\345\217\221\351\200\201", Q_NULLPTR));
        tx->setText(QApplication::translate("Widget", "\345\217\221\351\200\201", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
