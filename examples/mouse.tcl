#!/usr/bin/env tclsh

package require caca

set cv [caca::create_canvas 80 24]
set dp [caca::create_display $cv]

$dp set_time 40000
$dp set_cursor 0
$dp set_mouse 1

set quit 0
set eh [$dp create_event_handle]
while {$::quit == 0} {
    while {[$dp get_event $::caca::EVENT_ANY $eh 0]==1} {
        set type [$eh get_type]
        set ::x 0
        set ::y 0
        set ::b 0
        set ::p ""
        if {$type == $::caca::EVENT_KEY_PRESS} {
           set ::quit 1
        }
        if {$type == $::caca::EVENT_MOUSE_MOTION} {
           set x [$eh get_mouse_x]
           set y [$eh get_mouse_y]
        }
        if {$type == $::caca::EVENT_MOUSE_PRESS} {
           set p "pressed" 
           set b [$eh get_mouse_button]
        }
        if {$type == $::caca::EVENT_MOUSE_RELEASE} {
           set p "released" 
           set b [$eh get_mouse_button]
        }

        $cv put_str 0 0 "$x, $y"
        if {$b > 0} {
            $cv put_str 0 1 "Mouse button $b $p"
        }
    }

    $dp refresh
}

$eh close
$dp close
$cv close
