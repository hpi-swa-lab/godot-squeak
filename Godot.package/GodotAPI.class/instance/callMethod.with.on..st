as yet unclassified
callMethod: aSymbol with: arguments on: anInstance

	^ (anInstance respondsTo: aSymbol)
			ifTrue: [
				anInstance perform: aSymbol withArguments: arguments]
			ifFalse: [
				Transcript showln: 'received message for '; show: anInstance class; show: ' that it doesn''t understand: '; show: aSymbol.
				nil].