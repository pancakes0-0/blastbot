#pragma once

#define foreground_white            ( FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN )
#define foreground_yellow           ( FOREGROUND_RED | FOREGROUND_GREEN )
#define foreground_cyan             ( FOREGROUND_BLUE | FOREGROUND_GREEN )
#define foreground_magenta          ( FOREGROUND_RED | FOREGROUND_BLUE )
#define foreground_black            0

#define foreground_intense_red      ( FOREGROUND_RED | FOREGROUND_INTENSITY )
#define foreground_intense_green    ( FOREGROUND_GREEN | FOREGROUND_INTENSITY )
#define foreground_intense_blue     ( FOREGROUND_BLUE | FOREGROUND_INTENSITY )
#define foreground_intense_white    ( foreground_white | FOREGROUND_INTENSITY )
#define foreground_intense_yellow   ( foreground_yellow | FOREGROUND_INTENSITY )
#define foreground_intense_cyan     ( foreground_cyan | FOREGROUND_INTENSITY )
#define foreground_intense_magenta  ( foreground_magenta | FOREGROUND_INTENSITY )

namespace fmt_detail 
{
    inline int str_len ( const char* s ) {
        int n = 0;
        while ( s [ n ] ) ++n;
        return n;
    }

    inline void mem_copy ( char* dst, const char* src, int n ) {
        for ( int i = 0; i < n; ++i ) dst [ i ] = src [ i ];
    }

    inline int append ( char* buf, int pos, int cap, const char* src, int len ) {
        for ( int i = 0; i < len && pos < cap - 1; ++i )
            buf [ pos++ ] = src [ i ];
        return pos;
    }

    inline int append_str ( char* buf, int pos, int cap, const char* s ) {
        return s ? append ( buf, pos, cap, s, str_len ( s ) ) : append ( buf, pos, cap, "(null)", 6 );
    }

    inline int u64_to_dec ( uint64_t v, char out [ 24 ] ) {
        if ( v == 0 ) { out [ 0 ] = '0'; return 1; }
        char tmp [ 24 ];
        int  n = 0;
        while ( v ) { tmp [ n++ ] = '0' + ( char )( v % 10 ); v /= 10; }
        for ( int i = 0; i < n; ++i ) out [ i ] = tmp [ n - 1 - i ];
        return n;
    }

    inline int i64_to_dec ( int64_t v, char out [ 24 ] ) {
        if ( v < 0 ) {
            out [ 0 ] = '-';
            char tmp [ 24 ];
            int  n = u64_to_dec ( ( uint64_t )( -v ), tmp );
            mem_copy ( out + 1, tmp, n );
            return n + 1;
        }
        return u64_to_dec ( ( uint64_t )v, out );
    }

    inline int u64_to_hex ( uint64_t v, char out [ 18 ] ) {
        static const char digits [ ] = "0123456789abcdef";
        if ( v == 0 ) { out [ 0 ] = '0'; return 1; }
        char tmp [ 18 ];
        int  n = 0;
        while ( v ) { tmp [ n++ ] = digits [ v & 0xF ]; v >>= 4; }
        for ( int i = 0; i < n; ++i ) out [ i ] = tmp [ n - 1 - i ];
        return n;
    }

    inline void u32_to_hex8 ( uint32_t v, char out [ 9 ] ) {
        static const char digits [ ] = "0123456789ABCDEF";
        for ( int i = 7; i >= 0; --i ) {
            out [ i ] = digits [ v & 0xF ];
            v >>= 4;
        }
        out [ 8 ] = '\0';
    }

    inline int f64_to_str ( double v, char out [ 32 ] ) {
        int pos = 0;

        if ( v < 0.0 ) { out [ pos++ ] = '-'; v = -v; }

        uint64_t bits;
        __builtin_memcpy ( &bits, &v, 8 );
        uint64_t exp = ( bits >> 52 ) & 0x7FF;
        uint64_t mant = bits & 0x000F'FFFF'FFFF'FFFFULL;

        if ( exp == 0x7FF ) {
            const char* tag = mant ? "nan" : "inf";
            mem_copy ( out + pos, tag, 3 );
            return pos + 3;
        }

        auto  int_part = ( uint64_t )v;
        char  tmp [ 24 ];
        int   n = u64_to_dec ( int_part, tmp );
        mem_copy ( out + pos, tmp, n );
        pos += n;

        out [ pos++ ] = '.';
        double frac = v - ( double )int_part;
        for ( int i = 0; i < 6; ++i ) {
            frac *= 10.0;
            int d = ( int )frac;
            out [ pos++ ] = '0' + ( char )d;
            frac -= d;
        }

        return pos;
    }

