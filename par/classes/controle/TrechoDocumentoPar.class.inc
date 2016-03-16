<?php

Interface TrechoDocumentoPar{

	public function parsearDados();

	public function getArrayDados();

}

/**
 * Classe que encapsula as regras de criacao dos Trechos dos Documentos
 */
class TrechoDocumentoParCreator
{

	/**
	 * Factory Method
	 * @return array $arrayDados - array de dados do respectivo trecho
	 */
	public function parsearTrecho( TrechoDocumentoPar $trecho ){

		$trecho->parsearDados();
		return $trecho->getArrayDados();

	}

}

abstract class AbstractTrechoDocumentoPar
{

	protected $ModeloDocumentoPar;
	protected $DadosDocumentoParCreator;

	public function __construct( ModeloDocumentoPar $ModeloDocumentoPar ){
		$this->ModeloDocumentoPar = $ModeloDocumentoPar;
		$this->DadosDocumentoParCreator = new DadosDocumentoParCreator();
	}

	public function getArrayDados(){
		return $this->arrayDados;		
	}
}

/**
 * 
 */
class IdentificacaoDocumentoPar extends AbstractTrechoDocumentoPar implements TrechoDocumentoPar
{
	protected $arrayDados = array();

	/**
	 * @internal está sendo pressuposto para o modelo MDOID_TERMO_COMPROMISSO_MUNICIPIOS,
	 * 			 que para cada documento só existe uma identificação, por isso o "[0]"
	 * @return void
	 */
	public function parsearDados(){

		switch ( $this->ModeloDocumentoPar->mdoid ) {
			case MDOID_TERMO_COMPROMISSO_MUNICIPIOS:

				$this->arrayDados[0]['programa']			 	= $this->DadosDocumentoParCreator->parsearDado( 
					new Programa( $this->ModeloDocumentoPar ) );

				$this->arrayDados[0]['dopano']				 	= $this->DadosDocumentoParCreator->parsearDado( 
					new Exercicio( $this->ModeloDocumentoPar ) );

				$this->arrayDados[0]['processo_numero']			= $this->DadosDocumentoParCreator->parsearDado( 
					new NumeroProcesso( $this->ModeloDocumentoPar ) );

				$this->arrayDados[0]['prefeitura_nome']			= $this->DadosDocumentoParCreator->parsearDado( 
					new NomePrefeitura( $this->ModeloDocumentoPar ) );

				$this->arrayDados[0]['prefeitura_cnpj']			= $this->DadosDocumentoParCreator->parsearDado( 
					new Cnpj( $this->ModeloDocumentoPar ) );

				$this->arrayDados[0]['prefeitura_endereco']		= $this->DadosDocumentoParCreator->parsearDado( 
					new Endereco( $this->ModeloDocumentoPar ) );

				$this->arrayDados[0]['prefeitura_municipio']	= $this->DadosDocumentoParCreator->parsearDado( 
					new Municipio( $this->ModeloDocumentoPar ) );

				$this->arrayDados[0]['prefeitura_uf']			= $this->DadosDocumentoParCreator->parsearDado( 
					new Uf( $this->ModeloDocumentoPar ) );

				$this->arrayDados[0]['prefeito_nome']			= $this->DadosDocumentoParCreator->parsearDado( 
					new NomePrefeito( $this->ModeloDocumentoPar ) );

				$this->arrayDados[0]['prefeito_cpf']			= $this->DadosDocumentoParCreator->parsearDado( 
					new CpfPrefeito( $this->ModeloDocumentoPar ) );

				$this->arrayDados[0]['mesinicial']				= $this->DadosDocumentoParCreator->parsearDado( 
					new MesInicial( $this->ModeloDocumentoPar ) );

				$this->arrayDados[0]['mesfinal']				= $this->DadosDocumentoParCreator->parsearDado( 
					new MesFinal( $this->ModeloDocumentoPar ) );

				break;
			
			default:
				return false;
				break;
		}
		
		return $this;
	}

}

class AcoesFinanceirasDocumentoPar extends AbstractTrechoDocumentoPar implements TrechoDocumentoPar
{
	protected $arrayDados = array();

