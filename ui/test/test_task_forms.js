define( [ 'js/task_forms', 'test/dummy_api' ], function(libTaskForms,libDummyApi) {
	
	function roundTimeToMinute(time)
	{
		return Math.floor(time.getTime() / 60 / 1000) * 60 * 1000;
	}
	
	var testDiv = document.getElementById("test_div");
	var testForm = null;
	var testApi = null;
	
	describe("TaskFormsLib Add Task Suite", function() {
		
		beforeEach(function() {
			testApi = new libDummyApi.DummyApi();
			testForm = new libTaskForms.AddTaskForm(testApi);
			
			spyOn(testApi,'insertTask');
		});
		
		it("is allocated", function() {
			expect(testForm).not.toEqual(null);
		});
		
		it("renders form", function() {
			testForm.render(testDiv);
			expect(testDiv.innerHTML).not.toBe("");
		});
		
		it("adds a task on a button click", function() {
			
			testForm.render(testDiv);
			
			var now = new Date();
			var testName = "test name";
			var testStatus = "Complete";
			var testDuration = 3600 * 24 + 1 * 3600 + 60;
			var testRecurrancePeriod = 2 * 3600 * 24 + 1 * 3600 + 60;
			var testRecurranceLateOffset = 3 * 3600 * 24 + 1 * 3600 + 60;
			
			testForm.nameInput.setValue(testName);
			$(testForm.earliestStartTimeInput).datetimepicker('setDate', now );
			$(testForm.latestEndTimeInput).datetimepicker('setDate', now );
			testForm.statusInput.setValue(testStatus);
			testForm.durationInput.setValue(testDuration);
			testForm.recurrancePeriodInput.setValue(testRecurrancePeriod);
			testForm.recurranceLateOffsetInput.setValue(testRecurranceLateOffset);
			
			testForm.submitButton.click();
			
			var expectedTime = roundTimeToMinute(now) / 1000;
			
			var callArgs = testApi.insertTask.calls.argsFor(0);
			expect(callArgs[0]).toBe(testName);
			expect(callArgs[1]).toBe(expectedTime);
			expect(callArgs[2]).toBe(expectedTime);
			expect(callArgs[3]).toBe(testDuration);
			expect(callArgs[4]).toBe(testStatus);
			expect(callArgs[5]).toBe(testRecurrancePeriod);
			expect(callArgs[6]).toBe(testRecurranceLateOffset);
		});
		
		afterEach(function() {
			testForm = null;
			testApi = null;
			testDiv.innerHTML = "";
		});
		
	});
	
	describe("TaskFormsLib Edit Task Suite", function() {
		
		beforeEach(function() {
			testApi = new libDummyApi.DummyApi();
			testForm = new libTaskForms.UpdateTaskForm(testApi);
			
			spyOn(testApi,'updateTask');
			spyOn(testApi,'removeTask');
			
			testForm.render(testDiv);
		});
		
		it("is allocated", function() {
			expect(testForm).not.toEqual(null);
		});
		
		it("renders form", function() {
			expect(testDiv.innerHTML).not.toBe("");
		});
		
		it("upates a task on submit button click", function() {
			
			var now = new Date();
			var dummyTaskId = 1;
			var testName = "test name";
			var testDuration = 3600 * 24 + 1 * 3600 + 60;
			var testRecurrancePeriod = 2 * 3600 * 24 + 1 * 3600 + 60;
			var testRecurranceLateOffset = 3 * 3600 * 24 + 1 * 3600 + 60;
			var testStatus = "Complete";
			
			testForm.taskIdInput.setValue(dummyTaskId);
			testForm.nameInput.setValue(testName);
			testForm.earliestStartTimeInput.setValue(now);
			testForm.latestEndTimeInput.setValue(now);
			testForm.statusInput.setValue(testStatus);
			testForm.durationInput.setValue(testDuration);
			testForm.recurrancePeriodInput.setValue(testRecurrancePeriod);
			testForm.recurranceLateOffsetInput.setValue(testRecurranceLateOffset);
			
			testForm.submitButton.click();
			
			var expectedTime = roundTimeToMinute(now) / 1000;
			
			var callArgs = testApi.updateTask.calls.argsFor(0);
			expect(callArgs[0]).toBe(dummyTaskId);
			expect(callArgs[1]).toBe(testName);
			expect(callArgs[2]).toBe(expectedTime);
			expect(callArgs[3]).toBe(expectedTime);
			expect(callArgs[4]).toBe(testDuration);
			expect(callArgs[5]).toBe(testStatus);
			expect(callArgs[6]).toBe(testRecurrancePeriod);
			expect(callArgs[7]).toBe(testRecurranceLateOffset);
		});
		
		it("removes a task on submit button click", function() {
			
			var now = new Date();
			var dummyTaskId = 1;
			
			testForm.taskIdInput.setValue(dummyTaskId);
			
			testForm.deleteButton.click();
			
			var expectedTime = roundTimeToMinute(now) / 1000;
			
			var callArgs = testApi.removeTask.calls.argsFor(0);
			expect(callArgs[0]).toBe(dummyTaskId);
		});
		
		it("fills task data on set", function(){
			
			var taskId = 1;
			var name = "test task";
			var now = new Date();
			var dummyTime = Math.round(roundTimeToMinute(now) / 1000);
			var duration = 1 * 3600 * 24 + 1 * 3600 + 60;
			var recurrancePeriod = 2 * 3600 * 24 + 2 * 3600 + 60;
			var recurranceLateOffset = 3 * 3600 * 24 + 3 * 3600 + 60;
			
			testForm.setTaskData(taskId,name,dummyTime,dummyTime,duration,recurrancePeriod,recurranceLateOffset);
			
			expect(parseInt(testForm.taskIdInput.getValue())).toBe(taskId);
			expect(testForm.nameInput.getValue()).toBe(name);
			expect(testForm.earliestStartTimeInput.getValue().getTime()).toBe(dummyTime * 1000);
			expect(testForm.latestEndTimeInput.getValue().getTime()).toBe(dummyTime * 1000);
			expect(testForm.durationInput.getValue()).toBe(duration);
			expect(testForm.recurrancePeriodInput.getValue()).toBe(recurrancePeriod);
			expect(testForm.recurranceLateOffsetInput.getValue()).toBe(recurranceLateOffset);
		});
		
		afterEach(function() {
			testForm = null;
			testApi = null;
			testDiv.innerHTML = "";
		});
		
	});
	
});