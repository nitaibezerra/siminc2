<?php

Interface DocumentoParInterface{
	
	/**
	 * @return void
	 */
	public function parsearDadosParaBanco();

	/**
	 * @return void
	 */
	public function registraDocumentoPar();

	/**
	 * @return bool
	 */
	public function registraHistoricoDocumentoPar();
}

/**
 * Abstract Factory DocumentoPar
 *
 * Classe tem o objetivo de ciar família de objetos Trecho sem que os mesmos
 * sejam apontados diretamente na classe DocumentoPar
 */
class AbstractFactory_DocumentoPar{

	protected $TrechoDocumentoParCreator;

	/**
	 * @internal Prepara instância da classes que compõem o DocumentoPar: TrechoDocumentoParCreator.
	 */
	public function __construct(){
		$this->TrechoDocumentoParCreator = new TrechoDocumentoParCreator();
		$this->DadosDocumentoParCreator = new DadosDocumentoParCreator();
	}

	/**
	 * Busca, trabalhando com as Factory's dos Trechos, o array 
	 * de dados prontos para 
	 *
	 * @return array $arrayDados
	 */
	public function carregaDados( ModeloDocumentoPar $ModeloDocumentoPar ){
		$this->ModeloDocumentoPar = $ModeloDocumentoPar;

		switch ( $this->ModeloDocumentoPar->mdoid ) {
			case MDOID_TERMO_COMPROMISSO_MUNICIPIOS:

				$arrayDados['Identificacao'] = $this->TrechoDocumentoParCreator->parsearTrecho( 
					new IdentificacaoDocumentoPar( $this->ModeloDocumentoPar ) );

				$arrayDados['AcoesFinanceiras'] = $this->TrechoDocumentoParCreator->parsearTrecho( 
					new AcoesFinanceirasDocumentoPar( $this->ModeloDocumentoPar ) );

				$arrayDados['Empenhos'] = $this->TrechoDocumentoParCreator->parsearTrecho( 
					new EmpenhoDocumentoPar( $this->ModeloDocumentoPar ) );

				$arrayDados['assinatura_data'] = $this->DadosDocumentoParCreator->parsearDado( new DataAssinatura( $this->ModeloDocumentoPar ) );

				break;
			default:
				return false;
				break;
		}
		
		return $arrayDados;
	}

}

/**
 * Controller do DocumentoPar
 */
class DocumentoPar implements DocumentoParInterface
{
	// Modelo
	protected $ModeloDocumentoPar;
	// Fábrica
	protected $AbstractFactory_DocumentoPar;
	// Dados
	protected $arrayDados;

	public function __construct( $dopid ){
		$this->ModeloDocumentoPar = new ModeloDocumentoPar();
		$this->ModeloDocumentoPar->carregarPorId( $dopid );

		$this->AbstractFactory_DocumentoPar = new AbstractFactory_DocumentoPar();
	}

	/**
	 * Prepara Estado do {objeto} DocumentoPar para inserir dados, obtidos do HTML, no BD
	 *
	 * @return void
	 */
	public function parsearDadosParaBanco(){
		
		$this->arrayDados = $this->AbstractFactory_DocumentoPar->carregaDados( $this->ModeloDocumentoPar );
		$this->registraDocumentoPar();

	}

	/**
	 * Prepara Estado do {objeto} DocumentoPar para inserir dados, obtidos na fonte da 
	 * criação do HTML do documento, no BD
	 *
	 * @return void
	 */
	public function prepararDadosDaFonte(){
		// TODO: preparar dados  da fonte da criação do documento/termo para registrar
	}

	/**
	 * Registra Estado Atual do {objeto} Documento Par.
	 * Interface:
	 *		Array(
	 *			[Identificacao] => Array(),
	 *			[AcoesFinanceiras] => Array(),
	 *			[Empenhos] => Array()
	 *		)
	 *
	 * @return void
	 * @todo abstrair trechos pelo modelo do documento
	 */
	public function registraDocumentoPar(){
		global $db;
		// ver($this->arrayDados,d);

		$termocomposicao = $this->buscaTermoComposicao( $this->ModeloDocumentoPar->dopid );
		
		$this->loopIdentificacao(false);
		$this->loopAcoesFinanciadas(false, $termocomposicao);
		$this->loopEmpenhos(false, $termocomposicao);
		
		$this->registraHistoricoDocumentoPar();

		$this->atualizaDataAssinatura( $this->ModeloDocumentoPar->dopid, $this->arrayDados['assinatura_data'] );
	}

