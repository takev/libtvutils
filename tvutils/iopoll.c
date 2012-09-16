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
#include <tvutils/iopoll.h>

tvu_int tvu_iopoll_update(int self, tvu_iopoll_t * restrict _events, tvu_int _events_size, tvu_iopoll_t const * restrict _filters, tvu_int _filters_size, tvu_time_t _timeout)
{
#if defined(TVU_HAVE_EPOLL)
    struct epoll_event  filter;
    struct epoll_event  events[_events_size];
    int                 nr_events;
    int                 timeout = tvu_to_ms(_timeout);
    tvu_int             i;

    for (i = 0; i < _filters_size; i++) {
        filter.data.fd = _filters[i].fd;
        if (_filters[i].del) {
            filter.events = 0;
            epoll_ctl(self, EPOLL_CTL_DEL, _filters[i].fd, &filter);
        } else if (_filters[i].add) {
            filter.events = _filters[i].read ? EPOLLIN : 0;
            filter.events|= _filters[i].write ? EPOLLOUT : 0;
            epoll_ctl(self, EPOLL_CTL_ADD, _filters[i].fd, &filter);
        } else {
            filter.events = _filters[i].read ? EPOLLIN : 0;
            filter.events|= _filters[i].write ? EPOLLOUT : 0;
            epoll_ctl(self, EPOLL_CTL_MOD, _filters[i].fd, &filter);
        }
    }

    if ((nr_events = epoll_wait(self, events, _events_size, timeout)) == -1) {
        return -1;
    }

    for (i = 0; i < nr_events; i++) {
        _events[i].fd = events[i].data.fd
        _events[i].read = (events[i].events & (EPOLLIN | EPOLLHUB | EPOLLRDHUB)) > 0;
        _events[i].write = (events[i].events & EPOLLOUT) > 0;
    }
    return nr_events;

#elif defined(TVU_HAVE_KEVENT)
    struct kevent   filters[_filters_size * 2];
    struct kevent   events[_events_size];
    int             nr_events;
    struct timespec timeout = tvu_to_timespec(_timeout);
    tvu_int         i, j, k;

    for (j = i = 0; i < _filters_size; i++) {
        if (_filters[i].del) {
            EV_SET(&filters[j], _filters[i].fd, EVFILT_READ , EV_DELETE, 0, 0, 0); j++;
            EV_SET(&filters[j], _filters[i].fd, EVFILT_WRITE, EV_DELETE, 0, 0, 0); j++;
        } else if (_filters[i].add) {
            EV_SET(&filters[j], _filters[i].fd, EVFILT_READ , EV_ADD | (_filters[i].read  ? EV_ENABLE : EV_DISABLE), 0, 0, 0); j++;
            EV_SET(&filters[j], _filters[i].fd, EVFILT_WRITE, EV_ADD | (_filters[i].write ? EV_ENABLE : EV_DISABLE), 0, 0, 0); j++;
        } else {
            EV_SET(&filters[j], _filters[i].fd, EVFILT_READ , _filters[i].read  ? EV_ENABLE : EV_DISABLE, 0, 0, 0); j++;
            EV_SET(&filters[j], _filters[i].fd, EVFILT_WRITE, _filters[i].write ? EV_ENABLE : EV_DISABLE, 0, 0, 0); j++;
        }
    }

    if ((nr_events = kevent(self, filters, j, events, _events_size, &timeout)) == -1) {
        return -1;
    }

    for (j = i = 0; i < nr_events; i++) {
        // First find if the event for this fd is already created.
        for (k = 0; k < j; k++) {
            if (_events[k].fd == events[i].ident) {
                goto found_duplicate;
            }
        }
create_new_entry:
        j++;
        _events[k].fd = events[i].ident;
        _events[k].read = false;
        _events[j].write = false;
found_duplicate:
        _events[k].read  |= events[i].filter == EVFILT_READ;
        _events[k].write |= events[i].filter == EVFILT_WRITE;
    }

    return j; // Number of events, may be less than nr_events, since there can be duplicates.
#endif
}


