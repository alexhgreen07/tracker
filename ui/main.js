define(['jquery','jqueryjsonrpc'],function($){

   idx = 1;
   
   function doAjaxRequest(m, parameters, target) {
	   var tmpObject = {jsonrpc: 2.0, method: m, params: parameters, id: idx++};
	   var string = JSON.stringify(tmpObject);
	   
	   $.jsonRPC.setup({
		   endPoint: '/',
		   namespace: ''
	   });
	   
		$.jsonRPC.request(m, {
			params: parameters,
			success: function(data) {
				$(target).html(data.result);
			},
			error: function(result) {
				alert("Ajax failed to fetch data");
			}
		 });
   }
   
	$(document).ready(function(){
		$("#testform").submit(function() {
		   doAjaxRequest('sayHello',{name:$("#thename").val()},"#result");
		   return false;
	   });
	});
	   
});
