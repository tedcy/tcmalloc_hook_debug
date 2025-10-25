hook前：

(gdb) x/10i 0x7ffff6dcf4f0
   0x7ffff6dcf4f0 <__GI___libc_free>:   push   %r13
   0x7ffff6dcf4f2 <__GI___libc_free+2>: push   %r12
   0x7ffff6dcf4f4 <__GI___libc_free+4>: push   %rbp
   0x7ffff6dcf4f5 <__GI___libc_free+5>: push   %rbx
   0x7ffff6dcf4f6 <__GI___libc_free+6>: sub    $0x28,%rsp
   0x7ffff6dcf4fa <__GI___libc_free+10>:        mov    0x33f9f7(%rip),%rax        # 0x7ffff710eef8
   0x7ffff6dcf501 <__GI___libc_free+17>:        mov    (%rax),%rax
   0x7ffff6dcf504 <__GI___libc_free+20>:        test   %rax,%rax
   0x7ffff6dcf507 <__GI___libc_free+23>:        jne    0x7ffff6dcf5e0 <__GI___libc_free+240>
   0x7ffff6dcf50d <__GI___libc_free+29>:        test   %rdi,%rdi

hook后：

(gdb) x/10i 0x7ffff6dcf4f0
   0x7ffff6dcf4f0 <__GI___libc_free>:   jmpq   *0x0(%rip)        # 0x7ffff6dcf4f6 <__GI___libc_free+6>
   0x7ffff6dcf4f6 <__GI___libc_free+6>: xchg   %eax,%esi
   0x7ffff6dcf4f7 <__GI___libc_free+7>: push   %rbp
   0x7ffff6dcf4f8 <__GI___libc_free+8>: add    %al,(%rax)
   0x7ffff6dcf4fb <__GI___libc_free+11>:        add    %al,(%rax)
   0x7ffff6dcf4fd <__GI___libc_free+13>:        add    %cl,%ah
   0x7ffff6dcf4ff <__GI___libc_free+15>:        int3   
   0x7ffff6dcf500 <__GI___libc_free+16>:        int3   
   0x7ffff6dcf501 <__GI___libc_free+17>:        mov    (%rax),%rax
   0x7ffff6dcf504 <__GI___libc_free+20>:        test   %rax,%rax
   
(gdb) x/gx 0x7ffff6dcf4f6
0x7ffff6dcf4f6 <__GI___libc_free+6>:    0x0000000000405596
(gdb) x/10xb 0x7ffff6dcf4f6
0x7ffff6dcf4f6 <__GI___libc_free+6>:    0x96    0x55    0x40    0x00    0x00    0x00    0x00    0x00
0x7ffff6dcf4fe <__GI___libc_free+14>:   0xcc    0xcc
(gdb) x/x 0x405596
0x405596 <my_free(void*)>:      0x55
   
(gdb) x/10i free_f
   0x7ffff654b018:      push   %r13
   0x7ffff654b01a:      push   %r12
   0x7ffff654b01c:      push   %rbp
   0x7ffff654b01d:      push   %rbx
   0x7ffff654b01e:      sub    $0x28,%rsp
   0x7ffff654b022:      mov    0xbc3ecf(%rip),%rax        # 0x7ffff710eef8
   0x7ffff654b029:      jmpq   *0x0(%rip)        # 0x7ffff654b02f
   0x7ffff654b02f:      add    %esi,%ebp
   0x7ffff654b031:      fdiv   %st,%st(6)
   0x7ffff654b033:      (bad) 
   
x/gx 0x7ffff654b02f
0x7ffff654b02f: 0x00007ffff6dcf501