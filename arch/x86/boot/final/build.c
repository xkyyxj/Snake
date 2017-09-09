#include <stdio.h>
#include <string.h>
#include "../init.h"	//将BOOT_LOADER_LENGTH定义在init.h当中，便于以后boot&loader变长了之后的修改

//定义boot以及loader总共占用的字节数 (移动到init.h当中)
//#define BOOT_LOADER_LENGTH (5 * 512)

unsigned char buffer[1024];

int main(int argc, char **args){
    if(argc >= 4){
        unsigned long bt_total_length = 0;
        size_t read_count = 0;
        FILE *boot_loader, *kernel, *final_img;
        boot_loader = fopen(args[1],"r");
        kernel = fopen(args[2],"r");
        final_img = fopen(args[3],"w");

        //读取boot_loader程序并且写入到最终镜像
        while(!feof(boot_loader)){
            read_count = fread(buffer,sizeof(unsigned char),sizeof(buffer),boot_loader);
            fwrite(buffer,sizeof(unsigned char),read_count,final_img);
            bt_total_length += read_count;
        }

        //根据boot_loader缺失的字节数填充(并且保留一部分空间用于存储Loader获取到的系统信息)
        long fill_length = BOOT_LOADER_LENGTH + MAX_SYSTEMINFO_LENGTH - bt_total_length;
        if(fill_length > 0){
            //填充0
            memset(buffer,0,sizeof(buffer));
            int fill_times = (fill_length / 1024);
            unsigned int last_fill_length = fill_length % 1014;
            fwrite(buffer,sizeof(unsigned char),last_fill_length,final_img);
            for(int i = 0;i < fill_times;i++){
                fwrite(buffer,sizeof(unsigned char),sizeof(buffer),final_img);
            }
        }

        //读取kernel并且写入到最终镜像当中
        while(!feof(kernel)){
            read_count = fread(buffer,sizeof(unsigned char),sizeof(buffer),kernel);
            fwrite(buffer,sizeof(unsigned char),read_count,final_img);
        }

        fclose(boot_loader);
        fclose(kernel);
        fclose(final_img);
    }
    return 0;
}
    
