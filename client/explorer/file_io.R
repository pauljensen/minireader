
# datafile format:  filename.od
# 
# Columns:
#   time value well plate experiment
#
# Descriptions:
#         time:  hours since experiment start
#        value:  OD reading
#         well:  integer well ID -- 1 (A1) through 96 (H12)
#        plate:  plate ID
#   experiment:  experiment ID
#
# 

get.experiment.filenames <- function() {
  all.files <- list.files(path="./data",pattern="\\.od$")
  return(all.files)
}

load.experiment.file <- function(filename) {
  read.table(paste("./data/",filename,sep=""),
             as.is=TRUE, header=TRUE, sep="\t",
             colClasses=c("numeric","numeric","integer",
                           "character","character"))
}

