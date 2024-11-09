/******************************************************************************/
/*---------------------------    Include      --------------------------------*/
/******************************************************************************/

#include "flash_type.h"
#include "flash.h"

/******************************************************************************/
/*---------------------------    Macro        --------------------------------*/
/******************************************************************************/


/******************************************************************************/
/*----------------------------Callback Function ------------------------------*/
/******************************************************************************/
/* Declaration of Fls notification function (Test_FeeJobEndNotification())*/
void Test_FeeJobEndNotification(void)
{
#if CONFIG_FEE
    Fee_JobEndNotification();
#endif
}

/* Declaration of Fls notification function (Test_FeeJobErrorNotification())*/
void Test_FeeJobErrorNotification(void)
{
#if CONFIG_FEE
    Fee_JobErrorNotification();
#endif
}

/******************************************************************************/
/*----------------------------Local Function ---------------------------------*/
/******************************************************************************/
/**
 * @brief initialize Flash module
 */
static void apm32e10xflash_init(void)
{

}

static std_return_t apm32e10xflash_get_offset_addr(uint32_t start_addr, uint32_t size, uint32_t *p_offset_addr)
{
    std_return_t ret_val = E_NOK;

    return ret_val;
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
void apm32e10xflash_preinit(void)
{
    flash_register(&apm32e10xflash_device, 1 );
}
