.table
int a__main
int b__main
int c__main
.code
jump __main
__main:
sub $0, b__main, a__main
mul $0, $0, a__main
mov c__main, $0
println c__main
