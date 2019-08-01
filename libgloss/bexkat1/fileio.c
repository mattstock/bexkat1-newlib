#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "glue.h"
#include "serial.h"
#include "misc.h"
#if 0
#include "ff.h"

extern time_t fat2time(int fatdate, int fattime);
#endif

typedef struct {
  const char *name;
  int (*open)(const char *path,
	      int flags, int mode);
  int (*close)(int fd);
  int (*read)(int fd, char *ptr, int len);
  int (*write)(int fd, const char *ptr, int len);
  int (*fstat)(int fd, struct stat *buf);
  off_t (*lseek)(int fd, off_t offset, int whence);
} driver_t;

off_t nop_lseek(int fd, off_t offset, int whence) {
  errno = EIO;
  return ((off_t)-1);
}

int nop_open(const char *path,
	     int flags, int mode) {
  errno = EIO;
  return -1;
}

int nop_close(int fd) {
  errno = EIO;
  return -1;
}

int nop_fstat(int fd, struct stat *buf) {
  buf->st_mode = S_IFCHR;	/* Always pretend to be a tty */
  buf->st_blksize = 0;

  return (0);
}

#if 0
/* for FatFS, keep the various records we need statically */
static FATFS fatfs_fatfs;
static FIL fatfs_fils[1];

int fatfs_open(const char *path,
	       int flags, int mode) {
  FRESULT res;
  int fat_flags = 0;

  if (f_mount(&fatfs_fatfs, "", 1) != FR_OK) {
    errno = EACCES;
    return -1;
  }

  fat_flags = FA_READ;
  if (flags & O_WRONLY)
    fat_flags |= FA_WRITE;
  if (flags & O_RDWR)
    fat_flags |= FA_READ | FA_WRITE;
  if ((flags & (O_CREAT|O_TRUNC)) == (O_CREAT|O_TRUNC))
    fat_flags |= FA_CREATE_ALWAYS;
  else if (flags & O_CREAT)
    fat_flags |= FA_CREATE_NEW;
  else
    fat_flags |= FA_OPEN_EXISTING;

  res = f_open(&fatfs_fils[0], path, fat_flags);
  switch (res) {
  case FR_OK:
    if (flags & O_APPEND)
      f_lseek(&fatfs_fils[0], f_size(&fatfs_fils[0]));
    return 4;
  case FR_DISK_ERR:
  case FR_INT_ERR:
  case FR_INVALID_DRIVE:
  case FR_INVALID_OBJECT:
  case FR_LOCKED:
  case FR_NOT_READY:
  case FR_NO_FILESYSTEM:
  case FR_INVALID_NAME:
  case FR_NOT_ENABLED:
    errno = EIO;
    break;
  case FR_EXIST:
    errno = EEXIST;
    break;
  case FR_TIMEOUT:
    errno = EINTR;
    break;
  case FR_TOO_MANY_OPEN_FILES:
    errno = ENFILE;
    break;
  case FR_NO_PATH:
  case FR_NO_FILE:
    errno = ENOENT;
    break;
  case FR_NOT_ENOUGH_CORE:
    errno = ENOMEM;
    break;
  case FR_DENIED:
  case FR_WRITE_PROTECTED:
    errno = EACCES;
    break;
  }
  return -1;
}

int fatfs_close(int fd) {
  FRESULT res;

  res = f_close(&fatfs_fils[fd-4]);
  switch (res) {
  case FR_OK:
    f_mount((void *)0, "", 0);
    return 0;
    break;
  default: 
    errno = EIO;
  }
  return -1;
}

int fatfs_read(int fd, char *ptr, int len) {
  FRESULT res;
  int count;

  res = f_read(&fatfs_fils[fd-4], ptr, len, &count);
  if (res == FR_OK) {
    return count;
  }
  errno = EIO;
  return -1;
}

int fatfs_write(int fd, const char *ptr, int len) {
  FRESULT res;
  int count;

  res = f_write(&fatfs_fils[fd-4], ptr, len, &count);
  if (res == FR_OK) {
    return count;
  }
  errno = EIO;
  return -1;
}

off_t fatfs_lseek(int fd, off_t offset, int whence) {
  FRESULT res;
  FIL *mine = &fatfs_fils[fd-4];
  int val;

  switch (whence) {
  case SEEK_CUR:
    val = f_tell(mine) + offset;
    break;
  case SEEK_END:
    val = f_size(mine) + offset;
    break;
  default:
    val = offset;
    break;
  }
  res = f_lseek(mine, offset);
  if (res == FR_OK) {
    return val;
  }

  errno = EIO;
  return -1;
}

