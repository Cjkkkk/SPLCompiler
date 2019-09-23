# SPL
## 1 environment
- Windows 10
- [GCC](https://sourceforge.net/projects/mingw-w64/)
- [Flex & Bison](https://sourceforge.net/projects/winflexbison/files/winflexbison3-latest.zip/download)
	- 一定要使用bison 3.0以上的版本
	- 下载解压后将win_flex和win_bison更名为flex和bison，并添加到系统变量中
- [NASM]()

## 2 getting started
* 编译程序
在与可执行文件同目录下创建assem目录用于存储汇编代码/byte_code目录用于存储中间代码

* 编译test目录下的test6.spl
* 运行nasm生成可执行代码
```bash=
nasm -felf64 hello.asm && gcc hello.o && ./a.out
```
* 输出
```
27
```

## 3 references
### IR optimization
* [TAC](https://web.stanford.edu/class/archive/cs/cs143/cs143.1128/lectures/13/Slides13.pdf)
* [determine if a temp can be reused](https://en.wikipedia.org/wiki/Sethi%E2%80%93Ullman_algorithm)
* [control flow graph](https://en.wikipedia.org/wiki/Control-flow_graph)
* [SSA phi insertion](http://pages.cs.wisc.edu/~fischer/cs701.f08/lectures/Lecture23.4up.pdf)
* [rename variable](https://www.cs.cmu.edu/afs/cs/academic/class/15745-s16/www/lectures/L8-SSA.pdf)
* [UD chain/DU chain](https://en.wikipedia.org/wiki/Use-define_chain)
* [constant folding/constant propagation](https://www.zhihu.com/question/55976094/answer/147302764)
* [constant propagation implementation](http://lampwww.epfl.ch/resources/lamp/teaching/advancedCompiler/2005/slides/05-UsingSSA_CP-1on1.pdf)

* [cs143](https://web.stanford.edu/class/archive/cs/cs143/cs143.1128/lectures)
* [SSA dead code](http://www.cs.colostate.edu/~mstrout/CS553Fall09/Slides/lecture14-SSA.ppt.pdf)
### register allocation
* [register allocation algorithm](https://web.stanford.edu/class/archive/cs/cs143/cs143.1128/lectures/17/Slides17.pdf)
