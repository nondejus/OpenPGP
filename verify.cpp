#include "verify.h"

#include "PKCS1.h"

std::string find_keyid(const Tag2::Ptr & tag2){
    std::string out = "";
    // Search Subpackets
    // Most likely in unhashed subpackets
    std::vector <Subpacket::Ptr> subpackets = tag2 -> get_unhashed_subpackets();
    for(Subpacket::Ptr const & s : subpackets){
        if (s -> get_type() == 16){
            std::string temp = s -> raw();
            Tag2Sub16 tag2sub16(temp);
            out = tag2sub16.get_keyid();
            break;
        }
    }
    // if not found in unhashed subpackets, search hashed subpackets
    if (!out.size()){
        subpackets = tag2 -> get_hashed_subpackets();
        for(Subpacket::Ptr const & s : subpackets){
            if (s -> get_type() == 16){
                std::string temp = s -> raw();
                Tag2Sub16 tag2sub16(temp);
                out = tag2sub16.get_keyid();
                break;
            }
        }
    }
    return out;
}

Tag6::Ptr find_signingkey(const std::string & keyid, const PGPPublicKey & pub){
    Tag6::Ptr out = nullptr;
    for(Packet::Ptr const & p : pub.get_packets()){
        if ((p -> get_tag() == 5) || (p -> get_tag() == 6) || (p -> get_tag() == 7) || (p -> get_tag() == 14)){
            std::string raw = p -> raw();
            Tag6 * temp = NULL;
            if (p -> get_tag() == 5){
                temp = new Tag5(raw);
            }
            else if (p -> get_tag() == 6){
                temp = new Tag6(raw);
            }
            else if (p -> get_tag() == 7){
                temp = new Tag7(raw);
            }
            else if (p -> get_tag() == 14){
                temp = new Tag14(raw);
            }

            // check if keyids match
            if (temp -> get_keyid() == keyid){
                out = Tag6::Ptr(temp);
                break;
            }
            delete temp;
        }
    }
    return out;
}

bool pka_verify(const std::string & hashed_message, const std::vector<PGPMPI> & signing, const uint8_t hash, const uint8_t pka, const std::vector<PGPMPI> & signature){
    if ((pka == 1) || (pka == 3)){ // RSA
        std::string new_hashed_message = EMSA_PKCS1_v1_5(hash, hashed_message, bitsize(signing[0]) >> 3);
        return RSA_verify(new_hashed_message, signature, signing);
    }
    else if (pka == 17){ // DSA
        return DSA_verify(hashed_message, signature, signing);
    }
    return false;
}

bool pka_verify(const std::string & hashed_message, const Tag6::Ptr signing, const Tag2::Ptr & signature){
    return pka_verify(hashed_message, signing -> get_mpi(), signature -> get_hash(), signature -> get_pka(), signature -> get_mpi());
}

// Signature type 0x00 and 0x01
bool verify_cleartext_signature(const PGPPublicKey & pub, const PGPCleartextSignature & message){
    if ((pub.get_ASCII_Armor() != 1) && (pub.get_ASCII_Armor() != 2)){
        throw std::runtime_error("Error: A PGP key is required.");
    }

    // Find key id from signature to match with public key
    std::string temp = message.get_sig().get_packets()[0] -> raw();
    Tag2::Ptr signature = std::make_shared<Tag2>(); signature -> read(temp);

    // check left 16 bits
    std::string hash = to_sign_01(message.get_message(), signature);
    if (hash.substr(0, 2) != signature -> get_left16()){
        throw std::runtime_error("Error: Hash and given left 16 bits of hash do not match.");
    }

    // find key id in signature
    std::string keyid = find_keyid(signature);
    if (!keyid.size()){
        throw std::runtime_error("Error: No Key ID subpacket found.");
    }

    // find matching public key packet and get the mpi
    Tag6::Ptr signingkey = find_signingkey(keyid, pub);
    if (!signingkey){
        return false;
    }

    // get string to check
    return pka_verify(hash, signingkey, signature);
}

bool verify_cleartext_signature(const PGPSecretKey & pri, const PGPCleartextSignature & message){
    return verify_cleartext_signature(PGPPublicKey(pri), message);
}

