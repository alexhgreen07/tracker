define( [ ], function() {
	
	function DummyApi()
	{
	}
	DummyApi.prototype.insertTask = function(earliestStartTime,latestEndTime,duration,success,error)
	{
		success(true);
	};
	DummyApi.prototype.updateTask = function(taskId,earliestStartTime,latestEndTime,duration,success,error)
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