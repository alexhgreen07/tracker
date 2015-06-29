define( [ 'js/task_forms' ], function(libTaskForms) {
	
	describe("TaskFormsLib Add Task Suite", function() {
		
		var testDiv = document.getElementById("test_div");
		var testForm = null;
		var testApi = null;
		
		function DummyApi()
		{
			this.lastAddedTask = null;
		}
		DummyApi.prototype.insertTask = function(earliestStartTime,latestEndTime,duration,success,error)
		{
			this.lastAddedTask = {
				earliestStartTime: earliestStartTime,
				latestEndTime: latestEndTime,
				duration: duration
			};
			
			success(true);
		};
		
		function roundTimeToMinute(time)
		{
			return Math.floor(time.getTime() / 60 / 1000) * 60 * 1000;
		}
		
		beforeEach(function() {
			testApi = new DummyApi();
			testForm = new libTaskForms.AddTaskForm(testApi);
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
			
			//round to minute
			var expectedTime = roundTimeToMinute(now);
			expect(testApi.lastAddedTask.earliestStartTime).toEqual(expectedTime);
			expect(testApi.lastAddedTask.latestEndTime).toEqual(expectedTime);
			expect(testApi.lastAddedTask.duration).toEqual(0);
		});
		
		afterEach(function() {
			testForm = null;
			testApi = null;
			testDiv.innerHTML = "";
		});
		
	});
	
});