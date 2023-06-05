# pcb2svg

## convert  ODB++ pcb layout features  to a svg file


![](out.svg) 

* currently  a small subset  of symbols is supported
* there is no facility to read ODB++ files directly ( usally  ODB++ files are *.tgz files which means  it 
is a tar archive  format being   compressed with gzip-

## build 

* run cmake to generate your build files
* tested with  Visual Studio 2017

## usage 

Basically you have to extract your  ODB++ file locally to disk. Navigate to desired step folder.  Then choose desired layer folder. The features file in here  contains the geometrical informations whether  the profile file contains the outline of current pcb layer.

call command line: 

```
pcb2svg.exe profile features output.svg
```

## examples

The example files  are taken from the official odb++ page [4]



## third party 


this project uses header only library 

simple-svg 


[https://github.com/adishavit/simple-svg.git](https://github.com/adishavit/simple-svg.git)


Further links:

[1] Wikipedia page [https://en.wikipedia.org/wiki/ODB%2B%2B](https://en.wikipedia.org/wiki/ODB%2B%2B)

[2] Main Web Page  ODB++  [https://odbplusplus.com/](https://odbplusplus.com/)

[3] ODB++ Specification   [https://odbplusplus.com/wp-content/uploads/sites/2/2021/02/odb_spec_user.pdf]

[4] example file [https://odbplusplus.com/wp-content/resource-docs/designodb_rigidflex.tgz](https://odbplusplus.com/wp-content/resource-docs/designodb_rigidflex.tgz)
