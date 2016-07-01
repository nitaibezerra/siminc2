<?php


Interface DadosDocumentoPar
{

	// public function iteracaoDados();

	// public function extrair();

	// public function getDado();

}

Interface TrechoIdentificacaoDocumentoPar extends DadosDocumentoPar{ }

Interface TrechoAcoesFinanceirasDocumentoPar extends DadosDocumentoPar{ }

Interface TrechoEmpenhosDocumentoPar extends DadosDocumentoPar{ }

/**
 * Abstração da criação de instância de Dados do DocumentoPar
 */
class DadosDocumentoParCreator
{
	 /**
	 * Factory Method
	 * @return string $dado - dado requisitado pelo trecho
	 */
	public function parsearDado( DadosDocumentoPar $dado ){

		$dado->extrair();
		return $dado->getDado();

	}
}

abstract class AbstractDadosDocumentoPar implements DadosDocumentoPar 
{
	protected $dado;
	protected $ModeloDocumentoPar;
	public $textoComContinuidade;

	public function __construct( ModeloDocumentoPar $ModeloDocumentoPar ){
		$this->ModeloDocumentoPar = $ModeloDocumentoPar;
		$this->ModeloDocumentoPar->doptexto = htmlspecialchars_decode($this->ModeloDocumentoPar->doptexto);
	}

	abstract public function extrair();

	public function getDado(){
		return $this->dado;
	}
}


// ################################################################################################## IDENTIFICACAO

class Programa extends AbstractDadosDocumentoPar implements TrechoIdentificacaoDocumentoPar
{
	protected $dado;

	public function extrair(){
		$paramBuscaPrograma = '01 - PROGRAMA(S)</B><BR>';
		$posicaoInicial = strpos($this->ModeloDocumentoPar->doptexto, $paramBuscaPrograma) + strlen($paramBuscaPrograma);
		$param2BuscaPrograma = '</TD>';
		$quantidadeCaracteres = strpos( strstr($this->ModeloDocumentoPar->doptexto, $paramBuscaPrograma), $param2BuscaPrograma );
		$programa = substr($this->ModeloDocumentoPar->doptexto, $posicaoInicial, $quantidadeCaracteres);
		$programa = trim(strip_tags($programa)); 
		
		$this->dado = $programa;

		return $this;
	}
}

/**
 * @see propriedade par.documento.dopano é o exercício
 */
class Exercicio extends AbstractDadosDocumentoPar implements TrechoIdentificacaoDocumentoPar
{
	protected $dado;

	public function extrair(){
		$paramBuscaExercicio = utf8_decode('02 - EXERCÍCIO</B><BR>');
		$posicaoInicial = strpos($this->ModeloDocumentoPar->doptexto, $paramBuscaExercicio) + strlen($paramBuscaExercicio);
		$param2BuscaExercicio = '</TD>';
		$quantidadeCaracteres = strpos( strstr($this->ModeloDocumentoPar->doptexto, $paramBuscaExercicio), $param2BuscaExercicio );
		$exercicio = substr($this->ModeloDocumentoPar->doptexto, $posicaoInicial, $quantidadeCaracteres);
		$exercicio = trim(strip_tags($exercicio));

		$this->dado = $exercicio;

		return $this;
	}
}

class NumeroProcesso extends AbstractDadosDocumentoPar implements TrechoIdentificacaoDocumentoPar
{
	protected $dado;

	public function extrair(){
		$paramBuscaNProcesso = utf8_decode('03 - Nº PROCESSO</B><BR>');
		$posicaoInicial = strpos($this->ModeloDocumentoPar->doptexto, $paramBuscaNProcesso) + strlen($paramBuscaNProcesso);
		$param2BuscaNProcesso = '</TD>';
		$quantidadeCaracteres = strpos( strstr($this->ModeloDocumentoPar->doptexto, $paramBuscaNProcesso), $param2BuscaNProcesso );
		$nprocesso = substr($this->ModeloDocumentoPar->doptexto, $posicaoInicial, $quantidadeCaracteres);
		$nprocesso = trim(strip_tags($nprocesso));

		$this->dado = $nprocesso;

		return $this;
	}
}

class NomePrefeitura extends AbstractDadosDocumentoPar implements TrechoIdentificacaoDocumentoPar
{
	protected $dado;

