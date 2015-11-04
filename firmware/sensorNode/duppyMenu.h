
#include <duppyTypes.h>
#include <DigoleSerial.h>

#define MAX_ENTRIES 10

typedef struct MENU_ENTRY {
    char *label;
    void (*callBack)(void);
} menu_entry;

class DuppyMenu
{
    public:
        DuppyMenu(DigoleSerialDisp *display);
        void registerEntry(char *label, void (*callBack)(void));
        void runMenu();
    
    private:
        menu_entry _menu[MAX_ENTRIES];
        DigoleSerialDisp *_myDisp;
        int _menuCount;
        int _cursor;
        int _frame;
        void drawCursor(int index);
        void initCursor();
        void changeCursor(int prevCursor, int cursor);
        void printMenuItem(char *item);
        void displayMenu(int start, int end);
};
