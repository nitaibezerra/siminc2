<?php

class TipoModalidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela 		= "gestaodocumentos.tipomodalidade";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria 		= array( "tmdid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     		= array(
									  	'tmdid' => null,
									  	'tmddescricao' => null
									  );

    protected $arObrigatorios		= array(
    									'tmddescricao'
    								  );

    protected $arLabels				= array(
    									'Descrição'
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

    public function listaTipoModalidade(){
    	if( $_REQUEST['tipo_fil'] == 'fil' ){
        	echo "<script>abrirPesquisa('mais');</script>";
            if ($_REQUEST['tmddescricao_pes']) {
                $tmddescricao = removeAcentos(trim($_REQUEST['tmddescricao_pes']));
                $where = " AND public.removeacento(tm.tmddescricao) ilike ('%{$tmddescricao}%') ";
            }
        }else{
            $where = "";
        }

        $acao = "
            <center>
                <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscaDados('||tm.tmdid||');\" title=\"Editar Solicitante\" >
                <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirDados('||tm.tmdid||');\" title=\"Excluir Solicitante\" >
            </center>        
        ";

        $sql = "
            SELECT  '{$acao}' as acao,
                    tm.tmddescricao
            FROM gestaodocumentos.tipomodalidade AS tm
            WHERE 1=1 {$where} 
            ORDER BY tm.tmddescricao
        ";
        $cabecalho = array("A&ccedil;&atilde;o", "Descri&ccedil;&atilde;o");  
        $alinhamento = Array('center', 'left');
        $tamanho = Array('3%', '70%');
        $this->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
    }

	public function buscaDados($dados){
        $tmdid = $dados['pk'];

        $sql = "
            SELECT * FROM gestaodocumentos.tipomodalidade WHERE tmdid = {$tmdid};
        ";
        $dados = $this->pegaLinha($sql);
        
        $dados["tmddescricao"] = iconv("ISO-8859-1", "UTF-8", $dados["tmddescricao"]);
        
        echo simec_json_encode($dados);
        die;
    
    }
    
    public function excluirDados( $dados ){
        $sql = "
            DELETE FROM gestaodocumentos.tipomodalidade WHERE tmdid = {$dados['pk']} RETURNING tmdid;
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
        
        if($tmdid == ''){
            $sql = "
                INSERT INTO gestaodocumentos.tipomodalidade(
                    tmddescricao
                )VALUES(
                    '{$tmddescricao}'
                )RETURNING tmdid;
             ";
        }else{
            $sql = "
                UPDATE gestaodocumentos.tipomodalidade
                        SET tmddescricao  = '{$tmddescricao}'
                WHERE tmdid = {$tmdid} RETURNING tmdid;
            ";
        }
        $tmdid = $this->pegaUm($sql);
        
        if($tmdid > 0){
            $this->commit();
            $this->sucesso('', '');
        }
    }
}