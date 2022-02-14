#include <pspkernel.h>
#include <pspdebug.h>
#include <pspsdk.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <psputility_netmodules.h>
#include <psputility_netparam.h>
#include <pspwlan.h>
#include <pspnet.h>
#include <pspnet_apctl.h>
#include <psphttp.h>
#include <pspnet_inet.h>
#include <pspnet_resolver.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <pspsdk.h>
#include <pspuser.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <psputility.h>
#include <psputility_netmodules.h>
#include <psputility_htmlviewer.h>
#include <pspnet.h>
#include <pspssl.h>
#include "sha2.h"
#include <psprtc.h>
#include "bitcoin_miner.h"
#include "bitcoin_miner_opt.h"
#define printf pspDebugScreenPrintf





PSP_MODULE_INFO("NEW", 0, 2, 0);

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common)
{
   sceKernelExitGame();
   return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp)
{
   int cbid;

   cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
   sceKernelRegisterExitCallback(cbid);
   sceKernelSleepThreadCB();

   return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void)
{
   int thid = 0;

   thid = sceKernelCreateThread("update_thread", CallbackThread,
                 0x11, 0xFA0, PSP_THREAD_ATTR_USER, 0);
   if(thid >= 0)
   {
      sceKernelStartThread(thid, 0, 0);
   }

   return thid;
}






int net_thread(SceSize args, void *argp)
{

  //miner();
  miner_opt();
  //client();
  sceKernelSleepThread();

  return 0;
}


/* Simple thread */
int main(int argc, char **argv)
{
   
   
   SceUID thid;

   SetupCallbacks();
   sceCtrlSetSamplingCycle(0);
   sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);
   pspDebugScreenInit();
   pspDebugScreenEnableBackColor(1);
   pspDebugScreenSetTextColor(0x00FFFF);

  thid = sceKernelCreateThread("net_thread", net_thread, 0x18, 0x10000, PSP_THREAD_ATTR_USER, NULL);
  if (thid < 0) {
    printf("Error! Thread could not be created!\n");
    sceKernelSleepThread();
  }

  sceKernelStartThread(thid, 0, NULL);

  sceKernelExitDeleteThread(0);

   return 0;
}