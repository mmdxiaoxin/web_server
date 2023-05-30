$(function() {
	$('#upload-form').submit(function(event) {
		event.preventDefault();
		var formData = new FormData(this);

		$.ajax({
			url: 'http://localhost:5000/upload',
			type: 'POST',
			data: formData,
			processData: false,
			contentType: false,
			success: function(data) {
				$('#result').html('<p>文件上传成功!</p>');
				alert('文件上传成功!');
			},
			error: function(jqXHR, textStatus, errorThrown) {
				$('#result').html('<p>文件上传失败: ' + errorThrown + '</p>');
			}
		});
	});
});