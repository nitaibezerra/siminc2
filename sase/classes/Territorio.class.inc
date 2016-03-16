<?php

/**
 * Classe Territorio
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo de Territorio
 */
class Territorio extends Modelo
{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sase.territorio";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('terid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'terid' => null,
		'mesid' => null,
		'muncod' => null,
		'usucpf' => null,
		'terstatus' => null
	);

	/**
	 * Atributos String da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosStr = array();

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
		'terid',
		'mesid',
		'muncod',
		'usucpf',
		'terstatus'
	);

	/**
	 * Campos Obrigatórios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		'mesid',
		'muncod',
		'terstatus'
	);
	/**
	 * Atributo indireto estado
	 */
	public $estuf = null;

	/**
	 * Monta query para a função monta_lista da classe_simec
	 *
	 * @param array $filtros
	 * @author Sávio Resende
	 * @return $sql
	 */
	public function montaListaQuery( $filtros = false ){

//        '<a style=\"cursor:pointer\" onclick=\"editarTerritorio( ' || t.terid || ' )\"><img src=\"/imagens/alterar.gif\"/></a>
//				<a style=\"cursor:pointer\" onclick=\"excluirTerritorio( ' || t.terid || ' )\"><img src=\"/imagens/excluir.gif\"/></a>' as acao,


		$sql = "
			SELECT 
                '<a style=\"cursor:pointer\" onclick=\"editarTerritorio( ' || t.terid || ' )\"><img src=\"/imagens/alterar.gif\"/></a>' as acao,
				mes.mesdsc,
				e.estuf,
				m.mundescricao,
				CASE WHEN terstatus = 'I' THEN 'Inativo' WHEN terstatus = 'A' THEN 'Ativo' END AS terstatus
			FROM " . $this->stNomeTabela . " t
			JOIN territorios.municipio m ON m.muncod = t.muncod
			JOIN territorios.estado e ON e.estuf = m.estuf
			JOIN sase.mesoregiao mes ON mes.mesid = t.mesid ";
		if( $filtros != false ){
			$sql .= " WHERE 1=1 ";
			foreach ($filtros as $chave => $valor)
				$sql .= " AND " . $chave . ((in_array($chave, $this->arAtributosStr))?" LIKE '%".$valor."%'":" = '".$valor."'") . " ";
		}

		return $sql;
	}

	/**
	 * Atualiza Territorio
	 *
	 * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizarTerritorio(){
		if( $this->validaCamposObrigatorios() ){
			$this->alterar();
			return $this->commit();
		}

		return 'invalido';
	}

	/**
	 * Exclui logicamente o Territorio
 	 * 
	 * @param integer $stacod
	 * @return bool
	 * @author Sávio Resende
	 */
	public function excluirTerritorio( $terid ){
		$this->carregarPorId( $terid );
		$this->arAtributos['terstatus'] = 'I';
		$this->alterar();
		return $this->commit();
	}

	/**
	 * Popula Objeto com Array
	 *
	 * @param array $arDados
	 * @return $this
	 * @author Sávio Resende
	 */
	public function populaTerritorio( Array $arDados ){
		$this->popularObjeto( $this->arCampos, $arDados );
		return $this;
	}

	/**
	 * Valida campos obrigatorios no objeto populado
	 *
	 * @author Sávio Resende
	 * @return bool
	 */
	public function validaCamposObrigatorios(){
		foreach ($this->arAtributosObrigatorios as $chave => $valor)
			if( !isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]) )
				return false;

		return true;
	}

	/**
	 * Cadastra Territorio Montado no Objeto
	 *
	 * @author Sávio Resende
	 * @return bool|string - retorna string 'invalido' para quando tiver campos obrigatorios vazios
	 */
	public function cadastrarTerritorio(){
		if( $this->validaCamposObrigatorios() ){
			$this->terid = $this->inserir();
			return $this->commit();
		}

		return 'invalido';
	}

	/**
	 * Monta objeto Território
	 *
	 * @param $terid
	 * @author Sávio Resende
	 */
	public function carregarTerritorio( $terid ){

		$this->carregarPorId( $terid );
		if( $this->muncod ){
			$sql = " select estuf from territorios.municipio where muncod = '" . $this->muncod . "' ";
			$this->estuf = $this->pegaUm( $sql );
		}

	}

	/**
	 * Monta legenda de acordo com os estados selecionados apresentando a quantidade de municipios envolvidos com a situação
	 *
	 * @author Sávio Resende
	 * @param array $estuf
	 * @return html
	 */
	public function montaLegenda( Array $estuf, $orgid ){
		global $db;
		?>
		<div id="legendaMapaContainer">
		<div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
<!--		<ul>-->
			<?php
			$sql = "
				select
					m.mesdsc as mesdsc, 
					m.mescor as mescor, 
					mun.estuf as estuf, 
					o.orgdsc as orgdsc, 
					oes.orgid as orgid,
					m.mesid as mesid
				from sase.mesoregiao m
				left join sase.orgaoestado oes on oes.oesid = m.oesid
				left join sase.orgao o on o.orgid = oes.orgid
				left join sase.territorio t on t.mesid = m.mesid
				join territorios.municipio mun on mun.muncod = t.muncod
				where messtatus = 'A' and o.orgid = '{$orgid}'
				and oes.estuf in ( '" . (implode("','", $estuf)) . "' )
				group by 1,2,3,4,5,6
				order by o.orgdsc ASC
			";

			$lista = $db->carregar( $sql );
            if (is_array($lista)) {
                echo "<ul>";
                $est = "";
                foreach ($lista as $key => $value) {
                    $sql = "
					select count(t.terid) as total
					from sase.territorio t
					join sase.mesoregiao mes on mes.mesid = t.mesid
					join territorios.municipio m on m.muncod = t.muncod
					where 
						mes.mesid = {$value['mesid']} 
						" . (($estuf != '' && count($estuf) > 0) ? " and m.estuf in ( '" . (implode("','", $estuf)) . "' ) " : "");
                    $total = $db->pegaUm($sql);

                    echo "<li><table><tr><td><span style='background:" . $value['mescor'] . ";' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>" . (int)$total . "</b>&nbsp;&nbsp;</td><td>" . $value['mesdsc'] . "</td></tr></table></li>";

                }
                echo "</ul>";
            }
			?>
<!--		</ul>-->
		</div>
		<?php
	}

}
