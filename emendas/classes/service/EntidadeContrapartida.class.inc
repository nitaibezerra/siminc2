<?php
/**
 * Modelo de Entidade Beneficiada Contrapartida
 * @author Wesley Romualdo
 */
class EntidadeContrapartida extends Modelo{
	
	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "emenda.entbeneficiadacontrapartida";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('ebcid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos	= array(
		'ebcid' => null,
  		'enbid' => null,
  		'ebcexercicio' => null,
  		'ebccontrapartida' => null
	);

	/**
     * Gera o filtro de pesquisa para listas as entidades com as suas contrapartidas mínimas
     * @name geraFiltroContrapartida
     * @author Wesley Romualdo
     * @access public
     * @param array $dados - post do formulário
     * @return array
     */
	public function geraFiltroContrapartida( $dados ){
	
		if( $dados["enbcnpj"] ){
	
			$dados["enbcnpj"] = str_replace(".", "", $dados["enbcnpj"]);
			$dados["enbcnpj"] = str_replace("/", "", $dados["enbcnpj"]);
			$dados["enbcnpj"] = str_replace("-", "", $dados["enbcnpj"]);
	
		}
	
		!empty( $dados["enbcnpj"] ) ? $filtro[] = " eb.enbcnpj ilike '{$dados["enbcnpj"]}'" : "";
		!empty( $dados["enbnome"] )	? $filtro[] = " eb.enbnome ilike '%".trim($dados["enbnome"])."%'" : "";
	
	
		if (!empty($dados["contrapartida"])){
	
			$dados["contrapartida"] == 'S' ? $filtro[] = " ec.ebccontrapartida is not null" : $filtro[] = " ec.ebccontrapartida is null";
				
		}
	
		return $filtro;
	
	}

