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
	addrinfo ai_struct = addrinfo{
	    .ai_family = AF_INET,
	    .ai_socktype = SOCK_STREAM,
		.ai_protocol = IPPROTO_TCP
	};

	// pointer to the results from the address info
	addrinfo* ai_res;
	// Use getaddrinfo to lookup the ip address
	int error_code = getaddrinfo(remoteHostName, nullptr, &ai_struct, &ai_res);

	// Handle potential error codes from hostname lookup
	if (error_code) {
	    const char* error_string = gai_strerror(error_code);
		printf("Failed to lookup hostname: %s", error_string);
		return error_code;
	}

	// print out the discovered information
	sockaddr* sockAddr = ai_res->ai_addr;
	assert(AF_INET == sockAddr->sa_family);

	sockaddr_in* inAddr = (sockaddr_in*)sockAddr;
	int port = inAddr->sin_port;
	uint32_t ipNumber = inAddr->sin_addr.s_addr;

	// resolve ip int to human readable for IPv4
	char readableIPv4[INET_ADDRSTRLEN];
	const char* error = inet_ntop(sockAddr->sa_family, sockAddr->sa_data, readableIPv4, inAddr->sin_len);
	error = inet_ntop(sockAddr->sa_family, &inAddr->sin_addr, readableIPv4, inAddr->sin_len);
	printf("IPv4: %s", readableIPv4);

	// free the created object
	freeaddrinfo(ai_res);

	return 0;
}


//--    print_usage()               ///{{{1///////////////////////////////////
void print_usage( const char* aProgramName )
{
	fprintf( stderr, "Usage: %s <hostname>\n", aProgramName );
}
