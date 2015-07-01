define( [ 'js/calendar_form', 'test/dummy_api' ], function(libCalendarForm,libDummyApi) {
	
	describe("CalendarFormLib Suite", function() {
		
		var testDiv = document.getElementById("test_div");
		var testForm = null;
		var testApi = null;
		
		beforeEach(function() {
			testApi = new libDummyApi.DummyApi();
			testForm = new libCalendarForm.CalendarForm(testApi);
			
			spyOn(testApi, 'getEvents');
		});
		
		it("is allocated", function() {
			expect(testForm).not.toEqual(null);
		});
		
		it("renders form", function() {
			testForm.render(testDiv);
			expect(testDiv.innerHTML).not.toBe("");
		});
		
		it("queries events on refresh", function() {
			testForm.render(testDiv);
			testForm.refresh();
			expect(testApi.getEvents).toHaveBeenCalled();
		});
		
		it("converts server event to calendar event", function() {
			
			var startTime = new Date();
			
			var dummyEvent = {
				startTime: startTime.getTime() / 1000,
				duration: 60*60
			};
			
			var convertedEvent = testForm.convertServerEventToCalendarEvent(dummyEvent);
			
			expect(convertedEvent.title).toBe("Event");
			expect(convertedEvent.start.getTime()).toBe(dummyEvent.startTime * 1000);
			expect(convertedEvent.end.getTime()).toBe((dummyEvent.startTime + dummyEvent.duration) * 1000);
		});
		
		afterEach(function() {
			testForm = null;
			testDiv.innerHTML = "";
		});
		
	});
	
});