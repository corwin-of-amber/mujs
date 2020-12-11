
#include <stdio.h>
#include <dlfcn.h>
#include <stdint.h>

#include "modclin.h"
#include "jsi.h"
#include "jsvalue.h"


const int JS_TOPAQUE = JS_TNULL;  /** @todo define distinct type tag */


static void js_pushopaque(js_State *J, void *p) {
    js_Value v;
    v.type = JS_TOPAQUE;
    v.u.object = p;
    js_pushvalue(J, v);
}

int js_isopaque(js_State *J, int idx) {
    return js_tovalue(J, idx)->type == JS_TOPAQUE;
}

static void *js_toopaque(js_State *J, int idx) {
	js_Value *v = js_tovalue(J, idx);
	if (v->type == JS_TOPAQUE) return (void*)(v->u.object);
    js_typeerror(J, "not a C value");
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

static void jsB_cstring_tojsstring(js_State *J) {
    const char *s = js_toopaque(J, 1);
    js_pushstring(J, s);
}

static void jsB_cint(js_State *J) {
    size_t i = js_tointeger(J, 1);  /* cast to word size */
    js_pushopaque(J, (void*)i);
}

static void jsB_cint_tonumber(js_State *J) {
    size_t i = (size_t)js_toopaque(J, 1);
    js_pushnumber(J, i);
}

static void jsB_csetw(js_State *J) {
    void **base = js_toopaque(J, 1);
    size_t offset = js_tointeger(J, 2);
    void *val = js_toopaque(J, 3);
    base[offset] = val;
    js_pushundefined(J);
}

static void jsB_cset16(js_State *J) {
    uint16_t *base = js_toopaque(J, 1);
    size_t offset = js_tointeger(J, 2);
    void *val = js_toopaque(J, 3);
    base[offset] = (uint16_t)val;
    js_pushundefined(J);
}

static void jsB_cset32(js_State *J) {
    uint32_t *base = js_toopaque(J, 1);
    size_t offset = js_tointeger(J, 2);
    void *val = js_toopaque(J, 3);
    base[offset] = (uint32_t)val;
    js_pushundefined(J);
}


typedef void *(*foreign0)();
typedef void *(*foreign1)(void *, ...);

static void jsB_ccall(js_State *J) {
    void *fp = js_toopaque(J, 1), *retval;

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
    js_globalfunc(J, jsB_cint, "cint");
    js_globalfunc(J, jsB_cint_tonumber, "cint_");
    js_globalfunc(J, jsB_cstring, "cstring");
    js_globalfunc(J, jsB_cstring_tojsstring, "cstring_");
    js_globalfunc(J, jsB_ccall, "ccall");
    js_globalfunc(J, jsB_csetw, "csetw");
    js_globalfunc(J, jsB_cset16, "cset16");
    js_globalfunc(J, jsB_cset32, "cset32");

    return 0;
}
