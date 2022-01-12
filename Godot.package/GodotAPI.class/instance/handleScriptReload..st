message handling
handleScriptReload: scriptReloadData

	Transcript cr; show: 'script reload: '; show: scriptReloadData scriptPath fromCString; show: ' source: '; show: scriptReloadData scriptSource fromCString