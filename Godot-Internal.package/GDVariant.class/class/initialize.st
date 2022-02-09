class initialization
initialize

	" GDVariant initialize "

	VariantClasses := Dictionary withAll: {
		self variantNil -> UndefinedObject.
		self variantBoolean -> Boolean.
		self variantInteger -> Integer.
		self variantFloat -> Float.
		self variantString -> String.
		self variantVector2 -> GDVector2.
		self variantRect2 -> GDRect2.
		self variantVector3 -> GDVector3.
		self variantTransform2d -> GDTransform2D.
		self variantPlane -> GDPlane.
		self variantQuat -> GDQuat.
		self variantAabb -> GDAABB.
		self variantBasis -> GDBasis.
		self variantTransform -> GDTransform.
		self variantColor -> GDColor.
		self variantNodePath -> GDNodePath.
		self variantRid -> GDRid.
		self variantObject -> GDBaseObject.
		self variantDictionary -> GDDictionary.
		self variantArray -> GDArray.
		self variantPoolByteArray -> GDPoolByteArray.
		self variantPoolIntArray -> GDPoolIntArray.
		self variantPoolRealArray -> GDPoolRealArray.
		self variantPoolStringArray -> GDPoolStringArray.
		self variantPoolVector2Array -> GDPoolVector2Array.
		self variantPoolVector3Array -> GDPoolVector3Array.
		self variantPoolColorArray -> GDPoolColorArray.
	}