	/**
	 * Busca termo composicao
	 *
	 * @return array termocomposicao
	 */
	private function buscaTermoComposicao( $dopid ){
		global $db;

		$sql = " select * from par.termocomposicao where dopid = {$this->ModeloDocumentoPar->dopid} ";
		$termocomposicao = $db->carregar( $sql );

		return $termocomposicao[0];
	}

	/**
	 * Método de Loop para inserção no BD
	 *
	 * @todo transferir esse método para a classe "TrechoDocumentoPar" pq é uma ação 
	 *		 referente ao mesmo.
	 * @todo usar modelo para abstrair insersao no BD
	 * @return bool
	 */
	protected function loopIdentificacao( $historico = false ){
		global $db;

		foreach ($this->arrayDados['Identificacao'] as $key => $value) {
			
			$mesinicial_partes = explode('/', $value['mesinicial']);
			$value['mesinicial'] = $mesinicial_partes[1] . '-' . $mesinicial_partes[0] . '-01';
			$mesfinal_partes = explode('/', $value['mesfinal']);
			$value['mesfinal'] = $mesfinal_partes[1] . '-' . $mesfinal_partes[0] . '-01';

			if( !$historico ){
				$sql = " 
					INSERT INTO par.documentopar_identificacao ( 
						dopid,
						programa,
						processo_numero,
						prefeitura_nome,
						prefeitura_cnpj,
						prefeitura_endereco,
						prefeitura_municipio,
						prefeitura_uf,
						prefeito_nome,
						prefeito_cpf,
						mesinicial ,
						mesfinal )
					VALUES(
						'{$this->ModeloDocumentoPar->dopid}', 
						'{$value['programa']}', 
						'{$value['processo_numero']}', 
						'{$value['prefeitura_nome']}', 
						'{$value['prefeitura_cnpj']}', 
						'{$value['prefeitura_endereco']}', 
						'{$value['prefeitura_municipio']}', 
						'{$value['prefeitura_uf']}', 
						'{$value['prefeito_nome']}', 
						'{$value['prefeito_cpf']}', 
						'{$value['mesinicial']}', 
						'{$value['mesfinal']}' ) ";
			}else{
				$sql = " 
					INSERT INTO par.documentopar_historico_identificacao ( 
						dphid,
					 	programa,
					 	processo_numero,
					 	prefeitura_nome,
					 	prefeitura_cnpj,
					 	prefeitura_endereco,
					 	prefeitura_municipio,
					 	prefeitura_uf,
					 	prefeito_nome,
					 	prefeito_cpf,
					 	mesinicial,
					 	mesfinal )
					VALUES(
						'{$historico}', 
						'{$value['programa']}', 
						'{$value['processo_numero']}', 
						'{$value['prefeitura_nome']}', 
						'{$value['prefeitura_cnpj']}', 
						'{$value['prefeitura_endereco']}', 
						'{$value['prefeitura_municipio']}', 
						'{$value['prefeitura_uf']}', 
						'{$value['prefeito_nome']}', 
						'{$value['prefeito_cpf']}', 
						'{$value['mesinicial']}', 
						'{$value['mesfinal']}' ) ";
			}

			$db->executar( $sql );
		}
		return $db->commit();
	}

