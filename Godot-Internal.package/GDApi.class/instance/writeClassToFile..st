file synchronisation
writeClassToFile: aClass

	TonelWriter exportClass: aClass on: (FileStream forceNewFileNamed: (self pathForClass: aClass))