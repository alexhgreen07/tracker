define( [ 'js/event_forms', 'test/dummy_api' ], function(libEventForms,libDummyApi) {
	
	function roundTimeToMinute(time)
	{
		return Math.floor(time.getTime() / 60 / 1000) * 60 * 1000;
	}
	
	var testDiv = document.getElementById("test_div");
	var testForm = null;
	var testApi = null;
	
	describe("Event Forms Add Event Suite", function() {
		
		beforeEach(function() {
			testApi = new libDummyApi.DummyApi();
			testForm = new libEventForms.AddEventForm(testApi);
			testForm.render(testDiv);
			
			spyOn(testApi,'insertEvent');
		});
		
		it("is allocated", function() {
			expect(testForm).not.toEqual(null);
		});
		
		it("renders form", function() {
			expect(testDiv.innerHTML).not.toBe("");
		});
		
		it("add an event on submit button click", function() {
			
			var taskId = 1;
			var startTime = new Date();
			var duration = 3600;
			
			testForm.taskIdInput.setValue(taskId);
			testForm.startTimeInput.setValue(startTime);
			testForm.durationInput.setValue(duration);
			
			testForm.submitButton.click();
			
			var callArgs = testApi.insertEvent.calls.argsFor(0);
			
			expect(callArgs[0]).toBe(taskId);
			expect(callArgs[1]).toBe(roundTimeToMinute(startTime) / 1000);
			expect(callArgs[2]).toBe(duration);
		});
		
		afterEach(function() {
			testForm = null;
			testApi = null;
			testDiv.innerHTML = "";
		});
		
	});
	
	describe("Event Forms Edit Event Suite", function() {
		
		beforeEach(function() {
			testApi = new libDummyApi.DummyApi();
			testForm = new libEventForms.EditEventForm(testApi);
			
			testForm.render(testDiv);
			
			spyOn(testApi,'removeEvent');
		});
		
		it("is allocated", function() {
			expect(testForm).not.toEqual(null);
		});
		
		it("renders form", function() {
			expect(testDiv.innerHTML).not.toBe("");
		});
		
		it("removes event on remove button click", function() {
			
			var eventId = 1;
			testForm.eventIdInput.setValue(eventId);
			
			testForm.removeButton.click();
			
			var callArgs = testApi.removeEvent.calls.argsFor(0);
			expect(callArgs[0]).toBe(eventId);
		});
		
		afterEach(function() {
			testForm = null;
			testApi = null;
			testDiv.innerHTML = "";
		});
	});
	
});