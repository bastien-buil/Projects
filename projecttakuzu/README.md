# Takuzu GNU/Linux

The project is a video game in C++ with Qt based on the game [Takuzu](https://fr.wikipedia.org/wiki/Takuzu). 

The principle of the game is to fill in a grid with two kinds of symbols while respecting:
- in each line and column, there must be the same number of the two symbols,
- all lines and columns must be different,
- no more than 2 identical numbers side by side.


### Authors: Bastien Buil - J****** C**** A*****

## Project content

The game is functionnal, includind the following functionnalities:

- Display of rows/columns correctness 
- Display of errors in rows/columns (including identical rows/columns)
- Display of the remaining paws 
- Display of the number of undo
- Display of the time elapsed
- Tooltips for some displays (you can disable them)
- Ability to start a new game from the menu with a specific window
- Second set of icons
- Rules in the menu
- Shortcuts (undo/redo, new game...)
- Translation to french

## Quick overview of the project structure

We developped the game with the MVP architectural pattern. We kept Qt classes
away from the model class in order to clearly separate the model from the view
(There is one exception for gridparser, it is for ease reason and easy
 compatibility with gridreader)
Some Qt classes are used in the presenter. In particular, we used QUndoCommand
to implement the Command pattern in our game (the only command is to change the
		content of a cell)

The game owns a takuzu grid, itself owns cells. We used the convention
"White/Black" to name the pawns of each symbol. The takuzu grid knows the rules
of the game and therefore can check itself. The gamemodel encapsulates the grid
and controls the access. 

The presenter owns a gamemodel but also owns a timer, which is a QObject for
ease reason and also to show we can use slots and signals. 

The view is only a single widget, which is drawed programmatically. We breakdown
each part of the view (grid, info panel, info lines) to separate each
responsability while keeping a common behavior (draw itself for example). The
refactor of the view should had been continued for each part of the project
(especially some enums in the model should have their proper file for instance). 
Generally speaking, each class tends to have a single responsability.

## Bugs

No bugs have been found at the moment
