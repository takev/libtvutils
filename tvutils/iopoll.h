/* libtvutils - Library of utilities.
 * Copyright (C) 2012  Take Vos <take.vos@vosgames.nl>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef TVU_IOPOLL_H
#define TVU_IOPOLL_H

#include <stdbool.h>
#include <unistd.h>
#include <tvutils/target.h>
#include <tvutils/time.h>

#if defined(TVU_HAVE_EPOLL)
#include <sys/epoll.h>
#elif defined(TVU_HAVE_KEVENT)
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#else
#error "Need either epoll or kevent."
#endif

typedef struct {
    int     fd;     //< File descriptor to monitor.
    bool    write;  //< Waiting to write.
    bool    read;   //< Waiting to read.
    bool    add;    //< Add filter to the poll event queue.
    bool    del;    //< Delete filter from the poll event queue.
} tvu_iopoll_t;

static inline int tvu_iopoll_create(void)
{
#if defined(TVU_HAVE_EPOLL)
    return epoll_create1(FD_CLOEXEC);
#elif defined(TVU_HAVE_KEVENT)
    return kqueue();
#endif
}

static inline int tvu_iopoll_destroy(int self)
{
#if defined(TVU_HAVE_EPOLL)
    return close(self);
#elif defined(TVU_HAVE_KEVENT)
    return close(self);
#endif
}

static inline tvu_int tvu_iopoll_update(int self, tvu_iopoll_t * restrict _events, tvu_int _events_size, tvu_iopoll_t const * restrict _filters, tvu_int _filters_size, tvu_time_t _timeout);


#endif
