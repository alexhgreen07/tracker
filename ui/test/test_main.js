define( [ 'js/main' ], function(libmain) {
	
	describe("MainLib Suite", function() {
		
		var testApplication = null;
		var testDiv = document.getElementById("test_div");
		
		beforeEach(function() {
			testApplication = new libmain.Application();
		});
		
		it("renders main application", function() {
			testApplication.render(testDiv);
			expect(testDiv.innerHTML).not.toBe("");
		});
		
		afterEach(function() {
			testApplication = null;
			testDiv.innerHTML = "";
		});
		
	});
	
});