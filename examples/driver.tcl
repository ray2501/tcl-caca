#!/usr/bin/env tclsh

package require caca

set driver_list [caca::get_display_driver_list]

set cv [caca::create_canvas 0 0]
set dp [caca::create_display $cv]
$cv set_color_ansi white black

set eh [$dp create_event_handle]
while {1} {
    $cv put_str 1 0 "Available drivers:"
    set cur_driver [$dp get_driver]
    set n 0
    foreach {driver desc} $driver_list {
       if {[string compare $driver $cur_driver]==0} {
           $cv put_str 2 [expr $n + 2] "$driver ($desc) (*)"
       } else {
           $cv put_str 2 [expr $n + 2] "$driver ($desc) ( )"
       }

       incr n
    }

    $dp refresh

    if {[$dp get_event $::caca::EVENT_KEY_PRESS $eh 5000000]==1} {
       break
    }
}

$eh close
$dp close
$cv close
