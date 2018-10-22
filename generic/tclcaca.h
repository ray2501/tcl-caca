/*
 * For C++ compilers, use extern "C"
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <tcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <caca.h>

/*
 * Only the _Init function is exported.
 */

extern DLLEXPORT int    Caca_Init(Tcl_Interp * interp);


static int Caca_get_display_driver_list(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[]);

static int Caca_get_export_list(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[]);

static int Caca_get_import_list(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[]);

static int Caca_get_font_list(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[]);

static int Caca_rand(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[]);

static int Caca_create_canvas(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[]);

static int CANVAS_MAIN(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

static int Caca_create_display(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[]);

static int Caca_create_display_with_driver(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[]);

static int DISPLAY_MAIN(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

static int EVENT_MAIN(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

static int Caca_create_dither(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[]);

static int DITHER_MAIN(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);


/*
 * end block for C++
 */

#ifdef __cplusplus
}
#endif

