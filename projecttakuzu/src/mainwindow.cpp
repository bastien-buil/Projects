/**
* @file mainwindow.cpp
* @author Bastien Buil
* @author J****** C**** A*****
**/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamepresenter.h"
#include "roundicondrawer.h"
#include "icondrawertype.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    GamePresenter* presenter = new GamePresenter(ui->centralwidget, ui->centralwidget);
    ui->centralwidget->setPresenter(presenter);
    ui->centralwidget->setIconDrawer(new RoundIconDrawer());

    QActionGroup* iconTypeGroup = new QActionGroup( this );
    ui->actionCircle->setChecked(true);
    ui->actionCircle->setActionGroup(iconTypeGroup);
    ui->actionCircle->setData(IconDrawerType::ROUND_ICONS);
    ui->actionGNUTux->setActionGroup(iconTypeGroup);
    ui->actionGNUTux->setData(IconDrawerType::GNU_TUX);
    connect(iconTypeGroup, SIGNAL(triggered(QAction*)), presenter, SLOT(changeIcon(QAction*)));


    QActionGroup* tooltipGroup = new QActionGroup( this );
    ui->actionTooltipEnable->setChecked(true);
    ui->actionTooltipEnable->setActionGroup(tooltipGroup);
    ui->actionTooltipEnable->setData(false);
    ui->actionTooltipDisable->setActionGroup(tooltipGroup);
    ui->actionTooltipDisable->setData(true);
    connect(tooltipGroup, SIGNAL(triggered(QAction*)), presenter, SLOT(setTooltip(QAction*)));

    connect(ui->actionRedo, SIGNAL(triggered()), presenter, SLOT(redo()));
    connect(ui->actionUndo, SIGNAL(triggered()), presenter, SLOT(undo()));
    connect(ui->actionNew,SIGNAL(triggered()),presenter,SLOT(showNewGameOptionsDialog()));
    connect(ui->actionAbout,SIGNAL(triggered()),presenter,SLOT(showAboutDialog()));
    connect(ui->actionRules,SIGNAL(triggered()),presenter,SLOT(showRulesDialog()));
    ui->retranslateUi(this);

}


MainWindow::~MainWindow()
{
    delete ui;
}

