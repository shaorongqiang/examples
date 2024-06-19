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

        mpz_t x, y, y1, s;
        mpz_init_set_str(x, "123456789123456789", 10);
        mpz_init_set_str(y, "123456789123456789", 10);
        mpz_init_set_str(s, "123456789123456789", 10);

        ok(eth_abi_mpint(&abi, x));
        ok(eth_abi_mpint(&abi, y));
        ok(eth_abi_mpint(&abi, s));

        mpz_clears(x, y, s, NULL);
    }

    ok(eth_abi_to_hex(&abi, &hex, &hexlen));
    ok(eth_abi_free(&abi));

    printf("encoded abi: %s\n", hex);

    free(hex);

    return 0;
}
