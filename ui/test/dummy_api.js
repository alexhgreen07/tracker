define( [ ], function() {
	
	function DummyApi()
	{
		this.taskLookup = {};
	}
	DummyApi.prototype.insertTask = function(name,earliestStartTime,latestEndTime,duration,success,error)
	{
		success(true);
	};
	DummyApi.prototype.updateTask = function(taskId,name,earliestStartTime,latestEndTime,duration,success,error)
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
	
	return {
		DummyApi: DummyApi
	};
	
});