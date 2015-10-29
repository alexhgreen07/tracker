/**
Contains JSON RPC API object
@module api
@requires module:jquery
@requires module:jqueryjsonrpc
*/
define( [ 'jquery', 'jqueryjsonrpc' ], function($) {
	
	/**
	@class The API object interfacing to the server
	@constructor Api
	@alias module:api
	*/
	function Api()
	{
		this.rpc = $.jsonRPC.setup( {
			endPoint : '/',
			namespace : ''
		});
		
		this.taskLookup = {};
	}
	
	/**
	@method fillTaskLookup
	@memberof module:api~Api
	@instance
	@param {Object} results
	*/
	Api.prototype.fillTaskLookup = function(results)
	{
		this.taskLookup = {};
		for(var key in results)
		{
			var task = results[key];
			this.taskLookup[task.taskId] = task;
		}
	};
	
	/**
	@method converTaskDateStringsToInts
	@memberof module:api~Api
	@instance
	@param {Object} results
	*/
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
	
	/**
	@method converEventDateStringsToInts
	@memberof module:api~Api
	@instance
	@param {Object} results
	*/
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
	
	/**
	@method exit
	@memberof module:api~Api
	@instance
	@param success
	@param error
	*/
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
	
	/**
	@method reset
	@memberof module:api~Api
	@instance
	@param success
	@param error
	*/
	Api.prototype.reset = function(success,error)
	{
		error = error || function(data){};
		this.rpc.request('reset', {
			params : {},
			success : success,
			error : error
		});
	};
	
	/**
	@method sayHello
	@memberof module:api~Api
	@instance
	@param {string} name
	@param success
	@param error
	*/
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
	
	/**
	@method getAppData
	@memberof module:api~Api
	@instance
	@param success
	@param error
	*/
	Api.prototype.getAppData = function(success,error)
	{
		error = error || function(data){};
		this.rpc.request('getAppData', {
			params : {},
			success : function(data){
				this.converTaskDateStringsToInts(data.result.tasks);
				this.fillTaskLookup(data.result.tasks);
				this.converEventDateStringsToInts(data.result.events);
				
				success(data.result);
			}.bind(this),
			error : error
		});
	};
	
	/**
	@method insertTask
	@memberof module:api~Api
	@instance
	@param {string} name
	@param earliestStartTime
	@param latestEndTime
	@param duration
	@param {string} status
	@param recurringPeriod
	@param recurringLateOffset
	@param success
	@param error
	*/
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
				this.getAppData(function(){
					success(data.result);
				},error);
			}.bind(this),
			error : error
		});
	};
	
	/**
	@method updateTask
	@memberof module:api~Api
	@instance
	@param {number} taskId
	@param {string} name
	@param earliestStartTime
	@param latestEndTime
	@param duration
	@param {string} status
	@param recurringPeriod
	@param recurringLateOffset
	@param success
	@param error
	*/
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
				this.getAppData(function(){
					success(data.result);
				},error);
			}.bind(this),
			error : error
		});
	};
	
	/**
	@method updateRecurringTaskStatus
	@memberof module:api~Api
	@instance
	@param {string} taskId
	@param {string} recurringIndex
	@param {number} status
	@param success
	@param error
	*/
	Api.prototype.updateRecurringTaskStatus = function(taskId,recurringIndex,status,success,error)
	{
		error = error || function(data){};
		this.rpc.request('updateRecurringTaskStatus', {
			params : {
				taskId: taskId.toString(),
				recurringIndex: recurringIndex.toString(),
				status: status.toString(),
			},
			success : function(data){
				this.getAppData(function(){
					success(data.result);
				},error);
			}.bind(this),
			error : error
		});
	};
	
	/**
	@method removeTask
	@memberof module:api~Api
	@instance
	@param {number} taskId
	@param success
	@param error
	*/
	Api.prototype.removeTask = function(taskId,success,error)
	{
		error = error || function(data){};
		this.rpc.request('removeTask', {
			params : {
				taskId: taskId
			},
			success : function(data){
				this.getAppData(function(){
					success(data.result);
				},error);
			}.bind(this),
			error : error
		});
	};
	
	/**
	@method insertEvent
	@memberof module:api~Api
	@instance
	@param {number} taskId
	@param startTime
	@param duration
	@param {string} status
	@param recurringIndex
	@param success
	@param error
	*/
	Api.prototype.insertEvent = function(taskId,startTime,duration,status,recurringIndex,success,error)
	{
		error = error || function(data){};
		this.rpc.request('insertEvent', {
			params : {
				taskId: taskId.toString(),
				startTime: startTime.toString(),
				duration: duration.toString(),
				status: status,
				recurringIndex: recurringIndex.toString()
			},
			success : function(data){
				success(data.result);
			},
			error : error
		});
	};
	
	/**
	@method updateEvent
	@memberof module:api~Api
	@instance
	@param {number} eventId
	@param {number} taskId
	@param startTime
	@param duration
	@param {string} status
	@param recurringIndex
	@param success
	@param error
	*/
	Api.prototype.updateEvent = function(eventId,taskId,startTime,duration,status,recurringIndex,success,error)
	{
		error = error || function(data){};
		this.rpc.request('updateEvent', {
			params : {
				eventId: eventId.toString(),
				taskId: taskId.toString(),
				startTime: startTime.toString(),
				duration: duration.toString(),
				status: status,
				recurringIndex: recurringIndex.toString()
			},
			success : function(data){
				success(data.result);
			},
			error : error
		});
	};
	
	/**
	@method removeEvent
	@memberof module:api~Api
	@instance
	@param {number} eventId
	@param success
	@param error
	*/
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