message handling
handleNewInstance: newInstanceData

	instances at: newInstanceData owner put: (self classForPath: newInstanceData scriptPath fromCString) new.
	^ ExternalAddress new