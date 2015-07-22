define( [ ], function() {
	
	function DummyApi()
	{
		this.taskLookup = {};
	}
	DummyApi.prototype.insertTask = function(name,earliestStartTime,latestEndTime,duration,status,recurringPeriod,recurringLateOffset,success,error)
	{
		success(true);
	};
	DummyApi.prototype.updateTask = function(taskId,name,earliestStartTime,latestEndTime,duration,status,recurringPeriod,recurringLateOffset,success,error)
	{
		success(true);
	}
	DummyApi.prototype.removeTask = function(taskId,success,error)
	{
		success(true);
	};
	DummyApi.prototype.getEvents = function(success,error)
	{
		success([]);
	};
	DummyApi.prototype.insertEvent = function(taskId,startTime,duration,success,error)
	{
		success(true);
	};
	
	return {
		DummyApi: DummyApi
	};
	
});