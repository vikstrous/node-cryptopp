#include <v8.h>
#include <node.h>
#include <string>
#include <iostream>
#include "node_buffer.h"
#include "cryptopp/sha.h"
#include "cryptopp/tiger.h"
#include "cryptopp/ripemd.h"
#include "cryptopp/whrlpool.h"
#include "cryptopp/md2.h"
#include "cryptopp/md4.h"
#include "cryptopp/md5.h"

using namespace node;
using namespace v8;
using namespace CryptoPP;

#define ASSERT_IS_STRING_OR_BUFFER(val) \
  if (!val->IsString() && !Buffer::HasInstance(val)) { \
    return ThrowException(v8::Exception::TypeError(String::New("Not a string or buffer"))); \
  }

#define ADD_FUNCTION(t,f) t->Set(String::NewSymbol(#f), FunctionTemplate::New(f)->GetFunction())

#define HASHFUNC(lowercase, uppercase) \
  static Handle<Value> lowercase(const Arguments& args) \
  { \
    HashTransformation * ht = new uppercase(); \
    Handle<Value> res = hash(ht, uppercase::DIGESTSIZE, args); \
    delete ht; \
    return res; \
  }

void Binary2Hex(byte *buffer, char *outbuffer, int buffsize)
{
  for (int i = 0; i < buffsize; i++)
    sprintf(outbuffer+2*i,"%02X",*(buffer+i) & 0xFF);
  outbuffer[2*buffsize] = '\0';
}


static void HexEncode(unsigned char *md_value,
                      int md_len,
                      char** md_hexdigest,
                      int* md_hex_len) {
  *md_hex_len = (2*(md_len));
  *md_hexdigest = new char[*md_hex_len + 1];

  char* buff = *md_hexdigest;
  const int len = *md_hex_len;
  for (int i = 0; i < len; i += 2) {
    // nibble nibble
    const int index = i / 2;
    const char msb = (md_value[index] >> 4) & 0x0f;
    const char lsb = md_value[index] & 0x0f;

    buff[i] = (msb < 10) ? msb + '0' : (msb - 10) + 'a';
    buff[i + 1] = (lsb < 10) ? lsb + '0' : (lsb - 10) + 'a';
  }
  // null terminator
  buff[*md_hex_len] = '\0';
}


inline static Handle<Value> hash(HashTransformation *ht, int digestsize, const Arguments& args)
{
  if (args.Length() < 1) {
    Local<Value> exception = v8::Exception::TypeError(String::New("No arguments provided to hash function."));
    return ThrowException(exception);
  }

  //verify input parameters
  ASSERT_IS_STRING_OR_BUFFER(args[0]);

  //parse input
  char *buffer_data;
  ssize_t len;
  if (Buffer::HasInstance(args[0])) {
    Local<Object> buffer_obj = args[0]->ToObject();
    buffer_data = Buffer::Data(buffer_obj);
    len = Buffer::Length(buffer_obj);
  } else {
    Local<String> str = args[0]->ToString();
    len = str->Utf8Length()+1;
    buffer_data = new char[len];
    ssize_t written = str->WriteUtf8(buffer_data);
    assert(written == len);
  }

  // //create digest buffer
  byte * digest = new byte[digestsize];

  // //calcualte hash
  ht->CalculateDigest(digest, (const byte*)buffer_data, len);

  //if we had to allocate the buffer, delete it now
  if (!Buffer::HasInstance(args[0])) {
    delete[] buffer_data;
  }

  //encode output. Default is binary, check arg[2] for hex
  Local<Value> result;
  String::Utf8Value encoding(args[1]);
  if (strcasecmp(*encoding, "binary")) {
    result = Encode(digest, digestsize, BINARY);
  } else if (strcasecmp(*encoding, "hex")) {
    // Hex encoding
    char* hexdigest = new char[digestsize*2+1];
    Binary2Hex(digest, hexdigest, digestsize);
    result = Encode(hexdigest, digestsize*2+1, BINARY);
    delete [] hexdigest;
  } else {
    Local<Value> exception = v8::Exception::TypeError(String::New("Cryptopp hashes currently support only binary and hex encoding of output."));
    return ThrowException(exception);
  }

  HandleScope scope;
  return scope.Close(result);
}

/*
static Handle<Value> hmac(HashTransformation *ht, int digestsize, const Arguments& args){
  
  Local<String> result = String::New(Binary2Hex((char*)digest, digestsize));

  HandleScope scope;
  return scope.Close(result);
}*/

HASHFUNC(sha1, SHA1);
HASHFUNC(sha224, SHA224);
HASHFUNC(sha256, SHA256);
HASHFUNC(sha384, SHA384);
HASHFUNC(sha512, SHA512);
HASHFUNC(tiger, Tiger);
HASHFUNC(whirlpool, Whirlpool);
HASHFUNC(ripemd128, RIPEMD128);
HASHFUNC(ripemd160, RIPEMD160);
HASHFUNC(ripemd320, RIPEMD320);
HASHFUNC(ripemd256, RIPEMD256);
HASHFUNC(md2, MD2);
HASHFUNC(md4, MD4);
HASHFUNC(md5, MD5);

// extern "C" {
  static void init (Handle<Object> target)
  {
    ADD_FUNCTION(target, sha1);
    ADD_FUNCTION(target, sha224);
    ADD_FUNCTION(target, sha256);
    ADD_FUNCTION(target, sha384);
    ADD_FUNCTION(target, sha512);
    ADD_FUNCTION(target, tiger);
    ADD_FUNCTION(target, whirlpool);
    ADD_FUNCTION(target, ripemd160);
    ADD_FUNCTION(target, ripemd320);
    ADD_FUNCTION(target, ripemd128);
    ADD_FUNCTION(target, ripemd256);
    ADD_FUNCTION(target, md2);
    ADD_FUNCTION(target, md4);
    ADD_FUNCTION(target, md5);
  }

  NODE_MODULE(cryptopp, init);
// }
