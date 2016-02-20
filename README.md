# FIBSCookieMonster

Keep an updated version of FIBS Cookie Monster available. This repository contains the original FCM code with some modifications.

- Supporting version 1009 of **CLIP**
- Use dispatch table and function pointers to handle the state.
- Some code cleanup.

**TODO:** Merge the updates form BGO FCM.

The following part of this document is the original of documentation file by Paul Ferguson, except where noted.


## FIBSCookieMonster

*by Paul Ferguson*<br/>
*modifications and updates by Øystein Schønning-Johansen*

FIBS Cookie Monster (FCM) is a simple API (application programming interface) to assign numeric keys--cookies--to messages sent by the FIBS server. It identifies more than 200 distinct messages, as explained in this documentation and the source code.

## Download

FIBS Cookie Monster Source and Documentation Version 1.0 released 1 January 2003, tar format, 50K bytes.

## System Notes

FCM was developed and tested (I use the word "tested" very loosely) on Mac OS X 10.2.3, using Apple's most current gcc based development tools (Dec 2002 release). The code should work on other platforms, including nearly all flavors of Linux, Unix, or Windows. Although FCM is derived from the MacFIBS source code, it is significantly different internally, and may not work under Mac OS 9 (I have not tried it.)

FCM requires the regcomp/regexec/regfree functions to compile and execute extended regular expressions. These functions typically are part of the Standard C Library, refer to your compiler documentation or man pages for more info.

## Programming Notes

The remainder of this page provides a brief overview of FCM's design and use. It also includes several miscellaneous notes, presented in no particular order.

## FIBS Messages

A FIBS message is one line of ASCII text (when using FCM, the message should not include the CR-LF terminator). FIBS messages are the basic building block of communications with the FIBS server.

FIBS Cookie Monster's purpose is to identify FIBS messages. Some messages are identified by specific text patterns (for example "It's your turn to roll or double"), while other messages are identified by where they occur during a FIBS session (the Message of the Day messages sent after logging in). Using FCM, client software can process only the messages it is interested in, ignoring the rest.

FCM performs no lexical or syntactic analysis on the contents of FIBS messages. Some messages contain complex data, for example a `"board:..."` message contains more than 20 numbers containg info about the game state. Other messages, such as errors, contain just text strings. You are responsible for parsing and interpreting message contents, based on the requirements of your application.

FCM assumes your client uses **CLIP**, the **CLI**ent **P**rotocol, to communicate with FIBS. Information about **CLIP** can be found at Andreas Schneider's home page. Note that FCM uses a slightly modified version of clip.h, which is included.

    int FIBSCookie(char *);

`FIBSCookie()` is the principle function you will use. For each message, call `FIBSCookie(theMessage);` to obtain a cookie, which can be used to route the message to the appropriate parts of your application, using whatever notification mechanism you choose (such as a Publish/Subscribe design pattern.)

NOTE: You must call `FIBSCookie()` with every message received from the FIBS server, starting immediately after estabilishing a TCP/IP connection with the server. You cannot selectively call FIBSCookie with some messages, but not others, because the underlying state model may get confused.

If you disconnect and reconnect to the FIBS server, you should call `ResetFIBSCookieMonster();` before reconnecting to reset the state properly. *Øystein: This is done automatically if the cookie is `FIBS_Goodbye` or `FIBS_Timeout`.*

**Malformed Messages**

Clients of FCM may need to handle two special cases, where FIBS messages are not properly separated by line terminator characters. If `FIBSCookie(msg);` returns `FIBS_BAD_Board` or `FIBS_BAD_AcceptDouble`, it means msg is malformed. You must split the message into two separate messages and process them separately.

I'm not sure the cause of or the current status of this problem. While getting FCM ready I was not able to confirm that these malformed messages still occur, so I recommend that you test to see if you are receiving these in your application.

*Note by Øystein: I strongly believe this bug has been found and corrected at the FIBS server side. The code below may be redundant.*

The following code example shows one possible way to handle malformed messages (note that this code has not been tested.)

    using namespace std;
    
    void ProcessFIBSMessage(string& msg)
    {
      int cookie = FIBSCookie(msg.c_str());
      switch (cookie)
      {
      case FIBS_BAD_Board:
      {   // find end of the board:... message
          string tokens("0123456789:-\r");
          int i = msg.find(":0:");
          assert(i > 0);
          
          int j = msg.find_first_not_of(tokens, i);
          assert(j > 0);
          while (msg[--j] != ':')
              ;
          j += 2;
          
          string firstMsg(msg.substr(0, j-1));
          ProcessFIBSMessage(firstMsg);
          
          string secondMsg(msg.substr(j));
          ProcessFIBSMessage(secondMsg);
          return;
      }
      break;
      case FIBS_BAD_AcceptDouble:
      {
          string tokens("0123456789.");
          int i = msg.find("cube shows ");
          assert(i > 0);
          i += 11;	// after the find string
          int j = msg.find_first_not_of(tokens, i);
          assert(j > 0);
          string firstMsg(msg.substr(0, j-1));
          ProcessFIBSMessage(firstMsg);
          string secondMsg(msg.substr(j));
          ProcessFIBSMessage(secondMsg);
          return;
      }
      break;
      default:
      // process cookie and msg normally
      }
    }

**Telnet? We don't need no stinking Telnet!**

When you are using CLIP, FIBS does not send any telnet negotiation messages (after login, the server does send several telnet negotiation requests to non-CLIP clients.) Hence, you don't need to scan for telnet bytes in incoming FIBS messages, which makes things easier.

Enjoy!

\<fergy/\>
 
