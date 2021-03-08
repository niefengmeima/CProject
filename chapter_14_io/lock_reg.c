#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
	struct flock lock;
	lock.l_type = type; /*F_RDLCK=共享读锁,F_WRLCK=互斥写锁,F_UNLCK=解锁*/
	lock.l_start = offset;
	lock.l_whence = whence; /*SEEK_SET表示offset的位置就是开始,SEEK_CUR=当前,SEEK_END=结尾*/
	lock.l_len = len;

	return (fcntl(fd, cmd, &lock));
}

#define read_lock(fd, offset, whence, len) \
	lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
/*带w是阻塞版本*/
#define readw_lock(fd, offset, whence, len) \
	lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
#define write_lock(fd, offset, whence, len) \
	lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define writew_lock(fd, offset, whence, len) \
	lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
#define un_lock(fd, offset, whence, len) \
	lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))

pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
	struct flock lock;
	lock.l_type = type;
	lock.l_start = offset;
	lock.l_whence = whence;
	lock.l_len = len;

	if (fcntl(fd, F_GETLK, &lock) < 0)
		fprintf(stderr, "fcntl error");
	if (lock.l_type == F_UNLCK)
		return (0);		 //没有进程使用这把锁的时候返回0
	return (lock.l_pid); //返回拥有这把锁的线程的id.
}

#define is_read_lockable(fd, offset, whence, len) \
	(lock_test((fd), F_RDLCK, (offset), (whence), (len)) == 0)

#define is_write_lockable(fd, offset, whence, len) \
	(lock_test((fd), F_WRLCK, (offset), (whence), (len)))

static void lockabyte(const char *name, int fd, off_t offset)
{
	if (writew_lock(fd, offset, SEEK_SET, 1) < 0)
		fprintf("%s:writew_lock error", name);
	printf("%s:got the lock,byte %1d\n", name, offset);
}

int main(void)
{
	int fd;
	pid_t pid;

	if ((fd = creat("templock", O_WRONLY)) < 0)
		fprintf(stderr, "creat error");
	if (write(fd, "ab", 2) != 2)
		fprintf(stderr, "write error");

	if ((pid == fork()) < 0)
		fprintf(stderr, "fork error");
	else if (pid == 0)
	{
		lockabyte("child", fd, 0);
		lockabyte("child", fd, 1);
	}
	else
	{
		lockabyte("parent", fd, 1);
		lockabyte("parent", fd, 0);
	}
	exit(0);
}