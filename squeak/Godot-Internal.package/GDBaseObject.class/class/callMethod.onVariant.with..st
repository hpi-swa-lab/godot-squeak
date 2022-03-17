as yet unclassified
callMethod: methodName onVariant: aVariant with: argsSequence

	"TODO: All of the variants and strings here need to be destroyed"
	"TODO: this a chunky func, make it a less chunky func"
	| callError result methodNameGDString variants argsPointer message testVariant messageData |
	methodNameGDString := GDString allocateExternal.
	methodName insertIntoGDString: methodNameGDString.
	
	variants := GDVariant allocateExternal: argsSequence size.
	argsPointer := ExternalType void asPointerType allocateExternal: argsSequence size.
	argsSequence withIndexDo: [:arg :index || variant |
		variant := variants at: index.
		arg insertIntoGDVariant: variant.
		argsPointer at: index put: variant getHandle].
	
	callError := GDVariantCallError allocateExternal.
	result := GDVariant allocateExternal.
	
	"TODO: avoid copy"
	testVariant := GDVariant allocateExternal.
	GDApi instance variantNewCopy: testVariant with: aVariant.
	
	messageData := GDMessageGodotMethodCall allocateExternal
		methodName: methodNameGDString;
		self: testVariant;
		args: argsPointer;
		argCount: argsSequence size;
		error: callError;
		result: result.
		
	message := GDMessage allocateExternal.
	message type: GDMessage typeGodotFunctionCall.
	message data: messageData.
		
	GDMessageListener instance sendMessage: message.
	
	callError checkError.
	
	"TODO: should probably use an ensure here"
	methodNameGDString destroy.
	variants do: #destroy.
	"TODO: I think Godot frees the memory the variant points to beforehand? crashes sometimes, eg. on connect"
	"argsPointer free."
	
	^ result.