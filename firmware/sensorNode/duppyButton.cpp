
#include <Arduino.h>
#include <duppyTypes.h>
#include "duppyButton.h"

typedef struct button_state {
    long lastDebounceTime;
    boolean lastBounceState;
    boolean lastState;
    boolean currentState;
}; 

long debounceDelay = 10;

int debounceButton(int buttonPin, struct button_state *pState)
{
    int error = SUCCESS;
    boolean reading = digitalRead(buttonPin);
  
    if (reading != pState->lastBounceState)
    {
        pState->lastDebounceTime = millis();
    } 
    
    pState->lastBounceState = reading;

    if ((millis() - pState->lastDebounceTime) > debounceDelay)
    {
        pState->currentState = reading;
    }
    else
    {
        error = ERROR_INTERNAL;
    }

    return error;
}

struct button_state upState = {0};
struct button_state downState = {0};
struct button_state backState = {0};
struct button_state enterState = {0};

int getButtonPress()
{
    int button = NO_BUTTON;

    if (debounceButton(BUTTON_UP, &upState) == SUCCESS &&
        upState.currentState != upState.lastState)
    {
        if (upState.currentState == LOW)
        {
            button = BUTTON_UP;
        }
        upState.lastState = upState.currentState;
    } 
    else if(debounceButton(BUTTON_DOWN, &downState) == SUCCESS &&
        downState.currentState != downState.lastState)
    {
        if (downState.currentState == LOW)
        {
            button = BUTTON_DOWN;
        }
        downState.lastState = downState.currentState;
    } 
    else if (debounceButton(BUTTON_BACK, &backState) == SUCCESS &&
        backState.currentState != backState.lastState)
    {
        if (backState.currentState == LOW)
        {
            button = BUTTON_BACK;
        }
        backState.lastState = backState.currentState;
    } 
    else if (debounceButton(BUTTON_ENTER, &enterState) == SUCCESS &&
        enterState.currentState != enterState.lastState)
    {
        if (enterState.currentState == LOW)
        {
            button = BUTTON_ENTER;
        }
        enterState.lastState = enterState.currentState;
    } 
    else
    {
        button = NO_BUTTON;
    }

    return button;
}

