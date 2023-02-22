
#Código de generación de boxplots con los datos obtenidos durante las corridas
instancia1<-read.delim("/Users/danielvallejo/Documents/MaestriaCC2224/Semestre2/OptEst1/OptEstocastica/Tarea1/results.txt",header = FALSE,sep = '\n')
colnames(instancia1)<-c("V4")
local1<-read.table(text = gsub(" ", "\t", readLines("/Users/danielvallejo/Documents/MaestriaCC2224/Semestre2/OptEst1/OptEstocastica/Tarea2/salida_small.txt")))
df1<-as.data.frame(c(instancia1,local1))
colnames(df1)<-c("Random","Random/LocalSearch","Constructive/LocalSearch")
boxplot(df1)

summary(df1)


local1<-read.table(text = gsub(" ", "\t", readLines("/Users/danielvallejo/Documents/MaestriaCC2224/Semestre2/OptEst1/OptEstocastica/Tarea2/times_small.txt")))
colnames(local1)<-c("Random","Constructiva")
boxplot(local1,col='blue')
summary(local1)
####################

instancia1<-read.delim("/Users/danielvallejo/Documents/MaestriaCC2224/Semestre2/OptEst1/OptEstocastica/Tarea1/results_medium.txt",header = FALSE,sep = '\n')
colnames(instancia1)<-c("V4")
local1<-read.table(text = gsub(" ", "\t", readLines("/Users/danielvallejo/Documents/MaestriaCC2224/Semestre2/OptEst1/OptEstocastica/Tarea2/salida_medium.txt")))
df1<-as.data.frame(c(instancia1,local1))
colnames(df1)<-c("Random","Random/LocalSearch","Constructive/LocalSearch")
boxplot(df1)
summary(df1)

local1<-read.table(text = gsub(" ", "\t", readLines("/Users/danielvallejo/Documents/MaestriaCC2224/Semestre2/OptEst1/OptEstocastica/Tarea2/times_large.txt")))
colnames(local1)<-c("Random","Constructiva")
boxplot(local1,col='blue')
#####################

instancia1<-read.delim("/Users/danielvallejo/Documents/MaestriaCC2224/Semestre2/OptEst1/OptEstocastica/Tarea1/results_large.txt",header = FALSE,sep = '\n')
colnames(instancia1)<-c("V4")
local1<-read.table(text = gsub(" ", "\t", readLines("/Users/danielvallejo/Documents/MaestriaCC2224/Semestre2/OptEst1/OptEstocastica/Tarea2/salida_large.txt")))
df1<-as.data.frame(c(instancia1,local1))
colnames(df1)<-c("Random","Random/LocalSearch","Constructive/LocalSearch")
boxplot(df1)

summary(df1)