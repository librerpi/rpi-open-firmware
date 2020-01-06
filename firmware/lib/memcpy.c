

void *__memcpy(void *_dst, const void *_src, unsigned len)
{
	unsigned char *dst = _dst;
	const unsigned char *src = _src;
	while(len-- > 0) {
		*dst++ = *src++;
	}
	return _dst;
}

void *
memset(void *s, int c, unsigned int n)
{
	char *s1 = s;
	if (n>0) {
			n++;
			while (--n > 0) {
					*s1++ = c;
			}
	}
	return s;
}