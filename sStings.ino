


char * sstrcpy(char *dst, char const *src, size_t n) 
{
    char *ret = dst;
    while(n-- > 0) {
        if((*dst++ = *src++) == '\0')
            return ret;
    }
    *dst++ = '\0';
    return ret;
}
