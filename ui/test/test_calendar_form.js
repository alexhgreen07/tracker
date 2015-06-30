define( [ 'js/calendar_form', 'test/dummy_api' ], function(libCalendarForm,libDummyApi) {
	
	describe("CalendarFormLib Suite", function() {
		
		var testDiv = document.getElementById("test_div");
		var testForm = null;
		var testApi = null;
		
		beforeEach(function() {
			testApi = new libDummyApi.DummyApi();
			testForm = new libCalendarForm.CalendarForm(testApi);
		});
		
		it("is allocated", function() {
			expect(testForm).not.toEqual(null);
		});
		
		it("renders form", function() {
			testForm.render(testDiv);
			expect(testDiv.innerHTML).not.toBe("");
		});
		
		it("queries events on refresh", function(done) {
			testForm.render(testDiv);
			testForm.refresh(function(){
				expect(testApi.getEventsCalled).toBe(true);
				done();
			});
		});
		
		afterEach(function() {
			testForm = null;
			testDiv.innerHTML = "";
		});
		
	});
	
});