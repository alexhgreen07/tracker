define( [ 'moment', 'jquery', 'jqueryui', 'datetimepicker' ], function(moment,$) {
	
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
				this.nameInput.value,
				$(this.earliestStartTimeInput).datetimepicker('getDate').getTime() / 1000,
				$(this.latestEndTimeInput).datetimepicker('getDate').getTime() / 1000,
				this.getTimestampFromInputs(this.durationDayInput,this.durationInput),
				this.statusInput.value,
				this.getTimestampFromInputs(this.recurrancePeriodDayInput,this.recurrancePeriodInput),
				this.getTimestampFromInputs(this.recurranceLateOffsetDayInput,this.recurranceLateOffsetInput),
				this.submitSuccess.bind(this),
				this.submitError.bind(this));
	};
	AddTaskForm.prototype.renderEntryInputs = function(div)
	{
		div.appendChild(document.createTextNode("Name"));
		div.appendChild(document.createElement("br"));
		this.nameInput = div.appendChild(document.createElement("input"));
		this.nameInput.type = "text";
		this.nameInput.value = "Task Name";
		
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
		this.durationDayInput = div.appendChild(document.createElement("input"));
		this.durationDayInput.type = "text";
		this.durationDayInput.value = 0;
		this.durationInput = div.appendChild(document.createElement("input"));
		this.durationInput.type = "text";

		div.appendChild(document.createElement("br"));
		div.appendChild(document.createElement("br"));
		
		div.appendChild(document.createTextNode("Status"));
		div.appendChild(document.createElement("br"));
		this.statusInput = div.appendChild(document.createElement("select"));
		this.statusInput.options[this.statusInput.options.length] = new Option("Incomplete","Incomplete");
		this.statusInput.options[this.statusInput.options.length] = new Option("Complete","Complete");
		this.statusInput.options[this.statusInput.options.length] = new Option("Missed","Missed");
		
		div.appendChild(document.createElement("br"));
		div.appendChild(document.createElement("br"));
		
		div.appendChild(document.createTextNode("Recurrance Period"));
		div.appendChild(document.createElement("br"));
		this.recurrancePeriodDayInput = div.appendChild(document.createElement("input"));
		this.recurrancePeriodDayInput.type = "text";
		this.recurrancePeriodDayInput.value = 0;
		this.recurrancePeriodInput = div.appendChild(document.createElement("input"));
		this.recurrancePeriodInput.type = "text";
		
		div.appendChild(document.createElement("br"));
		div.appendChild(document.createElement("br"));
		
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
		
		$(this.earliestStartTimeInput).datetimepicker();
		$(this.earliestStartTimeInput).datetimepicker('setDate',new Date());
		$(this.latestEndTimeInput).datetimepicker();
		$(this.latestEndTimeInput).datetimepicker('setDate',new Date());

		var d = new Date(1970,1,1,1,0,0,0); 
		var timePickerOptions = {
			timeOnly: true,
			timeFormat: 'HH:mm',
		};
		$(this.durationInput).datetimepicker(timePickerOptions);
		$(this.durationInput).datetimepicker('setDate',d);
		$(this.recurrancePeriodInput).datetimepicker(timePickerOptions);
		$(this.recurrancePeriodInput).datetimepicker('setDate','00:00');
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
		var momentDuration = moment.duration(duration, 'seconds');
		var newDuration = new Date(0,0,0,momentDuration.hours(),momentDuration.minutes(),0,0);
		var recurrancePeriodDuration = moment.duration(recurrancePeriod, 'seconds');
		var newRecurrancePeriod = new Date(0,0,0,recurrancePeriodDuration.hours(),recurrancePeriodDuration.minutes(),0,0);
		var recurranceLateOffsetDuration = moment.duration(recurranceLateOffset, 'seconds');
		var newRecurranceLateOffset = new Date(0,0,0,recurranceLateOffsetDuration.hours(),recurranceLateOffsetDuration.minutes(),0,0);
		
		this.taskIdInput.value = taskId;
		this.nameInput.value = name;
		$(this.earliestStartTimeInput).datetimepicker('setDate', newEarliestStartTime );
		$(this.latestEndTimeInput).datetimepicker('setDate', newLatestEndTime );
		this.durationDayInput.value = momentDuration.days();
		$(this.durationInput).datetimepicker('setDate', newDuration );
		this.recurrancePeriodDayInput.value = recurrancePeriodDuration.days();
		$(this.recurrancePeriodInput).datetimepicker('setDate', newRecurrancePeriod );
		this.recurranceLateOffsetDayInput.value = recurranceLateOffsetDuration.days();
		$(this.recurranceLateOffsetInput).datetimepicker('setDate', newRecurranceLateOffset );
	};
	UpdateTaskForm.prototype.submitClickEvent = function()
	{
		this.api.updateTask(
				parseInt(this.taskIdInput.value),
				this.nameInput.value,
				$(this.earliestStartTimeInput).datetimepicker('getDate').getTime() / 1000,
				$(this.latestEndTimeInput).datetimepicker('getDate').getTime() / 1000,
				this.getTimestampFromInputs(this.durationDayInput,this.durationInput),
				this.statusInput.value,
				this.getTimestampFromInputs(this.recurrancePeriodDayInput,this.recurrancePeriodInput),
				this.getTimestampFromInputs(this.recurranceLateOffsetDayInput,this.recurranceLateOffsetInput),
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