	public function extrair(){
		$paramBuscaNomePrefeitura = utf8_decode('04 - NOME DA PREFEITURA</B><BR>');
		$posicaoInicial = strpos($this->ModeloDocumentoPar->doptexto, $paramBuscaNomePrefeitura) + strlen($paramBuscaNomePrefeitura);
		$param2BuscaNomePrefeitura = '</TD>';
		$quantidadeCaracteres = strpos( strstr($this->ModeloDocumentoPar->doptexto, $paramBuscaNomePrefeitura), $param2BuscaNomePrefeitura );
		$nomePrefeitura = substr($this->ModeloDocumentoPar->doptexto, $posicaoInicial, $quantidadeCaracteres);
		$nomePrefeitura = trim($nomePrefeitura);
		$tdRestante = strpos($nomePrefeitura, '</TD>');
		$nomePrefeitura = substr($nomePrefeitura, 0, $tdRestante);

		$this->dado = $nomePrefeitura;

		return $this;
	}
}

class Cnpj extends AbstractDadosDocumentoPar implements TrechoIdentificacaoDocumentoPar
{
	protected $dado;

	public function extrair(){
		$paramBuscaCNPJ = utf8_decode('05 - N.º DO CNPJ</B><BR>');
		$posicaoInicial = strpos($this->ModeloDocumentoPar->doptexto, $paramBuscaCNPJ) + strlen($paramBuscaCNPJ);
		$param2BuscaCNPJ = '</TD>';
		$quantidadeCaracteres = strpos( strstr($this->ModeloDocumentoPar->doptexto, $paramBuscaCNPJ), $param2BuscaCNPJ );
		$cnpj = substr($this->ModeloDocumentoPar->doptexto, $posicaoInicial, $quantidadeCaracteres);
		$cnpj = trim(strip_tags($cnpj));

		$this->dado = $cnpj;

		return $this;
	}
}

class Endereco extends AbstractDadosDocumentoPar implements TrechoIdentificacaoDocumentoPar
{
	protected $dado;

	public function extrair(){
		$paramBuscaEndereco = utf8_decode('06 - ENDEREÇO</B><BR>');
		$posicaoInicial = strpos($this->ModeloDocumentoPar->doptexto, $paramBuscaEndereco) + strlen($paramBuscaEndereco);
		$param2BuscaEndereco = '</TD>';
		$quantidadeCaracteres = strpos( strstr($this->ModeloDocumentoPar->doptexto, $paramBuscaEndereco), $param2BuscaEndereco );
		$endereco = substr($this->ModeloDocumentoPar->doptexto, $posicaoInicial, $quantidadeCaracteres);
		$endereco = trim($endereco); 
		$tdRestante = strpos($endereco, '</TD>');
		$endereco = substr($endereco, 0, $tdRestante);

		$this->dado = $endereco;

		return $this;
	}
}

class Municipio extends AbstractDadosDocumentoPar implements TrechoIdentificacaoDocumentoPar
{
	protected $dado;

	public function extrair(){
		$paramBuscaMunicipio = utf8_decode('07 - MUNICÍPIO</B><BR>');
		$posicaoInicial = strpos($this->ModeloDocumentoPar->doptexto, $paramBuscaMunicipio) + strlen($paramBuscaMunicipio);
		$param2BuscaMunicipio = '</TD>';
		$quantidadeCaracteres = strpos( strstr($this->ModeloDocumentoPar->doptexto, $paramBuscaMunicipio), $param2BuscaMunicipio );
		$municipio = substr($this->ModeloDocumentoPar->doptexto, $posicaoInicial, $quantidadeCaracteres);
		$municipio = trim($municipio); 
		$tdRestante = strpos($municipio, '</TD>');
		$municipio = substr($municipio, 0, $tdRestante);

		$this->dado = $municipio;

		return $this;
	}
}

class Uf extends AbstractDadosDocumentoPar implements TrechoIdentificacaoDocumentoPar
{
	protected $dado;

	public function extrair(){
		$paramBuscaUF = utf8_decode('08 - UF</B><BR>');
		$posicaoInicial = strpos($this->ModeloDocumentoPar->doptexto, $paramBuscaUF) + strlen($paramBuscaUF);
		$param2BuscaUF = '</TD>';
		$quantidadeCaracteres = strpos( strstr($this->ModeloDocumentoPar->doptexto, $paramBuscaUF), $param2BuscaUF );
		$uf = substr($this->ModeloDocumentoPar->doptexto, $posicaoInicial, $quantidadeCaracteres);
		$uf = trim(strip_tags($uf));

		$this->dado = $uf;

		return $this;
	}
}

class NomePrefeito extends AbstractDadosDocumentoPar implements TrechoIdentificacaoDocumentoPar
{
	protected $dado;

