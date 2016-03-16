<?php

class Tema extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela 		= "gestaodocumentos.tema";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria 		= array( "temid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     		= array(
									  	'temid' => null,
									  	'temdescricao' => null, 
									  	'temstatus' => null
									  );

    protected $arObrigatorios		= array(
    									'temdescricao',
    									'temstatus'
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

    public function listaTemas(){
    	if( $_REQUEST['tipo_fil'] == 'fil' ){
        	echo "<script>abrirPesquisa('mais');</script>";
            if ($_REQUEST['temdescricao_pes']) {
                $temdescricao = removeAcentos(trim($_REQUEST['temdescricao_pes']));
                $where = " AND public.removeacento(td.temdescricao) ilike ('%{$temdescricao}%') ";
            }
            if($_REQUEST['temstatus_pes']){
            	$temstatus = removeAcentos(trim($_REQUEST['temstatus_pes']));
                $where = " AND td.temstatus = '{$temstatus}' ";
            }
        }else{
            $where = "";
        }

        $acao = "
            <center>
                <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscaDados('||td.temid||');\" title=\"Editar Solicitante\" >
                <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirDados('||td.temid||');\" title=\"Excluir Solicitante\" >
            </center>        
        ";

        $sql = "
            SELECT  '{$acao}' as acao,
                    td.temdescricao,
                    CASE
                    	WHEN td.temstatus = 'A' THEN '<font style=\"color:green\">Ativo</font>'
                    	WHEN td.temstatus = 'I' THEN '<font style=\"color:red\">Inativo</font>'
                    END as temstatus
            FROM {$this->stNomeTabela} AS td
            WHERE 1=1 {$where} 
            ORDER BY td.temdescricao
        ";
        // ver(simec_htmlentities($sql),d);
        $cabecalho = array("A&ccedil;&atilde;o", "Descri&ccedil;&atilde;o", "Status");
        $alinhamento = Array('center', 'left','left');
        $tamanho = Array('3%', '30%','40%');
        $this->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
    }

    public function buscaDados($dados){
    	$temid = $dados['pk'];

	    $sql = "
	        SELECT * 
	        FROM {$this->stNomeTabela} 
	        WHERE temid = {$temid};
	    ";

	    $dados = $this->pegaLinha($sql);
	    
	    $dados["temdescricao"] = iconv("ISO-8859-1", "UTF-8", $dados["temdescricao"]);
	    
	    echo simec_json_encode($dados); die;
	}

	public function excluirDados( $dados ){
	    $sql = "
	        DELETE FROM {$this->stNomeTabela} WHERE temid = {$dados['pk']} RETURNING temid;
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
	    extract($dados);
	    
	    if($temid == ''){
	        $sql = "
	            INSERT INTO {$this->stNomeTabela}(
	                temdescricao,
	                temstatus
	            )VALUES(
	                '{$temdescricao}',
	                '{$temstatus}'
	            )RETURNING temid;
	         ";
	    }else{
	        $sql = "
	            UPDATE {$this->stNomeTabela}
	                    SET temdescricao  = '{$temdescricao}',
	                    temstatus = '{$temstatus}'
	            WHERE temid = {$temid} RETURNING temid;
	        ";
	    }
	    $temid = $this->pegaUm($sql);
	    
	    if($temid > 0){
	        $this->commit();
	        $this->sucesso('', '');
	    }
	}
}