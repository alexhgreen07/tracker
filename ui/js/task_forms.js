define( [ './form_helpers', 'moment', 'jquery', 'jqueryui', 'datetimepicker' ], function(libFormHelpers,moment,$) {
	
	function AddTaskForm(api)
	{
		this.api = api;
		
		this.nameInput = null;
		this.earliestStartTimeInput = null;
		this.latestEndTimeInput = null;
		this.statusInput = null;
		this.durationInput = null;
		this.durationDayInput = null;
		this.recurrancePeriodInput = null;
		this.recurrancePeriodDayInput = null;
		this.recurranceLateOffsetInput = null;
		this.recurranceLateOffsetDayInput = null;
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
				this.nameInput.getValue(),
				this.earliestStartTimeInput.getValue().getTime() / 1000,
				this.latestEndTimeInput.getValue().getTime() / 1000,
				this.durationInput.getValue(),
				this.statusInput.value,
				this.recurrancePeriodInput.getValue(),
				this.recurranceLateOffsetInput.getValue(),
				this.submitSuccess.bind(this),
				this.submitError.bind(this));
	};
	AddTaskForm.prototype.renderEntryInputs = function(div)
	{
		this.nameInput = new libFormHelpers.TextFormField("Name","Task Name");
		this.nameInput.render(div);
		
		this.earliestStartTimeInput = new libFormHelpers.DateTimeFormField("Earliest Start Time",(new Date()));
		this.earliestStartTimeInput.render(div);
		
		this.latestEndTimeInput = new libFormHelpers.DateTimeFormField("Latest End Time",(new Date()));
		this.latestEndTimeInput.render(div);
		
		this.durationInput = new libFormHelpers.DurationFormField("Duration",3600);
		this.durationInput.render(div);
		
		div.appendChild(document.createTextNode("Status"));
		div.appendChild(document.createElement("br"));
		this.statusInput = div.appendChild(document.createElement("select"));
		this.statusInput.options[this.statusInput.options.length] = new Option("Incomplete","Incomplete");
		this.statusInput.options[this.statusInput.options.length] = new Option("Complete","Complete");
		this.statusInput.options[this.statusInput.options.length] = new Option("Missed","Missed");
		
		div.appendChild(document.createElement("br"));
		div.appendChild(document.createElement("br"));
		
		this.recurrancePeriodInput = new libFormHelpers.DurationFormField("Recurrance Period",0);
		this.recurrancePeriodInput.render(div);
		
		this.recurranceLateOffsetInput = new libFormHelpers.DurationFormField("Recurrance Late Offset",0);
		this.recurranceLateOffsetInput.render(div);

		div.appendChild(document.createElement("br"));
		
		this.resultsDiv = div.appendChild(document.createElement("div"));
		
		div.appendChild(document.createElement("br"));
		
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
	UpdateTaskForm.prototype.setTaskData = function(taskId, name, earliestStartTime, latestEndTime, duration, recurrancePeriod, recurranceLateOffset){
		
		var newEarliestStartTime = new Date(earliestStartTime * 1000);
		var newLatestEndTime = new Date(latestEndTime * 1000);
		var recurranceLateOffsetDuration = moment.duration(recurranceLateOffset, 'seconds');
		var newRecurranceLateOffset = new Date(0,0,0,recurranceLateOffsetDuration.hours(),recurranceLateOffsetDuration.minutes(),0,0);
		
		this.taskIdInput.setValue(taskId);
		this.nameInput.setValue(name);
		this.earliestStartTimeInput.setValue(newEarliestStartTime);
		this.latestEndTimeInput.setValue(newLatestEndTime);
		this.durationInput.setValue(duration);
		this.recurrancePeriodInput.setValue(recurrancePeriod);
		this.recurranceLateOffsetInput.setValue(recurranceLateOffset);
	};
	UpdateTaskForm.prototype.submitClickEvent = function()
	{
		this.api.updateTask(
				parseInt(this.taskIdInput.getValue()),
				this.nameInput.getValue(),
				this.earliestStartTimeInput.getValue().getTime() / 1000,
				this.latestEndTimeInput.getValue().getTime() / 1000,
				this.durationInput.getValue(),
				this.statusInput.value,
				this.recurrancePeriodInput.getValue(),
				this.recurranceLateOffsetInput.getValue(),
				this.submitSuccess.bind(this),
				this.submitError.bind(this));
	};
	UpdateTaskForm.prototype.deleteClickEvent = function()
	{
		this.api.removeTask(
				parseInt(this.taskIdInput.getValue()),
				this.submitSuccess.bind(this),
				this.submitError.bind(this));
	};
	UpdateTaskForm.prototype.render = function(parent)
	{
		var div = parent.appendChild(document.createElement("div"));
		
		this.taskIdInput = new libFormHelpers.TextFormField("Task","");
		this.taskIdInput.render(div);

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