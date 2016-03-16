<?php
	
class SubacaoItensComposicao extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.subacaoitenscomposicao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "icoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'icoid' => null, 
									  	'icoano' => null, 
									  	'icoordem' => null,
    									'icodescricao' => null,
    									'icoquantidade' => null,
									    'icovalor' => null,
									    'icovalortotal' => null,
									    'icostatus' => null,
									    'sbaid' => null,
									    'unddid' => null,
    									'icodetalhe' => null,
									    'usucpf' => null,
									    'dtatualizacao' => null,
    									'picid' => null
									  );
									  
	public function carregaItensComposicaoPorSbaid($sbaid, $anoref)
	{
		$sql = "SELECT 
					unddid || '_' || icoid || '' as codigo,
					icoid, 
				  	icoano, 
				  	icoordem,
		    		icodescricao as descricao,
		    		icodescricao,
		    		icoquantidade,
				    icovalor,
				    icovalortotal,
				    icostatus,
				    sbaid,
				    unddid,
				    und.unddsc,
				    ics.icodetalhe
				FROM {$this->stNomeTabela} ics
				INNER JOIN par.unidademedida und ON und.undid = ics.unddid
				WHERE ics.sbaid = {$sbaid}
				AND icoano = '{$anoref}'
				ORDER BY icodescricao";
		
		return $this->carregar($sql);
	}
	
	public function recuperaDescricaoItem($iditem)
	{
		return $this->pegaUm("select icodescricao from {$this->stNomeTabela} where icoid = {$iditem}");
	}
	
	public function retornaItensComposicaoPorSubacao($sbaid,$icoano)
	{
		$sql = "select 
					picid,
					icoquantidade
				from
					{$this->stNomeTabela}
				where
					sbaid = $sbaid
				and
					icoano = $icoano
				and
					icostatus = 'A'";
		$arrDados = $this->carregar($sql);
		if($arrDados){
			foreach($arrDados as $dado){
				$arrQtde[$dado['picid']] = $dado['icoquantidade'];
			}
			return $arrQtde;
		}else{
			return false;
		}
	}
	
	public function recuperaItemPorSubacao($sbaid,$icoano,$picid)
	{
		$sql = "select 
					icoid
				from
					{$this->stNomeTabela}
				where
					sbaid = $sbaid
				and
					icoano = $icoano
				and
					picid = $picid
				and
					icostatus = 'A'";
		return $this->pegaUm($sql);
	}
	
	function montaLista($sbaid,$icoano = null, $sbacronograma = null)
	{
		$v = "";
		if( $icoano == 'totalizadores' ){
			$totalizador = "sum(icoquantidade) as icoqtd,";
		}elseif( $icoano ){
			if( $sbacronograma == 2 ){
				$v = "sic.";
			}
			$ano = "AND	{$v}icoano = $icoano";
		}

		if( $sbacronograma == 1 ){
			$sql = "SELECT
					'<center><img class=\"middle link\" title=\"Excluir Item\" onclick=\"deletarItem(this, '|| icoid ||', '|| icoano ||' )\"  src=\"../imagens/excluir.gif\" /></center>' as acao,
					icodescricao,
					'<center>' || sum(icoquantidade) || '</center>' as qtd,
					TO_CHAR(icovalor, '000G000G000D99') as valor,
					TO_CHAR(sum((icoquantidade*icovalor)), '000G000G000D99') as total
				FROM
					par.subacaoitenscomposicao
				WHERE 
					icostatus = 'A'
				AND
					sbaid = $sbaid
				{$ano}
				GROUP BY
					icodescricao, icovalor, icoid, icoano
				ORDER BY
					icodescricao";
		} else {
			$sql = "SELECT
					'<center><img class=\"middle link\" title=\"Excluir Item\" onclick=\"deletarItem(this, '|| sic.icoid ||', '|| sic.icoano ||' )\"  src=\"../imagens/excluir.gif\" /></center>' as acao,
					sic.icodescricao,";
			if( $totalizador ){
				$sql .= $totalizador;
			} else {
			$sql .= "'<input size=\"15\" id=\"icoqtd' || sic.icoid || '\"
								type=\"text\"
								class=\"CampoEstilo\"
								onmouseover=\"MouseOver(this);\"
								onfocus=\"MouseClick(this);\"
								onmouseout=\"MouseOut(this);\"
								onblur=\"MouseBlur(this); calcularValorTotal(' || sic.icoid || ');\"
								onfocus=\"this.select()\"
								onclick=\"return popupSelecionarEscolasItemComposicao($sbaid,$icoano,' || sic.icoid || ')\"
								name=\"icoqtd[{$icoano}]['|| sic.icoid ||']\"
								readonly=\"readonly\"
								value=\"' || 
									case 
							    		when substring( trim(to_char(coalesce(sum(seiqtd), 0), '9999999990D99')), position(',' in trim(to_char(coalesce(sum(seiqtd), 0), '9999999990D99'))) +1, 2 ) = '00'
							    			then trim(to_char(coalesce(sum(seiqtd), 0), '999999999'))
							    		else trim(to_char(coalesce(sum(seiqtd), 0), '9999999990D99'))
							    	END
								|| '\" />' as icoqtd,";
			}
			$sql .=	"TO_CHAR(sic.icovalor, '000G000G000D99') as valor,
					TO_CHAR(sum((seiqtd*sic.icovalor)), '000G000G000D99') as total
				FROM
					par.subacaoitenscomposicao sic
				LEFT JOIN
					par.subacaoescolas ses on ses.sbaid = sic.sbaid
				LEFT JOIN
					par.subescolas_subitenscomposicao ssc ON ssc.icoid = sic.icoid AND ssc.sesid = ses.sesid
				WHERE 
					sic.icostatus = 'A'
				AND
					sic.sbaid = $sbaid
				{$ano}
				GROUP BY
					sic.icodescricao, sic.icoid, sic.icoano, sic.icovalor
				ORDER BY
					sic.icodescricao";
				//ver($sql);
		}

		$cabecalho = array("Ação", "Item","Qtde","Valor","Valor Total");
		$soma = array(4 => 'S');

		return $this->monta_lista_simples($sql,$cabecalho,100,5,$soma,"100%","S");
	}
	
	function conta($sbaid,$icoano = null)
	{
		$ano = "AND	icoano = $icoano";
		$sql = "SELECT
					count(icodescricao)
				FROM
					par.subacaoitenscomposicao
				WHERE 
					icostatus = 'A'
				AND
					sbaid = $sbaid
				{$ano}";

		return $this->pegaUm($sql);
	}

	public function deletaPorPtoid($ptoid)
	{
		$sql = "DELETE FROM par.subescolas_subitenscomposicao WHERE icoid IN (SELECT icoid FROM par.subacaoitenscomposicao WHERE sbaid IN ( SELECT sbaid FROM par.subacao WHERE sbastatus = 'A' AND aciid in(select aciid FROM par.acao WHERE acistatus = 'A' AND ptoid = {$ptoid})));";
		
		$sql.= "DELETE FROM par.subacaoitenscomposicao WHERE sbaid IN ( SELECT sbaid FROM par.subacao WHERE sbastatus = 'A' AND aciid in(select aciid FROM par.acao WHERE acistatus = 'A' AND ptoid = {$ptoid}));"; 
		
		$this->executar( $sql );
	}
	
	public function deletaPorSbaid($sbaid, $ano = array())
	{
		$anos = implode( $ano, "," );
		$sql = "DELETE FROM par.subescolas_subitenscomposicao WHERE icoid IN (SELECT icoid FROM par.subacaoitenscomposicao WHERE sbaid = {$sbaid} AND icoano IN ({$anos}));";
		
		$sql.= "DELETE FROM par.subacaoitenscomposicao WHERE sbaid = {$sbaid} AND icoano IN ({$anos});"; 
		
		$this->executar( $sql );
	}
}									  