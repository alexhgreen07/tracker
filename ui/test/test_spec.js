define( [ 'js/api' ], function(libapi) {

	describe("ApiLib Suite", function() {
		
		var testApi = null;
		
		beforeEach(function() {
			testApi = new libapi.Api();
		});

		it("says hello", function(done) {
			testApi.sayHello("test",function(result){
				expect(result).toEqual("Hello: test");
				done();
			});
		});
		
		it("gets empty tasks table", function(done) {
			testApi.getTasks(function(result){
				expect(result).toEqual([]);
				done();
			});
		});
		
		it("gets empty events table", function(done) {
			testApi.getEvents(function(result){
				expect(result).toEqual([]);
				done();
			});
		});
		
		it("inserts single task", function(done) {
			
			var earliestStartTime = 1;
			var latestEndTime = 2;
			var duration = 3;
			
			testApi.insertTask(earliestStartTime,latestEndTime,duration,function(result){
				expect(result).toEqual(true);
				done();
			});
		});
		
		it("gets a single task in table", function(done) {
			
			var earliestStartTime = 1;
			var latestEndTime = 2;
			var duration = 3;
			
			testApi.insertTask(earliestStartTime,latestEndTime,duration,function(result){
				testApi.getTasks(function(result){
					
					var expectedTable = [
	                     {
	                    	 taskId: 1,
	                    	 earliestStartTime: earliestStartTime, 
	                    	 latestEndTime: latestEndTime, 
	                    	 duration: duration
                    	 }
					];
					
					expect(result[0]).toEqual(expectedTable[0]);
					done();
				});
			});
		});

		afterEach(function() {
			testApi = null;
		});

	});

});