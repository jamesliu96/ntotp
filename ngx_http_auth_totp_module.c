#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static char *ntotp_auth(ngx_conf_t *, ngx_command_t *, void *);
static ngx_int_t ntotp_handler(ngx_http_request_t *);

static ngx_command_t  ntotp_commands[] = {
    {
        ngx_string("totp"),
        NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_TAKE1
        ntotp_auth,
        NGX_HTTP_LOC_CONF_OFFSET,
        0,
        NULL
    },
    ngx_null_command
};

static ngx_http_module_t  ntotp_module_ctx = {
    NULL,    /* preconfiguration */
    NULL,    /* postconfiguration */

    NULL,    /* create main configuration */
    NULL,    /* init main configuration */

    NULL,    /* create server configuration */
    NULL,    /* merge server configuration */

    NULL,    /* create location configuration */
    NULL     /* merge location configuration */
};


ngx_module_t  ngx_http_auth_basic_module = {
    NGX_MODULE_V1,
    &ntotp_module_ctx,    /* module context */
    ntotp_commands,       /* module directives */
    NGX_HTTP_MODULE,      /* module type */
    NULL,                 /* init master */
    NULL,                 /* init module */
    NULL,                 /* init process */
    NULL,                 /* init thread */
    NULL,                 /* exit thread */
    NULL,                 /* exit process */
    NULL,                 /* exit master */
    NGX_MODULE_V1_PADDING
};

static char *
ntotp_auth(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_http_core_loc_conf_t *corecf;
    corecf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    corecf->handler = handler;
    return NGX_CONF_OK;
};

static ngx_int_t
ntotp_handler(ngx_http_request_t *req) {
    u_char html[1024] = "test";
    req->headers_out.status = 200;
    int len = sizeof(html) - 1;
    req->headers_out.content_length_n = len;
    ngx_str_set(&req->headers_out.content_type, "text/plain");
    ngx_http_send_header(req);

    ngx_buf_t *b;
    b = ngx_pcalloc(req->pool, sizeof(ngx_buf_t));
    ngx_chain_t out;
    out.buf = b;
    out.next = NULL;
    b->pos = html;
    b->last = html + len;
    b->memory = 1;
    b->last_buf = 1;

    return ngx_http_output_filter(req, &out);
}