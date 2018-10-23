#!/usr/bin/env tclsh

package require caca

set cv [caca::create_canvas 8 2]
$cv set_color_ansi white black
$cv put_str 0 0 "ABcde"
$cv set_color_ansi lightred black
$cv put_str 5 0 "\\o/"
$cv set_color_ansi white blue
$cv put_str 0 1 "&$âøÿØ?!"

set fonts [caca::get_font_list]
set f [caca::load_font [lindex $fonts 0]]
set w [expr [$cv get_width] * [$f get_width]]
set h [expr [$cv get_height] * [$f get_height]]

set fillchar 0x00
set size [expr 4 * $w * $h]
set buf {}
for {set count 0} {$count < $size} {incr count} {
    append buf [binary format c $fillchar]
}

$cv render $f $buf $w $h [expr 4 * $w]

$cv set_size 80 32
set dp [caca::create_display $cv]
set d [caca::create_dither 32 $w $h [expr 4 * $w] 0xff00 0xff0000 0xff000000 0xff]
$d bitmap $cv 0 0 [$cv get_width] [$cv get_height] $buf
$dp refresh

set eh [$dp create_event_handle]
$dp get_event $::caca::EVENT_KEY_PRESS $eh -1

$eh close
$dp close
$d close
$f close
$cv close

