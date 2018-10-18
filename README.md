# tcl-caca

This package is Tcl bindings for Colour ASCII Art library ([libcaca](http://caca.zoy.org/wiki/libcaca)).
Now it is under initial development stage.


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
caca::create_display CANVAS_HANDLE  
caca::create_display_with_driver CANVAS_HANDLE driver_name  

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
CANVAS_HANDLE close  

DISPLAY_HANDLE get_driver  
DISPLAY_HANDLE set_driver driver_name  
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