	/**
	 * Variável que guarda o ponto onde continuiar a extração com o loop
	 */
	private $buscaDeLinha;
	/**
	 * Variável que determina o final da extração de Ações Financeiras
	 */
	private $buscaTotalGeral;
	/**
	 * Variável usada para busca da próxima linha
	 */
	private $textoLista;
	/**
	 * Variável usada para gestão da posição de navegação no texto(html) durante a extração
	 */
	private $posicaoInicialDaLinha;
	private $posicaoFinalDaLinha;

	/**
	 * @return void
	 */
	public function parsearDados(){

		$this->extrairSessaoAcoesFinanciadas();

	}

	/**
	 * @return void
	 * @internal faz loop necessário para extração de linhas
	 * @internal armazena na variável $arrayDados os campos
	 */
	protected function extrairSessaoAcoesFinanciadas(){
		$this->preparaTextoParaExtracao();

		$Subacao 		= new Subacao( $this->ModeloDocumentoPar );
		$Tipo 			= new Tipo( $this->ModeloDocumentoPar );
		$TipoSubacao 	= new TipoSubacao( $this->ModeloDocumentoPar );
		$Arp 			= new Arp( $this->ModeloDocumentoPar );
		$MetasQtds 		= new MetasQtds( $this->ModeloDocumentoPar );
		$PrecoUnitario 	= new PrecoUnitario( $this->ModeloDocumentoPar );
		$Total 			= new Total( $this->ModeloDocumentoPar );

		// loop para identificar todas as acoes financeiras
		$Subacao->textoComContinuidade = $this->buscaDeLinha;
		while( $this->buscaDeLinha !== false && $this->buscaTotalGeral === false ){

			$array = array();
			$array['subacao'] 		= $this->DadosDocumentoParCreator->parsearDado( $Subacao );
				$Tipo->textoComContinuidade = $Subacao->textoComContinuidade;

			$array['tipo'] 			= $this->DadosDocumentoParCreator->parsearDado( $Tipo );
				$TipoSubacao->textoComContinuidade = $Tipo->textoComContinuidade;

			$array['tiposubacao'] 	= $this->DadosDocumentoParCreator->parsearDado( $TipoSubacao );
				$Arp->textoComContinuidade = $TipoSubacao->textoComContinuidade;

			$array['arp'] 			= $this->DadosDocumentoParCreator->parsearDado( $Arp );
				$MetasQtds->textoComContinuidade = $Arp->textoComContinuidade;

			$array['metasqtds'] 	= $this->DadosDocumentoParCreator->parsearDado( $MetasQtds );
				$PrecoUnitario->textoComContinuidade = $MetasQtds->textoComContinuidade;

			$array['precounitario'] = $this->DadosDocumentoParCreator->parsearDado( $PrecoUnitario );
				$Total->textoComContinuidade = $PrecoUnitario->textoComContinuidade;

			$array['total'] 		= $this->DadosDocumentoParCreator->parsearDado( $Total );
			// ver($array,d);

			array_push($this->arrayDados, $array);
			unset($array);

			$this->textoLista = $Total->textoComContinuidade;

			$this->preparaTextoParaProximaLinha();
		}
	}

