/*
    test.c -- Unit test program for GoAhead

    Usage: goahead-test [options] [documents] [endpoints...]
        Options:
        --auth authFile        # User and role configuration
        --home directory       # Change to directory to run
        --log logFile:level    # Log to file file at verbosity level
        --route routeFile      # Route configuration file
        --verbose              # Same as --log stderr:2
        --version              # Output version information

    Copyright (c) All Rights Reserved. See details at the end of the file.
 */

/********************************* Includes ***********************************/

#include    "goahead.h"
#include    "js.h"

/********************************* Defines ************************************/

static int finished = 0;

#undef ME_GOAHEAD_LISTEN
#if ME_PACK_SSL
    #define ME_GOAHEAD_LISTEN "http://127.0.0.1:8080, https://127.0.0.1:4443, http://[::1]:8090, https://[::1]:4453"
#else
    #define ME_GOAHEAD_LISTEN "http://127.0.0.1:8080, http://[::1]:8090"
#endif

/********************************* Forwards ***********************************/

static void initPlatform();
static void logHeader();
static void usage();

static bool testHandler(Webs *wp);
#if ME_GOAHEAD_JAVASCRIPT
static int aspTest(int eid, Webs *wp, int argc, char **argv);
static int bigTest(int eid, Webs *wp, int argc, char **argv);
#endif
static void actionTest(Webs *wp, char *path, char *query);
static void sessionTest(Webs *wp, char *path, char *query);
static void showTest(Webs *wp, char *path, char *query);
#if ME_GOAHEAD_UPLOAD
static void uploadTest(Webs *wp, char *path, char *query);
#endif
#if ME_GOAHEAD_LEGACY
static int legacyTest(Webs *wp, char *prefix, char *dir, int flags);
#endif
#if ME_UNIX_LIKE
static void sigHandler(int signo);
#endif

/*********************************** Code *************************************/

MAIN(goahead, int argc, char **argv, char **envp)
{
    char    *argp, *auth, *home, *documents, *endpoints, *endpoint, *route, *tok, *lspec;
    int     argind;

    route = "route.txt";
    auth = "auth.txt";

    for (argind = 1; argind < argc; argind++) {
        argp = argv[argind];
        if (*argp != '-') {
            break;

        } else if (smatch(argp, "--auth") || smatch(argp, "-a")) {
            if (argind >= argc) usage();
            auth = argv[++argind];

#if ME_UNIX_LIKE && !MACOSX
        } else if (smatch(argp, "--background") || smatch(argp, "-b")) {
            websSetBackground(1);
#endif

        } else if (smatch(argp, "--debugger") || smatch(argp, "-d") || smatch(argp, "-D")) {
            websSetDebug(1);

        } else if (smatch(argp, "--home")) {
            if (argind >= argc) usage();
            home = argv[++argind];
            if (chdir(home) < 0) {
                error("Can't change directory to %s", home);
                exit(-1);
            }
        } else if (smatch(argp, "--log") || smatch(argp, "-l")) {
            if (argind >= argc) usage();
            logSetPath(argv[++argind]);

        } else if (smatch(argp, "--verbose") || smatch(argp, "-v")) {
            logSetPath("stdout:2");

        } else if (smatch(argp, "--route") || smatch(argp, "-r")) {
            route = argv[++argind];

        } else if (smatch(argp, "--version") || smatch(argp, "-V")) {
            printf("%s: %s\n", ME_NAME, ME_VERSION);
            exit(0);

        } else if (*argp == '-' && isdigit((uchar) argp[1])) {
            lspec = sfmt("stdout:%s", &argp[1]);
            logSetPath(lspec);
            wfree(lspec);

        } else {
            usage();
        }
    }
    documents = ME_GOAHEAD_DOCUMENTS;
    if (argc > argind) {
        documents = argv[argind++];
    }
    initPlatform();
    if (websOpen(documents, route) < 0) {
        error("Can't initialize server. Exiting.");
        return -1;
    }
    logHeader();
    if (websLoad(auth) < 0) {
        error("Can't load %s", auth);
        return -1;
    }
    if (argind < argc) {
        while (argind < argc) {
            endpoint = argv[argind++];
            if (websListen(endpoint) < 0) {
                return -1;
            }
        }
    } else {
        endpoints = sclone(ME_GOAHEAD_LISTEN);
        for (endpoint = stok(endpoints, ", \t", &tok); endpoint; endpoint = stok(NULL, ", \t,", &tok)) {
            if (websListen(endpoint) < 0) {
                return -1;
            }
        }
        wfree(endpoints);
    }

    websDefineHandler("test", testHandler, 0, 0, 0);
    websAddRoute("/test", "test", 0);
#if ME_GOAHEAD_LEGACY
    websUrlHandlerDefine("/legacy/", 0, 0, legacyTest, 0);
#endif
#if ME_GOAHEAD_JAVASCRIPT
    websDefineJst("aspTest", aspTest);
    websDefineJst("bigTest", bigTest);
#endif
    websDefineAction("test", actionTest);
    websDefineAction("sessionTest", sessionTest);
    websDefineAction("showTest", showTest);
#if ME_GOAHEAD_UPLOAD
    websDefineAction("uploadTest", uploadTest);
#endif

#if ME_UNIX_LIKE && !MACOSX
    /*
        Service events till terminated
    */
    if (websGetBackground()) {
        if (daemon(0, 0) < 0) {
            error("Can't run as daemon");
            return -1;
        }
    }
#endif
    websServiceEvents(&finished);
    trace(1, "Instructed to exit\n");
    websClose();
    return 0;
}


static void logHeader()
{
    char    home[ME_GOAHEAD_LIMIT_STRING];

    getcwd(home, sizeof(home));
    trace(2, "Configuration for %s", ME_TITLE);
    trace(2, "---------------------------------------------");
    trace(2, "Version:            %s", ME_VERSION);
    trace(2, "BuildType:          %s", ME_DEBUG ? "Debug" : "Release");
    trace(2, "CPU:                %s", ME_CPU);
    trace(2, "OS:                 %s", ME_OS);
    trace(2, "Host:               %s", websGetServer());
    trace(2, "Directory:          %s", home);
    trace(2, "Documents:          %s", websGetDocuments());
    trace(2, "Configure:          %s", ME_CONFIG_CMD);
    trace(2, "---------------------------------------------");
}


static void usage() {
    fprintf(stderr, "\n%s Usage:\n\n"
        "  %s [options] [documents] [IPaddress][:port]...\n\n"
        "  Options:\n"
        "    --auth authFile        # User and role configuration\n"
#if ME_UNIX_LIKE && !MACOSX
        "    --background           # Run as a Unix daemon\n"
#endif
        "    --debugger             # Run in debug mode\n"
        "    --home directory       # Change to directory to run\n"
        "    --log logFile:level    # Log to file file at verbosity level\n"
        "    --route routeFile      # Route configuration file\n"
        "    --verbose              # Same as --log stderr:2\n"
        "    --version              # Output version information\n\n",
        ME_TITLE, ME_NAME);
    exit(-1);
}


void initPlatform() 
{
#if ME_UNIX_LIKE
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);
    signal(SIGKILL, sigHandler);
    #ifdef SIGPIPE
        signal(SIGPIPE, SIG_IGN);
    #endif
#elif ME_WIN_LIKE
    _fmode=_O_BINARY;
#endif
}


#if ME_UNIX_LIKE
static void sigHandler(int signo)
{
    finished = 1;
}
#endif


/*
    Simple handler and route test
    Note: Accesses to "/" are normally remapped automatically to /index.html
 */
static bool testHandler(Webs *wp)
{
    if (smatch(wp->path, "/")) {
        websRewriteRequest(wp, "/home.html");
        /* Fall through */
    }
    return 0;
}


#if ME_GOAHEAD_JAVASCRIPT
/*
    Parse the form variables: name, address and echo back
 */
static int aspTest(int eid, Webs *wp, int argc, char **argv)
{
	char	*name, *address;

	if (jsArgs(argc, argv, "%s %s", &name, &address) < 2) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	return (int) websWrite(wp, "Name: %s, Address %s", name, address);
}


/*
    Generate a large response
 */
static int bigTest(int eid, Webs *wp, int argc, char **argv)
{
    int     i;

    websSetStatus(wp, 200);
    websWriteHeaders(wp, -1, 0);
    websWriteEndHeaders(wp);
    websWrite(wp, "<html>\n");
    for (i = 0; i < 800; i++) {
        websWrite(wp, " Line: %05d %s", i, "aaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbccccccccccccccccccddddddd<br/>\r\n");
    }
    websWrite(wp, "</html>\n");
    websDone(wp);
    return 0;
}
#endif


