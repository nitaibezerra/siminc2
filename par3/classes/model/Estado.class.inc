<?php

class Par3_Model_Estado extends Modelo
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

	public function montaTabelaEstados($arWhere = array(), $arInner = null, $tipo = null)
	{
		$cabecalho = array("Ação","Sigla","Unidade da Federação");

		$arrPerfil = pegaPerfilGeral();
		$arrPerfil = (is_array($arrPerfil)) ? $arrPerfil : array();

		if (count(array_intersect(unserialize(GRUPO_DE_PERFIL_ESTADUAL), $arrPerfil)) > 0 || count(array_intersect(unserialize(GRUPO_DE_PERFIL_MUNICIPAL), $arrPerfil)) > 0)
		{
			$acoes = "'<center><a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abreExecucaoOrcamento(\'estado\',\''|| e.estuf ||'\',\'\',\''|| CASE WHEN iu.inuid IS NULL THEN 0 ELSE iu.inuid END 	||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a></center>'";
		}
		else
		{
			$acoes = "'<center><a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abrePlanoTrabalho(\'estado\',\''|| e.estuf ||'\',\'\',\''|| CASE WHEN iu.inuid IS NULL THEN 0 ELSE iu.inuid END 	||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a></center>'";
		}
		$cache = '3600';


		$sql = "SELECT DISTINCT
					$acoes as acao,
		            e.estuf,
		            e.estdescricao
				FROM territorios.estado AS e
					LEFT JOIN par.instrumentounidade iu ON iu.estuf = e.estuf AND iu.itrid IN ( 1, 3 )
					" . ( is_array($arWhere) && count($arWhere) ? ' where ' . implode(' AND ', $arWhere) : '' ) ."
				ORDER BY e.estuf ASC
				";

		$listagem = new Simec_Listagem();
		$listagem->setCabecalho($cabecalho);
		$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS, "*");
		$listagem->setQuery($sql);
		$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
	}

	public function descricaoEstado($estuf)
	{
		return $this->pegaUm("SELECT estdescricao FROM territorios.estado where estuf = '$estuf' ");
	}

    public function carregarUf($booSQL)
    {
    	$sql = "SELECT
        			estuf AS codigo,
                    estdescricao AS descricao
				FROM
                	{$this->stNomeTabela}
				ORDER BY
                estuf;";

		return ($booSQL) ? $sql : $this->carregar($sql);
	}
}