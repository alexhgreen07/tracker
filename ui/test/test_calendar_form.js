define( [ 'js/calendar_form' ], function(libCalendarForm) {
	
	describe("CalendarFormLib Suite", function() {
		
		var testDiv = document.getElementById("test_div");
		var testForm = null;
		
		beforeEach(function() {
			testForm = new libCalendarForm.CalendarForm();
		});
		
		it("is allocated", function() {
			expect(testForm).not.toEqual(null);
		});
		
		it("renders form", function() {
			testForm.render(testDiv);
			expect(testDiv.innerHTML).not.toBe("");
		});
		
		afterEach(function() {
			testForm = null;
			testDiv.innerHTML = "";
		});
		
	});
	
});