	/**
	 * Método de Loop para inserção no BD
	 *
	 * @todo transferir esse método para a classe "TrechoDocumentoPar" pq é uma ação 
	 *		 referente ao seu escopo.
	 * @todo decidir o que fazer com a situacao de comparacao de strings retornar falso.
	 * @return bool
	 */
	protected function loopAcoesFinanciadas( $historico = false, $termocomposicao ){
		global $db;

		foreach ($this->arrayDados['AcoesFinanceiras'] as $key => $value) {

			$ItemComposicao = $this->buscaDescricaoItemComposicao( $termocomposicao['sbdid'] );
			$subacao = explode('.', $value['subacao']);

			if( !$historico ){
				if( strtoupper(trim($ItemComposicao['icodescricao'])) != strtoupper(trim($value['tipo'])) ){}

				$sql = " 
					INSERT INTO par.termocomposicao_itens ( 
						icoid,
						tcoid,
					 	subacao_dimensao,
					 	subacao_area,
					 	subacao_indicador,
					 	subacao_subacao,
					 	tipo,
					 	arp,
					 	tiposubacao,
					 	metasqtds,
					 	precounitario,
					 	total )
					VALUES(
						{$ItemComposicao['icoid']},
						{$termocomposicao['tcoid']}, 
						'{$subacao[0]}', 
						'{$subacao[1]}', 
						'{$subacao[2]}', 
						'{$subacao[3]}', 
						'{$value['tipo']}', 
						'{$value['tiposubacao']}', 
						'{$value['arp']}', 
						'{$value['metasqtds']}', 
						'{$value['precounitario']}', 
						'{$value['total']}' ) ";
			}else{
				$sql = " 
					INSERT INTO par.termocomposicao_itens_historico ( 
					 	icoid,
						tchid,
					 	subacao_dimensao,
					 	subacao_area,
					 	subacao_indicador,
					 	subacao_subacao,
					 	tipo,
					 	tiposubacao,
					 	arp,
					 	metasqtds,
					 	precounitario,
					 	total )
					VALUES(
						{$ItemComposicao['icoid']},
						{$termocomposicao['tchid']}, 
						'{$subacao[0]}', 
						'{$subacao[1]}', 
						'{$subacao[2]}', 
						'{$subacao[3]}', 
						'{$value['tipo']}', 
						'{$value['tiposubacao']}', 
						'{$value['arp']}', 
						'{$value['metasqtds']}', 
						'{$value['precounitario']}', 
						'{$value['total']}' ) ";
					// ver($sql,d);
			}
			$db->executar( $sql );
		}
		return $db->commit();
	}

	/**
	 * Busca descrição do item composição para ser comparado com dado 'tipo'
	 * obtido na extração de dados para Ações Financiadas.
	 *
	 * @return array termocomposicao
	 */
	private function buscaDescricaoItemComposicao( $sbdid ){
		global $db;

		$sql = " 
			SELECT si.icoid, si.icodescricao
			FROM par.subacao s 
			INNER JOIN par.subacaodetalhe sd on sd.sbaid = s.sbaid 
			INNER JOIN par.subacaoitenscomposicao si on si.sbaid = s.sbaid and sd.sbdano = si.icoano
			WHERE
				sd.sbdid = {$sbdid} ";
		$itemComposicao = $db->carregar( $sql );

		return $itemComposicao[0];
	}

	/**
	 * Método de Loop para inserção no BD
	 *
	 * @todo transferir esse método para a classe "TrechoDocumentoPar" pq é uma ação 
	 *		 referente ao mesmo.
	 * @return bool
	 */
	protected function loopEmpenhos( $historico = false, $termocomposicao ){
		global $db;

		foreach ($this->arrayDados['Empenhos'] as $key => $value) {
			
			$Empenho = $this->buscaEmpenho( $termocomposicao['dopid'] );
			$subacao = explode('.', $value['subacao']);

			if( !$historico ){
				if( strtoupper(trim($Empenho['empnumero'])) != trim($value['numero']) ){}

				$sql = " 
					INSERT INTO par.documentopar_empenhos ( 
						tcoid,
						empid,
					 	subacao_dimensao,
					 	subacao_area,
					 	subacao_indicador,
					 	subacao_subacao,
					 	numero,
					 	valor )
					VALUES(
						{$termocomposicao['tcoid']}, 
						{$Empenho['empid']},
						'{$subacao[0]}', 
						'{$subacao[1]}', 
						'{$subacao[2]}', 
						'{$subacao[3]}', 
						'{$value['numero']}', 
						'{$value['valor']}' ) ";
			}else{
				$sql = " 
					INSERT INTO par.documentopar_historico_empenhos ( 
					 	tchid,
						empid,
					 	subacao_dimensao,
					 	subacao_area,
					 	subacao_indicador,
					 	subacao_subacao,
					 	numero,
					 	valor )
					VALUES(
						{$termocomposicao['tchid']}, 
						{$Empenho['empid']},
						'{$subacao[0]}', 
						'{$subacao[1]}', 
						'{$subacao[2]}', 
						'{$subacao[3]}', 
						'{$value['numero']}', 
						'{$value['valor']}' ) ";
			}
			$db->executar( $sql );
		}
		return $db->commit();
	}

