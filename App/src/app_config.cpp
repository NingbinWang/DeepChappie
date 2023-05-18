#include "app_config.h"
#include "iniparse.h"


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
            printf("rootdirnum get error\n");
            defaultconf->networkinfo.enable = 0;
        }else{
            defaultconf->networkinfo.enable = tmpvalue
        }
    }
    if(defaultconf->networkinfo.enable){
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
            printf("rootdirnum get error\n");
            defaultconf->storagerinfo.enable = 0;
        }else{
             defaultconf->storagerinfo.enable = tmpvalue;
        }
    }
    if( defaultconf->storagerinfo.enable ){
       value = iniparse_get_value(inih, "storager", "rootdirnum");
       if(value){
         i = atoi(value);
         if(i < 0){
             printf("rootdirnum get error\n");
             defaultconf->storagerinfo.dirnum=0;
         }else{
            defaultconf->storagerinfo.dirnum=i;
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
    }
    

    iniparse_free(inih);
    return ;
}

int app_defaultconfig(App_Defaultconf_t* defaultconf)
{
   app_conf_load(defaultconf);
   return 0;
}