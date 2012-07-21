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
#include <tvutils/daemon.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/file.h>
#include <sys/resource.h>

int     tvu_daemon_fd = -1;
utf8_t  *tvu_daemon_pidfile = NULL;

tvu_int tvu_deamon(const utf8_t * restrict pidfile)
{
    utf8_t          pid_s[22];
    int             fd;
    struct rlimit   rlp;

    tvu_daemon_pidfile = strdup(pidfile);

    if ((tvu_daemon_fd = open(pidfile, O_WRONLY | O_CREAT | O_TRUNC | O_EXLOCK | O_NONBLOCK, 0666)) == -1) {
        // If we can't open or create the file, then the other process definitly still is running.
        fprintf(stderr, "Could not open pidfile '%s', another process may still have the lock.\n", pidfile);
        return -1;
    }

    // Do not close files, we have to do that manually, so as not to close tvu_daemon_fd.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    if (daemon(0, 1) == -1) {
#pragma GCC diagnostic pop
        perror("Could not daemonize");
        return -1;
    }

    // Quickly write a pid into the file, so this process can be killed.
    snprintf(pid_s, 22, "%lli\n", (long long)getpid());
    write(tvu_daemon_fd, pid_s, strlen(pid_s));
    
    if (getrlimit(RLIMIT_NOFILE, &rlp) == -1) {
        return -1;
    }

    // Since we instruction daemon() not to close file, we have to do it
    // here manually.
    for (fd = 0; fd < rlp.rlim_max; fd++) {
        if (fd != tvu_daemon_fd) {
            // Only close files that are not the pidfile, and close them all
            // without checking errors.
            close(fd);
        }
    }

    // Add tvu_daemon_end to atexit, so that it removed the pid file.
    atexit(tvu_daemon_atexit);
    return 0;
}

void tvu_daemon_atexit(void)
{
    if (tvu_daemon_fd != -1) {
        if (close(tvu_daemon_fd) == -1) {
            perror("Could not close pidfile.");
        }
        tvu_daemon_fd = -1;
    }

    if (tvu_daemon_pidfile) {
        if (unlink(tvu_daemon_pidfile) == -1) {
            perror("Could not unlink pidfile.");
        }
        free(tvu_daemon_pidfile);
        tvu_daemon_pidfile = NULL;
    }
}

tvu_int tvu_daemon_kill(const utf8_t * restrict pidfile)
{
    utf8_t      pid_s[22];
    ssize_t     pid_s_size;
    int         fd;
    pid_t       pid = 0;

    if ((fd = open(pidfile, O_RDONLY, 0666)) == -1) {
        // No pidfile, nothing to kill.
        return 0;
    }

    if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
        if (errno == EWOULDBLOCK) {
            // Another process is still keeping a lock, so we should kill it.
            pid_s_size = read(fd, pid_s, 22);
            if (pid_s_size == 0) {
                // Pid was not yet written in file.
                errno = EAGAIN;
                return -1;
            }

            pid = strtoll(pid_s, NULL, 10);
            if (pid == 0) {
                // Pid was not parsable.
                errno = EINVAL;
                return -1;
            }

            if (kill(pid, SIGINT) == -1) {
                return -1;
            }

        } else {
            return -1;
        }
    }

    // From here on the process has died and abandoned the pidfile.
    if (close(fd) == -1) {
        return -1;
    }

    if (unlink(pidfile) == -1) {
        return -1;
    }

    return pid;
}

