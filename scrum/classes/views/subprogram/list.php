<?php
$dataForm = $this->dataForm;

$where = '';
if(is_array($dataForm)){
    $values = array();
    foreach($dataForm as $data){
        if(!empty($data['value'])){
            switch ($data['name']) {
                case 'subprgdsc':
                    $values[] = " subprg.{$data['name']} = '{$data['value']}'";
                    break;
                case 'prgid':
                    $values[] = " prg.{$data['name']} = '{$data['value']}'";
                    break;
                case 'sisid':
                    $values[] = "sis.{$data['name']} = '{$data['value']}'";
                    break;
                case 'sidid':
                    $values[] = "sid.{$data['name']} = '{$data['value']}'";
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

$sql = "SELECT subprg.subprgid , subprg.subprgdsc , prg.prgdsc , sisdsc , upper(sid.sidabrev) || ' - ' || sid.siddescricao AS siddsc 
        FROM scrum.subprg subprg
        LEFT JOIN scrum.programa prg ON prg.prgid = subprg.prgid
        LEFT JOIN seguranca.sistema sis ON sis.sisid = subprg.sisid
        LEFT JOIN demandas.sistemadetalhe sid ON sid.sidid = subprg.sidid
        {$where}
        ORDER BY prg.prgdsc , sisdsc , siddsc , subprg.subprgdsc";

$heads = array('Subprojeto' , 'Projeto' , 'Sistema do demandas' , 'Sistema');
//$config = array();
//$config['actions'] = array('edit' => 'editSubProgram', 'delete' => 'deleteSubProgram');
//
//listagem($sql, $heads, $config);

$actions = array('edit' => 'editSubProgram', 'delete' => 'deleteSubProgram');

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
function editSubProgram(id)
{
    $.renderAjax({controller: 'subprogram', action: 'form', container: 'container_form_subprogram', dataForm : {id : id}});
    return false;
}

/**
 * Comment
 */
function deleteSubProgram(id)
{
    $.deleteItem({controller: 'subprogram', action: 'delete', text : 'Deseja realmente deletar este sub-projeto?', id: id, functionSucsess: 'search'});
}

</script>