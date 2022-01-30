generating
source
	
	^ '{1}{2}
	<generated>
	
	^ self callOnVariant: ''{3}'' with: \{{4}\}' format: {self nameString. self methodArgumentString. name. self argumentArrayString}