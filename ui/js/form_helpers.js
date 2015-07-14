define( [ 'moment', 'jquery', 'jqueryui', 'datetimepicker' ], function(moment,$) {
	
	function FormField(name)
	{
		this.name = name;
		
		this.parent = null;
	}
	FormField.prototype.getValue = function()
	{
		return null;
	}
	FormField.prototype.setValue = function()
	{
		//to be overridden
	}
	FormField.prototype.render = function(parent)
	{
		this.parent = parent;
	}
	
	TextFormField.prototype = new FormField();
	TextFormField.prototype.constructor = TextFormField;
	function TextFormField(name,defaultValue)
	{
		FormField.prototype.constructor.call(this,name);
		
		this.defaultValue = defaultValue;
		this.textInput = null;
	}
	TextFormField.prototype.getValue = function()
	{
		return this.textInput.value;
	}
	TextFormField.prototype.setValue = function(value)
	{
		this.textInput.value = value;
	}
	TextFormField.prototype.render = function(parent)
	{
		FormField.prototype.render.call(this,parent);
		
		this.parent.appendChild(document.createTextNode(this.name));
		this.parent.appendChild(document.createElement("br"));
		this.textInput = this.parent.appendChild(document.createElement("input"));
		this.textInput.type = "text";
		this.textInput.value = this.defaultValue;
		
		this.parent.appendChild(document.createElement("br"));
		this.parent.appendChild(document.createElement("br"));
	}
	
	DateTimeFormField.prototype = new TextFormField();
	DateTimeFormField.prototype.constructor = DateTimeFormField;
	function DateTimeFormField(name,defaultValue)
	{
		TextFormField.prototype.constructor.call(this,name,"");
		
		this.defaultValue = defaultValue;
	}
	DateTimeFormField.prototype.getValue = function()
	{
		return $(this.textInput).datetimepicker('getDate');
	}
	DateTimeFormField.prototype.setValue = function(value)
	{
		$(this.textInput).datetimepicker('setDate',value);
	}
	DateTimeFormField.prototype.render = function(parent)
	{
		TextFormField.prototype.render.call(this,parent);

		$(this.textInput).datetimepicker();
		$(this.textInput).datetimepicker('setDate',this.defaultValue);
	}
	
	DurationFormField.prototype = new FormField();
	DurationFormField.prototype.constructor = DurationFormField;
	function DurationFormField(name,defaultValue)
	{
		FormField.prototype.constructor.call(this,name);
		
		this.defaultValue = defaultValue;
		this.textDayInput = null;
		this.textTimeInput = null;
	}
	DurationFormField.prototype.getValue = function()
	{
		var returnTimeStamp = 0;
		
		returnTimeStamp = parseInt(this.textDayInput.value) * 3600 * 24;
		var time = $(this.textTimeInput).datetimepicker('getDate');
		returnTimeStamp += time.getHours() * 3600 + time.getMinutes() * 60;
		
		return returnTimeStamp;
	}
	DurationFormField.prototype.setValue = function(value)
	{
		var momentDuration = moment.duration(value, 'seconds');
		var newDuration = new Date(0,0,0,momentDuration.hours(),momentDuration.minutes(),0,0);
		this.textDayInput.value = momentDuration.days();
		$(this.textTimeInput).datetimepicker('setDate', newDuration );
	}
	DurationFormField.prototype.render = function(parent)
	{
		FormField.prototype.render.call(this,parent);
		
		this.parent.appendChild(document.createTextNode(this.name));
		this.parent.appendChild(document.createElement("br"));
		this.textDayInput = this.parent.appendChild(document.createElement("input"));
		this.textDayInput.type = "text";
		this.textTimeInput = this.parent.appendChild(document.createElement("input"));
		this.textTimeInput.type = "text";
		
		this.parent.appendChild(document.createElement("br"));
		this.parent.appendChild(document.createElement("br"));
		
		$(this.textTimeInput).datetimepicker({
			timeOnly: true,
			timeFormat: 'HH:mm',
		});
		
		this.setValue(this.defaultValue);
	}
	
	return {
		FormField: FormField,
		TextFormField: TextFormField,
		DateTimeFormField: DateTimeFormField,
		DurationFormField: DurationFormField
	};
});