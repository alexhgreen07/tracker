define( [ ], function() {
	
	function DummyApi()
	{
		this.lastAddedTask = null;
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
	
	return {
		DummyApi: DummyApi
	};
	
});