#include "media_priv.h"

int halmedia_init(void)
{
    hdal_init(1);
    return 0;
}



int halmedia_exit(void)
{
    hdal_exit();
    return 0;
}