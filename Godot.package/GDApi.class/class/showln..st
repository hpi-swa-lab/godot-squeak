as yet unclassified
showln: anObject

	"Convenient place to change how/if Transcript output should happen; useful for e.g. more accurate benchmarking"
	self show: Character cr asString, anObject