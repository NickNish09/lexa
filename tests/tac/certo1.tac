.table
int i__global
int a__sum
int b__sum
int c__sum
int in__sum
float out__sum
int a__main
.code
jump __main
__sum:
pop c__sum
pop b__sum
pop a__sum
add $0, b__sum, a__sum
add $0, $0, c__sum
mov in__sum, $0
return in__sum
__main:
param 2
param 3
param 4
call __sum, 3
pop a__main
println a__main
param a__main
param 4
param 5
call __sum, 3
pop a__main
println a__main
