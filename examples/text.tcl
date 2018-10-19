#!/usr/bin/env tclsh

package require caca

set STRING {              |_|
   _,----._   | | 
  (/ @  @ \)   __ 
   |  OO  |   |_  
   \ `--' /   |__ 
    `----'        
              |_| 
 Hello world!  |  
                  }

set colormap [dict create 0 black 1 blue 2 green 3 cyan 4 red 5 magenta \
6 brown 7 lightgray 8 drakgray 9 lightblue 10 lightgreen  \
11 lightcyan 12 lightred 13 lightmagenta 14 yellow 15 white]

set pig [caca::create_canvas 0 0]
$pig import_from_memory $STRING "text"

set width [$pig get_width]
set height [$pig get_height]
set cv [caca::create_canvas [expr $width * 2 ] [expr $height * 2 ]]
$cv blit 0 0 $pig
$pig flip
$cv blit $width 0 $pig
$pig flip
$pig flop
$cv blit 0 $height $pig
$pig flop
$pig rotate_180
$cv blit $width $height $pig

for {set j 0} {$j < [$cv get_height]} {incr j} {
    for {set i 0} {$i < [$cv get_width]} {incr i 2} {
         set color [expr 9 + (($i + $j) % 6)]
         $cv set_color_ansi [dict get $colormap $color] default
         set a [$cv get_attr -1 -1]
         $cv put_attr $i $j $a
         $cv put_attr [expr $i + 1] $j $a
    }
}

set buffer [$cv export_to_memory "utf8"]
puts $buffer

$cv rotate_left 
set buffer [$cv export_to_memory "utf8"]
puts $buffer

$pig close
$cv close
