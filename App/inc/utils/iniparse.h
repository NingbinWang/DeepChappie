#ifndef _INIPARSE_H_
#define _INIPARSE_H_
#include "app_macro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#ifdef  __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define INIPARSE_MAX_SECTIONS   (64)
#define INIPARSE_MAX_CONFIGS    (64)
#define INIPARSE_DEF_TMPLEN     (4)

/*mode*/
typedef enum
{
    START,
    NEW_SECTION,    
    IN_SECTION,
    END_OF_FILE
}STATES_E;


typedef struct
{
    char *key;
    char *value;
}INIPARSE_CONFIG_T;

typedef struct
{
    char *name;
    int num_configs;
    INIPARSE_CONFIG_T configs[INIPARSE_MAX_CONFIGS];
}INIPARSE_SECTION_T;


/* @name:   iniparse
 * @brief:  INI解析结构体
 */
typedef struct
{
    char *filename;
    char *tmp;
    int tmp_cap;
    int current_line;
    int state;
    int num_sections;
    INIPARSE_SECTION_T sections[INIPARSE_MAX_SECTIONS];  
}INIPARSE_T;

void* iniparse_load(const char *inifile);
void iniparse_free(void* h);
void iniparse_dump(void* h);
char* iniparse_get_value(void* h, const char *section, const char *key);


#ifdef  __cplusplus
}
#endif  /* __cplusplus */

#endif