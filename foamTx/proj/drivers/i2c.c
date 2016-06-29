#include "../../proj/tl_common.h"
#include "../../proj/mcu/watchdog_i.h"
#include "i2c.h"

#ifndef PIN_I2C_SCL
#define PIN_I2C_SCL      GPIO_GP4
#endif

#ifndef PIN_I2C_SDA
#define PIN_I2C_SDA      GPIO_GP5
#endif
extern void (*i2c_irq_handler)(void);
#define TELINKID	0x5c
void i2c_long_wait(void){
	u8 delay=100;
	while(delay--);
}
static inline void i2c_sda_out(int v){
	gpio_set_output_en(PIN_I2C_SDA,1);
	gpio_set_input_en(PIN_I2C_SDA,0);
	gpio_write(PIN_I2C_SDA,v);
}
static inline void i2c_scl_out(int v){
	gpio_set_output_en(PIN_I2C_SCL,1);
	gpio_set_input_en(PIN_I2C_SCL,0);
	gpio_write(PIN_I2C_SCL,v);
}
static inline int i2c_sda_in(void){
	gpio_set_input_en(PIN_I2C_SDA,1);
	gpio_set_output_en(PIN_I2C_SDA,0);
	return gpio_read(PIN_I2C_SDA);
}
static void i2c_wait_busy(void)
{
	u8 Times=0;
	while ((i2c_sda_in() != 0)&&(Times++<200));
}
static void i2c_wirte_bit(int bit)
{
	i2c_scl_out(0);
	i2c_sda_out(bit);
	i2c_long_wait();
	i2c_scl_out(1);
	i2c_long_wait();
}
static int i2c_read_bit(void){
//	i2c_wirte_bit(1);
	i2c_scl_out(0);
//	i2c_sda_out(bit);
	i2c_long_wait();
	i2c_scl_out(1);
	i2c_long_wait();
	return i2c_sda_in();
}
static inline void i2c_scl_init(void){
	gpio_set_func(PIN_I2C_SCL, AS_GPIO);
}
static inline void i2c_sda_init(void){
	gpio_set_func(PIN_I2C_SDA, AS_GPIO);
	gpio_set_input_en(PIN_I2C_SDA, 1);
}

static inline void i2c_scl_idle(void){
	gpio_set_output_en(PIN_I2C_SCL, 0);
	gpio_write(PIN_I2C_SCL, 0);
}

static inline void i2c_sda_idle(void){
	gpio_set_output_en(PIN_I2C_SDA, 0);
	gpio_write(PIN_I2C_SDA, 0);
}
/******************************************
 * ���ܣ�i2cдһ���ֽ�
 * ������dat   д����
 * ���أ�
 *****************************************/
int i2c_write_byte(u8 dat){
	int i = 0x80;
	while(i){
		i2c_wirte_bit((dat & i));
		i = i >> 1;
	}
	i2c_sda_in();
	return i2c_read_bit();
}
/******************************************
 * ���ܣ�i2c��ʼ����ͨ��
 * ������
 * ���أ�
 *****************************************/
void i2c_start(void)
{
	i2c_scl_init();
	i2c_sda_init();
	i2c_sda_idle();
	i2c_scl_idle();
	i2c_sda_out(0);		//sda: 0
}
/******************************************
 * ���ܣ�i2c��������ͨ��
 * ������
 * ���أ�
 *****************************************/
void i2c_stop(void)
{
	i2c_sda_out(0);
	i2c_scl_out(0);
	i2c_scl_out(1);
	i2c_sda_out(1);
}
/******************************************
 * ���ܣ�i2c��һ���ֽ�
 * ������last  Ӧ��
 * ���أ�����һ���ֽ�����
 *****************************************/
u8 i2c_read_byte(int last){
	u8 dat = 0,i;
	for(i=0;i<8;i++){
//		i2c_wirte_bit(1);
		i2c_scl_out(0);
		//	i2c_sda_out(bit);
		i2c_long_wait();
		i2c_scl_out(1);
		i2c_long_wait();
		if(i2c_sda_in()){
			dat =(dat << 1) | 0x01;
		}else{
			dat = dat << 1;
		}
	}
	i2c_wirte_bit(last);
	return dat;
}
void i2c_master_init(void)
{
//	i2c_init();
}
/******************************************
 * ���ܣ�i2c 8λ��ַģʽ������
 * ������id      �豸id
 *       addr    ����ַ
 *       *p      �������ݴ���ĵ�ַ
 *       n       �����ݵĳ���
 * ���أ�
 *****************************************/
