/**
 * An Arduino EEPROM fake addapted from PaulStoffregen/EEPROM https://github.com/PaulStoffregen/EEPROM/blob/master/EEPROM.h
 *
 * Author: David Cedar
 * Email: david@epicecu.com
 * URL: https://github.com/epicecu/eeprom-fake
 */
#ifndef EEPROM_H
#define EEPROM_H

#include <inttypes.h>
#include <cstring>

#if defined(__has_include) && __has_include(<type_traits>)
#include <type_traits>
#endif

#if !defined(SET_EEPROM_SIZE)
#define SET_EEPROM_SIZE 1024
#endif

static uint8_t eeprom_data[SET_EEPROM_SIZE] = { 0 };

static uint8_t eeprom_read_byte(uint8_t* index)
{
    uint8_t indexValue = 0;
    if (index != 0) {
        indexValue = *index;
    }
    return eeprom_data[indexValue];
}

static void eeprom_write_byte(uint8_t* index, uint8_t value)
{
    uint8_t indexValue = 0;
    if (index != 0) {
        indexValue = *index;
    }
    eeprom_data[indexValue] = value;
}

static void eeprom_write_block(const uint8_t* data, uint8_t index, uint8_t size)
{
    memcpy(eeprom_data+index, data, size);
}

static void eeprom_read_block(uint8_t* data, uint8_t index, uint8_t size)
{
    memcpy(data, eeprom_data+index, size);
}

/***
    EERef class.
    This object references an EEPROM cell.
    Its purpose is to mimic a typical byte of RAM, however its storage is the EEPROM.
    This class has an overhead of two bytes, similar to storing a pointer to an EEPROM cell.
***/

struct EERef {

    EERef(const int index)
        : index(index) {}

    //Access/read members.
    uint8_t operator*() const { return eeprom_read_byte((uint8_t*)index); }
    operator const uint8_t() const { return **this; }

    //Assignment/write members.
    EERef& operator=(const EERef& ref) { return *this = *ref; }
    EERef& operator=(uint8_t in) { return eeprom_write_byte((uint8_t*)index, in), * this; }
    EERef& operator +=(uint8_t in) { return *this = **this + in; }
    EERef& operator -=(uint8_t in) { return *this = **this - in; }
    EERef& operator *=(uint8_t in) { return *this = **this * in; }
    EERef& operator /=(uint8_t in) { return *this = **this / in; }
    EERef& operator ^=(uint8_t in) { return *this = **this ^ in; }
    EERef& operator %=(uint8_t in) { return *this = **this % in; }
    EERef& operator &=(uint8_t in) { return *this = **this & in; }
    EERef& operator |=(uint8_t in) { return *this = **this | in; }
    EERef& operator <<=(uint8_t in) { return *this = **this << in; }
    EERef& operator >>=(uint8_t in) { return *this = **this >> in; }

    EERef& update(uint8_t in) { return  in != *this ? *this = in : *this; }

    /** Prefix increment/decrement **/
    EERef& operator++() { return *this += 1; }
    EERef& operator--() { return *this -= 1; }

    /** Postfix increment/decrement **/
    uint8_t operator++ (int) {
        uint8_t ret = **this;
        return ++(*this), ret;
    }

    uint8_t operator-- (int) {
        uint8_t ret = **this;
        return --(*this), ret;
    }

    int index; //Index of current EEPROM cell.
};

/***
    EEPtr class.
    This object is a bidirectional pointer to EEPROM cells represented by EERef objects.
    Just like a normal pointer type, this can be dereferenced and repositioned using
    increment/decrement operators.
***/

struct EEPtr {

    EEPtr(const int index)
        : index(index) {}

    operator const int() const { return index; }
    EEPtr& operator=(int in) { return index = in, *this; }

    //Iterator functionality.
    bool operator!=(const EEPtr& ptr) { return index != ptr.index; }
    EERef operator*() { return index; }

    /** Prefix & Postfix increment/decrement **/
    EEPtr& operator++() { return ++index, * this; }
    EEPtr& operator--() { return --index, * this; }
    EEPtr operator++ (int) { return index++; }
    EEPtr operator-- (int) { return index--; }

    int index; //Index of current EEPROM cell.
};

/***
    EEPROMClass class.
    This object represents the entire EEPROM space.
    It wraps the functionality of EEPtr and EERef into a basic interface.
    This class is also 100% backwards compatible with earlier Arduino core releases.
***/

struct EEPROMClass {

    EEPROMClass() { }

    //Basic user access methods.
    EERef operator[](const int idx) { return idx; }
    uint8_t read(int idx) { return EERef(idx); }
    void write(int idx, uint8_t val) { (EERef(idx)) = val; }
    void update(int idx, uint8_t val) { EERef(idx).update(val); }

    //STL and C++11 iteration capability.
    EEPtr begin() { return 0x00; }
    EEPtr end() { return length(); } //Standards requires this to be the item after the last valid entry. The returned pointer is invalid.
    uint16_t length() { return SET_EEPROM_SIZE + 1; }

    //Functionality to 'get' and 'put' objects to and from EEPROM.
    template< typename T > T& get(int idx, T& t) {
#if defined(__has_include) && __has_include(<type_traits>)
        static_assert(std::is_trivially_copyable<T>::value, "You can not use this type with EEPROM.get"); // the code below only makes sense if you can "memcpy" T
#endif
        uint8_t* ptr = (uint8_t*)&t;
        eeprom_read_block(ptr, idx, sizeof(T));
        return t;
    }

    template< typename T > const T& put(int idx, const T& t) {
#if defined(__has_include) && __has_include(<type_traits>)
        static_assert(std::is_trivially_copyable<T>::value, "You can not use this type with EEPROM.put"); // the code below only makes sense if you can "memcpy" T
#endif
        const uint8_t* ptr = (const uint8_t*)&t;
        eeprom_write_block(ptr, idx, sizeof(T));
        return t;
    }
};


// put - Specialization for Arduino Strings -------------------------------
// to put an Arduino String to the EEPROM we copy its internal buffer
// including the trailing \0 to the eprom

#if defined(__has_include) && __has_include(<Arduino.h>)
#if defined(ENABLE_STRING_SUPPORT)

template <>
inline const String& EEPROMClass::put(int idx, const String& s)
{
    const uint8_t* ptr = (uint8_t*)s.c_str();

#ifdef __arm__
    eeprom_write_block(ptr, (void*)idx, s.length() + 1); // length() doesn't account for the trailing \0
#else
    EEPtr e = idx;
    for (int count = s.length() + 1; count; --count, ++e)
        (*e).update(*ptr++);
#endif
    return s;
}

// get - Specialization for Arduino Strings -------------------------------
// to "get" an Arduino String from the EEPROM we append chars from the EEPROM
// into it until we find the delimiting /0.
// String.append is not very efficient, code could probably be opitimized if required...

template <>
inline String& EEPROMClass::get(int idx, String& s) {
    s = "";             // just in case...
    EEPtr e = idx;

    char c = *e;        // read in bytes until we find the terminating \0
    while (c != '\0')
    {
        s.append(c);
        c = *(++e);
    }
    return s;
}

#endif
#endif


static EEPROMClass EEPROM;

#endif // EEPROM_H