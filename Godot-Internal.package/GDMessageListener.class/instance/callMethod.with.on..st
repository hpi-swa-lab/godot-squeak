as yet unclassified
callMethod: aSymbol with: arguments on: anInstance

	^ (anInstance respondsTo: aSymbol)
			ifTrue: [
				anInstance perform: aSymbol withArguments: arguments]
			ifFalse: [
				GDApi showln: 'received message for ', anInstance class, ' that it doesn''t understand: ', aSymbol.
				nil].