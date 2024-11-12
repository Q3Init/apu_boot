/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "bootcheck.h"
#include "flash.h"
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
typedef void (*pFunction)(void);
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
static void JumpToApp_Function(void);
static void BootScanAppEntry(void);
/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
static pFunction Jump_To_Application;
static uint32 bootEnterAppFlag __attribute__((at(0x2001FFE0))) = ENTERED_WAY_POWERON;
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

/**
* BootSelectInit
*
* @return   none
*
* @brief  Check whether to jump according to the app flag
*/
void BootSelectInit(void)
{
   if (bootEnterAppFlag == APP_JUMP_TO_BOOT_REQ1) {
       bootEnterAppFlag = 0;
        (void)rte_init();
   } else {
       bootEnterAppFlag = 0;
       BootScanAppEntry();
   }
}

/**
* read_integrity_app_flag
*
 * @param flag: integrity app flag
 * @return E_OK      - success
 * @return E_NOK     - fail
*
* @brief  read integrity app flag
*/
std_return_t read_integrity_app_flag(uint32 *flag)
{
    std_return_t ret = E_NOK;
    ret = flash_read(INTERNAL_PFLASH_DRIVER_INDEX,INTEGRITY_APP_FLAG_ADDRESS,sizeof(uint32),flag);
    return ret;
}

/**
* earse_integrity_app_flag
*
 * @return E_OK      - success
 * @return E_NOK     - fail
*
* @brief  ease integrity app flag
*/
std_return_t earse_integrity_app_flag(void)
{
    std_return_t ret = E_NOK;
    uint32 sector_size = flash_get_sector_size(INTERNAL_PFLASH_DRIVER_INDEX,PFLASH_TYPE);
    ret = flash_erase(INTERNAL_PFLASH_DRIVER_INDEX,INTEGRITY_APP_FLAG_ADDRESS,sector_size);
    return ret;
}

/**
* write_integrity_app_flag
*
 * @param flag: integrity app flag
 * @return E_OK      - success
 * @return E_NOK     - fail
*
* @brief  read integrity app flag
*/
std_return_t write_integrity_app_flag(uint32 *flag)
{
    std_return_t ret = E_NOK;
    ret = flash_write(INTERNAL_PFLASH_DRIVER_INDEX,INTEGRITY_APP_FLAG_ADDRESS,sizeof(uint32),flag);
    return ret;
}

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
/**
* BootScanAppEntry
*
* @return   none
*
* @brief  Check app crc and integrity
*/
static void BootScanAppEntry(void)
{
    uint32 integrity_app_flag = 0;
    std_return_t ret = read_integrity_app_flag(&integrity_app_flag);
    if (ret == E_OK) {
        if (integrity_app_flag == INTEGRITY_APP_FLAG_OK) {
            JumpToApp_Function();
        }
    }
}

/**
* JumpToApp_Function
*
* @return   none
*
* @brief  Jump to the app slot
*/
static void JumpToApp_Function(void)
{
    uint32 jumpAddress;
    uint32 appAddress = flashSlotInfoTable[FLASH_SLOTA_APP_A].startAddr;
    
    __DISENBLE_IRQ();
    if (((*(__IO uint32 *)appAddress) & 0x2FFE0000) == 0x20000000) {
        jumpAddress = *(__IO uint32 *)(appAddress + 4); /* Jump to user application */ 
        Jump_To_Application = (pFunction)jumpAddress; /* Initialize user application's Stack Pointer */
        __set_MSP(*(__IO uint32*)appAddress);
        Jump_To_Application();  /* jump to app */
    } else {
        __ENABLE_IRQ(); /* enable global interrupt and ramain at boot */
    }
    
}
