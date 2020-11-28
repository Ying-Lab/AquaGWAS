
scinota <- "1e-8"
vec = as.vector(unlist(strsplit(scinota, "", fixed = T)))
eindex = grep('e', vec)
base = as.integer(substr(scinota, 1, eindex-1))
expo = as.integer(substr(scinota, eindex+1, nchar(scinota)))
ret = base * 10 ^ expo


getDataFromSciNotation <- function(scinota = "1e2")
{
    vec = as.vector(unlist(strsplit(scinota, "", fixed = T)))
    eindex = grep('e', vec)
    base = as.integer(substr(scinota, 1, eindex-1))
    expo = as.integer(substr(scinota, eindex+1, nchar(scinota)))
    ret = base * 10 ^ expo
}