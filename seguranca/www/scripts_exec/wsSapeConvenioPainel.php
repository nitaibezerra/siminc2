<?php
// carrega as funções gerais
define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );


// carrega as funções gerais
require_once BASE_PATH_SIMEC . '/global/config.inc';
//include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

/* configurações do relatorio - Memoria limite de 3000 Mbytes */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 3000 Mbytes */

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

$db = new cls_banco();
$convenioFNDE = $_SESSION['painel']['conveniofnde'] ? $_SESSION['painel']['conveniofnde'] : array();
unset($_SESSION['painel']['conveniofnde']);

if( empty($convenioFNDE) ){
	$sql = "SELECT dcoprocesso, dcocnpj, dcoconvenio, Replace(dcoconvenio,'/'||dcoano,'') as numeroconv, dcoano
			FROM painel.dadosconvenios
			WHERE dcoprocesso NOT IN ('23400014032200689', '23400004771200743', '23400005450200685', '23400014890200119')
			--AND dcodataatualizacao < (DATE(now()) - 7)
			ORDER BY dcodataatualizacao
			LIMIT 2000";
} else {
	$sql = "SELECT dcoprocesso, dcocnpj, '' as dcoconvenio FROM painel.dadosconvenios WHERE dcoprocesso IN ('".implode("','",$convenioFNDE)."')";
}
//$sql = "SELECT dcoprocesso, dcocnpj, dcoconvenio, Replace(dcoconvenio,'/'||dcoano,'') as numeroconv, dcoano FROM painel.dadosconvenios WHERE dcoprocesso in ('23034020102200149')";

$arDadosConvenio = $db->carregar( $sql );
$arDadosConvenio = $arDadosConvenio ? $arDadosConvenio : array();

$dataAtual = date("c");

$arrMes = array( 'JAN' => '01', 
				 'FEB' => '02',
				 'MAR' => '03',
				 'APR' => '04',
				 'MAY' => '05',
				 'JUN' => '06',
				 'JUL' => '07',
				 'AUG' => '08',
				 'SEP' => '09',
				 'OCT' => '10',
				 'NOV' => '11',
				 'DEC' => '12',
				);

foreach ($arDadosConvenio as $v) {
	$processo 		= $v['dcoprocesso'];
	$dcocnpjNE 		= $v['dcocnpj'];
	$an_convenio 	= ($v['dcoano']=='' ? 0 : $v['dcoano']);
	$nu_convenio 	= ($v['numeroconv']=='' ? 0 : $v['numeroconv']);
	$dcoconvenio	= $v['dcoconvenio'];

$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$dataAtual}</created>
	</header>
	<body>
		<params>
			<processo>$processo</processo>
	      	<an_convenio>$an_convenio</an_convenio>
	      	<nu_convenio>$nu_convenio</nu_convenio>
		</params>
	</body>
