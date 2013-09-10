/* Generated by the protocol buffer compiler.  DO NOT EDIT! */

#ifndef PROTOBUF_C_aitown_5fmessages_2eproto__INCLUDED
#define PROTOBUF_C_aitown_5fmessages_2eproto__INCLUDED

#include <google/protobuf-c/protobuf-c.h>

PROTOBUF_C_BEGIN_DECLS


typedef struct _AiTownIndexAdd AiTownIndexAdd;
typedef struct _AiTownIndexRem AiTownIndexRem;
typedef struct _AiTownIndex AiTownIndex;
typedef struct _AiTownIndexReply AiTownIndexReply;


/* --- enums --- */

typedef enum _AiTownIndexMessageType {
  AI_TOWN_INDEX_MESSAGE_TYPE__AITMT_INDEX_OK = 0,
  AI_TOWN_INDEX_MESSAGE_TYPE__AITMT_INDEX_ERROR = 1,
  AI_TOWN_INDEX_MESSAGE_TYPE__AITMT_INDEX_ADD = 2,
  AI_TOWN_INDEX_MESSAGE_TYPE__AITMT_INDEX_REM = 3,
  AI_TOWN_INDEX_MESSAGE_TYPE__AITMT_INDEX_LIST = 4
} AiTownIndexMessageType;

/* --- messages --- */

struct  _AiTownIndexAdd
{
  ProtobufCMessage base;
  char *name;
  char *address;
  protobuf_c_boolean has_port;
  int32_t port;
};
#define AI_TOWN_INDEX_ADD__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&ai_town_index_add__descriptor) \
    , NULL, NULL, 0,0 }


struct  _AiTownIndexRem
{
  ProtobufCMessage base;
  char *name;
};
#define AI_TOWN_INDEX_REM__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&ai_town_index_rem__descriptor) \
    , NULL }


struct  _AiTownIndex
{
  ProtobufCMessage base;
  AiTownIndexMessageType type;
  int32_t version;
  AiTownIndexAdd *add;
  AiTownIndexRem *rem;
};
#define AI_TOWN_INDEX__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&ai_town_index__descriptor) \
    , 0, 0, NULL, NULL }


struct  _AiTownIndexReply
{
  ProtobufCMessage base;
  AiTownIndexMessageType type;
  int32_t version;
  char *error_message;
  size_t n_list;
  AiTownIndexAdd **list;
};
#define AI_TOWN_INDEX_REPLY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&ai_town_index_reply__descriptor) \
    , 0, 0, NULL, 0,NULL }


/* AiTownIndexAdd methods */
void   ai_town_index_add__init
                     (AiTownIndexAdd         *message);
size_t ai_town_index_add__get_packed_size
                     (const AiTownIndexAdd   *message);
size_t ai_town_index_add__pack
                     (const AiTownIndexAdd   *message,
                      uint8_t             *out);
size_t ai_town_index_add__pack_to_buffer
                     (const AiTownIndexAdd   *message,
                      ProtobufCBuffer     *buffer);
AiTownIndexAdd *
       ai_town_index_add__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   ai_town_index_add__free_unpacked
                     (AiTownIndexAdd *message,
                      ProtobufCAllocator *allocator);
/* AiTownIndexRem methods */
void   ai_town_index_rem__init
                     (AiTownIndexRem         *message);
size_t ai_town_index_rem__get_packed_size
                     (const AiTownIndexRem   *message);
size_t ai_town_index_rem__pack
                     (const AiTownIndexRem   *message,
                      uint8_t             *out);
size_t ai_town_index_rem__pack_to_buffer
                     (const AiTownIndexRem   *message,
                      ProtobufCBuffer     *buffer);
AiTownIndexRem *
       ai_town_index_rem__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   ai_town_index_rem__free_unpacked
                     (AiTownIndexRem *message,
                      ProtobufCAllocator *allocator);
/* AiTownIndex methods */
void   ai_town_index__init
                     (AiTownIndex         *message);
size_t ai_town_index__get_packed_size
                     (const AiTownIndex   *message);
size_t ai_town_index__pack
                     (const AiTownIndex   *message,
                      uint8_t             *out);
size_t ai_town_index__pack_to_buffer
                     (const AiTownIndex   *message,
                      ProtobufCBuffer     *buffer);
AiTownIndex *
       ai_town_index__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   ai_town_index__free_unpacked
                     (AiTownIndex *message,
                      ProtobufCAllocator *allocator);
/* AiTownIndexReply methods */
void   ai_town_index_reply__init
                     (AiTownIndexReply         *message);
size_t ai_town_index_reply__get_packed_size
                     (const AiTownIndexReply   *message);
size_t ai_town_index_reply__pack
                     (const AiTownIndexReply   *message,
                      uint8_t             *out);
size_t ai_town_index_reply__pack_to_buffer
                     (const AiTownIndexReply   *message,
                      ProtobufCBuffer     *buffer);
AiTownIndexReply *
       ai_town_index_reply__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   ai_town_index_reply__free_unpacked
                     (AiTownIndexReply *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*AiTownIndexAdd_Closure)
                 (const AiTownIndexAdd *message,
                  void *closure_data);
typedef void (*AiTownIndexRem_Closure)
                 (const AiTownIndexRem *message,
                  void *closure_data);
typedef void (*AiTownIndex_Closure)
                 (const AiTownIndex *message,
                  void *closure_data);
typedef void (*AiTownIndexReply_Closure)
                 (const AiTownIndexReply *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCEnumDescriptor    ai_town_index_message_type__descriptor;
extern const ProtobufCMessageDescriptor ai_town_index_add__descriptor;
extern const ProtobufCMessageDescriptor ai_town_index_rem__descriptor;
extern const ProtobufCMessageDescriptor ai_town_index__descriptor;
extern const ProtobufCMessageDescriptor ai_town_index_reply__descriptor;

PROTOBUF_C_END_DECLS


#endif  /* PROTOBUF_aitown_5fmessages_2eproto__INCLUDED */
