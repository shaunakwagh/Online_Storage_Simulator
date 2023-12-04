////////////////////////////////////////////////////////////////////////////////
//
//  File           : sg_sim.c
//  Description    : This is the main program for the CMPSC311 programming
//                   assignment #2 (beginning of ScatterGather.com Internet
//                   service code)
//
//   Author        : Patrick McDaniel
//   Last Modified : Thu 03 Sep 2020 12:22:02 PM PDT
//

// Include Files
#include <unistd.h>
/*
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cmpsc311_util.h>
#include <cmpsc311_assocarr.h>
#include <cmpsc311_workload.h>
*/
#include <cmpsc311_log.h>

// Project Includes 
#include <sg_defs.h>

// Defines
#define SG_ARGUMENTS "hvul:"
#define USAGE \
	"USAGE: sg_sim [-h] [-v] [-l <logfile>] \n" \
	"\n" \
	"where:\n" \
	"    -h - help mode (display this message)\n" \
	"    -v - verbose output\n" \
	"    -u - perform the unit tests\n" \
	"    -l - write log messages to the filename <logfile>\n" \
	"\n" \

//
// Global Data
int verbose;
unsigned long SGServiceLevel; // Service log level
unsigned long SGDriverLevel; // Controller log level
unsigned long SGSimulatorLevel; // Simulation log level

//
// Functional Prototypes

int simulateScatterGather( void ); // ScatterGather simulation
int sg_unit_test( void ); // The program unit tests
extern int packetUnitTest( void ); // External function (packet processing)

//
// Functions

////////////////////////////////////////////////////////////////////////////////
//
// Function     : main
// Description  : The main function for the ScatterGather simulator
//
// Inputs       : argc - the number of command line parameters
//                argv - the parameters
// Outputs      : 0 if successful test, -1 if failure

int main( int argc, char *argv[] ) {

	// Local variables
	int ch, verbose = 0, log_initialized = 0, unit_tests = 0;
	
	// Process the command line parameters
	while ((ch = getopt(argc, argv, SG_ARGUMENTS)) != -1) {

		switch (ch) {
		case 'h': // Help, print usage
			fprintf( stderr, USAGE );
			return( -1 );

		case 'v': // Verbose Flag
			verbose = 1;
			break;

		case 'u': // Unit test Flag
			unit_tests = 1;
			break;

		case 'l': // Set the log filename
			initializeLogWithFilename( optarg );
			log_initialized = 1;
			break;

		default:  // Default (unknown)
			fprintf( stderr, "Unknown command line option (%c), aborting.\n", ch );
			return( -1 );
		}
	}

	// Setup the log as needed, log levels
	if ( ! log_initialized ) {
		initializeLogWithFilehandle( CMPSC311_LOG_STDERR );
	}
	SGServiceLevel = registerLogLevel("SG_SRVICE", 0); // Service log level
	SGDriverLevel = registerLogLevel("SG_DRIVER", 0); // Controller log level
	SGSimulatorLevel = registerLogLevel("SG_SIMULATOR", 0); // Simulation log level
	if ( verbose ) {
		enableLogLevels(LOG_INFO_LEVEL);
		enableLogLevels(SGServiceLevel | SGDriverLevel | SGSimulatorLevel);
	}

	// If exgtracting file from data
	if (unit_tests) {

		// Run the unit tests
		enableLogLevels( LOG_INFO_LEVEL );
		logMessage(LOG_INFO_LEVEL, "Running unit tests ....");
		if (sg_unit_test() == 0) {
			logMessage(LOG_INFO_LEVEL, "Unit tests completed successfully.\n\n");
		} else {
			logMessage(LOG_ERROR_LEVEL, "Unit tests failed, aborting.\n\n");
		}

	} else {

		// Run the simulation
		if ( simulateScatterGather() == 0 ) {
			logMessage( LOG_INFO_LEVEL, "ScatterGather.com simulation completed successfully!!!\n\n" );
		} else {
			logMessage( LOG_INFO_LEVEL, "ScatterGather.com simulation failed.\n\n" );
		}
	}

	// Return successfully
	return( 0 );
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : simulateScatterGather
// Description  : The main control loop for the processing of the SG
//                simulation (which calls the student code).
//
// Inputs       : none
// Outputs      : 0 if successful test, -1 if failure

int simulateScatterGather( void ) {

	// Return succesfully
	return( 0 );
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : sg_unit_test
// Description  : This is the interface for the global unit test function
//
// Inputs       : none
// Outputs      : 0 if successful test, -1 if failure

int sg_unit_test( void ) {

    // Print a lead in message
    logMessage( LOG_INFO_LEVEL, "ScatterGather: beginning unit tests ..." );

    // Do the UNIT tests
    if ( packetUnitTest() ) {
        logMessage( LOG_ERROR_LEVEL, "ScatterGather: unit tests failed." );
        return( -1 );
    }

    // Return successfully
    logMessage( LOG_INFO_LEVEL, "ScatterGather: exiting unit tests." );
    return( 0 );
}
