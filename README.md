# Snowplow

##### Snowplow is the name of our variante, that means "chasse-neige".

## Description of the variante :

The user gives a size of a window that will go over the alignment  (Example : 5)
Step by step, the algorithm will inspect column by column, searching for event. 
If there is an event, then the same event will be searched within the window provided before, further in the alignment.
The column of the farthest match event will be used as a the point to reach : 
	-> The upstream events will be pushed towards this point using gaps, if there is not another event in the middle
	-> To avoid mismatchs, we also add the same number of gaps just after this point

Once this step is done, we tried to regroup the points together and remove the column of only gaps, to clean the over numbered gaps. 

To precise, dots cannot get through a column of event(s). If a dot is on the left of a event(s)'s column, it cannot be pushed on its right.
Furthermore, if a dot is in a column of event(s), it can be pushed on the right just before the next column of event(s).

The algorithm will stop automatically the resulting alignment is the same than the previous alignment of the precedent loop.

## Installation, needed libraries and compilation

Just uncompress the archive to use Snowplow

*Needed libraries are :*

- iostream

- string

- vector

- algorithm

- fstream

- chrono
	
do :
`make` 
to compile the program

## General example of use :

`./launch_gaudin_pommellec_variante_1.py alignment.txt variante_1.par res_dir/`

- alignment.txt : path file of alignment resulting of standard method

elements must be separated with tabulations, with no added spaces or tabulations at the end of the line, one trace by line
with the same number of elements by line(as they are aligned)

- variante_1.par : parameters file of the variante
- res_dir : target directory for the results
	
### From toy_example repertory

Just do :
`./launch_gaudin_pommellec_variante_1_toy_example.py`
To get an example of Snowplow Variante on the file toy_example/standard_n40_l20_hard.txt,
resulting from Standard Alignment.
The outputs will be : 
snowplow_standard_n40_l20_hard.txt
snowplow_standard_n40_l20_hard.csv

## Parameters :

- size_window : gives the size of the window that will cross the alignment, in which the events can be pushed
- nbr_loops_max : maximal number of loops of snowplow

**Default parameters are :**
size_window : 5
nbr_loops_max : 10

*Implemented by Marinna Gaudin and Jolann Pommellec*
