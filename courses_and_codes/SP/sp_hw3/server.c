/* B05902022 張雅信 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <signal.h>
#define TIMEOUT_SEC 5		// timeout in seconds for wait for a connection 
#define MAXBUFSIZE  1024	// timeout in seconds for wait for a connection 
#define NO_USE      0		// status of a http request
#define ERROR	    -1	
#define READING     1		
#define WRITING     2		
#define ERR_EXIT(a) { perror(a); exit(1); }

typedef struct {
    char hostname[512];		// hostname
    unsigned short port;	// port to listen
    int listen_fd;		// fd to wait for a new connection
} http_server;
typedef struct {
    int conn_fd;		// fd to talk with client
    int status;			// not used, error, reading (from client)
                                // writing (to client)
    char file[MAXBUFSIZE];	// requested file
    char query[MAXBUFSIZE];	// requested query
    char host[MAXBUFSIZE];	// client host
    char* buf;			// data sent by/to client
    size_t buf_len;		// bytes used by buf
    size_t buf_size; 		// bytes allocated for buf
    size_t buf_idx; 		// offset for reading and writing
} http_request;
static char* logfilenameP;	// log file name
// Forwards
//
static void init_http_server( http_server *svrP,  unsigned short port );
// initailize a http_request instance, exit for error

static void init_request( http_request* reqP );
// initailize a http_request instance

static void free_request( http_request* reqP );
// free resources used by a http_request instance

static int read_header_and_file( http_request* reqP, int *errP );
// return 0: success, file is buffered in retP->buf with retP->buf_len bytes
// return -1: error, check error code (*errP)
// return 1: continue to it until return -1 or 0
// error code: 
// 1: client connection error 
// 2: bad request, cannot parse request
// 3: method not implemented 
// 4: illegal filename
// 5: illegal query
// 6: file not found
// 7: file is protected

static void set_ndelay( int fd );
// Set NDELAY mode on a socket.

static void add_to_buf( http_request *reqP, char* str, size_t len );
static void strdecode( char* to, char* from );
static int hexit( char c );
static char* get_request_line( http_request *reqP );
static void* e_malloc( size_t size );
static void* e_realloc( void* optr, size_t size );

int pids[1200];
int rpfd[2];
int dead_child=0;
char topipe[1000];
static void sighandler(int signum){
    //fprintf(stderr, "enter sighandler\n" );
    if(signum==SIGUSR1){
        //printf("SIGUSR1 recieved\n");
        //memset(topipe,0,sizeof(topipe));
        //int len;
        //sprintf(topipe,"%d process(es) died previously.\nPIDs of Running Processes:\n",dead_child);
        //for(int i=0;i<1200;i++)
        //    if(pids[i]!=0) sprintf(topipe,"%d\n",pids[i]);
        //write(infopfd[1],topipe,sizeof(topipe));
        //write(1,topipe,strlen(topipe));
    }
    return;
}
void add_header(http_request* reqP,int status){
    int buflen;
    char buf[10000];
    char timebuf[100];
    time_t now;
    reqP->buf_len = 0;
    memset(reqP->buf,reqP->buf_size,0);
    if(status==400)
        buflen = snprintf( buf, sizeof(buf), "HTTP/1.1 400 Bad Request\015\012Server: SP TOY\015\012" );
    else if(status==404)
        buflen = snprintf( buf, sizeof(buf), "HTTP/1.1 404 Not Found\015\012Server: SP TOY\015\012" );
    else if(status==200)
        buflen = snprintf( buf, sizeof(buf), "HTTP/1.1 200 OK\015\012Server: SP TOY\015\012" );
    else{
        fprintf(stderr, "add_header: status error\n");
        return;
    }
    add_to_buf( reqP, buf, buflen );
    now = time( (time_t*) 0 );
    (void) strftime( timebuf, sizeof(timebuf), "%a, %d %b %Y %H:%M:%S GMT", gmtime( &now ) );
    buflen = snprintf( buf, sizeof(buf), "Date: %s\015\012", timebuf );
    add_to_buf( reqP, buf, buflen );
    //buflen = snprintf(buf, sizeof(buf), "Content-Length: %ld\015\012", (int64_t) sb.st_size );
    //add_to_buf( reqP, buf, buflen );
    buflen = snprintf( buf, sizeof(buf), "Connection: close\015\012\015\012" );
    add_to_buf( reqP, buf, buflen );  
    return;      
}

int main( int argc, char** argv ) {
// Preparation    
    signal(SIGUSR1,sighandler);
    http_server server;		// http server
    http_request* requestP = NULL;// pointer to http requests from client
    int maxfd;                  // size of open file descriptor table
    struct sockaddr_in cliaddr; // used by accept()
    int clilen;
    int conn_fd;		// fd for a new connection with client
    int err;			// used by read_header_and_file()
    int i, ret, nwritten;
    // Parse args. 
    if ( argc != 3 ) {
        (void) fprintf( stderr, "usage:  %s port# logfile\n", argv[0] );
        exit( 1 );
    }
    logfilenameP = argv[2];
    int startfd;
    startfd = open(logfilenameP, O_RDWR | O_TRUNC | O_CREAT, 0777); 
    write(startfd,"NONE\n",10);
    // Initialize http server
    init_http_server( &server, (unsigned short) atoi( argv[1] ) );
    maxfd = getdtablesize();
    requestP = ( http_request* ) malloc( sizeof( http_request ) * maxfd );
    if ( requestP == (http_request*) 0 ) {
	fprintf( stderr, "out of memory allocating all http requests\n" );
	exit( 1 );
    }
    for ( i = 0; i < maxfd; i ++ )
        init_request( &requestP[i] );
    requestP[ server.listen_fd ].conn_fd = server.listen_fd;
    requestP[ server.listen_fd ].status = READING;
    fprintf( stderr, "\nstarting on %.80s, port %d, fd %d, maxconn %d, logfile %s...\n", server.hostname, server.port, server.listen_fd, maxfd, logfilenameP );

    //variables for select
    int selectreturn;
    fd_set conn_n_pipe;
    fd_set temp;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    FD_ZERO(&conn_n_pipe);
    FD_SET(server.listen_fd,&conn_n_pipe);
    //for(int i=0;i<1024;i++)
    //    if(FD_ISSET(i,&conn_n_pipe)) fprintf(stderr, "%d ",i );
    //variables for matching pipes and conn_fd
    int search_connfd[1200]={};

// Main loop. 
    while (1) {
	// Wait for a connection.
        //fprintf(stderr,"main loop\n");
        memcpy(&temp,&conn_n_pipe,sizeof(conn_n_pipe));
        //fprintf(stderr, "before select\n" );
        //for(int i=0;i<1024;i++)
        //    if(FD_ISSET(i,&temp)) fprintf(stderr, "%d ",i );
        selectreturn = select(1024,&temp,NULL,NULL,&timeout);
        if(selectreturn==0)
            continue;
        if(FD_ISSET(server.listen_fd,&temp)){
        	clilen = sizeof(cliaddr);
        	conn_fd = accept( server.listen_fd, (struct sockaddr *) &cliaddr, (socklen_t *) &clilen );
        	if ( conn_fd < 0 ) {
        	    if ( errno == EINTR || errno == EAGAIN ) continue; // try again 
        	    if ( errno == ENFILE ) {
        	        (void) fprintf( stderr, "out of file descriptor table ... (maxconn %d)\n", maxfd );
        	        continue;
                    }	
        	    ERR_EXIT( "accept" )
        	}
                requestP[conn_fd].conn_fd = conn_fd;
                requestP[conn_fd].status = READING;		
        	strcpy( requestP[conn_fd].host, inet_ntoa( cliaddr.sin_addr ) );
                set_ndelay( conn_fd );
                fprintf( stderr, "getting a new request... fd %d from %s\n", conn_fd, requestP[conn_fd].host );

        	while (1) {
                // fprintf(stderr,"169\n");
        	    ret = read_header_and_file( &requestP[conn_fd], &err );

        	    if ( ret ==1 ) continue;
        	    else if ( ret < 0 ) {
        	        // error for reading http header or requested file
                    fprintf( stderr, "error on fd %d, code %d\n", 
                    requestP[conn_fd].conn_fd, err );
                    requestP[conn_fd].status = ERROR;
        	        close( requestP[conn_fd].conn_fd );
        	        free_request( &requestP[conn_fd] );
                    break;
        	    } else if ( ret == 0 ) {
        	        // ready for writing
                    fprintf( stderr, "writing (buf %p, idx %d) %d bytes to request fd %d\n", 
                    requestP[conn_fd].buf, (int) requestP[conn_fd].buf_idx,
                    (int) requestP[conn_fd].buf_len, requestP[conn_fd].conn_fd );
        		// write once only and ignore error
        	        nwritten = write( requestP[conn_fd].conn_fd, requestP[conn_fd].buf, requestP[conn_fd].buf_len );
                    //nwritten = write( 1, requestP[conn_fd].buf, requestP[conn_fd].buf_len );
                    fprintf( stderr, "complete writing %d bytes on fd %d\n", nwritten, requestP[conn_fd].conn_fd );
        	        close( requestP[conn_fd].conn_fd );
        	        free_request( &requestP[conn_fd] );
                    break;
        	    }
                else {
                    fprintf(stderr,"add fdset to listen:pipe=%d\n",ret);
                    FD_SET(ret,&conn_n_pipe);
                    search_connfd[ret]=conn_fd;
                    break;
                }    	
            }
        }
        else{ // pipe selected
            //fprintf(stderr, "pipe selected\n");
            int read_pipe=-1;
            int buflen;
            char buf[10000];
            char timebuf[100];
            time_t now;
            for(int i=0;i<maxfd;i++)
                if(FD_ISSET(i,&temp)){
                    read_pipe = i;
                    break;
                }
            if(read_pipe<0){
                fprintf(stderr, "error:read_pipe\n" );
                break;
            }
            int tempconnfd = search_connfd[read_pipe];  
            int CGIstat; 
            waitpid(pids[read_pipe],&CGIstat,0);
            dead_child++;
            pids[read_pipe]=0;
            CGIstat = WEXITSTATUS(CGIstat);
            //printf("%d\n",CGIstat);
            if(CGIstat==0){
                add_header(&requestP[tempconnfd],200);     
                buflen = read(read_pipe,buf,sizeof(buf));
                add_to_buf( &requestP[tempconnfd], buf, buflen );
            }
            else if(CGIstat==1){
                //404 not found
                add_header(&requestP[tempconnfd],404);     
                buflen = snprintf( buf, sizeof(buf), "file not found\n" );
                add_to_buf( &requestP[tempconnfd], buf, buflen );
            }
            fprintf( stderr, "writing (buf %p, idx %d) %d bytes to request fd %d\n", 
            requestP[tempconnfd].buf, (int) requestP[tempconnfd].buf_idx,
            (int) requestP[tempconnfd].buf_len, requestP[tempconnfd].conn_fd );
            //fprintf(stderr, "fd=%d\n",requestP[tempconnfd].conn_fd );
            nwritten = write( requestP[tempconnfd].conn_fd, requestP[tempconnfd].buf, requestP[tempconnfd].buf_len );
            //nwritten = write( 1, requestP[tempconnfd].buf, requestP[tempconnfd].buf_len );
            //fprintf(stderr, "\nPRINT BUF(2):\n%s\n",requestP[tempconnfd].buf );
            fprintf( stderr, "complete writing %d bytes on fd %d\n", nwritten, requestP[tempconnfd].conn_fd );
            close( requestP[tempconnfd].conn_fd );
            free_request( &requestP[tempconnfd] );
            close(read_pipe);
            search_connfd[read_pipe]=0;
            FD_CLR(read_pipe,&conn_n_pipe);
        }
        //fprintf(stderr,"291\n");
    }
    free( requestP );
    return 0;
}


// ======================================================================================================
// You don't need to know how the following codes are working


static void init_request( http_request* reqP ) {
    reqP->conn_fd = -1;
    reqP->status = 0;		// not used
    reqP->file[0] = (char) 0;
    reqP->query[0] = (char) 0;
    reqP->host[0] = (char) 0;
    reqP->buf = NULL;
    reqP->buf_size = 0;
    reqP->buf_len = 0;
    reqP->buf_idx = 0;
}

static void free_request( http_request* reqP ) {
    if ( reqP->buf != NULL ) {
	free( reqP->buf );
	reqP->buf = NULL;
    }
    init_request( reqP );
}


#define ERR_RET( error ) { *errP = error; return -1; }
// return 0: success, file is buffered in retP->buf with retP->buf_len bytes
// return -1: error, check error code (*errP)
// return 1: read more, continue until return -1 or 0
// error code: 
// 1: client connection error 
// 2: bad request, cannot parse request
// 3: method not implemented 
// 4: illegal filename
// 5: illegal query
// 6: file not found
// 7: file is protected
//
static int read_header_and_file( http_request* reqP, int *errP ) {
    // Request variables
    char* file = (char *) 0;
    char* path = (char *) 0;
    char* query = (char *) 0;
    char* protocol = (char *) 0;
    char* method_str = (char *) 0;
    int r, fd;
    struct stat sb;
    char timebuf[100];
    int buflen;
    char buf[10000];
    time_t now;
    void *ptr;

    // Read in request from client
    while (1) {
	r = read( reqP->conn_fd, buf, sizeof(buf) );
	if ( r < 0 && ( errno == EINTR || errno == EAGAIN ) ) return 1;
	if ( r <= 0 ) ERR_RET( 1 )
	add_to_buf( reqP, buf, r );
	if ( strstr( reqP->buf, "\015\012\015\012" ) != (char*) 0 ||
	     strstr( reqP->buf, "\012\012" ) != (char*) 0 ) break;
    }
    // fprintf( stderr, "header: %s\n", reqP->buf );

    // Parse the first line of the request.
    method_str = get_request_line( reqP );
    if ( method_str == (char*) 0 ) ERR_RET( 2 )
    path = strpbrk( method_str, " \t\012\015" );
    if ( path == (char*) 0 ) ERR_RET( 2 )
    *path++ = '\0';
    path += strspn( path, " \t\012\015" );
    protocol = strpbrk( path, " \t\012\015" );
    if ( protocol == (char*) 0 ) ERR_RET( 2 )
    *protocol++ = '\0';
    protocol += strspn( protocol, " \t\012\015" );
    query = strchr( path, '?' );
    if ( query == (char*) 0 )
	query = "";
    else
	*query++ = '\0';

    if ( strcasecmp( method_str, "GET" ) != 0 ) ERR_RET( 3 )
    else {
        strdecode( path, path );
        if ( path[0] != '/' ) ERR_RET( 4 )
	else file = &(path[1]);
    }

    if ( strlen( file ) >= MAXBUFSIZE-1 ) ERR_RET( 4 )
    if ( strlen( query ) >= MAXBUFSIZE-1 ) ERR_RET( 5 )
	  
    strcpy( reqP->file, file );
    strcpy( reqP->query, query );
//fprintf(stderr, "%s, %s\n",file,query );
//// HANDLE READFILE ////
    int validfile=0;
    int validquery=0;
    char v[63]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789";
    for(int j=0;j<63;j++){
        for(int i=0;i<strlen(file);i++)
            if(file[i]==v[j]) validfile++;
        for(int i=9;i<strlen(query);i++)
            if(query[i]==v[j]) validquery++;
    }
   // fprintf(stderr, "%d %d %d %d %s %s\n",validfile,strlen(file),validquery,strlen(query)-9,file,query );
//// HANDLE 400 ////
    if((validquery!=strlen(query)-9 && validquery!=0) || validfile!=strlen(file)){
        //400
        add_header(reqP,400);
        buflen = snprintf( buf, sizeof(buf), "invalid name\n" );
        add_to_buf( reqP, buf, buflen );        
        //fprintf(stderr, "INFO PRINT BUF:\n%s",reqP->buf );
        return 0;
    }
    char checkcommand[9]="filename=";
    //file_reader1:fast ,2:slow
    if(strncmp(query,checkcommand,9)==0 && strcmp(file,"file_reader1")==0){
        //fprintf(stderr, "enter readfile\n");
        char pwdfilename[MAXBUFSIZE];
        sprintf(pwdfilename,"%s",&query[9]);
        if(fork()==0){
            if(execlp("./mmap_write","./mmap_write",pwdfilename,logfilenameP,(char *)0)<0) 
                fprintf(stderr, "exec error\n");
            fprintf(stderr, "exec error\n" );
        }
        //fprintf(stderr, "READ:after exec to mmap write\n" );
        pipe(rpfd);
        int pid = fork();
        if(pid==0){
            close(rpfd[0]);
            dup2(rpfd[1],1);
            if(execlp("./file_reader1","./file_reader1",pwdfilename,(char *)0)<0) 
                fprintf(stderr, "exec error\n");
            fprintf(stderr, "exec error\n" );
        }
        else{
            close(rpfd[1]);
            pids[rpfd[0]]=pid;
            //fprintf(stderr, "before return\n");
            return rpfd[0];
       }
    }
    else if(strncmp(query,checkcommand,9)==0 && strcmp(file,"file_reader2")==0){
        //fprintf(stderr, "enter readfile\n");
        char pwdfilename[MAXBUFSIZE];
        sprintf(pwdfilename,"%s",&query[9]);
        if(fork()==0){
            if(execlp("./mmap_write","./mmap_write",pwdfilename,logfilenameP,(char *)0)<0) 
                fprintf(stderr, "exec error\n");
            fprintf(stderr, "exec error\n" );
        }
        //fprintf(stderr, "READ:after exec to mmap write\n" );
        pipe(rpfd);
        int pid = fork();
        if(pid==0){
            close(rpfd[0]);
            dup2(rpfd[1],1);
            if(execlp("./file_reader2","./file_reader2",pwdfilename,(char *)0)<0) 
                fprintf(stderr, "exec error\n");
            fprintf(stderr, "exec error\n" );
        }
        else{
            close(rpfd[1]);
            pids[rpfd[0]]=pid;
            //fprintf(stderr, "before return\n");
            return rpfd[0];
       }
    }
//// HANDLE INFO ////    
    else if(strcmp(file,"info")==0) {         //info
        //fprintf(stderr, "enter info(380)\n" );
        int infopfd[2];
        pipe(infopfd);
        if(fork()==0){
            kill(getppid(),SIGUSR1);
            close(infopfd[0]);
            dup2(infopfd[1],1);
            if(execlp("./mmap_read","./mmap_read",logfilenameP,(char *)0)<0) 
                fprintf(stderr, "exec error\n");
            fprintf(stderr, "exec error\n" );
        }
        //pause();
        close(infopfd[1]);
        add_header(reqP,200);
        memset(topipe,0,sizeof(topipe));
        int len;
        sprintf(topipe,"%d process(es) died previously.\nPIDs of Running Processes:\n",dead_child);
        buflen = snprintf( buf, sizeof(buf), "%s",topipe );
        add_to_buf( reqP, buf, buflen );   
        //fprintf(stderr, "Dead Child:%d\n", dead_child);             
        for(int i=0;i<1024;i++)
            if(pids[i]!=0){
                buflen = snprintf( buf, sizeof(buf), "%d ",pids[i] );
                add_to_buf( reqP, buf, buflen );
            } 

        char infobuf[2000];
        memset(infobuf,0,sizeof(infobuf));
        read(infopfd[0],infobuf,1000);
        //fprintf(stderr, "after read:infobuf=%s\n",infobuf );
        buflen = snprintf( buf, sizeof(buf), "\n%s",infobuf);
        add_to_buf( reqP, buf, buflen );                

        return 0;
/*
	ptr = mmap( 0, (size_t) sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0 );
	if ( ptr == (void*) -1 ) ERR_RET( 8 )
        add_to_buf( reqP, ptr, sb.st_size );
	(void) munmap( ptr, sb.st_size );
	close( fd );
*/
	// printf( "\nbuf:\n%s\n", reqP->buf );
	// fflush( stdout );
