#include "mem.h"
#include <iostream>
#include <fstream>

using namespace rvsim;

Memory::Memory(uint64_t base_addr, uint64_t size, bool write_protect=false):
    is_write_protected_(write_protect),
    base_addr_(base_addr),
    size_(size)
{
    // Allocate memory
    if(!(mem_ = new uint8_t[size]))
        throw "Memory allocation failed";
}


Memory::~Memory()
{
    // Deallocate memory
    delete [] mem_;
    size_ = 0;
}


bool Memory::is_write_protected()
{
    return is_write_protected_;
}


void Memory::set_write_protect(bool protect)
{
    is_write_protected_ = protect;
}


void Memory::erase(uint8_t erase_val=0x00)
{
    bool was_write_protected = is_write_protected_;
    is_write_protected_ = false;
    for(unsigned i; i<size_; i++)
        mem_[i] = erase_val;
    is_write_protected_ = was_write_protected;
}


void Memory::read(uint8_t *data, uint64_t addr, uint64_t size)
{
    // exception message buffer
    char except_buf[50];

    uint32_t rel_start_addr = addr - base_addr_;    // relative to current memory block

    for (uint32_t i = 0; i < size; i++)
    {
        // get index
        uint32_t indx = rel_start_addr + i;
        
        if (!(indx < size_))    // check bounds
        {
            sprintf(except_buf, "invalid fetch address [0x%08lx]", addr + i);
            throw except_buf;
        }

        // copy from mem
        data[i] = mem_[indx];
    }
}


void Memory::write(uint8_t *data, uint64_t addr, uint64_t size)
{
    if(is_write_protected_)
    {
        char except_buf[70];
        sprintf(except_buf, "Attempted to store in a write-protected memory @ address [0x%08lx]", addr);
        throw except_buf;
    }

    // exception message buffer
    char except_buf[50];

    uint32_t rel_start_addr = addr - base_addr_;    // relative to current memory block

    for (uint32_t i = 0; i < size; i++)
    {
        // get index
        uint32_t indx = rel_start_addr + i;
        
        if (!(indx < size_))    // check bounds
        {
            sprintf(except_buf, "invalid store address [0x%08lx]", addr + i);
            throw except_buf;
        }

        // copy to mem
        mem_[indx] = data[i];
    }
}


uint64_t Memory::get_size()
{ 
    return size_;
}


uint64_t Memory::get_base_addr()
{
    return base_addr_;
}

void Memory::loadBinImage(const char* filename, uint64_t destination)
{
  std::ifstream ifs(filename);
  if (!ifs) {
    std::cout << "error: " << filename << " not found" << std::endl;
  }

  ifs.seekg(0, ifs.end);
  size_t size = ifs.tellg();
  std::vector<uint8_t> content(size);
  ifs.seekg(0, ifs.beg);
  ifs.read((char*)content.data(), size);

  this->erase();
  this->write(content.data(), destination, size);
}


/////////////////////////////////////////////////////////////////////
// MMU

void MMU::attach(MemDevice *m)
{
    /* perform necessary checks here */
    mems_.push_back(m);
}


MemDevice * MMU::lookup(uint64_t addr)
{
    for(unsigned i=0; i< mems_.size(); i++)
    {
        MemDevice *md = mems_[i];
        if(addr >= md->get_base_addr() && addr <= (md->get_base_addr()+md->get_size()-1))
            return md;
    }
    return nullptr;
}


void MMU::read(uint8_t *data, uint64_t addr, uint64_t size)
{
    MemDevice *m = lookup(addr);
    if(m)
        m->read(data, addr, size);
    else
        throw "Read request from unknown memory";
}


void MMU::write(uint8_t *data, uint64_t addr, uint64_t size)
{
    MemDevice *m = lookup(addr);
    if(m)
        m->write(data, addr, size);
    else
        throw "Write request to unknown memory";
}