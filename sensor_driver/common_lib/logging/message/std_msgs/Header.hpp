/** THIS IS AN AUTOMATICALLY GENERATED FILE.
 *  DO NOT MODIFY BY HAND!!
 *
 *  Generated by zcm-gen
 **/

#include <zcm/zcm_coretypes.h>

#ifndef __std_msgs_Header_hpp__
#define __std_msgs_Header_hpp__

#include <string>


namespace std_msgs {
class Header
{
    public:
        int64_t    seq;

        int64_t    stamp;

        std::string frame_id;

    public:
        /**
         * Destructs a message properly if anything inherits from it
        */
        virtual ~Header() {}

        /**
         * Encode a message into binary form.
         *
         * @param buf The output buffer.
         * @param offset Encoding starts at thie byte offset into @p buf.
         * @param maxlen Maximum number of bytes to write.  This should generally be
         *  equal to getEncodedSize().
         * @return The number of bytes encoded, or <0 on error.
         */
        inline int encode(void* buf, uint32_t offset, uint32_t maxlen) const;

        /**
         * Check how many bytes are required to encode this message.
         */
        inline uint32_t getEncodedSize() const;

        /**
         * Decode a message from binary form into this instance.
         *
         * @param buf The buffer containing the encoded message.
         * @param offset The byte offset into @p buf where the encoded message starts.
         * @param maxlen The maximum number of bytes to read while decoding.
         * @return The number of bytes decoded, or <0 if an error occured.
         */
        inline int decode(const void* buf, uint32_t offset, uint32_t maxlen);

        /**
         * Retrieve the 64-bit fingerprint identifying the structure of the message.
         * Note that the fingerprint is the same for all instances of the same
         * message type, and is a fingerprint on the message type definition, not on
         * the message contents.
         */
        inline static int64_t getHash();

        /**
         * Returns "Header"
         */
        inline static const char* getTypeName();

        // ZCM support functions. Users should not call these
        inline int      _encodeNoHash(void* buf, uint32_t offset, uint32_t maxlen) const;
        inline uint32_t _getEncodedSizeNoHash() const;
        inline int      _decodeNoHash(const void* buf, uint32_t offset, uint32_t maxlen);
        inline static uint64_t _computeHash(const __zcm_hash_ptr* p);
};

int Header::encode(void* buf, uint32_t offset, uint32_t maxlen) const
{
    uint32_t pos = 0;
    int thislen;
    int64_t hash = (int64_t)getHash();

    thislen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if(thislen < 0) return thislen; else pos += thislen;

    thislen = this->_encodeNoHash(buf, offset + pos, maxlen - pos);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int Header::decode(const void* buf, uint32_t offset, uint32_t maxlen)
{
    uint32_t pos = 0;
    int thislen;

    int64_t msg_hash;
    thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &msg_hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;
    if (msg_hash != getHash()) return -1;

    thislen = this->_decodeNoHash(buf, offset + pos, maxlen - pos);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

uint32_t Header::getEncodedSize() const
{
    return 8 + _getEncodedSizeNoHash();
}

int64_t Header::getHash()
{
    static int64_t hash = _computeHash(NULL);
    return hash;
}

const char* Header::getTypeName()
{
    return "Header";
}

int Header::_encodeNoHash(void* buf, uint32_t offset, uint32_t maxlen) const
{
    uint32_t pos_byte = 0;
    int thislen;

    thislen = __int64_t_encode_array(buf, offset + pos_byte, maxlen - pos_byte, &this->seq, 1);
    if(thislen < 0) return thislen; else pos_byte += thislen;

    thislen = __int64_t_encode_array(buf, offset + pos_byte, maxlen - pos_byte, &this->stamp, 1);
    if(thislen < 0) return thislen; else pos_byte += thislen;

    char* frame_id_cstr = (char*) this->frame_id.c_str();
    thislen = __string_encode_array(buf, offset + pos_byte, maxlen - pos_byte, &frame_id_cstr, 1);
    if(thislen < 0) return thislen; else pos_byte += thislen;

    return pos_byte;
}

int Header::_decodeNoHash(const void* buf, uint32_t offset, uint32_t maxlen)
{
    uint32_t pos_byte = 0;
    int thislen;

    thislen = __int64_t_decode_array(buf, offset + pos_byte, maxlen - pos_byte, &this->seq, 1);
    if(thislen < 0) return thislen; else pos_byte += thislen;

    thislen = __int64_t_decode_array(buf, offset + pos_byte, maxlen - pos_byte, &this->stamp, 1);
    if(thislen < 0) return thislen; else pos_byte += thislen;

    int32_t __frame_id_len__;
    thislen = __int32_t_decode_array(buf, offset + pos_byte, maxlen - pos_byte, &__frame_id_len__, 1);
    if(thislen < 0) return thislen; else pos_byte += thislen;
    if((uint32_t)__frame_id_len__ > maxlen - pos_byte) return -1;
    this->frame_id.assign(((const char*)buf) + offset + pos_byte, __frame_id_len__ - ZCM_CORETYPES_INT8_NUM_BYTES_ON_BUS);
    pos_byte += __frame_id_len__;

    return pos_byte;
}

uint32_t Header::_getEncodedSizeNoHash() const
{
    uint32_t enc_size = 0;
    enc_size += __int64_t_encoded_array_size(NULL, 1);
    enc_size += __int64_t_encoded_array_size(NULL, 1);
    enc_size += this->frame_id.size() + ZCM_CORETYPES_INT32_NUM_BYTES_ON_BUS + ZCM_CORETYPES_INT8_NUM_BYTES_ON_BUS;
    return enc_size;
}

uint64_t Header::_computeHash(const __zcm_hash_ptr*)
{
    uint64_t hash = (uint64_t)0xc86bbcf42728c594LL;
    return (hash<<1) + ((hash>>63)&1);
}

}

#endif
