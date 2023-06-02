### Análisis de los resultados obtenidos con el algoritmo genético ##
library(ggplot2)
library(cursr)
file<-read.delim("salida_1.txt",sep='\n',header=FALSE)
file_valid<-file[which(file$V1<=1.0),]
hist(file_valid,main = "Wake free Ratio Histogram 20 generators",breaks=20,col='green')
## Para el caso de 30 generadores tenemos lo siguiente
file<-read.delim("salida_1_30gen.txt",sep='\n',header=FALSE)
file_valid<-file[which(file$V1<=1.0),]
hist(file_valid,main = "Wake free Ratio Histogram 30 generators",breaks=20,col='blue')
