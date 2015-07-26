define( [ 'js/calendar_form', 'test/dummy_api' ], function(libCalendarForm,libDummyApi) {
	
	var testDiv = document.getElementById("test_div");
	var testForm = null;
	
	function DummyForm(){}
	DummyForm.prototype.render = function(parent){};
	DummyForm.prototype.setTaskData = function(){};
	
	describe("TaskActionForm Suite", function(){
		
		beforeEach(function() {
			dummyEditTaskForm = new DummyForm();
			dummyAddEventForm = new DummyForm();
			dummyEditEventForm = new DummyForm();
			testForm = new libCalendarForm.TaskActionForm(dummyEditTaskForm,dummyAddEventForm,dummyEditEventForm);
		});
		
		it("is allocated", function() {
			expect(testForm).not.toEqual(null);
		});
		
		afterEach(function() {
			testForm = null;
			testDiv.innerHTML = "";
		});
	});
	
	describe("CalendarForm Suite", function() {
		
		var dummyCalEvent = {
			serverEvent: {
				taskId: 1
			}
		};
		var dummyTaskLookup = {
			1: {
				taskId: 1,
				name: "test task",
				earliestStartTime: 0,
				latestEndTime: 0,
				duration: 0
			}
		};
		
		var dummyEditTaskForm = null;
		var testApi = null;
		
		beforeEach(function() {
			testApi = new libDummyApi.DummyApi();
			dummyEditTaskForm = new DummyForm();
			testForm = new libCalendarForm.CalendarForm(testApi,dummyEditTaskForm);
			
			testApi.taskLookup = dummyTaskLookup;
			
			spyOn(testApi, 'getEvents');
			spyOn(dummyEditTaskForm,'render');
			spyOn(dummyEditTaskForm,'setTaskData');
			
			testForm.render(testDiv);
		});
		
		it("is allocated", function() {
			expect(testForm).not.toEqual(null);
		});
		
		it("renders form", function() {
			expect(testDiv.innerHTML).not.toBe("");
		});
		
		it("renders edit task form", function(){
			expect(dummyEditTaskForm.render).toHaveBeenCalled();
		});
		
		it("queries events on refresh", function() {
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
			
			expect(convertedEvent.serverEvent).toBe(dummyEvent);
			expect(convertedEvent.title).toBe(dummyEvent.name);
			expect(convertedEvent.start.getTime()).toBe(dummyEvent.startTime * 1000);
			expect(convertedEvent.end.getTime()).toBe((dummyEvent.startTime + dummyEvent.duration) * 1000);
		});
		
		it("displays edit task form on event click", function() {
			testForm.eventClick(dummyCalEvent);
			expect($(testForm.editTaskFormDiv).is(":visible")).toBe(true);
			expect($(testForm.calendarDiv).is(":visible")).toBe(false);
		});
		
		it("fills task data in edit task form on event click", function(){
			testForm.eventClick(dummyCalEvent);
			expect(dummyEditTaskForm.setTaskData).toHaveBeenCalled();
		});
		
		it("displays calendar on back click", function() {
			testForm.backButton.click();
			expect($(testForm.editTaskFormDiv).is(":visible")).toBe(false);
			expect($(testForm.calendarDiv).is(":visible")).toBe(true);
		});
		
		afterEach(function() {
			testForm = null;
			testDiv.innerHTML = "";
		});
		
	});
	
});