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

# setup the command line parser arguments
parser = argparse.ArgumentParser("quick_2d_movie")
parser.add_argument("varname", help="name of variable to save (eg. rho). Set to 'all' to attempt to make 2d movies of all plotfile variables", type=str)
parser.add_argument("plotfiles_path", help="path to directory containing a series of plotfiles", type=str)
parser.add_argument("basename", help="base name of the plotfiles (i.e., before the numbers, eg. plt)", type=str)
parser.add_argument("output_path",    help="path to save directory", type=str)
args = parser.parse_args()

# Main

tsPath = args.plotfiles_path+args.basename+'???????/'

if args.varname == "all" :
    ts = yt.load(tsPath)
    ds0 = ts[0]
    for item in ds0.field_list:
        var = item[1]
        print('saving var={}'.format(var))
        try:
            quick2DMov(var, tsPath, args.output_path)
        except:
            print('something went wrong with saving var={}, (possibly to do with escaping special characters)'.format(var))
        pass       

else:
    quick2DMov(args.varname, tsPath, args.output_path)

