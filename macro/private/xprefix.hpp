#ifndef xpack_macro_private_xprefix
#define xpack_macro_private_xprefix

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
#define __xprefix_first_member_tmpl__(...)
#define __xprefix_first_member_pub__(...)               __VA_ARGS__
#define __xprefix_first_member_pro__(...)               __VA_ARGS__
#define __xprefix_first_member_pri__(...)               __VA_ARGS__

#define __xprefix_member_
#define __xprefix_member_tmpl__(...)
#define __xprefix_member_pub__(...)                     , __VA_ARGS__
#define __xprefix_member_pro__(...)                     , __VA_ARGS__
#define __xprefix_member_pri__(...)                     , __VA_ARGS__

#define __xprefix_field_none
#define __xprefix_field_pub__(name,...)                 public:     __VA_ARGS__ name;
#define __xprefix_field_pro__(name,...)                 protected:  __VA_ARGS__ name;
#define __xprefix_field_pri__(name,...)                 private:    __VA_ARGS__ name;

#define __xprefix_first_member_list_
#define __xprefix_first_member_list_pub__(name,...)     & __self::name
#define __xprefix_first_member_list_pro__(name,...)     & __self::name
#define __xprefix_first_member_list_pri__(name,...)     & __self::name

#define __xprefix_member_list_
#define __xprefix_member_list_pub__(name,...)           , & __self::name
#define __xprefix_member_list_pro__(name,...)           , & __self::name
#define __xprefix_member_list_pri__(name,...)           , & __self::name

#define __xprefix_first_self_management_
#define __xprefix_first_self_management_pub__(...)      & mixc::macro_xgc::fake<__VA_ARGS__>::item
#define __xprefix_first_self_management_pro__(...)      & mixc::macro_xgc::fake<__VA_ARGS__>::item
#define __xprefix_first_self_management_pri__(...)      & mixc::macro_xgc::fake<__VA_ARGS__>::item

#define __xprefix_self_management_
#define __xprefix_self_management_pub__(...)            , & mixc::macro_xgc::fake<__VA_ARGS__>::item
#define __xprefix_self_management_pro__(...)            , & mixc::macro_xgc::fake<__VA_ARGS__>::item
#define __xprefix_self_management_pri__(...)            , & mixc::macro_xgc::fake<__VA_ARGS__>::item

#define __xprefix_keep_tmpl_
#define __xprefix_keep_tmpl_tmpl__(...)                 <__VA_ARGS__>
#define __xprefix_keep_tmpl_xtmpl(...)                  <__VA_ARGS__>
#define __xprefix_keep_tmpl_pub__(...)
#define __xprefix_keep_tmpl_pro__(...)
#define __xprefix_keep_tmpl_pri__(...)

#define __xprefix_first_1st_
#define __xprefix_first_1st_xnt__(first,...)            first

#define __xprefix_first_2nd_
#define __xprefix_first_2nd_xnt__(first,...)            __VA_ARGS__

#define __xprefix_1st_
#define __xprefix_1st_xnt__(first,...)                  , first

#define __xprefix_2nd_
#define __xprefix_2nd_xnt__(first,...)                  , __VA_ARGS__

#define __xprefix_first_arg_
#define __xprefix_first_arg_xnt__(first,...)            __VA_ARGS__ first
#define __xprefix_arg_
#define __xprefix_arg_xnt__(first,...)                  , __VA_ARGS__ first

#define __xprefix_first_fields_
#define __xprefix_first_fields_xnt__(first,...)         __VA_ARGS__ first

#define __xprefix_fields_
#define __xprefix_fields_xnt__(first,...)               ; __VA_ARGS__ first
#endif
