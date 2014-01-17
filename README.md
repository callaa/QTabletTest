Qt tablet tester
-----------------

QTabletTester is a very simple program for testing if a Wacom tablet is working properly with Qt.
Both Qt4 and Qt5 are supported.

## Usage

There are two configuration options in the main menu:

* Mouse tracking: when this is checked, mouse events will be printed on mouse movement, even if no button is pressed
* Ignore mouse during tablet use: this option causes the test program to ignore mouse motion events when the tablet is used. According to Qt documentation, Qt should not be sending mouse events when tablet events are accepted, but it seems to do so anyway (at least some versions)

The **show event log** menu option will pop up a dialog showing the recorded events. **Save event log** will save the event log as a text file. The log can be edited in the dialog before saving.

### Does my tablet work at all?

Draw on window. If the cursor texts says something like "stylus (pen) move at....", then your tablet is recognized by Qt. If it it instead says "mouse move at...", then the tablet is not supported.

Drawing with a tablet will leave a red trail, while mouse will leave a blue trail.
