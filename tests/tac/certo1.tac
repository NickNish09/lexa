.table
int i__global
int a__sum
int b__sum
int c__sum
int in__sum
float out__sum
int a__main
.code
jump main
sum:
pop c__sum
pop b__sum
pop a__sum
add $0, $0, c__sum
mov in__sum, $0
return in__sum
main:
param 2
param 3
param 4
call sum, 3
pop a__main
println a__main
param a__main
param 4
param 5
call sum, 3
pop a__main
println a__main
