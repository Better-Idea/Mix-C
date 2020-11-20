#define xuser mixc::leon
#include"meta/has_cast.hpp"
#include<functional>
#include<stdio.h>

namespace xuser{
    enum class match_result_t{
        ack,
        nak,
    };

    inline bool start_with(const char * str, const char * value){
        while(str[0] == value[0]){
            str            += 1;
            value          += 1;
        }
        return value[0] == '\0';
    }

    inline bool equals(const char * str, const char * value){
        while(str[0] == value[0]){
            if (str[0] == '\0'){
                return true;
            }
            str            += 1;
            value          += 1;
        }
        return false;
    }

    using match_invoke  = std::function<void(asciis, match_result_t &, bool &, bool &)>;
    using send_invoke   = std::function<void(char *)>;

    template<uxx unit>
    struct words{
        uxx items[unit];
    };

    struct mutex{
        void wait(){}
        void release(){}
    };

    struct at_pack{
        send_invoke     send;
        match_invoke    match;
        mutex           barrier;
        bstate_t        exec_result;
    };

    template<uxx len>
    struct at_str{
        constexpr at_str(asciis value = ""){
            auto ptr = items;

            for(; value[0] != '\0'; ptr++, value++){
                if (value[0] == '\\' or value[0] == '\"'){
                    ptr[0]  = '\\';
                    ptr[1]  = value[0];
                    ptr    += 1;
                }
                else{
                    ptr[0]  = value[0];
                }
            }
            ptr[0] = '\0';
        }

        operator asciis (){
            return items;
        }
    private:
        char items[len];
    };

    struct at_cmd_queue{
        void push(at_pack && pack){
            // 前台提交请求并等待完成
            pack.barrier.wait();
        }

        at_pack & pop(){
            // 后台响应请求并在完成后解锁
        }
    };

    static inline at_cmd_queue at;

    constexpr ixx ignore = ixx(1) << (sizeof(ixx) * 8 - 1);

    union at_arg{
        template<class item_t>
        at_arg(item_t const & value){
            item_t & v = (item_t &)value;

            if constexpr (has_cast<asciis, item_t>){
                s = v[0] != '\0' ? asciis(v) : asciis(ignore);
            }
            else{
                i = ixx(v);
            }
        }

        asciis s;
        ixx    i = ignore;
    };

    template<class a0, class ... args>
    inline match_result_t fetch(asciis line, asciis fmt, a0 & first, args & ... rest){
        return sscanf_s(line, fmt, & first, (& rest)...) == 1 + sizeof...(args) ? match_result_t::ack : match_result_t::nak;
    }

    inline uxx truncated_last(char * str, uxx length, char value){
        while(length-- != 0){
            if (str[length] == value){
                break;
            }
        }
        str[length] = '\0';
        return length;
    }

    template<uxx i>
    inline void cmd(char * buffer, char * fmt, uxx fmt_len, at_arg * params){
        if constexpr (i != 0){
            if (auto v = params[i]; v.i == ignore){
                fmt_len         = truncated_last(fmt, fmt_len, ',');
                return cmd<i - 1>(buffer, fmt, fmt_len, params);
            }
        }
        sprintf(buffer, fmt, *(words<i> *)(params));
    }

    #define xat_send(fmt,...)                                           \
    .send = [&](char * buffer) {                                        \
        char __fmt[] = fmt;                                             \
        at_arg __arg[] = { ""/*ignore*/, __VA_ARGS__ };                 \
        constexpr uxx __i = sizeof(__arg) / sizeof(__arg[0]) - 1;       \
        cmd<__i>(buffer, __fmt, sizeof(fmt), __arg + 1);                \
    }

    #define xat_respx               .match = [&](asciis line, match_result_t & result, bool & loop, bool & without_ok)
    #define xat_resp(...)           xat_respx { result = fetch(line, __VA_ARGS__); }

    #define xat_cmd(func_name,data_type,token,fmt,...)                  \
    void func_name(data_type * value_ptr){                              \
        auto & value = value_ptr[0];                                    \
                                                                        \
        at.push(at_pack{                                                \
            xat_send("AT" token "?"),                                   \
            xat_resp(token ":" fmt, __VA_ARGS__)                        \
        });                                                             \
    }                                                                   \
                                                                        \
    void func_name(data_type const & value){                            \
        at.push(at_pack{                                                \
            xat_send("AT" token "=" fmt, __VA_ARGS__)                   \
        });                                                             \
    }

    struct sysmsg{
        uxx quit_with_info  : 1;
        uxx new_format      : 1;
        uxx                 : sizeof(uxx) * 8 - 2;

        operator ixx &(){
            return ixxp(this)[0];
        }
    };

    struct rfpower{
        ixx wifi_power;
        ixx blue_advertising_power  = ignore;
        ixx blue_scan_power         = ignore;
        ixx blue_connect_power      = ignore;
    };

    struct cwmode{
        uxx enable_station  : 1;
        uxx enable_softap   : 1;
        uxx                 : sizeof(uxx) * 8 - 2;

        operator ixx & (){
            return ixxp(this)[0];
        }
    };

    struct cwjap{
        at_str<64>  ssid;
        at_str<32>  password;
        at_str<64>  bssid               = "";
        ixx         enable_pci          = ignore;
        ixx         enable_reconnect    = ignore;
    };

    enum class error_cwjap_t : uxx{
        none,
        connection_timeout,
        wrong_password,
        target_missing,
        conncetion_failed,
        other,
    };

    void at_sysram(uxx * value){
        at.push(at_pack{
            xat_send("AT+SYSRAM?"),
            xat_resp("+SYSRAM:%u", value)
        });
    }

    xat_cmd(at_sysmsg, sysmsg, "+SYSMSG", "%u", value)
    xat_cmd(at_rfpower, rfpower, "+RFPOWER", "%d,%d,%d,%d", value.wifi_power, value.blue_advertising_power, value.blue_scan_power, value.blue_connect_power)
    xat_cmd(at_cwmode, cwmode, "+CWMODE", "%d", value)

    error_cwjap_t at_cwjap(cwjap const & value){
        error_cwjap_t error_code = error_cwjap_t::none;

        at.push(at_pack{
            xat_send("AT+CWJAP=%s,%s,%s,%d,%d", value.ssid, value.password, value.bssid, value.enable_pci, value.enable_reconnect),
            xat_resp("+CWJAP:%u", error_code)
        });
        return error_code;
    }
}
