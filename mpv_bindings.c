#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <math.h>

#include "csv.h"
#include "vorze.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>


/*******************************************************************/
/* TODO: Add support for switching files without restarting player */
/*******************************************************************/

/*****************************/
/* TODO: Add record support? */
/*****************************/

/***************************/
/* TODO: Clean up makefile */
/***************************/


/***********************************/
/* Evil duplicated code from main.c! */
/***********************************/


void handleTs(int ts, CsvEntry *csv, int vorzeHandle) {
    static char currV1=-1, currV2=-1;
    CsvEntry *ent;
    ent=csvGetForTs(csv, ts);
    if (currV1!=ent->v1 || currV2!=ent->v2) {
	vorzeSet(vorzeHandle, ent->v1, ent->v2);
    }
    currV1=ent->v1; currV2=ent->v2;
}


volatile sig_atomic_t gotalarm=0, gotkill=0;

void handle_signal(int signum) {
    if (signum==SIGALRM) {
	gotalarm=1;
	signal(SIGALRM, handle_signal);
    } else if (signum==SIGINT||signum==SIGQUIT||signum==SIGTERM) {
	gotkill=1;
    }
}

CsvEntry *csv, *ent;
char serport[1024];
int sock;
int port=23867;
int vorze;
int controlvorze=1;
int offset=0;

void init() {

    strcpy(serport, "");


    if (!controlvorze) enableSimulation();

    if (strlen(serport)==0) vorzeDetectPort(serport);

    vorze=vorzeOpen(serport);
    if (vorze<=0) exit(1);

    signal(SIGINT, handle_signal);
    signal(SIGQUIT, handle_signal);
    signal(SIGTERM, handle_signal);

    //csvFree(csv);
}

void vorze_load_file(char* path) {
    csv=csvLoad(path);
    if (csv==NULL) exit(1);
    handleTs(0, csv, vorze);
}


/**********************/
/* LUA bindings begin */
/**********************/

static int mpv_init_vorze(lua_State *L) {
    printf("Initing vorze");
    init();
    return 0;
}

static int mpv_load_file(lua_State *L) {
    const char* path = luaL_checkstring(L,1);
    char csvfile[1024];
    printf("Attempting to load file %s" , csvfile);
    strcpy(csvfile,path);
    vorze_load_file(csvfile);
    return 0;
}


static int mpv_handleTS(lua_State *L) {
    double d = lua_tonumber(L,1);
    d*=10;
    printf("\rFrame: %f  ", (float)d);
    handleTs((int)d, csv, vorze);
    return 0;
}

static int mpv_stop_vorze_motion(lua_State *L) {
    vorzeSet(vorze, 0, 0);
    return 0;
}

static int mpv_close_vorze(lua_State *L) {
    vorzeClose(vorze);
    return 0;
}

int luaopen_mpv_bindings(lua_State *L) {
    lua_register(
	L,
	"mpv_close_vorze",
	mpv_close_vorze);
    lua_register(
	L,
	"mpv_load_file",
	mpv_load_file);
    lua_register(
	L,
	"mpv_stop_vorze_motion",
	mpv_stop_vorze_motion);
    lua_register(
	L,
	"mpv_init_vorze",
	mpv_init_vorze);
    lua_register(
	L,
	"update_vorze_to_TS",
	mpv_handleTS);
    return 0;
}
