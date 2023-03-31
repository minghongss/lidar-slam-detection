// THIS IS AN AUTOMATICALLY GENERATED FILE.
// DO NOT MODIFY BY HAND!!
//
// Generated by zcm-gen

#include <stdint.h>
#include <stdlib.h>
#include <zcm/zcm_coretypes.h>
#include <zcm/zcm.h>

#ifndef _std_msgs_Int64_h
#define _std_msgs_Int64_h

#ifdef __cplusplus
extern "C" {
#endif

#define STD_MSGS_INT64_IS_LITTLE_ENDIAN 0
typedef struct _std_msgs_Int64 std_msgs_Int64;
struct _std_msgs_Int64
{
    int64_t    data;
};

/**
 * Create a deep copy of a std_msgs_Int64.
 * When no longer needed, destroy it with std_msgs_Int64_destroy()
 */
std_msgs_Int64* std_msgs_Int64_copy(const std_msgs_Int64* to_copy);

/**
 * Destroy an instance of std_msgs_Int64 created by std_msgs_Int64_copy()
 */
void std_msgs_Int64_destroy(std_msgs_Int64* to_destroy);

/**
 * Identifies a single subscription.  This is an opaque data type.
 */
typedef struct _std_msgs_Int64_subscription_t std_msgs_Int64_subscription_t;

/**
 * Prototype for a callback function invoked when a message of type
 * std_msgs_Int64 is received.
 */
typedef void(*std_msgs_Int64_handler_t)(const zcm_recv_buf_t* rbuf,
             const char* channel, const std_msgs_Int64* msg, void* userdata);

/**
 * Publish a message of type std_msgs_Int64 using ZCM.
 *
 * @param zcm The ZCM instance to publish with.
 * @param channel The channel to publish on.
 * @param msg The message to publish.
 * @return 0 on success, <0 on error.  Success means ZCM has transferred
 * responsibility of the message data to the OS.
 */
int std_msgs_Int64_publish(zcm_t* zcm, const char* channel, const std_msgs_Int64* msg);

/**
 * Subscribe to messages of type std_msgs_Int64 using ZCM.
 *
 * @param zcm The ZCM instance to subscribe with.
 * @param channel The channel to subscribe to.
 * @param handler The callback function invoked by ZCM when a message is received.
 *                This function is invoked by ZCM during calls to zcm_handle() and
 *                zcm_handle_timeout().
 * @param userdata An opaque pointer passed to @p handler when it is invoked.
 * @return pointer to subscription type, NULL if failure. Must clean up
 *         dynamic memory by passing the pointer to std_msgs_Int64_unsubscribe.
 */
std_msgs_Int64_subscription_t* std_msgs_Int64_subscribe(zcm_t* zcm, const char* channel, std_msgs_Int64_handler_t handler, void* userdata);

/**
 * Removes and destroys a subscription created by std_msgs_Int64_subscribe()
 */
int std_msgs_Int64_unsubscribe(zcm_t* zcm, std_msgs_Int64_subscription_t* hid);
/**
 * Encode a message of type std_msgs_Int64 into binary form.
 *
 * @param buf The output buffer.
 * @param offset Encoding starts at this byte offset into @p buf.
 * @param maxlen Maximum number of bytes to write.  This should generally
 *               be equal to std_msgs_Int64_encoded_size().
 * @param msg The message to encode.
 * @return The number of bytes encoded, or <0 if an error occured.
 */
int std_msgs_Int64_encode(void* buf, uint32_t offset, uint32_t maxlen, const std_msgs_Int64* p);

/**
 * Decode a message of type std_msgs_Int64 from binary form.
 * When decoding messages containing strings or variable-length arrays, this
 * function may allocate memory.  When finished with the decoded message,
 * release allocated resources with std_msgs_Int64_decode_cleanup().
 *
 * @param buf The buffer containing the encoded message
 * @param offset The byte offset into @p buf where the encoded message starts.
 * @param maxlen The maximum number of bytes to read while decoding.
 * @param msg Output parameter where the decoded message is stored
 * @return The number of bytes decoded, or <0 if an error occured.
 */
int std_msgs_Int64_decode(const void* buf, uint32_t offset, uint32_t maxlen, std_msgs_Int64* msg);

/**
 * Release resources allocated by std_msgs_Int64_decode()
 * @return 0
 */
int std_msgs_Int64_decode_cleanup(std_msgs_Int64* p);

/**
 * Check how many bytes are required to encode a message of type std_msgs_Int64
 */
uint32_t std_msgs_Int64_encoded_size(const std_msgs_Int64* p);
uint32_t std_msgs_Int64_struct_size(void);
uint32_t std_msgs_Int64_num_fields(void);
int      std_msgs_Int64_get_field(const std_msgs_Int64* p, uint32_t i, zcm_field_t* f);
const zcm_type_info_t* std_msgs_Int64_get_type_info(void);

// ZCM support functions. Users should not call these
int64_t  __std_msgs_Int64_get_hash(void);
uint64_t __std_msgs_Int64_hash_recursive(const __zcm_hash_ptr* p);
int      __std_msgs_Int64_encode_array(void* buf, uint32_t offset, uint32_t maxlen, const std_msgs_Int64* p, uint32_t elements);
int      __std_msgs_Int64_decode_array(const void* buf, uint32_t offset, uint32_t maxlen, std_msgs_Int64* p, uint32_t elements);
int      __std_msgs_Int64_decode_array_cleanup(std_msgs_Int64* p, uint32_t elements);
uint32_t __std_msgs_Int64_encoded_array_size(const std_msgs_Int64* p, uint32_t elements);
uint32_t __std_msgs_Int64_clone_array(const std_msgs_Int64* p, std_msgs_Int64* q, uint32_t elements);

#ifdef __cplusplus
}
#endif

#endif
