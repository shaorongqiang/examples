#include "c-uzkge.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ethc/abi.h>

#define ok(ethcop) assert(ethcop >= 0)

typedef struct Bytes2
{
    size_t len;
    uint8_t *data;
} Bytes2;

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
                                       (char *)out_outputs, &out_outputs_len,
                                       out_proof, &out_proof_len);
    if (res < 0)
    {
        return res;
    }
    Bytes2 verifier_params_encode = {len : verifier_params.len, data : verifier_params.data};

    Bytes2 inputs_encode[50];
    memset(inputs_encode, 0, sizeof(inputs_encode));
    for (int i = 0; i < inputs_len; i++)
    {
        inputs_encode[i].len = inputs[i].len;
        inputs_encode[i].data = inputs[i].data;
    }

    Bytes2 outputs_encode[50];
    memset(outputs_encode, 0, sizeof(outputs_encode));
    int outputs_len = out_outputs_len;
    for (int i = 0; i < outputs_len; i++)
    {
        outputs_encode[i].len = sizeof(out_outputs[i]);
        outputs_encode[i].data = out_outputs[i];
    }

    Bytes2 commitment_encode = {len : commitment.len, data : commitment.data};
    Bytes2 random_number_encode = {len : random_number.len, data : random_number.data};
    Bytes2 proof_encode = {len : out_proof_len, data : out_proof};

    struct eth_abi abi;
    char *hex;
    size_t hexlen;

    ok(eth_abi_init(&abi, ETH_ABI_ENCODE));

    // uint8_t *verifier_params_bytes[] = {verifier_params_encode.data};
    ok(eth_abi_bytes(&abi, &verifier_params_encode.data, &verifier_params_encode.len));

    // inputs
    ok(eth_abi_array(&abi, NULL));
    for (int i = 0; i < inputs_len; i++)
    {
        ok(eth_abi_bytes(&abi, &inputs_encode[i].data, &inputs_encode[i].len));
    }
    ok(eth_abi_array_end(&abi));

    // outputs
    ok(eth_abi_array(&abi, NULL));
    for (int i = 0; i < outputs_len; i++)
    {
        ok(eth_abi_bytes(&abi, &outputs_encode[i].data, &outputs_encode[i].len));
    }
    ok(eth_abi_array_end(&abi));

    ok(eth_abi_bytes(&abi, &commitment_encode.data, &commitment_encode.len));
    ok(eth_abi_bytes(&abi, &random_number_encode.data, &random_number_encode.len));
    ok(eth_abi_bytes(&abi, &proof_encode.data, &proof_encode.len));

    ok(eth_abi_to_hex(&abi, &hex, &hexlen));
    ok(eth_abi_free(&abi));

    printf("encoded abi: %s\n", hex);

    free(hex);
    return 0;
}
