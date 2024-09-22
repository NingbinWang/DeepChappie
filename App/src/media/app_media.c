#include "app_media.h"
int app_mediaconfigset()
{
    #if 0
    int shmId = 0;
	/* 创建直接共享内存 */
	shmId = shmget((key_t)MEDIA_SHARE_SEGMENT_ID,sizeof(MEDIA_PARAM_T),0666);
	if(-1 == shmId)
	{
		printf("error shmKey = %d\n", shmId);
		return -1;
	}
    
	pMediaInitParam =(MEDIA_PARAM_T *)shmat(shmId, NULL, 0);
	if ((void *)(-1) == (void *)(pMediaInitParam))
	{
		printf("!!! shmat pMediaInitParam SIZE=0x%x failed to exit !!!\n", sizeof(MEDIA_PARAM_T));
		return -1;
	}
    #else
    //config++++



    //config----
    //pMediaInitParam->vicfg.enable = 1;
    #endif

    return 0;
}


int app_mediainit()
{
    Media_Ipcinit();
    Media_Init();
    return 0;
}

int app_mediastart()
{
    return 0;
}
int app_mediastop()
{
    return 0;
}
