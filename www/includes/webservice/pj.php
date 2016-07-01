<?php
if (!$_POST['ajaxPJ']):
?>
	<script type="text/javascript" src="/includes/prototype.js"></script>
	<script src="/includes/webservice/pj.js"></script>
<?php
endif;
include_once 'config.inc';

/**
 * Classe para acesso ao webservice de pessoa jurídica.
 *
 * PS: Não esqueça de ler o leiame.txt
 *
 */
final class PessoaJuridicaClient
{
	/**
	 * Coloca o objeto do cliente do webservice.
	 *
	 * @var SoapClient
	 */
	private $soapClient;


	/**
	 * Construtor da classe.
	 *
	 * @param string $wsdl
	 */
	public function __construct($wsdl)
	{

		try{
			$this->soapClient = new Simec_SoapClient( $wsdl );
		} catch (Exception $e){
			exit("Não está conectado!");
		}

	}

	/**
	 * Retorna dados de pessoa jurídica pelo CNPJ.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosResumidoPessoaJuridicaPorCnpj( $cnpj )
	{
		return (  $this->soapClient->solicitarDadosResumidoPessoaJuridicaPorCnpj( $cnpj ) );
	}

	/**
	 * Retorna dados completo de pessoa jurídica por CNPJ.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosPessoaJuridicaPorCnpj( $cnpj )
	{
		return (  $this->soapClient->solicitarDadosPessoaJuridicaPorCnpj( $cnpj ) );
	}

	/**
	 * Retorna dados de Endereço da pessoa jurídica por CNPJ.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosEnderecoPessoaJuridicaPorCnpj( $cnpj )
	{
		return (  $this->soapClient->solicitarDadosEnderecoPessoaJuridicaPorCnpj( $cnpj ) );
	}

	/**
	 * Retorna dados de Contato da pessoa jurídica por CNPJ.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosContatoPessoaJuridicaPorCnpj( $cnpj )
	{
		return (  $this->soapClient->solicitarDadosContatoPessoaJuridicaPorCnpj( $cnpj ) );
	}

	/**
	 * Retorna as informações do sócio da pessoa jurídica.
	 *
	 * @param string $cnpj
	 * @return string
	 */
	public function solicitarDadosSocioPessoaJuridicaPorCnpj( $cnpj )
	{
		return (  $this->soapClient->solicitarDadosSocioPessoaJuridicaPorCnpj( $cnpj ) );
	}

}

if ($_POST['ajaxPJ']):

	$pj = str_replace(array('/', '.', '-'), '', $_POST['ajaxPJ']);

	/**
	 * Aqui é feita a chamada do método da classe cliente do webservice.
	 */
	$objPessoaJuridica = new PessoaJuridicaClient("http://ws.mec.gov.br/PessoaJuridica/wsdl");
	$xml = $objPessoaJuridica->solicitarDadosPessoaJuridicaPorCnpj($pj);

	// Substituindo o caracter especial '&' para seu respectivo código, pois o caracter sozinho no meio do xls causa um erro de string.
	$xml = str_replace(array("& "),array("&amp; "),$xml);

	$obj = (array) simplexml_load_string($xml);
	$xml = simplexml_load_string($xml);

	if (!$obj['PESSOA']) {
		die();
	}

	$empresa  = (array) $obj['PESSOA'];
	$endereco = (array) $obj['PESSOA']->ENDERECOS->ENDERECO;
	$contato  = (array) $obj['PESSOA']->CONTATOS->CONTATO;

	foreach($empresa as $k =>$val):
		if (ctype_upper($k)){continue;}
		$return[] = "$k#{$val}";
	endforeach;

	foreach($endereco as $k =>$val):
		if (ctype_upper($k)){continue;}
		$return[] = "$k#{$val}";
	endforeach;

	foreach($contato as $k =>$val):
		if (ctype_upper($k)){continue;}
		$return[] = "$k#{$val}";
	endforeach;

	for ($i=0; $i < count($xml->PESSOA->SOCIOS->SOCIO); $i++ ):
		foreach ($xml->PESSOA->SOCIOS->SOCIO[$i] as $k=>$val){
			$socio[] = "$k#{$val}";
		}
	endfor;

	if(is_array($return) && is_array($socio) ){
		die(implode('|', $return)."$$".implode('|', $socio));
	}
	die();

endif;