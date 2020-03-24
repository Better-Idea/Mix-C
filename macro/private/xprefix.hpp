#pragma once
#define xnt             xnt__
#define xpub(...)       pub__(__VA_ARGS__)
#define xpro(...)       pro__(__VA_ARGS__)
#define xpri(...)       pri__(__VA_ARGS__)
#define xtmpl(...)      tmpl__(__VA_ARGS__)

#define __xprefix_
#define __xprefix_base_
#define __xprefix_base_tmpl__(...)                      
#define __xprefix_base_pub__(...)                       , public      __VA_ARGS__
#define __xprefix_base_pro__(...)                       , protected   __VA_ARGS__
#define __xprefix_base_pri__(...)                       , private     __VA_ARGS__

#define __link(a,b)                                     a ## b
#define __link2(a,b)                                    __link(a,b)

#define __xprefix_using_base_pubx__(i,...)              using __link2(base, i) = __VA_ARGS__; using __link2(base, i) :: __link2(base, i);
#define __xprefix_using_base_prox__(i,...)              using __link2(base, i) = __VA_ARGS__; using __link2(base, i) :: __link2(base, i);
#define __xprefix_using_base_prix__(i,...)              using __link2(base, i) = __VA_ARGS__; using __link2(base, i) :: __link2(base, i);

#define __xprefix_using_base_
#define __xprefix_using_base_tmpl__(...)
#define __xprefix_using_base_pub__(...)                 __xprefix_using_base_pubx__(__COUNTER__,__VA_ARGS__)
#define __xprefix_using_base_pro__(...)                 __xprefix_using_base_prox__(__COUNTER__,__VA_ARGS__)
#define __xprefix_using_base_pri__(...)                 __xprefix_using_base_prix__(__COUNTER__,__VA_ARGS__)

#define __xprefix_member_header_
#define __xprefix_member_header_tmpl__(...)
#define __xprefix_member_header_pub__(...)              __VA_ARGS__
#define __xprefix_member_header_pro__(...)              __VA_ARGS__
#define __xprefix_member_header_pri__(...)              __VA_ARGS__

#define __xprefix_member_
#define __xprefix_member_tmpl__(...)
#define __xprefix_member_pub__(...)                     , __VA_ARGS__
#define __xprefix_member_pro__(...)                     , __VA_ARGS__
#define __xprefix_member_pri__(...)                     , __VA_ARGS__

#define __xprefix_field_
#define __xprefix_field_pub__(name,...)                 public:     __VA_ARGS__ name;
#define __xprefix_field_pro__(name,...)                 protected:  __VA_ARGS__ name;
#define __xprefix_field_pri__(name,...)                 private:    __VA_ARGS__ name;

#define __xprefix_member_list_header_
#define __xprefix_member_list_header_pub__(name,...)    & __self__::name
#define __xprefix_member_list_header_pro__(name,...)    & __self__::name
#define __xprefix_member_list_header_pri__(name,...)    & __self__::name

#define __xprefix_member_list_
#define __xprefix_member_list_pub__(name,...)           , & __self__::name
#define __xprefix_member_list_pro__(name,...)           , & __self__::name
#define __xprefix_member_list_pri__(name,...)           , & __self__::name

#define __xprefix_keep_tmpl_
#define __xprefix_keep_tmpl_tmpl__(...)                 <__VA_ARGS__>
#define __xprefix_keep_tmpl_xtmpl(...)                  <__VA_ARGS__>
#define __xprefix_keep_tmpl_pub__(...)
#define __xprefix_keep_tmpl_pro__(...)
#define __xprefix_keep_tmpl_pri__(...)

#define __xprefix_header_1st_
#define __xprefix_header_1st_xnt__(first,...)           first

#define __xprefix_header_2nd_
#define __xprefix_header_2nd_xnt__(first,...)           __VA_ARGS__

#define __xprefix_1st_
#define __xprefix_1st_xnt__(first,...)                  , first

#define __xprefix_2nd_
#define __xprefix_2nd_xnt__(first,...)                  , __VA_ARGS__

#define __xprefix_header_arg_
#define __xprefix_header_arg_xnt__(first,...)           __VA_ARGS__ first
#define __xprefix_arg_
#define __xprefix_arg_xnt__(first,...)                  , __VA_ARGS__ first

#define __xprefix_header_fields_
#define __xprefix_header_fields_xnt__(first,...)        __VA_ARGS__ first

#define __xprefix_fields_
#define __xprefix_fields_xnt__(first,...)               ; __VA_ARGS__ first