bool verify_detachedsig(const PGPPublicKey & pub, const std::string & data, const PGPDetachedSignature & sig){
    if (sig.get_ASCII_Armor() != 5){
        throw std::runtime_error("Error: A signature packet is required.");
    }

    if ((pub.get_ASCII_Armor() != 1) && (pub.get_ASCII_Armor() != 2)){
        throw std::runtime_error("Error: A PGP key is required.");
    }

    std::string temp = sig.get_packets()[0] -> raw();
    Tag2::Ptr signature = std::make_shared<Tag2>(); signature -> read(temp);

    // Check left 16 bits
    std::string hash = to_sign_00(data, signature);
    if (hash.substr(0, 2) != signature -> get_left16()){
        throw std::runtime_error("Error: Hash and given left 16 bits of hash do not match.");
        // return false;
    }

    // find key id in signature
    std::string keyid = find_keyid(signature);
    if (!keyid.size()){
        throw std::runtime_error("Error: No Key ID subpacket found.");
        // return false;
    }

    // find matching public key packet and get the mpi
    Tag6::Ptr signingkey = find_signingkey(keyid, pub);
    if (!signingkey){
        return false;
    }
    bool out = pka_verify(hash, signingkey, signature);
    return out;
}

bool verify_detachedsig(const PGPSecretKey & pri, const std::string & data, const PGPDetachedSignature & sig){
    return verify_detachedsig(PGPPublicKey(pri), data, sig);
}

bool verify_detachedsig(const PGPPublicKey & pub, std::ifstream & f, const PGPDetachedSignature & sig){
    if (!f){
        throw std::runtime_error("Error: Bad file.");
    }
    std::stringstream s;
    s << f.rdbuf();
    std::string data = s.str();

    return verify_detachedsig(pub, data, sig);
}

bool verify_detachedsig(const PGPSecretKey & pri, std::ifstream & f, const PGPDetachedSignature & sig){
    return verify_detachedsig(PGPPublicKey(pri), f, sig);
}