//	reqP->buf_idx = 0; // writing from offset 0
//	return 0;
    }

//// HANDLE 404 ////
    else{
        add_header(reqP,404);
        buflen = snprintf(buf,sizeof(buf),"CGI program not found\n");
        add_to_buf( reqP, buf, buflen );        
        //fprintf(stderr, "INFO PRINT BUF:\n%s",reqP->buf );
        return 0;
    }

        // for file request, read it in buf
/*
        r = stat( reqP->file, &sb );
        if ( r < 0 ) ERR_RET( 6 )

        fd = open( reqP->file, O_RDONLY );
        if ( fd < 0 ) ERR_RET( 7 )
*/
    return 0;
}


static void add_to_buf( http_request *reqP, char* str, size_t len ) { 
    char** bufP = &(reqP->buf);
    size_t* bufsizeP = &(reqP->buf_size);
    size_t* buflenP = &(reqP->buf_len);

    if ( *bufsizeP == 0 ) {
	*bufsizeP = len + 500;
	*buflenP = 0;
	*bufP = (char*) e_malloc( *bufsizeP );
    } else if ( *buflenP + len >= *bufsizeP ) {
	*bufsizeP = *buflenP + len + 500;
	*bufP = (char*) e_realloc( (void*) *bufP, *bufsizeP );
    }
    (void) memmove( &((*bufP)[*buflenP]), str, len );
    *buflenP += len;
    (*bufP)[*buflenP] = '\0';
}

