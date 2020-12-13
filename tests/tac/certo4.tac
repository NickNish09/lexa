.table
int i__global
int a__sum
int b__sum
int c__sum
int d__sum
int in__sum
int a__main
.code
jump __main
__sum:
pop d__sum
pop c__sum
pop b__sum
pop a__sum
add $0, b__sum, a__sum
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
call __sum, 3
pop a__main
slt $0, 10, a__main
brz $1, $0
println a__main
add $0, 5, 4
mul $0, $0, 2
mov a__main, $0
println a__main
