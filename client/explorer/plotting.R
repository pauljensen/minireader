
library(RColorBrewer)
library(plotrix)
library(ggplot2)

well.names <- 
c("A01", "A02", "A03", "A04", "A05", "A06", "A07", "A08", "A09", "A10", 
  "A11", "A12", "B01", "B02", "B03", "B04", "B05", "B06", "B07", "B08", 
  "B09", "B10", "B11", "B12", "C01", "C02", "C03", "C04", "C05", "C06", 
  "C07", "C08", "C09", "C10", "C11", "C12", "D01", "D02", "D03", "D04", 
  "D05", "D06", "D07", "D08", "D09", "D10", "D11", "D12", "E01", "E02", 
  "E03", "E04", "E05", "E06", "E07", "E08", "E09", "E10", "E11", "E12", 
  "F01", "F02", "F03", "F04", "F05", "F06", "F07", "F08", "F09", "F10", 
  "F11", "F12", "G01", "G02", "G03", "G04", "G05", "G06", "G07", "G8", 
  "G09", "G10", "G11", "G12", "H01", "H02", "H03", "H04", "H05", "H06", 
  "H07", "H08", "H09", "H10", "H11", "H12")

# ========================== Plotting ========================== 
plot.96.rates <- function(rates,title=NULL,rate.max=NULL) {
  colors <- colorRampPalette(brewer.pal(9,"Reds"))(1000)
  if (is.null(rate.max))
    rate.max <- max(rates) + 1e-5
  get.color <- function(x) colors[floor(x/rate.max*1000) + 1]
  
  well <- 0
  plot(-1,-1,xlim=c(0,12.5),ylim=c(0.5,9.5),xaxt="n",yaxt="n",
       xlab="",ylab="",main=title)
  for (i in 1:8) {
    for (j in 1:12) {
      well <- well + 1
      points(j,8-i+1,cex=4.5)
      points(j,8-i+1,col=get.color(rates[well]),pch=16,cex=4)
    }
  }
  
  xlabs <- c("A","B","C","D","E","F","G","H")
  for (i in 1:8)
    text(0,8-i+1,xlabs[i])
  for (j in 1:12)
    text(j,9,as.character(j))
  
  legend <- c("0",substr(as.character(rate.max),1,5))
  color.legend(2,-1.5,10,-1,legend=legend,colors) 
}

plot.96.curves <- function(data,rates,plate.id) {
  sub.data <- data[data$plate == plate.id,]
  sub.data$well.name <- well.names[sub.data$well]
  p <- qplot(time,value,data=sub.data,geom="line",ylim=c(0,max(data$value)),
             xlab="time [hours]",ylab="Optical Density") + 
    facet_wrap(~well.name,ncol=12) + theme_bw()
  print(p)
}



line.colors <- c("black","red","blue","green","yellow","purple")

plot.wells <- function(data,rates,wells) {
  plot(0,0,ylab="Optical Density",xlab="time [hours]",type="l",
       cex.lab=1.5, cex.axis=1.5,
       xlim=c(0,max(data$time)),ylim=c(0,max(data$value)))
  n.wells <- dim(wells)[1]
  legend.strings <- character(n.wells)
  for (i in 1:n.wells) {
    sub.data <- data[data$plate==wells$plate[i] & data$well==wells$well[i],]
    lines(sub.data$time,sub.data$value,lwd=4,col=line.colors[i])
    legend.strings[i] <- sprintf("%s - %s (%0.3f)",
                                 wells$plate[i],
                                 well.names[wells$well[i]],
                                 rates[rates$plate==wells$plate[i] & rates$well==wells$well[i],"rate"])
  }
  if (n.wells > 1)
    legend("bottomright",legend.strings,fill=line.colors[1:n.wells])
}


