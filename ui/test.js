// Requirejs Configuration Options
require.config( {
	// paths: maps ids with paths (no extension)
	paths : {
		'jasmine' : [ 'externals/jasmine-2.3.4/jasmine' ],
		'jasmine-html' : [ 'externals/jasmine-2.3.4/jasmine-html' ],
		'jasmine-boot' : [ 'externals/jasmine-2.3.4/boot' ]
	},
	// shim: makes external libraries compatible with requirejs (AMD)
	shim : {
		'jasmine-html' : {
			deps : [ 'jasmine' ]
		},
		'jasmine-boot' : {
			deps : [ 'jasmine', 'jasmine-html' ]
		}
	}
});

specs = [ "test/test_spec.js" ];

require( [ 'jasmine-boot' ], function() {
	require(specs, function() {
		// trigger Jasmine
		window.onload();
	})
});