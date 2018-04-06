<?
/*
 * Variaveis a serem setadas na SESSÃO DO SCRIPT
 * 
 *  SETAR : $_SESSION['downloadfileszip']['tipobuscararquivos'] : Define o tipo de busca que será realizado, o usuário pode implementar algum tipo caso não tenha.
 *  $$ OPÇÕES CADASTRADAS $$
 *  'buscarporsql' => Significa que a busca será pela tabela de public.arquivo (procedimento padrão de gravação no SIMEC). É necessario os campos 'arqid' e 'arqextensao'
 *  $$$ PARAMÊTROS RECEBIDOS $$$
 * 	'crtnum' => Indica qual SQL deverá ser executado, caso não exista, será executado todos os SQL cadastrados no array ($_SESSION['downloadfileszip']['bd']).
 *  SETAR : $_SESSION['downloadfileszip']['pasta'] : É um array contendo as pasta de origem e destino do arquivos tratados. Devera conter o seguinte array : array("origem"=>"yyyyyy","destino"=>"xxxxxx")
 * 
 *   
 */

session_start();

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

# Verifica se a sessão não expirou, se tiver expirada envia pra tela de login.
controlarAcessoSemAutenticacao();

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

function deletararquivos($files) {
	foreach($files as $fl) {
		unlink($fl);
	}
}

function processararquivos($files,$orig,$dest) {
	if($files[0]) {
		foreach($files as $f) {
			$endorigem =  "../../arquivos/".$orig."/".floor($f['arqid']/1000)."/".$f['arqid'];
			$enddestino = "../../arquivos/".$dest."/files_tmp/".$f['arqid'].".".$f['arqextensao'];
			if(file_exists($endorigem)) {
				if(copy($endorigem, $enddestino)) {
					$fzip[] = $enddestino; 	
				}
			}
		}
	}
	return $fzip;
}

include('../../includes/pclzip-2-6/pclzip.lib.php');
switch($_SESSION['downloadfileszip']['tipobuscararquivos']) {
	case 'buscarporlistaid':
		if(count($_REQUEST['fotosselecionadas']) > 0) {
			$files = $db->carregar("SELECT arqid, arqextensao FROM public.arquivo WHERE arqid IN('".implode("','",$_REQUEST['fotosselecionadas'])."')");
			$filezip = processararquivos($files,$_SESSION['downloadfileszip']['pasta']['origem'],$_SESSION['downloadfileszip']['pasta']['destino']);
			$nomearquivozip = $_SESSION['usucpf'].'_'.date('dmyhis').'.zip';
			$enderecozip = '../../arquivos/'.$_SESSION['downloadfileszip']['pasta']['destino'].'/files_tmp/'.$nomearquivozip;
			$archive = new PclZip($enderecozip);
			$archive->create( $filezip,  PCLZIP_OPT_REMOVE_ALL_PATH);
			deletararquivos($filezip);
		} else {
			echo "<script>alert('Não foi selecionado nenhuma foto.');window.close();</script>";
			exit;
		}
		header("Content-Disposition: attachment; filename=\"$nomearquivozip\"");
		header("Content-Type: application/oct-stream");
		header("Expires: 0");
		header("Pragma: public");
		header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
		readfile($enderecozip);
		break;
}
?>