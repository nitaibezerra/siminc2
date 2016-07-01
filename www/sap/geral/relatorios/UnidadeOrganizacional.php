<?php 
include "config.inc";
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db = new cls_banco();
    
$unidade_organizacional = strtoupper($_REQUEST['term']);

$sqlUnidadeOrganizacional = "select 
                                nu_matricula_siape, no_servidor 
                        from 
                                siape.vwservidorativo
                        where 
                                no_servidor like '%{$unidade_organizacional}%'";
$dados = $db->carregar($sqlUnidadeOrganizacional);     

exit($sqlUnidadeOrganizacional);
$comma_separated = array();
foreach ($dados as $key => $value){
    $array = array('value'=> utf8_encode($value['no_servidor']) , 'id'=> $value['nu_matricula_siape']);
    $comma_separated[] = $array;
}
echo simec_json_encode($comma_separated);