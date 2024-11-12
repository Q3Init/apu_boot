#ifndef FLASH_H
#define FLASH_H

#include "Platform_Types.h"
#include "flash_type.h"

#define PFLASH_TYPE (1u)
#define DFLASH_TYPE (2u)
#define INTERNAL_PFLASH_DRIVER_INDEX 0

/**
 * @brief initialize Flash device
 */
void flash_device_register(void);

/**
 * @brief initialize Flash module
 * @param driver_index -  driver index
 */
void flash_init(const uint8_t driver_index);

/**
 * @brief deinitialize Flash module
 * @param driver_index -  driver index
 */
void flash_deinit(const uint8_t driver_index);

/**
 * @brief read data from page
 *
 * @param addr -  address
 * @param len   - read length
 * @param data   - pointer to dest data
 * @param driver_index -  driver index
 *
 * @return E_OK      - success
 * @return E_NOT_OK  - otherwise
 */
std_return_t flash_read(const uint8_t driver_index, uint32_t addr, uint32_t len, uint32_t *data);

/**
 * @brief write data lager then one page
 *
 * @param addr      - flash address
 * @param len       - data length of array data
 * @param data      - data
 * @param driver_index -  driver index
 *
 * @return E_OK      - success
 * @return E_NOT_OK  - otherwise
 */
std_return_t flash_write(const uint8_t driver_index, uint32_t addr, uint32_t len, uint32_t *data);

/**
 * @brief erase logical sector starting at the sector address
 *        Upon executing this command, the BUSY fag of the corresponding bank is set
 *
 * @param addr - erase start address
 * @param driver_index -  driver index
 * @param size -  erase size - need align sector size
 *
 * @return E_OK      - success
 * @return E_NOT_OK  - otherwise
 */
std_return_t flash_erase(const uint8_t driver_index, uint32_t addr, uint32_t size);

/**
 * @brief get flash sector size
 *
 * @param flash_type      - flash type
 * @param driver_index -  driver index
 *
 * @return otherwise      - success
 * @return 0xffffffff  - failed
 */
uint32_t flash_get_sector_size(const uint8_t driver_index, uint8_t flash_type);

/**
 * @brief get flash page size
 *
 * @param flash_type      - flash type
 * @param driver_index -  driver index
 *
 * @return otherwise     - success
 * @return 0xffffffff  - failed
 */
uint32_t flash_get_page_size(const uint8_t driver_index, uint8_t flash_type);

/**
 * @brief function to register flash device
 *
 * @param p_dev device pointer
 * @param device_num device number
 *
 * @return E_OK      - success
 * @return E_NOT_OK  - otherwise
 */
std_return_t flash_register(const flash_device_t *p_dev, uint8_t device_num);

#endif
