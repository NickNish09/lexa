.table
int a__main
.code
jump __main
__main:
mov a__main, 10
W1:
slt $2, a__main, 0
brz EXIT_W1, $2
println a__main
sub $0, a__main, 1
mov a__main, $0
EXIT_W2:
nop
