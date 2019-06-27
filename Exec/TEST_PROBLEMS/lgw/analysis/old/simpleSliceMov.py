import yt

# Load data as a time series
ts = yt.load("../single_level/plt???????/") # I'd love a way to make this quiet

var = "rhopert";
filenum = 0
fileprefix='testout/'

for ds in ts:
    plot = yt.SlicePlot(ds, "z", var);
    plot.set_log(var, False);
    plot.set_cmap(field=var, cmap='RdBu')
    plot.annotate_title("t = {}".format(ds.current_time) )
    filenum = filenum + 1
    filename = "{}{:04}.png".format(fileprefix,filenum)
    plot.save(name=filename)
