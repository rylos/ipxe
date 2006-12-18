#include <console.h>
#include "etherboot.h"
#include "cmdline.h"
#include "cmdlinelib.h"
#include "cmdlist.h"
#include <gpxe/ansiesc.h>


#define CMDL_DELAY (2000 * TICKS_PER_SEC) / 1000;

void cmdl_exec_cmdline();
char cmdl_spin();

void cmdl_start()
{
	unsigned int stop;
	//int spin;

	//printf("gPXE %s (GPL) etherboot.org ...  ", VERSION);
	printf ( CSI "1m" /* bold */
		 "gPXE " VERSION " (GPL) etherboot.org\n"
		 CSI "0m" /* normal */
		 "Press Ctrl-B for gPXE command line..." );

	stop = currticks() + CMDL_DELAY;
	
	while(currticks() < stop){
	
		/*if(spin++ % 250 == 0){
			putchar(8);
			putchar(cmdl_spin());
		}*/
		
		if(iskey()){
			if(getchar() == 2){
				printf("\n\n");
				cmdl_exec_cmdline();
				break;
			}else{
				printf("skipping.\n");
				break;
			}
		}
	}
	putchar('\n');

	// empty the input buffer
	while(iskey()) {
		getchar();
	}
}

/*char cmdl_spin()
{
	static int state;*/
	//int spinner[4] = {'-', '\\', '|', '/'}; <- undefined reference to memcpy!
/*	int spinner[4];
	
	spinner[0] = '-';
	spinner[1] = '\\';
	spinner[2] = '|';
	spinner[3] = '/';

	return spinner[state++ % 4];
}*/

void cmdl_exec_cmdline(){
	cmd_line* cmd;
	cmd = cmdl_create();
	
	cmdl_setputchar(cmd, putchar);
	cmdl_setgetchar(cmd, getchar);
	cmdl_setprintf(cmd, printf);

	cmdl_setpropmt(cmd, "gPXE>");

	cmdl_enterloop(cmd);

	cmdl_free(cmd);
}