	public function extrair(){
		$paramBuscaNomePrefeito = utf8_decode('09 - NOME</B><BR>');
		$posicaoInicial = strpos($this->ModeloDocumentoPar->doptexto, $paramBuscaNomePrefeito) + strlen($paramBuscaNomePrefeito);
		$param2BuscaNomePrefeito = '</TD>';
		$quantidadeCaracteres = strpos( strstr($this->ModeloDocumentoPar->doptexto, $paramBuscaNomePrefeito), $param2BuscaNomePrefeito );
		$nomePrefeito = substr($this->ModeloDocumentoPar->doptexto, $posicaoInicial, $quantidadeCaracteres);
		$nomePrefeito = trim(strip_tags($nomePrefeito));

		$this->dado = $nomePrefeito;

		return $this;
	}
}

class CpfPrefeito extends AbstractDadosDocumentoPar implements TrechoIdentificacaoDocumentoPar
{
	protected $dado;

	public function extrair(){
		$paramBuscaCPFPrefeito = utf8_decode('10 - CPF</B><BR>');
		$posicaoInicial = strpos($this->ModeloDocumentoPar->doptexto, $paramBuscaCPFPrefeito) + strlen($paramBuscaCPFPrefeito);
		$param2BuscaCPFPrefeito = '</TD>';
		$quantidadeCaracteres = strpos( strstr($this->ModeloDocumentoPar->doptexto, $paramBuscaCPFPrefeito), $param2BuscaCPFPrefeito );
		$cpfprefeito = substr($this->ModeloDocumentoPar->doptexto, $posicaoInicial, $quantidadeCaracteres);
		$cpfprefeito = trim(strip_tags($cpfprefeito));

		$this->dado = $cpfprefeito;

		return $this;
	}
}

/**
 * @see versao date de propriedade par.documentopar.dopdatainiciovigencia
 */
class MesInicial extends AbstractDadosDocumentoPar implements TrechoIdentificacaoDocumentoPar
{
	protected $dado;

	public function extrair(){
		$paramBuscaMesInicial = utf8_decode('MêS INICIAL:</B><BR>');
		$posicaoInicial = strpos($this->ModeloDocumentoPar->doptexto, $paramBuscaMesInicial) + strlen($paramBuscaMesInicial);
		$param2BuscaMesInicial = '</TD>';
		$quantidadeCaracteres = strpos( strstr($this->ModeloDocumentoPar->doptexto, $paramBuscaMesInicial), $param2BuscaMesInicial );
		$mesInicial = substr($this->ModeloDocumentoPar->doptexto, $posicaoInicial, $quantidadeCaracteres);
		$mesInicial = trim(strip_tags($mesInicial));

		$this->dado = $mesInicial;

		return $this;
	}
}

/**
 * @see versao date de propriedade par.documentopar.dopdatafimvigencia
 */
class MesFinal extends AbstractDadosDocumentoPar implements TrechoIdentificacaoDocumentoPar
{
	protected $dado;

	public function extrair(){
		$paramBuscaMesFinal = utf8_decode('MêS FINAL:</B><BR>');
		$posicaoInicial = strpos($this->ModeloDocumentoPar->doptexto, $paramBuscaMesFinal) + strlen($paramBuscaMesFinal);
		$param2BuscaMesFinal = '</TD>';
		$quantidadeCaracteres = strpos( strstr($this->ModeloDocumentoPar->doptexto, $paramBuscaMesFinal), $param2BuscaMesFinal );
		$mesFinal = substr($this->ModeloDocumentoPar->doptexto, $posicaoInicial, $quantidadeCaracteres);
		$mesFinal = trim(strip_tags($mesFinal));

		$this->dado = $mesFinal;

		return $this;
	}
}

class DataAssinatura extends AbstractDadosDocumentoPar implements TrechoIdentificacaoDocumentoPar
{
	protected $dado;

