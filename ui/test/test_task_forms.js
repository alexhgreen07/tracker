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
			
			testForm.nameInput.value = "test name";
			$(testForm.earliestStartTimeInput).datetimepicker('setDate', now );
			$(testForm.latestEndTimeInput).datetimepicker('setDate', now );
			testForm.durationInput.value = 1;
			testForm.recurrancePeriodInput.value = 2;
			testForm.recurranceLateOffsetInput.value = 3;
			
			testForm.submitButton.click();
			
			var expectedTime = roundTimeToMinute(now) / 1000;
			
			var callArgs = testApi.insertTask.calls.argsFor(0);
			expect(callArgs[0]).toBe(testForm.nameInput.value);
			expect(callArgs[1]).toBe(expectedTime);
			expect(callArgs[2]).toBe(expectedTime);
			expect(callArgs[3]).toBe(1);
			expect(callArgs[4]).toBe(2);
			expect(callArgs[5]).toBe(3);
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
			
			testForm.taskIdInput.value = dummyTaskId;
			testForm.nameInput.value = "test name";
			$(testForm.earliestStartTimeInput).datetimepicker('setDate', now );
			$(testForm.latestEndTimeInput).datetimepicker('setDate', now );
			testForm.durationInput.value = 0;
			
			testForm.submitButton.click();
			
			var expectedTime = roundTimeToMinute(now) / 1000;
			
			var callArgs = testApi.updateTask.calls.argsFor(0);
			expect(callArgs[0]).toBe(dummyTaskId);
			expect(callArgs[1]).toBe(testForm.nameInput.value);
			expect(callArgs[2]).toBe(expectedTime);
			expect(callArgs[3]).toBe(expectedTime);
			expect(callArgs[4]).toBe(0);
		});
		
		it("removes a task on submit button click", function() {
			
			var now = new Date();
			var dummyTaskId = 1;
			
			testForm.taskIdInput.value = dummyTaskId;
			
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
			var duration = 10;
			
			testForm.setTaskData(taskId,name,dummyTime,dummyTime,duration);
			
			expect(parseInt(testForm.taskIdInput.value)).toBe(taskId);
			expect(testForm.nameInput.value).toBe(name);
			expect($(testForm.earliestStartTimeInput).datetimepicker('getDate').getTime()).toBe(dummyTime * 1000);
			expect($(testForm.latestEndTimeInput).datetimepicker('getDate').getTime()).toBe(dummyTime * 1000);
			expect(parseInt(testForm.durationInput.value)).toBe(duration);
		});
		
		afterEach(function() {
			testForm = null;
			testApi = null;
			testDiv.innerHTML = "";
		});
		
	});
	
});