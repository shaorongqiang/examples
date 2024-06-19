#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ethc/abi.h>

#define ok(ethcop) assert(ethcop >= 0)

int main()
{
    uint8_t f1[] = {47, 141, 209, 241, 167, 88, 60, 66, 196, 225, 42, 68, 225, 16, 64, 76, 115, 202, 108, 148, 129, 63, 133, 131, 93, 164, 251, 123, 177, 48, 29, 74};
    uint8_t f2[] = {30, 230, 120, 160, 71, 10, 117, 166, 234, 168, 254, 131, 112, 96, 73, 139, 168, 40, 163, 112, 59, 49, 29, 15, 119, 240, 16, 66, 74, 254, 176, 37};
    uint8_t f3[] = {32, 66, 165, 135, 169, 12, 24, 123, 10, 8, 124, 3, 226, 156, 150, 139, 149, 11, 29, 178, 109, 92, 130, 214, 102, 144, 90, 104, 149, 121, 12, 10};

    struct eth_abi abi;
    char *hex;
    size_t hexlen;

    ok(eth_abi_init(&abi, ETH_ABI_ENCODE));

    ok(eth_abi_array(&abi, NULL));
    ok(eth_abi_bytes32(&abi, f1));
    ok(eth_abi_bytes32(&abi, f2));
    ok(eth_abi_bytes32(&abi, f3));
    ok(eth_abi_array_end(&abi));

    ok(eth_abi_to_hex(&abi, &hex, &hexlen));
    ok(eth_abi_free(&abi));
    printf("encoded abi: %s\n", hex);
    free(hex);

    return 0;
}
