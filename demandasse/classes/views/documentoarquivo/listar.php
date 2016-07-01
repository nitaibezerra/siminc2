<?php
$this->listing->listing($this->data);
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
    function editarArquivo(id)
    {
        $.post(window.location.href, {'controller': 'documentoarquivo', 'action': 'formulario', 'id': id , editar : true}, function(html) {
            $('#container_formulario_arquivo').hide().html(html).fadeIn();
        });
    }

    function excluirArquivo(id)
    {
        $.deleteItem({controller: 'documentoarquivo', action: 'deletar', text : 'Deseja realmente deletar este documento?', id: id, functionSucsess: 'fecharModal'});
    }

    function downloadArquivo(id)
    {
        window.location.href = '/demandasse/download.php?dmaid=' + id;
    }


</script>