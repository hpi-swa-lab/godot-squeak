instance creation
apiNameFromJsonObject: aJsonObject

	"TODO: this name is defined by GDApi>>generateGDNativeApi. refactoring necessary"
	^ ((aJsonObject name subStrings: '_')
		allButFirst collect: #capitalized)
		join withFirstCharacterDownshifted