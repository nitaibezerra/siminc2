<?php
// carrega as funções gerais
define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );


error_reporting( E_ALL ^ E_NOTICE );

ini_set("memory_limit", "1024M");
set_time_limit(0);


$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";

function processararquivos($files,$orig,$dest) {
	if ($files[0]) {
		if(!is_dir(APPRAIZ."arquivos/".$dest."/files_tmp")) {
			mkdir(APPRAIZ."arquivos/".$dest."/files_tmp");
		}

		foreach($files as $f) {
			$endorigem  = APPRAIZ."arquivos/".$orig."/".floor($f['arqid']/1000)."/".$f['arqid'];
			$enddestino = APPRAIZ."arquivos/".$dest."/files_tmp/".$f['arqid'].".".$f['arqextensao'];

			if(file_exists($endorigem)) {
				if(copy($endorigem, $enddestino)) {

					//Clean-up memory
					ImageDestroy($thumb);
					ImageDestroy($source);

					if(file_exists($enddestino)) $fzip[] = $enddestino;
				}
			}
		}
	}

	return $fzip;
}

// CPF do administrador de sistemas
	$_SESSION['usucpforigem'] = '00000000191';
	$_SESSION['usucpf'] = '00000000191';


// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

$x  = file("arquivosobras.csv");

include(APPRAIZ . 'includes/pclzip-2-6/pclzip.lib.php');
$files = $db->carregar("SELECT arqid, arqextensao FROM public.arquivo WHERE arqid IN('".implode("','",$x)."') AND sisid=15");

$filezip = processararquivos($files,'obras','obras');
$nomearquivozip = 'MEC_'.date('dmyhis').'.zip';
$enderecozip = APPRAIZ.'arquivos/obras/files_tmp/'.$nomearquivozip;
$archive = new PclZip($enderecozip);
$archive->create( $filezip,  PCLZIP_OPT_REMOVE_ALL_PATH);
if($filezip) deletararquivos($filezip);

ob_clean();

header("Content-Disposition: attachment; filename=".$nomearquivozip);
header("Content-Type: application/oct-stream");
header("Expires: 0");
header("Pragma: public");
header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
readfile($enderecozip);




?>