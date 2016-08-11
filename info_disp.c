#include "menu_objs.h"
#include <sys/file.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "plcm_ioctl.h"



int main(int argc, char *argv[])
{
	unsigned char Keypad_Value = 0;
	unsigned char detect_dir;
	unsigned char Cur_Display = 0x08; // Current Display On/Off Ctrl
	unsigned char Ctrl = 0;

	if(argc == 2)
        {
                if(strcmp("-stop", argv[1]) == 0)
                {
                        printf("plcm_drv thread has been stopped.\n");
                        printf("sled_drv : PLCM_IOCTL_STOP_THREAD\n");
		        goto out;
        		}
		}
	// creates all the menus and items in the menu_objs c file
	initialize_menus_and_items();
    current_menu = main_menu;
   
    devfd = open("/dev/plcm_drv", O_RDWR);
	if(devfd == -1)
	{
		printf("Can't open /dev/plcm_drv\n");
		return -1;
	}

	// stops cursor from blinking 
	ioctl(devfd, PLCM_IOCTL_DISPLAY_B, 0);
	ioctl(devfd, PLCM_IOCTL_DISPLAY_C, 0);

    
	// shows the titles of each of the menu items
	//ioctl(devfd, PLCM_IOCTL_SET_LINE, 1);
	show_menu(current_menu);

// this  do while loop checks for button preses and then executes the functions on each menu item
 do{
	usleep(100);
    Keypad_Value = ioctl(devfd, PLCM_IOCTL_GET_KEYPAD, 0);
    detect_dir=(Keypad_Value & 0x68);

	    if(detect_dir == 0x40){

			on_btn_press(current_menu.item1);
		}else if(detect_dir == 0x60 ){
			
     		on_btn_press(current_menu.item2);	
		}else if(detect_dir == 0x48){

			on_btn_press(current_menu.item3);
		}else if(detect_dir == 0x68){

			on_btn_press(current_menu.item4);
		}
	}while(1);
out:
	close(devfd);
	return 0;
}