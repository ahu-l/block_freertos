#include "W25Q64.h"
#include "Hard_spi.h"
void W25Q64_init(void)
{
Hard_SPI_Init();
}
void W25Q64_ReadID(uint8_t  *MID ,uint16_t *DID)
{
	soft_spi_start();
	 Myspi_Swap_Byte(0X9F);
	*MID=Myspi_Swap_Byte(0XFF);
		*DID=Myspi_Swap_Byte(0XFF);
	*DID<<=8;
	*DID|=Myspi_Swap_Byte(0XFF);
	soft_spi_stop();
}
void W25Q64_W_Enable(void)
{
soft_spi_start();
	Myspi_Swap_Byte(W25Q64_WRITE_ENABLE	);
	soft_spi_stop();
}
void W25Q64_WaitBusy(void)
{
uint32_t Timeout;
	soft_spi_start();
	Myspi_Swap_Byte( W25Q64_READ_STATUS_REGISTER_1	);
	Timeout=100000;
	while((Myspi_Swap_Byte( W25Q64_DUMMY_BYTE)&0x01)==0x01)
	{
		Timeout--;
		if(Timeout==0)
			break;
	}
	soft_spi_stop();
}
void W25Q64_Write_data(uint32_t Address,uint8_t * DataArray,uint16_t Count)
{
	uint16_t i;
	W25Q64_W_Enable();
	soft_spi_start();
	Myspi_Swap_Byte(W25Q64_PAGE_PROGRAM	);
	Myspi_Swap_Byte(Address>>16);
	Myspi_Swap_Byte(Address>>8);
	Myspi_Swap_Byte(Address);
	for(i=0;i<Count;i++)
	{
			Myspi_Swap_Byte(DataArray[i]);
	}
	soft_spi_stop();
	W25Q64_WaitBusy();
}
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_W_Enable();
	soft_spi_start();
	Myspi_Swap_Byte( W25Q64_SECTOR_ERASE_4KB		);
		Myspi_Swap_Byte(Address>>16);
	Myspi_Swap_Byte(Address>>8);
	Myspi_Swap_Byte(Address);
	soft_spi_stop();
	W25Q64_WaitBusy();
}
void W25Q64_Read_data(uint32_t Address,uint8_t * DataArray,uint32_t Count)
{
	uint32_t i;
	soft_spi_start();
	Myspi_Swap_Byte(W25Q64_READ_DATA);
	Myspi_Swap_Byte(Address>>16);
	Myspi_Swap_Byte(Address>>8);
	Myspi_Swap_Byte(Address);
	for(i=0;i<Count;i++)
	{
			DataArray[i]=Myspi_Swap_Byte(W25Q64_DUMMY_BYTE	);
	}
	soft_spi_stop();
}