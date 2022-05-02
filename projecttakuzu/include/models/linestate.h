/**
* @file linestate.h
* @author Bastien Buil
* @author J****** C**** A*****
**/
#ifndef LINESTATE_H
#define LINESTATE_H

/**
 * @brief The LineState class represents the state of a line in a takuzu grid
 * It gives the number of remaining pawns of each color to put in the line and tells if there is at least one error on the line
 * It is almost a "named" tuple
 */
class LineState
{
public:
    LineState(int remainingWhitePawn, int remainingBlackPawn, int isLineCorrect);
    int getRemainingWhitePawn() const;
    int getRemainingBlackPawn() const;
    bool isCorrect() const;

private:
    int _remainingWhitePawn;
    int _remainingBlackPawn;
    bool _isLineCorrect;
};

#endif // LINESTATE_H
