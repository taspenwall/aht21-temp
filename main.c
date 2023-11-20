#include <stdio.h>

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#define addr _u(0x38)



void printBinary(uint8_t value) {
    for (int i = 7; i >= 0; --i) {
        printf("%d", (value & (1 << i)) ? 1 : 0);
    }
}

void aht21_read_data (){

    uint8_t r_buffer[6];
    uint8_t frames_read = i2c_read_blocking(i2c_default, addr, r_buffer,6,false);
    for (uint8_t i = 0; i<6; i++){
        printf("byte returned number %u : ", i);
        printBinary(r_buffer[i]);
        printf("\n");
        // print binary of each byte
        
    }
}

void aht21_prep (){
    sleep_ms(12);
    uint8_t trigger = 0xAC;
    
    i2c_write_blocking(i2c_default, addr, &trigger,1,false); //send the trigger command
    uint8_t to_send [2];
    to_send[0]=0x33;
    to_send[1]=0;
    i2c_write_blocking(i2c_default, addr, to_send, 2, false); // send the parameters for the trigger command
    sleep_ms(300);
    uint8_t buffer;
    uint8_t read_success = i2c_read_blocking(i2c_default,addr, &buffer, 1, false);
    if(read_success == 1){
        printf("successfully read the status bit.  The number of bytes read is %u . Binary of the status block ", read_success);
        printBinary(buffer);
        printf(" \n");
        uint8_t ready = buffer & 0x01; //look at the last bit of the status byte and check if is 0. his may be wrong but I thinking that bit[0] would be the 1st
                                          // and bit[7] would be the last

            if (ready==0){
                printf("sensor is ready to read\n");
                aht21_read_data();
            }
            else {
                printf("chip did not get ready");
            }
        }
    else {
            printf( "failed to read buffer\n");
        }


}

void aht21_init (){
    sleep_ms(150); //give sensor time to start up

    uint8_t buf;

    
   buf = 0x71;  //send 0x71 to check status of the aht21

    i2c_write_blocking(i2c_default, addr, &buf, 2, false);
    uint8_t rbuff = 0x0;
    uint8_t bytes_back = 0;
    bytes_back = i2c_read_blocking(i2c_default, addr, &rbuff,1,false);
    if(bytes_back==0) {
        printf("Failed to get data back, use anaylser\n");

    }
    else {
        
            if(rbuff!=0x18){
                printf ("Status byte returned is not 0x18,  the status byte is : ");
                printBinary(rbuff);
                printf("\n");
                printf("status byte hex : %u\n",rbuff);

            }
            else{
                printf("init of aht21 is successful\n");
                aht21_prep();
            }
    }


}



int main() {
    stdio_init_all();

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
    #warning i2c / bmp280_i2c example requires a board with I2C pins
        puts("Default I2C pins were not defined");
    return 0;
#else
    

    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);


    aht21_init();

    printf("End of program \n");
#endif    
}


