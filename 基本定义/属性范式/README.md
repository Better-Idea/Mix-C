# ���Է�ʽ
### ��ɶ�ã�
- ˵ʵ�����������ڷ������Ե�ʱ����дһ�����ţ�����

### ͷ�Է籩
������
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

������
```C++
//���
```

�����
```
//���ٲ�
```

### ��Ƴ���
- VC++ƽ̨һ���ؼ����ܽ�������⣬�����ܿ�ƽ̨��ɶ�ã�
- �����ǰ���������ʵ�ַ�ʽ���ʾ���֭ʵ��������е����ŵ��������ԣ�ϣ���ھ�����ƵĴ�����п���ʹ���������
