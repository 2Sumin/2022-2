/* blinking_hello.c
    modify hello1.c to display a blinking message
    use sleep function to pause the program between printing the message
    and then erasing it
*/

#include <stdio.h>
#include <curses.h>
#include <unistd.h>

int main()
{
    initscr();           
    clear(); 

    while(1) {
        move(10, 20);
        addstr("Hello, world");
        refresh();
        sleep(1);
        move(10, 20);
        addstr("            ");
        refresh();
        sleep(1);
    }         

    return 0;
}