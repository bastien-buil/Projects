
#include "gameviewstate.h"
#include "gridcomponent.h"
#include "gamepresenter.h"

GridComponent::GridComponent()
{

}

void GridComponent::drawGridLines(QPainter& p, QRect grid, int cellSize, int nbCellPerLine){
    for(int i = 1; i < nbCellPerLine; i++){
        p.drawLine(grid.x() + cellSize * i, grid.y(),  grid.x() + cellSize * i, grid.y() + grid.height());
        p.drawLine(grid.x(), grid.y() + cellSize * i,  grid.x() + grid.width(), grid.y() + cellSize * i);
    }
}

void GridComponent::drawGridContent(QPainter& p, QRect grid, int cellSize, int nbCellPerLine, IconDrawer* iconDrawer, QPoint selectedCell, GamePresenter* presenter){
    if(iconDrawer != nullptr){
        for(int i = 0; i < nbCellPerLine; i++){
            for(int j = 0; j < nbCellPerLine; j++){
                iconDrawer->draw(p, grid.x() + cellSize * j, grid.y() + cellSize * i, cellSize, presenter->getCell(i, j), selectedCell.x() == i && selectedCell.y() == j);
            }
        }
    }
}

void GridComponent::_draw(QPainter& p, GameViewState* state){
    p.setPen(Qt::black);

    int cellSize = state->getCellSize();
    int gridSize = cellSize * state->getPresenter()->getSizeGrid();

    QRect grid(state->getLeftPosition(), state->getTopPosition(), gridSize, gridSize);
    p.drawRect(grid);

    drawGridLines(p, grid, cellSize, state->getNbCellPerLine());
    drawGridContent(p, grid, cellSize, state->getNbCellPerLine(), state->getIconDrawer(), state->getSelectedCell(), state->getPresenter());
}
