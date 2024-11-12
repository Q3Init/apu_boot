/******************************************************************************/
/*---------------------------    Include      --------------------------------*/
/******************************************************************************/

#include "flash_type.h"
#include "flash.h"
#include "flash_cfg.h"

/******************************************************************************/
/*---------------------------    Macro        --------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*----------------------------Callback Function ------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*----------------------------Local Function ---------------------------------*/
/******************************************************************************/
/**
 * @brief initialize Flash module
 */
static void apm32e10xflash_init(void)
{
    /* Unlock the flash memory */
    FMC_Unlock();

    /* Clear all FMC flags */
    FMC_ClearStatusFlag(FMC_FLAG_OC | FMC_FLAG_PE | FMC_FLAG_WPE);
}

/**
 * @brief erase logical sector starting at the sector address
 *        Upon executing this command, the BUSY fag of the corresponding bank is set
 *
 * @param start_addr - sector start address
 */
static std_return_t apm32e10xflash_erase(uint32_t start_addr, uint32_t size)
{
    std_return_t ret_val = E_OK;
    uint32_t index = 0;
    uint32_t erase_addr = 0;
    uint32_t offset_addr = 0;
    if (((size <= 0) || (start_addr < APM32_FLASH_BASE_ADDR) ||
         ((start_addr + size) >= (APM32_FLASH_BASE_ADDR + APM32_FLASH_SIZE)))) // 非法地址
    {
        return E_NOK;
    }

    if (((start_addr - APM32_FLASH_BASE_ADDR) % CONFIG_APM32E1XX_PFLASH_PAGE_SIZE != 0) || (size % CONFIG_APM32E1XX_PFLASH_PAGE_SIZE != 0))
    {
        return E_NOK;
    }
    // 进行FLASH编程
    FMC_Unlock();

    while (1)
    {
        offset_addr = start_addr - APM32_FLASH_BASE_ADDR;
        index = offset_addr / CONFIG_APM32E1XX_PFLASH_PAGE_SIZE;
        erase_addr = index * CONFIG_APM32E1XX_PFLASH_PAGE_SIZE + APM32_FLASH_BASE_ADDR;

        if (FMC_STATUS_COMPLETE == FMC_ErasePage(erase_addr))
        {
            ret_val = E_OK;
        }
        else
        {
            ret_val = E_NOK;
            break;
        }
        // 清除所有标志位
        FMC_ClearStatusFlag(FMC_FLAG_OC | FMC_FLAG_PE | FMC_FLAG_WPE);
        start_addr = erase_addr + CONFIG_APM32E1XX_PFLASH_PAGE_SIZE;
        if (size > CONFIG_APM32E1XX_PFLASH_PAGE_SIZE)
        {
            size -= CONFIG_APM32E1XX_PFLASH_PAGE_SIZE;
        }
        else
        {
            break;
        }
    }

    // 退出FLASH编程
    FMC_Lock();
    return ret_val;
}

/**
 * @brief write data lager then one page
 *
 * @param start_addr  - page address
 * @param size        - data length
 * @param p_data      - data
 *
 * @return E_OK      - success
 * @return E_NOK  - otherwise
 */
static std_return_t apm32e10xflash_write(uint32_t start_addr, uint32_t size, uint32_t *p_data)
{
    std_return_t ret_val = E_OK;
    uint32_t i = 0;
    if ((start_addr < APM32_FLASH_BASE_ADDR) || (start_addr >= (APM32_FLASH_BASE_ADDR + APM32_FLASH_SIZE)) ||
        (start_addr + size) > (APM32_FLASH_BASE_ADDR + APM32_FLASH_SIZE)) // 非法
    {
        return E_NOK;
    }
    // 进行FLASH编程
    FMC_Unlock();
    for (i = 0; i < size; i++)
    {
        if (FMC_STATUS_COMPLETE == FMC_ProgramWord(start_addr + i*4, p_data[i]))
        {
            ret_val = E_OK;
        }
        else
        {
            ret_val = E_NOK;
            break;
        }
    }
    // 清除所有标志位
    FMC_ClearStatusFlag(FMC_FLAG_OC | FMC_FLAG_PE | FMC_FLAG_WPE);
    // 退出FLASH编程
    FMC_Lock();
    return ret_val;
}

/**
 * @brief read data from page
 *
 * @param page_addr - page address
 * @param p_wordl   - pointer to low 32bit data
 * @param p_wordh   - pointer to high 32bit data
 *
 * @return E_OK      - success
 * @return E_NOK  - otherwise
 */
static bool apm32e10xflash_read(uint32_t address, uint32_t length, uint32_t *p_buffer)
{
    std_return_t ret_val = E_OK;
    uint32_t i = 0;
    if ((address < APM32_FLASH_BASE_ADDR) || (address >= (APM32_FLASH_BASE_ADDR + APM32_FLASH_SIZE)) ||
        (address + length) > (APM32_FLASH_BASE_ADDR + APM32_FLASH_SIZE)) // 非法
    {
        return E_NOK;
    }
    for (i = 0; i < length; i++)
    {
        p_buffer[i] = *(volatile uint32_t *)(address + i);
    }

    return ret_val;
}

/**
 * @brief get flash sector size
 *
 * @param flash_type      - flash type
 *
 * @return E_OK      - success
 * @return E_NOK  - otherwise
 */
uint32_t apm32e10xflash_get_sector_size(uint8_t flash_type)
{
    uint32_t result = 0xFFFFFFFFu;
    if (PFLASH_TYPE == flash_type)
    {
        result = CONFIG_APM32E1XX_PFLASH_SECTOR_SIZE;
    }
    return result;
}

/**
 * @brief get flash sector size
 *
 * @param flash_type      - flash type
 *
 * @return E_OK      - success
 * @return E_NOK  - otherwise
 */
uint32_t apm32e10xflash_get_page_size(uint8_t flash_type)
{
    uint32_t result = 0xFFFFFFFFu;
    if (PFLASH_TYPE == flash_type)
    {
        result = CONFIG_APM32E1XX_PFLASH_PAGE_SIZE;
    }
    return result;
}

/******************************************************************************/
/*-------------------  Local Variable Definitions    -------------------------*/
/******************************************************************************/
static const flash_device_t apm32e10xflash_device =
    {
        .init = apm32e10xflash_init,
        .write = apm32e10xflash_write,
        .read = apm32e10xflash_read,
        .erase = apm32e10xflash_erase,
        .get_sector_size = apm32e10xflash_get_sector_size,
        .get_page_size = apm32e10xflash_get_page_size,
        .flash_driver_index = INTERNAL_PFLASH_DRIVER_INDEX,
};

/******************************************************************************/
/*----------------------------Global Function---------------------------------*/
/******************************************************************************/
/**
 * @brief register apm32e10x flash device to flash wrapper
 */
void apm32exx_flash_preinit(void)
{
    flash_register(&apm32e10xflash_device, 1);
}

