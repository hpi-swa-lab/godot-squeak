ffi
getProjectRoot
	"it might be necessary to deallocate the string returned from this"
	<apicall: char* godot_get_project_root()>
	^ self externalCallFailed