bool verify_message(const Tag6::Ptr & signing_key, const PGPMessage & m){
    if (m.match(PGPMessage::ENCRYPTEDMESSAGE)){
        // Encrypted Message :- Encrypted Data | ESK Sequence, Encrypted Data.
        throw std::runtime_error("Error: Use decrypt to verify message");
    }
    else if (m.match(PGPMessage::SIGNEDMESSAGE)){
        // // Signed Message :- Signature Packet, OpenPGP Message | One-Pass Signed Message.
        // // One-Pass Signed Message :- One-Pass Signature Packet, OpenPGP Message, Corresponding Signature Packet.

        // parse packets
        std::vector <Packet::Ptr> packets = m.get_packets();

        /*
        Note that if a message contains more than one one-pass signature,
        then the Signature packets bracket the message; that is, the first
        Signature packet after the message corresponds to the last one-pass
        packet and the final Signature packet corresponds to the first
        one-pass packet.
        */

        // Tag4_0, Tag4_1, ... , Tag4_n, Tag8/11, Tag2_n, ... , Tag2_1, Tag2_0

        unsigned int i = 0;
        std::list <Tag4::Ptr> OPSP;                                     // treat as stack
        while ((i < packets.size()) && (packets[i] -> get_tag() == 4)){
            std::string data = packets[i] -> raw();
            OPSP.push_front(std::shared_ptr <Tag4> (new Tag4(data)));   // put next Tag4 onto stack
            i++;

            if ((*(OPSP.rbegin())) -> get_nested() != 0){               // check if there are nested packets
                break;                                                  // probably unecessary
            }
        }

        // get signed data
        std::string binary = packets[i] -> raw();
        i++;
        binary = Tag11(binary).get_literal();                           // binary data hashed directly
        std::string text;                                               // text data line endings converted to <CR><LF>

        // cache text version of data
        // probably only one of binary or text is needed at one time
        if (binary[0] == '\n'){
            text = "\r";
        }
        text += std::string(1, binary[0]);
        unsigned int c = 1;
        while (c < binary.size()){
            if (binary[c] == '\n'){                                     // if current character is newline
                if (binary[c - 1] != '\r'){                             // if previous character was not carriage return
                    text += "\r";                                       // add a carriage return
                }
            }
            text += std::string(1, binary[c++]);                        // add current character
        }

        // get signatures
        std::list <Tag2::Ptr> SP;                                       // treat as queue
        while ((i < packets.size()) && (packets[i] -> get_tag() == 2)){
            std::string data = packets[i] -> raw();
            SP.push_front(std::shared_ptr <Tag2> (new Tag2(data)));     // put next Tag2 onto queue
            i++;
        }

        // check for signatures
        if (!OPSP.size() || !SP.size()){
            throw std::runtime_error("Error: No signature found");
        }

        // both lists should be the same size
        if (OPSP.size() != SP.size()){
            throw std::runtime_error("Error: Different number of One-Pass Signatures and Signature packets.");
        }

        // check for matching signature
        bool verify = false;
        while (OPSP.size() && SP.size()){

            // // extra warnings
            // // check that KeyIDs match
            // if ((*(OPSP.rbegin())) -> get_keyid() == (*(SP.begin())) -> get_keyid()){

                // // check that all the parameters match
                // bool match = true;

                // // Signature Type
                // if ((*(OPSP.rbegin())) -> get_type() != (*(SP.begin())) -> get_type()){
                    // match = false;
                    // std::cerr << "Warning: One-Pass Signature Packet and Signature Packet Signature Type mismatch" << std::endl;
                // }

                // // Hash Algorithm
                // if ((*(OPSP.rbegin())) -> get_hash() != (*(SP.begin())) -> get_hash()){
                    // match = false;
                    // std::cerr << "Warning: One-Pass Signature Packet and Signature Packet Hash Algorithm mismatch" << std::endl;
                // }

                // // Public Key Algorithm
                // if ((*(OPSP.rbegin())) -> get_pka() != (*(SP.begin())) -> get_pka()){
                    // match = false;
                    // std::cerr << "Warning: One-Pass Signature Packet and Signature Packet Public Key Algorithm mismatch" << std::endl;
                // }

                // // check signature
                // if (match){

                    // if KeyID of given key matches this Tag4/Tag2 pair's KeyID
                    if (signing_key -> get_keyid() == (*(SP.begin())) -> get_keyid()){

                        // get hashed data
                        std::string hashed;
                        switch ((*(OPSP.rbegin())) -> get_type()){
                            case 0:
                                hashed = to_sign_00(binary, *(SP.begin()));
                                break;
                            case 1:
                                hashed = to_sign_01(text, *(SP.begin()));
                                break;

                            // don't know if other signature types can be here

                            // certifications
                            case 0x10: case 0x11:
                            case 0x12: case 0x13:
                            default:
                                {
                                    std::cerr << "Warning: Bad signature type: " << static_cast <int> ((*(OPSP.rbegin())) -> get_type()) << std::endl;
                                    verify = false;
                                }
                                break;
                        }

                        // check if the key matches this signature
                        verify = pka_verify(hashed, signing_key, *(SP.begin()));
                    }
                // }
            // }
            // else{
                // verify = false;
                // std:cerr << "Warning: One-Pass Signature Packet and Signature Packet KeyID mismatch" << std::endl;
            // }
            OPSP.pop_back(); // pop top of stack
            SP.pop_front();  // pop front of queue
        }
        return verify;
    }
    else if (m.match(PGPMessage::COMPRESSEDMESSAGE)){
        // Compressed Message :- Compressed Data Packet.

        // only one compressed data packet
        std::string message = m.get_packets()[0] -> raw();

        // decompress data
        Tag8 tag8(message);
        message = tag8.get_data();

        return verify_message(signing_key, PGPMessage(message));
    }
    else if (m.match(PGPMessage::LITERALMESSAGE)){
        // Literal Message :- Literal Data Packet.

        // only one literal data packet
        std::string message = m.get_packets()[0] -> raw();

        // extract data
        Tag11 tag11(message);
        message = tag11.get_literal();

        return verify_message(signing_key, PGPMessage(message));
    }
    else{
        throw std::runtime_error("Error: Not an OpenPGP Message. Perhaps Detached Signature?");
        // return false;
    }

    return false; // get rid of compiler warnings
}

bool verify_message(const PGPPublicKey & pub, const PGPMessage & m){
    // get signing key
    Tag6::Ptr signing_key = nullptr;
    for(Packet::Ptr const & p : pub.get_packets()){
        // if its a public key packet
        if ((p -> get_tag() == 6) || (p -> get_tag() == 14)){
            std::string data = p -> raw();
            Tag6::Ptr tag6(new Tag6(data));

            // if its a signing key packet
            if ((tag6 -> get_pka() == 1) || (tag6 -> get_pka() == 3) || (tag6 -> get_pka() == 17)){
                // get keys
                signing_key = tag6;
                break;
            }
        }
    }
    if (!signing_key){
        throw std::runtime_error("Error: No public signing keys found");
        // return false;
    }
    return verify_message(signing_key, m);
}

bool verify_message(const PGPSecretKey & pri, const PGPMessage & m){
    return verify_message(PGPPublicKey(pri), m);
}

