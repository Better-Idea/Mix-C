#pragma once
#define xnt                                 xnt__
#define __xheader_1st_
#define __xheader_1st_xnt__(first,...)      first
#define __xheader_2nd_
#define __xheader_2nd_xnt__(first,...)      __VA_ARGS__
#define __xget_1st_
#define __xget_2nd_
#define __xget_1st_xnt__(first,...)         , first
#define __xget_2st_xnt__(first,...)         , __VA_ARGS__

#define __xheader_arg_
#define __xheader_arg_xnt__(first,...)      __VA_ARGS__ first
#define __xget_arg_
#define __xget_arg_xnt__(first,...)         , __VA_ARGS__ first

#define __xheader_fields_
#define __xheader_fields_xnt__(first,...)   __VA_ARGS__ first
#define __xget_fields_
#define __xget_fields_xnt__(first,...)      ; __VA_ARGS__ first
