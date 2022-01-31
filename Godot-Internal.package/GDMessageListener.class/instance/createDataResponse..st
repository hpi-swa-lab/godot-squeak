message handling
createDataResponse: responseData

	"responseData should be something that can be returned as a pointer"
	| response |
	"TODO: you can probably avoid allocating externally here by creating the response on the stack on the godot side"
	response := GDMessage allocateExternal.
	response type: GDMessage typeGodotFinishProcessing.
	response data: responseData.
	^ response