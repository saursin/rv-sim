#pragma once
#include <stdint.h>

class Memory
{
public:
    /**
     * @brief Construct a new Memory object
     * 
     * @param base_addr base address
     * @param size size in bytes
     * @param write_protect write protect
     */
    Memory(unsigned base_addr, unsigned size, bool write_protect);

    /**
     * @brief Destroy the Memory object
     */
    ~Memory();

    /**
     * @brief check if memory is write protected
     * @return true 
     * @return false 
     */
    bool is_write_protected();

    /**
     * @brief Modify write protect permission
     * @param protect
     */
    void set_write_protect(bool protect);

    /**
     * @brief Erase memory
     * 
     * @param erase_val value to be written upon erase operation (default: 0)
     */
    void erase(uint8_t erase_val);

    /**
     * @brief fetch bytes
     * 
     * @param buf byte buffer 
     * @param start_addr starting address
     * @param buf_sz buffer size
     * @throws char* invalid address exception
     */
    void fetch(uint8_t *buf, const uint32_t start_addr, const uint32_t buf_sz);

    /**
     * @brief store bytes
     * 
     * @param buf byte buffer 
     * @param start_addr starting address
     * @param buf_sz buffer size
     * @throws char* invalid address exception
     */
    void store(uint8_t *buf, const uint32_t start_addr, const uint32_t buf_sz);

    /**
     * @brief Get memory size
     * @return uint32_t size
     */
    uint32_t get_size();

    /**
     * @brief Get the base addr of memory block
     * 
     * @return uint32_t 
     */
    uint32_t get_base_addr();

private:
    uint8_t * mem_;
    bool is_write_protected_;
    uint32_t base_addr_;
    uint32_t size_;

};