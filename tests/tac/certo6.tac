.table
int a__main
.code
jump __main
__main:
mov a__main, 10
W1:
slt $2, 0, a__main
brz EXIT_W1, $2
println a__main
sub $0, a__main, 1
mov a__main, $0
jump W1
EXIT_W1:
nop
mov a__main, -10
W2:
slt $2, a__main, 0
brz EXIT_W2, $2
println a__main
add $0, a__main, 1
mov a__main, $0
jump W2
EXIT_W2:
nop
mov a__main, 10
W3:
sleq $2, 0, a__main
brz EXIT_W3, $2
println a__main
sub $0, a__main, 1
mov a__main, $0
jump W3
EXIT_W3:
nop
mov a__main, -10
W4:
sleq $2, a__main, 0
brz EXIT_W4, $2
println a__main
add $0, a__main, 1
mov a__main, $0
jump W4
EXIT_W4:
nop
mov a__main, 10
W5:
seq $2, 10, a__main
brz EXIT_W5, $2
println a__main
mov a__main, 9
jump W5
EXIT_W5:
nop
