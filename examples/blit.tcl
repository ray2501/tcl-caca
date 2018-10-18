#!/usr/bin/env tclsh

package require caca

set THE_PIG "\
  ,__         __,
  \\)`\\_..._/`(/
   .'  _   _  '.
  /    o\\ /o   \\
  |    .-.-.    |  _
  |   /() ()\\   | (,`)
 / \\  '-----'  / \\ .'
|   '-..___..-'   |
|                 |
|                 |
;                 ;
 \\      / \\      /
  \\-..-/'-'\\-..-/
jgs\\/\\/     \\/\\/"

set cv [caca::create_canvas 0 0]
set dp [caca::create_display $cv]
set eh [$dp create_event_handle]

set sprite [caca::create_canvas 0 0]
$sprite set_color_ansi lightred black
$sprite import_from_memory $THE_PIG "text"
$sprite set_handle [expr [$sprite get_width] / 2]  [expr [$sprite get_height] / 2]

$cv set_color_ansi white blue
$cv put_str 0 0 "Centered sprite"
$cv blit [expr [$cv get_width] / 2]  [expr [$cv get_height] / 2] $sprite

$dp refresh
$dp get_event $::caca::EVENT_KEY_PRESS $eh -1

$sprite close
$eh close
$dp close
$cv close
