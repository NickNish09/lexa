.table
int v[5] = {5, 4, 3, 2, 1}
int size = 5
.code
main:
mov $0, 0
sub $1, size,1
// while $0 < size
L1:
slt $2, $0, size
brz L2, $2
// print v[$0]
mov $2, &v
mov $2, $2[$0]
print $2
// if $0 < size - 1, print ", "
slt $2, $0, $1
brz L3, $2
print ','
print ' '
L3:
add $0, $0,
1
jump L1 // loop
L2:
println