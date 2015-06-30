define( [ ], function() {
	
	function DummyApi()
	{
		this.lastAddedTask = null;
		this.getEventsCalled = false;
	}
	DummyApi.prototype.insertTask = function(earliestStartTime,latestEndTime,duration,success,error)
	{
		this.lastAddedTask = {
			earliestStartTime: earliestStartTime,
			latestEndTime: latestEndTime,
			duration: duration
		};
		
		success(true);
	};
	DummyApi.prototype.getEvents = function(success,error)
	{
		this.getEventsCalled = true;
		success([]);
	};
	
	return {
		DummyApi: DummyApi
	};
	
});