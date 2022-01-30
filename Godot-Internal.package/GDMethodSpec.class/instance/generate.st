generating
generate

	self methodClass
		compile: self source
		classified: self methodCategory