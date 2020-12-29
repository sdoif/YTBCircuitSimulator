1st Year Project -- LT Spice Circuit Simulator
==============================================
*Created by Simon Staal, Salman Dhaif and Ruwan Silva*

Welcome to our Spice-like circuit simulator. The aim of this software is to
perform a transient analysis on a circuit of up to 100 nodes, with options to
specify the stop time and time-step of the simulation. DC and sinusoidal voltage
and current sources are supported, with several parameters affecting these. Full
notes of supported components and netlist formatting can be found in
netlist_spec.txt

Our software requires the instillation of the Eigen library for matrix processing.
Simply download the latest release here: http://eigen.tuxfamily.org/index.php?title=Main_Page
To install, put the folder in /usr/local/include/
Running simulator_compile.sh will create a binary of the program called simulator,
you can stream the input netlist via stdin and capture the output via stdout.
