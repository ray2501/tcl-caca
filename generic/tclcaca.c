/*
 * Tcl bindings for Colour ASCII Art library (libcaca)
 *
 * Copyright (c) <2018>, <Danilo Chang>
 * All rights reserved.
 */


#include "tclcaca.h"

/*
 * Global variable
 */
Tcl_HashTable *caca_hashtblPtr;

int canvas_count = 0;
int display_count = 0;
int event_count = 0;
int dither_count = 0;

TCL_DECLARE_MUTEX(myMutex);


void Caca_Thread_Exit(ClientData clientdata)
{
  if(caca_hashtblPtr) {
    Tcl_DeleteHashTable(caca_hashtblPtr);
    ckfree(caca_hashtblPtr);
  }
}


static int Caca_get_display_driver_list(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[])
{
  char const * const * list = NULL;
  Tcl_Obj *pResultStr = NULL;
  int count = 0;

  if( objc != 1 ){
    Tcl_WrongNumArgs(interp, 1, objv, 0);
    return TCL_ERROR;
  }

  list = caca_get_display_driver_list();
  if(!list) return TCL_ERROR;

  pResultStr = Tcl_NewListObj(0, NULL);
  for(count = 0; list[count] != NULL; count++) {
      Tcl_ListObjAppendElement(interp, pResultStr, 
          Tcl_NewStringObj(list[count], strlen(list[count])));
  }

  Tcl_SetObjResult(interp, pResultStr);
  return TCL_OK;
}


static int Caca_get_export_list(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[])
{
  char const * const * list = NULL;
  Tcl_Obj *pResultStr = NULL;
  int count = 0;

  if( objc != 1 ){
    Tcl_WrongNumArgs(interp, 1, objv, 0);
    return TCL_ERROR;
  }

  list = caca_get_export_list();
  if(!list) return TCL_ERROR;

  pResultStr = Tcl_NewListObj(0, NULL);
  for(count = 0; list[count] != NULL; count++) {
      Tcl_ListObjAppendElement(interp, pResultStr, 
          Tcl_NewStringObj(list[count], strlen(list[count])));
  }

  Tcl_SetObjResult(interp, pResultStr);
  return TCL_OK;

}


static int Caca_get_import_list(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[])
{
  char const * const * list = NULL;
  Tcl_Obj *pResultStr = NULL;
  int count = 0;

  if( objc != 1 ){
    Tcl_WrongNumArgs(interp, 1, objv, 0);
    return TCL_ERROR;
  }

  list = caca_get_import_list();
  if(!list) return TCL_ERROR;

  pResultStr = Tcl_NewListObj(0, NULL);
  for(count = 0; list[count] != NULL; count++) {
      Tcl_ListObjAppendElement(interp, pResultStr, 
          Tcl_NewStringObj(list[count], strlen(list[count])));
  }

  Tcl_SetObjResult(interp, pResultStr);
  return TCL_OK;

}


static int Caca_get_font_list(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[])
{
  char const * const * list = NULL;
  Tcl_Obj *pResultStr = NULL;
  int count = 0;

  if( objc != 1 ){
    Tcl_WrongNumArgs(interp, 1, objv, 0);
    return TCL_ERROR;
  }

  list = caca_get_font_list();
  if(!list) return TCL_ERROR;

  pResultStr = Tcl_NewListObj(0, NULL);
  for(count = 0; list[count] != NULL; count++) {
      Tcl_ListObjAppendElement(interp, pResultStr, 
          Tcl_NewStringObj(list[count], strlen(list[count])));
  }

  Tcl_SetObjResult(interp, pResultStr);
  return TCL_OK;

}


static int Caca_rand(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[])
{
  int min = 0, max = 0, result = 0;

  if( objc != 3 ){
    Tcl_WrongNumArgs(interp, 1, objv, "min max");
    return TCL_ERROR;
  }

  if(Tcl_GetIntFromObj(interp, objv[1], &min) != TCL_OK) {
    return TCL_ERROR;
  }

  if(Tcl_GetIntFromObj(interp, objv[2], &max) != TCL_OK) {
    return TCL_ERROR;
  }

  result = caca_rand(min, max);
  Tcl_SetObjResult(interp, Tcl_NewIntObj( result ));
  return TCL_OK;

}


static int Caca_create_canvas(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[])
{
  int width = 0, height = 0;
  caca_canvas_t *canvas = NULL;
  Tcl_HashEntry *newHashEntryPtr;
  char handleName[16 + TCL_INTEGER_SPACE];
  Tcl_Obj *pResultStr = NULL;
  int newvalue;

  if( objc != 3 ){
    Tcl_WrongNumArgs(interp, 1, objv, "width height");
    return TCL_ERROR;
  }

  if(Tcl_GetIntFromObj(interp, objv[1], &width) != TCL_OK) {
    return TCL_ERROR;
  }

  if(Tcl_GetIntFromObj(interp, objv[2], &height) != TCL_OK) {
    return TCL_ERROR;
  }

  if(width < 0 || height < 0) return TCL_ERROR;

  canvas = caca_create_canvas(width, height);
  if(!canvas) return TCL_ERROR;

  Tcl_MutexLock(&myMutex);
  sprintf( handleName, "caca_canvas_%d", canvas_count++ );

  pResultStr = Tcl_NewStringObj( handleName, -1 );

  newHashEntryPtr = Tcl_CreateHashEntry(caca_hashtblPtr, handleName, &newvalue);
  Tcl_SetHashValue(newHashEntryPtr, canvas);
  Tcl_MutexUnlock(&myMutex);

  Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) CANVAS_MAIN,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_SetObjResult(interp, pResultStr);
  return TCL_OK;

}


