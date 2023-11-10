import os;
from mypackage import moudle1
import time

a=199;
def fun1():
        global a;
        a=300;
        moudle1.myfun();
def fun2():
        fun1();
time.sleep(10);
fun2();
print(a);