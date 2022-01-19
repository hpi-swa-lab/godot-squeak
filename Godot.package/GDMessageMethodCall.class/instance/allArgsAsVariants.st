as yet unclassified
allArgsAsVariants

	^ (Interval from: 1 to: self argCount) asArray collect: [:index | self argumentAt: index] 