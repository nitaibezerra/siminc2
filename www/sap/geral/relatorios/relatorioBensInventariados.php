<?php 
include "config.inc";
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$tipo = $_REQUEST['tipo'];
$db = new cls_banco();

    switch ($tipo) {
        
        case "contaContabil":
            $contacontabil = $_REQUEST['contacontabil'];
            $sqlContaContabil = "SELECT 
                                        icbid as codigo, icbdsc as descricao
                                    FROM 
                                        sap.itemcontacontabil
                                    WHERE 
                                        ccbid = '{$contacontabil}'
                                    ORDER BY descricao ASC";
                $dados = $db->carregar($sqlContaContabil);                           
                foreach ($dados as &$element){
                    $element['descricao'] = utf8_encode($element['descricao']);
                }
                echo simec_json_encode($dados);

        break;
        case "endereco":
            $co_endereco = $_REQUEST['endereco'];
            $sqlEnderecoAndar = "select 
                                        andar.enaid as codigo, 
                                        andar.enadescricao as descricao
                                from 
                                        siorg.endereco as endereco
                                inner join siorg.enderecoandar as andar
                                on endereco.endid = andar.endid
                                where 
                                    andar.endid = '{$co_endereco}'
                                order by andar.enadescricao asc";
                $dados = $db->carregar($sqlEnderecoAndar);                           
                foreach ($dados as &$element){
                    $element['descricao'] = utf8_encode($element['descricao']);
                }
                echo simec_json_encode($dados);

        break;
        case "enderecoSala":
            $endereco_andar = $_REQUEST['endereco_andar'];
            $sqlEnderecoSala = "select 
                                    sala.enaid as codigo, sala.easdescricao as descricao 
                                from 
                                    siorg.enderecoandar as andar
                                inner join siorg.enderecoandarsala as sala
                                on sala.enaid = andar.enaid
                                where 
                                    sala.enaid = '{$endereco_andar}'
                                order 
                                    by sala.easdescricao asc";
                $dados = $db->carregar($sqlEnderecoSala);                           
                foreach ($dados as &$element){
                    $element['descricao'] = utf8_encode($element['descricao']);
                }
                echo simec_json_encode($dados);

        break;
        case "matriculaSiape":
            
            $nu_matricula_siape = $_REQUEST['nu_matricula_siape'];
            
            $sqlMatriculaSiape = "select 
                                            nu_matricula_siape, no_servidor 
                                    from 
                                            siape.vwservidorativo 
                                    where 
                                            nu_matricula_siape = '{$nu_matricula_siape}'";
            //exit($sqlMatriculaSiape);
            $dados = $db->carregar($sqlMatriculaSiape); 
            echo($dados[0]['no_servidor']);

        break;
        case "noServidor":
            
            $no_servidor = strtoupper($_REQUEST['term']);
            
            $sqlNomeServidor = "select 
                                            nu_matricula_siape, no_servidor 
                                    from 
                                            siape.vwservidorativo
                                    where 
                                            no_servidor ilike '%{$no_servidor}%'";
                                            //exit($sqlNomeServidor);
            $dados = $db->carregar($sqlNomeServidor);                              
            
            $comma_separated = array();
            foreach ($dados as $key => $value){
                $array = array('value'=> utf8_encode($value['no_servidor']) , 'id'=> $value['nu_matricula_siape']);
                $comma_separated[] = $array;
            }
            echo simec_json_encode($comma_separated);
            
        break;
        case "descricaoBem":
            
            $descricao_do_bem = strtoupper($_REQUEST['term']);
            
            $sqlDescricaoBem = "SELECT matid, matdsc FROM SAP.material where matdsc ilike '%{$descricao_do_bem}%' order by matdsc asc";
            $dados = $db->carregar($sqlDescricaoBem);                              
            
            $comma_separated = array();
            foreach ($dados as $key => $value){
                $array = array('value'=> utf8_encode($value['matdsc']) , 'id'=> $value['matid']);
                $comma_separated[] = $array;
            }
            echo simec_json_encode($comma_separated);
            
        break;    
}
