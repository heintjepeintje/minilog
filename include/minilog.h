#ifndef MINILOG_H_
#define MINILOG_H_

#include <stdint.h>
#include <stdio.h>

#ifndef ML_MAX_LOGGER_NAME_LENGTH
#	define ML_MAX_LOGGER_NAME_LENGTH 64
#endif

#ifndef ML_MAX_LOGGERS
#	define ML_MAX_LOGGERS 64
#endif

typedef uint32_t ml_logger_id_t;

typedef enum {
	ML_LOG_SEVERITY_TRACE,
	ML_LOG_SEVERITY_DEBUG,
	ML_LOG_SEVERITY_INFO,
	ML_LOG_SEVERITY_WARNING,
	ML_LOG_SEVERITY_ERROR,
	ML_LOG_SEVERITY_FATAL,
} ml_severity_t;

bool ml_create_logger(const char* name, uint32_t severity_flags, FILE* file, ml_logger_id_t logger_id);
void ml_log(ml_logger_id_t logger_id, ml_severity_t severity, const char *message, ...);
void ml_destroy_logger(ml_logger_id_t logger_id);

#endif // MINILOG_H_
