#pragma once

// 普通类
#define xname(...)                          name__(__VA_ARGS__)

// 模板类
#define xtmpl(...)                          tmpl__(__VA_ARGS__)

// 特化模板类
#define xspec(...)                          spec__(__VA_ARGS__)

// 公有的基类 (public base)
#define xpubb(...)                          pubb__(__VA_ARGS__)

// 受保护的基类 (protected base)
#define xprob(...)                          prob__(__VA_ARGS__)

// 私有基类 (private base)
#define xprib(...)                          prib__(__VA_ARGS__)

// 公有字段 (public field)
#define xpubf(...)                          pubf__(__VA_ARGS__)

// 受保护的字段 (protected field)
#define xprof(...)                          prof__(__VA_ARGS__)

// 私有的字段 (private field)
#define xprif(...)                          prif__(__VA_ARGS__)

// 与该结构关联的类型 (association)
#define xasso(...)                          asso__(__VA_ARGS__)

// 函数签名列表
#define xfunc(...)                          func__(__VA_ARGS__)

// 返回值
#define xret(...)                           ret__(__VA_ARGS__)

// 参数
#define xarg(...)                           arg__(__VA_ARGS__)