// Signature Type 0x10 - 0x13
bool verify_signature(const PGPPublicKey & signer, const PGPPublicKey & signee){
    if ((signer.get_ASCII_Armor() != 1) && (signer.get_ASCII_Armor() != 2)){
        throw std::runtime_error("Error: A PGP key is required.");
    }

    if ((signee.get_ASCII_Armor() != 1) && (signee.get_ASCII_Armor() != 2)){
        throw std::runtime_error("Error: A PGP key is required.");
    }

    std::string keyid;

    // find signature packet on signee
    Tag2::Ptr signature_packet = nullptr;
    for(Packet::Ptr const & p : signee.get_packets()){
        if (p -> get_tag() == 2){
            std::string temp = p -> raw();
            Tag2::Ptr tag2(new Tag2(temp));
            // if this signature packet is a certification signature packet
            if ((0x10 <= tag2 -> get_type()) && (tag2 -> get_type() <= 0x13)){
                keyid = find_keyid(tag2);
                break;            
            }            
        }
    }
    
    if (!keyid.size()){
        std::cerr << "Warning: No certification signature packet found" << std::endl;
        return false;
    }

    // find signing key
    Tag6::Ptr signingkey = find_signingkey(keyid, signer);

    // if there is no matching signing key
    if (!signingkey){
        return false;
    }

    uint8_t version = 0;
    std::string k = "";
    std::string u = "";

    // set packets to signatures to verify
    bool out = true;

    Tag6::Ptr tag6;
    // for each packet
    for(Packet::Ptr const & p : signee.get_packets()){
        std::string data = p -> raw();
        switch (p -> get_tag()){
            case 5: case 6: case 7: case 14:            // key packet
                tag6 = std::make_shared<Tag6>();
                tag6 -> read(data);
                k += overkey(tag6);                     // add current key packet to previous ones
                version = tag6 -> get_version();
                tag6.reset();
                break;
            case 13: case 17:                           // User packet
                {
                    ID::Ptr id;
                    if (p -> get_tag() == 13){
                        id = std::make_shared<Tag13>();
                    }
                    if (p -> get_tag() == 17){
                        id = std::make_shared<Tag17>();
                    }
                    id -> read(data);
                    u = certification(version, id);     // write over old user information
                }
                break;
            case 2:                                     // signature packet
                {
                    // copy packet data into signature packet
                    Tag2::Ptr tag2(new Tag2);
                    tag2 -> read(data);

                    // if signature is keybinding, erase the user information
                    if ((tag2 -> get_type() == 0x18) ||
                        (tag2 -> get_type() == 0x18)){
                        u = "";
                    }
                    // add hash contexts together and append trailer data
                    std::string with_trailer = addtrailer(k + u, tag2);
                    std::string hash = use_hash(tag2 -> get_hash(), with_trailer);
                    if (hash.substr(0, 2) == tag2 -> get_left16()){// quick signature check
                        out |= pka_verify(hash, signingkey, tag2);// proper signature check
                    }
                }
                break;
            default:
                {
                    std::stringstream s; s << static_cast <int> (p -> get_tag());
                    throw std::runtime_error("Error: Incorrect packet type found: " + s.str());
                }
                break;
        }
    }
    return out;
}

bool verify_signature(const PGPSecretKey & pri, const PGPPublicKey & signer){
    return verify_signature(PGPPublicKey(pri), signer);
}

bool verify_revoke(const Tag6::Ptr & key, const Tag2::Ptr & rev){
    return pka_verify(use_hash(rev -> get_hash(), addtrailer(overkey(key), rev)), key, rev);
}

bool verify_revoke(const PGPPublicKey & pub, const PGPPublicKey & rev){
    if ((pub.get_ASCII_Armor() != 1) && (pub.get_ASCII_Armor() != 2)){
        throw std::runtime_error("Error: A PGP key is required.");
    }

    if (rev.get_ASCII_Armor() != 1){
        throw std::runtime_error("Error: A revocation key is required.");
    }

    std::vector <Packet::Ptr> keys = pub.get_packets();

    // copy revocation signature into tag2
    std::vector <Packet::Ptr> rev_pointers = rev.get_packets();

    // get revocation key; assume only 1 packet
    std::string rev_str = rev_pointers[0] -> raw();
    Tag2::Ptr revoke = std::make_shared<Tag2>(rev_str);

    // for each key packet
    for(Packet::Ptr const & p : keys){
        // if the packet is a key packet
        if ((p -> get_tag() == 5) ||
            (p -> get_tag() == 6) ||
            (p -> get_tag() == 7) ||
            (p -> get_tag() == 14)){

            // copy key into Tag 6
            std::string key_str = p -> raw();
            Tag6::Ptr tag6 = std::make_shared<Tag6>(key_str);

            if (verify_revoke(tag6, revoke)){
                return true;
            }
        }
    }
    return false;
}

bool verify_revoke(const PGPSecretKey & pri, const PGPPublicKey & rev){
    PGPPublicKey pub(pri);
    return verify_revoke(pub, rev);
}