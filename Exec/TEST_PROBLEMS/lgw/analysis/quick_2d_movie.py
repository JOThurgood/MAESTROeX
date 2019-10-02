import argparse
import os
import yt
import re

def quick2DMov(var,tsPath,outDir):
    
    #make subdirectory for that variable 
    os.mkdir(outDir+'/'+re.escape(var))

    ts = yt.load(tsPath)
    
    filePath = outDir + '/' + re.escape(var) + "/" 
    filenum = 0
    for ds in ts:
        plot = yt.SlicePlot(ds, "z", var)
        plot.set_log(var, False);
        plot.set_cmap(field=var, cmap='RdBu')
        plot.annotate_title("t = {}".format(ds.current_time) )
        fileName = "{}{:04}.png".format(filePath,filenum)
        plot.save(name=fileName)
        filenum = filenum + 1

    return

parser = argparse.ArgumentParser("quick_2d_movie")
parser.add_argument("varname", help="name of variable to save (eg. rho)", type=str)
parser.add_argument("plotfiles_path", help="path to directory containing a series of plotfiles", type=str)
parser.add_argument("basename", help="base name of the plotfiles (i.e., before the numbers, eg. plt)", type=str)
parser.add_argument("output_path",    help="path to save directory", type=str)
args = parser.parse_args()

quick2DMov(args.varname, args.plotfiles_path+args.basename+'???????/', args.output_path)
