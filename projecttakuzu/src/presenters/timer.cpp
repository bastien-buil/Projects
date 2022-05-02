/**
* @file timer.cpp
* @author Bastien Buil
* @author J****** C**** A*****
**/
#include "timer.h"
#include <QDebug>

Timer::Timer(QObject *parents) : QObject(parents)
{
    _elapsedTimer = new QElapsedTimer();
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &Timer::update);
    _timer->start(100);



}

Timer::~Timer(){
    delete _elapsedTimer;
}


void Timer::startTimer(){
    if(!_timerRunning){
        _timerRunning = true;
        _elapsedTimer->start();
    }
}

void Timer::stopTimer(){
    if(_timerRunning){
        _timerRunning = false;
        _time += _elapsedTimer->elapsed();
    }
}

void Timer::clearTimer(){
    if(!_timerRunning){
            _time = 0;
            _timerRunning = false;
        }
}

int Timer::getCurrentTimeInMs(){
    if(_timerRunning){
        return _time + _elapsedTimer->elapsed();
    }
    return _time;
}

bool Timer::isTimerStarted(){
    return _timerRunning;
}

void Timer::update(){
    int time = getCurrentTimeInMs() / 1000;
    if(time != _lastCheckTimeInSecond){
        _lastCheckTimeInSecond = time;
        emit timeChanged();
    }
}
