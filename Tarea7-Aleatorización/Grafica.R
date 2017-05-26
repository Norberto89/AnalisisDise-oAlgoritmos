data<-read.csv("Tabla.csv", header = FALSE)
mat_data<- data.matrix(data[,2])
rownames(mat_data)<-data[,1]
jpeg("grafica.jpg")
plot(mat_data,type="o", axes = FALSE, xlab = "Distribuciones" ,ylab="Tiempo (s)")
axis(2)
axis(1, at=seq_along(mat_data),labels=as.character(rownames(mat_data)), las=2)
box()
dev.off()

