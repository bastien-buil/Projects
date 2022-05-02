/**
* @file gameoptionsdialog.cpp
* @author Bastien Buil
* @author J****** C**** A*****
**/
#include "gameoptionsdialog.h"
#include "ui_gameoptionsdialog.h"

#include <iostream>
#include <QButtonGroup>

GameOptionsDialog::GameOptionsDialog(int currentSizeGrid, Difficulty currentDifficulty,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameOptionsDialog),
    _sizeGridSelected(currentSizeGrid),
    _difficultySelected(currentDifficulty)
{
    ui->setupUi(this);
    ui->cancelNewGame->setDefault(true);
    setDialog();


}

void GameOptionsDialog::setDialog() {

    initCheckedButtons();

    QButtonGroup *sizeGridGroup = new QButtonGroup();
    sizeGridGroup->addButton(ui->sizeGrid6,6);
    sizeGridGroup->addButton(ui->sizeGrid8,8);
    sizeGridGroup->addButton(ui->sizeGrid10,10);
#if(QT_VERSION >= QT_VERSION_CHECK(5,15,0))
    connect(sizeGridGroup,SIGNAL(idClicked(int)),this,SLOT(onSizeGridChanged(int)));
#else
    connect(sizeGridGroup,SIGNAL(buttonClicked(int)),this,SLOT(onSizeGridChanged(int)));
#endif

    QButtonGroup *difficultyGroup = new QButtonGroup();
    difficultyGroup->addButton(ui->difficultyEasy,EASY);
    difficultyGroup->addButton(ui->difficultyHard,HARD);

#if(QT_VERSION >= QT_VERSION_CHECK(5,15,0))
    connect(difficultyGroup,SIGNAL(idClicked(int)),this,SLOT(onDifficultyChanged(int)));
#else
    connect(difficultyGroup,SIGNAL(buttonClicked(int)),this,SLOT(onDifficultyChanged(int)));
#endif


}

void GameOptionsDialog::initCheckedButtons() {

    switch (_sizeGridSelected) {
        case 10:
            ui->sizeGrid10->setChecked(true);
            break;
        case 8:
            ui->sizeGrid8->setChecked(true);
            break;
        default:
            ui->sizeGrid6->setChecked(true);
            break;
    }

    switch (_difficultySelected) {

        case EASY:
            ui->difficultyEasy->setChecked(true);
            break;
        case HARD:
            ui->difficultyHard->setChecked(true);
            break;
    }

}



void GameOptionsDialog::onSizeGridChanged(int size) {

    _sizeGridSelected = size;
}

void GameOptionsDialog::onDifficultyChanged(int difficulty) {

    _difficultySelected = (Difficulty) difficulty;
}

int GameOptionsDialog::getSizeGridSelected() {
    return _sizeGridSelected;
}

Difficulty GameOptionsDialog::getDifficultySelected() {
    return _difficultySelected;
}


GameOptionsDialog::~GameOptionsDialog()
{
    delete ui;
}
