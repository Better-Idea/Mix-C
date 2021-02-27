# 接口
C++ 没有显示定义的接口，而常规操作就是通过继承虚基类，实现虚函数... balabala  
我们在学习前人在 OOP 贡献的成果，同时也要妥协抽象带来的开销，虽然虚函数并不完美，但大多场合不会让你觉得鸡肋。  

## 分析
下列代码展示了虚基类的基本用法，按照该方式我们可以实现`针对接口编程`。但自 C++20 以后，
C++ 多了 concept 这么一个用于约束模板的特性，让我们可以通过模板实现同样的功能。  
**BUT** 并不是所有人都愿意把实现暴露在头文件中，他们希望更愿意把实现隐藏到 .cpp 文件中。  
此外，当有很多不同的 `符合类`（这里代指满足该 concept 约束的类）作为该函数的参数时会导致代码膨胀，对于 PC 机也许无伤大雅，但资源匮乏的嵌入式设备可能不会欢迎这种编程实践。所以 concept 暂时不会取代虚基类，当然这也不是 concept 设计的本意，只是它的出现让你多一种选择。  
**C++ 中面向接口编程的常规写法**
```C++
#define xuser mixc::powerful_cat
#include"mixc.hpp"

struct ax{
    virtual void hi()       = 0;
};

struct bx : ax{
    void hi(){
        xhint("hi, i'm bx");
    }
};

struct cx : ax{
    void hi(){
        xhint("hi, i'm cx");
    }
};

void hi(ax * someone){
    someone->hi();
}

int main(){
    bx b;
    cx c;
    hi(& b);
    hi(& c);
    return 0;
}

```

也有开发人员排斥虚基类，每一个虚基类都会让子类额外消耗指针大小的内存用于存放虚函数表。也就是继承的接口越多，额外的内存开销也就越多，此外虚函数通常难以内联，每次访问虚函数包装的属性时都会产生额外的调用。  
**以上都是抠门 C++ 程序员的老毛病**，C++ 追求高效，但有时候我们可能会被这些琐碎的事情困扰，从而让注意力变得分散。这样的想法并不存在对错，因为人们在接受 OOP 的熏陶时也会怀疑它是否真的像宣传那般的强大，是否可以设计的更好？  

**我们做出这样的思考:**  
- 可不可以把虚函数表移到外边去？
- 不同的接口是否可以组合成一个接口，并且只占一个接口所需的内存空间？

每一个接口所需的无非就是成员函数指针和对象指针，假如接口刚好就包含这两个东西，当初始化时从 `符合类`（这里代指满足该接口定义的类，而无需继承关系）中获取 this 指针和对应的成员函数指针，这样就把 `符合类` 的额外存储负担转移到接口上面。  
新的写法如下：
```C++
#define xuser mixc::powerful_cat
#include"macro/xinterface.hpp"
#include"mixc.hpp"

xinterface(
    xname(isay_hi),
    xfunc(hi, void()) // 定义函数
);

struct bx{
    void hi(){
        xhint("hi, i'm bx");
    }
};

struct cx{
    void hi(){
        xhint("hi, i'm cx");
    }
};

void hi(isay_hi someone){
    someone->hi();
}

int main(){
    bx b;
    cx c;
    hi(b);
    hi(c);
    return 0;
}
```
**作者偷偷告诉你：这种方式也提高了成员函数内联的概率**

