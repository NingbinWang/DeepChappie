



/*
IMediaManager *media_manager_init_instance(VOID)
{   
    INT iRet = ERROR;
    MEDIUM_MANAGER_BASE_T *pBase = NULL;
    pBase = (MEDIUM_MANAGER_BASE_T *)sys_mem_malloc(sizeof(MEDIUM_MANAGER_BASE_T));
    if(!pBase)
    {
        LOGGER_ERROR("sys_mem_malloc failed\n");
        return NULL;
    }
    memset(pBase, 0, sizeof(MEDIUM_MANAGER_BASE_T));

    iRet = medium_manager_init_interface(&pBase->stInterface);
    if(iRet < 0)
    {   
        LOGGER_ERROR("init interface error \n");
        sys_mem_free(pBase);
        pBase = NULL;
        return NULL;
    }

    iRet = medium_manager_init_priv_data(&pBase->stPrivData);
    if(iRet < 0)
    {   
        LOGGER_ERROR("init priv data error \n");
        sys_mem_free(pBase);
        pBase = NULL;
        return NULL;
    }
    return &pBase->stInterface;
}

void init_medium_manager_component(void)
{
    mediummanager = medium_manager_init_instance();
    if(mediummanager == NULL)
    {
        LOGGER_ERROR("notification_init_instance error \n");
    }
}

IMediumManager *medium_manager_get_instance(void)
{
    return mediummanager;
}
*/