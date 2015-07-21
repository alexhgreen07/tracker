define([ './form_helpers', 'moment', 'jquery', 'jqueryui', 'datetimepicker' ],function(libFormHelpers,moment,$){
	
	function AddEventForm()
	{
		this.taskIdInput = null;
		this.startTimeInput = null;
		this.durationInput = null;
		
		this.submitButton = null;
	}
	
	AddEventForm.prototype.renderEntryInputs = function(div)
	{
		this.taskIdInput = new libFormHelpers.TextFormField("Task","");
		this.taskIdInput.render(div);
		
		this.startTimeInput = new libFormHelpers.DateTimeFormField("Start Time",(new Date()));
		this.startTimeInput.render(div);
		
		this.durationInput = new libFormHelpers.DurationFormField("Duration",3600);
		this.durationInput.render(div);
	}
	
	AddEventForm.prototype.render = function(parent)
	{
		var div = parent.appendChild(document.createElement("div"));
		
		this.renderEntryInputs(div);
		
		this.submitButton = div.appendChild(document.createElement("input"));
		this.submitButton.type = "submit";
		this.submitButton.value = "Submit";
		
		div.appendChild(document.createElement("br"));

		$(this.submitButton).button();
		//$(this.submitButton).click(this.submitClickEvent.bind(this));
	}
	
	//EditEventForm inherits from AddEventForm
	EditEventForm.prototype = new AddEventForm();
	EditEventForm.prototype.constructor = EditEventForm;
	function EditEventForm()
	{
		this.eventIdInput = null;
		
		this.removeButton = null;
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

		$(this.submitButton).button();
		//$(this.submitButton).click(this.submitClickEvent.bind(this));
		
		$(this.removeButton).button();
		//$(this.removeButton).click(this.removeClickEvent.bind(this));
	}
	
	return {
		AddEventForm: AddEventForm,
		EditEventForm: EditEventForm
	};
});