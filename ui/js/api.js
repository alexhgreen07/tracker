define( [ 'jquery', 'jqueryjsonrpc' ], function($) {
	
	function Api()
	{
		$.jsonRPC.setup( {
			endPoint : '/',
			namespace : ''
		});
	}
	Api.prototype.sayHello = function(name,success,error)
	{
		error = error || function(data){};
		
		$.jsonRPC.request('sayHello', {
			params : {name:name},
			success : function(data){
				success(data.result);
			},
			error : error
		});
	};
	
	return {
		Api: Api
	};
});