define( [ 'js/form_helpers' ], function(libFormHelpers) {
	
	var testDiv = document.getElementById("test_div");
	var testInput = null;
	var testName = "Name";
	
	describe("Form Helpers Text Input Suite", function() {

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
			var newValue = "New Value";
			testInput.setValue(newValue);
			expect(testInput.getValue()).toEqual(newValue);
		});
		
		it("validates input value", function() {
			expect(testInput.textInput.value).toEqual(defaultValue);
		});
		
		afterEach(function() {
			testInput = null;
			testDiv.innerHTML = "";
		});
		
	});
	
	describe("Form Helpers DateTime Input Suite", function() {

		var defaultValue = new Date(Math.round((new Date()).getTime() / 1000 / 60) * 1000 * 60);
		
		beforeEach(function() {
			testInput = new libFormHelpers.DateTimeFormField(testName,defaultValue);
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
			var newValue = new Date(Math.round((new Date()).getTime() / 1000 / 60 - 1) * 1000 * 60);
			testInput.setValue(newValue);
			expect(testInput.getValue()).toEqual(newValue);
		});
		
		it("validates input value", function() {
			expect($(testInput.textInput).datetimepicker('getDate')).toEqual(defaultValue);
		});
		
		afterEach(function() {
			testInput = null;
			testDiv.innerHTML = "";
		});
		
	});
	
	describe("Form Helpers Duartion Input Suite", function() {
		
		var defaultValue = 60 + 3600 + 3600 * 24;
		
		beforeEach(function() {
			testInput = new libFormHelpers.DurationFormField(testName,defaultValue);
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
			var newValue = defaultValue - 60;
			testInput.setValue(newValue);
			expect(testInput.getValue()).toEqual(newValue);
		});
		
		it("validates input value", function() {
			expect(testInput.textDayInput.value).toBe("1");
			expect(testInput.textTimeInput.value).toBe("01:01");
		});
		
		afterEach(function() {
			testInput = null;
			testDiv.innerHTML = "";
		});
	});
	
});