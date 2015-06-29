// For any third party dependencies, like jQuery, place them in the lib folder.

// Configure loading modules from the lib directory,
// except for 'app' ones, which are in a sibling
// directory.
requirejs.config( {
	paths : {
		jquery : 'externals/jquery-2.1.4.min',
		jqueryjsonrpc : 'externals/jquery.jsonrpc',
		jqueryui : 'externals/jquery-ui-1.11.4/jquery-ui.min',
		moment: 'externals/fullcalendar/lib/moment.min',
		fullcalendar : 'externals/fullcalendar/fullcalendar.min'
	},
	shim : {
		jqueryjsonrpc : {
			deps : [ 'jquery' ]
		},
		jqueryui: {
			deps : [ 'jquery' ]
		},
		fullcalendar: {
			deps : [ 'jqueryui', 'moment' ]
		}
	}
});

if(!isTest)
{
	// Start loading the main app file. Put all of
	// your application logic in there.
	requirejs( [ 'js/main' ]);
}