</request>
XML;

	$urlWS = "http://www.fnde.gov.br/webservices/wssape/index.php/convenio/detalhar";
	try {
    	$xml = Fnde_Webservice_Client::CreateRequest()
			->setURL($urlWS)
			->setParams( array('xml' => $arqXml) )
			->execute();
		
		$xml = simplexml_load_string( stripslashes($xml));
		//ver($xml,d);
        if ( (int) $xml->status->result ){
        	
        	$obConvenio = $xml->body->convenio->children();
			
        	$arrConvenio = (array) $xml->body->convenio->itens;
			$dcototalconcedente = 0;
			$dcototalcontrapartida = 0;
			$dcodatainicio = '3000-01-01';
			$dcodatafim = '1900-01-01';
			if(is_array($arrConvenio['item'])){
				foreach ($arrConvenio['item'] as $convenio) {
					$dcovalorconcedente		= (float)$convenio->programas->programa->detalhamento->vl_concedente;
					$dcovalorcontrapartida	= (float)$convenio->programas->programa->detalhamento->vl_contrapartida;
					$dcototalconcedente		= $dcototalconcedente + $dcovalorconcedente;
					$dcototalcontrapartida	= $dcototalcontrapartida + $dcovalorcontrapartida;
					$dt_inicio_vigencia		= (string)$convenio->programas->programa->detalhamento->dt_inicio_vigencia;
					$dt_fim_vigencia		= (string)$convenio->programas->programa->detalhamento->dt_fim_vigencia;
					if($dcodatainicio>$dt_inicio_vigencia){
						$dcodatainicio = $dt_inicio_vigencia;
					}
					if($dcodatafim<$dt_fim_vigencia){
						$dcodatafim = $dt_fim_vigencia;
					}
				}
			}else{
				$dcototalconcedente		= (string)$obConvenio->itens->item->programas->programa->detalhamento->vl_concedente;
				$dcototalcontrapartida	= (string)$obConvenio->itens->item->programas->programa->detalhamento->vl_contrapartida;
				$dcodatainicio			= (string)$obConvenio->itens->item->programas->programa->detalhamento->dt_inicio_vigencia;
				$dcodatafim				= (string)$obConvenio->itens->item->programas->programa->detalhamento->dt_fim_vigencia;
			}
            //ver($dcodatafim,d);
			
        	$dcocnpj 				= (string)$obConvenio->itens->item->programas->programa->detalhamento->entidade->nu_cgc_entidade;
        	$dcorazaosocial 		= (string)$obConvenio->itens->item->programas->programa->detalhamento->entidade->no_razao_social;
        	$dcoprocesso			= (string)$obConvenio->nu_processo;
        	$dcoconvenio			= (string)$obConvenio->nu_convenio;
        	$an_convenio			= (string)$obConvenio->an_convenio;
        	$dcoconveniosiafi		= (string)$obConvenio->nu_conv_siafi;
        	$muncod					= (string)$obConvenio->itens->item->programas->programa->detalhamento->entidade->co_municipio_ibge;
        	$dcoprograma			= (string)$obConvenio->itens->item->programas->programa->ds_programa_fnde;
			$esfera					= (string)$obConvenio->itens->item->programas->programa->detalhamento->entidade->no_esfera_adm;
			switch ($esfera){
				case 'FEDERAL':
				$desid = 0;
				break;
				case 'ESTADUAL':
				$desid = 1;
				break;
				case 'MUNICIPAL':
				$desid = 2;
				break;
				case 'PARTICULAR':
				$desid = 3;
				break;
			}
			
			$dcovalorconcedente		= (string)$dcototalconcedente;
			$dcovalorcontrapartida	= (string)$dcototalcontrapartida;
			$dcovalorconveniado 	= (float)$dcototalconcedente+(float)$dcototalcontrapartida;
        	
        	$dcoprograma 	= utf8_decode($dcoprograma);
        	$dcorazaosocial = utf8_decode($dcorazaosocial);
        	$dcorazaosocial = str_replace("'","`",$dcorazaosocial);

        	$dcovalorconveniado 	= $dcovalorconveniado 		? "'".$dcovalorconveniado."'" 		: 'null';
        	$dcovalorempenhado 		= $dcovalorempenhado 		? "'".$dcovalorempenhado."'" 		: 'null';
        	$dcovalorpago 			= $dcovalorpago 			? "'".$dcovalorpago."'" 			: 'null';
        	$dcodatapagamento 		= $dcodatapagamento 		? "'".$dcodatapagamento."'" 		: 'null';
        	$dcoconveniosiafi 		= $dcoconveniosiafi 		? "'".$dcoconveniosiafi."'" 		: 'null';
        	$dcovalorconcedente 	= $dcovalorconcedente 		? "'".$dcovalorconcedente."'" 		: 'null';
        	$dcovalorcontrapartida 	= $dcovalorcontrapartida 	? "'".$dcovalorcontrapartida."'" 	: 'null';
        	$dcovalorprojeto 		= $dcovalorprojeto 			? "'".$dcovalorprojeto."'" 			: 'null';
        	$dcodatainicio 			= $dcodatainicio 			? "'".$dcodatainicio."'" 			: 'null';
        	$dcodatafim 			= $dcodatafim 				? "'".$dcodatafim."'" 				: 'null';
        	
        	$sql = "UPDATE painel.dadosconvenios SET
						  dcorazaosocial = '$dcorazaosocial',
						  dcoconvenio = '".$dcoconvenio.'/'.$an_convenio."',
						  muncod = '$muncod',
						  dcovalorconveniado = $dcovalorconveniado,
						  dcocnpj = '$dcocnpj',
						  dcoconveniosiafi = $dcoconveniosiafi,
						  dcovalorconcedente = $dcovalorconcedente,
						  dcovalorcontrapartida = $dcovalorcontrapartida,
						  dcodatainicio = $dcodatainicio,
						  dcodatafim = $dcodatafim,
						  dcoano = '$an_convenio',
						  dcoprograma = '$dcoprograma',
						  dcodataatualizacao = 'now()',
						  desid = $desid
					 WHERE dcoprocesso = '$processo'";
        	$db->executar( $sql );
        	$db->commit();
		}
	} catch (Exception $e){}

# WS Empenho
$usuarioNE = 'SIMEC_CONVENIO';
$senhaNE = '52474652'; 

