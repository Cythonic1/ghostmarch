#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unordered_map>

#include "./mainHeader.h"

std::unordered_map<std::string, std::string> omarchToGhostty = {{"catppuccin", "Catppuccin Mocha"},
                                                                {"catppuccin-latte", "Catppuccin Latte"},
                                                                {"everforest", "Everforest Dark - Hard"},
                                                                {"gruvbox", "Gruvbox Dark Hard"},
                                                                {"kanagawa", "Kanagawa Dragon"},
                                                                {"matte-black", "tokyonight_night"},
                                                                {"nord", "Nord"},
                                                                {"osaka-jade", "Everforest Dark  Hard"},
                                                                {"ristretto", "TokyoNight"},
                                                                {"rose-pine", "Rose Pine Dawn"},
                                                                {"tokyo-night", "TokyoNight"}};

FILE *logFile = NULL;

static inline char *getUserHome(void) {
    char *env = getenv("HOME");
    if (env == NULL) {
        LOG_ERROR("could not find the HOME environment variable");
        exit(EXIT_FAILURE);
    }

    return strdup(env);
}

static char *getOmarchyPath(void) {
    char *homeDir = getUserHome();
    char *buf = (char *)malloc(1024);
    snprintf(buf, MAX_BUF, "%s/.config/omarchy/current/theme", homeDir);
    free(homeDir);
    return buf;
}

static char *getGhosttyPath(void) {
    char *homeDir = getUserHome();
    char *buffer = (char *)malloc(1024);

    snprintf(buffer, MAX_BUF, "%s/.config/ghostty/config", homeDir);
    free(homeDir);
    return buffer;
}

static char *getGhosttyPathTmp(void) {
    char *homeDir = getUserHome();
    char *buffer = (char *)malloc(1024);

    snprintf(buffer, MAX_BUF, "%s/.config/ghostty/config.tmp", homeDir);
    free(homeDir);
    return buffer;
}

char *extract_theme(char *omarchyPath) {
    char *buf = (char *)malloc(MAX_BUF);
    printf("omarch path is %s\n", omarchyPath);

    if (realpath(omarchyPath, buf) == NULL) {
        LOG_ERROR("error reading symbolic for omarchy");
        exit(EXIT_FAILURE);
    }

    OKAY("Omarch point at %s", buf);

    // Find the theme name
    char *tok = strtok(buf, "/");
    char *ptrtok = NULL;
    while (tok != NULL) {
        ptrtok = tok;
        tok = strtok(NULL, "/");
    }

    OKAY("Theme is %s", ptrtok);

    char *theme = strdup(ptrtok);
    free(buf);
    return theme;
}

void modifieConfig(const char *ghosttyTheme, const char *ghosttyConfigPath, const char *ghosttyConfigPathTmp) {
    FILE *ghosttyConfig = fopen(ghosttyConfigPath, "r");

    if (ghosttyConfig == NULL) LOG_ERROR("Error openning ghostty config file");
    FILE *tmp = fopen(ghosttyConfigPathTmp, "w");

    if (tmp == NULL) {
        LOG_ERROR("Error creating tmp config file");
        exit(EXIT_FAILURE);
    }

    char buf[MAX_BUF] = {0};
    char formatted_theme[MAX_BUF] = {0};
    snprintf(formatted_theme, MAX_BUF, "theme = \"%s\" \n", ghosttyTheme);

    int replaced = 0;
    while (fgets(buf, MAX_BUF, ghosttyConfig) != NULL) {
        if (strstr(buf, "theme =") != NULL && !replaced) {
            fputs(formatted_theme, tmp);
            replaced = 1;
        } else {
            fputs(buf, tmp);
        }
    }

    if (!replaced) {
        fputs(formatted_theme, tmp);
        OKAY("Theme not found using default theme tokyonight_night");
    } else {
        OKAY("Config file updated successfully");
    }

    if (ferror(ghosttyConfig)) {
        LOG_ERROR("Error reading ghostty config file");
        fclose(ghosttyConfig);
        fclose(tmp);
        remove(ghosttyConfigPathTmp);  // Clean up temp file
        exit(EXIT_FAILURE);
    }

    fclose(ghosttyConfig);
    fclose(tmp);

    if (rename(ghosttyConfigPathTmp, ghosttyConfigPath) != 0) {
        perror("rename");
        remove(ghosttyConfigPathTmp);
        LOG_ERROR("Error replacing original file");
    }
    OKAY("Theme has been changed");

    // temp
    system("killall ghostty -SIGUSR2");
}

int main(int argc, char *argv[]) {
    char *omarchyPath = getOmarchyPath();
    char *ghosttyPath = getGhosttyPath();
    char *ghosttyPathTmp = getGhosttyPathTmp();

    logFile = fopen(logPath, "a");

    if (!logFile) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    // Should free theme
    char *theme = extract_theme(omarchyPath);
    std::string ghostty = omarchToGhostty[std::string(theme)];
    if (ghostty.length() == 0) {
        LOG_ERROR("theme not found...");
        free(theme);
        free(ghosttyPath);
        free(omarchyPath);
        free(ghosttyPathTmp);
        exit(EXIT_FAILURE);
    }

    OKAY("ghostty theme should be %s for this omarch theme %s", ghostty.c_str(), theme);

    // appliy the theme.
    modifieConfig(ghostty.c_str(), ghosttyPath, ghosttyPathTmp);

    // Clean up and free everything..
    free(theme);
    free(ghosttyPath);
    free(omarchyPath);
    free(ghosttyPathTmp);

    OKAY("Exit SUCCESS");
    fclose(logFile);

    // Exitting...
    return EXIT_SUCCESS;
}