	/**
	 * Prepara o campo "$this->buscaDeLinha", que é onde fica a continuidade do texto,
	 * para iniciar extração de dados no loop.
	 */
	private function preparaTextoParaExtracao(){
		// retirada do titulo da lista
		$paramBuscaIdentificacaoAcoesFinanceiras = utf8_decode('IDENTIFICAÇÃO E DELIMITAÇÃO DAS AÇÕES FINANCIADAS');
		$posicaoInicial = strpos($this->ModeloDocumentoPar->doptexto, $paramBuscaIdentificacaoAcoesFinanceiras) + strlen($paramBuscaIdentificacaoAcoesFinanceiras);
		$textoListaAcoesFinanciadas1 = substr($this->ModeloDocumentoPar->doptexto, $posicaoInicial);

		// busca da tabela da lista
		$paramBuscaIdentificacaoAcoesFinanceiras = utf8_decode('<TABLE ');
		$posicaoInicial = strpos($textoListaAcoesFinanciadas1, $paramBuscaIdentificacaoAcoesFinanceiras) + strlen($paramBuscaIdentificacaoAcoesFinanceiras);
		$this->textoLista = substr($textoListaAcoesFinanciadas1, $posicaoInicial);

		// busca posicao final da lista
		$posicaoFinal = strpos($this->textoLista, '</TABLE>');
		$this->textoLista = substr($this->textoLista, 0, $posicaoFinal);

		// busca linhas de conteudo
		$paramBuscaIdentificacaoAcoesFinanceiras = utf8_decode('<TR');
		$paramBuscaIdentificacaoAcoesFinanceirasFim = utf8_decode('</TR>');
		$this->posicaoInicialDaLinha = strpos($this->textoLista, $paramBuscaIdentificacaoAcoesFinanceiras);
		$this->posicaoFinalDaLinha = strpos(substr($this->textoLista, $this->posicaoInicialDaLinha), $paramBuscaIdentificacaoAcoesFinanceirasFim);
		$this->buscaDeLinha = substr($this->textoLista, $this->posicaoInicialDaLinha, $this->posicaoFinalDaLinha);

			// busca proxima linha (onde comeca o conteudo)
			$this->buscaDeLinha = substr(substr($this->textoLista, $this->posicaoInicialDaLinha), $this->posicaoFinalDaLinha+strlen('</TR>'));
			$this->buscaDeLinha = substr($this->buscaDeLinha, 0, strpos($this->buscaDeLinha, '</TR>'));
		$this->textoLista = substr($this->textoLista, $this->posicaoInicialDaLinha);

		// verifica se linha já não é a ultima
		$this->buscaTotalGeral = strpos($this->buscaDeLinha, 'TOTAL GERAL');
	}

	/**
	 * Prepara busca 
	 */
	private function preparaTextoParaProximaLinha(){
		// busca de linha
		$this->textoLista = substr($this->textoLista, $this->posicaoFinalDaLinha+strlen('</TR>'));
		// ver(simec_htmlentities($this->textoLista),d);
		$this->posicaoInicialDaLinha = strpos($this->textoLista, $paramBuscaIdentificacaoAcoesFinanceiras);
		$this->posicaoFinalDaLinha = strpos(substr($this->textoLista, $this->posicaoInicialDaLinha), $paramBuscaIdentificacaoAcoesFinanceirasFim);
		$this->buscaDeLinha = substr($this->textoLista, $this->posicaoInicialDaLinha, $this->posicaoFinalDaLinha);
		$this->buscaTotalGeral = strpos($this->buscaDeLinha, 'TOTAL GERAL');
	}
}

class EmpenhoDocumentoPar extends AbstractTrechoDocumentoPar implements TrechoDocumentoPar
{
	protected $arrayDados = array();
	
	/**
	 * Variável que guarda o ponto onde continuiar a extração com o loop
	 */
	private $buscaDeLinha;
	/**
	 * Variável que determina o final da extração de Ações Financeiras
	 */
	private $buscaTotalGeral;
	/**
	 * Variável usada para busca da próxima linha
	 */
	private $textoLista;
	/**
	 * Variável usada para gestão da posição de navegação no texto(html) durante a extração
	 */
	private $posicaoInicialDaLinha;
	private $posicaoFinalDaLinha;

	/**
	 * @return void
	 */
	public function parsearDados(){
		
		$this->extrairSessaoEmpenhos();

	}

