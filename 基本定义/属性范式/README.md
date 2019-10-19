# 属性范式
### 有啥用？
- 说实话它就让你在访问属性的时候少写一个括号，哈哈

### 头脑风暴
绝后型
```C++
#define $ (*(the_owner *)this)
union name{
private:
    typedef name the_owner;
    struct data{
        U32 A;
    }meta;

    data * operator->(){
        return & meta;
    }
public:
    struct {
        operator U32 (){
            return $->A;
        }
    private:
        friend the_owner;
        void operator= (U32 value){
            $->A = value;
        }
    }A;
};
```

膨胀型
```C++
//你猜
```

混合型
```
//你再猜
```

### 设计初衷
- VC++平台一个关键就能解决的问题，但不能跨平台有啥用？
- 代码君前后废弃三种实现方式，绞尽脑汁实现了这个有点蹩脚的属性特性，希望在精心设计的代码库中可以使用这个特性
