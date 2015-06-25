define( [ 'jquery', './api' ], function($,libapi) {
	
	$(document).ready(function() {
		
		var api = new libapi.Api();
		
		$("#testform").submit(function() {
			
			api.sayHello($("#thename").val(),function(data){
				$("#result").html(data.result);
			});
			
			return false;
		});
	});

});
