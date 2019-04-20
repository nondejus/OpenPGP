/*
SHA1.h
Secure Hash Algorithm 1

Copyright (c) 2013 - 2019 Jason Lee @ calccrypto at gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef __OPENSSL_SHA1__
#define __OPENSSL_SHA1__

#include <openssl/sha.h>

#include "Hashes/MerkleDamgard.h"

namespace OpenPGP {
    namespace Hash {
        class SHA1 : public MerkleDamgard {
            protected:
                SHA_CTX ctx;

            public:
                SHA1();
                SHA1(const std::string & data);
                void update(const std::string & str);
                virtual std::string hexdigest();
                virtual std::size_t blocksize() const;
                virtual std::size_t digestsize() const;
        };
    }
}

#endif