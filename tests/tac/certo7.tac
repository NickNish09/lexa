.table
int a__ola
int a__main
int b__main
int true = 1
int false = 0
.code
jump __main
__ola:
mov a__ola, true
return a__ola
__main:
mov a__main, 10
mov b__main, true
mov $0, b__main
brz L0, $0
println a__main
L0:
nop
