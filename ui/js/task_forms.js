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
	AddTaskForm.prototype.getTimestampFromInputs = function(dayInput, timeInput)
	{
		var returnTimeStamp = 0;
		
		returnTimeStamp = parseInt(dayInput.value) * 3600 * 24;
		var time = $(timeInput).datetimepicker('getDate');
		returnTimeStamp += time.getHours() * 3600 + time.getMinutes() * 60;
		
		return returnTimeStamp;
	};
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
				this.getTimestampFromInputs(this.recurranceLateOffsetDayInput,this.recurranceLateOffsetInput),
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

		div.appendChild(document.createTextNode("Recurrance Late Offset"));
		div.appendChild(document.createElement("br"));
		this.recurranceLateOffsetDayInput = div.appendChild(document.createElement("input"));
		this.recurranceLateOffsetDayInput.type = "text";
		this.recurranceLateOffsetDayInput.value = 0;
		this.recurranceLateOffsetInput = div.appendChild(document.createElement("input"));
		this.recurranceLateOffsetInput.type = "text";
		
		div.appendChild(document.createElement("br"));
		
		this.resultsDiv = div.appendChild(document.createElement("div"));
		
		div.appendChild(document.createElement("br"));

		var d = new Date(1970,1,1,1,0,0,0); 
		var timePickerOptions = {
			timeOnly: true,
			timeFormat: 'HH:mm',
		};
		$(this.recurranceLateOffsetInput).datetimepicker(timePickerOptions);
		$(this.recurranceLateOffsetInput).datetimepicker('setDate','00:00');
		
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
		
		this.taskIdInput.value = taskId;
		this.nameInput.value = name;
		$(this.earliestStartTimeInput).datetimepicker('setDate', newEarliestStartTime );
		$(this.latestEndTimeInput).datetimepicker('setDate', newLatestEndTime );
		this.durationInput.setValue(duration);
		this.recurrancePeriodInput.setValue(recurrancePeriod);
		this.recurranceLateOffsetDayInput.value = recurranceLateOffsetDuration.days();
		$(this.recurranceLateOffsetInput).datetimepicker('setDate', newRecurranceLateOffset );
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
				this.getTimestampFromInputs(this.recurranceLateOffsetDayInput,this.recurranceLateOffsetInput),
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