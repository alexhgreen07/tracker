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
	
	return {
		FormField: FormField,
		TextFormField: TextFormField,
		DateTimeFormField: DateTimeFormField
	};
});