static char* get_request_line( http_request *reqP ) { 
    int begin;
    char c;

    char *bufP = reqP->buf;
    int buf_len = reqP->buf_len;

    for ( begin = reqP->buf_idx ; reqP->buf_idx < buf_len; ++reqP->buf_idx ) {
	c = bufP[ reqP->buf_idx ];
	if ( c == '\012' || c == '\015' ) {
	    bufP[reqP->buf_idx] = '\0';
	    ++reqP->buf_idx;
	    if ( c == '\015' && reqP->buf_idx < buf_len && 
	        bufP[reqP->buf_idx] == '\012' ) {
		bufP[reqP->buf_idx] = '\0';
		++reqP->buf_idx;
	    }
	    return &(bufP[begin]);
	}
    }
    fprintf( stderr, "http request format error\n" );
    exit( 1 );
}



static void init_http_server( http_server *svrP, unsigned short port ) {
    struct sockaddr_in servaddr;
    int tmp;

    gethostname( svrP->hostname, sizeof( svrP->hostname) );
    svrP->port = port;
   
    svrP->listen_fd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( svrP->listen_fd < 0 ) ERR_EXIT( "socket" )

    bzero( &servaddr, sizeof(servaddr) );
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl( INADDR_ANY );
    servaddr.sin_port = htons( port );
    tmp = 1;
    if ( setsockopt( svrP->listen_fd, SOL_SOCKET, SO_REUSEADDR, (void*) &tmp, sizeof(tmp) ) < 0 ) 
	ERR_EXIT ( "setsockopt " )
    if ( bind( svrP->listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr) ) < 0 ) ERR_EXIT( "bind" )

    if ( listen( svrP->listen_fd, 1024 ) < 0 ) ERR_EXIT( "listen" )
}

