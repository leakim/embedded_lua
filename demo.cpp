//#include "../lua-5.4.6/src/lua.hpp"
#include "lua-5.4.7/src/lua.hpp"
#include <iostream>
#include <cassert>

void print_stack_entry(lua_State* LS, int index);
void print_table(lua_State* LS, int index);
void print_stack(lua_State* LS);


/**
* Calculate mirror index.
*
* Notes on the lua stack.
*
* top: 3
* [ -3 == 1 ]
* [ -2 == 2 ]
* [ -1 == 3 ]
*
*/
int stack_index(lua_State* LS, int index)
{
    int top = lua_gettop(LS);
    if (index < 0)
        return top + 1 + index;
    return index - 1 - top;
}


void print_table(lua_State* LS)
{
    print_stack(LS);
    lua_pushnil(LS);
    print_stack(LS);
    while (lua_next(LS, -2)) {
        print_stack(LS);
        lua_pop(LS, 1);
    }
    lua_pop(LS, 1);  // nill
}


void print_stack_entry(lua_State* LS, int index)
{
    if (lua_isnumber(LS, index)) {
        int v = lua_tonumber(LS, index);
        printf("\t[%3d => %3d] [%d => %s] number %d\n",
               stack_index(LS, index),
               index,
               lua_type(LS, index),
               lua_typename(LS, lua_type(LS, index)),
               v
               );
        return;
    }

    if (lua_isstring(LS, index)) {
        const char* v = lua_tostring(LS, index);
        printf("\t[%3d => %3d] [%d => %s] string %s\n",
               stack_index(LS, index),
               index,
               lua_type(LS, index),
               lua_typename(LS, lua_type(LS, index)),
               v
               );
        return;
    }

    printf("\t[%3d => %3d] [%d => %s] -\n",
           stack_index(LS, index),
           index,
           lua_type(LS, index),
           lua_typename(LS, lua_type(LS, index))
           );
}


void print_stack(lua_State* LS)
{
    int top = lua_gettop(LS);
    printf("top: %d\n", top);

    for(int i = 1; i <= top; i++) {
        print_stack_entry(LS, i);
    }
}

static void *l_alloc (void *ud, void *ptr, size_t osize, size_t nsize)
{
    (void)ud;
    (void)osize;  /* not used */
    if (nsize == 0) {
        free(ptr);
        return NULL;
    }
    return realloc(ptr, nsize);
}

const char * l_reader(lua_State *LS, void *ud, size_t *size)
{
    FILE *f=(FILE *)ud;
    static char buff[512];
    if (feof(f)) return NULL;
    *size=fread(buff,1,sizeof(buff),f);
    return (*size>0) ? buff : NULL;
}

void process_stack_variable(lua_State* LS,
                            const char* name,
                            int index)
{

#if 0
    switch(lua_type(SL, -1)) {
        case LUA_TNIL:
        case LUA_TBOOLEAN:
        case LUA_TLIGHTUSERDATA:
        case LUA_TNUMBER:
        case LUA_TSTRING:
        case LUA_TTABLE:
        case LUA_TFUNCTION:
        case LUA_TUSERDATA:
        case LUA_TTHREAD:
    }
#endif

    printf("[%d] [%s] %s = [%s] ",
           index,
           lua_typename(LS, lua_type(LS, -2)),
           name,
           lua_typename(LS, lua_type(LS, -1)));

    if (lua_isnumber(LS, -1)) {
        int v = lua_tonumber(LS,-1);
        printf("%d\n", v);
        return;
    }

    if (lua_isstring(LS, -1)) {
        const char* v = lua_tostring(LS,-1);
        printf("%s\n", v);
        return;
    }

    if (lua_istable(LS, -1)) {
#if 0
        lua_pushnil(LS);
        while (lua_next(LS, -2)) {
            print_stack(LS);
            lua_pop(LS, 1);  // entry
        }
        //lua_pop(LS, 1);  // nil
#endif
        printf("table\n");
        return;
    }

    printf("?\n");
}


int main()
{
    int rc;
    FILE* file;
    void* ud = NULL;
    lua_State* L;

    file = fopen("demo.lua", "r");
    assert(file != NULL);

    L = lua_newstate(l_alloc, ud);

    // load script
    rc = lua_load(L, l_reader, file, lua_tostring(L, -1), NULL); // MIWI
    printf("rc = %d\n", rc);

    // run script
    rc = lua_pcall(L, 0, 0, 1);
    printf("rc = %d\n", rc);

    lua_pushglobaltable(L);
    lua_pushnil(L);

    int index = 0;
    while (lua_next(L,-2) != 0) {
        //print_stack(L);
        if (lua_isstring(L, -2)) {
            const char *name = lua_tostring(L, -2);
            process_stack_variable(L, name, index);
        }
        lua_pop(L,1); // table entry
        index++;
    }
    lua_pop(L,1); // nil

    lua_close(L);
    fclose(file);

    return EXIT_SUCCESS;
}

