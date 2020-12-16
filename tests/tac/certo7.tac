.table
int a__ola
int a__main
int d__main
int b__main
int c__main
int true = 1
int false = 0
.code
jump __main
__ola:
mov a__ola, true
return a__ola
__main:
mov a__main, 10
mov d__main, 20
mov b__main, true
mov c__main, false
mov $0, b__main
brz L0, $0
println a__main
L0:
nop
mov $0, c__main
brz L1, $0
println d__main
L1:
nop
