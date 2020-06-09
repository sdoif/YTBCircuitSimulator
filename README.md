1st Year Project -- LT Spice Circuit Simulator

Troubleshooting:
Made some changes to input processing. Experiencing a seg fault for inputs of the type:
Vx node1 node2 value
Ix node1 node2 value

From troubleshooting i've isolated the cause of the segfault to:
line[4].rfind('-',0)==0
no idea why that causes a segfault. The same structure is used for inputs of the form:
Vx node1 node2 sine(value value value)
Ix node1 node2 sine(value value value)
And works without issue there.
