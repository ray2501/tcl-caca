#!/usr/bin/env tclsh

package require caca

set pig {,--.   ,--.
\\  /-~-\\/
 )' o O `(
(  ,---.  )
 `(_o_o_)'
   )`-'(}

set duck {                ,~~.
    __     ,   (  O )>
___( o)>   )`~~'   (
\\ <_. )   (  .__)   )
 `---'     `-.____,'}

set cv [caca::create_canvas 0 0]
set dp [caca::create_display $cv]

set image [caca::create_canvas 70 6]
set tmp [caca::create_canvas 70 6]
set sprite [caca::create_canvas 0 0]

$sprite set_color_ansi lightmagenta black
$sprite import_from_memory $pig "text"
$image blit 55 0 $sprite

$sprite set_color_ansi lightgreen black
$sprite import_from_memory $duck "text"
$image blit 30 1 $sprite

$image set_color_ansi lightcyan black
$image put_str 1 1 "hahaha mais vieux porc immonde !! \[⽼ ⾗\]"
$image set_color_ansi lightred black
$image put_char 38 1 [scan | %c]

$image set_color_ansi yellow black
$image put_str 4 2 "\\o\\ \\o| _o/ \\o_ |o/ /o/"

$image set_color_ansi white lightred
$image put_str 7 3 "▙▘▌▙▘▞▖▞▖▌ ▞▖▌ ▌▌"
$image put_str 7 4 "▛▖▌▛▖▚▘▚▘▚▖▚▘▚▖▖▖"
$image set_color_ansi black lightred
$image put_str 4 3 "▓▒░"
$image put_str 4 4 "▓▒░"
$image put_str 24 3 "░▒▓"
$image put_str 24 4 "░▒▓"

$cv set_color_ansi white blue
$cv put_str 0 0 "normal"
$cv blit 10 0 $image

$cv put_str 0 6 "flip"
$tmp blit 0 0 $image
$tmp flip
$cv blit 10 6 $tmp

$cv put_str 0 12 "flop"
$tmp blit 0 0 $image
$tmp flop
$cv blit 10 12 $tmp

$cv put_str 0 18 "rotate"
$tmp blit 0 0 $image
$tmp rotate_180
$cv blit 10 18 $tmp

$dp refresh

set eh [$dp create_event_handle]
$dp get_event $::caca::EVENT_KEY_PRESS $eh -1

$eh close
$dp close
$sprite close
$tmp close
$image close
$cv close
