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
#ifndef TVU_IOPOLL_H
#define TVU_IOPOLL_H

#ifdef TVU_HAVE_EPOLL
#include <sys/epoll.h>
#elif TVU_HAVE_KEVENT
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#endif

typedef struct {
    int     fd;     //< File descriptor to monitor
    bool    write;  //< Waiting to write
    bool    read;   //< Waiting to read
    bool    del;    //< Delete filter from the poll event queue.
} tvu_iopoll_t;

static inline int tvu_iopoll_create(void)
{
#ifdef TVU_HAVE_EPOLL
    return epoll_create1(FD_CLOEXEC);
#elif TVU_HAVE_KEVENT
    return kqueue();
#endif
}

static inline int tvu_iopoll_destroy(int self)
{
#ifdef TVU_HAVE_EPOLL
    return close(fd);
#elif TVU_HAVE_KEVENT
    return close(fd);
#endif
}

static inline int tvu_iopoll_update(int self, tvu_iopoll_t *_events, tvu_int *_events_size, tvu_iopoll_t *_filters, tvu_int _filters_size)
{
#ifdef TVU_HAVE_EPOLL
#elif TVU_HAVE_KEVENT
    struct kevent   filters[filters_size];
    struct kevent   events[]

    
#endif
}


#endif
