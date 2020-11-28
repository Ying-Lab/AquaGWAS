# Get parameter from args.
args = commandArgs(T)
eigenvalFile = args[1]
eigenvecFile = args[2]
output = ags[3]

data<-read.table(eigenvecFile)
eigenvalue<-read.table(eigenvalFile)

library(ggplot2)
library(RColorBrewer)

pal=brewer.pal(8,"Dark2")
pca.data<-data.frame(Sample=data[,1],X=data[,3],Y=data[,4],color=pal[data[,1]])
PC1=round(eigenvalue[1,1]/sum(eigenvalue),4)
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


ggplot(data=pca.data, aes(x=X, y=Y,col=factor(Sample),label=Sample))+ 
    xlab(paste("PC1 - ", PC1*100, "%", sep="")) + 
    ylab(paste("PC2 - ", PC2*100, "%", sep="")) +
    geom_point(size=3,pch=19)+
    guides(colour=guide_legend(title='Group',override.aes = list()))+
    theme_bw() + 
    ggtitle("PCA plot")

