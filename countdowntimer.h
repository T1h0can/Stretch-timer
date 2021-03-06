#ifndef COUNTDOWNTIMER_H
#define COUNTDOWNTIMER_H

// Qt
#include <QObject>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>

class CountdownTimer : public QObject
{
    Q_OBJECT
public:
    explicit CountdownTimer(QObject *parent = 0, int interval = 30);

    void start();               // Start countdown timer with 'interval' seconds
    void pauseUnpause();        // Pause/unpause timer
    void stop();                // Reset countdown timer
    int remainingTime();        // Calculates, sets and returns the remaining time

    bool isActive() const { return _countDownTimer->isActive(); }
    bool paused()   const { return _paused; }
    int interval()  const { return _interval; }
    bool setInterval(int interval);

private slots:
    void sendTimeout();         // Triggers void timeout()
    void sendTick();            // Triggers int tick(int)

signals:
    void timeout();             // Sends signal when countdown timer hit zero.
    int tick(int);              // Sends update signal.

private:
    QTimer *_countDownTimer;    // Main countdown timer.
    QTimer *_tickTimer;         // Sends update tick every second.
    QTime *_elapsedTimer;       // Needed to calculate remaining time.
    bool _paused;               // If timer is paused or not.
    int _remaining;             // Seconds remaining, needed if paused.
    int _interval;              // Time in seconds.
};

#endif // COUNTDOWNTIMER_H
