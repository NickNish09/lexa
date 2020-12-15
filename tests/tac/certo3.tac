.table
int i__global
int a__sumul
int b__sumul
int c__sumul
int in__sumul
float out__sumul
int a__mult
int b__mult
int c__mult
int a__main
.code
jump __main
__sumul:
pop c__sumul
pop b__sumul
pop a__sumul
add $0, b__sumul, a__sumul
mul $0, $0, c__sumul
mov in__sumul, $0
return in__sumul
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
call __sumul, 3
pop a__main
println a__main
param a__main
param 4
param 5
call __sumul, 3
pop a__main
println a__main
param 2
param 3
call __mult, 2
pop a__main
println a__main
