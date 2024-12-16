/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** BitConverter
*/

#ifndef BITCONVERTER_HPP_
    #define BITCONVERTER_HPP_

    #include <bitset>
    #include <iostream>
    #include <array>
    #include <unordered_map>
    #include <boost/asio.hpp>
    #include <boost/system/error_code.hpp>
    #include <cstring>
    #include <iostream>
    #include <tuple>
    #include <unordered_map>
    #include <memory>
    #include <functional>
    #include <map>
    #include <zlib.h>
    #include "NetworkReponse.hpp"
    #include "Using.hpp"
    #include "UUIDManager.hpp"

    #define ASIO boost::asio
    #define IP boost::asio::ip
    #define UDP boost::asio::ip::udp
    #define TCP boost::asio::ip::tcp
    #define SYSTEM boost::system
    #define MAX_SHORT 65535
    #define MAX_STRING 256
    #define MAX_ARGS 4
    #define MAX_PORT 4000

USING(
    ecs::cmp::UUID
);

#ifdef _MSC_VER
    #define PACKED_STRUCT(name) \
        __pragma(pack(push, 1)) struct name __pragma(pack(pop))
#else
    #define PACKED_STRUCT(name) struct __attribute__((packed)) name
#endif


namespace Network {
    PACKED_STRUCT(CommandInstruction) {
        unsigned char actionID;
        unsigned short argsTypes;
        unsigned short bufferSize;
    };

    class BitConverter {
    public:
        static BitConverter &getInstance()
        {
            static thread_local BitConverter instance;
            instance.clear();
            return instance;
        }

        enum class ArgsType: int {
            NUMBER  = 0,  // 0000
            CHAR    = 1,  // 0001
            STRING  = 2,  // 0010
            FLOAT   = 3,  // 0011
            VEC2    = 4,  // 0100
            VEC3    = 5,  // 0101
            UUID    = 6,  // 0110
            NOTHING = 15, // 1111
        };

        PACKED_STRUCT(Vec2) {
            double x;
            double y;
        };

        PACKED_STRUCT(Vec3) {
            double x;
            double y;
            double z;
        };

        BitConverter &setID(int id)
        {
            _actionID = static_cast<unsigned char>(id);
            return static_cast<BitConverter&>(*this);
        }

        BitConverter &addNumber(int number)
        {
            if (!checkSpace(sizeof(int), "Number"))
                return static_cast<BitConverter&>(*this);
            if (_argsNumber == 4) {
                std::cerr << "Too many args for this message" << std::endl;
                return static_cast<BitConverter&>(*this);
            }
            _types.at(_argsNumber) = ArgsType::NUMBER;

            if (!std::memcpy(&_binary[_offset], &number, sizeof(int)))
                _error = true;
            _offset += sizeof(int);
            _argsNumber++;
            return static_cast<BitConverter&>(*this);
        }

        static int getNumber(const std::vector<char> &binary, size_t &offset)
        {
            if (offset + sizeof(int) > binary.size())
                throw std::runtime_error("No space to read a number");
            int num = 0;
            std::memcpy(&num, &binary[offset], sizeof(int));
            offset += sizeof(int);
            return num;
        }

        BitConverter &addString(std::string str)
        {
            if (!checkSpace(MAX_STRING, "String"))
                return static_cast<BitConverter&>(*this);
            if (_argsNumber == 4) {
                std::cerr << "Too many args for this message" << std::endl;
                return static_cast<BitConverter&>(*this);
            }
            _types.at(_argsNumber) = ArgsType::STRING;

            char buffer[MAX_STRING] = {0};
            std::memcpy(buffer, str.c_str(), str.size());

            if (!std::memcpy(&_binary[_offset], buffer, MAX_STRING))
                _error = true;
            _offset += MAX_STRING;
            _argsNumber++;
            return static_cast<BitConverter&>(*this);
        }

        static std::string getString(const std::vector<char> &binary, size_t &offset)
        {
            if (offset + MAX_STRING > binary.size())
                throw std::runtime_error("No space to read a string");
            char tempStr[MAX_STRING + 1] = {0};
            std::memcpy(tempStr, &binary[offset], MAX_STRING);
            std::string str(tempStr);
            offset += MAX_STRING;
            return str;
        }

        BitConverter &addChar(unsigned char c)
        {
            if (!checkSpace(sizeof(unsigned char), "Char"))
                return static_cast<BitConverter&>(*this);
            if (_argsNumber == 4) {
                std::cerr << "Too many args for this message" << std::endl;
                return static_cast<BitConverter&>(*this);
            }
            _types.at(_argsNumber) = ArgsType::CHAR;

            if (!std::memcpy(&_binary[_offset], &c, sizeof(unsigned char)))
                _error = true;
            _offset += sizeof(unsigned char);
            _argsNumber++;
            return static_cast<BitConverter&>(*this);
        }

