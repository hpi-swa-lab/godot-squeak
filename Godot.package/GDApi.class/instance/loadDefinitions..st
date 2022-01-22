as yet unclassified
loadDefinitions: definitions

	scriptLoadingActive := true.
	[definitions do: #load] ensure: [scriptLoadingActive := false].