args = commandArgs(T)

pvalFile = args[1]

header = T
if (args[2] == "TRUE" || args[2] == "T")
{
    header <<- TRUE;
} else if (args[2] == "FALSE" || args[2] == "F")
{
    header <<- FALSE;
} 
correctType = args[3]
outFile = args[4]

print(paste("p-value file:", pvalFile))
print(paste("correct type: ", correctType))
print(paste("header: ", as.character(header)))
print(paste("output file: ", outFile))

data<-read.table(pvalFile, header = header)
ncol = length(data)
pval = as.numeric(unlist(data[ncol]))
pval_adjust = p.adjust(pval, correctType, n = length(pval))
data[paste0("P_CORR_", correctType)] <- pval_adjust
write.table(x = data, file = outFile, row.names=F, col.names=header, quote=F)

# Muanal correct pval,  "fdr"
#fdr_pval = pval*length(pval)/(rank(pval)-1)

# Muanal correct pval,  "Boferroni"
#bonf_pval = pval*length(pval)

# Muanal correct pval,  "Step Down"
#holm_pval = pval*(length(pval)-rank(pval)-1)
