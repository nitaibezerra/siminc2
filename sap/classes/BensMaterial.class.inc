<?php

/**
 * Modelo de Bens Material
 * @author Silas Matheus
 */
class BensMaterial extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sap.bensmaterial";

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('bmtid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos	= array(
		'bmtid' => null,
		'benid' => null,
		'matid' => null,
		'bmtdscit' => null,
		'bmtitmat' => null,
		'bmtgarantia' => null,
		'bmtvlrunitario' => null,
		'bmtjust' => null
	);

	/**
	 * Salva novo registro
	 * @name salvar
	 * @author Silas Matheus
	 * @access public
	 * @param array $campos - Campos para inserir
	 * @return array
	 */
	public function salvar($campos){

		//formata valor
		$campos['bmtvlrunitario'] = addslashes($campos['bmtvlrunitario']);
		$bmtvlrunitario = formata_valor_sql($campos['bmtvlrunitario']);

        //formata a data
        if(!empty($campos['bmtgarantia'])){
          $campos['bmtgarantia'] = addslashes($campos['bmtgarantia']);
          $bmtgarantia = "'" . formata_data_sql($campos['bmtgarantia']) . "'";
        }else{
           $bmtgarantia = 'null';
        }

		$campos['benid'] = addslashes($campos['benid']);
		$campos['matid'] = addslashes($campos['matid']);
		$campos['bmtdscit'] = addslashes($campos['bmtdscit']);
		$campos['bmtitmat'] = addslashes($campos['bmtitmat']);

		$sql = "INSERT INTO $this->stNomeTabela
					(benid, matid, bmtdscit, bmtitmat, bmtvlrunitario, bmtgarantia)
				VALUES
					({$campos['benid']}, {$campos['matid']}, '{$campos['bmtdscit']}', {$campos['bmtitmat']}, '{$bmtvlrunitario}', {$bmtgarantia})
				RETURNING bmtid";

	 	$bmtid = $this->pegaUm($sql);

	 	if(!empty($bmtid)){
	 		$this->commit();
	 		return array(0=>true,1=>$bmtid);
	 	}
	 	else{
	 		return array(0=>false,1=>$bmtid);
	 	}
	}

	/**
	 * Monta grid de materiais dos bens de acordo com o filtro
	 * @name filtrar
	 * @author Silas Matheus
	 * @access public
	 * @param int $benid - Código do Bem
	 * @param string - Filtro da pesquisa
	 * @return void
	 */
	public function filtrar($benid,$pesquisadsc=''){

		$dados = array();
		
		$sql = "SELECT
		
					'<center>
						<img src=\"/imagens/valida6.gif\" border=\"0\" title=\"Não Tombado\" />
					</center>' AS comandos1,
		
					'<center>
								
							 <a style=\"cursor:pointer;\" onclick=\"redirecionaTombamento(\'' || bmt.bmtid || '\')\">
							 <img src=\"/imagens/alterar.gif\" border=\"0\" title=\"Tombar\" />
						 	 </a>
							
							 <a style=\"cursor:pointer;\" onclick=\"redirecionaEditar(\'' || bmt.bmtid || '\')\">
							 <img src=\"/imagens/editar_nome.gif\" border=\"0\" title=\"Alterar\" />
						 	 </a>'
					||

						CASE WHEN
							ben.benstatus = 'I'
						THEN
								'<a style=\"cursor:pointer;\" onclick=\"redirecionaExcluir(\'' || bmt.bmtid || '\');\">
								 <img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
								 </a>
								 </center>'
						ELSE
							''
						END

					AS comandos2,
					ccb.ccbdsc,
					mat.matdsc,
					bmt.bmtitmat||' ' AS bmtitmat,
					bmt.bmtvlrunitario,
					(bmt.bmtvlrunitario * bmt.bmtitmat) valortotal,
					'<span style=\"display:none;\">' || bmt.bmtgarantia || '</span>' || to_char(bmt.bmtgarantia, 'DD/MM/YYYY') bmtgarantia
				FROM
					$this->stNomeTabela bmt
				INNER JOIN
					sap.bens ben
					ON bmt.benid=ben.benid
				INNER JOIN
					sap.material mat
					ON mat.matid=bmt.matid
				INNER JOIN
					sap.catalogo cat
					ON cat.catid=mat.catid
				INNER JOIN
					sap.itemcontacontabil icb
					ON icb.icbid=cat.icbid AND icb.ccbid=cat.ccbid
				INNER JOIN
					sap.contacontabil ccb
					ON icb.ccbid=ccb.ccbid
				WHERE ((SELECT COUNT(*) FROM sap.rgp rgp WHERE bmt.bmtid=rgp.bmtid AND bmt.benid='{$benid}') < (SELECT SUM(bmt2.bmtitmat) FROM sap.bensmaterial bmt2 WHERE bmt2.bmtid=bmt.bmtid AND bmt2.benid='{$benid}')) ";

		if($benid){
			$benid = addslashes($benid);
			$sql .= "AND ben.benid='{$benid}' ";
		}
		if(!empty($pesquisadsc)){
			$sql .= "AND (ccb.ccbdsc ILIKE '%{$pesquisadsc}%' OR mat.matdsc ILIKE '%{$pesquisadsc}%' OR ";
			$sql .= "CAST(bmt.bmtitmat AS text) ILIKE '%{$pesquisadsc}%' ";
			$sql .= "OR CAST(bmt.bmtvlrunitario AS text) ILIKE '%{$pesquisadsc}%' ";
			$sql .= "OR CAST((bmt.bmtvlrunitario * bmt.bmtitmat) AS text) ILIKE '%{$pesquisadsc}%' ";
			$sql .= "OR CAST(bmtgarantia AS text) ILIKE '%{$pesquisadsc}%' ";
			$sql .= "OR bmt.bmtdscit ILIKE '%{$pesquisadsc}%') ";
		}
		$sql .= "ORDER BY ccb.ccbdsc ";
		
		$resultado1 = $this->carregar($sql);
		if(is_array($resultado1) && count($resultado1) >= 1){
			foreach($resultado1 as $key => $value){
				
				foreach($value as $key2 => $value2){
					if($key2 != 'bmtvlrunitario' && $key2 != 'valortotal'){
						$value[$key2] = '<center>'.$value2.'</center>';
					}
				}
				
				$dados[] = $value;
			}
		}

		
		$sql = "SELECT
						
		
					'<center>
						<img src=\"/imagens/valida1.gif\" border=\"0\" title=\"Tombado\" />
					</center>' AS comandos1,
		
		
					'<center>
								
							 <a style=\"cursor:pointer;\" onclick=\"redirecionaTombamento(\'' || bmt.bmtid || '\')\">
							 <img src=\"/imagens/alterar.gif\" border=\"0\" title=\"Tombar\" />
						 	 </a>
							 
							 <a style=\"cursor:pointer;\" onclick=\"redirecionaEditar(\'' || bmt.bmtid || '\')\">
							 <img src=\"/imagens/editar_nome.gif\" border=\"0\" title=\"Alterar\" />
						 	 </a>'
					||

						CASE WHEN
							ben.benstatus = 'I'
						THEN
								'<a style=\"cursor:pointer;\" onclick=\"redirecionaExcluir(\'' || bmt.bmtid || '\');\">
								 <img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
								 </a>
								 </center>'
						ELSE
							''
						END

					AS comandos2,
					ccb.ccbdsc,
					mat.matdsc,
					bmt.bmtitmat||' ' AS bmtitmat,
					bmt.bmtvlrunitario,
					(bmt.bmtvlrunitario * bmt.bmtitmat) valortotal,
					'<span style=\"display:none;\">' || bmt.bmtgarantia || '</span>' || to_char(bmt.bmtgarantia, 'DD/MM/YYYY') bmtgarantia
				FROM
					$this->stNomeTabela bmt
				INNER JOIN
					sap.bens ben
					ON bmt.benid=ben.benid
				INNER JOIN
					sap.material mat
					ON mat.matid=bmt.matid
				INNER JOIN
					sap.catalogo cat
					ON cat.catid=mat.catid
				INNER JOIN
					sap.itemcontacontabil icb
					ON icb.icbid=cat.icbid AND icb.ccbid=cat.ccbid
				INNER JOIN
					sap.contacontabil ccb
					ON icb.ccbid=ccb.ccbid
				WHERE ((SELECT COUNT(*) FROM sap.rgp rgp WHERE bmt.bmtid=rgp.bmtid AND bmt.benid='{$benid}') = (SELECT SUM(bmt2.bmtitmat) FROM sap.bensmaterial bmt2 WHERE bmt2.bmtid=bmt.bmtid AND bmt2.benid='{$benid}')) ";

		if($benid){
			$benid = addslashes($benid);
			$sql .= "AND ben.benid='{$benid}' ";
		}
		if(!empty($pesquisadsc)){
			$sql .= "AND (ccb.ccbdsc ILIKE '%{$pesquisadsc}%' OR mat.matdsc ILIKE '%{$pesquisadsc}%' OR ";
			$sql .= "CAST(bmt.bmtitmat AS text) ILIKE '%{$pesquisadsc}%' ";
			$sql .= "OR CAST(bmt.bmtvlrunitario AS text) ILIKE '%{$pesquisadsc}%' ";
			$sql .= "OR CAST((bmt.bmtvlrunitario * bmt.bmtitmat) AS text) ILIKE '%{$pesquisadsc}%' ";
			$sql .= "OR CAST(bmtgarantia AS text) ILIKE '%{$pesquisadsc}%' ";
			$sql .= "OR bmt.bmtdscit ILIKE '%{$pesquisadsc}%') ";
		}
		$sql .= "ORDER BY ccb.ccbdsc ";
		
		$resultado2 = $this->carregar($sql);
		if(is_array($resultado2) && count($resultado2) >= 1){
			foreach($resultado2 as $key => $value){
				
				foreach($value as $key2 => $value2){
					if($key2 != 'bmtvlrunitario' && $key2 != 'valortotal'){
						$value[$key2] = '<center>'.$value2.'</center>';
					}
				}
				
				$dados[] = $value;
			}
		}
		
		
		$cabecalho = array("Situação","Comando","Conta Contábil","Material", "Qtd. do Material", "Valor Unitário", "Valor Total", "Dt. de Garantia");
		//$this->monta_lista($sql, $cabecalho, 20, 50, false, "center");
		$this->monta_lista_array($dados, $cabecalho, 20, 10, false, "center");

	}
	
	
	
	
	
	/**
	 * Conta os registros da pesquisa de itens do processo
	 * @name contarRegistrosPesquisaItem
	 * @author Alysson Rafael
	 * @access public
	 * @param int $benid - Código do Bem
	 * @param string $pesquisadsc - Filtro da pesquisa
	 * @return int
	 */
	public function contarRegistrosPesquisaItem($benid,$pesquisadsc=''){

	$dados = array();
		
		$sql = "SELECT
		
					'<center>
						<img src=\"/imagens/valida6.gif\" border=\"0\" title=\"Não Tombado\" />
					</center>' AS comandos1,
		
					'<center>
								
							 <a style=\"cursor:pointer;\" onclick=\"redirecionaTombamento(\'' || bmt.bmtid || '\')\">
							 <img src=\"/imagens/alterar.gif\" border=\"0\" title=\"Tombar\" />
						 	 </a>
							
							 <a style=\"cursor:pointer;\" onclick=\"redirecionaEditar(\'' || bmt.bmtid || '\')\">
							 <img src=\"/imagens/editar_nome.gif\" border=\"0\" title=\"Alterar\" />
						 	 </a>'
					||

						CASE WHEN
							ben.benstatus = 'I'
						THEN
								'<a style=\"cursor:pointer;\" onclick=\"redirecionaExcluir(\'' || bmt.bmtid || '\');\">
								 <img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
								 </a>
								 </center>'
						ELSE
							''
						END

					AS comandos2,
					ccb.ccbdsc,
					mat.matdsc,
					bmt.bmtitmat||' ' AS bmtitmat,
					bmt.bmtvlrunitario,
					(bmt.bmtvlrunitario * bmt.bmtitmat) valortotal,
					'<span style=\"display:none;\">' || bmt.bmtgarantia || '</span>' || to_char(bmt.bmtgarantia, 'DD/MM/YYYY') bmtgarantia
				FROM
					$this->stNomeTabela bmt
				INNER JOIN
					sap.bens ben
					ON bmt.benid=ben.benid
				INNER JOIN
					sap.material mat
					ON mat.matid=bmt.matid
				INNER JOIN
					sap.catalogo cat
					ON cat.catid=mat.catid
				INNER JOIN
					sap.itemcontacontabil icb
					ON icb.icbid=cat.icbid AND icb.ccbid=cat.ccbid
				INNER JOIN
					sap.contacontabil ccb
					ON icb.ccbid=ccb.ccbid
				WHERE ((SELECT COUNT(*) FROM sap.rgp rgp WHERE bmt.bmtid=rgp.bmtid AND bmt.benid='{$benid}') < (SELECT SUM(bmt2.bmtitmat) FROM sap.bensmaterial bmt2 WHERE bmt2.bmtid=bmt.bmtid AND bmt2.benid='{$benid}')) ";

		if($benid){
			$benid = addslashes($benid);
			$sql .= "AND ben.benid='{$benid}' ";
		}
		if(!empty($pesquisadsc)){
			$sql .= "AND (ccb.ccbdsc ILIKE '%{$pesquisadsc}%' OR mat.matdsc ILIKE '%{$pesquisadsc}%' OR ";
			$sql .= "CAST(bmt.bmtitmat AS text) ILIKE '%{$pesquisadsc}%' ";
			$sql .= "OR CAST(bmt.bmtvlrunitario AS text) ILIKE '%{$pesquisadsc}%' ";
			$sql .= "OR CAST((bmt.bmtvlrunitario * bmt.bmtitmat) AS text) ILIKE '%{$pesquisadsc}%' ";
			$sql .= "OR CAST(bmtgarantia AS text) ILIKE '%{$pesquisadsc}%' ";
			$sql .= "OR bmt.bmtdscit ILIKE '%{$pesquisadsc}%') ";
		}
		$sql .= "ORDER BY ccb.ccbdsc ";
		
		$resultado1 = $this->carregar($sql);
		if(is_array($resultado1) && count($resultado1) >= 1){
			foreach($resultado1 as $key => $value){
				$dados[] = $value;
			}
		}

		
		$sql = "SELECT
						
		
					'<center>
						<img src=\"/imagens/valida1.gif\" border=\"0\" title=\"Tombado\" />
					</center>' AS comandos1,
		
		
					'<center>
								
							 <a style=\"cursor:pointer;\" onclick=\"redirecionaTombamento(\'' || bmt.bmtid || '\')\">
							 <img src=\"/imagens/alterar.gif\" border=\"0\" title=\"Tombar\" />
						 	 </a>
							 
							 <a style=\"cursor:pointer;\" onclick=\"redirecionaEditar(\'' || bmt.bmtid || '\')\">
							 <img src=\"/imagens/alterar_01.gif\" border=\"0\" title=\"Alterar\" />
						 	 </a>'
					||

						CASE WHEN
							ben.benstatus = 'I'
						THEN
								'<a style=\"cursor:pointer;\" onclick=\"redirecionaExcluir(\'' || bmt.bmtid || '\');\">
								 <img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
								 </a>
								 </center>'
						ELSE
							''
						END

					AS comandos2,
					ccb.ccbdsc,
					mat.matdsc,
					bmt.bmtitmat||' ' AS bmtitmat,
					bmt.bmtvlrunitario,
					(bmt.bmtvlrunitario * bmt.bmtitmat) valortotal,
					'<span style=\"display:none;\">' || bmt.bmtgarantia || '</span>' || to_char(bmt.bmtgarantia, 'DD/MM/YYYY') bmtgarantia
				FROM
					$this->stNomeTabela bmt
				INNER JOIN
					sap.bens ben
					ON bmt.benid=ben.benid
				INNER JOIN
					sap.material mat
					ON mat.matid=bmt.matid
				INNER JOIN
					sap.catalogo cat
					ON cat.catid=mat.catid
				INNER JOIN
					sap.itemcontacontabil icb
					ON icb.icbid=cat.icbid AND icb.ccbid=cat.ccbid
				INNER JOIN
					sap.contacontabil ccb
					ON icb.ccbid=ccb.ccbid
				WHERE ((SELECT COUNT(*) FROM sap.rgp rgp WHERE bmt.bmtid=rgp.bmtid AND bmt.benid='{$benid}') = (SELECT SUM(bmt2.bmtitmat) FROM sap.bensmaterial bmt2 WHERE bmt2.bmtid=bmt.bmtid AND bmt2.benid='{$benid}')) ";

		if($benid){
			$benid = addslashes($benid);
			$sql .= "AND ben.benid='{$benid}' ";
		}
		if(!empty($pesquisadsc)){
			$sql .= "AND (ccb.ccbdsc ILIKE '%{$pesquisadsc}%' OR mat.matdsc ILIKE '%{$pesquisadsc}%' OR ";
			$sql .= "CAST(bmt.bmtitmat AS text) ILIKE '%{$pesquisadsc}%' ";
			$sql .= "OR CAST(bmt.bmtvlrunitario AS text) ILIKE '%{$pesquisadsc}%' ";
			$sql .= "OR CAST((bmt.bmtvlrunitario * bmt.bmtitmat) AS text) ILIKE '%{$pesquisadsc}%' ";
			$sql .= "OR CAST(bmtgarantia AS text) ILIKE '%{$pesquisadsc}%' ";
			$sql .= "OR bmt.bmtdscit ILIKE '%{$pesquisadsc}%') ";
		}
		$sql .= "ORDER BY ccb.ccbdsc ";
		
		$resultado2 = $this->carregar($sql);
		if(is_array($resultado2) && count($resultado2) >= 1){
			foreach($resultado2 as $key => $value){
				$dados[] = $value;
			}
		}
		
		return count($dados);

	}
	
	
	
	
	
	

	/**
	 * Recupera registro para edicao
	 * @name pegarRegistro
	 * @author Silas Matheus
	 * @access public
	 * @param int $id - Identificador
	 * @return array
	 */
	public function pegarRegistro($id){

		$id = addslashes($id);

		$sql = "SELECT
					*, (bmtvlrunitario * bmtitmat) valortotal, to_char(bmtgarantia, 'DD/MM/YYYY') bmtgarantia
				FROM
					$this->stNomeTabela bmt
				INNER JOIN
					sap.bens ben
					ON ben.benid=bmt.benid
				LEFT JOIN
					sap.empenho emp
					ON emp.empid=ben.empid
				INNER JOIN
					sap.material mat
					ON mat.matid=bmt.matid
				INNER JOIN
					sap.catalogo cat
					ON cat.catid=mat.catid
				INNER JOIN
					sap.itemcontacontabil icb
					ON icb.icbid=cat.icbid AND icb.ccbid=cat.ccbid
				INNER JOIN
					sap.contacontabil ccb
					ON ccb.ccbid=icb.ccbid
				WHERE
					bmt.bmtid=$id";

		$return = $this->carregar($sql);
		return $return[0];

	}

	/**
	 * Atualiza registro
	 * @name atualizar
	 * @author Silas Matheus
	 * @access public
	 * @param array $campos - Campos que serao atualizados
	 * @return int
	 */
	public function atualizar($campos){

		//formata a data
	    if(!empty($campos['bmtgarantia'])){
		  $campos['bmtgarantia'] = addslashes($campos['bmtgarantia']);
		  $bmtgarantia = "'" . formata_data_sql($campos['bmtgarantia']) . "'";
	    }else{
	       $bmtgarantia = 'null';
	    }

		//formata valor
		$campos['bmtvlrunitario'] = addslashes($campos['bmtvlrunitario']);
		$bmtvlrunitario = formata_valor_sql($campos['bmtvlrunitario']);

		$campos['matid'] = addslashes($campos['matid']);
		$campos['bmtdscit'] = addslashes($campos['bmtdscit']);
		$campos['bmtitmat'] = addslashes($campos['bmtitmat']);
		$campos['bmtid'] = addslashes($campos['bmtid']);

		$sql = "UPDATE
					$this->stNomeTabela
				SET
					matid={$campos['matid']},
					bmtdscit='{$campos['bmtdscit']}',
					bmtitmat={$campos['bmtitmat']},
					bmtvlrunitario='{$bmtvlrunitario}',
					bmtgarantia={$bmtgarantia}
				WHERE
					bmtid={$campos['bmtid']}
				RETURNING bmtid";

		$bmtid = $this->pegaUm($sql);
		$this->commit();
		return $bmtid;

	}

	/**
	 * Exclui registro
	 * @name excluir
	 * @author Silas Matheus
	 * @access public
	 * @param int $id - Identificador do Bem Material
	 * @return bool
	 */
	public function excluir($id){

		$id = addslashes($id);

		$sql = "DELETE FROM sap.rgp WHERE bmtid='{$id}' ";
		$this->executar($sql);
		
		$sql = "DELETE FROM $this->stNomeTabela WHERE bmtid='{$id}' ";
		$return = $this->executar($sql);
		$this->commit();
		return $return;

	}

	/**
	 * Valida se a soma total dos bens material é igual o valor do documento
	 * @name validaValorDocumento
	 * @author Silas Matheus
	 * @access public
	 * @param int $benid - Identificador do Bem
	 * @return bool
	 */
	public function validaValorDocumento($benid){

		$benid = addslashes($benid);

		// recuperando valor do documento "bem"
		$oBens = new Bens();
		$valorDocumento = $oBens->pegarValorDocumento($benid);

		// somando o valor total do bens material do bem
		$somaBensMaterial = $this->somaBensMaterial($benid);

		// comparando
		$diferencaValores = new Math($somaBensMaterial, $valorDocumento);
		$resultado = $diferencaValores->isEqual();

		return $resultado;

	}

	/**
	 * Soma o valor total dos bens material de acordo com o bem
	 * @name somaBensMaterial
	 * @author Silas Matheus
	 * @access public
	 * @param int $benid - Identificador do Bem
	 * @return float
	 */
	public function somaBensMaterial($benid){

		$benid = addslashes($benid);

		$sql = "select sum(bmtvlrunitario * bmtitmat) from $this->stNomeTabela where benid=" . $benid;

		return $this->pegaUm($sql);

	}

	/**
	 * Conta quantos bens materiais foram informados por bem
	 * @name quantidadeMaterial
	 * @author Silas Matheus
	 * @access public
	 * @param int $benid - Identificador do Bem
	 * @return int
	 */
	public function quantidadeMaterial($benid){

		$benid = addslashes($benid);

		$sql = "select count(*) from $this->stNomeTabela where benid=" . $benid;

		return $this->pegaUm($sql);

	}

	/**
	 * Conta quantos bens materiais foram informados por bem
	 * @name quantidadeMaterial
	 * @author Silas Matheus
	 * @access public
	 * @param int $benid - Identificador do Bem
	 * @return int
	 */
	public function somaUnidadesMaterial($benid){

		$benid = addslashes($benid);

		$sql = "select sum(benitdoc) from $this->stNomeTabela where benid=" . $benid;

		return $this->pegaUm($sql);

	}

	/**
	 * Valida se a qtd de bens material é igual a quantidade de itens do documento
	 * @name validaTotalUnidade
	 * @author Silas Matheus
	 * @access public
	 * @param int $benid - Identificador do Bem
	 * @return bool
	 */
	public function validaTotalUnidade($benid){

		$benid = addslashes($benid);

		// recuperando a quantidade total de unidades que o documento "bem" pode ter
		$oBens = new Bens();
		$totalUnidades = $oBens->pegarTotalUnidadesDocumento($benid);

		// pega a qunatidade de materiais adicionados
		$somaBensMaterial = $this->quantidadeMaterial($benid);

		// comparando
		$diferencaQuantidade = new Math($totalUnidades, $somaBensMaterial);
		$resultado = $diferencaQuantidade->isEqual();

		return $resultado;

	}

	/**
	 * A quantidade de itens do material não pode ser menor que a quantidade de itens já tombados para ele
	 * @name validaQuantidadeItens
	 * @author Silas Matheus
	 * @access public
	 * @param int $bmtid - Id do Bem
	 * @param int $bmtitmat - Quantidade de itens que esta entrando
	 * @return bool
	 */
	public function validaQuantidadeItens($bmtid, $bmtitmat){

		$resultado = true;

		$oRgp = new Rgp();

		// verificando as quantidades
		if($bmtitmat < $oRgp->pegarQuantidadeRgp($bmtid))
			$resultado = false;

		return $resultado;

	}

	/**
	 * Verifica se registro já existe
	 * @name existe
	 * @author Silas Matheus
	 * @access public
	 * @param array $campos - Campos para verificar
	 * @return bool
	 */
	public function existe($campos){

		$campos['benid'] = addslashes($campos['benid']);
		$campos['matid'] = addslashes($campos['matid']);
		$campos['bmtdscit'] = addslashes($campos['bmtdscit']);


		$sql = "SELECT
					count(*) count
				FROM
					$this->stNomeTabela
				WHERE
					benid='{$campos['benid']}'
				AND
					matid='{$campos['matid']}'
				AND
					bmtdscit='{$campos['bmtdscit']}'";

		if(!empty($campos['bmtid'])){
			$campos['bmtid'] = addslashes($campos['bmtid']);
			$sql .= "AND bmtid<>{$campos['bmtid']}";
		}

		$result = $this->carregar($sql);
 	   	$qtd = $result[0]['count'];

 	   	if($qtd > 0)
 	   		$resultado = true;

		return $resultado;

	}



	public function pegaValorDoItem($bmtid,$benid){
		$sql = "SELECT SUM(bmtvlrunitario * bmtitmat) FROM $this->stNomeTabela WHERE bmtid='{$bmtid}' AND benid='{$benid}' ";
		return $this->pegaUm($sql);
	}

	public function pegaQtdDoItem($bmtid,$benid){
		$sql = "SELECT SUM(bmtitmat) FROM $this->stNomeTabela WHERE bmtid='{$bmtid}' AND benid='{$benid}' ";
		return $this->pegaUm($sql);
	}

	public function pegaQtdMaterial($benid){

		$benid = addslashes($benid);

		$sql = "SELECT SUM(bmtitmat) FROM $this->stNomeTabela WHERE benid='{$benid}' ";
		
		return $this->pegaUm($sql);

	}
	
	
	public function contarItensProcesso($benid){
		//
		$benid = addslashes($benid);
		
		$sql = "SELECT COUNT(*) AS tot FROM $this->stNomeTabela bmt WHERE bmt.benid='{$benid}' ";
		$tot = $this->pegaUm($sql);
		return $tot;
		
	}



}