    template<typename T>
    inline int fmt_arg ( char* buf, int pos, int cap, const T& v ) {

        if constexpr ( __is_same ( T, const char* ) || __is_same ( T, char* ) ) {
            return append_str ( buf, pos, cap, v );
        }

        else if constexpr ( __is_same ( T, bool ) ) {
            return append_str ( buf, pos, cap, v ? xx ( "true" ) : xx ( "false" ) );
        }

        else if constexpr ( __is_pointer ( T ) ) {
            char tmp [ 20 ] = { '0', 'x' };
            int  n = u64_to_hex ( ( uint64_t )( uintptr_t )v, tmp + 2 );
            return append ( buf, pos, cap, tmp, n + 2 );
        }

        else if constexpr ( __is_same ( T, float ) || __is_same ( T, double ) ) {
            char tmp [ 32 ];
            int  n = f64_to_str ( ( double )v, tmp );
            return append ( buf, pos, cap, tmp, n );
        }

        else if constexpr ( __is_same ( T, int8_t ) ||
            __is_same ( T, int16_t ) ||
            __is_same ( T, int32_t ) ||
            __is_same ( T, int64_t ) ||
            __is_same ( T, int ) ||
            __is_same ( T, long ) ||
            __is_same ( T, long long ) ) {
            char tmp [ 24 ];
            int  n = i64_to_dec ( ( int64_t )v, tmp );
            return append ( buf, pos, cap, tmp, n );
        }

        else {
            char tmp [ 24 ];
            int  n = u64_to_dec ( ( uint64_t )v, tmp );
            return append ( buf, pos, cap, tmp, n );
        }
    }

    template<typename... Args>
    inline int format ( char* buf, int cap, const char* fmt, const Args&... args ) {

        int pos = 0;

        auto do_format = [ & ] ( auto format_one_arg ) __attribute__ ( ( always_inline ) ) {
            while ( *fmt ) {
                if ( fmt [ 0 ] == '{' && fmt [ 1 ] == '}' ) {
                    pos = format_one_arg ( buf, pos, cap );
                    fmt += 2;
                    return;
                }
                if ( pos < cap - 1 ) buf [ pos++ ] = *fmt;
                ++fmt;
            }
        };

        ( do_format ( [ & ] ( char* b, int p, int c ) {
            return fmt_arg ( b, p, c, args );
            } ), ... );

        while ( *fmt ) {
            if ( pos < cap - 1 ) buf [ pos++ ] = *fmt;
            ++fmt;
        }

        buf [ pos ] = '\0';
        return pos;
    }

}

namespace blastbot::utils 
{
	class c_logging {
    private:
        HANDLE  handle = INVALID_HANDLE_VALUE;
        int16_t color = foreground_white;

        static void write_raw ( HANDLE h, const char* s, int len ) {
            if ( h == INVALID_HANDLE_VALUE || !s || !len ) return;
            DWORD written;
            WriteConsoleA ( h, s, ( DWORD )len, &written, nullptr );
        }

        void write_str ( const char* s ) {
            int n = 0;
            while ( s [ n ] ) ++n;
            write_raw ( handle, s, n );
        }

        void emit ( const char* buf, int len ) {
#ifdef _DEBUG
            SetConsoleTextAttribute ( handle, color );
            write_raw ( handle, buf, len );
            write_raw ( handle, "\n", 1 );
#endif
        }

    public:
        bool init ( const char* title );
        void shutdown ( );

        void set_color ( int16_t color ) { this->color = color; }
        void pop_color ( ) { color = foreground_white; }

        void log ( const char* text ) {
#ifdef _DEBUG
            if ( handle == INVALID_HANDLE_VALUE ) return;
            int n = 0; while ( text [ n ] ) ++n;
            emit ( text, n );
#endif
        }

        void log_prefixed ( const char* prefix, int16_t prefix_color, const char* fmt_str ) {
#ifdef _DEBUG
            if ( handle == INVALID_HANDLE_VALUE ) return;
            SetConsoleTextAttribute ( handle, prefix_color );
            write_str ( prefix );
            SetConsoleTextAttribute ( handle, color );
            write_str ( fmt_str );
            write_raw ( handle, "\n", 1 );
            SetConsoleTextAttribute ( handle, foreground_white );
#endif
        }

        template<typename... Args>
        void log ( const char* fmt, Args&&... args ) {
#ifdef _DEBUG
            if ( handle == INVALID_HANDLE_VALUE ) return;
            char buf [ 1024 ];
            int  len = fmt_detail::format ( buf, sizeof ( buf ), fmt, args... );
            emit ( buf, len );
#endif
        }
	};
	inline auto g_logging = std::make_unique<c_logging> ( );
}