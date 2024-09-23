#include "MPU6050.h"
#include "my_hardware_iic.h"
#define MPU6050_address 0xD0
void I2C_waitcheckevent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
uint32_t count=10000;
	while(	I2C_CheckEvent(I2Cx,I2C_EVENT)!=SUCCESS)
	{
		
		if((count--)<=0)
			break;
			
	}

}
void  MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data)
{

		/* my_iic_start();
		 my_iic_send_byte(MPU6050_address);
		 my_iic_get_ack();
		 my_iic_send_byte(RegAddress);
		 my_iic_get_ack();
		 my_iic_send_byte(Data);
		 my_iic_get_ack();
		 my_iic_stop();
	*/
	I2C_GenerateSTART(I2C2,ENABLE);//产生开始信号
	I2C_waitcheckevent(I2C2,I2C_EVENT_MASTER_MODE_SELECT ); //EV5事件 就是开始信号发送完毕
	I2C_Send7bitAddress(I2C2,MPU6050_address,I2C_Direction_Transmitter);//自带接收应答
	I2C_waitcheckevent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED  );//EV6事件 代表地址已经发送完毕
	I2C_SendData(I2C2,RegAddress);
	I2C_waitcheckevent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING  );//EV8事件 移位寄存器有数据，DR寄存器没有数据
	I2C_SendData(I2C2,Data);
	I2C_waitcheckevent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED); //EV8_2事件 因为这是最后一个数据，所以需要移位寄存器也没有数据
	I2C_GenerateSTOP(I2C2,ENABLE);
}
uint8_t  MPU6050_ReadReg(uint8_t RegAddress)
{
		uint8_t Data;
		/* my_iic_start();
		 my_iic_send_byte(MPU6050_address);
		 my_iic_get_ack();
		 my_iic_send_byte(RegAddress);
		 my_iic_get_ack();
		 my_iic_start();
		 my_iic_send_byte(MPU6050_address|0x01);
		 my_iic_get_ack();
		Data=	 my_iic_get_byte();
		 my_iic_send_ack(1);
		 my_iic_stop();
	*/
	I2C_GenerateSTART(I2C2,ENABLE);//产生开始信号
	I2C_waitcheckevent(I2C2,I2C_EVENT_MASTER_MODE_SELECT ); //EV5事件 就是开始信号发送完毕
	I2C_Send7bitAddress(I2C2,MPU6050_address,I2C_Direction_Transmitter);//自带接收应答
	
	
	I2C_waitcheckevent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED  );//EV6事件 代表地址已经发送完毕
		I2C_SendData(I2C2,RegAddress);
	I2C_waitcheckevent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED );//EV8事件 移位寄存器有数据，
	I2C_GenerateSTART(I2C2,ENABLE);//产生开始信号
		I2C_waitcheckevent(I2C2,I2C_EVENT_MASTER_MODE_SELECT ); //EV5事件 就是开始信号发送完毕
	I2C_Send7bitAddress(I2C2,MPU6050_address,I2C_Direction_Receiver);//写完寄存器后要将模式改成接收模式
	I2C_waitcheckevent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED            );//EV6事件 代表地址已经发送完毕
	//有一个特殊规定 在接收最后一个数据之前要将停止位和应答位置好
	//如果只接受一个字节的数据 
	//因为他是自动发送ack的，你必须在他接受完数据之前将ack设置好，否则就会多接收一个字节
	I2C_AcknowledgeConfig(I2C2,DISABLE);
	I2C_GenerateSTOP(I2C2,ENABLE);
	I2C_waitcheckevent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED   );//EV7事件，此时数据已经被放到DR寄存器里
	Data= I2C_ReceiveData(I2C2);
		I2C_AcknowledgeConfig(I2C2, ENABLE);	
	return Data;
}
void MPU6050_Init(void)
{
	
	my_hardware_iic_init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0X01);
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0X00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0X09);
	MPU6050_WriteReg(MPU6050_CONFIG,0X06);
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0X18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0X18);
}
void MPU6050_getdata(MPU6050_accel * accel,MPU6050_gyro * gyro)
{
uint16_t DATA_H,DATA_L;
	DATA_H=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DATA_L=MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	accel->accel_x=(DATA_H<<8)|DATA_L;
	DATA_H=MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DATA_L=MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	accel->accel_y=(DATA_H<<8)|DATA_L;
	DATA_H=MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DATA_L=MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	accel->accel_z=(DATA_H<<8)|DATA_L;
	DATA_H=MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DATA_L=MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	gyro->gyro_x=(DATA_H<<8)|DATA_L;
	DATA_H=MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DATA_L=MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
		gyro->gyro_y=(DATA_H<<8)|DATA_L;
	DATA_H=MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DATA_L=MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	gyro->gyro_z=(DATA_H<<8)|DATA_L;
	
	
	
}
