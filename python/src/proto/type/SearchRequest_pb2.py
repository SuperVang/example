# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: SearchRequest.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='SearchRequest.proto',
  package='books',
  syntax='proto3',
  serialized_options=None,
  serialized_pb=_b('\n\x13SearchRequest.proto\x12\x05\x62ooks\"x\n\rSearchRequest\x12\r\n\x05query\x18\x01 \x01(\t\x12\x13\n\x0bpage_number\x18\x02 \x01(\x05\x12\x17\n\x0fresult_per_page\x18\x03 \x01(\x05\x12\x1d\n\x06\x63orpus\x18\x04 \x01(\x0e\x32\r.books.Corpus\x12\x0b\n\x03ips\x18\x05 \x03(\x05\"\x1a\n\x02Pt\x12\t\n\x01x\x18\x01 \x01(\x03\x12\t\n\x01y\x18\x02 \x01(\x03\"_\n\x0bResponseMsg\x12\x0b\n\x03msg\x18\x01 \x01(\t\x12\x10\n\x08msg_line\x18\x02 \x01(\x05\x12\x16\n\x03pts\x18\x03 \x03(\x0b\x32\t.books.Pt\x12\x19\n\x06\x63\x65nter\x18\x04 \x01(\x0b\x32\t.books.Pt\"\x1c\n\x04ObjA\x12\t\n\x01x\x18\x02 \x01(\x01\x12\t\n\x01y\x18\x03 \x01(\x01\" \n\x04ObjB\x12\x0b\n\x03lon\x18\x01 \x01(\x05\x12\x0b\n\x03lat\x18\x02 \x01(\x05\"[\n\x03Obj\x12\n\n\x02id\x18\x01 \x01(\x05\x12\x0c\n\x04type\x18\x02 \x01(\x05\x12\x18\n\x01\x61\x18\x03 \x01(\x0b\x32\x0b.books.ObjAH\x00\x12\x18\n\x01\x62\x18\x04 \x01(\x0b\x32\x0b.books.ObjBH\x00\x42\x06\n\x04impl\"#\n\x07ObjList\x12\x18\n\x04objs\x18\x01 \x03(\x0b\x32\n.books.Obj*+\n\x06\x43orpus\x12\r\n\tUNIVERSAL\x10\x00\x12\x07\n\x03WEB\x10\x01\x12\t\n\x05LOCAL\x10\x02\x62\x06proto3')
)

_CORPUS = _descriptor.EnumDescriptor(
  name='Corpus',
  full_name='books.Corpus',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='UNIVERSAL', index=0, number=0,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='WEB', index=1, number=1,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='LOCAL', index=2, number=2,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=471,
  serialized_end=514,
)
_sym_db.RegisterEnumDescriptor(_CORPUS)

Corpus = enum_type_wrapper.EnumTypeWrapper(_CORPUS)
UNIVERSAL = 0
WEB = 1
LOCAL = 2



_SEARCHREQUEST = _descriptor.Descriptor(
  name='SearchRequest',
  full_name='books.SearchRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='query', full_name='books.SearchRequest.query', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='page_number', full_name='books.SearchRequest.page_number', index=1,
      number=2, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='result_per_page', full_name='books.SearchRequest.result_per_page', index=2,
      number=3, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='corpus', full_name='books.SearchRequest.corpus', index=3,
      number=4, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='ips', full_name='books.SearchRequest.ips', index=4,
      number=5, type=5, cpp_type=1, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=30,
  serialized_end=150,
)


_PT = _descriptor.Descriptor(
  name='Pt',
  full_name='books.Pt',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='x', full_name='books.Pt.x', index=0,
      number=1, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='y', full_name='books.Pt.y', index=1,
      number=2, type=3, cpp_type=2, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=152,
  serialized_end=178,
)


_RESPONSEMSG = _descriptor.Descriptor(
  name='ResponseMsg',
  full_name='books.ResponseMsg',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='msg', full_name='books.ResponseMsg.msg', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='msg_line', full_name='books.ResponseMsg.msg_line', index=1,
      number=2, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='pts', full_name='books.ResponseMsg.pts', index=2,
      number=3, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='center', full_name='books.ResponseMsg.center', index=3,
      number=4, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=180,
  serialized_end=275,
)


_OBJA = _descriptor.Descriptor(
  name='ObjA',
  full_name='books.ObjA',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='x', full_name='books.ObjA.x', index=0,
      number=2, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='y', full_name='books.ObjA.y', index=1,
      number=3, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=277,
  serialized_end=305,
)


_OBJB = _descriptor.Descriptor(
  name='ObjB',
  full_name='books.ObjB',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='lon', full_name='books.ObjB.lon', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='lat', full_name='books.ObjB.lat', index=1,
      number=2, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=307,
  serialized_end=339,
)


