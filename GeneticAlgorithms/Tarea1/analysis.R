# Análisis de los resultados de cada una de las funciones para las diferentes dimensiones

## Para la función ackley
ackley_10<-read.delim("./ackley_dim10.txt",sep = '\n',header = FALSE)
colnames(ackley_10)<-c("dim 10")
ackley_30<-read.delim("./ackley_dim30.txt",sep='\n',header = FALSE)
colnames(ackley_30)<-c("dim 30")
boxplot(c(ackley_10,ackley_30),main='Boxplot para la función Ackley')

summary(ackley_10)s
summary(ackley_30)
##Para la función sphere

sphere_10<-read.delim("./sphere_dim10.txt",sep = '\n',header = FALSE)
colnames(sphere_10)<-c("dim 10")
sphere_30<-read.delim("./sphere_dim30.txt",sep='\n',header = FALSE)
colnames(sphere_30)<-c("dim 30")
boxplot(c(sphere_10,sphere_30),main='Boxplot para la función Sphere')

summary(sphere_10)
summary(sphere_30)

#Finalmente para la función Rastrigin

rastrigin_10<-read.delim("./rastrigin_dim10.txt",sep = '\n',header = FALSE)
colnames(rastrigin_10)<-c("dim 10")
rastrigin_30<-read.delim("./rastrigin_dim30.txt",sep='\n',header = FALSE)
colnames(rastrigin_30)<-c("dim 30")
boxplot(c(rastrigin_10,rastrigin_30),main='Boxplot para la función Rastrigin')

summary(rastrigin_10)
summary(rastrigin_30)