## 新式接口的不足
目前它只用于函数的参数，虽然它可以做成员变量，但这并不合适。因为实质上它是一个弱指针，它保留的 this 指针的有效性依赖于被指向对象的生命周期。  
就目前而言，我们不太愿意在类中引入成员接口，因为对于 `GC` 而言它是不受欢迎的，由于无法在编译期间判定一个含有接口的类是否有具有环形引用的潜质，导致我们必须要在运行时遍历所有可能的节点。  
我们给出如下情景：
```C++
#define xuser mixc::powerful_cat
#include"mixc.hpp"

xinterface(
    xname(iabstract_thing),
    xfunc(do_something, void())
);

xstruct(
    xname(ax),
    xpubf(pitem, iabstract_thing)
)
    void do_something(){
        // ...
    }
$

xstruct(
    xname(bx),
    xpubf(pitem, uxx)
)
    void do_something(){
        // ...
    }
$

```
预备知识：https://github.com/Better-Idea/Mix-C/tree/master/gc  
由于 `ax` 和 `bx` 都符合 `iabstract_thing` 接口的定义，所以 `ax::pitem` 可以指向两者中的任意一个，假如它指向 `ax` 则具有环形引用的潜质，指向 `bx` 则不。  
在接口指向一个对象时，它是可以知晓该对象成分的（但是通过接口还原指向对象的成分时就没那么简单了）。假如该对象类似于 `bx` 这样不存在环形引用潜质的结构，那么该对象对应的 `GC 路由函数` 默认为 nullptr，表示无需路由该节点。  
而接口在获取对象的成员函数指针时，也会额外提供槽位用于存放对象的 `GC 路由函数`
```
+ iabstract_thing 指向的接口信息表
+-- [0] bx::do_something    对外可见
+-- [1] nullptr             只对 GC 可见
```

当接口指向的对象类似于 `ax` 这样的结构时，事情就变得复杂了，原先在编译期间做的那一套剪枝操作基本上都得挪到运行时。`GC` 会假定带有成员接口的结构**也许**具有环形引用潜质（一定不具备这个潜质的 `GC 路由函数` 为 nullptr，所以不为 nullptr 的都得遍历）。原先通过模板传 `类型数组`，现在这个数组必须是一个运行时占用内存的实例，而 `类型` 则用整型变量表示（xstruct 会为结构体生成类型编号）。  
对于**也许**具有环形引用潜质，或者继承 gc_self_management 指示存在显式 `GC 路由函数` 的结构，具有类似如下的布局：
```
+ iabstract_thing 指向的接口信息表
+-- [0] ax::do_something    对外可见
+-- [1] gc_routing          只对 GC 可见
+-- [2] gc_type_list        只对 GC 可见
```
我们可以看到这个函数表多了一个成员 `gc_type_list`，它是一个与接口当前指向对象相关的类型数组指针，用于存放该与对象存在直接或间接指向关系的类型编号。  
运行时版 `GC 所需参数布局` 如下示意：
```C++
// 实际上 __gc_type_list 的地址就可以作为结构体的类型 id
struct ax{
    // 我们可以在编译期判定 ax 不具有环形引用潜质，所以需要 GC 遍历的节点个数为 0
    static inline voidp __gc_type_list[] = { voidp(0) };

    uxx i;
};

struct cx;
struct bx{
    // 我们可以在编译期判定 ax 不具有环形引用潜质，所以不必添加 ax::__gc_type_list
    static inline voidp __gc_type_list[] = { 
        // 元素个数为 2，该字段是为接口准备的
        // 接口的 gc_type_list 是一个 voidp * 指针，无法通过 sizeof 的方式获取元素个数
        voidp(2),
        bx::__gc_type_list, 

        // 这里不讨论语法问题，只是示意存放了相关的列表
        cx::__gc_type_list,
    };

    ax * m_ax;
    cx * m_cx;
};

struct cx{
    static inline voidp __gc_type_list[] = { 
        voidp(2), 
        cx::__gc_type_list,
        bx::__gc_type_list,
    };

    bx * m_bx;
};

struct iabstract_thing{
    struct info_t{
        // 该数组的长度是由该接口中定义的接口函数个数来决定的
        // 所以长度为 2
        voidp   func_list[2];

        // 对于接口，则是提供一个 __gc_type_list 指针
        voidp * gc_type_list;
    };

    template<class object_t>
    iabstract_thing(object_t const & object){
        // 获取 object_t 相关的信息
        // balabala...
    }

    info_t *    info_ptr;
    voidp       object_ptr;

    void do_something(){
        using func_t = void(*)(voidp);
        func_t(info_ptr->func_list[0])(object_ptr);
    }

    // 只对 GC 可见
    gc_ret_t gc_routing(){
        using  func_t = gc_ret_t(*)(voidp, voidp);
        if (info_ptr->func_list[1] == nullptr){
            return no_circle_ref;
        }
        return func_t(info_ptr->func_list[1])(object_ptr, info_ptr->gc_type_list);
    }
};

```
