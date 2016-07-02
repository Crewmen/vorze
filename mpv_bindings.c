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


/*****************************/
/* TODO: Add record support? */
/*****************************/

/***************************/
/* TODO: Clean up makefile */
/***************************/

/******************************************************************************************/
/* TODO: Add a flag in the lua to enable loading a csv of a different name than the video */
/******************************************************************************************/


/***********************************/
/* Evil duplicated code from main.c! */
/***********************************/

/*******************************************************************************************************************************/
/* This function should be moved into it's own file and included in both main.c and this, but I don't want to touch main.c atm */
/*******************************************************************************************************************************/
void handleTs(int ts, CsvEntry *csv, int vorzeHandle) {
    static char currV1=-1, currV2=-1;
    CsvEntry *ent;
    ent=csvGetForTs(csv, ts);
    if (currV1!=ent->v1 || currV2!=ent->v2) {
	vorzeSet(vorzeHandle, ent->v1, ent->v2);
    }
    currV1=ent->v1; currV2=ent->v2;
}



CsvEntry *csv = NULL, *ent = NULL;
char serport[1024];
int vorze;

void init() {

    strcpy(serport, "");

    if (strlen(serport)==0) vorzeDetectPort(serport);

    vorze=vorzeOpen(serport);
    if (vorze<=0) exit(1);
}

void vorze_load_file(char* path) {
    csvFree(csv); //Free the old csv, or free(null)
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
