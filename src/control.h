// isr of the toggle button
extern bool menu_toggle;

void main_display();
void second_display();

void toggle_menu()
{
    menu_toggle = !menu_toggle;   
}
bool get_menu(){
    return menu_toggle;
}