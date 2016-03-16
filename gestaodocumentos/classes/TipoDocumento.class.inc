<?php

class TipoDocumento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela 		= "gestaodocumentos.tipodocumento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria 		= array( "tpdid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     		= array(
									  	'tpdid' => null,
									  	'tpddescricao' => null, 
									  	'tpdstatus' => null
									  );

    protected $arObrigatorios		= array(
    									'tpddescricao',
    									'tpdstatus'
    								  );

    protected $arLabels				= array(
    									'Descrição',
    									'Status'
    								  );

    public function getAtributos(){
    	return array_keys($this->arAtributos);
    }

    public function getObrigatorios(){
    	return $this->arObrigatorios;
    }

    public function getLabels(){
    	return $this->arLabels;
    }

    public function listaTipoDocumento(){
    	if( $_REQUEST['tipo_fil'] == 'fil' ){
            echo "<script>abrirPesquisa('mais');</script>";
            if ($_REQUEST['tpddescricao_pes']) {
                $tpddescricao = removeAcentos(trim($_REQUEST['tpddescricao_pes']));
                $where = " AND public.removeacento(td.tpddescricao) ilike ('%{$tpddescricao}%') ";
            }
        }else{
            $where = "";
        }

        $acao = "
            <center>
                <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscaDados('||td.tpdid||');\" title=\"Editar Solicitante\" >
                <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirDados('||td.tpdid||');\" title=\"Excluir Solicitante\" >
            </center>        
        ";

        $sql = "
            SELECT  '{$acao}' as acao,
                    td.tpddescricao,
                    CASE 
                        WHEN td.tpdstatus = 'A' THEN '<font style=\"color:green\">Ativo</font>'
                        WHEN td.tpdstatus = 'I' THEN '<font style=\"color:red\">Inativo</font>'
                    END as tpdstatus
            FROM {$this->stNomeTabela} AS td
            WHERE 1=1 {$where} 
            ORDER BY td.tpddescricao
        ";
        // ver(simec_htmlentities($sql),d);
        $cabecalho = array("A&ccedil;&atilde;o", "Descri&ccedil;&atilde;o", "Status");
        $alinhamento = Array('center', 'left', 'left');
        $tamanho = Array('3%', '40%', '30%');
        $this->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
    }

    public function buscaDados($dados){
        $tpdid = $dados['pk'];

        $sql = "
            SELECT * FROM {$this->stNomeTabela} WHERE tpdid = {$tpdid};
        ";
        $dados = $this->pegaLinha($sql);
        
        $dados["tpddescricao"] = iconv("ISO-8859-1", "UTF-8", $dados["tpddescricao"]);
        
        echo simec_json_encode($dados);
        die;

    }

    public function excluirDados( $dados ){
        $sql = "
            DELETE FROM {$this->stNomeTabela} WHERE tpdid = {$dados['pk']} RETURNING tpdid;
        ";
        $solid = $this->pegaUm($sql);

        if($solid > 0){
            $this->commit();
            echo '<resp>';
            echo 'OK';
            echo '</resp>';
        }
        die();
    }

    public function salvarDados($dados){
        global $db;

        extract($dados);
        
        if($tpdid == ''){
            $sql = "
                INSERT INTO {$this->stNomeTabela}(
                    tpddescricao,
                    tpdstatus
                )VALUES(
                    '{$tpddescricao}',
                    '{$tpdstatus}'
                )RETURNING tpdid;
             ";
        }else{
            $sql = "
                UPDATE {$this->stNomeTabela}
                        SET tpddescricao  = '{$tpddescricao}',
                        tpdstatus = '{$tpdstatus}'
                WHERE tpdid = {$tpdid} RETURNING tpdid;
            ";
        }
        $tpdid = $db->pegaUm($sql);
        
        if($tpdid > 0){
            $db->commit();
            $db->sucesso('', '');
        }
    }
}