#include <limits.h>
#include <ctype.h>
#include <stdlib.h>

#define ULLONG_MAX U64_MAX

unsigned long long strtoull(const char *nptr, char **endptr, register int base)
{
	register const char *s = nptr;
	register unsigned long long acc;
	register int c;
	register unsigned long long cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * See strtol for comments as to the logic used.
	 */
	do {
		c = *s++;
	} while (isspace(c) != 0);
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+') {
		c = *s++;
	}

	if ((base == 0 || base == 16) && c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}

	if (base == 0) {
		base = c == '0' ? 8 : 10;
	}

	cutoff = (unsigned long long)ULLONG_MAX / (unsigned long long)base;
	cutlim = (unsigned long long)ULLONG_MAX % (unsigned long long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c) != 0) {
			c -= '0';
		} else if (isalpha(c) != 0) {
			c -= isupper(c) != 0 ? 'A' - 10 : 'a' - 10;
		} else {
			break;
		}
		if (c >= base) {
			break;
		}
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim)) {
			any = -1;
		} else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = ULLONG_MAX;
	} else if (neg) {
		acc = -acc;
	}
	if (endptr != NULL) {
		*endptr = (char *)(any ? (char *)s - 1 : nptr);
	}

	return acc;
}
