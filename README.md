JwiK
====

JVM for Arduino based in http://jwik.codeplex.com/

Project Description

The JwiK BriK is and open source programmable wireless 
microcontroller designed to simplify building the Internet 
of Things by combining the most prolific programming 
language in existence today (Java) with a high performance 
radio and low cost 8-bit microcontroller to create a true 
Swiss army knife for networked wireless applications. It is 
as versatile as your imagination is creative.

The JwiK BriK implements a subset of the Java specification 
using a custom class file format optimized for 
microcontrollers. The core libraries are all written in C 
and linked to the Java run-time, allowing application logic 
to be coded in the most prolific programming language in 
existing today and executed on a resource restricted 8-bit 
microcontroller with reasonably good execution speed.

The following Java capabilities ARE NOT supported by JwiK™:
 * Interfaces
 * Threads
 * Exceptions (try/throw/catch/implements)

The JwiK VM uses a modified class file format that 
integrates all classes in a project into one file. It is 
optimized for space and integrated into a single entity to 
allow easy storage and retrieval in microcontroller flash 
and RAM.