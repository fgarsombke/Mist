# Stephen Wan
# University of Texas
#
# Usage: arm-none-eabi-gdb --command=lm3s.gdb
#        (gdb) lm3s <name>.axf
define lm3s
       if $argc == 0
          help lm3s
       else
          target remote :3333
          monitor reset halt
          load $arg0
          symbol-file $arg0
          monitor reset
end
