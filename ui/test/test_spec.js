define( [ 'js/api' ], function(libapi) {

	describe("ApiLib Suite", function() {
		
		var testApi = null;
		
		beforeEach(function() {
			testApi = new libapi.Api();
		});

		it("should say hello", function(done) {
			
			testApi.sayHello("test",function(result){
				
				expect(result).toEqual("Hello: test");
				done();
			});
		});

		afterEach(function() {
			testApi = null;
		});

	});

});