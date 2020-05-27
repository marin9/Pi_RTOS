
void *memset(void *s, int c, int n){
	int p;
	char *m=(char*)s;

	for(p=0;p<n;p++){
		*m++=(char)c;
	}
	return s;
}

void *memcpy(void *dest, void *src, int n){
	int p=0;
	char *d=(char*)dest;
	char *s=(char*)src;

	while(p<n){
		*d++=*s++;
		++p;
	}
	return dest;
}

// Returns -1 if s1<s2, 0 if s1==s2 or 1 if s1>s2
int strcmp(const char *s1, const char *s2){
	int i;
	for(i=0;s1[i] || s2[i];i++){
		if(s1[i]<s2[i]){
			return -1;
		}else if(s1[i]>s2[i]){
			return 1;
		}
	}
	return 0;
}

int strlen(const char *s){
	int i=0;
	while(*s++) ++i;
	return i;
}
