#ifndef _TRANSENT_UTIL_
#define _TRANSENT_UTIL_

#ifdef DEBUG
#include <errno.h>
//#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define LOGERR(M, ...) fprintf(stderr, "[ERROR - line:%d] in FILE[%s] at FUNC[%s]:" M "\n", __LINE__, __FILE__, __func__, /*clean_errno(),*/ ##__VA_ARGS__)
#define LOGWARN(M, ...) fprintf(stderr, "[WARN - line:%d] in FILE[%s] at FUNC[%s]:" M "\n",__LINE__, __FILE__, __func__, /*clean_errno(),*/ ##__VA_ARGS__)
#define LOGINFO(M, ...) fprintf(stderr, "[INFO - line:%d] in FILE[%s] at FUNC[%s]:" M "\n", __LINE__, __FILE__, __func__, /*clean_errno(),*/ ##__VA_ARGS__)
#define check(A, M, ...) if(!(A)) { LOGERR(M, ##__VA_ARGS__);}
#define check_mem(A) check(A, "Memory allocation error.")
#else
#define LOGERR(M, ...)
#define LOGWARN(M, ...)
#define LOGINFO(M, ...) 
#define check(A, M, ...)
#define check_mem(A)
#endif

#endif