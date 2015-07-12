define( [ 'js/api' ], function(libapi) {

	describe("ApiLib Suite", function() {
		
		var name = "test name";
		var earliestStartTime = 1;
		var latestEndTime = 5;
		var duration = 2;
		
		var expectedDummyTask = {
           	 taskId: 1,
        	 name: name,
        	 earliestStartTime: earliestStartTime, 
        	 latestEndTime: latestEndTime, 
        	 duration: duration,
        	 status: 'Incomplete',
        	 recurringLateOffset: 1, 
        	 recurringPeriod: 10
    	 };
		
		var testApi = null;
		
		function insertDummyTask(success,error)
		{
			testApi.insertTask(name,earliestStartTime,latestEndTime,duration,10,1,success,error);
		}
		
		function updateDummyTask(success,error)
		{
			testApi.updateTask(1,name + 1,earliestStartTime + 1,latestEndTime + 1,duration + 1,11,2,success,error);
		}
		
		function removeDummyTask(success,error)
		{
			testApi.removeTask(1,success,error);
		}
		
		beforeEach(function() {
			testApi = new libapi.Api();
		});
		
		it("is allocated", function() {
			expect(testApi).not.toEqual(null);
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

		it("inserts single task", function(done) {
			insertDummyTask(function(result){
				expect(result).toEqual(true);
				done();
			});
		});
		
		it("gets an inserted task in table", function(done) {
			
			var expectedTable = [expectedDummyTask];
			
			insertDummyTask(function(){
				testApi.getTasks(function(result){
					expect(result).toEqual(expectedTable);
					done();
				});
			});
		});

		it("stores a lookup of the last task table", function(done) {
			
			var expectedLookup = {
                 1 : expectedDummyTask
			};
			
			insertDummyTask(function(){
				expect(testApi.taskLookup).toEqual(expectedLookup);
				done();
			});
		});
		
		it("updates a single task", function(done) {

			insertDummyTask(function(){
				updateDummyTask(function(result){
					expect(result).toEqual(true);
					done();
				});
			});
		});
		
		it("gets an updated task in table", function(done) {
			
			var expectedTable = [
                 {
                	 taskId: 1,
                	 name: name + 1,
                	 earliestStartTime: earliestStartTime + 1, 
                	 latestEndTime: latestEndTime + 1, 
                	 duration: duration + 1,
                	 status: 'Incomplete',
                	 recurringLateOffset: 2, 
                	 recurringPeriod: 11
            	 }
			];
			
			insertDummyTask(function(){
				updateDummyTask(function(){
					testApi.getTasks(function(result){
						expect(result).toEqual(expectedTable);
						done();
					});
				});
			});
		});
		
		it("removes a single task", function(done) {
			insertDummyTask(function(){
				removeDummyTask(function(result){
					expect(result).toEqual(true);
					done();
				});
			});
		});
		
		it("gets empty tasks table after remove", function(done) {
			insertDummyTask(function(){
				removeDummyTask(function(){
					testApi.getTasks(function(result){
						expect(result).toEqual([]);
						done();
					});
				});
			});
		});

		it("gets empty events table", function(done) {
			testApi.getEvents(function(result){
				expect(result).toEqual([]);
				done();
			});
		});
		
		it("gets events table with single event", function(done) {
			insertDummyTask(function(){
				testApi.getEvents(function(result){
					
					var expectedTable = [
	                     {
	                    	 taskId: 1,
	                    	 name: name,
	                    	 startTime: earliestStartTime,
	                    	 duration: duration
	                	 }
	    			];
					
					expect(result).toEqual(expectedTable);
					done();
				});
			});
		});
		
		afterEach(function(done) {
			testApi.exit(function(){done();},function(){done();});
			testApi = null;
		});

	});

});