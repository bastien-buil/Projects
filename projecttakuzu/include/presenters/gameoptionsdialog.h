/**
* @file gameoptionsdialog.h
* @author Bastien Buil
* @author J****** C**** A*****
**/
#ifndef GAMEOPTIONSDIALOG_H
#define GAMEOPTIONSDIALOG_H

#include <QDialog>
#include "grid.h"

namespace Ui {
class GameOptionsDialog;
}
/**
 * @brief The GameOptionsDialog class defines the behavior of the dialog for a new game
 * This class should probably be in the view (from the MVP pattern)
 */
class GameOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief GameOptionsDialog constructor. We give the currents game "parameters", grid size and difficulty to set them as default options
     * @param currentSizeGrid
     * @param currentDifficulty
     * @param parent
     */
    explicit GameOptionsDialog(int currentSizeGrid, Difficulty currentDifficulty, QWidget *parent = nullptr);
    ~GameOptionsDialog();
    /**
     * @brief getSizeGridSelected returns the grid size selected by the player
     * @return
     */
    int getSizeGridSelected();
    /**
     * @brief getDifficultySelected returns the difficulty selected by the player
     * @return
     */
    Difficulty getDifficultySelected();
public slots:
    /**
     * @brief onSizeGridChanged changes the selected size grid
     * @param size The new size selected
     */
    void onSizeGridChanged(int size);
    /**
     * @brief onDifficultyChanged changes the selected difficulty
     * @param difficulty The new difficulty, as an int (but it is converted to an enum after)
     */
    void onDifficultyChanged(int difficulty);

private:
    Ui::GameOptionsDialog *ui;
    int _sizeGridSelected;
    Difficulty _difficultySelected;
    /**
     * @brief setDialog sets the dialog parameters and details
     */
    void setDialog();
    /**
     * @brief initCheckedButtons checks the default buttons in the window
     */
    void initCheckedButtons();
};

#endif // GAMEOPTIONSDIALOG_H