	public function extrair(){
		$texto_array = preg_split ('/$\R?^/m', $this->ModeloDocumentoPar->doptexto);
		for ($i=count($texto_array); $i >= 0 ; $i--) { 
			if( strpos( $texto_array[$i], "text-align: right;") ){
				$posicaoInicial = strpos( $texto_array[$i] , "text-align: right;" );
				$posicaoFinal = strpos($texto_array[$i],"</p>");
				$string_data_assinatura = substr( $texto_array[$i] , $posicaoInicial , $posicaoFinal );
				// deixa só a data
				$string_data_assinatura = explode(',', $string_data_assinatura);
				$string_data_assinatura = explode('de', $string_data_assinatura[1]);
				$data_assinatura['day'] = trim($string_data_assinatura[0]);
				$data_assinatura['month'] = trim($string_data_assinatura[1]);
				$data_assinatura['year'] = trim(str_replace('.', '', $string_data_assinatura[2]));
				$i = -1;
			}
		}
		$data_assinatura['month'] 	= str_replace('&nbsp;','',strip_tags($data_assinatura['month']));
		$data_assinatura['year']	= str_replace('&nbsp;','',strip_tags($data_assinatura['year']));
		$data_assinatura['day']		= str_replace('&nbsp;','',strip_tags($data_assinatura['day']));
		$dataFormatadaUS = $data_assinatura['year'].'-'.(mes_numero($data_assinatura['month'])).'-'.$data_assinatura['day'];

		$this->dado = $dataFormatadaUS;

		return $this;
	}
}


// ################################################################################################## ACOES FINCANCEIRAS


/**
 * @internal Ordem (1)
 */
class Subacao extends AbstractDadosDocumentoPar implements TrechoAcoesFinanceirasDocumentoPar
{
	public function extrair(){
		$posicaoInicial = strpos($this->textoComContinuidade, '<TD');
		$posicaoFinal =  strpos($this->textoComContinuidade, '</TD>');//+strlen('</TD>');
		$acaoFinanciadaSubacao = trim(strip_tags(substr($this->textoComContinuidade, $posicaoInicial,$posicaoFinal)));
		$this->textoComContinuidade = substr($this->textoComContinuidade, $posicaoFinal);

		$this->dado = $acaoFinanciadaSubacao;

		return $this;
	}
}

/**
 * @internal Ordem (2)
 */
class Tipo extends AbstractDadosDocumentoPar implements TrechoAcoesFinanceirasDocumentoPar
{
	public function extrair(){
		$posicaoInicial = strpos($this->textoComContinuidade, '<TD');
		$posicaoFinal =  strpos($this->textoComContinuidade, '</TD>')+strlen('</TD>');
		$acaoFinanciadaTipo = strip_tags(substr($this->textoComContinuidade, $posicaoInicial,$posicaoFinal));
		$this->textoComContinuidade = substr($this->textoComContinuidade, $posicaoFinal);

		$this->dado = $acaoFinanciadaTipo;

		return $this;
	}
}

/**
 * @internal Ordem (3)
 */
class TipoSubacao extends AbstractDadosDocumentoPar implements TrechoAcoesFinanceirasDocumentoPar
{
	public function extrair(){
		$posicaoInicial = strpos($this->textoComContinuidade, '<TD');
		$posicaoFinal =  strpos($this->textoComContinuidade, '</TD>')+strlen('</TD>');
		$acaoFinanciadaTipoSubacao = substr($this->textoComContinuidade, $posicaoInicial,$posicaoFinal);
		$posicaoFinal =  strpos($acaoFinanciadaTipoSubacao, '</TD>')+strlen('</TD>'); // segundo nivel de busca do fechamento td
		$acaoFinanciadaTipoSubacao = strip_tags(substr($this->textoComContinuidade, $posicaoInicial,$posicaoFinal));
		$this->textoComContinuidade = substr($this->textoComContinuidade, $posicaoFinal);
		$this->textoComContinuidade = substr($this->textoComContinuidade, strpos($this->textoComContinuidade, '</TD>')+strlen('</TD>'));

		$this->dado = $acaoFinanciadaTipoSubacao;

		return $this;
	}
}

/**
 * @internal Ordem (4)
 */
class Arp extends AbstractDadosDocumentoPar implements TrechoAcoesFinanceirasDocumentoPar
{
	public function extrair(){
		$posicaoInicial = strpos($this->textoComContinuidade, '<TD');
		$posicaoFinal =  strpos($this->textoComContinuidade, '</TD>')+strlen('</TD>');
		$acaoFinanciadaARP = strip_tags(substr($this->textoComContinuidade, $posicaoInicial,$posicaoFinal));
		$this->textoComContinuidade = substr($this->textoComContinuidade, $posicaoFinal);

		$this->dado = $acaoFinanciadaARP;

		return $this;
	}
}

/**
 * @internal Ordem (5)
 */
class MetasQtds extends AbstractDadosDocumentoPar implements TrechoAcoesFinanceirasDocumentoPar
{
	public function extrair(){
		$posicaoInicial = strpos($this->textoComContinuidade, '<TD');
		$posicaoFinal =  strpos($this->textoComContinuidade, '</TD>')+strlen('</TD>');
		$acaoFinanciadaMetasQtds = strip_tags(substr($this->textoComContinuidade, $posicaoInicial,$posicaoFinal));
		$this->textoComContinuidade = substr($this->textoComContinuidade, $posicaoFinal);

		$this->dado = $acaoFinanciadaMetasQtds;

		return $this;
	}
}

