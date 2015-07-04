define( [ 'js/calendar_form', 'test/dummy_api' ], function(libCalendarForm,libDummyApi) {
	
	describe("CalendarFormLib Suite", function() {
		
		var testDiv = document.getElementById("test_div");
		var testForm = null;
		var dummyEditTaskForm = null;
		var testApi = null;
		
		function DummyForm(){}
		DummyForm.prototype.render = function(parent){};
		
		beforeEach(function() {
			testApi = new libDummyApi.DummyApi();
			dummyEditTaskForm = new DummyForm();
			testForm = new libCalendarForm.CalendarForm(testApi,dummyEditTaskForm);
			
			spyOn(testApi, 'getEvents');
			spyOn(dummyEditTaskForm,'render');
		});
		
		it("is allocated", function() {
			expect(testForm).not.toEqual(null);
		});
		
		it("renders form", function() {
			testForm.render(testDiv);
			expect(testDiv.innerHTML).not.toBe("");
		});
		
		it("renders edit task form", function(){
			testForm.render(testDiv);
			expect(dummyEditTaskForm.render).toHaveBeenCalled();
		});
		
		it("queries events on refresh", function() {
			testForm.render(testDiv);
			testForm.refresh();
			expect(testApi.getEvents).toHaveBeenCalled();
		});
		
		it("converts server event to calendar event", function() {
			
			var startTime = new Date();
			
			var dummyEvent = {
				name: "test task",
				startTime: startTime.getTime() / 1000,
				duration: 60*60
			};
			
			var convertedEvent = testForm.convertServerEventToCalendarEvent(dummyEvent);
			
			expect(convertedEvent.title).toBe(dummyEvent.name);
			expect(convertedEvent.start.getTime()).toBe(dummyEvent.startTime * 1000);
			expect(convertedEvent.end.getTime()).toBe((dummyEvent.startTime + dummyEvent.duration) * 1000);
		});
		
		afterEach(function() {
			testForm = null;
			testDiv.innerHTML = "";
		});
		
	});
	
});