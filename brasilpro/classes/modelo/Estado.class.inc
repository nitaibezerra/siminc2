<?php
	
class Estado extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "territorios.estado";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "estuf" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'estuf' => null, 
									  	'muncodcapital' => null, 
									  	'regcod' => null, 
									  	'estcod' => null, 
									  	'estdescricao' => null, 
									  );

	public function lista($arWhere = array(), $arInner = array())
	{
		$sql = "SELECT DISTINCT
					'<center>
						<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abrePlanoTrabalho(\'estado\',\''|| e.estuf ||'\',\'\',\''|| CASE WHEN iu.inuid IS NULL THEN 0 ELSE iu.inuid END 	||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
					</center>' as acao, 
		            e.estuf,
		            e.estdescricao, 
		            CASE WHEN ed.esddsc IS NULL THEN 'Diagnóstico' ELSE ed.esddsc END as sit
		            --iu.inuid
				FROM territorios.estado AS e   
					LEFT JOIN par.instrumentounidade iu ON iu.estuf = e.estuf
					LEFT JOIN workflow.documento d on d.docid = iu.docid AND d.tpdid = 44 
					LEFT JOIN workflow.estadodocumento ed on ed.esdid = d.esdid
					" . $arInner['join_dadosunidade'] . "
					" . ( is_array($arWhere) && count($arWhere) ? ' where ' . implode(' AND ', $arWhere) : '' ) ."
				ORDER BY e.estuf ASC
				";
		//ver($sql);
		$cabecalho = array("Ação","Sigla","Unidade da Federação", "Situação");
		$tamanho   = array( '5%', '10%', '50%', '10%' );															
		return $this->monta_lista($sql,$cabecalho,50,5,'N','95%',$par2,'',$tamanho);
	}
	
	public function descricaoEstado($estuf)
	{
		return $this->pegaUm("SELECT estdescricao FROM territorios.estado where estuf = '$estuf' ");
	}
	
}