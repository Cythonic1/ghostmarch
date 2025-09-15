#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unordered_map>

#define OmarchPath "/home/pythonic/.config/omarchy/current/theme"
#define GhosttyConfigFilePath "/home/pythonic/.config/ghostty/config"
#define tmpFile "tmp"
#define MAX_BUF 1024

#define LOG(msg, ...)                                                    \
    {                                                                    \
        fprintf(logFile, "File %s | Line %d :: \n", __FILE__, __LINE__); \
        fprintf(logFile, (msg), ##__VA_ARGS__);                          \
        printf("\n");                                                    \
    }

#define OKAY(MSG, ...) printf("[+] " MSG "\n", ##__VA_ARGS__)

std::unordered_map<std::string, std::string> omarchToGhostty = {
    {"catppuccin", "catppuccin-mocha"},
    {"catppuccin-latte", "catppuccin-latte"},
    {"everforest", "Everforest Dark - Hard"},
    {"gruvbox", "GruvboxDarkHard"},
    {"kanagawa", "Kanagawa Dragon"},
    {"matte-black", "tokyonight_night"},
    {"nord", "nord"},
    {"osaka-jade", "Everforest Dark - Hard"},
    {"ristretto", "tokyonight_night"},
    {"rose-pine", "rose-pine-dawn"},
    {"tokyo-night", "tokyonight_night"}};

FILE *logFile = NULL;

char *extract_theme() {
    char *buf = (char *)malloc(MAX_BUF);

    if (realpath(OmarchPath, buf) == NULL) {
        LOG("error reading symbolic for omarchy");
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG
    OKAY("Omarch point at %s", buf);
#endif  // DEBUG

    // Find the theme name
    char *tok = strtok(buf, "/");
    char *ptrtok = NULL;
    while (tok != NULL) {
        ptrtok = tok;
        tok = strtok(NULL, "/");
    }

#ifdef DEBUG
    OKAY("Theme is %s\n", ptrtok);
#endif  // DEBUG

    char *theme = strdup(ptrtok);
    free(buf);
    return theme;
}

void modifieConfigAndSendSignal(const char *ghosttyTheme) {
    FILE *ghosttyConfig = fopen(GhosttyConfigFilePath, "r");

    if (ghosttyConfig == NULL) LOG("Error openning ghostty config file");
    FILE *tmp = fopen(tmpFile, "w");

    if (tmp == NULL) {
        LOG("Error creating tmp config file");
        exit(EXIT_FAILURE);
    }

    char buf[2048] = {0};
    char formatted_theme[MAX_BUF] = {0};
    snprintf(formatted_theme, MAX_BUF, "theme = \"%s\" \n", ghosttyTheme);

    int replaced = 0;
    while (fgets(buf, 2048, ghosttyConfig) != NULL) {
        if (strstr(buf, "theme =") != NULL && !replaced) {
            fputs(formatted_theme, tmp);
            LOG("Found the replaced the theme to %s", ghosttyTheme);
            replaced = 1;
        } else {
            fputs(buf, tmp);
        }
    }

    if (!replaced) {
        fputs(formatted_theme, tmp);
#ifdef DEBUG
        OKAY("Theme not found using default theme tokyonight_night");
#endif  // DEBUG
    } else {
#ifdef DEBUG
        OKAY("Config file updated successfully");
#endif  // DEBUG
    }

    // Check for read errors
    if (ferror(ghosttyConfig)) {
        LOG("Error reading ghostty config file");
        fclose(ghosttyConfig);
        fclose(tmp);
        remove(tmpFile);  // Clean up temp file
        exit(EXIT_FAILURE);
    }

    fclose(ghosttyConfig);
    fclose(tmp);

    if (rename(tmpFile, GhosttyConfigFilePath) != 0) {
        remove(tmpFile);
        LOG("Error replacing original file");
    }
    LOG("Theme has been changed");
}

int main(int argc, char *argv[]) {
    logFile = fopen("log.txt", "a");

    if (!logFile) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    // Should free theme
    char *theme = extract_theme();
    std::string ghostty = omarchToGhostty[std::string(theme)];
    if (ghostty.length() == 0) {
        LOG("theme not found...");
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG
    OKAY("ghostty theme should be %s for this omarch theme %s", ghostty.c_str(), theme);
#endif  // DEBUG

    modifieConfigAndSendSignal(ghostty.c_str());
    return EXIT_SUCCESS;
}
