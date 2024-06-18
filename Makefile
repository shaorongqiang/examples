all: anemoi_hash point_add scalar_mul matchmaking_proof shuffle_proof

anemoi_hash:
	gcc anemoi_hash.c -o anemoi_hash -l ethc

point_add:
	gcc point_add.c -o point_add -l ethc -l gmp

scalar_mul:
	gcc scalar_mul.c -o scalar_mul -l ethc -l gmp

matchmaking_proof:
	gcc matchmaking_proof.c -o matchmaking_proof -l ethc -l c_uzkge

shuffle_proof:
	gcc shuffle_proof.c -o shuffle_proof -l ethc -l c_uzkge

clean:
	rm -f anemoi_hash point_add scalar_mul matchmaking_proof shuffle_proof