#!/usr/bin/env tclsh

package require caca

set cv [caca::create_canvas 32 16]
set dp [caca::create_display $cv]

for {set y 0} {$y < 16} {incr y} {
    for {set x 0} {$x < 16} {incr x} {
        set bgcolor [expr 0xff00 | ($y << 4) | $x]
        set fgcolor [expr 0xf000 | ((15 - $y) << 4) | ((15 - $x) << 8)]

        $cv set_color_argb $fgcolor $bgcolor
        $cv put_str [expr $x * 2] $y "CA"
    }
}

$cv set_color_ansi white lightblue
$cv put_str 2 1 " truecolor libcaca "

$dp refresh

set eh [$dp create_event_handle]
$dp get_event $::caca::EVENT_KEY_PRESS $eh -1

$eh close
$dp close
$cv close
