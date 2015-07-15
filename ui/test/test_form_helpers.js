define( [ 'js/form_helpers' ], function(libFormHelpers) {
	
	var testDiv = document.getElementById("test_div");
	
	describe("Form Helpers Text Input Suite", function() {
		
		var testInput = null;
		var testName = "Name";
		var defaultValue = "Value";
		
		beforeEach(function() {
			testInput = new libFormHelpers.TextFormField(testName,defaultValue);
			testInput.render(testDiv);
		});
		
		it("is allocated", function() {
			expect(testInput).not.toEqual(null);
		});
		
		it("renders the input", function() {
			expect(testDiv.innerHTML).toContain(testName);
		});
		
		it("is default value", function() {
			expect(testInput.getValue()).toEqual(defaultValue);
		});
		
		it("sets value", function() {
			var newValue = "New Name";
			testInput.setValue(newValue);
			expect(testInput.getValue()).toEqual(newValue);
		});
		
		it("validates input HTML", function() {
			expect(testInput.textInput.value).toEqual(defaultValue);
		});
		
		afterEach(function() {
			testInput = null;
			testDiv.innerHTML = "";
		});
		
	});
	
});