int fatfs_stat(const char *path, struct stat *buf) {
  FILINFO info;
  FRESULT res;

  res = f_stat(path, &info);
  if (res == FR_NO_FILE) {
    errno = ENOENT;
    return -1;
  }
  if (buf) {
    buf->st_dev = 0;
    buf->st_ino = 0;
    if (info.fattrib | AM_DIR)
      buf->st_mode = S_IFDIR;
    if (!(info.fattrib | AM_RDO))
      buf->st_mode |= S_IWUSR|S_IWGRP|S_IWOTH;
    buf->st_mode |= S_IRUSR|S_IRGRP|S_IROTH|S_IXUSR|S_IXGRP|S_IXOTH;
    buf->st_nlink = 1;
    buf->st_uid = 0;
    buf->st_gid = 0;
    buf->st_size = info.fsize;
    buf->st_blksize = 0;
    buf->st_blocks = 0;
    buf->st_atime = fat2time(info.fdate, info.ftime);
    buf->st_mtime = fat2time(info.fdate, info.ftime);
    buf->st_ctime = fat2time(info.fdate, info.ftime);
  }

  return 0;
}

int fatfs_fstat(int fd, struct stat *buf) {
  errno = EIO;
  return -1;
}

int fatfs_access(const char *path, int mode) {
  errno = EIO;
  return -1;
}

int fatfs_mkdir(const char *path, mode_t mode) {
  FRESULT res;

  res = f_mkdir(path);
  switch (res) {
  case FR_OK:
    return 0;
    break;
  case FR_EXIST:
    errno = EEXIST;
    break;
  case FR_NO_PATH:
    errno = ENOENT;
    break;
  default:
    errno = EIO;
    break;
  }
  return -1;
}

#endif

int serial_read(int fd, char *ptr, int len) {
  int i = 0;

  for (i = 0; i < len; i++) {
    *(ptr + i) = serial_getchar(fd == 3);
    if ((*(ptr + i) == '\n') || (*(ptr + i) == '\r')) {
      i++;
      break;
    }
  }
  return (i);
}

int serial_write(int fd, const char *ptr, int len) {
  int todo;

  for (todo = 0; todo < len; todo++) {
    serial_putchar(fd == 3, *ptr++);
  }
  return len;
}

const driver_t driver_serial0 = { "serial0",
				  nop_open,
				  nop_close,
				  serial_read,
				  serial_write,
				  nop_fstat,
				  nop_lseek };

const driver_t driver_serial1 = { "serial1",
				  nop_open,
				  nop_close,
				  serial_read,
				  serial_write,
				  nop_fstat,
				  nop_lseek };

/*const driver_t driver_fatfs = { "fatfs",
				fatfs_open,
				fatfs_close,
				fatfs_read,
				fatfs_write,
				fatfs_fstat,
				fatfs_lseek };
*/

const driver_t *driver_list[] = {
  &driver_serial0, /* stdin */
  &driver_serial0, /* stdout */
  &driver_serial0, /* stderr */
  &driver_serial1 /* 3 */
};

off_t
_lseek (int fd, off_t offset, int whence)
{
  return driver_list[fd]->lseek(fd, offset, whence);
}

#if 0
int
access (const char *path, int mode)
{
  return fatfs_access(path, mode);
}

int
mkdir (const char *path, mode_t mode)
{
  return fatfs_mkdir(path, mode);
}

int
_stat (const char *path, struct stat *buf)
{
  return fatfs_stat(path, buf);
}
#endif

int
_fstat (int fd, struct stat *buf)
{
  return driver_list[fd]->fstat(fd, buf);
}

int
_close (int file)
{
  return driver_list[file]->close(file);
}

int
_open (const char *path, int flags, int mode)
{
  int i;
  int fd = 4;
  for (i=0; i < 4; i++) {
    if (!strcmp(driver_list[i]->name, path)) {
      fd = i;
      break;
    }
  }
  return driver_list[fd]->open(path, flags, mode);
}

int
_read (int file, char *buf, int nbytes)
{
  return driver_list[file]->read(file, buf, nbytes);
}

int
_write (int file, char *ptr, int len)
{
  return driver_list[file]->write(file, ptr, len);
}
