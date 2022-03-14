as yet unclassified
baseline: spec
	<baseline>

	spec for: #common do: [
		spec
			package: 'JSON'
				with: [spec repository: 'http://www.squeaksource.com/JSON'];
			project: 'FFI' with: [
				spec
					className: 'ConfigurationOfFFI';
					repository: 'http://www.squeaksource.com/MetacelloRepository';
					versionString: #development;
					loads: #('Core')].
		
		spec
			package: 'Godot-Builtins';
			package: 'Godot-Internal' with: [
				spec
					requires: #('JSON' 'FFI' 'Godot-Builtins');
					postLoadDoIt: #postLoadInternal].
		
		spec group: 'default'
			with: #('Godot-Internal')].