#!/usr/bin/env tclsh

package require caca

set dp [caca::create_display]
set cv [$dp get_canvas_handle]

set buffer {}
for {set y 0} {$y < 256} {incr y} {
    for {set x 0} {$x < 256} {incr x} {
         set a 0
         set r [expr ($x * $y) / 256]
         set g [expr ($x * $y) / 256]
         set b [expr $x << 0]
 
         append buffer [binary format c4 [list $b $g $r $a]]
    }
}

set cdi [caca::create_dither 32 256 256 [expr 4 * 256] 0x00ff0000 0x0000ff00 0x000000ff 0x0]
$cdi bitmap $cv 0 0 [$cv get_width] [$cv get_height] $buffer
$cdi close

$dp refresh

set eh [$dp create_event_handle]
$dp get_event $::caca::EVENT_KEY_PRESS $eh -1

$eh close
$dp close
