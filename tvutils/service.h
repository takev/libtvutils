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
#ifndef TVU_SERVICE_H
#define TVU_SERVICE_H
#define TVU_SERVICE_H_IN

#include <tvutils/target.h>
#include <tvutils/random.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#ifdef TVU_HAVE_TIPC
#include <sys/tipc.h>
#else
#include <sys/un.h>
#endif

/** The length to allocate for struct sockaddr.
 * This should be large enough for struct sockaddr_tipc and struct sockaddr_un.
 * sockaddr_un has a variable length, we will use the following format:
 *    "/tmp/tvu_service_%09i_%09i" which is 36 characters long.
 */
#define TVU_SERVICE_ADDRLEN 64

/** Make an address for a service, used for binding and sendto.
 * Create an address which binds (or unbinds) to cluster scope.
 *
 * @param service_nr    service number (positive for name, negative for port).
 * @param bind          if true then bind to cluster, if false then unbind form cluster.
 * @param address       Address which is filled in.
 */
#ifdef TVU_HAVE_TIPC
static inline socklen_t tvu_service_make_address(tvu_int _service_nr, bool bind, void *_address)
{
    struct sockaddr_tipc    *address = _address;
    uint64_t                service_nr = TVU_ABS(_service_nr);

    if (_service_nr >= 0) {
        // This is a service name.
        address->family                  = AF_TIPC;
        address->addrtype                = TIPC_ADDR_NAME;
        address->scope                   = bind ? TIPC_CLUSTER_SCOPE : -TIPC_CLUSTER_SCOPE;
        address->addr.name.name.type     = service_nr >> 32;
        address->addr.name.name.instance = service_nr & 0xffffffff;
        address->addr.name.domain        = 0;
    } else {
        // This is a port number.
        address->family                  = AF_TIPC;
        address->addrtype                = TIPC_ADDR_ID;
        address->scope                   = bind ? TIPC_ZONE_SCOPE : -TIPC_ZONE_SCOPE;
        address->addr.name.id.node       = service_nr >> 32;
        address->addr.name.id.ref        = service_nr & 0xffffffff;
    }
    return sizeof (sockaddr_tipc);
}
#endif
#ifndef TVU_HAVE_TIPC
static inline socklen_t tvu_service_make_address(tvu_int _service_nr, bool bind __attribute__((unused)), void *_address)
{
    struct sockaddr_un      *address = _address;
    uint64_t                service_nr = TVU_ABS(_service_nr);

    if (_service_nr >= 0) {
        address->sun_family = AF_UNIX;
        return offsetof(struct sockaddr_un, sun_path) + sprintf(address->sun_path, "/tmp/tvu_service_%019llu", (unsigned long long)service_nr) + 1;
    } else {
        address->sun_family = AF_UNIX;
        return offsetof(struct sockaddr_un, sun_path) + sprintf(address->sun_path, "/tmp/tvu_client_%019llu", (unsigned long long)service_nr) + 1;
    }
}
#endif

/** Make an address for a service, used for binding and sendto.
 * Create an address which binds (or unbinds) to cluster scope.
 *
 * @param service_nr    service number.
 * @param bind          if true then bind to cluster, if false then unbind form cluster.
 * @param address       Address which is filled in.
 */
static inline tvu_int tvu_service_parse_address(void * restrict _address, socklen_t address_len __attribute__((unused)))
{
#ifdef TVU_HAVE_TIPC
    struct sockaddr_tipc *address = _address;
    if (address->addrtype == TIPC_ADDR_NAME) {
        return ((tvu_int)address->addr.name.name.type << 32) | (tvu_int)address->addr.name.name.instance;
    } else if (address->addrtype == TIPC_ADDR_ID) {
        return -(((tvu_int)address->addr.id.node << 32) | (tvu_int)address->addr.id.ref);
    } else {
        fprintf(stderr, "Expect either TIPC_ADDR_NAME or TIPC_ADDR_ID addresses.");
        abort();
    }
#else
    struct sockaddr_un *address = _address;
    unsigned long long service_nr;

    if (memcmp(address->sun_path, "/tmp/tvu_service_", 17) == 0) {
        sscanf(address->sun_path, "/tmp/tvu_service_%19llu", &service_nr);
        return service_nr;
    } else if (memcmp(address->sun_path, "/tmp/tvu_client_", 16) == 0) {
        sscanf(address->sun_path, "/tmp/tvu_client_%19llu", &service_nr);
        return -(tvu_int)service_nr;
    } else {
        fprintf(stderr, "Expect either /tmp/tvu_service_* or /tmp/tvu_client_* addresses.");
        abort();
    }
#endif
}

