//
// Created by kiryanenko on 05.10.19.
//

#include "SimpleTimer.h"

SimpleTimer::SimpleTimer(uint64_t interval) : _interval(interval) {
    _start = millis();
}

bool SimpleTimer::isReady() {
    return _start + _interval <= millis(); // true se o tempo já terminou
}

void SimpleTimer::setInterval(uint64_t interval) {
    _interval = interval;
}

void SimpleTimer::reset() {
    _start = millis();
}

double SimpleTimer::getTimePassed(){
    timerPassed = millis() - _start  ;
    return timerPassed;
}

double SimpleTimer::getRestTime(){
    restTimer = (_start + _interval) - millis()  ;
    return restTimer;
}

int SimpleTimer::getSeconds(){
    int t = getRestTime()*0.001;
    int sec = t%60;
    return (sec);

}

int SimpleTimer::getMinutes(){
    double t = getRestTime()*0.001;
    return (int(t/60));


    
}
