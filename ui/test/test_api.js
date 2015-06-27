define( [ 'js/api' ], function(libapi) {

	describe("ApiLib Suite", function() {
		
		var earliestStartTime = 1;
		var latestEndTime = 2;
		var duration = 3;
		
		var testApi = null;
		
		function insertDummyTask(success,error)
		{
			testApi.insertTask(earliestStartTime,latestEndTime,duration,success,error);
		}
		
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

		it("inserts single task", function(done) {
			insertDummyTask(function(result){
				expect(result).toEqual(true);
				done();
			});
		});
		
		it("gets a single task in table", function(done) {
			
			var expectedTable = [
                 {
                	 taskId: 1,
                	 earliestStartTime: earliestStartTime, 
                	 latestEndTime: latestEndTime, 
                	 duration: duration
            	 }
			];
			
			insertDummyTask(function(){
				testApi.getTasks(function(result){
					
					expect(result).toEqual(expectedTable);
					done();
				});
			});
		});

		it("gets empty events table", function(done) {
			testApi.getEvents(function(result){
				expect(result).toEqual([]);
				done();
			});
		});
		
		afterEach(function(done) {
			testApi.exit(function(){done();},function(){done();});
			testApi = null;
		});

	});

});