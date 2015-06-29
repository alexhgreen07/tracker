define( [ 'js/task_forms' ], function(libTaskForms) {
	
	describe("TaskFormsLib Add Task Suite", function() {
		
		var testForm = null;
		
		beforeEach(function() {
			testForm = new libTaskForms.AddTaskForm();
		});
		
		it("is allocated", function() {
			expect(testForm).not.toEqual(null);
		});
		
		afterEach(function() {
			testForm = null;
		});
		
	});
	
});