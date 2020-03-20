#pragma once
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
#define __xprefix_member_list_header_pub__(name,...)    decltype(& __self__::name)
#define __xprefix_member_list_header_pro__(name,...)    decltype(& __self__::name)
#define __xprefix_member_list_header_pri__(name,...)    decltype(& __self__::name)

#define __xprefix_member_list_
#define __xprefix_member_list_pub__(name,...)           , decltype(& __self__::name)
#define __xprefix_member_list_pro__(name,...)           , decltype(& __self__::name)
#define __xprefix_member_list_pri__(name,...)           , decltype(& __self__::name)


#define __xprefix_none_
#define __xprefix_none_tmpl__(...)  <__VA_ARGS__>
#define __xprefix_none_pub__(...)
#define __xprefix_none_pro__(...)
#define __xprefix_none_pri__(...)
