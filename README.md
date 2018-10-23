# tcl-caca

This package is Tcl bindings for Colour ASCII Art library ([libcaca](http://caca.zoy.org/wiki/libcaca)).


License
=====

MIT


Commands
=====

caca::get_display_driver_list  
caca::get_export_list  
caca::get_import_list  
caca::get_font_list  
caca::rand min max  
caca::create_canvas width height  
caca::create_display ?CANVAS_HANDLE?  
caca::create_display_with_driver CANVAS_HANDLE driver_name  
caca::create_dither bpp w h pitch rmark gmark bmark amark  
caca::load_font font_name  

CANVAS_HANDLE set_size width height  
CANVAS_HANDLE get_width  
CANVAS_HANDLE get_height  
CANVAS_HANDLE gotoxy x y  
CANVAS_HANDLE wherex  
CANVAS_HANDLE wherey  
CANVAS_HANDLE put_char x y char  
CANVAS_HANDLE get_char x y  
CANVAS_HANDLE put_str x y string  
CANVAS_HANDLE get_attr x y  
CANVAS_HANDLE set_attr attr  
CANVAS_HANDLE unset_attr attr  
CANVAS_HANDLE toggle_attr attr  
CANVAS_HANDLE put_attr x y attr  
CANVAS_HANDLE set_color_ansi fg bg  
CANVAS_HANDLE set_color_argb fg bg  
CANVAS_HANDLE clear  
CANVAS_HANDLE set_handle x y  
CANVAS_HANDLE get_handle_x  
CANVAS_HANDLE get_handle_y  
CANVAS_HANDLE blit x y SRC_HANDLE ?MASK_HANDLE?   
CANVAS_HANDLE set_boundaries x y w h  
CANVAS_HANDLE invert  
CANVAS_HANDLE flip  
CANVAS_HANDLE flop  
CANVAS_HANDLE rotate_180  
CANVAS_HANDLE rotate_left  
CANVAS_HANDLE rotate_right  
CANVAS_HANDLE stretch_left  
CANVAS_HANDLE stretch_right  
CANVAS_HANDLE draw_line x1 y1 x2 y2 char  
CANVAS_HANDLE draw_thin_line x1 y1 x2 y2  
CANVAS_HANDLE draw_polyline x_list y_list char  
CANVAS_HANDLE draw_thin_polyline x_list y_list  
CANVAS_HANDLE draw_circle x y r char  
CANVAS_HANDLE draw_ellipse x y a b char  
CANVAS_HANDLE draw_thin_ellipse x y a b  
CANVAS_HANDLE fill_ellipse x y a b char  
CANVAS_HANDLE draw_box x y w h char  
CANVAS_HANDLE draw_thin_box x y w h  
CANVAS_HANDLE draw_cp437_box x y w h  
CANVAS_HANDLE fill_box x y w h char   
CANVAS_HANDLE draw_triangle x1 y1 x2 y2 x3 y3 char  
CANVAS_HANDLE draw_thin_triangle x1 y1 x2 y2 x3 y3  
CANVAS_HANDLE fill_triangle x1 y1 x2 y2 x3 y3 char  
CANVAS_HANDLE get_frame_count  
CANVAS_HANDLE set_frame index  
CANVAS_HANDLE get_frame_name  
CANVAS_HANDLE set_frame_name name  
CANVAS_HANDLE create_frame index  
CANVAS_HANDLE free_frame index  
CANVAS_HANDLE import_from_memory data fmt  
CANVAS_HANDLE import_from_file file fmt  
CANVAS_HANDLE import_area_from_memory x y data fmt  
CANVAS_HANDLE import_area_from_file x y file fmt  
CANVAS_HANDLE export_to_memory fmt  
CANVAS_HANDLE export_area_to_memory x y w h fmt  
CANVAS_HANDLE render font_handle bytearray width height pitch  
CANVAS_HANDLE close  

`set_color_ansi` can accept below colors:  
black, blue, green, cyan, red, magenta,
brown, lightgray, drakgray, lightblue, lightgreen,
lightcyan, lightred, lightmagenta, yellow, white,
default and transparent

DISPLAY_HANDLE get_driver  
DISPLAY_HANDLE set_driver driver_name  
DISPLAY_HANDLE get_canvas_handle  
DISPLAY_HANDLE refresh  
DISPLAY_HANDLE get_time  
DISPLAY_HANDLE set_time time  
DISPLAY_HANDLE get_width  
DISPLAY_HANDLE get_height  
DISPLAY_HANDLE set_mouse flag  
DISPLAY_HANDLE set_cursor flag  
DISPLAY_HANDLE set_title title  
DISPLAY_HANDLE create_event_handle  
DISPLAY_HANDLE get_event type event_handle timeout  
DISPLAY_HANDLE close  

EVENT_HANDLE get_type  
EVENT_HANDLE get_key_ch  
EVENT_HANDLE get_key_utf32  
EVENT_HANDLE get_key_utf8  
EVENT_HANDLE get_mouse_button  
EVENT_HANDLE get_mouse_x  
EVENT_HANDLE get_mouse_y  
EVENT_HANDLE get_resize_width  
EVENT_HANDLE get_resize_height  
EVENT_HANDLE close  

DITHER_HANDLE set_brightness brightness  
DITHER_HANDLE get_brightness  
DITHER_HANDLE set_gamma gamma  
DITHER_HANDLE get_gamma  
DITHER_HANDLE set_contrast contrast  
DITHER_HANDLE get_contrast  
DITHER_HANDLE set_antialias antiallies  
DITHER_HANDLE get_antialias  
DITHER_HANDLE get_antialias_list  
DITHER_HANDLE set_color color  
DITHER_HANDLE get_color  
DITHER_HANDLE get_color_list  
DITHER_HANDLE set_charset charset  
DITHER_HANDLE get_charset  
DITHER_HANDLE get_charset_list  
DITHER_HANDLE set_algorithm alogorithm  
DITHER_HANDLE get_algorithm  
DITHER_HANDLE get_algorithm_list  
DITHER_HANDLE bitmap canvas_handle x y w h pixels  
DITHER_HANDLE close  

FONT_HANDLE get_width  
FONT_HANDLE get_height  
FONT_HANDLE close  


Variables
=====

caca::version  

caca::EVENT_NONE  
caca::EVENT_KEY_PRESS  
caca::EVENT_KEY_RELEASE  
caca::EVENT_MOUSE_PRESS  
caca::EVENT_MOUSE_RELEASE  
caca::EVENT_MOUSE_MOTION  
caca::EVENT_RESIZE  
caca::EVENT_QUIT  
caca::EVENT_ANY  

caca::STYLE_BOLD  
caca::STYLE_ITALICS  
caca::STYLE_UNDERLINE  
caca::STYLE_BLINK  

caca::KEY_UNKNOWN  
caca::KEY_CTRL_A  
caca::KEY_CTRL_B  
caca::KEY_CTRL_C  
caca::KEY_CTRL_D  
caca::KEY_CTRL_E  
caca::KEY_CTRL_F  
caca::KEY_CTRL_G  
caca::KEY_BACKSPACE  
caca::KEY_TAB  
caca::KEY_CTRL_J  
caca::KEY_CTRL_K  
caca::KEY_CTRL_L  
caca::KEY_RETURN  
caca::KEY_CTRL_N  
caca::KEY_CTRL_O  
caca::KEY_CTRL_P  
caca::KEY_CTRL_Q  
caca::KEY_CTRL_R  
caca::KEY_PAUSE  
caca::KEY_CTRL_T  
caca::KEY_CTRL_U  
caca::KEY_CTRL_V  
caca::KEY_CTRL_W  
caca::KEY_CTRL_X  
caca::KEY_CTRL_Y  
caca::KEY_CTRL_Z  
caca::KEY_ESCAPE  
caca::KEY_DELETE  
caca::KEY_UP  
caca::KEY_DOWN  
caca::KEY_LEFT  
caca::KEY_RIGHT  
caca::KEY_INSERT  
caca::KEY_HOME  
caca::KEY_END  
caca::KEY_PAGEUP  
caca::KEY_PAGEDOWN  
caca::KEY_F1  
caca::KEY_F2  
caca::KEY_F3  
caca::KEY_F4  
caca::KEY_F5  
caca::KEY_F6  
caca::KEY_F7  
caca::KEY_F8  
caca::KEY_F9  
caca::KEY_F10  
caca::KEY_F11  
caca::KEY_F12  
caca::KEY_F13  
caca::KEY_F14  
caca::KEY_F15  


UNIX BUILD
=====

I only test tcl-caca under openSUSE LEAP 15.0 and Ubuntu 14.04.

Users need install libcaca development files.
Below is an example for openSUSE:

        sudo zypper in libcaca-devel

Below is an example for Ubuntu:

        sudo apt-get install libcaca-dev

Building under most UNIX systems is easy, just run the configure script
and then run make. For more information about the build process, see the
tcl/unix/README file in the Tcl src dist. The following minimal example
will install the extension in the /opt/tcl directory.

        $ cd tcl-caca
        $ ./configure --prefix=/opt/tcl
        $ make
        $ make install

If you need setup directory containing tcl configuration (tclConfig.sh),
below is an example:

        $ cd tcl-caca
        $ ./configure --with-tcl=/opt/activetcl/lib
        $ make
        $ make install


Example
=====

Please check examples folder.

