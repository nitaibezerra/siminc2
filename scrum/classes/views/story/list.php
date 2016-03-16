<?php


$dataForm = $this->dataForm;

$where = '';
if(is_array($dataForm)){
    $values = array();
    foreach($dataForm as $data){
        
        if(!empty($data['value'])){
            switch ($data['name']) {
                case 'prgid':
                    $values[] = " prog.{$data['name']} = '{$data['value']}'";
                    break;
                case 'subprgid':
                    $values[] = "subp.{$data['name']} = '{$data['value']}'";
                    break;
                case 'esttitulo':
                    $values[] = " est.{$data['name']} = '{$data['value']}'";
                    break;
                case 'estdsc':
                    $values[] = " est.{$data['name']} LIKE '&{$data['value']}&' ";
                    break;

                default:
                    break;
            }
        }
        
    }
    
    if(count($values) > 0 ){
        $where = 'WHERE ' . implode(' AND ', $values);
    }
}

$sql = "SELECT est.estid 
                    , prog.prgdsc
                    , subp.subprgdsc
                    , est.esttitulo 
                    , estdsc 
                  FROM scrum.estoria est 
                  LEFT JOIN scrum.subprg subp ON ( subp.subprgid = est.subprgid )
                  LEFT JOIN scrum.programa prog ON ( prog.prgid = subp.prgid )
                  {$where}
                  ORDER BY prog.prgdsc , subp.subprgdsc , est.esttitulo
                  ";

$heads = array('Projeto' , 'Subprojeto' , 'Estoria', 'Descricao');
//$config = array();
//$config['actions'] = array('edit' => 'editStory', 'delete' => 'deleteStory');
//
//listagem($sql, $heads, $config);


$actions = array('edit' => 'editStory', 'delete' => 'deleteStory');

$list = new Listing();
//$list->setTypePage('M');
$list->setHead($heads);
$list->setActions($actions);
$list->listing($sql);
?>
<script lang="javascript">
    
/**
 * Comment
 */
function editStory(id)
{
    $('#formSaveStory').find('.has-error').removeClass('has-error');
    dataForm = { controller : 'story', action : 'entityJson' ,id: id};
    
    $.post(window.location.href, dataForm, function(result) {
        $('#formSaveStory #estid').val(result.estid.value);
        $('#formSaveStory #prgid').val(result.prgid.value);
        $.post(window.location.href, {controller: 'story', action: 'selectSubProgram', prgid: result.prgid.value}, function(html) {
            $('#container_select_subprograma').hide().html(html).fadeIn();
            $('form #subprgid').val(result.subprgid.value);
        });
        $('#formSaveStory #esttitulo').val(result.esttitulo.value);
        $('#formSaveStory #estdsc').val(result.estdsc.value);
        
//        console.info(result['prgid']);
//        console.info(result.prgid.value);
    },'json');
    
    $('#formSaveStory').find('#buttonClear').hide();
    $('#formSaveStory').find('#buttonSave').hide();
    $('#formSaveStory').find('#buttonSearch').hide();
    $('#formSaveStory').find('#buttonCancel').fadeIn();
    $('#formSaveStory').find('#buttonEdit').fadeIn();
    
}

/**
 * Comment
 */
function deleteStory(id)
{
    $.deleteItem({controller: 'story', action: 'delete', text : 'Deseja realmente deletar esta estória?', id: id, functionSucsess: 'search'});
}

</script>
    
