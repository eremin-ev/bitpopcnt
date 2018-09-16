#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t rdtsc(void)
{
	unsigned int low, high;

	asm volatile("rdtsc" : "=a" (low), "=d" (high));

	return low | ((uint64_t) high) << 32;
}

int bitcnt_1(unsigned v)
{
	int cnt = 0;
	while (v) {
		cnt += v & 1;
		v >>= 1;
	}
	return cnt;
}

int bitcnt_2(unsigned v)
{
	int cnt = 0;
	while (v) {
		v = v & (v - 1);
		cnt++;
	}
	return cnt;
}

static inline int bitcnt4(int v)
{
	switch (v & 0xf) {
	case 0x0: return 0;
	case 0x1: return 1;
	case 0x2: return 1;
	case 0x3: return 2;
	case 0x4: return 1;
	case 0x5: return 2;
	case 0x6: return 2;
	case 0x7: return 3;
	case 0x8: return 1;
	case 0x9: return 2;
	case 0xa: return 2;
	case 0xb: return 3;
	case 0xc: return 2;
	case 0xd: return 3;
	case 0xe: return 3;
	case 0xf: return 4;
	}
	return 0;
}

int bitcnt_3(unsigned v)
{
	int cnt = 0;

	while (v) {
		cnt += bitcnt4(v & 0xf);
		v >>= 4;
	}

	return cnt;
}

int main(int argc, char **argv, char **envp)
{
	int i, n, N = 256;
	int c_1, c_2, c_3;
	long t_1, t_2, t_3;

	int vi = -2;
	unsigned vu = 1 | (1<<31);

	printf("vi i %i, u %u, x %x\n", (int) vi, (unsigned) vi, vi);
	printf("vu i %i, u %u, x %x\n", (int) vu, (unsigned) vu, vu);

	/* warm-up */
	c_1 = c_2 = c_3 = 0;
	for (i = 0; i < N; i++) {
		c_3 = bitcnt_1(i);
		printf("bitcnt_1 %i %i\n", i, c_3);
		c_1 += c_3;

		//c_1 += bitcnt_2(rand());
	}
	for (i = 0; i < N; i++) {
		c_3 = bitcnt_1(i);
		printf("bitcnt_2 %i %i\n", i, c_3);
		c_2 += c_3;

		//c_2 += bitcnt_2(rand());
	}
	for (i = 0; i < N; i++) {
		c_3 += bitcnt_3(rand());
	}

	printf("t_1\tt_2\tt_3\n");

	/* real timing */
	for (n = 1; n < N; n++) {
		srand(314);
		c_1 = 0;
		t_1 = rdtsc();
		for (i = 0; i < n; i++) {
			c_1 += bitcnt_1(rand());
		}
		t_1 = rdtsc() - t_1;

		srand(314);
		c_2 = 0;
		t_2 = rdtsc();
		for (i = 0; i < n; i++) {
			c_2 += bitcnt_2(rand());
		}
		t_2 = rdtsc() - t_2;

		/*srand(314);
		c_3 = 0;
		t_3 = rdtsc();
		for (i = 0; i < n; i++) {
			c_3 += bitcnt_3(rand());
		}
		t_3 = rdtsc() - t_3;*/

		if (c_1 == c_2 && c_2 == c_3) {
			printf("%.2f\t%.2f\t%.2f\n",
				(double) t_1/t_1,
				(double) t_2/t_1,
				(double) t_3/t_1);
		} else {
			printf("c_1 %i t_1 %lu\n", c_1, t_1);
			printf("c_2 %i t_2 %lu\n", c_2, t_2);
			printf("c_3 %i t_3 %lu\n", c_3, t_3);
			//break;
		}
	}

	return 0;
}
