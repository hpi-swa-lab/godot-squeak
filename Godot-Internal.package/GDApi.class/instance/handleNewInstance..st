message handling
handleNewInstance: newInstanceData

	"TODO: the variant for the owner that is created here must be freed when the instance is destroyed. since this happens only when godot says so, the message handler for instance destruction would be a good place for this"
	| owner instance |
	owner := newInstanceData owner.
	instance := (self classForPath: newInstanceData scriptPath fromCString) new internalVariant: (self variantFromGDObject: owner).
	instances at: owner put: instance.
		
	^ ExternalAddress new