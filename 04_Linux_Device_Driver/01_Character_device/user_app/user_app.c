/* Day la chuong trinh tren user space tuong tac voi vchar_device
 *        vchar_device la mot thiet bi nam tren RAM. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

#define BUFFER_SIZE 1024
#define DEVICE_NODE "/dev/vchar_dev"
#define MAGIC_NUMBER 222
#define CLR_DATA_REGS _IO(MAGIC_NUMBER,0)
#define GET_STS_REGS _IOR(MAGIC_NUMBER,1, status_t *)
#define SET_RD_PER_DATA_REGS _IOW(MAGIC_NUMBER,2, unsigned char *)
#define SET_WR_PER_DATA_REGS _IOW(MAGIC_NUMBER,3, unsigned char *)



typedef struct {
	unsigned char read_count_h_reg;
	unsigned char read_count_l_reg;
	unsigned char write_count_h_reg;
	unsigned char write_count_l_reg;
	unsigned char device_status_reg;
} status_t;


/* ham kiem tra entry point open cua vchar driver */
int open_chardev() {
    int fd = open(DEVICE_NODE, O_RDWR);
    if(fd < 0) {
        printf("Can not open the device file\n");
        exit(1);
    }
    return fd;
}

/* ham kiem tra entry point release cua vchar driver */
void close_chardev(int fd) {
    close(fd);
}

/* ham kiem tra entrypoint read cua vchar driver */
void read_data_chardev(){
    int ret =0;
    char user_buf[BUFFER_SIZE];

    int fd = open_chardev();
    ret = read(fd, user_buf, BUFFER_SIZE);
    close_chardev(fd);

    if(ret < 0){
        printf("Could not read a message from %s\n", DEVICE_NODE);
    }
    else
        printf(" Read a message from %s: %s\n", DEVICE_NODE, user_buf);
}
/* ham kiem tra entry point write cua vchar driver */
void write_data_chardev(){
    int ret = 0;
    char user_buf[BUFFER_SIZE];
    printf("Enter your bla bla:");
    scanf(" %[^\n]s",user_buf);

    int fd = open_chardev();
    ret = write(fd, user_buf, strlen(user_buf) + 1);// ghi chuoi ky tu, bao gom ca
    close_chardev(fd);

    if(ret < 0)
        printf("Could not write the message to %s\n", DEVICE_NODE);
    else
        printf("Wrote the message to %s\n", DEVICE_NODE);
}
/* Cac ham kiem tra entry point ioctl cua vchar driver*/
void clear_data_chardev(){
    int fd = open_chardev();
    int ret = ioctl(fd,CLR_DATA_REGS);
    close_chardev(fd);
    printf("%s data registers of chardev\n",(ret < 0)?"Counld not clear":"Cleared");
}

void get_status_chardev(){
    unsigned int read_cnt,write_cnt;
    status_t status;
    int fd = open_chardev();
    ioctl(fd,GET_STS_REGS,&status);
    close_chardev(fd);

    read_cnt = status.read_count_h_reg<<8 | status.read_count_l_reg;
    write_cnt = status.write_count_h_reg<<8 | status.write_count_l_reg;

    printf("Statistic: number of reading (%u), number of writing (%u)\n",read_cnt,write_cnt);
}

void control_read_chardev(){
    unsigned char isReadEnable = 0;
    status_t status;
    char c ='n';

    printf("Do you want Disable or Enable to read from data registers (d/e)?");
    scanf(" %c",&c);

    int fd = open_chardev();
    ioctl(fd,GET_STS_REGS,&status);

    if(c == 'e'){
        if(status.device_status_reg&0x01)
        {
            printf("Device is already enable to read\n");
            return;
        }
        else{
            isReadEnable = 1;   
            ioctl(fd,SET_RD_PER_DATA_REGS,&isReadEnable); 
        }
        
    }
    else if (c == 'd'){
        if(status.device_status_reg&0x01)
        {
            isReadEnable = 0;
            ioctl(fd,SET_RD_PER_DATA_REGS,&isReadEnable); 
        }
        else{
            printf("Device is already disable to read\n");
            return; 
        }
    }

    
    printf("Finished to set!\n");
    close_chardev(fd);

}

void control_write_chardev(){
    unsigned char isWriteEnable = 0;
    status_t status;
    char c ='n';

    printf("Do you want Disable or Enable to write to data registers (d/e)?");
    scanf(" %c",&c);

    int fd = open_chardev();
    ioctl(fd,GET_STS_REGS,&status);

    if(c == 'e'){
        if(status.device_status_reg&0x02)
        {
            printf("Device is already enable to write\n");
            return;
        }
        else{
            isWriteEnable = 1;   
            ioctl(fd,SET_WR_PER_DATA_REGS,&isWriteEnable); 
        }
        
    }
    else if (c == 'd'){
        if(status.device_status_reg&0x02)
        {
            isWriteEnable = 0;
            ioctl(fd,SET_WR_PER_DATA_REGS,&isWriteEnable); 
        }
        else{
            printf("Device is already disable to write\n");
            return; 
        }
    }

    
    printf("Finished to set!\n");
    close_chardev(fd);

}
int main() {
    int ret = 0;
    char option = 'q';
    int fd = -1;
    printf("Select below options:\n");
    printf("\to (to open a device node)\n");
    printf("\tc (to close the device node)\n");
    printf("\tr (to read from the device node)\n");
    printf("\tw (to write to the device node)\n");
    printf("\tC (to clear data regsters)\n");
    printf("\tg (to get status registers)\n");
    printf("\tR (to enalble/disable to read data from data registers\n");
    printf("\tW (to enalble/disable to write data to data registers\n");
    printf("\tq (to quit the application)\n");
    while (1) {
        printf("Enter your option: ");
        scanf(" %c", &option);

        switch (option) {
            case 'o':
                if (fd < 0)
                    fd = open_chardev();
                else
                    printf("%s has already opened\n", DEVICE_NODE);
                break;
            case 'c':
                if (fd > -1)
                    close_chardev(fd);
                else
                    printf("%s has not opened yet! Can not close\n", DEVICE_NODE);
                fd = -1;
                break;
            case 'r':
                read_data_chardev();
                break;
            case 'w':
                write_data_chardev();
                break;
            case 'C':
                clear_data_chardev();
                break;
            case 'g':
                get_status_chardev();
                break;
            case 'R':
                control_read_chardev();
                break;
            case 'W':
                control_write_chardev();
                break;
            case 'q':
                if (fd > -1)
                    close_chardev(fd);
                printf("Quit the application. Good bye!\n");
                return 0;
            default:
                printf("invalid option %c\n", option);
                break;
        }
    };
}