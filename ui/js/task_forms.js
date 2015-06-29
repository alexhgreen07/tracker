define( [ 'moment', 'jquery', 'jqueryui' ], function(moment,$) {
	
	function AddTaskForm()
	{
		this.earliestStartTimeInput = null;
		this.latestEndTimeInput = null;
		this.durationInput = null;
		
		this.submitButton = null;
	}
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
		
		div.appendChild(document.createElement("br"));
		div.appendChild(document.createElement("br"));
		
		this.submitButton = div.appendChild(document.createElement("input"));
		this.submitButton.type = "submit";
		this.submitButton.value = "Submit";
		
		div.appendChild(document.createElement("br"));
		
		$(this.submitButton).button();
	};
	
	return {
		AddTaskForm: AddTaskForm
	};
	
});