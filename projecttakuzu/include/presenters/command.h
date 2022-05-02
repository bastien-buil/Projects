/**
* @file command.h
* @author Bastien Buil
* @author J****** C**** A*****
**/

#ifndef COMMAND_H
#define COMMAND_H

#include "gamemodel.h"
#include <QUndoCommand>

/**
 * @brief The ChangeCellCommand class implements a simple command to change the content of a cell
 */
class ChangeCellCommand : public QUndoCommand
{
public:
    /**
     * @brief ChangeCellCommand constructor
     * Simple constructor to set the cell content at (x,y) in the gamemodel given to the next cell content
     * @param model
     * @param x
     * @param y
     */
    ChangeCellCommand(GameModel *model, int x, int y);
    /**
     * @brief ChangeCellCommand alternate constructor
     * Constructor to set the cell content at (x,y) in the gamemodel given to the previous cell content
     * @param model
     * @param x
     * @param y
     * @param isReversed tells the command the cell content "traversal" is reversed
     */
    ChangeCellCommand(GameModel *model, int x, int y, bool isReversed);

    void redo() override;
    void undo() override;

private:
    GameModel* _model;
    int _x;
    int _y;
    bool _isReversed;
};

#endif // COMMAND_H
