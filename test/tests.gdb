# Test file for "ExtraCredit_2"


# commands.gdb provides the following functions for ease:
#   test "<message>"
#       Where <message> is the message to print. Must call this at the beginning of every test
#       Example: test "PINA: 0x00 => expect PORTC: 0x01"
#   checkResult
#       Verify if the test passed or failed. Prints "passed." or "failed." accordingly,
#       Must call this at the end of every test.
#   expectPORTx <val>
#       With x as the port (A,B,C,D)
#       The value the port is epected to have. If not it will print the erroneous actual value
#   setPINx <val>
#       With x as the port or pin (A,B,C,D)
#       The value to set the pin to (can be decimal or hexidecimal
#       Example: setPINA 0x01
#   printPORTx f OR printPINx f
#       With x as the port or pin (A,B,C,D)
#       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary
#       Example: printPORTC d
#   printDDRx
#       With x as the DDR (A,B,C,D)
#       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n

# Test 1
test "PINA: 0x00 => PORTB: 0x01, state: Run"
set state = Start
# Set inputs
setPINA 0x00
# Continue for several ticks
timeContinue 100
# Set expect values
expectPORTB 0x1
expect state Run
# Check pass/fail
checkResult

# Test 2
test "PINA: 0x01 => PORTB: 0x02, state: Release"
set state = Start
timeContinue 150
setPINA 0x01
expectPORTB 0x02
expect state Release
checkResult

# Test 3
test "PINA: 0x01 => PORTB: 0x02, state: Alarm"
set state = Alarm
timeContinue 10
setPINA 0x01
expectPORTB 0x02
expect state Alarm
checkResult

# Test 4
test "PINA: 0x00 => PORTB: 0x02, state: Alarm"
set state = Wait
timeContinue 42
setPINA 0x00
expectPORTB 0x02
expect state Alarm
checkResult

# Test 5
test "PINA: 0x01 => PORTB: 0x02, state: Alarm"
set state = Alarm
timeContinue 10
setPINA 0x02
expectPORTB 0x05
expect state Stop
checkResult

# Test 6
test "PINA: 0x01 => PORTB: 0x02, state: Alarm"
set state = Wait
timeContinue 10
setPINA 0x02
expectPORTB 0x05
expect state Stop
checkResult

# Test 7
test "PINA: 0x01 => PORTB: 0x02, state: Alarm"
set state = Alarm
timeContinue 50
setPINA 0x00
expectPORTB 0x02
expect state Alarm
checkResult


# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
