# Get parameter from args.
args = commandArgs(T)
eigenvalFile = args[1]
eigenvecFile = args[2]
output = args[3]

# data: FID, IID and the fisrt PCs eigenvectors
data<-read.table(eigenvecFile)
eigenvalue<-read.table(eigenvalFile)

Sample=data[,1]
X=data[,3]
Y=data[,4]
#color=rainbow(unique(Sample), alpha=0.75)
color = Sample		# Use the default color that is automatically assigned

# Calculate the proportion of eigenvalues, explains percent of the variation is.
PC1=round(eigenvalue[1,1]/sum(eigenvalue),4)	# 
PC2=round(eigenvalue[2,1]/sum(eigenvalue),4)

# Set output file name.
baseName <- as.character(basename(output)) 
suffix <- as.character(unlist(strsplit(baseName, split <- "\\.")))
suffix <- as.character(suffix[length(suffix)])

if (suffix == "png")
{
    png(file = output, width=1000,height=1000)
}
if (suffix == "jpeg" || suffix == "jpg")
{
    jpeg(file = output, width=1000,height=1000)
}

column = ceiling(length(unique(Sample))/20);		# No more than 20 legend in a column
par(mar=c(5.1, 4.1, 4.1, 4*(column+1)+0.1), xpd=TRUE)	# Reserve space in the right
plot(X, Y, main="Principal component",
     xlab=paste("PC1 - ", PC1*100, "%", sep=""),
     ylab=paste("PC2 - ", PC2*100, "%", sep=""),
     cex=2,
     pch=19,
     col=color)
legend("bottomright", 
       inset=c(-0.15*column,0),
       legend=sort(unique(color)), 
       title="Group", 
       col=sort(unique(color)), 
       bty="n",
       pch=19,
       cex=2,
       xpd=TRUE,
       ncol=column)
