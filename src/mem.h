#pragma once
#include <stdint.h>
#include <vector>

namespace rvsim {

/**
 * @brief Memory Device class
 */
class MemDevice
{
public:
    virtual uint64_t get_base_addr() = 0;
    virtual uint64_t get_size() = 0;
    virtual void read(uint8_t *data, uint64_t addr, uint64_t size) = 0;
    virtual void write(uint8_t *data, uint64_t addr, uint64_t size) = 0;
};


class Memory: public MemDevice
{
public:
    /**
     * @brief Construct a new Memory object
     * 
     * @param base_addr base address
     * @param size size in bytes
     * @param write_protect write protect
     */
    Memory(uint64_t base_addr, uint64_t size, bool write_protect);

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
    void read(uint8_t *data, uint64_t addr, uint64_t size);

    /**
     * @brief store bytes
     * 
     * @param buf byte buffer 
     * @param start_addr starting address
     * @param buf_sz buffer size
     * @throws char* invalid address exception
     */
    void write(uint8_t *data, uint64_t addr, uint64_t size);

    /**
     * @brief Get memory size
     * @return uint32_t size
     */
    uint64_t get_size();

    /**
     * @brief Get the base addr of memory block
     * 
     * @return uint32_t 
     */
    uint64_t get_base_addr();

private:
    uint8_t * mem_;
    bool is_write_protected_;
    uint64_t base_addr_;
    uint64_t size_;

};


class MMU
{
public:
    void attach(MemDevice *m);

    MemDevice * lookup(uint64_t addr);
    void read(uint8_t *data, uint64_t addr, uint64_t size);
    void write(uint8_t *data, uint64_t addr, uint64_t size);

private:
    std::vector<MemDevice *> mems_;
};


} // namespace rvsim