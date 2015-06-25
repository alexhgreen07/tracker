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
	Api.prototype.getTasks = function(success,error)
	{
		success([]);
	};
	Api.prototype.insertTask = function(earliestStartTime,latestEndTime,duration,success,error)
	{
		success(true);
	};
	
	return {
		Api: Api
	};
});