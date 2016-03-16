<?php
$dataForm = $this->dataForm;

$where = '';
if(is_array($dataForm)){
    $values = array();
    foreach($dataForm as $data){
        if(!empty($data['value'])){
            $values[] = " {$data['name']} = '{$data['value']}'";
        }
    }
    
    if(count($values) > 0 ){
        $where = 'WHERE ' . implode(' AND ', $values);
    }
}

$sql = "SELECT 
            prgid
            , prgdsc
            , prghrsprint
        FROM scrum.programa
        {$where}
        ORDER BY prgdsc , prghrsprint ";

$heads = array('Projeto' , 'Duracao da sprint (em horas)');
//$config = array();
//$config['actions'] = array('edit' => 'editProgram', 'delete' => 'deleteProgram');
//listagem($sql, $heads, $config);

$actions = array('edit' => 'editProgram', 'delete' => 'deleteProgram');

$list = new Listing();
$list->setTypePage('M');
$list->setHead($heads);
$list->setActions($actions);
$list->listing($sql);

?>

<script lang="javascript">
/**
 * Comment
 */
function editProgram(id)
{
    $.renderAjax({controller: 'program', action: 'form', container: 'container_form_program', dataForm : {id : id}});
    return false;
}

/**
 * Comment
 */
function deleteProgram(id)
{
    $.deleteItem({controller: 'program', action: 'delete', text : 'Deseja realmente deletar este projeto?', id: id, functionSucsess: 'search'});
}

</script>