/**
 * @internal Ordem (6)
 */
class PrecoUnitario extends AbstractDadosDocumentoPar implements TrechoAcoesFinanceirasDocumentoPar
{
	public function extrair(){
		$posicaoInicial = strpos($this->textoComContinuidade, '<TD');
		$posicaoFinal =  strpos($this->textoComContinuidade, '</TD>')+strlen('</TD>');
		$acaoFinanciadaPrecoUnitario = strip_tags(substr($this->textoComContinuidade, $posicaoInicial,$posicaoFinal));
		$this->textoComContinuidade = substr($this->textoComContinuidade, $posicaoFinal);

		$this->dado = $acaoFinanciadaPrecoUnitario;

		return $this;
	}
}

/**
 * @internal Ordem (7)
 */
class Total extends AbstractDadosDocumentoPar implements TrechoAcoesFinanceirasDocumentoPar
{
	public function extrair(){
		$posicaoInicial = strpos($this->textoComContinuidade, '<TD');
		$posicaoFinal =  strpos($this->textoComContinuidade, '</TD>');//+strlen('</TD>');
		$acaoFinanciadaTotal = trim(strip_tags(substr($this->textoComContinuidade, $posicaoInicial,$posicaoFinal)));
		$this->textoComContinuidade = substr($this->textoComContinuidade, $posicaoFinal);

		$this->dado = $acaoFinanciadaTotal;

		return $this;
	}
}


// ################################################################################################## EMPENHOS

/**
 * @internal Exige que seja na sequencia dessas classes (1)
 */
class SubacaoEmpenho extends AbstractDadosDocumentoPar implements TrechoEmpenhosDocumentoPar
{
	public function extrair(){
		$posicaoInicial = strpos($this->textoComContinuidade, '<TD');
		$posicaoFinal =  strpos($this->textoComContinuidade, '</TD>')+strlen('</TD>');
		$empenhoSubacao = trim(strip_tags(substr($this->textoComContinuidade, $posicaoInicial, $posicaoFinal)));
		$this->textoComContinuidade = substr($this->textoComContinuidade, $posicaoFinal);

		$this->dado = $empenhoSubacao;

		return $this;
	}
}

/**
 * @internal Exige que seja na sequencia dessas classes (2)
 */
class Numero extends AbstractDadosDocumentoPar implements TrechoEmpenhosDocumentoPar
{
	public function extrair(){
		$posicaoInicial = strpos($this->textoComContinuidade, '<TD');
		$posicaoFinal =  strpos($this->textoComContinuidade, '</TD>')+strlen('</TD>');
		$empenhoNumero = trim(strip_tags(substr($this->textoComContinuidade, $posicaoInicial, $posicaoFinal)));
		$this->textoComContinuidade = substr($this->textoComContinuidade, $posicaoFinal);

		$this->dado = $empenhoNumero;

		return $this;
	}
}

/**
 * @internal Exige que seja na sequencia dessas classes (3)
 */
class Valor extends AbstractDadosDocumentoPar implements TrechoEmpenhosDocumentoPar
{
	public function extrair(){
		$posicaoInicial = strpos($this->textoComContinuidade, '<TD');
		$posicaoFinal =  strpos($this->textoComContinuidade, '</TD>')+strlen('</TD>');
		$empenhoValor = trim(strip_tags(substr($this->textoComContinuidade, $posicaoInicial,$posicaoFinal)));
		$this->textoComContinuidade = substr($this->textoComContinuidade, $posicaoFinal);

		$this->dado = $empenhoValor;

		return $this;
	}
}



function mes_numero($mes)
{
	if (strval($mes) == 'JANEIRO') return 1;
	else if (strval($mes) == 'FEVEREIRO') return 2;
	else if (strval($mes) == 'MARÇO') return 3;
	else if (strval($mes) == 'ABRIL') return 4;
	else if (strval($mes) == 'MAIO') return 5;
	else if (strval($mes) == 'JUNHO') return 6;
	else if (strval($mes) == 'JULHO') return 7;
	else if (strval($mes) == 'AGOSTO') return 8;
	else if (strval($mes) == 'SETEMBRO') return 9;
	else if (strval($mes) == 'OUTUBRO') return 10;
	else if (strval($mes) == 'NOVEMBRO') return 11;
	else if (strval($mes) == 'DEZEMBRO') return 12;
}