	/**
	 * @return void
	 * @internal faz loop necessário para extração de linhas
	 * @internal armazena na variável $arrayDados os campos
	 */
	protected function extrairSessaoEmpenhos(){
		$this->preparaTextoParaExtracao();
		
		$SubacaoEmpenho	= new SubacaoEmpenho( $this->ModeloDocumentoPar );
		$Numero 		= new Numero( $this->ModeloDocumentoPar );
		$Valor 			= new Valor( $this->ModeloDocumentoPar );
		
		// loop para identificar todas as acoes financeiras
		$SubacaoEmpenho->textoComContinuidade = $this->buscaDeLinha;
		while( $this->buscaDeLinha !== false && $this->buscaTotalGeral === false ){

			$array = array();
			$array['subacao']	= $this->DadosDocumentoParCreator->parsearDado( $SubacaoEmpenho );
				$Numero->textoComContinuidade = $SubacaoEmpenho->textoComContinuidade;

			$array['numero'] 	= $this->DadosDocumentoParCreator->parsearDado( $Numero );
				$Valor->textoComContinuidade = $Numero->textoComContinuidade;

			$array['valor'] 	= $this->DadosDocumentoParCreator->parsearDado( $Valor );

			array_push($this->arrayDados, $array);
			unset($array);

			$this->textoLista = $Valor->textoComContinuidade;

			$this->preparaTextoParaProximaLinha();
		}
	}

	/**
	 * Prepara o campo "$this->buscaDeLinha", que é onde fica a continuidade do texto,
	 * para iniciar extração de dados no loop.
	 */
	private function preparaTextoParaExtracao(){
		// retirada do titulo da lista
		$paramBuscaIdentificacaoEmpenhos = utf8_decode('EMPENHOS');
		$posicaoInicial = strpos($this->ModeloDocumentoPar->doptexto, $paramBuscaIdentificacaoEmpenhos) + strlen($paramBuscaIdentificacaoEmpenhos);
		$textoListaEmpenho1 = substr($this->ModeloDocumentoPar->doptexto, $posicaoInicial); // ver($textoListaEmpenho1,d);
		// busca da tabela da lista
		$paramBuscaIdentificacaoEmpenhos = utf8_decode('<TABLE ');
		$posicaoInicial = strpos($textoListaEmpenho1, $paramBuscaIdentificacaoEmpenhos) + strlen($paramBuscaIdentificacaoEmpenhos);
		$this->textoLista = substr($textoListaEmpenho1, $posicaoInicial); // ver($this->textoLista,d);
		// busca posicao final da lista
		$posicaoFinal = strpos($this->textoLista, '</TABLE>'); // ver($posicaoFinal,d);
		$this->textoLista = substr($this->textoLista, 0, $posicaoFinal);
		// busca linhas de conteudo
		$paramBuscaIdentificacaoEmpenhos = utf8_decode('<TR');
		$paramBuscaIdentificacaoEmpenhosFim = utf8_decode('</TR>');
		$this->posicaoInicialDaLinha = strpos($this->textoLista, $paramBuscaIdentificacaoEmpenhos);
		$this->posicaoFinalDaLinha = strpos(substr($this->textoLista, $this->posicaoInicialDaLinha), $paramBuscaIdentificacaoEmpenhosFim);
		$this->buscaDeLinha = substr($this->textoLista, $this->posicaoInicialDaLinha, $this->posicaoFinalDaLinha);
			// busca proxima linha (onde comeca o conteudo)
			$this->buscaDeLinha = substr(substr($this->textoLista, $this->posicaoInicialDaLinha), $this->posicaoFinalDaLinha+strlen('</TR>'));
			$this->buscaDeLinha = substr($this->buscaDeLinha, 0, strpos($this->buscaDeLinha, '</TR>'));
		$this->textoLista = substr($this->textoLista, $this->posicaoInicialDaLinha);
		// verifica se linha já não é a ultima
		$this->buscaTotalGeral = strpos($this->buscaDeLinha, 'TOTAL GERAL');
	}

	/**
	 * Prepara busca 
	 */
	private function preparaTextoParaProximaLinha(){
		// busca de linha
		$this->textoLista = substr($this->textoLista, $this->posicaoFinalDaLinha+strlen('</TR><TR'));
		$this->posicaoInicialDaLinha = strpos($this->textoLista, $paramBuscaIdentificacaoEmpenhos);
		$this->posicaoFinalDaLinha = strpos(substr($this->textoLista, $this->posicaoInicialDaLinha), $paramBuscaIdentificacaoEmpenhosFim);
		$this->buscaDeLinha = substr($this->textoLista, $this->posicaoInicialDaLinha, $this->posicaoFinalDaLinha);
		$this->buscaTotalGeral = strpos($this->buscaDeLinha, 'TOTAL EMPENHO');
	}
}