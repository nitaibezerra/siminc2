<?php
$dataForm = $this->dataForm;

$where = '';
if(is_array($dataForm)){
    $values = array();
    foreach($dataForm as $data){
        if(!empty($data['value'])){
            switch ($data['name']) {
                case 'prgid':
                    $values[] = " prg.{$data['name']} = '{$data['value']}'";
                    break;
                case 'subprgid':
                    $values[] = "subprg.{$data['name']} = '{$data['value']}'";
                    break;
                case 'entdsc':
                    $values[] = " lower(ent.{$data['name']}) LIKE lower('%{$data['value']}%')";
                    break;
//                case 'estdsc':
//                    $values[] = " est.{$data['name']} = '{$data['value']}' ";
//                    break;

                default:
                    break;
            }
        }
    }
    
    
    
    if(count($values) > 0 ){
        $where = 'WHERE ' . implode(' AND ', $values);
    }
}
//ent.entid ,
$sql = "SELECT ent.entid , prg.prgdsc  , subprg.subprgdsc , est.esttitulo , TO_CHAR(ent.entdtcad , 'DD/MM/YYYY') , ent.entdsc , ent.enthrsexec
        FROM scrum.entregavel ent
        LEFT JOIN scrum.estoria est ON est.estid = ent.estid
        LEFT JOIN scrum.subprg subprg ON subprg.subprgid = est.subprgid
        LEFT JOIN scrum.programa prg ON prg.prgid = subprg.prgid
        {$where}
        ORDER BY ent.entdtcad DESC , prg.prgdsc, subprg.subprgdsc, est.esttitulo, ent.enthrsexec , ent.entdsc";

$heads = array('Projeto' , 'Subprojeto',  'Estoria' , 'Data' , 'Entregavel', 'Horas');
$actions = array('edit' => 'editPostit', 'delete' => 'deletePostit');

$list = new Listing();
$list->setHead($heads);
$list->setActions($actions);
$list->listing($sql);

//listagem($sql, $heads, $config);
?>

<script lang="javascript">
/**
 * Comment
 */
function editPostit(id)
{
    $.renderAjax({controller: 'postit', action: 'form', container: 'container_form_postit', dataForm : {id : id}});
    return false;
}

/**
 * Comment
 */
function deletePostit(id)
{
    $.deleteItem({controller: 'postit', action: 'delete', text : 'Deseja realmente deletar este entregável?', id: id, functionSucsess: 'search'});
}

</script>