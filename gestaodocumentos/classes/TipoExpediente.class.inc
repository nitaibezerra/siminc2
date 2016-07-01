<?php

class TipoExpediente extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela 		= "gestaodocumentos.tipoexpediente";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria 		= array( "tpeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     		= array(
									  	'tpeid' => null,
									  	'tpedsc' => null
									  );

    protected $arObrigatorios		= array(
    									'tpedsc'
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

    public function listaTipoExpediente(){
    	if( $_REQUEST['tipo_fil'] == 'fil' ){
            echo "<script>abrirPesquisa('mais');</script>";
            if ($_REQUEST['tpedsc_pes']) {
                $tpedsc = removeAcentos(trim($_REQUEST['tpedsc_pes']));
                $where = " AND public.removeacento(td.tpedsc) ilike ('%{$tpedsc}%') ";
            }
        }else{
            $where = "";
        }

        $acao = "
            <center>
                <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscaDados('||td.tpeid||');\" title=\"Editar Solicitante\" >
                <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirDados('||td.tpeid||');\" title=\"Excluir Solicitante\" >
            </center>        
        ";

        $sql = "
            SELECT  '{$acao}' as acao,
                    td.tpedsc
            FROM gestaodocumentos.tipoexpediente AS td
            WHERE 1=1 {$where} 
            ORDER BY td.tpedsc
        ";
        $cabecalho = array("A&ccedil;&atilde;o", "Descri&ccedil;&atilde;o");  
        $alinhamento = Array('center', 'left');
        $tamanho = Array('3%', '70%');
        // ver(simec_htmlentities($sql),d);
        $this->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
    }

	public function buscaDados($dados){
        $tpeid = $dados['pk'];

        $sql = "
            SELECT * FROM gestaodocumentos.tipoexpediente WHERE tpeid = {$tpeid};
        ";
        $dados = $this->pegaLinha($sql);
        
        $dados["tpedsc"] = iconv("ISO-8859-1", "UTF-8", $dados["tpedsc"]);
        
        echo simec_json_encode($dados);
        die;

    }

    public function excluirDados( $dados ){
        $sql = "
            DELETE FROM gestaodocumentos.tipoexpediente WHERE tpeid = {$dados['pk']} RETURNING tpeid;
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
        
        if($tpeid == ''){
            $sql = "
                INSERT INTO gestaodocumentos.tipoexpediente(
                    tpedsc
                )VALUES(
                    '{$tpedsc}'
                )RETURNING tpeid;
             ";
        }else{
            $sql = "
                UPDATE gestaodocumentos.tipoexpediente
                        SET tpedsc  = '{$tpedsc}'
                WHERE tpeid = {$tpeid} RETURNING tpeid;
            ";
        }
        $tpeid = $this->pegaUm($sql);
        
        if($tpeid > 0){
            $this->commit();
            $this->sucesso('', '');
        }
    }
}