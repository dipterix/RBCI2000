
This is a test repository. The goal to try to integrate [BCI2000](https://www.bci2000.org/) into R.

The repo is under construction.

## Install

```r
# Uncomment the following line if you don't have `remotes` package
# install.packages("remotes")

# Install RBCI2000 demo
remotes::install_github("dipterix/RBCI2000")
```

## Toy example

After installation, run the following command. Make sure the working directory is set to the `prog` folder in `BCI2000`

```r
setwd("C:/BCI2000/BCI2000 v3.6.beta.R7385/BCI2000.x64/prog/")
RBCI2000:::BCIDemo('./')
```