static int CANVAS_MAIN(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
  int choice;
  caca_canvas_t *canvas = NULL;
  Tcl_HashEntry *hashEntryPtr;
  char *canHandle;

  static const char *CAN_strs[] = {
    "set_size",
    "get_width",
    "get_height",
    "gotoxy",
    "wherex",
    "wherey",
    "put_char",
    "get_char",
    "put_str",
    "get_attr",
    "set_attr",
    "unset_attr",
    "toggle_attr",
    "put_attr",
    "set_color_ansi",
    "set_color_argb",
    "clear",
    "set_handle",
    "get_handle_x",
    "get_handle_y",
    "blit",
    "set_boundaries",
    "invert",
    "flip",
    "flop",
    "rotate_180",
    "rotate_left",
    "rotate_right",
    "stretch_left",
    "stretch_right",
    "draw_line",
    "draw_thin_line",
    "draw_polyline",
    "draw_thin_polyline",
    "draw_circle",
    "draw_ellipse",
    "draw_thin_ellipse",
    "fill_ellipse",
    "draw_box",
    "draw_thin_box",
    "draw_cp437_box",
    "fill_box",
    "draw_triangle",
    "draw_thin_triangle",
    "fill_triangle",
    "get_frame_count",
    "set_frame",
    "get_frame_name",
    "set_frame_name",
    "create_frame",
    "free_frame",
    "import_from_memory",
    "import_from_file",
    "import_area_from_memory",
    "import_area_from_file",
    "export_to_memory",
    "export_area_to_memory",
    "close",
    0
  };

  enum CAN_enum {
    CAN_SET_SIZE,
    CAN_GET_WIDTH,
    CAN_GET_HEIGHT,
    CAN_GOTOXY,
    CAN_WHEREX,
    CAN_WHEREY,
    CAN_PUT_CHAR,
    CAN_GET_CHAR,
    CAN_PUT_STR,
    CAN_GET_ATTR,
    CAN_SET_ATTR,
    CAN_UNGET_ATTR,
    CAN_TOGGLE_ATTR,
    CAN_PUT_ATTR,
    CAN_SET_COLOR_ANSI,
    CAN_SET_COLOR_ARGB,
    CAN_CLEAR,
    CAN_SET_HANDLE,
    CAN_GET_HANDLE_X,
    CAN_GET_HANDLE_Y,
    CAN_BLIT,
    CAN_SET_BOUNDARIES,
    CAN_INVERT,
    CAN_FLIP,
    CAN_FLOP,
    CAN_ROTATE_180,
    CAN_ROTATE_LEFT,
    CAN_ROTATE_RIGHT,
    CAN_STRETCH_LEFT,
    CAN_STRETCH_RIGHT,
    CAN_DRAW_LINE,
    CAN_DRAW_THIN_LINE,
    CAN_DRAW_POLYLINE,
    CAN_DRAW_THIN_POLYLINE,
    CAN_DRAW_CIRCLE,
    CAN_DRAW_ELLIPSE,
    CAN_DRAW_THIN_ELLIPSE,
    CAN_FILL_ELLIPSE,
    CAN_DRAW_BOX,
    CAN_DRAW_THIN_BOX,
    CAN_DRAW_CP437_BOX,
    CAN_FILL_BOX,
    CAN_DRAW_TRIANGLE,
    CAN_DRAW_THIN_TRIANGLE,
    CAN_FILL_TRIANGLE,
    CAN_GET_FRAME_COUNT,
    CAN_SET_FRAME,
    CAN_GET_FRAME_NAME,
    CAN_SET_FRAME_NAME,
    CAN_CREATE_FRAME,
    CAN_FREE_FRAME,
    CAN_IMPORT_FROM_MEMORY,
    CAN_IMPORT_FROM_FILE,
    CAN_IMPORT_AREA_FROM_MEMORY,
    CAN_IMPORT_AREA_FROM_FILE,
    CAN_EXPORT_TO_MEMORY,
    CAN_EXPORT_AREA_TO_MEMORY,
    CAN_CLOSE,
  };

  if( objc < 2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
    return TCL_ERROR;
  }

  if( Tcl_GetIndexFromObj(interp, objv[1], CAN_strs, "option", 0, &choice) ){
    return TCL_ERROR;
  }

  canHandle = Tcl_GetStringFromObj(objv[0], 0);
  hashEntryPtr = Tcl_FindHashEntry( caca_hashtblPtr, canHandle );
  if( !hashEntryPtr ) {
    if( interp ) {
        Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
        Tcl_AppendStringsToObj( resultObj, "invalid canvas handle ", canHandle, (char *)NULL );
    }

    return TCL_ERROR;
  }

  canvas = Tcl_GetHashValue( hashEntryPtr );

  switch( (enum CAN_enum)choice ){

    case CAN_SET_SIZE: {
      int width = 0, height = 0;
      int result = 0;

      if( objc != 4 ){
        Tcl_WrongNumArgs(interp, 2, objv, "width height");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &width) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &height) != TCL_OK) {
        return TCL_ERROR;
      }

      if(width < 0 || height < 0) return TCL_ERROR;
      result = caca_set_canvas_size(canvas, width , height);
      if(result < 0) return TCL_ERROR; 

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_GET_WIDTH: {
      int width = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      width = caca_get_canvas_width(canvas);
      Tcl_SetObjResult(interp, Tcl_NewIntObj( width ));
      break;
    }

    case CAN_GET_HEIGHT: {
      int height = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      height = caca_get_canvas_height(canvas);
      Tcl_SetObjResult(interp, Tcl_NewIntObj( height ));
      break;
    }

    case CAN_GOTOXY: {
      int x = 0, y = 0;

      if( objc != 4 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      caca_gotoxy(canvas, x, y);
      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_WHEREX: {
      int x = 0;
      
      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }
      
      x = caca_wherex(canvas);
      Tcl_SetObjResult(interp, Tcl_NewIntObj( x ));
      break;			     
    }

    case CAN_WHEREY: {
      int y = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      y = caca_wherey(canvas);
      Tcl_SetObjResult(interp, Tcl_NewIntObj( y ));
      break;
    }

    case CAN_PUT_CHAR: {
      int result = 0;
      int x = 0, y = 0;
      uint32_t ch = 0;

      if( objc != 5 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y char");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], (int *) &ch) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_put_char(canvas, x, y, ch);
      Tcl_SetObjResult(interp, Tcl_NewIntObj( result ));
      break;
    }

    case CAN_GET_CHAR: {
      int x = 0, y = 0;
      uint32_t ch = 0;

      if( objc != 4 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      ch = caca_get_char(canvas, x, y);
      Tcl_SetObjResult(interp, Tcl_NewIntObj( (int) ch ));
      break;
    }

    case CAN_PUT_STR: {
      int result = 0;
      int x = 0, y = 0;
      const char *str = NULL;
      int length = 0;

      if( objc != 5 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y string");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      str = Tcl_GetStringFromObj(objv[4], &length);
      if( !str || length < 1) {
        return TCL_ERROR;
      }

      result = caca_put_str(canvas, x, y, str);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_GET_ATTR: {
      uint32_t attr = 0;
      int x = 0, y = 0;

      if( objc != 4 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      attr = caca_get_attr(canvas, x, y);
      Tcl_SetObjResult(interp, Tcl_NewIntObj( attr ));
      break;
    }

    case CAN_SET_ATTR: {
      uint32_t attr = 0;
      int result = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "attr");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], (int *) &attr) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_set_attr(canvas, attr);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_UNGET_ATTR: {
      uint32_t attr = 0;
      int result = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "attr");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], (int *) &attr) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_unset_attr(canvas, attr);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_TOGGLE_ATTR: {
      uint32_t attr = 0;
      int result = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "attr");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], (int *) &attr) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_toggle_attr(canvas, attr);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_PUT_ATTR: {
      uint32_t attr = 0;
      int x = 0, y = 0;
      int result = 0;

      if( objc != 5 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y attr");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], (int *) &attr) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_put_attr(canvas, x, y, attr);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_SET_COLOR_ANSI: {
      int result = 0;
      const char *fg = NULL, *bg = NULL;
      int length = 0;
      uint8_t fg_value = CACA_DEFAULT, bg_value = CACA_DEFAULT;

      if( objc != 4 ){
        Tcl_WrongNumArgs(interp, 2, objv, "fg bg");
        return TCL_ERROR;
      }

      fg = Tcl_GetStringFromObj(objv[2], &length);
      if( !fg || length < 1) {
        return TCL_ERROR;
      }

      bg = Tcl_GetStringFromObj(objv[3], &length);
      if( !bg || length < 1) {
        return TCL_ERROR;
      }

      if(strcmp(fg, "black")==0) {
         fg_value = CACA_BLACK;
      } else if(strcmp(fg, "blue")==0) {
         fg_value = CACA_BLUE;
      } else if(strcmp(fg, "green")==0) {
         fg_value = CACA_GREEN;
      } else if(strcmp(fg, "cyan")==0) {
         fg_value = CACA_CYAN;
      } else if(strcmp(fg, "red")==0) {
         fg_value = CACA_RED;
      } else if(strcmp(fg, "magenta")==0) {
         fg_value = CACA_MAGENTA;
      } else if(strcmp(fg, "brown")==0) {
         fg_value = CACA_BROWN;
      } else if(strcmp(fg, "lightgray")==0) {
         fg_value = CACA_LIGHTGRAY;
      } else if(strcmp(fg, "drakgray")==0) {
         fg_value = CACA_DARKGRAY;
      } else if(strcmp(fg, "lightblue")==0) {
         fg_value = CACA_LIGHTBLUE;
      } else if(strcmp(fg, "lightgreen")==0) {
         fg_value = CACA_LIGHTGREEN;
      } else if(strcmp(fg, "lightcyan")==0) {
         fg_value = CACA_LIGHTCYAN;
      } else if(strcmp(fg, "lightred")==0) {
         fg_value = CACA_LIGHTRED;
      } else if(strcmp(fg, "lightmagenta")==0) {
         fg_value = CACA_LIGHTMAGENTA;
      } else if(strcmp(fg, "yellow")==0) {
         fg_value = CACA_YELLOW;
      } else if(strcmp(fg, "white")==0) {
         fg_value = CACA_WHITE;
      } else if(strcmp(fg, "default")==0) {
         fg_value = CACA_DEFAULT;
      } else if(strcmp(fg, "transparent")==0) {
         fg_value = CACA_TRANSPARENT;
      } else {
         return TCL_ERROR;
      }

     if(strcmp(bg, "black")==0) {
         bg_value = CACA_BLACK;
      } else if(strcmp(bg, "blue")==0) {
         bg_value = CACA_BLUE;
      } else if(strcmp(bg, "green")==0) {
         bg_value = CACA_GREEN;
      } else if(strcmp(bg, "cyan")==0) {
         bg_value = CACA_CYAN;
      } else if(strcmp(bg, "red")==0) {
         bg_value = CACA_RED;
      } else if(strcmp(bg, "magenta")==0) {
         bg_value = CACA_MAGENTA;
      } else if(strcmp(bg, "brown")==0) {
         bg_value = CACA_BROWN;
      } else if(strcmp(bg, "lightgray")==0) {
         bg_value = CACA_LIGHTGRAY;
      } else if(strcmp(bg, "drakgray")==0) {
         bg_value = CACA_DARKGRAY;
      } else if(strcmp(bg, "lightblue")==0) {
         bg_value = CACA_LIGHTBLUE;
      } else if(strcmp(bg, "lightgreen")==0) {
         bg_value = CACA_LIGHTGREEN;
      } else if(strcmp(bg, "lightcyan")==0) {
         bg_value = CACA_LIGHTCYAN;
      } else if(strcmp(bg, "lightred")==0) {
         bg_value = CACA_LIGHTRED;
      } else if(strcmp(bg, "lightmagenta")==0) {
         bg_value = CACA_LIGHTMAGENTA;
      } else if(strcmp(bg, "yellow")==0) {
         bg_value = CACA_YELLOW;
      } else if(strcmp(bg, "white")==0) {
         bg_value = CACA_WHITE;
      } else if(strcmp(bg, "default")==0) {
         bg_value = CACA_DEFAULT;
      } else if(strcmp(bg, "transparent")==0) {
         bg_value = CACA_TRANSPARENT;
      } else {
         return TCL_ERROR;
      }

      result = caca_set_color_ansi(canvas, fg_value, bg_value);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_SET_COLOR_ARGB: {
      int result = 0;
      uint16_t fg_value = 0x0fff, bg_value = 0x0000;

      if( objc != 4 ){
        Tcl_WrongNumArgs(interp, 2, objv, "fg bg");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], (int *) &fg_value) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], (int *) &bg_value) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_set_color_argb(canvas, fg_value, bg_value);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_CLEAR: {
      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      caca_clear_canvas (canvas);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_SET_HANDLE: {
      int result = 0;
      int x = 0, y = 0;

      if( objc != 4 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_set_canvas_handle(canvas, x, y);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_GET_HANDLE_X: {
      int val = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      val = caca_get_canvas_handle_x(canvas);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( val ));
      break;
    }

    case CAN_GET_HANDLE_Y: {
      int val = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      val = caca_get_canvas_handle_y(canvas);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( val ));
      break;
    }

    case CAN_BLIT: {
      int result = 0;
      int x = 0, y = 0;
      const char *srcHandle = NULL, *maskHandle = NULL;
      caca_canvas_t const *src = NULL, *mask = NULL;
      int length = 0;
      Tcl_HashEntry *hashEntryPtr1, *hashEntryPtr2;

      if( objc != 5 && objc != 6 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y SRC_HANDLE ?MASK_HANDLE?");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      srcHandle = Tcl_GetStringFromObj(objv[4], &length);
      if( !srcHandle || length < 1) {
        return TCL_ERROR;
      }

      hashEntryPtr1 = Tcl_FindHashEntry( caca_hashtblPtr, srcHandle );
      if( !hashEntryPtr1 ) {
        if( interp ) {
          Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
          Tcl_AppendStringsToObj( resultObj, "invalid canvas handle ", srcHandle, (char *)NULL );
        }

        return TCL_ERROR;
      }

      src = Tcl_GetHashValue( hashEntryPtr1 );

      if(objc == 6) { 
          maskHandle = Tcl_GetStringFromObj(objv[5], &length);
          if( !maskHandle || length < 1) {
            return TCL_ERROR;
          }

          hashEntryPtr2 = Tcl_FindHashEntry( caca_hashtblPtr, maskHandle );
          if( !hashEntryPtr2 ) {
            if( interp ) {
              Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
              Tcl_AppendStringsToObj( resultObj, "invalid canvas handle ", maskHandle, (char *)NULL );
            }

            return TCL_ERROR;
          }

          mask = Tcl_GetHashValue( hashEntryPtr2 );
      }

      result = caca_blit(canvas, x, y, src, mask);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_SET_BOUNDARIES: {
      int result = 0;
      int x = 0, y = 0, w = 0, h = 0;

      if( objc != 6 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y w h");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &w) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[5], &h) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_set_canvas_boundaries(canvas, x, y, w, h);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    /* canvas transformation */

    case CAN_INVERT: {
      int result = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      result = caca_invert(canvas);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_FLIP: {
      int result = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      result = caca_flip(canvas);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_FLOP: {
      int result = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      result = caca_flop(canvas);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_ROTATE_180: {
      int result = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      result = caca_rotate_180(canvas);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_ROTATE_LEFT: {
      int result = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      result = caca_rotate_left(canvas);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_ROTATE_RIGHT: {
      int result = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      result = caca_rotate_right(canvas);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_STRETCH_LEFT: {
      int result = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      result = caca_stretch_left(canvas);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_STRETCH_RIGHT: {
      int result = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      result = caca_stretch_right(canvas);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    /* primitives drawing */

    case CAN_DRAW_LINE: {
      int result = 0;
      int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
      uint32_t ch = 0;

      if( objc != 7 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x1 y1 x2 y2 char");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x1) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y1) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &x2) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[5], &y2) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[6], (int *) &ch) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_draw_line(canvas, x1, y1, x2, y2, ch);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_DRAW_THIN_LINE: {
      int result = 0;
      int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

      if( objc != 6 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x1 y1 x2 y2");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x1) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y1) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &x2) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[5], &y2) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_draw_thin_line(canvas, x1, y1, x2, y2);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_DRAW_POLYLINE: {
      int result = 0;
      int *x_array = NULL, *y_array = NULL;
      uint32_t ch = 0;
      int lengthx = 0, lengthy = 0;
      int count = 0;
      Tcl_Obj *elemListPtr = NULL;

      if( objc != 5 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x_list y_list char");
        return TCL_ERROR;
      }

      if(Tcl_ListObjLength(interp, objv[2], &lengthx) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_ListObjLength(interp, objv[3], &lengthy) != TCL_OK) {
        return TCL_ERROR;
      }

      if(lengthx ==0 || lengthy == 0) {
        return TCL_ERROR;
      }

      if (lengthx != lengthy) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], (int *) &ch) != TCL_OK) {
        return TCL_ERROR;
      }

      x_array = (int *) malloc (sizeof(int) * lengthx);
      y_array = (int *) malloc (sizeof(int) * lengthy);

      for(count = 0; count < lengthx; count++) {
          int x = 0;
          Tcl_ListObjIndex(interp, objv[2], count, &elemListPtr);
          if(Tcl_GetIntFromObj(interp, elemListPtr, &x) != TCL_OK) {
              if(x_array) free(x_array);
              if(y_array) free(y_array);
              return TCL_ERROR;
          }

          x_array[count] = x;
      }

      for(count = 0; count < lengthy; count++) {
          int y = 0;
          Tcl_ListObjIndex(interp, objv[3], count, &elemListPtr);
          if(Tcl_GetIntFromObj(interp, elemListPtr, &y) != TCL_OK) {
              if(x_array) free(x_array);
              if(y_array) free(y_array);
              return TCL_ERROR;
          }

          y_array[count] = y;
      }

      result = caca_draw_polyline(canvas, x_array, y_array, lengthx - 1, ch);
      if(result < 0)  {
          if(x_array) free(x_array);
          if(y_array) free(y_array);
          return TCL_ERROR;
      }

      if(x_array) free(x_array);
      if(y_array) free(y_array);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_DRAW_THIN_POLYLINE: {
      int result = 0;
      int *x_array = NULL, *y_array = NULL;
      int lengthx = 0, lengthy = 0;
      int count = 0;
      Tcl_Obj *elemListPtr = NULL;

      if( objc != 4 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x_list y_list");
        return TCL_ERROR;
      }

      if(Tcl_ListObjLength(interp, objv[2], &lengthx) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_ListObjLength(interp, objv[3], &lengthy) != TCL_OK) {
        return TCL_ERROR;
      }

      if(lengthx ==0 || lengthy == 0) {
        return TCL_ERROR;
      }

      if (lengthx != lengthy) {
        return TCL_ERROR;
      }

      x_array = (int *) malloc (sizeof(int) * lengthx);
      y_array = (int *) malloc (sizeof(int) * lengthy);

      for(count = 0; count < lengthx; count++) {
          int x = 0;
          Tcl_ListObjIndex(interp, objv[2], count, &elemListPtr);
          if(Tcl_GetIntFromObj(interp, elemListPtr, &x) != TCL_OK) {
              if(x_array) free(x_array);
              if(y_array) free(y_array);
              return TCL_ERROR;
          }

          x_array[count] = x;
      }

      for(count = 0; count < lengthy; count++) {
          int y = 0;
          Tcl_ListObjIndex(interp, objv[3], count, &elemListPtr);
          if(Tcl_GetIntFromObj(interp, elemListPtr, &y) != TCL_OK) {
              if(x_array) free(x_array);
              if(y_array) free(y_array);
              return TCL_ERROR;
          }

          y_array[count] = y;
      }

      result = caca_draw_thin_polyline(canvas, x_array, y_array, lengthx - 1);
      if(result < 0)  {
          if(x_array) free(x_array);
          if(y_array) free(y_array);
          return TCL_ERROR;
      }

      if(x_array) free(x_array);
      if(y_array) free(y_array);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_DRAW_CIRCLE: {
      int result = 0;
      int x = 0, y = 0, r = 0;
      uint32_t ch = 0;

      if( objc != 6 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y r char");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &r) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[5], (int *) &ch) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_draw_circle(canvas, x, y, r, ch);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_DRAW_ELLIPSE: {
      int result = 0;
      int x = 0, y = 0, a = 0, b = 0;
      uint32_t ch = 0;

      if( objc != 7 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y a b char");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &a) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[5], &b) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[6], (int *) &ch) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_draw_ellipse(canvas, x, y, a, b, ch);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_DRAW_THIN_ELLIPSE: {
      int result = 0;
      int x = 0, y = 0, a = 0, b = 0;

      if( objc != 6 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y a b");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &a) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[5], &b) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_draw_thin_ellipse(canvas, x, y, a, b);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_FILL_ELLIPSE: {
      int result = 0;
      int x = 0, y = 0, a = 0, b = 0;
      uint32_t ch = 0;

      if( objc != 7 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y a b char");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &a) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[5], &b) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[6], (int *) &ch) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_fill_ellipse(canvas, x, y, a, b, ch);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_DRAW_BOX: {
      int result = 0;
      int x = 0, y = 0, w = 0, h = 0;
      uint32_t ch = 0;

      if( objc != 7 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y w h char");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &w) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[5], &h) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[6], (int *) &ch) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_draw_box(canvas, x, y, w, h, ch);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_DRAW_THIN_BOX: {
      int result = 0;
      int x = 0, y = 0, w = 0, h = 0;

      if( objc != 6 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y w h");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &w) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[5], &h) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_draw_thin_box(canvas, x, y, w, h);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_DRAW_CP437_BOX: {
      int result = 0;
      int x = 0, y = 0, w = 0, h = 0;

      if( objc != 6 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y w h");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &w) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[5], &h) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_draw_cp437_box(canvas, x, y, w, h);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_FILL_BOX: {
      int result = 0;
      int x = 0, y = 0, w = 0, h = 0;
      uint32_t ch = 0;

      if( objc != 7 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y w h char");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &w) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[5], &h) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[6], (int *) &ch) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_fill_box(canvas, x, y, w, h, ch);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_DRAW_TRIANGLE: {
      int result = 0;
      int x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0;
      uint32_t ch = 0;

      if( objc != 9 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x1 y1 x2 y2 x3 y3 char");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x1) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y1) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &x2) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[5], &y2) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[6], &x3) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[7], &y3) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[8], (int *) &ch) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_draw_triangle(canvas, x1, y1, x2, y2, x3, y3, ch);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_DRAW_THIN_TRIANGLE: {
      int result = 0;
      int x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0;

      if( objc != 8 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x1 y1 x2 y2 x3 y3");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x1) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y1) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &x2) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[5], &y2) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[6], &x3) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[7], &y3) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_draw_thin_triangle(canvas, x1, y1, x2, y2, x3, y3);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_FILL_TRIANGLE: {
      int result = 0;
      int x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0;
      uint32_t ch = 0;

      if( objc != 9 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x1 y1 x2 y2 x3 y3 char");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x1) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y1) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &x2) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[5], &y2) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[6], &x3) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[7], &y3) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[8], (int *) &ch) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_fill_triangle(canvas, x1, y1, x2, y2, x3, y3, ch);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    /* canvas frame handling */

    case CAN_GET_FRAME_COUNT: {
      int count = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      count  = caca_get_frame_count(canvas);
      Tcl_SetObjResult(interp, Tcl_NewIntObj( count ));
      break;
    }

    case CAN_SET_FRAME: {
      int index = 0;
      int result = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "index");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &index) != TCL_OK) {
        return TCL_ERROR;
      }

      result  = caca_set_frame(canvas, index);
      if (result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_GET_FRAME_NAME: {
      const char *name = NULL;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      name  = caca_get_frame_name(canvas);
      Tcl_SetObjResult(interp, Tcl_NewStringObj(name, -1));
      break;
    }

    case CAN_SET_FRAME_NAME: {
      char *name = NULL;
      int length;
      int result = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "name");
        return TCL_ERROR;
      }

      name = Tcl_GetStringFromObj(objv[2], &length);
      if(name == NULL || length < 1) {
          return TCL_ERROR;
      }

      result  = caca_set_frame_name(canvas, name);
      if (result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_CREATE_FRAME: {
      int index = 0;
      int result = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "index");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &index) != TCL_OK) {
        return TCL_ERROR;
      }

      result  = caca_create_frame(canvas, index);
      if (result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_FREE_FRAME: {
      int index = 0;
      int result = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "index");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &index) != TCL_OK) {
        return TCL_ERROR;
      }

      result  = caca_free_frame(canvas, index);
      if (result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_IMPORT_FROM_MEMORY: {
      char *data = NULL, *fmt = NULL;
      int length = 0;
      int result = 0;

      if( objc != 4 ){
        Tcl_WrongNumArgs(interp, 2, objv, "data fmt");
        return TCL_ERROR;
      }

      data = Tcl_GetStringFromObj(objv[2], &length);
      if(data == NULL || length < 1) {
          return TCL_ERROR;
      }

      fmt = Tcl_GetStringFromObj(objv[3], &length);
      if(fmt == NULL) {
          return TCL_ERROR;
      }

      result = caca_import_canvas_from_memory(canvas, data, strlen(data), fmt);
      if (result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_IMPORT_FROM_FILE: {
      char *file = NULL, *fmt = NULL;
      int length = 0;
      int result = 0;

      if( objc != 4 ){
        Tcl_WrongNumArgs(interp, 2, objv, "file fmt");
        return TCL_ERROR;
      }

      file = Tcl_GetStringFromObj(objv[2], &length);
      if(file == NULL || length < 1) {
          return TCL_ERROR;
      }

      fmt = Tcl_GetStringFromObj(objv[3], &length);
      if(fmt == NULL) {
          return TCL_ERROR;
      }

      result = caca_import_canvas_from_file(canvas, file, fmt);
      if (result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_IMPORT_AREA_FROM_MEMORY: {
      char *data = NULL, *fmt = NULL;
      int x = 0, y = 0;
      int length = 0;
      int result = 0;

      if( objc != 6 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y data fmt");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      data = Tcl_GetStringFromObj(objv[4], &length);
      if(data == NULL || length < 1) {
          return TCL_ERROR;
      }

      fmt = Tcl_GetStringFromObj(objv[5], &length);
      if(fmt == NULL) {
          return TCL_ERROR;
      }

      result = caca_import_area_from_memory(canvas, x, y, data, strlen(data), fmt);
      if (result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_IMPORT_AREA_FROM_FILE: {
      char *file = NULL, *fmt = NULL;
      int x = 0, y = 0;
      int length = 0;
      int result = 0;

      if( objc != 6 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y file fmt");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      file = Tcl_GetStringFromObj(objv[4], &length);
      if(file == NULL || length < 1) {
          return TCL_ERROR;
      }

      fmt = Tcl_GetStringFromObj(objv[5], &length);
      if(fmt == NULL) {
          return TCL_ERROR;
      }

      result = caca_import_area_from_file(canvas, x, y, file, fmt);
      if (result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case CAN_EXPORT_TO_MEMORY: {
      char *fmt = NULL;
      int length = 0;
      int buffer_length = 0;
      void *buffer;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "fmt");
        return TCL_ERROR;
      }

      fmt = Tcl_GetStringFromObj(objv[2], &length);
      if(fmt == NULL) {
          return TCL_ERROR;
      }

      buffer = caca_export_canvas_to_memory(canvas, fmt, (size_t *) &buffer_length);
      if (buffer == NULL) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewStringObj( buffer, buffer_length ));
      break;
    }

    case CAN_EXPORT_AREA_TO_MEMORY: {
      int x = 0, y = 0, w = 0, h = 0;
      char *fmt = NULL;
      int length = 0;
      int buffer_length = 0;
      void *buffer;

      if( objc != 7 ){
        Tcl_WrongNumArgs(interp, 2, objv, "x y w h fmt");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &w) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[5], &h) != TCL_OK) {
        return TCL_ERROR;
      }

      fmt = Tcl_GetStringFromObj(objv[6], &length);
      if(fmt == NULL) {
          return TCL_ERROR;
      }

      buffer = caca_export_area_to_memory(canvas, x, y, w, h, fmt, (size_t *) &buffer_length);
      if (buffer == NULL) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewStringObj( buffer, buffer_length ));
      break;
    }

    case CAN_CLOSE: {
      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      caca_free_canvas (canvas);

      Tcl_MutexLock(&myMutex);
      if( hashEntryPtr )  Tcl_DeleteHashEntry(hashEntryPtr);
      Tcl_MutexUnlock(&myMutex);
      Tcl_DeleteCommand(interp, canHandle);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }
  }

  return TCL_OK;
}


static int Caca_create_display(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[])
{
  caca_canvas_t *canvas = NULL;
  caca_display_t *display = NULL;
  Tcl_HashEntry *hashEntryPtr;
  Tcl_HashEntry *newHashEntryPtr;
  char handleName[16 + TCL_INTEGER_SPACE];
  char *canHandle = NULL;
  int length = 0;
  Tcl_Obj *pResultStr = NULL;
  int newvalue;

  if( objc != 1 && objc != 2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "?canvas_handle?");
    return TCL_ERROR;
  }

  if (objc == 2) {
    canHandle = Tcl_GetStringFromObj(objv[1], &length);
    if(canHandle == NULL || length < 1) {
      return TCL_ERROR;
    }

    hashEntryPtr = Tcl_FindHashEntry( caca_hashtblPtr, canHandle );
    if( !hashEntryPtr ) {
      if( interp ) {
          Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
          Tcl_AppendStringsToObj( resultObj, "invalid canvas handle ", canHandle, (char *)NULL );
      }

      return TCL_ERROR;
    }

    canvas = Tcl_GetHashValue( hashEntryPtr );
    if(!canvas) return TCL_ERROR;
  } else {
    canvas = NULL;
  }

  display = caca_create_display(canvas);
  if (display == NULL) return TCL_ERROR;

  Tcl_MutexLock(&myMutex);
  sprintf( handleName, "caca_display_%d", display_count++ );

  pResultStr = Tcl_NewStringObj( handleName, -1 );

  newHashEntryPtr = Tcl_CreateHashEntry(caca_hashtblPtr, handleName, &newvalue);
  Tcl_SetHashValue(newHashEntryPtr, display);
  Tcl_MutexUnlock(&myMutex);

  Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) DISPLAY_MAIN,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_SetObjResult(interp, pResultStr);
  return TCL_OK;

}


static int Caca_create_display_with_driver(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[])
{
  caca_canvas_t *canvas = NULL;
  caca_display_t *display = NULL;
  Tcl_HashEntry *hashEntryPtr;
  Tcl_HashEntry *newHashEntryPtr;
  char handleName[16 + TCL_INTEGER_SPACE];
  char *canHandle = NULL;
  char *driver_name = NULL;
  int length = 0;
  Tcl_Obj *pResultStr = NULL;
  int newvalue;

  if( objc != 3 ){
    Tcl_WrongNumArgs(interp, 1, objv, "canvas_handle driver_name");
    return TCL_ERROR;
  }

  canHandle = Tcl_GetStringFromObj(objv[1], &length);
  if(canHandle == NULL || length < 1) {
    return TCL_ERROR;
  }

  hashEntryPtr = Tcl_FindHashEntry( caca_hashtblPtr, canHandle );
  if( !hashEntryPtr ) {
    if( interp ) {
        Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
        Tcl_AppendStringsToObj( resultObj, "invalid canvas handle ", canHandle, (char *)NULL );
    }

    return TCL_ERROR;
  }

  canvas = Tcl_GetHashValue( hashEntryPtr );
  if(!canvas) return TCL_ERROR;

  driver_name = Tcl_GetStringFromObj(objv[2], &length);
  if(driver_name == NULL || length < 1) {
    return TCL_ERROR;
  }

  display = caca_create_display_with_driver(canvas, driver_name);
  if (display == NULL) return TCL_ERROR;

  Tcl_MutexLock(&myMutex);
  sprintf( handleName, "caca_display_%d", display_count++ );

  pResultStr = Tcl_NewStringObj( handleName, -1 );

  newHashEntryPtr = Tcl_CreateHashEntry(caca_hashtblPtr, handleName, &newvalue);
  Tcl_SetHashValue(newHashEntryPtr, display);
  Tcl_MutexUnlock(&myMutex);

  Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) DISPLAY_MAIN,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_SetObjResult(interp, pResultStr);
  return TCL_OK;

}


static int DISPLAY_MAIN(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
  int choice;
  caca_display_t *display = NULL;
  Tcl_HashEntry *hashEntryPtr;
  char *disHandle;

  static const char *DIS_strs[] = {
    "get_driver",
    "set_driver",
    "get_canvas_handle",
    "refresh",
    "get_time",
    "set_time",
    "get_width",
    "get_height",
    "set_mouse",
    "set_cursor",
    "set_title",
    "create_event_handle",
    "get_event",
    "close",
    0
  };

  enum DIS_enum {
    DIS_GET_DRIVER,
    DIS_SET_DRIVER,
    DIS_GET_CANVAS,
    DIS_REFRESH,
    DIS_GET_TIME,
    DIS_SET_TIME,
    DIS_GET_WIDTH,
    DIS_GET_HEIGHT,
    DIS_SET_MOUSE,
    DIS_SET_CURSOR,
    DIS_SET_TITLE,
    DIS_CREATE_EVENT,
    DIS_GET_EVENT,
    DIS_CLOSE,
  };

  if( objc < 2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
    return TCL_ERROR;
  }

  if( Tcl_GetIndexFromObj(interp, objv[1], DIS_strs, "option", 0, &choice) ){
    return TCL_ERROR;
  }

  disHandle = Tcl_GetStringFromObj(objv[0], 0);
  hashEntryPtr = Tcl_FindHashEntry( caca_hashtblPtr, disHandle );
  if( !hashEntryPtr ) {
    if( interp ) {
        Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
        Tcl_AppendStringsToObj( resultObj, "invalid display handle ", disHandle, (char *)NULL );
    }

    return TCL_ERROR;
  }

  display = Tcl_GetHashValue( hashEntryPtr );

  switch( (enum DIS_enum)choice ){

    case DIS_GET_DRIVER:  {
      char const *name;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      name = caca_get_display_driver (display);

      Tcl_SetObjResult(interp, Tcl_NewStringObj(name, -1));
      break;
    }

    case DIS_SET_DRIVER:  {
      char *name = NULL;
      int length = 0;
      int result = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "driver_name");
        return TCL_ERROR;
      }

      name = Tcl_GetStringFromObj(objv[2], &length);
      if( !name || length < 1) {
        return TCL_ERROR;
      }

      result = caca_set_display_driver (display, name);
      if (result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case DIS_GET_CANVAS: {
      caca_canvas_t *canvas = NULL;
      Tcl_HashEntry *newHashEntryPtr;
      char handleName[16 + TCL_INTEGER_SPACE];
      Tcl_Obj *pResultStr = NULL;
      int newvalue;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      canvas = caca_get_canvas(display);
      if(!canvas) return TCL_ERROR;

      Tcl_MutexLock(&myMutex);
      sprintf( handleName, "caca_canvas_%d", canvas_count++ );

      pResultStr = Tcl_NewStringObj( handleName, -1 );

      newHashEntryPtr = Tcl_CreateHashEntry(caca_hashtblPtr, handleName, &newvalue);
      Tcl_SetHashValue(newHashEntryPtr, canvas);
      Tcl_MutexUnlock(&myMutex);

      Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) CANVAS_MAIN,
            (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

      Tcl_SetObjResult(interp, pResultStr);
      break;
    }

    case DIS_REFRESH: {
      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      caca_refresh_display (display);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case DIS_GET_TIME: {
      int val = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      val = caca_get_display_time(display);
      Tcl_SetObjResult(interp, Tcl_NewIntObj( val ));
      break;
    }

    case DIS_SET_TIME: {
      int val = 0;
      int result = 0; 

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "usec");
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[2], &val) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_set_display_time(display, val);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case DIS_GET_WIDTH: {
      int width = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      width = caca_get_display_width(display);
      Tcl_SetObjResult(interp, Tcl_NewIntObj( width ));
      break;
    }

    case DIS_GET_HEIGHT: {
      int height = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      height = caca_get_display_height(display);
      Tcl_SetObjResult(interp, Tcl_NewIntObj( height ));
      break;
    }

    case DIS_SET_MOUSE: {
      int val = 0;
      int result = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "flag");
        return TCL_ERROR;
      }

      if( Tcl_GetBooleanFromObj(interp, objv[2], &val) ) return TCL_ERROR;

      result = caca_set_mouse(display, val);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case DIS_SET_CURSOR: {
      int val = 0;
      int result = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "flag");
        return TCL_ERROR;
      }

      if( Tcl_GetBooleanFromObj(interp, objv[2], &val) ) return TCL_ERROR;

      result = caca_set_cursor(display, val);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case DIS_SET_TITLE: {
      int result = 0;
      const char *title = NULL;
      int length = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "title");
        return TCL_ERROR;
      }

      title = Tcl_GetStringFromObj(objv[2], &length);
      if( !title || length < 1) {
        return TCL_ERROR;
      }

      result = caca_set_display_title(display, title);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case DIS_CREATE_EVENT: {
      caca_event_t *event = NULL;
      Tcl_HashEntry *newHashEntryPtr;
      char handleName[16 + TCL_INTEGER_SPACE];
      Tcl_Obj *pResultStr = NULL;
      int newvalue;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      event = (caca_event_t *) malloc(sizeof(caca_event_t));

      Tcl_MutexLock(&myMutex);
      sprintf( handleName, "caca_event_%d", event_count++ );

      pResultStr = Tcl_NewStringObj( handleName, -1 );

      newHashEntryPtr = Tcl_CreateHashEntry(caca_hashtblPtr, handleName, &newvalue);
      Tcl_SetHashValue(newHashEntryPtr, event);
      Tcl_MutexUnlock(&myMutex);

      Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) EVENT_MAIN,
             (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

      Tcl_SetObjResult(interp, pResultStr);
      break;
    }

    case DIS_GET_EVENT: {
      caca_event_t *event = NULL;
      Tcl_HashEntry *hashEntryPtr;
      char *evtHandle = NULL;
      int type = 0, timeout = 0;
      int result = 0;

      if( objc != 5 ){
        Tcl_WrongNumArgs(interp, 2, objv, "type event_handle timeout");
        return TCL_ERROR;
      }

      evtHandle = Tcl_GetStringFromObj(objv[3], 0);
      hashEntryPtr = Tcl_FindHashEntry( caca_hashtblPtr, evtHandle );
      if( !hashEntryPtr ) {
          if( interp ) {
             Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
             Tcl_AppendStringsToObj( resultObj, "invalid event handle ", evtHandle, (char *)NULL );
          }

          return TCL_ERROR;
      }

      event = Tcl_GetHashValue( hashEntryPtr );

      if(Tcl_GetIntFromObj(interp, objv[2], &type) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &timeout) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_get_event(display, type, event, timeout);
      Tcl_SetObjResult(interp, Tcl_NewIntObj( result ));
      break;
    }

    case DIS_CLOSE: {
      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      caca_free_display (display);

      Tcl_MutexLock(&myMutex);
      if( hashEntryPtr )  Tcl_DeleteHashEntry(hashEntryPtr);
      Tcl_MutexUnlock(&myMutex);
      Tcl_DeleteCommand(interp, disHandle);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }
  }

  return TCL_OK;
}


static int EVENT_MAIN(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
  int choice;
  caca_event_t *event = NULL;
  Tcl_HashEntry *hashEntryPtr;
  char *evtHandle;

  static const char *EVT_strs[] = {
    "get_type",
    "get_key_ch",
    "get_key_utf32",
    "get_key_utf8",
    "get_mouse_button",
    "get_mouse_x",
    "get_mouse_y",
    "get_resize_width",
    "get_resize_height",
    "close",
    0
  };

  enum EVT_enum {
    EVT_GET_TYPE,
    EVT_GET_KEY_CH,
    EVT_GET_KEY_UTF32,
    EVT_GET_KEY_UTF8,
    EVT_GET_MOUSE_BUTTON,
    EVT_GET_MOUSE_X,
    EVT_GET_MOUSE_Y,
    EVT_RESIZE_WIDTH,
    EVT_RESIZE_HEIGHT,
    EVT_CLOSE,
  };

  if( objc < 2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
    return TCL_ERROR;
  }

  if( Tcl_GetIndexFromObj(interp, objv[1], EVT_strs, "option", 0, &choice) ){
    return TCL_ERROR;
  }

  evtHandle = Tcl_GetStringFromObj(objv[0], 0);
  hashEntryPtr = Tcl_FindHashEntry( caca_hashtblPtr, evtHandle );
  if( !hashEntryPtr ) {
    if( interp ) {
        Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
        Tcl_AppendStringsToObj( resultObj, "invalid event handle ", evtHandle, (char *)NULL );
    }

    return TCL_ERROR;
  }

  event = Tcl_GetHashValue( hashEntryPtr );

  switch( (enum EVT_enum)choice ){

    case EVT_GET_TYPE: {
      int type;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      type = (int) caca_get_event_type(event);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( type ));
      break;
    }

    case EVT_GET_KEY_CH: {
      int ch;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      ch = caca_get_event_key_ch(event);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( ch ));
      break;
    } 

    case EVT_GET_KEY_UTF32: {
      uint32_t ch;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      ch = caca_get_event_key_utf32(event);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( (int) ch ));
      break;
    }

    case EVT_GET_KEY_UTF8: {
      char utf8[7];

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      caca_get_event_key_utf8(event, utf8);

      Tcl_SetObjResult(interp, Tcl_NewStringObj( utf8,  -1));
      break;
    }

    case EVT_GET_MOUSE_BUTTON: {
      int val;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      val = caca_get_event_mouse_button(event);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( val ));
      break;
    }

    case EVT_GET_MOUSE_X: {
      int val;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      val = caca_get_event_mouse_x(event);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( val ));
      break;
    }

    case EVT_GET_MOUSE_Y: {
      int val;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      val = caca_get_event_mouse_y(event);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( val ));
      break;
    }

    case EVT_RESIZE_WIDTH: {
      int val;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      val = caca_get_event_resize_width(event);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( val ));
      break;
    }

    case EVT_RESIZE_HEIGHT: {
      int val;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      val = caca_get_event_resize_height(event);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( val ));
      break;
    }

    case EVT_CLOSE: {
      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      if (event) free (event);

      Tcl_MutexLock(&myMutex);
      if( hashEntryPtr )  Tcl_DeleteHashEntry(hashEntryPtr);
      Tcl_MutexUnlock(&myMutex);
      Tcl_DeleteCommand(interp, evtHandle);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }
  }

  return TCL_OK;
}


static int Caca_create_dither(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[])
{
  caca_dither_t *dither = NULL;
  Tcl_HashEntry *newHashEntryPtr;
  char handleName[16 + TCL_INTEGER_SPACE];
  int bpp = 0, w = 0, h = 0, pitch = 0;
  uint32_t rmark = 0, gmark = 0, bmark = 0, amark = 0;
  Tcl_Obj *pResultStr = NULL;
  int newvalue;

  if( objc != 9 ){
    Tcl_WrongNumArgs(interp, 1, objv, "bpp w h pitch rmark gmark bmark amark");
    return TCL_ERROR;
  }

  if(Tcl_GetIntFromObj(interp, objv[1], &bpp) != TCL_OK) {
    return TCL_ERROR;
  }

  if(Tcl_GetIntFromObj(interp, objv[2], &w) != TCL_OK) {
    return TCL_ERROR;
  }

  if(Tcl_GetIntFromObj(interp, objv[3], &h) != TCL_OK) {
    return TCL_ERROR;
  }

  if(Tcl_GetIntFromObj(interp, objv[4], (int *) &pitch) != TCL_OK) {
    return TCL_ERROR;
  }

  if(Tcl_GetIntFromObj(interp, objv[5], (int *) &rmark) != TCL_OK) {
    return TCL_ERROR;
  }

  if(Tcl_GetIntFromObj(interp, objv[6], (int *) &gmark) != TCL_OK) {
    return TCL_ERROR;
  }

  if(Tcl_GetIntFromObj(interp, objv[7], (int *) &bmark) != TCL_OK) {
    return TCL_ERROR;
  }

  if(Tcl_GetIntFromObj(interp, objv[8], (int *) &amark) != TCL_OK) {
    return TCL_ERROR;
  }

  dither = caca_create_dither(bpp, w, h, pitch, rmark, gmark, bmark, amark);
  if (dither == NULL) return TCL_ERROR;

  Tcl_MutexLock(&myMutex);
  sprintf( handleName, "caca_dither_%d", dither_count++ );

  pResultStr = Tcl_NewStringObj( handleName, -1 );

  newHashEntryPtr = Tcl_CreateHashEntry(caca_hashtblPtr, handleName, &newvalue);
  Tcl_SetHashValue(newHashEntryPtr, dither);
  Tcl_MutexUnlock(&myMutex);

  Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) DITHER_MAIN,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_SetObjResult(interp, pResultStr);
  return TCL_OK;

}


static int DITHER_MAIN(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
  int choice;
  caca_dither_t *dither = NULL;
  Tcl_HashEntry *hashEntryPtr;
  char *ditHandle;

  static const char *DIT_strs[] = {
    "set_brightness",
    "get_brightness",
    "set_gamma",
    "get_gamma",
    "set_contrast",
    "get_contrast",
    "set_antialias",
    "get_antialias",
    "get_antialias_list",
    "set_color",
    "get_color",
    "get_color_list",
    "set_charset",
    "get_charset",
    "get_charset_list",
    "set_algorithm",
    "get_algorithm",
    "get_algorithm_list",
    "bitmap",
    "close",
    0
  };

  enum DIT_enum {
    dit_set_brightness,
    dit_get_brightness,
    dit_set_gamma,
    dit_get_gamma,
    dit_set_contrast,
    dit_get_contrast,
    dit_set_antialias,
    dit_get_antialias,
    dit_get_antialias_list,
    dit_set_color,
    dit_get_color,
    dit_get_color_list,
    dit_set_charset,
    dit_get_charset,
    dit_get_charset_list,
    dit_set_algorithm,
    dit_get_algorithm,
    dit_get_algorithm_list,
    dit_bitmap,
    dic_close,
  };

  if( objc < 2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
    return TCL_ERROR;
  }

  if( Tcl_GetIndexFromObj(interp, objv[1], DIT_strs, "option", 0, &choice) ){
    return TCL_ERROR;
  }

  ditHandle = Tcl_GetStringFromObj(objv[0], 0);
  hashEntryPtr = Tcl_FindHashEntry( caca_hashtblPtr, ditHandle );
  if( !hashEntryPtr ) {
    if( interp ) {
        Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
        Tcl_AppendStringsToObj( resultObj, "invalid dither handle ", ditHandle, (char *)NULL );
    }

    return TCL_ERROR;
  }

  dither = Tcl_GetHashValue( hashEntryPtr );

  switch( (enum DIT_enum)choice ){

    case dit_set_brightness: {
      double val = 0.0;
      int result = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "brightness");
        return TCL_ERROR;
      }

      if(Tcl_GetDoubleFromObj(interp, objv[2], &val) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_set_dither_brightness(dither, (float) val);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case dit_get_brightness: {
      double val = 0.0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      val = caca_get_dither_brightness(dither);

      Tcl_SetObjResult(interp, Tcl_NewDoubleObj( val ));
      break;
    }

    case dit_set_gamma: {
      double val = 0.0;
      int result = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "gamma");
        return TCL_ERROR;
      }

      if(Tcl_GetDoubleFromObj(interp, objv[2], &val) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_set_dither_gamma(dither, (float) val);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case dit_get_gamma: {
      double val = 0.0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      val = caca_get_dither_gamma(dither);

      Tcl_SetObjResult(interp, Tcl_NewDoubleObj( val ));
      break;
    }

    case dit_set_contrast: {
      double val = 0.0;
      int result = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "contrast");
        return TCL_ERROR;
      }

      if(Tcl_GetDoubleFromObj(interp, objv[2], &val) != TCL_OK) {
        return TCL_ERROR;
      }

      result = caca_set_dither_contrast(dither, (float) val);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case dit_get_contrast: {
      double val = 0.0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      val = caca_get_dither_contrast(dither);

      Tcl_SetObjResult(interp, Tcl_NewDoubleObj( val ));
      break;
    }

    case dit_set_antialias: {
      char *val = NULL;
      int length = 0;
      int result = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "antialias");
        return TCL_ERROR;
      }

      val = Tcl_GetStringFromObj(objv[2], &length);
      if(!val || length < 1) {
        return TCL_ERROR;
      }

      result = caca_set_dither_antialias(dither, val);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case dit_get_antialias: {
      const char *val = NULL;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      val = caca_get_dither_antialias(dither);
      if(!val) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewStringObj( val, -1 ));
      break;
    }

    case dit_get_antialias_list: {
      char const * const *list = NULL;
      Tcl_Obj *pResultStr = NULL;
      int count = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      list = caca_get_dither_antialias_list(dither);
      if(!list) return TCL_ERROR;

      pResultStr = Tcl_NewListObj(0, NULL);
      for(count = 0; list[count] != NULL; count++) {
          Tcl_ListObjAppendElement(interp, pResultStr, 
              Tcl_NewStringObj(list[count], strlen(list[count])));
      }

      Tcl_SetObjResult(interp, pResultStr);
      break;
    }

    case dit_set_color: {
      char *val = NULL;
      int length = 0;
      int result = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "color");
        return TCL_ERROR;
      }

      val = Tcl_GetStringFromObj(objv[2], &length);
      if(!val || length < 1) {
        return TCL_ERROR;
      }

      result = caca_set_dither_color(dither, val);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case dit_get_color: {
      const char *val = NULL;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      val = caca_get_dither_color(dither);
      if(!val) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewStringObj( val, -1 ));
      break;
    }

    case dit_get_color_list: {
      char const * const *list = NULL;
      Tcl_Obj *pResultStr = NULL;
      int count = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      list = caca_get_dither_color_list(dither);
      if(!list) return TCL_ERROR;

      pResultStr = Tcl_NewListObj(0, NULL);
      for(count = 0; list[count] != NULL; count++) {
          Tcl_ListObjAppendElement(interp, pResultStr,
              Tcl_NewStringObj(list[count], strlen(list[count])));
      }

      Tcl_SetObjResult(interp, pResultStr);
      break;
    }

    case dit_set_charset: {
      char *val = NULL;
      int length = 0;
      int result = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "charset");
        return TCL_ERROR;
      }

      val = Tcl_GetStringFromObj(objv[2], &length);
      if(!val || length < 1) {
        return TCL_ERROR;
      }

      result = caca_set_dither_charset(dither, val);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case dit_get_charset: {
      const char *val = NULL;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      val = caca_get_dither_charset(dither);
      if(!val) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewStringObj( val, -1 ));
      break;
    }

    case dit_get_charset_list: {
      char const * const *list = NULL;
      Tcl_Obj *pResultStr = NULL;
      int count = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      list = caca_get_dither_charset_list(dither);
      if(!list) return TCL_ERROR;

      pResultStr = Tcl_NewListObj(0, NULL);
      for(count = 0; list[count] != NULL; count++) {
          Tcl_ListObjAppendElement(interp, pResultStr,
              Tcl_NewStringObj(list[count], strlen(list[count])));
      }

      Tcl_SetObjResult(interp, pResultStr);
      break;
    }

    case dit_set_algorithm: {
      char *val = NULL;
      int length = 0;
      int result = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "algorithm");
        return TCL_ERROR;
      }

      val = Tcl_GetStringFromObj(objv[2], &length);
      if(!val || length < 1) {
        return TCL_ERROR;
      }

      result = caca_set_dither_algorithm(dither, val);
      if(result < 0) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case dit_get_algorithm: {
      const char *val = NULL;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      val = caca_get_dither_algorithm(dither);
      if(!val) return TCL_ERROR;

      Tcl_SetObjResult(interp, Tcl_NewStringObj( val, -1 ));
      break;
    }

    case dit_get_algorithm_list: {
      char const * const *list = NULL;
      Tcl_Obj *pResultStr = NULL;
      int count = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      list = caca_get_dither_algorithm_list(dither);
      if(!list) return TCL_ERROR;

      pResultStr = Tcl_NewListObj(0, NULL);
      for(count = 0; list[count] != NULL; count++) {
          Tcl_ListObjAppendElement(interp, pResultStr,
              Tcl_NewStringObj(list[count], strlen(list[count])));
      }

      Tcl_SetObjResult(interp, pResultStr);
      break;
    }

    case dit_bitmap: {
      int x =  0, y = 0, w = 0, h = 0;
      caca_canvas_t *canvas = NULL;
      Tcl_HashEntry *hashEntryPtr;
      char *canHandle = NULL;
      int length = 0;
      unsigned char *bytearray = NULL;

      if( objc != 8 ){
        Tcl_WrongNumArgs(interp, 2, objv, "canvas_handle x y w h pixels");
        return TCL_ERROR;
      }

      canHandle = Tcl_GetStringFromObj(objv[2], &length);
      if(canHandle == NULL || length < 1) {
        return TCL_ERROR;
      }

      hashEntryPtr = Tcl_FindHashEntry( caca_hashtblPtr, canHandle );
      if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "invalid canvas handle ", canHandle, (char *)NULL );
        }

        return TCL_ERROR;
      }

      canvas = Tcl_GetHashValue( hashEntryPtr );
      if(!canvas) return TCL_ERROR;

      if(Tcl_GetIntFromObj(interp, objv[3], &x) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &y) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[5], &w) != TCL_OK) {
        return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[6], &h) != TCL_OK) {
        return TCL_ERROR;
      }

      bytearray = Tcl_GetByteArrayFromObj(objv[7], &length);
      if(!bytearray || length < 1) return TCL_ERROR;

      caca_dither_bitmap(canvas, x, y, w, h, dither,  (void *) bytearray);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }

    case dic_close: {
      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      caca_free_dither(dither);

      Tcl_MutexLock(&myMutex);
      if( hashEntryPtr )  Tcl_DeleteHashEntry(hashEntryPtr);
      Tcl_MutexUnlock(&myMutex);
      Tcl_DeleteCommand(interp, ditHandle);

      Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));
      break;
    }
  }

  return TCL_OK;
}

 
/*
 *----------------------------------------------------------------------
 *
 * Caca_Init --
 *
 *	Initialize the new package.
 *
 * Results:
 *	A standard Tcl result
 *
 * Side effects:
 *	The Caca package is created.
 *
 *----------------------------------------------------------------------
 */