// Set NDELAY mode on a socket.
static void set_ndelay( int fd ) {
    int flags, newflags;

    flags = fcntl( fd, F_GETFL, 0 );
    if ( flags != -1 ) {
	newflags = flags | (int) O_NDELAY; // nonblocking mode
	if ( newflags != flags )
	    (void) fcntl( fd, F_SETFL, newflags );
    }
}   

static void strdecode( char* to, char* from ) {
    for ( ; *from != '\0'; ++to, ++from ) {
	if ( from[0] == '%' && isxdigit( from[1] ) && isxdigit( from[2] ) ) {
	    *to = hexit( from[1] ) * 16 + hexit( from[2] );
	    from += 2;
	} else {
	    *to = *from;
        }
    }
    *to = '\0';
}


static int hexit( char c ) {
    if ( c >= '0' && c <= '9' )
	return c - '0';
    if ( c >= 'a' && c <= 'f' )
	return c - 'a' + 10;
    if ( c >= 'A' && c <= 'F' )
	return c - 'A' + 10;
    return 0;           // shouldn't happen
}


static void* e_malloc( size_t size ) {
    void* ptr;

    ptr = malloc( size );
    if ( ptr == (void*) 0 ) {
	(void) fprintf( stderr, "out of memory\n" );
	exit( 1 );
    }
    return ptr;
}


static void* e_realloc( void* optr, size_t size ) {
    void* ptr;

    ptr = realloc( optr, size );
    if ( ptr == (void*) 0 ) {
	(void) fprintf( stderr, "out of memory\n" );
	exit( 1 );
    }
    return ptr;
}
