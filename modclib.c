
#include <stdio.h>
#include <dlfcn.h>

#include "modclib.h"
#include "jsi.h"
#include "jsvalue.h"


const int JS_TOPAQUE = JS_TNULL;  /** @todo define distinct type tag */


static void js_pushopaque(js_State *J, void *p) {
    js_Value v;
    v.type = JS_TOPAQUE;
    v.u.object = p;
    js_pushvalue(J, v);
}

static void jsB_dlsym(js_State *J) {
    const char *nm = js_tostring(J, 1);
    void *s = dlsym(RTLD_DEFAULT, nm);
    js_pushopaque(J, s);
}

static void jsB_cstring(js_State *J) {
    const char *s = js_tostring(J, 1);
    js_pushopaque(J, (void*)s);
}

static void jsB_cint(js_State *J) {
    size_t i = js_tonumber(J, 1);  /* cast to word size */
    js_pushopaque(J, (void*)i);
}

typedef void *(*foreign0)();
typedef void *(*foreign1)(void *, ...);

static void jsB_ccall(js_State *J) {
    js_Value *v = js_tovalue(J, 1);
    void *fp = v->u.object, *retval;

    int n = js_gettop(J);
    switch (n) {  /* YEP */
    case 2: {
        retval = ((foreign0)(fp))(); break; }
    case 3: {
        void *arg0 = js_tovalue(J, 2)->u.object;
        retval = ((foreign1)(fp))(arg0); break; }
    case 4: {
        void *arg0 = js_tovalue(J, 2)->u.object,
             *arg1 = js_tovalue(J, 3)->u.object;
        retval = ((foreign1)(fp))(arg0, arg1); break; }
    case 5: {
        void *arg0 = js_tovalue(J, 2)->u.object,
             *arg1 = js_tovalue(J, 3)->u.object,
             *arg2 = js_tovalue(J, 4)->u.object;
        retval = ((foreign1)(fp))(arg0, arg1, arg2); break; }
    case 6: {
        void *arg0 = js_tovalue(J, 2)->u.object,
             *arg1 = js_tovalue(J, 3)->u.object,
             *arg2 = js_tovalue(J, 4)->u.object,
             *arg3 = js_tovalue(J, 5)->u.object;
        retval = ((foreign1)(fp))(arg0, arg1, arg2, arg3); break; }
    case 7: {
        void *arg0 = js_tovalue(J, 2)->u.object,
             *arg1 = js_tovalue(J, 3)->u.object,
             *arg2 = js_tovalue(J, 4)->u.object,
             *arg3 = js_tovalue(J, 5)->u.object,
             *arg4 = js_tovalue(J, 5)->u.object;
        retval = ((foreign1)(fp))(arg0, arg1, arg2, arg3,
                                  arg4); break; }
    case 8: {
        void *arg0 = js_tovalue(J, 2)->u.object,
             *arg1 = js_tovalue(J, 3)->u.object,
             *arg2 = js_tovalue(J, 4)->u.object,
             *arg3 = js_tovalue(J, 5)->u.object,
             *arg4 = js_tovalue(J, 5)->u.object,
             *arg5 = js_tovalue(J, 5)->u.object;
        retval = ((foreign1)(fp))(arg0, arg1, arg2, arg3,
                                  arg4, arg5); break; }
    case 9: {
        void *arg0 = js_tovalue(J, 2)->u.object,
             *arg1 = js_tovalue(J, 3)->u.object,
             *arg2 = js_tovalue(J, 4)->u.object,
             *arg3 = js_tovalue(J, 5)->u.object,
             *arg4 = js_tovalue(J, 5)->u.object,
             *arg5 = js_tovalue(J, 5)->u.object,
             *arg6 = js_tovalue(J, 5)->u.object;
        retval = ((foreign1)(fp))(arg0, arg1, arg2, arg3,
                                  arg4, arg5, arg6); break; }
    case 10: {
        void *arg0 = js_tovalue(J, 2)->u.object,
             *arg1 = js_tovalue(J, 3)->u.object,
             *arg2 = js_tovalue(J, 4)->u.object,
             *arg3 = js_tovalue(J, 5)->u.object,
             *arg4 = js_tovalue(J, 5)->u.object,
             *arg5 = js_tovalue(J, 5)->u.object,
             *arg6 = js_tovalue(J, 5)->u.object,
             *arg7 = js_tovalue(J, 5)->u.object;
        retval = ((foreign1)(fp))(arg0, arg1, arg2, arg3,
                                  arg4, arg5, arg6, arg7); break; }
    default:
        js_pushliteral(J, "ccall: max argument count exceeded");
        js_throw(J); return;
    }

    js_pushopaque(J, retval);
}

static void js_globalfunc(js_State *J, void (*func)(js_State *J),
                          const char *name)
{
	js_newcfunction(J, func, name, 0);
	js_setglobal(J, name);
}

int
module_clib(js_State *J) {

    js_globalfunc(J, jsB_dlsym, "dlsym");
    js_globalfunc(J, jsB_cstring, "cstring");
    js_globalfunc(J, jsB_cint, "cint");
    js_globalfunc(J, jsB_ccall, "ccall");

    return 0;
}
