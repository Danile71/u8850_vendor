/*
* Recovery touch for HUAWEI u8850 Vision by Danil_e71
* version 2
* 31.03.13
*/


#include <fcntl.h>
#include <linux/input.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "common.h"
#include <cutils/android_reboot.h>
#include <cutils/properties.h>
#include "minui/minui.h"
#include "recovery_ui.h"
#include "touch.h"


//don't disable hardware keys
#define touch_handle_key device_handle_key


#define MENU_TEXT_COLOR 0, 191, 255, 255
#define NORMAL_TEXT_COLOR 200, 200, 200, 255
#define HEADER_TEXT_COLOR NORMAL_TEXT_COLOR
#define CHAR_WIDTH BOARD_RECOVERY_CHAR_WIDTH
#define CHAR_HEIGHT BOARD_RECOVERY_CHAR_HEIGHT

static inline void draw_text(int row, const char* t){
if (t[0] != '\0')gr_text(0, (row+1)*CHAR_HEIGHT-1, t);
}

static int pos_x=-1;
static int pos_y=-1;
static int max=0;
static int start=0;
static int j = 0;
static int plus=0;
static int timeout=0;

static int touch_to_screen_y(double y)
{
return (int)(y*(double)gr_fb_height())/(double)1021;
}

int draw_touch_menu(char menu[MENU_MAX_ROWS][MENU_MAX_COLS],int menu_items, int menu_top, int menu_sel, int menu_show_start)
{
        int i = 0;
	j=0;
        int row = 0;
	start=menu_top;
	plus=menu_show_start;

        gr_color(MENU_TEXT_COLOR);
            gr_fill(0, (menu_top + menu_sel - menu_show_start) * CHAR_HEIGHT,
                    gr_fb_width(), (menu_top + menu_sel - menu_show_start + 1)*CHAR_HEIGHT+1);

            gr_color(HEADER_TEXT_COLOR);
            for (i = 0; i < menu_top; ++i) {
                draw_text(i, menu[i]);
                row++;
            }

            if (menu_items - menu_show_start + menu_top >= max_menu_rows){
                j = max_menu_rows - menu_top;max=1;}
            else{
                j = menu_items - menu_show_start;max=0;}

            gr_color(MENU_TEXT_COLOR);
            for (i = menu_show_start + menu_top; i < (menu_show_start + menu_top + j); ++i) {
                if (i == menu_top + menu_sel) {
                    gr_color(255, 255, 255, 255);
                    draw_text(i - menu_show_start , menu[i]);
                    gr_color(MENU_TEXT_COLOR);
                } else {
                    gr_color(MENU_TEXT_COLOR);
                    draw_text(i - menu_show_start, menu[i]);
                }
                row++;
                if (row >= max_menu_rows)
                    break;
            }

            gr_fill(0, row*CHAR_HEIGHT+CHAR_HEIGHT/2-1,
                    gr_fb_width(), row*CHAR_HEIGHT+CHAR_HEIGHT/2+1);
return row;
}



int touch_handle_input(int fd,struct input_event ev)
{
	if(fd!=15)return 0;

	int cur_pos=0;
	
	if(ev.type == 3)
{
	if(ev.code==48 && ev.value==255)timeout=0;
	
	if(ev.code==0x35)
	{
	pos_x=ev.value;
	return 0;
	}

	if(ev.code==0x36)
	{
	pos_y=ev.value;
	return 0;
	}
}
else
if(!ev.code && !ev.type && pos_y != -1 &&  pos_x != -1)
	{

	cur_pos=touch_to_screen_y((double)((double)pos_y/(double)CHAR_HEIGHT))-start;
	
	if((cur_pos==-1||cur_pos==j)&& max)ui_menu_select(cur_pos+plus);

	if(cur_pos>=0 && cur_pos<j)
		{
		ui_menu_select(cur_pos+plus);
			if(timeout)
			{
			timeout=0;
			pos_x=-1;
			pos_y=-1;
			return 1;
			}
			else timeout++;
		}
		
		
		
		
		
	}

return 0;
}


void touch_init()
{
pos_x=0;
pos_y=0;
}

int get_max_menu_rows(int max_menu_rows)
{
return max_menu_rows;
}


