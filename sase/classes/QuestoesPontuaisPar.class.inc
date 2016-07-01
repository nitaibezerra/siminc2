<?php

/**
 * Classe QuestoesPontuaisPar
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo de QuestoesPontuaisPar
 */
class QuestoesPontuaisPar extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sase.questoespontuaispar";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('qptid');//,'muncod','sqpcod','docid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'qptid' => null,
		'estuf' => null,
		'muncod' => null,
		'sqpcod' => null,
		'usucpf' => null,
		'docid' => null,
		'qptstatus' => null
	);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
		'qptid',
		'estuf',
		'muncod',
		'sqpcod',
		'usucpf',
		'docid',
		'qptstatus'	
	);

	/**
	 * Atributos da Tabela obrigatórios
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		'estuf',
		'muncod'
	);

	/**
	 * Atributos Integer da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosInt = array(
		'qptid' => null,
		'sqpcod' => null,
		'docid' => null
	);

	/**
	 * Array de lista de questoespontuaispars
	 * 
	 * @var array
	 */
	public $lista;

	/**
	 * Id do último assessorado inserido
	 * 
	 * @var integer
	 */
	public $inserido;

	/**
	 *
	 */
	public $tpdid = TPDID_SASE_QUESTOESPONTUAIS;

	/**
	 * Busca lista de dados baseado em filtro ou não
	 *
	 * @param array $filtros
	 * @author Sávio Resende
	 */
	public function buscaQuestoespontuaispar( $filtros = false ){
		$sql = $this->listaQuery( $filtros );
		
		$this->lista = $this->carregar( $sql );

		return $this;

	}

	/**
	 * Monta query para uma lista com todos os dados
	 *
	 * @param array $filtros
	 * @author Sávio Resende
	 */
	public function listaQuery( $filtros = false ){
		$sql = "
			SELECT * FROM " . $this->stNomeTabela;
		if( $filtros != false ){
			$sql .= " WHERE ";
			foreach ($filtros as $chave => $valor)
				$sql .= " " . $chave . " = " . ((!in_array($chave, $this->arAtributosInt))?"'".$valor."'":$valor) . " ";
		}

		return $sql;
	}

	/**
	 * Monta query para a função monta_lista da classe_simec
	 *
	 * @param array $filtros
	 * @author Sávio Resende
	 */
	public function montaListaQuery( $filtros = false ){
		$sql = "
			SELECT 
				'<a style=\"cursor:pointer\" onclick=\"editarQuestoesPontuaisPar( ' || a.qptid || ' )\"><img src=\"/imagens/alterar.gif\"/></a>
				<a style=\"cursor:pointer\" onclick=\"excluirQuestoesPontuaisPar( ' || a.qptid || ' )\"><img src=\"/imagens/excluir.gif\"/></a>' as acao,
				e.estdescricao as estado,
				m.mundescricao as municipio,
				s.sqpdsc as situacao,
				ed.esddsc as tramitacao,
				a.usucpf,
				CASE WHEN qptstatus = 'I' THEN 'Inativo' WHEN qptstatus = 'A' THEN 'Ativo' END AS qptstatus
			FROM " . $this->stNomeTabela . " a 
			JOIN territorios.estado e ON e.estuf = a.estuf
			JOIN territorios.municipio m ON m.muncod = a.muncod 
			JOIN sase.situacaoquestoespontuais s ON s.sqpcod = a.sqpcod 
			LEFT JOIN workflow.documento d ON d.docid = a.docid
			LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid ";
		if( $filtros != false ){
			$sql .= " WHERE 1=1 ";
			foreach ($filtros as $chave => $valor)
				$sql .= " AND " . $chave . " = " . ((!in_array($chave, $this->arAtributosInt))?"'".$valor."'":$valor) . " ";
		}
		// ver($sql,d);

		return $sql;
	}

	/**
	 * Insert
	 * 
	 * @param array $dadosQuestoesPontuaisPar
	 * @author Sávio Resende
	 */
	public function insereQuestoesPontuaisPar( Array $dadosQuestoesPontuaisPar ){

		if( !$this->validaDadosPorInserir( $dadosQuestoesPontuaisPar ) ) return false;

		$this->popularObjeto( $this->arCampos, $dadosQuestoesPontuaisPar );
		$this->arAtributos['docid'] = wf_cadastrarDocumento( $this->tpdid, '' );
		$this->arAtributos['qptid'] = $this->inserir();
		$this->inserido = $this->arAtributos['qptid'];
		$retorno = $this->commit();

		if( $retorno )
			$this->verificaExistenciaDocumentoWF( $this->inserido );

		if( !$retorno )
			return false;
		else
			return $this;
	}

	/**
	 * Valida dados por serem inseridos (campos obrigatorios)
	 *
	 * @param array dadosQuestoesPontuaisPar
	 * @author Sávio Resende
	 */
	public function validaDadosPorInserir( Array $dadosQuestoesPontuaisPar ){
		foreach ($this->arAtributosObrigatorios as $chave => $valor)
			if( !isset($dadosQuestoesPontuaisPar[$valor]) || !$dadosQuestoesPontuaisPar[$valor] )
				return false;

		return true;
	}

	/**
	 * Verifica se já existe questoespontuaispar para o estado ou municipio especificado no array
	 *
	 * @param array $dadosQuestoesPontuaisPar
	 * @author Sávio Resende
	 */
	public function verificaDuplicidade( Array $dadosQuestoesPontuaisPar ){
		if( !isset($dadosQuestoesPontuaisPar['estuf']) && !isset($dadosQuestoesPontuaisPar['muncod']) ) return false;

		$sql = "
			SELECT COUNT(*) as total
			FROM " . $this->stNomeTabela . " 
			WHERE 
				estuf = '" . $dadosQuestoesPontuaisPar['estuf'] . "' 
				OR muncod = '" . $dadosQuestoesPontuaisPar['muncod'] . "' ";
		$numeroResultados = $this->pegaUm( $sql );
		if( $numeroResultados > 0 ) return false;

		return true;
	}
	
	/**
	 * Exclui logicamente QuestoesPontuaisPar
	 *
	 * @param integer $qptid
	 * @return bool
	 * @author Sávio Resende
	 */
	public function excluirQuestoesPontuaisPar( $qptid ){
		$this->carregarPorId( $qptid );
		$this->arAtributos['qptstatus'] = 'I';
		$this->alterar();
		return $this->commit();
	}

	/**
	 * Exclui Logicamente QuestoesPontuaisPar
	 *
	 * @param integer $qptid
	 * @return bool
	 * @author Sávio Resende
	 */
	public function excluirLogicamenteQuestoesPontuaisPar( $qptid ){
		$this->carregarPorId( $qptid );
		$this->arAtributos['qptstatus'] = 'I';
		$this->alterar();
		return $this->commit();
	}

	/**
	 * Carrega Objeto por ID
	 *
	 * @param Integer $qptid
	 * @author Sávio Resende
	 */
	public function carregaQuestoesPontuaisPar( $qptid ){
		$this->carregarPorId( $qptid );

		$this->verificaExistenciaDocumentoWF( $qptid );
	}

	/**
	 * Verifica existencia de documento workflow, caso não existe cria
	 *
	 * @param Integer $qptid
	 * @author Sávio Resende
	 * @return str|bool retorna string '1' para wf já existente ou bool para resultado de atualziacao para o wf recém criado.
	 */
	public function verificaExistenciaDocumentoWF( $qptid ){

		if( $this->docid == null ){
			$this->arAtributos['docid'] = wf_cadastrarDocumento( $this->tpdid, '' );
			$this->alterar();
			return $this->commit();
		}

		return '1';

	}

	/**
	 * Popula Objeto com Array
	 *
	 * @param array $arDados
	 * @return $this
	 * @author Sávio Resende
	 */
	public function populaQuestoesPontuaisPar( Array $arDados ){
		$this->popularObjeto( $this->arCampos, $arDados );
		return $this;
	}

	/**
	 * Atualiza Questoes Pontuais Par
	 *
	 * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizarQuestoesPontuaisPar(){
		if( $this->validaCamposObrigatorios() ){
			$this->alterar();
			return $this->commit();
		}

		return 'invalido';
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
	 * Monta legenda de acordo com os estados selecionados apresentando a quantidade de municipios envolvidos com a situação
	 *
	 * @author Sávio Resende
	 * @param array $estuf
	 * @return html
	 */
	public function montaLegenda( Array $estuf ){
		global $db;
		?>
		<div id="legendaMapaContainer">
		<div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
		<ul>
			<?php
			$sql = "
				select 
					s.sqpcod,
					s.sqpdsc,
					s.sqpcor,
					count(qptid) as total
				from sase.situacaoquestoespontuais  s
				left join sase.questoespontuaispar q on q.sqpcod = s.sqpcod
				group by 1,2,3
				order by sqpcod ASC ";
			$lista = $db->carregar( $sql );
			foreach ($lista as $key => $value) {

				$sql = "
					select count(a.qptid) as total
					from sase.situacaoquestoespontuais s
					join sase.questoespontuaispar a on a.sqpcod = s.sqpcod
					where 
						" . (($estuf!=''&&count($estuf)>0)?"a.estuf in ( '". (implode( "','", $estuf )) ."' ) and ":"") . " 
						s.sqpcod = " . $value['sqpcod'] . " ";
				$total = $db->pegaUm( $sql );

				echo "<li ><span style='background:" . $value['sqpcor'] . ";' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;" . $value['sqpdsc'] . " - <b>" . (($total!='')?$total:'0') . "</b></li>";
			}
			?>
		</ul>
		</div>
		<?php
	}
}