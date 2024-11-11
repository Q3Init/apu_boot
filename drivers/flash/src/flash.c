/******************************************************************************/
/*---------------------------    Include      --------------------------------*/
/******************************************************************************/
#include "device.h"
#include "flash_type.h"
#include <string.h>

/******************************************************************************/
/*---------------------------    Macros      ---------------------------------*/
/******************************************************************************/
/** flash_state_t
 * @brief
 * REGIF_UNINIT: After POR, REG-IF shall be in REGIF_UNINIT
 * REGIF_READY : flash_init shall change the module state to REGIF_READY
 */
typedef uint32_t flash_state_t;

#define FLASH_UNINIT ((flash_state_t)0x0)
#define FLASH_READY ((flash_state_t)0x1)

#define CONFIG_MAX_FLASH_DRIVER_NUM 5

/******************************************************************************/
/*-------------------  Local Variable Definitions    -------------------------*/
/******************************************************************************/
/** pointer to reg configuration parameters set */
static const flash_device_t *gp_flash_device[CONFIG_MAX_FLASH_DRIVER_NUM] = {NULL};

/** holds the current state of regif module, initially FLASH_UNINIT */
static flash_state_t g_flash_state[CONFIG_MAX_FLASH_DRIVER_NUM] = {FLASH_UNINIT};

/******************************************************************************/
/*----------------------------Local Function ---------------------------------*/
/******************************************************************************/
DEVICE_INITFUNC(apm32exx_flash_preinit);

/**
 * @brief preinitialize Flash module,register flash devices
 */
static void flash_preinit(void)
{
    apm32exx_flash_preinit();
}

/******************************************************************************/
/*----------------------------Global Function --------------------------------*/
/******************************************************************************/
/**
 * @brief initialize Flash device
 */
void flash_device_register(void)
{
    for (uint8_t i = 0; i < CONFIG_MAX_FLASH_DRIVER_NUM; i++)
    {
        gp_flash_device[i] = NULL;
    }

    flash_preinit();
}

/**
 * @brief initialize Flash module
 * @param driver_index -  driver index
 */
void flash_init(const uint8_t driver_index)
{
	flash_device_register();
    if ((NULL != gp_flash_device[driver_index]) && (NULL != gp_flash_device[driver_index]->init))
    {
        gp_flash_device[driver_index]->init();
        g_flash_state[driver_index] = FLASH_READY;
    }
}

/**
 * @brief deinitialize Flash module
 * @param driver_index -  driver index
 */
void flash_deinit(const uint8_t driver_index)
{
    if ((NULL != gp_flash_device[driver_index]) && (NULL != gp_flash_device[driver_index]->deinit))
    {
        gp_flash_device[driver_index]->deinit();
        g_flash_state[driver_index] = FLASH_UNINIT;
        gp_flash_device[driver_index] = NULL;
    }
}

/**
 * @brief read data from page
 *
 * @param addr -  address
 * @param len   - read length
 * @param data   - pointer to dest data
 *
 * @return E_OK      - success
 * @return E_NOT_OK  - otherwise
 */
std_return_t flash_read(const uint8_t driver_index, uint32_t addr, uint32_t len, uint32_t *data)
{
    std_return_t result = E_NOK;

    if ((NULL != gp_flash_device[driver_index]) && (NULL != gp_flash_device[driver_index]->read) && (FLASH_READY == g_flash_state[driver_index]))
    {
        result = gp_flash_device[driver_index]->read(addr, len, data);
    }

    return result;
}

/**
 * @brief write data lager then one page
 *
 * @param addr      - flash address
 * @param len       - length =  data length must large than page size
 * @param data      - data
 *
 * @return E_OK      - success
 * @return E_NOT_OK  - otherwise
 */
std_return_t flash_write(const uint8_t driver_index, uint32_t addr, uint32_t len, uint32_t *data)
{
    std_return_t result = E_NOK;

    if ((NULL != gp_flash_device[driver_index]) && (NULL != gp_flash_device[driver_index]->write) && (FLASH_READY == g_flash_state[driver_index]))
    {
        result = gp_flash_device[driver_index]->write(addr, len, data);
    }

    return result;
}

/**
 * @brief erase logical sector starting at the sector address
 *        Upon executing this command, the BUSY fag of the corresponding bank is set
 *
 * @param driver_index -  driver index
 * @param addr - erase start address
 * @param size -  erase size - need align sector size
 *
 * @return E_OK      - success
 * @return E_NOT_OK  - otherwise
 */
std_return_t flash_erase(const uint8_t driver_index, uint32_t addr, uint32_t size)
{
    std_return_t result = E_NOK;

    if ((NULL != gp_flash_device[driver_index]) && (NULL != gp_flash_device[driver_index]->erase) && (FLASH_READY == g_flash_state[driver_index]))
    {
        result = gp_flash_device[driver_index]->erase(addr, size);
    }

    return result;
}

/**
 * @brief get flash sector size
 *
 * @param flash_type      - flash type
 *
 * @return E_OK      - success
 * @return E_NOT_OK  - otherwise
 */
uint32_t flash_get_sector_size(const uint8_t driver_index, uint8_t flash_type)
{
    uint32_t result = 0xffffffffu;

    if ((NULL != gp_flash_device[driver_index]) && (NULL != gp_flash_device[driver_index]->get_sector_size) && (FLASH_READY == g_flash_state[driver_index]))
    {
        result = gp_flash_device[driver_index]->get_sector_size(flash_type);
    }

    return result;
}
/**
 * @brief get flash page size
 *
 * @param flash_type      - flash type
 *
 * @return E_OK      - success
 * @return E_NOT_OK  - otherwise
 */
uint32_t flash_get_page_size(const uint8_t driver_index, uint8_t flash_type)
{
    uint32_t result = 0xffffffffu;

    if ((NULL != gp_flash_device[driver_index]) && (NULL != gp_flash_device[driver_index]->get_page_size) && (FLASH_READY == g_flash_state[driver_index]))
    {
        result = gp_flash_device[driver_index]->get_page_size(flash_type);
    }

    return result;
}

/**
 * @brief function to register flash device
 *
 * @param p_dev device pointer
 * @param device_num device pointer
 *
 * @return E_OK      - success
 * @return E_NOT_OK  - otherwise
 */
std_return_t flash_register(const flash_device_t *p_dev, uint8_t device_num)
{
    std_return_t ret = E_NOK;

    if ((device_num < CONFIG_MAX_FLASH_DRIVER_NUM) && (p_dev))
    {
        for (uint8_t i = 0; i < device_num; i++)
        {
            uint8_t index = p_dev[i].flash_driver_index;
            if ((NULL == gp_flash_device[index]) && (index < CONFIG_MAX_FLASH_DRIVER_NUM))
            {
                gp_flash_device[index] = &p_dev[i];
                ret = E_OK;
            }
            else
            {
                ret = E_NOK;
                break;
            }
        }
    }

    return ret;
}
