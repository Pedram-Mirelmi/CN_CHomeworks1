#pragma once

#include <memory>
#include <cstring>

using std::shared_ptr;

class ISerializable : public std::enable_shared_from_this<ISerializable>
{
public:
    virtual void deserialize(char* buffer) = 0;

    virtual void serialize(char* buffer) const = 0;

    virtual uint32_t calculateNeededSizeForThis() const = 0;

    template<typename primitive_t>
        static void serializePrimitiiveType(char*& buffer, primitive_t primitive_value, bool updateBuffer = true)
        {
            *(reinterpret_cast<primitive_t*>(buffer)) = primitive_value;
            if(updateBuffer)
                buffer += sizeof(primitive_t);
        }


        template<typename primitive_t>
        static void deserializePrimitiveType(char*& buffer, primitive_t& primitive, bool updateBuffer = true)
        {
            primitive = *(reinterpret_cast<primitive_t*>(buffer));
            if(updateBuffer)
                buffer += sizeof(primitive_t);
        }


        static void serializeByteArray(char*& buffer, const char* byteArray, uint32_t size, bool updateBuffer = true)
        {
            memcpy(buffer, byteArray, size);
            if(updateBuffer)
                buffer += size;
        }

        static void deserializeByteArray(char*& buffer, char* byteArray, uint32_t size, bool updateBuffer = true)
        {
            memcpy(byteArray, buffer, size);
            if(updateBuffer)
                buffer += size;
        }
};
