<div class="col-lg-12">
    <div class="page-header">
        <h1 id="forms">
            <!--                Dados da universidade --->
            <small>
                Lista de Reitoria
            </small>
        </h1>
    </div>
</div>
<?php
$this->listing->listing($this->data);
//        ver($_POST, d);
//        $this->render(__CLASS__, __FUNCTION__);
?>
<script language="JavaScript">
    $('.bt-inserir').click(function() {

//        $('#container-bt-inserir').hide();
        $('#container-formulario').fadeIn();
        $.post(window.location.href, {'controller': 'campus', 'action': 'formulario'}, function(html) {
            $('#modal').html(html).modal('show');
        });
    });

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
        $('#container-formulario').fadeIn();
        $.post(window.location.href, {'controller': 'campus', 'action': 'formulario', 'id': id}, function(html) {
            $('#modal').html(html).modal('show');
        });
    }

    function excluir(id)
    {
        $.deleteItem({controller: 'campus', action: 'deletar', text : 'Deseja realmente deletar este campus?', id: id, functionSucsess: 'fecharModal'});
    }


</script>