

calculate.growth.rate <- function(df,stride=10,window.frac=0.20,transform=log) {
  t <- df$time
  od <- df$value
  n <- length(t)
  width <- floor(window.frac * n)
  if (!is.null(transform))
    od <- transform(od)
  best <- list(rate=NULL,t.start=NULL,t.end=NULL)
  for (i in seq.int(1,n-width,stride)) {
    x <- t[i:(i+width)]
    y <- od[i:(i+width)]
    model <- lm(y ~ x)
    if (is.null(best$rate) || (model$coefficients["x"] > best$rate))
      best <- list(rate=model$coefficients["x"],t.start=t[i],t.end=t[i+width])
  }
  names(best$rate) <- NULL  # remove the name "x" on rate from the linear model
  return(best)
}


calculate.growth.rates <- function(data,...) {
  ddply(data,
        .(experiment,plate,well),
        function(x) as.data.frame(calculate.growth.rate(x)))
}

