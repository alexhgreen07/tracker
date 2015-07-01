define( [ 'jquery', 'jqueryjsonrpc' ], function($) {
	
	function Api()
	{
		this.rpc = $.jsonRPC.setup( {
			endPoint : '/',
			namespace : ''
		});
	}
	Api.prototype.exit = function(success,error)
	{
		error = error || function(data){};
		this.rpc.request('exit', {
			params : {},
			success : function(data){
			},
			error : error
		});
	};
	Api.prototype.sayHello = function(name,success,error)
	{
		error = error || function(data){};
		this.rpc.request('sayHello', {
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
		this.rpc.request('getTasks', {
			params : {},
			success : function(data){
				success(data.result);
			},
			error : error
		});
	};
	Api.prototype.insertTask = function(name,earliestStartTime,latestEndTime,duration,success,error)
	{
		error = error || function(data){};
		this.rpc.request('insertTask', {
			params : {
				name: name,
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
	Api.prototype.updateTask = function(taskId,name,earliestStartTime,latestEndTime,duration,success,error)
	{
		error = error || function(data){};
		this.rpc.request('updateTask', {
			params : {
				taskId: taskId,
				name: name,
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
	Api.prototype.removeTask = function(taskId,success,error)
	{
		error = error || function(data){};
		this.rpc.request('removeTask', {
			params : {
				taskId: taskId
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
		this.rpc.request('getEvents', {
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