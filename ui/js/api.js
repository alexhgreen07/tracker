define( [ 'jquery', 'jqueryjsonrpc' ], function($) {
	
	function Api()
	{
		this.rpc = $.jsonRPC.setup( {
			endPoint : '/',
			namespace : ''
		});
		
		this.taskLookup = {};
	}
	
	Api.prototype.fillTaskLookup = function(results)
	{
		this.taskLookup = {};
		for(var key in results)
		{
			var task = results[key];
			this.taskLookup[task.taskId] = task;
		}
	};
	
	Api.prototype.converTaskDateStringsToInts = function(results)
	{
		for(var key in results)
		{
			var task = results[key];
			task.duration = parseInt(task.duration);
			task.earliestStartTime = parseInt(task.earliestStartTime);
			task.latestEndTime = parseInt(task.latestEndTime);
			task.recurringLateOffset = parseInt(task.recurringLateOffset);
			task.recurringPeriod = parseInt(task.recurringPeriod);
		}
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
				this.converTaskDateStringsToInts(data.result);
				this.fillTaskLookup(data.result);
				
				success(data.result);
			}.bind(this),
			error : error
		});
	};
	Api.prototype.insertTask = function(name,earliestStartTime,latestEndTime,duration,recurringPeriod,recurringLateOffset,success,error)
	{
		error = error || function(data){};
		this.rpc.request('insertTask', {
			params : {
				name: name,
				earliestStartTime: earliestStartTime.toString(),
				latestEndTime: latestEndTime.toString(),
				duration: duration.toString(),
				recurringPeriod: recurringPeriod.toString(),
				recurringLateOffset: recurringLateOffset.toString()
			},
			success : function(data){
				this.getTasks(function(){
					success(data.result);
				},error);
			}.bind(this),
			error : error
		});
	};
	Api.prototype.updateTask = function(taskId,name,earliestStartTime,latestEndTime,duration,recurringPeriod,recurringLateOffset,success,error)
	{
		error = error || function(data){};
		this.rpc.request('updateTask', {
			params : {
				taskId: taskId,
				name: name,
				earliestStartTime: earliestStartTime.toString(),
				latestEndTime: latestEndTime.toString(),
				duration: duration.toString(),
				recurringPeriod: recurringPeriod.toString(),
				recurringLateOffset: recurringLateOffset.toString()
			},
			success : function(data){
				this.getTasks(function(){
					success(data.result);
				},error);
			}.bind(this),
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
				this.getTasks(function(){
					success(data.result);
				},error);
			}.bind(this),
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