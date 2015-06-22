idx = 1;

function doAjaxRequest(m, parameters, target) {
	var tmpObject = {jsonrpc: 2.0, method: m, params: parameters, id: idx++};
	var string = JSON.stringify(tmpObject);
	
	$.ajax({
		type: "POST",
		url: ".",
		data: string,
		dataType: "json",
	   contentType: "application/json",
	})
	.done(function(data) {
		$(target).html(data.result);
	})
	.fail(function() {
		alert("Ajax failed to fetch data");
	})
}

$(document).ready(function(){
	$("#testform").submit(function() {
	  doAjaxRequest('sayHello',{name:$("#thename").val()},"#result");
	  return false;
	});
});