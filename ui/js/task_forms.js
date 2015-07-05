define( [ 'moment', 'jquery', 'jqueryui', 'datetimepicker' ], function(moment,$) {
	
	function AddTaskForm(api)
	{
		this.api = api;
		
		this.nameInput = null;
		this.earliestStartTimeInput = null;
		this.latestEndTimeInput = null;
		this.durationInput = null;
		this.resultsDiv = null;
		
		this.submitButton = null;
	}
	AddTaskForm.prototype.clearResults = function()
	{
		this.resultsDiv.innerHTML = "";
	};
	AddTaskForm.prototype.submitSuccess = function()
	{
		this.resultsDiv.innerHTML = "Task submitted";
		setTimeout(this.clearResults.bind(this),2000);
	};
	AddTaskForm.prototype.submitError = function()
	{
		this.resultsDiv.innerHTML = "Task submission error";
		setTimeout(this.clearResults.bind(this),2000);
	};
	AddTaskForm.prototype.submitClickEvent = function()
	{
		this.api.insertTask(
				this.nameInput.value,
				$(this.earliestStartTimeInput).datetimepicker('getDate').getTime() / 1000,
				$(this.latestEndTimeInput).datetimepicker('getDate').getTime() / 1000,
				parseInt(this.durationInput.value),
				this.submitSuccess.bind(this),
				this.submitError.bind(this));
	};
	AddTaskForm.prototype.renderEntryInputs = function(div)
	{
		div.appendChild(document.createTextNode("Name"));
		div.appendChild(document.createElement("br"));
		this.nameInput = div.appendChild(document.createElement("input"));
		this.nameInput.type = "text";
		
		div.appendChild(document.createElement("br"));
		div.appendChild(document.createElement("br"));
		
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
		
		this.resultsDiv = div.appendChild(document.createElement("div"));
		
		div.appendChild(document.createElement("br"));
		
		$(this.earliestStartTimeInput).datetimepicker();
		$(this.earliestStartTimeInput).datetimepicker('setDate',new Date());
		$(this.latestEndTimeInput).datetimepicker();
		$(this.latestEndTimeInput).datetimepicker('setDate',new Date());
	};
	AddTaskForm.prototype.render = function(parent)
	{
		var div = parent.appendChild(document.createElement("div"));
		
		this.renderEntryInputs(div);
		
		this.submitButton = div.appendChild(document.createElement("input"));
		this.submitButton.type = "submit";
		this.submitButton.value = "Submit";
		
		div.appendChild(document.createElement("br"));

		$(this.submitButton).button();
		$(this.submitButton).click(this.submitClickEvent.bind(this));
	};
	
	//UpdateTaskForm inherits from AddTaskForm
	UpdateTaskForm.prototype = new AddTaskForm();
	UpdateTaskForm.prototype.constructor = UpdateTaskForm;
	function UpdateTaskForm(api)
	{
		this.api = api;
		
		this.taskIdInput = null;
		this.deleteButton = null;
	}
	UpdateTaskForm.prototype.setTaskData = function(taskId, name, earliestStartTime, latestEndTime, duration){
		
		var newEarliestStartTime = new Date(earliestStartTime * 1000);
		var newLatestEndTime = new Date(latestEndTime * 1000);
		
		this.taskIdInput.value = taskId;
		this.nameInput.value = name;
		$(this.earliestStartTimeInput).datetimepicker('setDate', newEarliestStartTime );
		$(this.latestEndTimeInput).datetimepicker('setDate', newLatestEndTime );
		this.durationInput.value = duration;
	};
	UpdateTaskForm.prototype.submitClickEvent = function()
	{
		this.api.updateTask(
				parseInt(this.taskIdInput.value),
				this.nameInput.value,
				$(this.earliestStartTimeInput).datetimepicker('getDate').getTime() / 1000,
				$(this.latestEndTimeInput).datetimepicker('getDate').getTime() / 1000,
				parseInt(this.durationInput.value),
				this.submitSuccess.bind(this),
				this.submitError.bind(this));
	};
	UpdateTaskForm.prototype.deleteClickEvent = function()
	{
		this.api.removeTask(
				parseInt(this.taskIdInput.value),
				this.submitSuccess.bind(this),
				this.submitError.bind(this));
	};
	UpdateTaskForm.prototype.render = function(parent)
	{
		var div = parent.appendChild(document.createElement("div"));
		
		div.appendChild(document.createTextNode("Task"));
		div.appendChild(document.createElement("br"));
		this.taskIdInput = div.appendChild(document.createElement("input"));
		this.taskIdInput.type = "text";
		
		div.appendChild(document.createElement("br"));
		div.appendChild(document.createElement("br"));
		
		this.renderEntryInputs(div);
		
		this.submitButton = div.appendChild(document.createElement("input"));
		this.submitButton.type = "submit";
		this.submitButton.value = "Submit";
		
		div.appendChild(document.createElement("br"));
		div.appendChild(document.createElement("br"));
		
		this.deleteButton = div.appendChild(document.createElement("input"));
		this.deleteButton.type = "submit";
		this.deleteButton.value = "Delete";
		
		div.appendChild(document.createElement("br"));

		$(this.submitButton).button();
		$(this.submitButton).click(this.submitClickEvent.bind(this));
		$(this.deleteButton).button();
		$(this.deleteButton).click(this.deleteClickEvent.bind(this));
	};
	
	return {
		AddTaskForm: AddTaskForm,
		UpdateTaskForm: UpdateTaskForm
	};
	
});