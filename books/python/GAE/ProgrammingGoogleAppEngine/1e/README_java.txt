Running the Java Examples
-------------------------

The java/ directory contains examples for Google App Engine's Java
runtime environment.  In order to run a Java example on your computer,
you need the Java 5 SDK, or a later version, and the Google App Engine
Java SDK.  To build the examples using the build scripts provided, you
also need Apache Ant.

You can get the Java 5 SDK from Sun's website:
  http://java.sun.com/javase/downloads/index.jsp

You can download the App Engine Java SDK as a Zip archive from here:
  http://code.google.com/appengine/downloads.html

You can get Apache Ant here:
  http://ant.apache.org/

To build and run a Java example using Ant, change the current working
directory to the root directory of one of the samples.  Use Ant to
build the "runserver" target to build the sample and start the
development server, setting the "sdk.dir" property to the path of your
App Engine SDK:
  cd java/ch02/clock1
  ant -Dsdk.dir=/path/to/appengine-java-sdk runserver

(The default "sdk.dir" is a relative path, 5 levels up from the
example's directory.  This is the directory that contains
"pgae-examples" if you clone the Mercurial repository.)

Alternatively, you can use the Eclipse IDE and the Google Plugin for
Eclipse to build and run the examples.  Each App Engine Java app needs
a copy of the App Engine JARs in the final WAR directory.  To save
space in this collection, these JARs have been omitted from the
example directories.  In Eclipse, the easiest way to get a sample
running is to create a new App Engine project, copy the source files
from the example into the project, then run (or debug) the project
from within Eclipse.

You can get the Eclipse IDE from the Eclipse website:
  http://www.eclipse.org/

For instructions on installing the Google Plugin for Eclipse, which
includes the App Engine Java SDK, see the Google Plugin website:
  http://code.google.com/eclipse/


Uploading the Java Examples to App Engine
-----------------------------------------

Before you can upload a Java example to App Engine, you must edit the
war/WEB-INF/appengine-web.xml file and change the <application> and
<version> elements so they contain the desired application ID and
version ID:
  <?xml version="1.0" encoding="utf-8"?>
  <appengine-web-app xmlns="http://appengine.google.com/ns/1.0">
    <application>APP-ID</application>
    <version>VERSION-ID</version>
  </appengine-web-app>

If you're using Ant, build the "update" target to build the project
and attempt to upload the app.  If this is the first time you've
uploaded an app, the upload step will fail with an error message, but
do it anyway to build the project:
  cd java/ch02/clock1
  ant update

The "update" target needs you to use the "appcfg update" command
("appcfg.sh update" in Mac OS X or Linux) for the first update so it
can prompt for and store your credentials.  Run this command,
specifying the location of the war/ directory as an argument:
  appcfg update war

"appcfg" prompts you for your Google account email address and
password, and uploads the app.  For future uploads, you can run
"ant update" from the example's root directory to build and upload the
app, and it will use your stored credentials (until they expire).

If you're using Eclipse, you can upload the example by clicking the
App Engine button in the Eclipse toolbar.  Eclipse builds the project,
prompts for your Google Account email address and password, and
uploads the app.

Test the uploaded example by visiting the corresponding appspot.com
URL:
  http://VERSION-ID.latest.APP-ID.appspot.com/

* Note: When you upload an example that performs datastore queries,
  App Engine may need to build indexes requested by the application's
  configuration before it can serve requests for the example
  successfully.  If you get a NeedIndexError, check the Administration
  Console's "Datastore Indexes" to check the status of the index
  building process.


The Java Examples
-----------------

java/ch02/clock1/
    A simple Java app, with a WAR directory structure (war/) and a
    source directory (src/) using a standard layout, and an Apache Ant
    build file with targets for compiling the code and running the
    development server.  The example includes a simple servlet class
    that outputs a dynamic message to the browser.

java/ch02/clock2/
    An example with Google Accounts sign-in and sign-out
    functionality.

java/ch02/clock3/
    A clock application that prompts a signed-in user for a timezone
    offset to customize the display, and remembers the preference for
    future sessions using the datastore.

java/ch02/clock4/
    An improved version of the customizable clock that caches the
    user's preference data using the memcache, resulting in a faster
    user experience.

java/ch03/environment/
    Prints information about the Java environment, such as environment
    variables, system properties, servlet data, the app server
    filesystem, and the request data.

java/ch03/googleaccounts/
    Configuring Google Accounts authentication, in Java.

java/ch03/logging/
    A simple demonstration of the java.util.logging package.

java/ch03/secureconnections/
    Configuring secure connections, in Java.

java/ch03/staticfiles/
    Configuring static files, in Java.

java/ch03/jsp/
    An example using Java ServerPages (JSPs).

java/ch04/booklowlevel/
    Creating "Book" entities using the low-level datastore API.

java/ch04/bookjpa/
    Creating "Book" entities using JPA.

java/ch04/entities/
    Manipulating datastore entities using the low-level datastore API.

java/ch04/types/
    A demonstration of entity property types, including MVPs.

java/ch05/queries/
    Using the low-level datastore API to perform queries.

java/ch06/transactions/
    Performing transactions on entity groups using the low-level datastore API.

java/ch08/ids/
    JPA examples illustrating the three different kinds of primary key fields.

java/ch08/properties/
    Examples of property types.

java/ch08/queries/
    Performing queries using JPQL.

java/ch08/relationships/
    Modeling relationships with JPA.

java/ch08/transactions/
    Using transactions with JPA.

java/ch09/memcache/
    A demonstration of the features of the memcache API.

java/ch10/urlfetch/
    Calling the URL Fetch service, using either URLConnection or URLFetchService.

java/ch11/sendingmail/
    Sending email messages in Java.

java/ch11/receivingmail/
    Receiving email messages in Java.

java/ch11/xmpp/
    Sending and receiving XMPP (chat) messages in Java.

java/ch12/remoteapi/
    A simple app with the remote API handler installed.  See web.xml.
    Additional code samples for using the bulk loader tools are in the
    python/ch12/ directory.

java/ch13/cron/
    A simple example of scheduled task (cron) configuration.

java/ch13/tasks/
    A simple example showing how task queues are configured, and how
    tasks are created.