void i2c_addr8_read(u8 id, u8 addr,u8 *p,u8 n)
{
	i2c_start();

	i2c_write_byte(id);
	i2c_wait_busy();
	i2c_write_byte(addr);
	i2c_wait_busy();
	i2c_stop();
	i2c_long_wait();
	i2c_start();
	i2c_write_byte(id | 1);
	i2c_wait_busy();

	for(int k = 0; k < n; ++k){
		*p++ = i2c_read_byte( k ==(n-1) );
	}
	i2c_stop();

}
/******************************************
 * ���ܣ�i2c 16λ��ַģʽ������
 * ������id      �豸id
 *       addr    ����ַ
 *       *p      �������ݴ���ĵ�ַ
 *       n       �����ݵĳ���
 * ���أ�
 *****************************************/
void i2c_addr16_read(u8 id, u16 addr,u8 *p,u8 n)
{
	i2c_start();

	i2c_write_byte(id);
	i2c_long_wait();
//	i2c_wait_busy();
	i2c_write_byte(addr>>8);
//	i2c_wait_busy();
	i2c_long_wait();
	i2c_write_byte(addr&0xff);
//	i2c_wait_busy();
	i2c_long_wait();
	i2c_stop();
	i2c_long_wait();
	i2c_start();
	i2c_write_byte(id | 1);
	i2c_long_wait();
//	i2c_wait_busy();
	for(int k = 0; k < n; ++k){
		*p++ = i2c_read_byte( k ==(n-1) );
	}
	i2c_stop();

}
/******************************************
 * ���ܣ�i2c dmaģʽ�µĶ�
 * ������id      ���豸id
 *      address  ����ַ
 *      addr_mod �豸�ĵ�ַģʽ     0  8bit��ַ      1   16bit��ַ
 * ���أ�����������
 *****************************************/
u8 i2c_dma_read(u8 id,int address,u8 addr_mod)
{
	u8 dat;
	if(addr_mod==1)
		i2c_addr16_read (id, address, &dat, 1);
	else
		i2c_addr8_read (id, address, &dat, 1);
	return dat;
}
/******************************************
 * ���ܣ�i2c 8λ��ַģʽд����
 * ������id      �豸id
 *       addr    ����ַ
 *       data    д����
 * ���أ�
 *****************************************/
void i2c_addr8_write(u8 id, u8 addr, u8 dat)
{
	i2c_start();
	i2c_write_byte(id&0xfe);
	i2c_wait_busy();
	i2c_write_byte(addr);
	i2c_wait_busy();
	i2c_write_byte(dat);
	i2c_wait_busy();
	i2c_stop();
}
/******************************************
 * ���ܣ�i2c 16λ��ַģʽд����
 * ������id      �豸id
 *       addr    ����ַ
 *       data    д����
 * ���أ�
 *****************************************/
void i2c_addr16_write(u8 id, u16 addr, u8 data)
{
	i2c_start();
	i2c_write_byte(id&0xfe);
	i2c_wait_busy();
	i2c_write_byte(addr>>8);
	i2c_wait_busy();
	i2c_write_byte(addr&0xff);
	i2c_wait_busy();
	i2c_write_byte(data);
	i2c_wait_busy();
	i2c_stop();
}
/******************************************
 * ���ܣ�i2c dmaģʽ�µ�д
 * ������id      д�豸id
 *      address  д��ַ
 *      data     д����
 *      addr_mod �豸�ĵ�ַģʽ     0  8bit��ַ      1   16bit��ַ
 * ���أ�
 *****************************************/
void i2c_dma_write(u8 id,int address, int data,u8 addr_mod)
{
	static unsigned int prev_wr_timing=0;

	while(!clock_time_exceed(prev_wr_timing, 5000)){
	#if(MODULE_WATCHDOG_ENABLE)
			wd_clear();
	#endif
		};
	if(addr_mod==1)
		i2c_addr16_write(id, address, data);
	else
		i2c_addr8_write(id, address, data);
	prev_wr_timing = clock_time();

//	WaitUs(5500);  //wait 5ms at least
}
/******************************************
 * ���ܣ�i2c mapģʽ��д����
 * ������id     дi2c�豸��id
 *       data   д����
 *       len    д�ĳ���
 * ���أ�
 *****************************************/
void i2c_map_write(u8 id,u8 *data,u8 len)
{
	u8 i;
	i2c_start();
	i2c_write_byte(id);
	i2c_wait_busy();
	for(i=0;i<len;i++){
		i2c_write_byte(data[i]);
	}
	i2c_stop();
}
/******************************************
 * ���ܣ�ӳ��ģʽ������
 * ������id   ��i2c�豸id
 *       buf  �������ݴ���Ļ���
 *       len  �����ݵĳ���
 * ���أ�
 *****************************************/
