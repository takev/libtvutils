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
#ifndef TVU_DAEMON_H
#define TVU_DAEMON_H

#include <tvutils/types.h>

/** Start daemon process.
 * Daemonizes the process, @see daemon().
 * This includes changing dir to /root and closing all open files except for the pid file.
 * A pid file is created and the pid is encoded as a string in this file. The pid file is
 * exclusively locked so that an outside process can determine if this process is still running.
 *
 * @param pidfile   Location of pid file.
 * @returns         -1 on error.
 */
tvu_int tvu_daemon(const utf8_t * restrict pidfile);

/** Close and delete the pid file.
 * This function is called atexit automatically, so you do not have to invoke this.
 * This function may be called multiple times without bad effects.
 */
void tvu_daemon_atexit(void);

/** Kill the deamon from whom the pid file is.
 * The deamon is only killed, if it still has a exclusive lock on the pid file.
 * This way we know the pid file still belongs to a process that created it.
 *
 * @param pidfile   Location of the pid file.
 * @returns         The pid of the process that was killed, 0 if no process was killed, -1 on error.
 */
tvu_int tvu_daemon_kill(const utf8_t * restrict pidfile);

#endif
