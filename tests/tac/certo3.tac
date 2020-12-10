.table
float sal__salaryClt
float s__salaryClt
int person__main__age
float person__main__sal
int person2__main__age
float person2__main__sal
int personresult__main__age
float personresult__main__sal
float saldisc__main
.code
jump __main
__salaryClt:
pop sal__salaryClt
mul $0, 0.8, sal__salaryClt
mov s__salaryClt, $0
return s__salaryClt
__main:
call __salaryClt, 0
pop saldisc__main
println saldisc__main
add $0, person2__main, person__main
mov personresult__main, $0
println personresult__main
