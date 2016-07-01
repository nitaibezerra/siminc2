<?php
	
class Cronograma_PadraoMi extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.cronogramapadraomi";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "cpmid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
                                        'cpmid' => null, 
                                        'estuf' => null, 
                                        'tmiid' => null, 
                                        'cpmvalor' => null, 
                                        'cpmstatus' => null, 
                                        'tpoid' => null,
                                        );

    /**
     * Retorna o cronograma padrao de acordo com a UF e o tipo
     *
     * @param $uf
     * @param $tpoid
     */
    public function pegaCronogramaPadrao($uf, $tpoid){
        $sql = "SELECT * FROM obras2.cronogramapadraomi WHERE estuf = '{$uf}' AND tpoid = {$tpoid} AND cpmstatus = 'A'";
        return $this->pegaLinha($sql);
    }

    public function listaCombo() {

        $sql = "SELECT 
                cpm.cpmid AS codigo, 
                cpm.estuf || ' - ' || tmi.tminome || ' - ' || cpm.cpmvalor AS descricao 
                FROM obras2.cronogramapadraomi cpm
                JOIN obras2.tecnologiami tmi on tmi.tmiid = cpmid
                WHERE cpm.cpmstatus = 'A'";
        $dados = $this->carregar($sql);
        return ($dados ? $dados : array());
    }
    
    public function listaSql(){
        $sql = "
             SELECT 
             '<center><div style=\"width:60px;\">
					<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/alterar.gif\"
	 					style=\"cursor: pointer\"
	 					onclick=\"javascript: alterar(' || cpm.cpmid || ');\"
	 					title=\"Alterar\">
                    <img
	 					align=\"absmiddle\"
	 					src=\"/imagens/send.png\"
	 					style=\"cursor: pointer;\"
	 					onclick=\"javascript: duplicar(' || cpm.cpmid || ');\"
	 					title=\"Duplicar\">
				    <img
	 					align=\"absmiddle\"
	 					src=\"/imagens/excluir.gif\"
	 					style=\"cursor: pointer; margin-left: 3px;\"
	 					onclick=\"javascript: deletar(' || cpm.cpmid || ');\"
	 					title=\"Deletar\">
				    '
                        
				  '</div></center>' as acao,
             cpm.estuf,
             tp.tpodsc,
             tmi.tminome,
             cpm.cpmvalor
             
             FROM obras2.cronogramapadraomi cpm 
             JOIN obras2.tecnologiami tmi on tmi.tmiid = cpm.tmiid
             JOIN obras2.tipologiaobra tp on tp.tpoid = cpm.tpoid
             WHERE cpm.cpmstatus = 'A'
             ORDER BY cpm.cpmid DESC
            ";
//        $dados = $this->carregar( $sql );
        return $sql;
    }
    
    public function excluir($cpmid)
	{
		$sql = "UPDATE {$this->stNomeTabela} SET cpmstatus = 'I' WHERE cpmid = $cpmid";
		if($this->executar($sql)){
			$this->commit();
		}
	}
    
}