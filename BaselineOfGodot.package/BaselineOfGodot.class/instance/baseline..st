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
					loads: #('Core')];
			baseline: 'Tonel'
				with: [spec repository: 'github://squeak-smalltalk/squeak-tonel:squeak/'].
		
		spec
			package: 'Godot-Builtins';
			package: 'Godot-Internal' with: [
				spec
					requires: #('JSON' 'FFI' 'Tonel' 'Godot-Builtins');
					postLoadDoIt: #postLoadInternal].
		
		spec group: 'default'
			with: #('Godot-Internal')].