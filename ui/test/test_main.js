define( [ 'js/main',  'js/api', 'js/calendar_form' ], function(libMain,libApi,libCalendarForm) {
	
	describe("MainLib Suite", function() {
		
		var testApplication = null;
		var testDiv = document.getElementById("test_div");
		
		beforeEach(function() {
			testApplication = libMain.buildApplication();
		});
		
		it("is allocated", function() {
			expect(testApplication).not.toEqual(null);
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