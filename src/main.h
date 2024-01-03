#ifndef MAIN_H
#define MAIN_H

enum class TimerMode
{
    SuddenDeath,
    Fischer,
    Hourglass,
    ByoYomi,
    TimerMode_MAX
};

enum class ButtonState
{
    None,
    Hold,
    Select,
    Left,
    Right,
    Up,
    Down,
    ButtonState_MAX
};

enum class ClockState
{
    Welcome,
    ModeSet,
    TimeSet,
    Ready,
    White,
    Black,
    Paused,
    GameOver,
    ClockState_MAX
};

void setup();
ButtonState readButtonState();
ButtonState readButton();
void cycleStateLeft();
void moveToNextState();
void cycleSelectionLeft();
void cycleSelectionRight();
void printState();
void printTimerMode();
void loop();

#endif // MAIN_H