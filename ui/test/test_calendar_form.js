define( [ 'js/calendar_form', 'test/dummy_api' ], function(libCalendarForm,libDummyApi) {
	
	var testDiv = document.getElementById("test_div");
	var testForm = null;
	
	var dummyCalEvent = {
		serverEvent: {
			taskId: 1
		}
	};
	
	function DummyForm(){}
	DummyForm.prototype.render = function(parent){};
	
	describe("TaskActionForm Suite", function(){
		
		var dummyEditTaskForm = null;
		var dummyAddEventForm = null;
		var dummyEditEventForm = null;
		
		beforeEach(function() {
			dummyEditTaskForm = new DummyForm();
			dummyAddEventForm = new DummyForm();
			dummyEditEventForm = new DummyForm();
			testForm = new libCalendarForm.TaskActionForm(dummyEditTaskForm,dummyAddEventForm,dummyEditEventForm);
			
			spyOn(dummyEditTaskForm,'render');
			spyOn(dummyAddEventForm,'render');
			spyOn(dummyEditEventForm,'render');
			
			testForm.render(testDiv);
		});
		
		it("is allocated", function() {
			expect(testForm).not.toEqual(null);
		});
		
		it("renders form", function() {
			expect(testDiv.innerHTML).not.toBe("");
		});
		
		it("renders child forms", function(){
			expect(dummyEditTaskForm.render).toHaveBeenCalled();
			expect(dummyAddEventForm.render).toHaveBeenCalled();
			expect(dummyEditEventForm.render).toHaveBeenCalled();
		});
		
		it("displays edit task form on button click", function() {
			testForm.editTaskButton.click();
			expect($(testForm.editTaskFormDiv).is(":visible")).toBe(true);
			expect($(testForm.buttonsDiv).is(":visible")).toBe(false);
		});
		
		it("displays add event form on button click", function() {
			testForm.addEventButton.click();
			expect($(testForm.addEventFormDiv).is(":visible")).toBe(true);
			expect($(testForm.buttonsDiv).is(":visible")).toBe(false);
		});
		
		it("displays edit event form on button click", function() {
			testForm.editEventButton.click();
			expect($(testForm.editEventFormDiv).is(":visible")).toBe(true);
			expect($(testForm.buttonsDiv).is(":visible")).toBe(false);
		});
		
		afterEach(function() {
			testForm = null;
			testDiv.innerHTML = "";
		});
	});
	
	describe("CalendarForm Suite", function() {
		
		var dummyTaskLookup = {
			1: {
				taskId: 1,
				name: "test task",
				earliestStartTime: 0,
				latestEndTime: 0,
				duration: 0
			}
		};
		
		var dummyTaskActionsForm = null;
		var testApi = null;
		
		beforeEach(function() {
			testApi = new libDummyApi.DummyApi();
			dummyTaskActionsForm = new DummyForm();
			dummyTaskActionsForm.editTaskForm = {setTaskData: function(task){}};
			testForm = new libCalendarForm.CalendarForm(testApi,dummyTaskActionsForm);
			
			testApi.taskLookup = dummyTaskLookup;
			
			spyOn(testApi, 'getEvents');
			spyOn(dummyTaskActionsForm,'render');
			spyOn(dummyTaskActionsForm.editTaskForm,'setTaskData');
			
			testForm.render(testDiv);
		});
		
		it("is allocated", function() {
			expect(testForm).not.toEqual(null);
		});
		
		it("renders form", function() {
			expect(testDiv.innerHTML).not.toBe("");
		});
		
		it("renders task actions form", function(){
			expect(dummyTaskActionsForm.render).toHaveBeenCalled();
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
		
		it("displays task actions form on event click", function() {
			testForm.eventClick(dummyCalEvent);
			expect($(testForm.taskActionFormDiv).is(":visible")).toBe(true);
			expect($(testForm.calendarDiv).is(":visible")).toBe(false);
		});
		
		it("fills task data in edit task form on event click", function(){
			testForm.eventClick(dummyCalEvent);
			expect(dummyTaskActionsForm.editTaskForm.setTaskData).toHaveBeenCalled();
		});
		
		it("displays calendar on back click", function() {
			testForm.backButton.click();
			expect($(testForm.taskActionFormDiv).is(":visible")).toBe(false);
			expect($(testForm.calendarDiv).is(":visible")).toBe(true);
		});
		
		afterEach(function() {
			testForm = null;
			testDiv.innerHTML = "";
		});
		
	});
	
});