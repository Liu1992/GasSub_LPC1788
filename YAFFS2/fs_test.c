#include "yaffsfs.h"

#include <string.h>
#include <stdio.h>

void dump_directory_tree_worker(const char *dname,int recursive)
{
	yaffs_DIR *d;
	struct yaffs_dirent *de;
	struct yaffs_stat s;
	char str[1000];

	d = yaffs_opendir(dname);

	if(!d)
	{
		printf("opendir failed\n");
	}
	else
	{
		while((de = yaffs_readdir(d)) != NULL)
		{
			sprintf(str,"%s/%s",dname,de->d_name);

			yaffs_lstat(str,&s);

			printf("%s inode %d obj %x length %lld mode %X ",
				str,s.st_ino,de->d_dont_use, s.st_size,s.st_mode);
			switch(s.st_mode & S_IFMT)
			{
				case S_IFREG: printf("data file"); break;
				case S_IFDIR: printf("directory"); break;
				case S_IFLNK: printf("symlink -->");
							  if(yaffs_readlink(str,str,100) < 0)
								printf("no alias");
							  else
								printf("\"%s\"",str);
							  break;
				default: printf("unknown"); break;
			}

			printf("\n");

			if((s.st_mode & S_IFMT) == S_IFDIR && recursive)
				dump_directory_tree_worker(str,1);

		}

		yaffs_closedir(d);
	}

}

void dumpDir(const char *dname)
{	dump_directory_tree_worker(dname,0);
	printf("\n");
	printf("Free space in %s is %d\n\n",dname,(int)yaffs_freespace(dname));
}

void FsTest(void) {
    int f;
    int n;
    char str[64] = "hello yamiede~\n";
    char buf[64];
		int len1, len2, len3;
    
    yaffs_format("/nand", 0, 0, 0);
    yaffs_mount("/nand");
		yaffs_mkdir("/nand/web", 0666);

    yaffs_unlink("/nand/web/index.htm");
  
    f = yaffs_open("/nand/web/index.html", O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE);
    if (f >= 0)
    {
				len1 = strlen(str);
        n = yaffs_write(f, str, len1 );
				n = yaffs_write(f, str, len1 );
				n = yaffs_write(f, str, len1 );
				//printf("len=%d\n", len1 );
        yaffs_close(f);
        printf("%d\n", n);
    }

    strcpy( str, "route uri=qwertyuio\n" );
		//printf( "str=%s\n", str );
    f = yaffs_open("/nand/route.txt", O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE);
    if (f >= 0)
    {
				len2 = strlen(str);
        n = yaffs_write(f, str, len2 );
				//printf("len=%d\n", len2 );
        yaffs_close(f);
        printf("%d\n", n);
    }

    strcpy( str, "nand_auth=asdfghjklmnbvcxz\n" );
		//printf( "str=%s\n", str );
    f = yaffs_open("/nand/auth.txt", O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE);
    if (f >= 0)
    {
				len3 = strlen(str);
        n = yaffs_write(f, str, len3 );
        //printf("len=%d\n", len3 );
				yaffs_close(f);
        printf("%d\n", n);
    }

    f = yaffs_open("/nand/web/index.html", O_RDONLY, 0);
    if (f >= 0)
    {
        n = yaffs_read(f, buf, len1 * 4 );
        yaffs_close(f);
        buf[n] = '\0';
        printf("%d %s\n", n, buf);
    }
		
		f = yaffs_open("/nand/route.txt", O_RDONLY, 0);
    if (f >= 0)
    {
        n = yaffs_read(f, buf, len2 );
        yaffs_close(f);
        buf[n] = '\0';
        printf("%d %s\n", n, buf);
    }
		
		f = yaffs_open("/nand/auth.txt", O_RDONLY, 0);
    if (f >= 0)
    {
        n = yaffs_read(f, buf, len3 );
        yaffs_close(f);
        buf[n] = '\0';
        printf("%d %s\n", n, buf);
    }
    
    dumpDir("/nand");
    yaffs_unmount("/nand");
}
