#include "./minilog.h"

#include <string.h>
#include <time.h>
#include <stdarg.h>

#define ML_MAX_SEVERITY_NAME_LENGTH 32
#define ML_MAX_SEVERITY_COLOR_LENGTH 32
#define ML_TEMP_MESSAGE_BUFFER_LENGTH 256
#define ML_TEMP_TIME_STRING_BUFFER_LENGTH 32

struct ml_logger_data {
	char name[ML_MAX_LOGGER_NAME_LENGTH + 1];
	uint32_t severity_flags;
	FILE *file;
};

struct ml_severity_data {
	char name[ML_MAX_SEVERITY_NAME_LENGTH];
	char color[ML_MAX_SEVERITY_COLOR_LENGTH];
};

struct ml_logger_data loggers[ML_MAX_LOGGERS] = {0};

struct ml_severity_data severity_data[] = {
	{ .name = "TRACE", .color = "\033[35m" },
	{ .name = "DEBUG", .color = "\033[32m" },
	{ .name = "INFO", .color = "\033[34m" },
	{ .name = "WARNING", .color = "\033[33m" },
	{ .name = "ERROR", .color = "\033[31m" },
	{ .name = "FATAL", .color = "\033[37;41m" }
};

bool ml_create_logger(const char* name, uint32_t severity_flags, FILE* file, ml_logger_id_t logger_id) {
	if (logger_id >= ML_MAX_LOGGERS) return false;

	struct ml_logger_data* logger = &loggers[logger_id];
	if (logger->file != NULL) return false;
	if (name == NULL || file == NULL) return false;

	const size_t name_length = strlen(name);
	if (name_length > ML_MAX_LOGGER_NAME_LENGTH) return false;
	memcpy(logger->name, name, name_length * sizeof(char));

	logger->severity_flags = severity_flags;
	logger->file = file;

	return true;
}

void ml_log(ml_logger_id_t logger_id, ml_severity_t severity, const char *message, ...) {
	if (logger_id > ML_MAX_LOGGERS) return;

	struct ml_logger_data* logger = &loggers[logger_id];
	if (logger->file == NULL) return;

	if (severity < ML_LOG_SEVERITY_TRACE || severity > ML_LOG_SEVERITY_FATAL) return;
	if (!(logger->severity_flags & (1 << severity))) return; 
	struct ml_severity_data* current_severity_data = &severity_data[severity];

	va_list arguments;
	va_start(arguments, message);

	char buffer[ML_TEMP_MESSAGE_BUFFER_LENGTH] = {0};
	vsnprintf(buffer, ML_TEMP_MESSAGE_BUFFER_LENGTH, message, arguments);

	time_t time_seconds = time(NULL);
	struct tm* time_data = localtime(&time_seconds);

	char timestr[ML_TEMP_TIME_STRING_BUFFER_LENGTH] = {0};
	strftime(timestr, ML_TEMP_TIME_STRING_BUFFER_LENGTH, "%X", time_data);

	fprintf(logger->file, "[%s] [%s%s\033[0m] %s > %s\n", timestr, current_severity_data->color, current_severity_data->name, logger->name, buffer);

	va_end(arguments);
}

void ml_destroy_logger(ml_logger_id_t logger_id) {
	if (logger_id > ML_MAX_LOGGERS) return;

	struct ml_logger_data* logger = &loggers[logger_id];
	memset(logger->name, 0, ML_MAX_LOGGER_NAME_LENGTH);

	logger->file = NULL;
	logger->severity_flags = 0;

	return;
}