int
Caca_Init(Tcl_Interp *interp)
{
    const char *version = NULL;

    if (Tcl_InitStubs(interp, "8.6", 0) == NULL) {
	return TCL_ERROR;
    }

    if (Tcl_PkgProvide(interp, PACKAGE_NAME, PACKAGE_VERSION) != TCL_OK) {
	return TCL_ERROR;
    }

    Tcl_CreateNamespace(interp, "caca", NULL, NULL);

    caca_hashtblPtr = (Tcl_HashTable *) ckalloc(sizeof(Tcl_HashTable));
    Tcl_InitHashTable(caca_hashtblPtr, TCL_STRING_KEYS);

    Tcl_CreateThreadExitHandler(Caca_Thread_Exit, (ClientData)NULL);

    version = caca_get_version();
    Tcl_SetVar2Ex(interp, "caca::version", NULL, Tcl_NewStringObj(version, -1), 0);

    /*
     * Now we add some variables here
     */

    Tcl_SetVar2Ex(interp, "caca::EVENT_NONE", NULL, Tcl_NewIntObj(0x0000), 0);
    Tcl_SetVar2Ex(interp, "caca::EVENT_KEY_PRESS", NULL, Tcl_NewIntObj(0x0001), 0);
    Tcl_SetVar2Ex(interp, "caca::EVENT_KEY_RELEASE", NULL, Tcl_NewIntObj(0x0002), 0);
    Tcl_SetVar2Ex(interp, "caca::EVENT_MOUSE_PRESS", NULL, Tcl_NewIntObj(0x0004), 0);
    Tcl_SetVar2Ex(interp, "caca::EVENT_MOUSE_RELEASE", NULL, Tcl_NewIntObj(0x0008), 0);
    Tcl_SetVar2Ex(interp, "caca::EVENT_MOUSE_MOTION", NULL, Tcl_NewIntObj(0x0010), 0);
    Tcl_SetVar2Ex(interp, "caca::EVENT_RESIZE", NULL, Tcl_NewIntObj(0x0020), 0);
    Tcl_SetVar2Ex(interp, "caca::EVENT_QUIT", NULL, Tcl_NewIntObj(0x0040), 0);
    Tcl_SetVar2Ex(interp, "caca::EVENT_ANY", NULL, Tcl_NewIntObj(0xffff), 0);

    Tcl_SetVar2Ex(interp, "caca::STYLE_BOLD", NULL, Tcl_NewIntObj(0x01), 0);
    Tcl_SetVar2Ex(interp, "caca::STYLE_ITALICS", NULL, Tcl_NewIntObj(0x02), 0);
    Tcl_SetVar2Ex(interp, "caca::STYLE_UNDERLINE", NULL, Tcl_NewIntObj(0x04), 0);
    Tcl_SetVar2Ex(interp, "caca::STYLE_BLINK", NULL, Tcl_NewIntObj(0x08), 0);

    /*
     * Commands
     */

    Tcl_CreateObjCommand(interp, "caca::get_display_driver_list", 
                        (Tcl_ObjCmdProc *) Caca_get_display_driver_list,
                        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "caca::get_export_list", 
                        (Tcl_ObjCmdProc *) Caca_get_export_list,
                        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "caca::get_import_list", 
                        (Tcl_ObjCmdProc *) Caca_get_import_list,
                        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "caca::get_font_list", 
                        (Tcl_ObjCmdProc *) Caca_get_font_list,
                        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "caca::rand", 
                        (Tcl_ObjCmdProc *) Caca_rand,
                        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "caca::create_canvas", 
                        (Tcl_ObjCmdProc *) Caca_create_canvas,
                        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "caca::create_display", 
                        (Tcl_ObjCmdProc *) Caca_create_display,
                        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "caca::create_display_with_driver", 
                        (Tcl_ObjCmdProc *) Caca_create_display_with_driver,
                        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "caca::create_dither",
                        (Tcl_ObjCmdProc *) Caca_create_dither,
                        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    return TCL_OK;
}
