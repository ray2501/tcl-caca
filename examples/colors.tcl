#!/usr/bin/env tclsh

package require caca

set colormap [dict create 0 black 1 blue 2 green 3 cyan 4 red 5 magenta \
6 brown 7 lightgray 8 drakgray 9 lightblue 10 lightgreen  \
11 lightcyan 12 lightred 13 lightmagenta 14 yellow 15 white]

set cv [caca::create_canvas 80 24]
set dp [caca::create_display $cv]
$cv set_color_ansi white black

set eh [$dp create_event_handle]

$cv set_color_ansi lightgray black
$cv clear

for {set i 0} {$i < 16} {incr i} {
    $cv set_color_ansi lightgray black
    if {$i > 8} {
        $cv put_str 3 [expr $i + 3] "ANSI $i"
    } else {
        $cv put_str 3 [expr $i + 2] "ANSI $i"
    }

    for {set j 0} {$j < 16} {incr j} {
        $cv set_color_ansi [dict get $colormap $i] [dict get $colormap $j]
        if {$j >= 8} {
           if {$i >= 8} {
                $cv put_str [expr 13 + $j * 4] [expr $i + 3] "Aaホ"
           } else {
                $cv put_str [expr 13 + $j * 4] [expr $i + 2] "Aaホ"
           }
        } else {
           if {$i >= 8} {
                $cv put_str [expr 12 + $j * 4] [expr $i + 3] "Aaホ"
           } else {
                $cv put_str [expr 12 + $j * 4] [expr $i + 2] "Aaホ"
           }
        }
    }
}

$cv set_color_ansi lightgray black

$cv put_str 3 20 "This is bold    This is blink    This is italics    This is underline"
$cv set_attr $::caca::STYLE_BOLD
$cv put_str [expr 3 + 8] 20 bold
$cv set_attr $::caca::STYLE_BLINK
$cv put_str [expr 3 + 24] 20 blink
$cv set_attr $::caca::STYLE_ITALICS
$cv put_str [expr 3 + 41] 20 italics
$cv set_attr $::caca::STYLE_UNDERLINE
$cv put_str [expr 3 + 60] 20 underline

$dp refresh

$dp get_event $::caca::EVENT_KEY_PRESS $eh -1

$eh close
$dp close
$cv close
