/*
 * Copyright (c) 2022-2023 Antmicro <www.antmicro.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef c_bindings_wasi_ext_lib_h_INCLUDED
#define c_bindings_wasi_ext_lib_h_INCLUDED

#include <wasi/api.h>

#include <stdlib.h>

#define _IOC_NONE 0U
#define _IOC_WRITE 1U
#define _IOC_READ 2U

#define _IORW_OFF 30
#define _IOS_OFF 16
#define _IOM_OFF 8
#define _IOF_OFF 0

#define _IORW_MASK 0xc0000000
#define _IOS_MASK 0x3fff0000
#define _IOM_MASK 0x0000ff00
#define _IOF_MASK 0x000000ff

#define _IOC(rw, maj, func, size)                                              \
    (rw << _IORW_OFF | size << _IOS_OFF | maj << _IOM_OFF | func << _IOF_OFF)

#define _IO(maj, func) _IOC(_IOC_NONE, maj, func, 0)
#define _IOW(maj, func, size) _IOC(_IOC_WRITE, maj, func, size)
#define _IOR(maj, func, size) _IOC(_IOC_READ, maj, func, size)
#define _IOWR(maj, func, size) _IOC(_IOC_WRITE | _IOC_READ, maj, func, size)

#define _IOGRW(mn) ((mn & _IORW_MASK) >> _IORW_OFF)
#define _IOGS(mn) ((mn & _IOS_MASK) >> _IOS_OFF)
#define _IOGM(mn) ((mn & _IOM_MASK) >> _IOM_OFF)
#define _IOGF(mn) ((mn & _IOF_MASK) >> _IOF_OFF)

#define _MAX_FD_NUM 1024

#define WGETGS _IOR(2, 0, 4)
#define WGETRH _IO(2, 1)
#define WGETRB _IO(2, 2)

// FIFO does not have a major, 0 is a placeholder, it might potentially conflict
// with memory device driver
#define FIFOSKERNW _IOW(0, 0, 4)
#define FIFOSKERNR _IOW(0, 1, 4)
#define FIFOSCLOSERM _IOW(0, 2, 4)

// Extended fs_fdflags
#define WASI_EXT_FDFLAG_CTRL_BIT ((__wasi_fdflags_t)0x0020)
#define WASI_EXT_FDFLAG_MASK ((__wasi_fdflags_t)0xffc0)
#define WASI_EXT_FDFLAG_CLOEXEC ((__wasi_fdflags_t)0x0040)

#define MKDEV(maj, min) ((maj << 20) | min)

// Fnctl commands
enum FcntlCommand { F_MVFD, F_GETFD, F_SETFD };

enum RedirectType {
    READ,
    WRITE,
    APPEND,
    READWRITE,
    PIPEIN,
    PIPEOUT,
    DUPLICATE,
    CLOSE
};
struct Redirect {
    union Data {
        struct Path {
            const char *path_str;
            size_t path_len;
        } path;

        int fd_src;
    } data;

    int fd_dst;
    enum RedirectType type;
};

struct Env {
    const char *attrib;
    const char *val;
};

#ifdef HTERM
typedef uint32_t WasiEvents;
#define WASI_EVENTS_NUM ((size_t)2)
#define WASI_EVENTS_MASK_SIZE ((size_t)4) // number of bytes
// Hterm events
#define WASI_EVENT_WINCH ((WasiEvents)(1 << 0))
#define WASI_EVENT_SIGINT ((WasiEvents)(1 << 1))
#endif

int wasi_ext_chdir(const char *);
int wasi_ext_getcwd(char *, size_t);
int wasi_ext_isatty(int);
int wasi_ext_set_env(const char *, const char *);
int wasi_ext_getpid();
#ifdef HTERM
int wasi_ext_event_source_fd(uint32_t);
int wasi_ext_attach_sigint(int32_t);
#endif
int wasi_ext_clean_inodes();
int wasi_ext_spawn(const char *, const char *const *, size_t,
                   const struct Env *, size_t, int, const struct Redirect *,
                   size_t, int *);
int wasi_ext_kill(int, int);
int wasi_ext_ioctl(int, unsigned int, void *);
int wasi_ext_fcntl(int, enum FcntlCommand, void *);
int wasi_ext_mount(int, const char *, int, const char *, const char *, uint64_t,
                   const char *);
int wasi_ext_umount(const char *);
int wasi_ext_mknod(const char *, int);

#endif
