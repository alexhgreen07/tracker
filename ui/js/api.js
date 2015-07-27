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
	
	Api.prototype.converEventDateStringsToInts = function(results)
	{
		for(var key in results)
		{
			var event = results[key];
			event.eventId = parseInt(event.eventId);
			event.taskId = parseInt(event.taskId);
			event.startTime = parseInt(event.startTime);
			event.duration = parseInt(event.duration);
		}
	};
	
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
	Api.prototype.reset = function(success,error)
	{
		error = error || function(data){};
		this.rpc.request('reset', {
			params : {},
			success : success,
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
	Api.prototype.insertTask = function(name,earliestStartTime,latestEndTime,duration,status,recurringPeriod,recurringLateOffset,success,error)
	{
		error = error || function(data){};
		this.rpc.request('insertTask', {
			params : {
				name: name,
				earliestStartTime: earliestStartTime.toString(),
				latestEndTime: latestEndTime.toString(),
				duration: duration.toString(),
				status: status,
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
	Api.prototype.updateTask = function(taskId,name,earliestStartTime,latestEndTime,duration,status,recurringPeriod,recurringLateOffset,success,error)
	{
		error = error || function(data){};
		this.rpc.request('updateTask', {
			params : {
				taskId: taskId,
				name: name,
				earliestStartTime: earliestStartTime.toString(),
				latestEndTime: latestEndTime.toString(),
				duration: duration.toString(),
				status: status,
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
				this.converEventDateStringsToInts(data.result);
				success(data.result);
			}.bind(this),
			error : error
		});
	};
	Api.prototype.insertEvent = function(taskId,startTime,duration,success,error)
	{
		error = error || function(data){};
		this.rpc.request('insertEvent', {
			params : {
				taskId: taskId.toString(),
				startTime: startTime.toString(),
				duration: duration.toString()
			},
			success : function(data){
				success(data.result);
			},
			error : error
		});
	};
	Api.prototype.updateEvent = function(eventId,taskId,startTime,duration,success,error)
	{
		error = error || function(data){};
		this.rpc.request('updateEvent', {
			params : {
				eventId: eventId.toString(),
				taskId: taskId.toString(),
				startTime: startTime.toString(),
				duration: duration.toString()
			},
			success : function(data){
				success(data.result);
			},
			error : error
		});
	};
	Api.prototype.removeEvent = function(eventId,success,error)
	{
		error = error || function(data){};
		this.rpc.request('removeEvent', {
			params : {
				eventId: eventId.toString()
			},
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