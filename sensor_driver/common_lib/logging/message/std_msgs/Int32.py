"""ZCM type definitions
This file automatically generated by zcm.
DO NOT MODIFY BY HAND!!!!
"""

try:
    import cStringIO.StringIO as BytesIO
except ImportError:
    from io import BytesIO
import struct

class Int32(object):
    __slots__ = ["data"]

    IS_LITTLE_ENDIAN = False;
    def __init__(self):
        self.data = 0

    def encode(self):
        buf = BytesIO()
        buf.write(Int32._get_packed_fingerprint())
        self._encode_one(buf)
        return buf.getvalue()

    def _encode_one(self, buf):
        buf.write(struct.pack(">i", self.data))

    def decode(data):
        if hasattr(data, 'read'):
            buf = data
        else:
            buf = BytesIO(data)
        if buf.read(8) != Int32._get_packed_fingerprint():
            raise ValueError("Decode error")
        return Int32._decode_one(buf)
    decode = staticmethod(decode)

    def _decode_one(buf):
        self = Int32()
        self.data = struct.unpack(">i", buf.read(4))[0]
        return self
    _decode_one = staticmethod(_decode_one)

    _hash = None
    def _get_hash_recursive(parents):
        if Int32 in parents: return 0
        tmphash = (0xa686f8efe4f1a997) & 0xffffffffffffffff
        tmphash  = (((tmphash<<1)&0xffffffffffffffff)  + ((tmphash>>63)&0x1)) & 0xffffffffffffffff
        return tmphash
    _get_hash_recursive = staticmethod(_get_hash_recursive)
    _packed_fingerprint = None

    def _get_packed_fingerprint():
        if Int32._packed_fingerprint is None:
            Int32._packed_fingerprint = struct.pack(">Q", Int32._get_hash_recursive([]))
        return Int32._packed_fingerprint
    _get_packed_fingerprint = staticmethod(_get_packed_fingerprint)

