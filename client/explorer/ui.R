
library(shiny)


pageWithoutSidebar <- function(headerPanel,mainPanel) {
  bootstrapPage(div(class="container-fluid", 
                    div(class="row-fluid",headerPanel),
                    div(class="row-fluid",mainPanel)))
}


shinyUI(pageWithoutSidebar(
  headerPanel("Minireader Data Explorer"),
  
  mainPanel(
    tabsetPanel(
      
      # ============== Load Data tab ==================
      tabPanel("Load Data",
        uiOutput("load.data.file.selector"),
        verbatimTextOutput("load.data.summary")
      ),
      
      # ============== Plate View tab ==================
      tabPanel("Plate View",
        div(class="container-fluid",
            div(class="row-fluid",
                div(class="span6",
                    uiOutput("plate.view.plate.selector")
                ),
                div(class="span6",
                    radioButtons("plate.view.plot.type","Plot type:",
                                 c("Growth Rates" = "rates",
                                   "Growth Curves" = "curves"))
                )
            ),
            div(class="row-fluid",
                plotOutput("plate.view.plot")
            )
        )         
      ),
      
      # ============== Well View tab ==================
      tabPanel("Well View",
        pageWithSidebar(
          headerPanel(""),
          div(class="span6",
              uiOutput("well.view.plate.selector"),
              uiOutput("well.view.well.grids")
          ),
          div(class="span6",
              plotOutput("well.view.plot")
          )
        )       
      ),
      
      # ============== Export Data tab ==================
      tabPanel("Export Data")
    )
  )
))
