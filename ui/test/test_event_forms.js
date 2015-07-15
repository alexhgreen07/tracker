define( [ 'js/event_forms', 'test/dummy_api' ], function(libEventForms,libDummyApi) {
	
	var testDiv = document.getElementById("test_div");
	var testForm = null;
	var testApi = null;
	
	describe("Event Forms Add Event Suite", function() {
		
		beforeEach(function() {

			testForm = new libEventForms.AddEventForm(testApi);
		});
		
		it("is allocated", function() {
			expect(testForm).not.toEqual(null);
		});
		
		afterEach(function() {
			testForm = null;
			testApi = null;
			testDiv.innerHTML = "";
		});
		
	});
	
});