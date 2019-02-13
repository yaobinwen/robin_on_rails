# Modify Const Value

The code shows how to modify a constant by using indirect pointers.

Run `gcc main.c` to compile it. I'm using `gcc (Ubuntu 4.8.5-4ubuntu8~14.04.2) 4.8.5`, and it gives a warning:

```
main.c: In function ‘main’:
main.c:10:8: warning: assignment from incompatible pointer type [enabled by default]
     p1 = &p2;
        ^
```

This warning suggests the unsafe operation. Fixing it will prevent the constant modification from happening.

The code is excerpted from [_"再再论指针: 第九章 指针与const"_](https://blog.csdn.net/megaboy/article/details/482769).