void i2c_map_read(u8 id,u8 *buf,u8 len)
{
	u8 i;
	i2c_start();
	i2c_write_byte(id|1);
	i2c_wait_busy();
	for(i=0;i<len-1;i++){
		buf[i]=i2c_read_byte(i==(len-1));
//		i2c_sda_out(1);
	}
	i2c_stop();
}
/******************************************
 * ���ܣ�i2c slaveģʽ��ʼ��
 * ������i2c_set   i2c������ģʽ
 *       buf      i2c��mapģʽ��ӳ���ַ
 * ���أ�
 *****************************************/
void I2C_SlaveInit(I2C_info_t i2c_set,u8 * pBuf)
{
    unsigned char tmp = 0;

    /******1. Set SCL/SDA Pins******/
    if (i2c_set.i2c_port != I2C_PORT0) {
        //disable GP4/GP5 i2c function
		tmp = read_reg8(0x8005d4); //clear 0x5d4[0]
		tmp &= 0xfe;
		write_reg8(0x8005d4, tmp);

		if (i2c_set.i2c_port != I2C_PORT1) {
            //disable GP12/GP13 i2c function
		    tmp = read_reg8(0x80058e); //set 0x58e[5]
		    tmp |= 0x20;
		    write_reg8(0x80058e, tmp);

		    if (i2c_set.i2c_port != I2C_PORT2) {
                //disable GP22/GP23 i2c function
			    tmp = read_reg8(0x800596); //clear 0x596[7]
			    tmp |= 0x80;
			    write_reg8(0x800596, tmp);

			    //enable GP30/GP31 i2c function
			    tmp = read_reg8(0x80059e); //clear 0x59e[7]
			    tmp &= 0x7f;
			    write_reg8(0x80059e, tmp);
			    //set GP30/GP31 input enable
			    tmp = read_reg8(0x800599);
			    tmp |= 0xc0;
			    write_reg8(0x800599, tmp);
		    }
		    else {
                //enable GP22/GP23 i2c function
			    tmp = read_reg8(0x800596); //clear 0x596[7]
			    tmp &= 0x7f;
			    write_reg8(0x800596, tmp);
			    //set GP22/GP23 input enable
			    tmp = read_reg8(0x800591);
			    tmp |= 0xc0;
			    write_reg8(0x800591, tmp);
		    }
		}
		else {
            //enable GP12/GP13 i2c function
		    tmp = read_reg8(0x80058e); //clear 0x58e[5]
		    tmp &= 0xdf;
		    write_reg8(0x80058e, tmp);
		    //set GP12/GP13 input enable
		    tmp = read_reg8(0x800589);
		    tmp |= 0x30;
		    write_reg8(0x800589, tmp);
		}
    }
    else {
	    /***set GP4/GP5 as SCL/SDA***/
	    //clear 0x586[4] 0x586[5]
	    tmp = read_reg8(0x800586);
	    tmp &= 0xcf;
	    write_reg8(0x800586, tmp);
	    //set 0x5d4[0]
	    tmp = read_reg8(0x8005d4);
	    tmp |= 0x01;
	    write_reg8(0x8005d4, tmp);
	    //set GP4/GP5 input enable
	    tmp = read_reg8(0x800581);
	    tmp |= 0x30;
	    write_reg8(0x800581, tmp);
    }

    /******2. Set I2C Slave Mode(DMA or Mapping)******/
    if (i2c_set.mode == I2C_SLAVE_MAP) {
    	//set 0x03[2] to enable mapping mode
	    tmp = read_reg8(0x800003);
	    tmp |= 0x04;
	    write_reg8(0x800003, tmp);
	    //set buf's address low byte
	    tmp = (unsigned int)pBuf & 0xff;
	    write_reg8(0x80003e, tmp);
	    //set buf's address high byte
	    tmp = ((unsigned int)pBuf>>8) & 0xff;
	    write_reg8(0x80003f, tmp);
    }

    /******3. Set I2C Slave Irq******/
    if (i2c_set.irq == I2C_IRQ_ENABLE) {
        write_reg8(0x800640, read_reg8(0x800640)|0x80); //enable irq_host_cmd
    	write_reg8(0x800643, read_reg8(0x800643)|0x01); //enable irq function
    	i2c_irq_handler=i2c_set.i2c_irq_handler;
    }
    else {
    	write_reg8(0x800640, read_reg8(0x800640)&0x7f); //disable irq_host_cmd
    }
}
