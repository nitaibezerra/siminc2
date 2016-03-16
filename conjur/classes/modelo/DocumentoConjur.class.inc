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

	public function  factoryMethod( $objetoRelatorio, Array $dados ){
		
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
	 * @todo registrar campos ao formato: 'prcid' => null
	 */
	protected $arAtributos = array();

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 * @todo registrar campos aqui
	 */
	protected $arCampos = array();

	/**
	 * Filtros usados no carregamento das listas
	 *
	 * @internal workflow.acaoestadodocumento.aedid
	 *			 workflow.historicodocumento.htddata
	 *			 conjur.processoconjur.cooid
	 * @todo registrar filtros ao modelo: 'aedid' => null
	 */
	protected $filtrosProcessos = array();


	public $lista = array();

	/**
	 * Atributos da Tabela obrigatórios
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 * @todo registrar os atributos obrigatorios aqui
	 */
	protected $arAtributosObrigatorios = array();


}