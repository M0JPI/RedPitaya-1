#pragma once

#include <DataManager.h>
#include <CustomParameters.h>

#include <sys/syslog.h> //Add custom RP_LCR LOG system
#include "rp.h"

#define __SHORT_FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define FATAL(X)  {fprintf(stderr, "Error at line %d, file %s errno %d [%s] %s\n", __LINE__, __SHORT_FILENAME__, errno, strerror(errno),X); exit(1);}
#define WARNING(...) { char error_msg[512]; snprintf(error_msg,512,__VA_ARGS__);fprintf(stderr,"[W] %s:%d %s\n",__SHORT_FILENAME__,__LINE__,error_msg);}

#ifdef TRACE_ENABLE
#define TRACE(...) { char error_msg[512]; snprintf(error_msg,512,__VA_ARGS__);fprintf(stderr,"[T] %s:%d %s\n",__SHORT_FILENAME__,__LINE__,error_msg);}
#define TRACE_SHORT(...) { char error_msg[512]; snprintf(error_msg,512,__VA_ARGS__);fprintf(stderr,"[T] %s\n",error_msg);}
#else
#define TRACE(...)
#define TRACE_SHORT(...)
#endif


#define CH_SIGNAL_SIZE_DEFAULT      1024

#define EXEC_CHECK_MUTEX(x, mutex){ \
 		int retval = (x); \
 		if(retval != RP_OK) { \
            pthread_mutex_unlock((&mutex)); \
 			return retval; \
 		} \
}

#define IF_VALUE_CHANGED(X, ACTION) \
if (X.Value() != X.NewValue()) { \
    int res = ACTION;\
    if (res == RP_OK) { \
        X.Update(); \
    } \
}

#define IF_VALUE_CHANGED_BOOL(X, ACTION1, ACTION2) \
if (X.Value() != X.NewValue()) { \
    if (X.NewValue()) { \
        ACTION1;    X.Update(); \
    } else { \
        ACTION2;    X.Update(); }}

#define IS_NEW(X) X.Value() != X.NewValue()


#ifdef __cplusplus
extern "C" {
#endif




/* Parameters description structure - must be the same for all RP controllers */
typedef struct rp_app_params_s {
    char  *name;
    float  value;
    int    fpga_update;
    int    read_only;
    float  min_val;
    float  max_val;
} rp_app_params_t;


/* Signal measurement results structure - filled in worker and updated when
 * also measurement signal is stored from worker
 */
typedef struct rp_osc_meas_res_s {
    float min;
    float max;
    float amp;
    float avg;
    float freq;
    float period;
} rp_osc_meas_res_t;




//Rp app functions
const char *rp_app_desc(void);
int rp_app_init(void);
int rp_app_exit(void);
int rp_set_params(rp_app_params_t *p, int len);
int rp_get_params(rp_app_params_t **p);
int rp_get_signals(float ***s, int *sig_num, int *sig_len);

auto getModelS() -> std::string;
auto getMaxADC() -> uint32_t;

void updateParametersByConfig();

#ifdef __cplusplus
}
#endif
