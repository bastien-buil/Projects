/**
* @file gamepresenter.h
* @author Bastien Buil
* @author J****** C**** A*****
**/
#ifndef GAMEPRESENTER_H
#define GAMEPRESENTER_H

#include <QObject>
#include <QUndoStack>
#include "cell.h"
#include "gamemodel.h"
#include "grid.h"
#include "gameview.h"
#include "timer.h"

class GameView;

/**
 * @brief The GamePresenter class is the principal presenter of the project (MVP architecture)
 */
class GamePresenter : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief GamePresenter constructor
     * @param gameView is usually the view which created the gamepresenter
     * @param parent
     */
    GamePresenter(GameView* gameView, QObject *parent = nullptr);
    ~GamePresenter();
    /**
     * @brief getCell returns a const ref to a cell according to its coordinates @see GameModel::getCell
     * @param x
     * @param y
     * @return A const ref to a cell in the game grid
     */
    const Cell& getCell(int x, int y) const;
    /**
     * @brief getSizeGrid returns the size of the game grid @see GameModel::getSizeGrid
     * @return size of the game grid
     */
    int getSizeGrid() const;
    /**
     * @brief getStateRow returns the "state" of a row, given by its index @see LineState @see GameModel::getStateRow
     * @param rowIndex
     * @return
     */
    LineState getStateRow(int rowIndex) const;
    /**
     * @brief getStateColumn returns the "state" of a column, given by its index @see LineState @see GameModel::getStateColumn
     * @param columnIndex
     * @return
     */
    LineState getStateColumn(int columnIndex) const;
    /**
     * @brief nextCellValue adds a new command to change the cell content of the cell at (x,y) to the next value @see CELL_CONTENT
     * This method will not add a command if:
     * - The game is terminated (the board will be frozen then)
     * - The cell content can not be modified
     * @param x
     * @param y
     */
    void nextCellValue(int x, int y);
    /**
     * @brief previousCellValue adds a new command to change the cell content of the cell at (x,y) to the previous value @see CELL_CONTENT
     * This method will not add a command if:
     * - The game is terminated (the board will be frozen then)
     * - The cell content can not be modified
     */
    void previousCellValue(int x, int y);
    /**
     * @brief getTimeInMSecond returns the time elapsed in milliseconds since the beginning of the game @see Timer
     * @return
     */
    int getTimeInMSecond();
    /**
     * @brief getNumberOfUndo returns the number of undo done by the player so far
     * @return
     */
    int getNumberOfUndo();
    /**
     * @brief restartTimer restarts the timer
     * Usually used after a new game
     */
    void restartTimer();



public slots:
    /**
     * @brief redo slot to redo the last command
     * If the game is terminated, this slot does nothing
     */
    void redo();
    /**
     * @brief undo slot to undo the last command
     * If the game is terminated, this slot does nothing
     */
    void undo();
    /**
     * @brief showGameOptionsDialog slot displays a window to start a new game
     */
    void showNewGameOptionsDialog();
    /**
     * @brief showAboutDialog slot displays a small window with authors' name
     */
    void showAboutDialog();
    /**
     * @brief showRulesDialog slot displays a window with the Takuzu's rules
     */
    void showRulesDialog();
    /**
     * @brief changeIcon slot changes the icon of the pawn according to the selected icon
     * @param action
     */
    void changeIcon(QAction* action);
    /**
     * @brief setTooltip slot enables or disables the tooltips display according to the selected option
     * @param action
     */
    void setTooltip(QAction* action);

private:
    Timer* _timer;
    GameView* _gameView;
    GameModel* _gameModel;
    /**
     * @brief _undoStack stores the commands during the game and navigate through it with redo and undo
     */
    QUndoStack* _undoStack;
    bool _isGameTerminated;
    /**
     * @brief resetGame terminates the current game (finished or not) and starts a new one
     * @param newSizeGrid The grid size of the new game
     * @param newDifficulty The difficulty of the new game
     */
    void resetGame(int newSizeGrid,Difficulty newDifficulty);
    /**
     * @brief isGameTerminated checks if the game is finished
     * Obviously, it asks several methods of the model
     * @return
     */
    bool isGameTerminated();
    /**
     * @brief showEndDialog shows a dialog if the game is finished (e.g won)
     */
    void showEndDialog();
    /**
     * @brief updateGame updates the view and the presenter at each action of the player
     * Checks if the game is terminated to launch appropriate methods
     */
    void updateGame();

};


#endif // GAMEPRESENTER_H
