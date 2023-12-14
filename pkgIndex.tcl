# -*- tcl -*-
# Tcl package index file, version 1.1
#
if {[package vsatisfies [package provide Tcl] 9.0-]} {
    package ifneeded caca 0.7 \
	    [list load [file join $dir libtcl9caca0.7.so] [string totitle caca]]
} else {
    package ifneeded caca 0.7 \
	    [list load [file join $dir libcaca0.7.so] [string totitle caca]]
}
