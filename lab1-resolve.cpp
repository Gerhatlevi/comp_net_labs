/********************************************************* -- SOURCE -{{{1- */
/** Translate host name into IPv4
 *
 * Resolve IPv4 address for a given host name. The host name is specified as
 * the first command line argument to the program.
 *
 * Build program:
 *  $ g++ -Wall -g -o resolve <file>.cpp
 */
/******************************************************************* -}}}1- */

#include <netinet/in.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include <assert.h>
#include <limits.h>
#include <unistd.h>


//--//////////////////////////////////////////////////////////////////////////
//--    local declarations          ///{{{1///////////////////////////////////

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void print_usage( const char* aProgramName );

//--    local config                ///{{{1///////////////////////////////////

/* HOST_NAME_MAX may be missing, e.g. if you're running this on an MacOS X
 * machine. In that case, use MAXHOSTNAMELEN from <sys/param.h>. Otherwise
 * generate an compiler error.
 */
#if !defined(HOST_NAME_MAX)
#	if defined(__APPLE__)
#		include <sys/param.h>
#		define HOST_NAME_MAX MAXHOSTNAMELEN
#	else  // !__APPLE__
#		error "HOST_NAME_MAX undefined!"
#	endif // ~ __APPLE__
#endif // ~ HOST_NAME_MAX

//--    main()                      ///{{{1///////////////////////////////////
int main( int aArgc, char* aArgv[] )
{
	// Check if the user supplied a command line argument.
	if( aArgc != 2 )
	{
		print_usage( aArgv[0] );
		return 1;
	}

	// The (only) argument is the remote host that we should resolve.
	const char* remoteHostName = aArgv[1];

	// Get the local host's name (i.e. the machine that the program is
	// currently running on).
	const size_t kHostNameMaxLength = HOST_NAME_MAX+1;
	char localHostName[kHostNameMaxLength];

	if( -1 == gethostname( localHostName, kHostNameMaxLength ) )
	{
		perror( "gethostname(): " );
		return 1;
	}

	// Print the initial message
	printf( "Resolving `%s' from `%s':\n", remoteHostName, localHostName );

	// Construct the addrinfo struct for hints
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	// use AF_UNSPEC to allow for both IPv4 and IPv6 interfaces
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// pointer to the results from the address info
	addrinfo* ai_res;
	// Use getaddrinfo to lookup the ip address
	int error_code = getaddrinfo(remoteHostName, nullptr, &hints, &ai_res);

	// Handle potential error codes from hostname lookup
	if (error_code) {
	    const char* error_string = gai_strerror(error_code);
		printf("Failed to lookup hostname: %s", error_string);
		return error_code;
	}

	// print out the discovered information
	addrinfo* ai_origin = ai_res;
	while (ai_res) {
    	sockaddr* sockAddr = ai_res->ai_addr;
    	assert(AF_INET == sockAddr->sa_family || AF_INET6 == sockAddr->sa_family);

    	sockaddr_in* inAddr = (sockaddr_in*)sockAddr;

    	// resolve ip int to human readable
        const char* error;
        char readableIPv4[INET_ADDRSTRLEN];
        char readableIPv6[INET_ADDRSTRLEN];

        switch (sockAddr->sa_family) {
            case AF_INET:
               	error = inet_ntop(AF_INET, &inAddr->sin_addr, readableIPv4, INET_ADDRSTRLEN);
                if (!error) {
                    printf("Failed to parse IP address");
                }
               	printf("IPv4: %s\n", readableIPv4);
                break;
            case AF_INET6:
               	error = inet_ntop(AF_INET6, &inAddr->sin_addr, readableIPv6, INET6_ADDRSTRLEN);
                if (!error) {
                    printf("Failed to parse IP address");
                }
               	printf("IPv6: %s\n", readableIPv6);
                break;
            default:
                printf("Protocol Family Not Supported: %d", sockAddr->sa_family);
        }

        ai_res = ai_res->ai_next;
	}

	// free the created object
	freeaddrinfo(ai_origin);

	return 0;
}


//--    print_usage()               ///{{{1///////////////////////////////////
void print_usage( const char* aProgramName )
{
	fprintf( stderr, "Usage: %s <hostname>\n", aProgramName );
}