        static unsigned char getChar(const std::vector<char> &binary, size_t &offset)
        {
            if (offset + sizeof(unsigned char) > binary.size())
                throw std::runtime_error("No space to read a char");
            char c = 0;
            std::memcpy(&c, &binary[offset], sizeof(unsigned char));
            offset += sizeof(unsigned char);
            return c;
        }

        BitConverter &addFloat(float value)
        {
            if (!checkSpace(sizeof(float), "Float"))
                return static_cast<BitConverter&>(*this);
            if (_argsNumber == 4) {
                std::cerr << "Too many args for this message" << std::endl;
                return static_cast<BitConverter&>(*this);
            }
            _types.at(_argsNumber) = ArgsType::FLOAT;

            if (!std::memcpy(&_binary[_offset], &value, sizeof(float)))
                _error = true;
            _offset += sizeof(float);
            _argsNumber++;
            return static_cast<BitConverter&>(*this);
        }

        static float getFloat(const std::vector<char> &binary, size_t &offset)
        {
            if (offset + sizeof(float) > binary.size())
                throw std::runtime_error("No space to read a float");
            float value = 0;
            std::memcpy(&value, &binary[offset], sizeof(float));
            offset += sizeof(float);
            return value;
        }

        BitConverter &addVec2(Vec2 vec)
        {
            if (!checkSpace(sizeof(Vec2), "Vec2"))
                return static_cast<BitConverter&>(*this);
            if (_argsNumber == 4) {
                std::cerr << "Too many args for this message" << std::endl;
                return static_cast<BitConverter&>(*this);
            }
            _types.at(_argsNumber) = ArgsType::VEC2;

            if (!std::memcpy(&_binary[_offset], &vec, sizeof(Vec2)))
                _error = true;
            _offset += sizeof(Vec2);
            _argsNumber++;
            return static_cast<BitConverter&>(*this);
        }

        static Vec2 getVec2(const std::vector<char> &binary, size_t &offset)
        {
            if (offset + sizeof(Vec2) > binary.size())
                throw std::runtime_error("No space to read a Vec2");
            Vec2 vec = {0};
            std::memcpy(&vec, &binary[offset], sizeof(Vec2));
            offset += sizeof(Vec2);
            return vec;
        }

        BitConverter &addVec3(Vec3 vec)
        {
            if (!checkSpace(sizeof(Vec3), "Vec3"))
                return static_cast<BitConverter&>(*this);
            if (_argsNumber == 4) {
                std::cerr << "Too many args for this message" << std::endl;
                return static_cast<BitConverter&>(*this);
            }
            _types.at(_argsNumber) = ArgsType::VEC3;

            if (!std::memcpy(&_binary[_offset], &vec, sizeof(Vec3)))
                _error = true;
            _offset += sizeof(Vec3);
            _argsNumber++;
            return static_cast<BitConverter&>(*this);
        }

        static Vec3 getVec3(const std::vector<char> &binary, size_t &offset)
        {
            if (offset + sizeof(Vec3) > binary.size())
                throw std::runtime_error("No space to read a Vec3");
            Vec3 vec = {0};
            std::memcpy(&vec, &binary[offset], sizeof(Vec3));
            offset += sizeof(Vec3);
            return vec;
        }

        BitConverter &addUUID(const UUID &uuid)
        {
            if (!checkSpace(sizeof(UUID), "UUID"))
                return static_cast<BitConverter&>(*this);
            if (_argsNumber == 4) {
                std::cerr << "Too many args for this message" << std::endl;
                return static_cast<BitConverter&>(*this);
            }
            _types.at(_argsNumber) = ArgsType::UUID;

            if (!std::memcpy(&_binary[_offset], uuid.data(), sizeof(UUID)))
                _error = true;
            _offset += sizeof(UUID);
            _argsNumber++;
            return static_cast<BitConverter&>(*this);
        }

        static UUID getUUID(const std::vector<char>& binary, size_t &offset)
        {
            if (offset + sizeof(UUID) > binary.size())
                throw std::runtime_error("No space to read a UUID");

            UUID uuid{};
            std::memcpy(uuid.data(), &binary[offset], sizeof(UUID));
            offset += sizeof(UUID);
            return uuid;
        }

        BitConverter &compactMessage(std::vector<char> &buffer)
        {
            std::vector<char> tmp = {};
            CommandInstruction command;
            command.actionID = _actionID;
            command.bufferSize = _offset;
            command.argsTypes = bitsToShort(argsToBits(_types));
            const char *interpret = reinterpret_cast<const char*>(&command);
            tmp.insert(tmp.end(), interpret, interpret + sizeof(CommandInstruction));
            buffer.insert(buffer.end(), interpret, interpret + sizeof(CommandInstruction));
            buffer.insert(buffer.end(), _binary.begin(), _binary.begin() + _offset);
            clear();
            return static_cast<BitConverter&>(*this);
        }

