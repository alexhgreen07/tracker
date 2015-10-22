define([ './form_helpers', 'moment', 'jquery', 'jqueryui', 'datetimepicker' ],function(libFormHelpers,moment,$){
	
	function AddEventForm(api)
	{
		this.api = api;
		
		this.taskIdInput = null;
		this.recurringIndexInput = null;
		this.startTimeInput = null;
		this.durationInput = null;
		this.statusInput = null;
		
		this.resultsDiv = null;
		
		this.submitButton = null;
	}
	
	AddEventForm.prototype.submitSuccess = function()
	{
		this.resultsDiv.innerHTML = "Event submitted";
		setTimeout(this.clearResults.bind(this),2000);
	};
	AddEventForm.prototype.submitError = function()
	{
		this.resultsDiv.innerHTML = "Event submission error";
		setTimeout(this.clearResults.bind(this),2000);
	};
	AddEventForm.prototype.clearResults = function()
	{
		this.resultsDiv.innerHTML = "";
	};
	
	AddEventForm.prototype.submitButtonClick = function()
	{
		this.api.insertEvent(
			parseInt(this.taskIdInput.getValue()),
			this.startTimeInput.getValue().getTime() / 1000,
			this.durationInput.getValue(),
			this.statusInput.getValue(),
			parseInt(this.recurringIndexInput.getValue()),
			this.submitSuccess.bind(this),
			this.submitError.bind(this)
		);
	};
	
	AddEventForm.prototype.renderEntryInputs = function(div)
	{
		this.taskIdInput = new libFormHelpers.TextFormField("Task","");
		this.taskIdInput.render(div);
		
		this.recurringIndexInput = new libFormHelpers.TextFormField("Recurring Index","0");
		this.recurringIndexInput.render(div);
		
		this.startTimeInput = new libFormHelpers.DateTimeFormField("Start Time",(new Date()));
		this.startTimeInput.render(div);
		
		this.durationInput = new libFormHelpers.DurationFormField("Duration",3600);
		this.durationInput.render(div);
		
		this.statusInput = new libFormHelpers.SelectFormField(
				"Status",
				[
				 	new Option("Logged","Logged"),
				 	new Option("Running","Running")
				],
				"Logged");
		this.statusInput.render(div);
		
		div.appendChild(document.createElement("br"));
		div.appendChild(document.createElement("br"));
	}
	
	AddEventForm.prototype.render = function(parent)
	{
		var div = parent.appendChild(document.createElement("div"));
		
		this.renderEntryInputs(div);
		
		this.submitButton = div.appendChild(document.createElement("input"));
		this.submitButton.type = "submit";
		this.submitButton.value = "Submit";
		
		div.appendChild(document.createElement("br"));
		div.appendChild(document.createElement("br"));
		
		this.resultsDiv = div.appendChild(document.createElement("div"));
		
		div.appendChild(document.createElement("br"));

		$(this.submitButton).button();
		$(this.submitButton).click(this.submitButtonClick.bind(this));
	}
	
	AddEventForm.prototype.setEventData = function(taskId,recurringIndex)
	{
		this.taskIdInput.setValue(taskId);
		this.recurringIndexInput.setValue(recurringIndex);
	}
	
	//EditEventForm inherits from AddEventForm
	EditEventForm.prototype = new AddEventForm();
	EditEventForm.prototype.constructor = EditEventForm;
	function EditEventForm(api)
	{
		AddEventForm.prototype.constructor.call(this,api);
		
		this.eventIdInput = null;
		
		this.removeButton = null;
	}
	
	EditEventForm.prototype.setEventData = function(eventId,taskId,startTime,duration,status,recurringIndex)
	{
		AddEventForm.prototype.setEventData.call(this,taskId,recurringIndex);
		
		this.eventIdInput.setValue(eventId);
		this.startTimeInput.setValue(new Date(startTime * 1000));
		this.durationInput.setValue(duration);
		this.statusInput.setValue(status);
	};
	
	EditEventForm.prototype.submitButtonClick = function()
	{
		this.api.updateEvent(
			parseInt(this.eventIdInput.getValue()),
			parseInt(this.taskIdInput.getValue()),
			this.startTimeInput.getValue().getTime() / 1000,
			this.durationInput.getValue(),
			this.statusInput.getValue(),
			parseInt(this.recurringIndexInput.getValue()),
			this.submitSuccess.bind(this),
			this.submitError.bind(this)
		);
	};
	
	EditEventForm.prototype.removeButtonClick = function()
	{
		this.api.removeEvent(
			parseInt(this.eventIdInput.getValue()),
			this.submitSuccess.bind(this),
			this.submitError.bind(this)
		);
	}
	
	EditEventForm.prototype.render = function(parent)
	{
		var div = parent.appendChild(document.createElement("div"));
		
		this.eventIdInput = new libFormHelpers.TextFormField("Event","");
		this.eventIdInput.render(div);
		
		this.renderEntryInputs(div);
		
		this.submitButton = div.appendChild(document.createElement("input"));
		this.submitButton.type = "submit";
		this.submitButton.value = "Submit";
		
		div.appendChild(document.createElement("br"));
		div.appendChild(document.createElement("br"));
		
		this.removeButton = div.appendChild(document.createElement("input"));
		this.removeButton.type = "submit";
		this.removeButton.value = "Delete";
		
		div.appendChild(document.createElement("br"));
		div.appendChild(document.createElement("br"));
		
		this.resultsDiv = div.appendChild(document.createElement("div"));
		
		div.appendChild(document.createElement("br"));

		$(this.submitButton).button();
		$(this.submitButton).click(this.submitButtonClick.bind(this));
		
		$(this.removeButton).button();
		$(this.removeButton).click(this.removeButtonClick.bind(this));
	}
	
	return {
		AddEventForm: AddEventForm,
		EditEventForm: EditEventForm
	};
});