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
    function editar(id)
    {
        $.post(window.location.href, {'controller': 'procedencia', 'action': 'formulario', 'id': id}, function(html) {
            $('#modal').html(html).modal('show');
        });
    }

    function excluir(id)
    {
        $.deleteItem({controller: 'procedencia', action: 'deletar', text : 'Deseja realmente deletar esta procedencia?', id: id, functionSucsess: 'fecharModal'});
    }


</script>