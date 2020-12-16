.table
int a__main
int b__main
int c__main
.code
jump __main
__main:
mov a__main, 10
mov b__main, 5
sub $0, a__main, b__main
mul $0, $0, a__main
mov c__main, $0
println c__main
