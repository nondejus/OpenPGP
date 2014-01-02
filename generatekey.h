/*
generatekey.h
Key pair generation function

Copyright (c) 2013 Jason Lee

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

#include <gmpxx.h>

#include "Keys/PGPTypes.h"
#include "PKA/PKA.h"
#include "cfb.h"
#include "pgptime.h"
#include "PKCS1.h"
#include "sign.h"
#include "sigcalc.h"
#include "usehash.h"

#ifndef __GENERATE_KEY__
#define __GENERATE_KEY__

// Fills in provided empty keys
void generate_keys(PGP & public_key, PGP & private_key, const std::string & passphrase = "", const std::string & user = "", const std::string & comment = "", const std::string & email = "", const unsigned int DSA_bits = 2048, const unsigned int ElGamal_bits = 2048);

// Given a private key with its packets filled with non PKA data, will
// fill in the following fields of both the public key and private key:
//  public PKA values
//  private PKA values
//  key id (optional)
//
// All other fields should be filled by the user
void add_key_values(PGP & pub, PGP & pri, const std::string & passphrase = "", const bool new_keyid = false, const unsigned int pri_key_size = 1024, const unsigned int subkey_size = 1024);

#endif
