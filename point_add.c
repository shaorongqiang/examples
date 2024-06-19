#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ethc/abi.h>

#define ok(ethcop) assert(ethcop >= 0)

int main()
{
    struct eth_abi abi;
    char *hex;
    size_t hexlen;

    ok(eth_abi_init(&abi, ETH_ABI_ENCODE));
    {

        mpz_t x1, y1, x2, y2;
        mpz_init_set_str(x1, "123456789123456789", 10);
        mpz_init_set_str(y1, "123456789123456789", 10);
        mpz_init_set_str(x2, "123456789123456789", 10);
        mpz_init_set_str(y2, "123456789123456789", 10);

        ok(eth_abi_mpint(&abi, x1));
        ok(eth_abi_mpint(&abi, y1));
        ok(eth_abi_mpint(&abi, x2));
        ok(eth_abi_mpint(&abi, y2));

        mpz_clears(x1, y1, x2, y2, NULL);
    }

    ok(eth_abi_to_hex(&abi, &hex, &hexlen));
    ok(eth_abi_free(&abi));

    printf("encoded abi: %s\n", hex);

    free(hex);

    return 0;
}
