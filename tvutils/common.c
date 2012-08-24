/* libtvutils - Library of utilities.
 * Copyright (C) 2012  Take Vos <take.vos@vosgames.nl>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; of version 2 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <stdlib.h>
#include <dlfcn.h>
#include <tvutils/common.h>
#include <tvutils/base64.h>
#include <tvutils/path.h>

typedef tvu_int (*init_func_t)(void);

tvu_int tvu_call_init(utf8_t *name)
{
    void        *lib;
    init_func_t func;

    if ((lib = dlopen(NULL, RTLD_NOW)) == NULL) {
        return -1;
    }

    if ((func = (init_func_t)dlsym(lib, name)) == NULL) {
        // Init function is not found, do not make this an error, the library was just not linked.
        return 0;
    }

    return func();
}

#define TVU_CALL_INIT(name) \
    if ((ret = tvu_call_init(name)) == -1) {\
        return -1;\
    }\
    lib_count+= ret;

tvu_int tvu_init(utf8_t *argv0)
{
    tvu_int lib_count = 0;
    tvu_int ret;

    tvu_base_init();
    tvu_path_init(argv0);

    // Now call all init function of libraries.
    TVU_CALL_INIT("tvn_init");
    return lib_count;
}

