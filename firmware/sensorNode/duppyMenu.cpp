
#include "duppyMenu.h"

#define MAX_DISPLAYED 3

DuppyMenu::DuppyMenu(DigoleSerialDisp *display)
{
    _myDisp = display;
    _menuCount = 0; 
    _frame = 0;
}

void DuppyMenu::registerEntry(char *label, void (*callBack)(void))
{
    _menu[_menuCount].label = label;
    _menu[_menuCount].callBack = callBack;
    _menuCount++;
}

void DuppyMenu::runMenu()
{
    int button = NO_BUTTON;
    int prevCursor = 0;
    int cursor = 0;
    long now = 0;

    displayMenu(0, MAX_DISPLAYED);
    initCursor();

    now = millis();

    while(1)
    {
        if (millis() > (now + 10000))
        {
            _myDisp->clearScreen();
            _menu[0].callBack(); 
            goto resetMenu;
        }	

        button = getButtonPress();
        if (button != NO_BUTTON)
        {
            now = millis();
            switch(button)
            {
                case BUTTON_UP:
                    if (cursor > 0)
                    {
                        prevCursor = cursor;
                        cursor--;
                        changeCursor(prevCursor, cursor);
                    }
                    break;
                case BUTTON_DOWN:
                    if (cursor < _menuCount-1)
                    {
                        prevCursor = cursor;
                        cursor++;
                        changeCursor(prevCursor, cursor);
                    }
                    break;
                case BUTTON_BACK:
                    break;
                case BUTTON_ENTER:
                    _myDisp->clearScreen();
                    _menu[cursor].callBack(); 
resetMenu:
                    displayMenu(0, MAX_DISPLAYED);
                    initCursor();
                    cursor = 0;
                    prevCursor = 0;
                    button = NO_BUTTON;
                    now = millis();
                    break;
            }
        }
    }
}

void DuppyMenu::drawCursor(int index)
{
    _myDisp->drawBox(0, 32 * index, 160, 32);
}

void DuppyMenu::initCursor()
{
    _myDisp->setMode('~');
    drawCursor(0);
    _frame = 0;
}

void DuppyMenu::changeCursor(int prevCursor, int cursor)
{
    if ((prevCursor < cursor) && (_frame == MAX_DISPLAYED))
    {
        displayMenu(cursor-MAX_DISPLAYED, cursor);
        _myDisp->setMode('~');
        drawCursor(MAX_DISPLAYED);
    }
    else if ((prevCursor > cursor) && (_frame == 0))
    {
        displayMenu(cursor,cursor+MAX_DISPLAYED);
        _myDisp->setMode('~');
        drawCursor(0);
    }
    else
    {
        if (prevCursor < cursor)
        {
            _myDisp->setMode('~');
            drawCursor(_frame);
            _frame++;
            drawCursor(_frame);
        }
        else if (prevCursor > cursor)
        {
            _myDisp->setMode('~');
            drawCursor(_frame);
            _frame--;
            drawCursor(_frame);
        }
    }
}

void DuppyMenu::printMenuItem(char *item)
{
    _myDisp->setFont(51);
    _myDisp->print(item);
    _myDisp->nextTextLine();
    _myDisp->setFont(10);
    _myDisp->nextTextLine();
}

void DuppyMenu::displayMenu(int start, int end)
{
    int i = 0;
    _myDisp->clearScreen();
    _myDisp->setPrintPos(0, 1, _TEXT_);
    for (i=start; i<=end && i<_menuCount; i++)
    {
        printMenuItem(_menu[i].label);
    }
}