/*
    Implement /action/actionTest. Parse the form variables: name, address and echo back.
 */
static void actionTest(Webs *wp, char *path, char *query)
{
	char	*name, *address;

	name = websGetVar(wp, "name", NULL);
	address = websGetVar(wp, "address", NULL);
    websSetStatus(wp, 200);
    websWriteHeaders(wp, -1, 0);
    websWriteEndHeaders(wp);
	websWrite(wp, "<html><body><h2>name: %s, address: %s</h2></body></html>\n", name, address);
	websDone(wp);
}


static void sessionTest(Webs *wp, char *path, char *query)
{
	char	*number;

    if (scaselessmatch(wp->method, "POST")) {
        number = websGetVar(wp, "number", 0);
        websSetSessionVar(wp, "number", number);
    } else {
        number = websGetSessionVar(wp, "number", 0);
    }
    websSetStatus(wp, 200);
    websWriteHeaders(wp, -1, 0);
    websWriteEndHeaders(wp);
    websWrite(wp, "<html><body><p>Number %s</p></body></html>\n", number);
    websDone(wp);
}


static void showTest(Webs *wp, char *path, char *query)
{
    WebsKey     *s;

    websSetStatus(wp, 200);
    websWriteHeaders(wp, -1, 0);
    websWriteEndHeaders(wp);
    websWrite(wp, "<html><body><pre>\n");
    for (s = hashFirst(wp->vars); s; s = hashNext(wp->vars, s)) {
        websWrite(wp, "%s=%s\n", s->name.value.string, s->content.value.string);
    }
    websWrite(wp, "</pre></body></html>\n");
    websDone(wp);
}


#if ME_GOAHEAD_UPLOAD
/*
    Dump the file upload details. Don't actually do anything with the uploaded file.
 */
static void uploadTest(Webs *wp, char *path, char *query)
{
    WebsKey         *s;
    WebsUpload  *up;
    char            *upfile;

    websSetStatus(wp, 200);
    websWriteHeaders(wp, -1, 0);
    websWriteHeader(wp, "Content-Type", "text/plain");
    websWriteEndHeaders(wp);
    if (scaselessmatch(wp->method, "POST")) {
        for (s = hashFirst(wp->files); s; s = hashNext(wp->files, s)) {
            up = s->content.value.symbol;
            websWrite(wp, "FILE: %s\r\n", s->name.value.string);
            websWrite(wp, "FILENAME=%s\r\n", up->filename);
            websWrite(wp, "CLIENT=%s\r\n", up->clientFilename);
            websWrite(wp, "TYPE=%s\r\n", up->contentType);
            websWrite(wp, "SIZE=%d\r\n", up->size);
            upfile = sfmt("%s/tmp/%s", websGetDocuments(), up->clientFilename);
            rename(up->filename, upfile);
            wfree(upfile);
        }
        websWrite(wp, "\r\nVARS:\r\n");
        for (s = hashFirst(wp->vars); s; s = hashNext(wp->vars, s)) {
            websWrite(wp, "%s=%s\r\n", s->name.value.string, s->content.value.string);
        }
    }
    websDone(wp);
}
#endif


#if ME_GOAHEAD_LEGACY
/*
    Legacy handler with old parameter sequence
 */
static int legacyTest(Webs *wp, char *prefix, char *dir, int flags)
{
    websSetStatus(wp, 200);
    websWriteHeaders(wp, -1, 0);
    websWriteHeader(wp, "Content-Type", "text/plain");
    websWriteEndHeaders(wp);
    websWrite(wp, "Hello Legacy World\n");
    websDone(wp);
    return 1;
}

#endif

/*
    @copy   default

    Copyright (c) Embedthis Software LLC, 2003-2014. All Rights Reserved.

    This software is distributed under commercial and open source licenses.
    You may use the Embedthis GoAhead open source license or you may acquire 
    a commercial license from Embedthis Software. You agree to be fully bound
    by the terms of either license. Consult the LICENSE.md distributed with
    this software for full details and other copyrights.

    Local variables:
    tab-width: 4
    c-basic-offset: 4
    End:
    vim: sw=4 ts=4 expandtab

    @end
 */
