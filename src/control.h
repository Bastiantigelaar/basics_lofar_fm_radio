// isr of the toggle button
extern bool menu_toggle;

void main_display();
void second_display();

void toggle_menu()
{
    menu_toggle = !menu_toggle;
    if (menu_toggle)
    {
        // inladen menu 1
        main_display();
    }
    else
    {
        // inladen menu 2
        second_display();
    }
}