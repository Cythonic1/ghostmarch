#pragma once

#include <iostream>
#include <unordered_map>

/**
 * @brief Get the current local time as a formatted string.
 *
 * Format: "YYYY-MM-DD HH:MM:SS"
 *
 * @return Pointer to a static buffer containing the timestamp.
 *         Overwritten on each call. Not thread-safe.
 */
static inline const char *current_time() {
    static char buf[32];
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    return buf;
}

/* ---------------------------------------------------------------------------
 * Logging Macros
 * -------------------------------------------------------------------------*/

/**
 * @brief Log an error message to the global log file.
 *
 * Format:
 *   [ERROR] [YYYY-MM-DD HH:MM:SS] File <file> | Line <line> :: <message>
 *
 * Example:
 *   LOG_ERROR("could not open config file: %s", path);
 *
 * Notes:
 *   - Requires `logFile` to be opened globally.
 *   - Appends a newline automatically.
 */
#define LOG_ERROR(msg, ...)                                                                   \
    do {                                                                                      \
        fprintf(logFile, "\x1b[31m[ERROR]\x1b[0m [%s] File %s | Line %d :: ", current_time(), \
                __FILE__, __LINE__);                                                          \
        fprintf(logFile, msg, ##__VA_ARGS__);                                                 \
        fprintf(logFile, "\n");                                                               \
    } while (0)

/**
 * @brief Log a success/info message to both the log file and stdout.
 *
 * Format:
 *   [OKAY] [YYYY-MM-DD HH:MM:SS] <message>
 *
 * Example:
 *   OKAY("Theme switched to %s", theme);
 *
 * Notes:
 *   - Requires `logFile` to be opened globally.
 *   - Writes to both the log file and the terminal.
 *   - Appends a newline automatically.
 */
#define OKAY(MSG, ...)                                                                            \
    do {                                                                                          \
        fprintf(logFile, "\x1b[32m[OKAY]\x1b[0m  [%s] " MSG "\n", current_time(), ##__VA_ARGS__); \
        printf("\x1b[32m[OKAY]\x1b[0m " MSG "\n", ##__VA_ARGS__);                                 \
    } while (0)

/* ---------------------------------------------------------------------------
 * Global Variables
 * -------------------------------------------------------------------------*/

/**
 * @brief Map of Omarchy theme names to their corresponding Ghostty themes.
 *
 * Defined in the implementation file (omarchy_ghostty.cpp).
 * Declared here as extern to avoid multiple definition issues.
 */
extern std::unordered_map<std::string, std::string> omarchToGhostty;

/**
 * @brief Global log file handle.
 *
 * Opened in `main()` and used throughout the program for logging.
 */
extern FILE *logFile;

/* ---------------------------------------------------------------------------
 * Constants
 * -------------------------------------------------------------------------*/

/**
 * @brief Maximum buffer size for string operations.
 */
#define MAX_BUF 1024

/**
 * @brief Path to store the log file
 */
static const char *logPath = "/tmp/ghostty_omarchy_theme.log";

/* ---------------------------------------------------------------------------
 * Function Prototypes
 * -------------------------------------------------------------------------*/

/**
 * @brief Get the user’s home directory path.
 *
 * @return Newly allocated string containing the home directory path.
 *         Caller must free() the returned string.
 */
static inline char *getUserHome(void);

/**
 * @brief Construct the path to the Omarchy theme symlink.
 *
 * @return Newly allocated string containing the Omarchy theme path.
 *         Caller must free() the returned string.
 */
static char *getOmarchyPath(void);

/**
 * @brief Construct the path to the Ghostty configuration file.
 *
 * @return Newly allocated string containing the Ghostty config path.
 *         Caller must free() the returned string.
 */
static char *getGhosttyPath(void);

/**
 * @brief Extract the current theme name from Omarchy’s symlink.
 *
 * @param omarchyPath Path to Omarchy’s theme symlink.
 * @return Newly allocated string containing the theme name.
 *         Caller must free() the returned string.
 */
char *extract_theme(char *omarchyPath);

/**
 * @brief Modify Ghostty’s config file to apply the given theme.
 *
 * @param ghosttyTheme      The Ghostty theme name to apply.
 * @param ghosttyConfigPath Path to Ghostty’s configuration file.
 */
void modifieConfig(const char *ghosttyTheme, const char *ghosttyConfigPath,
                   const char *ghosttyConfigPathTmp);
