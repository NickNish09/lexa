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
mov person__main__age, 20
mov person__main__sal, 500.3
mov person2__main__age, 22
mov person2__main__sal, 350.1
add personresult__main__age, person2__main__age, person__main__age
add personresult__main__sal, person2__main__sal, person__main__sal
print personresult__main__age
print ' '
print personresult__main__sal
