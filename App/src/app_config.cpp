#include "app_config.h"
#include "iniparse.h"

#define ENBALE 1
char *gConPath  = PATH_APPCONF;

void app_conf_load(App_Defaultconf_t* defaultconf)
{
   void *inih = NULL;
   char *value = NULL;
   int tmpvalue = 0;
   int i = 0;
   char key[64] = {0};
   //Read configuration file(s)
    inih = iniparse_load(gConPath);
    if (!inih)
    {
        printf("Cannot open %s: %s\n", gConPath, strerror(errno));
        return ;
    }
    //iniparse_dump(inih);
    //network config
    value = iniparse_get_value(inih, "network", "enable");
    if(value){
        tmpvalue = atoi(value);
        if(tmpvalue < 0){
            printf("network disable\n");
            defaultconf->networkinfo.enable = 0;
        }else{
            defaultconf->networkinfo.enable = tmpvalue;
        }
    }
    if(defaultconf->networkinfo.enable == ENBALE){
      value = iniparse_get_value(inih, "network", "devname");
      if(value){
         strncpy(defaultconf->networkinfo.DevName, value, strlen(value));
      }
      value = iniparse_get_value(inih, "network", "ip");
      if(value){
         strncpy(defaultconf->networkinfo.IP, value, strlen(value));
      }
      value = iniparse_get_value(inih, "network", "netmask");
      if(value){
         strncpy(defaultconf->networkinfo.NetMask, value, strlen(value));
      }
      value = iniparse_get_value(inih, "network", "gateway");
      if(value){
          strncpy(defaultconf->networkinfo.GateWay, value, strlen(value));
      }
    }
    //storager
    value = iniparse_get_value(inih, "storager", "enable");
    if(value){
        tmpvalue = atoi(value);
        if(tmpvalue < 0){
            printf("storager disable\n");
            defaultconf->storagerinfo.enable = 0;
        }else{
             defaultconf->storagerinfo.enable = tmpvalue;
        }
    }
    if( defaultconf->storagerinfo.enable == ENBALE){
       value = iniparse_get_value(inih, "storager", "rootdirnum");
       if(value){
         tmpvalue = atoi(value);
         if(tmpvalue < 0){
             printf("rootdirnum get error\n");
             defaultconf->storagerinfo.dirnum=0;
         }else{
            defaultconf->storagerinfo.dirnum=tmpvalue;
         }
       }
       for (i = 0; i < defaultconf->storagerinfo.dirnum; i++)
       {
            snprintf(key, sizeof(key), "dirname%d", i);
            value = iniparse_get_value(inih, "storager", key);
            if (value)
            {
                strncpy(defaultconf->storagerinfo.dirname[i], value, strlen(value));
            }
       }
       value = iniparse_get_value(inih, "storager", "tfid");
       if(value){
         tmpvalue = atoi(value);
         if(tmpvalue < 0){
             printf("tf get error\n");
             defaultconf->storagerinfo.tfid=-1;
         }else{
             defaultconf->storagerinfo.tfid=tmpvalue;
         }
       }
       value = iniparse_get_value(inih, "storager", "emmcid");
       if(value){
         tmpvalue = atoi(value);
         if(tmpvalue < 0){
             printf("emmcid get error\n");
             defaultconf->storagerinfo.emmcid=-1;
         }else{
             defaultconf->storagerinfo.emmcid=tmpvalue;
         }
       }
    }
    //media
    value = iniparse_get_value(inih, "media", "enable");
    if(value){
        tmpvalue = atoi(value);
        if(tmpvalue < 0){
            printf("media disable\n");
            defaultconf->mediainfo.enable = 0;
        }else{
             defaultconf->mediainfo.enable = tmpvalue;
        }
    }
    if(defaultconf->mediainfo.enable == ENBALE)
    {
       value = iniparse_get_value(inih, "media", "type");
      if(value){
         strncpy(defaultconf->mediainfo.type, value, strlen(value));
      }
    }
    //gsensor
    value = iniparse_get_value(inih, "gsensor", "enable");
    if(value){
        tmpvalue = atoi(value);
        if(tmpvalue < 0){
            printf("gsensor disable\n");
            defaultconf->gsensorinfo.enable = 0;
        }else{
             defaultconf->gsensorinfo.enable = tmpvalue;
        }
    }
    //notification
    value = iniparse_get_value(inih, "notification", "enable");
    if(value){
        tmpvalue = atoi(value);
        if(tmpvalue < 0){
            printf("notification disable\n");
            defaultconf->notificationinfo.enable = 0;
        }else{
            defaultconf->notificationinfo.enable = tmpvalue;
        }
    }
    if(defaultconf->notificationinfo.enable){
       value = iniparse_get_value(inih, "notification", "pingpongsize");
       if(value){
         tmpvalue = atoi(value);
         if(tmpvalue < 0){
            defaultconf->notificationinfo.pingpongsize = 0;
          }else{
            defaultconf->notificationinfo.pingpongsize = tmpvalue;
          }
        }
    }
//serial
   value = iniparse_get_value(inih, "serial0", "enable");
    if(value){
        tmpvalue = atoi(value);
        if(tmpvalue < 0){
            printf("notification disable\n");
            defaultconf->serialinfo0.enable = 0;
        }else{
            defaultconf->serialinfo0.enable = tmpvalue;
        }
    }
    iniparse_free(inih);
    return ;
}

int app_defaultconfig(App_Defaultconf_t* defaultconf)
{
   app_conf_load(defaultconf);
   return 0;
}