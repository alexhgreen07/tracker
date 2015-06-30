define( [ 'js/task_forms', 'test/dummy_api' ], function(libTaskForms,libDummyApi) {
	
	describe("TaskFormsLib Add Task Suite", function() {
		
		var testDiv = document.getElementById("test_div");
		var testForm = null;
		var testApi = null;
		
		function roundTimeToMinute(time)
		{
			return Math.floor(time.getTime() / 60 / 1000) * 60 * 1000;
		}
		
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
			
			$(testForm.earliestStartTimeInput).datetimepicker('setDate', now );
			$(testForm.latestEndTimeInput).datetimepicker('setDate', now );
			testForm.durationInput.value = 0;
			
			testForm.submitButton.click();
			
			var expectedTime = roundTimeToMinute(now) / 1000;
			
			var callArgs = testApi.insertTask.calls.argsFor(0);
			expect(callArgs[0]).toBe(expectedTime);
			expect(callArgs[1]).toBe(expectedTime);
			expect(callArgs[2]).toBe(0);
		});
		
		afterEach(function() {
			testForm = null;
			testApi = null;
			testDiv.innerHTML = "";
		});
		
	});
	
});