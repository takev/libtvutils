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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <tvutils/types.h>
#include <tvutils/target.h>
#include <tvutils/cuid.h>
#ifdef TVU_HAVE_TIPC
#include <unistd.h>
#include <sys/socket.h>
#include <linux/tipc.h>
#endif

tvu_cuid_state_t *tvu_cuid_state = NULL;

int tvu_cuid_init(void)
{
    int shmid;
#ifdef TVU_HAVE_TIPC
    int                     fd;
    struct sockaddr_tipc    localaddr;
    socklen_t               addrlen = sizeof (struct sockaddr_tipc);
#endif

    if (tvu_cuid_state) {
        // If the tvu_cuid_state is already allocated then don't initialize it again.
        return 0;
    }

    // Retrieve or create and attach the tvu_cuid_state from shared memory.
    if ((shmid = shmget(TVID_SHM_KEY, sizeof (tvu_cuid_state_t), IPC_CREAT | 0666)) == -1) {
        return -1;
    }

    if ((tvu_cuid_state = shmat(shmid, NULL, 0)) == (void *) -1) {
        return -1;
    }

    // Now find out what the node id is of this server. We can overwrite it in shared
    // memory any time we like.
#ifdef TVU_HAVE_TIPC
    if ((fd = socket(AF_TIPC, SOCK_RDM, 0)) == -1) {
        return -1;
    }

    if (getsockname(fd, (struct sockaddr *)&localaddr, &addrlen) == -1) {
        return -1;
    }

    if (close(fd) == -1) {
        return -1;
    }

    // This may not use more than 12 bits.
    tvu_cuid_state->node_id = tipc_node(localaddr.addr.id.node);
#else
    // Unix domain sockets will be local host, so zero.
    tvu_cuid_state->node_id = 0;
#endif

    return 1;
}

