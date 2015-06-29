define( [ 'js/task_forms' ], function(libTaskForms) {
	
	describe("TaskFormsLib Add Task Suite", function() {
		
		var testDiv = document.getElementById("test_div");
		var testForm = null;
		
		beforeEach(function() {
			testForm = new libTaskForms.AddTaskForm();
		});
		
		it("is allocated", function() {
			expect(testForm).not.toEqual(null);
		});
		
		it("renders form", function() {
			testForm.render(testDiv);
			expect(testDiv.innerHTML).not.toBe("");
			alert("");
		});
		
		afterEach(function() {
			testForm = null;
			testDiv.innerHTML = "";
		});
		
	});
	
});