#include <neon/ne_session.h>
#include <neon/ne_request.h>
#include <stdio.h>

static int
my_verify(void *userdata, int failures, const ne_ssl_certificate *cert)
{
  return 0;
}

int main()
{
  ne_session *s = ne_session_create("https", "translate.google.com",443);
  ne_ssl_set_verify(s,my_verify,"translate.google.com");
  ne_set_useragent(s,"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.101 Safari/537.36");

  //  char *path =   "/translate_tts?ie=UTF-8&q=$line&tl=$lang&total=$lines&idx=$i&textlen=$len&client=t&tk=$token";
  char *path =   "/translate_tts?ie=UTF-8&q=I+Love+You&tl=en&total=1&idx=1&textlen=10&client=t&tk=33388883338";

  ne_request *req = ne_request_create(s, "GET", path);
  ne_add_request_header(req, "ACcept", "*/*");
  ne_add_request_header(req, "Accept-Encoding", "identity;q=1, *;q=0");
  ne_add_request_header(req, "Accept-Language","en-US,en;q=0.5");
  ne_add_request_header(req, "DNT","1");
  ne_add_request_header(req, "Range","bytes=0-");
  ne_add_request_header(req, "Referer","https://translate.google.com/");

  char buf[128];
  FILE *f = fopen("1.mp3","a+");
  int n = ne_begin_request(req);
  if (n)     printf("dispatched result %s \n", ne_get_error(s));
  else while(n=ne_read_response_block(req,buf,sizeof(buf))){
      printf("got block of %d \n", n);
      fwrite(buf, sizeof(buf[0]), n, f);
    }
  ne_end_request(req);
  fclose(f);

  ne_session_destroy(s);

  return 0;
}
