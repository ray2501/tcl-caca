#!/usr/bin/env tclsh

package require caca

puts "libcaca version:\n $::caca::version\n"
puts "Available drivers:\n [::caca::get_display_driver_list]\n"
puts "Available fonts:\n [::caca::get_font_list]\n"
puts "Export formats:\n [::caca::get_export_list]\n"
puts "Import formats:\n [::caca::get_import_list]\n"
