#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>

#include "main.h"

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

ClockState currentState;
ClockState previousState;
ButtonState currentButton;
ButtonState lastButton;
TimerMode timerMode;
TimerMode previousTimerMode;

int debounceCycles = 0;

void setup()
{
    lcd.begin(16, 2);
    currentButton = ButtonState::None;
    lastButton = ButtonState::None;
    lcd.print("  Chess Clock   ");
    lcd.setCursor(0, 1);
    lcd.print("    < v0.1 >    ");
    delay(2000);
    previousState = ClockState::GameOver;
    currentState = ClockState::ModeSet;
    previousTimerMode = TimerMode::TimerMode_MAX;
    timerMode = TimerMode::SuddenDeath;
}

void loop()
{
    if (debounceCycles > 0)
    {
        debounceCycles--;
        delay(10);
        return;
    }

    auto button = readButton();
    switch (button)
    {
        case ButtonState::Left:
            cycleSelectionLeft();
            break;

        case ButtonState::Right:
            cycleSelectionRight();
            break;

        case ButtonState::Select:
            moveToNextState();
            break;
        
        default:
            break;
    }

    if (currentState != previousState)
    {
        printState();
        previousState = currentState;
    }

    if(timerMode != previousTimerMode)
    {
        printTimerMode();
        previousTimerMode = timerMode;
    }
}


ButtonState readButton()
{
    // Read the button state from the analog input.
    auto value = readButtonState();
    if (value == lastButton)
    {
        return ButtonState::Hold;
    }
    else
    {
        debounceCycles = 5;
        lastButton = value;
        return value;
    }
}

void moveToNextState()
{
    if (currentState == ClockState::GameOver) return;
    currentState = (ClockState)((int)currentState + 1);
}

void cycleSelectionLeft()
{
    switch (currentState)
    {
        case ClockState::ModeSet:
            timerMode = (timerMode == (TimerMode)0) 
                ? (TimerMode)((int)TimerMode::TimerMode_MAX - 1) 
                : (TimerMode)((int)timerMode - 1);
            break;
        case ClockState::TimeSet:
            break;
        default: break;
    }
}

void cycleSelectionRight()
{
    switch (currentState)
    {
        case ClockState::ModeSet:
            timerMode = (timerMode == (TimerMode)((int)TimerMode::TimerMode_MAX - 1))
                ? (TimerMode)0 
                : (TimerMode)((int)timerMode + 1);
            break;
        default: 
            break;
    }
}

ButtonState readButtonState()
{
    auto buttonValue = analogRead(0);
    if (buttonValue >= 900)             return ButtonState::None;
    else if (buttonValue < 50)          return ButtonState::Right;
    else if (buttonValue < 150)         return ButtonState::Up;
    else if (buttonValue < 300)         return ButtonState::Down;
    else if (buttonValue < 600)         return ButtonState::Left;
    else                                return ButtonState::Select;
}

void printState()
{
    lcd.clear();
    delay(200);
    switch (currentState)
    {
        case ClockState::ModeSet:       lcd.print("Select Mode:    ");  break;
        case ClockState::TimeSet:       lcd.print("Select Time:    ");  break;
        case ClockState::Ready:         lcd.print("Ready to Start  ");  break;
        case ClockState::White:         lcd.print("White to play   ");  break;
        case ClockState::Black:         lcd.print("Black to play   ");  break;
        case ClockState::Paused:        lcd.print("     Paused     ");  break;
        case ClockState::GameOver:      lcd.print("   Game Over!   ");  break;
        
        case ClockState::ClockState_MAX:
            break;
    }
}

void printTimerMode()
{
    lcd.setCursor(0, 1);
    switch (timerMode)
    {
        case TimerMode::SuddenDeath:    lcd.print(" Sudden Death   ");  break;
        case TimerMode::Fischer:        lcd.print(" Fischer        ");  break;
        case TimerMode::Hourglass:      lcd.print(" Hourglass      ");  break;
        case TimerMode::ByoYomi:        lcd.print(" Byo-Yomi       ");  break;
        
        case TimerMode::TimerMode_MAX:                                  
            break;
    }
}
