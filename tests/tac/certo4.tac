.table
int i__global
int a__sum
int b__sum
int c__sum
int d__sum
int in__sum
int a__main
int b__main
.code
jump __main
__sum:
pop d__sum
pop c__sum
pop b__sum
pop a__sum
add $0, a__sum, b__sum
add $0, $0, c__sum
add $0, $0, d__sum
mov in__sum, $0
return in__sum
__main:
param 10
param 3
param 4
param 5
call __sum, 4
pop a__main
println a__main
param a__main
param 4
param 5
param 6
call __sum, 4
pop a__main
param 1
param 1
param 1
param 1
call __sum, 4
pop b__main
slt $0, 10, a__main
brz L0, $0
println a__main
L0:
nop
slt $0, a__main, 10
brz L1, $0
println b__main
L1:
nop
add $0, 4, 5
mul $0, $0, 2
mov a__main, $0
println a__main
