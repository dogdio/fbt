#include <string.h>
#include <stdio.h>

#include <openssl/evp.h>
#include <openssl/aes.h>

//
// $ sudo apt-get install openssl libssl-dev
// $ gcc openssl.c -lcrypto
//

void hexdump(unsigned char *buf, int len)
{
	int i;
	printf("## buf=%p, len=%d\n",buf,len);
	for(i = 0; i < len; i++) {
		if(i && i%16==0)
			printf("\n");
		printf("%02x ", buf[i]);
	}
	printf("\n\n");
}

#define BUF_SIZE 48
int main(void)
{
	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
	const unsigned char *key = (const unsigned char *)"1234567890123456";
	const unsigned char *iv = (const unsigned char *)"abcdefghijklmnop";
	unsigned char src[BUF_SIZE];
	unsigned char enc[BUF_SIZE];
	unsigned char dec[BUF_SIZE];
	int slen = sizeof(src);
	int elen = sizeof(enc);
	int dlen = sizeof(dec);
	int i, ret = -1;
	
	for(i = 0; i < sizeof(src); i++)
		src[i] = i;
	hexdump(src, sizeof(src));

	// ECB Encrypt
	EVP_CIPHER_CTX_init(ctx);

	if(EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, iv) != 1) {
		printf("EVP_EncryptInit_ex error\n");
		goto end;
	}
	EVP_CIPHER_CTX_set_padding(ctx, 0);

	if(EVP_EncryptUpdate(ctx, enc, &elen, src, slen) != 1) {
		printf("EVP_EncryptUpdate error elen=%d\n", elen);
		goto end;
	}

	if(EVP_CIPHER_CTX_cleanup(ctx) != 1) {
		printf("EVP_CIPHER_CTX_cleanup error\n");
		goto end;
	}
	hexdump(enc, elen);

	// ECB Decrypt
	memset(dec, 0xff, sizeof(dec));
	EVP_CIPHER_CTX_init(ctx);

	if(EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, iv) != 1) {
		printf("EVP_DecryptInit_ex error\n");
		goto end;
	}
	EVP_CIPHER_CTX_set_padding(ctx, 0);

	if(EVP_DecryptUpdate(ctx, dec, &dlen, enc, elen) != 1) {
		printf("EVP_DecryptUpdate error dlen=%d\n", dlen);
		goto end;
	}

	if(EVP_CIPHER_CTX_cleanup(ctx) != 1) {
		printf("EVP_CIPHER_CTX_cleanup error\n");
		goto end;
	}

	hexdump(dec, dlen);

	if(dlen == sizeof(dec) && memcmp(dec, src, dlen) == 0)
		ret = 0;

	EVP_CIPHER_CTX_free(ctx);
end:
	printf("ret=%d\n", ret);
	return ret;
}


