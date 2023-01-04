#pragma once

#include <memory>
#include <cstring>
#include <sstream>

using std::shared_ptr;

class ISerializable : public std::enable_shared_from_this<ISerializable>
{
protected:
    using stringstream = std::stringstream;
public:
    virtual void deserialize(stringstream& stream) = 0;

    virtual void serialize(stringstream& stream) const = 0;

    virtual uint32_t calculateNeededSizeForThis() const = 0;

    template<typename primitive_t>
    static void serializePrimitiiveType(stringstream& stream, primitive_t primitive_value)
    {
        stream.write(reinterpret_cast<char*>(&primitive_value), sizeof(primitive_t));
    }


    template<typename primitive_t>
    static void deserializePrimitiveType(stringstream& stream, primitive_t& primitive)
    {
        stream.read(reinterpret_cast<char*>(&primitive), sizeof(primitive_t));
    }


    static void serializeByteArray(stringstream& stream, const char* byteArray, uint32_t size)
    {
        stream.write(byteArray, size);
    }

    static void deserializeByteArray(stringstream& stream, char* byteArray, uint32_t size)
    {
        stream.read(byteArray, size);
    }
};
