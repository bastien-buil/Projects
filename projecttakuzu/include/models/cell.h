/**
* @file cell.h
* @author Bastien Buil
* @author J****** C**** A*****
**/
#ifndef CELL_H
#define CELL_H

/**
 * @brief The CELL_CONTENT enum defines the content of a cell at model level
 * It does not mean that the content is indeed black or white during the game
 */
enum CELL_CONTENT {
    EMPTY_CELL = 0,
    WHITE,
    BLACK
};

/**
 * @brief The Cell class defines a cell in a grid
 */
class Cell
{
public:
    Cell();
    /**
     * @brief isCorrect tells if the cell is correct or if it is part of an error in the grid
     * @return true if the cell is correct (no error) false otherwise
     */
    bool isCorrect() const;
    /**
     * @brief setCorrect sets the correctness of the cell
     * @param isCorrect
     */
    void setCorrect(bool isCorrect);
    /**
     * @brief setContent sets the content of a cell. By default the content is EMPTY_CELL
     * @param content The new content of a cell
     */
    void setContent(CELL_CONTENT content);
    /**
     * @brief setDefinitiveCell makes the cell not mutable. This is not reversible.
     * This function should only be used at the beginning of a game
     */
    void setDefinitiveCell();
    /**
     * @brief isChangeable tells if the content of the cell can be changed
     * @return true if the content of the cell can be changed or false otherwise
     */
    bool isChangeable() const;
    /**
     * @brief getContent returns the content of the cell
     * @return the content of the cell
     */
    CELL_CONTENT getContent() const;
    /**
     * @brief setCellContentToNextValue sets the content of the cell to the "next value" according to the enum (@see CELL_CONTENT)
     */
    void setCellContentToNextValue();
    /**
     * @brief setCellContentToPreviousValue sets the content of the cell to the "previous value" according to the enum (@see CELL_CONTENT)
     */
    void setCellContentToPreviousValue();

private:
    CELL_CONTENT _content;
    bool _isCorrectInGrid;
    bool _isChangeable;
    /**
     * @brief getNextCellContent returns the next cell content according to the current content of the cell and the enum CELL_CONTENT (@see CELL_CONTENT)
     * This method should became a function for the enum in a specific file
     * @return
     */
    CELL_CONTENT getNextCellContent();
    /**
     * @brief getPreviousCellContent returns the previous cell content according to the current content of the cell and the enum CELL_CONTENT (@see CELL_CONTENT)
     * This method should became a function for the enum in a specific file
     * @return
     */
    CELL_CONTENT getPreviousCellContent();
};

#endif // CELL_H
