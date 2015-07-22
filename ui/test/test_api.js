define( [ 'js/api' ], function(libapi) {

	describe("ApiLib Suite", function() {
		
		var expectedDummyTask = {
           	 taskId: 1,
        	 name: "test name",
        	 earliestStartTime: 1, 
        	 latestEndTime: 5, 
        	 duration: 2,
        	 status: 'Complete',
        	 recurringLateOffset: 1, 
        	 recurringPeriod: 10
    	 };
		
		var testApi = null;
		testApi = new libapi.Api();
		
		function insertDummyTask(success,error)
		{
			testApi.insertTask(
					expectedDummyTask.name,
					expectedDummyTask.earliestStartTime,
					expectedDummyTask.latestEndTime,
					expectedDummyTask.duration,
					expectedDummyTask.status,
					expectedDummyTask.recurringPeriod,
					expectedDummyTask.recurringLateOffset,
					success,error);
		}
		
		function updateDummyTask(success,error)
		{
			testApi.updateTask(1,
					expectedDummyTask.name + 1,
					expectedDummyTask.earliestStartTime + 1,
					expectedDummyTask.latestEndTime + 1,
					expectedDummyTask.duration + 1,
					"Missed",
					expectedDummyTask.recurringPeriod + 1,
					expectedDummyTask.recurringLateOffset + 1,
					success,error);
		}
		
		function removeDummyTask(success,error)
		{
			testApi.removeTask(1,success,error);
		}
		
		var expectedDummyEvent = {
			eventId: 1,
			taskId: 1,
			startTime: 2,
			duration: 3,
			name: expectedDummyTask.name
		};
		
		function insertDummyEvent(success,error)
		{
			insertDummyTask(function(){
				testApi.insertEvent(
					expectedDummyEvent.taskId,
					expectedDummyEvent.startTime,
					expectedDummyEvent.duration,
					success,
					error);
			});
		}
		
		function removeDummyEvent(success,error)
		{
			testApi.removeEvent(
				expectedDummyEvent.eventId,
				success,
				error);
		}
		
		beforeEach(function() {
			
		});
		
		it("is allocated", function() {
			expect(testApi).not.toEqual(null);
		});
		
		it("says hello", function(done) {
			testApi.sayHello("test",function(result){
				expect(result).toEqual("Hello: test");
				console.log("hello!");
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
                	 name: expectedDummyTask.name + 1,
                	 earliestStartTime: expectedDummyTask.earliestStartTime + 1, 
                	 latestEndTime: expectedDummyTask.latestEndTime + 1, 
                	 duration: expectedDummyTask.duration + 1,
                	 status: 'Missed',
                	 recurringLateOffset: expectedDummyTask.recurringLateOffset + 1, 
                	 recurringPeriod: expectedDummyTask.recurringPeriod + 1
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
		
		it("gets events table with single scheduled event", function(done) {
			
			var dummyStartTime = Math.round((new Date()).getTime() / 1000) + 60;
			
			testApi.insertTask(
					expectedDummyTask.name,
					dummyStartTime,
					dummyStartTime + expectedDummyTask.duration,
					expectedDummyTask.duration,
					"Incomplete",
					expectedDummyTask.recurringPeriod,
					expectedDummyTask.recurringLateOffset,
					function(){
						testApi.getEvents(function(result){
							
							var expectedTable = [
			                     {
			                    	 eventId: 0,
			                    	 taskId: 1,
			                    	 name: expectedDummyTask.name,
			                    	 startTime: dummyStartTime,
			                    	 duration: expectedDummyTask.duration
			                	 }
			    			];
							
							expect(result).toEqual(expectedTable);
							done();
						});
					});
		});
		
		it("inserts a single event",function(done){
			insertDummyEvent(function(result){
					expect(result).toEqual(true);
					done();
			});
		});
		
		it("gets an inserted event in table", function(done) {
			
			var expectedTable = [expectedDummyEvent];
			
			insertDummyEvent(function(){
				testApi.getEvents(function(result){
					expect(result).toEqual(expectedTable);
					done();
				});
			});
		});
		
		it("removes a single event", function(done) {
			insertDummyEvent(function(){
				removeDummyEvent(function(result){
					expect(result).toEqual(true);
					done();
				});
			});
		});
		
		it("gets empty events table after remove", function(done) {
			insertDummyEvent(function(){
				removeDummyEvent(function(result){
					testApi.getEvents(function(result){
						expect(result).toEqual([]);
						done();
					});
				});
			});
		});
		
		afterEach(function(done) {
			testApi.reset(function(){
				done();
			},
			function(){
				done();
			});
		});

	});

});