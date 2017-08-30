![Picture](http://i.imgur.com/ncbKAVY.png)

# Trap Receiver

Windows SNMP Trap Receiver with a plethora of features to facilitate or enhance your fault manangement system(s).

### Getting Started

This code base is currently Windows only.  The solution and project files, with the exception of "Setup", are Visual Studio 2012 based.

The Setup project is based on Visual Studio 2005.  Setup type projects have changed significantly since VS 2005, and migration of this project is not supported.  Setup is only needed to build a distributed installer, so this should not be a big issue but definitely something we want to address.

### Prerequisites

Trap Receiver uses sendEmail.exe from the awesome dudes at caspian.dotconf.net.  Get it <a href="http://caspian.dotconf.net/menu/Software/SendEmail/">here</a>

It also relies on a number of other bits of software.  For example, the SNMP packet library available <a href="http://github.com/bigmacd/pktlib">here</a>, and the two projects (mibparse and mibdb) available <a href="http://github.com/bigmacd/mibparse">here</a>.

Overall there is a lot of machinery and it is a bit of a hassle to get up and running, but hopefully we can fix that!.


### Installing

Clone or download the zip file.  Start Visual Studio by:
* Type "start TrapRcvr.sln" or
* Double-click the TrapRcvr.sln file in Windows Explorer.

### Building

* Build->Build Solution or F7

### Running

There are two main components:
* The Windows Service
* The Main GUI Window

#### The Windows Service

This is the heart of Trap Receiver's functionality.  You can run in it in debug, in which case it act like a console app.  You can run it as a service.  From the command line, enter "trthread -i" and it will install itself as a windows service.  Verify using "trthread -v" and remove using "trthread -r".  Start the service with "net start TrapRcvr" and stop it with "net stop TrapRcvr" (the Windows Service applet is an option too).

The service reads configuration information from the registry (written there by the Main Window GUI).  

#### The Main Window GUI

The GUI does display traps received (while the GUI is running).  It also executes actions which need to interact with the desktop (for example, playing a sound).  Most importantly, it provides an, um, easy way to configure the application.

#### Awful(ly) Old High Level Diagrams:

Overall:

![Picture](http://i.imgur.com/K4WJHP2.png)

Trap Listener:

![Picture](http://i.imgur.com/Na2t1iq.png)

Action Manager:

![Picture](http://i.imgur.com/f72Eeub.png)

Filter:

![Picture](http://i.imgur.com/qyxSHNj.png)

Logger: 

![Picture](http://i.imgur.com/mSUDue6.png)

GUI:

![Picture](http://i.imgur.com/j0KzM7X.png)

### Contributing

Please read CONTRIBUTING.md for details on our code of conduct, and the process for submitting pull requests to us.

### Setup

Versioning

We use SemVer for versioning. For the versions available, see the tags on this repository.

### Authors

Martin Cooley


License

This project is licensed under the MIT License - see the LICENSE.md file for details

Acknowledgments

Many thanks to Brandon Zehm and his sendEmail.exe.  It is an awesome drop-in replacement for the previous paid 3rd party library and was key to allowing Trap Receiver to move to open source.