	/**
	 * Busca Empenho
	 *
	 * @return array empenho obtido da tabela par.empenho
	 */
	private function buscaEmpenho( $dopid ){
		global $db;

		$sql = "
			SELECT 
				emp.empid,
				emp.empnumero
			FROM 
			    par.documentopar dop
			INNER JOIN par.processopar prp ON prp.prpid = dop.prpid and prp.prpstatus = 'A'
			INNER JOIN par.empenho emp ON emp.empnumeroprocesso = prp.prpnumeroprocesso and empstatus = 'A'
			WHERE dop.dopid = {$dopid}
		";
		$empenho = $db->carregar( $sql );

		return $empenho[0];
	}

	/**
	 * Função feita para comparar Empenhos entre o que foi obtido 
	 * no parseamento e o que consta associado com o documentopar.
	 * 
	 * @return bool
	 */
	private function verificaEmpenhosAssociados(){
		// TODO: 
	}

	/**
	 * Função feita para comparar Ações Financiadas entre o que foi obtido 
	 * no parseamento e o que consta associado com o documentopar.
	 * 
	 * @return bool
	 */
	private function verificaAcoesFincanciadasAssociadas(){
		// TODO: 
	}

	/**
	 * Registra Estado Atual do {objeto} DocumentoPar como Dado Histórico.
	 *
	 * @return bool
	 */
	public function registraHistoricoDocumentoPar(){
		global $db;

		$dphid = $this->copyDocumentoParRow();

		// insercao de termocomposicao
		$sql = " INSERT INTO par.termocomposicao_historico (dphid) VALUES ({$dphid}) RETURNING tchid ";
		$tchid = $db->pegaUm( $sql );

		$termocomposicao = $this->buscaTermoComposicao( $this->ModeloDocumentoPar->dopid );
		$termocomposicao['tchid'] = $tchid;

		// insercao das tabelas dependentes do termo composicao
		$this->loopIdentificacao($dphid);
		$this->loopAcoesFinanciadas(true, $termocomposicao);
		$this->loopEmpenhos(true, $termocomposicao);

	}

	/**
	 * Copia Linha de Documento Par para tabela historica ( par.documentopar_historico )
	 *
	 * @return dphid (id da tabela par.documentopar_historico)
	 */
	private function copyDocumentoParRow(){
		global $db;

		$sql = " 
			INSERT INTO par.documentopar_historico 
				( dopid, prpid, doptexto, dopstatus, dopdiasvigencia, dopdatainicio, dopdatafim, mdoid, dopdatainclusao, usucpfinclusao, dopdataalteracao, usucpfalteracao, dopjustificativa, dopdatavalidacaofnde, dopusucpfvalidacaofnde, dopdatavalidacaogestor, dopusucpfvalidacaogestor, dopusucpfstatus, dopdatastatus, dopdatapublicacao, doppaginadou, dopnumportaria, proid, dopreformulacao, dopidpai, dopidaditivo, dopnumerodocumento, dopobservacao, dopdatafimvigencia, dopvalortermo, dopdatainiciovigencia, arqid, dopacompanhamento, dopano, dopdocumentocarregado, assinatura_data )
				SELECT 
				dopid, prpid, doptexto, dopstatus, dopdiasvigencia, dopdatainicio, dopdatafim, mdoid, dopdatainclusao, usucpfinclusao, dopdataalteracao, usucpfalteracao, dopjustificativa, dopdatavalidacaofnde, dopusucpfvalidacaofnde, dopdatavalidacaogestor, dopusucpfvalidacaogestor, dopusucpfstatus, dopdatastatus, dopdatapublicacao, doppaginadou, dopnumportaria, proid, dopreformulacao, dopidpai, dopidaditivo, dopnumerodocumento, dopobservacao, dopdatafimvigencia, dopvalortermo, dopdatainiciovigencia, arqid, dopacompanhamento, dopano, dopdocumentocarregado, assinatura_data
				FROM par.documentopar WHERE dopid = {$this->ModeloDocumentoPar->dopid}
			RETURNING dphid ";

		$dphid = $db->carregar( $sql );
		return $dphid[0]['dphid'];

	}

	/**
	 * Atualiza data de assinatura do Termo
	 *
	 * @return bool
	 */
	private function atualizaDataAssinatura( $dopid, $assinatura_data ){
		global $db;

		$sql = " UPDATE par.documentopar SET assinatura_data = '{$assinatura_data}' WHERE dopid = {$dopid} ";
		$db->executar( $sql );

		return $db->commit();
	}

}