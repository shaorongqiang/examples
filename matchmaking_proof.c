#include "c-uzkge.h"
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <ethc/abi.h>

#define ok(ethcop) assert(ethcop >= 0)

int main()
{
    char verifier_params_array[10240] = {0};
    int res = __verifier_matchmaking_params(verifier_params_array, sizeof(verifier_params_array));
    if (res < 0)
    {
        return res;
    }
    Bytes verifier_params = {len : res, data : verifier_params_array};

    char rng_seed_array[32] = {0};
    Bytes rng_seed = {len : sizeof(rng_seed_array), data : rng_seed_array};
    {
        time_t t;
        srand((unsigned)time(&t));
        int ret = rand();
        memcpy(rng_seed_array, &ret, sizeof(ret));
    }

    char input_array[50][32] = {{0}};
    Bytes inputs[50];
    memset(inputs, 0, sizeof(inputs));
    int inputs_len = sizeof(input_array) / sizeof(input_array[0]);
    int ret = 0;
    for (int i = 0; i < inputs_len; i++)
    {
        inputs[i].len = sizeof(input_array[i]);
        inputs[i].data = input_array[i];
        ret = rand();
        memcpy(inputs[i].data, &ret, sizeof(ret));
    }

    char committed_seed_array[32] = {0};
    {
        int ret = rand();
        memcpy(committed_seed_array, &ret, sizeof(ret));
    }
    struct Bytes committed_seed = {len : sizeof(committed_seed_array), data : committed_seed_array};

    char committed_array[32] = {0};
    struct Bytes commitment = {len : sizeof(committed_array), data : committed_array};

    {
        Bytes data[] = {committed_seed};
        int res = __anemoi_hash(data, sizeof(data) / sizeof(data[0]), committed_array);
        if (res < 0)
        {
            return res;
        }
    }

    char random_number_array[32] = {0};
    {
        int ret = rand();
        memcpy(random_number_array, &ret, sizeof(ret));
    }
    struct Bytes random_number = {len : sizeof(random_number_array), data : random_number_array};

    char out_outputs[60][32] = {{0}};
    uint32_t out_outputs_len = sizeof(out_outputs) / sizeof(out_outputs[0]);

    char out_proof[20480] = {0};
    uint32_t out_proof_len = sizeof(out_proof);
    res = __generate_matchmaking_proof(rng_seed,
                                       inputs, inputs_len,
                                       committed_seed,
                                       random_number,
                                       out_outputs, &out_outputs_len,
                                       out_proof, &out_proof_len);
    if (res < 0)
    {
        return res;
    }
    Bytes outputs[50];
    memset(outputs, 0, sizeof(outputs));
    int outputs_len = out_outputs_len;
    for (int i = 0; i < outputs_len; i++)
    {
        outputs[i].len = sizeof(out_outputs[i]);
        outputs[i].data = out_outputs[i];
    }

    struct eth_abi abi;
    char *hex;
    size_t hexlen;

    ok(eth_abi_init(&abi, ETH_ABI_ENCODE));

    ok(eth_abi_bytes(&abi, &verifier_params.data, &verifier_params.len));

    // inputs
    ok(eth_abi_array(&abi, NULL));
    for (int i = 0; i < inputs_len; i++)
    {
        ok(eth_abi_bytes(&abi, &inputs[i].data, &inputs[i].len));
    }
    ok(eth_abi_array_end(&abi));

    // outputs
    ok(eth_abi_array(&abi, NULL));
    for (int i = 0; i < outputs_len; i++)
    {
        ok(eth_abi_bytes(&abi, &outputs[i].data, &outputs[i].len));
    }
    ok(eth_abi_array_end(&abi));

    ok(eth_abi_bytes(&abi, &commitment.data, &commitment.len));
    ok(eth_abi_bytes(&abi, &random_number.data, &random_number.len));
    ok(eth_abi_bytes(&abi, &out_proof, &out_proof_len));

    ok(eth_abi_to_hex(&abi, &hex, &hexlen));
    ok(eth_abi_free(&abi));

    printf("encoded abi: %s\n", hex);

    free(hex);
    return 0;
}
