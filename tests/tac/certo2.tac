.table
int i__global
int a__sum
int b__sum
int c__sum
int in__sum
float out__sum
int a__mult
int b__mult
int c__mult
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
__mult:
pop b__mult
pop a__mult
mul $0, b__mult, a__mult
mov c__mult, $0
return c__mult
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
param 2
param 3
call __mult, 2
pop a__main
println a__main
