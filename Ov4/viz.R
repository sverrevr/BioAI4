library(plotly)
setwd("C:\\Users\\eirik\\OneDrive\\Dokumenter\\NTNU\\IT3708\\JSSP\\Ov4")   # set working directory
df <- read.csv("Gantt.csv", stringsAsFactors = F)


client    <- "Sample Client"
#cols      <- RColorBrewer::brewer.pal(length(unique(df$Job)), name = "Set3")
cols <- palette(c(rgb(93,192,138, maxColorValue=255),
rgb(184,91,200, maxColorValue=255),
rgb(96,182,71, maxColorValue=255),
rgb(113,99,207, maxColorValue=255),
rgb(179,181,56, maxColorValue=255),
rgb(214,67,148, maxColorValue=255),
rgb(53,132,90, maxColorValue=255),
rgb(212,63,86, maxColorValue=255),
rgb(72,182,210, maxColorValue=255),
rgb(207,80,44, maxColorValue=255),
rgb(101,128,198, maxColorValue=255),
rgb(219,146,52, maxColorValue=255),
rgb(153,80,139, maxColorValue=255),
rgb(98,128,52, maxColorValue=255),
rgb(213,143,206, maxColorValue=255),
rgb(140,120,42, maxColorValue=255),
rgb(184,88,107, maxColorValue=255),
rgb(191,175,109, maxColorValue=255),
rgb(156,91,45, maxColorValue=255),
rgb(228,139,113, maxColorValue=255)))
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