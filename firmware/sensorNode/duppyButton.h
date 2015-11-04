
#define BUTTON_UP    5
#define BUTTON_DOWN  6
#define BUTTON_BACK  7
#define BUTTON_ENTER 8
#define NO_BUTTON -1

int debounceButton(int buttonPin, struct button_state *pState);
int getButtonPress();