$arqXmlNE = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$dataAtual}</created>
	</header>
	<body>
		<auth>
			<usuario>$usuarioNE</usuario>
			<senha>$senhaNE</senha>
		</auth>
		<params>
			<co_cnpj>$dcocnpjNE</co_cnpj>
			<nu_processo>$processo</nu_processo>
			<co_programa_fnde></co_programa_fnde>
			<efetivados>S</efetivados>
			<rownum>0</rownum>
			<numero_de_linhas>20</numero_de_linhas>
		</params>
	</body>
</request>
XML;

	#Produção
	$urlWSNE = "http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne";
	
	try {
    	$xmlNE = Fnde_Webservice_Client::CreateRequest()
			->setURL($urlWSNE)
			->setParams( array('xml' => $arqXmlNE, 'method' => 'consultarAndamentoNE') )
			->execute();	
		
		$xmlNE = simplexml_load_string( stripslashes($xmlNE));		
		//ver($xmlNE,d);
        if ( (int) $xmlNE->status->result ){
        	
        	if( $xmlNE->status->message->code == '1' ){
	        	$obAndamentoNE = $xmlNE->body->row->children();
				
				$arrAndamentoNE = (array) $xmlNE->body;
				$dcototalempenhado = 0;
				foreach ($arrAndamentoNE['row'] as $andamento) {
					if(is_array($arrAndamentoNE['row'])){
						$dcovalorempenhado 	= (float)$andamento->VALOR_TOTAL_EMPENHADO;
						$dcototalempenhado	= $dcototalempenhado + $dcovalorempenhado;
					}else{
						$dcototalempenhado = (string)$obAndamentoNE->VALOR_TOTAL_EMPENHADO;
					}
				}
				$dcovalorempenhado = (string)$dcototalempenhado;
	        	$dcovalorempenhado = $dcovalorempenhado ? $dcovalorempenhado : 'null';
	        	
	        	$sql = "UPDATE painel.dadosconvenios SET
							dcovalorempenhado = $dcovalorempenhado,
							dcodataatualizacao = 'now()'
						WHERE dcoprocesso = '$processo'";
	        	
	        	$db->executar( $sql );
	        	$db->commit();
        	}
		} 
	} catch (Exception $e){}

# WS Pagamento
$arqXmlPG = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$dataAtual}</created>
	</header>
	<body>
		<auth>
			<usuario>$usuarioNE</usuario>
			<senha>$senhaNE</senha>
		</auth>
		<params>
			<nu_processo>$processo</nu_processo>
			<an_exercicio></an_exercicio>
		</params>
	</body>
