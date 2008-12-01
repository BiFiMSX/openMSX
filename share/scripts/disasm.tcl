# helper functions
proc getPC {} {
	expr [debug read "CPU regs" 20] * 256 + \
	     [debug read "CPU regs" 21]
}
proc getSP {} {
	expr [debug read "CPU regs" 22] * 256 + \
	     [debug read "CPU regs" 23]
}

# very common debug functions

set_help_text peek \
{Read a byte from the given memory location.
Equivalent to "debug read memory <addr>".

usage:
  peek <addr>
}
proc peek {addr} {
	return [debug read memory $addr]
}

set_help_text poke \
{Write a byte to the given memory location.
Equivalent to "debug write memory <addr> <val>".

usage:
  poke <addr> <val>
}
proc poke {addr val} {
	debug write memory $addr $val
}


#
# disasm
#
set_help_text disasm \
{Disassemble z80 instructions

Usage:
  disasm                Disassemble 8 instr starting at the currect PC
  disasm <addr>         Disassemble 8 instr starting at address <adr>
  disasm <addr> <num>   Disassemble <num> instr starting at address <addr>
}
proc disasm {{address -1} {num 8}} {
	if {$address == -1} { set address [getPC] }
	for {set i 0} {$i < $num} {incr i} {
		set l [debug disasm $address]
		append result [format "%04X  %s\n" $address [join $l]]
		set address [expr $address + [llength $l] - 1]
	}
	return $result
}


#
# run_to
#
set_help_text run_to \
{Run to the specified address, if a breakpoint is reached earlier we stop
at that breakpoint.}
proc run_to {address} {
#	after break "debug remove_bp $address"
	set bp [ debug set_bp $address ]
	after break "debug remove_bp $bp"
	debug cont
}


#
# step_in
#
set_help_text step_in \
{Step in. Execute the next instruction, also go into subroutines.}
proc step_in {} {
	debug step
}


# TODO step_out   needs changes in openMSX itself


#
# step_over
#
set_help_text step_over \
{Step over. Execute the next instruction but don't step into subroutines.
Only 'call' or 'rst' instructions are stepped over. Note: 'push xx / jp nn'
sequences can in theory also be used as calls but these are not skipped
by this command.}
proc step_over {} {
	set address [getPC]
	set l [debug disasm $address]
	if {[string match "call*" [lindex $l 0]] ||
	    [string match "rst*"  [lindex $l 0]] ||
	    [string match "ldir*" [lindex $l 0]] ||
	    [string match "cpir*" [lindex $l 0]] ||
	    [string match "inir*" [lindex $l 0]] ||
	    [string match "otir*" [lindex $l 0]] ||
	    [string match "lddr*" [lindex $l 0]] ||
	    [string match "cpdr*" [lindex $l 0]] ||
	    [string match "indr*" [lindex $l 0]] ||
	    [string match "otdr*" [lindex $l 0]] ||
	    [string match "halt*" [lindex $l 0]]} {
		run_to [expr $address + [llength $l] - 1]
	} else {
		debug step
	}
}
