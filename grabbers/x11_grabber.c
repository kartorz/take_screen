/* 
 *   Copyright (c) 2013
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *    If you want to get more infomation about,see <http://www.gnu.org/licenses/>.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "log.h"
#include "grabber.h"

typedef unsigned long Pixel;

Display *dpy = NULL;
int      screen = 0;
int img_format = ZPixmap;
int user_colormap = 0;
int is_index_color = 0;

static const char display_name[] = ":0.0";

static void init_x11()
{
    dpy = XOpenDisplay(display_name);
    if(dpy == NULL)
    {
        fatalerr("init_X11:  unable to open display '%s'\n",
                     XDisplayName(display_name));
    }
    screen = XDefaultScreen(dpy);
}


static int init_grabber()
{
    init_x11();
    return 0;
}

void close_display(void)
{
    if (dpy == NULL)
      return;
      
    XCloseDisplay(dpy);
    dpy = NULL;
}

#if 0
static Window select_window(void)
{
  int status;
  Cursor cursor;
  XEvent event;
  Window target_win = None, root = RootWindow(dpy,screen);
  int buttons = 0;

  /* Make the target cursor */
  cursor = XCreateFontCursor(dpy, XC_crosshair);

  /* Grab the pointer using target cursor, letting it room all over */
  status = XGrabPointer(dpy, root, False,
			ButtonPressMask|ButtonReleaseMask, GrabModeSync,
			GrabModeAsync, root, cursor, CurrentTime);
  if (status != GrabSuccess) Fatal_Error("Can't grab the mouse.");

  /* Let the user select a window... */
  while ((target_win == None) || (buttons != 0)) {
    /* allow one more event */
    XAllowEvents(dpy, SyncPointer, CurrentTime);
    XWindowEvent(dpy, root, ButtonPressMask|ButtonReleaseMask, &event);
    switch (event.type) {
    case ButtonPress:
      if (target_win == None) {
	target_win = event.xbutton.subwindow; /* window selected */
	if (target_win == None) target_win = root;
      }
      buttons++;
      break;
    case ButtonRelease:
      if (buttons > 0) /* there may have been some down before we started */
	buttons--;
       break;
    }
  } 

  XUngrabPointer(dpy, CurrentTime);      /* Done with pointer */

  if (!descend || (target_win == root))
    return(target_win);

  target_win = Find_Client(dpy, root, target_win);

  return(target_win);
}
#endif

static int image_size(XImage * image)
{
    if (image->format != ZPixmap)
      return(image->bytes_per_line * image->height * image->depth);

    return(image->bytes_per_line * image->height);
}

static int find_colors(Visual *vis, Colormap cmap, XColor **colors)
{
    int i,ncolors ;

    ncolors = vis->map_entries;

    if (!(*colors = (XColor *) malloc (sizeof(XColor) * ncolors)))
      fatalerr("Out of memory!");

    if (vis->class == DirectColor || vis->class == TrueColor) {
        is_index_color = 0;
        LOG(LOG_DEBUG, "directcolor or truecolor of visul class is used\n");
	Pixel red, green, blue, red1, green1, blue1;

	red = green = blue = 0;

#define lowbit(x) ((x) & (~(x) + 1))

	red1 = lowbit(vis->red_mask);
	green1 = lowbit(vis->green_mask);
	blue1 = lowbit(vis->blue_mask);
	for (i=0; i<ncolors; i++) {
	  (*colors)[i].pixel = red|green|blue;
	  (*colors)[i].pad = 0;
	  red += red1;
	  if (red > vis->red_mask)
	    red = 0;
	  green += green1;
	  if (green > vis->green_mask)
	    green = 0;
	  blue += blue1;
	  if (blue > vis->blue_mask)
	    blue = 0;
	}
    } else {
        is_index_color = 1;
        LOG(LOG_DEBUG, "use index colormap\n");
         for (i=0; i<ncolors; i++) {
             (*colors)[i].pixel = i;
             (*colors)[i].pad = 0;
         }
    }

    XQueryColors(dpy, cmap, *colors, ncolors);
    
    return(ncolors);
}

static int get_xcolors(XWindowAttributes *win_info,XColor **colors)
{
    int i, ncolors;
    Colormap cmap = win_info->colormap;

    if(user_colormap)
	/* assume the visual will be OK ... */
        cmap = XListInstalledColormaps(dpy, win_info->root, &i)[0];
    if (!cmap)
        return(0);
   
    ncolors = find_colors( win_info->visual, cmap, colors) ;
    return ncolors ;
}

