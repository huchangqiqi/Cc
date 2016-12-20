/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: student.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "student.pb-c.h"
void   student__init
                     (Student         *message)
{
  static Student init_value = STUDENT__INIT;
  *message = init_value;
}
size_t student__get_packed_size
                     (const Student *message)
{
  assert(message->base.descriptor == &student__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t student__pack
                     (const Student *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &student__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t student__pack_to_buffer
                     (const Student *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &student__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Student *
       student__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Student *)
     protobuf_c_message_unpack (&student__descriptor,
                                allocator, len, data);
}
void   student__free_unpacked
                     (Student *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &student__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor student__field_descriptors[7] =
{
  {
    "id",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Student, id),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "name",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Student, name),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "gender",
    3,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Student, gender),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "age",
    4,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(Student, has_age),
    offsetof(Student, age),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "object",
    5,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Student, object),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "home_address",
    6,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Student, home_address),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "phone",
    7,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Student, phone),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned student__field_indices_by_name[] = {
  3,   /* field[3] = age */
  2,   /* field[2] = gender */
  5,   /* field[5] = home_address */
  0,   /* field[0] = id */
  1,   /* field[1] = name */
  4,   /* field[4] = object */
  6,   /* field[6] = phone */
};
static const ProtobufCIntRange student__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 7 }
};
const ProtobufCMessageDescriptor student__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "Student",
  "Student",
  "Student",
  "",
  sizeof(Student),
  7,
  student__field_descriptors,
  student__field_indices_by_name,
  1,  student__number_ranges,
  (ProtobufCMessageInit) student__init,
  NULL,NULL,NULL    /* reserved[123] */
};
