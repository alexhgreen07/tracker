define( [ 'moment', 'jquery', 'jqueryui', 'datetimepicker' ], function(moment,$) {
	
	function AddTaskForm(api)
	{
		this.api = api;
		
		this.earliestStartTimeInput = null;
		this.latestEndTimeInput = null;
		this.durationInput = null;
		this.resultsDiv = null;
		
		this.submitButton = null;
	}
	AddTaskForm.prototype.submitSuccess = function()
	{
		this.resultsDiv.innerHTML = "Task submitted";
	};
	AddTaskForm.prototype.submitError = function()
	{
		this.resultsDiv.innerHTML = "Task submission error";
	};
	AddTaskForm.prototype.submitClickEvent = function()
	{
		this.api.insertTask(
				$(this.earliestStartTimeInput).datetimepicker('getDate').getTime(),
				$(this.latestEndTimeInput).datetimepicker('getDate').getTime(),
				parseInt(this.durationInput.value),
				this.submitSuccess.bind(this),
				this.submitError.bind(this));
	};
	AddTaskForm.prototype.render = function(parent)
	{
		var div = parent.appendChild(document.createElement("div"));
		
		div.appendChild(document.createTextNode("Earliest Start Time"));
		div.appendChild(document.createElement("br"));
		this.earliestStartTimeInput = div.appendChild(document.createElement("input"));
		this.earliestStartTimeInput.type = "text";
		
		div.appendChild(document.createElement("br"));
		div.appendChild(document.createElement("br"));
		
		div.appendChild(document.createTextNode("Latest End Time"));
		div.appendChild(document.createElement("br"));
		this.latestEndTimeInput = div.appendChild(document.createElement("input"));
		this.latestEndTimeInput.type = "text";
		
		div.appendChild(document.createElement("br"));
		div.appendChild(document.createElement("br"));
		
		div.appendChild(document.createTextNode("Duration"));
		div.appendChild(document.createElement("br"));
		this.durationInput = div.appendChild(document.createElement("input"));
		this.durationInput.type = "text";
		this.durationInput.value = 0;
		
		div.appendChild(document.createElement("br"));
		div.appendChild(document.createElement("br"));
		
		this.resultsDiv = div.appendChild(document.createElement("div"));
		
		div.appendChild(document.createElement("br"));
		div.appendChild(document.createElement("br"));
		
		this.submitButton = div.appendChild(document.createElement("input"));
		this.submitButton.type = "submit";
		this.submitButton.value = "Submit";
		
		div.appendChild(document.createElement("br"));

		$(this.earliestStartTimeInput).datetimepicker();
		$(this.latestEndTimeInput).datetimepicker();
		$(this.submitButton).button();
		$(this.submitButton).click(this.submitClickEvent.bind(this));
	};
	
	return {
		AddTaskForm: AddTaskForm
	};
	
});