_OBJ = _descriptor.Descriptor(
  name='Obj',
  full_name='books.Obj',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='id', full_name='books.Obj.id', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='type', full_name='books.Obj.type', index=1,
      number=2, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='a', full_name='books.Obj.a', index=2,
      number=3, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='b', full_name='books.Obj.b', index=3,
      number=4, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
    _descriptor.OneofDescriptor(
      name='impl', full_name='books.Obj.impl',
      index=0, containing_type=None, fields=[]),
  ],
  serialized_start=341,
  serialized_end=432,
)


_OBJLIST = _descriptor.Descriptor(
  name='ObjList',
  full_name='books.ObjList',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='objs', full_name='books.ObjList.objs', index=0,
      number=1, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=434,
  serialized_end=469,
)

_SEARCHREQUEST.fields_by_name['corpus'].enum_type = _CORPUS
_RESPONSEMSG.fields_by_name['pts'].message_type = _PT
_RESPONSEMSG.fields_by_name['center'].message_type = _PT
_OBJ.fields_by_name['a'].message_type = _OBJA
_OBJ.fields_by_name['b'].message_type = _OBJB
_OBJ.oneofs_by_name['impl'].fields.append(
  _OBJ.fields_by_name['a'])
_OBJ.fields_by_name['a'].containing_oneof = _OBJ.oneofs_by_name['impl']
_OBJ.oneofs_by_name['impl'].fields.append(
  _OBJ.fields_by_name['b'])
_OBJ.fields_by_name['b'].containing_oneof = _OBJ.oneofs_by_name['impl']
_OBJLIST.fields_by_name['objs'].message_type = _OBJ
DESCRIPTOR.message_types_by_name['SearchRequest'] = _SEARCHREQUEST
DESCRIPTOR.message_types_by_name['Pt'] = _PT
DESCRIPTOR.message_types_by_name['ResponseMsg'] = _RESPONSEMSG
DESCRIPTOR.message_types_by_name['ObjA'] = _OBJA
DESCRIPTOR.message_types_by_name['ObjB'] = _OBJB
DESCRIPTOR.message_types_by_name['Obj'] = _OBJ
DESCRIPTOR.message_types_by_name['ObjList'] = _OBJLIST
DESCRIPTOR.enum_types_by_name['Corpus'] = _CORPUS
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

SearchRequest = _reflection.GeneratedProtocolMessageType('SearchRequest', (_message.Message,), {
  'DESCRIPTOR' : _SEARCHREQUEST,
  '__module__' : 'SearchRequest_pb2'
  # @@protoc_insertion_point(class_scope:books.SearchRequest)
  })
_sym_db.RegisterMessage(SearchRequest)

Pt = _reflection.GeneratedProtocolMessageType('Pt', (_message.Message,), {
  'DESCRIPTOR' : _PT,
  '__module__' : 'SearchRequest_pb2'
  # @@protoc_insertion_point(class_scope:books.Pt)
  })
_sym_db.RegisterMessage(Pt)

ResponseMsg = _reflection.GeneratedProtocolMessageType('ResponseMsg', (_message.Message,), {
  'DESCRIPTOR' : _RESPONSEMSG,
  '__module__' : 'SearchRequest_pb2'
  # @@protoc_insertion_point(class_scope:books.ResponseMsg)
  })
_sym_db.RegisterMessage(ResponseMsg)

ObjA = _reflection.GeneratedProtocolMessageType('ObjA', (_message.Message,), {
  'DESCRIPTOR' : _OBJA,
  '__module__' : 'SearchRequest_pb2'
  # @@protoc_insertion_point(class_scope:books.ObjA)
  })
_sym_db.RegisterMessage(ObjA)

ObjB = _reflection.GeneratedProtocolMessageType('ObjB', (_message.Message,), {
  'DESCRIPTOR' : _OBJB,
  '__module__' : 'SearchRequest_pb2'
  # @@protoc_insertion_point(class_scope:books.ObjB)
  })
_sym_db.RegisterMessage(ObjB)

Obj = _reflection.GeneratedProtocolMessageType('Obj', (_message.Message,), {
  'DESCRIPTOR' : _OBJ,
  '__module__' : 'SearchRequest_pb2'
  # @@protoc_insertion_point(class_scope:books.Obj)
  })
_sym_db.RegisterMessage(Obj)

ObjList = _reflection.GeneratedProtocolMessageType('ObjList', (_message.Message,), {
  'DESCRIPTOR' : _OBJLIST,
  '__module__' : 'SearchRequest_pb2'
  # @@protoc_insertion_point(class_scope:books.ObjList)
  })
_sym_db.RegisterMessage(ObjList)


# @@protoc_insertion_point(module_scope)
