#ifndef TRANSFORM_UTIL
#define TRANSFORM_UTIL
#include <string>
#include <typeinfo>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

typedef unsigned char Byte;
typedef Byte* PByte;
typedef Byte* PByteC;
typedef unsigned short UShort;
typedef unsigned int UInt32;

UShort CRC16(PByte puchMsg, UShort usDataLen);
template<typename ... Args>
std::string str_format(const char* pformat, Args... args) {
	int len_str = std::snprintf(nullptr, 0, pformat, args...);
	if (0 >= len_str)
		return std::string("");
	len_str++;
	char* pstr_out = nullptr;
	pstr_out = new (std::nothrow)char[len_str];
	if (NULL == pstr_out || nullptr == pstr_out)
		return std::string("");
	std::snprintf(pstr_out, len_str, pformat, args...);
	std::string str(pstr_out);
	delete pstr_out;
	pstr_out = nullptr;
	return str;
};

//#ifdef NOLOG
//#define LOG(...)
//#define LogInfo(...)
//#define LogError(...)
//#define LogWarning(...)
//#define LogDebug(...)
//#define log(...)
//#define logInfo(...)
//#define logError(...)
//#define logWarning(...)
//#define logDebug(...)
//#else
//#define NONECOLOR                 "\033[0m"
//#define BLACKCOLOR                "\033[0;30m"
//#define L_BLACKCOLOR              "\033[1;30m"
//#define REDCOLOR                  "\033[0;31m"
//#define L_REDCOLOR                "\033[1;31m"
//#define GREENCOLOR                "\033[0;32m"
//#define L_GREENCOLOR              "\033[1;32m"
//#define BROWNCOLOR                "\033[0;33m"
//#define YELLOWCOLOR               "\033[1;33m"
//#define BLUECOLOR                 "\033[0;34m"
//#define L_BLUECOLOR               "\033[1;34m"
//#define PURPLECOLOR               "\033[0;35m"
//#define L_PURPLECOLOR             "\033[1;35m"
//#define CYANCOLOR                 "\033[0;36m"
//#define L_CYANCOLOR               "\033[1;36m"
//#define GRAYCOLOR                 "\033[0;37m"
//#define WHITECOLOR                "\033[1;37m"
//
//#define BOLDCOLOR                 "\033[1m"
//#define UNDERLINECOLOR            "\033[4m"
//#define BLINKCOLOR                "\033[5m"
//#define REVERSECOLOR              "\033[7m"
//#define HIDECOLOR                 "\033[8m"
//#define CLEARCOLOR                "\033[2J"
//#define CLRLINECOLOR              "\r\033[K" //or "\033[1K\r"
//
//#ifdef NOCOLOR
//#define ERRORSTART "Error:"
//#define ERROREND 
//#define WARNINGSTART "Warning:"
//#define WARNINGEND
//#else
//#define ERRORSTART REDCOLOR "Error:" BROWNCOLOR
//#define ERROREND NONECOLOR
//#define WARNINGSTART PURPLECOLOR "Warning:" BLUECOLOR
//#define WARNINGEND NONECOLOR
//#endif
//#define INFOSTART "Info:"
//#define INFOEND 
//#define DEBUGSTART "Debug:"
//#define DEBUGEND 
//
//void consolelogger_log(const char* format, ...);
//
//#ifdef NOTIME
//#define TIMEARG(_ptm)
//#define TIMEFORMAT
//#else
//#define TIMEARG(_ptm) ((_ptm)->tm_year + 1900), ((_ptm)->tm_mon + 1), (_ptm)->tm_mday, (_ptm)->tm_hour, (_ptm)->tm_min, (_ptm)->tm_sec
//#define TIMEFORMAT "%d-%d-%d %d:%d:%d "
//#endif
//
//#ifdef NOLOCALTION
//#define LOCALTIONARG
//#define LOCALTION
//#else
//#define LOCALTIONARG(_modulename) _modulename,__FILE__, __FUNCTION__, __LINE__
//#define LOCALTION " %s %s %s():%d "
//#endif
//
//#define LOG(format,...) { consolelogger_log(format,##__VA_ARGS__); }
//#define log(format,...) { consolelogger_log(format,##__VA_ARGS__); }
//
//#ifdef NOLOGINFO
//#define LogInfo(...)
//#define logInfo(...)
//#else
//#define LogInfo(modulename,format,...) do{   \
//    time_t now;         \
//    struct tm*  localnow =new tm() ;                      \
//    time(&now);                     \
//    localtime_s(localnow,&now);                     \
//    LOG(TIMEFORMAT INFOSTART format LOCALTION INFOEND,TIMEARG(localnow),##__VA_ARGS__,LOCALTIONARG(modulename));\
//    }while(false)
//#define logInfo(format,...) LogInfo("GEOMETRYBOOLEAN",format,##__VA_ARGS__)
//#endif
//
//#ifdef NOLOGERROR
//#define LogError(...)
//#define logError(...)
//#else
//#define LogError(modulename,format,...) do{   \
//    time_t now;         \
//    struct tm*  localnow =new tm() ;                      \
//    time(&now);                     \
//    localtime_s(localnow,&now);                     \
// LOG(TIMEFORMAT ERRORSTART format LOCALTION  ERROREND,TIMEARG(localnow),##__VA_ARGS__,LOCALTIONARG(modulename));\
//    }while(false)
//#define logError(format,...) LogError("GEOMETRYBOOLEAN",format,##__VA_ARGS__)
//#endif
//
//#ifdef NOLOGWARNING
//#define LogWarning(...)
//#define logWarning(...)
//#else
//#define LogWarning(modulename,format,...) do{   \
//    time_t now;         \
//    struct tm*  localnow =new tm() ;                      \
//    time(&now);                     \
//    localtime_s(localnow,&now);                     \
//    LOG(TIMEFORMAT WARNINGSTART LOCALTION format WARNINGEND,TIMEARG(localnow),LOCALTIONARG(modulename),##__VA_ARGS__);\
//    }while(false)
//#define logWarning(format,...) LogWarning("GEOMETRYBOOLEAN",format,##__VA_ARGS__)
//#endif
//
//#ifdef NOLOGDEBUG
//#define LogDebug(...)
//#define logDebug(...)
//#else
//#define LogDebug(modulename,format,...) do{   \
//    time_t now;         \
//    struct tm*  localnow =new tm() ;                      \
//    time(&now);                     \
//    localtime_s(localnow,&now);                     \
//    LOG(TIMEFORMAT DEBUGSTART LOCALTION  DEBUGEND,TIMEARG(localnow),LOCALTIONARG(modulename),##__VA_ARGS__);\
//    }while(false)
//#define logDebug(format,...) LogDebug("GEOMETRYBOOLEAN",format,##__VA_ARGS__)
//#endif
//
//void consolelogger_log(const char* format, ...)
//{
//    va_list args;
//    va_start(args, format);
//    (void)vprintf(format, args);
//    va_end(args);
//    (void)printf("\n");
//}
//#endif
#endif
