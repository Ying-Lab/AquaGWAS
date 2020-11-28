getDataFromSciNotation <- function(scinota = "1E2")
{
    vec = as.vector(unlist(strsplit(tolower(scinota), "", fixed = T)))
    eindex = grep('e', vec)
    base = as.integer(substr(scinota, 1, eindex-1))
    expo = as.integer(substr(scinota, eindex+1, nchar(scinota)))
    ret = base * 10 ^ expo
}

drawManhattan <- function(args)
{
    debug = args[1]     # "debugno" or the path of manghattan.R
    inData = args[2]
    out =args[3]
    gwline = args[4]
    sgline = args[5]
    
    genomewide = -log10(getDataFromSciNotation(gwline))
    suggest = -log10(getDataFromSciNotation(sgline))

    data <- read.table(inData, header = T)
    
    data$SNP<-as.character(data$SNP)
    data$CHR<-as.integer(data$CHR)
    
    newData<-na.omit(data)
    
    if (debug == "debugno")
    {
        # Release path.
        path = paste0(getwd(), "/script/qqman/manhattan.R")
        source(path)
    }
    else
    {
        source(debug)
    }

    # when the first chr is 0, data of chr_0 will not be plot, without +1 of vector col;
    color = unique(newData$CHR)
    # Avoid the data of chr_0 can't be plotted.
    color = color + 1
	
    manhattan(newData, output = out, suggestiveline = suggest, genomewideline = genomewide, col = color)   
    write(out)
}

drawQQplot <- function(args)
{
    debug = args[1]     # "debugno" or the path of qq.R
    inData = args[2]
    out=args[3]
    
#   write(inData, stdout())
#    write(output, stdout())
    
    data <- read.table(inData, header = T)
    data <- unlist(c(data)) 
    
    newData<-na.omit(data)
    
    if (debug == "debugno")
    {
        # Release path.
        path = paste0(getwd(), "/script/qqman/qq.R")
        source(path)
    }
    else
    {
        source(debug)
    }
	qq(newData, out)
    write(out);
}

main <- function()
{
#	curDir = paste("\nwork directory:", getwd())
#	write(curDir, stdout())

	# Get parameter from args.
    args = commandArgs(T)
    
    if (args[1] == "manhattan")
    {
        drawManhattan(args[-1])
    }
    if (args[1] == "qqplot")
    {
        drawQQplot(args[-1])
    }
    # "a.assoc.linear"
}

main()
