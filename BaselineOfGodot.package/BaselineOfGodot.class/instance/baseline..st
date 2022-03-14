as yet unclassified
baseline: spec
	<baseline>

	spec for: #common do: [
		spec
			package: 'JSON'
				with: [spec repository: 'http://www.squeaksource.com/JSON'];
			package: 'FFI-Kernel'
				with: [spec repository: 'http://source.squeakfoundation.org/FFI'].
		
		spec
			package: 'Godot-Builtins';
			package: 'Godot-Internal' with: [
				spec
					requires: #('JSON' 'FFI-Kernel' 'Godot-Builtins');
					postLoadDoIt: #postLoadInternal].
		
		spec group: 'default'
			with: #('Godot-Internal')].