</request>
XML;

	#Produção
	$urlWSPG = "http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/extrato";
	try {
    	$xmlPG = Fnde_Webservice_Client::CreateRequest()
			->setURL($urlWSPG)
			->setParams( array('xml' => $arqXmlPG, 'method' => 'consultar') )
			->execute();		
		
		$xmlPG = simplexml_load_string( stripslashes($xmlPG));
		//ver($xmlPG,d);
        if ( (int) $xmlPG->status->result ){
        	
        	if( $xmlPG->status->message->code == '1' && !($xmlPG->body->row->status == '0 - Registros deste processo nÃ£o encontrados.')){

				$obPagamento = $xmlPG->body->row->children();
				
	        	$arrPagamento = (array) $xmlPG->body;
	        	
	        	$sql = "DELETE FROM painel.dadosrepassesconvenios WHERE drcprocesso = '$processo'";
	        	$db->executar( $sql );
				
				$sql = "DELETE FROM painel.dadosprograma WHERE dprprocesso = '$processo'";
	        	$db->executar( $sql );
	        	
				foreach ($arrPagamento['row'] as $pagamento) {
					if(is_array($arrPagamento['row'])){
						$drcconveniosiafi 	= (string)$pagamento->nu_convenio_siafi;
						$drcbanco 			= (string)$pagamento->nu_banco;
						$drcagencia 		= (string)$pagamento->nu_agencia;
						$drcconta 			= (string)$pagamento->nu_conta_corrente_favorecido;
						$drcvalorpago 		= (string)$pagamento->vl_pago;
						$drcdatapagamento 	= (string)$pagamento->dt_emissao;
						$an_exercicio 		= (string)$pagamento->an_exercicio;
						$co_tipo_documento 	= (string)$pagamento->co_tipo_documento;
						$nu_documento_siafi = (string)$pagamento->nu_documento_siafi;
						$drcprograma 		= (string)$pagamento->ds_programa_fnde;
						$dprcodprograma 	= (string)$pagamento->co_programa_fnde;
						$numValidacao = 0;
					}else{
						$drcconveniosiafi 	= (string)$obPagamento->nu_convenio_siafi;
						$drcbanco 			= (string)$obPagamento->nu_banco;
						$drcagencia 		= (string)$obPagamento->nu_agencia;
						$drcconta 			= (string)$obPagamento->nu_conta_corrente_favorecido;
						$drcvalorpago 		= (string)$obPagamento->vl_pago;
						$drcdatapagamento 	= (string)$obPagamento->dt_emissao;
						$an_exercicio 		= (string)$obPagamento->an_exercicio;
						$co_tipo_documento 	= (string)$obPagamento->co_tipo_documento;
						$nu_documento_siafi = (string)$obPagamento->nu_documento_siafi;
						$drcprograma 		= (string)$obPagamento->ds_programa_fnde;
						$dprcodprograma 	= (string)$obPagamento->co_programa_fnde;
						$numValidacao = 1;
					}
					
					$datapag = explode('-',  $drcdatapagamento );
					$drcdatapagamento = $datapag[0].'-'.$arrMes[$datapag[1]].'-'.$datapag[2];
					$drcordembancaria = $an_exercicio.$co_tipo_documento.$nu_documento_siafi;		        	
					$drcprograma = utf8_decode($drcprograma);
					
					$drcconveniosiafi = $drcconveniosiafi ? "'".$drcconveniosiafi."'" : 'null';
					$drcbanco = $drcbanco ? "'".$drcbanco."'" : 'null';
					$drcagencia = $drcagencia ? "'".$drcagencia."'" : 'null';
					$drcconta = $drcconta ? "'".$drcconta."'" : 'null';
					$drcvalorpago = $drcvalorpago ? "'".$drcvalorpago."'" : 'null';
					$drcdatapagamento = $drcdatapagamento ? "'".$drcdatapagamento."'" : 'null';
					$drcordembancaria = $drcordembancaria ? "'".$drcordembancaria."'" : 'null';
					$drcprograma = $drcprograma ? "'".$drcprograma."'" : 'null';
					$dprcodprograma = $dprcodprograma ? "'".$dprcodprograma."'" : 'null';
					
					$sql = "INSERT INTO painel.dadosrepassesconvenios(drcconveniosiafi, drcprocesso, drcprograma, drcbanco,
								drcagencia, drcconta, drcvalorpago, drcordembancaria, drcdatapagamento)	  							
							VALUES ($drcconveniosiafi, '$processo', $drcprograma, $drcbanco,
								$drcagencia, $drcconta, $drcvalorpago, $drcordembancaria, $drcdatapagamento)";
					$db->executar( $sql );
										
					$dprid = $db->pegaUm( "SELECT COUNT(dprid) FROM painel.dadosprograma WHERE dprcodprograma = ".$dprcodprograma." AND dprprocesso = '".$processo."'" );
        			if( (int) $dprid == 0 ){
						$sql = "INSERT INTO painel.dadosprograma(dprprograma, dprcodprograma, dprconveniosiafi, dprprocesso)
							VALUES ($drcprograma, $dprcodprograma, $drcconveniosiafi, '$processo')";
						$db->executar( $sql );
					}
					
					if ($numValidacao==1){break;}
				}
	        	
	        	$sql = "SELECT sum(drcvalorpago) FROM painel.dadosrepassesconvenios WHERE drcprocesso = '$processo'";
	        	$dcovalorpago = $db->pegaUm( $sql );
	        	
	        	$sql = "SELECT max(drcdatapagamento) FROM painel.dadosrepassesconvenios WHERE drcprocesso = '$processo'";
	        	$dcodatapagamento = $db->pegaUm( $sql );
	        	
	        	$dcovalorpago = $dcovalorpago ? "'".$dcovalorpago."'" : 'null';
	        	$dcodatapagamento = $dcodatapagamento ? "'".$dcodatapagamento."'" : 'null';
	        	
	        	$sql = "UPDATE painel.dadosconvenios SET
							dcovalorpago = $dcovalorpago,
							dcodatapagamento = $dcodatapagamento,
							dcodataatualizacao = 'now()'
						WHERE dcoprocesso = '$processo'";
	        	
	        	$db->executar( $sql );
	        	$db->commit();
        	}
		}
	} catch (Exception $e){
		if($e->getCode() == 404){
			echo "<script>
					alert('Erro-Serviço Empenho encontra-se temporariamente indisponível.\nFavor tente mais tarde.');
				 </script>";
			die;
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );
			
		echo "<script>
				alert('Erro-WS Solicitar Empenho no SIGEF: $erroMSG');
			 </script>";
		die;
	}
} //fim do for

/*
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "WS Atualizar Convenios";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA);
$mensagem->Subject = "WS Atualizar Convenios";

$mensagem->Body = $corpoemail;
$mensagem->IsHTML( true );
$mensagem->Send();
/*
 * FIM
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
?>