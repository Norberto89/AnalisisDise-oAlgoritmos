library(plotly)
data=read.csv("Resultados_filt.csv",header=FALSE)
mat_data=data.matrix(data[2:nrow(data),2:ncol(data)])
rownames(mat_data)<-data[2:nrow(data),1]
colnames(mat_data)<-data[1,2:ncol(data)]
mat_data<-mat_data[,order(as.double(colnames(mat_data)))]
ax<-list(title = "Log10 axis")
p<-plot_ly(x=log10(as.double(colnames(mat_data))),y=rownames(mat_data),z=mat_data, type = "heatmap")
p<-plot_ly(x=log10(as.double(colnames(mat_data))),y=rownames(mat_data),z=mat_data, type = "heatmap")%>%layout(title= "Gap heatmap",xaxis=ax)
p
