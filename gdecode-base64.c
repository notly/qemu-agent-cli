#include <glib.h>
int main(int argc, char *argv[])
{
guchar *buf = NULL;
gsize size = 0;

if (argc != 2) {
g_print("Usage: %s <base64-encoded-string>\n",argv[0]);
return(-1);
}
buf = g_base64_decode(argv[1], &size);
if (buf == NULL) {
g_print("b_base64_decode error: %s",argv[1]);
return(-1);
}
g_print("%s\n", buf);

return(0);
} 
