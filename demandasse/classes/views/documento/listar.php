<?php
$this->listing->listing($this->data);

//ver($this);

//        ver($_POST, d);
//        $this->render(__CLASS__, __FUNCTION__);
?>

<script language="JavaScript">

        /**
         * Comment
         */
//            function editPostit(id)
//        {
//            $.renderAjax({controller: 'postit', action: 'form', container: 'container_form_postit', dataForm : {id : id}});
//            return false;
//        }

    /**
     * Comment
     */
//    function deletePostit(id)
//    {
//        $.deleteItem({controller: 'postit', action: 'delete', text : 'Deseja realmente deletar este entregável?', id: id, functionSucsess: 'search'});
//    }
//
//
    function editar(id)
    {
        $.post(window.location.href, {'controller': 'documento', 'action': 'formulario', 'id': id}, function(html) {
            $('#modal').html(html).modal('show');
        });
    }

    function excluir(id)
    {
        $.deleteItem({controller: 'documento', action: 'deletar', text : 'Deseja realmente deletar este documento?', id: id, functionSucsess: 'fecharModal'});
    }

    function file(id)
    {
        $.post(window.location.href, {'controller': 'documentoarquivo', 'action': 'index', 'id': id}, function(html) {
            $('#modal').html(html).modal('show');
        });
    }


    function enviaremail(id)
    {
    	if (document.getElementById('dtprazo['+id+']') != null) {
	    	var vPrazo = document.getElementById('dtprazo['+id+']').value;
	    	
	    	if(vPrazo != 'red'){
	    		alert("Para Enviar E-mail, é necessário que o prazo do documento esteja vencido!");
	    		return false;
	    	}else{
		    	$.post(window.location.href, {'controller': 'documento', 'action': 'formularioMensagem', 'id': id}, function(html) {
		            $('#modal').html(html).modal('show');
		        });
	    	}
    	} else {
    		alert("Para Enviar E-mail, é necessário que o documento possua um prazo vencido!");
    		return false;
    	}
    }

</script>