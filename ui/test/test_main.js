define( [ 'js/main',  'test/dummy_api' ], function(libMain,libDummyApi) {
	
	describe("MainLib Suite", function() {
		
		var testApplication = null;
		var dummyApi = null;
		var dummyCalendarForm = null;
		var dummyAddTaskForm = null;
		var testDiv = document.getElementById("test_div");
		
		function DummyForm()
		{
			this.hasRendered = false;
		}
		DummyForm.prototype.render = function(parent)
		{
			this.hasRendered = true;
		};
		
		beforeEach(function() {
			dummyApi = new libDummyApi.DummyApi();
			dummyCalendarForm = new DummyForm();
			dummyAddTaskForm = new DummyForm();
			testApplication = new libMain.Application(dummyApi,dummyCalendarForm,dummyAddTaskForm);
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
			expect(dummyCalendarForm.hasRendered).toBe(true);
		});
		
		it("renders add task form", function() {
			testApplication.render(testDiv);
			expect(dummyAddTaskForm.hasRendered).toBe(true);
		});
		
		afterEach(function() {
			testApplication = null;
			testDiv.innerHTML = "";
		});
		
	});
	
});