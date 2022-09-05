as yet unclassified
postLoadInternal

	| gdnativeApiJson godotApiJson |
	
	godotApiJson := (HTTPLoader default
		retrieveContentsFor: 'https://raw.githubusercontent.com/godotengine/godot-headers/3.5/api.json').
	gdnativeApiJson := (HTTPLoader default
		retrieveContentsFor: 'https://raw.githubusercontent.com/godotengine/godot-headers/3.5/gdnative_api.json').

	GDApi generateGDNativeApi: gdnativeApiJson contentStream.
  	GDApiObjectsGenerator generateGodotApi: godotApiJson contentStream.
  	GDApiObjectsGenerator generateGdnativeApi: gdnativeApiJson contentStream.