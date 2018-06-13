/* See LICENSE for licence details. */
/* util.h */
#ifndef _UTIL_H_
#define _UTIL_H_

/* error functions */
void logging(enum loglevel_t loglevel, char *format, ...);

/* wrapper of C functions */
int eopen(const char *path, int flag);
int eclose(int fd);
FILE *efopen(const char *path, char *mode);
int efclose(FILE *fp);
void *emmap(void *addr, size_t len, int prot, int flag, int fd, off_t offset);
int emunmap(void *ptr, size_t len);
void *ecalloc(size_t nmemb, size_t size);
void *erealloc(void *ptr, size_t size);
int epselect(int maxfd, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timespec *ts, const sigset_t *sigmask);
ssize_t ewrite(int fd, const void *buf, size_t size);
int esigaction(int signo, struct sigaction *act, struct sigaction *oact);
int etcgetattr(int fd, struct termios *tm);
int etcsetattr(int fd, int action, const struct termios *tm);
int eopenpty(int *amaster, int *aslave, char *aname,
	const struct termios *termp, const struct winsize *winsize);
pid_t eforkpty(int *amaster, char *name,
	const struct termios *termp, const struct winsize *winsize);
int esetenv(const char *name, const char *value, int overwrite);
int eexecvp(const char *file, const char *argv[]);
int eexecl(const char *path);
long estrtol(const char *nptr, char **endptr, int base);

/* parse_arg functions */
void reset_parm(struct parm_t *pt);
void add_parm(struct parm_t *pt, char *cp);
void parse_arg(char *buf, struct parm_t *pt, int delim, int (is_valid)(int c));

/* other functions */
int my_ceil(int val, int div);
int dec2num(char *str);
int hex2num(char *str);
int sum(struct parm_t *parm);

#endif /* _UTIL_H_ */