        static std::vector<char> zip(const std::vector<char> &buffer)
        {
            uLongf compressedSize = compressBound(buffer.size());
            uLongf originalSize = buffer.size();

            std::vector<char> compressedBuffer(2 * sizeof(uLongf) + compressedSize);

            if (compress(reinterpret_cast<Bytef*>(compressedBuffer.data() + 2 * sizeof(uLongf)), &compressedSize,
                        reinterpret_cast<const Bytef*>(buffer.data()), buffer.size()) != Z_OK) {
                throw std::runtime_error("Compression failed");
            }

            compressedBuffer.resize(2 * sizeof(uLongf) + compressedSize);

            std::memcpy(compressedBuffer.data(), &compressedSize, sizeof(uLongf));
            std::memcpy(compressedBuffer.data() + sizeof(uLongf), &originalSize, sizeof(uLongf));

            return compressedBuffer;
        }

        static std::vector<char> unzip(const std::shared_ptr<std::vector<char>> &compressedBuffer)
        {
            uLongf compressedSize{};
            uLongf originalSize{};
            std::memcpy(&compressedSize, compressedBuffer->data(), sizeof(uLongf));
            std::memcpy(&originalSize, compressedBuffer->data() + sizeof(uLongf), sizeof(uLongf));

            std::vector<char> decompressedBuffer(originalSize);

            if (uncompress(reinterpret_cast<Bytef*>(decompressedBuffer.data()), &originalSize,
                        reinterpret_cast<const Bytef*>(compressedBuffer->data() + 2 * sizeof(uLongf)),
                        compressedSize) != Z_OK) {
                throw std::runtime_error("Decompression failed");
            }

            return decompressedBuffer;
        }

    private:
        BitConverter() :
            _error(false)
        {
            clear();
        }

        std::bitset<16> argsToBits(std::array<ArgsType, MAX_ARGS> argsType)
        {
            std::array<int, 4> binary = {
                static_cast<int>(argsType[0]),
                static_cast<int>(argsType[1]),
                static_cast<int>(argsType[2]),
                static_cast<int>(argsType[3])
            };
            unsigned int combined = (binary[0] << 12) | (binary[1] << 8) | (binary[2] << 4) | binary[3];

            std::bitset<16> result(combined);
            return result;
        }

        std::array<ArgsType, MAX_ARGS> bitsToArgs(std::bitset<16> bits)
        {
            unsigned int combined = bits.to_ulong();
            std::array<unsigned int, MAX_ARGS> values{};
            for (int i = 0; i < MAX_ARGS; i++)
                values[(MAX_ARGS - 1) - i] = (combined >> i * MAX_ARGS) & 0xF;

            std::array<ArgsType, MAX_ARGS> args{};
            for (int i = 0; i < MAX_ARGS; i++) {
                if (binaryToType.find(values[i]) != binaryToType.end())
                    args[i] = binaryToType[values[i]];
                else
                    args[i] = ArgsType::NOTHING;
            }
            return args;
        }

        std::bitset<16> shortToBits(short value)
        {
            std::bitset<16> bits(value);
            return bits;
        }

        short bitsToShort(std::bitset<16> bits)
        {
            unsigned short value = static_cast<unsigned short>(bits.to_ulong());
            return value;
        }

        bool errorBitConverter()
        {
            return _error;
        }

        bool checkSpace(size_t size, std::string type)
        {
            if (_offset + size > _binary.size()) {
                std::cerr << "No more space to add a " << type << "." << std::endl;
                return false;
            }
            return true;
        }

        std::unordered_map<int, ArgsType> binaryToType = {
            {0,   ArgsType::NUMBER},
            {1,     ArgsType::CHAR},
            {2,   ArgsType::STRING},
            {3,    ArgsType::FLOAT},
            {4,     ArgsType::VEC2},
            {5,     ArgsType::VEC3},
            {6,     ArgsType::UUID},
            {15, ArgsType::NOTHING},
        };

        void clear()
        {
            _error = false;
            _actionID = 0;
            _argsNumber = 0;
            _offset = 0;
            _types.fill(ArgsType::NOTHING);
            _binary.fill(0);
        }

        unsigned char _actionID;
        bool _error;
        size_t _offset;
        int _argsNumber;
        std::array<BitConverter::ArgsType, MAX_ARGS> _types;
        std::array<char, MAX_SHORT> _binary;
    };
}
#endif /* !BITCONVERTER_HPP_ */
