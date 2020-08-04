#include "rpi.h"

#define FILE_OPEN		1
#define FILE_CREATE		2

#define FILE_SIZE		4096
#define FILE_NUMBER		32


typedef struct {
	uint id;
	uint size;
	uint offset;
	char name[16];
} file_t;



int fs_init(int (*read)(), int (*write)());
int fs_open(file_t *fd, char *name, uint flags);
int fs_read(file_t *fd, char *buff, uint len);
int fs_write(file_t *fd, char *buff, uint len);
int fs_seek(file_t *fd, uint offset, uint current);
int fs_rename(file_t *fd, char *new_name);
int fs_list(file_t *fd);
int fs_next(filt_t *fd);
int fs_remove(char *name);
int fs_format(uint fsize, uint msize);
int fs_count();