static inline int tvu_service_socket(void)
{
#ifdef TVU_HAVE_TIPC
    return socket(AF_TIPC, SOCK_RDM, 0);
#else
    return socket(AF_UNIX, SOCK_DGRAM, 0);
#endif
}

static inline int tvu_service_bind(int socket, tvu_int service_nr)
{
    void                *address = alloca(TVU_SERVICE_ADDRLEN);
    socklen_t           address_len = tvu_service_make_address(service_nr, true, address);

#ifndef TVU_HAVE_TIPC
    if ((unlink(((struct sockaddr_un *)address)->sun_path) == -1) && (errno != ENOENT)) {
        return -1;
    }
#endif
    return bind(socket, (struct sockaddr *)address, address_len);
}

#ifdef TVU_HAVE_TIPC
static inline int tvu_service_unbind(int socket, tvu_int service_nr)
{
    void        *address = alloca(TVU_SERVICE_ADDRLEN);
    socklen_t   address_len = tvu_service_make_address(service_nr, false, address);

    return bind(socket, (struct sockaddr *)address, address_len);
}
#endif
#ifndef TVU_HAVE_TIPC
static inline int tvu_service_unbind(int socket __attribute__((unused)), tvu_int service_nr __attribute__((unused)))
{
    return 0;
}
#endif

#ifdef TVU_HAVE_TIPC
static inline int tvu_service_client_bind(int socket, tvu_int *service_nr)
{
    void        *address = alloca(TVU_SERVICE_ADDRLEN);
    socklen_t   address_len = TVU_SERVICE_ADDRLEN;
    int         r;
    
    if ((r = getsockname(socket, address, address_len)) == -1) {
        return -1;
    }

    *service_nr = tvu_service_parse_address(address, address_len)
    return 0;
}
#endif
#ifndef TVU_HAVE_TIPC
static inline int tvu_service_client_bind(int socket, tvu_int *service_nr)
{
    tvu_int     pid = getpid() & 0xffffffff;
    tvu_int     r = tvu_random() & 0xffffffff;

    *service_nr = -((pid << 32) | r);
    return tvu_service_bind(socket, *service_nr);;
}
#endif

static inline int tvu_service_sendto(int socket, tvu_int service_nr, void const * restrict buffer, size_t length, int flags)
{
    void        *address = alloca(TVU_SERVICE_ADDRLEN);
    socklen_t   address_len = tvu_service_make_address(service_nr, true, address);

    return sendto(socket, buffer, length, flags, (struct sockaddr *)address, address_len);
}

static inline int tvu_service_recvfrom(int socket, tvu_int * restrict service_nr, void * restrict buffer, size_t length, int flags)
{
    ssize_t     r;
    void        *address = alloca(TVU_SERVICE_ADDRLEN);
    socklen_t   address_len;

    if ((r = recvfrom(socket, buffer, length, flags, address, &address_len)) == -1) {
        return -1;
    }

    *service_nr = tvu_service_parse_address(address, address_len);
    return r;
}

static inline int tvu_service_close(int socket, tvu_int service_nr)
{
    #ifndef TVU_HAVE_TIPC
    void        *address = alloca(TVU_SERVICE_ADDRLEN);
    socklen_t   address_len;
    address_len = tvu_service_make_address(service_nr, true, address);

    unlink(((struct sockaddr_un *)address)->sun_path);
    #endif
    return close(socket);
}

#undef TVU_SERVICE_H_IN
#endif
