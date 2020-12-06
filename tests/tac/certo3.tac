.table
int i__global
int a__sum
int b__sum
int c__sum
int in__sum
float out__sum
int a__mul
int b__mul
int c__mul
int a__main
.code
jump main_
sum_:
pop c__sum
pop b__sum
pop a__sum
add $0, $0, c__sum
mov in__sum, $0
return in__sum
mul_:
pop b__mul
pop a__mul
mul $0, b__mul, a__mul
mov c__mul, $0
return c__mul
main_:
param 2
param 3
param 4
call sum_, 3
pop a__main
println a__main
param a__main
param 4
param 5
call sum_, 3
pop a__main
println a__main
param 2
param 3
call mul_, 2
pop a__main
println a__main