static void convert_to_colorbuf(scimg* scimg, XImage *img, XColor *colors, int ncolors)
{
    int h, w;
    unsigned  cp_index;
    unsigned row_dt;
    int ori_bottom = 0;
    int ori_right = 0;

    unsigned long pixel;
    BYTE *buf_str = scimg->data;
    BYTE *buf_row;

    if(scimg->origin == ORI_BOT_LEFT || scimg->origin == ORI_BOT_RIGHT)
    {
        buf_str = scimg->data + scimg->w*(scimg->h-1)*3;
        ori_bottom = 1;
    }else if(scimg->origin == ORI_BOT_RIGHT || scimg->origin == ORI_UPP_RIGHT)
    {
        ori_right = 1;
    }

    for(h = 0; h< img->height; h++)
    {
        if(ori_bottom)
            buf_row = buf_str - h*scimg->w*3;
        else
            buf_row = buf_str + h*scimg->w*3;
         
        if(ori_right)
            buf_row += (scimg->w-h-1)*3;
        
        for(w = 0; w< img->width; w++)
        {
            pixel = XGetPixel(img, w, h);
            
            if(is_index_color)
            {
                if(pixel < ncolors)
                {
                    *(buf_row) = colors[pixel].red;
                    *(buf_row + 1) = colors[pixel].green;
                    *(buf_row + 2) = colors[pixel].blue;
                }else
                {
                    LOG(LOG_ERROR, "colormap index outo f range(%d -> %d)\n", cp_index, ncolors);
                }
            }else
            {
#define GET_COLOR_INDEX(shift, mask)                          \
                cp_index = (pixel & mask) >> shift;   \
                if(cp_index > ncolors){               \
                    LOG(LOG_ERROR, "colormap index outo f range(%d -> %d)\n", cp_index, ncolors); \
                    cp_index = 0; \
                }

                GET_COLOR_INDEX(16, img->red_mask)
                *(buf_row) = colors[cp_index].red; 

                GET_COLOR_INDEX(8, img->green_mask)
                *(buf_row+1) = colors[cp_index].green; 

                GET_COLOR_INDEX(0, img->blue_mask)
                *(buf_row+2) = colors[cp_index].blue;
            }
            
            if(ori_right)
                buf_row -= 3;
            else
                buf_row += 3;
        }
        
    }
}

static int grab_screen_image(scimg *scimg)
{
   XColor *colors;
    unsigned buffer_size;
    int header_size;
    int ncolors, i;
    XWindowAttributes win_info;
    XImage *image;
    unsigned x,y,width,height;
    unsigned dwidth, dheight;
    int bw;

    if (!XGetWindowAttributes(dpy, RootWindow(dpy, screen), &win_info)) 
        fatalerr("Can't get target window attributes.");

    x = win_info.x;
    y = win_info.y;
    width = win_info.width;
    height = win_info.height;
    dwidth = DisplayWidth (dpy, screen);
    dheight = DisplayHeight (dpy, screen);

    if(x + width > dwidth) width =  dwidth - x;
    if(y + height > dheight) height = dheight -y;

    image = XGetImage(dpy, RootWindow(dpy, screen), x, y, 
                      width, height, AllPlanes, img_format);

    LOG(LOG_DEBUG, "grabscreen: Getting colors.\n");
    //fwrite(image->data, image->bytes_per_line, image->height, stdout);
    ncolors = get_xcolors(&win_info, &colors);

    buffer_size = image_size(image);

    scimg->w = width;
    scimg->h = height;
    scimg->size = buffer_size;
    scimg->depth = image->depth;
    LOG(LOG_DEBUG, "grab_screen_image (%d, %d, %d, %d)\n", width, height, buffer_size, image->depth);

    if(scimg->data == NULL)  //When recording screen or projection, will grab screen cyclically.
    {
        if(!(scimg->data = malloc(buffer_size)))
            fatalerr("Can't get target window attributes.");
    }

    convert_to_colorbuf(scimg, image, colors, ncolors);
    //printf("******* screen image raw data ********\n");
    //fwrite(scimg->data, image->bytes_per_line, image->height, stdout);
    XDestroyImage(image);
    return 0;
}

static void uninit_grabber()
{
    close_display();
    return ;
}

REGISTER_GRABBER(x11)
