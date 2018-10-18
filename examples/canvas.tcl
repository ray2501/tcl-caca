#!/usr/bin/env tclsh

package require caca

for {set i 0} {$i < 128} {incr i} {
    set w [caca::rand 1 1000]
    set h [caca::rand 1 1000]

    set cv [caca::create_canvas $w $h]
    $cv put_char [expr $w - 1] [expr $h - 1] 88
    if {[$cv get_char [expr $w - 1] [expr $h - 1]] != 88} {
       puts "Error: $w-$h"
    }

    $cv close
}

