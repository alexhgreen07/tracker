// For any third party dependencies, like jQuery, place them in the lib folder.

// Configure loading modules from the lib directory,
// except for 'app' ones, which are in a sibling
// directory.
requirejs.config({
	paths: {
		jquery: 'externals/jquery-2.1.4.min',
		jqueryjsonrpc: 'externals/jquery.jsonrpc'
	},
	shim:
	{
		jqueryjsonrpc: {
			deps: ['jquery']
		}
	}
});

// Start loading the main app file. Put all of
// your application logic in there.
requirejs(['main']);