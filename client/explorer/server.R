library(shiny)

source("file_io.R",local=TRUE)
source("processing.R",local=TRUE)
source("plotting.R",local=TRUE)


shinyServer(function(input, output) {
  
  # ============== Load Data tab ==================
  data <- reactive({
    if (!is.null(input$load.data.filename))
      load.experiment.file(input$load.data.filename)
    else
      NULL
  })
  
  rates <- reactive({
    if (!is.null(data()))
      calculate.growth.rates(data())
    else
      NULL
  })
  
  output$load.data.file.selector <- renderUI({
    selectInput("load.data.filename", "Choose an experiment file (.od):",
                choices = get.experiment.filenames(),multiple=T)
  })
  
  output$load.data.summary <- renderPrint({
    n.plates <- length(unique(data()$plate))
    if (!is.null(data())) {
      rates()  # make sure calculations are done on load
      say(
        sprintf('Datafile: "%s"',input$load.data.filename),
        if (n.plates == 1)
          sprintf("    1 plate")
        else
          sprintf("    %i plates",n.plates),
        sprintf("    %i data points spanning %0.2f hours",dim(data())[1],max(data()$time))
      )
    } else {
      cat("No data file loaded.")
    }
  })
  
  plate.choices <- reactive({
    choices <- unique(data()$plate)
    names(choices) <- paste("Plate",choices)
    choices
  })
  
  # ================ Plate View tab ===================
  
  output$plate.view.plate.selector <- renderUI({
    selectInput("plate.view.plate", "Select a plate:",
                choices = plate.choices())
  })
  
  output$plate.view.plot <- renderPlot({
    if (input$plate.view.plot.type == "rates") {
      plate <- rates()$rate[rates()$plate == as.numeric(input$plate.view.plate)]
      plot.96.rates(plate,rate.max=max(rates()$rate))
    } else {
      plot.96.curves(data(),rates(),as.numeric(input$plate.view.plate))
    }
  })
  
  # ================ Well View tab ====================
  
  output$well.view.plate.selector <- renderUI({
    selectInput("well.view.plate", "Select wells to plot:",
                choices = plate.choices())
  })
  
  output$well.view.well.grids <- renderUI({
    wrapper <- function(plate.id) conditionalPanel(if (input$well.view.plate == plate.id) "true" else "false",
                                                   make.plate.grid(plate.id,input))
    lapply(as.list(plate.choices()),wrapper)
  })
  
  get.active.wells <- function(inputs) {
    wells <- data.frame()
    for (plate.id in plate.choices()) {
      well.names <- generate.well.names(plate.id)
      for (well in 1:96) {
        if (inputs[[well.names[well]]])
          wells <- rbind(wells,data.frame(plate=plate.id,well=well))
      }
    }
    return(wells)
  }
  
  wells <- reactive(get.active.wells(input))
  output$well.view.plot <- renderPlot({
    plot.wells(data(),rates(),wells())
  })

})


say <- function(...) {
  str <- NULL
  for (s in list(...)) {
    if (is.null(str))
      str <- s
    else
      str <- paste(str,s,sep="\n")
  }
  cat(str)
}

generate.well.names <- function(plate.id) {
  paste(plate.id,"-",as.character(1:96),sep="")
}

td <- function(...) tags$td(align="center",valign="middle",width="20px",...)

make.plate.grid <- function(plate.id,input) {
  col.headings <- c("",as.character(1:12),recursive=T)
  rows <- vector("list",8)
  rows[[1]] <- tags$tr(lapply(as.list(col.headings),td))
  row.headings <- c("A","B","C","D","E","F","G","H")
  well.names <- paste(plate.id,"-",as.character(1:96),sep="")
  for (i in 1:8) {
    input.names <- well.names[12*(i-1)+(1:12)]
    input.strings <- character(12)
    for (j in 1:12) {
      name <- input.names[j]
      to.check <- !is.null(input[[name]]) && input[[name]]
      input.strings[j] <- paste('<input id="',name,'" type="checkbox" ',
                                if (to.check) "checked" else "",'/>',sep="")
    }
    final.inputs <- c(row.headings[i],input.strings,recursive=T)
    rows[[i+1]] <- lapply(as.list(final.inputs),function(x) td(HTML(x)))
  }
  tags$table(border="1px",cellpadding="3px",lapply(rows,tags$tr))
}


