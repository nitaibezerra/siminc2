<?php 

interface ProcessoConjurInterface{
	
	/**
	 * Constroi Relatorio
	 */
	public function constroiRelatorio( $relatorio, $dados );
}

/**
 * Construtor de Relat�rios
 * 
 */
class RelatorioFactory{
	
	public $relatorio;

	public function factoryMethod( $objetoRelatorio, Array $dados ){
		
		switch ($objetoRelatorio) {
			case 'RelatorioConjurManifestacoes':
				if( $dados['relT'] ) $contexto = 'todos';
				if( $dados['relA'] ) $contexto = 'advogado';
				if( $dados['relC'] ) $contexto = 'coordenacao';
				if( $dados['relM'] ) $contexto = 'mec';

				$dados = array(
					'advogados' 	=> $dados['advid'],
					'coordenacoes' 	=> $dados['coonid'],
					'periodos'		=> array('inicio'=>$dados['htddatainicial'], 'fim'=>$dados['htddatfinal']),
					'contexto'		=> $contexto );
				break;
			case 'RelatorioConjurMediasProcessos':

				// ver($dados,d);
				$dados = array(
					'agrupamentoCoordenacao' => $dados['agrupamentoCoordenacao'],
					'agrupamentoAdvogados' => $dados['agrupamentoAdvogados'],
					'advogados' 	=> $dados['advid'],
					'coordenacoes' 	=> $dados['coonid'],
					'periodos'		=> array('inicio'=>$dados['htddata']['inicio'], 'fim'=>$dados['htddata']['fim']) );
				break;
		}

		$this->relatorio = new $objetoRelatorio( $dados );

	}

}

/**
 * Classe ProcessoConjur
 *
 * @internal objeto carregando ResultSet com responsabilidade de montar um RowSet também. 
 *			 Sugestão: dividir em abstrações com maior granularidade em relação aos objetos manuseados (resultset || rowset)
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo de ProcessoConjur
 */
class ProcessoConjur extends Modelo implements ProcessoConjurInterface{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "conjur.processoconjur";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('prcid');//,'muncod','stacod','docid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'prcid' => null,
		'tasid' => null,
		'cooid' => null,
		'tprid' => null,
		'usucpf' => null,
		'tspid' => null,
		'prcnumsidoc' => null,
		'prcdtentrada' => null,
		'prcdesc' => null,
		'prcstatus' => null,
		'prcdtinclusao' => null,
		'unicod' => null,
		'unitpocod' => null,
		'advid' => null,
		'prcnomeinteressado' => null,
		'tipid' => null,
		'proid' => null,
		'prcnumeroprocjudicial' => null,
		'prcnumeroprocjudantigo' => null,
		'prcprioritario' => null,
		'tacid' => null,
		'prctiposidemec' => null
	);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
		'prcid',
		'tasid',
		'cooid',
		'tprid',
		'usucpf',
		'tspid',
		'prcnumsidoc',
		'prcdtentrada',
		'prcdesc',
		'prcstatus',
		'prcdtinclusao',
		'unicod',
		'unitpocod',
		'advid',
		'prcnomeinteressado',
		'tipid',
		'proid',
		'prcnumeroprocjudicial',
		'prcnumeroprocjudantigo',
		'prcprioritario',
		'tacid',
		'prctiposidemec'
	);

	/**
	 * Filtros usados no carregamento das listas
	 *
	 * @internal workflow.acaoestadodocumento.aedid
	 *			 workflow.historicodocumento.htddata
	 *			 conjur.processoconjur.cooid
	 */
	protected $filtrosProcessos = array(
		'aedid' => null,
		'htddata' => null,
		'cooid' => null
	);

	public $lista = array();

	/**
	 * Atributos da Tabela obrigatórios
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		'usucpf',
		'prcstatus',
		'prcdtinclusao'
	);

	/**
	 * Constantes que contem as acoes necessárias para especificar documento em estado de Entrada
	 *
	 * @internal workflow.documento.aedid
	 */
	protected $acaoEstadoDocEntrada = array( 
		WF_AEDID_ENCAMINHAR_ANALISE_GAB,
        WF_AEDID_ENCAMINHAR_CGAA,
        WF_AEDID_ENCAMINHAR_CGAC,
        WF_AEDID_ENCAMINHAR_CGAE 
    );

	/**
	 * Constantes que contem as acoes necessárias para especificar documento em estado de Saída
	 *
	 * @internal workflow.documento.aedid
	 */
    protected $acaoEstadoDocSaida = array( WF_AEDID_ENCERRAR_PROCESSO );

    /**
	 * Constantes que contem as acoes necessárias para especificar documento em estado de Arquivado
	 *
	 * @internal workflow.documento.esdid
	 */
    protected $estadoDocumentoArquivado = array( WF_ESDID_ARQUIVADO );

	/**
	 *
	 */
	public $tpdid = TIPODOC;


	/**
	 * Constroi relatorio
	 *
	 * @param string $relatorio
	 * @param array $dados - array com o seguinte formato:
	 * 						 array(
	 *						 	'advogados' 	=> array(),
	 *							'coordenacoes' 	=> array(),
	 *							'periodos'		=> array('inicio'=>'', 'fim'=> ''),
	 *							'contexto'		=> array()
	 * 						 )
	 * 
	 */
	public function constroiRelatorio( $relatorio, $dados ){
		if( empty($relatorio) ) return;
		if( empty($dados) ) return;
		
		$Construtor = new RelatorioFactory();
		$objetoRelatorio = $Construtor->factoryMethod( $relatorio, $dados );

		echo $objetoRelatorio->relatorio;

	}


	/**
	 * Aplica respons�vel pelos avisos do respectivo processo na tabela 'conjur.respostaexterna'
	 *
	 * @param Integer $prcid
	 * @param Array $responsavel
	 */
	public function aplicaResponsavelPorInformativos( $prcid, $responsavel ){

		if( empty($prcid) || empty($responsavel) )
			return;

		foreach ($responsavel as $key => $value) {
			$sql = " INSERT INTO conjur.respostaexterna 
							( prcid, usucpf, espid, dt_delegacao )
					 VALUES ( {$prcid}, '{$value}', (SELECT espid FROM conjur.estruturaprocesso WHERE prcid = {$prcid}), NOW() ) ";
			// ver($sql,d);
			$this->executar( $sql );
		}
		return $this->commit();
	}


	/**
	 *
	 */
	// public function busca Responsaveis Sistema
}