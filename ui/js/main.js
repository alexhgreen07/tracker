define( [ './api', 'jquery', 'jqueryui' ], function(libapi,$) {
	
	$(document).ready(function() {
		
		var api = new libapi.Api();
		
		$( "#tabs" ).tabs();
		
		$("#testform").submit(function() {
			
			api.sayHello($("#thename").val(),function(result){
				$("#result").html(result);
			});
			
			return false;
		});
	});

});
