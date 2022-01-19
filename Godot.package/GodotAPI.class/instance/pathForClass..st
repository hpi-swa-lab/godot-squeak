file synchronisation
pathForClass: aClass
	
	"^ pathToClass detect: [:association | association key = aClass] key"
	^ 'TEST.st'