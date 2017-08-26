//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: MSG_CATEGORY
//
// MessageText:
//
//  Normal
//
#define MSG_CATEGORY                     0x00000001L

//
// MessageId: MSG_000
//
// MessageText:
//
//  The %1 service was installed.
//
#define MSG_000                          0x00000064L

//
// MessageId: MSG_001
//
// MessageText:
//
//  The %1 service was removed.
//
#define MSG_001                          0x00000065L

//
// MessageId: MSG_002
//
// MessageText:
//
//  The %1 service could not be removed.
//
#define MSG_002                          0x00000066L

//
// MessageId: MSG_003
//
// MessageText:
//
//  The control handler could not be installed.
//
#define MSG_003                          0x00000067L

//
// MessageId: MSG_004
//
// MessageText:
//
//  The initialization process failed.
//
#define MSG_004                          0x00000068L

//
// MessageId: MSG_005
//
// MessageText:
//
//  The service was started.
//
#define MSG_005                          0x00000069L

//
// MessageId: MSG_006
//
// MessageText:
//
//  The service received an unsupported request.
//
#define MSG_006                          0x0000006AL

//
// MessageId: MSG_007
//
// MessageText:
//
//  Debug: %1
//
#define MSG_007                          0x0000006BL

//
// MessageId: MSG_008
//
// MessageText:
//
//  The service was stopped.
//
#define MSG_008                          0x0000006CL

//
// MessageId: MSG_009
//
// MessageText:
//
//  There are %1 days left in the trial.
//
#define MSG_009                          0x0000006DL

//
// MessageId: MSG_010
//
// MessageText:
//
//  The trial has expired, please register your application.
//
#define MSG_010                          0x0000006EL

