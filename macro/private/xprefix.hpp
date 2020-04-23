#pragma once
#define xnt             xnt__
#define xpub(...)       pub__(__VA_ARGS__)
#define xpro(...)       pro__(__VA_ARGS__)
#define xpri(...)       pri__(__VA_ARGS__)
#define xhas(...)       has__(__VA_ARGS__)

#define __xprefix_
#define __xprefix_first_base_
#define __xprefix_first_base_tmpl__(...)                      
#define __xprefix_first_base_pub__(...)                 : public      __VA_ARGS__
#define __xprefix_first_base_pro__(...)                 : protected   __VA_ARGS__
#define __xprefix_first_base_pri__(...)                 : private     __VA_ARGS__

#define __xprefix_base_
#define __xprefix_base_tmpl__(...)                      
#define __xprefix_base_pub__(...)                       , public      __VA_ARGS__
#define __xprefix_base_pro__(...)                       , protected   __VA_ARGS__
#define __xprefix_base_pri__(...)                       , private     __VA_ARGS__

#define __xprefix_first_member_
#define __xprefix_first_member_pub__(...)               __VA_ARGS__
#define __xprefix_first_member_pro__(...)               __VA_ARGS__
#define __xprefix_first_member_pri__(...)               __VA_ARGS__
#define __xprefix_first_member_has__(...)               __VA_ARGS__

#define __xprefix_member_
#define __xprefix_member_pub__(...)                     , __VA_ARGS__
#define __xprefix_member_pro__(...)                     , __VA_ARGS__
#define __xprefix_member_pri__(...)                     , __VA_ARGS__
#define __xprefix_member_has__(...)                     , __VA_ARGS__

#define __xprefix_field_
#define __xprefix_field_pub__(name,...)                 public:     __VA_ARGS__ name;
#define __xprefix_field_pro__(name,...)                 protected:  __VA_ARGS__ name;
#define __xprefix_field_pri__(name,...)                 private:    __VA_ARGS__ name;
#define __xprefix_field_has__(name,...)                 

#define __xprefix_first_member_list_
#define __xprefix_first_member_list_pub__(name,...)     & the_t::name
#define __xprefix_first_member_list_pro__(name,...)     & the_t::name
#define __xprefix_first_member_list_pri__(name,...)     & the_t::name
#define __xprefix_first_member_list_has__(...)          & mixc::macro_xgc::fake<__VA_ARGS__>::item

#define __xprefix_member_list_
#define __xprefix_member_list_pub__(name,...)           , & the_t::name
#define __xprefix_member_list_pro__(name,...)           , & the_t::name
#define __xprefix_member_list_pri__(name,...)           , & the_t::name
#define __xprefix_member_list_has__(...)                , & mixc::macro_xgc::fake<__VA_ARGS__>::item
