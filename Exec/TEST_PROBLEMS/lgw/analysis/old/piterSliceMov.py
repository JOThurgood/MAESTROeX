#
# This doesn't seem to do anything different - the serial loop seems to be auto parallelised
# (if not the loop, the constructing of hte image seems to use multithreading)


import yt
yt.enable_parallelism()

# Load data as a time series
ts = yt.load("../single_level/plt???????/", parallel=20) # I'd love a way to make this quiet

var = "rhopert";
fileprefix='testout/'

for ds in ts.piter():
    plot = yt.SlicePlot(ds, "z", var);
    plot.set_log(var, False);
    plot.set_cmap(field=var, cmap='RdBu')
    plot.annotate_title("t = {}".format(ds.current_time) )
    filenum = int(ds.fullpath[-7:]) # set the index to correspond to the plt file index
    filename = "{}{:07}.png".format(fileprefix,filenum)
    plot.save(filename)
