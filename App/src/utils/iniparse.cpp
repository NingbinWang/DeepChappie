#include "iniparse.h"
#include <string.h>

/* Trims whitespace from the passed in string between begin_ind and
 * end_ind characters in the string. It returns the begin index and
 * the end index of the first and last valid (non-whitespace)
 * characters in the string. */
static void iniparse_trim(char *str, int *begin_ind, int *end_ind)
{
    int b = *begin_ind;
    int e = *end_ind;
    int i;

    /* find first index of non-whitespace character */
    for (i = b; i < e; i++)
    {
        char c = str[i];
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
            break;
    }
    *begin_ind = i;
            
    /* find last index of non-whitespace character */
    for (i = e-1; i >= b; i--)
    {
        char c = str[i];
        if(c != ' ' && c != '\t' && c != '\n' && c != '\r')
            break;
    }
    *end_ind = i;
}

static int iniparse_readline(FILE *fp, INIPARSE_T *inip)
{
    int i = 0;
    int b;
    int new = 0;

    for (;;)
    {
        char c;

        /* resize buffer if necessary */
        if (i >= inip->tmp_cap)
        {
            inip->tmp_cap *= 2;
            inip->tmp = realloc(inip->tmp, sizeof(*inip->tmp) * inip->tmp_cap);
            if (inip->tmp == NULL)
            {
                printf("realloc tmp fail\n");
                break;
            }
        }

        /* read next byte from file */
        b = getc(fp);

        /* if end of file then end */
        if (b == EOF)
        {
            inip->state = END_OF_FILE;
            break;
        }

        /* test for end of line */
        c = b;
        if (c == '\r' || c == '\n')
        {
            inip->current_line++;
            break;
        }

        /* place char in buffer */
        inip->tmp[i++] = c;     
        new = 1;
    }
    
    /* place end-of-string marker */
    if (new && inip->tmp)
        inip->tmp[i] = '\0';

    return new;
}

static INIPARSE_SECTION_T *at_iniparse_section(FILE *fp, INIPARSE_T *inip)
{
    int new;
    INIPARSE_SECTION_T *section = NULL;

    APP_RET_RETURN((inip->num_sections >= INIPARSE_MAX_SECTIONS), NULL, "sections overlimit array fail");

    section = &inip->sections[inip->num_sections];
    inip->num_sections++;
    memset(section, 0x00, sizeof(INIPARSE_SECTION_T));

    while (1)
    {
        new = 0;
        switch (inip->state)
        {
            case START:
            case IN_SECTION:
                new = iniparse_readline(fp, inip);
                break;
            case NEW_SECTION:
                new = 1;
                break;
            case END_OF_FILE:
                return section;
            default:
                printf("invalid state %d in parse_section\n", inip->state);
                break;
        }

        if (new)
        {
            int i, x, y, b = 0, e = strlen(inip->tmp);
            INIPARSE_CONFIG_T *cfg;
            iniparse_trim(inip->tmp, &b, &e);

            /* check for comments */
            if (inip->tmp[b] == '#' || inip->tmp[b] == ';')
                continue;

            /* check for new section */
            if (inip->tmp[b] == '[')
            {
                if (inip->state == IN_SECTION)
                {
                    inip->state = NEW_SECTION;
                    break;
                }
                else
                {
                    /* fill section name */
                    inip->tmp[e] = '\0';
                    section->name = strdup(&inip->tmp[b+1]);
                    
                    inip->state = IN_SECTION;
                    continue;
                }
            }

            APP_RET_RETURN((section->num_configs >= INIPARSE_MAX_CONFIGS), section, "configs overlimit array fail");
            cfg = &section->configs[section->num_configs];
            section->num_configs++;
            cfg->key    = NULL;
            cfg->value  = NULL;

            /* read key */
            for (i = b; i <= e; i++)     
            {
                char c = inip->tmp[i];
                if(c == ':' || c == '=')
                    break;
            }
            x = b; y = i;
            at_iniparse_trim(inip->tmp, &x, &y);
            inip->tmp[y+1] = '\0';
            cfg->key = strdup(&inip->tmp[x]);

            /* read value */
            x = i+1;
            y = e+1;
            at_iniparse_trim(inip->tmp, &x, &y);
            inip->tmp[y+1] = '\0';
            cfg->value = strdup(&inip->tmp[x]);
        }
    }

    return section;
}


static int iniparse_parse(FILE *fp, INIPARSE_T *inip)
{
    char finished = 0;

    inip->num_sections = 0;

    while (!finished)
    {
        switch (inip->state)
        {
            case START:
            case NEW_SECTION:
            {
                iniparse_section(fp, inip);
                break;
            }
            case END_OF_FILE:
                finished = 1;
                break;
            default:
                printf("error parsing file at line %d\n", inip->current_line);
                finished = 1;
                break;
        }
    }
    return 0;
}


void* iniparse_load(const char *inifile)
{
    INIPARSE_T *inip = NULL;
    FILE *fp = NULL;

    APP_RET_RETURN((!inifile), NULL, "inifile is NULL");

    inip = malloc(sizeof(struct iniparse));
    APP_RET_RETURN((!inip), NULL, "malloc iniparse fail");
    memset(inip, 0x00, sizeof(struct iniparse));

    inip->filename  = (char*)inifile;
    inip->tmp       = malloc(sizeof(*inip->tmp)*INIPARSE_DEF_TMPLEN);
    inip->tmp_cap   = INIPARSE_DEF_TMPLEN;
    inip->current_line = 0;
    inip->state     = START;
    fp = fopen(inip->filename, "r");
    if (fp == NULL)
    {
        free(inip->tmp);
        free(inip);
        return NULL;
    }
    APP_RET_RETURN((fp == NULL), NULL, "fopen %s fail", inip->filename);

    if (iniparse_parse(fp, inip))
    {
        printf("iniparse %s fail\n", inip->filename);
        fclose(fp);
        free(inip->tmp);
        free(inip);
        return NULL;
    }

    fclose(fp);
    fp = NULL;
    return (void *)inip;
}