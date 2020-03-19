#pragma once
#define xnt                                 xnt__
#define __xprefix_xheader_1st_
#define __xprefix_xheader_1st_xnt__(first,...)         first

#define __xprefix_xheader_2nd_
#define __xprefix_xheader_2nd_xnt__(first,...)         __VA_ARGS__

#define __xprefix_xget_1st_
#define __xprefix_xget_1st_xnt__(first,...)             , first

#define __xprefix_xget_2nd_
#define __xprefix_xget_2nd_xnt__(first,...)             , __VA_ARGS__

#define __xprefix_xheader_arg_
#define __xprefix_xheader_arg_xnt__(first,...)          __VA_ARGS__ first
#define __xprefix_xget_arg_
#define __xprefix_xget_arg_xnt__(first,...)             , __VA_ARGS__ first

#define __xprefix_xheader_fields_
#define __xprefix_xheader_fields_xnt__(first,...)       __VA_ARGS__ first

#define __xprefix__xget_fields_
#define __xprefix__xget_fields_xnt__(first,...)         ; __VA_ARGS__ first
