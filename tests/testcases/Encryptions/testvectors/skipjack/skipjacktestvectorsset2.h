#ifndef __SKIPJACKTESTVECTORSSET2__
#define __SKIPJACKTESTVECTORSSET2__

#include "../plainkeycipher.h"

// Test vector from <https://www.cosic.esat.kuleuven.be/nessie/testvectors/bc/skipjack/Skipjack-80-64.unverified.test-vectors>

static const std::vector <PlainKeyCipher> SKIPJACK_TEST_VECTORS_SET_2 = {
    std::make_tuple("8000000000000000", "00000000000000000000", "679F18E9B428F364"),
    std::make_tuple("4000000000000000", "00000000000000000000", "BFE0C31047B3B07C"),
    std::make_tuple("2000000000000000", "00000000000000000000", "F7F8CDD7252E0781"),
    std::make_tuple("1000000000000000", "00000000000000000000", "368768298CB0568F"),
    std::make_tuple("0800000000000000", "00000000000000000000", "D17369B9DF0324C3"),
    std::make_tuple("0400000000000000", "00000000000000000000", "8A06C56AB256DB26"),
    std::make_tuple("0200000000000000", "00000000000000000000", "CAD51CADCB3391EF"),
    std::make_tuple("0100000000000000", "00000000000000000000", "92C5CC9EFBC4E7B8"),
    std::make_tuple("0080000000000000", "00000000000000000000", "89F7A357E36F2691"),
    std::make_tuple("0040000000000000", "00000000000000000000", "A82241D61075823C"),
    std::make_tuple("0020000000000000", "00000000000000000000", "612DA32A3626D7B3"),
    std::make_tuple("0010000000000000", "00000000000000000000", "0C53D4D4D56F4BBD"),
    std::make_tuple("0008000000000000", "00000000000000000000", "5A183B86F33A0773"),
    std::make_tuple("0004000000000000", "00000000000000000000", "5320F3C06EE8E808"),
    std::make_tuple("0002000000000000", "00000000000000000000", "7B40C3AD13B46C79"),
    std::make_tuple("0001000000000000", "00000000000000000000", "F13FFCE7A151015D"),
    std::make_tuple("0000800000000000", "00000000000000000000", "1A6DF48889B6B65D"),
    std::make_tuple("0000400000000000", "00000000000000000000", "BD0F91873C0A2120"),
    std::make_tuple("0000200000000000", "00000000000000000000", "FFFC46F6F0A2770A"),
    std::make_tuple("0000100000000000", "00000000000000000000", "0A8FC06FF145E937"),
    std::make_tuple("0000080000000000", "00000000000000000000", "96AD848867769697"),
    std::make_tuple("0000040000000000", "00000000000000000000", "FB8F36F134597813"),
    std::make_tuple("0000020000000000", "00000000000000000000", "4345778085558DF1"),
    std::make_tuple("0000010000000000", "00000000000000000000", "2E5168203D2588B3"),
    std::make_tuple("0000008000000000", "00000000000000000000", "4A360BAC0E25487B"),
    std::make_tuple("0000004000000000", "00000000000000000000", "BB2E32C0F6C494E6"),
    std::make_tuple("0000002000000000", "00000000000000000000", "114F8E8DEDC5A7E5"),
    std::make_tuple("0000001000000000", "00000000000000000000", "FE293F45014F6F34"),
    std::make_tuple("0000000800000000", "00000000000000000000", "AE242CA30B0B1A40"),
    std::make_tuple("0000000400000000", "00000000000000000000", "9F1B13AF9930887D"),
    std::make_tuple("0000000200000000", "00000000000000000000", "187D68E8399F56A2"),
    std::make_tuple("0000000100000000", "00000000000000000000", "C106571E14D61BDC"),
    std::make_tuple("0000000080000000", "00000000000000000000", "847C58790F189880"),
    std::make_tuple("0000000040000000", "00000000000000000000", "666DA3B9C032B156"),
    std::make_tuple("0000000020000000", "00000000000000000000", "1C2038020C12FF92"),
    std::make_tuple("0000000010000000", "00000000000000000000", "3A264F89FC56FDC2"),
    std::make_tuple("0000000008000000", "00000000000000000000", "D0D02C6739B21F5F"),
    std::make_tuple("0000000004000000", "00000000000000000000", "9681D904EDACCD90"),
    std::make_tuple("0000000002000000", "00000000000000000000", "3DDD3DB15AEA9F87"),
    std::make_tuple("0000000001000000", "00000000000000000000", "E6B6B4E93ADFEDE7"),
    std::make_tuple("0000000000800000", "00000000000000000000", "83A77271F09A3E65"),
    std::make_tuple("0000000000400000", "00000000000000000000", "D1CB1AD2F46D205F"),
    std::make_tuple("0000000000200000", "00000000000000000000", "664C52E94CCBE231"),
    std::make_tuple("0000000000100000", "00000000000000000000", "EF79345A2C326314"),
    std::make_tuple("0000000000080000", "00000000000000000000", "A862757D588844A8"),
    std::make_tuple("0000000000040000", "00000000000000000000", "620846438052FF6C"),
    std::make_tuple("0000000000020000", "00000000000000000000", "5203371B18B0D482"),
    std::make_tuple("0000000000010000", "00000000000000000000", "126684994DA7A90A"),
    std::make_tuple("0000000000008000", "00000000000000000000", "66B269BE5D9C5DAB"),
    std::make_tuple("0000000000004000", "00000000000000000000", "C394E111820BE46A"),
    std::make_tuple("0000000000002000", "00000000000000000000", "065F1E4273C37BB3"),
    std::make_tuple("0000000000001000", "00000000000000000000", "8D211E0BC5B45366"),
    std::make_tuple("0000000000000800", "00000000000000000000", "09E6B9D1D83FA236"),
    std::make_tuple("0000000000000400", "00000000000000000000", "D51889022323412A"),
    std::make_tuple("0000000000000200", "00000000000000000000", "E338DCFA2F9F3DD6"),
    std::make_tuple("0000000000000100", "00000000000000000000", "6AB920CBC084AA26"),
    std::make_tuple("0000000000000080", "00000000000000000000", "50EB2F55752D9D2C"),
    std::make_tuple("0000000000000040", "00000000000000000000", "C37E6168C147D9B7"),
    std::make_tuple("0000000000000020", "00000000000000000000", "1ECCC80C1864C046"),
    std::make_tuple("0000000000000010", "00000000000000000000", "8E20334AA2BD18AA"),
    std::make_tuple("0000000000000008", "00000000000000000000", "D3552687DDB47645"),
    std::make_tuple("0000000000000004", "00000000000000000000", "B83EF4F87BC9AB9B"),
    std::make_tuple("0000000000000002", "00000000000000000000", "F84007D2D00612EA"),
    std::make_tuple("0000000000000001", "00000000000000000000", "4104F9902AC24AF4"),
};

#endif // __SKIPJACKTESTVECTORSSET2__
