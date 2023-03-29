#include <stdio.h>
#include <unistd.h>
#include "Framework.h"
int app_main()
{
  Framework_Init();
  while (1)
  {
    Framework_work();
    sleep(100);
  }
  
  return 0;
}

