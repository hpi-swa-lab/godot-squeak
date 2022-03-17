as yet unclassified
initialize

	self class properties do: [:property |
		self perform: property first asSimpleSetter with: property second]