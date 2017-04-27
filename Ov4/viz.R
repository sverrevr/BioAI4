library(plotly)
setwd("C:\\Users\\eirik\\OneDrive\\Dokumenter\\NTNU\\IT3708\\JSSP\\Ov4")   # set working directory
df <- read.csv("Gantt.csv", stringsAsFactors = F)


client    <- "Sample Client"
#palette from http://tools.medialab.sciences-po.fr/iwanthue/
#cols <- RColorBrewer::brewer.pal(length(unique(df$Job)), name = "Set3")
cols <- palette(rainbow(length(unique(df$Job))))
cols <- palette(rainbow(length(unique(df$Job)))) #er en bug som gjor at det maa kjoeres 2 ganger
df$color  <- factor(df$Job, labels = cols)


p <- plot_ly()
for(i in 1:(nrow(df))){
  p <- add_trace(p,
                 type = "scatter",
                 x = c(df$Start[i], df$Start[i] + df$Duration[i]), 
                 y = c(df$Machine[i], df$Machine[i]), 
                 mode = "lines",
                 line = list(color = df$color[i], width = 20),
                 #marker	= list(color = 'rgba(255,255,255,1)', size = 16, symbol="diamond-tall"),
                 showlegend = F,
                 textposition = "top right",
                 hoverinfo = "text",
				 #textfont= list(size = "6"),
                 text = paste(	"Job: ", df$Job[i], "<br>",
								"Start:", df$Start[i], "<br>",
                              	"Duration: ", df$Duration[i], "<br>"
                              ),#df$Task[i]),
                 evaluate = T
  )
}

p