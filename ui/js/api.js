define( [ 'jquery', 'jqueryjsonrpc' ], function($) {
	
	function Api()
	{
		$.jsonRPC.setup( {
			endPoint : '/',
			namespace : ''
		});
	}
	Api.prototype.exit = function(success,error)
	{
		error = error || function(data){};
		$.jsonRPC.request('exit', {
			params : {},
			success : function(data){
			},
			error : error
		});
	};
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
		error = error || function(data){};
		$.jsonRPC.request('getTasks', {
			params : {},
			success : function(data){
				success(data.result);
			},
			error : error
		});
	};
	Api.prototype.insertTask = function(earliestStartTime,latestEndTime,duration,success,error)
	{
		error = error || function(data){};
		$.jsonRPC.request('insertTask', {
			params : {
				earliestStartTime: earliestStartTime,
				latestEndTime: latestEndTime,
				duration: duration
			},
			success : function(data){
				success(data.result);
			},
			error : error
		});
	};
	
	Api.prototype.getEvents = function(success,error)
	{
		error = error || function(data){};
		$.jsonRPC.request('getEvents', {
			params : {},
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