	/**
     * Lista as entidades da emenda, informando suas repectivas contrapartidas mínima
     * @name listaEntidadesContrapartida
     * @author Wesley Romualdo
     * @access public
     * @param array $filtros - post do formulário
     * @return array
     */
	public function listaEntidadesContrapartida( $filtros ){	
		$sql = "SELECT
					eb.enbcnpj as cnpj,
					eb.enbnome as nome,
					CASE WHEN ebccontrapartida is not null
						 THEN
							'<input type=\"text\" style=\"text-align: left;\" name=\"ebccontrapartida[' || eb.enbid || ']\" id=\"ebccontrapartida_' || eb.enbid || '\" onblur=\"MouseBlur(this);verificaTotal(this.value, this); this.value=mascaraglobal(\'###,##\',this.value);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" class=\"normal\" onkeyup=\"this.value=mascaraglobal(\'###,##\',this.value);\" value=\"' || ebccontrapartida || '\" maxlength=\"6\" size=\"8\"/> %'
						 ELSE
						 	'<input type=\"text\" style=\"text-align: left;\" name=\"ebccontrapartida[' || eb.enbid || ']\" id=\"ebccontrapartida_' || eb.enbid || '\" onblur=\"MouseBlur(this);verificaTotal(this.value, this); this.value=mascaraglobal(\'###,##\',this.value);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" class=\"normal\" onkeyup=\"this.value=mascaraglobal(\'###,##\',this.value);\" value=\"\" maxlength=\"6\" size=\"8\"/> %'
					END as valor, eb.enbano
				FROM
					emenda.entidadebeneficiada eb
				LEFT JOIN
					emenda.entbeneficiadacontrapartida ec ON eb.enbid = ec.enbid
					and ec.ebcexercicio = '{$_SESSION['exercicio']}'
				WHERE
					eb.enbano = {$_SESSION['exercicio']}
					" . ( !empty($filtros) ? 'AND' . implode(" AND ", $filtros) : "" );
		
		$cabecalho = array( "CPNJ", "Nome da Entidade", "Contrapartida Mínima", "Ano" );
		$this->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '95%');
	
	
	}
	
	/**
     * Cria a contrapartida mínima de uma emenda
     * @name insereContrapartidaMinima
     * @author Wesley Romualdo
     * @access public
     * @param array $dados - post do formulário
     * @return array
     */
	public function insereContrapartidaMinima( $dados ){
		$dados["ebccontrapartida"] = ( $dados["ebccontrapartida"] ? $dados["ebccontrapartida"] : array() );
		foreach( $dados["ebccontrapartida"] as $chave=>$valor ){
	
			$valor = str_replace( ",", ".", $valor );
			$valor = !empty($valor) ? "'" . $valor . "'" : 'null';
	
			$sql = "SELECT ebcid FROM emenda.entbeneficiadacontrapartida WHERE enbid = {$chave} and ebcexercicio = '{$_SESSION['exercicio']}'";
			$ebcid = $this->pegaUm( $sql );
	
			if ( $ebcid ){
	
				$sql = "UPDATE emenda.entbeneficiadacontrapartida
						SET ebccontrapartida = {$valor}
						WHERE ebcid = {$ebcid}";
				$this->executar( $sql );
	
			}else{
	
				$sql = "INSERT INTO emenda.entbeneficiadacontrapartida(enbid, ebcexercicio, ebccontrapartida)
						VALUES ( '{$chave}', '{$_SESSION['exercicio']}', {$valor})";
				$this->executar( $sql );
	
			}
	
		}
	
		$this->commit();
		$this->sucesso( "principal/entidadebeneficiada/contrapartida/definir" );	
	}
	
	/**
     * Insere contrapartida modalidade de aplicação
     * @name insereContrapartidaModalidade
     * @author Wesley Romualdo
     * @access public
     * @param array $dados - post do formulário
     * @return array
     */
	public function insereContrapartidaModalidade( $dados ){
		
		foreach ($dados['rmccontrapartida'] as $key => $v) {
			$sql = '';
				$contrapartida = str_replace( ',', '.', $v );			
			if( !empty( $contrapartida ) ){
				
				$sql = "SELECT count(mapcod) FROM emenda.responsavelmodcontrapartida WHERE mapcod = '{$key}' and resid = ".$dados['resid']." and rmcano = '{$_SESSION['exercicio']}'";
				$total = $this->pegaUm( $sql );
				if( $total == 0 ){
					
					$sql = "INSERT INTO emenda.responsavelmodcontrapartida( resid, mapcod, rmccontrapartida, rmcano) 
							VALUES (".$dados['resid'].", '{$key}', $contrapartida, '{$_SESSION['exercicio']}')";
					$this->executar( $sql );
				} else {
					$sql = "UPDATE emenda.responsavelmodcontrapartida SET
							  rmccontrapartida = $contrapartida						 
							WHERE mapcod = '{$key}' and resid = ".$dados['resid']." and rmcano = '{$_SESSION['exercicio']}'";
					$this->executar( $sql );
				}
			}
		}
		return $this->commit();	
	}
	
	/**
     * Carrega tipos de entidade
     * @name carregaTipoEntidade
     * @author Wesley Romualdo
     * @access public
     * @param int $resid - id do responsavel
     * @return array
     */
	public function carregaTipoEntidade( $resid = '' ){
		if( !empty($resid) ){
			$col = ' ,rmc.resid, rmc.rmccontrapartida ';
			$filtro = " left join emenda.responsavelmodcontrapartida rmc on rmc.mapcod = ma.mapcod and rmc.rmcano = '{$_SESSION['exercicio']}' and rmc.resid = ".$resid;
		}
		$sql = "SELECT ma.mapcod, ma.mapdsc $col
				FROM public.modalidadeaplicacao ma $filtro
				WHERE ma.mapcod in ('30', '40', '50') ";
		$arDados = $this->carregar($sql);
		$arDados = $arDados ? $arDados : array();
		
		return $arDados;
	}
}
?>