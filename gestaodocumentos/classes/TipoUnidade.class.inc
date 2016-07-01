<?php

class TipoUnidade extends Modelo{

	/**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela 		= "gestaodocumentos.tipounidade";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria 		= array( "tunid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     		= array(
									  	'tunid' => null,
									  	'tundescricao' => null,
									  	'tunprofundidade' => null
									  );

    protected $arObrigatorios		= array(
    									'tundescricao',
    									'tunprofundidade'
    								  );

    protected $arLabels				= array(
    									'Descrição',
    									'Profundidade'
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

    public function listaTipoUnidade(){
    	if( $_REQUEST['tipo_fil'] == 'fil' ){
            echo "<script>abrirPesquisa('mais');</script>";
            if ($_REQUEST['tundescricao_pes']) {
                $tundescricao = removeAcentos(trim($_REQUEST['tundescricao_pes']));
                $where = " AND public.removeacento(td.tundescricao) ilike ('%{$tundescricao}%') ";
            }
            if($_REQUEST['tunprofundidade_pes']){
                $tunprofundidade = removeAcentos(trim($_REQUEST['tunprofundidade_pes']));
                $where = " AND td.tunprofundidade = '{$tunprofundidade}' ";
            }
        }else{
            $where = "";
        }

        $acao = "
            <center>
                <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscaDados('||td.tunid||');\" title=\"Editar Solicitante\" >
                <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirDados('||td.tunid||');\" title=\"Excluir Solicitante\" >
            </center>        
        ";

        $sql = "
            SELECT  '{$acao}' as acao,
                    td.tundescricao,
                    CASE
                        WHEN td.tunprofundidade = '1' THEN 'Diretoria'
                        WHEN td.tunprofundidade = '2' THEN 'Superintendência'
                        WHEN td.tunprofundidade = '3' THEN 'Secretaria'
                    END as tunprofundidade
            FROM gestaodocumentos.tipounidade AS td
            WHERE 1=1 {$where} 
            ORDER BY td.tundescricao
        ";
        $cabecalho = array("A&ccedil;&atilde;o", "Descri&ccedil;&atilde;o", "Profundidade");
        $alinhamento = Array('center', 'left','left');
        $tamanho = Array('3%', '30%','40%');
        $this->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
    }

    public function buscaDados($dados){
        $tunid = $dados['pk'];

        $sql = "
            SELECT * FROM gestaodocumentos.tipounidade WHERE tunid = {$tunid};
        ";
        $dados = $this->pegaLinha($sql);
        
        $dados["tundescricao"] = iconv("ISO-8859-1", "UTF-8", $dados["tundescricao"]);
        
        echo simec_json_encode($dados);
        die;

    }

    public function excluirDados( $dados ){
        $sql = "
            DELETE FROM gestaodocumentos.tipounidade WHERE tunid = {$dados['pk']} RETURNING tunid;
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
        
        if($tunid == ''){
            $sql = "
                INSERT INTO gestaodocumentos.tipounidade(
                    tundescricao,
                    tunprofundidade
                )VALUES(
                    '{$tundescricao}',
                    '{$tunprofundidade}'
                )RETURNING tunid;
             ";
        }else{
            $sql = "
                UPDATE gestaodocumentos.tipounidade
                        SET tundescricao  = '{$tundescricao}',
                        tunprofundidade = '{$tunprofundidade}'
                WHERE tunid = {$tunid} RETURNING tunid;
            ";
        }
        $tunid = $this->pegaUm($sql);
        
        if($tunid > 0){
            $this->commit();
            $this->sucesso('', '');
        }
    }
	
}