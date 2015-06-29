define( [ 'js/calendar_form' ], function(libCalendarForm) {
	
	describe("CalendarFormLib Suite", function() {
		
		var testForm = null;
		
		beforeEach(function() {
			testForm = new libCalendarForm.CalendarForm();
		});
		
		it("is allocated", function() {
			expect(testForm).not.toEqual(null);
		});
		
		afterEach(function() {
			testForm = null;
		});
		
	});
	
});