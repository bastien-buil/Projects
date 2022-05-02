/**
* @file timer.h
* @author Bastien Buil
* @author J****** C**** A*****
**/
#ifndef TIMER_H
#define TIMER_H

#include <QElapsedTimer>
#include <QTimer>

/**
 * @brief The Timer class measures the in-game time and gives appropriate services to handle the time and get it
 */
class Timer : public QObject
{
    Q_OBJECT
public:
    Timer(QObject *parent = nullptr);
    ~Timer();
    /**
     * @brief startTimer starts the timer if the timer is not already running
     */
    void startTimer();
    /**
     * @brief stopTimer stops the timer if the timer is not already stopped
     */
    void stopTimer();
    /**
     * @brief clearTimer resets the timer to 0 if the timer is stopped
     */
    void clearTimer();
    /**
     * @brief getCurrentTimeInMs returns the time elapsed since the last reset of the timer in milliseconds
     * @return
     */
    int getCurrentTimeInMs();
    bool isTimerStarted();

signals:
    /**
     * @brief timeChanged signal sent if the time has changed
     */
    void timeChanged();

private:
    QElapsedTimer* _elapsedTimer;
    QTimer* _timer;
    bool _timerRunning = false;
    int _time = 0;
    /**
     * @brief _lastCheckTimeInSecond saves the time with a second precision.
     * If the current time of the timer is different from this variable, we consider the time has changed
     */
    int _lastCheckTimeInSecond = 0;

private slots:
    /**
     * @brief update slot used when the inner timer of the class timeouts
     */
    void update();
};

#endif // TIMER_H
