define( [ 'js/main',  'test/dummy_api' ], function(libMain,libDummyApi) {
	
	describe("MainLib Suite", function() {
		
		var testApplication = null;
		var dummyApi = null;
		var dummyCalendarForm = null;
		var dummyAddTaskForm = null;
		var testDiv = document.getElementById("test_div");
		
		function DummyForm(){}
		DummyForm.prototype.render = function(parent){};
		
		beforeEach(function() {
			dummyApi = new libDummyApi.DummyApi();
			dummyCalendarForm = new DummyForm();
			dummyAddTaskForm = new DummyForm();
			testApplication = new libMain.Application(dummyApi,dummyCalendarForm,dummyAddTaskForm);
			
			spyOn(dummyCalendarForm,'render');
			spyOn(dummyAddTaskForm,'render');
		});
		
		it("is allocated", function() {
			expect(testApplication).not.toEqual(null);
		});
		
		it("renders main application", function() {
			testApplication.render(testDiv);
			expect(testDiv.innerHTML).not.toBe("");
		});
		
		it("renders calendar form", function() {
			testApplication.render(testDiv);
			expect(dummyCalendarForm.render).toHaveBeenCalled();
		});
		
		it("renders add task form", function() {
			testApplication.render(testDiv);
			expect(dummyAddTaskForm.render).toHaveBeenCalled();
		});
		
		afterEach(function() {
			testApplication = null;
			testDiv.innerHTML = "";
		});
		
	});
	
});